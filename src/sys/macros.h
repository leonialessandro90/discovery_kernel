#ifndef MACROS_H_
#define MACROS_H_

#define INT_TO_FUN\
		asm("POP {R0}");\
		asm("STR R0, [SP, #28]");\
		asm("POP {R0-R3}");\
		asm("ADD SP, SP, #12");


#define FUN_TO_INT\
		asm("SUB SP, SP, #4");\
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
		asm("MSR APSR_nzcvq, R0");\
		asm("LDR R0, [R1, #60]");\
		asm("ORR R0, R0, #0x1");\
		asm("PUSH {R0}");

#endif
