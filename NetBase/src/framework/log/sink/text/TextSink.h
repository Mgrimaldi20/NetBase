#ifndef __NETBASE_FRAMEWORK_LOG_SINK_TEXT_TEXTSINK_H__
#define __NETBASE_FRAMEWORK_LOG_SINK_TEXT_TEXTSINK_H__

#include <memory>

#include "NetBaseAPI.h"

#include "../../formatter/text/TextFormatter.h"

#include "../Sink.h"

/*
* Class: Sink
* An abstract interface designed to represent a text based logging destination.
* A text sink can be anything text related, eg. files, consoles, debug windows, etc.
*
*	Write: Pure virtual function, designed to write an entry to the destination - from Sink interface
*	GetName: Pure virtual function, get the name of the sink, can be set to anything - from Sink interface
*	SetFormatter: Pure virtual function, sets the text formatter for text based outputs
*/
class NETBASE_API TextSink : public Sink
{
public:
	TextSink() = default;
	virtual ~TextSink() = default;

	virtual void Write(const Entry &entry) = 0;
	virtual std::string &GetName() = 0;

	virtual void SetFormatter(std::unique_ptr<TextFormatter> fmtter) = 0;
};

#endif
