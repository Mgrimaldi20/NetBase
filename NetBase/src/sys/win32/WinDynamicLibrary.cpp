#include <system_error>
#include <format>

#include "WinDynamicLibrary.h"

WinDynamicLibrary::WinDynamicLibrary(std::filesystem::path fullpath)
	: DynamicLibrary(fullpath),
	dllhandle()
{
	dllhandle = LoadLibrary(fullpath.c_str());	// returns a wchar_t on win32
	if (!dllhandle || dllhandle == INVALID_HANDLE_VALUE)
	{
		std::string errormsg = std::format(
			"An error has occured while trying to load DLL file: {}",
			fullpath.string()
		);

		throw std::system_error(
			std::error_code(GetLastError(), std::system_category()),
			std::move(errormsg)
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
		std::string errormsg = std::format(
			"An error has occured while trying to grab the function: {} from DLL file: {}",
			funcname,
			fullpath.string()
		);

		throw std::system_error(
			std::error_code(GetLastError(), std::system_category()),
			std::move(errormsg)
		);
	}

	return func;
}

std::unique_ptr<DynamicLibrary> DynamicLibrary::CreateDynamicLibrary(std::filesystem::path fullpath)
{
	return std::make_unique<WinDynamicLibrary>(fullpath);
}
