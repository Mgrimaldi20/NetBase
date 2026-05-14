#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_STACKTRACEPOLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_STACKTRACEPOLICY_H__

#include "../Policy.h"

/*
* Class: StacktracePolicy
* Checks the log level, if its Fatal, fill in the stacktrace for logging.
* 
*	Transform: Returns true if the log level was Fatal and get the current stacktrace
*	GetName: Returns the name of the policy
*/
class StacktracePolicy : public Policy
{
public:
	StacktracePolicy();
	virtual ~StacktracePolicy() = default;

	bool Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;
};

#endif
