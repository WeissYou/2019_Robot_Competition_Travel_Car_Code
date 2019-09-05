#ifndef _Track_h_
#define _Track_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "Control.h"
#include "data_flash.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define VALUE_OF_TRACK  200  //允许寻迹的阈值
#define VALUE_OF_WHITE 200  //禁止循迹的阈值
/* 扩展变量 ------------------------------------------------------------------*/
extern uint8_t Threshold[8];  //阈值
extern uint8_t Track;  //轨迹
extern uint8_t Sensors;
extern uint8_t Black_Value[8];
extern uint8_t White_Value[8];
/* 函数声明 ------------------------------------------------------------------*/
uint8_t Track_Recognition(void);
uint8_t Sensor_On_Track(void);
uint8_t Sensor_On_Cross(void);
void Get_Black_Value(void);
void Get_White_Value(void);
void Get_Threshold(void);
//uint8_t Automatic_Threshold(void);
void PD_Track(void);

#endif /* _Track_h_ */

/************************************************ 文件结束 ************************************************/
