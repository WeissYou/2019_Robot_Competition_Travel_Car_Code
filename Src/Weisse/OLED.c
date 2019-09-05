/**
  ******************************************************************************
  * 文件名程: bsp_i2c_OLED.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: 板载OLED(AT24C02)底层驱动程序
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "S_IIC.h"
#include "OLED.h"
#include "codetab.h"
#include "Sensor.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
unsigned char  ge,shi,bai,qian;

/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 向OLED写入命令
  * 输入参数: I2C_Command：命令代码
  * 返 回 值: 无
  * 说    明：无
  */
void WriteCmd(unsigned char I2C_Command)//写命令
{
	write_i2c(OLED_DEV_ADDR,0x00, I2C_Command);
}

/**
  * 函数功能: 向OLED写入数据
  * 输入参数: I2C_Data：数据
  * 返 回 值: 无
  * 说    明：无
  */
void WriteDat(unsigned char I2C_Data)//写数据
{
	write_i2c(OLED_DEV_ADDR,0x40, I2C_Data);
}

/**
  * 函数功能: 初始化OLED
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_Init(void)
{
//  I2C_InitGPIO();
	HAL_Delay(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

/**
  * 函数功能: 设置光标
  * 输入参数: x：光标x位置
  *           y：光标y位置
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

/**
  * 函数功能: 填充整个屏幕
  * 输入参数: fill_Data：要填充的数据
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

/**
  * 函数功能: 清屏
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

/**
  * 函数功能: 将OLED从休眠中唤醒
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

/**
  * 函数功能: 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

/**
  * 函数功能: 显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * 输入参数: x,y： 起始点坐标(x:0~127, y:0~7);
  *           ch：要显示的字符; 
  *           TextSize：字符大小(1:6*8 ; 2:8*16)
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
//			while(ch != '\0')
//			{
				c = ch - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
//			}
		}break;
		case 2:
		{
//			while(ch != '\0')
//			{
				c = ch - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
//			}
		}break;
	}
}

/**
  * 函数功能: 显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * 输入参数: x,y： 起始点坐标(x:0~127, y:0~7);
  *           ch[]：要显示的字符串; 
  *           TextSize：字符大小(1:6*8 ; 2:8*16)
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

/**
  * 函数功能: 显示codetab.h中的汉字,16*16点阵
  * 输入参数: x,y：起始点坐标(x:0~127, y:0~7) 
  *           N：汉字在codetab.h中的索引
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

/**
  * 函数功能: OLED_DrawBMP，显示BMP位图
  * 输入参数: x0,y0 :起始点坐标(x0:0~127, y0:0~7)
  *           x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
  * 返 回 值: 无
  * 说    明：无
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

/*************************************
  * 函数功能: 十六进制转字符码
  * 输入参数: unsigned char HEX_data
  * 返 回 值: unsigned char ASCII_data
  * 说    明: 无
  ************************************/
unsigned char  HEX_TO_ASCII(unsigned char  HEX_data)
{
  unsigned char  ASCII_data;
  switch(HEX_data)
	{       
	  case 0x00:ASCII_data=0x30;break;
	  case 0x01:ASCII_data=0x31;break;
	  case 0x02:ASCII_data=0x32;break;
	  case 0x03:ASCII_data=0x33;break;
	  case 0x04:ASCII_data=0x34;break;
	  case 0x05:ASCII_data=0x35;break;
	  case 0x06:ASCII_data=0x36;break;
	  case 0x07:ASCII_data=0x37;break;
	  case 0x08:ASCII_data=0x38;break;
	  case 0x09:ASCII_data=0x39;break;
	  case 0x0a:ASCII_data=0x41;break;
	  case 0x0b:ASCII_data=0x42;break;
	  case 0x0c:ASCII_data=0x43;break;
	  case 0x0d:ASCII_data=0x44;break;
	  case 0x0e:ASCII_data=0x45;break;
	  case 0x0f:ASCII_data=0x46;break;
  }  
  return(ASCII_data);  
}

/*************************************
  * 函数功能: 位数分离
  * 输入参数: unsigned int temp_data
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void conversion(unsigned int temp_data)  
{  
  unsigned char  ge_data,shi_data,bai_data ,qian_data;
	qian_data=temp_data/1000 ;
	temp_data=temp_data%1000;   
	bai_data=temp_data/100 ;
	temp_data=temp_data%100;   
	shi_data=temp_data/10 ;
	temp_data=temp_data%10;   
	ge_data=temp_data;
	qian= qian_data;
	bai = bai_data;
	shi = shi_data;
	ge  = ge_data ; 	
}

/*************************************
  * 函数功能: OLED显示信息
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  ************************************/
void OLED_Show_Infos(void)
{ 
  uint8_t i;
  OLED_ShowStr(0,0,(unsigned char*)"Samplings:",1);
  OLED_ShowStr(0,1,(unsigned char*)"1-6",1);
  for(i = 0; i < 6; i ++)
  {
    conversion(ADC_Converted_Value[i]);
    ge=HEX_TO_ASCII(ge);
    shi=HEX_TO_ASCII(shi);
    bai=HEX_TO_ASCII(bai);
//    qian=HEX_TO_ASCII(qian);
    OLED_ShowChar(14, i + 2, ge, 1);	
    OLED_ShowChar(7, i + 2, shi, 1);
    OLED_ShowChar(0, i + 2, bai, 1);	
//    OLED_ShowChar(0, i + 2, qian, 1);
  }
  
  OLED_ShowStr(35,1,(unsigned char*)"7",1);
  for(i = 6; i < 7; i ++)
  {
    conversion(ADC_Converted_Value[i]);
    ge=HEX_TO_ASCII(ge);
    shi=HEX_TO_ASCII(shi);
    bai=HEX_TO_ASCII(bai);
//    qian=HEX_TO_ASCII(qian);
    OLED_ShowChar(49, i - 4, ge, 1);	
    OLED_ShowChar(42, i - 4, shi, 1);
    OLED_ShowChar(35, i - 4, bai, 1);	
//    OLED_ShowChar(42, i - 4, qian, 1);
  }
  
  OLED_ShowStr(64,0,(unsigned char*)"Threshold:",1);
  conversion(Threshold);
  ge=HEX_TO_ASCII(ge);
  shi=HEX_TO_ASCII(shi);
  bai=HEX_TO_ASCII(bai);
//  qian=HEX_TO_ASCII(qian);
  OLED_ShowChar(78, 1, ge, 1);	
  OLED_ShowChar(71, 1, shi, 1);
  OLED_ShowChar(64, 1, bai, 1);	
//  OLED_ShowChar(90, 1, qian, 1);
  
  OLED_ShowStr(64,3,(unsigned char*)"Track:",1);
  conversion(Track);
  ge=HEX_TO_ASCII(ge);
//  shi=HEX_TO_ASCII(shi);
  OLED_ShowChar(71, 4, ge, 1);	
//  OLED_ShowChar(64, 4, shi, 1);
//  OLED_ShowChar(90, 4, bai, 1);	
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
