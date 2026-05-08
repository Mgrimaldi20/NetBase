#ifndef __NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_FILESINK_H__

#include <fstream>
#include <filesystem>
#include <string>
#include <memory>

#include "../../formatter/Formatter.h"

#include "../Sink.h"

/*
* Class: FileSink
* An implementation of the Sink interface to write a log entry to the file specified by the path.
* The Sink will create the file and directory/s if they do not exist.
* 
*	Write: Writes a formatted log entry to the file opened
*	GetName: Gets the name of the file Sink, will be the name of the file
*/
class FileSink : public Sink
{
public:
	FileSink(const std::filesystem::path &fullpath, std::shared_ptr<Formatter> formatter);
	virtual ~FileSink() = default;

	void Write(const Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::ofstream logfile;
	std::string sinkname;
	std::shared_ptr<Formatter> formatter;
};

#endif
