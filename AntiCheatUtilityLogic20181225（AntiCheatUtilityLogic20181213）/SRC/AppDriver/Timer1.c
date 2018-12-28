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
** Descriptions:		��ʱ��0��غ���
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
**   Time1��ʼ������
**
****************************************************************************************/
void Time1Init(void)
{
	//MIC_ER			|= (1<<16);									//ʹ��Timer1�ж�
	TIMCLK_CTRL1	= 0x0C;  //0x0C                           		/* ʹ�ܶ�ʱ��0��ʱ��            */
	T1TCR			= 0x02;                           			/* ��λ�����ܶ�ʱ��0            */
	T1IR			= 0xFF;                           			/* ��������ж�                 */ 
	T1TC			= 0x00000000;								/* ��ʱ������Ϊ0                */										
	T1PR			= 0x00000081;						 		/* ʱ��16��Ƶ                   */											
	T1PC			= 0x00000000;
	T1MCR			= 0x0000;						       		/* ����T0MR0ƥ���λT0TC��    */
	                                                    		/* �������жϱ�־               */
	T1CTCR			= 0x00;                                                                    				
	//T1MR0			= Fpclk / (16*OS_TICKS_PER_SEC);			/* 1���Ӷ�ʱ                    */
	T1TCR			= 0x00;						   				/* �رն�ʱ��0                  */										
    
//	micIrqFuncSet(17, 0, (unsigned int)IRQ_Timer1);				/* �����ж�������				*/
}
