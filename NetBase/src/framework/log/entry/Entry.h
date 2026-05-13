#ifndef __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRY_ENTRY_H__
#define __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRY_ENTRY_H__

#include <chrono>
#include <string>
#include <source_location>

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
		Error
	};

	std::string_view GetLevelStr(Entry::Level entrylevel) const;

	std::source_location srcloc;
	std::chrono::system_clock::time_point time;
	std::string logname;
	Entry::Level level;
	std::string message;
};

#endif
