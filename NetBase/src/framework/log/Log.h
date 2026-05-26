#ifndef __NETBASE_FRAMEWORK_LOG_LOG_H__
#define __NETBASE_FRAMEWORK_LOG_LOG_H__

#include <format>
#include <vector>
#include <memory>
#include <string>
#include <source_location>

#include "NetBaseAPI.h"

#include "entry/Entry.h"
#include "sink/Sink.h"
#include "policy/Policy.h"

/*
* Class: Log
* The logging system, controls wiritng to various log sink, created with a default sink.
* Different logging levels are provided to represent the class of information to log.
* The logger will apply attached policies, and send entries to the output sinks for formatting and writing.
* 
*	Debug: Log a formatted debug message, designed for debugging purposes and development, not in in release builds
*	Info: Log a formatted information message, designed for general program information and state
*	Warn: Log a formatted warning message, designed for recoverable issues or abnormal state
*	Error: Log a formatted error message, designed for unrecoverable code errors, program should quit
*	SetName: Sets the name of the logger to the name specified, changes the name
*	AttachSink: Add a log sink to route logs to a particular destination
*	AttachPolicy: Add a log policy to control the log outputs and entries
*/
class NETBASE_API Log
{
public:
	template<typename ...Args>
	struct FormatContext
	{
		template<typename T>
			//requires std::constructible_from<std::format_string<Args...>, T const &>	// intellisense hates this for some reason, all logging calls give false errors
		consteval FormatContext(
			T const &fmt,
			std::source_location loc = std::source_location::current()
		) noexcept
			: fmt(fmt),
			loc(loc)
		{}

		std::format_string<Args...> fmt;
		std::source_location loc;
	};

	Log(
		std::string logname = {},
		std::vector<std::shared_ptr<Sink>> sinks = {},
		std::vector<std::shared_ptr<Policy>> policies = {}
	);

	~Log();

	template<typename ...Args>
	inline void Debug(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Info(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Warn(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Error(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	inline void Fatal(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	void SetLogName(std::string name);

	void AttachSink(std::shared_ptr<Sink> sink);
	void AttachPolicy(std::shared_ptr<Policy> policy);

private:
	void Write(Entry::Level level, std::string msg, std::source_location loc);

	std::string logname;

	std::vector<std::shared_ptr<Sink>> sinks;
	std::vector<std::shared_ptr<Policy>> policies;
};

template<typename ...Args>
inline void Log::Debug(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
#if defined(NETBASE_DEBUG)
	Write(Entry::Level::Debug, std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
#else
	(void)fmt;
	(void)std::initializer_list<int>{((void)args, 0)...};
#endif
}

template<typename ...Args>
inline void Log::Info(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	Write(Entry::Level::Info, std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline void Log::Warn(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	Write(Entry::Level::Warn, std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline void Log::Error(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	Write(Entry::Level::Error, std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline void Log::Fatal(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	Write(Entry::Level::Fatal, std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

#endif
