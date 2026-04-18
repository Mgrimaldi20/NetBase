#ifndef _NETBASE_FRAMEWORK_LOG_SINK_SINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_SINK_H_

#include <string_view>

class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void Write(std::string_view msg) = 0;
	virtual std::string_view GetName() = 0;
};

#endif
