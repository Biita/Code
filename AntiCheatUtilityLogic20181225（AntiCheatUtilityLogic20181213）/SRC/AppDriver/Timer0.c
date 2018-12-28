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
#define	__TIMER0_C
#include "Timer0.h"

#define		SETUPALIAS				g_Setup			//���ò����ṹ
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

static	void IRQ_Timer0(void);		//�жϴ���
static  uint16 s_TimeCha;

/*********************************************************************************************************
** ��������:  IRQ_Timer0
** ��������:  ��ʱ��0�жϷ������
** ��ڲ���:  ��
** ���ڲ���:  ��
** ����˵��:
*********************************************************************************************************/
void IRQ_Timer0(void)
{ 	
	T0IR			= 0x01;					                /* ����жϱ�־	                */
//#if PROC_DOG_EN > 0
//	WDTIM_COUNTER	= 1;									/* ι��							*/
//	FeedExternDog();			//�ⲿ��
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
**   Time0��ʼ������
**
****************************************************************************************/
void Time0Init(void)
{
	MIC_ER			|= (1<<16);									//ʹ��Timer0�ж�
	TIMCLK_CTRL1	= 0x04;                             		/* ʹ�ܶ�ʱ��0��ʱ��            */
	T0TCR			= 0x02;                           			/* ��λ�����ܶ�ʱ��0            */
	T0IR			= 0xFF;                           			/* ��������ж�                 */ 
	T0TC			= 0x00000000;								/* ��ʱ������Ϊ0                */										
	T0PR			= 0x0000000F;						 		/* ʱ��16��Ƶ                   */											
	T0PC			= 0x00000000;
	T0MCR			= 0x0003;						       		/* ����T0MR0ƥ���λT0TC��    */
	                                                    		/* �������жϱ�־               */
	T0CTCR			= 0x00;                                                                    				
	T0MR0			= Fpclk / (16*OS_TICKS_PER_SEC);			/* 1���Ӷ�ʱ 5ms��ʱ            */
	T0TCR			= 0x01;						   				/* ������ʱ��0                  */										
    
	micIrqFuncSet(16, 0, (unsigned int)IRQ_Timer0);				/* �����ж�������				*/
}

