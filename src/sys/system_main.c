#include "system_main.h"

uint8_t num_active_task = 0;

inline void terminate_task()
{
	register list * zombie;
	asm("CPSID I");
	num_active_task--;
	zombie = list_remove(&ready);
	mem_free(((des_task_block *)(zombie->elem))->top_stack);
	mem_free(zombie);
	running = SCHEDULER();
	LOAD_STATE
	if( running->swapped_from == INTERRUPT){
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
	list_insert(&ready, (void *)des_task);
}

void wait(uint32_t numberOfCycles)
{
	while (numberOfCycles != 0)
		numberOfCycles--;
}

inline des_task_block * RR_scheduler()
{
	register des_task_block * next = (des_task_block *)list_round_shift(&ready);
	if (num_active_task > 1 && !next->id)
		return (des_task_block *)list_round_shift(&ready);
	else
		return next;
}

TASK dummy(int p) { for (;;) ; }

void SystemInit()
{
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON); // Power on external crystal 8MHz
	RCC_WaitForHSEStartUp(); // Wait stabilization
	// Setup PLL (SOURCE/PLLM)*PLLN = (8/8)*336
	// SYSCLK = PLL/PLLP = 336/2=168, OTG-SDIO CLK = PLL/PLLQ = 336/7=48MHz
	RCC_PLLConfig (RCC_PLLSource_HSE, 8, 336 , 2, 7);
	RCC_PLLCmd(ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); // CPU clock = SYSCLK
	RCC_PCLK2Config(RCC_HCLK_Div2); // SPCLK2 = SYSCLK /2 = 84 MHz
	RCC_PCLK1Config(RCC_HCLK_Div4); // SPCLK1 = SYSCLK /2 = 42 MHz
	FLASH_PrefetchBufferCmd(ENABLE); // Enable Flash prefetch
	FLASH_SetLatency(FLASH_Latency_5);// Set Flash latency
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ; // WAIT PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Switch clocksource
	while(RCC_GetSYSCLKSource() != 0x08) ; // wait switch complete
}

void sys_init()
{
	mem_init();
	activate_task(&dummy, 10, 50);
	running = SCHEDULER();
	led_init();
}

void activate_dummy()
{
	LOAD_STATE
	INT_TO_FUN
	asm("POP {PC}");
}

int main()
{
	sys_init();
	user_main();
	init_timer();
	activate_dummy();

	for(;;);
}
