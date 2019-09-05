#ifndef _Track_h_
#define _Track_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "Control.h"
#include "data_flash.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define VALUE_OF_TRACK  200  //����Ѱ������ֵ
#define VALUE_OF_WHITE 200  //��ֹѭ������ֵ
/* ��չ���� ------------------------------------------------------------------*/
extern uint8_t Threshold[8];  //��ֵ
extern uint8_t Track;  //�켣
extern uint8_t Sensors;
extern uint8_t Black_Value[8];
extern uint8_t White_Value[8];
/* �������� ------------------------------------------------------------------*/
uint8_t Track_Recognition(void);
uint8_t Sensor_On_Track(void);
uint8_t Sensor_On_Cross(void);
void Get_Black_Value(void);
void Get_White_Value(void);
void Get_Threshold(void);
//uint8_t Automatic_Threshold(void);
void PD_Track(void);

#endif /* _Track_h_ */

/************************************************ �ļ����� ************************************************/
