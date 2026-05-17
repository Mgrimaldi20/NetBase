#include "NetBaseAPIImpl.h"

NetBaseAPIImpl::NetBaseAPIImpl(
	std::shared_ptr<CmdDispatcher> dispatcher,
	std::shared_ptr<ChannelManager> manager,
	std::shared_ptr<Log> log
)
	: dispatcher(dispatcher),
	manager(manager),
	log(log)
{
}

std::shared_ptr<CmdDispatcher> NetBaseAPIImpl::GetCmdDispatcher()
{
	return dispatcher;
}

std::shared_ptr<ChannelManager> NetBaseAPIImpl::GetChannelManager()
{
	return manager;
}

std::shared_ptr<Log> NetBaseAPIImpl::GetLogger()
{
	return log;
}
