#include "Driver.h"

Driver::Driver(
	std::string drivername,
	std::vector<std::shared_ptr<Sink>> sinks,
	std::vector<std::shared_ptr<Policy>> policies
)
	: drivername(std::move(drivername)),
	sinks(sinks),
	policies(policies)
{
}

void Driver::Submit(Entry &entry)
{
	for (auto &policy : policies)
	{
		if (!policy->Transform(entry))
			return;
	}

	for (auto &sink : sinks)
		sink->Write(entry);
}

std::string &Driver::GetName()
{
	return drivername;
}

const std::vector<std::shared_ptr<Sink>> Driver::GetSinks() const
{
	return sinks;
}

const std::vector<std::shared_ptr<Policy>> Driver::GetPolicies() const
{
	return policies;
}
