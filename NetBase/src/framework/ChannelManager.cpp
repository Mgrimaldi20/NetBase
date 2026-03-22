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

std::shared_ptr<Channel> ChannelManager::Create(const std::string &channelname)
{
	return channels.emplace(
		channelname,
		std::make_shared<Channel>(channelname, log)
	).first->second;
}

std::shared_ptr<Channel> ChannelManager::Fetch(const std::string &channelname)
{
	try
	{
		std::shared_ptr<Channel> channel = channels.at(channelname);
		return channel;
	}

	catch (const std::exception &e)
	{
		log->Warn("Channel Manager error: {}", e.what());
	}

	return nullptr;
}

bool ChannelManager::Exists(const std::string &channelname)
{
	return channels.contains(channelname);
}

std::vector<std::shared_ptr<Channel>> ChannelManager::FetchAll()
{
	std::vector<std::shared_ptr<Channel>> elements;

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
