#include <charconv>
#include <string_view>
#include <system_error>
#include <format>

#include "ParserImpl.h"

ClientAPI::Parser::ParsedCmd ParserImpl::Parse(std::string_view data, std::uint_least64_t length)
{
	// just something silly to stop the warnings... do whatever you need to here
	if (length == 0 || data.empty())
		throw std::runtime_error("Command is empty");

	while (!data.empty() && (data.back() == '\n' || data.back() == '\r'))
		data.remove_suffix(1);

	if (data.empty())
		throw std::runtime_error("Command contains only newlines");

	Parser::ParsedCmd cmd =
	{
		.cmdid = static_cast<std::uint_least16_t>(-1),
		.data = ""
	};

	std::from_chars_result result = std::from_chars(
		data.data(),
		data.data() + data.size(),
		cmd.cmdid
	);

	if (result.ec != std::errc())
		throw std::runtime_error(std::format("Could not parse command :: {}", data));

	std::size_t parsed = result.ptr - data.data();
	if (parsed < data.size())
	{
		std::string_view payload = data.substr(parsed);

		if (!payload.empty() && payload.front() == ' ')
			payload.remove_prefix(1);

		cmd.data = payload;
	}

	return cmd;
}
