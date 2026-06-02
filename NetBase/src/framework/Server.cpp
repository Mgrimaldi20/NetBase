#include <system_error>

#include "Session.h"

#include "Server.h"

constexpr int NET_SIGINT = SIGINT;
constexpr int NET_SIGTERM = SIGTERM;

Server::Server(
	asio::io_context &ioctx,
	asio::ip::port_type port,
	std::shared_ptr<Log> log,
	std::shared_ptr<CmdDispatcher> dispatcher,
	ClientAPI::Parser &parser
)
	: ioctx(ioctx),
	log(log),
	signals(ioctx, NET_SIGINT, NET_SIGTERM)
{
	RegisterSignals();

	// start the server and listen for incoming connections on the specified port number
	asio::co_spawn(ioctx, Listener(port, dispatcher, parser), asio::detached);

	log->Info("Server started");
	log->Info("NetBase server running on port: {}", port);
	log->Info("Press Ctrl-C to exit...");
}

Server::~Server()
{
	log->Info("Shutting down the Server");
}

asio::awaitable<void> Server::Listener(
	asio::ip::port_type port,
	std::shared_ptr<CmdDispatcher> dispatcher,
	ClientAPI::Parser &parser
)
{
	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
	asio::ip::tcp::acceptor acceptor(ioctx.get(), endpoint);

	try
	{
		while (true)
		{
			asio::error_code ec;
			auto socket = co_await acceptor.async_accept(asio::redirect_error(asio::use_awaitable, ec));

			if (ec)
			{
				if (ec == asio::error::operation_aborted)
				{
					log->Info("Operation aborted: Listener stopped");
					break;
				}

				throw std::system_error(ec);
			}

			std::make_shared<Session>(
				std::move(socket),
				dispatcher,
				parser,
				log
			)->Start();
		}
	}

	catch (const std::exception &e)
	{
		log->Info("Listener exiting: {}", e.what());
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
		ioctx.get().stop();
	});
}
