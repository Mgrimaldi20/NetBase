#include "StacktracePolicy.h"

StacktracePolicy::StacktracePolicy()
	: policyname("StacktracePolicy")
{
}

bool StacktracePolicy::Applicable(const Entry &entry)
{
	if (entry.level != Entry::Level::Fatal)
		return false;

	return true;
}

void StacktracePolicy::Transform(Entry &entry)
{
	entry.stacktrace = std::stacktrace::current(3);	// skip 3 frames (to call site)
}

std::string &StacktracePolicy::GetName()
{
	return policyname;
}
