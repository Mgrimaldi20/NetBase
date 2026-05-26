#include "../../NetBase/src/framework/CmdDispatcher.h"
#include "../../NetBase/src/framework/ChannelManager.h"
#include "../../NetBase/src/framework/Channel.h"

#include "../../NetBase/src/framework/log/Log.h"
#include "../../NetBase/src/framework/log/entry/Entry.h"
#include "../../NetBase/src/framework/log/sink/text/console/ConsoleSink.h"
#include "../../NetBase/src/framework/log/sink/text/file/FileSink.h"
#include "../../NetBase/src/framework/log/formatter/text/basic/BasicTextFormatter.h"
#include "../../NetBase/src/framework/log/policy/trace/StacktracePolicy.h"
#include "../../NetBase/src/framework/log/policy/trace/SourceLocationPolicy.h"

#include "ClientAPIImpl.h"

ClientAPIImpl::ClientAPIImpl(
	std::shared_ptr<NetBaseAPI> netbaseapi,
	std::shared_ptr<ClientAPI::Parser> parser,
	std::string protoname
)
	: netbaseapi(netbaseapi),
	parser(parser),
	protoname(std::move(protoname))
{
	netbaseapi->GetLogger()->SetLogName(this->protoname);

	netbaseapi->GetLogger()->AttachSink(std::make_shared<ConsoleSink>(std::make_unique<BasicTextFormatter>()));

	netbaseapi->GetLogger()->AttachPolicy(std::make_shared<StacktracePolicy>(Entry::Level::Fatal));
	netbaseapi->GetLogger()->AttachPolicy(std::make_shared<SourceLocationPolicy>(Entry::Level::Debug));
}

void ClientAPIImpl::RegisterCmds()
{
	netbaseapi->GetCmdDispatcher()->Register(
		0,
		[this](std::weak_ptr<Client> client, const CmdDispatcher::ParsedCmd &cmd)
		{
			std::shared_ptr<Channel> channel = netbaseapi->GetChannelManager()->Create(cmd.data.data());
			channel->Join(client.lock());
		}
	);

	netbaseapi->GetCmdDispatcher()->Register(
		1,
		[this](std::weak_ptr<Client> client, const CmdDispatcher::ParsedCmd &cmd)
		{
			std::shared_ptr<Channel> channel = netbaseapi->GetChannelManager()->Fetch(cmd.data.data());
			channel->Broadcast("Hello\n");

			client.lock()->Send("Response Message\n");
		}
	);
}

std::shared_ptr<ClientAPI::Parser> ClientAPIImpl::GetParser()
{
	return parser;
}

std::string &ClientAPIImpl::GetProtocolName()
{
	return protoname;
}
