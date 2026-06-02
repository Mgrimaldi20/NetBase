#ifndef __NETBASE_NETBASEAPIIMPL_H__
#define __NETBASE_NETBASEAPIIMPL_H__

#include <memory>

#include "NetBaseAPI.h"

/*
* Class: NetBaseAPIImpl
* The implementation of the abstract NetBaseAPI interface that will be sent over to the client.
* 
*	GetCmdDispatcher: Gets the command dispatcher instance from the engine
*	GetChannelManager: Gets the channel manager instance from the engine
*	GetLogger: Creates and gets a new logger instance that can be configured by the client
*/
class NetBaseAPIImpl : public NetBaseAPI
{
public:
	NetBaseAPIImpl(
		std::shared_ptr<CmdDispatcher> dispatcher,
		std::shared_ptr<ChannelManager> chmanager,
		std::shared_ptr<Log> log
	);

	virtual ~NetBaseAPIImpl() = default;

	CmdDispatcher &GetCmdDispatcher() override final;
	ChannelManager &GetChannelManager() override final;
	Log &GetLogger() override final;

private:
	std::shared_ptr<CmdDispatcher> dispatcher;
	std::shared_ptr<ChannelManager> chmanager;
	std::shared_ptr<Log> log;
};

#endif
