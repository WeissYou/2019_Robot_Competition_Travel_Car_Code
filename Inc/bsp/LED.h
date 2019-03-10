#ifndef _LED_h_
#define _LED_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define LED_1_OFF  (HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET)) 
#define	LED_2_OFF  (HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET)) 
#define	LED_3_OFF  (HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET)) 
#define	LED_4_OFF  (HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET)) 
#define	LED_5_OFF  (HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_RESET)) 
#define	LED_6_OFF  (HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_RESET)) 
#define	LED_7_OFF  (HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_RESET)) 
#define	LED_8_OFF  (HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_RESET)) 
#define	LED_9_OFF  (HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_9_Pin, GPIO_PIN_RESET)) 
#define	LED_10_OFF (HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_10_Pin, GPIO_PIN_RESET)) 
#define	LED_11_OFF (HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_11_Pin, GPIO_PIN_RESET)) 
#define	LED_12_OFF (HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_12_Pin, GPIO_PIN_RESET)) 

#define LED_1_ON  (HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET)) 
#define	LED_2_ON  (HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET)) 
#define	LED_3_ON  (HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET)) 
#define	LED_4_ON  (HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET)) 
#define	LED_5_ON  (HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_SET)) 
#define	LED_6_ON  (HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_SET)) 
#define	LED_7_ON  (HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_SET)) 
#define	LED_8_ON  (HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_SET)) 
#define	LED_9_ON  (HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_9_Pin, GPIO_PIN_SET)) 
#define	LED_10_ON (HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_10_Pin, GPIO_PIN_SET)) 
#define	LED_11_ON (HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_11_Pin, GPIO_PIN_SET)) 
#define	LED_12_ON (HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_12_Pin, GPIO_PIN_SET)) 
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void LED_Init(void);
void LED_Blink(uint8_t Calculated_Track);
void LED_blink(uint16_t Objective_Track);
#endif /* _LED_h_ */

/************************************************ 文件结束 ************************************************/
