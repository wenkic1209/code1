/**
  **********************************  STM8S  ***********************************
  * @�ļ���     �� LCD12232.c
  * @����       �� wenkic
  * @��汾     �� V2.2.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��12��26��
  * @ժҪ       �� UARTԴ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-12-27 V1.0.0:��ʼ�汾
	LCD12232����:
	            ʹ�õ���68ϵ�е�MCU�ӿ� (E,R/W,A0,D0->D7)
							     E�Ǹߵ�ƽ��Ч, R/W �ߵ�ƽʹ�� ,A0Ϊ1ʱ������,0ʱΪָ������
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "LCD12232.h"
#include "sys.h"       // λ����ͷ�ļ�
#include "delay.h"     //��ʱ����ͷ�ļ�
#include "stm32f10x.h" //stm32ͷ�ļ�
#include "delively_model.h"

/* ���������ݱ���--------------------------------------------------------------*/ 
/*
--------------------------------------------------------------------|
     4      |  RESET      |      PB1       |   ��λ��								|
--------------------------------------------------------------------|
     5      |  CS1(E1)    |      PA0       |   (��ѡ��1���)					|
--------------------------------------------------------------------|
     6      |  CS2(E2)    |      PA1       |   (��ѡ��2�ұ�)					| 					
--------------------------------------------------------------------|
     7      |  R/W        |      PB5       |   ��д����   (ʹ��)			|
--------------------------------------------------------------------|
     8      |  A0         |      PB2       |   ����(��)/����(��)ѡ��	|
--------------------------------------------------------------------|
     9      |  D0-->D7    |   PB8-->PB15   |   ������								|			
--------------------------------------------------------------------|
     17     |  LED+       |   PB0          |   ��ʾ�����						|			
--------------------------------------------------------------------|
*/

#define BaseDelayCount  100         //LCD��ʱ����,�ڲ������Ļ�����Ŀ
/************************************************
�������� �� LCD_Delay
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
ע    �� : (_nop)12M->1us  24M->0.5us->500ns  48M->0.25us->250ns 
*************************************************/
void LCD_Delay(unsigned char Numcount)
{
    u8 i;
		u16 count =BaseDelayCount;
		for(i=0;i<Numcount;i++)
		{
		   count = BaseDelayCount;
			 while(count--);
		}
}
/************************************************
�������� �� LCD_DatGPIO_Init
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/ 
void LCD_DatGPIO_OUT(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);	 //ʱ��ʹ��	
   //PA0  PA1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //�������
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10|GPIO_Pin_11 |GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;	
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);	
}
void LCD_DatGPIO_IN(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);	 //ʱ��ʹ��	
   //PA0  PA1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            //��������
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10|GPIO_Pin_11 |GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;	
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);	
}

/************************************************
�������� �� LCD_E1E2
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/ 
void  LCD_E1E2(u8 E1E2) //ѡ��������2Ϊ������1Ϊ������0Ϊ����
{   
	  u8 temp=0x00;
    if(E1E2&0x0F)temp|=0x01;
    if(E1E2&0xF0)temp|=0x10;			
		switch(temp)
		{
				case 0x01  : LCD_E1 = 0; LCD_E2 = 1; break;  //ѡ������
				case 0x10  : LCD_E1 = 1; LCD_E2 = 0; break;  //ѡ������
				case 0x00  : LCD_E1 = 0; LCD_E2 = 0; break;  //ѡ��������				
				case 0x11  : LCD_E1 = 1; LCD_E2 = 1; break;  //������
		}
}
/************************************************
�������� �� LCD_ReadByte()
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
u8 LCD_ReadByte(void)
{
  u8 TempDat;
	LCD_DatGPIO_IN();  //�������ݹܽ�Ϊ����
	//E1=0 E2=0
	LCD_E1E2(0x11);
	LCD_A0 = 0;
	LCD_RW = 1;
	delay_us(20);	
	TempDat=LCD_RDat();

	LCD_DatGPIO_OUT();  //�������ݹܽ�Ϊ���  
	return TempDat;	
}
/************************************************
�������� �� LCD_Busy()
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_Busy(void)
{
		 u8 i;
    do{
		  i=LCD_ReadByte();
		}while((i&0x80)==0x80);        
}	
/************************************************
�������� �� LCD_WriteByte
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
#define LCD_CMD   0   //д����
#define LCD_DAT   1   //д����

void LCD_WriteByte(u8 Dat,u8 E1E2,u8 cmd )
{
      LCD_Busy();
			LCD_A0 = cmd;   //A0Ϊ1ʱ������,0ʱΪָ������		
      //E1=1 E2=1			
			LCD_E1E2(0x11) ;   //����ʹ�ܹ� E=1	
			LCD_WDat(Dat); 
	    delay_us(20);

			LCD_RW  = 0 ;   
			LCD_E1E2(E1E2); //ѡ����ʹ��	
	
	    delay_us(20) ;
	
			LCD_RW  = 1 ;	  //������������	
			LCD_E1E2(0x11) ;   //����ʹ�ܹ� E=1 
}


/************************************************
�������� �� LCD_SetPage
��    �� �� ����ҳ
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_SetPage(u8 Page)
{
  if(Page <= 3) 
   LCD_WriteByte(0XB8|Page,0x00,LCD_CMD);  //����ҳ
	else
	 LCD_WriteByte(0XB8,0x00,LCD_CMD);  //ҳΪ0
}
/************************************************
�������� �� LCD_Clear
��    �� �� �����ҳ ���� ȫ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
ע��:����ģʽ 
     1.cmd==0 ȫ����ģʽ
     2.cmd==1 ��˫�����ģʽ (���� 1 or 3  ;����2 or 4)
     3.cmd==2 �������ģʽ   (���� 1, 3, 5, 7 ;����2, 4, 6, 8)
     
*************************************************/
void  LCD_Clear(u8 Page,u8 cmd)
{
		u8 i,h,M,temp;
	 if(cmd==0)//ȫ����ģʽ
	 {
			for(h=0;h<4;h++)
			{
				M=h|0xb8;
				LCD_WriteByte(M      ,0x00,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,0x00,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
				{                
					LCD_WriteByte(0X00,0x00,LCD_DAT);  //��Ϊ0			
				}			
			}
	 }else if(cmd==1){//��˫�����ģʽ
		 
		 if(Page==1 || Page==2)
			 {
		    if(Page==1)temp=0x01;
			  else temp=0x10;
				LCD_WriteByte(0xB8   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00,temp,LCD_DAT);  //��Ϊ0			
			  }
				LCD_WriteByte(0xB9   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00,temp,LCD_DAT);  //��Ϊ0			
			  }			 
		 }else  {//if(Page==3 || Page==4)
		    if(Page==3)temp=0x01;
			  else temp=0x10;
				LCD_WriteByte(0xBA   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }
				LCD_WriteByte(0xBB   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }		 
		 }

	 }else{//�������ģʽ
		  if(Page==1||Page==2)
			{
			  if(Page==1)temp=0x01;
			  else temp=0x10;
				LCD_WriteByte(0xB8   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }			
			}else if (Page==3||Page==4){
			  if(Page==3)temp=0x01;
			  else temp=0x10;	
				LCD_WriteByte(0xB9   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }				
			}else if (Page==5||Page==6){
			  if(Page==5)temp=0x01;
			  else temp=0x10;					
				LCD_WriteByte(0xBA   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }				
			}else {//if (Page==7||Page==8)
			  if(Page==7)temp=0x01;
			  else temp=0x10;				
				LCD_WriteByte(0xBB   ,temp,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,temp,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
			  {                
			  	LCD_WriteByte(0X00 ,temp,LCD_DAT);  //��Ϊ0			
			  }					
			}
	 }
}
/************************************************
�������� �� LCD_DisAddr
��    �� �� x,y,Page,E1E2
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
//x(0-->14),Page ҳ ,E1E2 ������ ,����
void  LCD_DisAddr(u8 x,u8 Page,u8 E1E2, u8 *dat,u8 len)
{
		u8 i;
    if(Page>=4)Page=3;
		LCD_WriteByte(0xC0     ,E1E2,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
		LCD_WriteByte(Page|0xB0,E1E2,LCD_CMD); //���� ҳ��ַ(0)
		LCD_WriteByte(x        ,E1E2,LCD_CMD); //���� �е�ַ(0x00-->0x50(0-->80)) 
	 for(i=0;i<len;i++)
	 {
			LCD_WriteByte((u8)*dat++,E1E2,LCD_DAT);  //��Ϊ0
   }
}
/************************************************
�������� �� LCD_DisPageByte
��    �� �� ��������(Byte��д��(����ʾ���ֳ�(15X4�Ŀ�)))
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
//����xֵ(x(0-->14)) (Page(0-->3))
void LCD_DisPageByte(u8 x,u8 Page,u8 * dat)
{
	 u8 E1E2=0,x_temp,i;
	 u8 offset; //x��ƫ�Ƶ�ַ
	if(Page>=4)Page=3;      //��ֹҳ���
	if(x!=7)
	{
		if(x<=6){E1E2 = 0x01;offset=1;x_temp=x;}
		else    {if(x>=15)x=14; //��ֹX���
						 E1E2 = 0x10;offset=4;x_temp=x-8;}
		LCD_WriteByte(0xC0            ,E1E2,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
		LCD_WriteByte(Page|0xb8       ,E1E2,LCD_CMD); //���� ҳ��ַ(0)
		LCD_WriteByte(((x_temp*8)+offset) ,E1E2,LCD_CMD); //���� �е�ַ(0x00-->0x50(0-->80))
   for(i=0;i<8;i++)
		{
		  LCD_WriteByte(*dat++,E1E2,LCD_DAT);  //��Ϊ0		
		}
	}else {
		
		{E1E2 = 0x01;offset=1;x_temp=x;}
		LCD_WriteByte(0xC0                ,E1E2,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
		LCD_WriteByte(Page|0xb8           ,E1E2,LCD_CMD); //���� ҳ��ַ(0)
		LCD_WriteByte(((x_temp*8)+offset) ,E1E2,LCD_CMD); //���� �е�ַ(0x00-->0x50(0-->80)) 
   for(i=0;i<4;i++)
		{
		  LCD_WriteByte(*dat++,E1E2,LCD_DAT);  //��Ϊ0		
		}	
		{E1E2 = 0x10;}
		LCD_WriteByte(0xC0            ,E1E2,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
		LCD_WriteByte(Page|0xb8       ,E1E2,LCD_CMD); //���� ҳ��ַ(0)
		LCD_WriteByte(0 ,E1E2,LCD_CMD); //���� �е�ַ(0x00-->0x50(0-->80))	
   for(i=0;i<4;i++)
		{
		  LCD_WriteByte(*dat++,E1E2,LCD_DAT);  //��Ϊ0		
		}		
	}
}
/************************************************
�������� �� LCD_DisNumByte
��    �� �� ��ʾ����(16X8������)һ��ΪASCII��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisNumByte(u8 x,u8 y, u8 * dat)
{
	u8 Page=0;
		if(y==1) Page=0;       
		else     Page=2;       //������ʾ��ҳ��
		LCD_DisPageByte(x,Page  ,dat);
	  LCD_DisPageByte(x,Page+1,dat+8);
}
void LCD_DisNum(u8 x,u8 y,u32 Num)
{
	u8 x_temp=x;
	u32 temp;
   if(Num>=100000000)  //9λ
	 {
		 temp=Num%1000000000/100000000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;		 
	 }
	 if(Num>=10000000)
	 {
		 temp=Num%100000000/10000000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;		 
	 }		 
	 if(Num>=1000000)
	 {
		 temp=Num%10000000/1000000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);
     x_temp++;			 
	 }		 
	 if(Num>=100000) 
	 {
		 temp=Num%1000000/100000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;		 
	 }		 
	 if(Num>=10000) 
	 {
		 temp=Num%100000/10000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;			 
	 }		 
	 if(Num>=1000) 
	 {
		 temp=Num%10000/1000;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;		 
	 }		 
	 if(Num>=100) 
	 {
		 temp=Num%1000/100;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);	
     x_temp++;		 
	 }		 
	 if(Num>=10) 
	 {
		 temp=Num%100/10;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);
     x_temp++;			 
	 }		 
		 temp=Num%10;
		 LCD_DisNumByte(x_temp, y,(u8 *)table_Num[temp]);		 
}
/************************************************
�������� �� LCD_DisArray
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,*Array����ָ�� ,locate '_'��ʾλ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisArray(u8 x, u8 y ,u8 len ,u8 * Array,u8 locate)
{
	u8 i=0;
	for(i=0;i<len;i++)
	{
		LCD_DisNumByte(x+i, y,(u8 *)table_Num[Array[i]]);		
	}
	LCD_DisNumByte(x+locate, y,(u8 *)table_Num[11]);	         /*"_"*/
  if(locate<len)LCD_DisNumByte(x+len, y,(u8 *)table_Num[10]);/*" "*/		
	
}
/************************************************
�������� �� LCD_DisArray_No_
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,*Array����ָ�� 
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisArray_No_(u8 x, u8 y ,u8 len ,u8 * Array)
{
	u8 i=0;
	for(i=0;i<len;i++)
	{
		LCD_DisNumByte(x+i, y,(u8 *)table_Num[Array[i]]);		
	}
//	LCD_DisNumByte(x+locate, y,(u8 *)table_Num[11]);	         /*"_"*/
//  if(locate<len)LCD_DisNumByte(x+len, y,(u8 *)table_Num[10]);/*" "*/		
	
}
/************************************************
�������� �� LCD_DisNumLen
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,��ʾ����
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisNumLen(u8 x, u8 y ,u8 len ,u32 Num)
{
	u8 i=0;
	u32 Temp_Num=Num;
	u8 X;
	for(i=len;i>0;i--)
	{
		X=Temp_Num%10;
		LCD_DisNumByte(x+i-1, y,(u8 *)table_Num[X%10]);	
   	Temp_Num/=10;	
	}	
}
/************************************************
�������� �� LCD_DisArrayToNum
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,��ʾ����
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
u32 LCD_DisArrayToNum(u8 * Array,u8 len)
{
	u8 i=0;
	u32 TempDat=0;

	for(i=len;i>0;i--)
	{
		  if(( * Array)<=9)
			{
				TempDat*=10;
				TempDat+=(* Array);
			}else{
				return TempDat;
			}
			Array++;
	}
	return TempDat;	
}
/************************************************
�������� �� LCD_DisNumToArray
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,��ʾ����
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisNumToArray(u32 Num,u8 *Array,u8 len)
{
	u8 i=0;
	u32 TempDat=Num;
  Array	+=len-1;       
	for(i=len;i>0;i--)
	{
		(*Array)=TempDat%10;
		 TempDat/=10;
		 Array--;          //��ַ�ݼ�   
	}
}
/************************************************
�������� �� LCD_DisNumToArrayInit
��    �� �� ��ʾ����(16X8������)һ��ΪASCII
��    �� �� (x,y)��ʼλ�� ,len ���� ,��ʾ����
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
void LCD_DisArrayInit(u8 *Array,u8 len)
{
	u8 i=0;
  Array	+=len-1;       
	for(i=len;i>0;i--)
	{
		(*Array)=10;
		 Array--;          //��ַ�ݼ�   
	}
}
/************************************************
�������� �� LCD_DisGBByte
��    �� �� ��ʾ����(16X16������)һ��Ϊ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/
/*
 x   0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
    ------------------------------------------------------------------------------------------
 y1    |	 |	 |	 |	 |	 |	 |	 |	 |	 |	  |	   |	  |	   |	  |	   |	  |	   |	  |	   |			
    ------------------------------------------------------------------------------------------
 y2    |	 |   |	 |	 |	 |	 |	 |   |	 |	  |	   |	  |	   |	  |	   |	  |	   |	  |	   |				
    ------------------------------------------------------------------------------------------
       ��  |   ��  |   ��  |    �� |   ��  |     ��  |     ��  |     ��  |     ��  |     ��  |
    ------------------------------------------------------------------------------------------
*/
void LCD_DisGBByte(u8 x ,u8 y ,u8 *dat)
{
		u8 Page=0;
		if(y==1) Page=0;       
		else     Page=2;       //������ʾ��ҳ��
		LCD_DisPageByte(x  ,Page    ,dat   );
	  LCD_DisPageByte(x+1,Page    ,dat+8 );
		LCD_DisPageByte(x  ,Page+1  ,dat+16);
	  LCD_DisPageByte(x+1,Page+1  ,dat+24);	
}

//��ʾ�ִ�
//x,y(��ʼλ��) len��ʾ������
void LCD_DisGB(u8 x ,u8 y ,u8 *dat,u8 len)//��ʾ�ִ�
{
	u8 i;
	u8 len1 = sizeof(dat)/sizeof(*dat);  //(�����С/����Ԫ�ش�С)=�����Ԫ�صĸ���
		for(i=0;i<len;i++)
	 {
			if((i*32)==len1)return ;
			LCD_DisGBByte(x+i*2 ,y ,dat+i*32);

	 }
}


/************************************************
�������� �� LCD_GPIO_Init
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� wankic
*************************************************/


void  LCD_GPIO_Init(void)
{
		u8 i,h;
		GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);	 //ʱ��ʹ��	
   //PA0  PA1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 ;
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	 //PB1 PB2  PB6 (PB8--PB15)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10|GPIO_Pin_11 |GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;	
	  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5  ;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	  GPIO_SetBits(GPIOB,GPIO_Pin_0);						 //PB.5 �����
	  LCD_LED = 0;

	  LCD_RESET=0;	 
		delay_ms(50);		
	  LCD_RESET=1;
		delay_ms(10);	

		LCD_WriteByte(0xE2     ,0x00,LCD_CMD); //���� ��λ 
		LCD_WriteByte(0xAF     ,0x00,LCD_CMD); //���� ����ʾ	
		LCD_WriteByte(0xA4     ,0x00,LCD_CMD); //���� ������ʾ(��̬����)	
		LCD_WriteByte(0xA9     ,0x00,LCD_CMD); //���� ��ʾռ�ձ�(1/32DUTY)	
	  LCD_WriteByte(0xA0     ,0x00,LCD_CMD); //���� ��ʾ����(0xA0 ���� ;0xA1 ����)	
    LCD_WriteByte(0xEE     ,0x00,LCD_CMD); //дģʽ
		
		LCD_WriteByte(0xC0     ,0x00,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
		LCD_WriteByte(0xB8     ,0x00,LCD_CMD); //���� ҳ��ַ(0)
		LCD_WriteByte(0x00     ,0x00,LCD_CMD); //���� �е�ַ(0x00-->0x50(0-->80)) 
	  LCD_Clear(0,0);

while(0)
{
		for(h=0;h<4;h++)
		{
				LCD_WriteByte(0xC0   ,0x01,LCD_CMD); //���� ��ʼ��(0xc0)|(0-->31)
				LCD_WriteByte(h|0xb8 ,0x01,LCD_CMD); //���� ҳ��ַ(0)
				LCD_WriteByte(0x00   ,0x01,LCD_CMD); //���� �е�ַ(0x00-->0x50)
				for(i=0;i<80;i++)
				{
					LCD_WriteByte(0X00,0x01,LCD_DAT);  //��Ϊ0
				}			
		}
			
     LCD_Clear(0,0);
     LCD_LED=1;		
     delay_ms(500);
     LCD_LED=0;			
     delay_ms(500);	

    
//     LCD_DisGBByte(0 ,2,(u8 *)table_Ni[0]);
//     LCD_DisGBByte(2 ,2,(u8 *)table_Ni[0]);
//     LCD_DisGBByte(4 ,2,(u8 *)table_Ni[0]);
//     LCD_DisGBByte(6 ,2,(u8 *)table_Ni[0]);
//     LCD_DisGBByte(8 ,2,(u8 *)table_Ni[0]);
//     LCD_DisGBByte(10,2,(u8 *)table_Ni[0]);	
//     LCD_DisGBByte(12,2,(u8 *)table_Ni[0]);
//		LCD_DisNum(3,2,123456789);
		 LCD_DisGB(0,1,(u8*)tableGB_Start1[0],5);		
		 LCD_DisGB(0,2,(u8*)tableGB_Start1[1],7);
			delay_ms(500);
			delay_ms(500);	
			delay_ms(500);
			delay_ms(500);		
			delay_ms(500);                     
			delay_ms(500);
     LCD_Clear(0,0);
		 LCD_DisGB(0 ,1,(u8*)tableGB_menu[0],2);	//0-->3	
		 LCD_DisGB(7 ,1,(u8*)tableGB_menu[1],2); 
		 LCD_DisGB(0 ,2,(u8*)tableGB_menu[2],2);		
		 LCD_DisGB(7 ,2,(u8*)tableGB_menu[3],2); 
		 LCD_DisGB(4   ,1,(u8*)tableGB_menu[4],1);		 
		 LCD_DisGB(4+7 ,1,(u8*)tableGB_menu[4],1);
		 LCD_DisGB(4   ,2,(u8*)tableGB_menu[4],1);
		 LCD_DisGB(4+7 ,2,(u8*)tableGB_menu[4],1);		 
			delay_ms(500);                     
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);			
			delay_ms(500);
			delay_ms(500);	
			delay_ms(500);
			delay_ms(500);			
	delay_ms(500);
	delay_ms(500);	
	delay_ms(500);
	delay_ms(500);					
}
				
}
