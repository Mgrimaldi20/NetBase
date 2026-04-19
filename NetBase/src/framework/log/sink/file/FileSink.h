#ifndef _NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H_
#define _NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H_

#include <fstream>
#include <filesystem>
#include <string>
#include <memory>

#include "../../formatter/Formatter.h"

#include "../Sink.h"

class FileSink : public Sink
{
public:
	FileSink(const std::filesystem::path &fullpath, std::unique_ptr<Formatter> formatter);
	virtual ~FileSink() = default;

	void Write(const Entry &entry) override final;
	std::string_view GetName() override final;

private:
	std::ofstream logfile;
	std::string sinkname;
	std::unique_ptr<Formatter> formatter;
};

#endif
