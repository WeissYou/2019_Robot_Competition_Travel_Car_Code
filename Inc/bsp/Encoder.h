#ifndef _ENCODER_H_
#define _ENCODER_H_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/* in 50ms */
#define _100PERCENT_COUNT 500
#define  _90PERCENT_COUNT 450
#define  _80PERCENT_COUNT 400
#define  _70PERCENT_COUNT 350
#define  _60PERCENT_COUNT 300
#define  _50PERCENT_COUNT 250
#define  _40PERCENT_COUNT 200
#define  _30PERCENT_COUNT 150
#define  _20PERCENT_COUNT 100
#define  _10PERCENT_COUNT  50
#define          NO_COUNT   0
/* ��չ���� ------------------------------------------------------------------*/
extern int16_t Encoder_OverflowCount[4];  //����������������
extern __IO int16_t Encoder_CaptureNumber[4];  //�������������
/* �������� ------------------------------------------------------------------*/
void Encoder_Init(void);
#endif /* _ENCODER_H_ */

/************************************************ �ļ����� ************************************************/
