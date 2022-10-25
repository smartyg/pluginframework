#ifndef _PLUGINFRAMEWORK_EXAMPLE_CALC_MAMAGER_
#define _PLUGINFRAMEWORK_EXAMPLE_CALC_MAMAGER_

#define CALC_PLUGIN_MANAGER_TAG "CALC"

#ifdef __cplusplus
extern "C" {
#endif

	typedef int (*calc_type) (int, int);

	typedef struct {
		const char* id;
		calc_type calc_func;
	} handler_type;

	void calc_register_function (const char *id, void *ptr);
	void calc_deregister_function (const char* id);
	int calc (const char* id, int a, int b);

#ifdef __cplusplus
}
#endif

#endif /* _PLUGINFRAMEWORK_EXAMPLE_CALC_MAMAGER_ */