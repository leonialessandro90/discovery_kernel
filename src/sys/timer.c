#include "system_main.h"

uint16_t count = 0;


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

void init_timer()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SysTick_Config(168000);
}
