/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskSDCard.h
** Last modified Date:  20131108
** Last Version:		1.0
** Descriptions:		SD任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20131108
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

#ifndef __TASK_SDCARD_H
#define __TASK_SDCARD_H

#include "config.h"
#include "common.h"
#include "SdApp.h"

#ifdef	__TASK_SDCARD_C
#define		TSD_EXT
#else
#define		TSD_EXT	extern	
#endif


/** 以下添加内容 **/
// 宏
//#define	SD_GETNEXTVEH_SECTOR(a,b)	(SD_OFFNET_SEC_BGN+SD_VEH_SECTOR_NUM*a+SD_SEC_NUM_FRAM*b)	 // 下一车序号，帧序号
//#define	SD_GETNEXTLOG_SECTOR(a)	 	(SD_DBGINFO_SEC_BGN+a)										 // 下一日志序号



#define	SD_GETNEXTVEH_SECTOR(a,b,c)	(SD_OFFNET_SEC_DATA_BGN+SD_VEH_SECTOR_NUM*(a*SD_IX_NUM+b)+SD_SEC_NUM_FRAM*c)	 // 下一车索引扇区，扇区子序号，帧序号
#define	SD_GETNEXT_SECTOR(a,b)	 	(a+b)										 // 下一序号
#define	SD_GETNEXTWAVE_SECTOR(a,b)	 	(a+b*SD_SEC_NUM_FRAM)										 // 下一波形


// 变量
TSD_EXT	OS_STK	TaskSDCardStk[TASK_SD_STACKSIZE];



// 函数
TSD_EXT	void	TaskSDCard(void *pdata);
TSD_EXT void IRQ_SD( void );
//
TSD_EXT uint32 FSDNewCardInit( void );
TSD_EXT uint32 FSDCptAddr( uint8*buf, int len );
//
TSD_EXT uint32 FSysnVehIx( void );
TSD_EXT uint32 FSysnLogIx( void );
TSD_EXT uint32 FSysnWaveIx( void );
TSD_EXT uint32 FSysnJDIx(void);

//

TSD_EXT	uint32 FCheckSD( uint8 *p_pu8Data, uint32 p_u32Len );
//
TSD_EXT	uint32 FSDFormOffnetCtx( uint8 *p_pu8Des, CSDOffnetIx*	p_cOffnetIx );
TSD_EXT	uint32 FSDFormOffnetBuf( uint8 *p_pu8Des1, uint8 *p_pu8Des2, uint8 *p_pu8FrameData, uint32 p_u32Len );
TSD_EXT uint32 FSDFormVehCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur );
TSD_EXT	uint32 FSDFormVehBuf( uint8 *p_pu8Des1, uint8 *p_pu8FrameData, uint32 p_u32Len );
TSD_EXT uint32 FSDFormLogCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur );
TSD_EXT uint32 FSDFormLogBuf( uint8 *p_pu8Des1, uint8 *p_pu8FrameData, uint32 p_u32Len );
TSD_EXT uint32 FSDFormWaveCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur );
TSD_EXT uint32 FSDFormWaveBuf( uint8 *p_pu8Des1, uint8 *p_pu8Des2, uint8 *p_pu8FrameData, uint32 p_u32Len );
TSD_EXT uint32 FSDFormJDCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur );

TSD_EXT	uint32 FSDCheckAntiShake( sd_struct *sds );
/** 以上添加内容 **/

#endif	 // #ifndef __TASK_SDCARD_H

