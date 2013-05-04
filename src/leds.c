#define SYSTEM_MAIN_H

#include <system_main.h>



void init_leds()
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void light_leds(uint32_t num)
{
 uint8_t i;
 uint32_t modr_reg;
 uint16_t odr_reg;
 uint32_t tmp_lck;

 modr_reg = 0x00FFFFFF & GPIOD->MODER;
 tmp_lck = 0x00010000;

 for(i=0; i<4; i++)
 {
	 modr_reg += ( num << (24 + i) ) & ( 0x00000001 << (24 + 2*i) );
 }
 odr_reg = (uint16_t)num << 12;
 tmp_lck |= odr_reg;

 GPIOD->MODER = modr_reg;
 GPIOD->ODR = odr_reg;
 GPIOD->IDR = 0x0000;
 GPIOD->OTYPER = 0x0000;
 GPIOD->OSPEEDR = 0x00000000;
 GPIOD->PUPDR = 0x00000000;
 GPIOD->BSRRH = 0x0000;
 GPIOD->BSRRL = 0x0000;
 GPIOD->LCKR = tmp_lck;
}
