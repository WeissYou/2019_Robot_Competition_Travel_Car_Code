#ifndef _Control_h_
#define _Control_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "DC_Motor.h"
#include "Track.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct
{
	__IO int SetDir;  //�趨���� Desired Direction
	__IO double P_value;  //�������� Proportional Const
	__IO double D_value;  //΢�ֳ��� Derivative Const
  __IO int Delta_Speed;  //���ҵ������ Delta Speed of both side Motors
	__IO double Last_Error;  //�ϴ�ƫ��Error[-1]  
	__IO int Prev_Error;  //��ǰƫ��Error[-2]
}Dir_PD;  //����PID�ṹ�嶨��
/* �궨�� --------------------------------------------------------------------*/
typedef enum
{
  Left = 0,
  Right
}Dir;
/* ��չ���� ------------------------------------------------------------------*/
/* ����PID�ṹ��ָ�� */
static Dir_PD sDir_PID;
static Dir_PD *sptr = &sDir_PID;
/* �������� ------------------------------------------------------------------*/
void PD_Init(void);  //����PID��ʼ��
void PD_Function(uint8_t track);  //����PIDִ�к���
void Turn(uint8_t dir);
void Reverse(void);
void Stop(void);

#endif /* _Control_h_ */
/************************************************ �ļ����� ************************************************/

