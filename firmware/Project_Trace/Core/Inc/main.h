/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Disp_CS_Pin GPIO_PIN_3
#define Disp_CS_GPIO_Port GPIOE
#define Disp_RES_Pin GPIO_PIN_5
#define Disp_RES_GPIO_Port GPIOE
#define Disp_DC_Pin GPIO_PIN_6
#define Disp_DC_GPIO_Port GPIOE
#define SW4_A_Pin GPIO_PIN_0
#define SW4_A_GPIO_Port GPIOA
#define SW4_B_Pin GPIO_PIN_3
#define SW4_B_GPIO_Port GPIOA
#define SPI1_SYNC_Pin GPIO_PIN_4
#define SPI1_SYNC_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SW3_A_Pin GPIO_PIN_6
#define SW3_A_GPIO_Port GPIOA
#define SW3_B_Pin GPIO_PIN_1
#define SW3_B_GPIO_Port GPIOB
#define SPI3_MOSI_Pin GPIO_PIN_2
#define SPI3_MOSI_GPIO_Port GPIOB
#define Range_att_40db_Pin GPIO_PIN_12
#define Range_att_40db_GPIO_Port GPIOB
#define Range_att_20db_Pin GPIO_PIN_13
#define Range_att_20db_GPIO_Port GPIOB
#define V_A_meter_Pin GPIO_PIN_15
#define V_A_meter_GPIO_Port GPIOB
#define ADC_FS0_Pin GPIO_PIN_14
#define ADC_FS0_GPIO_Port GPIOD
#define Range_gain_20db_Pin GPIO_PIN_6
#define Range_gain_20db_GPIO_Port GPIOC
#define Range_att_0db_Pin GPIO_PIN_7
#define Range_att_0db_GPIO_Port GPIOC
#define CLK_EN_Pin GPIO_PIN_8
#define CLK_EN_GPIO_Port GPIOC
#define MCO2_Pin GPIO_PIN_9
#define MCO2_GPIO_Port GPIOC
#define SPI3_SCK_Pin GPIO_PIN_10
#define SPI3_SCK_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_3
#define SW2_GPIO_Port GPIOD
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOD
#define ACCoupleEnable_Pin GPIO_PIN_3
#define ACCoupleEnable_GPIO_Port GPIOB
#define SPI1_MOSI_Pin GPIO_PIN_5
#define SPI1_MOSI_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
