/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @author  MMY Application Team , AMG CL
  * @version $Revision: 1506 $
  * @date    $Date: 2016-05-09 09:48:13 +0100 (Mon, 09 May 2016) $
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_it.h"    

/** @addtogroup Validation_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CURSOR_STEP             5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile bool uDelayUs;
volatile bool uTimeOut;
volatile bool uAppliTimeOut;
volatile bool terminal_display_enabled = 0;

bool uDataReady 				= false;
bool RF_DataExpected 			= false;
bool RF_DataReady 				= false;

uint16_t delay_micros           = 0;
uint16_t delay_timeout          = 0;
uint16_t delay_appli            = 0;
uint32_t nb_ms_elapsed          = 0;

/* External variables --------------------------------------------------------*/

#ifdef USE_CR95HF_UART
/* UART communication between the STM and the ST95 */
extern UART_HandleTypeDef huart;
#endif /* USE_CR95HF_UART */

extern TIM_HandleTypeDef        TimHandleDelay;
extern TIM_HandleTypeDef        TimHandleTimeout;
extern TIM_HandleTypeDef		TimHandleAppli;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
/******************************************************************************/


/**
 *	@brief  This function handles the RF transceiver interupts
 *	@brief  RFTRANS_95HF_IRQ_HANDLER	EXTI15_10_IRQHandler
 */

void RFTRANS_95HF_IRQ_HANDLER(void)
{
  HAL_GPIO_EXTI_IRQHandler(IRQOUT_RFTRANS_95HF_PIN);
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

/**
 * @brief  EXTI line detection callbacks
 * @param  GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == IRQOUT_RFTRANS_95HF_PIN)
  {		
    if(RF_DataExpected)			
      RF_DataReady = true;
    
    /* Answer to command ready*/
    uDataReady = true;		
  }
  if(GPIO_Pin == USER_BUTTON_PIN)
  {
    if ( BSP_PB_GetState( BUTTON_USER ) == GPIO_PIN_RESET )
    {
      
      terminal_display_enabled = 1;
      
    }
  }
}


/**
 *	@brief  This function configures the Extern Interrupt for the IRQ coming 
 *              from the RF transceiver
 */
void drvInt_Enable_Reply_IRQ(void)
{
  RF_DataExpected = false;
  uDataReady = false;
  
  HAL_NVIC_EnableIRQ(EXTI_RFTRANS_95HF_IRQ_CHANNEL);
}

/**
 *	@brief  This function configures the Extern Interrupt for the IRQ coming 
 *      from the RF transceiver
 */
void drvInt_Enable_RFEvent_IRQ(void)
{	
  RF_DataExpected = true;
  uDataReady = false;
  
  HAL_NVIC_EnableIRQ(EXTI_RFTRANS_95HF_IRQ_CHANNEL);
}

/**
 *	@brief  This function configures the Extern Interrupt for the IRQ coming 
 *      from the RF transceiver
 */
void drvInt_Disable_95HF_IRQ(void)
{
  RF_DataExpected = false;
  uDataReady = false;
  
  HAL_NVIC_DisableIRQ(EXTI_RFTRANS_95HF_IRQ_CHANNEL);
}

/**
 * @brief  This function handles the timer interrupt.
 * @param  None
 * @retval None
 */
void TIMER_DELAY_IRQ_HANDLER(void)
{
  HAL_TIM_IRQHandler(&TimHandleDelay);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMER_TIMEOUT_IRQ_HANDLER(void)
{
  HAL_TIM_IRQHandler(&TimHandleTimeout);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void APPLI_TIMER_TIMEOUT_IRQ_HANDLER(void)
{
  HAL_TIM_IRQHandler(&TimHandleAppli);
}


#ifdef USE_CR95HF_UART
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart);
}
#endif /* USE_CR95HF_UART */

/* Next function are wrapped in the IRQ handler */

/**
 * @brief  This function handles the timer interrupt.
 * @param  None
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &TimHandleDelay)
  {
    if (delay_micros > 0)
    	delay_micros--;
    else
    {
      uDelayUs = true;	
      /* Disable the Time out timer */
      HAL_TIM_Base_Stop_IT(htim);
    }
  }
  else if (htim == &TimHandleTimeout)
  {
    if (delay_timeout > 0)
      delay_timeout--;
    else
    {
      uTimeOut = true;	
      /* Disable the Time out timer */
      HAL_TIM_Base_Stop_IT(htim);
    }
  }
  else if (htim == &TimHandleAppli)
  {
    if (delay_appli > 0)
      delay_appli--;
    else
    {
      uAppliTimeOut = true;	
      /* Disable the Time out timer */
      HAL_TIM_Base_Stop_IT(htim);
    }
  }
}


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
