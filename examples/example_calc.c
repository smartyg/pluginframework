#include "config.h"
#include <features.h>
#include <stdio.h>
#include <pluginframework.h>

#include "calc_plugins/calc_manager.h"

int main (void) {
	pluginframework_add_manager (CALC_PLUGIN_MANAGER_TAG, &calc_register_function, &calc_deregister_function);

	pluginframework_scan_directory ("./.libs");
	pluginframework_scan_directory ("./");

	{
		int result;
		result = calc ("ADD", 10, 21);
		printf ("Result of calculation is: %d\n", result);
	}

	{
		int result;
		result = calc ("ADD", 10, -2);
		printf ("Result of calculation is: %d\n", result);
	}

	{
		int result;
		result = calc ("MULTIPLY", 10, 21);
		printf ("Result of calculation is: %d\n", result);
	}
}