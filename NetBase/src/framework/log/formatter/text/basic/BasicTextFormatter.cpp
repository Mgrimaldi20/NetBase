#include <chrono>
#include <format>
#include <sstream>
#include <source_location>
#include <stacktrace>

#include "BasicTextFormatter.h"

std::string BasicTextFormatter::Format(const Entry &entry) const
{
	std::chrono::zoned_time localtime(
		std::chrono::current_zone(),
		std::chrono::floor<std::chrono::seconds>(entry.timestamp)
	);

	return std::format(
		"{:%F %H:%M:%S %Z} [{}] [{}] {}{}{}\n",
		localtime,
		entry.logname,
		entry.GetLevelStr(entry.entrylevel),
		entry.message,
		GetSourceLocation(entry),
		GetStacktrace(entry)
	);
}

std::string BasicTextFormatter::GetSourceLocation(const Entry &entry) const
{
	if (!entry.location.has_value() || entry.entrylevel != Entry::Level::Debug)
		return "";

	std::source_location srcloc = entry.location.value();

	return std::format(
		"\n    >> {}\n    >> {}({},{})",
		srcloc.function_name(),
		srcloc.file_name(),
		srcloc.line(),
		srcloc.column()
	);
}

std::string BasicTextFormatter::GetStacktrace(const Entry &entry) const
{
	if (!entry.stacktrace.has_value() || entry.stacktrace.value().empty())
		return "";

	std::stacktrace traces = entry.stacktrace.value();

	std::stringstream sstream;

	sstream << "\n------------------------------------------------------------";

	for (const auto &trace : traces)
	{
		sstream
			<< "\n    >> " << trace.source_file() << "(" << trace.source_line() << ")"
			<< "\n    >> " << trace.description()
			<< "\n------------------------------------------------------------";
	}

	return sstream.str();
}
