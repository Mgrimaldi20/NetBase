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
*/
class EntryBuilder : private Entry
{
public:
	EntryBuilder(
		std::source_location srcloc = std::source_location::current(),
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now()
	);

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
