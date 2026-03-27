#include <algorithm>
#include <iterator>
#include <utility>
#include <exception>

#include "ChannelManager.h"

ChannelManager::ChannelManager(std::shared_ptr<Log> log, asio::any_io_executor exec)
	: channels(),
	strand(exec),
	log(log)
{
	log->Info("Channel Manager started");
}

ChannelManager::~ChannelManager()
{
	log->Info("Shutting down the Channel Manager");
}

asio::awaitable<std::shared_ptr<Channel>> ChannelManager::Create(std::string channelname)
{
	co_await asio::dispatch(strand, asio::use_awaitable);

	asio::any_io_executor exec = strand.get_inner_executor();

	auto [it, inserted] = channels.emplace(
		channelname,
		std::make_shared<Channel>(
			channelname,
			log,
			asio::make_strand(exec)
		)
	);

	if (!inserted)
		log->Warn("Failed to create Channel, already exists, fetching existing Channel: {}", channelname);

	co_return it->second;
}

asio::awaitable<std::shared_ptr<Channel>> ChannelManager::Fetch(std::string channelname)
{
	co_await asio::dispatch(strand, asio::use_awaitable);

	auto it = channels.find(channelname);
	if (it != channels.end())
		co_return it->second;

	co_return nullptr;
}

asio::awaitable<bool> ChannelManager::Exists(std::string channelname)
{
	co_await asio::dispatch(strand, asio::use_awaitable);
	co_return channels.contains(channelname);
}

asio::awaitable<std::vector<std::shared_ptr<Channel>>> ChannelManager::FetchAll()
{
	co_await asio::dispatch(strand, asio::use_awaitable);

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

	co_return elements;
}
