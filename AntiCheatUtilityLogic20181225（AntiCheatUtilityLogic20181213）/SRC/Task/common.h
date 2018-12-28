#ifndef __COMMON_H__
#define __COMMON_H__

//所有头文件在此处包含，其他文件都包含该头文件
#include <stdio.h>
#include <time.h>
#include <rt_misc.h>

#include "config.h"

#include "TaskKB.h"
#include "TaskTimer.h"
#include "TaskAnalysis.h"
#include "TaskProtocol.h"
#include "TaskWave.h"
//#include "WT_Task_LaserFunction.h"
//#include "WT_Task_ZTFunction.h"
//#include "TaskTest.h"
#include "Task_Laser_Interact.h"
#include "TaskDebugInfo.h"
#include "TaskSDCard.h"
#include "TaskJDInf.h"
#include "Task_UI.h"


#include "AdjustMacro.h" 
#include "UI.h"	  
#include "w5300app.h"
#include "uart5.h"
#include "UART1.h"
#include "uart3.h"
#include "UI.h"
#include "TDC256.h"	
//
#include "prtclWJOLD.h"
#include "prtclWJ.h"	//20121108
#include "prtclFXC.h"
#include "prtclWJJD.h"
//
#include "WT_Task.h"
#include "IRQ.h"
#include "stdarg.h"
#include "LCDApp.h"
#include "FPGA_APP.h"
#include "WT_Task_LoopLogicFunction.h"	 //wgh 20140305
//#include "W5300App.h"
#include "w5300.h"
#include "socket.h"
#include "JDProcess.h"

#include <string.h>
#include <stdio.h>

#ifdef	__COMMON_C
#define	CMN_EXT	
#else
#define	CMN_EXT	extern
#endif

#define MinSevWeigh  1000  //七轴车最小重量
#define MinVehWeigh  25000  //重量为25吨
#define MinSevAxleWeigh  2000  //七轴车轴重

#define F_Max(nA,nB) (((nA)>(nB))?(nA):(nB))
#define F_Min(nA,nB) ((nA<nB)?nA:nB)   
#define F_Nabs(p_lNumber) (((p_lNumber)<0)?(-(p_lNumber)):(p_lNumber))

#define	SUB(a,b)	( ((a)>(b)) ? ((a)-(b)) : ((b)-(a)) )

// 得到差值
#define tickGetDiff(a, b) ((a < b) ? (0xffffffff - (b - a)) : (a - b))
#define GetDiff(a,b) ((a < b) ? (b-a):(a-b))
#define CompDiffBig(a,b,c)  ((((a)>=(c)) || ((b)>=(c))) ? 1:0)
#define CompDiffLit(a,b,c)  ((((a)<=(c)) || ((b)<=(c))) ? 1:0)
#define BothCompDiffBig(a,b,c)	((((a)>=(c)) && ((b)>=(c))) ? 1:0)
#define IsLit(a,b) (((a) < (b)) ? (a):(b))
#define IsBig(a,b) (((a) > (b)) ? (a):(b))
#define IsBigFlag(a,b) (((a) > (b))? 1:0)
#define IsBigTwo(a,b) ((a >= b) ? (a - b): (b - a))
#define IsInMid(a, min, max)   ((((a)>=(min)) && ((a)<=(max)))? 1:0)
#define TheBigOne(a,b,c)   ((a) > (b))? (((a)>(c))?(a):(c)):(((b) > (c))?(b):(c))
#define TheLitOne(a,b,c)   ((a) < (b))? (((a)<(c))?(a):(c)):(((b) < (c))?(b):(c))
#define	TheMidOne(a,b,c)   (((a) > (b)) ? (((b) > (c)) ? (b) : (((a) > (c)) ? (c) : (a))) : (((a) > (c)) ? (a): (((b) > (c)) ? (c) : (a))))
#define WhetherEqual(a,b)  (((a)==(b))?1:0)

//定时器功能
typedef struct _TIMER_PIB
{
	uint32 start;  // start time
    uint32 curTick;  //now time
	uint32 interval; // timeout gap
}TIMER_PIB ;


CMN_EXT	uint16	AddCrc16( uint8 * p_pu8Ptr,uint16 p_u16Len);		//校验，写CRC
CMN_EXT	uint8	CheckCrc( uint8 * p_pu8Ptr,uint16 p_u16Len);		//校验，不写CRC
CMN_EXT	int32	ScaleUp(int32 p_n32Value, uint8 p_u8Scale);			//分度设置函数
CMN_EXT	uint8	bcd(uint32 p_u32Value,uint8 p_u8Pos);				//获取bcd数字

CMN_EXT	uint8	StaticXiuZhengIndex(int32 p_n32Weight);
CMN_EXT uint32 RootFloat(float number);
//CMN_EXT int wj_Printf(const char* cmd, ...);
CMN_EXT void timer_set(TIMER_PIB *t, uint32 interval);
CMN_EXT void timer_reset(TIMER_PIB *t);
CMN_EXT void timer_restart(TIMER_PIB *t);
CMN_EXT uint8 timer_expired(TIMER_PIB *t);

CMN_EXT void FReConnect( uint16 p_u16Len, uint8 p_u8Sockketport );

CMN_EXT uint32 FFendu( uint32 *p_pu16Wet);
CMN_EXT void	FSearchMaxZT(void * Address[5]);
CMN_EXT void 	FCalcWalkRun(void * Address[4]);
CMN_EXT void 	FJudgWalkRun(void * Address[4]);
CMN_EXT void Nm2Bcd(uint32 u32_Num, uint8 *u8_buf, uint8 p_u8Len);//旧高速预检兼容
CMN_EXT void Crc3_create(uint8 *p_pu8Ptr, uint16 p_u16Len);//旧高速预检兼容

CMN_EXT uint8 FIsRunYear(uint16 pYear);
CMN_EXT uint16 FDaysOneYear(uint16 pYear);
CMN_EXT uint8 FDaysOneMon(uint16 pYear, uint8 pMonth);
CMN_EXT unsigned short JDCRC16(unsigned char *puchMsg, unsigned short usDataLen);
///*****************************陈忠元封装初始化函数2014-12-22****************************/
CMN_EXT void Sys_init(void);//ucos系统、ARM系统及其外设、FPGA系统及其外设等相关系统级初始化
CMN_EXT void LogicMem_init(void);//逻辑变量初始化，用于参与逻辑判断的一些全局变量、结构体等信息的初始化
CMN_EXT void External_Equipment_init(void);	//外部设备初始化；激光、线圈、SD卡等外部设备的相关变量初始化
CMN_EXT void Fault_diagnosis_init(void);	//故障诊断模块相关变量初始化

CMN_EXT void FReadFromTDC(void);
CMN_EXT void FTaskBuild(void);
CMN_EXT void FIOStatusInt(void);


#endif

