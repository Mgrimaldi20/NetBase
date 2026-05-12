#ifndef __NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H__

#include <memory>

#include "../../formatter/Formatter.h"

#include "../Sink.h"

/*
* Class: ConsoleSink
* An implementation of the Sink interface to write a log entry to stdout.
* 
*	Write: Writes a formatted log entry to the console
*	SetFormatter: Sets the internal formatter to a newly defined one
*	GetName: Gets the name of the stdout Sink
*/
class ConsoleSink : public Sink
{
public:
	ConsoleSink(std::shared_ptr<Formatter> formatter = {});
	virtual ~ConsoleSink() = default;

	void Write(const Entry &entry) override final;
	void SetFormatter(std::shared_ptr<Formatter> fmtter) override final;
	std::string &GetName() override final;

private:
	std::string sinkname;
	std::shared_ptr<Formatter> formatter;
};

#endif
