#include "sem.h"

void sem_init(sem* s, int8_t param)
{
	s->value = param;
	s->queue = null;
}

void sem_wait(sem* s)
{
	asm("CPSID I");

	s->value--;
	if (s->value >= 0)
	{
		asm("CPSIE I");
		return;
	}
	//sem_insert_queue(s);
	running->swapped_from = FUNCTION;
	list_insert_tail(&(s->queue), (void*) running);
	asm volatile (
		"ADD SP, SP, #8"
	);		//Removing sem_wait params

	SAVE_STATE_FROM_FUN
	running = null;   //running is empty
	running = SCHEDULER();
	LOAD_STATE
	if( running->swapped_from == INTERRUPT ){
		INT_TO_FUN
	}
	num_active_task--;
	asm("CPSIE I");
	asm("POP {PC}");
}


void sem_signal(sem* s)
{
	des_task_block* resumed;
	asm("CPSID I");

	s->value++;
	if (s->value > 0)
	{
		asm("CPSIE I");
		return;
	}
	resumed = (des_task_block *) list_remove_head(&s->queue);
	list_insert_tail(&ready, resumed);
	num_active_task++;
	asm("CPSIE I");
}
