#ifndef _Track_h_
#define _Track_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "Control.h"
/* 类型定义 ------------------------------------------------------------------*/
extern uint8_t Threshold;             //阈值
extern uint8_t Calculated_Track;         //计算得到的黑线位置
extern uint8_t Last_Calculated_Track;  //上次计算得到的黑线位置
extern uint16_t Objective_Track;  //实际上的黑线位置
extern uint16_t Last_Objective_Track;  //上次实际上的黑线位置
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
uint16_t Basic_Track(void);
void Automatic_Threshold(void);
uint8_t Track_Recognition(void);
#endif /* _Track_h_ */

/************************************************ 文件结束 ************************************************/
