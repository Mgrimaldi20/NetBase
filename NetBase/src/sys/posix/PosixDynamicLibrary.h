#ifndef __NETBASE_SYS_POSIX_POSIXDYNAMICLIBRARY_H__
#define __NETBASE_SYS_POSIX_POSIXDYNAMICLIBRARY_H__

#include "DynamicLibrary.h"

/*
* Class: PosixDynamicLibrary
* POSIX based implementation of the DynamicLibrary interface to represent a Shared Library.
*
*	GetSymbol: Implementation of the DynamicLibrary interface to get the function from the lib
*/
class PosixDynamicLibrary : public DynamicLibrary
{
public:
	PosixDynamicLibrary(std::filesystem::path fullpath);
	virtual ~PosixDynamicLibrary();

	std::any GetSymbol(const std::string &funcname) override final;

private:
	void *handle;
};

#endif
