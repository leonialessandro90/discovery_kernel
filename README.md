discovery_kernel
================
This is a lightweight kernel for the StMicroelectronics' STM32F4-Discovery board.

Scheduling Algorithm
--------------------
It provides two kind of scheduling algorithm:
x   Round Robin
x   Rate Monotonic (with polling server).

Default compiling option is 'Round Robin'. If you want to compile with Rate Monotonic you have to add
> #define _PERIODIC_TASK
into src/sys/types.h

Semaphore
--------------------
Semaphore mechanims is provided with or without task's preemption.
You can add tasks' preemption adding
> #define _SEM_PREEMPTION
into src/sys/types.h
