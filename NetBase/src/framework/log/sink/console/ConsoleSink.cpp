#include <iostream>
#include <format>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::unique_ptr<Formatter> formatter)
	: formatter(std::move(formatter))
{
}

void ConsoleSink::Write(const Entry &entry)
{
	std::cout << formatter->Format(entry);
}

std::string_view ConsoleSink::GetName()
{
	return "STDOUT";
}
