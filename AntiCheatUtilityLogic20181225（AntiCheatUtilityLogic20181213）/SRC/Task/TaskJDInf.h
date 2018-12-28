/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskTJDINF.h
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

#ifndef	__TASKJDINF_H
#define	__TASKJDINF_H

#ifdef	__TASKJDINF_C
#define	TJDI_EXT	
#else
#define	TJDI_EXT	extern
#endif

#include "Config.h"
#include "LCDApp.h"

#include "WDT.h"
#include "PCF8563.h"  
#include "DS18B20.h"
#include "w5300.h"

#include "common.h"		//wujinlong 20121227

TJDI_EXT	OS_STK	TaskRec9Stk[TASK_JDINF_STACKSIZE];

TJDI_EXT	void	TaskRec9(void *pdata);


#endif
