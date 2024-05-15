/*
 * servo.h
 *
 *  Created on: 18 mars 2024
 *      Author: hippo
 */

#ifndef SERVOH
#define SERVOH



void SERVOinit(void);
void SERVO_set_position(uint16_t position);
void SERVO_rotation(void);

uint16_t getPosition(void);


#endif /* SERVO_H */
