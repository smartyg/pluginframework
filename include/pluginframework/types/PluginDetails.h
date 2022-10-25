#ifndef _PLUGINFRAMEWORK_TYPES_PLUGINDETAILS_
#define _PLUGINFRAMEWORK_TYPES_PLUGINDETAILS_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		const char* id;
		const char* name;
		const char* author;
		const char* license;
		uint8_t major_version;
		uint8_t minor_version;
		void* data;
	} PluginDetails;

#ifdef __cplusplus
}
#endif

#endif /* _PLUGINFRAMEWORK_TYPES_PLUGINDETAILS_ */