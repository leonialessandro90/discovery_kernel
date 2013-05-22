#ifndef SYSTEM_MAIN_H_
#define SYSTEM_MAIN_H_




#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "macros.h"

#include "types.h"
#include "memory.h"

#include "leds.h"
#include "list.h"
#include "task.h"
#include "sem.h"


void user_main();
void start_task(ADDR addr_fun, uint32_t param);
void init_timer();
inline des_task_block * SCHEDULER ();
void wait(uint32_t numberOfCycles);

#endif /* SYSTEM_MAIN_H_ */
