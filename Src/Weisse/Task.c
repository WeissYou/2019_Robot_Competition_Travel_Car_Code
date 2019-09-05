/*****************************************************************************
  * 文件名称: Task.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 任务管理
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Task.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t Task_Status = CARRIER_INIT;        // 任务状态
uint16_t Task_Delay_Time_Cnt = 0;                                  // 任务中的无阻塞延时时间计数
uint8_t Temp = 0;
uint8_t Target = 0;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 任务定时器管理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Task_TIM_Content(void)
{
//                                       uint8_t i;
    /*此函数必须率先调用*/             Get_ADC_Value();  //当前任务周期内锁存ADC数据
//                                       for(i = 0; i < 8; i ++)
//                                       {
//                                         printf("第%d路传感器采样值：%d；\n", i, ADC_Converted_Value[i]);
//                                         
//                                       }
//    /*此函数必须在上个函数之后调用*/   Threshold = Automatic_Threshold();  //获取当前阈值
//                                       printf("当前阈值：%d\n", Threshold);
    /*获取当前阈值后调用*/             Track = Track_Recognition();  //获取当前轨迹
//                                       printf("当前轨迹%d\n", Track);
    /*获取当前阈值后调用*/             Sensors = Sensor_On_Track();  //判断灰度传感器当前是否位于路口 
//                                       printf("压线的传感器路数：%d\n",Sensors);
//                                       printf("\n");
    /* 此函数固定此位置调用 */         Task_TIM_Handle();  //任务处理
    /* 以下函数调用无先后要求 */
                                       Alignment_Handle();  //停车校准机制
                                       Break_Handle();  //刹车机制
                                       Button_Handle();  //按键检测机制
                                       Buzzer_Timer_Handle();  //鸣叫时长处理
                                       Carrier_Position_Handle();  //机构位置处理机制
                                       Location_Handle();  //取物定位机制
                                       Overtime_Handle();  //超时处理机制
                                       RGB_Blink();  //RGB模块工作     
}

/*******************************************************************************
*                           陈苏阳@2018-11-24
* Function Name  :  Task_Delay_Start
* Description    :  任务延时开始
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Task_Delay_Start(unsigned int time)
{
    // 设置延时时间
    Task_Delay_Time_Cnt = time;
}// End of void Task_Delay_Start(unsigned int time)

/*******************************************************************************
*                           陈苏阳@2018-11-24
* Function Name  :  Task_Delay_Is_Delay
* Description    :  任务延时是否处于延时中
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
*                           陈苏阳@2018-11-24
* Function Name  :  Task_Timer_Handle
* Description    :  任务定时器处理函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Task_TIM_Handle(void)
{
  /* 如果任务延时时间计数还有值,则任务延时时间计数-1 */
  if(Task_Delay_Is_Delay() > 0)
  {
    Task_Delay_Time_Cnt --;
  }
  
  switch(Task_Status)
  { 
/*********************************机构预初始化*********************************/
    case CARRIER_INIT:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Position_Flag = INIT;
      Task_Status = START_AREA;  //切换状态至“出发区”
      break;
    }
/***********************************出发区*************************************/    
    case START_AREA:
    {
      if(RIGHT_SENSOR_4_WHITE && LEFT_SENSOR_4_WHITE)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = OUT_OF_START_AREA;  //切换状态至“第一条线”
      }
      else
      {
        PD_Track();
      }
      break;
    }
/*******************************抓取共用内容***********************************/
    case BEFORE_CATCH_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop();
        Task_Delay_Start(10);  //延时100ms
        Task_Status = BEFORE_CATCH_PART_TWO;  //切换状态至“取物前第二阶段” 
      }
      break;
    }
    
    case BEFORE_CATCH_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        if(Target != FIFTH_TARGET)
        {
          Arm_Control(Horizontal);  //手臂水平
        }
        Task_Status = BEFORE_CATCH_PART_THREE;  //切换状态至“取物前第三阶段” 
      }
      break;
    }
    
    case BEFORE_CATCH_PART_THREE:
    {
      if(Target != FIFTH_TARGET)  //除了抓取第五个目标物之前都使用修正机制
      {
        Location_Flag = ON;
      }
      if(Target == SECOND_TARGET)  //抓取第二个目标物的情况
      {
        if(LEFT_SENSOR_3_BLACK)  //若左3传感器在黑线上
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop();  //停车
          Task_Status = CATCH;  //切换状态至“取物” 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else if(Target == THIRD_TARGET)  //抓取第三个目标物的情况
      {
        if(RIGHT_SENSOR_3_BLACK)  //若右3传感器在黑线上
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop();  //停车
          Task_Status = CATCH;  //切换状态至“取物” 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else if(Target == FIFTH_TARGET)  //抓取第五个目标物的情况
      {
        if(LEFT_SENSOR_3_BLACK)  //若左3传感器在黑线上
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop();  //停车
          Task_Status = CATCH;  //切换状态至“取物” 
        }
        else
        {
          Slow_Straight(FRONT);
        }
      }
      else  //其余情况
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)  //若左3或右3传感器在黑线上
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop();  //停车
          Task_Status = CATCH;  //切换状态至“取物” 
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
      if(Location_Flag == OFF)  //若定位完成
      {
        Carrier_Task = LOAD;  //切换状态至“上料”
        HAL_TIM_Base_Stop_IT(&htim7);  //暂停任务定时器的中断
        switch(Target)
        {
          case FIRST_TARGET:
          {
            Task_Status = AFTER_CATCH_FIRST_TARGET_PART_ONE;  //切换状态至“抓取第一个物品后的第一阶段” 
            break;
          }
          case SECOND_TARGET:
          {
            Task_Status = AFTER_CATCH_SECOND_TARGET;  //切换状态至“抓取第二个物品后” 
            break;
          }
          case THIRD_TARGET:
          {
            Task_Status = AFTER_CATCH_THIRD_TARGET;  //切换状态至“抓取第三个物品后” 
            break;
          }
          
          case FOURTH_TARGET:
          {
            Task_Status = AFTER_CATCH_FOURTH_TARGET_PART_ONE;  //切换状态至“抓取第四个物品后” 
            break; 
          }
          
          case FIFTH_TARGET:
          {
            Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_ONE;  //切换状态至“抓取第个物五品后” 
            break; 
          }
        }
      }
      break;
    }
/*********************************放置共用内容*********************************/
    case BEFORE_UNLOAD:
    {
      if(Location_Flag == OFF)
      { 
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop();
        Task_Status = UNLOADING;  //切换状态至“下料”
      }
      break;
    }
    
    case UNLOADING:
    {
      if(Position_Flag == OVER)
      {
        Carrier_Task = UNLOAD;  //切换状态至“下料”
        HAL_TIM_Base_Stop_IT(&htim7);  //暂停任务定时器的中断
        Task_Status = AFTER_UNLOAD;  //切换状态至“已下料”
      }
      break;
    }
  
    case AFTER_UNLOAD:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//      Turn(LEFT);////////////////////////待改善/////////////////////////////////
      DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
      DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);  
      Task_Delay_Start(140);  //延时1.6s
      switch(Target)
      {
        case FIRST_TARGET:
        {
          Task_Status = BEFORE_FOURTH_CROSS_PART_ONE;  //切换状态至“第四路口前第一阶段”
          break;
        }
        case SECOND_TARGET:
        {
          Task_Status = BEFORE_TENTH_CROSS_PART_ONE;  //切换状态至“第十路口前第一阶段”
          Arm_Control(Vertical);  //机械臂垂直于地
          break;
        }
        
        case THIRD_TARGET:
        {
          Task_Status = BEFORE_SIXTEENTH_CROSS_PART_ONE;  //切换状态至“第十六路口前第一阶段”
          Arm_Control(Vertical);  //机械臂垂直于地
          break;
        }
        
        case FOURTH_TARGET:
        {
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_ONE;  //切换状态至“第二十五路口前第一阶段”
          Arm_Control(Vertical);  //机械臂垂直于地
          break;
        }
        
        case FIFTH_TARGET:
        {
          Task_Status = ENDING;  //切换状态至“终点”
          Arm_Control(Vertical);  //机械臂垂直于地
          break;
        }
      }
      break;
    }
/*******************************抓取第一个目标物*******************************/
    case OUT_OF_START_AREA:  //驶离出发区
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = FIRST_LINE;  //切换状态至“第一条线”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case FIRST_LINE:
    {
        if(Sensors >= 3)  //若灰度传感器压到横向长黑线
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = FIRST_CROSS;  //切换状态至“第一路口”
        }
        else
        {
          PD_Track();
        }
      break;
    }
    
    case FIRST_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = SECOND_LINE;  //切换状态至“第二条线”
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
      if(Sensors >= 3)  //若灰度传感器压到横向长黑线
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(10);  //延时100ms
        Task_Status = SECOND_CROSS;  //切换状态至“抓取第一个物品”
      }
      else
      {
         PD_Track();
      }
      break;
    }
    
    case SECOND_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      { 
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;  //刹车
          Task_Delay_Start(20);  //延时200ms
          Task_Status = THIRD_LINE;  //切换状态至“第三条线”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Position_Flag == OVER)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Turn(RIGHT);  //右转
          Task_Delay_Start(95);  //延时850ms
          Target = FIRST_TARGET;  //提前放置取物标志位
          Task_Status = BEFORE_CATCH_PART_ONE;  //切换状态至“取物前第一阶段” 
        }
      }
      break;
    }
/******************************放置第一个目标物********************************/
    case AFTER_CATCH_FIRST_TARGET_PART_ONE:
    {
      if(LEFT_SENSOR_1_BLACK || RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Position_Flag = WAIT;  //步进电机移至下极限位置  //步进电机启动
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        Turn(LEFT);  //左转
        DCMotor_Contrl(MOTOR_LEFT, BACKWARD_100PERCENT_PWM);
        DCMotor_Contrl(MOTOR_RIGHT, FORWARD_100PERCENT_PWM);  //90
        Task_Delay_Start(150);  //延时1700ms
        Task_Status = AFTER_CATCH_FIRST_TARGET_PART_TWO;
      }
      break;
    }
    
    case AFTER_CATCH_FIRST_TARGET_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_THIRD_CROSS;  //切换状态至“第三路口” 
        }
        else  //若限位开关1未被碰触
        {
          PD_Track();
        }
      }
      break;
    }
    
    case BEFORE_THIRD_CROSS:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Location_Flag = ON;  //开启定位机制
        Task_Status = THIRD_CROSS_PART_ONE;  //切换状态至“即将通过第三路口”    
      break;
    }
    
    case THIRD_CROSS_PART_ONE:
    {
      if(Location_Flag == OFF)  //若定位完成
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(100);  //延时1000ms
        Task_Status = THIRD_CROSS_PART_TWO;  //切换状态至“已通过第三个路口”
      }
      break;
    }
    
    case THIRD_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = AFTER_THIRD_CROSS;  //切换状态至“已通过第三个路口”
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
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Location_Flag = ON;  //开启停车位置校准机制
          Task_Status = BEFORE_UNLOAD;  //切换状态至“卸货前”
        }
        else
        {
          PD_Track();
        }
      
      break;
    }
/*****************************抓取第二个目标物*********************************/
    case BEFORE_FOURTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = BEFORE_FOURTH_CROSS_PART_TWO;  //切换状态至“第四路口前第二阶段”
      }
      break;
    }
    
    case BEFORE_FOURTH_CROSS_PART_TWO:
    {
        Task_Delay_Start(180);  //延时1.8s
        Arm_Control(Vertical);  //机械臂垂直于地
        Position_Flag = INIT;  //机构初始化
        Task_Status = BEFORE_FOURTH_CROSS_PART_THREE;  //切换状态至“第四路口前第三阶段”
      break;
    }
    
    case BEFORE_FOURTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = FOURTH_CROSS;  //切换状态至“通过第四路口”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
//          Stop();
//          Alignment_Flag = ON;  //开启倒车机制
          Task_Status = AFTER_FOURTH_CROSS;  //切换状态至“通过第四路口”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(87);  //延时1.5s
        Task_Status = FIFTH_CROSS_PART_ONE;  //切换状态至“第五路口第一阶段”
      }
      break;
    }
    
    case FIFTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop();  //停车
        Task_Delay_Start(10);  //延时100ms
        Task_Status = FIFTH_CROSS_PART_TWO;  //切换状态至“第五路口第二阶段”
      }
      break;
    }
    
    case FIFTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = FOURTH_LINE;  //切换状态至“第四条线”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = FIFTH_LINE;  //切换状态至“第五条线”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(10);  //延时100ms
        Task_Status = BEFORE_SIXTH_LINE;  //切换状态至“即将到达第六条线”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_SIXTH_LINE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;  //刹车
          Task_Delay_Start(20);  //延时200ms
//          Alignment_Flag = ON;  //开启倒车机制
          Task_Status = SIXTH_LINE;  //切换状态至“第六条线”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);  //左转
        Task_Delay_Start(95);  //延时850ms
        Target = SECOND_TARGET;
        Task_Status = BEFORE_CATCH_PART_ONE;  //切换状态至“抓取前” 
      }
      break;
    }
/******************************放置第二个目标物********************************/    
    case AFTER_CATCH_SECOND_TARGET:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Position_Flag = WAIT;  //步进电机移至下极限位置
      Turn(LEFT);  //左转
      Task_Delay_Start(95);  //延时850ms
      Task_Status = BEFORE_SIXTH_CROSS;  //切换状态至“第六路口之前” 
      break;
    }
    
    case BEFORE_SIXTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop();  //停车
        Task_Delay_Start(10);  //延时100ms
        Task_Status = AFTER_SIXTH_CROSS;  //切换状态至“第六路口” 
      }
      break;
    }
    
    case AFTER_SIXTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;  //刹车
          Task_Delay_Start(20);  //延时200ms
//          Alignment_Flag = ON;  //开启倒车机制
          Task_Status = BEFORE_SEVENTH_CROSS;  //切换状态至“第七路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);  //左转
        Task_Delay_Start(95);  //延时850ms
        Task_Status = SEVENTH_CROSS;  //切换状态至“第七路口” 
      }
      break;
    }
    
    case SEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop();  //停车
        Task_Delay_Start(10);  //延时100ms
        Task_Status = AFTER_SEVENTH_CROSS;  //切换状态至“通过第七路口”
      }
      break;
    } 
    
    case AFTER_SEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop();
          Location_Flag = ON;  //开启定位机制
          Task_Status = EIGHTH_CROSS;  //切换状态至“第八路口” 
        }
        else  //若限位开关1未被碰触
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
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = AFTER_EIGHTH_CROSS;  //切换状态至“通过第八路口” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Turn(LEFT);
          Task_Delay_Start(20);  //延时200ms
          Task_Status = BEFORE_NINETH_CROSS;  //切换状态至“第九路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = NINETH_CROSS;  //切换状态至“第九路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
//          Alignment_Flag = ON;  //开启倒车机制
          Task_Status = AFTER_NINETH_CROSS;  //切换状态至“第九路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = BEFORE_UNLOAD_SECOND_TARGET_PART_ONE;  //切换状态至“第九路口之前” 
      }
      break;
    }
    
    case BEFORE_UNLOAD_SECOND_TARGET_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_3_BLACK || RIGHT_SENSOR_3_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_UNLOAD_SECOND_TARGET_PART_THREE;  //切换状态至“第九路口之前” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Location_Flag = ON;  //开启停车位置校准机制
        Task_Status = BEFORE_UNLOAD;  //切换状态至“卸货前”
      }
      break;
    }
/******************************抓取第三个目标物********************************/    
    case BEFORE_TENTH_CROSS_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//        Alignment_Flag = ON;  //开启停车位置校准机制
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
        Task_Status = BEFORE_TENTH_CROSS_PART_TWO;  //切换状态至“第十路口前第二阶段”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Position_Flag = INIT;  //机构初始化
        Turn(RIGHT);
        Task_Delay_Start(95);  //延时800ms
        Task_Status = BEFORE_TENTH_CROSS_PART_THREE;  //切换状态至“第九路口之前” 
      }
      break;
    }
    
    case BEFORE_TENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(50);  //延时500ms
          Task_Status = BEFORE_TENTH_CROSS_PART_FOUR;  //切换状态至“第十路口” 
      }
      break;
    }
    
    case BEFORE_TENTH_CROSS_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = TENTH_CROSS;  //切换状态至“第十路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(20);  //延时220ms
        Task_Status = AFTER_TENTH_CROSS_PART_ONE;  //切换状态至“第十路口之后第一阶段”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TENTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(50);  //延时500ms
        Task_Status = AFTER_TENTH_CROSS_PART_TWO;  //切换状态至“第十一路口之前”
      }
      break;
    }
   
    case AFTER_TENTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_ELEVENTH_CROSS;  //切换状态至“第十一路口之前”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
//        Alignment_Flag = ON;  //开启停车位置校准机制
        Task_Status = ELEVENTH_CROSS;  //切换状态至“第十一路口”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = AFTER_ELEVENTH_CROSS;  //切换状态至“第九路口之后”
      }
      break;
    }
    
    case AFTER_ELEVENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = BEFORE_TWELFTH_CROSS;  //切换状态至“第十二路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = TWELFTH_CROSS;  //切换状态至“第十二路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(10);  //延时100ms
        Task_Status = AFTER_TWELFTH_CROSS_PART_ONE;  //切换状态至“第十二路口” 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWELFTH_CROSS_PART_ONE: 
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
//          Alignment_Flag = ON;  //开启停车位置校准机制
          Task_Status = AFTER_TWELFTH_CROSS_PART_TWO;  //切换状态至“抓取第三个目标物之前第二阶段” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(87);  //原值延时85
        Target = THIRD_TARGET;
        Task_Status = BEFORE_CATCH_PART_ONE;  //切换状态至“取物前”
      }
      break;
    }
/******************************放置第三个目标物********************************/
    case AFTER_CATCH_THIRD_TARGET:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Position_Flag = WAIT;  //步进电机移至下极限位置
      Turn(RIGHT);  //右转
      Task_Delay_Start(87);  //延时1800ms
      Task_Status = THIRTEENTH_CROSS;
      break;
    }
    
    case THIRTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(95);  //延时850ms
        Task_Status = BEFORE_FOURTEENTH_CROSS;  //切换状态至“取物前”
      }
      break;
    }
    
    case BEFORE_FOURTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Location_Flag = ON;  //开启停车位置校准机制
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
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(22);  //延时220ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = SIXTEENTH_CROSS;  
      }
      break;
    }
    
    case SIXTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = BEFORE_UNLOAD_THIRD_TARGET_PART_TWO;  
      break;
    }
    
    case BEFORE_UNLOAD_THIRD_TARGET_PART_TWO:
    {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Location_Flag = ON;  //开启停车位置校准机制
        Task_Status = BEFORE_UNLOAD;  //切换状态至“卸货前”
      break;
    }
  
/******************************抓取第四个目标物********************************/    
    case BEFORE_SIXTEENTH_CROSS_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//        Alignment_Flag = ON;  //开启停车位置校准机制
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
        Task_Status = BEFORE_SIXTEENTH_CROSS_PART_TWO;  //切换状态至“第十路口前第二阶段”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Position_Flag = INIT;  //机构初始化
        Turn(LEFT);
        Task_Delay_Start(95);  //延时800ms
        Task_Status = BEFORE_SIXTEENTH_CROSS_PART_THREE;  //切换状态至“第九路口之前” 
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(30);  //延时500ms
          Task_Status = BEFORE_SIXTEENTH_CROSS_PART_FOUR;  //切换状态至“第十路口” 
      }
      break;
    }
    
    case BEFORE_SIXTEENTH_CROSS_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
//        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = SIXTEENTH_CROSS_AGAIN;  //切换状态至“第十路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(22);  //延时220ms
        Task_Status = AFTER_SIXTEENTH_CROSS;  //切换状态至“第十路口之后第一阶段”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_SIXTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(50);  //延时500ms
          Task_Status = BEFORE_SEVENTEENTH_CROSS;  //切换状态至“第十一路口之前”
      }
      break;
    }
   
    case BEFORE_SEVENTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = SEVENTEENTH_CROSS;  //切换状态至“第十一路口之前”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
//        Alignment_Flag = ON;  //开启停车位置校准机制
        Task_Status = AFTER_SEVENTEENTH_CROSS;  //切换状态至“第十一路口”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_SEVENTEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
//      if(Alignment_Flag == OFF)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = BEFORE_EIGHTEENTH_CROSS_PART_ONE;  //切换状态至“第九路口之后”
      }
      break;
    }
    
    case BEFORE_EIGHTEENTH_CROSS_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(30);  //延时200ms
        Task_Status = BEFORE_EIGHTEENTH_CROSS_PART_TWO;  //切换状态至“第九路口之后”
      }
      break;
    }
    
    case BEFORE_EIGHTEENTH_CROSS_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(10);  //延时100ms
          Task_Status = EIGHTEENTH_CROSS;  //切换状态至“第十二路口之前” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = AFTER_EIGHTEENTH_CROSS_PART_ONE;  //切换状态至“第十二路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//        Stop();  //停车
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Position_Flag == OVER)
        {
          Turn(LEFT);
          Task_Delay_Start(87);  //延时200ms
          Task_Status = AFTER_EIGHTEENTH_CROSS_PART_THREE;  // 
        }
      }
      break;
    }
    
    case AFTER_EIGHTEENTH_CROSS_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
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
/******************************放置第四个目标物********************************/    
    case AFTER_CATCH_FOURTH_TARGET_PART_ONE:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Arm_Control(Vertical);  //机械臂内收，避免转向时碰触筒子
      Position_Flag = WAIT;  //步进电机移至下极限位置  //步进电机启动
      Turn(RIGHT);  //右转
      Task_Delay_Start(87);  //延时1800ms
      Task_Status = AFTER_CATCH_FOURTH_TARGET_PART_TWO;
      break;
    }
    
    case AFTER_CATCH_FOURTH_TARGET_PART_TWO:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
//          Alignment_Flag = ON;  //开启定位机制
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);  //右转
        Task_Delay_Start(95);  //延时850ms
        Task_Status = BEFORE_NINETEENTH_CROSS;
      }
      break;
    }
    
    case BEFORE_NINETEENTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Location_Flag = ON;  //开启停车位置校准机制
          Task_Status = BEFORE_TWENTY_FIRST_CROSS;  //切换状态至“第三路口” 
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
      if(Location_Flag == OFF)  //若定位完成
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = TWENTY_FIRST_CROSS;  //切换状态至“卸货前”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);  //左转
        Task_Delay_Start(20);  //延时200ms
        Task_Status = AFTER_TWENTY_FIRST_CROSS;  //切换状态至“已通过第三个路口”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case AFTER_TWENTY_FIRST_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(20);  //延时200ms
        Task_Status = BEFORE_TWENTY_SECOND_CROSS;  //切换状态至“卸货前”
      }
      break;
    }
    
    case BEFORE_TWENTY_SECOND_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        //if(Sensors >= 3)
        if(ADC_Converted_Value[0] <= Threshold[0] || ADC_Converted_Value[1] <= Threshold[1])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(20);  //延时200ms
          Task_Status = TWENTY_SECOND_CROSS;  //切换状态至“卸货前”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = AFTER_TWENTY_SECOND_CROSS;  //切换状态至“卸货前”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = BEFORE_TWENTY_THIRD_CROSS;  //切换状态至“卸货前”
      }
      break;
    }
    
    case BEFORE_TWENTY_THIRD_CROSS:
    {
      if(RIGHT_SENSOR_1_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
//        Alignment_Flag = ON;  //开启定位机制
        Task_Status = TWENTY_THIRD_CROSS;  //切换状态至“卸货前”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);  //右转
        Task_Delay_Start(70);  //延时700ms
        Task_Status = AFTER_TWENTY_THIRD_CROSS;  //切换状态至“已通过第三个路口”
      }
      break;
    }
    
   case AFTER_TWENTY_THIRD_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Delay_Start(50);  //延时500ms
        Task_Status = BEFORE_TWENTY_FOURTH_CROSS;  //切换状态至“卸货前”
      }
      break;
    }
    
    case BEFORE_TWENTY_FOURTH_CROSS:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(20);  //延时200ms
          Task_Status = TWENTY_FOURTH_CROSS;  //切换状态至“卸货前”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//          Alignment_Flag = ON;  //开启定位机制
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
          Task_Status = AFTER_TWENTY_FOURTH_CROSS;  //切换状态至“卸货前”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);  //右转
        Task_Delay_Start(95);  //延时850ms
        Task_Status = BEFORE_UNLOAD_FOURTH_TARGET_PART_ONE;  //切换状态至“已通过第三个路口”
      }
      break;
    }
    
    case BEFORE_UNLOAD_FOURTH_TARGET_PART_ONE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Arm_Control(OBLIQUE);  //机械臂微倾
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
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Location_Flag = ON;  //开启停车位置校准机制
      Task_Status = BEFORE_UNLOAD;  //切换状态至“卸货前”
      break;
    }

/******************************抓取第五个目标物********************************/
    case BEFORE_CATCH_FIFTH_TARGET_PART_ONE:
    {
      if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
//        Alignment_Flag = ON;  //开启停车位置校准机制
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TWO;  //切换状态至“第十路口前第二阶段”
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Position_Flag = INIT;  //机构初始化
        Turn(RIGHT);
        Task_Delay_Start(95);  //延时800ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_THREE;  //切换状态至“第九路口之前” 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(50);  //延时500ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FOUR;  //切换状态至“第十路口” 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FOUR:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FIVE;  //切换状态至“第十路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(75);  //延时750ms  85
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SIX;  //切换状态至“第十路口之后第一阶段”
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_SIX:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Delay_Start(50);  //延时500ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SEVEN;  //切换状态至“第十一路口之前”
      }
      break;
    }
   
    case BEFORE_CATCH_FIFTH_TARGET_PART_SEVEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_EIGHT;  //切换状态至“第十一路口之前”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_NINE;  //切换状态至“第十一路口”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TEN;  //切换状态至“第九路口之后”
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(20);  //延时200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_ELEVEN;  //切换状态至“第十二路口之前” 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_ELEVEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Sensors >= 3)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_TWELVE;  //切换状态至“第十二路口” 
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//        Alignment_Flag = ON;  //开启定位机制
        Stop_Flag = 1;
        Task_Delay_Start(20);  //延时200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_THIRTEEN;  //切换状态至“第十二路口” 
      }
      else
      {
        PD_Track();
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_THIRTEEN: 
    {
//      if(Alignment_Flag == OFF)  //若延时结束
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//        Location_Flag = ON;
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FOURTEEN;  //切换状态至“抓取第三个目标物之前第二阶段” 
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FOURTEEN:
    {
//      if(Location_Flag == OFF)
//      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_FIFTEEN;  //切换状态至“取物前”
//      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_FIFTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(HAL_GPIO_ReadPin(Obstacle_Right_GPIO_Port, Obstacle_Right_Pin) == GPIO_PIN_RESET)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Stop_Flag = 1;
//          Arm_Control(Horizontal);  //机械臂水平
          Task_Delay_Start(20);  //延时200ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SIXTEEN;  //切换状态至“抓取第三个目标物之前第二阶段” 
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(Position_Flag == OVER)
        {
          Turn(RIGHT);
          Task_Delay_Start(87);  //延时900ms
          Task_Status = BEFORE_CATCH_FIFTH_TARGET_PART_SEVENTEEN;  //切换状态至“第十二路口” 
        }
      }
      break;
    }
    
    case BEFORE_CATCH_FIFTH_TARGET_PART_SEVENTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(RIGHT_SENSOR_1_BLACK)
        {
          Target = FIFTH_TARGET;
//          Arm_Control(Horizontal);  //机械臂水平
          Task_Status = BEFORE_CATCH_PART_ONE;  //切换状态至“第十二路口” 
        }
        else
        {
          Slow_Straight(FRONT);
          Arm_Control(Horizontal);  //机械臂水平
        }
      } 
      break;
    }
/******************************放置第五个目标物********************************/
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_ONE:
    {
      Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
      Position_Flag = WAIT;  //步进电机移至下极限位置
      Turn(LEFT);  //左转
      Task_Delay_Start(95);  //延时850ms
      Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_THREE;
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_THREE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(95);  //延时850ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FIVE;  //切换状态至“取物前”
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FIVE:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
          Location_Flag = ON;  //开启停车位置校准机制
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
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(RIGHT);
        Task_Delay_Start(22);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
//        if(Sensors >= 3)
        if(ADC_Converted_Value[6] <= Threshold[6] || ADC_Converted_Value[7] <= Threshold[7])
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(70);  //延时700ms
//        Task_Delay_Start(87);  //延时750ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
//      if(ADC_Converted_Value[3] <= Threshold[3] && ADC_Converted_Value[4] <= Threshold[4])
      {
        Task_Delay_Start(100);  //延时500ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_THIRTEEN;  
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_THIRTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
       if(LEFT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
//          Alignment_Flag = ON;
          Stop_Flag = 1;
          Task_Delay_Start(20);  //延时200ms
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
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Turn(LEFT);
        Task_Delay_Start(87);  //延时200ms
        Task_Status = BEFORE_UNLOAD_FIFTH_TARGET_PART_FIFTEEN;  
      }
      break;
    }
    
    case BEFORE_UNLOAD_FIFTH_TARGET_PART_FIFTEEN:
    {
      if(Task_Delay_Is_Delay() == 0)  //若延时结束
      {
        if(LEFT_SENSOR_2_BLACK || RIGHT_SENSOR_2_BLACK)
        {
          Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
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
        Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  //蜂鸣器发声
        Location_Flag = ON;  //开启停车位置校准机制
        Task_Status = BEFORE_UNLOAD;  //切换状态至“卸货前”
      break;
    }
    
    case ENDING:  //终点
    {
      Stop();  
      break;
    }
  }

}
/************************************************ 文件结束 ************************************************/
