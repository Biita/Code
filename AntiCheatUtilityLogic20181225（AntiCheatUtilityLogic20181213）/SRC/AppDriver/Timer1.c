/*#define     TIMER1_BASE_ADDR         0x4004C000
#define     T1IR                    (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x000))
#define     T1TCR                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x004))
#define     T1TC                    (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x008))
#define     T1PR                    (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x00C))
#define     T1PC                    (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x010))
#define     T1MCR                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x014))
#define     T1MR0                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x018))
#define     T1MR1                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x01C))
#define     T1MR2                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x020))
#define     T1MR3                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x024))
#define     T1CCR                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x028))
#define     T1CR0                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x02C))
#define     T1CR1                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x030))
#define     T1CR2                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x034))
#define     T1CR3                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x038))
#define     T1EMR                   (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x03C))
#define     T1CTCR                  (*(volatile unsigned long *)(TIMER1_BASE_ADDR + 0x070))*/

/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Timer0.c
** Last modified Date:  2011-04-12
** Last Version:		1.0
** Descriptions:		计时器0相关函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		2011-04-12
** Version:				1.0
** Descriptions:		Timer0
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__TIMER1_C
#include "Timer1.h"



/***************************************************************************************
**
**   Time1初始化函数
**
****************************************************************************************/
void Time1Init(void)
{
	//MIC_ER			|= (1<<16);									//使能Timer1中断
	TIMCLK_CTRL1	= 0x0C;  //0x0C                           		/* 使能定时器0的时钟            */
	T1TCR			= 0x02;                           			/* 复位并禁能定时器0            */
	T1IR			= 0xFF;                           			/* 清除所有中断                 */ 
	T1TC			= 0x00000000;								/* 定时器设置为0                */										
	T1PR			= 0x00000081;						 		/* 时钟16分频                   */											
	T1PC			= 0x00000000;
	T1MCR			= 0x0000;						       		/* 设置T0MR0匹配后复位T0TC，    */
	                                                    		/* 并产生中断标志               */
	T1CTCR			= 0x00;                                                                    				
	//T1MR0			= Fpclk / (16*OS_TICKS_PER_SEC);			/* 1秒钟定时                    */
	T1TCR			= 0x00;						   				/* 关闭定时器0                  */										
    
//	micIrqFuncSet(17, 0, (unsigned int)IRQ_Timer1);				/* 加入中断向量表				*/
}
