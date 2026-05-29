#include "EntryBuilder.h"

EntryBuilder::EntryBuilder(std::source_location srcloc, std::chrono::system_clock::time_point time)
	: dest()
{
	location = srcloc;
	timestamp = time;
}

EntryBuilder::~EntryBuilder()
{
	if (!dest.expired())
		dest.lock()->Submit(*this);
}

EntryBuilder &EntryBuilder::Name(std::string name)
{
	logname = std::move(name);
	return *this;
}

EntryBuilder &EntryBuilder::Level(Entry::Level level)
{
	entrylevel = level;
	return *this;
}

EntryBuilder &EntryBuilder::Message(std::string msg)
{
	message = std::move(msg);
	return *this;
}

EntryBuilder &EntryBuilder::Dest(std::weak_ptr<Driver> driver)
{
	dest = driver;
	return *this;
}

EntryBuilder &EntryBuilder::Timestamp(std::chrono::system_clock::time_point time)
{
	timestamp = time;
	return *this;
}

EntryBuilder &EntryBuilder::SourceLocation(std::source_location srcloc)
{
	location = srcloc;
	return *this;
}

EntryBuilder &EntryBuilder::Stacktrace(std::stacktrace trace)
{
	stacktrace = trace;
	return *this;
}

std::string_view EntryBuilder::GetLevelStr(Entry::Level level) const
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
