#ifndef _Control_h_
#define _Control_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "DC_Motor.h"
#include "Track.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef struct
{
	__IO int SetDir;  //设定方向 Desired Direction
	__IO double P_value;  //比例常数 Proportional Const
	__IO double D_value;  //微分常数 Derivative Const
  __IO int Delta_Speed;  //左右电机差速 Delta Speed of both side Motors
	__IO double Last_Error;  //上次偏差Error[-1]  
	__IO int Prev_Error;  //当前偏差Error[-2]
}Dir_PD;  //方向PID结构体定义
/* 宏定义 --------------------------------------------------------------------*/
typedef enum
{
  Left = 0,
  Right
}Dir;
/* 扩展变量 ------------------------------------------------------------------*/
/* 方向PID结构体指针 */
static Dir_PD sDir_PID;
static Dir_PD *sptr = &sDir_PID;
/* 函数声明 ------------------------------------------------------------------*/
void PD_Init(void);  //方向PID初始化
void PD_Function(uint8_t track);  //方向PID执行函数
void Turn(uint8_t dir);
void Reverse(void);
void Stop(void);

#endif /* _Control_h_ */
/************************************************ 文件结束 ************************************************/

