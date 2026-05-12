#ifndef __NETBASE_FRAMEWORK_LOG_SINK_TEXT_CONSOLE_CONSOLESINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_TEXT_CONSOLE_CONSOLESINK_H__

#include "../TextSink.h"

/*
* Class: ConsoleSink
* An implementation of the Sink interface to write a log entry to stdout.
* 
*	Write: Writes a formatted log entry to the console
*	GetName: Gets the name of the stdout Sink
*	SetFormatter: Sets the internal formatter to a newly defined one
*/
class ConsoleSink : public TextSink
{
public:
	ConsoleSink(std::shared_ptr<TextFormatter> formatter = {});
	virtual ~ConsoleSink() = default;

	void Write(const Entry &entry) override final;
	std::string &GetName() override final;

	void SetFormatter(std::shared_ptr<TextFormatter> fmtter) override final;

private:
	std::string sinkname;
	std::shared_ptr<TextFormatter> formatter;
};

#endif
