/*****************************************************************************
  * 文件名称: RGB.c
  * 作    者: 尤伟宏
  * 编写日期: 四月 2019
  * 功    能: RGB模块相关
  ****************************************************************************
  * 说    明:
  * 
  *****************************************************************************/
	
/* 包含头文件 ----------------------------------------------------------------*/
#include "RGB.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/*************************************
  * 函数功能: RGB模块自检
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void RGB_Init(void)
{
  RGB_R_ON;  //红灯亮
  HAL_Delay(150);  
  RGB_R_OFF;  //红灯灭
  RGB_G_ON;  //绿灯亮
  HAL_Delay(150);
  RGB_G_OFF;  //绿灯灭
  RGB_B_ON;  //绿灯亮
  HAL_Delay(150);
  RGB_B_OFF;  //绿灯灭
  HAL_Delay(50);
}

/*************************************
  * 函数功能: RGB模块工作
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void RGB_Blink(void)
{
  static uint8_t i, j = 0;
  switch(i)
  {
    case 0:
    {
      RGB_R_ON;
      RGB_G_OFF;
      RGB_B_OFF;
      if(++ j > 10)
      {
        i ++;
        j = 0;
      }
      
      break;
    }
    
    case 1:
    {
      RGB_R_OFF;
      RGB_G_ON;
      RGB_B_OFF;
      if(++ j > 10)
      {
        i ++;
        j = 0;
      }
      break;
    }
    
    case 2:
    {
      RGB_R_OFF;
      RGB_G_OFF;
      RGB_B_ON;
      if(++ j > 10)
      {
        i ++;
        j = 0;
      }
      break;
    }
    
    case 3:
    {
      i = 0;
      j = 0;
      break;
    }
  }
}
    
/************************************************ 文件结束 ************************************************/
