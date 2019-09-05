/*****************************************************************************
  * �ļ�����: Carrier.c
  * ��    ��: ��ΰ��
  * ��д����: ���� 2019
  * ��    ��: װ���������
  ****************************************************************************
  * ˵    ��:
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Carrier.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
uint8_t Carrier_Task = 0;
uint8_t Position_Flag = 0;  //����λ�ñ�־λ
uint8_t Stepper_Flag = 0;
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ��е��λ�ô���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Carrier_Position_Handle(void)
{ 
  if(Position_Flag == INIT)
  {
    if(HAL_GPIO_ReadPin(Limit_High_GPIO_Port, Limit_High_Pin) == GPIO_PIN_RESET)  //����λ����1�ѱ�����
    {
      Stepper_Stop();  //�������ֹͣ
      Paw_Control(Open);  //��צ�ſ�
      Rotation_Control(Zero);  //��ת���λ
      Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
      Position_Flag = OVER;
    }
    else  /* if(HAL_GPIO_ReadPin(Limit_High_GPIO_Port, Limit_High_Pin) == GPIO_PIN_SET)  //����λ����1δ������ */
    {
      Stepper_Start(Up);  //������������˶�
    }
  }
  else if(Position_Flag == WAIT)
  {
    if(HAL_GPIO_ReadPin(Limit_Low_GPIO_Port, Limit_Low_Pin) == GPIO_PIN_RESET)  //����λ����1�ѱ�����
    {
      Stepper_Stop();  //�������ֹͣ
      Position_Flag = OVER;
    }
    else  /* if(HAL_GPIO_ReadPin(Limit_Low_GPIO_Port, Limit_Low_Pin) == GPIO_PIN_SET)  //����λ����2δ������ */
    {
      Stepper_Start(Down);  //������������˶�
    }
  }
  /* else if(Position_Flag == OVER) */
}

/*************************************
  * ��������: ��е�۴�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Carrier_Handle(void)
{
  static uint8_t i = 0;
  if(Carrier_Task == LOAD)
  {
    HAL_Delay(500);
    Paw_Control(Close);  //צ�ӱպ�
    HAL_Delay(500);
    Arm_Control(OBLIQUE);  //�ֱ���ֱ΢��
    HAL_Delay(500);
    Rotation_Control(Semi);  //��ת����ת����
    Carrier_Task = IDLE;  //��������
    HAL_TIM_Base_Start_IT(&htim7);  //����ʱ������
  }
  else if(Carrier_Task == UNLOAD)
  {
  if(i == FIRST_TARGET)
  {
    Arm_Control(Halb);  //�ֱ���б
    HAL_Delay(500);
    /* Ų������ */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  else if(i == SECOND_TARGET)
  {
//    Rotation_Control(240);  //��ת����ת����
    HAL_Delay(100);
    Arm_Control(Halb);  //�ֱ���б
    HAL_Delay(500);
    /* Ų������ */
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == THIRD_TARGET)
  {
    Arm_Control(Halb);  //�ֱ���б
    HAL_Delay(500);
    /* Ų������ */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == FOURTH_TARGET)
  {
    Arm_Control(Halb);  //�ֱ���б
    HAL_Delay(500);
    /* Ų������ */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
  
  else if(i == FIFTH_TARGET)
  {
    Arm_Control(Halb);  //�ֱ���б
    HAL_Delay(500);
    /* Ų������ */
    Slow_Turn(LEFT);
    HAL_Delay(600);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(LEFT);
    HAL_Delay(800);
    Stop();
    HAL_Delay(50);
    Slow_Turn(RIGHT);
    HAL_Delay(600);
    Stop();
    i ++;
  }
    
    Paw_Control(Open);  //צ���ſ�
    Rotation_Control(Semi);  //��ת����ת����
    HAL_Delay(500);
    while(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
    {
      Straight(BACK);
    }
    Stop();
    HAL_Delay(500);
    Arm_Control(Horizontal);  //�ֱ�ˮƽ
    Straight(BACK);
    HAL_Delay(500);
    Stop();
    Carrier_Task = IDLE;  //��������
    HAL_TIM_Base_Start_IT(&htim7);  //����ʱ������
  }
  /* else if(Carrier_Task == IDLE) */
}

/************************************************ �ļ����� ************************************************/
