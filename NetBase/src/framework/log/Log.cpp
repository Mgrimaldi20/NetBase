#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(
	std::string logname,
	std::vector<std::shared_ptr<Sink>> sinks,
	std::vector<std::unique_ptr<Policy>> policies
)
	: logname(std::move(logname)),
	sinks(),
	policies()
{
	for (const auto &sink : sinks)
		AttachSink(sink);

	for (auto &policy : policies)
		AttachPolicy(std::move(policy));

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

void Log::AttachPolicy(std::unique_ptr<Policy> policy)
{
	policies.push_back(std::move(policy));

	// i dont really like this much but it does what i want so whatever
	auto it = std::prev(policies.end());

	Info("Attached policy: {}", it->get()->GetName());
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

	for (auto &sink : sinks)
		sink->Write(entry);
}
