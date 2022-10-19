#include "config.h"
#include <features.h>
#include <stdio.h>
#include <cpluginmanager.h>

#include "calc_plugins/calc_manager.h"

int main (void) {
	cpluginmanager_addManager (CALC_PLUGIN_MANAGER_TAG, &calc_register_function, &calc_deregister_function);

	cpluginmanager_scanDirectory ("./.libs");
	cpluginmanager_scanDirectory ("./");

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