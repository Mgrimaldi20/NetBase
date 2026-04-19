#ifndef _NETBASE_FRAMEWORK_LOG_ENTRY_H_
#define _NETBASE_FRAMEWORK_LOG_ENTRY_H_

#include <chrono>
#include <string>

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
	Entry::Level level;
	std::string msg;
};

#endif
