#ifndef _Servo_h_
#define _Servo_h_

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
/* 类型定义 ------------------------------------------------------------------*/
//舵机控制指令
/* 定时器3通道1 */
#define Semi 230  //旋转半周
#define Zero 98  //原点

/* 定时器3通道2 */
#define Open       50  //爪子张开
#define Half       130  //爪子半张
#define Close      200  //爪子闭合

/* 定时器3通道3 */
#define Vertical 130  //机械臂置竖直
#define OBLIQUE 140  //机械臂倾斜
#define Halb  170  //机械臂更倾斜
#define Horizontal   230  //机械臂置水平
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void Servo_Init(void);
void Arm_Control(uint8_t action);
void Rotation_Control(uint8_t action);
void Paw_Control(uint8_t action);
void Motor_PWM_SetDutyRatio(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t value);

#endif /* _Servo_h_ */

/************************************************ 文件结束 ************************************************/
