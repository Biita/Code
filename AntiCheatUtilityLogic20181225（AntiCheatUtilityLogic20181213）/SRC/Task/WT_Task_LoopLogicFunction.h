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

LOOPLOGICFUNCTION_EXT void  FTransLocation(uint8 ZTID,uint8 *X, uint8 *Y,uint8 mode);//խ�����ת���������к���
LOOPLOGICFUNCTION_EXT void  FTransLocationInvers(uint8 X, uint8 Y, uint8 *ZTID);//խ�������к��еõ����
//zhangtonghan 20170531
LOOPLOGICFUNCTION_EXT void  FPostVehForLoop(void* AddressForPo[3],uint8 num);//�׳�����
LOOPLOGICFUNCTION_EXT uint8 AxleWgtJundge(uint16 wgt1, uint16 wgt2);
LOOPLOGICFUNCTION_EXT void FAxleScore(VehicleInfForLoop_Struct *VehInfo);
LOOPLOGICFUNCTION_EXT uint16 FVehScore(VehicleInfForLoop_Struct *VehInfo);
LOOPLOGICFUNCTION_EXT uint8 FUnPostVeh(VehicleInfForLoop_Struct *VehInfo);
// LOOPLOGICFUNCTION_EXT void 	FMatchFristRow(void * Address[6]);//����ƥ�䴦��
// LOOPLOGICFUNCTION_EXT void 	FMatchSecondRow(void * Address[6]);//�ڶ���ƥ�䴦��
// LOOPLOGICFUNCTION_EXT void 	FMatchThirdRow(void * Address[9]);//������ƥ�䴦��
// LOOPLOGICFUNCTION_EXT uint8 SpeedMatchSecondRow(void *AddressForAd[9]);//�ٶȵڶ���ƥ�䴦��
// LOOPLOGICFUNCTION_EXT uint8 SpeedMatchThirdRow(void *AddressForAd[9]); //�ٶȵ�����ƥ�䴦��

LOOPLOGICFUNCTION_EXT void 	FMatchHideFristRow(void * Address[8]);//����խ����һ��ƥ�䴦��
LOOPLOGICFUNCTION_EXT void 	FMatchHideSecondRow(void * Address[8]);//����խ���ڶ���ƥ�䴦��
// LOOPLOGICFUNCTION_EXT void 	FMatchHideThirdRow(void * Address[4]);//����խ��������ƥ�䴦��
// LOOPLOGICFUNCTION_EXT void 	FMatchHideFourthRow(void * Address[4]);//����խ��������ƥ�䴦��
LOOPLOGICFUNCTION_EXT void 	FWhetherOneOrTwoWhel(void * Address[6]);//����խ���ϼ����ּ�̥��
LOOPLOGICFUNCTION_EXT void 	FHideZTPostAxle(void * Address[1]); //����խ������
// LOOPLOGICFUNCTION_EXT void 	FRegressionWheel(void);//��̥�����ϲ�
LOOPLOGICFUNCTION_EXT void 	FWhelMatchAxle(void * Address[6]); //������ƥ�����
LOOPLOGICFUNCTION_EXT void 	FZTWheelCnt(void * Address[6]);//խ������̥�ĸ���
LOOPLOGICFUNCTION_EXT void 	FZTMatch2Whel(void * Address[10]);//խ��ƥ����
// LOOPLOGICFUNCTION_EXT void 	FWheelBelongPost(void * Address[7]);//��̥������λ��
// LOOPLOGICFUNCTION_EXT void 	FindFristRowMtchWheel(void * Address[7]);//Ѱ��������ڶ���ƥ��ĳ���
// LOOPLOGICFUNCTION_EXT void 	FMatchWheel2Axle(void * Address[4]); //ƥ��������
// LOOPLOGICFUNCTION_EXT void  FChangX2Line(void * Address[7]);//�Ὠ�����������м�λ�õ�����
// LOOPLOGICFUNCTION_EXT uint8 FWheelSearch4AxleZT(void * Address[8]);  //��Ѱ����խ��
// LOOPLOGICFUNCTION_EXT void  UnionWgtFunction(void* Address[7]);  //���ϳ���
// LOOPLOGICFUNCTION_EXT void  DetailedUnionWgtAlg(void* Address[10]);
LOOPLOGICFUNCTION_EXT uint8 FMatchHideZTRule(void * Address[11]);
// LOOPLOGICFUNCTION_EXT uint8 FZTWaveCoincideDegre(void * Address[4]);
// LOOPLOGICFUNCTION_EXT void 	FRowAndWheelMatch(void * Address[4]);
// LOOPLOGICFUNCTION_EXT void 	FindTwoFristRowMatch(void * Address[4]);
// LOOPLOGICFUNCTION_EXT uint8 FMatchRuleFunc(void * Address[11]); //ƥ�������
// LOOPLOGICFUNCTION_EXT void 	FindWonderColumMatch(void * Address[3]);//����������Ѱ�������ƥ��
// LOOPLOGICFUNCTION_EXT uint8 DetailedMatchRule(void * l_u32Address[9]); //��ϸ��ƥ�����
// LOOPLOGICFUNCTION_EXT uint8 FindColumEnd(Colum_Struct* l_pArrStr,uint8 length);//�����еĿյ�λ��
// LOOPLOGICFUNCTION_EXT uint8 FindWonderEnd(int16 *pArr,uint8 length); //�����������յ�λ��
// LOOPLOGICFUNCTION_EXT void 	MoveWonder(int16 *pArr,uint8 l_u8ColumPos, uint8 l_u8WonderPos,uint8 num,uint16 ID); //�ƶ�������λ��
// LOOPLOGICFUNCTION_EXT void 	MoveAndInitWonder(int16 *pArr,uint8 l_u8ColumPos);
// LOOPLOGICFUNCTION_EXT void 	ColumInit(Colum_Struct* l_pArrStr,uint8 Pos); //��ʼ����ָ��λ��
// LOOPLOGICFUNCTION_EXT void  MoveColumElem(Colum_Struct* l_pArrStr,uint8 length);
// LOOPLOGICFUNCTION_EXT void  MoveAndInitColum(Colum_Struct* l_pArrStr,uint8 pos);
// LOOPLOGICFUNCTION_EXT void  ColumInitSolo(void* Addressed[4]);
// LOOPLOGICFUNCTION_EXT uint8	FPassThrColumMatch(void * Address[4]); //ѹ�������ر�ƥ��(����ƥ�䳬��3�е�ƥ��)
// LOOPLOGICFUNCTION_EXT uint8	FMidTimeMatch(void * Address[4]);   //�����н������ĵ�ʱ��ƥ��
// LOOPLOGICFUNCTION_EXT uint8	FDiffTimeMatch(void * Address[4]);  //�����н���ʱ��ƥ��
// LOOPLOGICFUNCTION_EXT uint8 SpanColumnTimeMatch(void * Address[3]);//����ƥ��
// LOOPLOGICFUNCTION_EXT void 	FMatchColumAndCloum(void * Address[7]);//������ƥ�䴦��
LOOPLOGICFUNCTION_EXT void	FMatchAxleAndAxle(void *Address[5]);//������ƥ�����
// LOOPLOGICFUNCTION_EXT void	FAxleMatchRuleFunc(void *Address[5]);//������ƥ�����
LOOPLOGICFUNCTION_EXT void  FProPostAxle(void *Address[3],uint8 num);//
// LOOPLOGICFUNCTION_EXT	int8	FindWaitingForMatchColumNum(void* pItem,uint8 colum,uint8 begin);//Ѱ�Ҵ�ƥ����е�����
LOOPLOGICFUNCTION_EXT uint8 F_FindEnd8(uint16 lp_nArray[],uint8 cnArrayLength);//����ʵ��Ԫ�ظ���
LOOPLOGICFUNCTION_EXT int8  F_FindVehEnd(int16 lp_nArray[],uint8 cnArrayLength);
LOOPLOGICFUNCTION_EXT void  FindCurAxleOrPreVeh(void * Address[9]);//�鿴��ǰ���Ƿ���ǰ����
LOOPLOGICFUNCTION_EXT void  SevenAxleCtl(Vehicle_Struct *pItem); //���ᳵ����
// LOOPLOGICFUNCTION_EXT uint8	FSymmetricalOrNot(void * Address[2]);//���ζԳ����ж�
// LOOPLOGICFUNCTION_EXT void  FindFirstRowZT(void * Address[9]);//Ѱ�ҵ�һ��խ��
LOOPLOGICFUNCTION_EXT void BuildNewVeh(void *AddressForPo[3]);
LOOPLOGICFUNCTION_EXT void FCreatNewTrace(void * Address[10]);
LOOPLOGICFUNCTION_EXT void FWheelTraceMerge(void * Address[11]);
LOOPLOGICFUNCTION_EXT uint8 FTraceScore(void * Address[3]);
LOOPLOGICFUNCTION_EXT void FInsertTrace(void * Address[15]);
LOOPLOGICFUNCTION_EXT void FDeleteTrace(void * Address[4]);
LOOPLOGICFUNCTION_EXT uint8 FindHideColumEnd(HideColum_Struct* l_pArrStr,uint8 length,uint8 cmd);//�����еĿյ�λ��
LOOPLOGICFUNCTION_EXT void  MoveHideColumElem(HideColum_Struct* l_pArrStr,uint8 length);
LOOPLOGICFUNCTION_EXT void  ColumHideInit(HideColum_Struct* l_pArrStr,uint8 Pos);
LOOPLOGICFUNCTION_EXT void  FCommMtchZTFunc(void * Address[5]);
LOOPLOGICFUNCTION_EXT void  FMtchLeftAndRghtColum(void * Address[6]);
LOOPLOGICFUNCTION_EXT void 	TwoZTCoinDegFunc(void * Address[6]);//����խ���غ϶�ƥ��̶ȵĺ���
LOOPLOGICFUNCTION_EXT void 	FCoinMatchAdd(void * Address[6]);//����խ��ƥ�����������խ��
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
