#include <chrono>
#include <iterator>

#include "entry/EntryBuilderImpl.h"

#include "Log.h"

struct Log::Impl
{
	Impl()
		: logname(),
		driver()
	{}

	Impl(std::string logname, std::shared_ptr<Driver> driver)
		: logname(std::move(logname)),
		driver(driver)
	{}

	~Impl() = default;

	std::string logname;
	std::shared_ptr<Driver> driver;
};

Log::Log()
	: pimpl(PImplPtr<Log::Impl>::MakePImpl())
{
}

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: pimpl(PImplPtr<Log::Impl>::MakePImpl(logname, driver))
{
	SetLogName(std::move(logname));
	AttachDriver(driver);

	Info("Logger started: {}", pimpl->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", pimpl->logname);
}

EntryBuilder &Log::Debug(std::string msg, std::source_location loc)
{
#if defined(NETBASE_DEBUG)
	return EntryBuilderImpl(pimpl->driver, loc)
		.Name(pimpl->logname)
		.Level(Entry::Level::Debug)
		.Message(msg);
#else
	return {};
#endif
}

EntryBuilder &Log::Info(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(pimpl->driver, loc)
		.Name(pimpl->logname)
		.Level(Entry::Level::Info)
		.Message(msg);
}

EntryBuilder &Log::Warn(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(pimpl->driver, loc)
		.Name(pimpl->logname)
		.Level(Entry::Level::Warn)
		.Message(msg);
}

EntryBuilder &Log::Error(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(pimpl->driver, loc)
		.Name(pimpl->logname)
		.Level(Entry::Level::Error)
		.Message(msg);
}

EntryBuilder &Log::Fatal(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(pimpl->driver, loc)
		.Name(pimpl->logname)
		.Level(Entry::Level::Fatal)
		.Message(msg);
}

void Log::SetLogName(std::string name)
{
	if (name.empty())
		return;

	pimpl->logname = std::move(name);
}

void Log::AttachDriver(std::shared_ptr<Driver> newdriver)
{
	if (!newdriver)
		return;

	pimpl->driver = newdriver;

	Info("Attached new driver: {}", pimpl->driver->GetName());

	for (const auto &sinkname : pimpl->driver->GetSinkConfig())
		Info("Attached sink: {}", sinkname);

	for (const auto &policyname : pimpl->driver->GetPolicyConfig())
		Info("Attached policy: {}", policyname);
}
