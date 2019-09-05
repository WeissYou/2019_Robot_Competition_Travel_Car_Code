/*****************************************************************************
  * 文件名称: Carrier.c
  * 作    者: 尤伟宏
  * 编写日期: 三月 2019
  * 功    能: 装配机构控制
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Carrier.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
uint8_t Carrier_Task = 0;
uint8_t Position_Flag = 0;  //机构位置标志位
uint8_t Stepper_Flag = 0;
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 机械臂位置处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Carrier_Position_Handle(void)
{ 
  if(Position_Flag == INIT)
  {
    if(HAL_GPIO_ReadPin(Limit_High_GPIO_Port, Limit_High_Pin) == GPIO_PIN_RESET)  //若限位开关1已被碰触
    {
      Stepper_Stop();  //步进电机停止
      Paw_Control(Open);  //卡爪张开
      Rotation_Control(Zero);  //旋转轴归位
      Arm_Control(Vertical);  //机械臂垂直于地
      Position_Flag = OVER;
    }
    else  /* if(HAL_GPIO_ReadPin(Limit_High_GPIO_Port, Limit_High_Pin) == GPIO_PIN_SET)  //若限位开关1未被碰触 */
    {
      Stepper_Start(Up);  //步进电机向上运动
    }
  }
  else if(Position_Flag == WAIT)
  {
    if(HAL_GPIO_ReadPin(Limit_Low_GPIO_Port, Limit_Low_Pin) == GPIO_PIN_RESET)  //若限位开关1已被碰触
    {
      Stepper_Stop();  //步进电机停止
      Position_Flag = OVER;
    }
    else  /* if(HAL_GPIO_ReadPin(Limit_Low_GPIO_Port, Limit_Low_Pin) == GPIO_PIN_SET)  //若限位开关2未被碰触 */
    {
      Stepper_Start(Down);  //步进电机向下运动
    }
  }
  /* else if(Position_Flag == OVER) */
}

/*************************************
  * 函数功能: 机械臂处理函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Carrier_Handle(void)
{
  static uint8_t i = 0;
  if(Carrier_Task == LOAD)
  {
    HAL_Delay(500);
    Paw_Control(Close);  //爪子闭合
    HAL_Delay(500);
    Arm_Control(OBLIQUE);  //手臂竖直微倾
    HAL_Delay(500);
    Rotation_Control(Semi);  //旋转轴旋转半周
    Carrier_Task = IDLE;  //结束动作
    HAL_TIM_Base_Start_IT(&htim7);  //任务定时器启动
  }
  else if(Carrier_Task == UNLOAD)
  {
  if(i == FIRST_TARGET)
  {
    Arm_Control(Halb);  //手臂倾斜
    HAL_Delay(500);
    /* 挪动调整 */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  else if(i == SECOND_TARGET)
  {
//    Rotation_Control(240);  //旋转轴旋转半周
    HAL_Delay(100);
    Arm_Control(Halb);  //手臂倾斜
    HAL_Delay(500);
    /* 挪动调整 */
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == THIRD_TARGET)
  {
    Arm_Control(Halb);  //手臂倾斜
    HAL_Delay(500);
    /* 挪动调整 */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == FOURTH_TARGET)
  {
    Arm_Control(Halb);  //手臂倾斜
    HAL_Delay(500);
    /* 挪动调整 */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == FIFTH_TARGET)
  {
    Arm_Control(Halb);  //手臂倾斜
    HAL_Delay(500);
    /* 挪动调整 */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
    
    Paw_Control(Open);  //爪子张开
    Rotation_Control(Semi);  //旋转轴旋转半周
    HAL_Delay(500);
    while(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
    {
      Straight(BACK);
    }
    Stop();
    HAL_Delay(500);
    Arm_Control(Horizontal);  //手臂水平
    Straight(BACK);
    HAL_Delay(500);
    Stop();
    Carrier_Task = IDLE;  //结束动作
    HAL_TIM_Base_Start_IT(&htim7);  //任务定时器启动
  }
  /* else if(Carrier_Task == IDLE) */
}

/************************************************ 文件结束 ************************************************/
