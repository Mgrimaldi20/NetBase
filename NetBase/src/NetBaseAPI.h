#ifndef __NETBASE_NETBASEAPI_H__
#define __NETBASE_NETBASEAPI_H__

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "framework/CmdDispatcher.h"
#include "framework/ChannelManager.h"

class NetBaseAPI
{
public:
	NetBaseAPI() = default;
	virtual ~NetBaseAPI() = default;

	virtual std::shared_ptr<CmdDispatcher> GetCmdDispatcher() = 0;
	virtual std::shared_ptr<ChannelManager> GetChannelManager() = 0;
};

class ClientAPI
{
public:
	class Parser
	{
	public:
		virtual CmdDispatcher::ParsedCmd operator()(
			std::string_view data,
			std::uint_least64_t length
		) = 0;

		Parser() = default;
		virtual ~Parser() = default;
	};

	ClientAPI() = default;
	virtual ~ClientAPI() = default;

	virtual void RegisterCmds() = 0;
	virtual std::shared_ptr<ClientAPI::Parser> GetParser() = 0;

	virtual std::string GetProtocolName() = 0;
};

// to be implemented by the client protocol library
using GetClientAPI = std::shared_ptr<ClientAPI> (*)(std::shared_ptr<NetBaseAPI>);

#endif
