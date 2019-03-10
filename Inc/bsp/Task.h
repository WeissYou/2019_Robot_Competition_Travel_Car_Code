#ifndef _Task_h_
#define _Task_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "LED.h"
#include "Servo.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum
{
  WAITING = 0,                  //�ȴ�����
	TRIP,                         //ȥ��
	RETURN,                       //�س�
	
	FIRST_CROSSROAD,            //��һ·��
	SECOND_CROSSROAD,            //�ڶ�·��
	THIRD_CROSSROAD,              //����·��
	FOURTH_CROSSROAD,             //����·��
	FIFTH_CROSSROAD,              //����·��
	SIXTH_CROSSROAD,              //����·��
	SEVENTH_CROSSROAD,            //����·��
	EIGHTH_CROSSROAD,             //�ڰ�·��
  NINTH_CROSSROAD,              //�ھ�·��
	TENTH_CROSSROAD,              //��ʮ·��
	
	FIRST_EXTRA_CROSSROAD,        //��һ����·��
	SECOND_EXTRA_CROSSROAD,       //�ڶ�����·��
	
	FIRST_OBSTACLED_CROSSROAD,    //��һ�ϰ���·��
	SECOND_OBSTACLED_CROSSROAD,   //�ڶ��ϰ���·��
	
	FIRST_PLATFORM,               //��һƽ̨
	SECOND_PLATFORM,              //�ڶ�ƽ̨
	THIRD_PLATFORM,               //����ƽ̨
	FOURTH_PLATFORM,               //����ƽ̨
	FIFTH_PLATFORM,              //����ƽ̨
	SIXTH_PLATFORM,               //����ƽ̨
	SEVENTH_PLATFORM,               //����ƽ̨
	
  BRIDGE,                       //��
	SLOPE,                        //��б��
	DELTA,                        //������
	SEESAW,                      //���ΰ�
	ROUNDABOUT,                   //ת��
}TASK_STATUS;// ����״̬
/* �궨�� --------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void Task_Timer_Handle(void);
void Task_Delay_Start(unsigned int time);
#endif /* _Task_h_ */

/************************************************ �ļ����� ************************************************/
