#ifndef __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRY_ENTRY_H__
#define __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRY_ENTRY_H__

#include <string>
#include <string_view>
#include <chrono>
#include <optional>
#include <source_location>
#include <stacktrace>

/*
* Struct: Entry
* Represents a log entry, contains most information needed for enough context.
* Contains a Level enum class to set the severity level of the log.
* 
*	GetLevelStr: Takes in an entry level enum and returns a string representation
*/
struct Entry
{
	enum class Level
	{
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

	Entry()
		: entrylevel(Entry::Level::Error)
	{}

	virtual ~Entry() = default;

	virtual std::string_view GetLevelStr(Entry::Level entrylevel) const = 0;

	std::chrono::system_clock::time_point timestamp;
	std::string logname;
	Entry::Level entrylevel;
	std::string message;
	std::optional<std::source_location> location;
	std::optional<std::stacktrace> stacktrace;
};

#endif
