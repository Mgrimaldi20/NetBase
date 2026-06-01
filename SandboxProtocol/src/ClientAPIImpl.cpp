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

	netbaseapi->GetLogger()->AttachDriver(
		std::make_shared<Driver>(
			"ClientAPIMainDriver",
			std::vector<std::shared_ptr<Sink>>
			{
				std::make_shared<ConsoleSink>(std::make_unique<BasicTextFormatter>())
			},
			std::vector<std::shared_ptr<Policy>>
			{
				std::make_shared<StacktracePolicy>(Entry::Level::Fatal),
				std::make_shared<SourceLocationPolicy>(Entry::Level::Debug)
			}
		)
	);
}

void ClientAPIImpl::RegisterCmds()
{
	netbaseapi->GetCmdDispatcher()->Register(
		0,
		[this](std::weak_ptr<Client> client, const CmdDispatcher::ParsedCmd &cmd)
		{
			std::string channelname = std::string(cmd.data);
			std::shared_ptr<Channel> channel = netbaseapi->GetChannelManager()->Create(channelname);
			channel->Join(client.lock());
		}
	);

	netbaseapi->GetCmdDispatcher()->Register(
		1,
		[this](std::weak_ptr<Client> client, const CmdDispatcher::ParsedCmd &cmd)
		{
			std::string channelname = std::string(cmd.data);
			std::shared_ptr<Channel> channel = netbaseapi->GetChannelManager()->Fetch(channelname);
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
