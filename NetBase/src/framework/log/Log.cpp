#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(
	std::string logname,
	std::vector<std::shared_ptr<Sink>> sinks,
	std::vector<std::shared_ptr<Policy>> policies
)
	: logname(std::move(logname)),
	sinks(sinks),
	policies(policies)
{
	for (const auto &sink : this->sinks)
		Info("Attached sink: {}", sink->GetName());

	for (const auto &policy : this->policies)
		Info("Attached policy: {}", policy->GetName());

	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);
}

void Log::SetLogName(std::string name)
{
	logname = std::move(name);
}

void Log::AttachSink(std::shared_ptr<Sink> sink)
{
	sinks.push_back(sink);
	Info("Attached sink: {}", sink->GetName());
}

void Log::AttachPolicy(std::shared_ptr<Policy> policy)
{
	policies.push_back(policy);
	Info("Attached policy: {}", policy->GetName());
}

void Log::Write(Entry::Level level, std::string msg, std::source_location loc)
{
	Entry entry =
	{
		.time = std::chrono::system_clock::now(),
		.logname = logname,
		.level = level,
		.message = std::move(msg),
		.srcloc = std::move(loc)
	};

	for (auto &policy : policies)
	{
		if (!policy->Transform(entry))
			return;
	}

	for (auto &sink : sinks)
		sink->Write(entry);
}
