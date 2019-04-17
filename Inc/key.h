#ifndef _key_H
#define _key_H
#include "main.h"
#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)  //KEY0����PH3
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)  //KEY1����PH2
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) //KEY2����PC13
#define WK_UP       HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin)  //WKUP����PA0

#define KEY0_PASS   1
#define KEY1_PASS   2
#define KEY2_PASS   3
#define WKUP_PASS   4
uint8_t Key_Scan(uint8_t mode);
void Get_KeyVul(void);
#endif



