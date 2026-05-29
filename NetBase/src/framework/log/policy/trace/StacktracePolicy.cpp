#include "StacktracePolicy.h"

StacktracePolicy::StacktracePolicy(Entry::Level level)
	: policyname("StacktracePolicy"),
	level(level)
{
}

bool StacktracePolicy::Transform(Entry &entry)
{
	if (entry.entrylevel != level)
		return true;

	entry.stacktrace = std::stacktrace::current();
	return true;
}

std::string &StacktracePolicy::GetName()
{
	return policyname;
}
