#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__

#include "NetBaseAPI.h"

#include "../../entry/Entry.h"

#include "../Policy.h"

/*
* Class: StacktracePolicy
* Checks the log level, if its the same as the level set, then grab the stack trace.
* 
*	Transform: Gets the current stack trace if applicable and returns true if further steps are needed
*	GetName: Returns the name of the policy
*/
class NETBASE_API StacktracePolicy : public Policy
{
public:
	StacktracePolicy(Entry::Level level);
	virtual ~StacktracePolicy() = default;

	bool Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;

	Entry::Level level;
};

#endif
