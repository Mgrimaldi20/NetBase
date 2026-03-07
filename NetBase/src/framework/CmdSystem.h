#ifndef _NETBASE_FRAMEWORK_CMDSYSTEM_H_
#define _NETBASE_FRAMEWORK_CMDSYSTEM_H_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>
#include <vector>

#include "Log.h"
#include "ByteBuffer.h"

#include "sys/win32/io/IOContext.h"

/*
* Class: CmdSystem
* Handles the parsing and the creation of command objects from incoming data packets.
* Each command object encapsulates the logic for executing specific commands and acts upon a context.
* 
*	RegisterHandler: Registers a handler function for a specific command, allows multiple handlers for the same command type (for acks, etc.)
*	ParseCommand: Parses incoming data to identify the command type and creates the corresponding command object
*/
class CmdSystem
{
public:
	// the first ByteBuffer is the response to send back to the sender
	// the second ByteBuffer is the message to broadcast to other clients
	// the vector is the list of client ids to broadcast to
	using CmdResult = std::pair<ByteBuffer, std::pair<ByteBuffer, std::vector<std::string>>>;
	using CmdHandler = std::function<CmdResult(ByteBuffer &)>;

	CmdSystem(Log &log);
	~CmdSystem();

	void RegisterHandler(const std::string &cmdname, CmdSystem::CmdHandler handler);
	CmdSystem::CmdResult ParseCommand(ByteBuffer &incoming);

private:
	std::unordered_map<std::string, CmdSystem::CmdHandler> handlers;

	Log &log;
};

#endif
