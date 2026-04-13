#ifndef _NETBASE_FRAMEWORK_CMDDISPATCHER_H_
#define _NETBASE_FRAMEWORK_CMDDISPATCHER_H_

#include <cstdint>
#include <memory.h>
#include <unordered_map>
#include <functional>
#include <string_view>

#include "Asio.h"
#include "Client.h"
#include "Log.h"

struct ParsedCmd
{
	std::uint_least16_t id;
	std::string_view data;
};

class CmdDispatcher
{
public:
	using CmdHandlerFn = std::function<asio::awaitable<void>(std::shared_ptr<Client>, const ParsedCmd &)>;

	CmdDispatcher(std::shared_ptr<Log> log);
	~CmdDispatcher();

	void Register(std::uint_least16_t id, CmdHandlerFn fn);

	asio::awaitable<void> Dispatch(std::shared_ptr<Client> client, ParsedCmd parsedcmd);

private:
	std::unordered_map<std::uint_least16_t, CmdHandlerFn> handlers;

	std::shared_ptr<Log> log;
};

#endif
