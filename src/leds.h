/*
 * leds.h
 *
 *  Created on: May 3, 2013
 *      Author: Alessio
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <system_main.h>

#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/*
 * Turns on the specified leds.
 *
 * It is possible to turn on more than one LED, by
 * just passing the sum of the chosen leds.
 *
 * Leds positioning:
 *  __________
 * |          |
 * |          |
 * |          |
 * |          |
 * |   LED1   |
 * |LED0  LED2|
 * |   LED3   |
 * |__________|
 *
 */

#define LED0 1
#define LED1 2
#define LED2 4
#define LED3 8

void led_init();
void led_on(uint8_t num);
void led_off(uint8_t num);

#endif /* LEDS_H_ */
