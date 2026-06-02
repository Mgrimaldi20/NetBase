#pragma once

#include "../../NetBase/src/NetBaseAPI.h"

class ParserImpl : public ClientAPI::Parser
{
public:
	ParserImpl() = default;
	virtual ~ParserImpl() = default;

	ClientAPI::Parser::ParsedCmd Parse(std::string_view data) override final;
};
