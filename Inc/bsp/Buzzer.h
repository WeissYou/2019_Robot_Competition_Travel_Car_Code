#ifndef _Buzzer_h_
#define _Buzzer_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "Task.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum
{
	BEEPState_OFF = 0,
	BEEPState_ON,
}BEEPState_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define BEEP_ON                       HAL_GPIO_WritePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin,GPIO_PIN_RESET)    // ����͵�ƽ
#define BEEP_OFF                      HAL_GPIO_WritePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin,GPIO_PIN_SET)  // ����ߵ�ƽ
#define BEEP_TOGGLE                   HAL_GPIO_TogglePin(Buzzer_Signal_GPIO_Port,Buzzer_Signal_Pin)                // �����ת
#define IS_BEEP_STATE(STATE)           (((STATE) == BEEPState_OFF) || ((STATE) == BEEPState_ON))
// ����������ʱ��
#define BUZZER_SHORT_TWEET_TIME         10      // ���÷�����������ʱ��Ϊ25����ʱ���ж����ڣ���500ms
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void BEEP_GPIO_Init(void);
void BEEP_StateSet(BEEPState_TypeDef state);
void Buzzer_Tweet(unsigned char tweet_time);
void Buzzer_Timer_Handle(void);

#endif /* _Buzzer_h_ */

/************************************************ �ļ����� ************************************************/
