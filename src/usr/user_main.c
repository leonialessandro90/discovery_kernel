

#include "../sys/system_main.h"


sem sem_led;

TASK task1(int a);
TASK task2(int a);
TASK task3(int a);
TASK task4(int a);
TASK ape3(int k);

TASK task0(int b)
{
	led_on(LED0);
}

TASK task1(int b)
{
	int a=50;
	int i=0;
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

TASK task0_per(int b){
	sem_wait(&sem_led);
	led_on(LED0);
	sem_signal(&sem_led);
}

TASK task1_per(int b)
{
	int a=50;
	if( (a == 50) && (b == 1) ) {
		sem_wait(&sem_led);
		led_on(LED1);
		sem_signal(&sem_led);
	}
}



TASK task2_per(int b)
{
	int a = 5;

	send_ape_request(&ape3, 4, 4, 0);
	if( (a == 5) && (b == 2) ) {
		sem_wait(&sem_led);
		led_on(LED2);
		sem_signal(&sem_led);
	}
}



TASK task3_per(int k)
{
	int a = 1000000;

	if( (a == 1000000) && (k == 3) ) {
		sem_wait(&sem_led);
		led_on(LED3);
		sem_signal(&sem_led);
	}
}

TASK task4_per(int k)
{
	sem_wait(&sem_led);
	led_on(LED0);
}

TASK ape1(int k)
{
	sem_wait(&sem_led);
	led_on(LED0);
	sem_signal(&sem_led);
}

TASK ape2(int k)
{
	int a = 10;
	if( (a == 10) && (k == 4) ){
		sem_wait(&sem_led);
		led_on(LED1);
		sem_signal(&sem_led);
	}
}

TASK ape3(int k)
{
	int a = 10;
	if( (a == 10) && (k == 4) ) {
		sem_wait(&sem_led);
		led_on(LED2);
		sem_signal(&sem_led);
	}
}

void user_main()
{
	sem_init(&sem_led, 1);
//	activate_task(&task0, 1, 1);
#ifndef _PERIODIC_TASK
	activate_task(&task1, 1, 1, 0);
	activate_task(&task2, 1, 2, 0);
	activate_task(&task3, 1, 3, 0);
	activate_task(&task4, 1, 4, 0);
#else
	activate_task(&task0_per, 1, 1, 8);
	activate_task(&task1_per, 1, 1, 20);
	activate_task(&task2_per, 1, 2, 15);
	activate_task(&task3_per, 1, 3, 15);
	send_ape_request(&ape1, 3, 0, 0);
	send_ape_request(&ape2, 4, 4, 0);
#endif
}

