#include <system_error>
#include <format>

#include "WinDynamicLibrary.h"

WinDynamicLibrary::WinDynamicLibrary(std::filesystem::path fullpath)
	: DynamicLibrary(fullpath),
	dllhandle()
{
	if (!std::filesystem::exists(fullpath))
		throw std::runtime_error(std::format("File: {} does not exist", fullpath.string()));

	if (!std::filesystem::is_regular_file(fullpath))
		throw std::runtime_error(std::format("Library path: {} is not a file", fullpath.string()));

	dllhandle = LoadLibrary(fullpath.c_str());
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
	return std::unique_ptr<WinDynamicLibrary>(new WinDynamicLibrary(fullpath));
}
