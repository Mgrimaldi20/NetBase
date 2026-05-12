#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string logname, std::vector<std::shared_ptr<Sink>> sinks)
	: sinks(),
	logname(std::move(logname))
{
	for (const auto &sink : sinks)
		AttachSink(sink);

	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);
}

void Log::AttachSink(std::shared_ptr<Sink> sink)
{
	sinks.push_back(sink);
	Info("Attached sink: {}", sink->GetName());
}

void Log::Write(Entry::Level level, std::string msg)
{
	Entry entry =
	{
		.time = std::chrono::system_clock::now(),
		.logname = logname,
		.level = level,
		.message = std::move(msg)
	};

	for (auto &sink : sinks)
		sink->Write(entry);
}
