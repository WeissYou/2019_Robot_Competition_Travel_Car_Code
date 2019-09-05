/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
//extern __IO uint8_t ADC_ConvertedValueLocal[12];
// AD转换结果值
//extern uint32_t ADC_ConvertedValue[12];
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Lock_ADC_Data(void);
void MX_TIM1_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Left_Sensor_1_Pin GPIO_PIN_2
#define Left_Sensor_1_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_3
#define KEY_1_GPIO_Port GPIOE
#define KEY_0_Pin GPIO_PIN_4
#define KEY_0_GPIO_Port GPIOE
#define Left_Sensor_3_Pin GPIO_PIN_6
#define Left_Sensor_3_GPIO_Port GPIOE
#define Left_Sensor_4_Pin GPIO_PIN_0
#define Left_Sensor_4_GPIO_Port GPIOC
#define Left_Sensor_2_Pin GPIO_PIN_2
#define Left_Sensor_2_GPIO_Port GPIOC
#define KEY_UP_Pin GPIO_PIN_0
#define KEY_UP_GPIO_Port GPIOA
#define Front_Sensor_Aout_Pin GPIO_PIN_1
#define Front_Sensor_Aout_GPIO_Port GPIOA
#define Right_Sensor_2_Pin GPIO_PIN_2
#define Right_Sensor_2_GPIO_Port GPIOA
#define Front_Sensor_D1_Pin GPIO_PIN_3
#define Front_Sensor_D1_GPIO_Port GPIOA
#define Right_Sensor_3_Pin GPIO_PIN_4
#define Right_Sensor_3_GPIO_Port GPIOA
#define Front_Sensor_D2_Pin GPIO_PIN_5
#define Front_Sensor_D2_GPIO_Port GPIOA
#define Right_Sensor_4_Pin GPIO_PIN_6
#define Right_Sensor_4_GPIO_Port GPIOA
#define Front_Sensor_D3_Pin GPIO_PIN_7
#define Front_Sensor_D3_GPIO_Port GPIOA
#define Right_Sensor_1_Pin GPIO_PIN_4
#define Right_Sensor_1_GPIO_Port GPIOC
#define Motor_IN2_Pin GPIO_PIN_8
#define Motor_IN2_GPIO_Port GPIOE
#define Motor_IN1_Pin GPIO_PIN_9
#define Motor_IN1_GPIO_Port GPIOE
#define Motor_IN4_Pin GPIO_PIN_10
#define Motor_IN4_GPIO_Port GPIOE
#define Motor_IN3_Pin GPIO_PIN_11
#define Motor_IN3_GPIO_Port GPIOE
#define Limit_High_Pin GPIO_PIN_13
#define Limit_High_GPIO_Port GPIOE
#define Limit_Low_Pin GPIO_PIN_14
#define Limit_Low_GPIO_Port GPIOE
#define Remote_CLK_Pin GPIO_PIN_12
#define Remote_CLK_GPIO_Port GPIOB
#define Remote_DO_Pin GPIO_PIN_13
#define Remote_DO_GPIO_Port GPIOB
#define Remote_DI_Pin GPIO_PIN_14
#define Remote_DI_GPIO_Port GPIOB
#define Remote_CS_Pin GPIO_PIN_15
#define Remote_CS_GPIO_Port GPIOB
#define Obstacle_Right_Pin GPIO_PIN_14
#define Obstacle_Right_GPIO_Port GPIOD
#define Obstacle_Left_Pin GPIO_PIN_15
#define Obstacle_Left_GPIO_Port GPIOD
#define Servo_1_Pin GPIO_PIN_6
#define Servo_1_GPIO_Port GPIOC
#define Servo_2_Pin GPIO_PIN_7
#define Servo_2_GPIO_Port GPIOC
#define Servo_3_Pin GPIO_PIN_8
#define Servo_3_GPIO_Port GPIOC
#define Debug_TX_Pin GPIO_PIN_9
#define Debug_TX_GPIO_Port GPIOA
#define Debug_RX_Pin GPIO_PIN_10
#define Debug_RX_GPIO_Port GPIOA
#define Buzzer_Signal_Pin GPIO_PIN_4
#define Buzzer_Signal_GPIO_Port GPIOD
#define RGB_B_Pin GPIO_PIN_5
#define RGB_B_GPIO_Port GPIOD
#define RGB_G_Pin GPIO_PIN_6
#define RGB_G_GPIO_Port GPIOD
#define RGB_R_Pin GPIO_PIN_7
#define RGB_R_GPIO_Port GPIOD
#define Stepper_DIR_Pin GPIO_PIN_6
#define Stepper_DIR_GPIO_Port GPIOB
#define Stepper_CP_Pin GPIO_PIN_8
#define Stepper_CP_GPIO_Port GPIOB
#define Stepper_EN_Pin GPIO_PIN_0
#define Stepper_EN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
