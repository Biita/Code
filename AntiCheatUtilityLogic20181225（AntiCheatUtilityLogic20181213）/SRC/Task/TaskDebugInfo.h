/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskDebugInfoOutput.h
** Last modified Date:  20130812
** Last Version:		1.0
** Descriptions:		调试信息输出任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20130812
** Version:				1.0
** Descriptions:		调试信息通过串口输出
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:			
** Descriptions:	
**
*********************************************************************************************************/
#if DEBUGINFO_TASK_SP_EN > 0

#ifndef __TASK_DEBUGINFO_OUTPUT_H
#define __TASK_DEBUGINFO_OUTPUT_H

#include "config.h"
#include "common.h"

#ifdef	__TASK_DEBUGINFO_OUTPUT_C
#define		TDO_EXT
#else
#define		TDO_EXT	extern	
#endif


/** 以下添加内容 **/
// 变量
TDO_EXT	OS_STK	TaskDebugInfoStk[TASK_DEBUGINFO_STACKSIZE];



// 函数
TDO_EXT	void	TaskDebugInfo(void *pdata);


/** 以上添加内容 **/

#endif	 // #ifndef __TASK_DEBUGINFO_OUTPUT_H
#endif	 // #if DEBUGINFO_TASK_SP_EN > 0
