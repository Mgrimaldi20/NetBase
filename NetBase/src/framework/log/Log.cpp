#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: logname(std::move(logname)),
	driver(driver)
{
	for (const auto &sink : this->driver->GetSinks())
		Info("Attached sink: {}", sink->GetName());

	for (const auto &policy : this->driver->GetPolicies())
		Info("Attached sink: {}", policy->GetName());

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

void Log::AttachDriver(std::shared_ptr<Driver> driver)
{
	this->driver = driver;
	Info("Attached new driver: {}", driver->GetName());
}
