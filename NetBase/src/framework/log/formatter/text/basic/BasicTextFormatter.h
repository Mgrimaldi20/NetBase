#ifndef __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXT_BASIC_BASICTEXTFORMATTER_H__
#define __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXT_BASIC_BASICTEXTFORMATTER_H__

#include <source_location>
#include <stacktrace>

#include "../TextFormatter.h"

/*
* Class: TextFormatter
* A basic text formatter to turn an entry into a basic line format.
* Full file information will only be printed on debug builds for debug level logging.
*
*	Format: Turns a log entry into a formatted string
*/
class BasicTextFormatter : public TextFormatter
{
public:
	BasicTextFormatter() = default;
	virtual ~BasicTextFormatter() = default;

	std::string Format(const Entry &entry) const override final;

private:
	std::string GetSourceLocation(const Entry &entry) const;
	std::string GetStacktrace(const Entry &entry) const;
};

#endif
