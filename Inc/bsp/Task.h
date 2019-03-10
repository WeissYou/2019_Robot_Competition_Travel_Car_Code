#ifndef _Task_h_
#define _Task_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "LED.h"
#include "Servo.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  WAITING = 0,                  //等待出发
	TRIP,                         //去程
	RETURN,                       //回程
	
	FIRST_CROSSROAD,            //第一路口
	SECOND_CROSSROAD,            //第二路口
	THIRD_CROSSROAD,              //第三路口
	FOURTH_CROSSROAD,             //第四路口
	FIFTH_CROSSROAD,              //第五路口
	SIXTH_CROSSROAD,              //第六路口
	SEVENTH_CROSSROAD,            //第七路口
	EIGHTH_CROSSROAD,             //第八路口
  NINTH_CROSSROAD,              //第九路口
	TENTH_CROSSROAD,              //第十路口
	
	FIRST_EXTRA_CROSSROAD,        //第一额外路口
	SECOND_EXTRA_CROSSROAD,       //第二额外路口
	
	FIRST_OBSTACLED_CROSSROAD,    //第一障碍物路口
	SECOND_OBSTACLED_CROSSROAD,   //第二障碍物路口
	
	FIRST_PLATFORM,               //第一平台
	SECOND_PLATFORM,              //第二平台
	THIRD_PLATFORM,               //第三平台
	FOURTH_PLATFORM,               //第四平台
	FIFTH_PLATFORM,              //第五平台
	SIXTH_PLATFORM,               //第六平台
	SEVENTH_PLATFORM,               //第七平台
	
  BRIDGE,                       //桥
	SLOPE,                        //大斜坡
	DELTA,                        //三角区
	SEESAW,                      //跷跷板
	ROUNDABOUT,                   //转盘
}TASK_STATUS;// 任务状态
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void Task_Timer_Handle(void);
void Task_Delay_Start(unsigned int time);
#endif /* _Task_h_ */

/************************************************ 文件结束 ************************************************/
