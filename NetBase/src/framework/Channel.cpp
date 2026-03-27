#include "Channel.h"

Channel::Channel(std::string_view channelname, std::shared_ptr<Log> log)
	: channelname(channelname),
	clients(),
	log(log)
{
	log->Info("Channel: {} has been created", channelname);
}

Channel::~Channel()
{
	log->Info("Channel: {} has been closed", channelname);
}

void Channel::Join(std::shared_ptr<Client> client)
{
	clients.insert(client);

	log->Info("Client: {} has joined channel: {}", client->GetAddr(), channelname);
}

void Channel::Leave(std::shared_ptr<Client> client)
{
	clients.erase(client);

	log->Info("Client: {} has left channel: {}", client->GetAddr(), channelname);
}

void Channel::Broadcast(std::string_view message)
{
	log->Debug("Channel: {} :: {}", channelname, message);

	for (const std::shared_ptr<Client> &client : clients)
		client->Send(message);
}
