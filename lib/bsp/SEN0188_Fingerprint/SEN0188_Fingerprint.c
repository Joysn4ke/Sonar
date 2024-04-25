/**
  ******************************************************************************
  * @file    main.c
  * @author  Samuel Poiraud
  * @version V2.0
  * @date    25/10/2017
  * @brief   Fonction main
  ******************************************************************************
*/
#if 0			//Code non mis en forme / nettoyé.... .

#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
#include "stm32f1_timer.h"
#include "test.h"
#include "stm32f1_ili9341.h"
#include "systick.h"


typedef enum
{
	PID_UNKNOW = 0,
	PID_COMMAND = 1,
	PID_DATA = 2,
	PID_ACK = 7,
	PID_END = 8,
}pid_e;

typedef struct
{
	uint32_t addr;
	pid_e pid;
	uint16_t lenght;
	uint8_t datas[256];
}frame_t;

bool_e parse_frames(uint8_t byte, frame_t * pframe);

void send_command(frame_t * pframe);
#define MODULE_DEFAULT_ADDRESS	0xFFFFFFFF

#define COMMAND_ID_READ_SYS_PARAM		0x0f
#define OPEN_LED						0x50
#define CLOSE_LED						0x51
#define GET_IMG							0x01
#define GET_IMAGE_FREE					0x52
#define GET_ECHO						0x53
#define REG_MODEL						0x05

volatile static bool_e flag_ack = FALSE;
volatile static bool_e flag_ack_fail = FALSE;
volatile static uint32_t t = 0;

void process_ms(void)
{
	if(t)
		t--;
}


int main(void)
{
	HAL_Init();			//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	BSP_GPIO_Enable();	//Activation des périphériques GPIO
	SYS_ClockConfig();		//Configuration des horloges.

	//Initialisation du port de la led Verte (carte Nucleo)

	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)

	//BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_IT_RISING_FALLING,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_12,GPIO_MODE_IT_RISING_FALLING,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI15_10_IRQn));

	//Initialisation de l'UART1 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
	//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
	//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsulée sur l'USB vers le PC de développement.
	UART_init(UART2_ID,115200);
	UART_init(UART1_ID,57600);

	//"Indique que les printf sortent vers le périphérique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	Systick_add_callback_function(&process_ms);

	//Fonction blocante à des fins de test :
	//test();
	ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);


	frame_t frame;
	//Tâche de fond :

		uint8_t toto = 4;
		uint16_t abcd;

		abcd = (uint16_t)(toto);

	//send_setPwd(MODULE_DEFAULT_ADDRESS, 0xCAFEDECA);
/*	send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
	send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMG);
	//send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMAGE_FREE);
	//send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_ECHO);
	send_Img2Tz(MODULE_DEFAULT_ADDRESS, 0x01);
	send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, REG_MODEL);
	send_store(MODULE_DEFAULT_ADDRESS, 0x01, 0x1100);
*/
	while(1)
	{
		uint8_t c;
		state_machine();

		if(UART_data_ready(UART1_ID))
		{
			c = UART_get_next_byte(UART1_ID);
			if(parse_frames(c, &frame))
			{
				//ILI9341_Puts(0, 230, "frame recue", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);

				//	ILI9341_Puts(400,100, "led allume", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);
				if(frame.pid == PID_ACK)
				{

						switch(frame.datas[0])
						{
							case 0x00:
									//ILI9341_Puts(0,120, "ack ok", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
									flag_ack = TRUE;
									break;
							case 0x01:
									//ILI9341_Puts(0,150, "paquet non recu", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);;
									flag_ack_fail = TRUE;
									break;
							case 0x02:
									//ILI9341_Puts(0,180, "empreinte non detectee", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
									flag_ack_fail = TRUE;
									break;
							case 0x03:
									//ILI9341_Puts(0,210, "fail", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
									flag_ack_fail = TRUE;
									break;
							case 0x0a:
									//ILI9341_Puts(0,210, "fail character", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
									flag_ack_fail = TRUE;
									break;
							case 0x09:
									//ILI9341_Puts(0,210, "empreinte non reconnue", &Font_11x18, ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
									flag_ack_fail = TRUE;
									break;
						}
				}
					for(int i=0; i<frame.lenght && i<256; i++)
							printf("%08x ", frame.datas[i]);


			}

		}
	}

}




void send_setPwd(uint32_t address, uint32_t password)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+5;
	frame.datas[0] = 0x12;
	frame.datas[1] = (uint8_t)((password>>24)&0xFF);
	frame.datas[2] = (uint8_t)((password>>16)&0xFF);
	frame.datas[3] = (uint8_t)((password>>8)&0xFF);
	frame.datas[4] = (uint8_t)((password)&0xFF);
	send_command(&frame);
}

void send_Img2Tz(uint32_t address, uint8_t buffer_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+2;
	frame.datas[0] = 0x02;
	frame.datas[1] = buffer_id;
	send_command(&frame);
}

void send_store(uint32_t address, uint8_t buffer_id, uint16_t page_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+4;
	frame.datas[0] = 0x06;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((page_id>>8)&0xFF);
	frame.datas[3] = (uint8_t)((page_id)&0xFF);
	send_command(&frame);
}

void send_command_withoutparameters(uint32_t address, uint8_t command_id)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+1;
	frame.datas[0] = command_id;
	send_command(&frame);
}

void send_search(uint32_t address, uint8_t buffer_id, uint16_t start_page, uint16_t page_num)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+6;
	frame.datas[0] = 0x04;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((start_page>>8)&0xFF);
	frame.datas[3] = (uint8_t)((start_page)&0xFF);
	frame.datas[4] = (uint8_t)((page_num>>8)&0xFF);
	frame.datas[5] = (uint8_t)((page_num)&0xFF);
	send_command(&frame);
}




bool_e parse_frames(uint8_t byte, frame_t * pframe)
{
	typedef enum
	{
		STEP_HEADER_WAIT_EF,
		STEP_HEADER_WAIT_01,
		STEP_ADDR0,
		STEP_ADDR1,
		STEP_ADDR2,
		STEP_ADDR3,
		STEP_PID,
		STEP_LENGTH0,
		STEP_LENGTH1,
		STEP_DATAS,
		STEP_CS0,
		STEP_CS1
	}step_e;

	static step_e step = STEP_HEADER_WAIT_EF;
	static uint16_t size_datas = 0;
	static uint16_t checksum;
	static uint16_t sum;

	bool_e ret;
	ret = FALSE;
	if(step==STEP_PID)
		sum = byte;
	else if(step == STEP_LENGTH0 || step == STEP_LENGTH1 || step == STEP_DATAS)
		sum = sum + ((uint16_t)(byte));

	switch(step)
	{
		case STEP_HEADER_WAIT_EF:
			if(byte == 0xEF)
				step = STEP_HEADER_WAIT_01;
			break;
		case STEP_HEADER_WAIT_01:
			if(byte == 0x01)
				step = STEP_ADDR0;
			else
				step = STEP_HEADER_WAIT_EF;
			break;
		case STEP_ADDR0:
			pframe->addr = (uint32_t)byte<<24;
			step++;
			break;
		case STEP_ADDR1:
			pframe->addr |= (uint32_t)byte<<16;
			step++;
			break;
		case STEP_ADDR2:
			pframe->addr |= (uint32_t)byte<<8;
			step++;
			break;
		case STEP_ADDR3:
			pframe->addr |= (uint32_t)byte;
			step++;
			break;
		case STEP_PID:
			if(byte == PID_COMMAND || byte == PID_DATA || byte == PID_ACK || byte ==PID_END)
			{
				pframe->pid = byte;
				printf("pid OK=%d\n", pframe->pid);
			}
			else
			{
				printf("pid KO !!!=%d\n", pframe->pid);
				pframe->pid = PID_UNKNOW;
			}
			step++;
			break;
		case STEP_LENGTH0:
			pframe->lenght = U16FROMU8(byte, 0);
			step++;
			break;
		case STEP_LENGTH1:
			pframe->lenght |= ((uint16_t)(byte));

			if(pframe->lenght <= 256)
			{
				size_datas = 0;
				step = STEP_DATAS;
				printf("length=%d\n", pframe->lenght);
			}
			else
			{
				printf("mauvais length=%d\n", pframe->lenght);
				step = STEP_HEADER_WAIT_EF;
			}
			break;
		case STEP_DATAS:
			pframe->datas[size_datas] = byte;
			size_datas++;
			if(size_datas >= pframe->lenght - 2)
				step = STEP_CS0;
			break;
		case STEP_CS0:
			checksum = ((uint16_t)(byte))<<8;
			step++;
			break;
		case STEP_CS1:
			checksum |= ((uint16_t)(byte));
			if(checksum == sum)
			{
				ret = TRUE;
			}
			else
			{
				printf("mauvais checksum sum=%d, checksum=%d!\n", sum, checksum);
			}
			step = STEP_HEADER_WAIT_EF;
			break;
		default:
			break;
	}
	return ret;
}

void send_command(frame_t * pframe)
{
	uint8_t buf[300];
	uint16_t size = 0;
	buf[size++] = 0xEF;
	buf[size++] = 0x01;
	buf[size++] = (char)(pframe->addr>>24)&0xFF;
	buf[size++] = (char)(pframe->addr>>16)&0xFF;
	buf[size++] = (char)(pframe->addr>>8)&0xFF;
	buf[size++] = (char)(pframe->addr)&0xFF;
	buf[size++] = (pframe->pid);
	buf[size++] = (char)(pframe->lenght>>8)&0xFF;
	buf[size++] = (char)(pframe->lenght)&0xFF;
	for(int i = 0; i<pframe->lenght-2; i++)
		buf[size++] = pframe->datas[i];
	uint16_t sum = 0;
	for(int i = 6; i<6+1+pframe->lenght; i++)
	{
		sum+= (uint16_t)buf[i];
	}
	buf[size++] = (char)(sum>>8)&0xFF;
	buf[size++] = (char)(sum)&0xFF;
	UART_puts(UART1_ID, buf, size);
}



void state_machine(void){
	typedef enum
	 {
			 INIT = 0,
			 LED_ALLUME,
			 GET_IMAGE,
			 FAIL,
			 IMG_TZ,
			 REG_MOD,
			 STORE,
			 SEARCH,


	 }state_e;
	 static state_e state = INIT;
	 static state_e previous_state = INIT; //permet de sauvegarder l'état précédent
	 bool_e entrance;
	 entrance = (state!=previous_state)?TRUE:FALSE;
	 //le booléen entrance est vrai si on vient juste de changer d'état !
	 previous_state = state; //l'état courant est sauvegardé, ce sera le prochain "état précédent"
	 static bool_e img_index = 0;
	 switch(state)
	 {
			 case INIT:
				 	 Systick_add_callback_function(&process_ms);
				 	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,0);
				 	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
				 	 state = LED_ALLUME;
				 	 break;
			 case LED_ALLUME:
				 	 if(entrance)
				 	 {
				 		 send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
				 		 t = 1500;
				 		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,0);
				 		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
				 	 }
				 	 if(flag_ack)
				 	 {
				 		 flag_ack = FALSE;
				 		 //ILI9341_Puts(0,0, "ack ok", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);
				 		 state = GET_IMAGE;
				 	 }
				 	 if(flag_ack_fail)
				 	 {
				 		 state = FAIL;
				 		 //ILI9341_Puts(0, 30, "ack fail", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);
				 	 }
				 	 if(!t){
				 		 //ILI9341_Puts(0, 60, "time out", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);
				 		 state = FAIL;
				 	 }
				 	 break;
			 case GET_IMAGE:
				 	 if(entrance)
				 	 {
				 		 send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMG);
				 		 t = 3000;
				 	 }
				 	 if(flag_ack)
				 	 {
				 		 flag_ack = FALSE;
						 //ILI9341_Puts(0,0, "ack ok", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_YELLOW);
						 state = IMG_TZ;
				 	 }
				 	 if(flag_ack_fail)
					 {
					 	 flag_ack_fail = FALSE;
						 state = FAIL;
						 //ILI9341_Puts(0, 60, "ack fail", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_YELLOW);
					 }
				 	 if(!t)
				 	 {
						 state = FAIL;
						 //ILI9341_Puts(0, 90, "time out", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_YELLOW);
					 }
				 	 break;
			  case IMG_TZ:
			 		 if(entrance)
			 		 {
			 			 send_Img2Tz(MODULE_DEFAULT_ADDRESS, 0x01+img_index);
			 			 t = 4000;
			 		 }
			 		 if(flag_ack)
			 		 {
			 			 flag_ack = FALSE;
			 			 //ILI9341_Puts(0,0, "ack ok", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_BLUE);
			 			 if(img_index == 0)
			 			 {
			 			 img_index = TRUE;
			 			 state = SEARCH;
			 			 }
			 			 else
			 			 {
			 			 img_index = FALSE;
			 			 state = SEARCH;
			 			 }
			 		  }
			 		  if(flag_ack_fail)
			 		  {
			 			 flag_ack_fail = FALSE;
			 			 state = FAIL;
			 			 //ILI9341_Puts(0, 60, "ack fail", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_BLUE);
			 		  }
			 		  if(!t)
			 		  {
			 			 flag_ack_fail = FALSE;
			 			 state = FAIL;
			 			 //ILI9341_Puts(0, 90, "time out", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_BLUE);
			 		  }
			 		  break;
			  case SEARCH:

			 		  if(entrance)
			 		  {
			 			  send_search(MODULE_DEFAULT_ADDRESS, 0x01, 0x0001, 0x0001);
			 			  t = 4000;
			 		  }
			 		  if(flag_ack)
			 		  {
			 			  state = SEARCH;
			 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,1);
			 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
			 			  ILI9341_Fill(ILI9341_COLOR_GREEN);
			 			  ILI9341_Puts(0,150, "Vous pouvez entrer", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_GREEN);
			 		  }

			 		  if(flag_ack_fail)
			 		  {
			 			  flag_ack_fail = FALSE;
			 			  state = FAIL;
			 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,0);
			 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
			 			  ILI9341_Fill(ILI9341_COLOR_RED);
			 			  ILI9341_Puts(0, 150, "Entree refusee", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_RED);

			 		  }
			 		  if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
			 		  {
			 		 		flag_ack_fail = FALSE;
			 		 		state= REG_MOD;
			 		  }
			 		  else
			 		  {
			 		 		state=SEARCH;
			 		  }
			 		  if(!t)
			 		  {
			 			  flag_ack_fail = FALSE;
			 			  state = FAIL;
			 			  //ILI9341_Puts(0, 90, "retirez votre empreinte", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_BLUE);
			 		  }
			 		  break;
			 	case REG_MOD:
			 				 if(entrance)
			 				 {
			 					send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, REG_MODEL);
			 				 	t = 3000;
			 				 }
			 				 if(flag_ack)
			 				 {
			 				 	 flag_ack = FALSE;
			 				 	 //ILI9341_Puts(0,0, "ack ok", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_RED);
			 				 	 state = STORE;
			 				 }
			 				 if(flag_ack_fail)
			 				 {
			 					 	flag_ack_fail = FALSE;
			 				 		state = GET_IMAGE;
			 				 		//ILI9341_Puts(0, 60, "ack fail - try again", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_BLUE);
			 				 }
			 				  if(!t){
			 					    flag_ack_fail = FALSE;
			 				 		state = FAIL;
			 				 		//ILI9341_Puts(0, 90, "time out", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_RED);
			 				  }
			 				  break;
			 	case STORE:
			 				 if(entrance)
			 				 {
			 						send_store(MODULE_DEFAULT_ADDRESS, 0x01, 0x0001);
			 						t = 8000;
			 				 }
			 				 if(flag_ack)
			 				 {
			 						flag_ack = FALSE;
			 						ILI9341_Puts(0,60, "Empreinte enregistree", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_RED);
			 						state = SEARCH;
			 				 }
			 				 if(flag_ack_fail)
			 				 {
			 					flag_ack_fail = FALSE;
			 					 state = FAIL;
			 						ILI9341_Puts(0, 60, "Echec de l'enregistrement", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_RED);
			 				}
			 				if(!t)
			 				{
			 						flag_ack_fail = FALSE;
			 						state = FAIL;
			 						//ILI9341_Puts(0, 90, "time out", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_RED);
			 				}
			 				break;


			 case FAIL:
				 t=500;
				 state = GET_IMAGE;
				 break;

			 default:
				 	 break;
	 }

}
#endif
