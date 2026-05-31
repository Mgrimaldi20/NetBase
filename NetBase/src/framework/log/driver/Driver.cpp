#include "Driver.h"

struct Driver::Impl
{
	Impl(
		std::string drivername,
		std::vector<std::shared_ptr<Sink>> sinks,
		std::vector<std::shared_ptr<Policy>> policies
	)
		: drivername(std::move(drivername)),
		sinks(sinks),
		policies(policies)
	{}

	~Impl() = default;

	std::string drivername;

	std::vector<std::shared_ptr<Sink>> sinks;
	std::vector<std::shared_ptr<Policy>> policies;
};

Driver::Driver(
	std::string drivername,
	std::vector<std::shared_ptr<Sink>> sinks,
	std::vector<std::shared_ptr<Policy>> policies
)
	: pimpl(PImplPtr<Driver::Impl>::MakePImpl(std::move(drivername), sinks, policies))
{
}

Driver::~Driver() = default;

void Driver::Submit(Entry &entry)
{
	for (auto &policy : pimpl->policies)
	{
		if (!policy->Transform(entry))
			return;
	}

	for (auto &sink : pimpl->sinks)
		sink->Write(entry);
}

std::string &Driver::GetName()
{
	return pimpl->drivername;
}

std::vector<std::string> Driver::GetSinkConfig() const
{
	std::vector<std::string> sinkcfg;

	for (const auto &sink: pimpl->sinks)
		sinkcfg.push_back(sink->GetName());

	return sinkcfg;
}

std::vector<std::string> Driver::GetPolicyConfig() const
{
	std::vector<std::string> policycfg;

	for (const auto &policy: pimpl->policies)
		policycfg.push_back(policy->GetName());

	return policycfg;
}
