#ifndef __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H__
#define __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H__

#include "../entry/Entry.h"

/*
* Class: TextFormatter
* Format a log entry as a simple text string, designed for simple text based outputs.
* 
*	Format: Will turn the entry into a basic general purpose text format
*/
class TextFormatter
{
public:
	TextFormatter() = default;
	virtual ~TextFormatter() = default;

	std::string Format(const Entry &entry) const;
};

#endif
