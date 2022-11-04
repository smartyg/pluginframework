#include "config.h"

#include "pluginframework/plugins.h"

#include <string>

#include "pluginframework/types/PluginDetails.h"
#include "pluginframework.h"

void pluginframework_register_plugin (const char* tag, const PluginDetails details) {
	pluginframework::Controller::getInstance ().registerPlugin (std::string (tag), details);
}

void pluginframework_remove_plugin (const char* tag, const char* id) {
	pluginframework::Controller::getInstance ().removePlugin (std::string (tag), std::string (id));
}
