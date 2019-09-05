/*****************************************************************************
  * �ļ�����: Button.c
  * ��    ��: ��ΰ��
  * ��д����: ���� 2019
  * ��    ��: ��������
  ****************************************************************************
  * ˵    ��: ͨ��������ȡ�����ؼ�ֵ
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Button.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ���������� */
#define KEY_UP_ON (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
#define KEY_0_ON (!HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin))
#define KEY_1_ON (!HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin))
/* ����û������ */
#define KEY_UP_OFF (!HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
#define KEY_0_OFF (HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin))
#define KEY_1_OFF (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin))
/* ˽�б��� ------------------------------------------------------------------*/
uint8_t Sampling = 0;
uint8_t Button_Pressed = 0;
uint8_t Wait = 0;
uint8_t Button_Delay_Time_Cnt = 0;
uint8_t KEY_UP_Pressed = 0;
uint8_t KEY_0_Pressed = 0;
uint8_t KEY_1_Pressed = 0;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ������ʱ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Button_Delay_Start(unsigned int time)
{
  // ������ʱʱ��
  Button_Delay_Time_Cnt = time;
}// End of void Button_Delay_Start(unsigned int time)

/*************************************
  * ��������: ������ʱ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
unsigned char Button_Delay_Is_Delay(void)
{
  if(Button_Delay_Time_Cnt>0)return 1;
  return 0;
}// End of unsigned char Button_Delay_Is_Delay(void)

/*************************************
  * ��������: ������Ӧ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Button_Handle(void)
{
  uint8_t i;
  if(Button_Delay_Time_Cnt > 0)
  {
    Button_Delay_Time_Cnt --;
  }
  if(!Sampling)
  {
    if(!KEY_UP_Pressed)  //��û�а������ܱ�����
    {
      if(KEY_UP_ON)  //��KEY_UP������
      {
        KEY_UP_Pressed = 1;  //���豻����
        Button_Delay_Start(2);  //20������ٴμ��
      }
    }
    else /* if(KEY_UP_Pressed) */  //����Ѽ��豻����
    {
      if(!Button_Delay_Is_Delay())  //����ʱʱ�����
      {
        if(!Wait)  
        {
          if(KEY_UP_ON)  //KEY_UP���ɱ�����
          {
            Wait = 1;  //�ȴ�KEY_UP���ɿ�
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_UP_OFF)
          {
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // �����ʾ��
            Wait = 0;  //��λ
            KEY_UP_Pressed = 0;  //��λ
            Sampling = 1;  //�������ģʽ
          }
        }
      }
    }
  }
  else
  {
    /* KEYUP��� */
    if(!KEY_UP_Pressed)  //��û�а������ܱ�����
    {
      if(KEY_UP_ON)  //��KEYUP������
      {
        KEY_UP_Pressed = 1;  //���豻����
        Button_Delay_Start(2);  //20������ٴμ��
      }  
    }
    else /* if(KEY_UP_Pressed) */  //����Ѽ��豻����
    {
      if(!Button_Delay_Is_Delay())  //����ʱʱ�����
      {
        if(!Wait)  
        {
          if(KEY_UP_ON)  //KEYUP���ɱ�����
          {
            Wait = 1;  //�ȴ�KEY_UP���ɿ�
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_UP_OFF)  //��KEY_0�ѱ��ɿ�
          {
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // �����ʾ��
            WriteDataFlash((unsigned char*)&test_struct,sizeof(data_flash_info_struct));  //д���²���ֵ��Flash
            for(i = 0; i < 8; i ++)
            {
              Threshold[i] = (test_struct.Black_Value_in_Flash[i] + test_struct.White_Value_in_Flash[i]) / 2;  //������ֵ
              printf("��%d·��ֵ��%d\n", i+1, Threshold[i]);  //��ӡ��ֵ
            }
            Sampling = 0;  //��������ģʽ  
            Wait = 0;  //��λ
            KEY_UP_Pressed = 0;  //��λ
          }
        }
      }
    }
    
    /* KEY0��� */
    if(!KEY_0_Pressed)  //��û�а������ܱ�����
    {
      if(KEY_0_ON)  //��KEY_0������
      {
        KEY_0_Pressed = 1;  //���豻����
        Button_Delay_Start(2);  //20������ٴμ��
      }
    }
    else /* if(KEY_0_Pressed) */  //����Ѽ��豻����
    {
      if(!Button_Delay_Is_Delay())  //����ʱʱ�����
      {
        if(!Wait)  
        {
          if(KEY_0_ON)  //KEY_0���ɱ�����
          {
            Wait = 1;  //�ȴ�KEY_UP���ɿ�
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_0_OFF)  //��KEY_0�ѱ��ɿ�
          {
            for(i = 0; i < 8; i ++)
            {
              Black_Value[i] = ADC_Converted_Value[i];  //ת�����ֵ
              printf("��%d·��ɫֵ��%d\n", i+1, ADC_Converted_Value[i]);  //�򴮿ڴ�ӡ����ֵ
              test_struct.Black_Value_in_Flash[i] = Black_Value[i];  //������ֵ�����д��Flash�Ľṹ��
            }
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // �����ʾ��
            Wait = 0;  //��λ
            KEY_0_Pressed = 0;  //��λ
          }
        }
      }
    }
    /* KEY1��� */
    if(!KEY_1_Pressed)  //��û�а������ܱ�����
    {
      if(KEY_1_ON)  //��KEY_1������
      {
        KEY_1_Pressed = 1;  //���豻����
        Button_Delay_Start(2);  //20������ٴμ��
      }
    }
    else /* if(KEY_1_Pressed) */  //����Ѽ��豻����
    {
      if(!Button_Delay_Is_Delay())  //����ʱʱ�����
      {
        if(!Wait)  
        {
          if(KEY_1_ON)  //KEY_1���ɱ�����
          {
            Wait = 1;  //�ȴ�KEY_UP���ɿ�
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_1_OFF)  //��KEY_1�ѱ��ɿ�
          {
            for(i = 0; i < 8; i ++)
            {
              White_Value[i] = ADC_Converted_Value[i];  //ת�����ֵ
              printf("��%d·��ɫֵ��%d\n", i+1, ADC_Converted_Value[i]);  //�򴮿ڴ�ӡ����ֵ
              test_struct.White_Value_in_Flash[i] = White_Value[i];  //������ֵ�����д��Flash�Ľṹ��
            }
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // �����ʾ��
            Wait = 0;  //��λ
            KEY_1_Pressed = 0;  //��λ
          }
        }
      }
    }
  }
}

/************************************************ �ļ����� ************************************************/
