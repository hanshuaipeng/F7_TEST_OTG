#include "lcd.h"
#include "font.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint32_t POINT_COLOR=BLACK;		//������ɫ
uint32_t BACK_COLOR =WHITE;  	//����ɫ 
/************************************************************************************************
�������ܣ�ָ��λ����ʾһ���ַ�
��ڲ�������x,y������
			num����ʾ���ַ�
			size����ʾ�Ĵ�С12/16/24/32
			mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
����ֵ����
************************************************************************************************/
void LTDC_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
	 uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����3216����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LTDC_Draw_Point(x,y,POINT_COLOR);
			else if(mode==0)LTDC_Draw_Point(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcdltdc.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcdltdc.width)return;	//��������
				break;
			}
		}  	 
	} 
}

/************************************************************************************************
�������ܣ�ָ��λ����ʾ�ַ���
��ڲ�������x,y������
			str����ʾ���ַ���
			size����ʾ�Ĵ�С12/16/24/32
����ֵ����
************************************************************************************************/
void LTDC_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *str)
{
	 while((*str<='~')&&(*str>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        LTDC_ShowChar(x,y,*str,size,0);
        x+=size/2;
        str++;
    }  
}
/************************************************************************************************
�������ܣ�m^n����
��ڲ�����
����ֵ��m^n�η�.
************************************************************************************************/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
/************************************************************************************************
�������ܣ���ʾ����,��λΪ0,����ʾ
��ڲ�����	x,y :�������	 
			len :���ֵ�λ��
			size:�����С
			color:��ɫ 
			num:��ֵ(0~4294967295);
����ֵ��
************************************************************************************************/ 
void LTDC_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LTDC_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
/************************************************************************************************
�������ܣ���ʾ����,��λΪ0,����ʾ
��ڲ�����	x,y :�������	 
			num:��ֵ(0~999999999);	
			len:����(��Ҫ��ʾ��λ��)
			size:�����С
			num:��ֵ(0~4294967295);
			mode:
				[7]:0,�����;1,���0.
				[6:1]:����
				[0]:0,�ǵ�����ʾ;1,������ʾ.
����ֵ��
************************************************************************************************/ 
void LTDC_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LTDC_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LTDC_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
/************************************************************************************************
�������ܣ�����
��ڲ�����	x1,y1:�������
			x2,y2:�յ�����  
����ֵ��
************************************************************************************************/ 
void LTDC_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LTDC_Draw_Point(uRow,uCol,POINT_COLOR);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
/************************************************************************************************
�������ܣ�������	  
��ڲ�����(x1,y1),(x2,y2):���εĶԽ�����
����ֵ��
************************************************************************************************/ 
void LTDC_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LTDC_DrawLine(x1,y1,x2,y1);
	LTDC_DrawLine(x1,y1,x1,y2);
	LTDC_DrawLine(x1,y2,x2,y2);
	LTDC_DrawLine(x2,y1,x2,y2);
}
/************************************************************************************************
�������ܣ���ָ��λ�û�һ��ָ����С��Բ
��ڲ�����(x,y):���ĵ�
			r :�뾶
����ֵ��
************************************************************************************************/
void LTDC_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LTDC_Draw_Point(x0+a,y0-b,POINT_COLOR);             //5
 		LTDC_Draw_Point(x0+b,y0-a,POINT_COLOR);             //0           
		LTDC_Draw_Point(x0+b,y0+a,POINT_COLOR);             //4               
		LTDC_Draw_Point(x0+a,y0+b,POINT_COLOR);             //6 
		LTDC_Draw_Point(x0-a,y0+b,POINT_COLOR);             //1       
 		LTDC_Draw_Point(x0-b,y0+a,POINT_COLOR);             
		LTDC_Draw_Point(x0-a,y0-b,POINT_COLOR);             //2             
  		LTDC_Draw_Point(x0-b,y0-a,POINT_COLOR);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	
