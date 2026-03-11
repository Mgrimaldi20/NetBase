#include <iostream>
#include <charconv>
#include <string_view>
#include <system_error>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>

#include "asio/io_context.hpp"
#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/awaitable.hpp"
#include "asio/use_awaitable.hpp"
#include "asio/write.hpp"
#include "asio/signal_set.hpp"

#include "asio/ip/tcp.hpp"
#include "asio/ip/udp.hpp"

#include "framework/Log.h"

constexpr int NET_SIGINT = SIGINT;
constexpr int NET_SIGTERM = SIGTERM;

#if defined(SIGBREAK)
constexpr int NET_RESTART_SIG = SIGBREAK;
#elif defined(SIGTSTP)
constexpr int NET_RESTART_SIG = SIGTSTP;
#endif

constexpr unsigned int NET_DEFAULT_THREADS = 2;

static std::atomic<bool> restartserver;
static asio::ip::port_type serverport = 5001;	// default port number of the server

static bool ValidateOptions(int argc, char **argv);

static asio::awaitable<void> Listener(asio::io_context &ioctx, asio::ip::port_type port, std::shared_ptr<Log> log);
static asio::awaitable<void> Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log);

int main(int argc, char **argv)
{
	if (!ValidateOptions(argc, argv))
		return 1;

	std::shared_ptr<Log> log = std::make_shared<Log>();

	// TODO: The server should be implemented in a separate class, and the main function should only be responsible for parsing options and starting the server.
	// The server should also handle graceful shutdown and cleanup of resources when the program is terminated.
	// The server should also handle multiple clients concurrently, and should be able to broadcast messages to all relevant clients when a command is executed.
	// The server should also handle errors and exceptions gracefully, and should log any relevant information to the log file or console.

	do
	{
		restartserver = false;

		unsigned int numthreads = std::thread::hardware_concurrency() * 2;
		numthreads = (numthreads == 0) ? NET_DEFAULT_THREADS : numthreads;

		if (numthreads == NET_DEFAULT_THREADS)
		{
			log->Warn("The number of threads avaliable is equal to the default number [{}]\n"
				"If this is not correct, you may wish to restart NetBase as the"
				"correct number of system threads have not been detected",
				NET_DEFAULT_THREADS
			);
		}

		// create the iocp/io_uring context for the server
		asio::io_context ioctx;

		// start the server and listen for incoming connections on the specified port number
		asio::co_spawn(ioctx, Listener(ioctx, serverport, log), asio::detached);

		// register signal handler for graceful shutdown or restart
		asio::signal_set signals(ioctx, NET_SIGINT, NET_SIGTERM, NET_RESTART_SIG);
		signals.async_wait([&](const std::error_code &ec, int signal)
		{
			if (!ec)
			{
				log->Info("Signal received: {}", signal);

				if (signal == NET_RESTART_SIG)
				{
					log->Info("NetBase server is restarting...\n");
					restartserver = true;
				}

				ioctx.stop();
			}
		});

		log->Info("Number of worker threads available: {}", numthreads);
		log->Info("NetBase server running on port: {}", serverport);
		log->Info("Press Ctrl-C to exit, or Ctrl-Break/Ctrl-Z to restart...");

		// run the server with a thread pool and handle requests until stopped
		std::vector<std::thread> threads;
		for (unsigned int i=0; i<numthreads; i++)
		{
			threads.emplace_back([&ioctx]()
			{
				ioctx.run();
			});
		}

		for (std::thread &thread : threads)
		{
			if (thread.joinable())
				thread.join();
		}	
	} while (restartserver);

	log->Info("NetBase server is exiting...");

	return 0;
}

bool ValidateOptions(int argc, char **argv)
{
	for (int i=1; i<argc; i++)
	{
		if (argv[i][0] != '-')
			continue;

		switch (argv[i][1])
		{
			case 'p':
			{
				std::string_view portstr(argv[i] + 2);

				if (portstr.empty() || portstr[0] != ':')
				{
					std::cout << "Invalid port number format: " << argv[i] << std::endl;
					return false;
				}

				std::from_chars_result result = std::from_chars(
					portstr.data() + 1,
					portstr.data() + portstr.size(),
					serverport
				);

				if (result.ec != std::errc())
				{
					std::cout << "Invalid port number: " << portstr.substr(1) << std::endl;
					return false;
				}

				std::cout << "Server port set to: " << serverport << std::endl;

				break;
			}

			case '?':
				std::cout << std::endl << "Usage:" << std::endl
					<< "NetBase [-p:<port>] [-?]" << std::endl
					<< "--------------------------------------------------" << std::endl
					<< "-p:<port>    Specify the port number of the server" << std::endl
					<< "-?           Prints out this help message and exit" << std::endl;

				return false;

			default:
				std::cout << "Unknown command line options flag: " << argv[i] << std::endl;
				return false;
		}
	}

	return true;
}

asio::awaitable<void> Listener(asio::io_context &ioctx, asio::ip::port_type port, std::shared_ptr<Log> log)
{
	asio::ip::tcp::acceptor acceptor(ioctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

	while (true)
	{
		asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
		asio::co_spawn(ioctx, Session(std::move(socket), log), asio::detached);
	}
}

asio::awaitable<void> Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log)
{
	try
	{
		while (true)
		{
			char buffer[1024];
			char response[1024] = "Hello client!";

			std::size_t n = co_await socket.async_read_some(asio::buffer(buffer), asio::use_awaitable);
			(void)n;
			// parse, handle, respond...
			co_await asio::async_write(socket, asio::buffer(response), asio::use_awaitable);
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
	}
}
