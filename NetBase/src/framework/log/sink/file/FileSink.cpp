#include <ostream>
#include <print>

#include "FileSink.h"

FileSink::FileSink(const std::filesystem::path &fullpath, std::shared_ptr<Formatter> formatter)
	: sinkname(fullpath.filename().string()),
	formatter(formatter)
{
	if (fullpath.empty())
		throw std::runtime_error("The full path provided to the Logger is empty");

	if (!fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Logger has no file name");

	if (fullpath.has_parent_path())
		std::filesystem::create_directories(fullpath.parent_path());

	logfile.open(fullpath.string());
}

void FileSink::Write(const Entry &entry)
{
	if (formatter)
		std::print(logfile, "{}", formatter->Format(entry));
}

void FileSink::SetFormatter(std::shared_ptr<Formatter> fmtter)
{
	formatter = fmtter;
}

std::string &FileSink::GetName()
{
	return sinkname;
}
