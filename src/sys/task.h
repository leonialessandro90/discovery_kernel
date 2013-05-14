#ifndef TASK_H_
#define TASK_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "types.h"
#include "list.h"
#include "macros.h"

#define SCHEDULER RR_scheduler


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
uint8_t num_active_task;

void terminate_task();
void activate_task(TASK *, uint8_t, uint32_t);
des_task_block * RR_scheduler();

#endif
