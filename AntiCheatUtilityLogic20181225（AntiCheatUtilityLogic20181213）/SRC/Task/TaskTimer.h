/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskTimer.h
** Last modified Date:  20110512
** Last Version:		1.0
** Descriptions:		º¸≈Ã»ŒŒÒ
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110512
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/

#ifndef	__TASKTIMER_H
#define	__TASKTIMER_H

#ifdef	__TASKTIMER_C
#define	TMR_EXT	
#else
#define	TMR_EXT	extern
#endif

#include "Config.h"
#include "LCDApp.h"

#include "WDT.h"
#include "PCF8563.h"  
#include "DS18B20.h"
#include "w5300.h"

#include "common.h"		//wujinlong 20121227

TMR_EXT	OS_STK	TaskRec2Stk[TASK_TIME_STACKSIZE];

TMR_EXT	void	TaskRec2(void *pdata);

TMR_EXT	uint32 FNetKeepalive(void);
TMR_EXT	uint32 FNetSendKeepalive( SOCKET l_sk );			
TMR_EXT	uint32 FNetCheckClose(void);

#endif
