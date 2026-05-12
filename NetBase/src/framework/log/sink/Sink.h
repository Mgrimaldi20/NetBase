#ifndef __NETBASE_FRAMEWORK_LOG_SINK_SINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_SINK_H__

#include <string>

#include "../entry/Entry.h"

/*
* Class: Sink
* An abstract interface designed to represent a logging destination.
* A sink can be anything, eg. console, file, db, rotating files, etc.
* For text based sinks, the TextSink derived interface can be used.
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
