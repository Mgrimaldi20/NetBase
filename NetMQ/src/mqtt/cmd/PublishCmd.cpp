#include <mutex>

#include "PublishCmd.h"

PublishCmd::PublishCmd(std::shared_ptr<IOContext> ioctx, SubManager &manager, ByteBuffer &params)
	: Cmd(ioctx, manager),
	ackdata({ .type = Cmd::Type::Publish })
{
	(void)params;

	options = Options::Ack;
}

void PublishCmd::ExecuteCmd()
{
	if (!ioctx->GetConnected().load())
		return;

	std::scoped_lock lock(manager.subsmtx);

	std::span<std::byte> topickey(topic.begin(), topic.end());
	auto iter = manager.subscriptions.find(topickey);

	if (iter != manager.subscriptions.end())
	{
		for (auto subscriber : iter->second)
			subscriber->PostSend(msg);

		ackdata.reason = Cmd::ReasonCode::Success;
	}

	ackdata.reason = Cmd::ReasonCode::NoMatchingSubscribers;
}

void PublishCmd::ExecuteAck()
{
	ioctx->PostSend(
		ackbuffer
		.WriteUInt<Cmd::Type>(ackdata.type)
		.WriteUInt<Cmd::ReasonCode>(ackdata.reason)
		.Build()
	);
}

const bool PublishCmd::AckRequired() const noexcept
{
	return (options == Options::Ack) ? true : false;
}
