#ifndef _NETBASE_SYS_POSIX_POSIXDYNAMICLIBRARY_H_
#define _NETBASE_SYS_POSIX_POSIXDYNAMICLIBRARY_H_

#include <filesystem>

#include "DynamicLibrary.h"

class PosixDynamicLibrary : public DynamicLibrary
{
public:
	PosixDynamicLibrary(const std::filesystem::path &fullpath);
	virtual ~PosixDynamicLibrary();

	std::any GetSymbol(const std::string &funcname) override final;
};

#endif
