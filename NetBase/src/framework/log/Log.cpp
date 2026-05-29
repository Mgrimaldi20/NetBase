#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
{
	SetLogName(std::move(logname));
	AttachDriver(driver);

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

	for (const auto &sinkname : driver->GetSinkConfig())
		Info("Attached sink: {}", sinkname);

	for (const auto &policyname : driver->GetPolicyConfig())
		Info("Attached policy: {}", policyname);
}
