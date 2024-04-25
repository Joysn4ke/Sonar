/**
  ******************************************************************************
  * @file    drv_spi.c 
  * @author  MMY Application Team
  * @version $Revision: 1508 $
  * @date    $Date: 2016-01-08 09:50:04 +0100 (Fri, 08 Jan 2016) $
  * @brief   This file provides a set of firmware functions to manages SPI communications
  ******************************************************************************
  * @copyright
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
	******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"
#include "stm32f1xx_nucleo.h" 


/** @addtogroup BSP
 * @{
 */

/** @addtogroup X_NUCLEO_NFC03A1_Spi
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_External_Variables
 * @{
 */
 /* SPI Handle declaration */
extern SPI_HandleTypeDef hspi;
/**
 * @}
 */ 

/* Global variables ----------------------------------------------------------*/
 /* Public Functions ----------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_Public_Functions
 * @{
 */
/**  
 *	@brief  Initialise HAL SPI for NFC03A1
 *  @param  None
 *  @retval None
 */
void RFTRANS_SPI_Init(void) 
{	
  
  if(HAL_SPI_GetState(&hspi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hspi.Instance	= RFTRANS_95HF_SPI;
    /* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
    to verify these constraints:
    - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
    Since the provided driver doesn't use read capability from LCD, only constraint 
    on write baudrate is considered.
    - SD card SPI interface max baudrate is 25MHz for write/read
    - PCLK2 max frequency is 100 MHz 
    */ 
    hspi.Init.Mode 		        = SPI_MODE_MASTER;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi.Init.NSS 				= SPI_NSS_SOFT;
    hspi.Init.CLKPolarity		= SPI_POLARITY_HIGH;
    hspi.Init.CLKPhase			= SPI_PHASE_2EDGE;
    hspi.Init.Direction 		= SPI_DIRECTION_2LINES;
    hspi.Init.DataSize 			= SPI_DATASIZE_8BIT;
    hspi.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
    hspi.Init.TIMode 			= SPI_TIMODE_DISABLE;
    hspi.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLE;
    /* Configure SPI Pin */
    RFTRANS_SPI_MSPInit();
    HAL_SPI_Init(&hspi);
  }
}






/**
 * @}
 */ 

/**
 * @}
 */ 

/**
 * @}
 */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
