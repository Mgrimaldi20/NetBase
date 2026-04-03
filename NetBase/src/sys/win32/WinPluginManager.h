#ifndef _NETBASE_SYS_WIN_WINPLUGINMANAGER_H_
#define _NETBASE_SYS_WIN_WINPLUGINMANAGER_H_

#include <filesystem>

#include "sys/PluginManager.h"

class WinPluginManager : public PluginManager
{
public:
	WinPluginManager(std::filesystem::path fullpath);
	virtual ~WinPluginManager() = default;

	PluginManager::RegisterPluginFn LoadRegisterPluginFn() override final;
};

#endif
