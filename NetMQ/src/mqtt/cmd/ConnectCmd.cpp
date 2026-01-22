#include <stdexcept>
#include <algorithm>
#include <format>
#include <array>
#include <unordered_set>

#include "framework/UUID.h"

#include "ConnectCmd.h"

ConnectCmd::ConnectCmd(std::shared_ptr<IOContext> ioctx, SubManager &manager, ByteBuffer &params)
	: Cmd(ioctx, manager),
	ackdata({ .type = Cmd::Type::ConnAck })
{
	static constexpr std::string_view HEADER_STRING = "MQTT";

	uintmax_t remaininglen = params.ReadVarUInt();
	if (remaininglen != params.Size())
		ackdata.reason = Cmd::ReasonCode::MalformedPacket;

	uint16_t headerlen = params.ReadUInt<uint16_t>();
	std::string header = params.ReadString(headerlen);
	if (header != HEADER_STRING)
		ackdata.reason = Cmd::ReasonCode::UnsupportedProtocolVersion;

	std::byte version = params.ReadUInt<std::byte>();
	if (version != std::byte(MQTT_SPEC_VERSION))
		ackdata.reason = Cmd::ReasonCode::UnsupportedProtocolVersion;

	Flags flags = params.ReadUInt<Flags>();
	if (Bitmask::HasFlag(flags, Flags::Reserved))
		ackdata.reason = Cmd::ReasonCode::ProtocolError;

	if (Bitmask::HasFlag(flags, Flags::CleanStart))
	{
	}

	if (Bitmask::HasFlag(flags, Flags::WillFlag))
	{
	}

	if (Bitmask::HasFlag(flags, Flags::WillQoS1) || Bitmask::HasFlag(flags, Flags::WillQoS2))
	{
		if (!Bitmask::HasFlag(flags, Flags::WillFlag))
			ackdata.reason = Cmd::ReasonCode::ProtocolError;
	}

	if (Bitmask::HasFlag(flags, Flags::WillRetain))
	{
	}

	if (Bitmask::HasFlag(flags, Flags::PasswordFlag))
	{
	}

	if (Bitmask::HasFlag(flags, Flags::UsernameFlag))
	{
	}

	uint16_t keepalive = params.ReadUInt<uint16_t>();

	uintmax_t propertieslen = params.ReadVarUInt();
	uintmax_t propertiesread = 0;
	while (propertiesread < propertieslen)
	{
		Cmd::Property prop = params.ReadUInt<Cmd::Property>();
		propertiesread += sizeof(prop);

		switch (prop)
		{
			case Cmd::Property::SessionExpiryInterval:
			{
				uint32_t interval = params.ReadUInt<uint32_t>();
				propertiesread += sizeof(interval);
				break;
			}

			case Cmd::Property::ReceiveMaximum:
			{
				uint16_t recvmax = params.ReadUInt<uint16_t>();
				propertiesread += sizeof(recvmax);
				break;
			}

			case Cmd::Property::MaximumPacketSize:
			{
				uint32_t maxpacketsize = params.ReadUInt<uint32_t>();
				propertiesread += sizeof(maxpacketsize);
				break;
			}

			case Cmd::Property::TopicAliasMaximum:
			{
				uint16_t topicaliasmax = params.ReadUInt<uint16_t>();
				propertiesread += sizeof(topicaliasmax);
				break;
			}

			case Cmd::Property::RequestResponseInformation:
			{
				std::byte reqrespinfo = params.ReadUInt<std::byte>();
				propertiesread += sizeof(reqrespinfo);
				break;
			}

			case Cmd::Property::RequestProblemInformation:
			{
				std::byte reqprobinfo = params.ReadUInt<std::byte>();
				propertiesread += sizeof(reqprobinfo);
				break;
			}

			case Cmd::Property::UserProperty:
				break;

			case Cmd::Property::AuthenticationMethod:
				break;

			case Cmd::Property::AuthenticationData:
				break;

			default:
				ackdata.reason = Cmd::ReasonCode::MalformedPacket;
				break;
		}
	}
}

void ConnectCmd::ExecuteCmd()
{
	if (ioctx->GetConnected().load())
		return;

	ioctx->SetClientID(clientid);
	ioctx->SetConnected(true);

	ackdata.reason = Cmd::ReasonCode::Success;
}

void ConnectCmd::ExecuteAck()
{
	ioctx->PostSend(
		ackbuffer
		.WriteUInt<Cmd::Type>(ackdata.type)
		.WriteUInt<Cmd::ReasonCode>(ackdata.reason)
		.Build()
	);
}
