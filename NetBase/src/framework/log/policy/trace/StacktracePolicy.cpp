#include "StacktracePolicy.h"

StacktracePolicy::StacktracePolicy()
	: policyname("StacktracePolicy")
{
}

bool StacktracePolicy::Transform(Entry &entry)
{
	if (entry.level != Entry::Level::Fatal)
		return false;

	entry.stacktrace = std::stacktrace::current(4);	// skip 4 frames (to call site)
	return true;
}

std::string &StacktracePolicy::GetName()
{
	return policyname;
}
