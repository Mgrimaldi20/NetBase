#ifndef __NETBASE_FRAMEWORK_CMDDISPATCHER_H__
#define __NETBASE_FRAMEWORK_CMDDISPATCHER_H__

#include <cstdint>
#include <memory>
#include <functional>
#include <string_view>
#include <initializer_list>
#include <utility>

#include "NetBaseAPI.h"

#include "Client.h"
#include "PImplPtr.h"

#include "log/Log.h"

/*
* Class: CmdDispatcher
* Responsible for registering user defined commands, and dispatching/calling parsed commands.
* Contains the ParsedCmd struct which is created by the client parser interface.
* A command handler is a function object that can store any type of functor, function, lambda object.
* The handler function will give the protocol access to the client which called it, and the parsed data.
*
*	Register: Registers a command handler within the system by its id
*	Dispatch: Calls the registered handlers by id using the client as context
*/
class NETBASE_API CmdDispatcher
{
public:
	struct ParsedCmd
	{
		std::uint_least16_t cmdid;
		std::string_view data;

		static CmdDispatcher::ParsedCmd Map(ClientAPI::Parser::ParsedCmd parsedcmd);
	};

	using CmdHandlerFn = std::function<void(std::weak_ptr<Client>, const CmdDispatcher::ParsedCmd &)>;

	CmdDispatcher(std::shared_ptr<Log> log);
	~CmdDispatcher();

	void Register(std::uint_least16_t cmdid, CmdHandlerFn fn);
	void Register(std::initializer_list<std::pair<std::uint_least16_t, CmdHandlerFn>> elems);

	void Dispatch(std::weak_ptr<Client> client, CmdDispatcher::ParsedCmd parsedcmd);

private:
	struct Impl;
	PImplPtr<Impl> pimpl;
};

#endif
