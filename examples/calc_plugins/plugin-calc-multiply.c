#include "config.h"
#include <features.h>

#include <pluginframework/plugins.h>

#include "calc_manager.h"

#define PLUGIN_ID "MULTIPLY"

void __attribute__((constructor)) register_handler (void);
void __attribute__((destructor)) remove_handler (void);
int calc_function_multiply (int a, int b);



void register_handler (void) {
	PluginDetails details;

	details.id = PLUGIN_ID;
	details.name = "calc multiply plugin";
	details.author = "Martijn Goedhart";
	details.license = "GPLv3.0+";
	details.major_version = 0;
	details.minor_version = 1;
	details.data = &calc_function_multiply;

	pluginframework_register_plugin (CALC_PLUGIN_MANAGER_TAG, details);
}

void remove_handler (void) {
	pluginframework_remove_plugin (CALC_PLUGIN_MANAGER_TAG, PLUGIN_ID);
}

int calc_function_multiply (int a, int b) {
	return a * b;
}