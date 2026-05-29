#include <unordered_set>

#include "Channel.h"

struct Channel::Impl
{
	Impl(std::string channelname)
		: clients(),
		channelname(std::move(channelname))
	{}

	~Impl() = default;

	std::unordered_set<std::shared_ptr<Client>> clients;
	std::string channelname;
};

Channel::Channel(std::string channelname, std::shared_ptr<Log> log)
	: pimpl(std::move(channelname)),
	log(log)
{
	log->Info("Channel: {} has been created", channelname);
}

Channel::~Channel()
{
	log->Info("Channel: {} has been closed", pimpl->channelname);
}

void Channel::Join(std::shared_ptr<Client> client)
{
	pimpl->clients.insert(client);
	log->Info("Client: {} has joined Channel: {}", client->GetAddr(), pimpl->channelname);
}

void Channel::Leave(std::shared_ptr<Client> client)
{
	pimpl->clients.erase(client);
	log->Info("Client: {} has left Channel: {}", client->GetAddr(), pimpl->channelname);
}

void Channel::Broadcast(std::string message)
{
	log->Debug("Broadcasting on Channel: {} :: {}", pimpl->channelname, message);

	for (const auto &client : pimpl->clients)
		client->Send(message);

	log->Debug("Broadcast on Channel: {} :: Complete", pimpl->channelname);
}
