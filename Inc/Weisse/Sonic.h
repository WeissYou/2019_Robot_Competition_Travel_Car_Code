#ifndef _SONIC_H_
#define _SONIC_H_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct              //�����ߵ�ƽ����
{   
  uint8_t ucFinishFlag;
  uint8_t ucStartFlag;
  uint16_t usCtr;
  uint16_t usPeriod;
}STRUCT_CAPTURE;
/* �궨�� --------------------------------------------------------------------*/
// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��84MHz/��GENERAL_TIM_PRESCALER+1��
#define GENERAL_TIM_PRESCALER           83  // ʵ��ʱ��Ƶ��Ϊ��1MHz
// ���嶨ʱ�����ڣ�����ʱ����ʼ������BASIC_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define GENERAL_TIM_PERIOD              0xFFFF  // ��ʱ�������ж�Ƶ��Ϊ��1MHz/1000=1KHz����1ms��ʱ����
// ���ն�ʱ��Ƶ�ʼ���Ϊ�� 84MHz/��GENERAL_TIM_PRESCALER+1��/GENERAL_TIM_PERIOD
// ������Ҫ����20ms���ڶ�ʱ����������Ϊ�� 84MHz/��83+1��/1000=1Hz����1ms����
// �������� GENERAL_TIM_PRESCALER=83��GENERAL_TIM_PERIOD=1000��
#define GENERAL_TIM_STRAT_ICPolarity        TIM_INPUTCHANNELPOLARITY_RISING          //��������ʼ����
#define GENERAL_TIM_END_ICPolarity          TIM_INPUTCHANNELPOLARITY_FALLING         //�����Ľ�������
/* ��չ���� ------------------------------------------------------------------*/
extern STRUCT_CAPTURE strCapture;
extern uint32_t temp;
extern uint32_t ulTmrClk, ulTime;
/* �������� ------------------------------------------------------------------*/
void Sonic_Init(void);
void Sonic_Start(void);
void TIM_IC_Content(void);

#endif /* _DC_Motor_h_ */

/************************************************ �ļ����� ************************************************/
