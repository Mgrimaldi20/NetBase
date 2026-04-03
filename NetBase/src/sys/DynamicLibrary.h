#ifndef _NETBASE_SYS_DYNAMICLIBRARY_H_
#define _NETBASE_SYS_DYNAMICLIBRARY_H_

#include <any>
#include <string>
#include <filesystem>

class DynamicLibrary
{
public:
	DynamicLibrary(std::filesystem::path fullpath)
		: fullpath(fullpath) {}

	virtual ~DynamicLibrary() = default;

	virtual std::any GetSymbol(const std::string &funcname) = 0;

protected:
	std::filesystem::path fullpath;
};

#endif
