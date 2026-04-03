#include <format>
#include <stdexcept>

#include <dlfcn.h>

#include "PosixDynamicLibrary.h"

PosixDynamicLibrary::PosixDynamicLibrary(std::filesystem::path fullpath)
	: DynamicLibrary(fullpath),
	handle()
{
	handle = dlopen(fullpath.c_str(), RTLD_LAZY);
	if (!handle.has_value())
	{
		std::string errormsg = std::format(
			"An error has occured while trying to load Shared Library file: {} :: {}",
			fullpath.string(),
			dlerror()
		);

		throw std::runtime_error(errormsg);
	}

	dlerror();
}

PosixDynamicLibrary::~PosixDynamicLibrary()
{
	dlclose(handle);
}

std::any PosixDynamicLibrary::GetSymbol(const std::string &funcname)
{
	void *func = dlsym(handle, funcname.c_str());
	if (!func)
	{
		std::string errormsg = std::format(
			"An error has occured while trying to grab the function: {} from Dynamic Library file: {} :: {}",
			funcname,
			fullpath.string(),
			dlerror()
		);

		throw std::runtime_error(errormsg);
	}

	dlerror();

	return func;
}

std::unique_ptr<DynamicLibrary> DynamicLibrary::CreateDynamicLibrary(std::filesystem::path fullpath)
{
	return std::make_unique<PosixDynamicLibrary>(fullpath);
}
