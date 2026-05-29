#include "SourceLocationPolicy.h"

SourceLocationPolicy::SourceLocationPolicy(Entry::Level level)
	: policyname("SourceLocationPolicy"),
	level(level)
{
}

bool SourceLocationPolicy::Transform(Entry &entry)
{
	if (entry.entrylevel == level)
		return true;

	entry.location.reset();
	return true;
}

std::string &SourceLocationPolicy::GetName()
{
	return policyname;
}
