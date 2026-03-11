#include "Server.h"

Server::Server(asio::ip::port_type port, std::shared_ptr<Log> log)
	: ioctx(),
	acceptor(ioctx),
	port(port),
	threads(),
	log(log)
{
}

Server::~Server()
{
}

asio::awaitable<void> Server::Listener()
{
	return asio::awaitable<void>();
}
