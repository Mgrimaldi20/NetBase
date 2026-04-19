#ifndef _NETBASE_FRAMEWORK_LOG_FORMATTER_FORMATTER_H_
#define _NETBASE_FRAMEWORK_LOG_FORMATTER_FORMATTER_H_

#include <string>

#include "../Entry.h"

class Formatter
{
public:
	Formatter() = default;
	virtual ~Formatter() = default;

	virtual std::string Format(const Entry &entry) const = 0;
};

#endif
