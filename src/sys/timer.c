#include "system_main.h"
#include "task.h"


uint16_t count = 0;
uint8_t first = 0;
#ifdef _PERIODIC_TASK


void stack_reset(){

	register des_task_block * zombie = running;
	ADDR * sp_zombie;

	sp_zombie = (ADDR *) running->context.R13;
	*(sp_zombie) = (REG) zombie->context.R0;
	*(sp_zombie + 1) = (REG) zombie->context.R1;
	*(sp_zombie + 2) = (REG) zombie->context.R2;
	*(sp_zombie + 3) = (REG) zombie->context.R3;
	*(sp_zombie + 4) = (REG) zombie->context.R12;
	*(sp_zombie + 5) = (REG) zombie->context.R14;
	*(sp_zombie + 6) = (REG) zombie->context.R15;
	*(sp_zombie + 7) = (REG) zombie->context.R16;

}

void SysTick_Handler(void)
{

	asm("CPSID I");

	count++;
	if (count==100) {
		count = 0;
		RM_List_Management();
		if (dec_capacitance==1)
			server_capacitance--;
		if (first==0 && list_count(ready)!=0){
				first=1;
				SAVE_STATE_FROM_INT
				running = (des_task_block*) list_remove_head(&ready);
				stack_reset();
				LOAD_STATE
				if (running->swapped_from==FUNCTION)
					FUN_TO_INT
				else
					asm("POP {R0}");
				asm("MOV LR, 0xFFFFFFF9");
				asm("CPSIE I");
				asm("BX LR");
		}

	}
	asm("CPSIE I");
}
#else

void SysTick_Handler(void)
{
	asm("CPSID I");
	count++;
	if (count==100) {
		count = 0;
		SAVE_STATE_FROM_INT
		running->swapped_from = INTERRUPT;
		running = SCHEDULER();
		LOAD_STATE
		if (running->swapped_from == FUNCTION) {
			FUN_TO_INT
		} else {
			asm("POP {R0}");
		}
		asm("MOV LR, 0xFFFFFFF9");
		asm("CPSIE I");
		asm("BX LR");
	}
	asm("CPSIE I");
}
#endif

void init_timer()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SysTick_Config(168000);
}
