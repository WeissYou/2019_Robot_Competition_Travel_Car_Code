#ifndef _RGB_H_
#define _RGB_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/* RGB亮灭控制 */
#define RGB_R_ON (HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_RESET))
#define RGB_R_OFF (HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_SET))
#define RGB_G_ON (HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_RESET))
#define RGB_G_OFF (HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_SET))
#define RGB_B_ON (HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_RESET))
#define RGB_B_OFF (HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_SET))
/* RGB闪烁 */
#define RGB_R_BLK (HAL_GPIO_TogglePin(RGB_R_GPIO_Port, RGB_R_Pin))
#define RGB_G_BLK (HAL_GPIO_TogglePin(RGB_G_GPIO_Port, RGB_G_Pin))
#define RGB_B_BLK (HAL_GPIO_TogglePin(RGB_B_GPIO_Port, RGB_B_Pin))
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void RGB_Init(void);
void RGB_Blink(void);

#endif /* _RGB_h_ */

/************************************************ 文件结束 ************************************************/
