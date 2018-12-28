/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			prtclWJ.h
** Last modified Date:  20121031
** Last Version:		1.0
** Descriptions:		非现场执法协议
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wang ZiFeng
** Created date:		20121031
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------	
*********************************************************************************************************/
#ifndef __PRTCLFXC_H
#define __PRTCLFXC_H
				
#include "config.h"

#ifdef  __PRTCLFXC_C
#define	PRTCLFXC_EXT	 
#else
#define	PRTCLFXC_EXT	extern
#endif



PRTCLFXC_EXT	void ProcessCmd_FXC(uint8 *pData);
PRTCLFXC_EXT	void SendCommandToPC_FXC(uint8 p_u8VehIndex,uint8 p_u8CommandNO);

PRTCLFXC_EXT	void ToBuff_FXC( Vehicle_Struct *pdata );
PRTCLFXC_EXT	void SendDeviceStatus_FXC(void);

//PRTCLFXC_EXT	uint8 FFormHeartFrame_FXC(void);
PRTCLFXC_EXT	uint8 FSendHeartbeat_FXC(void);
PRTCLFXC_EXT	uint8 FJudgeUpdate_FXC(void);

//PRTCLFXC_EXT	uint8 FFormExtDataFrame_FXC(void);

#endif

