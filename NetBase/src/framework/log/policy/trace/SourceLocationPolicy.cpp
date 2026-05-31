#include "SourceLocationPolicy.h"

struct SourceLocationPolicy::Impl
{
	Impl(std::string policyname)
		: policyname(policyname)
	{}

	~Impl() = default;

	std::string policyname;
};

SourceLocationPolicy::SourceLocationPolicy(Entry::Level level)
	: pimpl(PImplPtr<SourceLocationPolicy::Impl>::MakePImpl("SourceLocationPolicy")),
	level(level)
{
}

SourceLocationPolicy::~SourceLocationPolicy() = default;

bool SourceLocationPolicy::Transform(Entry &entry)
{
	if (entry.entrylevel == level)
		return true;

	entry.location.reset();
	return true;
}

std::string &SourceLocationPolicy::GetName()
{
	return pimpl->policyname;
}
