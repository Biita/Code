/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskAnalysis.C
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
#define	__TASKANALYSIS_C

#include "common.h"																			 

#define		SETUPALIAS				g_Setup			//设置参数结构
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
typedef struct tagTranZT{
	uint32 	u32chlSum;	//窄条积分和
	uint32 	u32uptm;		//上称时刻
	uint32 	u32dntm;		//下秤时刻
	uint32	width;			//波形宽度
	uint32  wmax;				//最大值
	uint32  MaxValueTime;		//最大值时刻
	uint8	flag;				//窄条存在标志
	uint8   TakePos;
	uint16  ZTPos;
	uint32	u32realCnt;  //真实点数
	uint8	ZTMappedPos;
}TranZT_Struct;
typedef struct _tagTranZT{
	uint32 	u32chlSum;	//窄条积分和
	uint32 	u32uptm;		//上称时刻
	uint32 	u32dntm;		//下秤时刻
	uint16 	ZTPos;
	uint8   TakePos;
	uint8	ZTId;
	uint16	ZTSpd;
	uint8	ZTmapos;
}ListZT;
typedef	struct tagHideZT{
//	uint32 ZTWeight;  //当前列重量
	uint16 ZTPos[2]; //当前列的窄条位置信息
	uint16 ZTSpd;    //当前列信息 cm/s
	uint8  ZTCnt;    // 当前列有几根窄条
}HideZT_Struct;   //隐藏布局列信息
typedef struct tagXieList{
	uint8 ZTmapindex[6];//窄条存储的位置
	uint8 ZTRghtmapindex[6];//窄条存储的位置
	uint8 ZTRghtcnt;  //斜行窄条计算的个数
	uint8 ZTcnt;  //斜行窄条计算的个数
}XieListZT;
typedef struct tagZTSpdlen
{
	uint16 	spd; //窄条速度 0.01km/h
	uint16 	length; //窄条接地长度
	uint16 	ZTpos1; //窄条速度由窄条1位置组成
	uint16 	ZTpos2; //窄条位置2
	uint16	ZTMaxValue;
	uint8 	ZTake1;
//	uint8	ZTake2;
}Str_ZTSpdlen;
// typedef struct tagZTSort{
// 	uint16	ZTPos;
// 	uint8	ZTake;
// }Str_ZTSort;
typedef struct tagZTWeight
{
	uint32 ZTWeight;
	uint16 ZTPos;
	uint8  TakePos;    //触发数字窄条的位置 1:左侧;2:右侧;3:整体
}Str_ZTWght;
#define 	KEY_TIRE_VALUE		  125
#define 	AIXSGROUP_TYPE_LIMIT  140
//extern uint32		m_WeightStatic[6];
////////////////////  判车型 wgh 20130708
#define    min_my(a, b)    (((a)<=(b)) ? (a):(b))
#define    abs_my(a)       ((a>=0) ? a:-(a))
// 车型
#define    CARMDEFAULT      0x00000000   // 默认车型
#define    CARM11           0x0000000B
#define    CARM12           0x0000000C
#define    CARM15           0x0000000F
#define    CARM112          0x00000070
#define    CARM122          0x0000007A
#define    CARM114          0x00000072
#define    CARM115          0x00000073
#define    CARM125          0x0000007D
#define    CARM118          0x00000076
#define    CARM119          0x00000077
#define    CARM128          0x00000080
#define    CARM129          0x00000081
#define    CARM155          0x0000009B
#define    CARM1125         0x00000465
#define    CARM149          0x00000095
#define    CARM159          0x0000009F
#define    CARM1129         0x00000469
#define    CARM11151        0x00002B8F
#define    CARM12151        0x00002F77

// 判别界限
#define		V2ALLWTUPPER		2700			// 两轴车总重上界
#define		V2ALLWTLOWER		1500			// 两轴车总重下界
#define		V2AX2WTTHRESHOLD	1500			// 两轴车后轴轴重12车判别界线

#define		AXGRPDISTHRESHOLD	1900		// 联轴轴距 1.9m
//#define	AXLELDISB			1700		// 联轴轴距判别下界 1.7m
//#define	AXLELDISP			2100		// 联轴轴距判别上界 2.1m

#define		AX1DISTHRESHOLD		2700		// 第一轴轴距通用阈值 2.7m
#define		AX1DISLOWER			2450		// 第一轴轴距判别下界,用于判断11X与12X 2.45m
#define		AX1DISUPPER			2950		// 第一轴轴距判别上界,用于判断11X与12X 2.95m
////////////////////

uint8 JCPostMapped[6]={1,3,5,0,2,4};  //交错窄条的物理映射位置
//窄条宽度拟合的速度，速度放大1000倍
uint32	ZTWidthFittingSpd[400] = {
198249 ,70586 ,43670 ,31829 ,25132 ,20811 ,17787 ,15548 ,13823 ,12450 ,11332 ,10404 ,9619 ,8948 ,8367 ,7858 ,7410 ,7011 ,6654 ,6333 ,
6042   ,5778  ,5536  ,5314  ,5110  ,4921  ,4747  ,4584  ,4433  ,4292  ,4159  ,4035  ,3918 ,3808 ,3704 ,3606 ,3513 ,3425 ,3341 ,3262 ,
3186   ,3114  ,3045  ,2979  ,2916  ,2856  ,2798  ,2743  ,2689  ,2638  ,2589  ,2542  ,2496 ,2452 ,2410 ,2369 ,2330 ,2292 ,2255 ,2219 ,
2185   ,2151  ,2119  ,2088  ,2057  ,2028  ,1999  ,1971  ,1944  ,1918  ,1892  ,1867  ,1843 ,1819 ,1796 ,1774 ,1752 ,1731 ,1710 ,1690 ,
1670   ,1651  ,1632  ,1614  ,1596  ,1578  ,1561  ,1544  ,1528  ,1512  ,1496  ,1481  ,1466 ,1451 ,1437 ,1422 ,1409 ,1395 ,1382 ,1369 ,
1356   ,1343  ,1331  ,1319  ,1307  ,1295  ,1284  ,1273  ,1262  ,1251  ,1240  ,1230  ,1219 ,1209 ,1199 ,1190 ,1180 ,1171 ,1161 ,1152 ,
1143   ,1134  ,1126  ,1117  ,1109  ,1100  ,1092  ,1084  ,1076  ,1068  ,1061  ,1053  ,1046 ,1038 ,1031 ,1024 ,1017 ,1010 ,1003 ,996 ,
990    ,983   ,976   ,970   ,964   ,958   ,951   ,945   ,939   ,933   ,928   ,922   ,916  ,911  ,905  ,900  ,894  ,889  ,884  ,878 ,
873    ,868   ,863   ,858   ,853   ,848   ,844   ,839   ,834   ,830   ,825   ,820   ,816  ,812  ,807  ,803  ,799  ,794  ,790  ,786 ,
782    ,778   ,774   ,770   ,766   ,762   ,758   ,754   ,751   ,747   ,743   ,740   ,736  ,732  ,729  ,725  ,722  ,718  ,715  ,712 ,
708    ,705   ,702   ,699   ,695   ,692   ,689   ,686   ,683   ,680   ,677   ,674   ,671  ,668  ,665  ,662  ,659  ,656  ,653  ,651 ,
648    ,645   ,642   ,640   ,637   ,634   ,632   ,629   ,626   ,624   ,621   ,619   ,616  ,614  ,611  ,609  ,607  ,604  ,602  ,599 ,
597    ,595   ,592   ,590   ,588   ,586   ,583   ,581   ,579   ,577   ,575   ,572   ,570  ,568  ,566  ,564  ,562  ,560  ,558  ,556 ,
554    ,552   ,550   ,548   ,546   ,544   ,542   ,540   ,538   ,536   ,535   ,533   ,531  ,529  ,527  ,525  ,524  ,522  ,520  ,518 ,
517    ,515   ,513   ,511   ,510   ,508   ,506   ,505   ,503   ,502   ,500   ,498   ,497  ,495  ,494  ,492  ,490  ,489  ,487  ,486 ,
484    ,483   ,481   ,480   ,478   ,477   ,475   ,474   ,472   ,471   ,470   ,468   ,467  ,465  ,464  ,463  ,461  ,460  ,458  ,457 ,
456    ,454   ,453   ,452   ,451   ,449   ,448   ,447   ,445   ,444   ,443   ,442   ,440  ,439  ,438  ,437  ,435  ,434  ,433  ,432 ,
431    ,429   ,428   ,427   ,426   ,425   ,424   ,422   ,421   ,420   ,419   ,418   ,417  ,416  ,415  ,413  ,412  ,411  ,410  ,409 ,
408    ,407   ,406   ,405   ,404   ,403   ,402   ,401   ,400   ,399   ,398   ,397   ,396  ,395  ,394  ,393  ,392  ,391  ,390  ,389 ,
388    ,387   ,386   ,385   ,384   ,383   ,382   ,381   ,380   ,379   ,379   ,378   ,377  ,376  ,375  ,374  ,373  ,372  ,371  ,371 };
//volatile int32 dis[64] = {0}; 

void  DelayNS(unsigned int  idly)//浩叔 直接拿过来用
{  
	unsigned int  i;
	for(; idly>0; idly--){
		for(i=0; i<5000; i++);
	}
}
/*********************************************************************************************************
** Function name:		TaskRec4
** Descriptions:		判断行车方向
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110512	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------  
*********************************************************************************************************/	  


void TaskRec4(void *tdata)
{ 
 	Vehicle_Struct *pItem = (Vehicle_Struct *)0;
//	uint32	l_u32tmp = 0;
	//uint8	ret = 0;
	uint8	l_err = 0;
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	
//	uint32	l_u32Temp1 = 0;
//#if  NET2VEHINFO > 0
//	uint32	l_u32TempAllWeight = 0;//临时变量，总重
//	uint32	l_u32Temp1 = 0;
//	uint8 *		l_pu8Tmp;	   //指针，指向所有数据
//#endif	   

#if	OS_TASK_CREATE_EXT_EN > 0
  	OS_STK_DATA data;
#endif

//#if LASER_UNNORMAL_EN > 0
//	CVehSimInfo l_cCurVehSimInfo;		// 当前车简单信息
//	uint8	l_u8MatchFlag = 0;			// 匹配标志，匹配上为1					
//#endif

	tdata = tdata;

//#if LASER_UNNORMAL_EN > 0
//	memset( &l_cCurVehSimInfo, 0, sizeof(CVehSimInfo) );					
//#endif

	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// 任务开始运行
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T4 Recd Task Begin [Veh].\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif



	while(1)
	{
		
		pItem = (Vehicle_Struct *)OSQPend(g_pQShouWei,0,&l_err);

		if(l_err == OS_NO_ERR)
		{


#if  OS_TASK_CREATE_EXT_EN >0
			//((TaskStatics*)OSTCBCur->OSTCBExtPtr)->tmLast = OSTimeGet();
		   	OSTaskStkChk(OS_PRIO_SELF,&data);
			((TaskStatics*)OSTCBCur->OSTCBExtPtr)->stkCurUsage = data.OSUsed*100/OSTCBCur->OSTCBStkSize;
			if(data.OSUsed > ((TaskStatics*)OSTCBCur->OSTCBExtPtr)->stkMaxUsage) 
				((TaskStatics*)OSTCBCur->OSTCBExtPtr)->stkMaxUsage = data.OSUsed;
#endif 		
		 	pItem->m_u8ProFlag = 0;
			//预处理
			FPreDeal( pItem );
			
			if((PROMODPARAM.m_u8StudyAlgEn[0] == 1) || (PROMODPARAM.m_u8StudyAlgEn[1] == 1) || (PROMODPARAM.m_u8StudyAlgEn[2] == 1)
				|| (PROMODPARAM.m_u8StudyAlgEn[3] == 1) || (PROMODPARAM.m_u8StudyAlgEn[4] == 1))
			{
				FStudyAlg(pItem);
			}

			if( 0 == pItem->u8AixCnt )
			{
				continue;
			}
			if(g_u8ChlForShow == 0)
			{
				OSQPost(g_pSemDisplayCheliangInfo,pItem);
			}
			else if(g_u8ChlForShow == pItem -> u8psLane/2 + 1)
			{
				OSQPost(g_pSemDisplayCheliangInfo,pItem);
			}
	
				if(PROMODPARAM.m_u8EnableFlag[2] == 1)	//线圈交调
				{
			//		if(PROMODPARAM.m_u8TriggerMode == 4 || PROMODPARAM.m_u8TriggerMode == 2) //线圈模式
					{
						FJDProcess(pItem);
					}
			//		if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)  //激光模式
					{
					}
				}
				if(PROMODPARAM.m_au32Thres[12]>0)
				{
					g_u32VehCntForTest++;
				}
				if(g_u8IfRunSta	== 0)
				{
					ToProtocolBuff(pItem);
				}

	//#endif
		 	
			

#if DEBUGINFO_TASK_SP_EN > 0
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T4 Recd FormFrame.\r\n",
								   g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								   g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
#endif
//#endif

//#if TEST_CAPTURETIME_EN > 0
//		   	g_u16TestCaptime = 0;
//			g_u8TestSPFlag = 0;
//			g_u8TestIOFlag = 0;
//			g_u32T1TCBeginTime = 0;
//#endif
						  
		}	 //end if(l_err == OS_NO_ERR)
	// DelayNS(2000);
	} //end while(1)
}

/*********************************************************************************************************
** Function name:		FPreDeal
** Descriptions:		出车前预处理
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20130808	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FPreDeal( Vehicle_Struct *pItem )					    
{
	int32 Axisnum, i, j, temp;
//	uint16 MaoPao,MaoPaoTemp;
	uint16 AxleTrigerID = 0;
//	uint8 tyrtype[16] ;
	int32 tyrweight[16];
//	int16 Axisgrouptype[16];
	int16 Axisgroupaxisnum[16];
//	int32 Axisdis[16];
	int32 Axisgroupweight[16];
	uint32 Axisgroupcount=0;
	uint32 Axistype;
	int32 modify = 0;
//	void* l_u32UnionWgtAddr[6];

	uint8	l_u8DefVehTypeFlag = 0;	// 默认车型，0默认，1其他车型，需要进行默认车型处理
	uint8	l_au8AxleGrpAxleCnt[ZHOUSIZE];
	uint8	l_au8AxleGrpType[ZHOUSIZE];
	uint8	l_u8AxleGrpCnt = 0;
	uint32	l_u32Ret = 0;
	uint8	l_u8Tempi = 0;
	uint8	l_u8Tempj = 0;
	uint32	l_u32AxleGrpSum = 0;
	uint8	l_u8Temp = 0;
	uint8   l_u8LaneNum;

	uint16	l_u16SendLen = 0;
	uint8	l_u8Err = 0;

	uint8	l_u8VehType = 0;		// 车型类型确认

	uint32  l_u32TotalDis = 0;

	uint32  l_u32TimeNow;
	uint16  l_u16VehSpeed;

	uint8   l_u8FindFlag;//长宽高是否找到

	int32   l_n32TotalFen;
	int32   l_n32TotalFen1;
	uint32  l_totalweight;//总重
	
	if(pItem->m_BigVehFollowLitVeh > 0)
	{
			for(i=0;i < pItem->u8AixCnt-1;i++)
			{
				pItem->au16AxleWeight[i]=pItem->au16AxleWeight[i+1];
				pItem->au16AxleDistance[i]=pItem->au16AxleDistance[i+1];
				pItem->au8AxleTAIType[i]=pItem->au8AxleTAIType[i+1];
				pItem->u16AxleList[i]=pItem->u16AxleList[i+1];
				pItem->n8accSpeed[i] = pItem->n8accSpeed[i+1];
			}
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "\n Big Veh Follow Lit Veh :%d.\n", 
												pItem->m_BigVehFollowLitVeh);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			pItem->u8AixCnt--;
	}	
	if((pItem->u8AixCnt > 6) && (pItem->m_BigVehFollowLitVeh == 0))
		SevenAxleCtl(pItem);
	// 车辆数据处理
	Axisnum = pItem->u8AixCnt; 	 //轴数
	l_u8LaneNum = pItem->u8psLane/2;
	  
	temp = 0; 
	l_totalweight = 0;
	
	for (i=0; i < Axisnum; i++)
	{
		AxleTrigerID = pItem->u16AxleList[i];	  //车辆轴编号
//		tyrtype[i] = g_AxleInfo[AxleTrigerID].u16AixsType;	 //胎型
		tyrweight[i] = g_AxleInfo[AxleTrigerID].s32AxleWeight;	   //轮重
	
//		Axisdis[i] = pItem->au16AxleDistance[i];		//轴距	
		temp+= g_AxleInfo[AxleTrigerID].u16speed; 
		if(pItem->au16AxleDistance[i] < 20)
		{
			if(i == 0)
			{
				pItem->au16AxleDistance[i] = 200;
			}
			else
			{
				pItem->au16AxleDistance[i] = 400;
			}
		}
		else
		{
			if(pItem->au16AxleDistance[i] > 900)
			{
				if(i == 0)
				{
					pItem->au16AxleDistance[i] = 200;
				}
				else
				{
					pItem->au16AxleDistance[i] = 700;
				}
			}
		}
		if(i < Axisnum-1)
		{
			l_u32TotalDis += pItem->au16AxleDistance[i]/10; 
		}				 			
	}
	for (i = 0; i<Axisnum; i++)
	{
#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		if(i == 0)
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\n%d:%d    ", i,tyrweight[i]);
		else	
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%d:%d    ", i,tyrweight[i]);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		if(i == Axisnum-1)
		{
			///////////////////////////////
			g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
			g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 19+2*Axisnum;				
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x01;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 19+2*Axisnum;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = Axisnum;
			for (j = 0; j<Axisnum; j++)
			{
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+2*j] = ((tyrweight[j] >> 8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16+2*j] = (tyrweight[j] &0xFF);
			}
			
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+2*Axisnum] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16+2*Axisnum] = 0x00;
			AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
			OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
			g_u8CurNetDebugIndex++;
			g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
			//////////////////////////////
		}
		OSSemPost(g_pSemSendThtouSP);
#endif
	}

	pItem->u16speed = temp/Axisnum;
	l_u16VehSpeed = pItem->u16speed;
///////////////////////////轴组型判断/////////////////////////////////////////////
	memset(Axisgroupaxisnum,1,sizeof(int16)*16);

	FGetCarAxleGroupMode( pItem );
///////////////////////////////////////////////////////////////////////	
	
	Axistype = 0;		  //轴型清0
	
	Axisgroupcount = pItem->u8AixGrpCnt;
	for (i = 0; i<Axisgroupcount; i++)
	{
		Axistype = Axistype*10 + pItem->au8AxleGrpType[i];   //轴型  				
//		pItem->au8AxleGrpType[i]	= Axisgrouptype[i]; //轴组型
	}

//	pItem->u8AixCnt = Axisnum; // 20131025
//	pItem->u8AixGrpCnt = Axisgroupcount; //轴组数量

	switch(Axistype)
	{
		case 11:
		case 12:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][0];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			break;
		case 13:
		case 14:
		case 15:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][1];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=2;
			break;
		case 112:
		case 122:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][2];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=1;
			break;
		case 113:
		case 115:
		case 114:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][3];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=2;
			break;
		case 123:
		case 124:
		case 125:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][4];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=2;
			break;
		case 116:
		case 117:
		case 118:
		case 119:
		case 126:
		case 127:
		case 128:
		case 129:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][5];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=3;
			break;		
		case 133:
		case 135:
		case 155:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][6];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=2;
			Axisgroupaxisnum[2]=2;
			break;
		case 1123:
		case 1125:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][7];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=1;
			Axisgroupaxisnum[2]=2;
			break;
		case 156:
		case 157:
		case 158:
		case 159:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][8];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=2;
			Axisgroupaxisnum[2]=3;
			break;
		case 1126:
		case 1127:
		case 1128:
		case 1129:
			modify = SETUPALIAS.au16VehTotalModify[l_u8LaneNum][8];
			Axisgroupaxisnum[0]=1;
			Axisgroupaxisnum[1]=1;
			Axisgroupaxisnum[2]=1;
			Axisgroupaxisnum[2]=3;
			break;
		default:
			modify = 10000;
			break;
	}
//     //联合共称，需要先判断
// 	if(pItem->UnionWgtFlg == 1)
// 	{
// 		l_u32UnionWgtAddr[0] = &Axistype;
// 		l_u32UnionWgtAddr[1] = &pItem->u8AixCnt;
// 		l_u32UnionWgtAddr[2] = pItem->u16AxleList;
// 		l_u32UnionWgtAddr[3] = pItem->u16HideAxleList;
// 		l_u32UnionWgtAddr[4] = pItem;
// 		UnionWgtFunction(l_u32UnionWgtAddr);
// 	}
	memset(tyrweight,0,sizeof(uint16)*16);
	for (i=0; i < Axisnum; i++)
	{
		tyrweight[i] = pItem->au16AxleWeight[i];
	}
	//车型速度修正// 20131121
	switch(Axistype)
	{
		case 11:
		case 12:
			l_u8VehType = 0;
			break;
		case 13:
		case 14:
		case 15:
			l_u8VehType = 1;
			break;
		case 112:
		case 122:
			l_u8VehType = 2;
			break;
		case 113:
		case 115:
		case 114:
			l_u8VehType = 3;
			break;
		case 123:
		case 124:
		case 125:
			l_u8VehType = 4;
			break;
		case 116:
		case 117:
		case 118:
		case 119:
		case 126:
		case 127:
		case 128:
		case 129:
			l_u8VehType = 5;
			break;		
		case 133:
		case 135:
		case 155:
			l_u8VehType = 6;
			break;
		case 1123:
		case 1125:
			l_u8VehType = 7;
			break;
		case 156:
		case 157:
		case 158:
		case 159:
		case 1126:
		case 1127:
		case 1128:
		case 1129:
			l_u8VehType = 8;
			break;
		default:
			l_u8VehType = 9;
			break;
	}


	if( pItem->u16speed >= 990  )
	{
		pItem->u16speed = 990;
	}
	l_n32TotalFen = 0;
	l_n32TotalFen1 = 0;
	for	(i=0;i<Axisnum;i++)
	{
		//速度整体修正
		tyrweight[i] = tyrweight[i]*g_Setup.au16Speedmodify[l_u8LaneNum][pItem->u16speed/50]/10000;
		//车型整体修正
		tyrweight[i] = tyrweight[i]*modify/10000;
		//车型速度修正
		if( l_u8VehType<=8 )
		{
//			if( pItem->u16speed >= 990  )
//			{
//				pItem->u16speed = 990;
//			}
			tyrweight[i] = tyrweight[i] * g_Setup.au16VehSpeedModify[l_u8LaneNum][l_u8VehType][pItem->u16speed/50] / VSMSTANDARD;
		}
		//
		tyrweight[i] = tyrweight[i]*SETUPALIAS.u16PlatformGain/10000;
		l_n32TotalFen = l_n32TotalFen + tyrweight[i];
		FFendu( (uint32*)&tyrweight[i]);
		if(i<Axisnum-1)
		{
			l_n32TotalFen1 = l_n32TotalFen1 + tyrweight[i];
		}
		pItem->au16AxleWeight[i] = tyrweight[i]/10;
	}
	FFendu( (uint32*)&l_n32TotalFen);
	
	if((l_n32TotalFen - l_n32TotalFen1) < 0)
		pItem->au16AxleWeight[Axisnum-1] = 0;
	else
		pItem->au16AxleWeight[Axisnum-1] = (l_n32TotalFen - l_n32TotalFen1)/10;
	
	for (i = 0; i<Axisnum; i++)
	{
#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		if(i==0)
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\n%d:%d    ", i,tyrweight[i]);
		else
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%d:%d    ", i,tyrweight[i]);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		if(i == Axisnum-1)
		{
			///////////////////////////////
			g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
			g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 19+2*Axisnum;				
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x02;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 19+2*Axisnum;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = Axisnum;
			for (j = 0; j<Axisnum; j++)
			{
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+2*j] = ((tyrweight[j] >> 8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16+2*j] = (tyrweight[j] &0xFF);
			}
			
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+2*Axisnum] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16+2*Axisnum] = 0x00;
			AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
			OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
			g_u8CurNetDebugIndex++;
			g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
			//////////////////////////////
		}
		OSSemPost(g_pSemSendThtouSP);
#endif
	}
	for	(i=0;i<Axisnum;i++)
	{
		l_totalweight +=tyrweight[i];
	}
	// 轴组中直接根据分度化的轴重计算，需要修改
	temp = 0;
	for (i=0;i<Axisgroupcount;i++)
	{
		Axisgroupweight[i] = 0;
		for(j=0;j<Axisgroupaxisnum[i];j++)
		{
			Axisgroupweight[i]+=tyrweight[temp];
			temp++;
		} 

		FFendu( (uint32*)&Axisgroupweight[i]);
		pItem->au16AxleGrpWeight[i] = Axisgroupweight[i]/10;			
	}
	
	

//#if VEH_FILTERWEIGHT0_EN > 0
	// 滤除重量为0
	FVehFilterAxleWeight0( pItem );
//#endif

	// 轴重判断
	FVehAxleWtJudge( pItem );

//#if VEH_ADDAXLE_EN >0
	// 增轴处理，轴数为1或者为0的处理
//	if( 1 == pItem->u8AixCnt && (PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3))
//	{
//		FVehAxleCnt1to2( pItem );
//	}
//	else if( 1 == pItem->u8AixCnt && (PROMODPARAM.m_u8TriggerMode == 2 || PROMODPARAM.m_u8TriggerMode == 4))
//	{
	if(1 == pItem->u8AixCnt)	
	{
		pItem->u8AixCnt = 0;
	}
//	}
//	else if( 0 == pItem->u8AixCnt  && (PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3))
//	{
//#if VEH_NOT_POST_WT0 > 0
//#else
		// 使用激光下，如果轴数为0，则进行补轴，否则，使用线圈不进行补轴
//		if( g_u8IsLaserorCoilFlag )
//		{
//			//FVehAxleCnt0to2( pItem );
//		}
//#endif
//	} 
//#endif

	Axisnum = pItem->u8AixCnt; 	 //轴数
//		for (i = 0; i<Axisnum; i++)
//	{
//#if DEBUGINFO_TASK_SP_EN > 0
//		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%d    %d    ", i,pItem->au16AxleWeight[i]);
//		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//		g_u16CurDebugInd++;
//		g_u16CurDebugInd %= COM_NUM;
//		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//		OSSemPost(g_pSemSendThtouSP);
//#endif
//	}

	// 重新计算车型，20131122 
	Axistype = 0;		  //轴型清0	
	for (i = 0; i<pItem->u8AixGrpCnt; i++)
	{
		Axistype = Axistype*10+(pItem->au8AxleGrpType[i]&0x0F);   //轴组型 				
	}
	// 默认车型
	l_u8DefVehTypeFlag = 0;
	switch(Axistype)
	{
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 112:
		case 122:
		case 113:
		case 114:
		case 124:
		case 115:
		case 125:
		case 126:
		case 127:
		case 129:
		case 155:
		case 156:
		case 157:
		case 158:
		case 159:
		case 1126:
		case 1127:
		case 1128:
		case 1129:
			l_u8DefVehTypeFlag = 0;
			break;
		default:
			l_u8DefVehTypeFlag = 1;
			break;
	}

	if( l_u8DefVehTypeFlag )
	{
		//
		l_u32Ret = FCptDefaultVehTpye( pItem->u8AixCnt, &l_u8AxleGrpCnt, l_au8AxleGrpAxleCnt, l_au8AxleGrpType );
	
		if( SUCCESS == l_u32Ret )
		{
			pItem->u8AixGrpCnt = l_u8AxleGrpCnt;
			for( l_u8Tempi=0; l_u8Tempi<l_u8AxleGrpCnt; l_u8Tempi++ )
			{
				pItem->au8AxleGrpType[l_u8Tempi] = l_au8AxleGrpType[l_u8Tempi];
			}
			for( l_u8Tempi=0; l_u8Tempi<l_u8AxleGrpCnt; l_u8Tempi++ )
			{
				l_u32AxleGrpSum = 0;
				for( l_u8Tempj=0; l_u8Tempj<l_au8AxleGrpAxleCnt[l_u8Tempi]; l_u8Tempj++ )
				{
					l_u32AxleGrpSum += pItem->au16AxleWeight[l_u8Temp+l_u8Tempj];
				}
				l_u8Temp += l_au8AxleGrpAxleCnt[l_u8Tempi];
				pItem->au16AxleGrpWeight[l_u8Tempi] = l_u32AxleGrpSum;
			}
		}
	}
	/////////////
	pItem->m_u8PointGet = pItem->m_au8AxlePoint[0];
	for(i = 1; i<pItem->u8AixCnt; i++)
	{
		if(pItem->m_au8AxlePoint[i] < pItem->m_u8PointGet)
		{
			pItem->m_u8PointGet = pItem->m_au8AxlePoint[i];
		}
	}
	/////////////
	pItem->u16speed = l_u16VehSpeed;

	pItem->m_u16VehLength = 0;
	pItem->m_u16VehWide = 0;
	pItem->m_u16VehHeight = 0;
	l_u8FindFlag = 0;
	if(pItem->u8AixCnt > 1)
	{
		if(g_u8DevErr == 0 || g_u8DevErr == 1)
		{
			if(pItem->m_u8LaserVehID != 0)
			{
				for(i = g_u8RasterDataWriteIndex-1; i>g_u8RasterDataWriteIndex-51; i--)
				{
					for(j = 0; j<8 ; j++)
					{
						if((0x08 == (g_RasterData[i].headinfo[j].u8type & 0x0C)) || (0x0C == (g_RasterData[i].headinfo[j].u8type & 0x0C)))
						{
							if(g_RasterData[i].headinfo[j].m_u8VehID == pItem->m_u8LaserVehID)
							{
								pItem->m_u16VehLength = 0;
								pItem->m_u16VehWide = g_RasterData[i].headinfo[j].m_u16VehWide;
								pItem->m_u16VehHeight = g_RasterData[i].headinfo[j].m_u16VehHeight;
								l_u8FindFlag = 1;
							}
						}
						if(l_u8FindFlag == 1)
						{
							break;
						}
					}
					if(l_u8FindFlag == 1)
					{
						break;
					}
				}
				if(pItem->m_u16VehLength == 0)
				{
					if(pItem->m_u16VehWide == 0 || pItem->m_u16VehHeight == 0)
					{
						if(pItem->u8AixCnt == 2)
						{
							pItem->m_u16VehWide = 180; //cm
							pItem->m_u16VehHeight = 200;//cm
						}
						else
						{
							pItem->m_u16VehWide = 300;
							pItem->m_u16VehHeight = 280;
						}
					}
//					pItem->m_u16VehLength = (pItem->m_u16LaserCnt-10)*40*(pItem->u16speed/36)/100;
//					if((l_u32TotalDis > pItem->m_u16VehLength) || (l_u32TotalDis*2 < pItem->m_u16VehLength) )
					{
						pItem->m_u16VehLength = l_u32TotalDis + 20;
					}
				}
			}
			else
			{
				if(pItem->u8AixCnt == 2)
				{
					pItem->m_u16VehWide = 180; //cm
					pItem->m_u16VehHeight = 200;//cm
				}
				else
				{
					pItem->m_u16VehWide = 300;
					pItem->m_u16VehHeight = 280;
				}
				pItem->m_u16VehLength = l_u32TotalDis + 20;	  //分米
			}
		}
		else
		{
			if(pItem->u8AixCnt == 2)
			{
				pItem->m_u16VehWide = 180; //cm
				pItem->m_u16VehHeight = 200;//cm
			}
			else
			{
				pItem->m_u16VehWide = 300;
				pItem->m_u16VehHeight = 280;
			}
			pItem->m_u16VehLength = l_u32TotalDis + 20;	  //分米
		}
		switch(pItem->u8AixCnt)
		{
			case 2:
				if(pItem->m_u16VehLength > 100 || pItem->m_u16VehLength < 10)
				{
					pItem->m_u16VehLength = 70;
				}
				if(pItem->m_u16VehHeight < 100)
				{
					pItem->m_u16VehHeight = 180;
				}
				if(pItem->m_u16VehWide < 150)
				{
					pItem->m_u16VehWide = 150;
				}
			break;
			case 3:
			case 4:
			case 5:
			case 6:
				if(pItem->m_u16VehLength > 200 || pItem->m_u16VehLength < 10)
				{
					pItem->m_u16VehLength = 180;
				}
				if(pItem->m_u16VehHeight < 260)
				{
					pItem->m_u16VehHeight = 260;
				}
				if(pItem->m_u16VehWide < 200)
				{
					pItem->m_u16VehWide = 200;
				}
			break;
		}
	}
	else
	{
		pItem->m_u16VehLength = 0;
		pItem->m_u16VehWide = 0;
		pItem->m_u16VehHeight = 0;
	}
	pItem->m_u8LaserPos[0] = g_u8LaserPos[pItem->m_u8LaserVehID][0];
	pItem->m_u8LaserPos[1] = g_u8LaserPos[pItem->m_u8LaserVehID][1];
	pItem->m_u8LaserPos[2] = g_u8LaserPos[pItem->m_u8LaserVehID][2];
	pItem->m_u8LaserPos[3] = g_u8LaserPos[pItem->m_u8LaserVehID][3];
	g_u8LaserPos[pItem->m_u8LaserVehID][0] = 0;
	g_u8LaserPos[pItem->m_u8LaserVehID][1] = 0;
	g_u8LaserPos[pItem->m_u8LaserVehID][2] = 0;
	g_u8LaserPos[pItem->m_u8LaserVehID][3] = 0;

//		for (i = 0; i<Axisnum; i++)
//	{
//#if DEBUGINFO_TASK_SP_EN > 0
//		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%d    %d    ", i,pItem->au16AxleWeight[i]);
//		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//		g_u16CurDebugInd++;
//		g_u16CurDebugInd %= COM_NUM;
//		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//		OSSemPost(g_pSemSendThtouSP);
//#endif
//	}


#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\n%02u:%02u:%02u,VehType=%x,%x,%x,%x Weigh=%d,Spd=%d.\r\n",
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
						   pItem->au8AxleGrpType[0],pItem->au8AxleGrpType[1],
						   pItem->au8AxleGrpType[2],pItem->au8AxleGrpType[3],l_totalweight,pItem->u16speed/10);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	///////////////////////////////
	g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
	g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 19+pItem->u8AixGrpCnt;				
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x03;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 19+pItem->u8AixGrpCnt;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = pItem->u8AixGrpCnt;
	for (j = 0; j<pItem->u8AixGrpCnt; j++)
	{
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+j] = pItem->au8AxleGrpType[j];
	}
	
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15+Axisnum] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16+Axisnum] = 0x00;
	AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
	OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
	g_u8CurNetDebugIndex++;
	g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
	//////////////////////////////
	OSSemPost(g_pSemSendThtouSP);
#endif


////////////////////////////交调信息计算添加/////////////////////
	pItem->m_u8VehType = 0;
	pItem->m_u16LoopVal = 0;

/////////////////////////////////////////////////////////////////
/////////////////////////////车辆时间统计/////////////////////////
	if(g_Setup.u8Protocol == PROTOL_WJ_NEW)
	{
		for(i=0; i<g_AxleInfo[pItem->u16AxleList[0]].u16ChListSum; i++)
		{
			if(i == 0)
			{
				pItem->m_u32BuildTime = g_ChlData[g_AxleInfo[pItem->u16AxleList[0]].u16ChList[i]].u32uptm;
			}
			else if(pItem->m_u32BuildTime > g_ChlData[g_AxleInfo[pItem->u16AxleList[0]].u16ChList[i]].u32uptm)
			{
				pItem->m_u32BuildTime = g_ChlData[g_AxleInfo[pItem->u16AxleList[0]].u16ChList[i]].u32uptm;
			}
		}
		ReadFPGAGlobalTime(l_u32TimeNow);
		pItem->m_u32EndTime	= l_u32TimeNow;
	}
//////////////////////////////////////////////////////////////////

}

/*********************************************************************************************************
** Function name:		FVehAxleCnt1to2
** Descriptions:		
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20131020	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FVehAxleCnt1to2( Vehicle_Struct *p_pcVehData )					    
{
	if( 1 == p_pcVehData->u8AixCnt )
	{
		p_pcVehData->u8AixCnt = 2;
		p_pcVehData->u8AixGrpCnt = 2;
		//
		p_pcVehData->u16AxleList[1] = p_pcVehData->u16AxleList[0];
		p_pcVehData->au8AxleTAIType[1] = p_pcVehData->au8AxleTAIType[0];
		p_pcVehData->au8AxleGrpType[1] = p_pcVehData->au8AxleGrpType[0];
		p_pcVehData->au16AxleWeight[1] = p_pcVehData->au16AxleWeight[0] * 95 / 100;
		p_pcVehData->au16AxleGrpWeight[1] = p_pcVehData->au16AxleWeight[1];
		p_pcVehData->au16AxleDistance[0] = VEH_AXLE_DEF_DIST;
		
		//
		g_AxleInfo[p_pcVehData->u16AxleList[1]].u16speed = g_AxleInfo[p_pcVehData->u16AxleList[0]].u16speed;	// 增加车速赋值，20131203
		//
		return SUCCESS;
	}
	else
	{
		return ERR_VEH_NOTAXLECNT1;
	}
		
}

/*********************************************************************************************************
** Function name:		FVehAxleWtJudge
** Descriptions:		重量有效性判断
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20131126	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FVehAxleWtJudge( Vehicle_Struct *p_pcVehData )					    
{	
	switch( p_pcVehData->u8AixCnt )
	{
		case 1:
			if( p_pcVehData->au16AxleWeight[0] > VEH_AXLE1_MAX_WT )
			{
				p_pcVehData->au16AxleWeight[0] = VEH_AXLE1_DEF_WT;
			}
			break;
	
		case 2:			
			if( p_pcVehData->au16AxleWeight[0] > VEH_AXLE1_MAX_WT )
			{
				p_pcVehData->au16AxleWeight[0] = p_pcVehData->au16AxleWeight[0] * 35 / 100;
			}
			if( p_pcVehData->au16AxleWeight[0] > VEH_AXLE1_MAX_WT )
			{
				p_pcVehData->au16AxleWeight[0] = VEH_AXLE1_DEF_WT;
			}
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			return ERR_VEH_AXLECNT_OVER6;	
	}

	return SUCCESS;	
}

/*********************************************************************************************************
** Function name:		FVehAxleCnt0to2
** Descriptions:		
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20131029	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FVehAxleCnt0to2( Vehicle_Struct *p_pcVehData )					    
{
	if( 0 == p_pcVehData->u8AixCnt )
	{
		p_pcVehData->u8AixCnt = 2;
		p_pcVehData->u8AixGrpCnt = 2;
		//
		//p_pcVehData->u16AxleList[0] = p_pcVehData->u16AxleList[0];
		p_pcVehData->au8AxleTAIType[0] = 0x00;
		p_pcVehData->au8AxleGrpType[0] = 0x01;
		p_pcVehData->au16AxleWeight[0] = VEH_AXLE_DEF_WT;
		p_pcVehData->au16AxleGrpWeight[0] = VEH_AXLE_DEF_WT;		
		//
		//p_pcVehData->u16AxleList[1] = p_pcVehData->u16AxleList[1];
		p_pcVehData->au8AxleTAIType[1] = 0x00;
		p_pcVehData->au8AxleGrpType[1] = 0x01;
		p_pcVehData->au16AxleWeight[1] = VEH_AXLE_DEF_WT;
		p_pcVehData->au16AxleGrpWeight[1] = VEH_AXLE_DEF_WT;
		//
		p_pcVehData->au16AxleDistance[0] = VEH_AXLE_DEF_DIST;
		p_pcVehData->u16speed = VEH_AXLE_DEF_SPEED;

		g_AxleInfo[p_pcVehData->u16AxleList[0]].u16speed = VEH_AXLE_DEF_SPEED;	// 增加车速赋值， 20131203
		g_AxleInfo[p_pcVehData->u16AxleList[1]].u16speed = VEH_AXLE_DEF_SPEED;
		//
		return SUCCESS;
	}
	else
	{
		return ERR_VEH_NOTAXLECNT0;
	}
		
}


/*********************************************************************************************************
** Function name:		FVehFilterAxleWeight0
** Descriptions:		
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20131020	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FVehFilterAxleWeight0( Vehicle_Struct *p_pcVehData )					    
{
	uint8 	l_u8AxleCnt = 0;
	uint8	l_u8AxleWeight0Flag = 0;		// 重量为0标志，1表示存在
	uint8	l_au8AxleGrpAxleCnt[ZHOUSIZE];
	uint8	l_au8AxleGrpType[ZHOUSIZE];
	uint8	l_u8AxleGrpCnt = 0;
	uint8	l_u8Tempi = 0;
	uint8	l_u8Tempj = 0;
	uint32	l_u32Ret = 0;
	uint32	l_u32AxleGrpSum = 0;
	uint8	l_u8Temp = 0;

	//
	memset( l_au8AxleGrpAxleCnt, 0, ZHOUSIZE );

	//
	l_u8AxleCnt = p_pcVehData->u8AixCnt;

	for( l_u8Tempi=0; l_u8Tempi<l_u8AxleCnt; l_u8Tempi++ )
	{
		if( p_pcVehData->au16AxleWeight[l_u8Tempi] < VEH_AXLE_MIN_WEIGHT )
		{
			if( l_u8Tempi != l_u8AxleCnt-1 )
			{
				for( l_u8Tempj=l_u8Tempi+1; l_u8Tempj<l_u8AxleCnt; l_u8Tempj++ )
				{
					p_pcVehData->au16AxleWeight[l_u8Tempj-1] = p_pcVehData->au16AxleWeight[l_u8Tempj];
					p_pcVehData->u16AxleList[l_u8Tempj-1] = p_pcVehData->u16AxleList[l_u8Tempj];					
				}
				l_u8Tempi--;// 因为当前位置已经为后一个重量，还没有判断是否为0 wujinlong 20131101
			}
			
			//
			if( p_pcVehData->u8AixCnt > 0 )
			{
				p_pcVehData->u8AixCnt--;
				l_u8AxleCnt--;
				l_u8AxleWeight0Flag = 1;
			}
			
		}
	}

	//
	if( !l_u8AxleWeight0Flag )
	{
		return ERR_VEH_NOTWEGHT0;	
	}

	if( p_pcVehData->u8AixGrpCnt > p_pcVehData->u8AixCnt )
	{
		p_pcVehData->u8AixGrpCnt = p_pcVehData->u8AixCnt;
	}

	//
	l_u32Ret = FCptDefaultVehTpye( p_pcVehData->u8AixCnt, &l_u8AxleGrpCnt, l_au8AxleGrpAxleCnt, l_au8AxleGrpType );

	if( SUCCESS == l_u32Ret )
	{
		p_pcVehData->u8AixGrpCnt = l_u8AxleGrpCnt;
		for( l_u8Tempi=0; l_u8Tempi<l_u8AxleGrpCnt; l_u8Tempi++ )
		{
			p_pcVehData->au8AxleGrpType[l_u8Tempi] = l_au8AxleGrpType[l_u8Tempi];
		}
		for( l_u8Tempi=0; l_u8Tempi<l_u8AxleGrpCnt; l_u8Tempi++ )
		{
			l_u32AxleGrpSum = 0;
			for( l_u8Tempj=0; l_u8Tempj<l_au8AxleGrpAxleCnt[l_u8Tempi]; l_u8Tempj++ )
			{
				l_u32AxleGrpSum += p_pcVehData->au16AxleWeight[l_u8Temp+l_u8Tempj];
			}
			l_u8Temp += l_au8AxleGrpAxleCnt[l_u8Tempi];
			p_pcVehData->au16AxleGrpWeight[l_u8Tempi] = l_u32AxleGrpSum;
		}
	}
	else
	{
		// 默认车型计算错误，需要处理


		return ERR_VEH_DEFAULTVEH;
	}
	
	//
	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FCptDefaultVehTpye
** Descriptions:		
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20131020	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FCptDefaultVehTpye( uint8 p_pu8AxleCnt, uint8 *p_pu8AxleGrpCnt, uint8 *p_pau8AxleGrpAxleCnt, uint8 *p_pau8AxleGrpType )					    
{
	switch( p_pu8AxleCnt )
	{
		case 6:
			// 159
			*p_pu8AxleGrpCnt = 3;
			*(p_pau8AxleGrpAxleCnt+0) = 1;
			*(p_pau8AxleGrpAxleCnt+1) = 2;
			*(p_pau8AxleGrpAxleCnt+2) = 3;
			*(p_pau8AxleGrpType+0) = 0x01;
			*(p_pau8AxleGrpType+1) = 0x35;
			*(p_pau8AxleGrpType+2) = 0x59;
			break;

		case 5:			
			// 155
			*p_pu8AxleGrpCnt = 3;
			*(p_pau8AxleGrpAxleCnt+0) = 1;
			*(p_pau8AxleGrpAxleCnt+1) = 2;
			*(p_pau8AxleGrpAxleCnt+2) = 2;
			*(p_pau8AxleGrpType+0) = 0x01;
			*(p_pau8AxleGrpType+1) = 0x35;
			*(p_pau8AxleGrpType+2) = 0x35;
			break;

		case 4:
			// 115
			*p_pu8AxleGrpCnt = 3;
			*(p_pau8AxleGrpAxleCnt+0) = 1;
			*(p_pau8AxleGrpAxleCnt+1) = 1;
			*(p_pau8AxleGrpAxleCnt+2) = 2;
			*(p_pau8AxleGrpType+0) = 0x01;
			*(p_pau8AxleGrpType+1) = 0x01;
			*(p_pau8AxleGrpType+2) = 0x35;
			break;

		case 3:
			// 15
			*p_pu8AxleGrpCnt = 2;
			*(p_pau8AxleGrpAxleCnt+0) = 1;
			*(p_pau8AxleGrpAxleCnt+1) = 2;
			*(p_pau8AxleGrpType+0) = 0x01;
			*(p_pau8AxleGrpType+1) = 0x35;
			break;

		case 2:
			// 11
			*p_pu8AxleGrpCnt = 2;
			*(p_pau8AxleGrpAxleCnt+0) = 1;
			*(p_pau8AxleGrpAxleCnt+1) = 1;
			*(p_pau8AxleGrpType+0) = 0x01;
			*(p_pau8AxleGrpType+1) = 0x01;
			break;

		default:
			return FAIL;
	}

	//
	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		TaskRec6
** Descriptions:		计算重量
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wujinlong		  
** Created Date:		20110512	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  	zhangtonghan	
** Modified date:	  	20180625
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void TaskRec6(void *tdata)					    
{
	TranZT_Struct 	l_TranZT[64];
	XieListZT		l_XieZTList;
	HideZT_Struct 	l_HideZTLine[3];
 	Axle_Struct * 	l_pAxleTempInfo;//*l_AxleOneInfo;
	TrigerID_Struct l_MachTrigID;	//计算轴距用
	TrigID4AxleDis  l_TrigID4AxleDisCal;    //计算轴距需要的窄条
	uint8 			AxisNum;  //归属车辆的第几轴
	volatile uint8 	WbNums;//弯板数（该轴有几根窄条）
	uint16 l_u16Index = 0,l_u16tmp = 0;//临时变量
	uint8  l_u8tempi;
	uint32 width[12]={0};//窄条宽度
 	volatile int32 dis[64] = {0};//fangrui 窄条间距存储变量
	int32 spd=0;//初算速度
//	int32 spdtemp;//同列窄条平均速度
	uint32 Axisspeed = 0;//轴速，m/s，放大1000倍。
	int32 Tyrtypetemp = 0;//计算胎型中间量
	int32 Tyrtype;//胎型
	uint32 tyrweight;
	uint32 Alltyrweight;
//	int8 Dir;
	uint8 WalkAndRunFlag;    //走走停停标识, 1:中航窄条走走停停,2:数字窄条走走停停	
// 	volatile int32 Ls[CHANNELNUM]={0};//弹性体宽度	 
	uint16 l_u16AGrain[CHANNELNUM];//学习算法
	uint32 l_u32TimeNow;
	uint8  l_u8AgaintempId;
	uint8  l_u8FirPoint=100;
	uint8  l_u8SecPoint = 100;
	uint8  l_u8Err;	
	uint16 l_u16SendLen;
//	void *AddressForPo[3];
	uint8 l_u8DelteFlag;
	volatile uint32 Spddd,td,tu;
	/* 压三边补偿变量(liumingxin)(20170621) */
	uint32  passthrlinewgt[3];       // 压三边各列重量(6.9)
//	uint8 	ZTakePos; 
	int8 	Offset[4];
//	uint8 	AccleIndex;
	TranZT_Struct l_TranDigitZT[3][2];     //按列存储数字窄条每根窄条的信息 lishui
	int16  l_n16DigitZT_Pos[3][4];         //数字窄条位置,用于读取窄条间距      
	uint32 l_u32DigitYZsumweight[3];       //数字窄条一字部分每列的总重
	uint32 l_u32sololinelft[2],l_u32sololinerght[2];
	
	/* 轴异常情况(liumingxin)(20180104) */
	uint8  l_u8ZTID_Buffer[12];          //存储每根轴匹配的窄条ID
	uint8  l_u8ZTCnt_Buffer[12];         //计算每根窄条出现次数
	uint32 l_u32ZTSum_Buffer[12];        //存储每根窄条的积分和
	uint32 l_u32ColumSpd_Buffer[3];      //保存列的速度
	uint32 l_u32ZTSumMax = 0;            //窄条积分和最大值
	uint32 l_u32ZTSumAver = 0;           //该轴去掉一个最大值后窄条积分和的平均值
//	uint32 l_u32WheelSpd[2];             //存储左轮速度和右轮速度
	//uint32 l_u32ZTSumMin = 0xFFFF;       //窄条积分和最小值
	uint8  l_u8ZTIDMin = 0xff;           //该轴传过来的窄条ID的最小值
//	int    l_n8SameZTID;                 //存储该轴相同ID号
	/* end (liumingxin)(20180104) */
	
	/* 计算轴距 */
	uint8   preAxlePos = 0;            //存储前一轴的位置
	/* end */
	
	/* 违例码 */
	uint8 l_u8ViolationCode = 0;    //违例码
	uint8 l_u8ViolationCodeOutput = 0;    // 0x80:走走停停,0x40:跨界,0x20:点刹,0x10:S弯,0x08:加速,0x40:减速,0x20:压三边,0x01:跨道(liumingxin)(20170808)
	uint8 l_u8AccOrDecSpd = 0;      // 0:无加减速, 1:加速, 2:减速
	/* end */
	
	uint8 	whel_minnum=0;
	uint8 	whel_minlane=0;
	uint8	HideZTMapPost=0;
	int32 	speed[12];//实用速度，放大1000倍
//	uint16 	l_u16gap=0;
//	void*	l_u32XieAddr[5];
//	uint8	l_u8XieMode; //0:默认;1:寻找中间位置;2:寻找最多的个数点
	void*	l_u32WalkRunAddr[4];
	void*	l_u32JudgWalkRunAddr[4];
	uint8	l_u8WalkRunFlag;
	uint16 	AxleDistence;
	uint32 	l_u32AxleAccTim;
	void * 	l_u32AddrAxleDist[5];
	void * 	l_u32AddrFcap[4];
	void *  AddressForPo[3];
//	uint8	l_u8SoloLine[3]={0xff,0xff,0xff};  //隐藏窄条当前列只有一根窄条
	VehicleInfForLoop_Struct (*VehStruct)[CHL_SAVEVEH];
	uint8 	l_u8MatchFlag;//布局与正反向组合
//	uint16  l_u16preAxleSpd;
	void * ptemp;
	// 等待初始化结束
	uint8 l_u8headortail=0;
	
	/**** 存储每根窄条重量和速度 ****/
//	uint8  ret = 0;
	uint32 l_u32ZTWgt[12];     //存储每根窄条的重量,单位kg
	uint32 l_u32ZTSpd[12];     //存储每根窄条的速度,单位mm/s
	uint16 l_u16ZTPos[12];     //存储窄条Pos
	uint32 l_u32WheelWgt[2];   //0:左轮重,1:右轮重,单位kg
	uint32 l_u32WheelSpd[2];   //0:左轮速度,1:右轮速度,单位mm/s
	uint8  l_u8ZTCnt = 0;
	uint8 i;
	
//	uint32 l_u32AxleWgt = 0;  //轴重,单位kg
//	uint32 l_u32AxleSpd = 0;  //轴速,单位0.1km/h
	/**** end ****/
	
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}
// 	for(l_u16Index=0;l_u16Index<64;l_u16Index++)
// 	{
// 		  dis[l_u16Index]=PROMODPARAM.m_au16ZTGap[l_u16Index];//间距转换 fangrui
// 		  if(PROMODPARAM.m_u8EnableFlag[6] == 1)
// 		  {
// 		  	 Ls[l_u16Index] = 30*POINTRATE/1000;   //SETUPALIAS.u8Diszt[l_u16Index]*POINTRATE/1000;
// 		  }
// 		  else
// 		  {
// 		  	 Ls[l_u16Index] = 50*POINTRATE/1000;
// 		  }
// 	}
	

	OSTimeDly(100*OS_TICKS_PER_SEC/1000);
	while(1)
	{
		OSTimeDly(3);
		
		if(OSSemAccept(g_pSemTask6Param))
		{  	

			for(l_u16Index=0;l_u16Index<48;l_u16Index++)
			{
					dis[l_u16Index]=PROMODPARAM.m_au16ZTGap[l_u16Index];//间距转换 fangrui
// 				  if(PROMODPARAM.m_u8EnableFlag[6] == 1)
// 				  {
// 				  	Ls[l_u16Index] = 30*POINTRATE/1000;//SETUPALIAS.u8Diszt[l_u16Index]*POINTRATE/1000;
// 				  }
// 				  else
// 				  {
// 				  	Ls[l_u16Index] = 50*POINTRATE/1000;
// 				  }
			}		  
		}	
		//轴重计算
		l_pAxleTempInfo	= OSQAccept(g_pQAxleCalc);		//20121114
		if(l_pAxleTempInfo != (Axle_Struct *)0)
		{
			if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //一字布局
			{
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nZTNum=%d, ID=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->u16ChListSum,
										 g_ChlData[l_pAxleTempInfo->u16ChList[0]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16ChList[1]].u8Id,g_ChlData[l_pAxleTempInfo->u16ChList[2]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16ChList[3]].u8Id,g_ChlData[l_pAxleTempInfo->u16ChList[4]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16ChList[5]].u8Id);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nZTPos=%d,Pos=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->u16ChListSum,
										 l_pAxleTempInfo->u16ChList[0],l_pAxleTempInfo->u16ChList[1],l_pAxleTempInfo->u16ChList[2],
										 l_pAxleTempInfo->u16ChList[3],l_pAxleTempInfo->u16ChList[4],l_pAxleTempInfo->u16ChList[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else    //交错布局
			{
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nLeftNum=%d,  ID=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->LeftWheelZTCnt,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[0]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16LeftWheeList[1]].u8Id,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[2]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16LeftWheeList[3]].u8Id,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[4]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16LeftWheeList[5]].u8Id);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nLeftWhel=%d, Take=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->LeftWheelZTCnt,l_pAxleTempInfo->u8LeftWheelTakeZT[0],
										 l_pAxleTempInfo->u8LeftWheelTakeZT[1],l_pAxleTempInfo->u8LeftWheelTakeZT[2],
										 l_pAxleTempInfo->u8LeftWheelTakeZT[3],l_pAxleTempInfo->u8LeftWheelTakeZT[4],
										 l_pAxleTempInfo->u8LeftWheelTakeZT[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nLeftPos=%d,  Pos=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->LeftWheelZTCnt,l_pAxleTempInfo->u16LeftWheeList[0],
										 l_pAxleTempInfo->u16LeftWheeList[1],l_pAxleTempInfo->u16LeftWheeList[2],
										 l_pAxleTempInfo->u16LeftWheeList[3],l_pAxleTempInfo->u16LeftWheeList[4],
										 l_pAxleTempInfo->u16LeftWheeList[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRightNum=%d, ID=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->RightWheelZTCnt,g_ChlData[l_pAxleTempInfo->u16RightWheeList[0]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16RightWheeList[1]].u8Id,g_ChlData[l_pAxleTempInfo->u16RightWheeList[2]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16RightWheeList[3]].u8Id,g_ChlData[l_pAxleTempInfo->u16RightWheeList[4]].u8Id,
										 g_ChlData[l_pAxleTempInfo->u16RightWheeList[5]].u8Id);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRightWhel=%d,Take=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->RightWheelZTCnt,l_pAxleTempInfo->u8RightWheelTakeZT[0],
										 l_pAxleTempInfo->u8RightWheelTakeZT[1],l_pAxleTempInfo->u8RightWheelTakeZT[2],
										 l_pAxleTempInfo->u8RightWheelTakeZT[3],l_pAxleTempInfo->u8RightWheelTakeZT[4],
										 l_pAxleTempInfo->u8RightWheelTakeZT[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRightPos=%d, Pos=%02u,%02u,%02u,%02u,%02u,%02u.", 
										 l_pAxleTempInfo->RightWheelZTCnt,l_pAxleTempInfo->u16RightWheeList[0],
										 l_pAxleTempInfo->u16RightWheeList[1],l_pAxleTempInfo->u16RightWheeList[2],
										 l_pAxleTempInfo->u16RightWheeList[3],l_pAxleTempInfo->u16RightWheeList[4],
										 l_pAxleTempInfo->u16RightWheeList[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			//////////////////学习程序///////////////////////
			
			if((PROMODPARAM.m_u8StudyAlgEn[0] == 1) || (PROMODPARAM.m_u8StudyAlgEn[1] == 1)
				|| (PROMODPARAM.m_u8StudyAlgEn[2] == 1) || (PROMODPARAM.m_u8StudyAlgEn[3] == 1)
				|| (PROMODPARAM.m_u8StudyAlgEn[4] == 1))
			{
				for(l_u8tempi =0;l_u8tempi < CHANNELNUM; l_u8tempi++)
				{
					l_u16AGrain[l_u8tempi] = SETUPALIAS.an32AxGain[l_u8tempi];
					SETUPALIAS.an32AxGain[l_u8tempi] = SETUPALIAS.an16BDAxGain[l_u8tempi];
				}
			}

			memset(&spd,0,sizeof(uint32));
			memset(speed,0,sizeof(uint32)*12);
			memset(width,0,sizeof(uint32)*12);
			memset(l_TranZT,0,sizeof(TranZT_Struct)*64);
			memset(l_HideZTLine,0,sizeof(HideZT_Struct)*3);		
			memset(l_TranDigitZT,0,sizeof(TranZT_Struct)*6);
			memset(&l_MachTrigID,0,sizeof(TrigerID_Struct));
			memset(&l_TrigID4AxleDisCal,-1,sizeof(TrigID4AxleDis));
			memset(passthrlinewgt, 0, sizeof(uint32)*3);  //初始化压三边补偿各列的重量(liumingxin)(20170621)
			memset(l_n16DigitZT_Pos,-1,sizeof(int16)*12);
			memset(Offset,-1,sizeof(int8)*4);
			memset(l_u32DigitYZsumweight,0,sizeof(int32)*3);
			memset(l_u8ZTID_Buffer, 0, sizeof(uint8)*12);
			memset(l_u8ZTCnt_Buffer, 0, sizeof(uint32)*12);
			memset(l_u32ZTSum_Buffer, 0, sizeof(uint32)*12);
			memset(l_u32ColumSpd_Buffer, 0, sizeof(uint32)*3);
			memset(&l_XieZTList,0,sizeof(XieListZT));
			memset(l_u32WheelSpd,0,sizeof(l_u32WheelSpd));
			
			memset(l_u32ZTWgt, 0, sizeof(uint32)*12);
			memset(l_u32ZTSpd, 0, sizeof(uint32)*12);
			memset(l_u32WheelWgt, 0, sizeof(uint32)*2);
			memset(l_u32WheelSpd, 0, sizeof(uint32)*2);
			memset(l_u16ZTPos, 0, sizeof(uint16)*12);

//			ret = 0;
			tyrweight = 0;
			Alltyrweight=0;
			Axisspeed = 0;
//			spdtemp = 0;
			Tyrtype = 0;
			Tyrtypetemp = 0;
			l_u8AgaintempId = 0;
			l_u8DelteFlag = 0;
			WalkAndRunFlag  = 0;
//			AccleIndex = 0;
			memset(&l_u32sololinelft,0,sizeof(uint32)*2);
			memset(&l_u32sololinerght,0,sizeof(uint32)*2);
			l_u32ZTSumMax = 0;            //窄条积分和最大值
			l_u32ZTSumAver = 0;           //该轴去掉一个最大值后窄条积分和的平均值
			l_u8ZTIDMin = 0xff;           //该轴传过来的窄条ID的最小值
			preAxlePos = 0;    //存储前一轴的位置
			l_u8ViolationCode = 0;
			l_u8ViolationCodeOutput = 0;
			l_u8AccOrDecSpd = 0;
//			l_u8XieMode = 0;
			l_u8WalkRunFlag=0;
			AxleDistence = 0;
			l_u32AxleAccTim =0;
			l_u8headortail = 0;
			l_u8MatchFlag = 0;
			l_u8ZTCnt = 0;
//			l_u32AxleWgt = 0;  //轴重,单位kg
//			l_u32AxleSpd = 0;  //轴速,单位0.1km/h
			
			AxisNum =  l_pAxleTempInfo->u8OrderID;//当前轴是第几根轴
			if(l_pAxleTempInfo->u8OrderID==1)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //一字布局
				{
					AxisNum=1;
					
				}
				else     //交错
				{
					AxisNum=1;
					if(l_pAxleTempInfo->LeftWheelZTCnt == 1) 
						l_pAxleTempInfo->LeftWheelZTCnt = 1;
				}
			}
			if(l_pAxleTempInfo->u8OrderID==2)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)   //一字布局
				{
					AxisNum=2;
				}
				else      //交错
				{
					AxisNum=2;
				}
			}
			if(l_pAxleTempInfo->u8OrderID==3)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)     //一字布局
				{
					AxisNum=3;
				}
				else    //交错
				{
					AxisNum=3;
				}
			}
			if(l_pAxleTempInfo->u8OrderID==4)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)   //一字布局
				{
					AxisNum=4;
				}
				else    //交错
				{
					AxisNum=4;
				}
			}
			if(l_pAxleTempInfo->u8OrderID==5)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //一字布局
				{
					AxisNum=5;
				}
				else    //交错
				{
					AxisNum=5;
				}
			}
			if(l_pAxleTempInfo->u8OrderID==6)
			{
				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //一字布局
				{
					AxisNum=6;
				}
				else    //交错
				{
					AxisNum=6;
				}
			}
			//首轴将走走停停标志置0
// 			if((l_pAxleTempInfo->u8OrderID == 1) && (g_RunAndWalkEnable != 0))
// 			{
// 				if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //隐藏
// 				{
// 					g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].RunAndWalkFlag = 0;
// 				}
// 				else    //交错
// 				{
// 					if(l_pAxleTempInfo->Dir == 1)
// 						g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].RunAndWalkFlag = 0;
// 					else
// 						g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].RunAndWalkFlag = 0;
// 				}
// 			}
			if(l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)    //一字布局
			{
				goto BeginAlgor;
			}
			else
			{
				//先判断该轴是不是走走停停的
				l_u32JudgWalkRunAddr[0] = l_pAxleTempInfo;
				l_u32JudgWalkRunAddr[1] = &l_u8WalkRunFlag;
				FJudgWalkRun(l_u32JudgWalkRunAddr);
				//l_u8WalkRunFlag = 1;
				if(l_u8WalkRunFlag)
				{
					l_u32WalkRunAddr[0] = l_pAxleTempInfo;
	// 					l_u32WalkRunAddr[1] = &Tyrtypetemp;
					l_u32WalkRunAddr[1] = &Axisspeed;
					l_u32WalkRunAddr[2] = &Alltyrweight;

					FCalcWalkRun(l_u32WalkRunAddr);
					goto ENDWEIGHT;
				}
				else 
					goto BeginAlgor;
			}
BeginAlgor:
			if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错布局
			{
				FCalcZTWgtAndSpeed(l_pAxleTempInfo, l_u32ZTWgt, l_u32ZTSpd, l_u32WheelWgt, l_u32WheelSpd);
				
				if(l_pAxleTempInfo->LeftWheelZTCnt > 0 && l_pAxleTempInfo->RightWheelZTCnt > 0)
				{
					Alltyrweight = l_u32WheelWgt[0] + l_u32WheelWgt[1];    //轴总重,单位kg
				}
				else if(l_pAxleTempInfo->LeftWheelZTCnt > 0 && l_pAxleTempInfo->RightWheelZTCnt == 0)    //无右轮
				{
					Alltyrweight = l_u32WheelWgt[0] * 2;    //轴总重,单位kg
				}
				else if(l_pAxleTempInfo->LeftWheelZTCnt == 0 && l_pAxleTempInfo->RightWheelZTCnt > 0)    //无左轮
				{
					Alltyrweight = l_u32WheelWgt[1] * 2;    //轴总重,单位kg
				}
				Axisspeed = ((l_u32WheelSpd[0] > 0) && (l_u32WheelSpd[1] > 0)) ? ((l_u32WheelSpd[0] + l_u32WheelSpd[1]) / 2) : F_Max(l_u32WheelSpd[0], l_u32WheelSpd[1]);   //轴速,单位mm/s
			
				for(i = 0; i < l_pAxleTempInfo->LeftWheelZTCnt; i++)
				{
					l_u16ZTPos[l_u8ZTCnt] = l_pAxleTempInfo->u16LeftWheeList[i];
					l_u8ZTCnt++;
				}
				for(i = 0; i < l_pAxleTempInfo->RightWheelZTCnt; i++)
				{
					l_u16ZTPos[l_u8ZTCnt] = l_pAxleTempInfo->u16RightWheeList[i];
					l_u8ZTCnt++;
				}
				
				memset(l_pAxleTempInfo->ZTWgt, 0, sizeof(uint32)*12);
				memset(l_pAxleTempInfo->WheelWgt, 0, sizeof(uint32)*2);
				memset(l_pAxleTempInfo->ZTPos, 0, sizeof(uint16)*12);
				memcpy(l_pAxleTempInfo->ZTWgt, l_u32ZTWgt, sizeof(uint32)*12);
				memcpy(l_pAxleTempInfo->WheelWgt, l_u32WheelWgt, sizeof(uint32)*2);
				memcpy(l_pAxleTempInfo->ZTPos, l_u16ZTPos, sizeof(uint16)*12);
			}	
			else		//一字布局
			{
				tyrweight=0;
				
			 	WbNums  =  l_pAxleTempInfo->u16ChListSum;
				whel_minnum = g_ChlData[l_pAxleTempInfo->u16ChList[0]].u8Id;
				for(l_u16Index=1;l_u16Index<WbNums;l_u16Index++)
				{
					whel_minnum = IsBigFlag(whel_minnum,g_ChlData[l_pAxleTempInfo->u16ChList[l_u16Index]].u8Id) ? g_ChlData[l_pAxleTempInfo->u16ChList[l_u16Index]].u8Id:whel_minnum;
				}
				whel_minlane = whel_minnum/HideZTRow;

				for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
				{
					l_u16tmp=l_pAxleTempInfo->u16ChList[l_u16Index];
					l_u8AgaintempId=g_ChlData[l_u16tmp].u8Id;
					HideZTMapPost = l_u8AgaintempId - whel_minlane*HideZTRow;
					l_HideZTLine[l_u8AgaintempId/HideZTRow - whel_minlane].ZTPos[l_HideZTLine[l_u8AgaintempId/HideZTRow - whel_minlane].ZTCnt] = l_u16tmp;
					l_HideZTLine[l_u8AgaintempId/HideZTRow - whel_minlane].ZTCnt++;
					
					l_TranZT[HideZTMapPost].u32chlSum = g_ChlData[l_u16tmp].u32chlSum*g_Setup.an32AxGain[l_u8AgaintempId]/1000;
					l_TranZT[HideZTMapPost].u32uptm = g_ChlData[l_u16tmp].u32uptm;//上称时间
					l_TranZT[HideZTMapPost].u32dntm = g_ChlData[l_u16tmp].u32dntm;//下称时间
					l_TranZT[HideZTMapPost].MaxValueTime = g_ChlData[l_u16tmp].u32Maxtm;    //最大值时刻
					l_TranZT[HideZTMapPost].wmax = g_ChlData[l_u16tmp].u16ptMaxValue;//最大值
					l_TranZT[HideZTMapPost].ZTPos = g_ChlData[l_u16tmp].u16SelfID;
					l_TranZT[HideZTMapPost].u32realCnt = g_ChlData[l_u16tmp].u32realCnt;					
				}
				
				for(l_u16Index=0;l_u16Index < 3;l_u16Index++)  //一个轴最多压三列
				{
					if(l_HideZTLine[l_u16Index].ZTCnt == 2)
					{
						l_HideZTLine[l_u16Index].ZTSpd = PROMODPARAM.m_au16ZTGap[g_ChlData[l_TranZT[2*l_u16Index].ZTPos].u8Id]*600/GetDiff(l_TranZT[2*l_u16Index].u32uptm,l_TranZT[2*l_u16Index+1].u32uptm);
						Axisspeed +=l_HideZTLine[l_u16Index].ZTSpd;
					}
// 					else if(l_HideZTLine[l_u16Index].ZTCnt == 1)
// 					{
// 							l_u8SoloLine[l_u16Index] = l_u16Index;
// 					}
				}
				for(l_u16Index=0;l_u16Index < 3;l_u16Index++) //将只有一根窄条的列速度设置为旁边列速度
				{
					if((l_HideZTLine[l_u16Index].ZTCnt == 1) && (l_u16Index == 0))
					{
						l_HideZTLine[l_u16Index].ZTSpd = (l_HideZTLine[l_u16Index+1].ZTCnt == 2)? l_HideZTLine[l_u16Index+1].ZTSpd:l_HideZTLine[l_u16Index+2].ZTSpd;
					}
					else if((l_HideZTLine[l_u16Index].ZTCnt == 1) && (l_u16Index == 2))
					{
						l_HideZTLine[l_u16Index].ZTSpd = (l_HideZTLine[l_u16Index-1].ZTCnt == 2)? l_HideZTLine[l_u16Index-1].ZTSpd:l_HideZTLine[l_u16Index-2].ZTSpd;
					}
					else if(l_HideZTLine[l_u16Index].ZTCnt == 1)
					{
						l_HideZTLine[l_u16Index].ZTSpd = l_HideZTLine[l_u16Index-1].ZTSpd;
					}
				}
				//计算一字布局重量
				for(l_u16Index=0;l_u16Index < 3;l_u16Index++)
				{
					tyrweight = 0;
					for(l_u16tmp=0;l_u16tmp<l_HideZTLine[l_u16Index].ZTCnt;l_u16tmp++)
					{
						if(l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum > 120000)
						{
							if(g_ChlData[l_TranZT[HideZTRow*l_u16Index+l_u16tmp].ZTPos].u8BoardType == 1) //中航窄条
							{
								tyrweight += l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum/180*l_HideZTLine[l_u16Index].ZTSpd;
								l_u32ZTWgt[l_u8ZTCnt] = l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum/180*l_HideZTLine[l_u16Index].ZTSpd/100;
							}
							else
							{
								tyrweight += l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum/300*l_HideZTLine[l_u16Index].ZTSpd;
								l_u32ZTWgt[l_u8ZTCnt] = l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum/300*l_HideZTLine[l_u16Index].ZTSpd/100;
							}
						}
						else	
						{							
							if(g_ChlData[l_TranZT[HideZTRow*l_u16Index+l_u16tmp].ZTPos].u8BoardType == 1) //中航窄条
							{
								tyrweight += l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum*l_HideZTLine[l_u16Index].ZTSpd/180;
								l_u32ZTWgt[l_u8ZTCnt] = l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum*l_HideZTLine[l_u16Index].ZTSpd/180/100;
							}
							else
							{
								tyrweight += l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum*l_HideZTLine[l_u16Index].ZTSpd/300;
								l_u32ZTWgt[l_u8ZTCnt] = l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum*l_HideZTLine[l_u16Index].ZTSpd/300/100;
							}
						}
						l_u16ZTPos[l_u8ZTCnt] = l_TranZT[HideZTRow*l_u16Index+l_u16tmp].ZTPos;  //存储窄条Pos
						l_u8ZTCnt++;
						//if(l_HideZTLine[l_u16Index].ZTSpd > 0)
							Tyrtypetemp += l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32chlSum/GetDiff(l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32dntm,l_TranZT[HideZTRow*l_u16Index+l_u16tmp].u32uptm)*100000/l_HideZTLine[l_u16Index].ZTSpd;//胎型计算
					}
					if(l_HideZTLine[l_u16Index].ZTCnt > 0)
						Alltyrweight += tyrweight/l_HideZTLine[l_u16Index].ZTCnt/100;
				}
				Axisspeed = 10*Axisspeed/2;  //mm/s
				Tyrtypetemp = Tyrtypetemp/WbNums;
				
				memset(l_pAxleTempInfo->ZTWgt, 0, sizeof(uint32)*12);
				memset(l_pAxleTempInfo->WheelWgt, 0, sizeof(uint32)*2);
				memset(l_pAxleTempInfo->ZTPos, 0, sizeof(uint16)*12);
				memcpy(l_pAxleTempInfo->ZTWgt, l_u32ZTWgt, sizeof(uint32)*12);
				memcpy(l_pAxleTempInfo->WheelWgt, l_u32WheelWgt, sizeof(uint32)*2);
				memcpy(l_pAxleTempInfo->ZTPos, l_u16ZTPos, sizeof(uint16)*12);
			}
			
			
ENDWEIGHT:			
			if(Tyrtypetemp<63)
			{
				Tyrtype = 1;
			}
			else
			{
				Tyrtype = 2;
			}
			if(AxisNum==1)
			{
				Tyrtype = 1;
			}	
			// 赋值
			l_pAxleTempInfo->WheelSpd = Axisspeed/10;
			l_pAxleTempInfo->u16speed = Axisspeed*36/10/1000 * 10;// 速度单位0.1Km/h  wujinlong 20130110			
			l_pAxleTempInfo->s32AxleWeight = Alltyrweight;	
			l_pAxleTempInfo->u16AixsType  = Tyrtype;
			
			
			
			
			
			/**** 判断轴异常情况 ****/
			if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错布局
			{
				for(l_u8tempi = 0; l_u8tempi< l_pAxleTempInfo->LeftWheelZTCnt; l_u8tempi++)
				{
					l_u8ZTID_Buffer[l_u8tempi] = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id;    //存储窄条ID,用于轴异常的判断
					l_u32ZTSum_Buffer[l_u8tempi] = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32chlSum;      //存储每根窄条的积分和,用于判断积分和异常
				}
				for(l_u8tempi = 0; l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt; l_u8tempi++)
				{
					l_u8ZTID_Buffer[l_u8tempi+l_pAxleTempInfo->LeftWheelZTCnt] = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id;    //存储窄条ID,用于轴异常的判断
					l_u32ZTSum_Buffer[l_u8tempi+l_pAxleTempInfo->LeftWheelZTCnt] = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32chlSum;      //存储每根窄条的积分和,用于判断积分和异常
				}
				WbNums = l_pAxleTempInfo->LeftWheelZTCnt + l_pAxleTempInfo->RightWheelZTCnt;
				if(WbNums < 6)   
				{
					//l_pAxleTempInfo->u8AxleErrorType |= 0x08;
				}
			}
			else    //隐藏板块
			{
				WbNums = l_pAxleTempInfo->u16ChListSum;
				for(l_u8tempi = 0; l_u8tempi< WbNums; l_u8tempi++)
				{
					l_u8ZTID_Buffer[l_u8tempi] = g_ChlData[l_pAxleTempInfo->u16ChList[l_u8tempi]].u8Id;    //存储窄条ID,用于轴异常的判断
					l_u32ZTSum_Buffer[l_u8tempi] = g_ChlData[l_pAxleTempInfo->u16ChList[l_u8tempi]].u32chlSum;      //存储每根窄条的积分和,用于判断积分和异常
				}
				if(WbNums < 4)    //每根轴匹配的窄条数小于4个
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x08;
				}
			}

			//判断该轴传过来的窄条ID是否有两个相同的
			for(l_u8tempi = 0; l_u8tempi< WbNums; l_u8tempi++)
			{
				if((l_u8ZTID_Buffer[l_u8tempi] < l_u8ZTIDMin) && (l_u8tempi < 12))
				{
					l_u8ZTIDMin = l_u8ZTID_Buffer[l_u8tempi];    //该轴的窄条ID最小值
				}
				if((l_u32ZTSum_Buffer[l_u8tempi] > l_u32ZTSumMax) && (l_u8tempi < 12))
				{
					l_u32ZTSumMax = l_u32ZTSum_Buffer[l_u8tempi];    //找出该轴所有窄条的最大积分和
				}
				l_u32ZTSumAver +=  l_u32ZTSum_Buffer[l_u8tempi];
			}
			if(WbNums > 1)
			{
				l_u32ZTSumAver = (l_u32ZTSumAver - l_u32ZTSumMax) / (WbNums - 1);    //去掉最大值后的积分和的均值
				if(l_u32ZTSumMax > 100*l_u32ZTSumAver)    //最大积分和远大于平均积分和
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x01;
				}
			}
			for(l_u8tempi = 0; l_u8tempi< WbNums; l_u8tempi++)    //查找是否有重复的窄条
			{
				if(l_u8ZTID_Buffer[l_u8tempi] >= l_u8ZTIDMin)
				{
					l_u8ZTCnt_Buffer[l_u8ZTID_Buffer[l_u8tempi]-l_u8ZTIDMin]++;
				}
				if(l_u8ZTCnt_Buffer[l_u8ZTID_Buffer[l_u8tempi]-l_u8ZTIDMin] > 1)
				{
					//l_pAxleTempInfo->u8AxleErrorType |= 0x04;
//					l_n8SameZTID = l_u8ZTID_Buffer[l_u8tempi];
					break;
				}
			}
			
			//判断不同列(轮速度差是否较大)
			if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错布局
			{
				if((l_u32WheelSpd[0] > 0 && l_u32WheelSpd[1] > 0) && ((l_u32WheelSpd[0] * 100 > l_u32WheelSpd[1] * 150) ||
				   (l_u32WheelSpd[0] * 100 < l_u32WheelSpd[1] * 60)) )
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x10;    //不同轮速度差较大
				}
			}
			else    //隐藏板块
			{
				if((l_HideZTLine[0].ZTSpd > 0) && (l_HideZTLine[1].ZTSpd > 0) && ((l_HideZTLine[0].ZTSpd*100>l_HideZTLine[1].ZTSpd*150) || 
				   (l_HideZTLine[0].ZTSpd*100<l_HideZTLine[1].ZTSpd*60)))
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x10;    //不同列速度差较大
				}
				else if((l_HideZTLine[0].ZTSpd > 0) && (l_HideZTLine[2].ZTSpd > 0) && ((l_HideZTLine[0].ZTSpd*100>l_HideZTLine[2].ZTSpd*150) || 
				   (l_HideZTLine[0].ZTSpd*100<l_HideZTLine[2].ZTSpd*60)))
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x10;    //不同列速度差较大
				}
				else if((l_HideZTLine[1].ZTSpd > 0) && (l_HideZTLine[2].ZTSpd > 0) && ((l_HideZTLine[1].ZTSpd*100>l_HideZTLine[2].ZTSpd*150) || 
				   (l_HideZTLine[1].ZTSpd*100<l_HideZTLine[2].ZTSpd*60)))
				{
					l_pAxleTempInfo->u8AxleErrorType |= 0x10;    //不同列速度差较大
				}
			}
			
			/**** end ****/						
				
			if((l_pAxleTempInfo->Dir == 1)&&(g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_n8CurrentAxleNum !=-1)&&(l_pAxleTempInfo->ZTLayouType == JCLAYOUT))
			{
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au8AxleTAIType[l_pAxleTempInfo->u8OrderID-1] = Tyrtype;
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[l_pAxleTempInfo->u8OrderID-1] = Alltyrweight;
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed += l_pAxleTempInfo->u16speed;
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed = g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed*10/l_pAxleTempInfo->u8OrderID/10;
				//g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u8AixCnt++;
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u8NormalLogiclvl |= l_pAxleTempInfo->u8AxleErrorType;

				if(g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] > 50)
				{
					g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] = 0;
				}
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] += (l_u8FirPoint/2 + l_u8SecPoint);
				g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] /= 2;
				
				if(g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u8AixCnt <= 2)
				{
					//g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime = g_au32LoopCaTime[l_pAxleTempInfo->u8ChlNum/2];
				}
			}
			else if((l_pAxleTempInfo->Dir == -1)&&(g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_n8CurrentAxleNum!=-1)&&(l_pAxleTempInfo->ZTLayouType == JCLAYOUT))
			{
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au8AxleTAIType[l_pAxleTempInfo->u8OrderID-1] = Tyrtype;
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[l_pAxleTempInfo->u8OrderID-1] = Alltyrweight;
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed += l_pAxleTempInfo->u16speed;
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed = g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16speed*10/l_pAxleTempInfo->u8OrderID/10;
				//g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u8AixCnt++;
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u8NormalLogiclvl |= l_pAxleTempInfo->u8AxleErrorType;
				
				if(g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] > 50)
				{
					g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] = 0;
				}
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] += (l_u8FirPoint/2 + l_u8SecPoint);
				g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_au8AxlePoint[l_pAxleTempInfo->u8OrderID-1] /= 2;
				
				if(g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u8AixCnt <= 2)
				{
					//g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime = g_au32LoopCaTime[l_pAxleTempInfo->u8ChlNum/2];
				}
			}
			else if((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT) && (l_pAxleTempInfo->Dir == 1))
			{
				g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[l_pAxleTempInfo->u8OrderID-1] = Alltyrweight;
				g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].m_u8HideLogiclvl |= l_pAxleTempInfo->u8AxleErrorType;				
			}
			else if((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT) && (l_pAxleTempInfo->Dir == -1))
			{
				g_HideZTReVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[l_pAxleTempInfo->u8OrderID-1] = Alltyrweight;
				g_HideZTReVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].m_u8HideLogiclvl |= l_pAxleTempInfo->u8AxleErrorType;				
			}
			if(AxisNum > 1)	  
			{
				if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错
				{
					if(l_pAxleTempInfo->Dir == 1)
					{
						preAxlePos = g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16AxleList[AxisNum - 2];	
					}
					else
					{
						preAxlePos = g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].u16AxleList[AxisNum - 2];
					}
				}
				else    //数字窄条
				{
					if(l_pAxleTempInfo->Dir == 1)
						preAxlePos = g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].u16HideAxleList[AxisNum - 2];
					else
						preAxlePos = g_HideZTReVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].u16HideAxleList[AxisNum - 2];
				}
				l_u32AddrAxleDist[0] = l_pAxleTempInfo;
				l_u32AddrAxleDist[1] = &g_AxleInfo[preAxlePos];
				l_u32AddrAxleDist[2] = &(l_pAxleTempInfo->ZTLayouType);
				l_u32AddrAxleDist[3] = &AxleDistence;
				l_u32AddrAxleDist[4] = &l_u32AxleAccTim;
				SelectZT4AxleDisCalUPdate(l_u32AddrAxleDist);
				if((0 != AxleDistence) && (0 != l_u32AxleAccTim))//保证轴距计算出数据和时间
				{
					if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错
					{
						if(l_pAxleTempInfo->Dir == 1)    //正向
						{
							g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au16AxleDistance[AxisNum - 2] =  AxleDistence;
							g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].n8accSpeed[AxisNum - 2] = F_Nabs(l_pAxleTempInfo->u16speed - g_AxleInfo[preAxlePos].u16speed) * POINTRATE / 36 / l_u32AxleAccTim;  // dv/dt:dv=(spd1-spd2)/10/3.6,dt=l_u32DiffTime/POINTRATE
						}
						else    //逆向
						{
							g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au16AxleDistance[AxisNum - 2] =  AxleDistence;
							g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].n8accSpeed[AxisNum - 2] = F_Nabs(l_pAxleTempInfo->u16speed - g_AxleInfo[preAxlePos].u16speed) * POINTRATE / 36 / l_u32AxleAccTim;  // dv/dt:dv=(spd1-spd2)/10/3.6,dt=l_u32DiffTime/POINTRATE
						}
					}
					else    //隐藏
					{
						if(l_pAxleTempInfo->Dir == 1)    //正向
							g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleDistance[AxisNum - 2] =  AxleDistence;
						else
							g_HideZTReVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleDistance[AxisNum - 2] =  AxleDistence;
					}
					if((g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].n8accSpeed[AxisNum - 2] >= 2) || 
					   (F_Nabs(l_pAxleTempInfo->u16speed - g_AxleInfo[preAxlePos].u16speed)>20 && g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].n8accSpeed[AxisNum - 2] > 0))
					{
						if(l_pAxleTempInfo->u16speed > g_AxleInfo[preAxlePos].u16speed)
						{
							l_u8AccOrDecSpd = 1;    // 加速
						}
						else 
						{
							l_u8AccOrDecSpd = 2;    // 减速
						}
					}
				}
			}
			
			
			// 11:正向交错, 10:正向一字,   01:逆向交错,  00:逆向一字
			l_u8MatchFlag = ((((l_pAxleTempInfo->Dir == 1)? 0x01:0x00)<<4) & 0xf0) | (((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)? 0x01:0x00) & 0x0f);
			switch(l_u8MatchFlag)
			{
				case 0x00:
					ptemp = &g_HideZTReVehicleStruct[0][0];
				break;
				case 0x01:
					ptemp = &g_asReVehiclePerChannelForLoop[0][0];
				break;
				case 0x10:
					ptemp = &g_HideZTVehicleStruct[0][0];
				break;
				case 0x11:
					ptemp = &g_asVehiclePerChannelForLoop[0][0];
				break;
			}
			VehStruct = ptemp;
			

			switch(AxisNum)
			{
				case 2:					
					if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(BothCompDiffBig(g_AxleInfo[VehStruct[l_pAxleTempInfo->u8ChlNum][0].u16AxleList[AxisNum-2]].u16speed,l_pAxleTempInfo->u16speed,150)) && \
						(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleDistance[AxisNum - 2] > 800)) || ((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(BothCompDiffBig(g_AxleInfo[VehStruct[l_pAxleTempInfo->u8ChlNum][0].u16HideAxleList[AxisNum-2]].u16speed,l_pAxleTempInfo->u16speed,150)) && \
						(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleDistance[AxisNum - 2] > 800)))
					{								
						ReadFPGAGlobalTime(l_u32TimeNow);
						l_u8DelteFlag =2;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_BigVehFollowLitVeh=1;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)
						{
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum = ((PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f) << 4) | (PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f);	
							
							l_u32AddrFcap[0]	= &(VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum);
							l_u32AddrFcap[1]	= &l_u32TimeNow;
							l_u32AddrFcap[2]	= &(l_pAxleTempInfo->Dir);
							l_u32AddrFcap[3]	= &l_u8headortail;
							FCapture(l_u32AddrFcap,1);
							FCaptureInfoRec(l_u32AddrFcap);   //记录第一次抓拍的信息
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						}
					}
					else if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(GetDiff(g_AxleInfo[VehStruct[l_pAxleTempInfo->u8ChlNum][0].u16AxleList[AxisNum-2]].u16speed,l_pAxleTempInfo->u16speed)>120) && \
						(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleDistance[AxisNum - 2] > 800)) || ((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(GetDiff(g_AxleInfo[VehStruct[l_pAxleTempInfo->u8ChlNum][0].u16HideAxleList[AxisNum-2]].u16speed,l_pAxleTempInfo->u16speed)>120) && \
						(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleDistance[AxisNum - 2] > 800)))
					{								
						ReadFPGAGlobalTime(l_u32TimeNow);
						l_u8DelteFlag =4;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_BigVehFollowLitVeh=1;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						
						if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)
						{
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum = ((PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f) << 4) | (PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f);
							
							l_u32AddrFcap[0]	= &(VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum);
							l_u32AddrFcap[1]	= &l_u32TimeNow;
							l_u32AddrFcap[2]	= &(l_pAxleTempInfo->Dir);
							l_u32AddrFcap[3]	= &l_u8headortail;
							FCapture(l_u32AddrFcap,1);
							FCaptureInfoRec(l_u32AddrFcap);   //记录第一次抓拍的信息
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						}							
					}
					else if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[0] * 6 < Alltyrweight)) ||\
						((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[0] * 6 < Alltyrweight)))
					{
						ReadFPGAGlobalTime(l_u32TimeNow);
						l_u8DelteFlag =3;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_BigVehFollowLitVeh=1;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						
						if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)
						{
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum = ((PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f) << 4) | (PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f);
							
							l_u32AddrFcap[0]	= &(VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum);
							l_u32AddrFcap[1]	= &l_u32TimeNow;
							l_u32AddrFcap[2]	= &(l_pAxleTempInfo->Dir);
							l_u32AddrFcap[3]	= &l_u8headortail;
							FCapture(l_u32AddrFcap,1);
							FCaptureInfoRec(l_u32AddrFcap);   //记录第一次抓拍的信息
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						}
					}
// 					else if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[0] < 1700 )&& (VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[0] * 4 >= Alltyrweight)) ||\
// 						((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[0] < 1700 )&& (VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[0] * 4 >= Alltyrweight)))
// 					{
// 						Dir = 1;
// 						AddressForPo[0] = &l_pAxleTempInfo->u8ChlNum;
// 						AddressForPo[1] = &Dir;
// 						AddressForPo[2] = &VehStruct[l_pAxleTempInfo->u8ChlNum][0];
// 						FPostVehForLoop(AddressForPo,26);//抛车
// 					}
					else if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[0] < 2000) && VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16AxleWeight[0] * 4 < Alltyrweight) ||\
						((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[0] < 2000) && VehStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleWeight[0] * 4 < Alltyrweight))
					{
						ReadFPGAGlobalTime(l_u32TimeNow);	
						l_u8DelteFlag =1;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_BigVehFollowLitVeh=1;
						VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						
						if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)
						{
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum = ((PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f) << 4) | (PROMODPARAM.m_au8CHLSET[l_pAxleTempInfo->u8ChlNum] & 0x0f);
							l_u32AddrFcap[0]	= &(VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_Capture_LaneNum);
							l_u32AddrFcap[1]	= &l_u32TimeNow;
							l_u32AddrFcap[2]	= &(l_pAxleTempInfo->Dir);
							l_u32AddrFcap[3]	= &l_u8headortail;
							FCapture(l_u32AddrFcap,1);
							FCaptureInfoRec(l_u32AddrFcap);   //记录第一次抓拍的信息
							VehStruct[l_pAxleTempInfo->u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
						}
					}					
					break;
				case 3:
					if(VehStruct[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].m_BigVehFollowLitVeh == 1)
					{
						if(((l_pAxleTempInfo->ZTLayouType == JCLAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].au16AxleWeight[1] < 1700) && (VehStruct[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].au16AxleWeight[1] * 4 >= Alltyrweight)) ||\
						((l_pAxleTempInfo->ZTLayouType == HIDELAYOUT)&&(VehStruct[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].au16HideAxleWeight[1] < 1700) && (VehStruct[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].au16HideAxleWeight[1] * 4 >= Alltyrweight)))
						
						{
							AddressForPo[0] = &l_pAxleTempInfo->u8ChlNum;
							AddressForPo[1] = &(l_pAxleTempInfo->Dir);
							AddressForPo[2] = &VehStruct[l_pAxleTempInfo->u8ChlNum][0];
							FPostVehForLoop(AddressForPo, 27);//抛车
						}
					}
					break;
			}
						

			
			/**** 增加违例码(liumingxin)(20180208)****/
			if(l_pAxleTempInfo->AxleStatus == 2)    //跨道
			{
// 				l_u8ViolationCode |= 0x01;   
// 				l_u8ViolationCodeOutput = 0x01;    // 输出给车辆的违例码
			}
// 			if(l_u8PassThr_flag == 1)    //压三边
// 			{
// 				l_u8ViolationCode |= (0x01 << 1);
// 				l_u8ViolationCodeOutput = 0x02;
// 			}
			if(l_u8AccOrDecSpd == 2)    // 减速
			{
				if(AxisNum >= 3)
				{
					l_u8ViolationCode |= (0x01 << 5);    // 点刹
					l_u8ViolationCodeOutput = 0x20;    
				}
				else 
				{
					l_u8ViolationCode |= (0x01 << 2);    // 减速
					l_u8ViolationCodeOutput = 0x04;
				}
			}
			if(l_u8AccOrDecSpd == 1)    // 加速
			{
				l_u8ViolationCode |= (0x01 << 3);
				l_u8ViolationCodeOutput = 0x08;
			}
			if((WalkAndRunFlag != 0) && (l_pAxleTempInfo->RunAndWalkFlag != 0))    //走走停停
			{
				l_u8ViolationCode |= 0x80;   
				l_u8ViolationCodeOutput = 0x80;				
			}
			if(l_pAxleTempInfo->Dir == 1)
			{
				if(g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].u8ViolationCode < l_u8ViolationCodeOutput)
				{
					g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].u8ViolationCode = l_u8ViolationCodeOutput;    // 按照优先级取违例码
				}
			}
			else
			{
				if(g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].u8ViolationCode < l_u8ViolationCodeOutput)
				{
					g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][l_pAxleTempInfo->u8Owner].u8ViolationCode = l_u8ViolationCodeOutput;    // 按照优先级取违例码
				}
			}
			/**** end****/
			/////////////////
			if((PROMODPARAM.m_u8StudyAlgEn[0] == 1) || (PROMODPARAM.m_u8StudyAlgEn[1] == 1)
				|| (PROMODPARAM.m_u8StudyAlgEn[2] == 1) || (PROMODPARAM.m_u8StudyAlgEn[3] == 1)
				|| (PROMODPARAM.m_u8StudyAlgEn[4] == 1))
			{
				for(l_u8tempi =0;l_u8tempi < CHANNELNUM; l_u8tempi++)
				{
					SETUPALIAS.an32AxGain[l_u8tempi] = l_u16AGrain[l_u8tempi];
				}
			}
			
// 			if((l_pAxleTempInfo->ZTLayouType == JCLAYOUT) && (Axisspeed > 200))   //针对交错布局轴计算强制抛车时间
// 			{
// 				ReadFPGAGlobalTime(l_u32TimeNow);
// 				if(l_pAxleTempInfo->Dir == 1)
// 				{
// 					g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u8PostAxleFlag=1;
// 					g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u32ForcePostTime=l_u32TimeNow+15000*6000/Axisspeed; //轴过15米需要的点数
// 				}
// 				else if(l_pAxleTempInfo->Dir == -1)
// 				{
// 					g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u8PostAxleFlag=1;
// 					g_asReVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].m_u32ForcePostTime=l_u32TimeNow+13000*6000/Axisspeed; //轴过13米需要的点数
// 				}
// 			}			
			
			if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错
			{
				if(AxisNum <= 1)
				{ 
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nAx %d:Wt=%d,Dr=%d,L=%d,St=%d,Spd=%d,A%d[%d,%d,%d,%d],e=%02x,B=%d.",
														AxisNum,l_pAxleTempInfo->s32AxleWeight,l_pAxleTempInfo->Dir,l_pAxleTempInfo->u8ChlNum,l_pAxleTempInfo->AxleStatus,
														l_pAxleTempInfo->u16speed/10,l_pAxleTempInfo->AxleAccCnt,l_pAxleTempInfo->AxleAcc[0],l_pAxleTempInfo->AxleAcc[1],
														l_pAxleTempInfo->AxleAcc[2],l_pAxleTempInfo->AxleAcc[3],l_pAxleTempInfo->u8AxleErrorType, l_u8DelteFlag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
				else
				{
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nAx %d:Wt=%d,Dr=%d,L=%d,St=%d,d=%d,Spd=%d,A%d[%d,%d,%d,%d],e=%02x,B=%d.",
													AxisNum,l_pAxleTempInfo->s32AxleWeight,l_pAxleTempInfo->Dir,l_pAxleTempInfo->u8ChlNum,
													l_pAxleTempInfo->AxleStatus,g_asVehiclePerChannelForLoop[l_pAxleTempInfo->u8ChlNum][0].au16AxleDistance[AxisNum - 2],
													l_pAxleTempInfo->u16speed/10,l_pAxleTempInfo->AxleAccCnt,l_pAxleTempInfo->AxleAcc[0],l_pAxleTempInfo->AxleAcc[1],
													l_pAxleTempInfo->AxleAcc[2],l_pAxleTempInfo->AxleAcc[3],l_pAxleTempInfo->u8AxleErrorType,l_u8DelteFlag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
			}
			else
			{
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nHAx %d:Wgt=%d,L=%d,Spd=%d,d=%d,e=%02x,B=%d.",
													AxisNum,l_pAxleTempInfo->s32AxleWeight,l_pAxleTempInfo->u8ChlNum,l_pAxleTempInfo->u16speed/10,
													g_HideZTVehicleStruct[l_pAxleTempInfo->u8ChlNum][0].au16HideAxleDistance[AxisNum - 2],
													l_pAxleTempInfo->u8AxleErrorType,l_u8DelteFlag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
			}
			/////////////////
		}
	}  		
} 

/********************************************************************************************************
** Function name:     FCalcZTWgtAndSpeed
** Descriptions:      计算一根轴包含的每根窄条的重量和速度,计算每个轴包含的轮的重量和速度
** input parameters:  l_pAxleTempInfo:轴信息结构体
					  ZTWgt:指向存储每根窄条的重量数组
					  ZTSpd:指向存储每根窄条的速度数组
					  WheelWgt:指向存储每个轮的重量数组
					  WheelSpd:指向存储每个轮的速度数组
** output parameters: 计算状态
**
** Created by:	  liumingxin
** Created Date:  20181015
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	 
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int8 FCalcZTWgtAndSpeed(Axle_Struct *l_pAxleTempInfo, uint32 *ZTWgt, uint32 *ZTSpd, uint32 *WheelWgt, uint32 *WheelSpd)
{
	int8  ret = 0;            //返回值
	uint32 l_u32ZTWgt[12];    //存储每根窄条的重量,单位kg
	uint32 l_u32ZTSpd[12];    //存储每根窄条的速度,单位mm/s
	uint32 l_u32WheelWgt[2];  //0:左轮重,1:右轮重,单位kg
	uint32 l_u32WheelSpd[2];   //0:左轮速度,1:右轮速度,单位mm/s
	
	int8  i, j, k, cnt;
	int8  l, r;
	uint8  id, colum, takepos;
	uint16 pos, gap;
	uint32 spd;
	uint8  X, Y, X1, Y1;
	uint8  delta_y;
	
	uint8  ZTNums = 0;     			  //窄条数
	uint8  ColumMin = 0xff;           //每个轮包含的最小列号
	uint8  l_u8ZTCnt[3]; 
	TranZT_Struct LeftWheel_ZTPos[3][3];     //按列存储轮的窄条
	TranZT_Struct RightWheel_ZTPos[3][3];    //按列存储轮的窄条
	uint32 LeftWheel_ZTSpd[3][3];     //单位mm/s
	uint32 RightWheel_ZTSpd[3][3];    //单位mm/s
	uint32 LeftWheel_ZTWgt[3][3];     //单位kg
	uint32 RightWheel_ZTWgt[3][3];    //单位kg
	uint32 ZTSpd4Acc[6];
	int32  ZTPos4Acc[6];
	uint16 LeftWheel_Acc[4];          //加速度,单位为0.1m/s2
	uint16 RightWheel_Acc[4];         //加速度,单位为0.1m/s2
	             

	memset(l_u32ZTWgt, 0, sizeof(uint32)*12);
	memset(l_u32ZTSpd, 0, sizeof(uint32)*12);
	memset(l_u32WheelWgt, 0, sizeof(uint32)*2);
	memset(l_u32WheelSpd, 0, sizeof(uint32)*2);
	
	memset(LeftWheel_ZTPos, 0, sizeof(TranZT_Struct)*3*3);
	memset(RightWheel_ZTPos, 0, sizeof(TranZT_Struct)*3*3);
	memset(l_u8ZTCnt, 0, sizeof(uint8)*3);
	memset(LeftWheel_ZTSpd, 0, sizeof(uint32)*3*3);
	memset(RightWheel_ZTSpd, 0, sizeof(uint32)*3*3);
	memset(LeftWheel_ZTWgt, 0, sizeof(uint32)*3*3);
	memset(RightWheel_ZTWgt, 0, sizeof(uint32)*3*3);
	memset(ZTSpd4Acc, 0, sizeof(uint32)*6);
	memset(ZTPos4Acc, -1, sizeof(int32)*6);
	memset(LeftWheel_Acc, 0, sizeof(uint16)*4);
	memset(RightWheel_Acc, 0, sizeof(uint16)*4);
	
	
	l = r = 0;
	k = 0;
	
	if(l_pAxleTempInfo == (Axle_Struct *)0)    //轴数据为空则返回错误
		return -1;   
	
	if(l_pAxleTempInfo->ZTLayouType == JCLAYOUT)    //交错布局
	{
		/**** 计算左轮中窄条重量和速度 ****/
		ColumMin = 0xff;
		memset(l_u8ZTCnt, 0, sizeof(uint8)*3);
		
		ZTNums  =  l_pAxleTempInfo->LeftWheelZTCnt;
		//找到左轮包含的最小列号
		for(i = 0; i < ZTNums; i++)
		{
			pos = l_pAxleTempInfo->u16LeftWheeList[i];
			id = g_ChlData[pos].u8Id;
			colum = id / PROMODPARAM.m_u8ZTRow;
			if(colum < ColumMin)
				ColumMin = colum;
		}
		
		//解析左轮的窄条数据(按列存储)
		for(i = 0; i < ZTNums; i++)
		{
			pos = l_pAxleTempInfo->u16LeftWheeList[i];
			id = g_ChlData[pos].u8Id;
			colum = id / PROMODPARAM.m_u8ZTRow - ColumMin;
			cnt = l_u8ZTCnt[colum]++;
			takepos = l_pAxleTempInfo->u8LeftWheelTakeZT[i];
			
			//按列存储窄条数据
			LeftWheel_ZTPos[colum][cnt].ZTPos = pos;
			LeftWheel_ZTPos[colum][cnt].TakePos = takepos;
			LeftWheel_ZTPos[colum][cnt].MaxValueTime = g_ChlData[pos].u32Maxtm;
			switch(takepos)
			{
				case 0x01:
					LeftWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32FirstZTSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					LeftWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32FirstZTUpTmSec;
					LeftWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32FirstZTDnTmSec;
					LeftWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16FirstZTMaxValue;
					LeftWheel_ZTPos[colum][cnt].u32realCnt = GetDiff(g_ChlData[pos].u32FirstZTDnTmFir, g_ChlData[pos].u32FirstZTUpTmFir);
					break;
				case 0x02:
					LeftWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32SecondZTSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					LeftWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32SecondZTUpTmSec;
					LeftWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32SecondZTDnTmSec;
					LeftWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16SecondZTMaxValue;
					LeftWheel_ZTPos[colum][cnt].u32realCnt = GetDiff(g_ChlData[pos].u32SecondZTDnTmFir, g_ChlData[pos].u32SecondZTUpTmFir);
					break;
				case 0x03:
					LeftWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32chlSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					LeftWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32uptm;
					LeftWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32dntm;
					LeftWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16ptMaxValue;
					LeftWheel_ZTPos[colum][cnt].u32realCnt = g_ChlData[pos].u32realCnt;
					break;
			}
		}
		//按列计算速度
		for(i = 0; i < 3; i++)
		{
			switch(l_u8ZTCnt[i])
			{
				case 0:      //该列无窄条
					break;
				case 1:      //该列有1根窄条,通过相邻列计算速度
					switch(i)
					{
						case 0:    //找+1列
							if(l_u8ZTCnt[i+1] > 0)
							{
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i+1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								LeftWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][0].u32uptm, LeftWheel_ZTPos[i+1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 1;
							}
							break;
						case 1:    //找-1列或+1列
							if(l_u8ZTCnt[i-1] > 0)
							{
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i-1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								LeftWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][0].u32uptm, LeftWheel_ZTPos[i-1][0].u32uptm);    //单位mm/s
							}
							else if(l_u8ZTCnt[i+1] > 0)
							{
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i+1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								LeftWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][0].u32uptm, LeftWheel_ZTPos[i+1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 2;
							}
							break;
						case 2:    //找-1列
							if(l_u8ZTCnt[i-1] > 0)
							{
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[LeftWheel_ZTPos[i-1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								LeftWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][0].u32uptm, LeftWheel_ZTPos[i-1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 3;
							}
							break;
						default :
							break;
					}
					ZTSpd4Acc[Y] = LeftWheel_ZTSpd[i][0];
					ZTPos4Acc[Y] = LeftWheel_ZTPos[i][0].ZTPos;
					break;
				case 2:      //该列有2根窄条
					FTransLocation(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
					gap = PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id]*GetDiff(g_ChlData[LeftWheel_ZTPos[i][0].ZTPos].u8Id, g_ChlData[LeftWheel_ZTPos[i][1].ZTPos].u8Id);
					spd = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][0].u32uptm, LeftWheel_ZTPos[i][1].u32uptm);    //单位mm/s
					LeftWheel_ZTSpd[i][0] = spd;
					LeftWheel_ZTSpd[i][1] = spd;
					ZTSpd4Acc[Y] = LeftWheel_ZTSpd[i][0];
					ZTPos4Acc[Y] = LeftWheel_ZTPos[i][0].ZTPos;
					break;
				case 3:      //该列有3根窄条
					for(j = 0; j < 2; j++)
					{
						FTransLocation(g_ChlData[LeftWheel_ZTPos[i][j].ZTPos].u8Id, &X, &Y, 2);
						gap = PROMODPARAM.m_au16ZTGap[g_ChlData[LeftWheel_ZTPos[i][j].ZTPos].u8Id];
						spd = gap * POINTRATE / GetDiff(LeftWheel_ZTPos[i][j].u32uptm, LeftWheel_ZTPos[i][j+1].u32uptm);    //单位mm/s
						LeftWheel_ZTSpd[i][j] = spd;
						ZTSpd4Acc[Y] = spd;
						ZTPos4Acc[Y] = LeftWheel_ZTPos[i][j].ZTPos;
					}
					FTransLocation(g_ChlData[LeftWheel_ZTPos[i][2].ZTPos].u8Id, &X, &Y, 2);
					LeftWheel_ZTSpd[i][2] = spd;
					ZTSpd4Acc[Y] = spd;
					ZTPos4Acc[Y] = LeftWheel_ZTPos[i][2].ZTPos;
					break;
				default :
					break;
			}
		}
		//计算每根窄条重量和速度
		for(i = 0; i < 3; i++)
		{
			switch(l_u8ZTCnt[i])
			{
				case 0:      //该列无窄条
					break;
				case 1:      //该列有1根窄条
					if(LeftWheel_ZTPos[i][0].u32chlSum > 120000)
						LeftWheel_ZTWgt[i][0] = LeftWheel_ZTPos[i][0].u32chlSum / 300 * LeftWheel_ZTSpd[i][0] / 1000;
					else		
						LeftWheel_ZTWgt[i][0] = LeftWheel_ZTPos[i][0].u32chlSum * LeftWheel_ZTSpd[i][0] / 300 / 1000;	
					l_u32ZTWgt[k] = LeftWheel_ZTWgt[i][0];
					l_u32ZTSpd[k] = LeftWheel_ZTSpd[i][0];
					k++;
					break;
				case 2:      //该列有2根窄条
					for(j = 0; j < 2; j++)
					{
						if(LeftWheel_ZTPos[i][j].u32chlSum > 120000)
							LeftWheel_ZTWgt[i][j] = LeftWheel_ZTPos[i][j].u32chlSum / 300 * LeftWheel_ZTSpd[i][j] / 1000;
						else		
							LeftWheel_ZTWgt[i][j] = LeftWheel_ZTPos[i][j].u32chlSum * LeftWheel_ZTSpd[i][j] / 300 / 1000;	
						l_u32ZTWgt[k] = LeftWheel_ZTWgt[i][j];
						l_u32ZTSpd[k] = LeftWheel_ZTSpd[i][j];
						k++;
					}
					break;
				case 3:      //该列有3根窄条
					for(j = 0; j < 3; j++)
					{
						if(LeftWheel_ZTPos[i][j].u32chlSum > 120000)
							LeftWheel_ZTWgt[i][j] = LeftWheel_ZTPos[i][j].u32chlSum / 300 * LeftWheel_ZTSpd[i][j] / 1000;
						else		
							LeftWheel_ZTWgt[i][j] = LeftWheel_ZTPos[i][j].u32chlSum * LeftWheel_ZTSpd[i][j] / 300 / 1000;	
						l_u32ZTWgt[k] = LeftWheel_ZTWgt[i][j];
						l_u32ZTSpd[k] = LeftWheel_ZTSpd[i][j];
						k++;
					}
					break;
				default :
					break;
			}
		}
		//计算加速度
		l = 0;
		for(i = 1; i < 5; i++)
		{
			if(ZTPos4Acc[i] >= 0)
			{
				for(j = i-1; j >= 0; j--)
				{
					if(ZTPos4Acc[j] != -1)
					{
						LeftWheel_Acc[l++] = POINTRATE*GetDiff(ZTSpd4Acc[i], ZTSpd4Acc[j]) / GetDiff(g_ChlData[ZTPos4Acc[i]].u32uptm, g_ChlData[ZTPos4Acc[j]].u32uptm)/100;    //加速度单位为0.1m/s2	
						break;
					}
				}
			}
		}
		//计算左轮总重和左轮速度
		for(i = 0; i < 3; i++)
		{
			if(l_u8ZTCnt[i] > 0)
			{
				l_u32WheelWgt[0] += (LeftWheel_ZTWgt[i][0] + LeftWheel_ZTWgt[i][1] + LeftWheel_ZTWgt[i][2]) / l_u8ZTCnt[i];
			}
			//用该轮包含的所有列中包含的窄条数最多的列平均速度作为轮的速度
			if((i == 0 && l_u8ZTCnt[i] > 0) || (i > 0 && l_u8ZTCnt[i] > l_u8ZTCnt[i-1]))
			{
				l_u32WheelSpd[0] = (LeftWheel_ZTSpd[i][0] + LeftWheel_ZTSpd[i][1] + LeftWheel_ZTSpd[i][2]) / l_u8ZTCnt[i];
			}
		}
		/**** end ****/
		
		/**** 计算右轮中窄条重量和速度 ****/
		ColumMin = 0xff;
		memset(l_u8ZTCnt, 0, sizeof(uint8)*3);
		memset(ZTSpd4Acc, 0, sizeof(uint32)*6);
		memset(ZTPos4Acc, -1, sizeof(int32)*6);
		
		ZTNums  =  l_pAxleTempInfo->RightWheelZTCnt;
		//找到右轮包含的最小列号
		for(i = 0; i < ZTNums; i++)
		{
			pos = l_pAxleTempInfo->u16RightWheeList[i];
			id = g_ChlData[pos].u8Id;
			colum = id / PROMODPARAM.m_u8ZTRow;
			if(colum < ColumMin)
				ColumMin = colum;
		}
		
		//解析右轮的窄条数据(按列存储)
		for(i = 0; i < ZTNums; i++)
		{
			pos = l_pAxleTempInfo->u16RightWheeList[i];
			id = g_ChlData[pos].u8Id;
			colum = id / PROMODPARAM.m_u8ZTRow - ColumMin;
			cnt = l_u8ZTCnt[colum]++;
			takepos = l_pAxleTempInfo->u8RightWheelTakeZT[i];
			
			//按列存储窄条数据
			RightWheel_ZTPos[colum][cnt].ZTPos = pos;
			RightWheel_ZTPos[colum][cnt].TakePos = takepos;
			RightWheel_ZTPos[colum][cnt].MaxValueTime = g_ChlData[pos].u32Maxtm;
			switch(takepos)
			{
				case 0x01:
					RightWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32FirstZTSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					RightWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32FirstZTUpTmSec;
					RightWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32FirstZTDnTmSec;
					RightWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16FirstZTMaxValue;
					RightWheel_ZTPos[colum][cnt].u32realCnt = GetDiff(g_ChlData[pos].u32FirstZTDnTmFir, g_ChlData[pos].u32FirstZTUpTmFir);
					break;
				case 0x02:
					RightWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32SecondZTSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					RightWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32SecondZTUpTmSec;
					RightWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32SecondZTDnTmSec;
					RightWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16SecondZTMaxValue;
					RightWheel_ZTPos[colum][cnt].u32realCnt = GetDiff(g_ChlData[pos].u32SecondZTDnTmFir, g_ChlData[pos].u32SecondZTUpTmFir);
					break;
				case 0x03:
					RightWheel_ZTPos[colum][cnt].u32chlSum = g_ChlData[pos].u32chlSum * g_Setup.an32AxGain[id] / 1000;    //积分和(乘以增益后)
					RightWheel_ZTPos[colum][cnt].u32uptm = g_ChlData[pos].u32uptm;
					RightWheel_ZTPos[colum][cnt].u32dntm = g_ChlData[pos].u32dntm;
					RightWheel_ZTPos[colum][cnt].wmax = g_ChlData[pos].u16ptMaxValue;
					RightWheel_ZTPos[colum][cnt].u32realCnt = g_ChlData[pos].u32realCnt;
					break;
			}
		}
		//按列计算速度
		for(i = 0; i < 3; i++)
		{
			switch(l_u8ZTCnt[i])
			{
				case 0:      //该列无窄条
					break;
				case 1:      //该列有1根窄条,通过相邻列计算速度
					switch(i)
					{
						case 0:    //找+1列
							if(l_u8ZTCnt[i+1] > 0)
							{
								FTransLocation(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[RightWheel_ZTPos[i+1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								RightWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][0].u32uptm, RightWheel_ZTPos[i+1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 1;
							}
							break;
						case 1:    //找-1列或+1列
							if(l_u8ZTCnt[i-1] > 0)
							{
								FTransLocation(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[RightWheel_ZTPos[i-1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								RightWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][0].u32uptm, RightWheel_ZTPos[i-1][0].u32uptm);    //单位mm/s
							}
							else if(l_u8ZTCnt[i+1] > 0)
							{
								FTransLocation(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[RightWheel_ZTPos[i+1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								RightWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][0].u32uptm, RightWheel_ZTPos[i+1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 2;
							}
							break;
						case 2:    //找-1列
							if(l_u8ZTCnt[i-1] > 0)
							{
								FTransLocation(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
								FTransLocation(g_ChlData[RightWheel_ZTPos[i-1][0].ZTPos].u8Id, &X1, &Y1, 2);
								delta_y = GetDiff(Y, Y1);
								gap =  PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id] * delta_y / 2;
								RightWheel_ZTSpd[i][0] = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][0].u32uptm, RightWheel_ZTPos[i-1][0].u32uptm);    //单位mm/s
							}
							else
							{
								return 3;
							}
							break;
						default :
							break;
					}
					ZTSpd4Acc[Y] = RightWheel_ZTSpd[i][0];
					ZTPos4Acc[Y] = LeftWheel_ZTPos[i][0].ZTPos;
					break;
				case 2:      //该列有2根窄条
					FTransLocation(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, &X, &Y, 2);
					gap = PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id]*GetDiff(g_ChlData[RightWheel_ZTPos[i][0].ZTPos].u8Id, g_ChlData[RightWheel_ZTPos[i][1].ZTPos].u8Id);
					spd = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][0].u32uptm, RightWheel_ZTPos[i][1].u32uptm);    //单位mm/s
					RightWheel_ZTSpd[i][0] = spd;
					RightWheel_ZTSpd[i][1] = spd;
					ZTSpd4Acc[Y] = spd;
					ZTPos4Acc[Y] = RightWheel_ZTPos[i][0].ZTPos;
					break;
				case 3:      //该列有3根窄条
					for(j = 0; j < 2; j++)
					{
						FTransLocation(g_ChlData[RightWheel_ZTPos[i][j].ZTPos].u8Id, &X, &Y, 2);
						gap = PROMODPARAM.m_au16ZTGap[g_ChlData[RightWheel_ZTPos[i][j].ZTPos].u8Id];
						spd = gap * POINTRATE / GetDiff(RightWheel_ZTPos[i][j].u32uptm, RightWheel_ZTPos[i][j+1].u32uptm);    //单位mm/s
						RightWheel_ZTSpd[i][j] = spd;
						ZTSpd4Acc[Y] = spd;
						ZTPos4Acc[Y] = RightWheel_ZTPos[i][j].ZTPos;
					}
					FTransLocation(g_ChlData[RightWheel_ZTPos[i][2].ZTPos].u8Id, &X, &Y, 2);
					RightWheel_ZTSpd[i][2] = spd;
					ZTSpd4Acc[Y] = spd;
					ZTPos4Acc[Y] = RightWheel_ZTPos[i][j].ZTPos;
					break;
				default :
					break;
			}
		}
		//按列计算每根窄条重量
		for(i = 0; i < 3; i++)
		{
			switch(l_u8ZTCnt[i])
			{
				case 0:      //该列无窄条
					break;
				case 1:      //该列有1根窄条
					if(RightWheel_ZTPos[i][0].u32chlSum > 120000)
						RightWheel_ZTWgt[i][0] = RightWheel_ZTPos[i][0].u32chlSum / 300 * RightWheel_ZTSpd[i][0] / 1000;
					else		
						RightWheel_ZTWgt[i][0] = RightWheel_ZTPos[i][0].u32chlSum * RightWheel_ZTSpd[i][0] / 300 / 1000;	
					l_u32ZTWgt[k] = RightWheel_ZTWgt[i][0];
					l_u32ZTSpd[k] = RightWheel_ZTSpd[i][0];
					k++;
					break;
				case 2:      //该列有2根窄条
					for(j = 0; j < 2; j++)
					{
						if(RightWheel_ZTPos[i][j].u32chlSum > 120000)
							RightWheel_ZTWgt[i][j] = RightWheel_ZTPos[i][j].u32chlSum / 300 * RightWheel_ZTSpd[i][j] / 1000;
						else		
							RightWheel_ZTWgt[i][j] = RightWheel_ZTPos[i][j].u32chlSum * RightWheel_ZTSpd[i][j] / 300 / 1000;	
						l_u32ZTWgt[k] = RightWheel_ZTWgt[i][j];
						l_u32ZTSpd[k] = RightWheel_ZTSpd[i][j];
						k++;
					}
					break;
				case 3:      //该列有3根窄条
					for(j = 0; j < 3; j++)
					{
						if(RightWheel_ZTPos[i][j].u32chlSum > 120000)
							RightWheel_ZTWgt[i][j] = RightWheel_ZTPos[i][j].u32chlSum / 300 * RightWheel_ZTSpd[i][j] / 1000;
						else		
							RightWheel_ZTWgt[i][j] = RightWheel_ZTPos[i][j].u32chlSum * RightWheel_ZTSpd[i][j] / 300 / 1000;	
						l_u32ZTWgt[k] = RightWheel_ZTWgt[i][j];
						l_u32ZTSpd[k] = RightWheel_ZTSpd[i][j];
						k++;
					}
					break;
				default :
					break;
			}
		}
		//计算加速度
		r = 0;
		for(i = 1; i < 5; i++)
		{
			if(ZTPos4Acc[i] >= 0)
			{
				for(j = i-1; j >= 0; j--)
				{
					if(ZTPos4Acc[j] != -1)
					{
						RightWheel_Acc[r++] = POINTRATE*GetDiff(ZTSpd4Acc[i], ZTSpd4Acc[j]) / GetDiff(g_ChlData[ZTPos4Acc[i]].u32uptm, g_ChlData[ZTPos4Acc[j]].u32uptm)/100;    //加速度单位为0.1m/s2	
						break;
					}
				}
			}
		}
		//计算右轮总重和右轮速度
		for(i = 0; i < 3; i++)
		{
			if(l_u8ZTCnt[i] > 0)
			{
				l_u32WheelWgt[1] += (RightWheel_ZTWgt[i][0] + RightWheel_ZTWgt[i][1] + RightWheel_ZTWgt[i][2]) / l_u8ZTCnt[i];
			}
			//用该轮包含的所有列中包含的窄条数最多的列平均速度作为轮的速度
			if((i == 0 && l_u8ZTCnt[i] > 0) || (i > 0 && l_u8ZTCnt[i] > l_u8ZTCnt[i-1]))
			{
				l_u32WheelSpd[1] = (RightWheel_ZTSpd[i][0] + RightWheel_ZTSpd[i][1] + RightWheel_ZTSpd[i][2]) / l_u8ZTCnt[i];
			}
		}
		/**** end ****/
		
		//加速度赋值
		for(i = 0; i < 4; i++)
		{
			if(l > r)
				l_pAxleTempInfo->AxleAcc[i] = LeftWheel_Acc[i];
			else
				l_pAxleTempInfo->AxleAcc[i] = RightWheel_Acc[i];
		}
		
	}
// 	else    //一字布局
// 	{
// 		
// 	}
	
	//输出计算的数据
	memcpy(ZTWgt, l_u32ZTWgt, sizeof(uint32)*12);
	memcpy(ZTSpd, l_u32ZTSpd, sizeof(uint32)*12);
	memcpy(WheelWgt, l_u32WheelWgt, sizeof(uint32)*2);
	memcpy(WheelSpd, l_u32WheelSpd, sizeof(uint32)*2);
	
	return ret;
}

/********************************************************************************************************
** Function name:     SelectZT4AxleDisCal
** Descriptions:      选择用于计算轴距的窄条
** input parameters:  preAxle:指向前一轴 ,currAxle:指向当前轴
					  retData:指向存储找出的ID结构体
					  type:窄条类型  0-中航,1-数字
                      unormalID:异常的ID(该ID在轴中出现2次以上),该ID不用于计算轴距
** output parameters: 找到的可用于计算轴距的窄条数
**
** Created by:	  liumingxin
** Created Date:  20180209
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	 
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 SelectZT4AxleDisCal(Axle_Struct *preAxle, Axle_Struct *currAxle, TrigID4AxleDis *retData, uint8 type, int unormalID)
{
	int i = 0, j = 0;
	uint8  ZTIndex = 0;
	int preID[3], currID[3];
	
	memset(preID, -1, sizeof(int)*3);
	memset(currID, -1, sizeof(int)*3);
	
	if(type == 0)    //中航窄条
	{
		for(i = 0; i < preAxle->u16ChListSum; i++)
		{
			preID[ZTIndex] = preAxle->u16ChList[i];
			for(j = 0; j < currAxle->u16ChListSum; j++)
			{
				currID[ZTIndex] = currAxle->u16ChList[j];
				if((g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id) && (g_ChlData[preID[ZTIndex]].u8Id != unormalID))
				{
					retData->TrigPreID[ZTIndex] = preID[ZTIndex];
					retData->TrigCurrID[ZTIndex] = currID[ZTIndex];
					ZTIndex++;
					break;
				}
			}
			if(ZTIndex > 2)
				break;
		}
	}
	else    //数字窄条
	{
		for(i = 0; i < preAxle->LeftWheelZTCnt; i++)
		{
			preID[ZTIndex] = preAxle->u16LeftWheeList[i];
			for(j = 0; j < currAxle->LeftWheelZTCnt; j++)
			{
				currID[ZTIndex] = currAxle->u16LeftWheeList[j];
				if((g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id) && (g_ChlData[preID[ZTIndex]].u8Id != unormalID))
				{
					retData->TrigPreID[ZTIndex] = preID[ZTIndex];
					retData->TrigCurrID[ZTIndex] = currID[ZTIndex];
					ZTIndex++;
					break;
				}
			}
			if(ZTIndex > 2)
				break;
		}
		if(ZTIndex < 3)
		{
			for(i = 0; i < preAxle->RightWheelZTCnt; i++)
			{
				preID[ZTIndex] = preAxle->u16RightWheeList[i];
				for(j = 0; j < currAxle->RightWheelZTCnt; j++)
				{
					currID[ZTIndex] = currAxle->u16RightWheeList[j];
					if((g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id) && (g_ChlData[preID[ZTIndex]].u8Id != unormalID))
					{
						retData->TrigPreID[ZTIndex] = preID[ZTIndex];
						retData->TrigCurrID[ZTIndex] = currID[ZTIndex];
						ZTIndex++;
						break;
					}
				}
				if(ZTIndex > 2)
					break;
			}
		}
	}

	return ZTIndex;
}

/*******************************************************************************
*该程序为SelectZT4AxleDisCal的变通版本，直接返回轴距，速度用的当前轴的速度
* Cover SelectZT4AxleDisCal liumingxin 20180209
**-------------------------------------------------------------------------------------------------------
** Modified by: zhangtonghan	
** Modified date:20180622
**-------------------------------------------------------------------------------------------------------
*******************************************************************************/
void SelectZT4AxleDisCalUPdate(void *Address[5])
{
	TrigID4AxleDis retData;
	Axle_Struct *preAxle,*currAxle;
	uint32 *p_u32DiffTime;
	uint8 *p_type,type;
	uint16 *Dist;
	int i = 0, j = 0,l_u8tempi;
	uint8  ZTIndex = 0;
	int preID[3], currID[3];
	SortAxleDist wheelbase[4];//轴距
	//SortAxleDist
	uint32 l_u32DiffTime;
	SortAxleDist l_u16AxleDistance_Max,l_u16AxleDistance_Min,l_u16AxleDistance_Mid;
	uint8 l_u8Err;
	uint16 l_u16SendLen;

	currAxle 		= Address[0];
	preAxle			= Address[1];
	p_type			= Address[2];
	Dist			= Address[3];
	p_u32DiffTime	= Address[4];

	type			= *p_type;
	//l_u32DiffTime	= *p_u32DiffTime;
	memset(preID, -1, sizeof(int)*3);
	memset(currID, -1, sizeof(int)*3);
	memset(wheelbase,0,sizeof(SortAxleDist)*4);
	memset(&l_u16AxleDistance_Max,0,sizeof(SortAxleDist));
	memset(&l_u16AxleDistance_Min,0,sizeof(SortAxleDist));
	memset(&l_u16AxleDistance_Mid,0,sizeof(SortAxleDist));
	
	if(type == 0)    //交错窄条
	{
		for(i = 0; i < preAxle->LeftWheelZTCnt; i++)
		{
			preID[ZTIndex] = preAxle->u16LeftWheeList[i];
			for(j = 0; j < currAxle->LeftWheelZTCnt; j++)
			{
				currID[ZTIndex] = currAxle->u16LeftWheeList[j];
				if(g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id)
				{
					retData.TrigPreID[ZTIndex] = preID[ZTIndex];
					retData.TrigCurrID[ZTIndex] = currID[ZTIndex];
					ZTIndex++;
					break;
				}
			}
			if(ZTIndex > 2)
				break;
		}
		if(ZTIndex < 3)
		{
			for(i = 0; i < preAxle->RightWheelZTCnt; i++)
			{
				preID[ZTIndex] = preAxle->u16RightWheeList[i];
				for(j = 0; j < currAxle->RightWheelZTCnt; j++)
				{
					currID[ZTIndex] = currAxle->u16RightWheeList[j];
					if(g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id) 
					{
						retData.TrigPreID[ZTIndex] = preID[ZTIndex];
						retData.TrigCurrID[ZTIndex] = currID[ZTIndex];
						ZTIndex++;
						break;
					}
				}
				if(ZTIndex > 2)
					break;
			}
		}
	}
	else
	{
		for(i = 0; i < preAxle->u16ChListSum; i++)
		{
			preID[ZTIndex] = preAxle->u16ChList[i];
			for(j = 0; j < currAxle->u16ChListSum; j++)
			{
				currID[ZTIndex] = currAxle->u16ChList[j];
				if(g_ChlData[preID[ZTIndex]].u8Id == g_ChlData[currID[ZTIndex]].u8Id)
				{
					retData.TrigPreID[ZTIndex] = preID[ZTIndex];
					retData.TrigCurrID[ZTIndex] = currID[ZTIndex];
					ZTIndex++;
					break;
				}
			}
			if(ZTIndex > 2)
				break;
		}	
	}
	//开始计算轴距
	if(ZTIndex)
	{
		//l_u16AverSpeed =  (g_AxleInfo[preAxlePos].u16speed + l_pAxleTempInfo->u16speed) / 2;	 //两轴的轴速均值,单位0.1km/h
		for(l_u8tempi = 0; l_u8tempi < ZTIndex; l_u8tempi++)
		{
			l_u32DiffTime = (GetDiff((g_ChlData[retData.TrigPreID[l_u8tempi]].u32uptm + g_ChlData[retData.TrigPreID[l_u8tempi]].u32dntm),
									 (g_ChlData[retData.TrigCurrID[l_u8tempi]].u32uptm + g_ChlData[retData.TrigCurrID[l_u8tempi]].u32dntm))) / 2;
			wheelbase[l_u8tempi].l_u16AxleDist =  preAxle->u16speed * l_u32DiffTime /2160; //距离为cm
			wheelbase[l_u8tempi].l_u32AxleTime =  l_u32DiffTime;
			if(wheelbase[l_u8tempi].l_u16AxleDist >= l_u16AxleDistance_Max.l_u16AxleDist)
			{
				//l_u16AxleDistance_Max = wheelbase[l_u8tempi];
				memcpy(&l_u16AxleDistance_Max,&wheelbase[l_u8tempi],sizeof(SortAxleDist));
			}
			if(wheelbase[l_u8tempi].l_u16AxleDist <= l_u16AxleDistance_Min.l_u16AxleDist)
			{
				memcpy(&l_u16AxleDistance_Min,&wheelbase[l_u8tempi],sizeof(SortAxleDist));
				//l_u16AxleDistance_Min = wheelbase[l_u8tempi];
			}
			if(l_u8tempi == 0)
			{
				//l_u16AxleDistance_Min = wheelbase[0];
				memcpy(&l_u16AxleDistance_Min,&wheelbase[0],sizeof(SortAxleDist));
			}
		}
		l_u16AxleDistance_Mid.l_u16AxleDist = wheelbase[0].l_u16AxleDist + wheelbase[1].l_u16AxleDist + wheelbase[2].l_u16AxleDist - l_u16AxleDistance_Max.l_u16AxleDist - l_u16AxleDistance_Min.l_u16AxleDist;
		l_u16AxleDistance_Mid.l_u32AxleTime = wheelbase[0].l_u32AxleTime + wheelbase[1].l_u32AxleTime + wheelbase[2].l_u32AxleTime - l_u16AxleDistance_Max.l_u32AxleTime - l_u16AxleDistance_Min.l_u32AxleTime;
		//选出的三组窄条计算的得到的三组轴距中,最大值和最小值之差超过1.4m则选取中间值作为轴距;否则取平均值作为轴距
		if((GetDiff(l_u16AxleDistance_Max.l_u16AxleDist, l_u16AxleDistance_Min.l_u16AxleDist) > 140) && (ZTIndex == 3))
		{
			wheelbase[3].l_u16AxleDist = l_u16AxleDistance_Mid.l_u16AxleDist;
			wheelbase[3].l_u32AxleTime = l_u16AxleDistance_Mid.l_u32AxleTime;
		}
		else
		{
			wheelbase[3].l_u16AxleDist = (wheelbase[0].l_u16AxleDist + wheelbase[1].l_u16AxleDist + wheelbase[2].l_u16AxleDist) / ZTIndex;
			wheelbase[3].l_u32AxleTime = (wheelbase[0].l_u32AxleTime + wheelbase[1].l_u32AxleTime + wheelbase[2].l_u32AxleTime) / ZTIndex;
		}		
	}
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\nAxleDist Cnt=%d,%d,%d,%d,P=%d,%d,%d,Dis=%d,%d,Spd=%d.\r\n",
		ZTIndex,g_ChlData[retData.TrigCurrID[0]].u8Id,g_ChlData[retData.TrigCurrID[1]].u8Id,g_ChlData[retData.TrigCurrID[2]].u8Id,
		retData.TrigPreID[0],retData.TrigPreID[1],retData.TrigPreID[2],
		wheelbase[3].l_u16AxleDist,wheelbase[3].l_u32AxleTime,preAxle->u16speed/10);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
    OSSemPost(g_pSemSendThtouSP);
	
	*Dist = wheelbase[3].l_u16AxleDist;
	*p_u32DiffTime = wheelbase[3].l_u32AxleTime;
	//return 
}




int intersect(Axle_Struct *preAxle,Axle_Struct *NextAxle,TrigerID_Struct *retData)
{
	int index1,index2,preID,NextID,ret = 0;
//	int Array3[CHANNELNUM];
	for (index1=0; index1< preAxle->u16ChListSum; index1++)
	{
		preID = preAxle->u16ChList[index1];
		for(index2=0; index2< NextAxle->u16ChListSum; index2++)
		{
			NextID = NextAxle->u16ChList[index2];//
			if(g_ChlData[preID].u8Id == g_ChlData[NextID].u8Id)
			{
				retData->TrigPreID  =  preID;
				retData->TrigNextID =  NextID;
				ret = 1;
				break;
			} 	 	  
		}  
		if(ret == 1)
			break;	
	}
	return ret;
}
void FStudyAlg( Vehicle_Struct *pItem )
{
	uint16 WeightChaZhi;
	uint16 WeightSum;
	uint16 Rand_num;
	uint16 StanderWgh;
	uint8 ZbFlag;
	uint8 Rate,Rate1,Rate2,Rate3,Rate4;
	uint8 l_u8tempi,l_u8tempj;
	uint16 l_u16AxleWgh;
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	uint32 tyrweight;
	ZbFlag = 0;
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg Bgn, ZUHE=%d,%d,%d,%d,%d,%d.\r\n",
						   pItem -> u8AixCnt, PROMODPARAM.m_u8StudyAlgEn[0], PROMODPARAM.m_u8StudyAlgEn[1],
						   PROMODPARAM.m_u8StudyAlgEn[2],PROMODPARAM.m_u8StudyAlgEn[3],PROMODPARAM.m_u8StudyAlgEn[4]);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	///////////////////////////////
	g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
	g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 24;				
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x07;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 24;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = pItem -> u8AixCnt;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = PROMODPARAM.m_u8StudyAlgEn[0];
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = PROMODPARAM.m_u8StudyAlgEn[1];
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = PROMODPARAM.m_u8StudyAlgEn[2];
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = PROMODPARAM.m_u8StudyAlgEn[3];
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][19] = PROMODPARAM.m_u8StudyAlgEn[4];

	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][20] = 0x00;
	g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][21] = 0x00;
	
	AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
	OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
	g_u8CurNetDebugIndex++;
	g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
	//////////////////////////////
	OSSemPost(g_pSemSendThtouSP);
#endif
	if((PROMODPARAM.m_u8StudyAlgEn[0] == 1) && (pItem -> u8AixCnt == 2))
	{
		WeightSum = pItem -> au16AxleWeight[0] + pItem -> au16AxleWeight[1];
		if(PROMODPARAM.m_u16StudyWeight[0][0] != 0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[0][0];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[0][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[0] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[1];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0] * 100 / WeightSum;
					}
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*Rate/100;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*(100-Rate)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[0] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[1];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0] * 100 / WeightSum;
					}
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*Rate/100;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*(100-Rate)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			} 	
		}
		if(PROMODPARAM.m_u16StudyWeight[0][1] != 0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[0][1];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[0][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[0] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[1];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0] * 100 / WeightSum;
					}
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*Rate/100;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*(100-Rate)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[0] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[1];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0] * 100 / WeightSum;
					}
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*Rate/100;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[0][0]*10)+1)/1000)*(100-Rate)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			} 	
		}
	}
	if((PROMODPARAM.m_u8StudyAlgEn[1] == 1) && (pItem -> u8AixCnt == 3))
	{
		WeightSum = pItem -> au16AxleWeight[0] + pItem -> au16AxleWeight[1] + pItem -> au16AxleWeight[2];
		if(PROMODPARAM.m_u16StudyWeight[1][0] != 0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[1][0];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi*100 / StanderWgh  <=PROMODPARAM.m_u8StudyLimit[1][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[2] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[3];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[4];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					}
					
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate/100;
					
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*(100-Rate-Rate1)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[2] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[3];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[4];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*(100-Rate-Rate1)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
		if(PROMODPARAM.m_u16StudyWeight[1][1] != 0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[1][1];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi*100 / StanderWgh  <=PROMODPARAM.m_u8StudyLimit[1][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[2] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[3];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[4];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*(100-Rate-Rate1)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[2] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[3];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[4];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[1][0]*10)+1)/1000)*(100-Rate-Rate1)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
	}
	if((PROMODPARAM.m_u8StudyAlgEn[2] == 1) && (pItem -> u8AixCnt == 4))
	{
		WeightSum = pItem -> au16AxleWeight[0] + pItem -> au16AxleWeight[1] + pItem -> au16AxleWeight[2] + pItem -> au16AxleWeight[3];
		if(PROMODPARAM.m_u16StudyWeight[2][0]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[2][0];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[2][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[5] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[6];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[7];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[8];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[5] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[6];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[7];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[8];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
		if(PROMODPARAM.m_u16StudyWeight[2][1]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[2][1];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[2][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[5] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[6];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[7];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[8];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[5] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[6];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[7];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[8];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[2][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
	}
	if((PROMODPARAM.m_u8StudyAlgEn[3] == 1) && (pItem -> u8AixCnt == 5))
	{
		WeightSum = pItem -> au16AxleWeight[0] + pItem -> au16AxleWeight[1] + pItem -> au16AxleWeight[2] + pItem -> au16AxleWeight[3] + pItem -> au16AxleWeight[4];
		if(PROMODPARAM.m_u16StudyWeight[3][0]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[3][0];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[3][1]))
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[9] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[10];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[11];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[12];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[13];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[9] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[10];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[11];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[12];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[13];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
		if(PROMODPARAM.m_u16StudyWeight[3][1]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[3][1];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[3][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[9] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[10];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[11];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[12];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[13];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[9] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[10];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[11];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[12];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[13];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate1/100;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate2/100;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[3][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
	}
	if((PROMODPARAM.m_u8StudyAlgEn[4] == 1) && (pItem -> u8AixCnt == 6))
	{
		WeightSum = pItem -> au16AxleWeight[0] + pItem -> au16AxleWeight[1] + pItem -> au16AxleWeight[2] + pItem -> au16AxleWeight[3] + pItem -> au16AxleWeight[4]  + pItem -> au16AxleWeight[5];
		if(PROMODPARAM.m_u16StudyWeight[4][0]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[4][0];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[4][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[14] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[15];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[16];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[17];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[18];
						Rate4 = g_cProModParam.m_au8LaneBetwWide[19];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
						Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate2/100;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
//					Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate4/100;
					pItem -> au16AxleWeight[5] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3-Rate4)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[14] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[15];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[16];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[17];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[18];
						Rate4 = g_cProModParam.m_au8LaneBetwWide[19];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
						Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate2/100;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
//					Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate4/100;
					pItem -> au16AxleWeight[5] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3-Rate4)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
		if(PROMODPARAM.m_u16StudyWeight[4][1]!=0)
		{
			StanderWgh = PROMODPARAM.m_u16StudyWeight[4][1];
			if(WeightSum > StanderWgh)
			{
				WeightChaZhi = WeightSum - StanderWgh;
				ZbFlag = 1;
			}
			else
			{
				WeightChaZhi = StanderWgh - WeightSum;
				ZbFlag = 2;
			}
			if((WeightChaZhi *100/ StanderWgh  <=PROMODPARAM.m_u8StudyLimit[4][1]) )
			{
				if(ZbFlag == 1)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[14] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[15];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[16];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[17];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[18];
						Rate4 = g_cProModParam.m_au8LaneBetwWide[19];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
						Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate2/100;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
//					Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					pItem -> au16AxleWeight[3] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate4/100;
					pItem -> au16AxleWeight[5] = (StanderWgh + StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3-Rate4)/100;
				}
				else if(ZbFlag == 2)
				{
					Rand_num = rand();
					if(g_cProModParam.m_au8LaneBetwWide[14] == 1)
					{
						Rate = g_cProModParam.m_au8LaneBetwWide[15];
						Rate1 = g_cProModParam.m_au8LaneBetwWide[16];
						Rate2 = g_cProModParam.m_au8LaneBetwWide[17];
						Rate3 = g_cProModParam.m_au8LaneBetwWide[18];
						Rate4 = g_cProModParam.m_au8LaneBetwWide[19];
					}
					else
					{
						Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
						Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
						Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
						Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
						Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					}
//					Rate = pItem -> au16AxleWeight[0]*100/WeightSum;
					pItem -> au16AxleWeight[0] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate/100;
//					Rate1 = pItem -> au16AxleWeight[1]*100/WeightSum;
//					Rate2 = pItem -> au16AxleWeight[2]*100/WeightSum;
					pItem -> au16AxleWeight[1] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate1/100;
					pItem -> au16AxleWeight[2] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate2/100;
//					Rate3 = pItem -> au16AxleWeight[3]*100/WeightSum;
//					Rate4 = pItem -> au16AxleWeight[4]*100/WeightSum;
					pItem -> au16AxleWeight[3] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate3/100;
					pItem -> au16AxleWeight[4] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*Rate4/100;
					pItem -> au16AxleWeight[5] = (StanderWgh - StanderWgh*(Rand_num%(PROMODPARAM.m_u8StudyLimit[4][0]*10)+1)/1000)*(100-Rate-Rate1-Rate2-Rate3-Rate4)/100;
				}
				pItem->m_u8ProFlag = 1;
			#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Study Alg, Weight=%d.\r\n",
									   WeightSum);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				///////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 20;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x08;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 20;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = ((WeightSum>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (WeightSum & 0xFF);
				
			
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = 0x00;
				
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
			#endif
			}
		}
	}
	////////////////////////
	for(l_u8tempi=0; l_u8tempi < pItem -> u8AixCnt; l_u8tempi++)
	{
		l_u16AxleWgh = pItem -> au16AxleWeight[l_u8tempi];
		pItem -> au16AxleWeight[l_u8tempi] = 0;
		
		for(l_u8tempj=0; l_u8tempj < g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].LeftWheelZTCnt; l_u8tempj++)		
		{
			pItem -> au16AxleWeight[l_u8tempi] += l_u16AxleWgh*g_Setup.an32AxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16LeftWheeList[l_u8tempj]].u8Id]/g_Setup.an16BDAxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16LeftWheeList[l_u8tempj]].u8Id];
		}
		for(l_u8tempj=0; l_u8tempj < g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].RightWheelZTCnt; l_u8tempj++)		
		{
			pItem -> au16AxleWeight[l_u8tempi] += l_u16AxleWgh*g_Setup.an32AxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16RightWheeList[l_u8tempj]].u8Id]/g_Setup.an16BDAxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16RightWheeList[l_u8tempj]].u8Id];
		}
				
//		for(l_u8tempj=0; l_u8tempj < g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].LeftWheelZTCnt + g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].RightWheelZTCnt; l_u8tempj++)
//		{
//			pItem -> au16AxleWeight[l_u8tempi] += l_u16AxleWgh*g_Setup.an32AxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u8Id]/g_Setup.an16BDAxGain[g_ChlData[g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u8Id];
//		}
		
		pItem -> au16AxleWeight[l_u8tempi] = pItem -> au16AxleWeight[l_u8tempi]/(g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].LeftWheelZTCnt + g_AxleInfo[pItem -> u16AxleList[l_u8tempi]].RightWheelZTCnt);
	    tyrweight = pItem -> au16AxleWeight[l_u8tempi]*10;
		FFendu( (uint32*)&tyrweight);
		pItem -> au16AxleWeight[l_u8tempi] = tyrweight/10;
	}
	////////////////////////
}

void FGetCarAxleGroupMode(Vehicle_Struct * l_pn32Tmp)
{
	int AxleModel = CARMDEFAULT;
	uint8 AxleNum;
//	int l_an32UpHalfPos[20];		// 上称半载点
//	int l_an32DownHalfPos[20]; 	// 下称半载点

	int AxleDis[20]; 			// 轴间距
	int AxleW[20]; 				// 轴重
	int AxleV[20]; 				// 轴速
//	int DataState[20]; 			// 获取数据状态
	int l_n32TotalWeight=0;
	int i;

	AxleNum = l_pn32Tmp->u8AixCnt;
	for(i=0; i<AxleNum; i++)
	{
		if(i<AxleNum-1)
			AxleDis[i]= l_pn32Tmp -> au16AxleDistance[i]*10;
		AxleW[i]= g_AxleInfo[l_pn32Tmp->u16AxleList[i]].s32AxleWeight;
		AxleV[i]= g_AxleInfo[l_pn32Tmp->u16AxleList[i]].u16speed/10;
		l_n32TotalWeight += AxleW[i];
	}
	// 判断输入轴状态
	switch (AxleNum)
	{
		case 2:
			AxleModel	= CARM12;
			break;	  				
		
		case 3:
			AxleModel	= CARM112;
			break;
		
		case 4:
			AxleModel	= CARM115;
			break;

		case 5:
			AxleModel	= CARM129;
			break;
		
		case 6:
			AxleModel	= CARM1129;
			break;

		default:
			AxleModel	= CARMDEFAULT;
			break;
	}

	// 获取轴间距		
	switch (AxleNum)
	{
		case 2:		//2轴车
			if ((l_n32TotalWeight >= V2ALLWTUPPER) || (AxleDis[0]>=AX1DISTHRESHOLD && AxleW[1]>=V2AX2WTTHRESHOLD))		
			{	//总重大于阈值，或轴距超过阈值且后轴重超过阈值
				AxleModel	= CARM12;
			}
			else
			{
				AxleModel	= CARM11;
			}
			break;

		case 3:		//3轴车
			if ((abs_my(AxleV[0]-AxleV[1]) << 2) <= min_my(AxleV[0], AxleV[1]))		//前两轴速度相差在一定范围内
			{
				if (AxleDis[1] < AXGRPDISTHRESHOLD)	//2、3轴轴距小于联轴轴距
				{
					AxleModel	= CARM15;
				}
				else						//2、3轴轴距过大
				{
					if (AxleDis[0]<=AX1DISLOWER)		//1、2轴轴距小于前轴距下限阈值
					{
						AxleModel	= CARM112;
					} 
					else if(AxleDis[0]>=AX1DISUPPER)	//1、2轴轴距大于前轴距上限阈值
					{
						AxleModel	= CARM122;
					}
					else								//1、2轴轴距在阈值之间
					{
						if (AxleW[1] * 100 < 160 * AxleW[0])	//2轴重量小于1轴重量的一定倍数
						{
							AxleModel	= CARM112;
						}
						else									//2轴重量大于1轴重量的一定倍数
						{
							AxleModel	= CARM122;
						}
					}
				}
			}
			else		//前两轴速度相差过大
			{
				if (AxleDis[0] >= AxleDis[1])		//若2、3轴轴距小于1、2轴轴距
				{
					AxleModel	= CARM15;
				} 
				else if (AxleW[1] * 100 < 160 * AxleW[0])		//2轴重量小于1轴重量的一定倍数
				{
					AxleModel	= CARM112;
				}
				else											//2轴重量大于1轴重量的一定倍数
				{
					AxleModel	= CARM122;
				}
			}
			break;

		case 4:		//四轴车
			if ((abs_my(AxleV[0]-AxleV[1]) << 2) <= min_my(AxleV[0], AxleV[1]))	 //前两轴速度差距不大
			{	
				if (AxleDis[0] <= AX1DISLOWER)	//前两轴轴距小于阈值
				{
					if (AxleW[3] >= AxleW[2]*2)	//第四轴重量大于第三轴一定比例
					{
						AxleModel	= CARM114;
					}
					else						//第四轴重量不大于第三轴一定比例
					{
						AxleModel	= CARM115;
					}
				}
				else		//前两轴轴距大于阈值
				{  
					AxleModel	= CARM125;
				}				
			}		
			else		//前两轴速度相差过大
			{
				if ((AxleW[2] + AxleW[3])*100 >= 225 * AxleW[1])	//三四轴重量之和大于第二轴一定比例
				{
					if (AxleW[3]>=AxleW[2]*2)		//三四轴重量差距过大
					{
						AxleModel	= CARM114;
					}
					else
					{
						AxleModel	= CARM115;
					}
				}
				else
				{
					AxleModel	= CARM125;
				}
			}
			break;

		case 5:		//5轴车
			if (abs_my(AxleV[0]-AxleV[1])*100<=25*min_my(AxleV[0], AxleV[1]))	//前两轴速度差距不大
			{
				if (abs_my(AxleV[1]-AxleV[2])*100<=25*min_my(AxleV[1], AxleV[2])
					&& abs_my(AxleV[2]-AxleV[3])*100<=25*min_my(AxleV[2], AxleV[3]))		//二三四轴速度差距不大
				{
					if (AxleDis[1] >= AXGRPDISTHRESHOLD)		//2、3轴轴距较大，不是联轴
					{
						if (AxleDis[2] < AXGRPDISTHRESHOLD)			//3、4轴轴距小，是联轴
						{
							if (AxleDis[0] >= AX1DISTHRESHOLD)		//前两轴轴距大于阈值，是12X
							{
								if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)	//后三个轴重量差别较大
									AxleModel	= CARM128;
								else
									AxleModel	= CARM129;
							}
							else		//前两轴轴距小于阈值，是11X
							{  
								if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)
									AxleModel	= CARM118;
								else
									AxleModel	= CARM119;
							}
						}
						else	//3、4轴轴距大，不是联轴
						{
							AxleModel	= CARM1125;
						}
					}
					else	//2、3轴是联轴，认为其是155
					{
						AxleModel	= CARM155;
					}
				}
				else		//二三四轴速度有一定差距
				{
					if (AxleDis[1]>=AxleDis[2])	//二三轴间距大于一二轴
					{
						if (AxleDis[0]>=AX1DISTHRESHOLD)	//一二轴间距超过阈值，为12X
						{
							if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)	//后三轴重量有一定差异
								AxleModel	= CARM128;
							else			//后三轴重量差距不大
								AxleModel	= CARM129;
						}
						else		//一二轴间距小于阈值，为11X
						{ 
							if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)	//后三轴重量有一定差异
								AxleModel	= CARM118;
							else			//后三轴重量差距不大
								AxleModel	= CARM119;
						}	
					}
					else	//二三轴间距小于一二轴
					{
						AxleModel	= CARM155;
					}
				}
			}		
			else	//前两轴速度有点一定差异
			{
				if (AxleDis[1]>=AxleDis[2])	//二三轴轴距大于三四轴轴距
				{
					if (AxleW[1]*100>=AxleW[0]*80)	//第二轴重量大于第一轴一定比例
					{ 
						if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)	//后三轴重量差异不大
							AxleModel	= CARM128;
						else			//后三轴重量有一定差异
							AxleModel	= CARM129;
					}
					else 	//第二轴重量小于第一轴一定比例
					{
						if (AxleW[3]>=AxleW[2]*2 || AxleW[3]>=AxleW[4]*2)
							AxleModel	= CARM118;
						else
							AxleModel	= CARM119;
					}

				}
				else 		//二三轴轴距小于三四轴轴距
				{
					if (AxleW[2]*100<150*AxleW[1])	//第三轴重量小于第二轴的一定倍数
						AxleModel	= CARM155;
					else		//第三种重量大于第二轴的一定倍数
						AxleModel	= CARM1125;
				}
			}																
			break;

		case 6:
			if (abs_my(AxleV[0]-AxleV[1])*100 <= 25*min_my(AxleV[0], AxleV[1]))		//1、2轴速度相差不大
			{
				if(abs_my(AxleV[1]-AxleV[2])*100 <= 25*min_my(AxleV[1], AxleV[2])
					&& abs_my(AxleV[2]-AxleV[3])*100 <= 25*min_my(AxleV[2], AxleV[3]))	//二三四轴速度相差不大
				{
					if (AxleDis[0]>=AX1DISTHRESHOLD)	//一二轴间距较大，不是11X
					{
						if (AxleDis[1]>=AXGRPDISTHRESHOLD)		//二三轴间距较大，不是联轴
						{
							if (AxleDis[2]>=AXGRPDISTHRESHOLD)		//三四轴间距较大，不是联轴
							{
								if (AxleW[4] < AxleW[5]*2)	//5、6轴重量差距在一定范围内
								{
									AxleModel	= CARM1129;
								}
								else 	//5、6轴重量差别较大
								{
									if (AxleW[3]>=AxleW[2]*2)	//三四轴重量差别较大
									{ 
										if((AxleW[3]+AxleW[4])*100<225*AxleW[1])	//4、5轴重量之和小于第二轴的一定倍数
											AxleModel	= CARM12151;
										else	//4、5轴重量之和大于第二轴的一定倍数
											AxleModel	= CARM11151;
									}
									else	//三四轴重量差别在一定范围内
									{
										AxleModel	= CARM1129;
									}
								}
							}
							else	//三四轴间距小，是联轴
							{
								AxleModel	= CARM12151;
							}
						}
						else	//二三轴间距小，是联轴
						{  
							if (AxleDis[2]>=AXGRPDISTHRESHOLD)		//三四轴间距较大，不是联轴
							{
								if (AxleW[2]>=AxleW[1]*2)	//二三轴重量差距较大
									AxleModel	= CARM149;
								else		//二三轴重量差距不大
									AxleModel	= CARM159;
							}
							else	//三四轴间距小，是联轴
							{
								if (AxleW[2]>=AxleW[1]*2)	//二三轴重量差别较大
									AxleModel	= CARM149;
								else		//二三轴重量差别小
									AxleModel	= CARM159;
							}
						}
					}
					else		//一二轴间距较小，是11X
					{
						if (AxleDis[1]>=AXGRPDISTHRESHOLD)		//二三轴间距较大，不是联轴
						{
							if (AxleDis[2]>=AXGRPDISTHRESHOLD)		//三四轴间距较大，不是联轴
							{
								AxleModel	= CARM1129;
							}
							else	//三四轴间距小，是联轴
							{
								AxleModel	= CARM11151;
							}
						}
						else	//二三轴间距小，是联轴
						{  
							if (AxleW[2]>=AxleW[1]*2)	//二三轴重量差别较大
								AxleModel	= CARM149;
							else		//二三轴重量差别小
								AxleModel	= CARM159;
						}
					}
				}
				else	//二三四轴速度有一定差异
				{
					if (AxleDis[1]<AXGRPDISTHRESHOLD)	//二三轴轴距较小，是联轴
					{
						if (AxleW[2]>=AxleW[1]*2)	//二三轴重量差别较大
							AxleModel	= CARM149;
						else		//二三轴重量差别小
							AxleModel	= CARM159;
					}
					else
					{
						if (AxleW[4] < AxleW[5]*2)	//5、6轴重量差距在一定范围内
						{
							AxleModel	= CARM1129;
						}
						else 	//5、6轴重量差别较大
						{
							if (AxleW[3]>=AxleW[2]*2)	//三四轴重量差别较大
							{ 
								if((AxleW[3]+AxleW[4])*100<225*AxleW[1])	//4、5轴重量之和小于第二轴的一定倍数
									AxleModel	= CARM12151;
								else	//4、5轴重量之和大于第二轴的一定倍数
									AxleModel	= CARM11151;
							}
							else	//三四轴重量差别在一定范围内
							{
								AxleModel	= CARM1129;
							}
						}
					}
				}	////二三四轴速度
			}	
			else		//1、2轴速度有差异
			{
				if (l_n32TotalWeight>=12500)	//总重达到一定值
				{
					if (AxleW[2]*100<140*AxleW[1])	//二三轴重量相差不大
					{
						AxleModel = CARM159;
					}
					else	//二三轴重量相差较大
					{
						if ((AxleW[4] >= AxleW[5]*2)&&(AxleW[3]>=AxleW[2]*2))	//三四轴重量差别较大,5、6轴重量差别较大
						{
							if((AxleW[3]+AxleW[4])*100<225*AxleW[1])	//4、5轴重量之和小于第二轴的一定倍数
								AxleModel	= CARM12151;
							else	//4、5轴重量之和大于第二轴的一定倍数
								AxleModel	= CARM11151;
						} 
						else	//三四轴重量差别在一定范围内,或5、6轴重量差别在一定范围内
						{
							AxleModel	= CARM1129;
						}					
					}
				}
				else	//总重小于阈值
				{
					if (AxleDis[0]*100>=150*AxleDis[1])		//1、2轴轴距小于2、3轴轴距的一定倍数
					{
						AxleModel	= CARM159;
					}
					else 	//1、2轴轴距小于2、3轴轴距的差异不大
					{ 
						if ((AxleW[4] >= AxleW[5]*2)&&(AxleW[3]>=AxleW[2]*2))	//三四轴重量差别较大,5、6轴重量差别较大
						{
							if((AxleW[3]+AxleW[4])*100<225*AxleW[1])	//4、5轴重量之和小于第二轴的一定倍数
								AxleModel	= CARM12151;
							else	//4、5轴重量之和大于第二轴的一定倍数
								AxleModel	= CARM11151;
						} 
						else	//三四轴重量差别在一定范围内,或5、6轴重量差别在一定范围内
						{
							AxleModel	= CARM1129;
						}
					}
				}
			}	
			break;
		default:
			AxleModel = CARMDEFAULT;
			break;
	}
	
	switch(AxleModel)
	{
		case CARM11:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;		  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> u8AixGrpCnt=2;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
			break;
		case CARM12:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x12;		  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=2;
			l_pn32Tmp -> u8AixGrpCnt=2;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
			break;
		case CARM15:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x35;		  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=5;
			l_pn32Tmp -> u8AixGrpCnt=2;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1]+l_pn32Tmp -> au16AxleWeight[2];
			break;
		case CARM112:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x12;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=2;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
			break;
		case CARM122:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x12;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x12;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=2;
			l_pn32Tmp -> au8AxleGrpType[2]=2;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
			break;
		case CARM114:
//			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
//			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
//			l_pn32Tmp -> au8AxleTAIType[2]=0x12;	  
//			l_pn32Tmp -> au8AxleGrpType[0]=1;
//			l_pn32Tmp -> au8AxleGrpType[1]=1;
//			l_pn32Tmp -> au8AxleGrpType[2]=4;
//			l_pn32Tmp -> u8AixGrpCnt=3;
//			break;
		case CARM115:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x35;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=5;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3];
			break;
		case CARM125:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x12;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x35;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=2;
			l_pn32Tmp -> au8AxleGrpType[2]=5;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3];
			break;
		case CARM118:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x38;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM119:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x79;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM128:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x12;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x38;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=2;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM129:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x12;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x79;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=2;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM155:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x35;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x35;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=5;
			l_pn32Tmp -> au8AxleGrpType[2]=5;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[2]+l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM1125:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x12;
			l_pn32Tmp -> au8AxleTAIType[3]=0x35;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=2;
			l_pn32Tmp -> au8AxleGrpType[3]=5;
			l_pn32Tmp -> u8AixGrpCnt=4;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
//			l_pn32Tmp -> au16AxleGrpWeight[3]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
			break;
		case CARM149:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x14;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x79;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=4;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1]+l_pn32Tmp -> au16AxleWeight[2];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4]+l_pn32Tmp -> au16AxleWeight[5];
			break;
		case CARM159:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x35;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x79;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=5;
			l_pn32Tmp -> au8AxleGrpType[2]=7;
			l_pn32Tmp -> u8AixGrpCnt=3;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1]+l_pn32Tmp -> au16AxleWeight[2];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4]+l_pn32Tmp -> au16AxleWeight[5];
			break;
		case CARM1129:
		case CARM11151:
		case CARM12151:
			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
			l_pn32Tmp -> au8AxleTAIType[2]=0x12;
			l_pn32Tmp -> au8AxleTAIType[3]=0x79;	  
			l_pn32Tmp -> au8AxleGrpType[0]=1;
			l_pn32Tmp -> au8AxleGrpType[1]=1;
			l_pn32Tmp -> au8AxleGrpType[2]=2;
			l_pn32Tmp -> au8AxleGrpType[3]=7;
			l_pn32Tmp -> u8AixGrpCnt=4;
//			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
//			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
//			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
//			l_pn32Tmp -> au16AxleGrpWeight[3]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4]+l_pn32Tmp -> au16AxleWeight[5];
			break;
//		case CARM11151:
//			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
//			l_pn32Tmp -> au8AxleTAIType[1]=0x01;	
//			l_pn32Tmp -> au8AxleTAIType[2]=0x01;
//			l_pn32Tmp -> au8AxleTAIType[3]=0x35;
//			l_pn32Tmp -> au8AxleTAIType[4]=0x01;	  
//			l_pn32Tmp -> au8AxleGrpType[0]=1;
//			l_pn32Tmp -> au8AxleGrpType[1]=1;
//			l_pn32Tmp -> au8AxleGrpType[2]=1;
//			l_pn32Tmp -> au8AxleGrpType[3]=5;
//			l_pn32Tmp -> au8AxleGrpType[3]=1;
//			l_pn32Tmp -> u8AixGrpCnt=5;
////			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
////			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
////			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
////			l_pn32Tmp -> au16AxleGrpWeight[3]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
////			l_pn32Tmp -> au16AxleGrpWeight[4]=l_pn32Tmp -> au16AxleWeight[5];
//			break;
//		case CARM12151:
//			l_pn32Tmp -> au8AxleTAIType[0]=0x01;	
//			l_pn32Tmp -> au8AxleTAIType[1]=0x12;	
//			l_pn32Tmp -> au8AxleTAIType[2]=0x01;
//			l_pn32Tmp -> au8AxleTAIType[3]=0x35;
//			l_pn32Tmp -> au8AxleTAIType[4]=0x01;	  
//			l_pn32Tmp -> au8AxleGrpType[0]=1;
//			l_pn32Tmp -> au8AxleGrpType[1]=2;
//			l_pn32Tmp -> au8AxleGrpType[2]=1;
//			l_pn32Tmp -> au8AxleGrpType[3]=5;
//			l_pn32Tmp -> au8AxleGrpType[3]=1;
//			l_pn32Tmp -> u8AixGrpCnt=5;
////			l_pn32Tmp -> au16AxleGrpWeight[0]=l_pn32Tmp -> au16AxleWeight[0];
////			l_pn32Tmp -> au16AxleGrpWeight[1]=l_pn32Tmp -> au16AxleWeight[1];
////			l_pn32Tmp -> au16AxleGrpWeight[2]=l_pn32Tmp -> au16AxleWeight[2];
////			l_pn32Tmp -> au16AxleGrpWeight[3]=l_pn32Tmp -> au16AxleWeight[3]+l_pn32Tmp -> au16AxleWeight[4];
////			l_pn32Tmp -> au16AxleGrpWeight[4]=l_pn32Tmp -> au16AxleWeight[5];
//			break;
		default:
			l_pn32Tmp -> u8AixGrpCnt=l_pn32Tmp->u8AixCnt;
			break;
	}
}

void   FCompJDLen(uint32 pMax, uint8 pType, uint32 *pLenMax, uint32 *pLenMin)
{
	pMax=pMax/2;
	if(pType == 1)	   //单胎
	{  
		if( pMax<1000 )	  //最大值小于400
		{
			*pLenMax = 230;		//给出接地长度最大值，单位（米），扩大1000倍
        	*pLenMin = 50;		 //给出接地长度最小值，单位（米），扩大1000倍
		}
		else if(pMax<2300)
		{
			*pLenMax = 1623*pMax/100000+275;
			*pLenMin = 1623*pMax/100000+65;
		}
        else
		{
			*pLenMax = 420;
        	*pLenMin = 130;
		}
	}
	else	   //双胎
	{
		if( pMax<2500)
        {
			*pLenMax = 230;
       		*pLenMin = 70;
		}
		else if(pMax<6000)
		{
			*pLenMax = 3655*pMax/100000+165;
			*pLenMin = 3655*pMax/100000-45;
		}
		else
		{
			*pLenMax = 400;
			*pLenMin = 130;
		}
	}
}

uint16 getjdlen(uint16 s,uint16 c,uint16 m)
{
	uint16 l;
	uint32 ctem;
	uint32 mtem;
	uint32 ltem;
	uint32 stem;
	ctem=c;
	mtem=m;
	stem=s;
	ltem=((2*stem+30)*1000/(100+RootFloat((ctem-mtem)*10000/ctem))-300)/10;
	l=ltem;
	return l;
}

/*********************************************************************************************************
** Function name:		SevenAxleCtl
** Descriptions:		七轴车处理模块
** input parameters:	None 
** output parameters:	none
**
** Created by:			Zhangtonghan		  
** Created Date:		20170515	  
**-------------------------------------------------------------------------------------------------------
*/
void SevenAxleCtl(Vehicle_Struct *pItem)
{
	uint32 l_u32tempweight=0;
	uint8 l_tempi,l_tempj,l_u8Err,l_u8flag=0;
//	uint16 l_u16axleweight=0;
	uint16	l_u16SendLen;
//	l_u16axleweight= pItem->au16AxleWeight[0]; //将首轴重量赋值给该变量
	
	for(l_tempi=0;l_tempi < pItem->u8AixCnt;l_tempi++)  //计算车辆总重
	{
		l_u32tempweight += pItem->au16AxleWeight[l_tempi];
	}
	
	for(l_tempi=0;l_tempi < pItem->u8AixCnt;l_tempi++)  //找出哪跟轴小于1t
	{
		if(pItem->au16AxleWeight[l_tempi] < MinSevWeigh)
			break;
	}			
	if(l_tempi != pItem->u8AixCnt) //该七轴车中有小于1t的，移位
	{
		for(l_tempj=l_tempi;l_tempj < pItem->u8AixCnt-1;l_tempj++)
		{
			pItem->au16AxleWeight[l_tempj]=pItem->au16AxleWeight[l_tempj+1];
			pItem->n8accSpeed[l_tempj] = pItem->n8accSpeed[l_tempj+1];
			pItem->u16AxleList[l_tempj] = pItem->u16AxleList[l_tempj+1];
			pItem->au8AxleTAIType[l_tempj] = pItem->au8AxleTAIType[l_tempj+1];
			pItem->au8AxleGrpType[l_tempj] = pItem->au8AxleGrpType[l_tempj+1];
			pItem->au16AxleGrpWeight[l_tempj] = pItem->au16AxleGrpWeight[l_tempj+1];
			pItem->au16AxleDistance[l_tempj] = pItem->au16AxleDistance[l_tempj+1];
			pItem->m_au8AxlePoint[l_tempj] = pItem->m_au8AxlePoint[l_tempj+1];
		}
		l_u8flag=1;
		pItem->u8AixCnt = 6;
	}
	else
	{
		//l_tempj=0;
		if(l_u32tempweight > MinVehWeigh)
		{
			for(l_tempi=0;l_tempi < pItem->u8AixCnt;l_tempi++)  //找出小于2t的轴
			{
				if(MinSevAxleWeigh > pItem->au16AxleWeight[l_tempi])
				{
					l_u8flag=2;
					break;	
				}
			}
			for(l_tempj=l_tempi;l_tempj < pItem->u8AixCnt-1;l_tempj++)  //将轴移位
			{
				pItem->au16AxleWeight[l_tempj]=pItem->au16AxleWeight[l_tempj+1];
				pItem->n8accSpeed[l_tempj] = pItem->n8accSpeed[l_tempj+1];
				pItem->u16AxleList[l_tempj] = pItem->u16AxleList[l_tempj+1];
				pItem->au8AxleTAIType[l_tempj] = pItem->au8AxleTAIType[l_tempj+1];
				pItem->au8AxleGrpType[l_tempj] = pItem->au8AxleGrpType[l_tempj+1];
				pItem->au16AxleGrpWeight[l_tempj] = pItem->au16AxleGrpWeight[l_tempj+1];
				pItem->au16AxleDistance[l_tempj] = pItem->au16AxleDistance[l_tempj+1];
				pItem->m_au8AxlePoint[l_tempj] = pItem->m_au8AxlePoint[l_tempj+1];
			}
			
			pItem->u8AixCnt = 6;
		}
		//最后一根轴小于前面三根轴的一半或者是大于它们的一半
		else if(((pItem->au16AxleWeight[6] * 2 <  pItem->au16AxleWeight[5]) || (pItem->au16AxleWeight[6] * 10 >  pItem->au16AxleWeight[5] * 15)) && \
			 ((pItem->au16AxleWeight[6] * 2 <  pItem->au16AxleWeight[4]) || (pItem->au16AxleWeight[6] * 10 >  pItem->au16AxleWeight[4] * 15))  && \
		  ((pItem->au16AxleWeight[6] * 2 <  pItem->au16AxleWeight[3]) || (pItem->au16AxleWeight[6] * 10 >  pItem->au16AxleWeight[3] * 15)))
		{
			l_u8flag=3;
			pItem->u8AixCnt = 6;
		}
	}
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "\n Seven Axle Control : %d \n",l_u8flag);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
}

/*********************************************************************************************************
** Function name:		FSearchMaxZT
** Descriptions:		计算斜行算法
** input parameters:	 
** output parameters:	none
**
** Created by:			Zhangtonghan		  
** Created Date:		20180702  
**-------------------------------------------------------------------------------------------------------
*/
void FSearchMaxZT(void * Address[5])
{
	Axle_Struct * l_pAxleTempInfo;
	XieListZT * l_pXielistZT;
	uint8 * l_pmode;
	uint8 l_u16Index,WbNums,l_leftfindflag=0,l_rghtfindflag=0,l_u8flg=0;
	
	
	l_pAxleTempInfo = Address[0];
	l_pXielistZT	= Address[1];
	l_pmode			= Address[2];
	
	
	WbNums = l_pAxleTempInfo->LeftWheelZTCnt;
	for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
	{
		switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u16Index])
		{
			case 0x01:
				if(!(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x01))
				{
					l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
					l_pXielistZT->ZTcnt++;
				}
			break;
			case 0x02:
				if(!(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x01))
				{
					l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
					l_pXielistZT->ZTcnt++;
				}
			break;
			case 0x03:
				if((!(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x01)) || \
					(!(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x01)))
				{
					l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;;
					l_pXielistZT->ZTcnt++;
				}
			break;
		}
	}
	if(l_pXielistZT->ZTcnt == 0)
	{
		for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
		{
			switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u16Index])
			{
				case 0x01:
					if(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x07))
					{
						l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
						l_pXielistZT->ZTcnt++;
					}
				break;
				case 0x02:
					if(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x07))
					{
						l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
						l_pXielistZT->ZTcnt++;
					}
				break;
				case 0x03:
					if((((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x07)) || \
						(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x07)))
					{
						l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
						l_pXielistZT->ZTcnt++;
					}
				break;
			}
		}
		if(l_pXielistZT->ZTcnt == 0)
		{
			for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
			{
				switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u16Index])
				{
					case 0x01:
						if(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x03))
						{
							l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
							l_pXielistZT->ZTcnt++;
						}
					break;
					case 0x02:
						if(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x03))
						{
							l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
							l_pXielistZT->ZTcnt++;
						}
					break;
					case 0x03:
						if((((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x03)) || \
							(((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x03)))
						{
							l_pXielistZT->ZTmapindex[l_pXielistZT->ZTcnt] = l_u16Index;
							l_pXielistZT->ZTcnt++;
						}
					break;
				}
			}
			if(l_pXielistZT->ZTcnt >0 )
				l_leftfindflag = 3;
		}
		else
			l_leftfindflag = 2;
	}
	else
		l_leftfindflag = 1;
	
	//寻找右侧
	WbNums = l_pAxleTempInfo->RightWheelZTCnt;
	for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
	{
		switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u16Index])
		{
			case 0x01:
				if(!(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x01))
				{
					l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
					l_pXielistZT->ZTRghtcnt++;
				}
			break;
			case 0x02:
				if(!(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x01))
				{
					l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
					l_pXielistZT->ZTRghtcnt++;
				}
			break;
			case 0x03:
				if((!(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x01)) || \
					(!(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x01)))
				{
					l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
					l_pXielistZT->ZTRghtcnt++;
				}
			break;
		}
	}
	if(l_pXielistZT->ZTRghtcnt == 0)
	{
		for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
		{
			switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u16Index])
			{
				case 0x01:
					if(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x07))
					{
						l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
						l_pXielistZT->ZTRghtcnt++;
					}
				break;
				case 0x02:
					if(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x07))
					{
						l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
						l_pXielistZT->ZTRghtcnt++;
					}
				break;
				case 0x03:
					if((((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x07)) || \
						(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0E) ||\
						((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x07)))
					{
						l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
						l_pXielistZT->ZTRghtcnt++;
					}
				break;
			}
		}
		if(l_pXielistZT->ZTRghtcnt == 0)
		{
			for(l_u16Index=0;l_u16Index<WbNums;l_u16Index++)
			{
				switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u16Index])
				{
					case 0x01:
						if(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x03))
						{
							l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
							l_pXielistZT->ZTRghtcnt++;
						}
					break;
					case 0x02:
						if(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x03))
						{
							l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
							l_pXielistZT->ZTRghtcnt++;
						}
					break;
					case 0x03:
						if((((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8FirstZTMaxAndUpDnPos & 0x0f) == 0x03)) || \
							(((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x0C) ||\
							((g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u16Index]].u8SecondZTMaxAndUpDnPos & 0x0f) == 0x03)))
						{
							l_pXielistZT->ZTRghtmapindex[l_pXielistZT->ZTRghtcnt] = l_u16Index;
							l_pXielistZT->ZTRghtcnt++;
						}
					break;
				}
			}
			if(l_pXielistZT->ZTRghtcnt >0 )
				l_rghtfindflag = 3;
		}
		else
			l_rghtfindflag = 2;
	}
	else
		l_rghtfindflag = 1;

	//给出选择谁的重量来计算斜行 1:中间位置;2:压3个点;3：压2个点 
	// l_u8mode 1:左侧 2:右侧 3:两个都用
	l_u8flg = ((l_leftfindflag&0x0f) << 4) | (l_rghtfindflag&0x0f);
	
	switch(l_u8flg)
	{
		case 0x11:
		case 0x22:
		case 0x33:
			*l_pmode  = 3;
		break;
		case 0x12:
		case 0x13:
		case 0x23:
		case 0x10:
		case 0x20:
		case 0x30:
			*l_pmode  = 1;
		break;
		case 0x21:
		case 0x31:
		case 0x32:
		case 0x01:
		case 0x02:
		case 0x03:
			*l_pmode  = 2;
		break;
	}
	
}

/*********************************************************************************************************
** Function name:		FJudgWalkRun
** Descriptions:		判断该轴是否为走走停停
** input parameters:	 
** output parameters:	none
**
** Created by:			Zhangtonghan		  
** Created Date:		20180711  
**-------------------------------------------------------------------------------------------------------
*/
void FJudgWalkRun(void * Address[4])
{
	Axle_Struct * l_pAxleTempInfo;
	uint8	*p_walkrunflg;//默认为0;接地长度差1；接地长度差2;接地长度差3；波形不对称4;
	uint8	l_u8tempi,l_u8index=0,l_walkrunflg=0,pos1,pos2;
	uint16	l_u16ZTGap,l_u16ZTlength,l_u16spd;
	uint32	ZT1uptime,ZT2uptime;
	Str_ZTSpdlen	Ztspdlenlist[12];
	uint32 	WaveLft=0,WaveRht=0;
	uint16	Maxspd=0,Minspd=0,MaxLenth=0,MinLength=0;

	
	l_pAxleTempInfo		= Address[0];
	p_walkrunflg		= Address[1];
	
	memset(Ztspdlenlist,0,sizeof(Str_ZTSpdlen)*12);
	
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->LeftWheelZTCnt-1;l_u8tempi++)
	{
		pos1 = JCPostMapped[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id/PROMODPARAM.m_u8ZTRow%2*PROMODPARAM.m_u8ZTRow+g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id%PROMODPARAM.m_u8ZTRow];
		pos2 = JCPostMapped[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u8Id/PROMODPARAM.m_u8ZTRow%2*PROMODPARAM.m_u8ZTRow+g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u8Id%PROMODPARAM.m_u8ZTRow];
		switch(GetDiff(pos1,pos2))
		{
			case 0x01:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]/2;
			break;
			case 0x02:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id];
			break;
			case 0x03:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id] +  PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id+1]/2;
			break;
			case 0x04:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id] +  PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id+1];
			break;
		}
		
		ZT1uptime = (l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi] == 0x03)? g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm:((l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi] == 0x01)? g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmSec:g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmSec);
		ZT2uptime = (l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi+1] == 0x03)? g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32uptm:((l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi+1] == 0x01)? g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32FirstZTUpTmSec:g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32SecondZTUpTmSec);
		switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
		{
			case 0x01:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u16FirstZTMaxValue;
			break;
			case 0x02:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u16SecondZTMaxValue;
			break;
			case 0x03:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u16ptMaxValue;
			break;
			default:
				//l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
			break;
		}
		
		
		l_u16spd = l_u16ZTGap*60/GetDiff(ZT2uptime,ZT1uptime)*36; //放大了100倍，单位0.01Km/h
		Ztspdlenlist[l_u8index].spd 	= l_u16spd;
		Ztspdlenlist[l_u8index].length 	= l_u16ZTlength;
		Ztspdlenlist[l_u8index].ZTpos1	= l_pAxleTempInfo->u16LeftWheeList[l_u8tempi];
		Ztspdlenlist[l_u8index].ZTpos2	= l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1];
		Ztspdlenlist[l_u8index].ZTake1	= l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi];
		l_u8index++;
		if(l_u8tempi == l_pAxleTempInfo->LeftWheelZTCnt-2)
		{
			Ztspdlenlist[l_u8index].spd 	= l_u16spd;
			switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi+1])
			{
				case 0x01:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32FirstZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32FirstZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u16FirstZTMaxValue;
				break;
				case 0x02:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32SecondZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32SecondZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u16SecondZTMaxValue;
				break;
				case 0x03:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1]].u16ptMaxValue;
				break;
				default:
					//l_u16ZTGap = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				break;
			}
			Ztspdlenlist[l_u8index].length 	= l_u16ZTlength;
			Ztspdlenlist[l_u8index].ZTpos1	= l_pAxleTempInfo->u16LeftWheeList[l_u8tempi+1];
			Ztspdlenlist[l_u8index].ZTake1	= l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi+1];
			l_u8index++;
		}
	}
	
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt-1;l_u8tempi++)
	{
		pos1 = JCPostMapped[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id/PROMODPARAM.m_u8ZTRow%2*PROMODPARAM.m_u8ZTRow+g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id%PROMODPARAM.m_u8ZTRow];
		pos2 = JCPostMapped[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u8Id/PROMODPARAM.m_u8ZTRow%2*PROMODPARAM.m_u8ZTRow+g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u8Id%PROMODPARAM.m_u8ZTRow];
		switch(GetDiff(pos1,pos2))
		{
			case 0x01:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]/2;				
			break;
			case 0x02:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id];				
			break;
			case 0x03:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id] +  PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id+1]/2;
			break;
			case 0x04:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id] +  PROMODPARAM.m_au16ZTGap[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id+1];
			break;
		}
		
		ZT1uptime = (l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi] == 0x03)? g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32uptm:((l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi] == 0x01)? g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmSec:g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmSec);
		ZT2uptime = (l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi+1] == 0x03)? g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32uptm:((l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi+1] == 0x01)? g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32FirstZTUpTmSec:g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32SecondZTUpTmSec);
		switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
		{
			case 0x01:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u16FirstZTMaxValue;
			break;
			case 0x02:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u16SecondZTMaxValue;
			break;
			case 0x03:
				l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u16ptMaxValue;
			break;
			default:
				//l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
			break;
		}
		
		
		l_u16spd = l_u16ZTGap*60/GetDiff(ZT2uptime,ZT1uptime)*36; //放大了100倍，单位0.01Km/h
		Ztspdlenlist[l_u8index].spd 	= l_u16spd;
		Ztspdlenlist[l_u8index].length 	= l_u16ZTlength;
		Ztspdlenlist[l_u8index].ZTpos1	= l_pAxleTempInfo->u16RightWheeList[l_u8tempi];
		Ztspdlenlist[l_u8index].ZTpos2	= l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1];
		Ztspdlenlist[l_u8index].ZTake1	= l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi];
		l_u8index++;
		if(l_u8tempi == l_pAxleTempInfo->RightWheelZTCnt-2)
		{
			Ztspdlenlist[l_u8index].spd 	= l_u16spd;
			switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi+1])
			{
				case 0x01:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32FirstZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32FirstZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u16FirstZTMaxValue;
				break;
				case 0x02:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32SecondZTDnTmSec,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32SecondZTUpTmSec)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u16SecondZTMaxValue;
				break;
				case 0x03:
					l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32dntm,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
					Ztspdlenlist[l_u8index].ZTMaxValue = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1]].u16ptMaxValue;
				break;
				default:
					//l_u16ZTlength = l_u16ZTGap*GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm)/GetDiff(ZT2uptime,ZT1uptime);  //单位mm
				break;
			}
			Ztspdlenlist[l_u8index].length 	= l_u16ZTlength;
			Ztspdlenlist[l_u8index].ZTpos1	= l_pAxleTempInfo->u16RightWheeList[l_u8tempi+1];
			Ztspdlenlist[l_u8index].ZTake1	= l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi+1];
			l_u8index++;
		}
	}
	

	/***************************************
	*判断波形对称与否
	*****************************************/
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt+l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
	{
		if(Ztspdlenlist[l_u8tempi].ZTake1 == 0x03)
		{
			WaveLft = GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32Maxtm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm);
			WaveRht = GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32Maxtm);
			if(GetDiff(WaveLft,WaveRht)*100 > 60*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm))
			{
				l_walkrunflg = 2; //波形不对称
				break;
			}
		}
	}
	if(l_walkrunflg == 0) //左右轮都对称，则判断速度及接地长度
	{
		//先按照接地长度排序,排序太浪费CPU，直接对比选择
		MinLength = MaxLenth = Ztspdlenlist[0].length;
		for(l_u8tempi = 1;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt+l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
		{
			MaxLenth = ((Ztspdlenlist[l_u8tempi].length > MaxLenth) ? Ztspdlenlist[l_u8tempi].length:MaxLenth);
			MinLength = ((Ztspdlenlist[l_u8tempi].length < MinLength) ? Ztspdlenlist[l_u8tempi].length:MinLength);
		}

// 				//先按照速度段排序
		Minspd = Maxspd = Ztspdlenlist[0].spd;
		for(l_u8tempi = 1;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt+l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
		{

			Maxspd = ((Ztspdlenlist[l_u8tempi].spd > Maxspd)? Ztspdlenlist[l_u8tempi].spd : Maxspd);
			Minspd = ((Ztspdlenlist[l_u8tempi].spd < Minspd)? Ztspdlenlist[l_u8tempi].spd : Minspd);
		}

		
		if((100*GetDiff(Maxspd,Minspd) >= 20*Minspd)&&(Minspd <= 250))
		{
			if((MinLength < 110) || (MaxLenth >= 2*MinLength))
			{
				l_walkrunflg = 4;
			}
			else
			{
				l_walkrunflg = 0;
			}
		}
		else
			l_walkrunflg = 0;		
	}
	//end 走走停停判断
	*p_walkrunflg = l_walkrunflg;
}
/*********************************************************************************************************
** Function name:		FCalcWalkRun
** Descriptions:		计算走走停停重量
** input parameters:	 
** output parameters:	none
**
** Created by:			Zhangtonghan		  
** Created Date:		20180706  
**-------------------------------------------------------------------------------------------------------
*/
void FCalcWalkRun(void * Address[4])
{
	Axle_Struct * l_pAxleTempInfo;
// 	uint16	l_u16leftspeed[6]={0};
// 	uint16	l_u16rightspeed[6]={0};
	uint32	l_u32Allfitspeed[12]={0};
//	int32	*pTyrtypetemp;
	uint32	*pAxleSpeed;
	uint32	*pAxleWeight,weightmp=0,AllWeight=0,EveryZTWeight[6],MaxWeight=0,MinWeight=0;
	uint8	*p_walkrunflg;//默认为0;接地长度差1；接地长度差2;接地长度差3；波形不对称4;
	uint8	l_u8tempi,l_u8index=0,l_walkrunflg=0;
	int8 	l_u8tempj;
	uint16	l_u16ZTGap,l_u16spd,l_u16JDmean=0,l_u16JDLow=0,l_u16JDHigh=0;//,Minspd=0;

	Str_ZTSpdlen	Ztspdlenlist[12],Sortemp;

	uint16	MaxspdPos=0,Maxspd=0,Maxspdfit=0,MaxMaxvale=0;
	uint8	MinZTId=0,l_u8mapline=0,l_u8maprow=0;
// 	Str_ZTSort	ZTSort[10];
	ListZT	ZTSort[3][3],ZTSortList[9],ZTSortList2[9],ZTSortemp;
	uint8	l_cloum[3];
	uint16 mu_factor=0;//放大系数
	
	l_pAxleTempInfo		= Address[0];
	//pTyrtypetemp		= Address[1];
	pAxleSpeed			= Address[1];
	pAxleWeight			= Address[2];
	

	memset(Ztspdlenlist,0,sizeof(Str_ZTSpdlen)*12);
	memset(&Sortemp,0,sizeof(Str_ZTSpdlen));
	memset(&ZTSortemp,0,sizeof(ListZT));
	memset(&ZTSort,0,sizeof(ListZT)*9);
	memset(&ZTSortList,0,sizeof(ListZT)*9);
	memset(&ZTSortList2,0,sizeof(ListZT)*9);
	memset(&l_cloum,0,sizeof(uint8)*3);
	memset(EveryZTWeight,0,sizeof(uint32)*6);
	memset(l_u32Allfitspeed,0,sizeof(uint32)*12);
	
	//开始走走停停重量计算
	//先计算各个段速度
	//if
	MinZTId = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[0]].u8Id;
	for(l_u8tempi=1;l_u8tempi< l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
	{
		(MinZTId > g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id) ? (MinZTId = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id):MinZTId;
	}
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
	{
		(MinZTId > g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id) ? (MinZTId = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id):MinZTId;
	}
	//左轮放
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
	{
		l_u8mapline = g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id/PROMODPARAM.m_u8ZTRow - MinZTId/PROMODPARAM.m_u8ZTRow;
		l_u8maprow	= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id%PROMODPARAM.m_u8ZTRow;
		switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
		{
			case 0x01:
				ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTSum;
				ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmSec;
				ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTDnTmSec;
			break;
			case 0x02:
				ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTSum;
				ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmSec;
				ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTDnTmSec;
			break;
			case 0x03:
				ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32chlSum;
				ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32uptm;
				ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32dntm;
			break;
		}
		l_cloum[l_u8mapline]++;
		ZTSort[l_u8mapline][l_u8maprow].TakePos 	= l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi];
		ZTSort[l_u8mapline][l_u8maprow].ZTPos		= l_pAxleTempInfo->u16LeftWheeList[l_u8tempi];
		ZTSort[l_u8mapline][l_u8maprow].ZTId		= g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id;
	}
	//右轮放
	for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
	{
		l_u8mapline = g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id/PROMODPARAM.m_u8ZTRow - MinZTId/PROMODPARAM.m_u8ZTRow;
		l_u8maprow	= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id%PROMODPARAM.m_u8ZTRow;
		
		if(ZTSort[l_u8mapline][l_u8maprow].TakePos != 0)
		{
			ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32chlSum;
			ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32uptm;
			ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32dntm;
			ZTSort[l_u8mapline][l_u8maprow].TakePos 	= 0x03;
			ZTSort[l_u8mapline][l_u8maprow].ZTPos		= l_pAxleTempInfo->u16RightWheeList[l_u8tempi];
		}
		else
		{
			switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
			{
				case 0x01:
					ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTSum;
					ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmSec;
					ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTDnTmSec;
				break;
				case 0x02:
					ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTSum;
					ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmSec;
					ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTDnTmSec;
				break;
				case 0x03:
					ZTSort[l_u8mapline][l_u8maprow].u32chlSum	= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32chlSum;
					ZTSort[l_u8mapline][l_u8maprow].u32uptm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32uptm;
					ZTSort[l_u8mapline][l_u8maprow].u32dntm		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32dntm;
				break;
			}
			ZTSort[l_u8mapline][l_u8maprow].TakePos 	= l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi];
			ZTSort[l_u8mapline][l_u8maprow].ZTPos		= l_pAxleTempInfo->u16RightWheeList[l_u8tempi];
			l_cloum[l_u8mapline]++;
		}

		ZTSort[l_u8mapline][l_u8maprow].ZTId		= g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id;
	}
	
	//计算速度
	l_u8index=0;
	for(l_u8tempi=0;l_u8tempi<3;l_u8tempi++)
	{
		for(l_u8tempj=0;l_u8tempj<3;l_u8tempj++)
		{
			if(ZTSort[l_u8tempi][l_u8tempj].TakePos != 0)
			{
				memcpy(&ZTSortList[l_u8index],&ZTSort[l_u8tempi][l_u8tempj],sizeof(ListZT));
				l_u8index++;
			}
		}
	}
	for(l_u8tempi=0;l_u8tempi<l_u8index;l_u8tempi++)
	{
		ZTSortList[l_u8tempi].ZTmapos = JCPostMapped[ZTSortList[l_u8tempi].ZTId/PROMODPARAM.m_u8ZTRow%2*PROMODPARAM.m_u8ZTRow+ZTSortList[l_u8tempi].ZTId%PROMODPARAM.m_u8ZTRow];
	}
	
	//将前两列按照编号排序
	memcpy(ZTSortList2,ZTSortList,sizeof(ListZT)*9);		
	memset(Ztspdlenlist,0,sizeof(Str_ZTSpdlen)*12);
	memset(&Sortemp,0,sizeof(Str_ZTSpdlen));
	memset(&ZTSortemp,0,sizeof(ListZT));
	l_u16ZTGap =0;
	l_u8index = 0;
	
	for(l_u8tempi=1;l_u8tempi<l_cloum[0]+l_cloum[1];l_u8tempi++)
	{
		if(ZTSortList[l_u8tempi].ZTmapos < ZTSortList[l_u8tempi-1].ZTmapos)
		{
			memcpy(&ZTSortemp,&ZTSortList[l_u8tempi],sizeof(ListZT));
			for(l_u8tempj = l_u8tempi-1;l_u8tempj >= 0 && ZTSortList[l_u8tempj].ZTmapos > ZTSortemp.ZTmapos;l_u8tempj--)
				memcpy(&ZTSortList[l_u8tempj+1],&ZTSortList[l_u8tempj],sizeof(ListZT));
			memcpy(&ZTSortList[l_u8tempj+1],&ZTSortemp,sizeof(ListZT));
		}		
	}
	
	for(l_u8tempi=0;l_u8tempi<l_cloum[0]+l_cloum[1]-1;l_u8tempi++)
	{
		switch(GetDiff(ZTSortList[l_u8tempi].ZTmapos,ZTSortList[l_u8tempi+1].ZTmapos))
		{
			case 0x01:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id]/2;
			break;
			case 0x02:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id];
			break;
			case 0x03:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id] + PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id+1]/2;
			break;
			case 0x04:
				l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id] + PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id+1];
			break;
		}
		Ztspdlenlist[l_u8index].spd = l_u16ZTGap*60/GetDiff(g_ChlData[ZTSortList[l_u8tempi].ZTPos].u32uptm,g_ChlData[ZTSortList[l_u8tempi+1].ZTPos].u32uptm)*36; //放大了100倍，单位0.01Km/h
		Ztspdlenlist[l_u8index].ZTpos1 = ZTSortList[l_u8tempi].ZTPos;
		Ztspdlenlist[l_u8index].ZTpos2 = ZTSortList[l_u8tempi+1].ZTPos;
		l_u8index++;
	}
	
	
	//压缝进行另一半的速度计算 且必须大于两根窄条
	if(l_cloum[2] >= 2)
	{
		for(l_u8tempi=l_cloum[0]+1;l_u8tempi<l_cloum[0]+l_cloum[1]+l_cloum[2];l_u8tempi++)
		{
			if(ZTSortList2[l_u8tempi].ZTmapos < ZTSortList2[l_u8tempi-1].ZTmapos)
			{
				memcpy(&ZTSortemp,&ZTSortList2[l_u8tempi],sizeof(ListZT));
				for(l_u8tempj = l_u8tempi-1;l_u8tempj >= 0 && ZTSortList2[l_u8tempj].ZTmapos > ZTSortemp.ZTmapos;l_u8tempj--)
					memcpy(&ZTSortList2[l_u8tempj+1],&ZTSortList2[l_u8tempj],sizeof(ListZT));
				memcpy(&ZTSortList2[l_u8tempj+1],&ZTSortemp,sizeof(ListZT));
			}		
		}
	
		for(l_u8tempi=l_cloum[0];l_u8tempi<l_cloum[0]+l_cloum[1]+l_cloum[2]-1;l_u8tempi++)
		{
			switch(GetDiff(ZTSortList[l_u8tempi].ZTmapos,ZTSortList[l_u8tempi+1].ZTmapos))
			{
				case 0x01:
					l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id]/2;
				break;
				case 0x02:
					l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id];
				break;
				case 0x03:
					l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id] + PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id+1]/2;
				break;
				case 0x04:
					l_u16ZTGap = PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id] + PROMODPARAM.m_au16ZTGap[g_ChlData[ZTSortList[l_u8tempi].ZTPos].u8Id+1];
				break;
			}
			Ztspdlenlist[l_u8index].spd = l_u16ZTGap*60/GetDiff(g_ChlData[ZTSortList[l_u8tempi].ZTPos].u32uptm,g_ChlData[ZTSortList[l_u8tempi+1].ZTPos].u32uptm)*36; //放大了100倍，单位0.01Km/h
			Ztspdlenlist[l_u8index].ZTpos1 = ZTSortList[l_u8tempi].ZTPos;
			Ztspdlenlist[l_u8index].ZTpos2 = ZTSortList[l_u8tempi+1].ZTPos;
			l_u8index++;
		}
		
		
		
		
		
	}
	memset(&Sortemp,0,sizeof(Str_ZTSpdlen));
	for(l_u8tempi = 1;l_u8tempi < l_u8index;l_u8tempi++)
	{
		if(Ztspdlenlist[l_u8tempi].spd < Ztspdlenlist[l_u8tempi-1].spd)
		{
			memcpy(&Sortemp,&Ztspdlenlist[l_u8tempi],sizeof(Str_ZTSpdlen));
			for(l_u8tempj = l_u8tempi-1;l_u8tempj >= 0 && Ztspdlenlist[l_u8tempj].spd > Sortemp.spd;l_u8tempj--)
				memcpy(&Ztspdlenlist[l_u8tempj+1],&Ztspdlenlist[l_u8tempj],sizeof(Str_ZTSpdlen));
			memcpy(&Ztspdlenlist[l_u8tempj+1],&Sortemp,sizeof(Str_ZTSpdlen));
		}
	}
//	Minspd = Ztspdlenlist[0].spd;
	Maxspd = Ztspdlenlist[l_u8index - 1].spd;
	MaxspdPos = Ztspdlenlist[l_u8index - 1].ZTpos1;
	//判断速度
	l_u8tempj=0;
	for(l_u8tempi = 0;l_u8tempi < l_u8index;l_u8tempi++)
	{
		if((GetDiff(Maxspd,Ztspdlenlist[l_u8tempi].spd)*100 < 20*Maxspd) || (Ztspdlenlist[l_u8tempi].spd > 432)) //速度差小于20%或者速度大于1.2m/s
		{
			if((GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm))*100 > \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm)) && \
				(GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm))*100 <= \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm)))
			{
				Ztspdlenlist[l_u8tempi].length = Ztspdlenlist[l_u8tempi].spd/36* GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm)/600 - 5;
				l_u8tempj++;
			}
			else if((GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm))*100 <= \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm)) && \
				(GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm))*100 > \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm)))
			{
				Ztspdlenlist[l_u8tempi].length = Ztspdlenlist[l_u8tempi].spd/36* GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm)/600 - 5;
				l_u8tempj++;
			}
			else if((GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm))*100 <= \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm)) && \
				(GetDiff(GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm),GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u16ptMaxValue,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm))*100 <= \
				20*GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm)))
			{
				Ztspdlenlist[l_u8tempi].length = Ztspdlenlist[l_u8tempi].spd/36* (GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos1].u32uptm)+GetDiff(g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32dntm,g_ChlData[Ztspdlenlist[l_u8tempi].ZTpos2].u32uptm))/1200 - 5;
				l_u8tempj++;
			}
		}
	}
	//是否有对称波形
	if(l_u8tempj > 0)
	{
		for(l_u8tempi = 0;l_u8tempi < l_u8index;l_u8tempi++)
		{
			if(Ztspdlenlist[l_u8tempi].length >0 )
			{
				l_u16JDmean += Ztspdlenlist[l_u8tempi].length;
			}
		}
		l_u16JDmean = l_u16JDmean/l_u8tempj;
		

		if(l_pAxleTempInfo->u8OrderID <3)
		{
			l_u16JDLow 	= MaxMaxvale/(8*600);
			l_u16JDHigh = MaxMaxvale/(4*600);
		}
		else
		{
			l_u16JDLow 	= MaxMaxvale/(16*600);
			l_u16JDHigh = MaxMaxvale/(8*600);
		}
		if(l_u16JDmean < l_u16JDLow)
			l_u16JDmean = l_u16JDLow;
		else if(l_u16JDmean > l_u16JDHigh)
			l_u16JDmean = l_u16JDHigh;
		l_u8index = 0;
		weightmp=0;
		AllWeight = 0;
		if(l_pAxleTempInfo->AxleStatus == 2)
		{
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmFir);
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u16spd;
					break;
					case 0x02:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmFir);
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u16spd;
					break;
					case 0x03:
						l_u16spd = (l_u16JDmean+5)*6000/g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32realCnt;
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32Maxtm/1000/300*l_u16spd;
					break;
				}
			}
			AllWeight = weightmp/PROMODPARAM.m_u8ZTRow/100;
		
			weightmp=0;
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmFir);
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u16spd;
					break;
					case 0x02:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmFir);
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u16spd;
					break;
					case 0x03:
						l_u16spd = (l_u16JDmean+5)*6000/g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32realCnt;
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32Maxtm/1000/300*l_u16spd;
					break;
				}
			}
			AllWeight += weightmp/PROMODPARAM.m_u8ZTRow/100;
		}
		else
		{
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmFir);
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u16spd/100;
					break;
					case 0x02:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmFir);
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u16spd/100;
					break;
					case 0x03:
						l_u16spd = (l_u16JDmean+5)*6000/g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32realCnt;
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32Maxtm/1000/300*l_u16spd/100;
					break;
				}
			}
			if(l_pAxleTempInfo->LeftWheelZTCnt == 3)
			{
				MaxWeight = TheBigOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				MinWeight = TheLitOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				if(GetDiff(MaxWeight,MinWeight)*100 > 60*MinWeight)
					AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]-MinWeight)/2;
				else
					AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2])/3;
			}
			else if(l_pAxleTempInfo->LeftWheelZTCnt == 2)
			{
				AllWeight = (EveryZTWeight[0]+EveryZTWeight[1])/2;
			}
			else
			{
				AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]+EveryZTWeight[3]+EveryZTWeight[4]+EveryZTWeight[5])/PROMODPARAM.m_u8ZTRow;
			}
			memset(EveryZTWeight,0,sizeof(uint32)*6);
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmFir);
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u16spd/100;
					break;
					case 0x02:
						l_u16spd = (l_u16JDmean+5)*6000/GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmFir);
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u16spd/100;
					break;
					case 0x03:
						l_u16spd = (l_u16JDmean+5)*6000/g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32realCnt;
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32Maxtm/1000/300*l_u16spd/100;
					break;
				}
			}
			if(l_pAxleTempInfo->LeftWheelZTCnt == 3)
			{
				MaxWeight = TheBigOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				MinWeight = TheLitOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				if(GetDiff(MaxWeight,MinWeight)*100 > 60*MinWeight)
					AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]-MinWeight)/2;
				else
					AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2])/3;
			}
			else if(l_pAxleTempInfo->LeftWheelZTCnt == 2)
			{
				AllWeight += (EveryZTWeight[0]+EveryZTWeight[1])/2;
			}
			else
			{
				AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]+EveryZTWeight[3]+EveryZTWeight[4]+EveryZTWeight[5])/PROMODPARAM.m_u8ZTRow;
			}
			
		}
	}
	else  //没有对称波形
	{
		//速度拟合		
		Maxspdfit = ZTWidthFittingSpd[g_ChlData[MaxspdPos].u32realCnt/10]; //拟合的最大速度，单位m/s放大1000
		if(Maxspdfit > 600)
		{
			mu_factor = Maxspd*10000/ZTWidthFittingSpd[g_ChlData[MaxspdPos].u32realCnt/10]/36;
			if(ZTWidthFittingSpd[g_ChlData[MaxspdPos].u32realCnt/10]*100 > Maxspd*36*2)
			{
				l_u8index = 0;
				for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
				{
					switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
					{
						case 0x01:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTUpTmFir)/10];
						break;
						case 0x02:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[GetDiff(g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTUpTmFir)/10];
						break;
						case 0x03:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32realCnt/10];
						break;			
					}
				}
				for(l_u8tempi=0;l_u8tempi< l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
				{
					switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
					{
						case 0x01:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTUpTmFir)/10];
						break;
						case 0x02:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[GetDiff(g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTDnTmFir,g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTUpTmFir)/10];
						break;
						case 0x03:
							l_u32Allfitspeed[l_u8index++] = ZTWidthFittingSpd[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32realCnt/10];
						break;			
					}
				}
				for(l_u8tempi=0;l_u8tempi< l_u8index;l_u8tempi++)
				{
					l_u32Allfitspeed[l_u8tempi] = l_u32Allfitspeed[l_u8tempi]*mu_factor/100; //速度放大1000倍
				}
			}
			else
			{
				memset(l_u32Allfitspeed,Maxspdfit,sizeof(uint16)*12);
			}
		}
		else
		{
			memset(l_u32Allfitspeed,Maxspdfit,sizeof(uint16)*12);
		}
		//开始计算重量
		//EveryZTWeight
		if(l_pAxleTempInfo->AxleStatus == 2)
		{
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
					case 0x02:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
					case 0x03:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32chlSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
				}
			}
			AllWeight = weightmp/PROMODPARAM.m_u8ZTRow/1000;
			
			weightmp=0;
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
					case 0x02:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
					case 0x03:
						weightmp += g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32chlSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
				}
			}
			AllWeight += weightmp/PROMODPARAM.m_u8ZTRow/1000;
		}
		else
		{
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->LeftWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8LeftWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
					case 0x02:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
					case 0x03:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16LeftWheeList[l_u8tempi]].u32chlSum/1000/300*l_u32Allfitspeed[l_u8tempi];
					break;
				}
			}
			if(l_pAxleTempInfo->LeftWheelZTCnt == 3)
			{
				MaxWeight = TheBigOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				MinWeight = TheLitOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				if(GetDiff(MaxWeight,MinWeight)*100 > 60*MinWeight)
					AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]-MinWeight)/2000;
				else
					AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2])/3000;
			}
			else if(l_pAxleTempInfo->LeftWheelZTCnt == 2)
			{
				AllWeight = (EveryZTWeight[0]+EveryZTWeight[1])/2000;
			}
			else
			{
				AllWeight = (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]+EveryZTWeight[3]+EveryZTWeight[4]+EveryZTWeight[5])/PROMODPARAM.m_u8ZTRow/1000;
			}
			memset(EveryZTWeight,0,sizeof(uint32)*6);
			for(l_u8tempi = 0;l_u8tempi < l_pAxleTempInfo->RightWheelZTCnt;l_u8tempi++)
			{
				switch(l_pAxleTempInfo->u8RightWheelTakeZT[l_u8tempi])
				{
					case 0x01:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32FirstZTSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
					case 0x02:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32SecondZTSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
					case 0x03:
						EveryZTWeight[l_u8tempi] = g_Setup.an32AxGain[g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u8Id]*g_ChlData[l_pAxleTempInfo->u16RightWheeList[l_u8tempi]].u32chlSum/1000/300*l_u32Allfitspeed[l_pAxleTempInfo->LeftWheelZTCnt+l_u8tempi];
					break;
				}
			}
			if(l_pAxleTempInfo->RightWheelZTCnt == 3)
			{
				MaxWeight = TheBigOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				MinWeight = TheLitOne(EveryZTWeight[0],EveryZTWeight[1],EveryZTWeight[2]);
				if(GetDiff(MaxWeight,MinWeight)*100 > 60*MinWeight)
					AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]-MinWeight)/2000;
				else
					AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2])/3000;
			}
			else if(l_pAxleTempInfo->RightWheelZTCnt == 2)
			{
				AllWeight += (EveryZTWeight[0]+EveryZTWeight[1])/2000;
			}
			else
			{
				AllWeight += (EveryZTWeight[0]+EveryZTWeight[1]+EveryZTWeight[2]+EveryZTWeight[3]+EveryZTWeight[4]+EveryZTWeight[5])/PROMODPARAM.m_u8ZTRow/1000;
			}
		}
	}
	*pAxleWeight =  AllWeight;
	*p_walkrunflg = l_walkrunflg;
	*pAxleSpeed	= Maxspd *100 / 36; //传回去速度为最大速度，Maxspd的单位km/h且放大100倍, 传出去的速度单位为mm/s
}

