/*****************************************************************************
  * 文件名称: DC_Motor.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 直流电机相关
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "DC_Motor.h"
#include "Control.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: L298N直流电机控制
  * 输入参数: number：电机编号，支持三个电机驱动
  *             参数：1：对应高级定时器通道1和互补通道1
  *                   2：停机
  *           PWM：电机速度和方向调节
  *             参数：0 - 999 ：数值与499差值越大，速度越快
  * 返 回 值: 无
  * 说    明：无
  */
void DCMotor_Contrl(uint8_t motor_number, uint16_t PWM)
{
  switch(motor_number)
  {
    case MOTOR_LEFT: 
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM);   
      break;
    case MOTOR_RIGHT:
      HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM);   
      break;
  }
}

/************************************************ 文件结束 ************************************************/
