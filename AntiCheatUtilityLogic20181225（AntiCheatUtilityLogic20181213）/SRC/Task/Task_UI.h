/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Task_UI.h
** Last modified Date:  20150715
** Last Version:		1.0
** Descriptions:		界面显示，用于液晶显示
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wangfeng
** Created date:		20150715
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

#ifndef	__TASK_UI_H
#define	__TASK_UI_H

#ifdef	_TASK_UI_C
#define	TUI_EXT	
#else
#define	TUI_EXT	extern
#endif

#include "Config.h"
#include "LCDApp.h"

#include "WDT.h"
#include "PCF8563.h"  
#include "DS18B20.h"
#include "w5300.h"

//#include "common.h"
		

TUI_EXT	OS_STK	Task_UIStk[TASK_UI_STACKSIZE];

TUI_EXT	void	Task_UI(void *tdata);
TUI_EXT	uint8 	DoKey_ESC(void);
TUI_EXT	uint8 	DoKey_ENTER(void);
TUI_EXT	uint8 	DoKey_F1(void);
TUI_EXT	uint8 	DoKey_F2(void);
TUI_EXT	uint8 	DoKey_F3(void);
TUI_EXT uint8   DoKey_F4(void);	   // rp
TUI_EXT uint8   DoKey_F11(void);
TUI_EXT uint8   DoKey_F12(void);
TUI_EXT	uint8 	DoKey_0(void);
TUI_EXT	uint8 	DoKey_1(void);
TUI_EXT	uint8 	DoKey_2(void);
TUI_EXT	uint8 	DoKey_3(void);
TUI_EXT	uint8 	DoKey_4(void);
TUI_EXT	uint8 	DoKey_5(void);
TUI_EXT	uint8 	DoKey_6(void);
TUI_EXT	uint8 	DoKey_7(void);
TUI_EXT	uint8 	DoKey_8(void);
TUI_EXT	uint8 	DoKey_9(void);
//TUI_EXT	uint8 	DoKey_1(void);
//TUI_EXT	uint8 	DoKey_1(void);
//TUI_EXT	uint8 	DoKey_1(void);
//TUI_EXT	uint8 	DoKey_1(void);

#endif
