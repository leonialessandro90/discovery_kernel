#include "task.h"

uint8_t id = 0;		// task identifier


#ifdef _PERIODIC_TASK

void RM_List_Management(void){
	uint8_t i=0;
	uint8_t Nelem=list_count(ready);
	uint8_t Welem=list_count(waiting);
	des_task_block * des_task_i;
	for (i=0; i<Nelem; i++){
		des_task_i = (des_task_block *)list_getDescriptor(i,ready);
		if (des_task_i->nextActivation>0 && des_task_i->id!=0)
			des_task_i->nextActivation--;
	}
	for (i=0; i<Welem; i++){
		des_task_i = (des_task_block *)list_getDescriptor(i,waiting);
		if (des_task_i->nextActivation>0)
			des_task_i->nextActivation--;
		if (des_task_i->nextActivation==0){
			list_Transfer(&ready,&waiting,i);
			des_task_i->nextActivation=des_task_i->period;
		}
	}
}

void reactivate_periodic_task()
{
	register des_task_block * zombie = running;
	//ADDR * sp_zombie;

	asm("CPSID I");

	if (zombie->id==1){
		server_capacitance=SERVER_CAPACITANCE;
		dec_capacitance=0;
	}
	zombie->context.R0 = zombie->context_app.R0;
	zombie->context.SP = zombie->context_app.SP;
	zombie->context.LR = zombie->context_app.LR;
	zombie->context.PC = zombie->context_app.PC;
	zombie->nextActivation = zombie->period;

	list_insert_tail(&waiting,(void *)zombie);

	running = (des_task_block *)list_remove_head(&ready);

	if (running == null){
		running = (des_task_block *)dummy_list->elem;
		first=0;
	}

	LOAD_STATE
	if (running->swapped_from == INTERRUPT) {
		INT_TO_FUN
	}
	asm("CPSIE I");
	asm("POP {PC}");
}

void terminate_ape_request()
{
	register des_task_block * zombie;
	asm("CPSID I");
	num_ape_request--;
	zombie = running;
	running = (des_task_block *)list_remove_head(&ready);
	mem_free(zombie->top_stack);
	mem_free(zombie);
	LOAD_STATE
	if (running->swapped_from == INTERRUPT) {
		INT_TO_FUN
	}
	asm("CPSIE I");
	asm("POP {PC}");
}

void serve_ape_request(des_task_block * req)
{
	running->swapped_from = FUNCTION;
	periodic_list_insert(&ready, (void *)running);
	asm volatile("ADD SP, SP,#8");
	asm ("MOV R2,R7");
	SAVE_STATE_FROM_FUN
	asm volatile("MOV R7,R2");

	running=req;

	LOAD_STATE

	if( running->swapped_from == INTERRUPT )
		INT_TO_FUN
	asm("CPSIE I");
	asm("POP {PC}");

}
void dynamic_task_activate(TASK * addr_fun, uint8_t priority, uint32_t param, uint8_t period){


	ADDR * sp_new_task;			// stack pointer of the new task
	des_task_block * des_task;	// new task's descriptor

	des_task = (des_task_block *) mem_alloc(sizeof(des_task_block));

	des_task->top_stack = (STACK) mem_alloc(DIM_SINGLE_STACK);

	sp_new_task = (ADDR *) (des_task->top_stack + DIM_SINGLE_STACK - 32); //DA TESTARE!!!

	des_task->id = id++;
	des_task->priority = priority;
	des_task->period = period;
	des_task->nextActivation = period;
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
	des_task->context.LR = (uint32_t) &reactivate_periodic_task;
	des_task->context.PC = (uint32_t) addr_fun;
	des_task->context.XPSR = 0x1000000;

	des_task->context_app.R0 = param;
	des_task->context_app.SP = (REG) sp_new_task;
	des_task->context_app.LR = (uint32_t) &reactivate_periodic_task;
	des_task->context_app.PC = (uint32_t) addr_fun;

	num_active_task++;

	if (period < running->period){					//PREEMPTION
		running->swapped_from = FUNCTION;
				periodic_list_insert(&ready, (void *)running);
		asm volatile("ADD SP, SP,#24");
		asm ("MOV R2,R7");
		SAVE_STATE_FROM_FUN
		asm volatile("MOV R7,R2");

		running=des_task;

		LOAD_STATE

		if( running->swapped_from == INTERRUPT )
			INT_TO_FUN
		asm("CPSIE I");
		asm("POP {PC}");

	}

	else
		periodic_list_insert(&ready, (void *)des_task);

}

void send_ape_request(TASK * addr_fun, uint8_t priority, uint32_t param, uint8_t period)
{

	ADDR * sp_new_task;			// stack pointer of the new task
	des_task_block * des_task;	// new task's descriptor

	des_task = (des_task_block *) mem_alloc(sizeof(des_task_block));

	des_task->top_stack = (STACK) mem_alloc(DIM_SINGLE_STACK);

	sp_new_task = (ADDR *) (des_task->top_stack + DIM_SINGLE_STACK - 32); //DA TESTARE!!!

	des_task->id = id++;
	des_task->priority = priority;
	des_task->period = period;
	des_task->nextActivation = period;
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
	des_task->context.LR = (uint32_t) &terminate_ape_request;
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

	num_ape_request++;

	priority_list_insert(&ape_queue, (void *)des_task);
}
#endif


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

void activate_task(TASK * addr_fun, uint8_t priority, uint32_t param, uint8_t period)
{
	ADDR * sp_new_task;			// stack pointer of the new task
	des_task_block * des_task;	// new task's descriptor

	des_task = (des_task_block *) mem_alloc(sizeof(des_task_block));

	des_task->top_stack = (STACK) mem_alloc(DIM_SINGLE_STACK);

	sp_new_task = (ADDR *) (des_task->top_stack + DIM_SINGLE_STACK - 32); //DA TESTARE!!!

	des_task->id = id++;
	des_task->priority = priority;
	des_task->period = period;
	des_task->nextActivation = period;
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
#ifdef _PERIODIC_TASK
	des_task->context.LR = (uint32_t) &reactivate_periodic_task;

	des_task->context_app.R0 = param;
	des_task->context_app.SP = (REG) sp_new_task;
	des_task->context_app.LR = (uint32_t) &reactivate_periodic_task;
	des_task->context_app.PC = (uint32_t) addr_fun;
#else
	des_task->context.LR = (uint32_t) &terminate_task;
#endif
	des_task->context.PC = (uint32_t) addr_fun;
	des_task->context.XPSR = 0x01000000;



	*(sp_new_task) = (REG) des_task->context.R0;
	*(sp_new_task + 1) = (REG) des_task->context.R1;
	*(sp_new_task + 2) = (REG) des_task->context.R2;
	*(sp_new_task + 3) = (REG) des_task->context.R3;
	*(sp_new_task + 4) = (REG) des_task->context.R12;
	*(sp_new_task + 5) = (REG) des_task->context.LR;
	*(sp_new_task + 6) = (REG) des_task->context.PC;
	*(sp_new_task + 7) = (REG) des_task->context.XPSR;

	num_active_task++;
	#ifndef	_PERIODIC_TASK
		list_insert_tail(&ready, (void *)des_task);
	#else
		if (des_task->id==1)
				server_capacitance=SERVER_CAPACITANCE;
			if (des_task->id==0){
				periodic_list_insert(&dummy_list, (void *)des_task);
				running = des_task;
			}
			else

				periodic_list_insert(&ready, (void *)des_task);
	#endif


}


inline des_task_block * RR_scheduler()
{
	register des_task_block* next;
	if (running)
		list_insert_tail(&ready, running);
	next = (des_task_block*) list_remove_head(&ready);
	if(num_active_task > 1 && next->id == DUMMY) {
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
