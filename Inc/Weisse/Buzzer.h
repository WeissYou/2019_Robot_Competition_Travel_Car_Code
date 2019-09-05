#ifndef _Buzzer_h_
#define _Buzzer_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
	BEEPState_OFF = 0,
	BEEPState_ON,
}BEEPState_TypeDef;

/* 宏定义 --------------------------------------------------------------------*/
#define BEEP_ON                       HAL_GPIO_WritePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin,GPIO_PIN_RESET)    // 输出低电平
#define BEEP_OFF                      HAL_GPIO_WritePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin,GPIO_PIN_SET)  // 输出高电平
#define BEEP_TOGGLE                   HAL_GPIO_TogglePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin)                // 输出反转
#define IS_BEEP_STATE(STATE)           (((STATE) == BEEPState_OFF) || ((STATE) == BEEPState_ON))
// 蜂鸣器短鸣时间
#define BUZZER_SHORT_TWEET_TIME         2      // 设置蜂鸣器短鸣的时间为2个定时器中断周期，即20ms
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
//void BEEP_GPIO_Init(void);
void BEEP_StateSet(BEEPState_TypeDef state);
void Buzzer_Tweet(unsigned char tweet_time);
void Buzzer_Timer_Handle(void);

#endif /* _Buzzer_h_ */

/************************************************ 文件结束 ************************************************/
