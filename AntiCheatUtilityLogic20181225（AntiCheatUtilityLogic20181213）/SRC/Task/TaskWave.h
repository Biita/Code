/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskWave.h
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
#ifndef	__TASKWAVE_H
#define	__TASKWAVE_H
			 




#ifdef	__TASKWAVE_C
#define	TWV_EXT	  
#else
#define	TWV_EXT	extern
#endif

#include "Config.h"
#include "Uart2.h" 
#include "w5300app.h"
#include "Common.h"	
#define	UARTSENDDATA(a,b)	U2SendBytes(a,b)  


TWV_EXT	OS_STK	TaskRecWaveStk[TASK_WAVE_STACKSIZE];	//发波形任务
TWV_EXT	void	TaskRecWave(void *pdata);




#endif
