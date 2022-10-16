#include "config.h"
#include <features.h>

#include "calc_manager.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static handler_type *registered_handlers = NULL;
static size_t registered_handlers_size = 0;

void calc_register_function (const char *id, void *ptr) {
	printf ("pointer to block is: %p; size: %ld\n", registered_handlers, registered_handlers_size);
	handler_type* tmp = (handler_type*)realloc (registered_handlers, (registered_handlers_size + 1) * sizeof (handler_type));
	if (tmp != NULL) {
		registered_handlers = tmp;
		handler_type *entry = (handler_type*)(registered_handlers + registered_handlers_size);
		registered_handlers_size++;
		printf ("pointer to block is: %p; size: %ld\n", registered_handlers, registered_handlers_size);
		printf ("pointer entry id: %p\n", entry);
		entry->id = id;
		entry->calc_func = (calc_type)ptr;
	}
}

void calc_deregister_function (const char* id) {
	// removing a entry from a malloc/realloc space is very much work, just NULLing the id pointer also works.
	size_t i = 0;
	while (i < registered_handlers_size) {
		handler_type *entry = registered_handlers + i;
		if (strcmp (entry->id, id) == 0) {
			entry->id = NULL; // This also makes the function unavailable.
			return;
		}
		i++;
	}
}

int calc (const char* id, int a, int b) {
	// This function just returns 0 in case of error (non-valid `id`), a real function should have propper error handling.
	if (id == NULL) return 0;
	size_t i = 0;
	while (i < registered_handlers_size) {
		handler_type *entry = registered_handlers + i;
		if (strcmp (entry->id, id) == 0)
			return entry->calc_func (a, b);
		i++;
	}
	return 0;
}