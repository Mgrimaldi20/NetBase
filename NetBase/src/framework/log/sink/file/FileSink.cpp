#include "FileSink.h"

FileSink::FileSink(const std::filesystem::path &fullpath)
	: sinkname(fullpath.filename().string())
{
	if (fullpath.empty())
		throw std::runtime_error("The full path provided to the Logger is empty");

	if (!fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Logger has no file name");

	if (fullpath.has_parent_path())
		std::filesystem::create_directories(fullpath.parent_path());

	logfile.open(fullpath.string());
}

void FileSink::Write(std::string_view msg)
{
	logfile << msg;
}

std::string_view FileSink::GetName()
{
	return sinkname;
}
