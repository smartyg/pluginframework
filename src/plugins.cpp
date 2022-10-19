#include "config.h"
#include <features.h>

#include "cpluginmanager/plugins.h"

#include <string>

#include "cpluginmanager/types/PluginDetails.h"
#include "PluginManager.hpp"

void cpluginmanager_register_plugin (const char* tag, const PluginDetails details) {
	pluginmanager::Manager::getInstance ().registerPlugin (std::string (tag), details);
}

void cpluginmanager_remove_plugin (const char* tag, const char* id) {
	pluginmanager::Manager::getInstance ().removePlugin (std::string (tag), std::string (id));
}
