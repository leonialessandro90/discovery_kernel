#include "system_main.h"

uint8_t num_active_task = 0;

inline void terminate_task()
{
	asm("CPSID I");

	num_active_task--;
	free(running->top_stack);
	//free(running);
	ready = ready->next;
	running = ready->elem;
	running = SCHEDULER();
	//c'è da fare la free, state calmi
//	list_delete_tail(&ready);
	CARICA_STATO
	INT_TO_FUN
	asm("CPSIE I");
	asm("add r0, r0, #1");
	asm("bx r0");
	/*asm("POP {R7}");
	asm("POP {R1}");
	asm("PUSH {LR}");
	asm("PUSH {R7}");*/

}

void activate_task(TASK* addr_fun, uint8_t priority, uint32_t param)
{
	static uint8_t id = 0;
	ADDR * sp_new_task;
	des_task_block * des_task = (des_task_block *) malloc(sizeof(des_task_block));

	list_insert(&ready, (void *)des_task);

	des_task->top_stack = malloc(DIM_SINGLE_STACK);

	sp_new_task = des_task->top_stack + DIM_SINGLE_STACK / 2; //DA TESTARE!!!

	des_task->id = id++;
	des_task->context.R0 = param;
	des_task->context.SP = (REG) sp_new_task;
	des_task->context.XPSR = 0x1000000;
	des_task->context.LR = (uint32_t)&terminate_task;
	des_task->context.PC = addr_fun;

	des_task->priority = priority;

	*(sp_new_task) = (REG) des_task->context.R0;
	*(sp_new_task + 1) = (REG) des_task->context.R1;
	*(sp_new_task + 2) = (REG) des_task->context.R2;
	*(sp_new_task + 3) = (REG) des_task->context.R3;
	*(sp_new_task + 4) = (REG) des_task->context.R12;
	*(sp_new_task + 5) = (REG) des_task->context.LR;
	*(sp_new_task + 6) = (REG) des_task->context.PC;
	*(sp_new_task + 7) = (REG) des_task->context.XPSR;

	num_active_task++;
}


inline des_task_block * RR_scheduler()
{
	register des_task_block * next;
	//if(running == null)
	//	list_insert(&ready, (void *)running);

	next = (des_task_block *)list_round_shift(&ready);

	if(num_active_task > 1 && next->id == 0) {
		next = (des_task_block *)list_round_shift(&ready);
	}
	return next;
}


void sys_init()
{
	led_init();
}

TASK dummy(int p) { for (;;) ; }

void activate_dummy()
{
	CARICA_STATO
	INT_TO_FUN
	asm("bx r0");
}




int main()
{

	sys_init();

	activate_task(&dummy, 10, 50);

	running = SCHEDULER();

	user_main();

	init_timer();

	activate_dummy();

	for(;;);
}
