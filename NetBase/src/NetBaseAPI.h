#ifndef _NETBASE_NETBASEAPI_H_
#define _NETBASE_NETBASEAPI_H_

#include <cstdint>

class NetBaseAPI
{
public:
	NetBaseAPI() = default;
	virtual ~NetBaseAPI() = default;
};

class Parser
{
public:
	struct ParsedCmd
	{
		std::uint_least16_t cmdid;
		std::uint_least64_t length;
		char *data;
	};

	virtual Parser::ParsedCmd operator()() = 0;

	Parser() = default;
	virtual ~Parser() = default;
};

#endif
