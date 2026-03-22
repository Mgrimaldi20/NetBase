#ifndef _NETBASE_FRAMEWORK_CHANNELMANAGER_H_
#define _NETBASE_FRAMEWORK_CHANNELMANAGER_H_

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Channel.h"

#include "Log.h"

/*
* Class: ChannelManager
* A manager for all of the channels, allows for fetching and creation of channel objects.
*
*	Fetch: Gets a channel if it already exists
*	Create: Creates a new channel if it doesnt exist and returns it
*	FetchAll: Fetches a list of every channel managed by the system
*/
class ChannelManager
{
public:
	ChannelManager(std::shared_ptr<Log> log);
	~ChannelManager();

	std::shared_ptr<Channel> Create(const std::string &channelname);
	std::shared_ptr<Channel> Fetch(const std::string &channelname);
	bool Exists(const std::string &channelname);

	std::vector<std::shared_ptr<Channel>> FetchAll();

private:
	std::unordered_map<std::string, std::shared_ptr<Channel>> channels;

	std::shared_ptr<Log> log;
};

#endif
