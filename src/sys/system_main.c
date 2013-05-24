#include "system_main.h"

TASK dummy(int p) { for (;;) ; }

#ifdef _PERIODIC_TASK
TASK server(int p) {
	des_task_block* next;
	while (num_ape_request!=0 && server_capacitance>0){
		dec_capacitance=1;
		next=(des_task_block *)list_remove_head(&ape_queue);
		serve_ape_request(next);
	}
}

void activate_server(){
	activate_task(&server, 2, 0, 5);
}
#endif

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
	num_active_task = 0;
	activate_task(&dummy, 10, 50, 255);
#ifndef _PERIODIC_TASK
	running = SCHEDULER();
#endif
	led_init();
}

void activate_dummy()
{
	asm("CPSID I");
	LOAD_STATE
	INT_TO_FUN
	asm("CPSIE I");
	asm("POP {PC}");
}

int main()
{
	sys_init();
#ifdef _PERIODIC_TASK
	activate_server();
#endif
	user_main();
	init_timer();
	activate_dummy();
	for(;;);
}
