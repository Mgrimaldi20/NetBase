#include <unordered_map>

#include "CmdDispatcher.h"

struct CmdDispatcher::Impl
{
	Impl(std::shared_ptr<Log> log)
		: handlers(),
		log(log)
	{}

	~Impl() = default;

	std::unordered_map<std::uint_least16_t, CmdHandlerFn> handlers;

	std::shared_ptr<Log> log;
};

CmdDispatcher::CmdDispatcher(std::shared_ptr<Log> log)
	: pimpl(PImplPtr<CmdDispatcher::Impl>::MakePImpl(log))
{
	pimpl->log->Info("Command Dispatcher started");
}

CmdDispatcher::~CmdDispatcher()
{
	pimpl->log->Info("Shutting down the Command Dispatcher");
}

void CmdDispatcher::Register(std::uint_least16_t cmdid, CmdHandlerFn fn)
{
	bool inserted = pimpl->handlers.emplace(cmdid, fn).second;

	if (!inserted)
		pimpl->log->Warn("Failed to register CmdHandler");

	pimpl->log->Info("Registered command with ID: {}", cmdid);
}

void CmdDispatcher::Register(std::initializer_list<std::pair<std::uint_least16_t, CmdHandlerFn>> elems)
{
	for (auto &[key, val] : elems)
		Register(key, val);
}

void CmdDispatcher::Dispatch(std::weak_ptr<Client> client, ClientAPI::Parser::ParsedCmd parsedcmd)
{
	CmdHandlerFn handler = pimpl->handlers.at(parsedcmd.cmdid);

	if (!handler)
		return;

	handler(client, parsedcmd);
}
