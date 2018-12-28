/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JDProcess.h
** Last modified Date:  20131108
** Last Version:		1.0
** Descriptions:		交调信息处理任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wgh
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

#ifndef __TASK_JDPRO_H
#define __TASK_JDPRO_H

#include "config.h"
#include "common.h"
#include "PCF8563.h"
//#include "SdApp.h"

#ifdef	__TASK_JDPRO_C
#define		TJD_EXT
#else
#define		TJD_EXT	extern	
#endif

TJD_EXT void    FJDProcess(Vehicle_Struct *pItem);
TJD_EXT uint8  FJDVehType(uint8 pLane, uint8 pAxleNum, uint16 pTotalDis, uint16 pVehLength);
TJD_EXT uint8  FJDJudgeVehType(uint8 pLane, uint8 pCarType);
TJD_EXT uint8  FJDInfToSaveSend(uint16 pTimeNum);
TJD_EXT uint8  FJD61ToSaveSend(uint16 pTimeNum);
TJD_EXT uint8  FJD62ToSaveSend(uint16 pTimeNum);
TJD_EXT uint8  FJD68ToSaveSend(uint16 pTimeNum);
TJD_EXT void   FTime_Process(SystemTime* psTime);
TJD_EXT uint8  FChangLaneToBG(uint8 pLane);
TJD_EXT uint32 FGetSaveNumFromDate(SystemTime* psTime, uint16 pTimeNum);
TJD_EXT uint32 FGetPreYearNum(uint16 pYear);
TJD_EXT uint32 FGetLastSaveNum(uint32 pSaveNum, uint8 pPeriod);
TJD_EXT uint8* FGetDateFromNum(uint32 pSaveNum);
TJD_EXT void   FSave01ToTDC(uint8 pCMD);
TJD_EXT void   FRead01FromTDC(uint8 pCMD);
TJD_EXT void   FSend01Info(uint32 pSaveID, uint32 pSendID);
TJD_EXT void   FSend6FInfo(uint8 pCmdType, uint32 pSaveID, uint32 pSendID);
TJD_EXT uint32 FGetDifference(uint32 pNum1, uint32 pNum2);
TJD_EXT uint32 FJudgeDifference(uint32 pNum, uint32 pThr);
TJD_EXT uint32 FGetNextSDNum(uint32 pNum, uint8 pPeriod);
TJD_EXT uint8  FCheckSendData(uint8* pBuf, uint8 pCMD);
TJD_EXT uint16 FGetSendDataForErr(uint8 pCMD, uint8* pDataBuf, uint8* pTimeBuf, uint8 pErrFlag);
TJD_EXT uint8  FCheckDeviceID(uint8* pBuf);
TJD_EXT void   FSave39ToSD(Vehicle_Struct *pdata);
TJD_EXT void   FSave39ToTDC(uint8 pCMD);
TJD_EXT void   FRead39FromTDC(uint8 pCMD);
TJD_EXT void   FSend39Info(uint32 pSaveID, uint32 pSendID);
TJD_EXT uint16 FGet39OrderForErr(uint8* pDataBuf,uint32 pSendNum,uint8 pErrFlag);
TJD_EXT void   FSave61ToTDC(uint8 pCMD);
TJD_EXT void   FSave62ToTDC(uint8 pCMD);
TJD_EXT void   FSave68ToTDC(uint8 pCMD);
TJD_EXT void   FRead61FromTDC(uint8 pCMD);
TJD_EXT void   FRead62FromTDC(uint8 pCMD);
TJD_EXT void   FRead68FromTDC(uint8 pCMD);
#endif
