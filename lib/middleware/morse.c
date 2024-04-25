//hypothèse : envoi d'informations textuelles suivant le codage morse qui présente une compacité intéressante :
#if USE_MORSE

//    00 : espace
//    1 suivi d'un 0 : ti
//	  11 : ta

// exemple :    10010101001001101101100
//				1 00 10101    00 1 	00  11011011 00
//				ti	 ti ti ti    ti		ta ta ta 
//				E 	 S 			 E 		O
void MORSE_test(void)
{
	uint8_t tab[10];
	uint8_t i = 0;
	tab[i++] = 0b10010101;
	tab[i++] = 0b00100110;
	tab[i++] = 0b11011000;
	tab[i++] = 0;
	MORSE_decode(tab);	//1ère étape du décodage : trouver les ti, ta et espace.
	//TODO deuxième étape du décodage : identifier les lettres.
}

uint8_t MORSE_encode(char * s, uint8_t * tab, uint8_t tab_size)
{
	//TODO
}

void MORSE_decode(uint8_t * tab)
{
	uint8_t in, i, k;
	n_one = 0;
	n_zero = 0;
	for(k=0; tab[k] ; k++)
	{
		in = tab[k];
		for(i=0; i<8; i++)
		{
			b = in>>(7-i)&1;
			
			if(!b && n_one == 1)
				event(EVENT_TI);
			
			n_zero = (!b)?n_zero+1:0;
			n_one = (b)?n_one+1:0;
			
			if(!b && n_zero==2)
				event(EVENT_SPACE);
			if(b && n_one ==2)
				event(EVENT_TA);
		}
	}
}


#endif
