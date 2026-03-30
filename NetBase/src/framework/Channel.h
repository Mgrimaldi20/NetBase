#ifndef _NETBASE_FRAMEWORK_CHANNEL_H_
#define _NETBASE_FRAMEWORK_CHANNEL_H_

#include <unordered_set>
#include <string_view>
#include <string>
#include <memory>

#include "Asio.h"
#include "Client.h"
#include "Log.h"

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
	Channel(std::string_view channelname, std::shared_ptr<Log> log, asio::any_io_executor exec);
	~Channel();

	void Join(std::shared_ptr<Client> client);
	void Leave(std::shared_ptr<Client> client);

	void Broadcast(std::shared_ptr<std::string> message);

private:
	std::unordered_set<std::shared_ptr<Client>> clients;

	asio::strand<asio::any_io_executor> strand;

	std::string channelname;
	std::shared_ptr<Log> log;
};

#endif
