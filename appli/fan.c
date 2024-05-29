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


/**
 * @brief  Initializes the fan by configuring the GPIO pin connected to the fan.
 *         The GPIO pin is set to output mode with no pull-up or pull-down resistors and high speed.
 * @retval None
 */
void FAN_init(void)
{
	//Initialisation du port du ventilateur en sortie Push-Pull
	BSP_GPIO_PinCfg(FAN_GPIO, FAN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	FAN_set(FALSE);
}


/**
 * @brief  Sets the state of the fan.
 * @param  enable: Boolean value indicating whether to enable (TRUE) or disable (FALSE) the fan.
 * @retval None
 */
void FAN_set(bool_e enable)
{
	HAL_GPIO_WritePin(FAN_GPIO, FAN_PIN, enable);
}
