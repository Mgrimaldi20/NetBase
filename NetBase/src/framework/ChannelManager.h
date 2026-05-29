#ifndef __NETBASE_FRAMEWORK_CHANNELMANAGER_H__
#define __NETBASE_FRAMEWORK_CHANNELMANAGER_H__

#include <string>
#include <memory>
#include <vector>

#include "NetBaseAPI.h"

#include "Channel.h"
#include "PImplPtr.h"

#include "log/Log.h"

/*
* Class: ChannelManager
* A manager for all of the channels, allows for fetching and creation of channel objects.
*
*	Create: Creates a new channel if it doesnt exist and returns it
*	Fetch: Gets a channel if it already exists
*	FetchAll: Fetches a list of every channel managed by the system
*	Exists: Check if a channel with the specified name exists
*	Destroy: Deletes a channel if it exists and kicks out all connected clients
*/
class NETBASE_API ChannelManager
{
public:
	ChannelManager(std::shared_ptr<Log> log);
	~ChannelManager();

	std::shared_ptr<Channel> Create(std::string channelname);

	std::shared_ptr<Channel> Fetch(std::string channelname);
	std::vector<std::shared_ptr<Channel>> FetchAll();

	bool Exists(std::string channelname);

	void Destroy(std::string channelname);

private:
	struct Impl;
	PImplPtr<Impl> pimpl;

	std::shared_ptr<Log> log;
};

#endif
