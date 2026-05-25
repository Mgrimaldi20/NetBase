#include <charconv>
#include <string_view>
#include <system_error>
#include <format>

#include "ParserImpl.h"

ClientAPI::Parser::ParsedCmd ParserImpl::Parse(
	std::string_view data,
	std::uint_least64_t length
)
{
	// just something silly to stop the warnings... do whatever you need to here
	if (length == 0)
		throw std::runtime_error("Length of command is 0");

	if (data.empty())
		throw std::runtime_error("Command is null");

	Parser::ParsedCmd cmd =
	{
		.cmdid = (std::uint_least16_t)-1,
		.data = data.substr(1)
	};

	std::from_chars_result result = std::from_chars(
		data.data() + 1,
		data.data() + data.size(),
		cmd.cmdid
	);

	if (result.ec != std::errc())
		throw std::runtime_error(std::format("Could not parse command :: {}", data));

	return cmd;
}
