/*****************************************************************************
  * �ļ�����: Sensor.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: �Ҷȴ�����������������������״̬���
  ****************************************************************************
  * ˵    ��:
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Sensor.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
uint16_t ADC_Converted_Value[8] = {0};
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ��ȡADC���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Get_ADC_Value(void)
{
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[7] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
  /* Ƭѡͨ��һ */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[2] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[0] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���  //
  }
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[3] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[1] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[5] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  } 
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[6] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
  /* Ƭѡͨ���� */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADCת����ʼ
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[4] = HAL_ADC_GetValue(&hadc1);  //��ȡADC���//
  }
}

/*************************************
  * ��������: ������У��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Sensor_Check(uint8_t dir)
{
  if(dir == LEFT)
  {
    if(RIGHT_SENSOR_1_WHITE)
    {
      RGB_R_ON;  //���
    }
    else 
    {
      RGB_R_OFF;
    }
  
    if(RIGHT_SENSOR_2_WHITE)
    {
      RGB_G_ON;  //�̹�
    }
    else 
    {
      RGB_G_OFF;  
    }
  
    if(RIGHT_SENSOR_3_WHITE)
    {
      RGB_B_ON;  //����
    }  
    else 
    {
      RGB_B_OFF;
    }
  }
  else
  {
    if(RIGHT_SENSOR_4_WHITE)
    {
      /* �Ƶ��� */
      RGB_R_ON;  
      RGB_G_ON;  
    }
    else 
    {
      RGB_G_OFF;  
      RGB_B_OFF;
    }
  
    if(RIGHT_SENSOR_1_WHITE)
    {
      RGB_R_ON;  //���
    }
    else 
    {
      RGB_R_OFF;
    }
  
    if(RIGHT_SENSOR_2_WHITE)
    {
      RGB_G_ON;  //�̹�
    }
    else 
    {
      RGB_G_OFF;  
    }
  
    if(RIGHT_SENSOR_3_WHITE)
    {
      RGB_B_ON;  //����
    }
    else 
    {
      RGB_B_OFF;
    }
  
    if(RIGHT_SENSOR_4_WHITE)
    {
      /* �Ƶ��� */
      RGB_R_ON;  
      RGB_G_ON;  
    }
    else 
    {
      RGB_G_OFF;  
      RGB_B_OFF;
    }
  }
}

/************************************************ �ļ����� ************************************************/
