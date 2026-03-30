#ifndef _NETBASE_SYS_DYNAMICLIBRARY_H_
#define _NETBASE_SYS_DYNAMICLIBRARY_H_

#include <any>
#include <string>

class DynamicLibrary
{
public:
	DynamicLibrary() = default;
	virtual ~DynamicLibrary() = default;

	virtual std::any GetSymbol(const std::string &funcname) = 0;
};

#endif
