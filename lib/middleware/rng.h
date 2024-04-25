/*
 * rng.h
 *
 *  Created on: 22 juin 2022
 *      Author: Nirgal
 */

#ifndef MIDDLEWARE_RNG_H_
#define MIDDLEWARE_RNG_H_

#include <stdint.h>

void RNG_init(uint16_t n);
uint16_t RNG_get_next_number(uint16_t mod);
void RNG_test(void);

#endif /* MIDDLEWARE_RNG_H_ */
