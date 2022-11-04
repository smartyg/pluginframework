#ifndef _PLUGINFRAMEWORK_PLUGINS_
#define _PLUGINFRAMEWORK_PLUGINS_

#include <pluginframework/types/PluginDetails.h>

#ifdef __cplusplus
extern "C" {
#endif

	void pluginframework_register_plugin (const char* tag, const PluginDetails details);
	void pluginframework_remove_plugin (const char* tag, const char* id);

#ifdef __cplusplus
}
#endif

#endif /* _PLUGINFRAMEWORK_PLUGINS_ */