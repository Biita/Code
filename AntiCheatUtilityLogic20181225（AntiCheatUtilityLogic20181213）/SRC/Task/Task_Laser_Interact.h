/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Task_Laser_Interact.h
** Last modified Date:  20130719
** Last Version:		1.0
** Descriptions:		�����뼤��ģ�齻������
**
**--------------------------------------------------------------------------------------------------------
** Created by:			YANG MING
** Created date:		20130719
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

#ifndef	__TASK_LASER_INTERACT_H
#define	__TASK_LASER_INTERACT_H
		
#include "config.h"
#include "common.h"
#include "AdjustMacro.h"

//#define ARM_LASER_TEST  //ARM�ͼ���ģ������ʱ�Ĳ��Դ��� 

#ifdef	__TASK_LASER_INTERACT_C
#define	TLI_EXT		
#else
#define	TLI_EXT	extern
#endif

TLI_EXT	OS_STK	Task_Laser_InteractStk[TASK_LASERINTR_STACKSIZE];	//���⽻������
TLI_EXT	void	Task_Laser_Interact(void *pdata);

#endif

