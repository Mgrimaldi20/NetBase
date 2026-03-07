#include "CmdSystem.h"

CmdSystem::CmdSystem(Log &log)
	: log(log)
{
	log.Info("Command System started");
}

CmdSystem::~CmdSystem()
{
	log.Info("Shutting down the Command System");
}

void CmdSystem::RegisterHandler(const std::string &cmdname, CmdSystem::CmdHandler handler)
{
	bool res = handlers.insert_or_assign(cmdname, handler).second;
	if (!res)
		log.Warn("Handler for command '{}' was replaced", cmdname);

	else
		log.Info("Registered handler for command '{}'", cmdname);
}

CmdSystem::CmdResult CmdSystem::ParseCommand(ByteBuffer &incoming)
{
	std::string cmdname = incoming.ReadString(4);

	auto it = handlers.find(cmdname);
	if (it == handlers.end())
	{
		log.Warn("Received unknown command: {}", cmdname);
		return CmdResult();
	}

	return it->second(incoming);
}
