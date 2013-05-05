
//#define SYSTEM_MAIN_H_

#include "system_main.h"
#include "sem.h"

sem sem_led;

TASK task3(int a)
{
	int i=0;
	int count = 1000000;
	for (;;) {
		sem_wait(&sem_led);
		led_on(LED0 + LED2);
		sem_signal(&sem_led);
		for (i=0; i<count; i++);
		sem_wait(&sem_led);
		led_on(LED1 + LED3);
		sem_signal(&sem_led);
		for (i=0; i<count; i++);
	}
}

int fun2(int par)
{
	return par+10;
}

TASK task2(int b)
{
	int i=0;
	int count = 1000000;
	for (;;) {
		sem_wait(&sem_led);
		led_on(LED0 + LED2);
		sem_signal(&sem_led);
		for (i=0; i<count; i++);
		sem_wait(&sem_led);
		led_on(LED1 + LED3);
		sem_signal(&sem_led);
		for (i=0; i<count; i++);
	}
}

TASK task1(int b)
{
	int a=0;
	int i=0;
	while(a<10000) {
		for (i=0; i<10000; i++);
		if (a % 2 == 0){
			sem_wait(&sem_led);
			led_on(LED1);
			sem_signal(&sem_led);
		} else {
			sem_wait(&sem_led);
			led_off(LED1);
			sem_signal(&sem_led);
		}
	}
}

void user_main()
{
	sem_init(&sem_led, 0);
	//activate_task(&task1, 1, 2);
	//activate_task(&task2, 2, 3);
	//activate_task(&task3, 3, 4);
}

