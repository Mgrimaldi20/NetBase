#include <print>
#include <iostream>

#include "ConsoleSink.h"

struct ConsoleSink::Impl
{
	Impl(std::string sinkname, std::unique_ptr<TextFormatter> formatter)
		: sinkname(sinkname),
		formatter(std::move(formatter))
	{}

	~Impl() = default;

	std::string sinkname;
	std::unique_ptr<TextFormatter> formatter;
};

ConsoleSink::ConsoleSink(std::unique_ptr<TextFormatter> formatter)
	: pimpl(PImplPtr<ConsoleSink::Impl>::MakePImpl("STDOUT", std::move(formatter)))
{
}

ConsoleSink::~ConsoleSink() = default;

void ConsoleSink::Write(const Entry &entry)
{
	if (pimpl->formatter)
		std::print(std::cout, "{}", pimpl->formatter->Format(entry));
}

void ConsoleSink::Flush()
{
	std::cout.flush();
}

std::string &ConsoleSink::GetName()
{
	return pimpl->sinkname;
}

void ConsoleSink::SetFormatter(std::unique_ptr<TextFormatter> fmtter)
{
	pimpl->formatter = std::move(fmtter);
}
