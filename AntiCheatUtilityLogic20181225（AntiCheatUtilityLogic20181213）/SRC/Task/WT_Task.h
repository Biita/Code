/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			WT_Task.h
** Last modified Date:  20110511
** Last Version:		1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Created by:			
** Created date:		
** Version:				
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__WT_TASK_H
#define	__WT_TASK_H

#ifdef	__WT_TASK_C
#define	WT_EXT
#else
#define	WT_EXT	extern
#endif

#include "Config.h"
#include "IRQ.h"

#include "UI.h"	   
#include "Common.h"

//#include "TaskKB.h"
#include "TaskTimer.h"
#include "TaskAnalysis.h"
#include "TaskWave.h"
#include "TaskProtocol.h"
//#include "TaskTest.h"
#include "Task_Laser_Interact.h"
//#include "TaskDebugInfo.h"

//#include "WT_Task_ZTFunction.h"
//#include "WT_Task_LaserFunction.h"
#include "TaskJDInf.h"

WT_EXT 	OS_STK	TaskStartStk[TASK_START_STACKSIZE];					
WT_EXT 	OS_STK	TaskRec1Stk[TASK_LOGIC_STACKSIZE];	
WT_EXT 	OS_STK	TaskGetDStk[TASKSTACKSIZE];
WT_EXT 	OS_STK	CheckStackTaskUseSTk[TASK_STK_SIZE_CHKTASK];
//WT_EXT 	OS_STK	TaskTestStk[TASKSTACKSIZE];


				
WT_EXT	void	JZInit(void);			//启动OS前初始化
WT_EXT	void	RunStartTask(void);		//调用启动任务
WT_EXT	void	TaskStart(void *pdata); 
WT_EXT	void	TaskRec1(void *pdata);	 
WT_EXT	void	CheckStackTaskUse(void *pdata);
//WT_EXT	uint8 FCptVehPos( int32 *p_pn8Row1ZT, int32 p_n32LaserLeftOri, int32 p_n32LaserRightOri );
//WT_EXT  void  FLogicForLaster(void); //wgh 20140303
WT_EXT  void  FLogicForLoop(void); //wgh 20140303
WT_EXT  void  FInitVehStr(VehicleInfForLoop_Struct * VehSt, int8 Flag);//wgh 20140303 
//#if LOGIC_FILTER_ZTDNIME_OVER_EN > 0	
//WT_EXT	uint32 FFilterOverTime( uint32 *p_pau32ZTDnTime, int8 *p_pan8ZTFiltered, int32 p_n32InLen );
//WT_EXT	uint32 FCptZTNumInLimit( uint32 *p_pau32ZTDnTime, int *p_pau32ZTNum, int32 p_n32InLen );
//#endif

//#if LOGIC_YAFENG_PROCESS_EN > 0
//WT_EXT	uint32 FUpdateYaFengZT( CYaFengStack *p_pcYafengstack, ELEMENT *p_pcEle, Channal_Struct* p_pcChldata );
//WT_EXT	uint32 FFilterTimeOver( CYaFengStack *p_pcYafengstack, Channal_Struct* p_pcChldata, uint32 p_u32Timenow );
//#endif



WT_EXT	uint32 FIO2ZT4Wave( void );
WT_EXT  void  FCheckDeviceERR(void);
WT_EXT  void  FRecordIO(uint8* pBuf, uint8* pBuf1);
WT_EXT  void  FRecordIO_Laser(uint8* pBuf, uint8* pBuf1);
WT_EXT  void  FLaserProcess(uint8* pBuf, uint32 pTime);
WT_EXT  void  FSendWaveBE(uint8* pBuf, uint32 pTime ,uint8 judgeflag);
WT_EXT  void FVehXieZouStatus(void *AddressForAd[4]);
WT_EXT  void  FVehPassStatus(uint8* pBuf, uint8* pBuf1, uint8* pBuf2);
WT_EXT  void  FDeleteValidZTOrColum(uint32 pTime,uint8* pdownloop, uint8* puploop);
WT_EXT  void  FCheckIODevice(uint8* pBuf,uint8 pDX);
WT_EXT  void  FCheckZTDevice(ZTINFRECELEMENT l_pZtInf, uint8 pDX, uint8 pDY);
WT_EXT  void  FPostVehForNormal(void* Address[3]);
WT_EXT  void  FHideZTPostVehForNormal(void* Address[8]);//隐藏窄条抛车
WT_EXT  void  FPostVehForAbNormal(void* Address[3]);
WT_EXT  void  FIOOutput(uint8* pBuf);
WT_EXT  void FCaptureInfoRec(void* Address[4]);
WT_EXT  void FSecondeCapture(void);
WT_EXT  void  FCapture(void* Address[4], uint8 cmd);
WT_EXT  uint8 IsHaveFlag(void *AddressForAd[6],uint16 *speed);
//WT_EXT	void Laser2LoopConv( void );
//WT_EXT	void Loop2LaserConv( void );

extern sndFrame_Struct s_CoilOrLaserGpioBuf[WAVE_BUFSIZE];
extern uint8           s_CoilOrLaserGpioIndex;



#endif
