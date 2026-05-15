#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__

#include "../../entry/Entry.h"

#include "../Policy.h"

/*
* Class: StacktracePolicy
* Checks the log level, if its the same as the level set, then grab the stack trace.
* 
*	Applicable: Checks if the log level is the level provided, returns true if it is
*	Transform: Gets the current stack trace if the policy was applicable
*	GetName: Returns the name of the policy
*/
class StacktracePolicy : public Policy
{
public:
	StacktracePolicy(Entry::Level level);
	virtual ~StacktracePolicy() = default;

	bool Applicable(const Entry &entry) override final;
	void Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;

	Entry::Level level;
};

#endif
