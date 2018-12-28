/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskProtocol.h
** Last modified Date:  20110523
** Last Version:		1.0
** Descriptions:		��λ��ͨ������						
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110523
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
#ifndef	__TASKPROTOCOL_H

#include "common.h"

#ifdef	__TASKPROTOCOL_C
#define	TPT_EXT	 
#else
#define	TPT_EXT	extern
#endif
 
	


TPT_EXT		void	TaskRec7(void *pdata);
TPT_EXT 	OS_STK	TaskRec7Stk[TASK_COMM_STACKSIZE];		//����0��������

TPT_EXT		void	CommondToPC(uint8 p_u8VehIndex,uint8 p_u8CommandNO);	//����λ���������� 

TPT_EXT		void  	ToProtocolBuff(Vehicle_Struct *pdata);									//���͹�������
TPT_EXT		void 	FSendCmd2PC_ZONG(uint8 p_u8VehIndex,uint8 p_u8CommandNO);
TPT_EXT		void 	FProcessCmd_ZONG(uint8 *pData);
TPT_EXT		void	CommondToPC(uint8 p_u8VehIndex,uint8 p_u8CommandNO);	//����λ����������

#endif		//#ifndef	__TASKPROTOCOL_H
