#ifndef _Servo_h_
#define _Servo_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
//�������ָ��
/* ��ʱ��3ͨ��1 */
#define Semi 230  //��ת����
#define Zero 98  //ԭ��

/* ��ʱ��3ͨ��2 */
#define Open       50  //צ���ſ�
#define Half       130  //צ�Ӱ���
#define Close      200  //צ�ӱպ�

/* ��ʱ��3ͨ��3 */
#define Vertical 130  //��е������ֱ
#define OBLIQUE 140  //��е����б
#define Halb  170  //��е�۸���б
#define Horizontal   230  //��е����ˮƽ
/* �궨�� --------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void Servo_Init(void);
void Arm_Control(uint8_t action);
void Rotation_Control(uint8_t action);
void Paw_Control(uint8_t action);
void Motor_PWM_SetDutyRatio(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t value);

#endif /* _Servo_h_ */

/************************************************ �ļ����� ************************************************/
