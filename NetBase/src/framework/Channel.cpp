#include "Channel.h"

Channel::Channel(std::string_view channelname, std::shared_ptr<Log> log, asio::any_io_executor exec)
	: strand(exec),
	clients(),
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
	asio::dispatch(strand, [this, client]()
	{
		clients.insert(client);
		log->Info("Client: {} has joined Channel: {}", client->GetAddr(), channelname);
	});
}

void Channel::Leave(std::shared_ptr<Client> client)
{
	asio::dispatch(strand, [this, client]()
	{
		clients.erase(client);
		log->Info("Client: {} has left Channel: {}", client->GetAddr(), channelname);
	});
}

void Channel::Broadcast(std::shared_ptr<std::string> message)
{
	asio::dispatch(strand, [this, message]()
	{
		log->Debug("Broadcasting on Channel: {} :: {}", channelname, *message);

		for (const std::shared_ptr<Client> &client : clients)
			client->Send(message);

		log->Debug("Broadcast on Channel: {} :: Complete", channelname);
	});
}
