

/**
  **********************************  STM32F103C8  ***********************************
  * @文件名     ： LCD12232.h
  * @作者       ： wenkic
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年12月12日
  * @摘要       ： UART头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _LCD12232_H_
#define _LCD12232_H_

/* 包含的头文件 --------------------------------------------------------------*/
#include "sys.h"  //位带操作


/* 宏定义 --------------------------------------------------------------------*/
#define LCD_E1      PAout(0)   													////// (块选择1左边)		  
#define LCD_E2      PAout(1)                            ////// (块选择2右边)	
#define LCD_A0      PBout(2)                            //////  A0为1时是数据,0时为指令数据
#define LCD_RW      PBout(5)                            //////  读写使能端
#define LCD_RESET   PBout(1)                            ////// 复位端	
#define LCD_LED     PBout(0)                            ////
#define LCD_WDat(X) GPIO_Write(GPIOB,(u16)((u16)(X<<8)|(u8)GPIO_ReadOutputData(GPIOB)))       //////
#define LCD_RDat() (u8)(GPIO_ReadInputData(GPIOB)>>8)  //////





/* 变量申明 ------------------------------------------------------------------*/
//显示1个数
void LCD_DisNumByte(u8 x,u8 y, u8 * dat);
//显示一个数字
void LCD_DisNum(u8 x,u8 y,u32 Num);
//显示一个数字(定长度)不够前面补零
void LCD_DisNumLen(u8 x, u8 y ,u8 len ,u32 Num);
//将数组转换成数字
u32 LCD_DisArrayToNum(u8 * Array,u8 len);
//将数字转换成数组中,限制长度,前面不够就不零(为显示公式准备)
void LCD_DisNumToArray(u32 Num,u8 *Array,u8 len);
//显示一个数组数字(带上"_")
void LCD_DisArray(u8 x, u8 y ,u8 len ,u8 * Array,u8 locate);
//显示一个数组数字(不带上"_")
void LCD_DisArray_No_(u8 x, u8 y ,u8 len ,u8 * Array);
//显示一个数组的初始化
void LCD_DisArrayInit(u8 *Array,u8 len);
//显示16 X 16的汉字
void LCD_DisGBByte(u8 x ,u8 y ,u8 *dat);
//x,y(起始位置) len显示多少字
void LCD_DisGB(u8 x ,u8 y ,u8 *dat,u8 len);//显示字串



/* 函数申明 ------------------------------------------------------------------*/
void  LCD_GPIO_Init(void);  //LCD初始化
void  LCD_Clear(u8 Page,u8 cmd); //清除屏幕

#endif /* _LCD12232.h_ */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/


