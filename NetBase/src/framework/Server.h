#ifndef _NETBASE_FRAMEWORK_SERVER_H_
#define _NETBASE_FRAMEWORK_SERVER_H_

#include <memory>
#include <thread>
#include <vector>

#include "asio/io_context.hpp"
#include "asio/awaitable.hpp"

#include "asio/ip/tcp.hpp"

#include "Log.h"

class Server
{
public:
	Server(asio::ip::port_type port, std::shared_ptr<Log> log);
	~Server();

private:
	asio::awaitable<void> Listener();

	asio::io_context ioctx;
	asio::ip::tcp::acceptor acceptor;
	asio::ip::port_type port;

	std::vector<std::thread> threads;

	std::shared_ptr<Log> log;
};

#endif
