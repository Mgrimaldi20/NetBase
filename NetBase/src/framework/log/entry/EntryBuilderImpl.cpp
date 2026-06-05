#include "EntryBuilderImpl.h"

EntryBuilderImpl::EntryBuilderImpl(
	std::weak_ptr<Driver> driver,
	std::source_location srcloc,
	std::chrono::system_clock::time_point time
)
	: driver(driver)
{
	location = srcloc;
	timestamp = time;
}

EntryBuilderImpl::~EntryBuilderImpl()
{
	if (std::shared_ptr<Driver> dest = driver.lock())
		dest->Submit(*this);
}

EntryBuilder &EntryBuilderImpl::Name(std::string name)
{
	logname = name;
	return *this;
}

EntryBuilder &EntryBuilderImpl::Level(Entry::Level level)
{
	entrylevel = level;
	return *this;
}

EntryBuilder &EntryBuilderImpl::Message(std::string msg)
{
	message = std::move(msg);
	return *this;
}

EntryBuilder &EntryBuilderImpl::Timestamp(std::chrono::system_clock::time_point time)
{
	timestamp = time;
	return *this;
}

EntryBuilder &EntryBuilderImpl::SourceLocation(std::source_location srcloc)
{
	location = srcloc;
	return *this;
}

EntryBuilder &EntryBuilderImpl::Stacktrace(std::stacktrace trace)
{
	stacktrace = trace;
	return *this;
}

std::string_view EntryBuilderImpl::GetLevelStr(Entry::Level level) const
{
	switch (level)
	{
		case Entry::Level::Debug: return "DEBUG";
		case Entry::Level::Info: return "INFO";
		case Entry::Level::Warn: return "WARN";
		case Entry::Level::Error: return "ERROR";
		case Entry::Level::Fatal: return "FATAL";
		default: return "UNKNOWN";
	}
}
