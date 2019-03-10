/*****************************************************************************
  * 文件名称: Control.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 小车控制
  ****************************************************************************
  * 说    明: 基于方向PD算法
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Control.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define P_DAT 0.01f
#define D_DAT 0.01f
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 方向PD初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void PD_Init(void)
{
	sptr -> SetDir = 6;  //设定方向 Desired Direction
	sptr -> P_value = P_DAT;  //比例常数 Proportional Const
	sptr -> D_value = D_DAT;  //微分常数 Derivative Const
  sptr -> Delta_Speed = 0;  //左右电机差速 Delta Speed of both side Motors
	sptr -> Prev_Error = 0;  //当前偏差Error[-2]
}
/*************************************
  * 函数功能: 方向PD执行函数
  * 输入参数: uint8_t track
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void PD_Function(uint8_t track)
{
	int pError, dError;
	sptr->Last_Error = sptr->Prev_Error;  //Last_Error为上次偏差，Prev_Error为当前偏差
	sptr->Prev_Error = track - sptr->SetDir;  //当前误差 = 实际压线位置 - 理想压线位置
	sptr->Delta_Speed = PWM_Duty[Motor_Left] - PWM_Duty[Motor_Right];  //电机差速 = 左侧电机高电平时长 - 右侧电机高电平时长
	
	pError = sptr->Prev_Error * 2 / sptr->P_value;  //比例运算
	dError = sptr -> D_value * (sptr->Prev_Error - sptr->Last_Error);  //微分运算
	
	if(sptr->Prev_Error > 0)
	{
		sptr->Delta_Speed += (pError + dError);  //电机差速调整
	}
	else /* if(sptr->Prev_Error < 0) */
	{
    sptr->Delta_Speed -= (pError + dError);  //电机差速调整
	}
	
	/* 新的目标计数值 */
  PWM_Duty[Motor_Left] += sptr->Delta_Speed;
  PWM_Duty[Motor_Right] += sptr->Delta_Speed;	
	
	/* 新的占空比设定 */
	DCMotor_Contrl(Motor_Left, PWM_Duty[Motor_Left]);
  DCMotor_Contrl(Motor_Right, PWM_Duty[Motor_Right]);
}

/*************************************
  * 函数功能: 转向90°
  * 输入参数: uint8_t dir
  * 返 回 值: 无
  * 说    明: 基于指南针反馈
  ************************************/
void Turn(uint8_t dir)
{
  if(dir == Right)
	{
		  DCMotor_Contrl(Motor_Left, FORWARD_100PERCENT_PWM);
		  DCMotor_Contrl(Motor_Right, FORWARD_100PERCENT_PWM);
	}
	else /* if(dir == Left) */
	{
		  DCMotor_Contrl(Motor_Left, BACKWARD_100PERCENT_PWM);
		  DCMotor_Contrl(Motor_Right, BACKWARD_100PERCENT_PWM);
	}
}

/*************************************
  * 函数功能: 转向180°
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 基于指南针反馈
  ************************************/
void Reverse(void)
{
	DCMotor_Contrl(Motor_Left, FORWARD_100PERCENT_PWM);
	DCMotor_Contrl(Motor_Right, FORWARD_100PERCENT_PWM);
}

/*************************************
  * 函数功能: 停车
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Stop(void)
{
	DCMotor_Contrl(Motor_Left, STOP_PWM);
	DCMotor_Contrl(Motor_Right, STOP_PWM);
}
/************************************************ 文件结束 ************************************************/
