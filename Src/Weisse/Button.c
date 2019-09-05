/*****************************************************************************
  * 文件名称: Button.c
  * 作    者: 尤伟宏
  * 编写日期: 四月 2019
  * 功    能: 按键采样
  ****************************************************************************
  * 说    明: 通过按键读取两个关键值
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Button.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 按键被按下 */
#define KEY_UP_ON (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
#define KEY_0_ON (!HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin))
#define KEY_1_ON (!HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin))
/* 按键没被按下 */
#define KEY_UP_OFF (!HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
#define KEY_0_OFF (HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin))
#define KEY_1_OFF (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin))
/* 私有变量 ------------------------------------------------------------------*/
uint8_t Sampling = 0;
uint8_t Button_Pressed = 0;
uint8_t Wait = 0;
uint8_t Button_Delay_Time_Cnt = 0;
uint8_t KEY_UP_Pressed = 0;
uint8_t KEY_0_Pressed = 0;
uint8_t KEY_1_Pressed = 0;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 按键延时
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Button_Delay_Start(unsigned int time)
{
  // 设置延时时间
  Button_Delay_Time_Cnt = time;
}// End of void Button_Delay_Start(unsigned int time)

/*************************************
  * 函数功能: 按键延时
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
unsigned char Button_Delay_Is_Delay(void)
{
  if(Button_Delay_Time_Cnt>0)return 1;
  return 0;
}// End of unsigned char Button_Delay_Is_Delay(void)

/*************************************
  * 函数功能: 按键响应处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
    if(!KEY_UP_Pressed)  //若没有按键可能被按下
    {
      if(KEY_UP_ON)  //若KEY_UP被按下
      {
        KEY_UP_Pressed = 1;  //假设被按下
        Button_Delay_Start(2);  //20毫秒后再次检测
      }
    }
    else /* if(KEY_UP_Pressed) */  //如果已假设被按下
    {
      if(!Button_Delay_Is_Delay())  //若延时时间结束
      {
        if(!Wait)  
        {
          if(KEY_UP_ON)  //KEY_UP依旧被按下
          {
            Wait = 1;  //等待KEY_UP被松开
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_UP_OFF)
          {
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // 完成提示音
            Wait = 0;  //复位
            KEY_UP_Pressed = 0;  //复位
            Sampling = 1;  //进入采样模式
          }
        }
      }
    }
  }
  else
  {
    /* KEYUP检测 */
    if(!KEY_UP_Pressed)  //若没有按键可能被按下
    {
      if(KEY_UP_ON)  //若KEYUP被按下
      {
        KEY_UP_Pressed = 1;  //假设被按下
        Button_Delay_Start(2);  //20毫秒后再次检测
      }  
    }
    else /* if(KEY_UP_Pressed) */  //如果已假设被按下
    {
      if(!Button_Delay_Is_Delay())  //若延时时间结束
      {
        if(!Wait)  
        {
          if(KEY_UP_ON)  //KEYUP依旧被按下
          {
            Wait = 1;  //等待KEY_UP被松开
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_UP_OFF)  //若KEY_0已被松开
          {
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // 完成提示音
            WriteDataFlash((unsigned char*)&test_struct,sizeof(data_flash_info_struct));  //写入新采样值到Flash
            for(i = 0; i < 8; i ++)
            {
              Threshold[i] = (test_struct.Black_Value_in_Flash[i] + test_struct.White_Value_in_Flash[i]) / 2;  //更新阈值
              printf("第%d路阈值：%d\n", i+1, Threshold[i]);  //打印阈值
            }
            Sampling = 0;  //结束采样模式  
            Wait = 0;  //复位
            KEY_UP_Pressed = 0;  //复位
          }
        }
      }
    }
    
    /* KEY0检测 */
    if(!KEY_0_Pressed)  //若没有按键可能被按下
    {
      if(KEY_0_ON)  //若KEY_0被按下
      {
        KEY_0_Pressed = 1;  //假设被按下
        Button_Delay_Start(2);  //20毫秒后再次检测
      }
    }
    else /* if(KEY_0_Pressed) */  //如果已假设被按下
    {
      if(!Button_Delay_Is_Delay())  //若延时时间结束
      {
        if(!Wait)  
        {
          if(KEY_0_ON)  //KEY_0依旧被按下
          {
            Wait = 1;  //等待KEY_UP被松开
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_0_OFF)  //若KEY_0已被松开
          {
            for(i = 0; i < 8; i ++)
            {
              Black_Value[i] = ADC_Converted_Value[i];  //转存采样值
              printf("第%d路黑色值：%d\n", i+1, ADC_Converted_Value[i]);  //向串口打印采样值
              test_struct.Black_Value_in_Flash[i] = Black_Value[i];  //将采样值存入待写入Flash的结构体
            }
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // 完成提示音
            Wait = 0;  //复位
            KEY_0_Pressed = 0;  //复位
          }
        }
      }
    }
    /* KEY1检测 */
    if(!KEY_1_Pressed)  //若没有按键可能被按下
    {
      if(KEY_1_ON)  //若KEY_1被按下
      {
        KEY_1_Pressed = 1;  //假设被按下
        Button_Delay_Start(2);  //20毫秒后再次检测
      }
    }
    else /* if(KEY_1_Pressed) */  //如果已假设被按下
    {
      if(!Button_Delay_Is_Delay())  //若延时时间结束
      {
        if(!Wait)  
        {
          if(KEY_1_ON)  //KEY_1依旧被按下
          {
            Wait = 1;  //等待KEY_UP被松开
          }
        }
        else /*  if(Wait) */
        {
          if(KEY_1_OFF)  //若KEY_1已被松开
          {
            for(i = 0; i < 8; i ++)
            {
              White_Value[i] = ADC_Converted_Value[i];  //转存采样值
              printf("第%d路白色值：%d\n", i+1, ADC_Converted_Value[i]);  //向串口打印采样值
              test_struct.White_Value_in_Flash[i] = White_Value[i];  //将采样值存入待写入Flash的结构体
            }
            Buzzer_Tweet(BUZZER_SHORT_TWEET_TIME);  // 完成提示音
            Wait = 0;  //复位
            KEY_1_Pressed = 0;  //复位
          }
        }
      }
    }
  }
}

/************************************************ 文件结束 ************************************************/
