#ifndef _ENCODER_H_
#define _ENCODER_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/* in 50ms */
#define _100PERCENT_COUNT 500
#define  _90PERCENT_COUNT 450
#define  _80PERCENT_COUNT 400
#define  _70PERCENT_COUNT 350
#define  _60PERCENT_COUNT 300
#define  _50PERCENT_COUNT 250
#define  _40PERCENT_COUNT 200
#define  _30PERCENT_COUNT 150
#define  _20PERCENT_COUNT 100
#define  _10PERCENT_COUNT  50
#define          NO_COUNT   0
/* 扩展变量 ------------------------------------------------------------------*/
extern int16_t Encoder_OverflowCount[4];  //电机编码器溢出计数
extern __IO int16_t Encoder_CaptureNumber[4];  //电机编码器计数
/* 函数声明 ------------------------------------------------------------------*/
void Encoder_Init(void);
#endif /* _ENCODER_H_ */

/************************************************ 文件结束 ************************************************/
