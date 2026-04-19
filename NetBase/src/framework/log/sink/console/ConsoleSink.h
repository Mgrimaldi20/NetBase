#ifndef _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_CONSOLESINK_H_

#include <memory>

#include "../../formatter/Formatter.h"

#include "../Sink.h"

class ConsoleSink : public Sink
{
public:
	ConsoleSink(std::unique_ptr<Formatter> formatter);
	virtual ~ConsoleSink() = default;

	void Write(const Entry &entry) override final;
	std::string_view GetName() override final;

private:
	std::unique_ptr<Formatter> formatter;
};

#endif
