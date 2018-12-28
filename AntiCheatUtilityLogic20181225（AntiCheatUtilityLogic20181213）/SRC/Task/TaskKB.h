/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskKB.h
** Last modified Date:  20110511
** Last Version:		1.0
** Descriptions:		键盘任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110511
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

#ifndef	__TASKKB_H
#define	__TASKKB_H
		
#include "Config.h"
#include "LCDApp.h"
//#if	YBVERSION >= 30		//3.0仪表功能
#include "Keyboard.h"
//#endif

#ifdef	__TASKKB_C
#define	TKB_EXT		
#else
#define	TKB_EXT	extern
#endif

#include "KBMacro.h"

TKB_EXT	OS_STK	TaskRec3Stk[TASK_KEY_STACKSIZE];	//键盘任务
TKB_EXT	void	TaskRec3(void *pdata);


#endif
