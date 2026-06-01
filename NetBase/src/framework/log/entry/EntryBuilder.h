#ifndef __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__
#define __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__

#include <memory>

#include "../driver/Driver.h"

#include "Entry.h"

/*
* Class: EntryBuilder
* Extends a private log Entry class to implement the GetLevelStr function and provide a fluid interface.
* 
*	Name: Sets the log name
*	Level: Sets the log level from Entry::Level
*	Message: Sets the log full message string
*	Dest: Sets the log driver to the supplied driver config
*	Timestamp: Sets the timestamp of the log message, defaulted
*	SourceLocation: Sets the source location, defaulted
*	Stacktrace: Sets the stacktrace of the log message
*/
class EntryBuilder : private Entry
{
public:
	EntryBuilder(
		std::source_location srcloc = std::source_location::current(),
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now()
	);

	EntryBuilder(const EntryBuilder &) = delete;
	EntryBuilder &operator=(const EntryBuilder &) = delete;

	EntryBuilder(EntryBuilder &&) noexcept = default;
	EntryBuilder &operator=(EntryBuilder &&) noexcept = default;

	virtual ~EntryBuilder();

	EntryBuilder &Name(std::string name);
	EntryBuilder &Level(Entry::Level level);
	EntryBuilder &Message(std::string msg);
	EntryBuilder &Dest(std::weak_ptr<Driver> driver);

	EntryBuilder &Timestamp(std::chrono::system_clock::time_point time);
	EntryBuilder &SourceLocation(std::source_location srcloc);
	EntryBuilder &Stacktrace(std::stacktrace trace);

private:
	std::string_view GetLevelStr(Entry::Level level) const override final;

	std::weak_ptr<Driver> dest;
};

#endif
