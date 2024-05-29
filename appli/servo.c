/*
 * servo.c
 *
 *  Created on: 18 mars 2024
 *      Author: hippo
 */


#include "config.h"
#include "stm32f1_timer.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"

#define PERIOD_TIMER 10 //ms

static uint16_t position = 18;

static uint16_t current_position;



/**
 * @brief  Initializes the servo motor.
 *         Configures and starts Timer 1 with a period of 10 ms.
 *         Enables PWM signal on Timer 1 Channel 1 (PA8 pin).
 *         Sets the duty cycle for a servo position of 0 degrees.
 * @retval None
 */
void SERVO_init(void){
    //initialisation et lancement du timer1 �  une période de 10 ms
    TIMER_run_us(TIMER1_ID, PERIOD_TIMER*1000, FALSE); //10000us = 10ms
    //activation du signal PWM sur le canal 1 du timer 1 (broche PA8)

    //TIMER_enable_PWM(TIMER1_ID, TIM_CHANNEL_3, 150, FALSE, FALSE);
    TIMER_enable_PWM(TIMER1_ID, TIM_CHANNEL_1, 150, FALSE, TRUE);

    //rapport cyclique reglé pour une position servo de 50%
    SERVO_set_position(0);
}



/**
 * @brief  Sets the position of the servo motor.
 *         Sets the current position variable.
 *         Sets the duty cycle of the PWM signal to control the servo position.
 * @param  position: The desired position of the servo motor in degrees.
 * @retval None
 */
void SERVO_set_position(uint16_t position) {
    current_position = position;
    uint16_t servopos;
    TIMER_run_us(TIMER1_ID,10000,FALSE);

    TIMER_set_duty(TIMER1_ID, TIM_CHANNEL_1, position+100);
}


/**
 * @brief  Rotates the servo motor in a continuous manner.
 *         Rotates the servo motor back and forth within a defined range of positions.
 * @retval None
 */
void SERVO_rotation(void) {
    static bool_e rotation = TRUE; // Initialisation � true par d�faut

    rotation = (position >= 158) ? !rotation : rotation;

    if (rotation) {										//Test pour �viter de faire position - 10 < 0
		position = position + 10;
    } else if(position - 10 > 18) {
    	position = position - 10;
    } else {
    	position = 18;
    	rotation = !rotation;
    }

    SERVO_set_position(position);
}


/**
 * @brief  Gets the current position of the servo motor.
 * @retval The current position of the servo motor in degrees.
 */
uint16_t getPosition(void){
	return position;
}


