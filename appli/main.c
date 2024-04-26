#define USE_SCREEN_TFT_ILI9341 1
#define USE_FONT11x18 1
#define USE_FONT7x10 1

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "stm32f1_rtc.h"
#include "macro_types.h"
#include "systick.h"

#include "tft_ili9341/stm32f1_ili9341.h"

#include "stm32f1_timer.h"
#include "stm32f1_adc.h"

#include "HC-SR04/HCSR04.h"

//Include fichiers projet
#include "ecran.h"
#include "servo.h"

//##################################
//			State Machine
//##################################
typedef enum
    {
        INIT,
        MENU_CHOICE,
        SCANNING_ENVIRONNEMENT,
		PAUSE,
        SCREEN_DISPLAY,
    }state_e;

static state_e state = INIT;
static state_e previous_state = INIT;

bool_e test_scan;


static void state_machine(void);
//##################################
//##################################
//##################################

//te

//##################################
//	          Write Led
//##################################
void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}
//##################################
//##################################
//##################################


bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}


static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;

}




int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi re  tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2   la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig es vers la sonde de d bogage, la liaison UART  tant ensuite encapsul e sur l'USB vers le PC de d veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p riph rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms   la liste des fonctions appel es automatiquement chaque ms par la routine d'interruption du p riph rique SYSTICK
	Systick_add_callback_function(&process_ms);


	Screen_init();
	//SERVO_init();
	BUTTON_init();


	while(1)	//boucle de tache de fond
	{
		//printf("main");
		//isClicked();
		//HCSR04_demo_state_machine();

		//test_scan = scanning_enable();
		//test_scan = TRUE;


		state_machine();
	}
}



/**/
static void state_machine(void)
{

    bool_e entrance = (state!=previous_state)?TRUE:FALSE;
    previous_state = state;
    //bool_e test_scan = FALSE;
    //uint16_t distance = 0;


    bool_e button_event = BUTTON_state_machine();    //� chaque passage ici, on scrute un �ventuel �v�nement sur le bouton


    switch(state)
    {
        case INIT:
        	/*
            //Screen_init();
            //SERVO_init();
            //BUTTON_init();
            //UltraSound_init();
             */
            Systick_add_callback_function(&process_ms);
            state = MENU_CHOICE;
            break;

        case MENU_CHOICE:
            if(entrance)
            {
                //printf("[STATEMACHINE] choix menu\n");

				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_DrawFilledRectangle(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10) + string_length("Scanne environnement", &Font_7x10), closeButton.centerY + TEXT_HEIGHT_7_10, ILI9341_COLOR_WHITE);
				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, "Choix menu", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			}

            test_scan = scanning_enable();

            if (test_scan)
            {
                state = SCANNING_ENVIRONNEMENT;
            }
            break;

        case SCANNING_ENVIRONNEMENT:
            if(entrance)
            {
                //printf("[STATEMACHINE] scanne l'environnement\n");

            	uint32_t current_time = HAL_GetTick();		// Obtenir le temps actuel en millisecondes

            	if (current_time - last_display_time >= DISPLAY_REFRESH_INTERVAL) {
					ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_DrawFilledRectangle(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10) + string_length("Scanne environnement", &Font_7x10), closeButton.centerY + TEXT_HEIGHT_7_10, ILI9341_COLOR_WHITE);
					ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, "Scanne environnement", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
            	}
            	last_display_time = current_time; // Mettre � jour le temps du dernier affichage
            }

            HCSR04_state_machine();
            //SERVO_rotation();
            uint8_t id_hcsr04;
            uint16_t *distance_capteur;
			//uint16_t * DISTANCE_GLOBAL;
            //printf("print scanning environnment %u\n", DISTANCE_GLOBAL);


            if (HCSR04_get_value(id_hcsr04,  &distance_capteur) > 0)
            //if (distance > 0)
            {
                state = SCREEN_DISPLAY;
            }
            else{
            	//state = SCANNING_ENVIRONNEMENT;
            	state = PAUSE;
            }

            isClicked();
            break;

        case PAUSE:
			if(entrance) {
				t = 3000;
				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_DrawFilledRectangle(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10) + string_length("Scanne environnement", &Font_7x10), closeButton.centerY + TEXT_HEIGHT_7_10, ILI9341_COLOR_WHITE);
				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, "Pause", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			}


			if(!t) {
				state = SCANNING_ENVIRONNEMENT;
			}
			break;

        case SCREEN_DISPLAY:
            if(entrance)
            {
            	//printf("print screen display %u\n", DISTANCE_GLOBAL);



            	char buffer[50];
				sprintf(buffer, "%d", HCSR04_get_value(id_hcsr04, &distance_capteur ));

				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY  + TEXT_HEIGHT_7_10 * 2, "Distance : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Distance : ", &Font_7x10), closeButton.centerY  + TEXT_HEIGHT_7_10 * 2, &buffer, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

                state = SCANNING_ENVIRONNEMENT;
            }
            break;
        default:
            break;
    }
}




void isClicked(void) {
	static uint16_t static_x,static_y;
	uint16_t x, y;

	//code temporaire
	DrawQuadrilateral(static_quadrilateral);		// Dessiner le quadrilat�re
	//code temporaire

	if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	{
		//ILI9341_DrawCircle(static_x, static_y, 15,ILI9341_COLOR_WHITE);
		//ILI9341_DrawCircle(x, y, 15, ILI9341_COLOR_BLUE);

		//print_screen_current_state("Clicked", closeButton.centerY);

		static_x = x;
		static_y = y;

		if(isClickedOnRectangle(static_x, static_y, static_quadrilateral.x1, static_quadrilateral.y1, static_quadrilateral.x2, static_quadrilateral.y2)) {
			uint16_t a = 3;
			state = PAUSE;
		}
		else if(isClickedOnCircle(static_x, static_y, closeButton.centerX, closeButton.centerY, closeButton.radius)) {
			uint16_t a = 3;
			state = MENU_CHOICE;
		}
	}
}



