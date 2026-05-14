#include <chrono>
#include <format>
#include <source_location>

#include "BasicTextFormatter.h"

std::string BasicTextFormatter::Format(const Entry &entry) const
{
	std::chrono::zoned_time localtime(
		std::chrono::current_zone(),
		std::chrono::floor<std::chrono::seconds>(entry.time)
	);

	const std::string dbgfmt = [](
		const std::source_location &srcloc,
		const Entry::Level &level
	) -> std::string
	{
		if (level != Entry::Level::Debug)
			return "";

		return std::format(
			"\n    >> {}\n    >> {}({},{})",
			srcloc.function_name(),
			srcloc.file_name(),
			srcloc.line(),
			srcloc.column()
		);
	}(entry.srcloc, entry.level);

	return std::format(
		"{:%F %H:%M:%S %Z} [{}] [{}] {}{}\n",
		localtime,
		entry.logname,
		entry.GetLevelStr(entry.level),
		entry.message,
		dbgfmt
	);
}
