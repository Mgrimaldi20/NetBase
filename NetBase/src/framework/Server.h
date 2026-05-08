#ifndef __NETBASE_FRAMEWORK_SERVER_H__
#define __NETBASE_FRAMEWORK_SERVER_H__

#include <memory>

#include "Asio.h"
#include "CmdDispatcher.h"

#include "log/Log.h"

/*
* Class: Server
* The main server class, responsible for accepting incoming connections and spawning sessions.
* The server will run on a single thread, and coroutines will be used to handle async accepts.
* 
*	Run: Runs the server, and blocks until the server is stopped
*/
class Server
{
public:
	Server(
		asio::ip::port_type port,
		std::shared_ptr<Log> log,
		std::shared_ptr<CmdDispatcher> dispatcher
	);

	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;

	Server(Server &&) = default;
	Server &operator=(Server &&) = default;

	~Server();

	void Run();

private:
	asio::awaitable<void> Listener();

	void RegisterSignals();

	// create the iocp/io_uring context for the server
	asio::io_context ioctx;

	asio::signal_set signals;

	asio::ip::port_type port;
	std::shared_ptr<Log> log;
	std::shared_ptr<CmdDispatcher> dispatcher;
};

#endif
