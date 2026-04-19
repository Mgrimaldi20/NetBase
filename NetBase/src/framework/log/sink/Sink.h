#ifndef _NETBASE_FRAMEWORK_LOG_SINK_SINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_SINK_H_

#include <string_view>

#include "../Entry.h"

class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void Write(const Entry &entry) = 0;
	virtual std::string_view GetName() = 0;
};

#endif
