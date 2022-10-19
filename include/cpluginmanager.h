#ifndef _LIBCPLUGINMANAGER_CPLUGINMANAGER_
#define _LIBCPLUGINMANAGER_CPLUGINMANAGER_

#ifdef __cplusplus
extern "C" {
#endif

	typedef void (*register_function)(const char*, void *);
	typedef void (*deregister_function)(const char* id);

	void cpluginmanager_addManager (const char* tag, register_function, deregister_function);

	void cpluginmanager_scanDirectory (const char* directory);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCPLUGINMANAGER_CPLUGINMANAGER_ */