#include "key.h"
/*****************************************
�������ܣ�����ɨ��
��ڲ�����mode��0����֧��������1��֧������
����ֵ������ֵ
*****************************************/
uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up=1;
    if(mode == 1)
    {
        key_up=1;
    }
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
    {
        HAL_Delay(10);
        key_up=0;
        if(KEY0 == 0) 
        {            
            return KEY0_PASS;
        }
        else if(KEY1 == 0)  
        {
            return KEY1_PASS;
        }
        else if(KEY2 == 0)  
        {
            return KEY2_PASS;
        }
        else if(WK_UP == 1) 
        {
            return WKUP_PASS;
        }
    }
    else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
    {
        key_up=1;
    }
    return 0;
}
/*****************************************
�������ܣ���ȡ����ֵ
��ڲ�������
����ֵ����
*****************************************/
void Get_KeyVul(void)
{
    switch(Key_Scan(0))
      {
          case KEY0_PASS:
              printf("key0 is pass\r\n");
          break;
          case KEY1_PASS:
              printf("key1 is pass\r\n");
          break;
          case KEY2_PASS:
              printf("key2 is pass\r\n");
          break;
          case WKUP_PASS:
              printf("wk_up is pass\r\n");
          break;
          default : break;
      }
}

