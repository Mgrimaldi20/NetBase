#include <chrono>
#include <iterator>

#include "entry/EntryBuilderImpl.h"

#include "Log.h"

struct Log::Impl
{
	Impl(std::string logname, std::shared_ptr<Driver> driver)
		: logname(std::move(logname)),
		driver(driver)
	{}

	~Impl() = default;

	std::string logname;
	std::shared_ptr<Driver> driver;
};

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: pimpl(PImplPtr<Log::Impl>::MakePImpl(std::move(logname), driver))
{
	Info("Attached new driver: {}", pimpl->driver->GetName());

	for (const auto &sinkname : pimpl->driver->GetSinkConfig())
		Info("Attached sink: {}", sinkname);

	for (const auto &policyname : pimpl->driver->GetPolicyConfig())
		Info("Attached policy: {}", policyname);

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
