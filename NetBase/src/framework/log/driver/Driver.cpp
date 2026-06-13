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

std::vector<std::string> Driver::GetSinkConfig() const
{
	std::vector<std::string> sinkcfg;

	for (const auto &sink: sinks)
		sinkcfg.push_back(sink->GetName());

	return sinkcfg;
}

std::vector<std::string> Driver::GetPolicyConfig() const
{
	std::vector<std::string> policycfg;

	for (const auto &policy: policies)
		policycfg.push_back(policy->GetName());

	return policycfg;
}
