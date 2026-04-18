#ifndef _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_

#include "../Sink.h"

class ConsoleSink : public Sink
{
public:
	ConsoleSink() = default;
	virtual ~ConsoleSink() = default;

	void Write(std::string_view msg) override final;
	std::string_view GetName() override final;
};

#endif
