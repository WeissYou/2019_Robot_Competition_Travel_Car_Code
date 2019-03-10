/*****************************************************************************
  * �ļ�����: Control.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: С������
  ****************************************************************************
  * ˵    ��: ���ڷ���PD�㷨
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Control.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define P_DAT 0.01f
#define D_DAT 0.01f
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ����PD��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void PD_Init(void)
{
	sptr -> SetDir = 6;  //�趨���� Desired Direction
	sptr -> P_value = P_DAT;  //�������� Proportional Const
	sptr -> D_value = D_DAT;  //΢�ֳ��� Derivative Const
  sptr -> Delta_Speed = 0;  //���ҵ������ Delta Speed of both side Motors
	sptr -> Prev_Error = 0;  //��ǰƫ��Error[-2]
}
/*************************************
  * ��������: ����PDִ�к���
  * �������: uint8_t track
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void PD_Function(uint8_t track)
{
	int pError, dError;
	sptr->Last_Error = sptr->Prev_Error;  //Last_ErrorΪ�ϴ�ƫ�Prev_ErrorΪ��ǰƫ��
	sptr->Prev_Error = track - sptr->SetDir;  //��ǰ��� = ʵ��ѹ��λ�� - ����ѹ��λ��
	sptr->Delta_Speed = PWM_Duty[Motor_Left] - PWM_Duty[Motor_Right];  //������� = ������ߵ�ƽʱ�� - �Ҳ����ߵ�ƽʱ��
	
	pError = sptr->Prev_Error * 2 / sptr->P_value;  //��������
	dError = sptr -> D_value * (sptr->Prev_Error - sptr->Last_Error);  //΢������
	
	if(sptr->Prev_Error > 0)
	{
		sptr->Delta_Speed += (pError + dError);  //������ٵ���
	}
	else /* if(sptr->Prev_Error < 0) */
	{
    sptr->Delta_Speed -= (pError + dError);  //������ٵ���
	}
	
	/* �µ�Ŀ�����ֵ */
  PWM_Duty[Motor_Left] += sptr->Delta_Speed;
  PWM_Duty[Motor_Right] += sptr->Delta_Speed;	
	
	/* �µ�ռ�ձ��趨 */
	DCMotor_Contrl(Motor_Left, PWM_Duty[Motor_Left]);
  DCMotor_Contrl(Motor_Right, PWM_Duty[Motor_Right]);
}

/*************************************
  * ��������: ת��90��
  * �������: uint8_t dir
  * �� �� ֵ: ��
  * ˵    ��: ����ָ���뷴��
  ************************************/
void Turn(uint8_t dir)
{
  if(dir == Right)
	{
		  DCMotor_Contrl(Motor_Left, FORWARD_100PERCENT_PWM);
		  DCMotor_Contrl(Motor_Right, FORWARD_100PERCENT_PWM);
	}
	else /* if(dir == Left) */
	{
		  DCMotor_Contrl(Motor_Left, BACKWARD_100PERCENT_PWM);
		  DCMotor_Contrl(Motor_Right, BACKWARD_100PERCENT_PWM);
	}
}

/*************************************
  * ��������: ת��180��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ����ָ���뷴��
  ************************************/
void Reverse(void)
{
	DCMotor_Contrl(Motor_Left, FORWARD_100PERCENT_PWM);
	DCMotor_Contrl(Motor_Right, FORWARD_100PERCENT_PWM);
}

/*************************************
  * ��������: ͣ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Stop(void)
{
	DCMotor_Contrl(Motor_Left, STOP_PWM);
	DCMotor_Contrl(Motor_Right, STOP_PWM);
}
/************************************************ �ļ����� ************************************************/
