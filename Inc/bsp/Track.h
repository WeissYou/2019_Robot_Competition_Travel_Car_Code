#ifndef _Track_h_
#define _Track_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "Control.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
extern uint8_t Threshold;             //��ֵ
extern uint8_t Calculated_Track;         //����õ��ĺ���λ��
extern uint8_t Last_Calculated_Track;  //�ϴμ���õ��ĺ���λ��
extern uint16_t Objective_Track;  //ʵ���ϵĺ���λ��
extern uint16_t Last_Objective_Track;  //�ϴ�ʵ���ϵĺ���λ��
/* �궨�� --------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
uint16_t Basic_Track(void);
void Automatic_Threshold(void);
uint8_t Track_Recognition(void);
#endif /* _Track_h_ */

/************************************************ �ļ����� ************************************************/
