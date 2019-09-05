#ifndef _DATA_FLASH_h_
#define _DATA_FLASH_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef struct
{
  uint8_t Black_Value_in_Flash[8];  //Flash中黑色的采样值
  uint8_t White_Value_in_Flash[8];  //Flash中白色的采样值
}data_flash_info_struct;
extern data_flash_info_struct test_struct;
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void WriteDataFlash(uint8_t* buff, uint32_t Len);
void ReadDataFlash(uint8_t* buff, uint32_t dest_addr, uint32_t Len);
void Flash_Handle(void);

#endif /* _DATA_FLASH_h_ */

/************************************************ 文件结束 ************************************************/



