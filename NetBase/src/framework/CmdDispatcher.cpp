#include "CmdDispatcher.h"

CmdDispatcher::CmdDispatcher(std::shared_ptr<Log> log)
	: log(log)
{
	log->Info("Command Dispatcher started");
}

CmdDispatcher::~CmdDispatcher()
{
	log->Info("Shutting down the Command Dispatcher");
}

void CmdDispatcher::Register(std::uint_least16_t cmdid, CmdHandlerFn fn)
{
	bool inserted = handlers.emplace(cmdid, fn).second;

	if (!inserted)
		log->Warn("Failed to register CmdHandler");

	log->Info("Registered command with ID: {}", cmdid);
}

void CmdDispatcher::Register(std::initializer_list<std::pair<std::uint_least16_t, CmdHandlerFn>> elems)
{
	for (auto &[key, val] : elems)
		Register(key, val);
}

void CmdDispatcher::Dispatch(std::weak_ptr<Client> client, CmdDispatcher::ParsedCmd parsedcmd)
{
	CmdHandlerFn handler = handlers.at(parsedcmd.cmdid);

	if (!handler)
		return;

	handler(client, parsedcmd);
}

CmdDispatcher::ParsedCmd CmdDispatcher::ParsedCmd::Map(ClientAPI::Parser::ParsedCmd parsedcmd)
{
	return
	{
		.cmdid = parsedcmd.cmdid,
		.data = parsedcmd.data
	};
}
