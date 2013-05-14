#ifndef SYSTEM_MAIN_H_
#define SYSTEM_MAIN_H_

#define SCHEDULER RR_scheduler

#include "types.h"

#include "memory.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "leds.h"
#include "list.h"

#define INT_TO_FUN\
		asm("POP {R0}");\
		asm("STR R0, [SP, #28]");\
		asm("POP {R0-R3}");\
		asm("ADD SP, SP, #12");


#define FUN_TO_INT\
		asm("SUB SP, SP, #8");\
		asm("LDR LR, [R1, #56]");\
		asm("PUSH {LR,R12,R3,R2,R1,R0}");\
		asm("MOV LR, 0xFFFFFFF9");\
		asm("LDR R0, [R1, #60]");\
		asm("STR R0, [SP, #28]");\
		asm("LDR R0, [R1, #64]");\
		asm("STR R0, [SP, #32]");

#define SAVE_STATE_FROM_INT\
		asm("POP {R7}");\
		asm("ADD SP, SP, #4");\
		asm("LDR R1, =running");\
		asm("LDR R1, [R1, 0]");\
		asm("STR R4, [R1, #16]");\
		asm("STR R5, [R1, #20]");\
		asm("STR R6, [R1, #24]");\
		asm("STR R7, [R1, #28]");\
		asm("STR R8, [R1, #32]");\
		asm("STR R9, [R1, #36]");\
		asm("STR R10, [R1, #40]");\
		asm("STR R11, [R1, #44]");\
		asm("STR SP, [R1, #52]");\
		\
		asm("LDR R0, [SP, #0]");\
		asm("STR R0, [R1, #0]");\
		asm("LDR R0, [SP, #4]");\
		asm("STR R0, [R1, #4]");\
		asm("LDR R0, [SP, #8]");\
		asm("STR R0, [R1, #8]");\
		asm("LDR R0, [SP, #12]");\
		asm("STR R0, [R1, #12]");\
		asm("LDR R0, [SP, #16]");\
		asm("STR R0, [R1, #48]");\
		\
		asm("LDR R0, [SP, #20]");\
		asm("STR R0, [R1, #56]");\
		asm("LDR R0, [SP, #24]");\
		asm("STR R0, [R1, #60]");\
		asm("LDR R0, [SP, #28]");\
		asm("STR R0, [R1, #64]");

//i don't need to store the value of LR because of it is saved
//in the stack of the function that called this macro
#define SAVE_STATE_FROM_FUN\
		asm("POP {R7,LR}");\
		asm("LDR R1, =running");\
		asm("LDR R1, [R1, 0]");\
		asm("STR R4, [R1, #16]");\
		asm("STR R5, [R1, #20]");\
		asm("STR R6, [R1, #24]");\
		asm("STR R7, [R1, #28]");\
		asm("STR R8, [R1, #32]");\
		asm("STR R9, [R1, #36]");\
		asm("STR R10, [R1, #40]");\
		asm("STR R11, [R1, #44]");\
		asm("STR R12, [R1, #48]");\
		asm("STR SP, [R1, #52]");\
		asm("STR LR, [R1, #60]");\
		asm("MRS R0, XPSR");\
		asm("STR R0, [R1, #64]");

#define LOAD_STATE\
		asm("LDR R1, =running");\
		asm("LDR R1, [R1,0]");\
		\
		asm("LDR R4, [R1, #16]");\
		asm("LDR R5, [R1, #20]");\
		asm("LDR R6, [R1, #24]");\
		asm("LDR R7, [R1, #28]");\
		asm("LDR R8, [R1, #32]");\
		asm("LDR R9, [R1, #36]");\
		asm("LDR R10, [R1, #40]");\
		asm("LDR R11, [R1, #44]");\
		asm("LDR R12, [R1, #48]");\
		asm("LDR SP, [R1, #52]");\
		asm("LDR LR, [R1, #56]");\
		asm("LDR R0, [R1, #64]");\
		asm("MSR APSR, R0");\
		asm("LDR R0, [R1, #60]");\
		asm("ORR R0, R0, #0x1");\
		asm("PUSH {R0}");

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
