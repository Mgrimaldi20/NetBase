#ifndef _NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H_
#define _NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H_

#include "../Formatter.h"

/*
* Class: TextFormatter
* An implementation of the Formatter interface to format a log entry as a simple text string.
* 
*	Format: Will turn the entry into a basic general purpose text format
*/
class TextFormatter : public Formatter
{
public:
	TextFormatter() = default;
	virtual ~TextFormatter() = default;

	std::string Format(const Entry &entry) const override final;
};

#endif
