#ifndef __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDERIMPL_H__
#define __NETBASE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDERIMPL_H__

#include <memory>

#include "../driver/Driver.h"

#include "EntryBuilder.h"
#include "Entry.h"

/*
* Class: EntryBuilderImpl
* Extends EntryBuilder and a log Entry class to implement the GetLevelStr function and provide a fluid interface.
* 
*	Name: Sets the log name
*	Level: Sets the log level from Entry::Level
*	Message: Sets the log full message string
*	Timestamp: Sets the timestamp of the log message, defaulted
*	SourceLocation: Sets the source location, defaulted
*	Stacktrace: Sets the stacktrace of the log message
*/
class EntryBuilderImpl : public EntryBuilder, private Entry
{
public:
	EntryBuilderImpl(
		std::weak_ptr<Driver> driver,
		std::source_location srcloc = std::source_location::current(),
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now()
	);

	EntryBuilderImpl(const EntryBuilderImpl &) = delete;
	EntryBuilderImpl &operator=(const EntryBuilderImpl &) = delete;

	EntryBuilderImpl(EntryBuilderImpl &&) noexcept = default;
	EntryBuilderImpl &operator=(EntryBuilderImpl &&) noexcept = default;

	virtual ~EntryBuilderImpl();

	EntryBuilder &Name(std::string name) override final;
	EntryBuilder &Level(Entry::Level level) override final;
	EntryBuilder &Message(std::string msg) override final;

	EntryBuilder &Timestamp(std::chrono::system_clock::time_point time) override final;
	EntryBuilder &SourceLocation(std::source_location srcloc) override final;
	EntryBuilder &Stacktrace(std::stacktrace trace) override final;

private:
	std::string_view GetLevelStr(Entry::Level level) const override final;

	std::weak_ptr<Driver> driver;
};

#endif
