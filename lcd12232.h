

/**
  **********************************  STM32F103C8  ***********************************
  * @�ļ���     �� LCD12232.h
  * @����       �� wenkic
  * @��汾     �� V2.2.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��12��12��
  * @ժҪ       �� UARTͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _LCD12232_H_
#define _LCD12232_H_

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "sys.h"  //λ������


/* �궨�� --------------------------------------------------------------------*/
#define LCD_E1      PAout(0)   													////// (��ѡ��1���)		  
#define LCD_E2      PAout(1)                            ////// (��ѡ��2�ұ�)	
#define LCD_A0      PBout(2)                            //////  A0Ϊ1ʱ������,0ʱΪָ������
#define LCD_RW      PBout(5)                            //////  ��дʹ�ܶ�
#define LCD_RESET   PBout(1)                            ////// ��λ��	
#define LCD_LED     PBout(0)                            ////
#define LCD_WDat(X) GPIO_Write(GPIOB,(u16)((u16)(X<<8)|(u8)GPIO_ReadOutputData(GPIOB)))       //////
#define LCD_RDat() (u8)(GPIO_ReadInputData(GPIOB)>>8)  //////





/* �������� ------------------------------------------------------------------*/
//��ʾ1����
void LCD_DisNumByte(u8 x,u8 y, u8 * dat);
//��ʾһ������
void LCD_DisNum(u8 x,u8 y,u32 Num);
//��ʾһ������(������)����ǰ�油��
void LCD_DisNumLen(u8 x, u8 y ,u8 len ,u32 Num);
//������ת��������
u32 LCD_DisArrayToNum(u8 * Array,u8 len);
//������ת����������,���Ƴ���,ǰ�治���Ͳ���(Ϊ��ʾ��ʽ׼��)
void LCD_DisNumToArray(u32 Num,u8 *Array,u8 len);
//��ʾһ����������(����"_")
void LCD_DisArray(u8 x, u8 y ,u8 len ,u8 * Array,u8 locate);
//��ʾһ����������(������"_")
void LCD_DisArray_No_(u8 x, u8 y ,u8 len ,u8 * Array);
//��ʾһ������ĳ�ʼ��
void LCD_DisArrayInit(u8 *Array,u8 len);
//��ʾ16 X 16�ĺ���
void LCD_DisGBByte(u8 x ,u8 y ,u8 *dat);
//x,y(��ʼλ��) len��ʾ������
void LCD_DisGB(u8 x ,u8 y ,u8 *dat,u8 len);//��ʾ�ִ�



/* �������� ------------------------------------------------------------------*/
void  LCD_GPIO_Init(void);  //LCD��ʼ��
void  LCD_Clear(u8 Page,u8 cmd); //�����Ļ

#endif /* _LCD12232.h_ */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/


