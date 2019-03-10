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
uint8_t Threshold;             //阈值
uint8_t Calculated_Track;         //计算得到的黑线位置
uint8_t Last_Calculated_Track;  //上次计算得到的黑线位置
uint16_t Objective_Track;  //实际上的黑线位置
uint16_t Last_Objective_Track;  //上次实际上的黑线位置
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 自动阈值算法
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Automatic_Threshold(void)
{
	 static uint8_t i;
	 static uint8_t value1_max, value1_min;
	
	 value1_max = ADC_ConvertedValueLocal[0];  //动态阈值算法，读取最大和最小值
   for(i = 0; i < 12; i ++)   
   {
     if(value1_max <= ADC_ConvertedValueLocal[i])
     value1_max = ADC_ConvertedValueLocal[i];
   }
	 value1_min = ADC_ConvertedValueLocal[0];  //最小值
   for(i = 0; i < 12; i ++) 
   {
     if(value1_min >= ADC_ConvertedValueLocal[i])
     value1_min = ADC_ConvertedValueLocal[i];
   }
   Threshold = (value1_max + value1_min) / 2;	  //计算出本次中线提取的阈值
}//End of void Automatic_Threshold(void)

/*************************************
  * 函数功能: 简单二值化算法
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
	Last_Objective_Track = Objective_Track;  //保存上一次的轨迹
	return Objective_Track;
}

/*************************************
  * 函数功能: 轨迹识别算法
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
uint8_t Track_Recognition(void)
{
	 static int8_t i;
	 static uint8_t left, right;
	 for(i = 0; i < 12; i++)   //从左侧寻找轨迹
	 {
		 if(ADC_ConvertedValueLocal[i] > Threshold && ADC_ConvertedValueLocal[i + 1] > Threshold)
		 {
		   left = i;
			 break;	
		 }
	 }
	 for(i = 11; i > -1; i--)  //从右侧寻找轨迹
   {
		 if(ADC_ConvertedValueLocal[i] > Threshold && ADC_ConvertedValueLocal[i + 1] > Threshold)
		 {
		   right = i;
		   break;	
		 }
   }
	 Calculated_Track = (right + left) / 2;  //计算中线位置
//	 if(abs(Calculated_Track - Last_Calculated_Track) > 4)   //计算中线的偏差，如果太大
//	 {
//	   Calculated_Track = Last_Calculated_Track;    //则取上一次的值
//	 }
	 Last_Calculated_Track = Calculated_Track;  //保存上一次的轨迹
	 return Calculated_Track;
}//End of Track_Recognition(void)
/************************************************ 文件结束 ************************************************/
