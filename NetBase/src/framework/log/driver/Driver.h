#ifndef __NETBASE_FRAMEWORK_LOG_DRIVER_DRIVER_H__
#define __NETBASE_FRAMEWORK_LOG_DRIVER_DRIVER_H__

#include <memory>
#include <vector>
#include <string>

#include "NetBaseAPI.h"

#include "../sink/Sink.h"
#include "../policy/Policy.h"
#include "../entry/Entry.h"

/*
* Class: Driver
* A wrapper around the different config options that a logger can ingest and use.
* Basically a big configuration wrapper for logging, add sinks and policies.
* 
*	Submit: Perform all of the policy application and sink writing applicable
*	GetName: Gets the name of the driver class, mainly for observability
*	GetSinks: Returns a const look into the underlying sinks
*	GetPolicies: Returns a const look into the underlying policies
*/
class NETBASE_API Driver
{
public:
	Driver(
		std::string drivername,
		std::vector<std::shared_ptr<Sink>> sinks = {},
		std::vector<std::shared_ptr<Policy>> policies = {}
	);

	~Driver() = default;

	void Submit(Entry &entry);

	std::string &GetName();

	std::vector<std::string> GetSinkConfig() const;
	std::vector<std::string> GetPolicyConfig() const;

private:
	std::string drivername;

	std::vector<std::shared_ptr<Sink>> sinks;
	std::vector<std::shared_ptr<Policy>> policies;
};

#endif
