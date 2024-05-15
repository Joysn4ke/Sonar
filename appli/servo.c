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
static uint16_t current_position;

void SERVO_init(void){
    //initialisation et lancement du timer1 �  une période de 10 ms
    TIMER_run_us(TIMER1_ID, PERIOD_TIMER*1000, FALSE); //10000us = 10ms
    //activation du signal PWM sur le canal 1 du timer 1 (broche PA8)
    TIMER_enable_PWM(TIMER1_ID, TIM_CHANNEL_1, 150, FALSE, TRUE);
    //rapport cyclique reglé pour une position servo de 50%
    SERVO_set_position(0);
}


void SERVO_set_position(uint16_t position){
    current_position = position;
    uint16_t servopos;
    TIMER_run_us(TIMER1_ID,10000,FALSE);
    TIMER_set_duty(TIMER1_ID, TIM_CHANNEL_1, position+100);
    //HAL_Delay(300);
    HAL_Delay(150);
}

void SERVO_rotation(void)
{
    static uint16_t position = 0;
    position = (position > 180)?0:(position+10); //de 0 � 100%, par pas de 1%
    SERVO_set_position(position);

    HAL_Delay(10); //anti-rebond "de fortune" en cadencant la lecture du bouton
}
