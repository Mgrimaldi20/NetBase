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

void CmdDispatcher::Register(std::uint_least16_t id, CmdHandlerFn fn)
{
	bool inserted = handlers.emplace(id, fn).second;

	if (!inserted)
		log->Warn("Failed to register CmdHandler");

	log->Info("Registered command with ID: {}", id);
}

asio::awaitable<void> CmdDispatcher::Dispatch(std::shared_ptr<Client> client, ParsedCmd parsedcmd)
{
	CmdHandlerFn handler = handlers.at(parsedcmd.id);

	if (!handler)
		co_return;

	co_await handler(client, parsedcmd);

	co_return;
}
