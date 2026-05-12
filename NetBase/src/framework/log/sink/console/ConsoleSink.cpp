#include <print>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::shared_ptr<Formatter> formatter)
	: sinkname("STDOUT"),
	formatter(formatter)
{
}

void ConsoleSink::Write(const Entry &entry)
{
	if (formatter)
		std::print("{}", formatter->Format(entry));
}

void ConsoleSink::SetFormatter(std::shared_ptr<Formatter> fmtter)
{
	formatter = fmtter;
}

std::string &ConsoleSink::GetName()
{
	return sinkname;
}
