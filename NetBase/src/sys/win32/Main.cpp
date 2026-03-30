#include <iostream>
#include <charconv>
#include <string_view>
#include <system_error>
#include <thread>
#include <memory>
#include <exception>

#include "framework/Log.h"
#include "framework/Server.h"

constexpr unsigned int NET_DEFAULT_THREADS = 2;
constexpr asio::ip::port_type NET_DEFAULT_PORT = 5001;

static asio::ip::port_type serverport = NET_DEFAULT_PORT;

static bool ValidateOptions(int argc, char **argv);

/* 
* TODO:
* Logs should perhaps be a thread local resource, so a logger is spawned for each thread and can write to their own files without issue.
*/
int main(int argc, char **argv)
{
	try
	{
		if (!ValidateOptions(argc, argv))
			return 1;

		std::shared_ptr<Log> log = std::make_shared<Log>();

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
