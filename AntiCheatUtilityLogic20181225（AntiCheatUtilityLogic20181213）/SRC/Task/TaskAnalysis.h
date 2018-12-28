/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskAnalysis.h
** Last modified Date:  20110512
** Last Version:		1.0
** Descriptions:		数据分析任务，包括分析行车方向(任务4)和计算重量(任务6)
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110512
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

#ifndef	__TASKANALYSIS_H
#define	__TASKANALYSIS_H

#ifdef	__TASKANALYSIS_C
#define	TAN_EXT	 

#else
#define	TAN_EXT	extern
#endif	 
#include "Config.h"
//#include "TaskProtocol.h"
//#include "no_Axle.h"
#include "Common.h"
//#include "sinfit.h"
//#include "UIOperation.h"

TAN_EXT	void	TaskRec4(void *pdata);
TAN_EXT	void	TaskRec6(void *pdata);

TAN_EXT	OS_STK	TaskRec4Stk[TASK_VEH_STACKSIZE];	//收尾后任务
TAN_EXT	OS_STK	TaskRec6Stk[TASK_ALG_STACKSIZE];	//计算轴重任务

TAN_EXT void 	FPreDeal( Vehicle_Struct *pItem );
uint8 SelectZT4AxleDisCal(Axle_Struct *preAxle, Axle_Struct *currAxle, TrigID4AxleDis *retData, uint8 type, int unormalID);
void SelectZT4AxleDisCalUPdate(void *Address[5]);
TAN_EXT int intersect(Axle_Struct *preAxle,Axle_Struct *NextAxle,TrigerID_Struct *retData);

TAN_EXT	uint32 FVehAxleCnt1to2( Vehicle_Struct *p_pcVehData );
TAN_EXT	uint32 FVehAxleCnt0to2( Vehicle_Struct *p_pcVehData );
TAN_EXT	uint32 FVehFilterAxleWeight0( Vehicle_Struct *p_pcVehData )	;
TAN_EXT	uint32 FCptDefaultVehTpye( uint8 p_pu8AxleCnt, uint8 *p_pu8AxleGrpCnt, uint8 *p_pau8AxleGrpAxleCnt, uint8 *p_pau8AxleGrpType );
TAN_EXT	uint32 FVehAxleWtJudge( Vehicle_Struct *p_pcVehData );
TAN_EXT void 	FStudyAlg( Vehicle_Struct *pItem );
TAN_EXT void   FGetCarAxleGroupMode( Vehicle_Struct *l_pn32Tmp );
TAN_EXT void   FCompJDLen(uint32 pMax, uint8 pType, uint32 *pLenMax, uint32 *pLenMin);
TAN_EXT uint16   getjdlen(uint16 s,uint16 c,uint16 m);//fangrui 计算接地长度
TAN_EXT int8 FCalcZTWgtAndSpeed(Axle_Struct *l_pAxleTempInfo, uint32 *ZTWgt, uint32 *ZTSpd, uint32 *WheelWgt, uint32 *WheelSpd);
#endif
