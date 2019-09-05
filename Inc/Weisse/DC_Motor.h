#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  MOTOR_LEFT = 0,
  MOTOR_RIGHT
}Motor_Number;
/* 宏定义 --------------------------------------------------------------------*/
#define FORWARD_100PERCENT_PWM 999
#define FORWARD_95PERCENT_PWM  974
#define FORWARD_90PERCENT_PWM  949
#define FORWARD_85PERCENT_PWM  924
#define FORWARD_80PERCENT_PWM  899
#define FORWARD_75PERCENT_PWM  874
#define FORWARD_70PERCENT_PWM  849
#define FORWARD_65PERCENT_PWM  824
#define FORWARD_60PERCENT_PWM  799
#define FORWARD_55PERCENT_PWM  774
#define FORWARD_50PERCENT_PWM  749
#define FORWARD_40PERCENT_PWM  699
#define FORWARD_30PERCENT_PWM  649
#define FORWARD_20PERCENT_PWM  599
#define FORWARD_10PERCENT_PWM  549
#define STOP_PWM               499
#define BACKWARD_10PERCENT_PWM 449
#define BACKWARD_20PERCENT_PWM 399
#define BACKWARD_30PERCENT_PWM 349
#define BACKWARD_40PERCENT_PWM 299
#define BACKWARD_45PERCENT_PWM 274
#define BACKWARD_50PERCENT_PWM 249
#define BACKWARD_60PERCENT_PWM 199
#define BACKWARD_70PERCENT_PWM 149
#define BACKWARD_80PERCENT_PWM 99
#define BACKWARD_90PERCENT_PWM 49
#define BACKWARD_95PERCENT_PWM 24
#define BACKWARD_100PERCENT_PWM  1
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void DCMotor_Contrl(uint8_t motor_number, uint16_t PWM);

#endif /* _DC_Motor_h_ */

/************************************************ 文件结束 ************************************************/
