#ifndef SYSTEM_MAIN_H_
#define SYSTEM_MAIN_H_

#define SCHEDULER RR_scheduler


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "macros.h"

#include "types.h"
#include "memory.h"

#include "leds.h"
#include "list.h"


typedef struct context_type_t {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;	//EBP
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t R13;	//SP
	uint32_t R14;	//LR
	uint32_t R15;	//PC  +60
	uint32_t R16;
} context_type;

typedef enum swapped_out_from_t {
	FUNCTION,
	INTERRUPT
} swapped_out_from;

typedef struct des_task_type_t {
	context_type context;
	uint8_t id;
	uint8_t priority;
	STACK top_stack;
	TASK* next_task;
	swapped_out_from swapped_from;
} des_task_block;

des_task_block * running; // currently running task
list * ready;		// head of ready processes queue

void user_main();
void start_task(ADDR addr_fun, uint32_t param);
void init_timer();
void activate_task(TASK* addr_fun, uint8_t priority, uint32_t param);
inline des_task_block * SCHEDULER ();
void wait(uint32_t numberOfCycles);

#endif /* SYSTEM_MAIN_H_ */
