#ifndef _PLUGINFRAMEWORK_PLUGINFRAMEWORK_
#define _PLUGINFRAMEWORK_PLUGINFRAMEWORK_

#ifdef __cplusplus
extern "C" {
#endif

	typedef void (*register_function)(const char*, void *);
	typedef void (*deregister_function)(const char* id);

	void pluginframework_add_manager (const char* tag, register_function, deregister_function);

	void pluginframework_scan_directory (const char* directory);

#ifdef __cplusplus
}

#include <pluginframework/Controller.hpp>

#endif

#endif /* _PLUGINFRAMEWORK_PLUGINFRAMEWORK_ */