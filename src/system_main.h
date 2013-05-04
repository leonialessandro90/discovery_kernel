#ifndef SYSTEM_MAIN_H_
#define SYSTEM_MAIN_H_

#define SCHEDULER RR_scheduler

#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "leds.h"

#define SALVA_STATO asm("POP {R7}");\
		asm("POP {LR}");\
		asm("LDR R1, =running");\
		asm("LDR R1, [R1, 0]");\
		asm("ADD R1, R1, #8");\
		asm("STR R4, [R1, #16]");\
		asm("STR R5, [R1, #20]");\
		asm("STR R6, [R1, #24]");\
		asm("STR R7, [R1, #28]");\
		asm("STR R8, [R1, #32]");\
		asm("STR R9, [R1, #36]");\
		asm("STR R10, [R1, #40]");\
		asm("STR R11, [R1, #44]");\
		asm("STR SP, [R1, #52]");\
		//asm("STR LR, [R1, #60]");

#define CARICA_STATO	asm("LDR R0, =running");\
		asm("LDR R0, [R0,0]");\
		asm("ADD R0, R0, #8");\
		asm("LDR R4, [R0, #16]");\
		asm("LDR R5, [R0, #20]");\
		asm("LDR R6, [R0, #24]");\
		asm("LDR R7, [R0, #28]");\
		asm("LDR R8, [R0, #32]");\
		asm("LDR R9, [R0, #36]");\
		asm("LDR R10, [R0, #40]");\
		asm("LDR R11, [R0, #44]");\
		asm("LDR R12, [R0, #48]");\
		asm("LDR SP, [R0, #52]");\
		//asm("LDR LR, [R0, #60]");

#define ADDR uint32_t*
#define THREAD void*
#define REG uint32_t
#define MEMORY_BASE 0x2001fff0
#define EBP_BASE 0x0
#define DIM_SINGLE_STACK 5000
#define MAX_NUM_TASK 10
#define TASK_ID uint8_t
//#define TASK void
#define BOOL uint8_t
#define TRUE 1
#define FALSE 0

#define BP R7
#define SP R13
#define PC R15
#define LR R14
#define XPSR R16

typedef struct context_type_t{
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
	uint32_t R15;	//PC
	uint32_t R16;
} context_type;

typedef struct des_task_type_t{
	uint8_t id;
	BOOL active;
	uint8_t priority;
	ADDR top_stack;
	context_type context;
	ADDR next_task;
} des_task_block;

int cane;
//des_task_type* des_task;
des_task_block des_task[MAX_NUM_TASK];
ADDR global_addr_carica_stato;
TASK_ID current_task;
des_task_block* running;

void user_main();
void salva_stato();
void carica_stato();
des_task_block* scheduler();
void start_task(ADDR addr_fun, uint32_t param);
void init_timer();
void activate_task(THREAD addr_fun, uint8_t priority, uint32_t param);
inline des_task_block * SCHEDULER ();


#endif /* SYSTEM_MAIN_H_ */
