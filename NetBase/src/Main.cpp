#include <iostream>
#include <print>
#include <charconv>
#include <string_view>
#include <system_error>
#include <memory>
#include <exception>
#include <filesystem>
#include <vector>

#include "NetBaseAPI.h"
#include "NetBaseAPIImpl.h"

#include "sys/DynamicLibrary.h"

#include "framework/Asio.h"
#include "framework/Server.h"
#include "framework/CmdDispatcher.h"
#include "framework/ChannelManager.h"

#include "framework/log/Log.h"
#include "framework/log/driver/Driver.h"
#include "framework/log/entry/Entry.h"
#include "framework/log/sink/text/console/ConsoleSink.h"
#include "framework/log/sink/text/file/FileSink.h"
#include "framework/log/formatter/text/basic/BasicTextFormatter.h"
#include "framework/log/policy/trace/StacktracePolicy.h"
#include "framework/log/policy/trace/SourceLocationPolicy.h"

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

		// single thread hint
		asio::io_context ioctx(1);

		std::shared_ptr<Log> log = std::make_shared<Log>(
			"NetBase",
			std::make_shared<Driver>(
				"NetBaseMainLogDriver",
				std::vector<std::shared_ptr<Sink>>
				{
					std::make_shared<ConsoleSink>(std::make_unique<BasicTextFormatter>())
				},
				std::vector<std::shared_ptr<Policy>>
				{
					std::make_shared<StacktracePolicy>(Entry::Level::Fatal),
					std::make_shared<SourceLocationPolicy>(Entry::Level::Debug)
				}
			)
		);

		std::shared_ptr<CmdDispatcher> dispatcher = std::make_shared<CmdDispatcher>(log);
		std::shared_ptr<ChannelManager> channelmanager = std::make_shared<ChannelManager>(log);

		std::unique_ptr<DynamicLibrary> dylib = DynamicLibrary::CreateDynamicLibrary(dylibpath);

		log->Info("Loaded plugin library: {}", dylibpath.string());

		std::any func = dylib->GetSymbol("GetClientAPI");

		// to be implemented by the client protocol library
		using GetClientAPI = std::shared_ptr<ClientAPI>(*)(std::shared_ptr<NetBaseAPI>);

		GetClientAPI GetAPI = std::any_cast<GetClientAPI>(func);

		// create a new logger instance for the client, they can add sinks and policies
		std::shared_ptr<ClientAPI> clientapi = GetAPI(
			std::make_shared<NetBaseAPIImpl>(dispatcher, channelmanager, std::make_shared<Log>())
		);

		std::shared_ptr<ClientAPI::Parser> parser;

		try
		{
			if (!clientapi)
				throw std::runtime_error("Trying to dereference the ClientAPI when its null");

			parser = clientapi->GetParser();
		}

		catch (const std::exception &e)
		{
			throw e;	// rethrow the exception to be caught by global
		}

		Server server(serverport, ioctx, log, dispatcher, parser);

		ioctx.run();

		log->Info("NetBase server is exiting...");
	}

	catch (const std::exception &e)
	{
		std::println(std::cerr, "{} :: Fatal Exception :: {}", typeid(e).name(), e.what());
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
					std::println("Invalid port number command line format: {}", argv[i]);
					return false;
				}

				std::from_chars_result result = std::from_chars(
					portstr.data() + 1,
					portstr.data() + portstr.size(),
					serverport
				);

				if (result.ec != std::errc())
				{
					std::println("Invalid port number: {}", portstr.substr(1));
					return false;
				}

				std::println("Server port set to: {}", serverport);

				break;
			}

			case 'd':
			{
				std::string_view fullpath(argv[i] + 2);

				if (fullpath.empty() || fullpath[0] != ':')
				{
					std::println("Invalid dynamic library path command line format: {}", argv[i]);
					return false;
				}

				std::filesystem::path path(fullpath.substr(1));
				if (!path.has_filename() || !path.has_extension())
				{
					std::println("Invalid dynamic library file path: {}", path.string());
					return false;
				}

				dylibpath = std::filesystem::canonical(path);

				std::println("Plugin to load (canonical path): {}", dylibpath.string());

				break;
			}

			case '?':
				std::println("\nUsage:\n");
				std::println("NetBase [-p:<port>] [-d:<dylib fullpath>] [-?]");
				std::println("------------------------------------------------------------");
				std::println("-p:<port>              Specify the port number of the server");
				std::println("-d:<dylib fullpath>    Specify the full path of the protocol");
				std::println("-?                     Prints out this help message and exit");

				return false;

			default:
				std::println("Unknown command line options flag: {}", argv[i]);
				return false;
		}
	}

	return true;
}
