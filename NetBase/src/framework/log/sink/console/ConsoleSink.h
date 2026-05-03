#ifndef _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_

#include <memory>

#include "../../formatter/Formatter.h"

#include "../Sink.h"

/*
* Class: ConsoleSink
* An implementation of the Sink interface to write a log entry to stdout.
* 
*	Write: Writes a formatted log entry to the console
*	GetName: Gets the name of the stdout Sink
*/
class ConsoleSink : public Sink
{
public:
	ConsoleSink(std::shared_ptr<Formatter> formatter);
	virtual ~ConsoleSink() = default;

	void Write(const Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string sinkname;
	std::shared_ptr<Formatter> formatter;
};

#endif
