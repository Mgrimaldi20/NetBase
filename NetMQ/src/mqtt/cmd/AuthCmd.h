#ifndef _NETMQ_AUTHCMD_H_
#define _NETMQ_AUTHCMD_H_

#include "framework/ByteBuffer.h"
#include "framework/Cmd.h"

class AuthCmd : public Cmd
{
public:
	AuthCmd(std::shared_ptr<IOContext> ioctx, SubManager &manager, ByteBuffer &params);
	virtual ~AuthCmd() = default;

private:
	void ExecuteCmd() override final;
	void ExecuteAck() override final;

	const bool AckRequired() const noexcept override final;
};

#endif
