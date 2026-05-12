#include <print>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::shared_ptr<TextFormatter> formatter)
	: sinkname("STDOUT"),
	formatter(formatter)
{
}

void ConsoleSink::Write(const Entry &entry)
{
	if (formatter)
		std::print("{}", formatter->Format(entry));
}

std::string &ConsoleSink::GetName()
{
	return sinkname;
}

void ConsoleSink::SetFormatter(std::shared_ptr<TextFormatter> fmtter)
{
	formatter = fmtter;
}
