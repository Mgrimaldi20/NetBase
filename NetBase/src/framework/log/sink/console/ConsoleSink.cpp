#include <print>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::unique_ptr<Formatter> formatter)
	: sinkname("STDOUT"),
	formatter(std::move(formatter))
{
}

void ConsoleSink::Write(const Entry &entry)
{
	std::print("{}", formatter->Format(entry));
}

std::string &ConsoleSink::GetName()
{
	return sinkname;
}
