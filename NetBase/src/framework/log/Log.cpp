#include <chrono>
#include <iterator>

#include "Log.h"

struct Log::Impl
{
	Impl() = default;
	~Impl() = default;

	std::string logname;
	std::shared_ptr<Driver> driver;
};

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: pimpl(PImplPtr<Log::Impl>::MakePImpl())
{
	SetLogName(std::move(logname));
	AttachDriver(driver);

	Info("Logger started: {}", this->pimpl->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", pimpl->logname);
}

EntryBuilder Log::Debug(std::string msg, std::source_location loc)
{
	return EntryBuilder(loc);
}

EntryBuilder Log::Info(std::string msg, std::source_location loc)
{
	return EntryBuilder(loc);
}

EntryBuilder Log::Warn(std::string msg, std::source_location loc)
{
	return EntryBuilder(loc);
}

EntryBuilder Log::Error(std::string msg, std::source_location loc)
{
	return EntryBuilder(loc);
}

EntryBuilder Log::Fatal(std::string msg, std::source_location loc)
{
	return EntryBuilder(loc);
}

void Log::SetLogName(std::string name)
{
	pimpl->logname = std::move(name);
}

void Log::AttachDriver(std::shared_ptr<Driver> newdriver)
{
	this->pimpl->driver = newdriver;

	Info("Attached new driver: {}", pimpl->driver->GetName());

	for (const auto &sinkname : pimpl->driver->GetSinkConfig())
		Info("Attached sink: {}", sinkname);

	for (const auto &policyname : pimpl->driver->GetPolicyConfig())
		Info("Attached policy: {}", policyname);
}
