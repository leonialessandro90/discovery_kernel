#define SYSTEM_MAIN_H_

#include "system_main.h"


cane = 0;
void task1(int a){
	while(cane<20)
	{
		if(a!=4)
			light_leds(4);
		else
		light_leds(1);
	}
	//for(;;);
}

int fun2(int par){
	return par+10;
}

void task2(int b){
	for(;;)
	{
		light_leds(2);
	}
	//for(;;);
}

void task3(int b){
	for(;;)
	{
		light_leds(4);
	}
	//for(;;);
}


void user_main(){
	unsigned short priority_1 = 0;
	unsigned short priority_2 = 1;
	unsigned short priority_3 = 2;
	activate_task(&task1,priority_1, 4);
	activate_task(&task2,priority_2, 10);
	activate_task(&task3,priority_3, 15);

}

