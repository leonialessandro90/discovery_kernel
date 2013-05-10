
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
	activate_task(&task3, 2, 3);
	for(i=0;i<1300000;i++){
		if( (a == 50) && (b == 1) )
			led_on(LED1);
	}
}

TASK task2(int b)
{
	int i=0;
	int a = 5;

	activate_task(&task4, 2, 4);

	for(i=0;i<500000;i++){
		if( (a == 5) && (b == 2) )
			led_on(LED2);
	}
}

TASK task3(int k)
{
	int i=0;
	int a = 1000000;
	for(i=0;i<1000000;i++) {
		if( (a == 1000000) && (k == 3) )
			led_on(LED3);
	}
}

TASK task4(int k)
{
	int i=0;
	int a = 10;
	for(i=0;i<2000000;i++) {
		if( (a == 10) && (k == 4) )
			led_on(LED0);
	}
}

void user_main()
{
	activate_task(&task1, 1, 1);
	activate_task(&task2, 1, 2);
}

