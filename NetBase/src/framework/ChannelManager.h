#ifndef _NETBASE_FRAMEWORK_CHANNELMANAGER_H_
#define _NETBASE_FRAMEWORK_CHANNELMANAGER_H_

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "../Asio.h"

#include "Channel.h"

#include "Log.h"

/*
* Class: ChannelManager
* A manager for all of the channels, allows for fetching and creation of channel objects.
*
*	Fetch: Gets a channel if it already exists
*	Create: Creates a new channel if it doesnt exist and returns it
*	Exists: Check if a channel with the specified name exists
*	FetchAll: Fetches a list of every channel managed by the system
*/
class ChannelManager
{
public:
	ChannelManager(std::shared_ptr<Log> log, asio::any_io_executor exec);
	~ChannelManager();

	asio::awaitable<std::shared_ptr<Channel>> Create(std::string channelname);
	asio::awaitable<std::shared_ptr<Channel>> Fetch(std::string channelname);
	asio::awaitable<bool> Exists(std::string channelname);

	asio::awaitable<std::vector<std::shared_ptr<Channel>>> FetchAll();

private:
	std::unordered_map<std::string, std::shared_ptr<Channel>> channels;

	asio::strand<asio::any_io_executor> strand;

	std::shared_ptr<Log> log;
};

#endif
