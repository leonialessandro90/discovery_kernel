#include "sem.h"

void sem_insert_queue(des_task_block* src)
{
	des_task_block* p = src;
	if(src == 0){
		src = running;
	}else {
		while(p->next_task != 0)
			p = p->next_task;
		p->next_task = running;
	}
	src->next_task = 0;
}

des_task_block* sem_remove_queue(des_task_block* src)
{
	des_task_block* p = src;
	if (src == 0)
		return NULL;
	src = src->next_task;
	return p;
}



void sem_init(sem* s, uint8_t param)
{
	s->value = param;
	s->queue = 0;
}

void sem_wait(sem* s)
{

	asm("CPSID I");
	asm("PUSH {LR}");

	s->value--;
	if (s->value >= 0)
	{
		asm("CPSIE I");
		return;
	}

	sem_insert_queue(s->queue);

	SALVA_STATO
	running = SCHEDULER();
	CARICA_STATO
	asm("CPSIE I");
	asm("bx lr");
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

	sem_insert_queue(s->queue);

	SALVA_STATO
	running = SCHEDULER();
	CARICA_STATO
	asm("CPSIE I");
	asm("bx lr");
}
