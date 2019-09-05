/*****************************************************************************
  * �ļ�����: Control.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: С������
  ****************************************************************************
  * ˵    ��: ����PDѭ�������ת�ٵ��ڡ�ͣ��λ�õ���
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Control.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
int16_t PWM_Duty[2] = {0};  //���ҵ��PWM�ߵ�ƽʱ��
uint8_t Alignment_Flag = 0;  //ͣ��У׼���Ʊ�־
uint8_t Location_Flag = 0;  //ȡ��ǰ��λ���Ʊ�־
int Last_Error = 0;  //�ϴ�ƫ��
int Error = 0;  //��ǰƫ��
uint8_t Converse_Flag = 0;  //������־
uint8_t Stop_Flag = 0;
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: PDѭ��
  * �������: uint8_t track_recognition
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void PD_Control(void)
{
        uint8_t i;
	int p_value, d_value;  //������������΢��������
 	Last_Error = Error;  //��¼�ϴ����
	Error = Track - SET_DIR;  //��ȡ��ǰ���
        if(Error < 0)
        {
          Error ++;
        }

//	  p_value = (int)(Error ^ 2) / P_Coefficient;  //��������
        p_value = P_DAT_999 * Error;  //��������
        if(p_value < 0)  //ȡ����ֵ
        {
          p_value = -p_value;
        }
	d_value = D_DAT_999 * (Error - Last_Error);  //΢������
        if(d_value < 0)  //ȡ����ֵ
        {
          d_value = -d_value;
        }

        if(!Converse_Flag)  //��������
        {
          /* �ж�����ֵӦ����Ķ��� */
          if(Error > 0)
          {
            PWM_Duty[MOTOR_LEFT] = FORWARD_100PERCENT_PWM - p_value - d_value;
            PWM_Duty[MOTOR_RIGHT] = FORWARD_100PERCENT_PWM;
          }
          else /* if(Error < 0) */
          {
            PWM_Duty[MOTOR_LEFT] = FORWARD_100PERCENT_PWM;
            PWM_Duty[MOTOR_RIGHT] = FORWARD_100PERCENT_PWM - p_value - d_value;
          }
        
          /* ת�����ٶ��޶� */
          for(i = 0; i < 2; i ++)
          {
            if(PWM_Duty[i] >= FORWARD_100PERCENT_PWM)
            {
              PWM_Duty[i] = FORWARD_100PERCENT_PWM;
            }
            if(PWM_Duty[i] < STOP_PWM)
            {
              PWM_Duty[i] = STOP_PWM;
            }
        
	    /* �µ�ռ�ձ��趨 */
	    DCMotor_Contrl(i, PWM_Duty[i]);
          }
        }
        else
        {
          /* �ж�����ֵӦ����Ķ��� */
          if(Error > 0)
          {
            PWM_Duty[MOTOR_LEFT] = BACKWARD_100PERCENT_PWM + p_value + d_value;
            PWM_Duty[MOTOR_RIGHT] = BACKWARD_100PERCENT_PWM;
          }
          else /* if(Error < 0) */
          {
            PWM_Duty[MOTOR_LEFT] = BACKWARD_100PERCENT_PWM;
            PWM_Duty[MOTOR_RIGHT] = BACKWARD_100PERCENT_PWM + p_value + d_value;
          }
        
          /* ת�����ٶ��޶� */
          for(i = 0; i < 2; i ++)
          {
            if(PWM_Duty[i] > STOP_PWM)
            {
              PWM_Duty[i] = STOP_PWM;
            }
            if(PWM_Duty[i] < BACKWARD_100PERCENT_PWM)
            {
              PWM_Duty[i] = BACKWARD_100PERCENT_PWM;
            }
            
	    /* �µ�ռ�ձ��趨 */
	    DCMotor_Contrl(i, PWM_Duty[i]);
          }
        }
}

/*************************************
  * ��������: ֱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Straight(uint8_t dir)
{
  if(dir == FRONT)
  {
    DCMotor_Contrl(MOTOR_LEFT, FORWARD_100PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);
  }
  else
  {
    DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_100PERCENT_PWM);
  }
}

/*************************************
  * ��������: ����ֱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Slow_Straight(uint8_t dir)
{
  if(dir == FRONT)
  {
    DCMotor_Contrl(MOTOR_LEFT, FORWARD_50PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, FORWARD_50PERCENT_PWM);
  }
  else
  {
    DCMotor_Contrl(MOTOR_LEFT, BACKWARD_50PERCENT_PWM);
    DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_50PERCENT_PWM);
  }
}

/*************************************
  * ��������: ת��
  * �������: uint8_t dir, uint8_t delay_ms
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Turn(uint8_t dir)
{
  if(dir == LEFT)  //�趨����
  {
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_90PERCENT_PWM);  //90
  }
  else
  {
      DCMotor_Contrl(MOTOR_LEFT, FORWARD_90PERCENT_PWM);  //90
      DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_100PERCENT_PWM);
  }
}

/*************************************
  * ��������: ����ת��
  * �������: uint8_t dir, uint8_t delay_ms
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Slow_Turn(uint8_t dir)
{
  if(dir == LEFT)  //�趨����
  {
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_50PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_40PERCENT_PWM);
  }
  else
  {
      DCMotor_Contrl(MOTOR_LEFT, FORWARD_40PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, BACKWARD_50PERCENT_PWM);
  }
}

/*************************************
  * ��������: ͣ��λ��У׼������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Alignment_Handle(void)
{
  if(Alignment_Flag)
  {
    if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)  //�����Ѿ�ͣ׼
    {
      Stop();  //ͣ��
      Alignment_Flag = 0;  //����У׼����
    }
    else if(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
    {
      Slow_Straight(FRONT);
    }
    else 
    {
      Slow_Straight(BACK);
    }
  }
}

/*************************************
  * ��������: ��λ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Location_Handle(void)
{
  if(Location_Flag != OFF)
  {
    if(Location_Flag == ON)
    {
      if(LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_1_BLACK)  //����ǰλ�ù��ڿ���
      {
        Slow_Straight(FRONT);  //ǰ��
      }
      else if((LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_3_BLACK) || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_4_BLACK))  //����ǰλ�ù��ڿ�ǰ
      {
        Slow_Straight(BACK);  //����
      }
      /* ����ǰλ��ƫ�� */
      else if(  (LEFT_SENSOR_1_WHITE && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_4_BLACK)  //����4�ں�����
              || (LEFT_SENSOR_1_BLACK && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_4_WHITE)  //����1�ں�����
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_4_BLACK)  //��1����4ͬʱ�ں�����
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_3_BLACK)  //��1����3ͬʱ�ں�����
              || (LEFT_SENSOR_1_BLACK && RIGHT_SENSOR_2_BLACK)  //��1����2ͬʱ�ں�����
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_4_BLACK)  //��2����4ͬʱ�ں�����
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_3_BLACK)  //��2����3ͬʱ�ں�����
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_4_BLACK)  //��3����4ͬʱ�ں�����
              )  
      {
        Slow_Turn(RIGHT);  //�ҵ�
      }
      /* ����ǰλ��ƫ�� */
      else if(  (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_1_WHITE && RIGHT_SENSOR_2_WHITE && RIGHT_SENSOR_3_WHITE && RIGHT_SENSOR_4_WHITE)  //����4�ں�����
              || (LEFT_SENSOR_1_WHITE && LEFT_SENSOR_2_WHITE && LEFT_SENSOR_3_WHITE && LEFT_SENSOR_4_WHITE && RIGHT_SENSOR_1_BLACK) //����1�ں�����
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_1_BLACK)  //��4����1ͬʱ�ں�����
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_2_BLACK)  //��4����2ͬʱ�ں�����
              || (LEFT_SENSOR_4_BLACK && RIGHT_SENSOR_3_BLACK)  //��4����3ͬʱ�ں�����
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_1_BLACK)  //��3����1ͬʱ�ں�����
              || (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_2_BLACK)  //��3����2ͬʱ�ں�����
              || (LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_1_BLACK)  //��2����1ͬʱ�ں�����
              )
      {
        Slow_Turn(LEFT);  //���
      }
      else if((LEFT_SENSOR_2_BLACK && RIGHT_SENSOR_2_BLACK) /*|| (LEFT_SENSOR_3_BLACK && RIGHT_SENSOR_3_BLACK) */)  //������ͣ׼
      {
        Stop();
        Location_Flag = PART_TWO;  //������һ���ֵ���
      }
    }
    if(Location_Flag == PART_TWO)
    {
      if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)  //����3����3������������
      {
        Stop();
        Location_Flag = OFF;  //������λ����
      }
      else //����
      {
        Slow_Straight(FRONT);//ǰ��
      }
    }
  }
}

/*************************************
  * ��������: ��ʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Overtime_Handle(void)
{
  static uint8_t overtime_timer = 0;
  static uint16_t overtime_cnt = 0;
  if(Location_Flag != OFF)
  {
    if(overtime_timer == 0)
    {
      overtime_cnt = OVERTIME;  //��ʱ��ʱ������
      overtime_timer ++;
    }
    else
    {
      if(overtime_cnt > 0)
      {
        overtime_cnt --;
      }
      else
      {
        Location_Flag = OFF;
        Stop();
        overtime_timer = 0;  //��ʱ��ʱ���ر�
      }
    }
  }
}

/*************************************
  * ��������: ɲ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Break_Handle(void)
{
  static uint8_t stop_cnt = 0;
  static uint8_t stop_lock = 0;
  if(Stop_Flag)
  {
    if(!stop_lock)
    {
      stop_cnt = 10;  //����ͣ��ʱ��100ms
      stop_lock = 1;  //ͣ��״̬��
     /* �ر�PWM��� */
      HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      /*Configure GPIO pin : Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN3_Pin */
      GPIO_InitStruct.Pin = Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN4_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(Motor_IN1_GPIO_Port, &GPIO_InitStruct);
  
      /*Configure GPIO pin : Motor_IN2_Pin */
      GPIO_InitStruct.Pin = Motor_IN2_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(Motor_IN2_GPIO_Port, &GPIO_InitStruct);
  
      HAL_GPIO_WritePin(Motor_IN1_GPIO_Port, Motor_IN1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN2_GPIO_Port, Motor_IN2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN3_GPIO_Port, Motor_IN3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Motor_IN4_GPIO_Port, Motor_IN4_Pin, GPIO_PIN_RESET);
    }
    else
    {
      if(stop_cnt)
      {
        stop_cnt --;
      }
      else
      {
        HAL_GPIO_DeInit(Motor_IN1_GPIO_Port, Motor_IN1_Pin|Motor_IN3_Pin|Motor_IN4_Pin);
        HAL_GPIO_DeInit(Motor_IN2_GPIO_Port, Motor_IN2_Pin);
        MX_TIM1_Init();
        Stop_Flag = 0;
        stop_lock = 0;
      }
    }
  }
}

/*************************************
  * ��������: ͣ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Stop(void)
{
  DCMotor_Contrl(MOTOR_LEFT, STOP_PWM);
  DCMotor_Contrl(MOTOR_RIGHT, STOP_PWM);
}
/************************************************ �ļ����� ************************************************/
