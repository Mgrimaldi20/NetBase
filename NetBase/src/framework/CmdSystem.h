#ifndef _NETBASE_FRAMEWORK_CMDSYSTEM_H_
#define _NETBASE_FRAMEWORK_CMDSYSTEM_H_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

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
	using CmdHandler = std::function<ByteBuffer(ByteBuffer &)>;

	CmdSystem(Log &log);
	~CmdSystem();

	void RegisterHandler(const std::string &cmdname, CmdHandler handler);
	ByteBuffer ParseCommand(ByteBuffer &incoming);

private:
	std::unordered_map<std::string, CmdHandler> handlers;

	Log &log;
};

#endif
