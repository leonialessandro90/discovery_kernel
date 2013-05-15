#include "task.h"

inline void terminate_task()
{
	register des_task_block * zombie;
	asm("CPSID I");
	num_active_task--;
	zombie = running;
	running = null;
	mem_free(zombie->top_stack);
	mem_free(zombie);
	running = SCHEDULER();
	LOAD_STATE
	if (running->swapped_from == INTERRUPT) {
		INT_TO_FUN
	}
	asm("CPSIE I");
	asm("POP {PC}");
}

void activate_task(TASK * addr_fun, uint8_t priority, uint32_t param)
{
	static uint8_t id = 0;		// task identifier
	ADDR * sp_new_task;			// stack pointer of the new task
	des_task_block * des_task;	// new task's descriptor

	des_task = (des_task_block *) mem_alloc(sizeof(des_task_block));

	des_task->top_stack = (STACK) mem_alloc(DIM_SINGLE_STACK);

	sp_new_task = (ADDR *) (des_task->top_stack + DIM_SINGLE_STACK - 32); //DA TESTARE!!!

	des_task->id = id++;
	des_task->priority = priority;
	des_task->swapped_from = INTERRUPT;

	des_task->context.R0 = param;
	des_task->context.R1 = 0;
	des_task->context.R2 = 0;
	des_task->context.R3 = 0;
	des_task->context.R4 = 0;
	des_task->context.R5 = 0;
	des_task->context.R6 = 0;
	des_task->context.R7 = 0;
	des_task->context.R8 = 0;
	des_task->context.R9 = 0;
	des_task->context.R10 = 0;
	des_task->context.R11 = 0;
	des_task->context.R12 = 0;

	des_task->context.SP = (REG) sp_new_task;
	des_task->context.LR = (uint32_t) &terminate_task;
	des_task->context.PC = (uint32_t) addr_fun;
	des_task->context.XPSR = 0x1000000;

	*(sp_new_task) = (REG) des_task->context.R0;
	*(sp_new_task + 1) = (REG) des_task->context.R1;
	*(sp_new_task + 2) = (REG) des_task->context.R2;
	*(sp_new_task + 3) = (REG) des_task->context.R3;
	*(sp_new_task + 4) = (REG) des_task->context.R12;
	*(sp_new_task + 5) = (REG) des_task->context.LR;
	*(sp_new_task + 6) = (REG) des_task->context.PC;
	*(sp_new_task + 7) = (REG) des_task->context.XPSR;

	num_active_task++;
	list_insert_tail(&ready, (void *)des_task);
}


inline des_task_block * RR_scheduler()
{
	register des_task_block* next;
	if (running)
		list_insert_tail(&ready, running);
	next = (des_task_block*) list_remove_head(&ready);
	if(num_active_task > 1 && !next->id) {  //dummy has id=0
		list_insert_tail(&ready, next);
		next = (des_task_block*)list_remove_head(&ready);
	}
	return next;
}

void wait(uint32_t numberOfCycles)
{
	while (numberOfCycles != 0)
		numberOfCycles--;
}
