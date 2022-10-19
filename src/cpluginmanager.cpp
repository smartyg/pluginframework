#include "config.h"
#include <features.h>

#include "cpluginmanager.h"

#include <string>
#include "PluginManager.hpp"

void cpluginmanager_addManager (const char* tag, register_function refister_func, deregister_function deregister_func) {
	pluginmanager::Manager::getInstance ().addManager (std::string (tag), refister_func, deregister_func);
}

void cpluginmanager_scanDirectory (const char* directory) {
	pluginmanager::Manager::getInstance ().scanDirectory (std::string (directory));
}
