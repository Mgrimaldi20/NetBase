#include <iostream>
#include <memory>
#include <exception>

#include "PosixPluginManager.h"

#include "framework/Asio.h"
#include "framework/Log.h"
#include "framework/Server.h"

constexpr unsigned int NET_DEFAULT_THREADS = 2;
constexpr asio::ip::port_type NET_DEFAULT_PORT = 5001;

static asio::ip::port_type serverport = NET_DEFAULT_PORT;

int main(int argc, char **argv)
{
	try
	{
		std::shared_ptr<Log> log = std::make_shared<Log>();
		std::shared_ptr<PluginManager> pluginmanager = std::make_shared<WinPluginManager>();

		unsigned int numthreads = std::thread::hardware_concurrency() * 2;
		numthreads = (numthreads == 0) ? NET_DEFAULT_THREADS : numthreads;

		if (numthreads == NET_DEFAULT_THREADS)
		{
			log->Warn(
				"The number of threads avaliable is equal to the default number [{}]\n"
				"If this is not correct, you may wish to restart NetBase as the"
				"correct number of system threads have not been detected",
				NET_DEFAULT_THREADS
			);
		}

		Server server(serverport, numthreads, log);
		server.Run();

		log->Info("NetBase server is exiting...");
	}

	catch (const std::exception &e)
	{
		std::cerr << typeid(e).name() << ": Exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
