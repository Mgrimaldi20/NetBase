#include "Channel.h"

Channel::Channel(std::string_view channelname, std::shared_ptr<Log> log)
	: clients(),
	channelname(channelname),
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
	log->Info("Client: {} has joined Channel: {}", client->GetAddr(), channelname);
}

void Channel::Leave(std::shared_ptr<Client> client)
{
	clients.erase(client);
	log->Info("Client: {} has left Channel: {}", client->GetAddr(), channelname);
}

void Channel::Broadcast(std::shared_ptr<std::string> message)
{
	log->Debug("Broadcasting on Channel: {} :: {}", channelname, *message);

	for (const auto &client : clients)
		client->Send(message);

	log->Debug("Broadcast on Channel: {} :: Complete", channelname);
}
