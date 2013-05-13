#include "sem.h"

void sem_insert_queue(sem * s)
{
	des_task_block* p = s->queue;
	if(s->queue == null){
		s->queue = running;
	}else {
		while(p->next_task != null)
			p = p->next_task;
		p->next_task = running;
	}
	running->next_task = null;
}

des_task_block* sem_remove_queue(sem * s)
{
	des_task_block* p = s->queue;
	if (s->queue == null)
		return NULL;
	s->queue = s->queue->next_task;
	return p;
}


void sem_init(sem* s, int8_t param)
{
	s->value = param;
	s->queue = null;
}

void sem_wait(sem* s)
{
	asm("CPSID I");
	//asm("PUSH {LR}");

	s->value--;
	if (s->value >= 0)
	{
		asm("CPSIE I");
		return;
	}

	sem_insert_queue(s);

	SAVE_STATE_FROM_FUN
	running->swapped_from = FUNCTION;
	running = SCHEDULER();
	LOAD_STATE
	if( running->swapped_from == INTERRUPT ){
		INT_TO_FUN
	}
	asm("CPSIE I");
	asm("BX R0");
}


void sem_signal(sem* s)
{
	asm("CPSID I");
	asm("PUSH {LR}");

	s->value++;
	if (s->value > 0)
	{
		asm("CPSIE I");
		return;
	}
/*
	sem_insert_queue(s);

	SAVE_STATE_FROM_FUN
	running->swapped_from = FUNCTION;
	running = SCHEDULER();
	LOAD_STATE
	asm("CPSIE I");
	asm("BX R0");
*/
}

