/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Timer1.h
** Last modified Date:  2013-08-12
** Last Version:		1.0
** Descriptions:		Timer1初始化、中断处理
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__TIMER1_H
#define	__TIMER1_H

#include "config.h"

#ifdef	__TIMER1_C
#define	T1_EXT
#include "WDT.h"
#else
#define	T1_EXT	extern
#endif

//初始化
T1_EXT	void Time1Init(void);

#endif		//__TIMER0_H
