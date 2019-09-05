#ifndef _SONIC_H_
#define _SONIC_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef struct              //测量高电平脉宽
{   
  uint8_t ucFinishFlag;
  uint8_t ucStartFlag;
  uint16_t usCtr;
  uint16_t usPeriod;
}STRUCT_CAPTURE;
/* 宏定义 --------------------------------------------------------------------*/
// 定义定时器预分频，定时器实际时钟频率为：84MHz/（GENERAL_TIM_PRESCALER+1）
#define GENERAL_TIM_PRESCALER           83  // 实际时钟频率为：1MHz
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD              0xFFFF  // 定时器产生中断频率为：1MHz/1000=1KHz，即1ms定时周期
// 最终定时器频率计算为： 84MHz/（GENERAL_TIM_PRESCALER+1）/GENERAL_TIM_PERIOD
// 比如需要产生20ms周期定时，可以设置为： 84MHz/（83+1）/1000=1Hz，即1ms周期
// 这里设置 GENERAL_TIM_PRESCALER=83；GENERAL_TIM_PERIOD=1000；
#define GENERAL_TIM_STRAT_ICPolarity        TIM_INPUTCHANNELPOLARITY_RISING          //测量的起始边沿
#define GENERAL_TIM_END_ICPolarity          TIM_INPUTCHANNELPOLARITY_FALLING         //测量的结束边沿
/* 扩展变量 ------------------------------------------------------------------*/
extern STRUCT_CAPTURE strCapture;
extern uint32_t temp;
extern uint32_t ulTmrClk, ulTime;
/* 函数声明 ------------------------------------------------------------------*/
void Sonic_Init(void);
void Sonic_Start(void);
void TIM_IC_Content(void);

#endif /* _DC_Motor_h_ */

/************************************************ 文件结束 ************************************************/
