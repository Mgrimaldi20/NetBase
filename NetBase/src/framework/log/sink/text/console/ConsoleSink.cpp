#include <print>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::unique_ptr<TextFormatter> formatter)
	: sinkname("STDOUT"),
	formatter(std::move(formatter))
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

void ConsoleSink::SetFormatter(std::unique_ptr<TextFormatter> fmtter)
{
	formatter = std::move(fmtter);
}
