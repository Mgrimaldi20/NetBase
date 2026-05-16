#include "SourceLocationPolicy.h"

SourceLocationPolicy::SourceLocationPolicy(Entry::Level level)
	: policyname("SourceLocationPolicy"),
	level(level)
{
}

bool SourceLocationPolicy::Transform(Entry &entry)
{
	if (entry.level == level)
		return true;

	entry.srcloc.reset();
	return true;
}

std::string &SourceLocationPolicy::GetName()
{
	return policyname;
}
