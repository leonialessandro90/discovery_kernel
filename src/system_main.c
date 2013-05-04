#define SYSTEM_MAIN_H

#include "system_main.h"


uint8_t num_active_task = 0;



inline uint8_t find_free_desc_task()
{
	uint8_t i;
	for(i = 0; i<MAX_NUM_TASK; i++)
	{
		if(des_task[i].active == FALSE)
			return i;
	}
	return 255;
}

inline void terminate_task()
{
	in_esecuzione->active = FALSE;
	free(in_esecuzione->top_stack);

	asm("LDR PC, =dummy");

}


inline void activate_task(ADDR addr_fun, uint8_t priority, uint32_t param){


	ADDR sp_new_task;
	uint8_t free_desc_task;

	free_desc_task = find_free_desc_task();

	//ebp_new_task = MEMORY_BASE + EBP_BASE + ((num_active_task+1) * DIM_SINGLE_STACK);
	des_task[free_desc_task].top_stack = malloc(DIM_SINGLE_STACK);

	sp_new_task = des_task[free_desc_task].top_stack + DIM_SINGLE_STACK / 2; //DA TESTARE!!!

	des_task[free_desc_task].context.R0 = param;
	des_task[free_desc_task].context.SP = sp_new_task;
	des_task[free_desc_task].context.XPSR = 0x1000000;
	des_task[free_desc_task].context.LR = &terminate_task;
	des_task[free_desc_task].context.PC = addr_fun;

	des_task[free_desc_task].active = TRUE;
	des_task[free_desc_task].priority = priority;

	*(sp_new_task) = (REG) des_task[free_desc_task].context.R0;
	*(sp_new_task + 1) = (REG) des_task[free_desc_task].context.R1;
	*(sp_new_task + 2) = (REG) des_task[free_desc_task].context.R2;
	*(sp_new_task + 3) = (REG) des_task[free_desc_task].context.R3;
	*(sp_new_task + 4) = (REG) des_task[free_desc_task].context.R12;
	*(sp_new_task + 5) = (REG) des_task[free_desc_task].context.LR;
	*(sp_new_task + 6) = (REG) des_task[free_desc_task].context.PC;
	*(sp_new_task + 7) = (REG) des_task[free_desc_task].context.XPSR;


	num_active_task++;


}




inline des_task_block* scheduler(){
	static uint8_t i = 0;
	i = (++i)%num_active_task;
	return (&des_task[i]);
}

inline void init_desc_task()
{
	uint8_t i;
	for(i=0; i<MAX_NUM_TASK; i++)
		des_task[i].active = FALSE;
}


void dummy()
{
	for(;;);
}

int main(){

	init_leds();

	init_desc_task();


	user_main();

	init_timer();

	in_esecuzione = -1;


	//CARICA_STATO
	//asm("bx lr");


dummy();


}
