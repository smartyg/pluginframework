#include "config.h"
#include <features.h>

#include <cpluginmanager/plugins.h>

#include "calc_manager.h"

#define PLUGIN_ID "ADD"

void __attribute__((constructor)) register_handler1 (void);
void __attribute__((destructor)) remove_handler1 (void);
int calc_function_add (int a, int b);

void register_handler1 (void) {
	PluginDetails details;

	details.id = PLUGIN_ID;
	details.name = "calc add plugin";
	details.author = "Martijn Goedhart";
	details.license = "GPLv3.0+";
	details.major_version = 0;
	details.minor_version = 1;
	details.data = &calc_function_add;

	cpluginmanager_register_plugin (CALC_PLUGIN_MANAGER_TAG, details);
}

void remove_handler1 (void) {
	cpluginmanager_remove_plugin (CALC_PLUGIN_MANAGER_TAG, PLUGIN_ID);
}

int calc_function_add (int a, int b) {
	return a + b;
}