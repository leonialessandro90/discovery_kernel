#ifndef SEM_H_
#define SEM_H_

#include "system_main.h"

#include "macros.h"
#include "task.h"

typedef struct sem_type_t{
	int8_t value;
	list* queue;
} sem;

void sem_init(sem* s, int8_t param);
void sem_wait(sem* s);
void sem_signal(sem* s);

#endif
