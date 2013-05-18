#ifndef MACROS_H_
#define MACROS_H_



#define INT_TO_FUN\
	asm(/* Get return address from the stack */\
		"POP {R0};"\
		/* Store return address in the future top of the stack: actual position of XPSR */\
		"STR R0, [SP, #28];"\
		/* Get R0-R3 registers from the interrupt stack: no inconsistency assumption */\
		"POP {R0-R3};"\
		/* Clean the remaining interrupt stack up to the previously stored return address */\
		"ADD SP, SP, #12;");


#define FUN_TO_INT\
	asm(/* Keep two locations free for store PC and XPSR directly from task descriptor (one location is already free)*/\
		"SUB SP, SP, #4;"\
		/* Store PC and XPSR in the interrupt stack */\
		"PUSH {LR,R12,R3,R2,R1,R0};"\
		"LDR R0, [R1, #60];"\
		"STR R0, [SP, #24];"\
		"LDR R0, [R1, #64];"\
		"MOV R0, #16777216;"\
		"STR R0, [SP, #28];");\
		/* Push all registers needed to build up the interrupt stack */\
		/* Store in LR a magic word to correctly return from interrupt */\
		//("MOV LR, 0xFFFFFFF9");


#define SAVE_STATE_FROM_INT\
	asm volatile(/* The interrupt function stored in the stack LR and R7	*/\
		/* Get back R7 and discard LR that is meaningless		*/\
		"POP {R7};"\
		"ADD SP, SP, #4;"\
		/* Store R4-R11 and SP in the task descriptor			*/\
		/* Others registers are stored in the interrupt stack	*/\
		"LDR R1, =running;"\
		"LDR R1, [R1, 0];"\
		"STR R4, [R1, #16];"\
		"STR R5, [R1, #20];"\
		"STR R6, [R1, #24];"\
		"STR R7, [R1, #28];"\
		"STR R8, [R1, #32];"\
		"STR R9, [R1, #36];"\
		"STR R10, [R1, #40];"\
		"STR R11, [R1, #44];"\
		"STR SP, [R1, #52];"\
		/* Get R0-R3, R12, LR, PC and XPSR from the interrupt stack	*/\
		/* Store them in the task descriptor */\
		"LDR R0, [SP, #0];"\
		"STR R0, [R1, #0];"\
		"LDR R0, [SP, #4];"\
		"STR R0, [R1, #4];"\
		"LDR R0, [SP, #8];"\
		"STR R0, [R1, #8];"\
		"LDR R0, [SP, #12];"\
		"STR R0, [R1, #12];"\
		"LDR R0, [SP, #16];"\
		"STR R0, [R1, #48];"\
		\
		"LDR R0, [SP, #20];"\
		"STR R0, [R1, #56];"\
		"LDR R0, [SP, #24];"\
		"STR R0, [R1, #60];"\
		"LDR R0, [SP, #28];"\
		"STR R0, [R1, #64];");


#define SAVE_STATE_FROM_FUN\
	asm volatile(/* The function stored in the stack LR and R7, they are both meaningful */\
		"POP {R7,LR};"\
		/* Store R4-R12, SP, LR and XPSR in the task descriptor	*/\
		/* Don't need to save R0-R3: inconsistency assumption	*/\
		"LDR R1, =running;"\
		"LDR R1, [R1, 0];"\
		"STR R4, [R1, #16];"\
		"STR R5, [R1, #20];"\
		"STR R6, [R1, #24];"\
		"STR R7, [R1, #28];"\
		"STR R8, [R1, #32];"\
		"STR R9, [R1, #36];"\
		"STR R10, [R1, #40];"\
		"STR R11, [R1, #44];"\
		"STR R12, [R1, #48];"\
		"STR SP, [R1, #52];"\
		"STR LR, [R1, #60];"\
		"MRS R0, XPSR;"\
		"STR R0, [R1, #64];");

#define LOAD_STATE\
	asm volatile(/* Load R4-R12, SP, LR and APSR from the task descriptor 		*/\
		/* R0-R3 are not loaded: don't know the situation of the stack	*/\
		"LDR R1, =running;"\
		"LDR R1, [R1,0];"\
		"LDR R4, [R1, #16];"\
		"LDR R5, [R1, #20];"\
		"LDR R6, [R1, #24];"\
		"LDR R7, [R1, #28];"\
		"LDR R8, [R1, #32];"\
		"LDR R9, [R1, #36];"\
		"LDR R10, [R1, #40];"\
		"LDR R11, [R1, #44];"\
		"LDR R12, [R1, #48];"\
		"LDR SP, [R1, #52];"\
		"LDR LR, [R1, #56];"\
		"LDR R0, [R1, #64];"\
		"MSR APSR_nzcvq, R0;"\
		/* Load PC from the task descriptor and OR its last bit for protection issue */\
		"LDR R0, [R1, #60];"\
		"ORR R0, R0, #0x1;"\
		/* Push return address (next instruction of the new task) in the stack */\
		"PUSH {R0};");

#endif
