#include <thread>
#include <vector>

#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/signal_set.hpp"
#include "asio/use_awaitable.hpp"

#include "Session.h"

#include "Server.h"

constexpr int NET_SIGINT = SIGINT;
constexpr int NET_SIGTERM = SIGTERM;

#if defined(SIGBREAK)
constexpr int NET_RESTART_SIG = SIGBREAK;
#elif defined(SIGTSTP)
constexpr int NET_RESTART_SIG = SIGTSTP;
#endif

Server::Server(asio::ip::port_type port, unsigned int numthreads, std::shared_ptr<Log> log)
	: ioctx(),
	restartserver(false),
	port(port),
	numthreads(numthreads),
	log(log)
{
	// start the server and listen for incoming connections on the specified port number
	asio::co_spawn(ioctx, Listener(), asio::detached);

	// register signal handler for graceful shutdown or restart
	asio::signal_set signals(ioctx, NET_SIGINT, NET_SIGTERM, NET_RESTART_SIG);
	signals.async_wait([&](const std::error_code &ec, int signal)
	{
		if (!ec)
		{
			log->Info("Signal received: {}", signal);

			if (signal == NET_RESTART_SIG)
			{
				log->Info("NetBase server is restarting...\n");
				restartserver = true;
			}

			ioctx.stop();
		}

		else
			log->Error("Signal handler error: {}", ec.message());
	});
}

Server::~Server()
{
	log->Info("Shutting down the server");
}

void Server::Run()
{
	do
	{
		restartserver = false;

		// run the server with a thread pool and handle requests until stopped
		std::vector<std::jthread> threads;
		for (unsigned int i=0; i<numthreads; i++)
		{
			threads.emplace_back([this]()
			{
				ioctx.run();
			});
		}
	} while (restartserver);
}

asio::awaitable<void> Server::Listener()
{
	asio::ip::tcp::acceptor acceptor(ioctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

	while (true)
	{
		asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
		std::make_shared<Session>(socket, log);
		//asio::co_spawn(ioctx, /*Session awaitable*/, log), asio::detached);
	}
}
