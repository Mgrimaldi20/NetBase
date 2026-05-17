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
			channel->Broadcast(std::make_shared<std::string>("Hello\n"));

			client.lock()->Send(std::make_shared<std::string>("Response Message\n"));
		}
	);
}

std::shared_ptr<ClientAPI::Parser> ClientAPIImpl::GetParser()
{
	return parser;
}

std::string ClientAPIImpl::GetProtocolName()
{
	return protoname;
}
