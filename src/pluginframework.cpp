#include "config.h"

#include "pluginframework.h"

#include <string>

void pluginframework_add_manager (const char* tag, register_function refister_func, deregister_function deregister_func) {
	pluginframework::Controller::getInstance ().addManager (std::string (tag), refister_func, deregister_func);
}

void pluginframework_scan_directory (const char* directory) {
	pluginframework::Controller::getInstance ().scanDirectory (std::string (directory));
}
