#include <iostream>

#include "ConsoleSink.h"

void ConsoleSink::Write(std::string_view msg)
{
	std::cout << msg;
}

std::string_view ConsoleSink::GetName()
{
	return "STDOUT";
}
