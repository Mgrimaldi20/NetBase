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

	log->Info("Number of worker threads available: {}", numthreads);
	log->Info("NetBase server running on port: {}", port);
	log->Info("Press Ctrl-C to exit...");
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
	asio::ip::tcp::acceptor acceptor(ioctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

	while (true)
	{
		std::make_shared<Session>(
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
		if (!ec)
		{
			log->Info("Signal received: {}", signal);
			ioctx.stop();
		}

		else
			log->Error("Signal handler error: {}", ec.message());
	});
}
