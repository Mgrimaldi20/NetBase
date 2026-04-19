#ifndef _NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H_
#define _NETBASE_FRAMEWORK_LOG_FORMATTER_TEXTFORMATTER_H_

#include "../Formatter.h"

class TextFormatter : public Formatter
{
public:
	TextFormatter() = default;
	virtual ~TextFormatter() = default;

	std::string Format(const Entry &entry) const override final;
};

#endif
