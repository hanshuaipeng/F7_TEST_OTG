/**
  ******************************************************************************
  * File Name          : SDMMC.c
  * Description        : This file provides code for the configuration
  *                      of the SDMMC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sdmmc.h"

/* USER CODE BEGIN 0 */
//SD_ReadDisk/SD_WriteDisk����ר��buf,�����������������ݻ�������ַ����4�ֽڶ����ʱ��,
//��Ҫ�õ�������,ȷ�����ݻ�������ַ��4�ֽڶ����.
__align(4) uint8_t SDIO_DATA_BUFFER[512];
//���ʹ��DMA�Ļ��������������������SD����д�Ƿ����
static volatile uint8_t SDCardWriteStatus=0,SDCardReadStatus=0; 
HAL_SD_CardInfoTypeDef SDCardInfo;
/* USER CODE END 0 */

SD_HandleTypeDef hsd1;

/* SDMMC1 init function */

void MX_SDMMC1_SD_Init(void)
{

  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */
    /* SDMMC1 clock enable */
    __HAL_RCC_SDMMC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN SDMMC1_MspInit 1 */
	
  /* USER CODE END SDMMC1_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC1_CLK_DISABLE();
  
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
//�õ�����Ϣ
//cardinfo:����Ϣ�洢��
//����ֵ:����״̬
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
	sta=HAL_SD_GetCardInfo(&hsd1,cardinfo);
    return sta;
}
//�ж�SD���Ƿ���Դ���(��д)����
//����ֵ:SD_TRANSFER_OK ������ɣ����Լ�����һ�δ���
//		 SD_TRANSFER_BUSY SD����æ�������Խ�����һ�δ���
uint8_t SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&hsd1)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}

//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
	sta=HAL_SD_ReadBlocks(&hsd1, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector�Ķ�����
	
	//�ȴ�SD������
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }
    INTX_ENABLE();//�������ж�
    return sta;
}  


//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;	
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
	sta=HAL_SD_WriteBlocks(&hsd1,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector��д����
		
	//�ȴ�SD��д��
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }    
	INTX_ENABLE();//�������ж�
    return sta;
}


void read_sdinfo()
{
	SD_GetCardInfo(&SDCardInfo);
	printf("BlockNbr:%d\r\n",SDCardInfo.BlockNbr);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
