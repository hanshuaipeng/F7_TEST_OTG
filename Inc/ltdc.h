/**
  ******************************************************************************
  * File Name          : LTDC.h
  * Description        : This file provides code for the configuration
  *                      of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ltdc_H
#define __ltdc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern LTDC_HandleTypeDef hltdc;

/* USER CODE BEGIN Private defines */
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  
//LCD LTDC��Ҫ������
typedef struct  
{							 
	uint32_t pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	uint32_t pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	uint16_t hsw;			//ˮƽͬ�����
	uint16_t vsw;			//��ֱͬ�����
	uint16_t hbp;			//ˮƽ����
	uint16_t vbp;			//��ֱ����
	uint16_t hfp;			//ˮƽǰ��
	uint16_t vfp;			//��ֱǰ�� 
	uint8_t activelayer;		//��ǰ����:0/1	
	uint8_t dir;				//0,����;1,����;
	uint16_t width;			//LCD���
	uint16_t height;			//LCD�߶�
	uint32_t pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 
extern _ltdc_dev lcdltdc;		            //����LCD LTDC����
/* USER CODE END Private defines */

void MX_LTDC_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ ltdc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
