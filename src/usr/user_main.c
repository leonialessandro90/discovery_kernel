
//#define SYSTEM_MAIN_H_

#include "../sys/system_main.h"
#include "../sys/sem.h"

sem sem_led;

TASK task1(int a);
TASK task2(int a);
TASK task3(int a);
TASK task4(int a);

TASK task1(int b)
{
	int a=50;
	int i=0;
	activate_task(&task2, 2, 3);
	for(i=0;i<1300000;i++){
		if(a == 50)
			led_on(LED3);
	//while(a<10000) {
		//for (i=0; i<10000; i++);
		//if (a % 2 == 0){
		//	sem_wait(&sem_led);
		//	led_on(LED1);
	//		sem_signal(&sem_led);
		//} else {
		//	sem_wait(&sem_led);
			//led_off(LED1);
			//sem_signal(&sem_led);

	}
}

int fun2(int par)
{
	return par+10;
}

TASK task2(int b)
{
	int i=0;
	int count = 5;

	activate_task(&task3, 3, 4);

	for(i=0;i<500000;i++){
		//sem_wait(&sem_led);
	//led_on(LED0 + LED2);
		//sem_signal(&sem_led);
		//for (i=0; i<count; i++);
		//sem_wait(&sem_led);
		//led_on(LED1 + LED3);
		//sem_signal(&sem_led);
		//for (i=0; i<count; i++);
		if(count == 5)
			led_on(LED1);
	}
}

TASK task3(int a)
{
	int i=0;
	int count = 1000000;
	for(i=0;i<1000000;i++) {
		/*
		//sem_wait(&sem_led);
		led_on(LED0 + LED2);
	//	sem_signal(&sem_led);
		for (i=0; i<count; i++);
	//	sem_wait(&sem_led);
		led_on(LED1 + LED3);
	//	sem_signal(&sem_led);
		for (i=0; i<count; i++);
	*/
		if(count == 1000000)
			led_on(LED0);
	}
}

TASK task4(int k)
{
	for(;;){
		led_on(LED2);
	}
}

void user_main()
{
	//sem_init(&sem_led, 0);
	activate_task(&task1, 1, 2);
	//activate_task(&task4, 4, 10);
}
