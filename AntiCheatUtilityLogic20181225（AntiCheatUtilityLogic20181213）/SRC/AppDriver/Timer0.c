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
#define	__TIMER0_C
#include "Timer0.h"

#define		SETUPALIAS				g_Setup			//设置参数结构
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

static	void IRQ_Timer0(void);		//中断处理
static  uint16 s_TimeCha;

/*********************************************************************************************************
** 函数名称:  IRQ_Timer0
** 函数功能:  定时器0中断服务程序。
** 入口参数:  无
** 出口参数:  无
** 函数说明:
*********************************************************************************************************/
void IRQ_Timer0(void)
{ 	
	T0IR			= 0x01;					                /* 清除中断标志	                */
//#if PROC_DOG_EN > 0
//	WDTIM_COUNTER	= 1;									/* 喂狗							*/
//	FeedExternDog();			//外部狗
//#endif
	if(PROMODPARAM.m_u8EnableFlag[2] != 0)
	{
//		if(g_u8BegTimeRead)
//			GetRTCTime(&g_DevCurTime);
		if(g_u8JDInitFlag == 1 && g_DevCurTime.u8Second < 2)
		{
			g_u16TotalMinutesNew = g_DevCurTime.u8Hour * 60 + g_DevCurTime.u8Minute;
			if(g_u16TotalMinutesNew < g_u16TotalMinutesOld)
			{
				s_TimeCha = g_u16TotalMinutesNew + 1440 - g_u16TotalMinutesOld;
			}
			else
			{
				s_TimeCha = g_u16TotalMinutesNew - g_u16TotalMinutesOld;
			}
			if(((g_u16TotalMinutesNew%g_u8PeriodTOSave) == 0)&&(s_TimeCha>= 1))
			{
				if(g_u16TotalMinutesNew == 0)
				{
				 	g_u16SaveTimeNum = 1440/g_u8PeriodTOSave;
				}
				else
				{
				  	g_u16SaveTimeNum = g_u16TotalMinutesNew/g_u8PeriodTOSave;
				}
				if(g_u16SaveTimeNum > 1440)
				{
					g_u16SaveTimeNum = 0;
				}
				g_u16TotalMinutesOld = g_u16TotalMinutesNew;
			
				OSSemPost(g_pSDSaveForJD);
			}
		}
	}
	OSTimeTick();	   
}

/***************************************************************************************
**
**   Time0初始化函数
**
****************************************************************************************/
void Time0Init(void)
{
	MIC_ER			|= (1<<16);									//使能Timer0中断
	TIMCLK_CTRL1	= 0x04;                             		/* 使能定时器0的时钟            */
	T0TCR			= 0x02;                           			/* 复位并禁能定时器0            */
	T0IR			= 0xFF;                           			/* 清除所有中断                 */ 
	T0TC			= 0x00000000;								/* 定时器设置为0                */										
	T0PR			= 0x0000000F;						 		/* 时钟16分频                   */											
	T0PC			= 0x00000000;
	T0MCR			= 0x0003;						       		/* 设置T0MR0匹配后复位T0TC，    */
	                                                    		/* 并产生中断标志               */
	T0CTCR			= 0x00;                                                                    				
	T0MR0			= Fpclk / (16*OS_TICKS_PER_SEC);			/* 1秒钟定时 5ms定时            */
	T0TCR			= 0x01;						   				/* 启动定时器0                  */										
    
	micIrqFuncSet(16, 0, (unsigned int)IRQ_Timer0);				/* 加入中断向量表				*/
}

