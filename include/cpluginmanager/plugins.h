#ifndef _LIBCPLUGINMANAGER_PLUGINS_
#define _LIBCPLUGINMANAGER_PLUGINS_

#include <cpluginmanager/types/PluginDetails.h>

#ifdef __cplusplus
extern "C" {
#endif

	void cpluginmanager_register_plugin (const char* tag, const PluginDetails details);
	void cpluginmanager_remove_plugin (const char* tag, const char* id);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCPLUGINMANAGER_PLUGINS_ */