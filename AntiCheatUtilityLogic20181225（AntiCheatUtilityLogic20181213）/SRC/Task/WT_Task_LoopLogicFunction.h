/****************************************Copyright (c)****************************************************
**                                     BEIJING  WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			WT_Task_LoopLogicFunction.h
** Last modified Date:  2014-3-5
** Last Version:		
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wgh
** Created date:		20140305
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
#ifndef __LOOPLOGICFUNCTION_H__
#define __LOOPLOGICFUNCTION_H__

#include "config.h"
#include "common.h"

#ifdef	__LOOPLOGICFUNCTION_C
#define	LOOPLOGICFUNCTION_EXT	
#else
#define	LOOPLOGICFUNCTION_EXT	extern
#endif

LOOPLOGICFUNCTION_EXT void  FTransLocation(uint8 ZTID,uint8 *X, uint8 *Y,uint8 mode);//窄条编号转换出所在列和行
LOOPLOGICFUNCTION_EXT void  FTransLocationInvers(uint8 X, uint8 Y, uint8 *ZTID);//窄条所在列和行得到编号
//zhangtonghan 20170531
LOOPLOGICFUNCTION_EXT void  FPostVehForLoop(void* AddressForPo[3],uint8 num);//抛车处理
LOOPLOGICFUNCTION_EXT uint8 AxleWgtJundge(uint16 wgt1, uint16 wgt2);
LOOPLOGICFUNCTION_EXT void FAxleScore(VehicleInfForLoop_Struct *VehInfo);
LOOPLOGICFUNCTION_EXT uint16 FVehScore(VehicleInfForLoop_Struct *VehInfo);
LOOPLOGICFUNCTION_EXT uint8 FUnPostVeh(VehicleInfForLoop_Struct *VehInfo);
// LOOPLOGICFUNCTION_EXT void 	FMatchFristRow(void * Address[6]);//首排匹配处理
// LOOPLOGICFUNCTION_EXT void 	FMatchSecondRow(void * Address[6]);//第二排匹配处理
// LOOPLOGICFUNCTION_EXT void 	FMatchThirdRow(void * Address[9]);//第三排匹配处理
// LOOPLOGICFUNCTION_EXT uint8 SpeedMatchSecondRow(void *AddressForAd[9]);//速度第二排匹配处理
// LOOPLOGICFUNCTION_EXT uint8 SpeedMatchThirdRow(void *AddressForAd[9]); //速度第三排匹配处理

LOOPLOGICFUNCTION_EXT void 	FMatchHideFristRow(void * Address[8]);//隐藏窄条第一排匹配处理
LOOPLOGICFUNCTION_EXT void 	FMatchHideSecondRow(void * Address[8]);//隐藏窄条第二排匹配处理
// LOOPLOGICFUNCTION_EXT void 	FMatchHideThirdRow(void * Address[4]);//隐藏窄条第三排匹配处理
// LOOPLOGICFUNCTION_EXT void 	FMatchHideFourthRow(void * Address[4]);//隐藏窄条第四排匹配处理
LOOPLOGICFUNCTION_EXT void 	FWhetherOneOrTwoWhel(void * Address[6]);//查找窄条上几个轮及胎型
LOOPLOGICFUNCTION_EXT void 	FHideZTPostAxle(void * Address[1]); //隐藏窄条抛轴
// LOOPLOGICFUNCTION_EXT void 	FRegressionWheel(void);//轮胎归属合并
LOOPLOGICFUNCTION_EXT void 	FWhelMatchAxle(void * Address[6]); //轮与轮匹配出轴
LOOPLOGICFUNCTION_EXT void 	FZTWheelCnt(void * Address[6]);//窄条上轮胎的个数
LOOPLOGICFUNCTION_EXT void 	FZTMatch2Whel(void * Address[10]);//窄条匹配轮
// LOOPLOGICFUNCTION_EXT void 	FWheelBelongPost(void * Address[7]);//轮胎归属及位置
// LOOPLOGICFUNCTION_EXT void 	FindFristRowMtchWheel(void * Address[7]);//寻找首排与第二排匹配的车轮
// LOOPLOGICFUNCTION_EXT void 	FMatchWheel2Axle(void * Address[4]); //匹配轮组轴
// LOOPLOGICFUNCTION_EXT void  FChangX2Line(void * Address[7]);//轴建立在左右轮中间位置的列上
// LOOPLOGICFUNCTION_EXT uint8 FWheelSearch4AxleZT(void * Address[8]);  //轮寻找轴窄条
// LOOPLOGICFUNCTION_EXT void  UnionWgtFunction(void* Address[7]);  //联合称重
// LOOPLOGICFUNCTION_EXT void  DetailedUnionWgtAlg(void* Address[10]);
LOOPLOGICFUNCTION_EXT uint8 FMatchHideZTRule(void * Address[11]);
// LOOPLOGICFUNCTION_EXT uint8 FZTWaveCoincideDegre(void * Address[4]);
// LOOPLOGICFUNCTION_EXT void 	FRowAndWheelMatch(void * Address[4]);
// LOOPLOGICFUNCTION_EXT void 	FindTwoFristRowMatch(void * Address[4]);
// LOOPLOGICFUNCTION_EXT uint8 FMatchRuleFunc(void * Address[11]); //匹配规则函数
// LOOPLOGICFUNCTION_EXT void 	FindWonderColumMatch(void * Address[3]);//在质疑区中寻找与该列匹配
// LOOPLOGICFUNCTION_EXT uint8 DetailedMatchRule(void * l_u32Address[9]); //详细的匹配规则
// LOOPLOGICFUNCTION_EXT uint8 FindColumEnd(Colum_Struct* l_pArrStr,uint8 length);//查找列的空的位置
// LOOPLOGICFUNCTION_EXT uint8 FindWonderEnd(int16 *pArr,uint8 length); //查找质疑区空的位置
// LOOPLOGICFUNCTION_EXT void 	MoveWonder(int16 *pArr,uint8 l_u8ColumPos, uint8 l_u8WonderPos,uint8 num,uint16 ID); //移动质疑区位置
// LOOPLOGICFUNCTION_EXT void 	MoveAndInitWonder(int16 *pArr,uint8 l_u8ColumPos);
// LOOPLOGICFUNCTION_EXT void 	ColumInit(Colum_Struct* l_pArrStr,uint8 Pos); //初始化列指定位置
// LOOPLOGICFUNCTION_EXT void  MoveColumElem(Colum_Struct* l_pArrStr,uint8 length);
// LOOPLOGICFUNCTION_EXT void  MoveAndInitColum(Colum_Struct* l_pArrStr,uint8 pos);
// LOOPLOGICFUNCTION_EXT void  ColumInitSolo(void* Addressed[4]);
// LOOPLOGICFUNCTION_EXT uint8	FPassThrColumMatch(void * Address[4]); //压三边轮重比匹配(包含匹配超过3列的匹配)
// LOOPLOGICFUNCTION_EXT uint8	FMidTimeMatch(void * Address[4]);   //列与列进行中心点时间匹配
// LOOPLOGICFUNCTION_EXT uint8	FDiffTimeMatch(void * Address[4]);  //列与列进行时间匹配
// LOOPLOGICFUNCTION_EXT uint8 SpanColumnTimeMatch(void * Address[3]);//跨列匹配
// LOOPLOGICFUNCTION_EXT void 	FMatchColumAndCloum(void * Address[7]);//列与列匹配处理
LOOPLOGICFUNCTION_EXT void	FMatchAxleAndAxle(void *Address[5]);//轴与轴匹配出车
// LOOPLOGICFUNCTION_EXT void	FAxleMatchRuleFunc(void *Address[5]);//轴与轴匹配规则
LOOPLOGICFUNCTION_EXT void  FProPostAxle(void *Address[3],uint8 num);//
// LOOPLOGICFUNCTION_EXT	int8	FindWaitingForMatchColumNum(void* pItem,uint8 colum,uint8 begin);//寻找待匹配的列的数量
LOOPLOGICFUNCTION_EXT uint8 F_FindEnd8(uint16 lp_nArray[],uint8 cnArrayLength);//数组实际元素个数
LOOPLOGICFUNCTION_EXT int8  F_FindVehEnd(int16 lp_nArray[],uint8 cnArrayLength);
LOOPLOGICFUNCTION_EXT void  FindCurAxleOrPreVeh(void * Address[9]);//查看当前轴是否是前车的
LOOPLOGICFUNCTION_EXT void  SevenAxleCtl(Vehicle_Struct *pItem); //七轴车处理
// LOOPLOGICFUNCTION_EXT uint8	FSymmetricalOrNot(void * Address[2]);//波形对称性判断
// LOOPLOGICFUNCTION_EXT void  FindFirstRowZT(void * Address[9]);//寻找第一排窄条
LOOPLOGICFUNCTION_EXT void BuildNewVeh(void *AddressForPo[3]);
LOOPLOGICFUNCTION_EXT void FCreatNewTrace(void * Address[10]);
LOOPLOGICFUNCTION_EXT void FWheelTraceMerge(void * Address[11]);
LOOPLOGICFUNCTION_EXT uint8 FTraceScore(void * Address[3]);
LOOPLOGICFUNCTION_EXT void FInsertTrace(void * Address[15]);
LOOPLOGICFUNCTION_EXT void FDeleteTrace(void * Address[4]);
LOOPLOGICFUNCTION_EXT uint8 FindHideColumEnd(HideColum_Struct* l_pArrStr,uint8 length,uint8 cmd);//查找列的空的位置
LOOPLOGICFUNCTION_EXT void  MoveHideColumElem(HideColum_Struct* l_pArrStr,uint8 length);
LOOPLOGICFUNCTION_EXT void  ColumHideInit(HideColum_Struct* l_pArrStr,uint8 Pos);
LOOPLOGICFUNCTION_EXT void  FCommMtchZTFunc(void * Address[5]);
LOOPLOGICFUNCTION_EXT void  FMtchLeftAndRghtColum(void * Address[6]);
LOOPLOGICFUNCTION_EXT void 	TwoZTCoinDegFunc(void * Address[6]);//两根窄条重合度匹配程度的函数
LOOPLOGICFUNCTION_EXT void 	FCoinMatchAdd(void * Address[6]);//隐藏窄条匹配左右两侧的窄条
LOOPLOGICFUNCTION_EXT void  WhetherCrossThreeLane(void* Address[5]);
LOOPLOGICFUNCTION_EXT void  ColumHideInitSolo(void* Addressed[4]);
LOOPLOGICFUNCTION_EXT void 	FMatchHideColumAndCloum(void * Address[6]);
LOOPLOGICFUNCTION_EXT void 	FHideZTColumMtchFunc(void * Address[6]);
LOOPLOGICFUNCTION_EXT void FArrangeTrace(void * Address[6]);
LOOPLOGICFUNCTION_EXT void FClearTrace(void * Address[6], uint8 cmd);
LOOPLOGICFUNCTION_EXT void FDelSingleTrace(void *trace, uint8 dir);
LOOPLOGICFUNCTION_EXT void FPostWheel(void * Address[2]);
LOOPLOGICFUNCTION_EXT void FInitTrace(void * Address[2]);
LOOPLOGICFUNCTION_EXT void FBuildWheel(void * Address[5]);
LOOPLOGICFUNCTION_EXT int8 FMatchWheel(void * Address[4]);
LOOPLOGICFUNCTION_EXT int8 FWheelTimeMatch(void * Address[7]);
LOOPLOGICFUNCTION_EXT uint8 FOverlapRatio(uint32 up1, uint32 dn1, uint32 up2, uint32 dn2);
LOOPLOGICFUNCTION_EXT int8 FWheelWgtMatch(void * Address[6]);
LOOPLOGICFUNCTION_EXT void FFindFirstTracePos(void * Address[2]);
LOOPLOGICFUNCTION_EXT void FWheelBelong2Axle(void * Address[4]);
//end 
#endif
