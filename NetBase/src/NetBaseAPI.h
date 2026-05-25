#ifndef __NETBASE_NETBASEAPI_H__
#define __NETBASE_NETBASEAPI_H__

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#if defined(NETBASE_WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(NETBASE_LINUX) || defined(NETBASE_APPLE)
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT
#define IMPORT
#endif

#if defined(NETBASE_EXPORTS)
#define NETBASE_API EXPORT
#else
#define NETBASE_API IMPORT
#endif

// forward decl these so they dont get caught up in the import/export stuff
class CmdDispatcher;
class ChannelManager;
class Log;

/*
* Class: NetBaseAPI
* The API interface that the NetBase engine will implement and send to the client protocol library.
* This interface contains everything thats needed to build a fully featured network protocol.
* 
*	GetCmdDispatcher: Gets a command dispatcher instance for registering and dispatching commands
*	GetChannelManager: gets a channel manager instance to create and destroy channels, also for joining clients
*	GetLogger: Gets a brand new logger instance for the protocol to use, client to configure
*/
class NetBaseAPI
{
public:
	NetBaseAPI() = default;
	virtual ~NetBaseAPI() = default;

	virtual std::shared_ptr<CmdDispatcher> GetCmdDispatcher() = 0;
	virtual std::shared_ptr<ChannelManager> GetChannelManager() = 0;
	virtual std::shared_ptr<Log> GetLogger() = 0;
};

/*
* Class: ClientAPI
* The API interface that is implemented by the client protocol that NetBase uses to parse commands and do protocol setup.
* The client must implement the virtual functions which provide a parser and the registration function.
* The registration function is called once at startup of NetBase, and should call the NetBaseAPI command dispatcher to register commands.
* 
*	Parser::Parse: Tells NetBase how to parse a command to determine what command to execute and gives the client protocol its data block
*	RegisterCmds: Should call the NetBaseAPI registration functions to set up commands with IDs and functor objects (any functor type)
*	GetParser: Gets the created Parser class instance to NetBase which contains the implemented Parse function
*	GetProtocolName: Retrieves the name of the protocol thats implemented (mostly used for logging)
*/
class ClientAPI
{
public:
	class Parser
	{
	public:
		struct ParsedCmd	// mapped to internal struct
		{
			std::uint_least16_t cmdid;
			std::string_view data;
		};

		Parser() = default;
		virtual ~Parser() = default;

		virtual ClientAPI::Parser::ParsedCmd Parse(
			std::string_view data,
			std::uint_least64_t length
		) = 0;
	};

	ClientAPI() = default;
	virtual ~ClientAPI() = default;

	virtual void RegisterCmds() = 0;
	virtual std::shared_ptr<ClientAPI::Parser> GetParser() = 0;

	virtual std::string &GetProtocolName() = 0;
};

#endif
