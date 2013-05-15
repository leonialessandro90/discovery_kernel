#ifndef LIST_H_
#define LIST_H_

#include "types.h"
#include "memory.h"

typedef struct list_t {
	void * elem;
	struct list_t * next;
} list;

void list_insert_tail(list ** lst, void * elem);
void * list_remove_head(list ** lst);
void * list_round_shift(list ** lst);
void * list_remove_tail(list ** lst);

#endif /* LIST_H_ */
