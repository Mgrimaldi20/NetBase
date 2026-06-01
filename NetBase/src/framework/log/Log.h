#ifndef __NETBASE_FRAMEWORK_LOG_LOG_H__
#define __NETBASE_FRAMEWORK_LOG_LOG_H__

#include <format>
#include <vector>
#include <memory>
#include <string>
#include <source_location>
#include <type_traits>
#include <utility>

#include "NetBaseAPI.h"

#include "../PImplPtr.h"

#include "entry/Entry.h"
#include "entry/EntryBuilder.h"
#include "driver/Driver.h"

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
*	AttachDriver: Swaps out the current driver config with a new one
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

	Log(std::string logname = {}, std::shared_ptr<Driver> driver = {});

	~Log();

	EntryBuilder Debug(std::string msg, std::source_location loc = std::source_location::current());
	EntryBuilder Info(std::string msg, std::source_location loc = std::source_location::current());
	EntryBuilder Warn(std::string msg, std::source_location loc = std::source_location::current());
	EntryBuilder Error(std::string msg, std::source_location loc = std::source_location::current());
	EntryBuilder Fatal(std::string msg, std::source_location loc = std::source_location::current());

	template<typename ...Args>
	EntryBuilder Debug(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	EntryBuilder Info(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	EntryBuilder Warn(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	EntryBuilder Error(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	template<typename ...Args>
	EntryBuilder Fatal(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args);

	void SetLogName(std::string name);
	void AttachDriver(std::shared_ptr<Driver> newdriver);

private:
	struct Impl;
	PImplPtr<Impl> pimpl;
};

template<typename ...Args>
inline EntryBuilder Log::Debug(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
#if defined(NETBASE_DEBUG)
	return Debug(std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
#else
	(void)fmt;
	(void)std::initializer_list<int>{((void)args, 0)...};

	return {};
#endif
}

template<typename ...Args>
inline EntryBuilder Log::Info(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	return Info(std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline EntryBuilder Log::Warn(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	return Warn(std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline EntryBuilder Log::Error(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	return Error(std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

template<typename ...Args>
inline EntryBuilder Log::Fatal(Log::FormatContext<std::type_identity_t<Args>...> fmt, Args && ...args)
{
	return Fatal(std::format(fmt.fmt, std::forward<Args>(args)...), fmt.loc);
}

#endif
