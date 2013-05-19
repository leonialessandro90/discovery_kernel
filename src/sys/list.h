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

uint8_t list_count(list* lst);
void list_Transfer(list ** ready, list ** waiting, uint8_t index);
void * list_getDescriptor(uint8_t index, list * lst);
void periodic_list_insert(list ** lst, void * elem);
void priority_list_insert(list ** lst, void * elem);
#endif /* LIST_H_ */
