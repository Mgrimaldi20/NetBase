#include "WinDynamicLibrary.h"

#include "WinPluginManager.h"

WinPluginManager::WinPluginManager(std::filesystem::path fullpath)
{
	plugin = std::make_unique<WinDynamicLibrary>(fullpath);
}

PluginManager::RegisterPluginFn WinPluginManager::LoadRegisterPluginFn()
{
	return std::any_cast<PluginManager::RegisterPluginFn>(
		plugin->GetSymbol("RegisterPlugin")
	);
}
