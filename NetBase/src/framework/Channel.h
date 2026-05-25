#ifndef __NETBASE_FRAMEWORK_CHANNEL_H__
#define __NETBASE_FRAMEWORK_CHANNEL_H__

#include <unordered_set>
#include <string>
#include <memory>

#include "NetBaseAPI.h"

#include "Client.h"

#include "log/Log.h"

/*
* Class: Channel
* Handles inter-client communication on channels, clients can join, leave and broadcast data on channels.
* Only unique clients can join a channel, the same client cannot join the same channel multiple times.
* A channel can only be created by a ChannelManager factory instance, as its a managed object.
* 
*	Join: Add a client to a channel
*	Leave: Remove a client from a channel
*	Broadcast: Send a message to all clients connected to the channel
*/
class Channel
{
public:
	Channel(std::string channelname, std::shared_ptr<Log> log);
	~Channel();

	void Join(std::shared_ptr<Client> client);
	void Leave(std::shared_ptr<Client> client);

	void Broadcast(std::string message);

private:
	std::unordered_set<std::shared_ptr<Client>> clients;

	std::string channelname;
	std::shared_ptr<Log> log;
};

#endif
