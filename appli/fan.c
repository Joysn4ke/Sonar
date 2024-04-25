/*
 * fan.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#include "config.h"
#include "stm32f1_gpio.h"
#include "fan.h"

#define FAN_GPIO				GPIOA
#define FAN_PIN					GPIO_PIN_8

void FAN_init(void)
{
	//Initialisation du port du ventilateur en sortie Push-Pull
	BSP_GPIO_PinCfg(FAN_GPIO, FAN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	FAN_set(FALSE);
}


void FAN_set(bool_e enable)
{
	HAL_GPIO_WritePin(FAN_GPIO, FAN_PIN, enable);
}
