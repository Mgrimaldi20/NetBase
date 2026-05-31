#include "StacktracePolicy.h"

struct StacktracePolicy::Impl
{
	Impl(std::string policyname)
		: policyname(policyname)
	{}

	~Impl() = default;

	std::string policyname;
};

StacktracePolicy::StacktracePolicy(Entry::Level level)
	: pimpl(PImplPtr<StacktracePolicy::Impl>::MakePImpl("StacktracePolicy")),
	level(level)
{
}

StacktracePolicy::~StacktracePolicy() = default;

bool StacktracePolicy::Transform(Entry &entry)
{
	if (entry.entrylevel != level)
		return true;

	entry.stacktrace = std::stacktrace::current();
	return true;
}

std::string &StacktracePolicy::GetName()
{
	return pimpl->policyname;
}
