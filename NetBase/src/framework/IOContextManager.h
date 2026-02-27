#ifndef _NETBASE_FRAMEWORK_IOCONTEXTMANAGER_H_
#define _NETBASE_FRAMEWORK_IOCONTEXTMANAGER_H_

#include "Log.h"

class IOContextManager
{
public:
	IOContextManager(Log &log);
	~IOContextManager();

	bool PostAccept();

	void Clear();

private:
	Log &log;
};

#endif
