#ifndef __NETBASE_FRAMEWORK_LOG_SINK_SINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_SINK_H__

#include <string>

#include "../formatter/Formatter.h"

#include "../entry/Entry.h"

/*
* Class: Sink
* An abstract interface designed to represent a logging destination.
* A sink can be anything, eg. console, file, db, rotating files, etc.
* 
*	Write: Pure virtual function, designed to write an entry to the destination
*	SetFormatter: Pure virtual function, set a formatter object if not set in constructor
*	GetName: Pure virtual function, get the name of the sink, can be set to anything
*/
class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void Write(const Entry &entry) = 0;
	virtual void SetFormatter(std::shared_ptr<Formatter> fmtter) = 0;
	virtual std::string &GetName() = 0;
};

#endif
