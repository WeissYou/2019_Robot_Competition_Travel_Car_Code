/*****************************************************************************
  * 文件名称: Control.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 小车控制
  ****************************************************************************
  * 说    明: 方向PD循迹、电机转速调节、停车位置调节
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Control.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
int16_t PWM_Duty[2] = {0};  //左右电机PWM高电平时长
uint8_t Alignment_Flag = 0;  //停车校准机制标志
uint8_t Location_Flag = 0;  //取物前定位机制标志
int Last_Error = 0;  //上次偏差
int Error = 0;  //当前偏差
uint8_t Converse_Flag = 0;  //倒车标志
uint8_t Stop_Flag = 0;
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: PD循迹
  * 输入参数: uint8_t track_recognition
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void PD_Control(void)
{
        uint8_t i;
	int p_value, d_value;  //比例运算结果、微分运算结果
 	Last_Error = Error;  //记录上次误差
	Error = Track - SET_DIR;  //获取当前误差
        if(Error < 0)
        {
          Error ++;
        }

//	  p_value = (int)(Error ^ 2) / P_Coefficient;  //比例运算
        p_value = P_DAT_999 * Error;  //比例运算
        if(p_value < 0)  //取绝对值
        {
          p_value = -p_value;
        }
	d_value = D_DAT_999 * (Error - Last_Error);  //微分运算
        if(d_value < 0)  //取绝对值
        {
          d_value = -d_value;
        }

        if(!Converse_Flag)  //若不倒车
        {
          /* 判断修正值应赋予的对象 */
          if(Error > 0)
          {
            PWM_Duty[MOTOR_LEFT] = FORWARD_100PERCENT_PWM - p_value - d_value;
            PWM_Duty[MOTOR_RIGHT] = FORWARD_100PERCENT_PWM;
          }
          else /* if(Error < 0) */
          {
            PWM_Duty[MOTOR_LEFT] = FORWARD_100PERCENT_PWM;
            PWM_Duty[MOTOR_RIGHT] = FORWARD_100PERCENT_PWM - p_value - d_value;
          }
        
          /* 转向与速度限定 */
          for(i = 0; i < 2; i ++)
          {
            if(PWM_Duty[i] >= FORWARD_100PERCENT_PWM)
            {
              PWM_Duty[i] = FORWARD_100PERCENT_PWM;
            }
            if(PWM_Duty[i] < STOP_PWM)
            {
              PWM_Duty[i] = STOP_PWM;
            }
        
	    /* 新的占空比设定 */
	    DCMotor_Contrl(i, PWM_Duty[i]);
          }
        }
        else
        {
          /* 判断修正值应赋予的对象 */
          if(Error > 0)
          {
            PWM_Duty[MOTOR_LEFT] = BACKWARD_100PERCENT_PWM + p_value + d_value;
            PWM_Duty[MOTOR_RIGHT] = BACKWARD_100PERCENT_PWM;
          }
          else /* if(Error < 0) */
          {
            PWM_Duty[MOTOR_LEFT] = BACKWARD_100PERCENT_PWM;
            PWM_Duty[MOTOR_RIGHT] = BACKWARD_100PERCENT_PWM + p_value + d_value;
          }
        
          /* 转向与速度限定 */
          for(i = 0; i < 2; i ++)
          {
            if(PWM_Duty[i] > STOP_PWM)
            {
              PWM_Duty[i] = STOP_PWM;
            }
            if(PWM_Duty[i] < BACKWARD_100PERCENT_PWM)
            {
              PWM_Duty[i] = BACKWARD_100PERCENT_PWM;
            }
            
	    /* 新的占空比设定 */
	    DCMotor_Contrl(i, PWM_Duty[i]);
          }
        }
}

/*************************************
  * 函数功能: 直行
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Straight(uint8_t dir)
{
  if(dir == FRONT)
  {
    DCMotor_Contrl(MOTOR_LEFT, FORWARD_100PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);
  }
  else
  {
    DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_100PERCENT_PWM);
  }
}

/*************************************
  * 函数功能: 慢速直行
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Slow_Straight(uint8_t dir)
{
  if(dir == FRONT)
  {
    DCMotor_Contrl(MOTOR_LEFT, FORWARD_50PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, FORWARD_50PERCENT_PWM);
  }
  else
  {
    DCMotor_Contrl(MOTOR_LEFT, BACKWARD_50PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_50PERCENT_PWM);
  }
}

/*************************************
  * 函数功能: 转向
  * 输入参数: uint8_t dir, uint8_t delay_ms
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Turn(uint8_t dir)
{
  if(dir == LEFT)  //设定方向
  {
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_90PERCENT_PWM);  //90
  }
  else
  {
      DCMotor_Contrl(MOTOR_LEFT, FORWARD_90PERCENT_PWM);  //90
      DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_100PERCENT_PWM);
  }
}

/*************************************
  * 函数功能: 慢速转向
  * 输入参数: uint8_t dir, uint8_t delay_ms
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Slow_Turn(uint8_t dir)
{
  if(dir == LEFT)  //设定方向
  {
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_50PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_40PERCENT_PWM);
  }
  else
  {
      DCMotor_Contrl(MOTOR_LEFT, FORWARD_40PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_50PERCENT_PWM);
  }
}

/*************************************
  * 函数功能: 停车位置校准处理函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Alignment_Handle(void)
{
  if(Alignment_Flag)
  {
    if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)  //若车已经停准
    {
      Stop();  //停车
      Alignment_Flag = 0;  //结束校准机制
    }
    else if(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
    {
      Slow_Straight(FRONT);
    }
    else 
    {
      Slow_Straight(BACK);
    }
  }
}

/*************************************
  * 函数功能: 定位处理函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Location_Handle(void)
{
  if(Location_Flag != OFF)
  {
    if(Location_Flag == ON)
    {
      if(LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_1_BLACK)  //若当前位置过于靠后
      {
        Slow_Straight(FRONT);  //前进
      }
      else if((LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_3_BLACK) || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_4_BLACK))  //若当前位置过于靠前
      {
        Slow_Straight(BACK);  //后退
      }
      /* 若当前位置偏左 */
      else if(  (LEFT_SENSOR_1_WHITE && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_4_BLACK)  //仅右4在黑线上
              || (LEFT_SENSOR_1_BLACK && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_4_WHITE)  //仅左1在黑线上
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_4_BLACK)  //左1和右4同时在黑线上
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_3_BLACK)  //左1和右3同时在黑线上
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_2_BLACK)  //左1和右2同时在黑线上
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_4_BLACK)  //左2和右4同时在黑线上
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_3_BLACK)  //左2和右3同时在黑线上
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_4_BLACK)  //左3和右4同时在黑线上
              )  
      {
        Slow_Turn(RIGHT);  //右调
      }
      /* 若当前位置偏右 */
      else if(  (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_1_WHITE && RIGHT_SENSOR_2_WHITE && RIGHT_SENSOR_3_WHITE && RIGHT_SENSOR_4_WHITE)  //仅左4在黑线上
              || (LEFT_SENSOR_1_WHITE && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_1_BLACK) //仅右1在黑线上
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_1_BLACK)  //左4和右1同时在黑线上
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_2_BLACK)  //左4和右2同时在黑线上
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_3_BLACK)  //左4和右3同时在黑线上
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_1_BLACK)  //左3和右1同时在黑线上
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_2_BLACK)  //左3和右2同时在黑线上
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_1_BLACK)  //左2和右1同时在黑线上
              )
      {
        Slow_Turn(LEFT);  //左调
      }
      else if((LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_2_BLACK) /*|| (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_3_BLACK) */)  //若车已停准
      {
        Stop();
        Location_Flag = PART_TWO;  //跳过第一部分调整
      }
    }
    if(Location_Flag == PART_TWO)
    {
      if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)  //若左3和右3传感器在线上
      {
        Stop();
        Location_Flag = OFF;  //结束定位机制
      }
      else //否则
      {
        Slow_Straight(FRONT);//前进
      }
    }
  }
}

/*************************************
  * 函数功能: 超时处理函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Overtime_Handle(void)
{
  static uint8_t overtime_timer = 0;
  static uint16_t overtime_cnt = 0;
  if(Location_Flag != OFF)
  {
    if(overtime_timer == 0)
    {
      overtime_cnt = OVERTIME;  //超时定时器启动
      overtime_timer ++;
    }
    else
    {
      if(overtime_cnt > 0)
      {
        overtime_cnt --;
      }
      else
      {
        Location_Flag = OFF;
        Stop();
        overtime_timer = 0;  //超时定时器关闭
      }
    }
  }
}

/*************************************
  * 函数功能: 刹车机制
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Break_Handle(void)
{
  static uint8_t stop_cnt = 0;
  static uint8_t stop_lock = 0;
  if(Stop_Flag)
  {
    if(!stop_lock)
    {
      stop_cnt = 10;  //设置停车时长100ms
      stop_lock = 1;  //停车状态锁
     /* 关闭PWM输出 */
      HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      /*Configure GPIO pin : Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN3_Pin */
      GPIO_InitStruct.Pin = Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN4_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(Motor_IN1_GPIO_Port, &GPIO_InitStruct);
  
      /*Configure GPIO pin : Motor_IN2_Pin */
      GPIO_InitStruct.Pin = Motor_IN2_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(Motor_IN2_GPIO_Port, &GPIO_InitStruct);
  
      HAL_GPIO_WritePin(Motor_IN1_GPIO_Port, Motor_IN1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN2_GPIO_Port, Motor_IN2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN3_GPIO_Port, Motor_IN3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN4_GPIO_Port, Motor_IN4_Pin, GPIO_PIN_RESET);
    }
    else
    {
      if(stop_cnt)
      {
        stop_cnt --;
      }
      else
      {
        HAL_GPIO_DeInit(Motor_IN1_GPIO_Port, Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN4_Pin);
        HAL_GPIO_DeInit(Motor_IN2_GPIO_Port, Motor_IN2_Pin);
        MX_TIM1_Init();
        Stop_Flag = 0;
        stop_lock = 0;
      }
    }
  }
}

/*************************************
  * 函数功能: 停车
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Stop(void)
{
  DCMotor_Contrl(MOTOR_LEFT, STOP_PWM);
  DCMotor_Contrl(MOTOR_RIGHT, STOP_PWM);
}
/************************************************ 文件结束 ************************************************/
