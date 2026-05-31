#ifndef __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXT_TEXTFORMATTER_H__
#define __NETBASE_FRAMEWORK_LOG_FORMATTER_TEXT_TEXTFORMATTER_H__

#include <string>

#include "NetBaseAPI.h"

#include "../../entry/Entry.h"

/*
* Class: TextFormatter
* An abstract interface to respresent a text formatter.
* Format log entry as a text string, designed for text based outputs.
* 
*	Format: Pure virtual function, designed to turn an entry into a formatted string
*/
class NETBASE_API TextFormatter
{
public:
	TextFormatter() = default;
	virtual ~TextFormatter() = default;

	virtual std::string Format(const Entry &entry) const = 0;
};

#endif
