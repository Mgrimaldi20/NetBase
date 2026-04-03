#ifndef _NETBASE_SYS_WIN_WINDYNAMICLIBRARY_H_
#define _NETBASE_SYS_WIN_WINDYNAMICLIBRARY_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "sys/DynamicLibrary.h"

class WinDynamicLibrary : public DynamicLibrary
{
public:
	WinDynamicLibrary(std::filesystem::path fullpath);
	virtual ~WinDynamicLibrary();

	std::any GetSymbol(const std::string &funcname) override final;

private:
	HMODULE dllhandle;
};

#endif
