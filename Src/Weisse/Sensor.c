/*****************************************************************************
  * 文件名称: Sensor.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 灰度传感器驱动、其他传感器的状态检测
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Sensor.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
uint16_t ADC_Converted_Value[8] = {0};
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: 获取ADC结果
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Get_ADC_Value(void)
{
  /* 片选通道零 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[7] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
  /* 片选通道一 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[2] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
  /* 片选通道二 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[0] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果  //
  }
  /* 片选通道三 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[3] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
  /* 片选通道四 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[1] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
  /* 片选通道五 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[5] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  } 
  /* 片选通道六 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[6] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
  /* 片选通道七 */
  HAL_GPIO_WritePin(Front_Sensor_D3_GPIO_Port, Front_Sensor_D3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D2_GPIO_Port, Front_Sensor_D2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Front_Sensor_D1_GPIO_Port, Front_Sensor_D1_Pin, GPIO_PIN_SET);
  HAL_ADC_Start(&hadc1);  //ADC转换开始
  HAL_ADC_PollForConversion(&hadc1,1);
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) 
  {
          ADC_Converted_Value[4] = HAL_ADC_GetValue(&hadc1);  //获取ADC结果//
  }
}

/*************************************
  * 函数功能: 传感器校对
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Sensor_Check(uint8_t dir)
{
  if(dir == LEFT)
  {
    if(RIGHT_SENSOR_1_WHITE)
    {
      RGB_R_ON;  //红光
    }
    else 
    {
      RGB_R_OFF;
    }
  
    if(RIGHT_SENSOR_2_WHITE)
    {
      RGB_G_ON;  //绿光
    }
    else 
    {
      RGB_G_OFF;  
    }
  
    if(RIGHT_SENSOR_3_WHITE)
    {
      RGB_B_ON;  //蓝光
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
      /* 黄灯亮 */
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
      RGB_R_ON;  //红光
    }
    else 
    {
      RGB_R_OFF;
    }
  
    if(RIGHT_SENSOR_2_WHITE)
    {
      RGB_G_ON;  //绿光
    }
    else 
    {
      RGB_G_OFF;  
    }
  
    if(RIGHT_SENSOR_3_WHITE)
    {
      RGB_B_ON;  //蓝光
    }
    else 
    {
      RGB_B_OFF;
    }
  
    if(RIGHT_SENSOR_4_WHITE)
    {
      /* 黄灯亮 */
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

/************************************************ 文件结束 ************************************************/
