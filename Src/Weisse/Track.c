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
uint8_t Track = 0;  //���λ��
uint8_t Last_Track = 0;  //�ϴι��λ��
uint8_t Sensors = 0;
uint8_t Black_Gotten = 0;
uint8_t White_Gotten = 0;
uint8_t Threshold_Gotten = 0;
uint8_t Black_Value[8] = {144, 137, 168, 106, 125, 122, 139, 110};
uint8_t White_Value[8] = {246, 234, 254, 209, 223, 223, 246, 223};
uint8_t Threshold[8] = {195, 185, 211, 157, 174, 172, 192, 166};  //��ֵ
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ����Ѱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��������ʱ���е�����Ч
  ************************************/
void PD_Track(void)
{ 
//  if(Automatic_Threshold() < VALUE_OF_WHITE)  //������δƫ����
//  {
    PD_Control();  //PDѰ��
//  }
//  else
//  { 
//    PD_Control();
//    //Stop();
//  }
}

/*************************************
  * ��������: ��ȡѹ�ߵĴ���������
  * �������: ��
  * �� �� ֵ: uint8_t number
  * ˵    ��: ��
  ************************************/
uint8_t Sensor_On_Track(void)
{
  uint8_t i;
  uint8_t number = 0;
  for(i = 0; i < 8; i ++)
  {
    if(ADC_Converted_Value[i] <= Threshold[i])  //������ֵС����ֵ
    {
      number ++;  //����ֵ��ֵ�����Ϊ1
    }
  }
  return number;
}

/*************************************
  * ��������: �Զ���ֵ�㷨
  * �������: ��
  * �� �� ֵ: uint8_t threshold
  * ˵    ��: ��
  ************************************/
//uint8_t Automatic_Threshold(void)
//{
//   static uint8_t i;
////   static uint8_t threshold;  //�м����0���м����1����ֵ
//   static uint8_t value_max, value_min;  //�����С����ֵ
//	
//   value_max = ADC_Converted_Value[0];  //��̬��ֵ�㷨����ȡ������Сֵ
//   for(i = 0; i < 8; i ++)   
//   {
//     if(value_max <= ADC_Converted_Value[i])
//     value_max = ADC_Converted_Value[i];
//   }
//   value_min = ADC_Converted_Value[0];  //��Сֵ
//   for(i = 0; i < 8; i ++) 
//   {
//     if(value_min >= ADC_Converted_Value[i])
//     value_min = ADC_Converted_Value[i];
//   }
//   Threshold = (value_max + value_min) / 2;
//   
//   return Threshold;
//}//End of uint8_t Automatic_Threshold(void)

/*************************************
  * ��������: ��ȡ��ɫ����ֵ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Get_Black_Value(void)
{
  uint8_t i;
  if(!Black_Gotten)
  {
    if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);  //��ʱ20ms
      if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
      {
        while(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET);
        Get_ADC_Value();  //��ǰ��������������ADC����
        for(i = 0; i < 8; i ++)
        {
          Black_Value[i] = ADC_Converted_Value[i];
          printf("��%d·��ɫֵ��%d\n", i+1, ADC_Converted_Value[i]);
        }
        /* �����ʾ�� */
        BEEP_StateSet(BEEPState_ON);
        HAL_Delay(20);
        BEEP_StateSet(BEEPState_OFF);
        Black_Gotten = 1;
      }
    }
  }
}

/*************************************
  * ��������: ��ȡ��ɫ����ֵ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Get_White_Value(void)
{
  uint8_t i;
  if(!White_Gotten)
  {
    if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);  //��ʱ20ms
      if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)
      {
        while(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET);
        Get_ADC_Value();  //��ǰ��������������ADC����
        for(i = 0; i < 8; i ++)
        {
          White_Value[i] = ADC_Converted_Value[i];
          printf("��%d·��ɫֵ��%d\n", i+1, ADC_Converted_Value[i]);
        }
        /* �����ʾ�� */
        BEEP_StateSet(BEEPState_ON);
        HAL_Delay(20);
        BEEP_StateSet(BEEPState_OFF);
        White_Gotten = 1;
      }
    }
  }
}

/*************************************
  * ��������: ������ֵ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Get_Threshold(void)
{
  uint8_t i;
  if(!Threshold_Gotten)
  {
    if(Black_Gotten && White_Gotten)
    {
      if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
      {
        HAL_Delay(20);  //��ʱ20ms
        if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
        {
          while(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET);
          for(i = 0; i < 8; i ++)
          {
            Threshold[i] = (Black_Value[i] + White_Value[i]) / 2;
            printf("��%d·��ֵ��%d\n", i+1, Threshold[i]);
          }
          /* �����ʾ�� */
          BEEP_StateSet(BEEPState_ON);
          HAL_Delay(20);
          BEEP_StateSet(BEEPState_OFF);
          Threshold_Gotten = 1;
          HAL_TIM_Base_Start_IT(&htim7);  //����ʱ������
        }
      }
    }
  }
}
     
/*************************************
  * ��������: �켣ʶ���㷨
  * �������: uint8_t automatic_threshold
  * �� �� ֵ: uint8_t calculated_track
  * ˵    ��: ��
  ************************************/
uint8_t Track_Recognition(void)
{
         int8_t i;
	 uint8_t left, right;  //����õ��Ĺ켣λ��
//         uint8_t calculated_track;
	 left = right = 0;
         
	 for(i = 0; i < 8; i++)   //�����Ѱ�ҹ켣
	 {
		 if(ADC_Converted_Value[i] <= Threshold[i] /*|| ADC_Converted_Value[i + 1] <= Threshold*/)  //3.16�ڴ��޸�һ���жϷ��ţ�����Ӧ�̵װ���
		 {
		   left = i;
	           break;	
		 }
		 else 
		 {      
			 left = 0;
		 }
	 }
	 for(i = 7; i > -1; i--)  //���Ҳ�Ѱ�ҹ켣
         {
		 if(ADC_Converted_Value[i] <= Threshold[i] /*|| ADC_Converted_Value[i + 1] <= Threshold*/)  //3.16�ڴ��޸�һ���жϷ��ţ�����Ӧ�̵װ���
		 {
		   right = i;
		   break;	
		 }
		 else
		 {
			 right = 8;
		 }
         }
	 Track = (left + right) / 2;  //��������λ��
//         if(abs(Track - Last_Track) > 2)  //�����ι켣ƫ�����
//         {
//           Track = Last_Track;
//         }
//         Last_Track = Track;
	 return Track;
}//End of uint8_t Track_Recognition(void

/************************************************ �ļ����� ************************************************/
