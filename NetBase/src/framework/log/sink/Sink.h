#ifndef _NETBASE_FRAMEWORK_LOG_SINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_H_

#include <string>

#include "../Entry.h"

/*
* Class: Sink
* An abstract interface designed to represent a logging destination.
* A sink can be anything, eg. console, file, db, rotating files, etc.
* 
*	Write: Pure virtual function, designed to write an entry to the destination
*	GetName: Pure virtual function, get the name of the sink, can be set to anything
*/
class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void Write(const Entry &entry) = 0;
	virtual std::string &GetName() = 0;
};

#endif
