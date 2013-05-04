#define SYSTEM_MAIN_H_

#include "leds.h"
#include "system_main.h"

void task1(int a)
{
	int cane = 0;
	while (cane < 20) {
		if (a != 4)
			led_on(3);
		else
			led_on(1);
	}
}

int fun2(int par)
{
	return par+10;
}

void task2(int b)
{
	//for (;;) light_leds(2);
}

void task3(int b)
{
	static int i;
	static int count = 1000000;
	for (;;) {
		led_on(LED0 + LED3);
		for (i=0; i<count; i++);
		led_off(LED0 + LED3);
		for (i=0; i<count; i++);
	}
}


void user_main()
{
	//activate_task(&task1, 1, 4);
	//activate_task(&task2, 1, 10);
	activate_task((void *)&task3, 2, 15);
}

