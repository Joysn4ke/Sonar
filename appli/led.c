/*
 * led.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
#include "systick.h"
#include "led.h"

#define LED_BLINK_PERIOD	200
#define LED_FLASH_PERIOD	500
#define LED_FLASH_DURATION	50


static void LED_process_ms(void);
static volatile led_state_e led_state;
static volatile uint32_t t = 0;


/**
 * @brief  Writes a state to the green LED.
 * @param  b: Boolean value indicating whether to turn the LED on (TRUE) or off (FALSE).
 * @retval None
 */
void writeLED(bool_e b) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}


/**
 * @brief  Initializes the green LED by configuring the GPIO pin connected to the LED.
 *         The GPIO pin is set to output mode with no pull-up or pull-down resistors and high speed.
 *         Also sets the LED to the off state and adds the LED processing function to the system tick callback.
 * @retval None
 */
void LED_init(void)
{
	//Initialisation du port de la led Verte  en sortie Push-Pull
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	LED_set(LED_OFF);
	Systick_add_callback_function(&LED_process_ms);
}


/**
 * @brief  Sets the state of the LED.
 * @param  state: The desired state of the LED. Can be one of the following:
 *         - LED_OFF: Turn the LED off.
 *         - LED_ON: Turn the LED on.
 *         - LED_BLINK: Make the LED blink at a predefined period.
 *         - LED_FLASH: Make the LED flash for a predefined duration.
 * @retval None
 */
void LED_set(led_state_e state)
{
	if(led_state != state)
		t = 0;			//si on change de mode, on reset le compteur.

	led_state = state;	//on enregistre l'état demandé
}


/**
 * @brief  Processes the LED state and updates the LED accordingly.
 *         This function is called periodically from the system tick handler.
 * @retval None
 */
static void LED_process_ms(void)
{
	switch(led_state)
	{
		case LED_BLINK:
			t++;
			if(t>LED_BLINK_PERIOD)
			{
				HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
				t = 0;
			}
			break;
		case LED_FLASH:
			t++;
			if(t==LED_FLASH_DURATION)
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
			}
			else if(t>LED_FLASH_PERIOD)
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
				t = 0;
			}
			break;
		case LED_OFF:
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
			break;
		case LED_ON:
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
			break;
	}
}
