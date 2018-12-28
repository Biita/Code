/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskDebugInfoOutput.h
** Last modified Date:  20130812
** Last Version:		1.0
** Descriptions:		������Ϣ�������
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20130812
** Version:				1.0
** Descriptions:		������Ϣͨ���������
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


/** ����������� **/
// ����
TDO_EXT	OS_STK	TaskDebugInfoStk[TASK_DEBUGINFO_STACKSIZE];



// ����
TDO_EXT	void	TaskDebugInfo(void *pdata);


/** ����������� **/

#endif	 // #ifndef __TASK_DEBUGINFO_OUTPUT_H
#endif	 // #if DEBUGINFO_TASK_SP_EN > 0
