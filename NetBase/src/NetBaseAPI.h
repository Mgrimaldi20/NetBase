#ifndef __NETBASE_NETBASEAPI_H__
#define __NETBASE_NETBASEAPI_H__

#include <cstdint>
#include <memory>
#include <string_view>

#include "framework/CmdDispatcher.h"
#include "framework/ChannelManager.h"

class NetBaseAPI
{
public:
	NetBaseAPI() = default;
	virtual ~NetBaseAPI() = default;

	std::shared_ptr<CmdDispatcher> dispatcher;
	std::shared_ptr<ChannelManager> channelmanager;
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

	std::shared_ptr<ClientAPI::Parser> parser;
};

using GetClientAPI = std::shared_ptr<ClientAPI> (*)(std::shared_ptr<NetBaseAPI>);

#endif
