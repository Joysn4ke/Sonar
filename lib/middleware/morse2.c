#include "systick.h"
#if 0
static volatile uint32_t t = 0;

void process_ms(void)
{
	if(t)
		t--;
}

typedef enum
{
	NONE,
	TI,
	TA
}morse_e;

typedef struct
{
	morse_e symbols[6];	//il faut garder au moins une case de + que la quantité max de ti et ta.
}letter_t;
	
static const letter_t letters[26] = 
{ 	(letter_t){{TI, TA}}, 			//A
	(letter_t){{TA, TI, TI, TI}}, 	//B
	(letter_t){{TI, TA, NONE}}, 	//C
	(letter_t){{TA, TT, TA, TI}}, 	//D
	(letter_t){{TI}} 				//E...
}

void MORSE_demo(void)
{
	while(MORSE_state_machine_process_ms("ACDC") == IN_PROGRESS);
}

running_e MORSE_state_machine_process_ms(char * message)
{
	typedef enum
	{
		INIT,
		IDLE,
		NEXT_SYMBOL,
		DO_TI,
		DO_TA,
		PAUSE_BETWEEN_TWO_SYMBOLS,
		LONG_PAUSE_BETWEEN_TWO_LETTERS
	}state_e;
	static state_e state = INIT;
	static state_e previous_state = INIT;
	running_e ret = IN_PROGRESS;
	static uint16_t index = 0;
	static letter_t * current_letter;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;
	previous_state = state;
	
	switch(state)
	{
		case INIT:
			SYSTICK_add_callback_function(&process_ms);
			state = IDLE;
			break;
		case IDLE:
			if(message != NULL)
			{
				index = 0;
				symbol_index = 0;
				state = NEXT_SYMBOL;
			}
			break;
		case NEXT_SYMBOL:
			if(message[index]!=0 && message[index]>='A' && message[index]<='Z')
			{
				current_letter = &letters[message[index]-'A'];	//lettre acceptable
				switch(current_letter.symbols[symbol_index])
				{
					case TI:
						state = DO_TI;
						break;
					case TA:
						state = DO_TA;
						break;
					case NONE:
					//no break;
					default:
						state = LONG_PAUSE_BETWEEN_TWO_LETTERS;
						index++;	//lettre suivante !
						break;
				}
				symbol_index++;
			}
			else
			{
				state = IDLE;
				//fin du message !
				ret = END_OK;
			}
			break;
		case DO_TI:
			if(entrance)
			{
				buzz(1);
				t = 500;
			}
			if(!t)
			{
				buzz(0);
				state = PAUSE_BETWEEN_TWO_SYMBOLS;
			}
			break;
		case DO_TA:
			if(entrance)
			{
				buzz(1);
				t = 1000;
			}
			if(!t)
			{
				buzz(0);
				state = PAUSE_BETWEEN_TWO_SYMBOLS;
			}
			break;
		case PAUSE_BETWEEN_TWO_SYMBOLS:
			if(entrance)
				t = 500;
			if(!t)
				state = NEXT_SYMBOL;
			break;
			
		case LONG_PAUSE_BETWEEN_TWO_LETTERS:
			if(entrance)
				t = 2000;
			if(!t)
				state = NEXT_SYMBOL;
			break;
		default:
			break;
	}
	return ret;
	
}
#endif
