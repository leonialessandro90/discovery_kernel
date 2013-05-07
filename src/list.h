#ifndef LIST_H_
#define LIST_H_

#include <malloc.h>
#include "types.h"

typedef struct list_t {
	void * elem;
	struct list_t * next;
} list;

void list_insert(list ** lst, void * elem);
void * list_remove(list ** lst);

#endif /* LIST_H_ */
