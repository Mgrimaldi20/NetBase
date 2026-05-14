#ifndef __NETBASE_FRAMEWORK_LOG_POLICY_POLICY_H__
#define __NETBASE_FRAMEWORK_LOG_POLICY_POLICY_H__

#include <string>

#include "../entry/Entry.h"

/*
* Class: Policy
* An abstract interface that represents a logging policy.
* Policies can be used to control log information, or to query the logger.
* 
*	Transform: Pure virtual function, applies the policy and returns if the policy was applied or not
*	GetName: Pure virtual function, get the name of the policy, can be set to anything
*/
class Policy
{
public:
	Policy() = default;
	virtual ~Policy() = default;

	virtual bool Transform(Entry &entry) = 0;
	virtual std::string &GetName() = 0;
};

#endif
