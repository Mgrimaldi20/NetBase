#include <unordered_set>

#include "Channel.h"

struct Channel::Impl
{
	Impl(std::string channelname, std::shared_ptr<Log> log)
		: clients(),
		channelname(std::move(channelname)),
		log(log)
	{}

	~Impl() = default;

	std::unordered_set<std::shared_ptr<Client>> clients;

	std::string channelname;
	std::shared_ptr<Log> log;
};

Channel::Channel(std::string channelname, std::shared_ptr<Log> log)
	: pimpl(PImplPtr<Channel::Impl>::MakePImpl(std::move(channelname), log))
{
	pimpl->log->Info("Channel: {} has been created", pimpl->channelname);
}

Channel::~Channel()
{
	pimpl->log->Info("Channel: {} has been closed", pimpl->channelname);
}

void Channel::Join(std::shared_ptr<Client> client)
{
	pimpl->clients.insert(client);
	pimpl->log->Info("Client: {} has joined Channel: {}", client->GetAddr(), pimpl->channelname);
}

void Channel::Leave(std::shared_ptr<Client> client)
{
	pimpl->clients.erase(client);
	pimpl->log->Info("Client: {} has left Channel: {}", client->GetAddr(), pimpl->channelname);
}

void Channel::Broadcast(std::string message)
{
	pimpl->log->Debug("Broadcasting on Channel: {} :: {}", pimpl->channelname, message);

	for (const auto &client : pimpl->clients)
		client->Send(message);

	pimpl->log->Debug("Broadcast on Channel: {} :: Complete", pimpl->channelname);
}
