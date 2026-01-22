#include "PingReqCmd.h"

PingReqCmd::PingReqCmd(std::shared_ptr<IOContext> ioctx, SubManager &manager, ByteBuffer &params)
	: Cmd(ioctx, manager),
	ackdata({ .type = Cmd::Type::PingResp, .remaininglen = 0 })
{
	(void)params;
}

void PingReqCmd::ExecuteCmd()
{
	if (!ioctx->GetConnected().load())
		return;
}

void PingReqCmd::ExecuteAck()
{
	ioctx->PostSend(
		ackbuffer
		.WriteUInt<Cmd::Type>(ackdata.type)
		.WriteUInt<uint8_t>(ackdata.remaininglen)
		.Build()
	);
}
