/*
 * rng.c
 *
 *  Created on: 22 juin 2022
 *      Author: Nirgal
 */
#include "rng.h"
#include "stdio.h"
#include "stm32f1_sys.h"

static uint16_t shift_register = 0;

//Nous conseillons d'initialiser ce module avec un n aléatoire (issu de bruit sur une entrée ADC ou d'un événement utilisateur !)
void RNG_init(uint16_t n)
{
	shift_register = n & 0x1FF;
}


//l'algorithme présenté ici s'inspire de ceci :
//https://guillaume-savaton-eseo.github.io/CoCiNum/circuits-logiques/sequentiel-activite.html#etape-5-allumer-les-voyants-dans-un-ordre-pseudo-aleatoire
//et plus généralement de cela : https://fr.wikipedia.org/wiki/Registre_%C3%A0_d%C3%A9calage_%C3%A0_r%C3%A9troaction_lin%C3%A9aire
//la variable mod indique le nombre max souhaité pour ce random.
//attention, cet algo de pseudo random suit toujours la même suite, longue de 512 valeurs, où l'ensemble des valeurs sont représentées
uint16_t RNG_get_next_number(uint16_t mod)
{
	uint16_t b0, b5;
	uint16_t new_b8;
	b0 = shift_register & 1;
	b5 = shift_register>>5 & 1;
	new_b8 = (~(b0 ^ b5))&1;
	shift_register = (uint16_t)((shift_register >> 1) | new_b8<<8);

	return (uint16_t)(shift_register % mod);
}


//fonction de test, blocante...
void RNG_test(void)
{
	while(1)
	{
		printf("%d\n", RNG_get_next_number(512));
		Delay_ms(100);
	}
}
