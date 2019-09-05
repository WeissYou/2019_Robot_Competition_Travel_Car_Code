#ifndef _DATA_FLASH_h_
#define _DATA_FLASH_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct
{
  uint8_t Black_Value_in_Flash[8];  //Flash�к�ɫ�Ĳ���ֵ
  uint8_t White_Value_in_Flash[8];  //Flash�а�ɫ�Ĳ���ֵ
}data_flash_info_struct;
extern data_flash_info_struct test_struct;
/* �궨�� --------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void WriteDataFlash(uint8_t* buff, uint32_t Len);
void ReadDataFlash(uint8_t* buff, uint32_t dest_addr, uint32_t Len);
void Flash_Handle(void);

#endif /* _DATA_FLASH_h_ */

/************************************************ �ļ����� ************************************************/



