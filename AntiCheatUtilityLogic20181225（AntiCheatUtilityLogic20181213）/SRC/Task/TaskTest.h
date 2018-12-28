/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskTest.h
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20140324
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

#ifndef _TASKTEST_H_
#define _TASKTEST_H_

#include "Config.h"
#include "FPGA_APP.h"
#include "TDC256.h"
#include "PCF8563.h"
#include "SdApp.h"


extern	void TaskTest(void);
extern 	OS_STK	TaskTestStk[TASK_TEST_STACKSIZE];


#endif
