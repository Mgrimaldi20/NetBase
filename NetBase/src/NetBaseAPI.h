#ifndef __NETBASE_NETBASEAPI_H__
#define __NETBASE_NETBASEAPI_H__

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "framework/CmdDispatcher.h"
#include "framework/ChannelManager.h"

#include "framework/log/Log.h"

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

class NetBaseAPI
{
public:
	NetBaseAPI() = default;
	virtual ~NetBaseAPI() = default;

	virtual std::shared_ptr<CmdDispatcher> GetCmdDispatcher() = 0;
	virtual std::shared_ptr<ChannelManager> GetChannelManager() = 0;
	virtual std::shared_ptr<Log> GetLogger() = 0;
};

class ClientAPI
{
public:
	class Parser
	{
	public:
		Parser() = default;
		virtual ~Parser() = default;

		virtual CmdDispatcher::ParsedCmd Parse(
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
