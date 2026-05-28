#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_SOURCELOCATIONPOLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_TRACE_SOURCELOCATIONPOLICY_H__

#include "NetBaseAPI.h"

#include "../../entry/Entry.h"

#include "../Policy.h"

/*
* Class: SourceLocationPolicy
* Checks the log level, if its the same as the level set, then grab the source location.
*
*	Transform: Gets the current source location if applicable and returns true if further steps are needed
*	GetName: Returns the name of the policy
*/
class NETBASE_API SourceLocationPolicy : public Policy
{
public:
	SourceLocationPolicy(Entry::Level level);
	virtual ~SourceLocationPolicy() = default;

	bool Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;

	Entry::Level level;
};

#endif
