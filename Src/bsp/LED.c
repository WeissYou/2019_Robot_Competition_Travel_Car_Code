/*****************************************************************************
  * 文件名称: LED.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: LED指示灯相关
  ****************************************************************************
  * 说    明: 共阴极解法，高电平有效
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "LED.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: LED初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 默认不发光
  ************************************/
void LED_Init(void)
{
  LED_1_OFF;
	LED_2_OFF;
	LED_3_OFF;
	LED_4_OFF;
	LED_5_OFF;
	LED_6_OFF;
	LED_7_OFF;
	LED_8_OFF;
	LED_9_OFF;
	LED_10_OFF;
	LED_11_OFF;
	LED_12_OFF;
}//End of void LED_Init(void)

/*************************************
  * 函数功能: LED发光
  * 输入参数: uint8_t track_position
  * 返 回 值: 无
  * 说    明: 配合轨迹识别算法使用
  ************************************/
void LED_Blink(uint8_t Calculated_Track)
{
	switch(Calculated_Track)
	{
		case 0:  LED_1_ON;
		case 1:  LED_2_ON;
		case 2:  LED_3_ON;
		case 3:  LED_4_ON;
		case 4:  LED_5_ON;
		case 5:  LED_6_ON;
		case 6:  LED_7_ON;
		case 7:  LED_8_ON;
		case 8:  LED_9_ON;
		case 9: LED_10_ON;
		case 10:LED_11_ON;
		case 11:LED_12_ON;
	}
}//End of void LED_Blink(void)

/*************************************
  * 函数功能: LED发光
  * 输入参数: uint16_t Objective_Track
  * 返 回 值: 无
  * 说    明: 配合一般二值化使用
  ************************************/
void LED_blink(uint16_t Objective_Track)
{
    if (Objective_Track&(0x0001 << 0))
      HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 1))
      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 2))
      HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 3))
      HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 4))
      HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 5))
      HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 6))
      HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 7))
      HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 8))
      HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 9))
      HAL_GPIO_WritePin(LED_10_GPIO_Port, LED_10_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_10_GPIO_Port, LED_10_Pin, GPIO_PIN_RESET);

    if (Objective_Track&(0x0001 << 10))
      HAL_GPIO_WritePin(LED_11_GPIO_Port, LED_11_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_11_GPIO_Port, LED_11_Pin, GPIO_PIN_RESET);
		
    if (Objective_Track&(0x0001 << 11))
      HAL_GPIO_WritePin(LED_12_GPIO_Port, LED_12_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(LED_12_GPIO_Port, LED_12_Pin, GPIO_PIN_RESET);

}//End of void LED_blink(void)
/************************************************ 文件结束 ************************************************/
