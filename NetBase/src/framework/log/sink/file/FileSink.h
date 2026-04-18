#ifndef _NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H_

#include <fstream>
#include <filesystem>
#include <string>

#include "../Sink.h"

class FileSink : public Sink
{
public:
	FileSink(const std::filesystem::path &fullpath);
	virtual ~FileSink() = default;

	void Write(std::string_view msg) override final;
	std::string_view GetName() override final;

private:
	std::ofstream logfile;
	std::string sinkname;
};

#endif
