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
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern __IO uint8_t ADC_ConvertedValueLocal[12];
// AD转换结果值
extern uint32_t ADC_ConvertedValue[12];
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Buzzer_Signal_Pin GPIO_PIN_2
#define Buzzer_Signal_GPIO_Port GPIOE
#define Buzzer_Pin GPIO_PIN_4
#define Buzzer_GPIO_Port GPIOE
#define Servo_1_Pin GPIO_PIN_5
#define Servo_1_GPIO_Port GPIOE
#define Servo_2_Pin GPIO_PIN_6
#define Servo_2_GPIO_Port GPIOE
#define Front_Sensor_8_Pin GPIO_PIN_0
#define Front_Sensor_8_GPIO_Port GPIOC
#define Front_Sensor_9_Pin GPIO_PIN_1
#define Front_Sensor_9_GPIO_Port GPIOC
#define Front_Sensor_10_Pin GPIO_PIN_2
#define Front_Sensor_10_GPIO_Port GPIOC
#define Front_Sensor_11_Pin GPIO_PIN_3
#define Front_Sensor_11_GPIO_Port GPIOC
#define Front_Sensor_1_Pin GPIO_PIN_2
#define Front_Sensor_1_GPIO_Port GPIOA
#define Front_Sensor_2_Pin GPIO_PIN_3
#define Front_Sensor_2_GPIO_Port GPIOA
#define Front_Sensor_3_Pin GPIO_PIN_4
#define Front_Sensor_3_GPIO_Port GPIOA
#define Front_Sensor_4_Pin GPIO_PIN_5
#define Front_Sensor_4_GPIO_Port GPIOA
#define Front_Sensor_5_Pin GPIO_PIN_6
#define Front_Sensor_5_GPIO_Port GPIOA
#define Front_Sensor_12_Pin GPIO_PIN_4
#define Front_Sensor_12_GPIO_Port GPIOC
#define Front_Sensor_6_Pin GPIO_PIN_0
#define Front_Sensor_6_GPIO_Port GPIOB
#define Front_Sensor_7_Pin GPIO_PIN_1
#define Front_Sensor_7_GPIO_Port GPIOB
#define Driver_IN2_Pin GPIO_PIN_8
#define Driver_IN2_GPIO_Port GPIOE
#define Driver_IN1_Pin GPIO_PIN_9
#define Driver_IN1_GPIO_Port GPIOE
#define Driver_IN4_Pin GPIO_PIN_10
#define Driver_IN4_GPIO_Port GPIOE
#define Driver_IN3_Pin GPIO_PIN_11
#define Driver_IN3_GPIO_Port GPIOE
#define MPU6050_TX_Pin GPIO_PIN_10
#define MPU6050_TX_GPIO_Port GPIOB
#define MPU6050_RX_Pin GPIO_PIN_11
#define MPU6050_RX_GPIO_Port GPIOB
#define Right_Sensor_2_Pin GPIO_PIN_15
#define Right_Sensor_2_GPIO_Port GPIOB
#define Right_Sensor_1_Pin GPIO_PIN_8
#define Right_Sensor_1_GPIO_Port GPIOD
#define Left_Sensor_2_Pin GPIO_PIN_9
#define Left_Sensor_2_GPIO_Port GPIOD
#define Left_Sensor_1_Pin GPIO_PIN_10
#define Left_Sensor_1_GPIO_Port GPIOD
#define Obstacle_Pin GPIO_PIN_9
#define Obstacle_GPIO_Port GPIOC
#define Debug_TX_Pin GPIO_PIN_9
#define Debug_TX_GPIO_Port GPIOA
#define Debug_RX_Pin GPIO_PIN_10
#define Debug_RX_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_10
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_11
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_12
#define LED_3_GPIO_Port GPIOC
#define LED_4_Pin GPIO_PIN_0
#define LED_4_GPIO_Port GPIOD
#define LED_5_Pin GPIO_PIN_1
#define LED_5_GPIO_Port GPIOD
#define LED_6_Pin GPIO_PIN_2
#define LED_6_GPIO_Port GPIOD
#define LED_7_Pin GPIO_PIN_3
#define LED_7_GPIO_Port GPIOD
#define LED_8_Pin GPIO_PIN_4
#define LED_8_GPIO_Port GPIOD
#define Compass_TX_Pin GPIO_PIN_5
#define Compass_TX_GPIO_Port GPIOD
#define Compass_RX_Pin GPIO_PIN_6
#define Compass_RX_GPIO_Port GPIOD
#define LED_9_Pin GPIO_PIN_8
#define LED_9_GPIO_Port GPIOB
#define LED_10_Pin GPIO_PIN_9
#define LED_10_GPIO_Port GPIOB
#define LED_11_Pin GPIO_PIN_0
#define LED_11_GPIO_Port GPIOE
#define LED_12_Pin GPIO_PIN_1
#define LED_12_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
