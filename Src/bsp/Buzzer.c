/*****************************************************************************
  * 文件名称: Buzzer.c
  * 作    者: 尤伟宏
  * 编写日期: 一月 2019
  * 功    能: 蜂鸣器配置
  ****************************************************************************
  * 说    明:
  *
  *****************************************************************************/

  /* 包含头文件 ----------------------------------------------------------------*/
#include "Buzzer.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
static uint16_t remaining_time_cnt = 0;              // 剩余时间
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 板载蜂鸣器IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：须置于任务定时器回调函数中
  */
void BEEP_GPIO_Init(void)
{
	/* 蜂鸣器测试 */
	BEEP_StateSet(BEEPState_ON);
  Task_Delay_Start(10);  //蜂鸣器鸣叫10个任务周期，即500ms
	BEEP_StateSet(BEEPState_OFF);
}//End of BEEP_GPIO_Init(void)

/**
  * 函数功能: 设置板载蜂鸣器的状态
  * 输入参数：state:设置蜂鸣器的状态。
  *             可选值：BEEPState_OFF：蜂鸣器不响；
  *             可选值：BEEPState_ON： 蜂鸣器响。
  * 返 回 值: 无
  * 说    明：该函数使用类似HALA库函数的编程方法，方便理解HAL库函数编程思想。
  */
void BEEP_StateSet(BEEPState_TypeDef state)
{
	/* 检查输入参数是否合法 */
	assert_param(BEEPState_TypeDef(state));

	/* 判断设置的蜂鸣器状态，如果设置为蜂鸣器响 */
	if (state == BEEPState_ON)
	{
		BEEP_ON;/* 蜂鸣器响 */
	}
	else /* state=BEEPState_OFF：设置蜂鸣器不响 */
	{
		BEEP_OFF;/* 蜂鸣器不响 */
	}
}//End of void BEEP_StateSet(BEEPState_TypeDef state)

/*******************************************************************************
*                           陈苏阳@2018-11-24
* Function Name  :  Buzzer_Tweet
* Description    :  蜂鸣器鸣叫
* Input          :  unsigned char tweet_time            // 鸣叫持续的时间
* Output         :  None
* Return         :  None
*******************************************************************************/
void Buzzer_Tweet(unsigned char tweet_time)
{
	// 设置剩余鸣叫时间
	remaining_time_cnt = tweet_time;

	// 开启蜂鸣器
	BEEP_StateSet(BEEPState_ON);

}// End of void Buzzer_Tweet(unsigned char tweet_time)


/*******************************************************************************
*                           陈苏阳@2018-11-24
* Function Name  :  Buzzer_Timer_Handle
* Description    :  蜂鸣器定时处理函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Buzzer_Timer_Handle(void)
{
	// 检查还有没有剩余时间
	if (remaining_time_cnt)
	{
		// 开启蜂鸣器
		BEEP_StateSet(BEEPState_ON);
		remaining_time_cnt--;
	}
	else
	{
		// 关闭蜂鸣器
		BEEP_StateSet(BEEPState_OFF);
	}
}// End of void Buzzer_Timer_Handle(void)

/************************************************ 文件结束 ************************************************/
