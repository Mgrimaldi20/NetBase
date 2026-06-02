#pragma once

#include <string>
#include <memory>

#include "../../NetBase/src/NetBaseAPI.h"

#include "../../NetBase/src/framework/CmdDispatcher.h"
#include "../../NetBase/src/framework/ChannelManager.h"
#include "../../NetBase/src/framework/Channel.h"
#include "../../NetBase/src/framework/PImplPtr.h"

#include "../../NetBase/src/framework/log/Log.h"
#include "../../NetBase/src/framework/log/driver/Driver.h"
#include "../../NetBase/src/framework/log/entry/Entry.h"
#include "../../NetBase/src/framework/log/entry/EntryBuilder.h"
#include "../../NetBase/src/framework/log/sink/text/console/ConsoleSink.h"
#include "../../NetBase/src/framework/log/sink/text/file/FileSink.h"
#include "../../NetBase/src/framework/log/formatter/text/basic/BasicTextFormatter.h"
#include "../../NetBase/src/framework/log/policy/trace/StacktracePolicy.h"
#include "../../NetBase/src/framework/log/policy/trace/SourceLocationPolicy.h"

class ClientAPIImpl : public ClientAPI
{
public:
	ClientAPIImpl(NetBaseAPI *netbaseapi, std::string protoname);

	virtual ~ClientAPIImpl() = default;

	void RegisterCmds() override final;
	ClientAPI::Parser &GetParser() override final;
	std::string &GetProtocolName() override final;

private:
	NetBaseAPI *netbaseapi;

	std::shared_ptr<ClientAPI::Parser> parser;
	std::string protoname;
};
