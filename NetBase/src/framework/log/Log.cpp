#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string_view logname, std::vector<std::shared_ptr<Sink>> sinks)
	: sinks(std::move(sinks)),
	logname(logname)
{
	for (const auto &sink : this->sinks)
		Info("Attached sink: {} to Logger: {}", sink->GetName(), logname);

	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);
}

void Log::AddSink(std::shared_ptr<Sink> sink)
{
	sinks.push_back(std::move(sink));
	auto it = std::prev(sinks.end());

	Info("Attached logger sink: {}", it->get()->GetName());
}

void Log::Write(Entry::Level level, std::string_view msg)
{
	Entry entry =
	{
		.time = std::chrono::system_clock::now(),
		.logname = logname,
		.level = level,
		.message = std::string(msg)
	};

	for (auto &sink : sinks)
		sink->Write(entry);
}
