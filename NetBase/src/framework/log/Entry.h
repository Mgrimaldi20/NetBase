#ifndef _NETBASE_FRAMEWORK_LOG_ENTRY_H_
#define _NETBASE_FRAMEWORK_LOG_ENTRY_H_

#include <chrono>
#include <string>

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

	std::chrono::system_clock::time_point time;
	std::string logname;
	Entry::Level level;
	std::string message;
};

#endif
