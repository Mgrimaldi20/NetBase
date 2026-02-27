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

void CmdSystem::RegisterHandler(const std::string &cmdname, CmdHandler handler)
{
	handlers[cmdname].emplace_back(std::move(handler));
}

void CmdSystem::ParseCommand(std::shared_ptr<IOContext> ioctx, ByteBuffer &incoming)
{
	std::string cmdname = incoming.ReadString(4);
	auto it = handlers.find(cmdname);
	if (it == handlers.end())
	{
		log.Warn("Received unknown command: {}", cmdname);
		return;
	}

	for (const CmdHandler &handler : it->second)
		handler(ioctx, incoming);
}
