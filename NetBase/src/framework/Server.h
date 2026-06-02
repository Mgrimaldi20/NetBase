#ifndef __NETBASE_FRAMEWORK_SERVER_H__
#define __NETBASE_FRAMEWORK_SERVER_H__

#include <memory>
#include <functional>

#include "NetBaseAPI.h"

#include "Asio.h"
#include "CmdDispatcher.h"

#include "log/Log.h"

/*
* Class: Server
* The main server class, responsible for accepting incoming connections and spawning sessions.
* The server will run on a single thread, and coroutines will be used to handle async accepts.
*/
class Server
{
public:
	Server(
		asio::io_context &ioctx,
		asio::ip::port_type port,
		std::shared_ptr<Log> log,
		std::shared_ptr<CmdDispatcher> dispatcher,
		ClientAPI::Parser &parser
	);

	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;

	Server(Server &&) = default;
	Server &operator=(Server &&) = default;

	~Server();

private:
	asio::awaitable<void> Listener(
		asio::ip::port_type port,
		std::shared_ptr<CmdDispatcher> dispatcher,
		ClientAPI::Parser &parser
	);

	void RegisterSignals();

	std::reference_wrapper<asio::io_context> ioctx;
	std::shared_ptr<Log> log;

	asio::signal_set signals;
};

#endif
