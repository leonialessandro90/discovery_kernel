#define _PERIODIC_TASK

#ifndef TASK_H_
#define TASK_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "types.h"
#include "list.h"
#include "macros.h"

#define SCHEDULER RR_scheduler

extern uint8_t first;



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

typedef struct context_app_type_t {
	uint32_t R0;
	uint32_t R13;	//SP
	uint32_t R14;	//LR
	uint32_t R15;	//PC  +60
} context_app_type;


typedef struct des_task_type_t {
	context_type context;
	context_app_type context_app;
	uint8_t id;
	uint8_t priority;
	uint8_t period;
	uint8_t nextActivation;
	STACK top_stack;
	TASK* next_task;
	swapped_out_from swapped_from;
} des_task_block;

des_task_block * running; 	// currently running task
list * ready;				// head of ready processes queue
list * waiting;
list * ape_queue;
list * dummy_list;

uint8_t num_active_task;
uint8_t num_ape_request;
uint8_t dec_capacitance;
uint8_t server_capacitance;

#ifdef _PERIODIC_TASK
void RM_List_Management(void);
void dynamic_task_activate(TASK * , uint8_t , uint32_t , uint8_t);
void reactivate_periodic_task();
void send_ape_request(TASK * , uint8_t , uint32_t, uint8_t);
void serve_ape_request(des_task_block * );
void terminate_ape_request();
#endif

void terminate_task();
void activate_task(TASK *, uint8_t, uint32_t, uint8_t);
des_task_block * RR_scheduler();


#endif
