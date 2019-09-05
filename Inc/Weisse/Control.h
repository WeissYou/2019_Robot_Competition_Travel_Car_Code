#ifndef _Control_h_
#define _Control_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "DC_Motor.h"
#include "Track.h"
#include "Task.h"
#include "Sensor.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  FRONT = 0,
  BACK,
  LEFT,
  RIGHT
}Dir_Command;

typedef enum
{
  OFF = 0,
  ON,
  PART_TWO
}Parking_Command;
/* 宏定义 --------------------------------------------------------------------*/
#define SET_DIR 4
/* 高速寻迹参数 */
#define P_DAT_999 50  
#define D_DAT_999 400
  
#define OVERTIME 400
/* 扩展变量 ------------------------------------------------------------------*/
extern int16_t PWM_Duty[2];
extern uint8_t Alignment_Flag;
extern uint8_t Location_Flag;
extern uint8_t Converse_Flag;
extern uint8_t Stop_Flag ;
/* 函数声明 ------------------------------------------------------------------*/
void PD_Control(void);
void Turn(uint8_t dir);
void Slow_Turn(uint8_t dir);
void Straight(uint8_t dir);
void Slow_Straight(uint8_t dir);
void Alignment_Handle(void);
void Location_Handle(void);
void Overtime_Handle(void);
void Break_Handle(void);
void Stop(void);

#endif /* _Control_h_ */
/************************************************ 文件结束 ************************************************/

