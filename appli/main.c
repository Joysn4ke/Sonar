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
#include "button.h"
#include "servo.h"



static void state_machine(void);


static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;

}

//extern uint16_t pointsAngle[15][2];

/*
extern static uint16_t strLenghtState = 112;					//uint16_t strLenghtState = string_length("Current state : ", &Font_7x10);
extern static uint16_t strLenghtPosMot = 126; 					//uint16_t strLenghtPosMot = string_length("Position moteur : ", &Font_7x10);
extern static uint16_t strLenghtDistance = 74;				//uint16_t strLenghtDistance = string_length("Distance : ", &Font_7x10);
extern static uint16_t strLenghtScanEnv = 140;					//uint16_t strLenghtScanEnv = string_length("Scanne environnement", &Font_7x10);

extern static uint16_t x = 30 + 20 + TEXT_GAP_7_10;		//static uint16_t x = closeButton.centerX + closeButton.radius + TEXT_GAP_7_10;
extern static uint16_t y = 30 + TEXT_HEIGHT_7_10;			//static uint16_t y = closeButton.centerY + TEXT_HEIGHT_7_10;
extern static uint16_t y2 = 30 + TEXT_HEIGHT_7_10 * 2;
extern static uint16_t y4 = 30 + TEXT_HEIGHT_7_10 * 4;
*/


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

	//On ajoute la fonction process_ms  la liste des fonctions appel es automatiquement chaque ms par la routine d'interruption du p riph rique SYSTICK
	//Systick_add_callback_function(&process_ms);


	Screen_init();
	SERVO_init();
	BUTTON_init();

	while(1)	//boucle de tache de fond
	{
		state_machine();
	}
}






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


/**/
static void state_machine(void)
{


    bool_e entrance = (state!=previous_state)?TRUE:FALSE;
    previous_state = state;

    uint16_t positionParam;
    uint16_t previousPosition;

    //bool_e button_event = BUTTON_state_machine();    //� chaque passage ici, on scrute un �ventuel �v�nement sur le bouton


    switch(state)
    {
        case INIT:
            //Systick_add_callback_function(&process_ms);
            state = MENU_CHOICE;
            break;

        case MENU_CHOICE:
            if(entrance)
            {
                //printf("[STATEMACHINE] choix menu\n");

				DrawMenu();

				/*
				ILI9341_Puts(x, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
				ILI9341_DrawFilledRectangle((uint16_t)(x + strLenghtState), closeButton.centerY, (uint16_t)(x + strLenghtState + strLenghtScanEnv), y, ILI9341_COLOR_BLACK);
				ILI9341_Puts((uint16_t)(x + strLenghtState), closeButton.centerY, "Choix menu", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
				*/
				PrintStateChoixMenu();
			}

            test_scan = scanning_enable();

            if (test_scan)
            {
                state = SCANNING_ENVIRONNEMENT;
                HideMenu();
                //DrawSonar(SCREEN_CENTER_X, (uint16_t)(SCREEN_LENGTH - 25), (uint16_t)(SCREEN_CENTER_X - 50), ILI9341_COLOR_WHITE);
                DrawSonar(SCREEN_CENTER_X, (uint16_t)(SCREEN_LENGTH - 25), (uint16_t)(SCREEN_CENTER_X - 50), ILI9341_COLOR_GREEN);
            }
            break;

        case SCANNING_ENVIRONNEMENT:
            if(entrance)
            {
            	/*
            	ILI9341_Puts(x, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
				ILI9341_DrawFilledRectangle((uint16_t)(x + strLenghtState), closeButton.centerY, (uint16_t)(x + strLenghtState + strLenghtScanEnv), y, ILI9341_COLOR_BLACK);
				ILI9341_Puts((uint16_t)(x + strLenghtState), closeButton.centerY, "Scanne environnement", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
				*/
            	PrintStateScan();
            }

            //printf("[STATEMACHINE] scanne l'environnement\n");


            HCSR04_state_machine();
            SERVO_rotation();


            uint32_t current_time = HAL_GetTick();		// Obtenir le temps actuel en millisecondes

			if (current_time - last_display_time >= DISPLAY_REFRESH_INTERVAL) {
				char buffer[50];
				//printf("Position : %d", getPosition());

				positionParam = getPosition();

				DrawScanning(SCREEN_CENTER_X, (uint16_t)(SCREEN_LENGTH - 25), (uint16_t)(SCREEN_CENTER_X - 50), ILI9341_COLOR_GREEN, positionParam, previousPosition);

				previousPosition = positionParam;


				////////////////////////////////
				/////// A SUPPRIMER ////////////
				////////////////////////////////
				ILI9341_Puts(xOrigin, yOrigin4, "Position moteur : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

				sprintf(buffer, "AAAAAAAAAAAAAAAAAAAAAAA");
				ILI9341_Puts((uint16_t)(xOrigin + strLenghtPosMot), yOrigin4, buffer, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);
				////////////////////////////////
				/////// A SUPPRIMER ////////////
				////////////////////////////////


				sprintf(buffer, "%d", positionParam);
				ILI9341_Puts((uint16_t)(xOrigin + strLenghtPosMot), yOrigin4, buffer, &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);


				last_display_time = current_time; // Mettre � jour le temps du dernier affichage
			}


			static uint16_t distance;

			HAL_StatusTypeDef status = HCSR04_get_value(0, &distance);

			//###########################################################
			//############## !!!!!!!! A SUPPRIMER !!!!!!!! ##############
			//###########################################################
			distance = (uint16_t)((current_time % 4000) + 1);
			//###########################################################
			//###########################################################
			//###########################################################

            if (distance > 0)
            {
                state = SCREEN_DISPLAY;
            }
            else{
            	state = SCANNING_ENVIRONNEMENT;
            }

            isClicked();
            break;

        case PAUSE:
			if(entrance) {
				t = 3000;
				/*
				ILI9341_Puts(xOrigin, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
				ILI9341_DrawFilledRectangle((uint16_t)(xOrigin + strLenghtState), closeButton.centerY, (uint16_t)(xOrigin + strLenghtState + strLenghtScanEnv), yOrigin, ILI9341_COLOR_BLACK);
				ILI9341_Puts((uint16_t)(xOrigin + strLenghtState), closeButton.centerY, "Pause", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
				*/
				PrintStatePause();
			}


			if(!t) {
				state = SCANNING_ENVIRONNEMENT;
			}
			break;

        case SCREEN_DISPLAY:
            if(entrance)
            {
            	/*
            	char buffer[50];

				ILI9341_Puts(xOrigin, yOrigin2, "Distance : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

				sprintf(buffer, "AAAAAAAAAAAAAAAAAAAAAAA");
				ILI9341_Puts((uint16_t)(xOrigin + strLenghtDistance), yOrigin2, buffer, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);

				sprintf(buffer, "%d", distance);
				ILI9341_Puts((uint16_t)(xOrigin + strLenghtDistance), yOrigin2, buffer, &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
				*/
            	PrintDistance(distance);
            	DrawTarget(SCREEN_CENTER_X,(uint16_t)(SCREEN_LENGTH - 25), (uint16_t)(distance * 50 / 2000), positionParam);
            }
            state = SCANNING_ENVIRONNEMENT;
            break;
        default:
            break;
    }
}




void isClicked(void) {
	static uint16_t static_x,static_y;
	Sint16 x, y;

	if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	{
		static_x = (uint16_t)x;
		static_y = (uint16_t)y;

		if(isClickedOnCircle(static_x, static_y, closeButton.centerX, closeButton.centerY, closeButton.radius)) {
			state = MENU_CHOICE;
			HideSonar(SCREEN_CENTER_X, (uint16_t)(SCREEN_LENGTH - 25), (uint16_t)(SCREEN_CENTER_X - 50), ILI9341_COLOR_BLACK);
			ILI9341_Puts(xOrigin, yOrigin4, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);		//Hide distance display
			ILI9341_Puts(xOrigin, yOrigin2, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);		//Hide position motor display
		}
	}
}



