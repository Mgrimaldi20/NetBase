#pragma once

#include "../../NetBase/src/NetBaseAPI.h"

class ParserImpl : public ClientAPI::Parser
{
public:
	ParserImpl() = default;
	virtual ~ParserImpl() = default;

	CmdDispatcher::ParsedCmd Parse(
		std::string_view data,
		std::uint_least64_t length
	) override final;
};
