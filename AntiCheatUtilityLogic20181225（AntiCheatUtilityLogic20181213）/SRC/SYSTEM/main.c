/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			main.c
** Last modified Date:  2011-03-31
** Last Version:		1.0
** Descriptions:		主函数文件
**
** 编译选项宏定义说明：
**	C/C++:
**		USEMODE:	用户模式，即正常使用。在ESAM模块中，可以校验ESAM，读EEP。
**		PUBMODE：	发布模式，即发布程序使用。在ESAM模块中，可以写CSK，写EEP。
**		UPDATEMODE：更新模式，即用于刷程序。在ESAM模块中，可以读SN，读EEP，写EEP	
**		LCD：		使用标准LCD开发板，加载标准LCD驱动及应用
** 		LCDNEW：	新LCD，加载3250开发板上的显示屏驱动及应用
**		__STRG_NAND:	NANDFlash存储器件，加载NAND相关驱动及应用
**		__STRG_NOR:	NORFlash存储器件，加载NOR相关驱动及应用
**		__STRG_SD：	SD卡器件，加载SD卡相关驱动和应用
**		__UCOS：	UCOS模式，写部分Flash时会关中断
**
**	ASM:
**		__FLASHMODE：	内存模式，从Flash中加载程序，因此程序不再打开缓存。
**						调试状态时，删除该宏定义。
**
**
**--------------------------------------------------------------------------------------------------------
** Created by:		
** Created date:		
** Version:			
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define  EXT_GLOBAL

#include "common.h"

 int main(void)
 {
//#if PROC_DOG_EN > 0	 	 	 
	FeedExternDog();		//喂外部狗
//#endif
	
	OSInit();                   	//初始化 uC/OS-II
	FeedExternDog();		//喂外部狗
	JZInit();	
	FeedExternDog();		//喂外部狗
	RunStartTask();					//开始启动任务 
	
  OSStart();                 		//开始多任务

	return(1);
 }
