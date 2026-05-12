#include <chrono>
#include <format>

#include "TextFormatter.h"

std::string TextFormatter::Format(const Entry &entry) const
{
	std::chrono::zoned_time localtime(
		std::chrono::current_zone(),
		std::chrono::floor<std::chrono::seconds>(entry.time)
	);

	return std::format(
		"{:%F %H:%M:%S %Z} [{}] [{}] {}\n",
		localtime,
		entry.logname,
		entry.GetLevelStr(entry.level),
		entry.message
	);
}
