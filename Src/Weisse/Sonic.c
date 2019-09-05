/*****************************************************************************
  * 文件名称: Servo.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 超声波模块驱动
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "Sonic.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
STRUCT_CAPTURE strCapture = { 0, 0, 0 };
uint32_t temp;
uint32_t ulTmrClk, ulTime;
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能：超声波模块用延时
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void US100_DelayUS ( __IO uint32_t ulCount )
{
  uint32_t i;
  for ( i = 0; i < ulCount; i ++ )
  {
    uint8_t uc = 12;     //设置值为12，大约延1微秒  	      
    while ( uc -- );     //延1微秒	
  }	
}

/*************************************
  * 函数功能：超声波模块初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Sonic_Init(void)
{
  printf ( "超声波测距\n" );  
  /* 获取定时器时钟周期 */	
	ulTmrClk = HAL_RCC_GetHCLKFreq() / GENERAL_TIM_PRESCALER;  //83为定时器预分频系数    
  /* 启动定时器 */
  HAL_TIM_Base_Start_IT(&htim11);  
  /* 启动定时器通道输入捕获并开启中断 */
  HAL_TIM_IC_Start_IT(&htim11, TIM_CHANNEL_1);
}
    
/*************************************
  * 函数功能: 超声波模块测距开始
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void Sonic_Start(void)
{
  HAL_GPIO_WritePin(Sonic_Trig_GPIO_Port, Sonic_Trig_Pin, GPIO_PIN_SET);
  US100_DelayUS(20);
  HAL_GPIO_WritePin(Sonic_Trig_GPIO_Port, Sonic_Trig_Pin, GPIO_PIN_RESET);
  
  /* 完成测量高电平脉宽 */
  if(strCapture.ucFinishFlag == 1 )
  {
    /* 计算高电平计数值 */
    ulTime = strCapture .usPeriod * GENERAL_TIM_PERIOD + strCapture .usCtr;
    temp=((ulTime % ulTmrClk)*340)/(1000*2);
    printf ( ">>测得距离为：%dmm\n",temp); 
    strCapture .ucFinishFlag = 0;		
    HAL_Delay(1000);       
  }    
}

/*************************************
  * 函数功能: 定时器捕获中断内容
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void TIM_IC_Content(void)
{
  TIM_IC_InitTypeDef sConfigIC;
  
  if (strCapture.ucStartFlag == 0)
  {
    htim11.Instance->CNT=0; // 清零定时器计数
    strCapture .usPeriod = 0;			
    strCapture .usCtr = 0;
    
    // 配置输入捕获参数，主要是修改触发电平
    sConfigIC.ICPolarity = GENERAL_TIM_END_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1);
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(&htim11, TIM_IT_CC1);
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htim11,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 1;			
  }		
  
  else
  {
    // 获取定时器计数值
    strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htim11,TIM_CHANNEL_1);
    // 配置输入捕获参数，主要是修改触发电平
    sConfigIC.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1);
    
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(&htim11, TIM_IT_CC1); 
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htim11,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 0;			
    strCapture .ucFinishFlag = 1;    
  }
 // HAL_Delay(1000);
}
    
/************************************************ 文件结束 ************************************************/
