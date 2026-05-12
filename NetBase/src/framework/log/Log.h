#ifndef __NETBASE_FRAMEWORK_LOG_LOG_H__
#define __NETBASE_FRAMEWORK_LOG_LOG_H__

#include <format>
#include <vector>
#include <memory>
#include <string>

#include "entry/Entry.h"
#include "sink/Sink.h"

/*
* Class: Log
* The logging system, controls wiritng to various log sink, created with a default sink.
* Different logging levels are provided to represent the class of information to log.
* The logger will print the current time, level, and message.
* 
*	Debug: Log a formatted debug message, designed for debugging purposes and development, not in in release builds
*	Info: Log a formatted information message, designed for general program information and state
*	Warn: Log a formatted warning message, designed for recoverable issues or abnormal state
*	Error: Log a formatted error message, designed for unrecoverable code errors, program should quit
*	AttachSink: Add a log sink to route logs to a particular destination
*/
class Log
{
public:
	Log(std::string logname, std::vector<std::shared_ptr<Sink>> sinks = {});
	~Log();

	template<typename ...Args>
	inline void Debug(std::format_string<Args...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Info(std::format_string<Args...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Warn(std::format_string<Args...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Error(std::format_string<Args...> fmt, Args && ...args);

	void AttachSink(std::shared_ptr<Sink> sink);

private:
	void Write(Entry::Level level, std::string msg);

	std::vector<std::shared_ptr<Sink>> sinks;
	std::string logname;
};

template<typename ...Args>
inline void Log::Debug(std::format_string<Args...> fmt, Args && ...args)
{
#if defined(NETBASE_DEBUG)
	Write(Entry::Level::Debug, std::format(fmt, std::forward<Args>(args)...));
#else
	(void)fmt;
	(void)std::initializer_list<int>{((void)args, 0)...};
#endif
}

template<typename ...Args>
inline void Log::Info(std::format_string<Args...> fmt, Args && ...args)
{
	Write(Entry::Level::Info, std::format(fmt, std::forward<Args>(args)...));
}

template<typename ...Args>
inline void Log::Warn(std::format_string<Args...> fmt, Args && ...args)
{
	Write(Entry::Level::Warn, std::format(fmt, std::forward<Args>(args)...));
}

template<typename ...Args>
inline void Log::Error(std::format_string<Args...> fmt, Args && ...args)
{
	Write(Entry::Level::Error, std::format(fmt, std::forward<Args>(args)...));
}

#endif
