#include <format>
#include <chrono>

#include "TextFormatter.h"

std::string TextFormatter::Format(const Entry &entry) const
{
	auto timepoint = std::chrono::floor<std::chrono::seconds>(
		std::chrono::current_zone()->to_local(entry.time)
	);

	return std::format(
		"{} [{}] {}\n",
		timepoint,
		entry.GetLevelStr(entry.level),
		entry.msg
	);
}
