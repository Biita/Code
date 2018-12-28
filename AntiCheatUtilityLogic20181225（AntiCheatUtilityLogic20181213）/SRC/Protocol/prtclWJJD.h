/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			prtclWJJD.h
** Last modified Date:  20121031
** Last Version:		1.0
** Descriptions:		非现场执法协议
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wgh
** Created date:		20121031
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------	
*********************************************************************************************************/
#ifndef __PRTCLWJJD_H
#define __PRTCLWJJD_H
				
#include "config.h"

#ifdef  __PRTCLWJJD_C
#define	PRTCLWJJD_EXT	 

#else
#define	PRTCLWJJD_EXT	extern
#endif

PRTCLWJJD_EXT	void WJJD_ProcessCmd(uint8 *pData);
PRTCLWJJD_EXT	void WJJDSendCommandToPC0(uint8 p_u8VehIndex,uint8 p_u8CommandNO);
PRTCLWJJD_EXT   uint16  FCompleteData(uint8 *pBuf, uint16 pLen);
PRTCLWJJD_EXT   uint8   FSendData(uint8 *pBuf, uint16 pLen);
PRTCLWJJD_EXT   void   FUpOrderData(uint8* pBuf, uint8 pCMD); 
PRTCLWJJD_EXT   void   FAnalyResendData(uint8* pBuf);
PRTCLWJJD_EXT   uint8  FReSenddata(uint8 l_pCMD, uint32 l_pBgnNum, uint32 l_pEndNum);
PRTCLWJJD_EXT   void   FProcessPrt(uint8* pBuf);
PRTCLWJJD_EXT   uint8  FCheckUserKey(uint8* pBuf);
PRTCLWJJD_EXT   void   FAskAxleLoadData(uint8* pBuf);
PRTCLWJJD_EXT   void   FReSend39data(void);
#endif
