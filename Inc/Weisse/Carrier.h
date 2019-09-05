#ifndef _Carrier_h_
#define _Carrier_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "Servo.h"
#include "Sensor.h"
#include "Stepper.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  IDLE = 0,  //空闲
  LOAD,  //上料
  UNLOAD,  //下料
}Carrier_Task_Flag;  //机构任务标志位

typedef enum
{
  OVER = 0,  //结束
  INIT,  //机构初始化
  WAIT,  //移至下极限位置等待卸货
}Carrier_Position_Flag;  //机构位置标志位
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
extern uint8_t Carrier_Task;
extern uint8_t Position_Flag;  //机构任务标志位
/* 函数声明 ------------------------------------------------------------------*/
void Carrier_Position_Handle(void);
void Carrier_Handle(void);

#endif /* _Carrier_h_ */
/************************************************ 文件结束 ************************************************/

