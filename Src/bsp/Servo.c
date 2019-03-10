/*****************************************************************************
  * 文件名称: Servo.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 舵机控制
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Servo.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
static uint16_t remaining_time_cnt = 0;
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 舵机初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 确保行进中小人双手下落
  ************************************/
void Servo_Init(void)
{
  /* 启动通道PWM输出，实际脉宽：0.5ms，对应舵机的0度 */
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
}

/*************************************
  * 函数功能: 碰触景点
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 小人抬手，一秒后落手
  ************************************/
void Touch_Platform(void)
{
	remaining_time_cnt = TOUCH_PLATFORM_DURATION;  //设置触碰平台时间
  __HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,100);// 脉宽：1.0ms，舵机运动抬手
}

/*************************************
  * 函数功能: 碰触景点处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 在任务定时器中轮循检查是否到时
  ************************************/
void Touch_Platform_Handle(void)
{
	if (remaining_time_cnt)  //检查是否存在剩余抬手时间
	{
		__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,100);// 脉宽：1.0ms，舵机运动抬手
		remaining_time_cnt--;
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,50);// 脉宽：0.5ms，舵机运动落手
	}
}
/************************************************ 文件结束 ************************************************/
