/*****************************************************************************
  * �ļ�����: Task.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: �������
  ****************************************************************************
  * ˵    ��:
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Task.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
uint8_t Task_Status = CARRIER_INIT;        // ����״̬
uint16_t Task_Delay_Time_Cnt = 0;                                  // �����е���������ʱʱ�����
uint8_t Temp = 0;
uint8_t Target = 0;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * ��������: ����ʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Task_TIM_Content(void)
{
//                                       uint8_t i;
    /*�˺����������ȵ���*/             Get_ADC_Value();  //��ǰ��������������ADC����
//                                       for(i = 0; i < 8; i ++)
//                                       {
//                                         printf("��%d·����������ֵ��%d��\n", i, ADC_Converted_Value[i]);
//                                         
//                                       }
//    /*�˺����������ϸ�����֮�����*/   Threshold = Automatic_Threshold();  //��ȡ��ǰ��ֵ
//                                       printf("��ǰ��ֵ��%d\n", Threshold);
    /*��ȡ��ǰ��ֵ�����*/             Track = Track_Recognition();  //��ȡ��ǰ�켣
//                                       printf("��ǰ�켣%d\n", Track);
    /*��ȡ��ǰ��ֵ�����*/             Sensors = Sensor_On_Track();  //�жϻҶȴ�������ǰ�Ƿ�λ��·�� 
//                                       printf("ѹ�ߵĴ�����·����%d\n",Sensors);
//                                       printf("\n");
    /* �˺����̶���λ�õ��� */         Task_TIM_Handle();  //������
    /* ���º����������Ⱥ�Ҫ�� */
                                       Alignment_Handle();  //ͣ��У׼����
                                       Break_Handle();  //ɲ������
                                       Button_Handle();  //����������
                                       Buzzer_Timer_Handle();  //����ʱ������
                                       Carrier_Position_Handle();  //����λ�ô������
                                       Location_Handle();  //ȡ�ﶨλ����
                                       Overtime_Handle();  //��ʱ�������
                                       RGB_Blink();  //RGBģ�鹤��     
}

/*******************************************************************************
*                           ������@2018-11-24
* Function Name  :  Task_Delay_Start
* Description    :  ������ʱ��ʼ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Task_Delay_Start(unsigned int time)
{
    // ������ʱʱ��
    Task_Delay_Time_Cnt = time;
}// End of void Task_Delay_Start(unsigned int time)

/*******************************************************************************
*                           ������@2018-11-24
* Function Name  :  Task_Delay_Is_Delay
* Description    :  ������ʱ�Ƿ�����ʱ��
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
unsigned char Task_Delay_Is_Delay(void)
{
    if(Task_Delay_Time_Cnt>0)return 1;
    return 0;
}// End of unsigned char Task_Delay_Is_Delay(void)

/*******************************************************************************
*                           ������@2018-11-24
* Function Name  :  Task_Timer_Handle
* Description    :  ����ʱ��������
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Task_TIM_Handle(void)
{
  /* ���������ʱʱ���������ֵ,��������ʱʱ�����-1 */
  if(Task_Delay_Is_Delay() > 0)
  {
    Task_Delay_Time_Cnt --;
  }
  
  switch(Task_Status)
  { 
/*********************************����Ԥ��ʼ��*********************************/
    case CARRIER_INIT:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Position_Flag = INIT;
      Task_Status = START_AREA;  //�л�״̬������������
      break;
    }
/***********************************������*************************************/    
    case START_AREA:
    {
      if(RIGHT_SENSOR_4_WHITE && LEFT_SENSOR_4_WHITE)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = OUT_OF_START_AREA;  //�л�״̬������һ���ߡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
/*******************************ץȡ��������***********************************/
    case BEFORE_CATCH_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop();
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = BEFORE_CATCH_PART_TWO;  //�л�״̬����ȡ��ǰ�ڶ��׶Ρ� 
      }
      break;
    }
    
    case BEFORE_CATCH_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        if(Target != FIFTH_TARGET)
        {
          Arm_Control(Horizontal);  //�ֱ�ˮƽ
        }
        Task_Status = BEFORE_CATCH_PART_THREE;  //�л�״̬����ȡ��ǰ�����׶Ρ� 
      }
      break;
    }
    
    case BEFORE_CATCH_PART_THREE:
    {
      if(Target != FIFTH_TARGET)  //����ץȡ�����Ŀ����֮ǰ��ʹ����������
      {
        Location_Flag = ON;
      }
      if(Target == SECOND_TARGET)  //ץȡ�ڶ���Ŀ��������
      {
        if(LEFT_SENSOR_3_BLACK)  //����3�������ں�����
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop();  //ͣ��
          Task_Status = CATCH;  //�л�״̬����ȡ� 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else if(Target == THIRD_TARGET)  //ץȡ������Ŀ��������
      {
        if(RIGHT_SENSOR_3_BLACK)  //����3�������ں�����
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop();  //ͣ��
          Task_Status = CATCH;  //�л�״̬����ȡ� 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else if(Target == FIFTH_TARGET)  //ץȡ�����Ŀ��������
      {
        if(LEFT_SENSOR_3_BLACK)  //����3�������ں�����
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop();  //ͣ��
          Task_Status = CATCH;  //�л�״̬����ȡ� 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else  //�������
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)  //����3����3�������ں�����
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop();  //ͣ��
          Task_Status = CATCH;  //�л�״̬����ȡ� 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      break;
    }
    
    case CATCH:
    {
      if(Location_Flag == OFF)  //����λ���
      {
        Carrier_Task = LOAD;  //�л�״̬�������ϡ�
        HAL_TIM_Base_Stop_IT(&htim7);  //��ͣ����ʱ�����ж�
        switch(Target)
        {
          case FIRST_TARGET:
          {
            Task_Status = AFTER_CATCH_FIRST_TARGET_PART_ONE;  //�л�״̬����ץȡ��һ����Ʒ��ĵ�һ�׶Ρ� 
            break;
          }
          case SECOND_TARGET:
          {
            Task_Status = AFTER_CATCH_SECOND_TARGET;  //�л�״̬����ץȡ�ڶ�����Ʒ�� 
            break;
          }
          case THIRD_TARGET:
          {
            Task_Status = AFTER_CATCH_THIRD_TARGET;  //�л�״̬����ץȡ��������Ʒ�� 
            break;
          }
          
          case FOURTH_TARGET:
          {
            Task_Status = AFTER_CATCH_FOURTH_TARGET_PART_ONE;  //�л�״̬����ץȡ���ĸ���Ʒ�� 
            break; 
          }
          
          case FIFTH_TARGET:
          {
            Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_ONE;  //�л�״̬����ץȡ�ڸ�����Ʒ�� 
            break; 
          }
        }
      }
      break;
    }
/*********************************���ù�������*********************************/
    case BEFORE_UNLOAD:
    {
      if(Location_Flag == OFF)
      { 
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop();
        Task_Status = UNLOADING;  //�л�״̬�������ϡ�
      }
      break;
    }
    
    case UNLOADING:
    {
      if(Position_Flag == OVER)
      {
        Carrier_Task = UNLOAD;  //�л�״̬�������ϡ�
        HAL_TIM_Base_Stop_IT(&htim7);  //��ͣ����ʱ�����ж�
        Task_Status = AFTER_UNLOAD;  //�л�״̬���������ϡ�
      }
      break;
    }
  
    case AFTER_UNLOAD:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//      Turn(LEFT);////////////////////////������/////////////////////////////////
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);  
      Task_Delay_Start(140);  //��ʱ1.6s
      switch(Target)
      {
        case FIRST_TARGET:
        {
          Task_Status = BEFORE_FOURTH_CROSS_PART_ONE;  //�л�״̬��������·��ǰ��һ�׶Ρ�
          break;
        }
        case SECOND_TARGET:
        {
          Task_Status = BEFORE_TENTH_CROSS_PART_ONE;  //�л�״̬������ʮ·��ǰ��һ�׶Ρ�
          Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
          break;
        }
        
        case THIRD_TARGET:
        {
          Task_Status = BEFORE_SIXTEENTH_CROSS_PART_ONE;  //�л�״̬������ʮ��·��ǰ��һ�׶Ρ�
          Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
          break;
        }
        
        case FOURTH_TARGET:
        {
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_ONE;  //�л�״̬�����ڶ�ʮ��·��ǰ��һ�׶Ρ�
          Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
          break;
        }
        
        case FIFTH_TARGET:
        {
          Task_Status = ENDING;  //�л�״̬�����յ㡱
          Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
          break;
        }
      }
      break;
    }
/*******************************ץȡ��һ��Ŀ����*******************************/
    case OUT_OF_START_AREA:  //ʻ�������
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = FIRST_LINE;  //�л�״̬������һ���ߡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case FIRST_LINE:
    {
        if(Sensors >= 3)  //���Ҷȴ�����ѹ�����򳤺���
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = FIRST_CROSS;  //�л�״̬������һ·�ڡ�
        }
        else
        {
          PD_Track();
        }
      break;
    }
    
    case FIRST_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = SECOND_LINE;  //�л�״̬�����ڶ����ߡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case SECOND_LINE:
    {
      if(Sensors >= 3)  //���Ҷȴ�����ѹ�����򳤺���
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = SECOND_CROSS;  //�л�״̬����ץȡ��һ����Ʒ��
      }
      else
      {
         PD_Track();
      }
      break;
    }
    
    case SECOND_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      { 
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;  //ɲ��
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = THIRD_LINE;  //�л�״̬�����������ߡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case THIRD_LINE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Position_Flag == OVER)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Turn(RIGHT);  //��ת
          Task_Delay_Start(95);  //��ʱ850ms
          Target = FIRST_TARGET;  //��ǰ����ȡ���־λ
          Task_Status = BEFORE_CATCH_PART_ONE;  //�л�״̬����ȡ��ǰ��һ�׶Ρ� 
        }
      }
      break;
    }
/******************************���õ�һ��Ŀ����********************************/
    case AFTER_CATCH_FIRST_TARGET_PART_ONE:
    {
      if(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Position_Flag = WAIT;  //������������¼���λ��  //�����������
        Stop();
        Task_Delay_Start(50);  
        Task_Status = AFTER_CATCH_FIRST_TARGET_PART_EX;
      }
      else
      {
        Straight(BACK);
      }
      break;
    }
    
    case AFTER_CATCH_FIRST_TARGET_PART_EX:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        Turn(LEFT);  //��ת
        DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
        DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);  //90
        Task_Delay_Start(150);  //��ʱ1700ms
        Task_Status = AFTER_CATCH_FIRST_TARGET_PART_TWO;
      }
      break;
    }
    
    case AFTER_CATCH_FIRST_TARGET_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_THIRD_CROSS;  //�л�״̬��������·�ڡ� 
        }
        else  //����λ����1δ������
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_THIRD_CROSS:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Location_Flag = ON;  //������λ����
        Task_Status = THIRD_CROSS_PART_ONE;  //�л�״̬��������ͨ������·�ڡ�    
      break;
    }
    
    case THIRD_CROSS_PART_ONE:
    {
      if(Location_Flag == OFF)  //����λ���
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(100);  //��ʱ1000ms
        Task_Status = THIRD_CROSS_PART_TWO;  //�л�״̬������ͨ��������·�ڡ�
      }
      break;
    }
    
    case THIRD_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = AFTER_THIRD_CROSS;  //�л�״̬������ͨ��������·�ڡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_THIRD_CROSS:
    {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Location_Flag = ON;  //����ͣ��λ��У׼����
          Task_Status = BEFORE_UNLOAD;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      
      break;
    }
/*****************************ץȡ�ڶ���Ŀ����*********************************/
    case BEFORE_FOURTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_FOURTH_CROSS_PART_TWO;  //�л�״̬��������·��ǰ�ڶ��׶Ρ�
      }
      break;
    }
    
    case BEFORE_FOURTH_CROSS_PART_TWO:
    {
        Task_Delay_Start(180);  //��ʱ1.8s
        Arm_Control(Vertical);  //��е�۴�ֱ�ڵ�
        Position_Flag = INIT;  //������ʼ��
        Task_Status = BEFORE_FOURTH_CROSS_PART_THREE;  //�л�״̬��������·��ǰ�����׶Ρ�
      break;
    }
    
    case BEFORE_FOURTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = FOURTH_CROSS;  //�л�״̬����ͨ������·�ڡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case FOURTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
//          Stop();
//          Alignment_Flag = ON;  //������������
          Task_Status = AFTER_FOURTH_CROSS;  //�л�״̬����ͨ������·�ڡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_FOURTH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(87);  //��ʱ1.5s
        Task_Status = FIFTH_CROSS_PART_ONE;  //�л�״̬��������·�ڵ�һ�׶Ρ�
      }
      break;
    }
    
    case FIFTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop();  //ͣ��
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = FIFTH_CROSS_PART_TWO;  //�л�״̬��������·�ڵڶ��׶Ρ�
      }
      break;
    }
    
    case FIFTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = FOURTH_LINE;  //�л�״̬�����������ߡ�
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case FOURTH_LINE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = FIFTH_LINE;  //�л�״̬�����������ߡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case FIFTH_LINE:
    {
//      if(Sensors >= 3)
      if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = BEFORE_SIXTH_LINE;  //�л�״̬������������������ߡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_SIXTH_LINE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;  //ɲ��
          Task_Delay_Start(20);  //��ʱ200ms
//          Alignment_Flag = ON;  //������������
          Task_Status = SIXTH_LINE;  //�л�״̬�����������ߡ�
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case SIXTH_LINE:
    {
//      if(Alignment_Flag == OFF)      
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);  //��ת
        Task_Delay_Start(95);  //��ʱ850ms
        Target = SECOND_TARGET;
        Task_Status = BEFORE_CATCH_PART_ONE;  //�л�״̬����ץȡǰ�� 
      }
      break;
    }
/******************************���õڶ���Ŀ����********************************/    
    case AFTER_CATCH_SECOND_TARGET:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Position_Flag = WAIT;  //������������¼���λ��
      Turn(LEFT);  //��ת
      Task_Delay_Start(95);  //��ʱ850ms
      Task_Status = BEFORE_SIXTH_CROSS;  //�л�״̬��������·��֮ǰ�� 
      break;
    }
    
    case BEFORE_SIXTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop();  //ͣ��
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = AFTER_SIXTH_CROSS;  //�л�״̬��������·�ڡ� 
      }
      break;
    }
    
    case AFTER_SIXTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;  //ɲ��
          Task_Delay_Start(20);  //��ʱ200ms
//          Alignment_Flag = ON;  //������������
          Task_Status = BEFORE_SEVENTH_CROSS;  //�л�״̬��������·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_SEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);  //��ת
        Task_Delay_Start(95);  //��ʱ850ms
        Task_Status = SEVENTH_CROSS;  //�л�״̬��������·�ڡ� 
      }
      break;
    }
    
    case SEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop();  //ͣ��
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = AFTER_SEVENTH_CROSS;  //�л�״̬����ͨ������·�ڡ�
      }
      break;
    } 
    
    case AFTER_SEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop();
          Location_Flag = ON;  //������λ����
          Task_Status = EIGHTH_CROSS;  //�л�״̬�����ڰ�·�ڡ� 
        }
        else  //����λ����1δ������
        {
          PD_Track();
        }
      }
      
      break;
    }
    
    case EIGHTH_CROSS:
    {
      if(Location_Flag == OFF)
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = AFTER_EIGHTH_CROSS;  //�л�״̬����ͨ���ڰ�·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case AFTER_EIGHTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Turn(LEFT);
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = BEFORE_NINETH_CROSS;  //�л�״̬�����ھ�·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_NINETH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = NINETH_CROSS;  //�л�״̬�����ھ�·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case NINETH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
//          Alignment_Flag = ON;  //������������
          Task_Status = AFTER_NINETH_CROSS;  //�л�״̬�����ھ�·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_NINETH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = BEFORE_UNLOAD_SECOND_TARGET_PART_ONE;  //�л�״̬�����ھ�·��֮ǰ�� 
      }
      break;
    }
    
    case BEFORE_UNLOAD_SECOND_TARGET_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_UNLOAD_SECOND_TARGET_PART_THREE;  //�л�״̬�����ھ�·��֮ǰ�� 
        }
        else
        {
          PD_Track();//Slow_Straight(FRONT);
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_SECOND_TARGET_PART_THREE:
    {
      if(Position_Flag == OVER)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Location_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = BEFORE_UNLOAD;  //�л�״̬����ж��ǰ��
      }
      break;
    }
/******************************ץȡ������Ŀ����********************************/    
    case BEFORE_TENTH_CROSS_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//        Alignment_Flag = ON;  //����ͣ��λ��У׼����
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = BEFORE_TENTH_CROSS_PART_TWO;  //�л�״̬������ʮ·��ǰ�ڶ��׶Ρ�
      }
      else
      {
        Slow_Straight(FRONT);
      }
      break;
    }
    
    case BEFORE_TENTH_CROSS_PART_TWO:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Position_Flag = INIT;  //������ʼ��
        Turn(RIGHT);
        Task_Delay_Start(95);  //��ʱ800ms
        Task_Status = BEFORE_TENTH_CROSS_PART_THREE;  //�л�״̬�����ھ�·��֮ǰ�� 
      }
      break;
    }
    
    case BEFORE_TENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(50);  //��ʱ500ms
          Task_Status = BEFORE_TENTH_CROSS_PART_FOUR;  //�л�״̬������ʮ·�ڡ� 
      }
      break;
    }
    
    case BEFORE_TENTH_CROSS_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = TENTH_CROSS;  //�л�״̬������ʮ·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case TENTH_CROSS:
    {
      if(LEFT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(20);  //��ʱ220ms
        Task_Status = AFTER_TENTH_CROSS_PART_ONE;  //�л�״̬������ʮ·��֮���һ�׶Ρ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TENTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(50);  //��ʱ500ms
        Task_Status = AFTER_TENTH_CROSS_PART_TWO;  //�л�״̬������ʮһ·��֮ǰ��
      }
      break;
    }
   
    case AFTER_TENTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_ELEVENTH_CROSS;  //�л�״̬������ʮһ·��֮ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;  
    }
    
    case BEFORE_ELEVENTH_CROSS:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
//        Alignment_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = ELEVENTH_CROSS;  //�л�״̬������ʮһ·�ڡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case ELEVENTH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = AFTER_ELEVENTH_CROSS;  //�л�״̬�����ھ�·��֮��
      }
      break;
    }
    
    case AFTER_ELEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = BEFORE_TWELFTH_CROSS;  //�л�״̬������ʮ��·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_TWELFTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = TWELFTH_CROSS;  //�л�״̬������ʮ��·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case TWELFTH_CROSS:
    {
      if(Sensors >= 3)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(10);  //��ʱ100ms
        Task_Status = AFTER_TWELFTH_CROSS_PART_ONE;  //�л�״̬������ʮ��·�ڡ� 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWELFTH_CROSS_PART_ONE: 
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
//          Alignment_Flag = ON;  //����ͣ��λ��У׼����
          Task_Status = AFTER_TWELFTH_CROSS_PART_TWO;  //�л�״̬����ץȡ������Ŀ����֮ǰ�ڶ��׶Ρ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWELFTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(87);  //ԭֵ��ʱ85
        Target = THIRD_TARGET;
        Task_Status = BEFORE_CATCH_PART_ONE;  //�л�״̬����ȡ��ǰ��
      }
      break;
    }
/******************************���õ�����Ŀ����********************************/
    case AFTER_CATCH_THIRD_TARGET:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Position_Flag = WAIT;  //������������¼���λ��
      Turn(RIGHT);  //��ת
      Task_Delay_Start(87);  //��ʱ1800ms
      Task_Status = THIRTEENTH_CROSS;
      break;
    }
    
    case THIRTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = AFTER_THIRTEENTH_CROSS;
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case AFTER_THIRTEENTH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(95);  //��ʱ850ms
        Task_Status = BEFORE_FOURTEENTH_CROSS;  //�л�״̬����ȡ��ǰ��
      }
      break;
    }
    
    case BEFORE_FOURTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Location_Flag = ON;  //����ͣ��λ��У׼����
          Task_Status = FOURTEENTH_CROSS;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case FOURTEENTH_CROSS:
    {
      if(Location_Flag == OFF)  
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = AFTER_FOURTEENTH_CROSS;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case AFTER_FOURTEENTH_CROSS:
    {
      if(LEFT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(22);  //��ʱ220ms
        Task_Status = FIFTEENTH_CROSS;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case FIFTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = AFTER_FIFTEENTH_CROSS;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case AFTER_FIFTEENTH_CROSS:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
//        Alignment_Flag = ON;
        Task_Status = BEFORE_SIXTEENTH_CROSS;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = SIXTEENTH_CROSS;  
      }
      break;
    }
    
    case SIXTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_UNLOAD_THIRD_TARGET_PART_ONE;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_THIRD_TARGET_PART_ONE:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_UNLOAD_THIRD_TARGET_PART_TWO;  
      break;
    }
    
    case BEFORE_UNLOAD_THIRD_TARGET_PART_TWO:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Location_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = BEFORE_UNLOAD;  //�л�״̬����ж��ǰ��
      break;
    }
  
/******************************ץȡ���ĸ�Ŀ����********************************/    
    case BEFORE_SIXTEENTH_CROSS_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//        Alignment_Flag = ON;  //����ͣ��λ��У׼����
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = BEFORE_SIXTEENTH_CROSS_PART_TWO;  //�л�״̬������ʮ·��ǰ�ڶ��׶Ρ�
      }
      else
      {
        Slow_Straight(FRONT);
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS_PART_TWO:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Position_Flag = INIT;  //������ʼ��
        Turn(LEFT);
        Task_Delay_Start(95);  //��ʱ800ms
        Task_Status = BEFORE_SIXTEENTH_CROSS_PART_THREE;  //�л�״̬�����ھ�·��֮ǰ�� 
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(30);  //��ʱ500ms
          Task_Status = BEFORE_SIXTEENTH_CROSS_PART_FOUR;  //�л�״̬������ʮ·�ڡ� 
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
//        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = SIXTEENTH_CROSS_AGAIN;  //�л�״̬������ʮ·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case SIXTEENTH_CROSS_AGAIN:
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(22);  //��ʱ220ms
        Task_Status = AFTER_SIXTEENTH_CROSS;  //�л�״̬������ʮ·��֮���һ�׶Ρ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_SIXTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(50);  //��ʱ500ms
          Task_Status = BEFORE_SEVENTEENTH_CROSS;  //�л�״̬������ʮһ·��֮ǰ��
      }
      break;
    }
   
    case BEFORE_SEVENTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = SEVENTEENTH_CROSS;  //�л�״̬������ʮһ·��֮ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;  
    }
    
    case SEVENTEENTH_CROSS:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
//        Alignment_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = AFTER_SEVENTEENTH_CROSS;  //�л�״̬������ʮһ·�ڡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_SEVENTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
//      if(Alignment_Flag == OFF)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = BEFORE_EIGHTEENTH_CROSS_PART_ONE;  //�л�״̬�����ھ�·��֮��
      }
      break;
    }
    
    case BEFORE_EIGHTEENTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(30);  //��ʱ200ms
        Task_Status = BEFORE_EIGHTEENTH_CROSS_PART_TWO;  //�л�״̬�����ھ�·��֮��
      }
      break;
    }
    
    case BEFORE_EIGHTEENTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(10);  //��ʱ100ms
          Task_Status = EIGHTEENTH_CROSS;  //�л�״̬������ʮ��·��֮ǰ�� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case EIGHTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = AFTER_EIGHTEENTH_CROSS_PART_ONE;  //�л�״̬������ʮ��·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_EIGHTEENTH_CROSS_PART_ONE:
    {
      if(HAL_GPIO_ReadPin(Obstacle_Left_GPIO_Port, Obstacle_Left_Pin) == GPIO_PIN_RESET)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//        Stop();  //ͣ��
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = AFTER_EIGHTEENTH_CROSS_PART_TWO;
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_EIGHTEENTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Position_Flag == OVER)
        {
          Turn(LEFT);
          Task_Delay_Start(87);  //��ʱ200ms
          Task_Status = AFTER_EIGHTEENTH_CROSS_PART_THREE;  // 
        }
      }
      break;
    }
    
    case AFTER_EIGHTEENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_1_BLACK)
        {
          Target = FOURTH_TARGET;
          Task_Status = BEFORE_CATCH_PART_ONE;  // 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      break;
    }
/******************************���õ��ĸ�Ŀ����********************************/    
    case AFTER_CATCH_FOURTH_TARGET_PART_ONE:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Arm_Control(Vertical);  //��е�����գ�����ת��ʱ����Ͳ��
      Position_Flag = WAIT;  //������������¼���λ��  //�����������
      Turn(RIGHT);  //��ת
      Task_Delay_Start(87);  //��ʱ1800ms
      Task_Status = AFTER_CATCH_FOURTH_TARGET_PART_TWO;
      break;
    }
    
    case AFTER_CATCH_FOURTH_TARGET_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
//          Alignment_Flag = ON;  //������λ����
          Task_Status = AFTER_CATCH_FOURTH_TARGET_PART_THREE;
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case AFTER_CATCH_FOURTH_TARGET_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);  //��ת
        Task_Delay_Start(95);  //��ʱ850ms
        Task_Status = BEFORE_NINETEENTH_CROSS;
      }
      break;
    }
    
    case BEFORE_NINETEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Location_Flag = ON;  //����ͣ��λ��У׼����
          Task_Status = BEFORE_TWENTY_FIRST_CROSS;  //�л�״̬��������·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_TWENTY_FIRST_CROSS:
    {
      if(Location_Flag == OFF)  //����λ���
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = TWENTY_FIRST_CROSS;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case TWENTY_FIRST_CROSS:
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);  //��ת
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = AFTER_TWENTY_FIRST_CROSS;  //�л�״̬������ͨ��������·�ڡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWENTY_FIRST_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = BEFORE_TWENTY_SECOND_CROSS;  //�л�״̬����ж��ǰ��
      }
      break;
    }
    
    case BEFORE_TWENTY_SECOND_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        //if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = TWENTY_SECOND_CROSS;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case TWENTY_SECOND_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = AFTER_TWENTY_SECOND_CROSS;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWENTY_SECOND_CROSS:
    {
//      if(Sensors >= 3)
      if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_TWENTY_THIRD_CROSS;  //�л�״̬����ж��ǰ��
      }
      break;
    }
    
    case BEFORE_TWENTY_THIRD_CROSS:
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
//        Alignment_Flag = ON;  //������λ����
        Task_Status = TWENTY_THIRD_CROSS;  //�л�״̬����ж��ǰ��
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case TWENTY_THIRD_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);  //��ת
        Task_Delay_Start(70);  //��ʱ700ms
        Task_Status = AFTER_TWENTY_THIRD_CROSS;  //�л�״̬������ͨ��������·�ڡ�
      }
      break;
    }
    
   case AFTER_TWENTY_THIRD_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Delay_Start(50);  //��ʱ500ms
        Task_Status = BEFORE_TWENTY_FOURTH_CROSS;  //�л�״̬����ж��ǰ��
      }
      break;
    }
    
    case BEFORE_TWENTY_FOURTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = TWENTY_FOURTH_CROSS;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case TWENTY_FOURTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//          Alignment_Flag = ON;  //������λ����
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = AFTER_TWENTY_FOURTH_CROSS;  //�л�״̬����ж��ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWENTY_FOURTH_CROSS:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);  //��ת
        Task_Delay_Start(95);  //��ʱ850ms
        Task_Status = BEFORE_UNLOAD_FOURTH_TARGET_PART_ONE;  //�л�״̬������ͨ��������·�ڡ�
      }
      break;
    }
    
    case BEFORE_UNLOAD_FOURTH_TARGET_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Arm_Control(OBLIQUE);  //��е��΢��
          Task_Status = BEFORE_UNLOAD_FOURTH_TARGET_PART_TWO;  
        }
        else
        {
          PD_Track();
//          Straight(FRONT);
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FOURTH_TARGET_PART_TWO:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Location_Flag = ON;  //����ͣ��λ��У׼����
      Task_Status = BEFORE_UNLOAD;  //�л�״̬����ж��ǰ��
      break;
    }

/******************************ץȡ�����Ŀ����********************************/
    case BEFORE_CATCH_FIFTH_TARGET_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
//        Alignment_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TWO;  //�л�״̬������ʮ·��ǰ�ڶ��׶Ρ�
      }
      else
      {
        Slow_Straight(FRONT);
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_TWO:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Position_Flag = INIT;  //������ʼ��
        Turn(RIGHT);
        Task_Delay_Start(95);  //��ʱ800ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_THREE;  //�л�״̬�����ھ�·��֮ǰ�� 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(50);  //��ʱ500ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FOUR;  //�л�״̬������ʮ·�ڡ� 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FIVE;  //�л�״̬������ʮ·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FIVE:
    {
      if(LEFT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(75);  //��ʱ750ms  85
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SIX;  //�л�״̬������ʮ·��֮���һ�׶Ρ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_SIX:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Delay_Start(50);  //��ʱ500ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SEVEN;  //�л�״̬������ʮһ·��֮ǰ��
      }
      break;
    }
   
    case BEFORE_CATCH_FIFTH_TARGET_PART_SEVEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_EIGHT;  //�л�״̬������ʮһ·��֮ǰ��
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;  
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_EIGHT:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_NINE;  //�л�״̬������ʮһ·�ڡ�
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_NINE:
    {
//      if(Sensors >= 3)
      if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TEN;  //�л�״̬�����ھ�·��֮��
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_TEN:
    {

      if(LEFT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_ELEVEN;  //�л�״̬������ʮ��·��֮ǰ�� 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_ELEVEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TWELVE;  //�л�״̬������ʮ��·�ڡ� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_TWELVE:
    {
      if(/*LEFT_SENSOR_2_BLACK ||*/ RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//        Alignment_Flag = ON;  //������λ����
        Stop_Flag = 1;
        Task_Delay_Start(20);  //��ʱ200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_THIRTEEN;  //�л�״̬������ʮ��·�ڡ� 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_THIRTEEN: 
    {
//      if(Alignment_Flag == OFF)  //����ʱ����
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//        Location_Flag = ON;
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FOURTEEN;  //�л�״̬����ץȡ������Ŀ����֮ǰ�ڶ��׶Ρ� 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FOURTEEN:
    {
//      if(Location_Flag == OFF)
//      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FIFTEEN;  //�л�״̬����ȡ��ǰ��
//      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FIFTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(HAL_GPIO_ReadPin(Obstacle_Right_GPIO_Port, Obstacle_Right_Pin) == GPIO_PIN_RESET)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Stop_Flag = 1;
//          Arm_Control(Horizontal);  //��е��ˮƽ
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SIXTEEN;  //�л�״̬����ץȡ������Ŀ����֮ǰ�ڶ��׶Ρ� 
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_SIXTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(Position_Flag == OVER)
        {
          Turn(RIGHT);
          Task_Delay_Start(87);  //��ʱ900ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SEVENTEEN;  //�л�״̬������ʮ��·�ڡ� 
        }
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_SEVENTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Target = FIFTH_TARGET;
//          Arm_Control(Horizontal);  //��е��ˮƽ
          Task_Status = BEFORE_CATCH_PART_ONE;  //�л�״̬������ʮ��·�ڡ� 
        }
        else
        {
          Slow_Straight(FRONT);
          Arm_Control(Horizontal);  //��е��ˮƽ
        }
      } 
      break;
    }
/******************************���õ����Ŀ����********************************/
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_ONE:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
      Position_Flag = WAIT;  //������������¼���λ��
      Turn(LEFT);  //��ת
      Task_Delay_Start(95);  //��ʱ850ms
      Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_THREE;
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FOUR;
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FOUR:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(95);  //��ʱ850ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FIVE;  //�л�״̬����ȡ��ǰ��
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FIVE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Location_Flag = ON;  //����ͣ��λ��У׼����
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_SIX;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_SIX:
    {
      if(Location_Flag == OFF)  
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_SEVEN;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_SEVEN:
    {
      if(LEFT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(RIGHT);
        Task_Delay_Start(22);  //��ʱ200ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_EIGHT;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_EIGHT:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_NINE;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_NINE:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_TEN;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_TEN:
    {
//      if(Sensors >= 3)
      if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_ELEVEN;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_ELEVEN:
    {
      if(LEFT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(70);  //��ʱ700ms
//        Task_Delay_Start(87);  //��ʱ750ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_TWELVE;  
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_TWELVE:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
        Task_Delay_Start(100);  //��ʱ500ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_THIRTEEN;  
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_THIRTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
       if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //��ʱ200ms
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FOURTEEN;  
        }
        else
        {
          PD_Track();
        }
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FOURTEEN:
    {
//      if(Alignment_Flag == OFF)
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Turn(LEFT);
        Task_Delay_Start(87);  //��ʱ200ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FIFTEEN;  
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FIFTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //����ʱ����
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
          Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_SIXTEEN;  
        }
        else
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_SIXTEEN:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //����������
        Location_Flag = ON;  //����ͣ��λ��У׼����
        Task_Status = BEFORE_UNLOAD;  //�л�״̬����ж��ǰ��
      break;
    }
    
    case ENDING:  //�յ�
    {
      Stop();  
      break;
    }
  }

}
/************************************************ �ļ����� ************************************************/
