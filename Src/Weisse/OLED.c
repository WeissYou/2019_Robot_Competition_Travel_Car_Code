/**
  ******************************************************************************
  * �ļ�����: bsp_i2c_OLED.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ����OLED(AT24C02)�ײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "S_IIC.h"
#include "OLED.h"
#include "codetab.h"
#include "Sensor.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
unsigned char  ge,shi,bai,qian;

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��OLEDд������
  * �������: I2C_Command���������
  * �� �� ֵ: ��
  * ˵    ������
  */
void WriteCmd(unsigned char I2C_Command)//д����
{
	write_i2c(OLED_DEV_ADDR,0x00, I2C_Command);
}

/**
  * ��������: ��OLEDд������
  * �������: I2C_Data������
  * �� �� ֵ: ��
  * ˵    ������
  */
void WriteDat(unsigned char I2C_Data)//д����
{
	write_i2c(OLED_DEV_ADDR,0x40, I2C_Data);
}

/**
  * ��������: ��ʼ��OLED
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_Init(void)
{
//  I2C_InitGPIO();
	HAL_Delay(100); //�������ʱ����Ҫ
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
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
  * ��������: ���ù��
  * �������: x�����xλ��
  *           y�����yλ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

/**
  * ��������: ���������Ļ
  * �������: fill_Data��Ҫ��������
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_Fill(unsigned char fill_Data)//ȫ�����
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
  * ��������: ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}

/**
  * ��������: ��OLED�������л���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

/**
  * ��������: ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

/**
  * ��������: ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * �������: x,y�� ��ʼ������(x:0~127, y:0~7);
  *           ch��Ҫ��ʾ���ַ�; 
  *           TextSize���ַ���С(1:6*8 ; 2:8*16)
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * �������: x,y�� ��ʼ������(x:0~127, y:0~7);
  *           ch[]��Ҫ��ʾ���ַ���; 
  *           TextSize���ַ���С(1:6*8 ; 2:8*16)
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: ��ʾcodetab.h�еĺ���,16*16����
  * �������: x,y����ʼ������(x:0~127, y:0~7) 
  *           N��������codetab.h�е�����
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: OLED_DrawBMP����ʾBMPλͼ
  * �������: x0,y0 :��ʼ������(x0:0~127, y0:0~7)
  *           x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: ʮ������ת�ַ���
  * �������: unsigned char HEX_data
  * �� �� ֵ: unsigned char ASCII_data
  * ˵    ��: ��
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
  * ��������: λ������
  * �������: unsigned int temp_data
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: OLED��ʾ��Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
