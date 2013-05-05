#ifndef TYPES_H_
#define TYPES_H_

#define null 0

#define ADDR uint32_t
#define REG uint32_t
#define MEMORY_BASE 0x2001fff0
#define EBP_BASE 0x0
#define DIM_SINGLE_STACK 5000
#define MAX_NUM_TASK 10
#define TASK_ID uint8_t
#define TASK void
#define BOOL uint8_t
#define TRUE 1
#define FALSE 0

#define BP R7
#define SP R13
#define PC R15
#define LR R14
#define XPSR R16
#endif /* TYPES_H_ */