#include <system_error>
#include <thread>
#include <vector>

#include "Session.h"

#include "Server.h"

constexpr int NET_SIGINT = SIGINT;
constexpr int NET_SIGTERM = SIGTERM;

Server::Server(asio::ip::port_type port, unsigned int numthreads, std::shared_ptr<Log> log)
	: ioctx(),
	signals(ioctx, NET_SIGINT, NET_SIGTERM),
	port(port),
	numthreads(numthreads),
	log(log)
{
	RegisterSignals();

	// start the server and listen for incoming connections on the specified port number
	asio::co_spawn(ioctx, Listener(), asio::detached);

	log->Info(
		"Number of worker threads available: {}\n"
		"NetBase server running on port: {}\n"
		"Press Ctrl-C to exit...",
		numthreads,
		port
	);
}

Server::~Server()
{
	log->Info("Shutting down the Server");
}

void Server::Run()
{
	// run the server with a thread pool and handle requests until stopped
	std::vector<std::jthread> threads;
	for (unsigned int i=0; i<numthreads; i++)
	{
		threads.emplace_back([this]()
		{
			ioctx.run();
		});
	}
}

asio::awaitable<void> Server::Listener()
{
	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
	asio::ip::tcp::acceptor acceptor(ioctx, endpoint);

	while (true)
	{
		Session::Create(
			co_await acceptor.async_accept(asio::use_awaitable),
			log
		)->Start();
	}
}

void Server::RegisterSignals()
{
	// register signal handler for graceful shutdown or restart
	signals.async_wait([this](const std::error_code &ec, int signal)
	{
		constexpr auto GetSignalStr = [](int signal)
		{
			switch (signal)
			{
				case NET_SIGINT: return "NET_SIGINT";
				case NET_SIGTERM: return "NET_SIGTERM";
				default: return "UNKNOWN";
			}
		};

		if (!ec)
		{
			log->Info("Signal received: {} : {}", signal, GetSignalStr(signal));
			ioctx.stop();
		}

		else
			log->Error("Signal handler error: {}", ec.message());
	});
}
