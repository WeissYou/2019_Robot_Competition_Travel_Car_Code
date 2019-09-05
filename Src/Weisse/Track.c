/*****************************************************************************
  * 文件名称: Track.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 循迹相关
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Track.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
uint8_t Track = 0;  //轨道位置
uint8_t Last_Track = 0;  //上次轨道位置
uint8_t Sensors = 0;
uint8_t Black_Gotten = 0;
uint8_t White_Gotten = 0;
uint8_t Threshold_Gotten = 0;
uint8_t Black_Value[8] = {144, 137, 168, 106, 125, 122, 139, 110};
uint8_t White_Value[8] = {246, 234, 254, 209, 223, 223, 246, 223};
uint8_t Threshold[8] = {195, 185, 211, 157, 174, 172, 192, 166};  //阈值
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 高速寻迹
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 不在任务定时器中调用无效
  ************************************/
void PD_Track(void)
{ 
//  if(Automatic_Threshold() < VALUE_OF_WHITE)  //若整车未偏离轨道
//  {
    PD_Control();  //PD寻迹
//  }
//  else
//  { 
//    PD_Control();
//    //Stop();
//  }
}

/*************************************
  * 函数功能: 获取压线的传感器数量
  * 输入参数: 无
  * 返 回 值: uint8_t number
  * 说    明: 无
  ************************************/
uint8_t Sensor_On_Track(void)
{
  uint8_t i;
  uint8_t number = 0;
  for(i = 0; i < 8; i ++)
  {
    if(ADC_Converted_Value[i] <= Threshold[i])  //若采样值小于阈值
    {
      number ++;  //采样值二值化结果为1
    }
  }
  return number;
}

/*************************************
  * 函数功能: 自动阈值算法
  * 输入参数: 无
  * 返 回 值: uint8_t threshold
  * 说    明: 无
  ************************************/
//uint8_t Automatic_Threshold(void)
//{
//   static uint8_t i;
////   static uint8_t threshold;  //中间变量0、中间变量1、阈值
//   static uint8_t value_max, value_min;  //最大、最小采样值
//	
//   value_max = ADC_Converted_Value[0];  //动态阈值算法，读取最大和最小值
//   for(i = 0; i < 8; i ++)   
//   {
//     if(value_max <= ADC_Converted_Value[i])
//     value_max = ADC_Converted_Value[i];
//   }
//   value_min = ADC_Converted_Value[0];  //最小值
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
  * 函数功能: 获取黑色采样值
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Get_Black_Value(void)
{
  uint8_t i;
  if(!Black_Gotten)
  {
    if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);  //延时20ms
      if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
      {
        while(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET);
        Get_ADC_Value();  //当前任务周期内锁存ADC数据
        for(i = 0; i < 8; i ++)
        {
          Black_Value[i] = ADC_Converted_Value[i];
          printf("第%d路黑色值：%d\n", i+1, ADC_Converted_Value[i]);
        }
        /* 完成提示音 */
        BEEP_StateSet(BEEPState_ON);
        HAL_Delay(20);
        BEEP_StateSet(BEEPState_OFF);
        Black_Gotten = 1;
      }
    }
  }
}

/*************************************
  * 函数功能: 获取白色采样值
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Get_White_Value(void)
{
  uint8_t i;
  if(!White_Gotten)
  {
    if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);  //延时20ms
      if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)
      {
        while(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET);
        Get_ADC_Value();  //当前任务周期内锁存ADC数据
        for(i = 0; i < 8; i ++)
        {
          White_Value[i] = ADC_Converted_Value[i];
          printf("第%d路白色值：%d\n", i+1, ADC_Converted_Value[i]);
        }
        /* 完成提示音 */
        BEEP_StateSet(BEEPState_ON);
        HAL_Delay(20);
        BEEP_StateSet(BEEPState_OFF);
        White_Gotten = 1;
      }
    }
  }
}

/*************************************
  * 函数功能: 计算阈值
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
        HAL_Delay(20);  //延时20ms
        if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)
        {
          while(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET);
          for(i = 0; i < 8; i ++)
          {
            Threshold[i] = (Black_Value[i] + White_Value[i]) / 2;
            printf("第%d路阈值：%d\n", i+1, Threshold[i]);
          }
          /* 完成提示音 */
          BEEP_StateSet(BEEPState_ON);
          HAL_Delay(20);
          BEEP_StateSet(BEEPState_OFF);
          Threshold_Gotten = 1;
          HAL_TIM_Base_Start_IT(&htim7);  //任务定时器启动
        }
      }
    }
  }
}
     
/*************************************
  * 函数功能: 轨迹识别算法
  * 输入参数: uint8_t automatic_threshold
  * 返 回 值: uint8_t calculated_track
  * 说    明: 无
  ************************************/
uint8_t Track_Recognition(void)
{
         int8_t i;
	 uint8_t left, right;  //计算得到的轨迹位置
//         uint8_t calculated_track;
	 left = right = 0;
         
	 for(i = 0; i < 8; i++)   //从左侧寻找轨迹
	 {
		 if(ADC_Converted_Value[i] <= Threshold[i] /*|| ADC_Converted_Value[i + 1] <= Threshold*/)  //3.16在此修改一个判断符号，以适应绿底白线
		 {
		   left = i;
	           break;	
		 }
		 else 
		 {      
			 left = 0;
		 }
	 }
	 for(i = 7; i > -1; i--)  //从右侧寻找轨迹
         {
		 if(ADC_Converted_Value[i] <= Threshold[i] /*|| ADC_Converted_Value[i + 1] <= Threshold*/)  //3.16在此修改一个判断符号，以适应绿底白线
		 {
		   right = i;
		   break;	
		 }
		 else
		 {
			 right = 8;
		 }
         }
	 Track = (left + right) / 2;  //计算中线位置
//         if(abs(Track - Last_Track) > 2)  //若两次轨迹偏差过大
//         {
//           Track = Last_Track;
//         }
//         Last_Track = Track;
	 return Track;
}//End of uint8_t Track_Recognition(void

/************************************************ 文件结束 ************************************************/
