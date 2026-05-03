#include <algorithm>
#include <iterator>
#include <utility>
#include <exception>

#include "ChannelManager.h"

ChannelManager::ChannelManager(std::shared_ptr<Log> log)
	: channels(),
	log(log)
{
	log->Info("Channel Manager started");
}

ChannelManager::~ChannelManager()
{
	log->Info("Shutting down the Channel Manager");
}

std::shared_ptr<Channel> ChannelManager::Create(std::string channelname)
{
	auto [it, inserted] = channels.emplace(
		channelname,
		std::make_shared<Channel>(
			channelname,
			log
		)
	);

	if (!inserted)
		log->Warn("Failed to create Channel, already exists, fetching existing Channel: {}", channelname);

	return it->second;
}

std::shared_ptr<Channel> ChannelManager::Fetch(std::string channelname)
{
	auto it = channels.find(channelname);
	if (it != channels.end())
		return it->second;

	return nullptr;
}

std::vector<std::shared_ptr<Channel>> ChannelManager::FetchAll()
{
	std::vector<std::shared_ptr<Channel>> elements;
	elements.reserve(channels.size());

	auto Transformer = [](const std::pair<const std::string, std::shared_ptr<Channel>> &pair)
	{
		return pair.second;
	};

	std::transform(
		channels.begin(),
		channels.end(),
		std::back_inserter(elements),
		Transformer
	);

	return elements;
}

bool ChannelManager::Exists(std::string channelname)
{
	return channels.contains(channelname);
}

void ChannelManager::Destroy(std::string channelname)
{
	if (!Exists(channelname))
		return;

	channels.erase(channelname);
}
