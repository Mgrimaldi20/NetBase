#ifndef __NETBASE_FRAMEWORK_LOG_FORMATTER_FORMATTER_H__
#define __NETBASE_FRAMEWORK_LOG_FORMATTER_FORMATTER_H__

#include <string>

#include "../entry/Entry.h"

/*
* Class: Formatter
* An abstract interface designed to represent a log entry formatter.
* The formatter takes the log entry and transforms it in a way the impl defines.
* 
*	Format: Pure virtual function, designed to take the entry and return its string representation
*/
class Formatter
{
public:
	Formatter() = default;
	virtual ~Formatter() = default;

	virtual std::string Format(const Entry &entry) const = 0;
};

#endif
