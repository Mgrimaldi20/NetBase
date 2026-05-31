#include <ostream>
#include <print>
#include <fstream>

#include "FileSink.h"

struct FileSink::Impl
{
	Impl(std::string sinkname, std::unique_ptr<TextFormatter> formatter)
		: logfile(),
		sinkname(sinkname),
		formatter(std::move(formatter))
	{}

	~Impl() = default;

	std::ofstream logfile;

	std::string sinkname;
	std::unique_ptr<TextFormatter> formatter;
};

FileSink::FileSink(const std::filesystem::path &fullpath, std::unique_ptr<TextFormatter> formatter)
	: pimpl(PImplPtr<FileSink::Impl>::MakePImpl(fullpath.filename().string(), std::move(formatter)))
{
	if (fullpath.empty())
		throw std::runtime_error("The full path provided to the Logger is empty");

	if (!fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Logger has no file name");

	if (fullpath.has_parent_path())
		std::filesystem::create_directories(fullpath.parent_path());

	pimpl->logfile.open(fullpath.string());
}

FileSink::~FileSink() = default;

void FileSink::Write(const Entry &entry)
{
	if (pimpl->formatter)
		std::print(pimpl->logfile, "{}", pimpl->formatter->Format(entry));
}

void FileSink::Flush()
{
	pimpl->logfile.flush();
}

std::string &FileSink::GetName()
{
	return pimpl->sinkname;
}

void FileSink::SetFormatter(std::unique_ptr<TextFormatter> fmtter)
{
	pimpl->formatter = std::move(fmtter);
}
