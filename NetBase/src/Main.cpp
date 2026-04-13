#include <iostream>
#include <charconv>
#include <string_view>
#include <system_error>
#include <thread>
#include <memory>
#include <exception>
#include <filesystem>

#include "framework/Asio.h"
#include "framework/Server.h"
#include "framework/Log.h"

constexpr unsigned int NET_DEFAULT_THREADS = 2;
constexpr asio::ip::port_type NET_DEFAULT_PORT = 5001;

static asio::ip::port_type serverport = NET_DEFAULT_PORT;
static std::filesystem::path dylibpath;

static bool ValidateOptions(int argc, char **argv);

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
			log->Warn("The number of threads avaliable is equal to the default number [{}]", NET_DEFAULT_THREADS);
			log->Warn("If this is not correct, you may wish to restart NetBase as the");
			log->Warn("correct number of system threads have not been detected");
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
					std::cout << "Invalid port number command line format: " << argv[i] << std::endl;
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

			case 'd':
			{
				std::string_view fullpath(argv[i] + 2);

				if (fullpath.empty() || fullpath[0] != ':')
				{
					std::cout << "Invalid dynamic library path command line format: " << argv[i] << std::endl;
					return false;
				}

				std::filesystem::path path(fullpath);
				if (!path.has_filename() || !path.has_extension())
				{
					std::cout << "Invalid dynamic library file path: " << path << std::endl;
					return false;
				}

				std::cout << "Plugin to load: " << path << std::endl;

				dylibpath = std::filesystem::canonical(path);

				break;
			}

			case '?':
				std::cout << std::endl << "Usage:" << std::endl
					<< "NetBase [-p:<port>] [-d:<dylib fullpath>] [-?]" << std::endl
					<< "------------------------------------------------------------" << std::endl
					<< "-p:<port>              Specify the port number of the server" << std::endl
					<< "-d:<dylib fullpath>    Specify the full path of the protocol" << std::endl
					<< "-?                     Prints out this help message and exit" << std::endl;

				return false;

			default:
				std::cout << "Unknown command line options flag: " << argv[i] << std::endl;
				return false;
		}
	}

	return true;
}
