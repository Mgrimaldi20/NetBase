#ifndef __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__
#define __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__

#include "Entry.h"

/*
* Class: EntryBuilder
* Interface for the entry builder, constructs the entry fluidly and should forward to a driver.
* 
*	Name: Sets the name of the log message
*	Level: Sets the log level of the entry
*	Message: The message to write to the driver
*	Timestamp: The timestamp of the message
*	SourceLocation: The source code location where the message originates
*	Stacktrace: The stack trace of the log message
*/
class EntryBuilder
{
public:
	EntryBuilder() = default;
	virtual ~EntryBuilder() = default;

	virtual EntryBuilder &Name(std::string name) = 0;
	virtual EntryBuilder &Level(Entry::Level level) = 0;
	virtual EntryBuilder &Message(std::string msg) = 0;

	virtual EntryBuilder &Timestamp(std::chrono::system_clock::time_point time) = 0;
	virtual EntryBuilder &SourceLocation(std::source_location srcloc) = 0;
	virtual EntryBuilder &Stacktrace(std::stacktrace trace) = 0;
};

#endif
