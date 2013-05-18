
//#define SYSTEM_MAIN_H_

#include "../sys/system_main.h"
#include "../sys/sem.h"

sem sem_led;

TASK task1(int a);
TASK task2(int a);
TASK task3(int a);
TASK task4(int a);

TASK task0(int b)
{
	led_on(LED0);
}

TASK task1(int b)
{
	int a=50;
	int i=0;
//	activate_task(&task3, 2, 3);
	for(i=0;i<1300000;i++){
		if( (a == 50) && (b == 1) ) {
			sem_wait(&sem_led);
			led_on(LED1);
			sem_signal(&sem_led);
		}
	}
}

TASK task2(int b)
{
	int i=0;
	int a = 5;

	//activate_task(&task4, 2, 4);

	for(i=0;i<500000;i++){
		if( (a == 5) && (b == 2) ){
			sem_wait(&sem_led);
			led_on(LED2);
			sem_signal(&sem_led);
		}
	}
}

TASK task3(int k)
{
	int i=0;
	int a = 1000000;
	for(i=0;i<1000000;i++) {
		if( (a == 1000000) && (k == 3) ){
			sem_wait(&sem_led);
			led_on(LED3);
			sem_signal(&sem_led);
		}
	}
}

TASK task4(int k)
{
	int i=0;
	int a = 10;
	for(i=0;i<2000000;i++) {
		if( (a == 10) && (k == 4) ){
			sem_wait(&sem_led);
			led_on(LED0);
			sem_signal(&sem_led);
		}
	}
}

void user_main()
{
	sem_init(&sem_led, 1);
//	activate_task(&task0, 1, 1);
	activate_task(&task1, 1, 1);
	activate_task(&task2, 1, 2);
	activate_task(&task3, 1, 3);
	activate_task(&task4, 1, 4);
}

