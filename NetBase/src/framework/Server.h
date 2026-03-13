#ifndef _NETBASE_FRAMEWORK_SERVER_H_
#define _NETBASE_FRAMEWORK_SERVER_H_

#include <memory>
#include <atomic>

#include "asio/io_context.hpp"
#include "asio/awaitable.hpp"

#include "asio/ip/tcp.hpp"
#include "asio/ip/udp.hpp"

#include "Log.h"

/*
* Class: Server
* The main server class, responsible for accepting incoming connections and spawning sessions.
* The server will run on multiple threads, and coroutines will be used to handle async operations.
* 
*	Run: Runs the server, kicks off the threads, and blocks until the server is stopped
*/
class Server
{
public:
	Server(asio::ip::port_type port, unsigned int numthreads, std::shared_ptr<Log> log);
	~Server();

	void Run();

private:
	asio::awaitable<void> Listener();

	// create the iocp/io_uring context for the server
	asio::io_context ioctx;

	std::atomic<bool> restartserver;

	asio::ip::port_type port;
	unsigned int numthreads;
	std::shared_ptr<Log> log;
};

#endif
