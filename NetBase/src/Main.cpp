#include <iostream>
#include <string_view>

#include "framework/Log.h"

static std::string_view serverport = "5001";	// default port number of the server

static bool ValidateOptions(int argc, char **argv);

int main(int argc, char **argv)
{
	if (!ValidateOptions(argc, argv))
		return 1;

	Log log;

	log.Info("Server port: {}", serverport);

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
				std::string_view arg(argv[i]);

				if (arg.length() <= 3 || arg[2] != ':')
				{
					std::cout << "Invalid port number format: " << argv[i] << std::endl;
					return false;
				}

				serverport = arg.substr(3);

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
