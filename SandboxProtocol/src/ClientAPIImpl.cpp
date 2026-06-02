#include <system_error>

#include "ParserImpl.h"

#include "ClientAPIImpl.h"

ClientAPIImpl::ClientAPIImpl(NetBaseAPI *netbaseapi, std::string protoname)
	: netbaseapi(netbaseapi),
	parser(std::make_shared<ParserImpl>()),
	protoname(std::move(protoname))
{
	if (!netbaseapi)
		throw std::runtime_error("NetBaseAPI is null");

	Log &log = netbaseapi->GetLogger();

	log.SetLogName(this->protoname);

	log.AttachDriver(
		std::make_shared<Driver>(
			"SandboxProtocolMainDriver",
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
	CmdDispatcher &dispatcher = netbaseapi->GetCmdDispatcher();
	ChannelManager &chmanager = netbaseapi->GetChannelManager();

	dispatcher.Register(
		0,
		[this, &chmanager](std::weak_ptr<Client> client, const ClientAPI::Parser::ParsedCmd &cmd)
		{
			std::string channelname = std::string(cmd.data);
			std::shared_ptr<Channel> channel = chmanager.Create(channelname);
			channel->Join(client.lock());
		}
	);

	dispatcher.Register(
		1,
		[this, &chmanager](std::weak_ptr<Client> client, const ClientAPI::Parser::ParsedCmd &cmd)
		{
			std::string channelname = std::string(cmd.data);
			std::shared_ptr<Channel> channel = chmanager.Fetch(channelname);
			channel->Broadcast("Hello\n");

			client.lock()->Send("Response Message\n");
		}
	);
}

ClientAPI::Parser &ClientAPIImpl::GetParser()
{
	return *parser;
}

std::string &ClientAPIImpl::GetProtocolName()
{
	return protoname;
}
