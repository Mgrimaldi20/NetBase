#include <system_error>

#include "Session.h"

#include "Server.h"

constexpr int NET_SIGINT = SIGINT;
constexpr int NET_SIGTERM = SIGTERM;

Server::Server(
	asio::ip::port_type port,
	asio::io_context &ioctx,
	std::shared_ptr<Log> log,
	std::shared_ptr<CmdDispatcher> dispatcher
)
	: signals(ioctx, NET_SIGINT, NET_SIGTERM),
	ioctx(ioctx),
	port(port),
	log(log),
	dispatcher(dispatcher)
{
	RegisterSignals();

	// start the server and listen for incoming connections on the specified port number
	asio::co_spawn(ioctx, Listener(), asio::detached);

	log->Info("Server started");
	log->Info("NetBase server running on port: {}", port);
	log->Info("Press Ctrl-C to exit...");
}

Server::~Server()
{
	log->Info("Shutting down the Server");
}

asio::awaitable<void> Server::Listener()
{
	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
	asio::ip::tcp::acceptor acceptor(ioctx, endpoint);

	while (true)
	{
		std::make_shared<Session>(
			co_await std::move(acceptor.async_accept(asio::use_awaitable)),
			dispatcher,
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
			
		if (ec)
		{
			log->Error("Signal handler error: {}", ec.message());
			return;
		}

		log->Info("Signal received: {} : {}", signal, GetSignalStr(signal));
		ioctx.stop();
	});
}
