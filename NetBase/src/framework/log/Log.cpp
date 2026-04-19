#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string_view logname, std::unique_ptr<Sink> sink)
	: sinks(),
	logname(logname),
	logmtx()
{
	AddSink(std::move(sink));
	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);

	for (auto &sink : sinks)
		Debug("\t- Attached sink: {}", sink->GetName());
}

void Log::AddSink(std::unique_ptr<Sink> sink)
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
		.level = level,
		.msg = std::string(msg)
	};

	std::scoped_lock lock(logmtx);

	for (auto &sink : sinks)
		sink->Write(entry);
}
