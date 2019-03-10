/*****************************************************************************
  * �ļ�����: Track.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: ѭ�����
  ****************************************************************************
  * ˵    ��:
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Track.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
uint8_t Threshold;             //��ֵ
uint8_t Calculated_Track;         //����õ��ĺ���λ��
uint8_t Last_Calculated_Track;  //�ϴμ���õ��ĺ���λ��
uint16_t Objective_Track;  //ʵ���ϵĺ���λ��
uint16_t Last_Objective_Track;  //�ϴ�ʵ���ϵĺ���λ��
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: �Զ���ֵ�㷨
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Automatic_Threshold(void)
{
	 static uint8_t i;
	 static uint8_t value1_max, value1_min;
	
	 value1_max = ADC_ConvertedValueLocal[0];  //��̬��ֵ�㷨����ȡ������Сֵ
   for(i = 0; i < 12; i ++)   
   {
     if(value1_max <= ADC_ConvertedValueLocal[i])
     value1_max = ADC_ConvertedValueLocal[i];
   }
	 value1_min = ADC_ConvertedValueLocal[0];  //��Сֵ
   for(i = 0; i < 12; i ++) 
   {
     if(value1_min >= ADC_ConvertedValueLocal[i])
     value1_min = ADC_ConvertedValueLocal[i];
   }
   Threshold = (value1_max + value1_min) / 2;	  //���������������ȡ����ֵ
}//End of void Automatic_Threshold(void)

/*************************************
  * ��������: �򵥶�ֵ���㷨
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
uint16_t Basic_Binary_Code(void)
{
	uint8_t i;
  for(i = 0; i < 12; i ++)
	{
	  if(ADC_ConvertedValueLocal[i] >= Threshold)
		{
			Objective_Track |= 0x0001 < i;
		}
		else /* if(ADC_ConvertedValueLocal[i] < Threshold) */
		{
			Objective_Track &= ~(0x0001 < i);
		}
	}
	Last_Objective_Track = Objective_Track;  //������һ�εĹ켣
	return Objective_Track;
}

/*************************************
  * ��������: �켣ʶ���㷨
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
uint8_t Track_Recognition(void)
{
	 static int8_t i;
	 static uint8_t left, right;
	 for(i = 0; i < 12; i++)   //�����Ѱ�ҹ켣
	 {
		 if(ADC_ConvertedValueLocal[i] > Threshold && ADC_ConvertedValueLocal[i + 1] > Threshold)
		 {
		   left = i;
			 break;	
		 }
	 }
	 for(i = 11; i > -1; i--)  //���Ҳ�Ѱ�ҹ켣
   {
		 if(ADC_ConvertedValueLocal[i] > Threshold && ADC_ConvertedValueLocal[i + 1] > Threshold)
		 {
		   right = i;
		   break;	
		 }
   }
	 Calculated_Track = (right + left) / 2;  //��������λ��
//	 if(abs(Calculated_Track - Last_Calculated_Track) > 4)   //�������ߵ�ƫ����̫��
//	 {
//	   Calculated_Track = Last_Calculated_Track;    //��ȡ��һ�ε�ֵ
//	 }
	 Last_Calculated_Track = Calculated_Track;  //������һ�εĹ켣
	 return Calculated_Track;
}//End of Track_Recognition(void)
/************************************************ �ļ����� ************************************************/
