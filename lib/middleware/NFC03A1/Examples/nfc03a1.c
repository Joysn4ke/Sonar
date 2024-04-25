/*
 * nfc03a1.c
 *
 *  Created on: 2 janv. 2017
 *      Author: Julie
 *      Code exemple pour le lecteur NFC - Lecture de l'identifiant d'une carte étudiante ESEO
 */

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "nfc03a1.h"

/* Code exemple du lecteur NFC03A1 : voir fichier NFC_example.txt--------------*/

/*
 * 1 - Inclure le header de la librairie NFC avec son chemin complet :
 *
 *		#include "../lib/bsp/NFC03A1/Examples/nfc03a1.h"
 */

/*
 * 2 - Déclarer le mode d'utilisation du module NFC, 2 modes sont disponibles
 * 	   dans cette librairie : Proximity Coupling Device (PCD), mode de lecture/
 * 	   écriture, et Proximity Inductive Coupling Card (PICC), mode d'émulation
 * 	   de tags.
 *
 *		extern DeviceMode_t devicemode;
 */

/*
 * 3 - Code à ajouter dans la fonction main avant la tâche de fond :
 *
 *		int8_t TagType = TRACK_NOTHING;
 *		bool TagDetected = false;
 *		bool terminal_msg_flag = false;
 *		static char dataOut[COM_XFER_SIZE];
 *
 *		NFC03A1_Init();
 *		NFC03A1_DisplayMenu();
 *
 *		terminal_msg_flag = true;
 */

/*
 * 4 - Code à ajouter dans la tâche de fond (attention utiliser fonction getUID
 * 	   plutôt que getUID_String, voir NFC_example.txt, problème substistant dans
 * 	   boucle)
 *
 *		devicemode = PCD;
 *		TagType = ConfigManager_TagHunting(TRACK_ALL);
 *
 *		if(TagType == TRACK_NFCTYPE4A) {
 *			TagDetected = true;
 *   		LED_Off(GREEN_LED1);
 *   		LED_On(BLUE_LED4);
 *
 *   		char* uid = NULL;
 *   		uid = getUID_String();
 *   		if (terminal_msg_flag == true) {
 *				terminal_msg_flag = false;
 *				sprintf(dataOut, "\n\n%s", uid);
 *				HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
 *			}
 *   		free(uid);
 *   	}
 *
 *		else {
 *   		TagDetected = false;
 *   		LED_Off(BLUE_LED2);
 *   		LED_Off(GREEN_LED3);
 *   		LED_Off(BLUE_LED4);
 *   		LED_Toggle(GREEN_LED1);
 *			if(terminal_msg_flag == false) {
 *				terminal_msg_flag = true ;
 *				sprintf( dataOut, "\n\nCurrently there is no NFC tag in the vicinity");
 *				HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
 *			}
 *   	}
 *
 *   	HAL_Delay(300);
 */

/* Private variables ---------------------------------------------------------*/

/* Timer configuration */
TIM_HandleTypeDef		TimHandleDelay;
TIM_HandleTypeDef       TimHandleTimeout;
TIM_HandleTypeDef       TimHandleAppli;
uint32_t delay_us;

/* SPI communication between the STM and the ST95 */
SPI_HandleTypeDef       hspi;
UART_HandleTypeDef      huart;

/* PCD/PICC global memory space */

/* TT1 (PCD only)*/
uint8_t TT1Tag[NFCT1_MAX_TAGMEMORY];

/* TT2 */
uint8_t TT2Tag[NFCT2_MAX_TAGMEMORY];

/* TT3 */
uint8_t TT3Tag[NFCT3_MAX_TAGMEMORY];
uint8_t *TT3AttribInfo = TT3Tag, *TT3NDEFfile = &TT3Tag[NFCT3_ATTRIB_INFO_SIZE];

/* TT4 */
uint8_t CardCCfile      [NFCT4_MAX_CCMEMORY];
uint8_t CardNDEFfileT4A [NFCT4_MAX_NDEFMEMORY];
uint8_t CardNDEFfileT4B [NFCT4_MAX_NDEFMEMORY];

/* TT5 (PCD only)*/
uint8_t TT5Tag[NFCT5_MAX_TAGMEMORY];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);

static void Error_Handler(void);

void NFC03A1_Init(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick*/
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	 /* Initialize interface pin select com with CR95HF */
	RFTRANS_ConfigureInterfacePin();
	#ifdef USE_CR95HF_UART
	/* Set interface pin to select UART */
	HAL_GPIO_WritePin(INTERFACE_GPIO_PORT, INTERFACE_PIN, GPIO_PIN_RESET);
	#else
	/* Set interface pin to select SPI */
	HAL_GPIO_WritePin(INTERFACE_GPIO_PORT, INTERFACE_PIN, GPIO_PIN_SET);
	#endif /* USE_CR95HF_UART */
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();

	/* Init Leds of the X-NUCLEO-NFC03A1 expansion board */
	initLED();

	/* 95HF HW Init */
	ConfigManager_HWInit();

	/* Initialize UART for terminal logging */
	USARTConfig();
}

void NFC03A1_DisplayMenu(void) {
	static char dataOut[COM_XFER_SIZE];

	/* Run the main menu */
	LED_On(GREEN_LED1);
	HAL_Delay(1000);
	LED_Off(GREEN_LED1);

	/*---HT UI default welcome msg----------*/
	sprintf( dataOut, "\n\n---------------------------------------\n******Welcome to x-cube-nfc3******\n----------------------------------------");
	HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);

	sprintf(dataOut, "\n\nPlease bring a student card to the board vicinity");
	HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
*/
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Configure PLL ------------------------------------------------------*/
  /* PLL configuration: PLLCLK = (HSI / 2) * PLLMUL = (8 / 2) * 16 = 64 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLLCLK / HSEPredivValue = 64 / 1 = 64 MHz */
  /* Enable HSI and activate PLL with HSi_DIV2 as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.LSEState            = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSEPredivValue      = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType           = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource        = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider       = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider      = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider      = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable SYSCFG Clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
 *  @brief  Delay timer config
 *  @param  None
 *  @retval None
 */
void MX_TIM2_Init(void)
{
  /* Set TIMx instance */
  TimHandleDelay.Instance             = TIMER_US_DELAY;
  TimHandleDelay.Init.Period          = TIMER_US_DELAY_PERIOD;
  TimHandleDelay.Init.Prescaler       = TIMER_US_DELAY_PRESCALER;
  TimHandleDelay.Init.ClockDivision   = 0;
  TimHandleDelay.Init.CounterMode     = TIM_COUNTERMODE_DOWN;
  HAL_TIM_Base_Init(&TimHandleDelay);
}

/**
 *  @brief  Timeout timer config
 *  @param  None
 *  @retval None
 */void MX_TIM3_Init(void)
{
  /* Set TIMx instance */
  TimHandleTimeout.Instance           = TIMER_TIMEOUT;
  TimHandleTimeout.Init.Period        = TIMER_TIMEOUT_PERIOD;
  TimHandleTimeout.Init.Prescaler     = TIMER_TIMEOUT_PRESCALER;
  TimHandleTimeout.Init.ClockDivision = 0;
  TimHandleTimeout.Init.CounterMode   = TIM_COUNTERMODE_DOWN;
  HAL_TIM_Base_Init(&TimHandleTimeout);
}

/**
 *  @brief  Appication timer config
 *  @param  None
 *  @retval None
 */
void MX_TIM4_Init(void)
{
  /* Set TIMx instance */
  TimHandleAppli.Instance             = APPLI_TIMER_TIMEOUT;
  TimHandleAppli.Init.Period          = APPLI_TIMER_TIMEOUT_PERIOD;
  TimHandleAppli.Init.Prescaler       = APPLI_TIMER_TIMEOUT_PRESCALER;
  TimHandleAppli.Init.ClockDivision   = 0;
  TimHandleAppli.Init.CounterMode     = TIM_COUNTERMODE_DOWN;
  HAL_TIM_Base_Init(&TimHandleAppli);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}




