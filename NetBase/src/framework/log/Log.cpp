#include <chrono>
#include <string>
#include <iterator>

#include "Log.h"

Log::Log(std::string logname, std::unique_ptr<Sink> sink)
	: sinks(),
	logname(std::move(logname)),
	logmtx()
{
	AddSink(std::move(sink));
	Info("Logger opened: {}", this->logname);
}

Log::~Log()
{
	Info("Closing Logger: {}", logname);
}

void Log::AddSink(std::unique_ptr<Sink> sink)
{
	sinks.push_back(std::move(sink));
	auto it = std::prev(sinks.end());
	Info("Attached logger sink: {}", it->get()->GetName());
}

std::string_view Log::GetTypeStr(Log::Type type)
{
	switch (type)
	{
		case Log::Type::Debug: return "DEBUG";
		case Log::Type::Info: return "INFO";
		case Log::Type::Warn: return "WARN";
		case Log::Type::Error: return "ERROR";
		default: return "UNKNOWN";
	}
}

void Log::Write(Log::Type type, std::string_view msg)
{
	auto timepoint = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

	std::string formatted = std::format(
		"{} [{}] {}\n",
		std::chrono::floor<std::chrono::seconds>(timepoint),
		GetTypeStr(type),
		msg
	);

	std::scoped_lock lock(logmtx);

	for (auto &sink : sinks)
		sink->Write(formatted);
}
