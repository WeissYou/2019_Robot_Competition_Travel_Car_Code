#ifndef _SENSOR_h_
#define _SENSOR_h_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "RGB.h"
#include "Control.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define LEFT_SENSOR_1_BLACK (HAL_GPIO_ReadPin(Left_Sensor_1_GPIO_Port, Left_Sensor_1_Pin) == 1)
#define LEFT_SENSOR_1_WHITE (HAL_GPIO_ReadPin(Left_Sensor_1_GPIO_Port, Left_Sensor_1_Pin) == 0)

#define LEFT_SENSOR_2_BLACK (HAL_GPIO_ReadPin(Left_Sensor_2_GPIO_Port, Left_Sensor_2_Pin) == 1)
#define LEFT_SENSOR_2_WHITE (HAL_GPIO_ReadPin(Left_Sensor_2_GPIO_Port, Left_Sensor_2_Pin) == 0)

#define LEFT_SENSOR_3_BLACK (HAL_GPIO_ReadPin(Left_Sensor_3_GPIO_Port, Left_Sensor_3_Pin) == 1)
#define LEFT_SENSOR_3_WHITE (HAL_GPIO_ReadPin(Left_Sensor_3_GPIO_Port, Left_Sensor_3_Pin) == 0)

#define LEFT_SENSOR_4_BLACK (HAL_GPIO_ReadPin(Left_Sensor_4_GPIO_Port, Left_Sensor_4_Pin) == 1)
#define LEFT_SENSOR_4_WHITE (HAL_GPIO_ReadPin(Left_Sensor_4_GPIO_Port, Left_Sensor_4_Pin) == 0)

#define RIGHT_SENSOR_1_BLACK (HAL_GPIO_ReadPin(Right_Sensor_1_GPIO_Port, Right_Sensor_1_Pin) == 1)
#define RIGHT_SENSOR_1_WHITE (HAL_GPIO_ReadPin(Right_Sensor_1_GPIO_Port, Right_Sensor_1_Pin) == 0)

#define RIGHT_SENSOR_2_BLACK (HAL_GPIO_ReadPin(Right_Sensor_2_GPIO_Port, Right_Sensor_2_Pin) == 1)
#define RIGHT_SENSOR_2_WHITE (HAL_GPIO_ReadPin(Right_Sensor_2_GPIO_Port, Right_Sensor_2_Pin) == 0)

#define RIGHT_SENSOR_3_BLACK (HAL_GPIO_ReadPin(Right_Sensor_3_GPIO_Port, Right_Sensor_3_Pin) == 1)
#define RIGHT_SENSOR_3_WHITE (HAL_GPIO_ReadPin(Right_Sensor_3_GPIO_Port, Right_Sensor_3_Pin) == 0)

#define RIGHT_SENSOR_4_BLACK (HAL_GPIO_ReadPin(Right_Sensor_4_GPIO_Port, Right_Sensor_4_Pin) == 1)
#define RIGHT_SENSOR_4_WHITE (HAL_GPIO_ReadPin(Right_Sensor_4_GPIO_Port, Right_Sensor_4_Pin) == 0)
/* ��չ���� ------------------------------------------------------------------*/
extern uint16_t ADC_Converted_Value[8];
/* �������� ------------------------------------------------------------------*/
void Get_ADC_Value(void);
void Sensor_Check(uint8_t dir);

#endif /* _SENSOR_h_ */

/************************************************ �ļ����� ************************************************/
