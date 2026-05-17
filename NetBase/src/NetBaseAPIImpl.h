#ifndef __NETBASE_NETBASEAPIIMPL_H__
#define __NETBASE_NETBASEAPIIMPL_H__

#include <memory>

#include "NetBaseAPI.h"

class NetBaseAPIImpl : public NetBaseAPI
{
public:
	NetBaseAPIImpl(
		std::shared_ptr<CmdDispatcher> dispatcher,
		std::shared_ptr<ChannelManager> manager,
		std::shared_ptr<Log> log
	);

	virtual ~NetBaseAPIImpl() = default;

	std::shared_ptr<CmdDispatcher> GetCmdDispatcher() override final;
	std::shared_ptr<ChannelManager> GetChannelManager() override final;
	std::shared_ptr<Log> GetLogger() override final;

private:
	std::shared_ptr<CmdDispatcher> dispatcher;
	std::shared_ptr<ChannelManager> manager;
	std::shared_ptr<Log> log;
};

#endif
