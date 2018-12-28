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
#ifndef __PRTCLWJ_H
#define __PRTCLWJ_H
				
#include "config.h"

#ifdef  __PRTCLWJ_C
#define	PRTCLWJ_EXT	 

#else
#define	PRTCLWJ_EXT	extern
#endif


//
extern uint32	m_u32VehInfo_Addr_Begin;	// 车辆信息开始位置
extern uint32	m_u32VehInfo_Data_Begin;
extern uint32	m_u32VehInfo_Addr_End;
//	
extern uint32	m_u32Log_Addr_Begin;	   // 日志开始位置
extern uint32	m_u32Log_Data_Begin;
extern uint32	m_u32Log_Addr_End;
//
extern uint32	m_u32Wave_Addr_Begin;	   // 波形开始位置
extern uint32	m_u32Wave_Data_Begin;
extern uint32	m_u32Wave_Addr_End;	

PRTCLWJ_EXT	void WJ_ProcessCmd(uint8 *pData);
PRTCLWJ_EXT	void SendCommandToPC0(uint8 p_u8VehIndex,uint8 p_u8CommandNO);

PRTCLWJ_EXT	void ToBuff0( Vehicle_Struct *pdata );
PRTCLWJ_EXT	void SendDeviceStatus0(void);
PRTCLWJ_EXT	void Tobuff_preWeigh( Vehicle_Struct *pdata);

//PRTCLWJ_EXT	uint8 FFormHeartFrame(void);
PRTCLWJ_EXT	uint8 FSendHeartbeat(void);
PRTCLWJ_EXT	uint8 FJudgeUpdate(void);

//PRTCLWJ_EXT	uint8 FFormExtDataFrame(void);
PRTCLWJ_EXT	void ToBuff0ForBD( Vehicle_Struct *pdata );
#endif

