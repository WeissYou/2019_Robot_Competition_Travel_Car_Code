#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  Motor_Left = 0,
	Motor_Right,
}Motor_Number;
/* 宏定义 --------------------------------------------------------------------*/
#define FORWARD_100PERCENT_PWM 999
#define FORWARD_90PERCENT_PWM  949
#define FORWARD_80PERCENT_PWM  899
#define FORWARD_70PERCENT_PWM  849
#define FORWARD_60PERCENT_PWM  799
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
#define BACKWARD_50PERCENT_PWM 249
#define BACKWARD_60PERCENT_PWM 199
#define BACKWARD_70PERCENT_PWM 149
#define BACKWARD_80PERCENT_PWM 099
#define BACKWARD_90PERCENT_PWM 049
#define BACKWARD_100PERCENT_PWM  1
/* 扩展变量 ------------------------------------------------------------------*/
extern __IO int16_t PWM_Duty[2];
/* 函数声明 ------------------------------------------------------------------*/
void DCMotor_Contrl(uint8_t motor_number, uint16_t PWM);

#endif /* _DC_Motor_h_ */

/************************************************ 文件结束 ************************************************/
