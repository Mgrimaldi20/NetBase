#ifndef _NETBASE_SYS_PLUGINMANAGER_H_
#define _NETBASE_SYS_PLUGINMANAGER_H_

#include <memory>
#include <functional>

#include "DynamicLibrary.h"

#include "NetBaseAPI.h"

class PluginManager
{
public:
	using RegisterPluginFn = std::function<void(NetBaseAPI &)>;

	PluginManager() = default;
	virtual ~PluginManager() = default;

	virtual PluginManager::RegisterPluginFn LoadRegisterPluginFn() = 0;

protected:
	std::unique_ptr<DynamicLibrary> plugin;
};

#endif
