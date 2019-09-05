#ifndef _OLED_H_
#define	_OLED_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "Track.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define OLED_DEV_ADDR			   0x78

/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void conversion(unsigned int temp_data);
unsigned char  HEX_TO_ASCII(unsigned char  HEX_data);
void OLED_Show_Infos(void);

#endif /* __I2C_OLED_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
