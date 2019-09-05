/*****************************************************************************
  * �ļ�����: Servo.c
  * ��    ��: ��ΰ��
  * ��д����: һ�� 2019
  * ��    ��: ������ģ������
  ****************************************************************************
  * ˵    ��:
  * 
  *****************************************************************************/
	
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Sonic.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
STRUCT_CAPTURE strCapture = { 0, 0, 0 };
uint32_t temp;
uint32_t ulTmrClk, ulTime;
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/*************************************
  * �������ܣ�������ģ������ʱ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void US100_DelayUS ( __IO uint32_t ulCount )
{
  uint32_t i;
  for ( i = 0; i < ulCount; i ++ )
  {
    uint8_t uc = 12;     //����ֵΪ12����Լ��1΢��  	      
    while ( uc -- );     //��1΢��	
  }	
}

/*************************************
  * �������ܣ�������ģ���ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Sonic_Init(void)
{
  printf ( "���������\n" );  
  /* ��ȡ��ʱ��ʱ������ */	
	ulTmrClk = HAL_RCC_GetHCLKFreq() / GENERAL_TIM_PRESCALER;  //83Ϊ��ʱ��Ԥ��Ƶϵ��    
  /* ������ʱ�� */
  HAL_TIM_Base_Start_IT(&htim11);  
  /* ������ʱ��ͨ�����벶�񲢿����ж� */
  HAL_TIM_IC_Start_IT(&htim11, TIM_CHANNEL_1);
}
    
/*************************************
  * ��������: ������ģ���࿪ʼ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void Sonic_Start(void)
{
  HAL_GPIO_WritePin(Sonic_Trig_GPIO_Port, Sonic_Trig_Pin, GPIO_PIN_SET);
  US100_DelayUS(20);
  HAL_GPIO_WritePin(Sonic_Trig_GPIO_Port, Sonic_Trig_Pin, GPIO_PIN_RESET);
  
  /* ��ɲ����ߵ�ƽ���� */
  if(strCapture.ucFinishFlag == 1 )
  {
    /* ����ߵ�ƽ����ֵ */
    ulTime = strCapture .usPeriod * GENERAL_TIM_PERIOD + strCapture .usCtr;
    temp=((ulTime % ulTmrClk)*340)/(1000*2);
    printf ( ">>��þ���Ϊ��%dmm\n",temp); 
    strCapture .ucFinishFlag = 0;		
    HAL_Delay(1000);       
  }    
}

/*************************************
  * ��������: ��ʱ�������ж�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  ************************************/
void TIM_IC_Content(void)
{
  TIM_IC_InitTypeDef sConfigIC;
  
  if (strCapture.ucStartFlag == 0)
  {
    htim11.Instance->CNT=0; // ���㶨ʱ������
    strCapture .usPeriod = 0;			
    strCapture .usCtr = 0;
    
    // �������벶���������Ҫ���޸Ĵ�����ƽ
    sConfigIC.ICPolarity = GENERAL_TIM_END_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1);
    // ����жϱ�־λ
    __HAL_TIM_CLEAR_IT(&htim11, TIM_IT_CC1);
    // �������벶�񲢿����ж�
    HAL_TIM_IC_Start_IT(&htim11,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 1;			
  }		
  
  else
  {
    // ��ȡ��ʱ������ֵ
    strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htim11,TIM_CHANNEL_1);
    // �������벶���������Ҫ���޸Ĵ�����ƽ
    sConfigIC.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1);
    
    // ����жϱ�־λ
    __HAL_TIM_CLEAR_IT(&htim11, TIM_IT_CC1); 
    // �������벶�񲢿����ж�
    HAL_TIM_IC_Start_IT(&htim11,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 0;			
    strCapture .ucFinishFlag = 1;    
  }
 // HAL_Delay(1000);
}
    
/************************************************ �ļ����� ************************************************/
