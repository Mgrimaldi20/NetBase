#pragma once

#include <string>
#include <memory>

#include "../../NetBase/src/NetBaseAPI.h"

class ClientAPIImpl : public ClientAPI
{
public:
	ClientAPIImpl(
		std::shared_ptr<NetBaseAPI> netbaseapi,
		std::shared_ptr<ClientAPI::Parser> parser,
		std::string protoname
	);

	virtual ~ClientAPIImpl() = default;

	void RegisterCmds() override final;
	std::shared_ptr<ClientAPI::Parser> GetParser() override final;
	std::string &GetProtocolName() override final;

private:
	std::shared_ptr<NetBaseAPI> netbaseapi;
	std::shared_ptr<ClientAPI::Parser> parser;

	std::string protoname;
};
