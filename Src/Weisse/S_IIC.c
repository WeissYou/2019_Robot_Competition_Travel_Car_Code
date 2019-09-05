/**
  ******************************************************************************
  * 文件名程: bsp_eeprom.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: mpr121底层驱动程序（软件I2C）
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "S_IIC.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: I2C总线位延迟，最快400KHz
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void I2C_Delay(void)
{
	uint8_t i;

	for (i = 0; i < 30; i++);
}

/**
  * 函数功能: CPU发起I2C总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_HIGH();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_LOW();
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
}

/**
  * 函数功能: CPU发起I2C总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_HIGH();
}

/**
  * 函数功能: CPU向I2C总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_SendByte(uint8_t Byte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (Byte & 0x80)
		{
			I2C_SDA_HIGH();
		}
		else
		{
			I2C_SDA_LOW();
		}
		I2C_Delay();
		I2C_SCL_HIGH();
		I2C_Delay();	
		I2C_SCL_LOW();
		if (i == 7)
		{
			I2C_SDA_HIGH(); // 释放总线
		}
		Byte <<= 1;	/* 左移一个bit */
		I2C_Delay();
	}
}


/**
  * 函数功能: CPU从I2C总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_HIGH();
		I2C_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_LOW();
		I2C_Delay();
	}
	return value;
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	I2C_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_LOW();
	I2C_Delay();
	return re;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Ack(void)
{
	I2C_SDA_LOW();	/* CPU驱动SDA = 0 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_NAck(void)
{
	I2C_SDA_HIGH();	/* CPU驱动SDA = 1 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();	
}

/**
  * 函数功能: 配置I2C总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
//void I2C_InitGPIO(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//  
//  /* 打开GPIO时钟 */
//	I2C_GPIO_CLK_ENABLE();
//
//  GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
//  HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
//
//  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
//  I2C_Stop();
//}

/**
  * 函数功能: 写入数据到i2c
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void write_i2c(uint8_t device,uint8_t addr,uint8_t dat)
{
	I2C_Start();
	I2C_SendByte(device);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_SendByte(dat);
	I2C_WaitAck();
	I2C_Stop();
}

/**
  * 函数功能: 从i2c读取数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
uint8_t read_i2c(uint8_t device,uint8_t addr)
{
	uint8_t dat;
	I2C_Start();
	I2C_SendByte(device);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(device+0x01);
	I2C_WaitAck();
	dat=I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
	return dat;
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
