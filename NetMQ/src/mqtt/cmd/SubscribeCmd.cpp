#include <mutex>
#include <vector>

#include "SubscribeCmd.h"

SubscribeCmd::SubscribeCmd(std::shared_ptr<IOContext> ioctx, SubManager &manager, ByteBuffer &params)
	: Cmd(ioctx, manager),
	ackdata({ .type = Cmd::Type::Subscribe })
{
	(void)params;
}

void SubscribeCmd::ExecuteCmd()
{
	if (!ioctx->GetConnected().load())
		return;

	std::scoped_lock lock(manager.subsmtx);

	std::span<std::byte> topickey(topic.begin(), topic.end());
	auto iter = manager.subscriptions.find(topickey);

	if (iter != manager.subscriptions.end())
	{
		std::vector<std::shared_ptr<IOContext>> &sublist = iter->second;

		if (std::find(sublist.begin(), sublist.end(), ioctx) == sublist.end())
			sublist.push_back(ioctx);
	}

	else
	{
		std::vector<std::shared_ptr<IOContext>> newsublist({ ioctx });
		newsublist.push_back(ioctx);
		manager.subscriptions[std::move(std::vector<std::byte>(topickey.begin(), topickey.end()))] = std::move(newsublist);
	}

	ackdata.reason = Cmd::ReasonCode::Success;
}

void SubscribeCmd::ExecuteAck()
{
	ioctx->PostSend(
		ackbuffer
		.WriteUInt<Cmd::Type>(ackdata.type)
		.WriteUInt<Cmd::ReasonCode>(ackdata.reason)
		.Build()
	);
}
