#include <system_error>
#include <format>

#include "WinDynamicLibrary.h"

WinDynamicLibrary::WinDynamicLibrary(std::filesystem::path fullpath)
	: dllhandle(),
	fullpath(fullpath)
{
	dllhandle = LoadLibrary(fullpath.c_str());
	if (!dllhandle || dllhandle == INVALID_HANDLE_VALUE)
	{
		throw std::system_error(
			std::error_code(GetLastError(), std::system_category()),
			std::format("An error has occured while trying to load DLL file: {}", fullpath)
		);
	}
}

WinDynamicLibrary::~WinDynamicLibrary()
{
	FreeLibrary(dllhandle);
}

std::any WinDynamicLibrary::GetSymbol(const std::string &funcname)
{
	FARPROC func = GetProcAddress(dllhandle, funcname.c_str());
	if (!func)
	{
		throw std::system_error(
			std::error_code(GetLastError(), std::system_category()),
			std::format("An error has occured while trying to grab the function: {} from DLL file: {}", funcname, fullpath)
		);
	}

	return func;
}
