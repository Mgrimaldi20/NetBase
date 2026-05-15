#include "StacktracePolicy.h"

StacktracePolicy::StacktracePolicy(Entry::Level level)
	: policyname("StacktracePolicy"),
	level(level)
{
}

bool StacktracePolicy::Applicable(const Entry &entry)
{
	if (entry.level != level)
		return false;

	return true;
}

void StacktracePolicy::Transform(Entry &entry)
{
	entry.stacktrace = std::stacktrace::current();
}

std::string &StacktracePolicy::GetName()
{
	return policyname;
}
