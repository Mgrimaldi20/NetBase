#ifndef _NETBASE_FRAMEWORK_SERVER_H_
#define _NETBASE_FRAMEWORK_SERVER_H_

#include <memory>

#include "Asio.h"
#include "Log.h"

/*
* Class: Server
* The main server class, responsible for accepting incoming connections and spawning sessions.
* The server will run on multiple threads, and coroutines will be used to handle async accepts.
* 
*	Run: Runs the server, kicks off the threads, and blocks until the server is stopped
*/
class Server
{
public:
	Server(asio::ip::port_type port, unsigned int numthreads, std::shared_ptr<Log> log);

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
	unsigned int numthreads;
	std::shared_ptr<Log> log;
};

#endif
