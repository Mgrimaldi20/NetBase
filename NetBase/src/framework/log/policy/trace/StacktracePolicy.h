#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_STACKTRACEPOLICY_H__

#include "../Policy.h"

/*
* Class: StacktracePolicy
* Checks the log level, if its Fatal, fill in the stacktrace for logging.
* 
*	Applicable: Checks if the log level is fatal, returns true if it is
*	Transform: Gets the current trace if the policy was applicable
*	GetName: Returns the name of the policy
*/
class StacktracePolicy : public Policy
{
public:
	StacktracePolicy();
	virtual ~StacktracePolicy() = default;

	bool Applicable(const Entry &entry) override final;
	void Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;
};

#endif
