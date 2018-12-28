/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JDProcess.C
** Last modified Date:  20110512
** Last Version:		1.0
** Descriptions:		数据分析任务，包括分析行车方向(任务4)和计算重量(任务6)
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wgh
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

#define	__TASK_JDPRO_C

#include "common.h"	
#include "PCF8563.h"
#include  "math.h"

#define		SETUPALIAS				g_Setup			//设置参数结构
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

uint16  su16Axle_2[5][2] = {1650, 2950,
						2950, 3550,
						3670, 3900,
						3670, 6050,
						6050, 8000
					   };

uint16  su16Axle_3[2][2] = {1000,15000,
                        15000,20000};

uint16  su16Axle_4[2][2] = {1000,15000,
                        15000,25000};

uint16  su16Axle_5[2][2] = {1000,10000,
                        10000,25000};

uint16  su16Axle_6[2][2] = {1000,10000,
                        10000,25000};

void  FJDProcess(Vehicle_Struct *pItem)
{
	uint16 l_u16Speed;
	uint16 l_u16VehLength;
	uint16 l_u16TotalAxleDis;
	uint8  l_u8tempi;
	uint8  l_u8LaneNum;
	uint8  l_u8VehType;
	uint32 l_u32TimeBetTwo;
	uint32 l_u32TimeOccupy;
	uint8  l_u8DisBetVeh;
	uint16	l_u16SendLen = 0;
	uint8	l_u8Err = 0;
	uint32  l_u32TotalWgh;
	uint8 i;
	uint32  l_u32DangliangZhou[6] = {0};
	uint32  l_u32DangliangZhouZo = 0;
	uint8   l_u8AxleCnt;
	uint16  l_u16Temp;
//	int32   l_n32Temp;
	l_u8LaneNum = pItem->u8psLane;
	l_u8AxleCnt = pItem->u8AixCnt;
//	if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//	{
//		l_u8LaneNum = l_u8LaneNum * 2;
//	}
	l_u16Speed = pItem->u16speed/36;
	l_u16TotalAxleDis = 0;
	for(l_u8tempi=0; l_u8tempi<l_u8AxleCnt; l_u8tempi++)
	{
		l_u16TotalAxleDis += pItem->au16AxleDistance[l_u8tempi];
	}
	l_u16TotalAxleDis *= 10;
	if(l_u8LaneNum%2 == 0)
	{
		l_u32TimeBetTwo	= tickGetDiff(g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopOnTime, g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LastLoopOnTime);
		l_u32TimeOccupy = g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32TimeOnOff;
		l_u16VehLength = l_u16Speed * g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32TimeOnOff * 625 / 10000 - 1000;
	}
	else
	{
		l_u32TimeBetTwo = tickGetDiff(F_Max(g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopOnTime, g_asLoopPerLaneInfo[l_u8LaneNum/2+1].mu32LoopOnTime),
						              F_Min(g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LastLoopOnTime, g_asLoopPerLaneInfo[l_u8LaneNum/2+1].mu32LastLoopOnTime));
		l_u32TimeOccupy = F_Max(g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32TimeOnOff, g_asLoopPerLaneInfo[l_u8LaneNum/2+1].mu32TimeOnOff);
		l_u16VehLength = l_u16Speed * F_Max(g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32TimeOnOff, g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32TimeOnOff) * 625 / 10000 - 1000;
	}
	l_u32TimeBetTwo = l_u32TimeBetTwo*625/10000/POINTRATE;
	l_u32TimeOccupy = l_u32TimeOccupy*625/10000/POINTRATE*1000;
	l_u8DisBetVeh = l_u16Speed * l_u32TimeBetTwo * 278 / 1000;
	if((PROMODPARAM.m_u8TriggerMode == 4) || (PROMODPARAM.m_u8TriggerMode == 2))
	{
	}
	else
	{
//		l_u8LaneNum = l_u8LaneNum*2;
	}

	 // 当量轴次
	for(l_u8tempi=0; l_u8tempi<l_u8AxleCnt; l_u8tempi++)
	{	
		l_u16Temp = pItem->au16AxleWeight[l_u8tempi];
		l_u32DangliangZhou[l_u8tempi] = (uint32)( (exp(455*(log(l_u16Temp*10)-log(10000))/100))*1000 );
	    l_u32DangliangZhouZo += l_u32DangliangZhou[l_u8tempi];
	}

	l_u8VehType = FJDVehType(l_u8LaneNum, l_u8AxleCnt, l_u16TotalAxleDis, l_u16VehLength);
	pItem->m_u8VehType = l_u8VehType;
	pItem->m_u16LoopVal = g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope;
	switch(l_u8VehType)
	{
		case ZhongXiaoKe:
			g_asTrafficData[l_u8LaneNum/2].mu32ZhongXiaoKeNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32ZhongXiaoKeTotalSpeed +=	(pItem->u16speed/10);
		break;          
		case XiaoHuo:
			g_asTrafficData[l_u8LaneNum/2].mu32XiaoHuoNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32XiaoHuoTotalSpeed +=	(pItem->u16speed/10);
		break;              
		case DaKe:
			g_asTrafficData[l_u8LaneNum/2].mu32DaKeNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32DaKeTotalSpeed += (pItem->u16speed/10);
		break;                 
		case ZhongHuo:
			g_asTrafficData[l_u8LaneNum/2].mu32ZhongHuoNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32ZhongHuoTotalSpeed += (pItem->u16speed/10);
		break;             
		case DaHuo:
			g_asTrafficData[l_u8LaneNum/2].mu32DaHuoNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32DaHuoTotalSpeed += (pItem->u16speed/10);
		break;                
		case TeDaHuo:
			g_asTrafficData[l_u8LaneNum/2].mu32TeDaHuoNum ++;
			g_asTrafficData[l_u8LaneNum/2].mu32TeDaHuoTotalSpeed += (pItem->u16speed/10);
		break;              
	}
	if(l_u32TimeBetTwo < g_u32FollowVehThr)
	{
		g_asTrafficData[l_u8LaneNum/2].mu32FollowVehNum ++;
	}
	g_asTrafficData[l_u8LaneNum/2].mu32VehNum ++;
	g_asTrafficData[l_u8LaneNum/2].mu32TimeOccupancy += l_u32TimeOccupy;
	g_asTrafficData[l_u8LaneNum/2].mu32TotalDisForVehHead += l_u8DisBetVeh;
	#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"JDPAN, Info=%d,%d,%d,%d,%d。\r\n",
							           l_u8VehType, l_u16TotalAxleDis, l_u8LaneNum,g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope,pItem->m_u16LoopVal );
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				//////////////////////////////
				g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
				g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 30;				
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 30;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = l_u8VehType;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = ((l_u16TotalAxleDis >> 8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = (l_u16TotalAxleDis &0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = l_u8LaneNum;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = ((g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope >> 24)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][19] = ((g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope >> 16)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][20] = ((g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope >> 8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][21] = (g_asLoopPerLaneInfo[l_u8LaneNum/2].mu32LoopScope &0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][22] = ((pItem->m_u16LoopVal >> 24)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][23] = ((pItem->m_u16LoopVal >> 16)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][24] = ((pItem->m_u16LoopVal >> 8)&0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][25] = (pItem->m_u16LoopVal &0xFF);
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][26] = 0x00;
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][27] = 0x00;
				AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
				OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
				g_u8CurNetDebugIndex++;
				g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
				//////////////////////////////
				OSSemPost(g_pSemSendThtouSP);
	#endif

	// 统计总重重量分段数据	
	l_u32TotalWgh = 0;
	for(l_u8tempi=0; l_u8tempi<pItem->u8AixCnt; l_u8tempi++)
	{
		l_u32TotalWgh += (pItem->au16AxleWeight[l_u8tempi]*10);
	}
	if( l_u32TotalWgh <= 10000 )
	{
	    i = 0;
	}
	else if( (l_u32TotalWgh%10000)==0 )
	{
	    i = l_u32TotalWgh/10000 -1;
	}
	else
	{
	   	i = l_u32TotalWgh/10000;
	}
	if( i>9 )
	{
	    i = 9;
	}
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].che_num++;                                                    // 车辆总数
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].allche_wt	+= l_u32TotalWgh;                                  // 车总重
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].acc_equivalent_axles += l_u32DangliangZhouZo;                 // 当量轴次合计


	// 统计轴数分段数据
	if((l_u8AxleCnt > 1) && (l_u8AxleCnt < 7))
		i = l_u8AxleCnt - 2;
	else
		i = 0;
	g_asAxleLoadData[l_u8LaneNum/2].axis_num_data[i].che_num++;                                                // 车辆总数
	g_asAxleLoadData[l_u8LaneNum/2].axis_num_data[i].allche_wt += l_u32TotalWgh;                                // 车总重 

	// 统计轴重分段数据
	for(l_u8tempi=0;l_u8tempi<l_u8AxleCnt;l_u8tempi++)
	{
//		if( pItem->au16AxleWeight[l_u8tempi]*10 > JDSYSPARAM.m_u32MaxForAxle )   // 检测轴是否超限
//		{
////		      // 超限标志
//		}
	    if( pItem->au16AxleWeight[l_u8tempi]*10 <= 5000 )
		{
		    i = 0;
		}
		else if( ((pItem->au16AxleWeight[l_u8tempi]*10)%5000)==0 )
		{
		    i = pItem->au16AxleWeight[l_u8tempi]*10/5000 - 1;
		}
		else
		{
		   	i = pItem->au16AxleWeight[l_u8tempi]*10/5000;
		}
		if( i>3 )
		{
		    i = 3;
		}  
 	    g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].axis_num++;                                               // 轴总数	
		g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].allaxle_wt += pItem->au16AxleWeight[l_u8tempi]*10;							     // 轴总重
        g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].acc_equivalent_axles += l_u32DangliangZhou[l_u8tempi];         // 当量轴次合计
	}
		
}

uint8  FJDVehType(uint8 pLane, uint8 pAxleNum, uint16 pTotalDis, uint16 pVehLength)
{
	uint8 l_u8CarType = 0;
	uint8 l_u8tempi;
	switch(pAxleNum)
	{
		case 2:
			if((pTotalDis >= 1650) && (pTotalDis <= g_u16SmallandMid))
			{
				l_u8CarType = FJDJudgeVehType(pLane, 0);
			}
			else if((pTotalDis > g_u16SmallandMid) && (pTotalDis<=8000))
			{
				l_u8CarType = FJDJudgeVehType(pLane, 1);
			}
			else
			{
				if(pTotalDis<1650)	    
	            {
	                return 1;     // 轴距异常，默认小客车  
	            }
	
	            if(pTotalDis>8000)  
	            {
	                return 4;     // 轴距异常，默认中型货车  
	            }
			}
			return l_u8CarType;
//		break;
		case 3:
			for(l_u8tempi = 0; l_u8tempi<2; l_u8tempi++)
			{
				if(IsInMid(pTotalDis, su16Axle_3[l_u8tempi][0], su16Axle_3[l_u8tempi][1]))
				{
					switch(l_u8tempi)
					{
						case 0:
							l_u8CarType = 5;
							return l_u8CarType;
						case 1:
							l_u8CarType = 6;
							return l_u8CarType;
					}
				}
			}
			l_u8CarType = 5;
			return l_u8CarType;
//		break;
		case 4:
			for(l_u8tempi = 0; l_u8tempi<2; l_u8tempi++)
			{
				if(IsInMid(pTotalDis, su16Axle_4[l_u8tempi][0], su16Axle_4[l_u8tempi][1]))
				{
					switch(l_u8tempi)
					{
						case 0:
							l_u8CarType = 5;
							return l_u8CarType;
						case 1:
							l_u8CarType = 6;
							return l_u8CarType;
					}
				}
			}
			l_u8CarType = 5;
			return l_u8CarType;
//		break;
		case 5:
			for(l_u8tempi = 0; l_u8tempi<2; l_u8tempi++)
			{
				if(IsInMid(pTotalDis, su16Axle_5[l_u8tempi][0], su16Axle_5[l_u8tempi][1]))
				{
					l_u8CarType = 6;
					return l_u8CarType;
				}
			}
			l_u8CarType = 6;
			return l_u8CarType;
//		break;
		case 6:
			for(l_u8tempi = 0; l_u8tempi<2; l_u8tempi++)
			{
				if(IsInMid(pTotalDis, su16Axle_6[l_u8tempi][0], su16Axle_6[l_u8tempi][1]))
				{
					l_u8CarType = 6;
					return l_u8CarType;
				}
			}
			l_u8CarType = 6;
			return l_u8CarType;
//		break;
		default:
			l_u8CarType = 6;
			return l_u8CarType;
//		break;
	}
}

uint8  FJDJudgeVehType(uint8 pLane, uint8 pCarType)
{
	uint32 l_u32LoopScope;
	if(pLane%2 != 0)
	{
		if(g_asLoopPerLaneInfo[pLane/2].mu32LoopScope >= g_asLoopPerLaneInfo[pLane/2+1].mu32LoopScope)
		{
			l_u32LoopScope = g_asLoopPerLaneInfo[pLane/2].mu32LoopScope * 2;
		}
		else
		{
			l_u32LoopScope = g_asLoopPerLaneInfo[pLane/2+1].mu32LoopScope * 2;
		}
	}
	else
	{
		l_u32LoopScope = g_asLoopPerLaneInfo[pLane/2].mu32LoopScope;
	}
	if(pCarType == 0)
	{
		if(l_u32LoopScope >= JDSYSPARAM.m_u16SmallKeHuoThr[pLane/2])
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if(l_u32LoopScope >= JDSYSPARAM.m_u16MidKeHuoThr[pLane/2])
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}

}

uint8  FJDInfToSaveSend(uint16 pTimeNum)
{
	uint8   l_pu8DataBuf[300];
	uint8   l_u8BufIndex = 0;
	uint8   l_u8tempi;
	uint16  l_u16CRCResult;
	uint32  l_u32SaveAddr;
	uint32  l_u32Ret;
//	l_pu8DataBuf = g_cJDSendVehData.au8data[0];
	if((l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0))
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	switch(JDSYSPARAM.m_u8OrderMod)
	{
		case 1:
		//命令号 串口发数用
			l_pu8DataBuf[l_u8BufIndex++] = 0xFF;
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			l_pu8DataBuf[l_u8BufIndex++] = 0x08;
			//数据长度
			l_pu8DataBuf[l_u8BufIndex++] = (54+32*PROMODPARAM.m_u8LaneNum<<8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (54+32*PROMODPARAM.m_u8LaneNum)&0xff;	//42字节公共数据，5字节单车道公共数据，每种车型3字
			//数据包类型
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			//设备身份识别码
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
			}
			//站点编号
			for(l_u8tempi=0;l_u8tempi<15;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
			}
			//设备硬件错误代码
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			//调查内容
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			
			//年
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u16Year;
			l_pu8DataBuf[l_u8BufIndex++] = (l_sDevCurTime.u16Year>>8);
			//月
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Month;
			//日
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Day;
			
			//交通数据处理周期
			l_pu8DataBuf[l_u8BufIndex++] = g_u8PeriodTOSave;
			//时间序号 低位在前
			l_pu8DataBuf[l_u8BufIndex++] = pTimeNum&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (pTimeNum>>8)&0xff;

			//车道数
			l_pu8DataBuf[l_u8BufIndex++] = PROMODPARAM.m_u8LaneNum;
		
			for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)
			{	
				// 将车道号转换成国标车道号
		 	  	l_pu8DataBuf[l_u8BufIndex++] = FChangLaneToBG(l_u8tempi);  
				
				//跟车百分比
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 100*(g_asTrafficData[l_u8tempi].mu32FollowVehNum-1)/g_asTrafficData[l_u8tempi].mu32VehNum;
					if(g_asTrafficData[l_u8tempi].mu32FollowVehPercent >= 100)
					{
					 	g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 10;
					}
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32FollowVehPercent;
			
				//平均车头间距
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = g_asTrafficData[l_u8tempi].mu32TotalDisForVehHead/g_asTrafficData[l_u8tempi].mu32VehNum;
				}
				//保护
				if(g_asTrafficData[l_u8tempi].mu32AveDisForVehHead == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 100;
				}
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead>>8)&0xff;
			
				//时间占有率
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TimeOccupancy*100/60000;

				//中小客交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum>>8)&0xff;
					
				//中小客平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed;

				//小货交通量
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoNum&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32XiaoHuoNum>>8)&0xff;
							
				//小货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32XiaoHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32XiaoHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32XiaoHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed;

				//大客交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum>>8)&0xff;
							
				//大客平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32DaKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = g_asTrafficData[l_u8tempi].mu32DaKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaKeNum;
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed;
							
				//中货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum>>8)&0xff;	
						
				//中货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32ZhongHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed;	
			
				//大货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum>>8)&0xff;
							
				//大货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32DaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32DaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaHuoNum;			
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed;

				//特大货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum>>8)&0xff;
							
				//特大货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32TeDaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32TeDaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32TeDaHuoNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed;
			
				//集装箱交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum>>8)&0xff;
							
				//集装箱平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = g_asTrafficData[l_u8tempi].mu32JiZhangXiangTotalSpeed/g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed;
				
				//拖拉机交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum>>8)&0xff;
							
				//拖拉机平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32TuoLaJiNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = g_asTrafficData[l_u8tempi].mu32TuoLaJiTotalSpeed/g_asTrafficData[l_u8tempi].mu32TuoLaJiNum;		
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed;
	
				//摩托车交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum>>8)&0xff;	
						
				//摩托车平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32MotoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32MotoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32MotoAveSpeed = g_asTrafficData[l_u8tempi].mu32MotoTotalSpeed/g_asTrafficData[l_u8tempi].mu32MotoNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32MotoAveSpeed;
			}

			memcpy(g_au8JDUartSendBuf+2,l_pu8DataBuf+5,l_u8BufIndex-5);
			g_au8JDUartSendBuf[0] = (l_u8BufIndex-5)&0xff;
			g_au8JDUartSendBuf[1] = ((l_u8BufIndex-5)>>8)&0xff;
			l_u16CRCResult = AddCrc16(g_au8JDUartSendBuf,l_u8BufIndex-3);
			//添加网络帧帧长度
			l_pu8DataBuf[l_u8BufIndex++] = (l_u8BufIndex-5)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = ((l_u8BufIndex-5)>>8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = l_u16CRCResult & 0xFF;
			l_pu8DataBuf[l_u8BufIndex++] = (l_u16CRCResult>>8) & 0xFF;
			memcpy(g_au8JDSDSaveBuf,l_pu8DataBuf+5,l_u8BufIndex-9);
			g_u16JDSDSaveLen = l_u8BufIndex-9;
		break;
		case 2:
			l_pu8DataBuf[l_u8BufIndex++] = 0xFF;
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			l_pu8DataBuf[l_u8BufIndex++] = 0x08;
			//数据长度
			l_pu8DataBuf[l_u8BufIndex++] = (54+68*PROMODPARAM.m_u8LaneNum<<8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (54+68*PROMODPARAM.m_u8LaneNum)&0xff;	//42字节公共数据，5字节单车道公共数据，每种车型3字
			//数据包类型
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			//设备身份识别码
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
			}
			//站点编号
			for(l_u8tempi=0;l_u8tempi<15;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
			}
			//设备硬件错误代码
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			//调查内容
			l_pu8DataBuf[l_u8BufIndex++] = 0x02;
			
			//年
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u16Year;
			l_pu8DataBuf[l_u8BufIndex++] = (l_sDevCurTime.u16Year>>8);
			//月
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Month;
			//日
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Day;
			
			//交通数据处理周期
			l_pu8DataBuf[l_u8BufIndex++] = g_u8PeriodTOSave;
			//时间序号 低位在前
			l_pu8DataBuf[l_u8BufIndex++] = pTimeNum&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (pTimeNum>>8)&0xff;

			//车道数
			l_pu8DataBuf[l_u8BufIndex++] = PROMODPARAM.m_u8LaneNum;
		
			for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)
			{	
				// 将车道号转换成国标车道号
		 	  	l_pu8DataBuf[l_u8BufIndex++] = FChangLaneToBG(l_u8tempi);  
				
				//跟车百分比
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 100*(g_asTrafficData[l_u8tempi].mu32FollowVehNum-1)/g_asTrafficData[l_u8tempi].mu32VehNum;
					if(g_asTrafficData[l_u8tempi].mu32FollowVehPercent >= 100)
					{
					 	g_asTrafficData[l_u8tempi].mu32FollowVehPercent = 10;
					}
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32FollowVehPercent;
			
				//平均车头间距
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = g_asTrafficData[l_u8tempi].mu32TotalDisForVehHead/g_asTrafficData[l_u8tempi].mu32VehNum;
				}
				//保护
				if(g_asTrafficData[l_u8tempi].mu32AveDisForVehHead == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 100;
				}
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead>>8)&0xff;
			
				//时间占有率
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TimeOccupancy*100/60000;

				//中小客交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum>>8)&0xff;
					
				//中小客平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;

				//小货交通量
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoNum&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32XiaoHuoNum>>8)&0xff;
							
				//小货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32XiaoHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32XiaoHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32XiaoHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;

				//大客交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum>>8)&0xff;
							
				//大客平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32DaKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = g_asTrafficData[l_u8tempi].mu32DaKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaKeNum;
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
							
				//中货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum>>8)&0xff;	
						
				//中货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32ZhongHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed;	

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
			
				//大货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum>>8)&0xff;
							
				//大货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32DaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32DaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaHuoNum;			
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;

				//特大货交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum>>8)&0xff;
							
				//特大货平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32TeDaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32TeDaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32TeDaHuoNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
			
				//集装箱交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum>>8)&0xff;
							
				//集装箱平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = g_asTrafficData[l_u8tempi].mu32JiZhangXiangTotalSpeed/g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				
				//拖拉机交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum>>8)&0xff;
							
				//拖拉机平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32TuoLaJiNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = g_asTrafficData[l_u8tempi].mu32TuoLaJiTotalSpeed/g_asTrafficData[l_u8tempi].mu32TuoLaJiNum;		
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
	
				//摩托车交通量
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum>>8)&0xff;	
						
				//摩托车平均地点车速
				if(g_asTrafficData[l_u8tempi].mu32MotoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32MotoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32MotoAveSpeed = g_asTrafficData[l_u8tempi].mu32MotoTotalSpeed/g_asTrafficData[l_u8tempi].mu32MotoNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32MotoAveSpeed;

				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
				l_pu8DataBuf[l_u8BufIndex++] = 0;
			}
			memcpy(g_au8JDUartSendBuf+2,l_pu8DataBuf+5,l_u8BufIndex-5);
			g_au8JDUartSendBuf[0] = (l_u8BufIndex-5)&0xff;
			g_au8JDUartSendBuf[1] = ((l_u8BufIndex-5)>>8)&0xff;
			l_u16CRCResult = AddCrc16(g_au8JDUartSendBuf,l_u8BufIndex-3);
			//添加网络帧帧长度
			l_pu8DataBuf[l_u8BufIndex++] = (l_u8BufIndex-5)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = ((l_u8BufIndex-5)>>8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = l_u16CRCResult & 0xFF;
			l_pu8DataBuf[l_u8BufIndex++] = (l_u16CRCResult>>8) & 0xFF;
			memcpy(g_au8JDSDSaveBuf,l_pu8DataBuf+5,l_u8BufIndex-9);
			g_u16JDSDSaveLen = l_u8BufIndex-9;
		break;
		default:
		break;
	}
	for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
	{
		memset(&g_asTrafficData[l_u8tempi], 0, sizeof(LANE_TRAFFIC_DATA));
	}
	g_au8JDSDSaveBuf[510] = (g_u16JDSDSaveLen>>8) & 0xFF;
	g_au8JDSDSaveBuf[511] = (g_u16JDSDSaveLen) & 0xFF;

	///////////////////////////////////////////往SD存
	g_u32JDSDSaveInd	= FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	l_u32SaveAddr = SD_JD_SEC_BGN + g_u32JDSDSaveInd;

	
	// 数据备份到SD卡中，按照时间进行索引
	l_u32Ret = SD_WriteBlock( &sd_info, l_u32SaveAddr, g_au8JDSDSaveBuf);   
	
	if( l_u32Ret != 0 )		//写SD卡出错 					 
	{
//		SD_Err_Reset_cnt++;
	}
	else
	{
//	    SD_Err_Reset_cnt = 0;
	}							  

	if( g_u8IsFirstSDSave != 1 )     // 未存储过01包
	{
		// 置位01包存储标志
		g_u8IsFirstSDSave = 1;
		FSave01ToTDC(3);

		if( g_u32JDSDSaveInd == 0 )
		{
		    g_u32JDSDSaveIndSuc = 0;
		}
		else
		{
			g_u32JDSDSaveIndSuc = FGetLastSaveNum(g_u32JDSDSaveInd,g_u8PeriodTOSave);  
		}
		// 存储已发送成功的01包序号
		FSave01ToTDC(2);
	}

	if( g_u32JDSDSaveInd < g_u32JDSDSaveIndSuc )    // 定时周期变化时
	{
 	    g_u32JDSDSaveIndSuc = FGetLastSaveNum(g_u32JDSDSaveInd,g_u8PeriodTOSave);  
	    // 存储01包成功发送的序号
	    FSave01ToTDC(2);
	}

    // 存储当前01包序号
	FSave01ToTDC(1);

	// 生成CRC
//	crc_create(frame, len);
//	// 发送
//	uart0_sendStr(frame, len+3);	 
	return l_u32SaveAddr;	    // 返回SD卡扇区地址
}

void  FTime_Process(SystemTime* psTime)
{
	if( psTime->u8Day == 1 )
	{
		switch (psTime->u8Month )
		{
		    case 1:
				   psTime->u16Year = psTime->u16Year - 1;
				   psTime->u8Month = 12;
				   psTime->u8Day = 31;
				   break;
			case 2:
				   psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 1;
				   psTime->u8Day = 31;
				   break;
			case 3:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 2;
				   if( FIsRunYear(psTime->u16Year) == 1 )
				   {
				       psTime->u8Day = 29;
				   }
				   else 
				   {
				   	   psTime->u8Day = 28;
				   }
			       break;
			case 4:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 3;
				   psTime->u8Month = 31;
				   break;
			case 5:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 4;
				   psTime->u8Month = 30;
				   break;
			case 6:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 5;
				   psTime->u8Day = 31;
				   break;
			case 7:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 6;
				   psTime->u8Day = 30;
				   break;
			case 8:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 7;
				   psTime->u8Day = 31;
				   break;
			case 9:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 8;
				   psTime->u8Day = 31;
				   break;
			case 10:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 9;
				   psTime->u8Day = 30;
				   break;
			case 11:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 10;
				   psTime->u8Day = 31; 
				   break;
			case 12:
			       psTime->u16Year = psTime->u16Year;
				   psTime->u8Month = 11;
				   psTime->u8Day = 30; 
				   break;
			default:
			       break;
		}	
	}
	else
	{
	    psTime->u16Year = psTime->u16Year;
		psTime->u8Month = psTime->u8Month;
		psTime->u8Day = psTime->u8Day - 1;
	}
}

uint8  FChangLaneToBG(uint8 pLane)
{
	uint8 l_u8BGLane;

	switch(PROMODPARAM.m_u8LaneNum)
	{
	    case 1:
			   l_u8BGLane = 0x01;
			   break;
		case 2:
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// 单向车道
			   {
                    //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 12;
							   break;
					}				       
			   }
			   else 	                // 双向车道
			   {
			        //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 31;
							   break;
					}
			   }
			   break;
		case 3:
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// 单向车道
			   {
                    //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 12;
							   break;
						case 2:
							   l_u8BGLane = 13;
							   break;
					}				       
			   }
			   else 	                // 双向车道
			   {
			        //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 31;
							   break;
						case 2:
							   l_u8BGLane = 32;
							   break;
					}
			   }
			   break;
		 case 4:
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// 单向车道
			   {
                    //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 12;
							   break;
						case 2:
							   l_u8BGLane = 13;
							   break;
						case 3:
							   l_u8BGLane = 14;
							   break;
					}		       
			   }
			   else 	                // 双向车道
			   {
			        //车道号
					switch(pLane)
					{
						case 0:
							   l_u8BGLane = 11;
							   break;
						case 1:
							   l_u8BGLane = 12;
							   break;
						case 2:
							   l_u8BGLane = 31;
							   break;
						case 3:
							   l_u8BGLane = 32;
							   break;
					}
			   }
			   break;
	 }

	 return l_u8BGLane;
}

uint32 FGetSaveNumFromDate(SystemTime* psTime, uint16 pTimeNum)
{
	uint16 l_u16Year;
    uint8  l_u8Month;
    uint8  l_u8Day;
    uint16 l_u16TimeNum;
    uint16 l_u16Totaldays = 0;  //总的天数
    uint32 l_u32NumberID;	       //计算得到的序列号
    uint32 l_u32NumberID1;	
    uint8  l_u8DaysPerMon[13];
    uint8  l_u8tempi;
    l_u16Year = psTime->u16Year;
    l_u8Month = psTime->u8Month;

    l_u8Day = psTime->u8Day;
    l_u16TimeNum = pTimeNum;
   	  
    if( (l_u16Year%100)== 0 )       // 整百年
    {
	     if( (l_u16Year%400)==0 )    // 闰年
		 {
			l_u8DaysPerMon[2]=29;  
	     }
		 else	           
		 {
			l_u8DaysPerMon[2]=28;   // 平年
		 }
    }
	else
	{
		if( (l_u16Year%4)==0 )    // 闰年
		{
			l_u8DaysPerMon[2]=29;  
		}
		else	           
		{
			l_u8DaysPerMon[2]=28;   // 平年
		}
	}
	
	l_u8DaysPerMon[0]=0;
	l_u8DaysPerMon[1]=31;

	l_u8DaysPerMon[3]=31;
	l_u8DaysPerMon[4]=30;
	l_u8DaysPerMon[5]=31;
	l_u8DaysPerMon[6]=30;
	l_u8DaysPerMon[7]=31;
	l_u8DaysPerMon[8]=31;
	l_u8DaysPerMon[9]=30;
	l_u8DaysPerMon[10]=31;
	l_u8DaysPerMon[11]=30;
	l_u8DaysPerMon[12]=31;

	for(l_u8tempi=1;l_u8tempi<l_u8Month;l_u8tempi++)
    {
		l_u16Totaldays = l_u16Totaldays+l_u8DaysPerMon[l_u8tempi];
	}
	l_u16Totaldays = l_u16Totaldays + l_u8Day-1;
	l_u16Year--;
	
	while(l_u16Year>=2014)
	{
	    if( (l_u16Year%100)== 0 )       // 整百年
        {
	        if( (l_u16Year%400)==0 )    // 闰年
		    {
			    l_u8DaysPerMon[2]=29;  
	     	}
			else	           
			{
			    l_u8DaysPerMon[2]=28;   // 平年
			}
        }
	    else
	    {
		    if( (l_u16Year%4)==0 )    // 闰年
		    {
			    l_u8DaysPerMon[2]=29;  
			}
			else	           
			{
				l_u8DaysPerMon[2]=28;   // 平年
			}
	    }

		l_u8DaysPerMon[0]=0;
		l_u8DaysPerMon[1]=31;
		
		l_u8DaysPerMon[3]=31;
		l_u8DaysPerMon[4]=30;
		l_u8DaysPerMon[5]=31;
		l_u8DaysPerMon[6]=30;
		l_u8DaysPerMon[7]=31;
		l_u8DaysPerMon[8]=31;
		l_u8DaysPerMon[9]=30;
		l_u8DaysPerMon[10]=31;
		l_u8DaysPerMon[11]=30;
		l_u8DaysPerMon[12]=31;

		for(l_u8tempi=1;l_u8tempi<13;l_u8tempi++)
	    {
			l_u16Totaldays = l_u16Totaldays+l_u8DaysPerMon[l_u8tempi];
		}
		l_u16Year--;
    }
    l_u32NumberID = l_u16Totaldays*1440 + l_u16TimeNum;
	l_u16Year = psTime->u16Year;

	if((l_u16Year - 2000)%3==0)
	{
	  	l_u32NumberID1 = l_u32NumberID - FGetPreYearNum(l_u16Year); 
	}
    else if((l_u16Year - 2000)%3==1)
    {
  	    l_u16Year--;
	    l_u32NumberID1 = l_u32NumberID - FGetPreYearNum(l_u16Year); 
    }
    else if((l_u16Year - 2000)%3==2)
    {
  	    l_u16Year = l_u16Year - 2;
	    l_u32NumberID1 = l_u32NumberID - FGetPreYearNum(l_u16Year); 
    }

    return l_u32NumberID1;
}

uint32 FGetPreYearNum(uint16 pYear)
{
	uint8   i;
//    uint8   l_u8Month;
//    uint8   l_u8Day;	
	uint16  l_u16Year;
//    uint16  l_u16TimeNum;
    uint16  l_u16TotalDays = 0;  //总的天数
    uint32  l_u32NumberID;	    //计算得到的序列号
    uint8   l_u8DaysPerMon[13];
 
    l_u16Year    = pYear - 1;
//    l_u8Month   = 1;
//    l_u8Day     = 1;
//    l_u16TimeNum = 1;

	while(l_u16Year>=2014)
	{
		if( (l_u16Year%100)== 0 )       // 整百年
        {
	    	if( (l_u16Year%400)==0 )    // 闰年
		    {
			    l_u8DaysPerMon[2]=29;  
	        }
		    else	           
		    {
			    l_u8DaysPerMon[2]=28;   // 平年
		    }
        }
	    else
	    {
		    if( (l_u16Year%4)==0 )    // 闰年
		    {
			    l_u8DaysPerMon[2]=29;  
		    }
		    else	           
		    {
			    l_u8DaysPerMon[2]=28;   // 平年
		    }
	    }
	
		l_u8DaysPerMon[0]=0;
		l_u8DaysPerMon[1]=31;
		
		l_u8DaysPerMon[3]=31;
		l_u8DaysPerMon[4]=30;
		l_u8DaysPerMon[5]=31;
		l_u8DaysPerMon[6]=30;
		l_u8DaysPerMon[7]=31;
		l_u8DaysPerMon[8]=31;
		l_u8DaysPerMon[9]=30;
		l_u8DaysPerMon[10]=31;
		l_u8DaysPerMon[11]=30;
		l_u8DaysPerMon[12]=31;

		for(i=1;i<13;i++)
	    {
			l_u16TotalDays=l_u16TotalDays+l_u8DaysPerMon[i];
		}
		   l_u16Year--;
    }

    l_u32NumberID = l_u16TotalDays*1440;
	return l_u32NumberID;
}

uint32 FGetLastSaveNum(uint32 pSaveNum, uint8 pPeriod)
{
	uint32 l_u32LastNum;
    uint8 *l_u8Buf;

    uint16 l_u16Year1;
	uint8  l_u8Month1;
	uint8  l_u8Day1;
//	uint16 l_u16TimeNum1;

	uint16 l_u16Year2;
	uint8  l_u8Month2;
	uint8  l_u8Day2;
	uint16 l_u16TimeNum2;
//	uint8  l_u8Buf1[6];
	SystemTime l_sTime;

   if((pSaveNum%1440)!=1)//判断和周期的大小
   {
        l_u32LastNum= pSaveNum-1;
   }
   else if((pSaveNum%1440)==1)
   {
        l_u8Buf = FGetDateFromNum(pSaveNum);
		l_u16Year1 = (l_u8Buf[1]<<8) + l_u8Buf[0];
		l_u8Month1 = l_u8Buf[2];
		l_u8Day1 = l_u8Buf[3];
//		l_u16TimeNum1 = (l_u8Buf[5]<<8) +  l_u8Buf[4];
	    switch(l_u8Month1)
		{
		   case 1:
		         if(l_u8Day1==1)
				 {
					  l_u16Year2 = l_u16Year1 - 1;
					  l_u8Month2 = 12;
					  l_u8Day2 = 31;
					  l_u16TimeNum2 = 1440/pPeriod;
				 }
				 else
				{
					   l_u16Year2 = l_u16Year1;
					   l_u8Month2 = l_u8Month1;
					   l_u8Day2 = l_u8Day1 - 1;
					   l_u16TimeNum2 = 1440/pPeriod; 
				 } 
				 break;
		   case 2:
		   case 4:
		   case 6:
		   case 8:
		   case 9:
		   case 11: 
			     if(l_u8Day1==1)
				 {
					  l_u16Year2 = l_u16Year1;
					  l_u8Month2 = l_u8Month1 - 1;
					  l_u8Day2 = 31;
					  l_u16TimeNum2 = 1440/pPeriod;
				 }
				else
				{
					   l_u16Year2 = l_u16Year1;
					   l_u8Month2 = l_u8Month1;
					   l_u8Day2 = l_u8Day1 - 1;
					   l_u16TimeNum2 = 1440/pPeriod; 
				 }
				 break;
		   case 5:
		   case 7:
		   case 10:
		   case 12:
		         if(l_u8Day1==1)
				 {
					  l_u16Year2 = l_u16Year1;
					  l_u8Month2 = l_u8Month1 - 1;
					  l_u8Day2 = 30;
					  l_u16TimeNum2 = 1440/pPeriod;
				 }
				 else
				{
					   l_u16Year2 = l_u16Year1;
					   l_u8Month2 = l_u8Month1;
					   l_u8Day2 = l_u8Day1 - 1;
					   l_u16TimeNum2 = 1440/pPeriod; 
				 }
				break;	
		   case  3:
				 if((l_u8Day1==1)&&(FIsRunYear(l_u16Year1) == 0))
				{
					  l_u16Year2 = l_u16Year1;
					  l_u8Month2 = l_u8Month1 - 1;
					  l_u8Day2 = 28;
					  l_u16TimeNum2 = 1440/pPeriod;
				}
				else if((l_u8Day1==1)&&(FIsRunYear(l_u16Year1) == 1))
				{
					  l_u16Year2 = l_u16Year1;
					  l_u8Month2 = l_u8Month1 - 1;
					  l_u8Day2 = 29;
					  l_u16TimeNum2 = 1440/pPeriod;
				}
			    else
				{
					   l_u16Year2 = l_u16Year1;
					   l_u8Month2 = l_u8Month1;
					   l_u8Day2 = l_u8Day1 - 1;
					   l_u16TimeNum2 = 1440/pPeriod; 
				 }
				break;   
		}
		l_sTime.u16Year = l_u16Year2;
		l_sTime.u8Month = l_u8Month2;
		l_sTime.u8Day = l_u8Day2;
		l_u32LastNum = FGetSaveNumFromDate(&l_sTime, l_u16TimeNum2);
   }
   return l_u32LastNum;
}

uint8* FGetDateFromNum(uint32 pSaveNum)
{
	uint16  l_u16Year;
    uint8   l_u8Month;
	uint8   l_u8Day;
	uint16  l_u16TimeNum;
	uint16  l_u16PassDays;
	uint16  l_u16Year1;
//	uint8   l_u8Month1;
//	uint8   l_u8Day1;
	uint16  l_u16TimeNum1;
	uint16  l_u16Year2;
//	uint8   l_u8Buf[6];
	static uint8   l_u8Buf1[6];
	uint32  l_u32Num1,l_u32Num2;
//	SystemTime	l_sDevCurTime;

//	GET8563(&l_sDevCurTime);
	l_u16Year1    = l_sDevCurTime.u16Year;
//	l_u8Month1   = l_sDevCurTime.u8Month;
//	l_u8Day1     = l_sDevCurTime.u8Day;
	l_u16TimeNum1 = g_u16SaveTimeNum;

//	l_u8Buf[0]=l_u16Year1;
//	l_u8Buf[1]=l_u16Year1>>8;
//	l_u8Buf[2]=l_u8Month1;
//	l_u8Buf[3]=l_u8Day1;
//	l_u8Buf[4]=l_u16TimeNum1;
//	l_u8Buf[5]=l_u16TimeNum1>>8;
	//计算目前日期在一个轮回中的序列号	
	l_u32Num1=FGetSaveNumFromDate(&l_sDevCurTime, l_u16TimeNum1); 
	l_u32Num2=pSaveNum;
    if((l_u16Year1-2000)%3==0)
	{
	   l_u16Year2 = l_u16Year1;
	}
	else if((l_u16Year1-2000)%3==1)
	{
	   l_u16Year2 = l_u16Year1-1;
	}
	else if((l_u16Year1-2000)%3==2)
	{
	   l_u16Year2 = l_u16Year1-2;
	}
	if(l_u32Num2>l_u32Num1)
	{
	   l_u16Year=l_u16Year2-3;
	}
	else if(l_u32Num2<=l_u32Num1)
	{
	   l_u16Year=l_u16Year2;
	}

	l_u8Month = 1;
	l_u8Day = 1;
	l_u16TimeNum = pSaveNum%1440;
	l_u16PassDays = pSaveNum/1440;
	while(l_u16PassDays/FDaysOneYear(l_u16Year))
	{
	  l_u16PassDays-=FDaysOneYear(l_u16Year);
	  l_u16Year++;
	}

	while(l_u16PassDays/FDaysOneMon(l_u16Year,l_u8Month))
	{
		l_u16PassDays-=FDaysOneMon(l_u16Year,l_u8Month);
		l_u8Month++;
		if(l_u8Month>12)
		l_u8Month=l_u8Month%12;
	}
	l_u8Day=l_u8Day+l_u16PassDays;
	if(l_u8Day==FDaysOneMon(l_u16Year,l_u8Month))
	{
	  l_u8Day=FDaysOneMon(l_u16Year,l_u8Month);
	}
	else if(l_u8Day>FDaysOneMon(l_u16Year,l_u8Month)) 
	{
	  l_u8Day-=FDaysOneMon(l_u16Year,l_u8Month);
      l_u8Month++; 
	 // if(l_u8Month>12);
		if(l_u8Month>12) //zhangtonghan 20170608
			l_u8Month=l_u8Month%12;
	}
	
    l_u8Buf1[0] = l_u16Year;
	l_u8Buf1[1] = l_u16Year>>8;
	l_u8Buf1[2] = l_u8Month;
	l_u8Buf1[3] = l_u8Day;
	l_u8Buf1[4] = l_u16TimeNum;
	l_u8Buf1[5] = l_u16TimeNum>>8;
	return &l_u8Buf1[0]; 
}

void  FSave01ToTDC(uint8 pCMD)
{
//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
//			l_u8Buf[0] = (g_u32JDSDSaveInd>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveInd>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveInd>>8);
//			l_u8Buf[3] = g_u32JDSDSaveInd;
			WriteC256(JD01CURNUM, (uint8 * )&g_u32JDSDSaveInd, 4);
//			ReadC256(JD01CURNUM, l_u8Buf, 4);
		break;
		case 2:
//			l_u8Buf[0] = (g_u32JDSDSaveIndSuc>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveIndSuc>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveIndSuc>>8);
//			l_u8Buf[3] = g_u32JDSDSaveIndSuc;
			WriteC256(JD01SUCNUM, (uint8 * )&g_u32JDSDSaveIndSuc, 4);
		break;
		case 3:
//			l_u8Buf[0] = g_u8IsFirstSDSave;
			WriteC256(JD01FIRSTFLAG, (uint8 * )&g_u8IsFirstSDSave, 1);
//			ReadC256(JD01FIRSTFLAG, l_u8Buf, 1);
		break;
		default:
		break;
	}
}

void   FRead01FromTDC(uint8 pCMD)
{
//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
			ReadC256(JD01CURNUM, (uint8 *)&g_u32JDSDSaveInd, 4);
		break;
		case 2:
			ReadC256(JD01SUCNUM, (uint8 *)&g_u32JDSDSaveIndSuc, 4);
		break;
		case 3:
			ReadC256(JD01FIRSTFLAG, (uint8 *)&g_u8IsFirstSDSave, 1);
		break;
		default:
		break;
	}
}

void   FSend01Info(uint32 pSaveID, uint32 pSendID)
{
	uint32 l_u32BaseAddr;
	uint32 l_u32NeedSendSDNum;
	uint8  l_pu8Buf[6];
	uint8*  l_pu8Buf1;
	uint32 l_u32SendNum;
	uint32 l_u32NumReadFromSD;
	uint8  l_u8Ret;
	uint8  l_pu8SendBuf[512];
	uint8  l_u8DataCheckFlag;
	uint8  l_u8SendErrFlag = 0;
	uint16 l_u16DataLen;
	uint8  l_u8Flag = 0;
	uint16 l_u16Rec01Cnt = 0;
	if(g_u8IsFirstSDSave != 1)
	{
		return;
	}
	l_u32BaseAddr = g_u32JDInfo_Addr_Begin;
	if(pSaveID > pSendID)
	{
		if(FGetDifference(pSaveID, pSendID) > JDSYSPARAM.m_u32DiffSend)
		{
			pSendID = FJudgeDifference(pSaveID, JDSYSPARAM.m_u32DiffSend);
			g_u32JDSDSaveIndSuc = pSendID;
			FSave01ToTDC(2);
		}
		l_u32NeedSendSDNum = FGetNextSDNum(pSendID, g_u8PeriodTOSave);
		l_pu8Buf1 = FGetDateFromNum(l_u32NeedSendSDNum);
		l_pu8Buf[0] = *l_pu8Buf1++;
		l_pu8Buf[1] = *l_pu8Buf1++;
		l_pu8Buf[2] = *l_pu8Buf1++;
		l_pu8Buf[3] = *l_pu8Buf1++;
		l_pu8Buf[4] = *l_pu8Buf1++;
		l_pu8Buf[5] = *l_pu8Buf1;
		l_u32SendNum = l_pu8Buf[4] + (l_pu8Buf[5]<<8);
		l_u8Ret = SD_ReadBlock(&sd_info, l_u32BaseAddr+l_u32NeedSendSDNum, l_pu8SendBuf);
		if(l_u8Ret)
		{
		}
		else
		{
		}
		l_u16DataLen = (l_pu8SendBuf[510]<<8) + l_pu8SendBuf[511];
		l_u32NumReadFromSD = l_pu8SendBuf[39] + (l_pu8SendBuf[40]<<8);
		if(l_u32NumReadFromSD != l_u32SendNum)
		{
			SD_ReadBlock(&sd_info, l_u32BaseAddr+l_u32NeedSendSDNum, l_pu8SendBuf);
			l_u16DataLen = (l_pu8SendBuf[510]<<8) + l_pu8SendBuf[511];
			l_u32NumReadFromSD = l_pu8SendBuf[39] + (l_pu8SendBuf[40]<<8);
		}
		l_u8DataCheckFlag = FCheckSendData(l_pu8SendBuf, 1);
		if((l_u32NumReadFromSD != l_u32SendNum)||(l_u16DataLen==0)||(l_u16DataLen>=1000))
		{
			l_u8DataCheckFlag = 1;
		}
		if( l_u8DataCheckFlag == 1 )   // 数据异常 
		{
			BeepON();
			OSTimeDly(10);
			BeepOFF();	
			OSTimeDly(10);
			SD_ReadBlock(&sd_info, l_u32BaseAddr+l_u32NeedSendSDNum, l_pu8SendBuf);
			if( l_u8SendErrFlag == 0 )
			{
			    l_u8SendErrFlag = 100;
			}
			SD_ReadBlock(&sd_info, l_u32BaseAddr+l_u32NeedSendSDNum+1, l_pu8SendBuf);
			// 数据异常			
		    l_u16DataLen = FGetSendDataForErr(1, l_pu8SendBuf, l_pu8Buf, l_u8SendErrFlag);
			//发送数据包 
			l_u8Flag = FSendData(l_pu8SendBuf, l_u16DataLen);
			
		}
		else   // 数据正常
		{
			//发送数据包
			l_u8Flag = FSendData(l_pu8SendBuf, l_u16DataLen);
		}
		if(l_u8Flag!= 0)
		{
			while(g_u8JD0AOrderRev == 0 && l_u16Rec01Cnt < 180)
			{
				OSTimeDly(10);
				l_u16Rec01Cnt ++;
			}
			if(g_u8JD0AOrderRev == 1)
			{
				g_u8JD0AOrderRev = 0;
				if(l_u32SendNum == g_u16RecOrd0ANum )
				{
				  	g_u16RecOrd0ANum = 0;
					// 存储01包成功发送的序号
					g_u32JDSDSaveIndSuc = l_u32NeedSendSDNum;
					FSave01ToTDC(2);
				}
				return;
			}
			else
			{
				g_u8JD0AOrderRev = 0;
				return;
			}
		}
		else
		{
			return;
		}
		//等待返回确认帧如果成功往铁电写
	}
	else if(pSaveID < pSendID)
	{
		g_u32JDSDSaveIndSuc = g_u32JDSDSaveInd;
		FSave01ToTDC(2);
		return;
	}
	else if(pSaveID == pSendID)
	{
		return;
	}
}

uint32 FGetDifference(uint32 pNum1, uint32 pNum2)
{
	 uint32 l_u32NeedSendNum = 0;//待发的数据包个数
	 uint32 l_u32LastNum;    //上一序列号
	 if(pNum1==pNum2)
	 {
	    l_u32NeedSendNum = 0;
	 }
	 else 
	 {
	   l_u32LastNum = FGetLastSaveNum(pNum1, g_u8PeriodTOSave);
	   l_u32NeedSendNum = l_u32NeedSendNum + 1;
	   while(l_u32LastNum!= pNum2)
	   {
	       l_u32NeedSendNum = l_u32NeedSendNum + 1;
	 	   l_u32LastNum = FGetLastSaveNum(l_u32LastNum, g_u8PeriodTOSave);
		   if(l_u32NeedSendNum > 3003)
		   {
		       break;
		   }
	   }
	 }
	 return l_u32NeedSendNum; 
}

uint32 FJudgeDifference(uint32 pNum, uint32 pThr)
{
	 uint32 l_u32tempi;
	 uint32 l_u32ResendNum = pNum;

	 for(l_u32tempi=0; l_u32tempi<pThr; l_u32tempi++)
	 {
	    l_u32ResendNum = FGetLastSaveNum(l_u32ResendNum, g_u8PeriodTOSave);
	 }
	 return l_u32ResendNum; 
}

uint32 FGetNextSDNum(uint32 pNum, uint8 pPeriod)
{
	uint32 l_u32NextNum;
    uint8  *l_pu8Buf;

    uint16 l_u16Year1;
	uint8  l_u8Month1;
	uint8  l_u8Day1;
//	uint16 l_u16Timenum1;

	uint16 l_u16Year2;
	uint8  l_u8Month2;
	uint8  l_u8Day2;
	uint16 l_u16Timenum2;
	SystemTime l_sTime;

   if((pNum%1440)<(1440/pPeriod))//判断和周期的大小
   {
        l_u32NextNum= pNum+1;
   }
   else if((pNum%1440)==(1440/pPeriod))
   {
        l_pu8Buf = FGetDateFromNum(pNum);
		l_u16Year1 = (l_pu8Buf[1]<<8) + l_pu8Buf[0];
		l_u8Month1 = l_pu8Buf[2];
		l_u8Day1 = l_pu8Buf[3];
//		l_u16Timenum1 = (l_pu8Buf[5]<<8) +  l_pu8Buf[4];
		switch(l_u8Month1)
		{
		   case 1:
		   case 3:
		   case 5:
		   case 7:
		   case 8:
		   case 10:
			     if(l_u8Day1==31)
				 {
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1  + 1;
				   l_u8Day2 = 1;
				   l_u16Timenum2 = 1;
				 }
				 else
				 {
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1;
				   l_u8Day2 = l_u8Day1 + 1;
				   l_u16Timenum2 = 1; 
				 }
				 break;
		   case 12: 
			     if(l_u8Day1==31)
				 {
				   l_u16Year2 = l_u16Year1 + 1;
				   l_u8Month2 = 1;
				   l_u8Day2 = 1;
				   l_u16Timenum2 = 1;
				  }
				 else
				 {
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1;
				   l_u8Day2 = l_u8Day1 + 1;
				   l_u16Timenum2 = 1; 
				 }
				 break;
		   case 4:
		   case 6:
		   case 9:
		   case 11:
		        if(l_u8Day1==30)
				{
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1  + 1;
				   l_u8Day2 = 1;
				   l_u16Timenum2 = 1;
				}
				else
				{
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1;
				   l_u8Day2 = l_u8Day1 + 1;
				   l_u16Timenum2 = 1; 
			    }
				break;	
		   case  2:
				 if((l_u8Day1==28)&&(FIsRunYear(l_u16Year1) == 0))
				{
				  l_u16Year2 = l_u16Year1;
				  l_u8Month2 = l_u8Month1  + 1;
				  l_u8Day2 = 1;
				  l_u16Timenum2 = 1;
				}
				else if((l_u8Day1==29)&&(FIsRunYear(l_u16Year1) == 1))
				{
				  l_u16Year2 = l_u16Year1;
				  l_u8Month2 = l_u8Month1  + 1;
				  l_u8Day2 = 1;
				  l_u16Timenum2 = 1;
				}
				else
				{
				   l_u16Year2 = l_u16Year1;
				   l_u8Month2 = l_u8Month1;
				   l_u8Day2 = l_u8Day1 + 1;
				   l_u16Timenum2 = 1; 
				 } 
				break;   
		}
		l_sTime.u16Year = l_u16Year2;
		l_sTime.u8Month = l_u8Month2;
		l_sTime.u8Day   = l_u8Day2;
		l_u32NextNum = FGetSaveNumFromDate(&l_sTime, l_u16Timenum2);
   }
   return l_u32NextNum;
}
uint8  FCheckSendData(uint8* pBuf, uint8 pCMD)
{
	uint8 l_u8tempi;
	uint8 l_u8Result=0;

    // 命令字检查
	if( pCMD != pBuf[0] )
	{
		l_u8Result = 1;
	}	
	// 检查设备身份识别码
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
	    if( pBuf[1+l_u8tempi] != JDSYSPARAM.m_au8DeviceID[l_u8tempi] )
		{
		    l_u8Result = 1;
		}	
	}
	// 检查站点编号
	for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
	{
	    if(pBuf[17+l_u8tempi] != JDSYSPARAM.m_au8StationID[l_u8tempi])
		{
		    l_u8Result = 1;
		}		
	}
		
	return l_u8Result;
}

uint16 FGetSendDataForErr(uint8 pCMD, uint8* pDataBuf, uint8* pTimeBuf, uint8 pErrFlag)
{
	uint8   l_u8tempi;
//	uint8   fenduan_num;
	uint8   l_u8Month;
	uint8   l_u8Day; 
	uint16  l_u16Year;
	uint16  l_u16TimeNum;
	uint16  l_u16Len;

	l_u16Year    = pTimeBuf[0] + (pTimeBuf[1]<<8);
	l_u8Month   = pTimeBuf[2];
	l_u8Day     = pTimeBuf[3];
	l_u16TimeNum = pTimeBuf[4] + (pTimeBuf[5]<<8);


	if( pCMD == 0x01 )
	{
	    l_u16Len = 0;
		switch(JDSYSPARAM.m_u8OrderMod)
		{
			case 0x01:
				//数据包类型
				pDataBuf[l_u16Len++] = 0x01;
				//设备身份识别码
				for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
				}
				//站点编号
				for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
				}
				//设备硬件错误代码
				pDataBuf[l_u16Len++] = pErrFlag;
				//调查内容
				pDataBuf[l_u16Len++] = 0x01;
				//年
				pDataBuf[l_u16Len++] = l_u16Year;
				pDataBuf[l_u16Len++] = (l_u16Year>>8);
				//月
				pDataBuf[l_u16Len++] = l_u8Month;
				//日
				pDataBuf[l_u16Len++] = l_u8Day;
				//交通数据处理周期
				pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
				//时间序号 低位在前
				pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
				pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
	
				//车道数
				pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
	
				for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
				{	
					// 将车道号转换成国标车道号
			 	  	pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi);  
					//跟车百分比
					pDataBuf[l_u16Len++] = 0;
					//平均车头间距
					pDataBuf[l_u16Len++] = 100;
					pDataBuf[l_u16Len++] = 0;
					//时间占有率
					pDataBuf[l_u16Len++] = 0;
					
					//中小客交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;	
					//中小客平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					//小货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;			
					//小货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					//大客交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//大客平均地点车速
					pDataBuf[l_u16Len++] = 0;
								
					//中货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//中货平均地点车速
					pDataBuf[l_u16Len++] = 0;	
				
					//大货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//大货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					//特大货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//特大货平均地点车速
					pDataBuf[l_u16Len++] = 0;
				
					//集装箱交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//集装箱平均地点车速
					pDataBuf[l_u16Len++] = 0;
					
					//拖拉机交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//拖拉机平均地点车速
					pDataBuf[l_u16Len++] = 0;
		
					//摩托车交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//摩托车平均地点车速
					pDataBuf[l_u16Len++] = 0;
				}
				break;
	
			case 0x02:
				pDataBuf[l_u16Len++] = 0x01;
				//设备身份识别码
				for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
				}
				//站点编号
				for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
				}
				//设备硬件错误代码
				pDataBuf[l_u16Len++] = pErrFlag;
				//调查内容
				pDataBuf[l_u16Len++] = 0x02;
				//年
				pDataBuf[l_u16Len++] = l_u16Year;
				pDataBuf[l_u16Len++] = (l_u16Year>>8);
				//月
				pDataBuf[l_u16Len++] = l_u8Month;
				//日
				pDataBuf[l_u16Len++] = l_u8Day;
				//交通数据处理周期
			    pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
				//时间序号 低位在前
				pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
				pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
				//车道数
				pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
			
				for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
				{
					// 将车道号转换成国标车道号
			 	  	pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi); 
					//跟车百分比
					pDataBuf[l_u16Len++] = 0;
					//平均车头间距
					pDataBuf[l_u16Len++] = 100;
					pDataBuf[l_u16Len++] = 0;
					//时间占有率
					pDataBuf[l_u16Len++] = 0;
	
					//中小客交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;					
					//中小客平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//小货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//小货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//大客交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//大客平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
								
					//中货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//中货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;	
				
					//大货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//大货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//特大货交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//特大货平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
				
					//集装箱交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//集装箱平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					
					//拖拉机交通量
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//拖拉机平均地点车速
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
		
					//摩托车交通量
					pDataBuf[l_u16Len++] =0;
					pDataBuf[l_u16Len++] =0;							
					//摩托车平均地点车速
					pDataBuf[l_u16Len++] =0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
				}
	
				break;
	
				default: 
					break;
		}
	
	}
	else
	{
		l_u16Len = 0; 
		// 命令字
		pDataBuf[l_u16Len++] = pCMD;
		//设备身份识别码
		for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
		{
		    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
		}
		//站点编号
		for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
		{
		    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
		}
		//设备硬件错误代码
		pDataBuf[l_u16Len++] = pErrFlag;
		//年
		pDataBuf[l_u16Len++] = l_u16Year;
		pDataBuf[l_u16Len++] = (l_u16Year>>8);
		//月
		pDataBuf[l_u16Len++] = l_u8Month;
		//日
		pDataBuf[l_u16Len++] = l_u8Day;
		// 数据处理周期
		pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
		//时间序号 低位在前
		pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
		pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
		//车道数
		pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
	
		for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
		{				 
			  // 将车道号转换成国标车道号
			  pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi); 
	          // 轴重量数据分段数
			  pDataBuf[l_u16Len++] = 0;//(fenduan_num&0xFF);
			  pDataBuf[l_u16Len++] = 0;//(fenduan_num>>8);
		 }
	}

	return l_u16Len;	    // 数据包长度
}
uint8  FCheckDeviceID(uint8* pBuf)
{
	uint8 l_u8tempi;
	uint8 l_u8Return = 1;
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
		if(pBuf[l_u8tempi] != JDSYSPARAM.m_au8DeviceID[l_u8tempi])
		{
			l_u8Return = 0;
			return l_u8Return;
		}
	}
	return l_u8Return;
}
void  FSave39ToSD(Vehicle_Struct *pdata)
{
	char  l_cSendTemp=0;
	uint8 i;
	uint8 j;
	uint8 l_u8OverFlag;
	uint8 l_u8Len = 0;
	uint8 l_u8Buf[50];
	uint32 l_u32AxleAdr;
	uint32 l_u32TotalWeight;
	uint16 l_u16TotalDis;
//	SystemTime	l_sDevCurTime;
//	GET8563(&l_sDevCurTime);

	i = 0;
	l_u8OverFlag = 0;

    //记录车辆序号
	g_u32JD39SaveSuccInd++;
	if( g_u32JD39SaveSuccInd > (g_u32JD61BaseAdr - 1000) )
	{
		g_u32JD39SaveSuccInd = 0;
	}
	l_u8Buf[i++] = g_u32JD39SaveSuccInd&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>8)&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>16)&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>24)&0xff;
	// 年、月、日
	l_u8Buf[i++] = g_DevCurTime.u16Year;    //
	l_u8Buf[i++] = g_DevCurTime.u16Year>>8;
	l_u8Buf[i++] = g_DevCurTime.u8Month;     
	l_u8Buf[i++] = g_DevCurTime.u8Day;	   
	// 记录车道数
	l_u8Buf[i++] = PROMODPARAM.m_u8LaneNum;   	     
	//记录车道号
//	if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//		l_u8Buf[i++] = FChangLaneToBG(pdata->u8psLane);   // 国标车道号
//	else
		l_u8Buf[i++] = FChangLaneToBG(pdata->u8psLane/2);   // 国标车道号
	// 时、分、秒
	l_u8Buf[i++] = g_DevCurTime.u8Hour;
	l_u8Buf[i++] = g_DevCurTime.u8Minute;
	l_u8Buf[i++] = g_DevCurTime.u8Second;
	//记录车型
	l_u8Buf[i++] = pdata->m_u8VehType;   // 
	//记录轴型
	l_u8Buf[i++] = pdata->u8AixCnt;   // 
	l_u32TotalWeight = 0;
	for (j=0; j < pdata->u8AixCnt; j++)
	{
		l_u32TotalWeight +=	(pdata->au16AxleWeight[j]*10);
		if( pdata->au16AxleWeight[j]*10 > JDSYSPARAM.m_u32MaxForAxle )
		{
			l_u8OverFlag = 1;
		}
	}
	//记录车辆总重
	l_u8Buf[i++] = l_u32TotalWeight&0xFF;       // 
	l_u8Buf[i++] = (l_u32TotalWeight>>8)&0xFF;        // 
	l_u8Buf[i++] = (l_u32TotalWeight>>16)&0xFF;        // 
	l_u8Buf[i++] = (l_u32TotalWeight>>24)&0xFF;        // 
	//记录速度
   	l_u8Buf[i++] = (pdata->u16speed/10)&0xFF;
	l_u8Buf[i++] = ((pdata->u16speed/10)>>8)&0xFF;

	// 轴重
	for (j=0; j < pdata->u8AixCnt; j++)
	{
		l_u8Buf[i++] = pdata->au16AxleWeight[j]*10;
		l_u8Buf[i++] = (pdata->au16AxleWeight[j]*10)>>8;
	}
	// 轴间距
	l_u16TotalDis = 0;
	for (j=0; j < pdata->u8AixCnt -1; j++)
	{
		l_u8Buf[i++] = pdata->au16AxleDistance[j]*10;
	    l_u8Buf[i++] = (pdata->au16AxleDistance[j]*10)>>8;
		l_u16TotalDis += pdata->au16AxleDistance[j]*10;
	}
	//记录总轴距
   	l_u8Buf[i++] =  l_u16TotalDis&0xFF;
	l_u8Buf[i++] =  (l_u16TotalDis>>8)&0xFF;
	// 记录数据长度
	l_u8Len = i;

	//  不足50个字节补0
	if( i < 50 )
	{
		for(j=i;j<50;j++)
		{
		    l_u8Buf[i++] = 0;
		}
	}

	// 温度
//	if((curRst.ln_wendu <= 333) && (curRst.ln_wendu >= 235))
//	{
//		l_cSendTemp = curRst.ln_wendu - 273;
//	}
//	else
//	{
	l_cSendTemp = 102;
//	}
	l_u8Buf[47] = l_cSendTemp;    
	l_u8Buf[48] = l_u8OverFlag;   // 超限标志
	l_u8Buf[49] = l_u8Len; 
	
	if( g_u8Is39FirstSave != 1 )     // 未写过单车数据
	{
	    // 清零单车存储标志
		g_u8Is39FirstSave = 1;
		FSave39ToTDC(3);
		// 存储当前单车发送序号
		FSave39ToTDC(1);
		// 存储成功发送的的单车序号
		if( g_u32JD39SaveSuccInd == 0 )
		{
		    g_u32JD39SendSuccInd = 0;
		    g_u32JD39SaveSuccInd = 0;
		}
		else
		{
		    g_u32JD39SendSuccInd = g_u32JD39SaveSuccInd - 1;
		}
		FSave39ToTDC(2);
	}
	// 将单车数据存入SD卡
	l_u32AxleAdr = g_u32JD39BaseAdr + g_u32JD39SaveSuccInd;
	SD_WriteBlock( &sd_info, l_u32AxleAdr, l_u8Buf);
	// 将单车序号存储在铁电中
	FSave39ToTDC(1);
}
void  FSave39ToTDC(uint8 pCMD)
{
	//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
//			l_u8Buf[0] = (g_u32JDSDSaveInd>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveInd>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveInd>>8);
//			l_u8Buf[3] = g_u32JDSDSaveInd;
			WriteC256(JD39CURNUM, (uint8 * )&g_u32JD39SaveSuccInd, 4);
//			ReadC256(JD01CURNUM, l_u8Buf, 4);
		break;
		case 2:
//			l_u8Buf[0] = (g_u32JDSDSaveIndSuc>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveIndSuc>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveIndSuc>>8);
//			l_u8Buf[3] = g_u32JDSDSaveIndSuc;
			WriteC256(JD39SUCNUM, (uint8 * )&g_u32JD39SendSuccInd, 4);
		break;
		case 3:
//			l_u8Buf[0] = g_u8IsFirstSDSave;
			WriteC256(JD39FIRSTFLAG, (uint8 * )&g_u8Is39FirstSave, 1);
//			ReadC256(JD01FIRSTFLAG, l_u8Buf, 1);
		break;
		default:
		break;
	}
}
void  FRead39FromTDC(uint8 pCMD)
{
	switch(pCMD)
	{
		case 1:
			ReadC256(JD39CURNUM, (uint8 *)&g_u32JD39SaveSuccInd, 4);
		break;
		case 2:
			ReadC256(JD39SUCNUM, (uint8 *)&g_u32JD39SendSuccInd, 4);
		break;
		case 3:
			ReadC256(JD39FIRSTFLAG, (uint8 *)&g_u8Is39FirstSave, 1);
		break;
		default:
		break;
	}
}
void  FSend39Info(uint32 pSaveID, uint32 pSendID)
{
	uint8   i;
	uint8   l_u8err;
	uint8   l_u8Len;
	uint8   l_u8ret;
	uint8   l_u8Send39ErrCnt = 0;
	uint8   l_u8FeedBack;
	uint8   l_u8OverFlag;
	uint8   l_u8ReadSDbuf[512];
	uint8   l_u8Buf[512];
	uint8   l_u8SendErrData[512];
	uint8   l_u8DataErrFlag = 0;
	uint16  l_u16AxleSendLen;
	uint16  l_u16AxleSendErrLen; 
	uint32  l_u32CarNumSend;
	uint32  l_u32Raddress;
	uint32  l_u32NeedSendNum;
	uint16  l_u16Rec39Cnt=0;
	
    if( g_u8Is39FirstSave != 1 )
	{
	   return;
	}

	l_u8Len = 0;
	// 命令字
	l_u8Buf[l_u8Len++] = 0x39; 
	// 站点编号
	for(i=0;i<15;i++)
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];	
	}
	// 设备身份识别码
	for(i=0;i<16;i++)					 
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	// 设备硬件错误代码
	l_u8Buf[l_u8Len++] = 0x00;

	if( g_u32JD39SaveSuccInd > g_u32JD39SendSuccInd )		
	{	
		if((g_u32JD39SaveSuccInd - g_u32JD39SendSuccInd) >JDSYSPARAM.m_u16VehNumForSend)
		{
		   g_u32JD39SendSuccInd = g_u32JD39SaveSuccInd - JDSYSPARAM.m_u16VehNumForSend;
		   FSave39ToTDC(2);
		}

		// 解析下一辆车的SD卡扇区首地址
		l_u32NeedSendNum = g_u32JD39SendSuccInd + 1;
		l_u32Raddress = g_u32JD39BaseAdr + l_u32NeedSendNum;
		// 读取要发送的单车数据
		l_u8ret = SD_ReadBlock(&sd_info, l_u32Raddress,l_u8ReadSDbuf);
		if( l_u8ret !=0 )
		{
			l_u8err = 101;
//			SD_Err_Reset_cnt++;
		}
//		else
//		{
//		    SD_Err_Reset_cnt = 0;
//		}

		for(i=0;i<50;i++)
		{
		   l_u8Buf[l_u8Len+i] =	l_u8ReadSDbuf[i];
		}
		// 读取温度值，在硬件错误代码中体现 
		l_u8Buf[32]   = l_u8ReadSDbuf[47];
		// 读取数据包长度
		l_u16AxleSendLen = l_u8ReadSDbuf[49] + l_u8Len;
		// 读取该车是否超限标志
		l_u8OverFlag    = l_u8ReadSDbuf[48]; 
		// 记录要发送车辆数据包的序号
		l_u32CarNumSend = l_u8Buf[33] + (l_u8Buf[34]<<8) + (l_u8Buf[35]<<16) + (l_u8Buf[36]<<24);	   
		
		if( (l_u32CarNumSend != l_u32NeedSendNum)||(l_u16AxleSendLen==0)||(l_u16AxleSendLen>512) )
		{
		    l_u8DataErrFlag = 1;     // 数据异常
			if( l_u8err == 0 )
			{
			   l_u8err = 100;
			   l_u8Send39ErrCnt++;
			}
		}
		else
		{
		    l_u8Send39ErrCnt = 0;
		}
		switch(JDSYSPARAM.m_u8SendVehType)//发车数据类型判断 
		{
		  case 1:
				if( l_u8DataErrFlag == 1 )
				{
		//		    if( err == 101 )
					{
						BeepON();
						OSTimeDly(10);
						BeepOFF();	
						OSTimeDly(10);
					}
					
					if( l_u8Send39ErrCnt == 1 )
					{
//						l_u16AxleSendErrLen = Get_Err_data39(send_data39_err,l_u32NeedSendNum,err);
					    // 发送39数据包
//				        Send_Feedback = SendData(send_data39_err,l_u16AxleSendErrLen);
						l_u16AxleSendErrLen = FGet39OrderForErr(l_u8SendErrData,l_u32NeedSendNum,l_u8err);
						//发送数据包 
						l_u8FeedBack = FSendData(l_u8SendErrData, l_u16AxleSendErrLen);
					}
					else  
					{
					    // 存储成功发送计数值
			   			g_u32JD39SendSuccInd = l_u32NeedSendNum;
						FSave39ToTDC(2);
						return ; 
					}
				}
				else
				{
					// 发送39数据包
				    l_u8FeedBack = FSendData(l_u8Buf,l_u16AxleSendLen);	
				}
				if( l_u8FeedBack == 1 )      // 发送成功
				{
					while(g_u8JD39OrderRev == 0 && l_u16Rec39Cnt < 180)
					{
						OSTimeDly(10);
						l_u16Rec39Cnt ++;
					}
					if(g_u8JD39OrderRev == 1)
					{
						g_u8JD39OrderRev = 0;
						if(l_u32NeedSendNum == g_u32RecOrd39Num )
						{
						  	g_u32RecOrd39Num = 0;
							g_u839UnRecCnt = 0;
							// 存储01包成功发送的序号
							g_u32JD39SendSuccInd = l_u32NeedSendNum;
							FSave39ToTDC(2);
						}
						return;
					}
					else
					{
						g_u839UnRecCnt ++;
						if(g_u839UnRecCnt > 10)
						{
							g_u32RecOrd39Num = 0;
							g_u839UnRecCnt = 0;
							// 存储01包成功发送的序号
							g_u32JD39SendSuccInd = l_u32NeedSendNum;
							FSave39ToTDC(2);
						}
						g_u8JD39OrderRev = 0;
						return;
					}	
				}
				else
				{
					
				}
				break;
					 
		case 2:
		     if( l_u8OverFlag == 0 )   // 该车不超限
			 {
			    // 存储成功发送计数值
	   			g_u32JD39SendSuccInd = l_u32NeedSendNum;
				FSave39ToTDC(2);
			 }
			 else if( l_u8OverFlag == 1 )
		     {
		   	    if( l_u8DataErrFlag == 1 )
				{
		//		    if( err == 101 )
					{
						BeepON();
						OSTimeDly(10);
						BeepOFF();	
						OSTimeDly(10);
					}
					
					if( l_u8Send39ErrCnt == 1 )
					{
//						l_u16AxleSendErrLen = Get_Err_data39(send_data39_err,l_u32NeedSendNum,err);
//					    // 发送39数据包
//				        Send_Feedback = SendData(send_data39_err,l_u16AxleSendErrLen);
						l_u16AxleSendErrLen = FGet39OrderForErr(l_u8SendErrData,l_u32NeedSendNum,l_u8err);
						//发送数据包 
						l_u8FeedBack = FSendData(l_u8SendErrData, l_u16AxleSendErrLen);
					}
					else  
					{
					    // 存储成功发送计数值
			   			g_u32JD39SendSuccInd = l_u32NeedSendNum;
						FSave39ToTDC(2);
						return ; 
					}
				}
				else
				{
					// 发送39数据包
				    l_u8FeedBack = FSendData(l_u8Buf,l_u16AxleSendLen);	
				}
				if( l_u8FeedBack == 1 )      // 发送成功
				{
					while(g_u8JD39OrderRev == 0 && l_u16Rec39Cnt < 60)
					{
						OSTimeDly(10);
						l_u16Rec39Cnt ++;
					}
					if(g_u8JD39OrderRev == 1)
					{
						g_u8JD39OrderRev = 0;
						if(l_u32NeedSendNum == g_u32RecOrd39Num )
						{
						  	g_u32RecOrd39Num = 0;
							g_u839UnRecCnt = 0;
							// 存储01包成功发送的序号
							g_u32JD39SendSuccInd = l_u32NeedSendNum;
							FSave39ToTDC(2);
						}
						return;
					}
					else
					{
						g_u839UnRecCnt ++;
						if(g_u839UnRecCnt > 10)
						{
							g_u32RecOrd39Num = 0;
							g_u839UnRecCnt = 0;
							// 存储01包成功发送的序号
							g_u32JD39SendSuccInd = l_u32NeedSendNum;
							FSave39ToTDC(2);
						}
						g_u8JD39OrderRev = 0;
						return;
					}	
				}
				else
				{
				}
			   
		    }      
			break; 
		}
    }		
	else if( g_u32JD39SaveSuccInd < g_u32JD39SendSuccInd )
	{
		if( g_u32JD39SaveSuccInd == 0 )
		{
			g_u32JD39SaveSuccInd = 0;
			g_u32JD39SendSuccInd = 0;
		}
		else
		{
		    g_u32JD39SendSuccInd = g_u32JD39SaveSuccInd - 1;
		}
		// 
		FSave39ToTDC(2);
        //
		FSave39ToTDC(1);
	}
	else if( g_u32JD39SaveSuccInd == g_u32JD39SendSuccInd )
	{
	}
}
uint16 FGet39OrderForErr(uint8* pDataBuf,uint32 pSendNum,uint8 pErrFlag)
{
	uint8  i;
	uint8  j;
	uint8  l_u8Len; 

//    SystemTime	l_sDevCurTime;
//	GET8563(&l_sDevCurTime);
	// 得到测试时间

	l_u8Len = 0;
	// 命令字
	pDataBuf[l_u8Len++] = 0x39; 
	// 站点编号
	for(i=0;i<15;i++)
	{
		pDataBuf[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];	
	}
	// 设备身份识别码
	for(i=0;i<16;i++)					 
	{
		pDataBuf[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	// 设备硬件错误代码
	pDataBuf[l_u8Len++] = pErrFlag;

    //记录车辆序号
	pDataBuf[l_u8Len++] = pSendNum&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>8)&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>16)&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>24)&0xff;
	// 年、月、日
	pDataBuf[l_u8Len++] = l_sDevCurTime.u16Year;    //
	pDataBuf[l_u8Len++] = l_sDevCurTime.u16Year>>8;
	pDataBuf[l_u8Len++] = l_sDevCurTime.u8Month;     
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Day >> 4)&0x0F)*10 + (l_sDevCurTime.u8Day&0x0F);	   
	// 记录车道数
	pDataBuf[l_u8Len++] = PROMODPARAM.m_u8LaneNum;   	     
	//记录车道号
	pDataBuf[l_u8Len++] = 11;   // 国标车道号
	// 时、分、秒
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Hour >> 4)&0x0F)*10 + (l_sDevCurTime.u8Hour&0x0F);
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Minute >> 4)&0x0F)*10 + (l_sDevCurTime.u8Minute&0x0F);
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Second >> 4)&0x0F)*10 + (l_sDevCurTime.u8Second&0x0F);
	//记录车型
	pDataBuf[l_u8Len++] = 1;   // 小客车
	//记录轴型
	pDataBuf[l_u8Len++] = 2;   // 
	//记录车辆总重
	pDataBuf[l_u8Len++] = 2000&0xFF;         // 
	pDataBuf[l_u8Len++] = (2000>>8)&0xFF;    // 
	pDataBuf[l_u8Len++] = (2000>>16)&0xFF;   // 
	pDataBuf[l_u8Len++] = (2000>>24)&0xFF;   // 
	//记录速度
   	pDataBuf[l_u8Len++] = 60&0xFF;
	pDataBuf[l_u8Len++] = (60>>8)&0xFF;

	// 轴重
	for (j=0; j < 2; j++)
	{
		pDataBuf[l_u8Len++] = 1000&0xFF;
		pDataBuf[l_u8Len++] = (1000>>8)&0xFF;
	}
	// 轴间距
	for (j=0; j < 1; j++)
	{
		pDataBuf[l_u8Len++] = 2600&0xFF;
	    pDataBuf[l_u8Len++] = (2600>>8)&0xFF;
	}
	//记录总轴距
   	pDataBuf[l_u8Len++] =  2600&0xFF;
	pDataBuf[l_u8Len++] =  (2600>>8)&0xFF;

	return l_u8Len;	    // 返回数据包长度
}
void  FSend6FInfo(uint8 pCmdType, uint32 pSaveID, uint32 pSendID)
{
	uint8   i;
	uint8   *pBuf;
	uint8   l_u8DateTimeNum[6];
	uint8   l_u8SendErr=0;
	uint8   l_u8FeedBack;
	uint8   l_u8DataCheckFlag = 0;
	uint8   l_u8ret;
	uint8   l_u8SendData[512];
	uint16  l_u16SendDataLen = 0;
	uint16  l_u16NeedSendTimeNum;
	uint16  l_u16ReadNumFromSD;
	uint16  l_u16Rec6FCnt=0;

	uint32  l_u32Raddress;
	uint32  l_u32Sd_BaseAddress;
	uint32  l_u32NeedSendSDNum;
	
	
	
	switch(pCmdType)
	{
	    case 0x61:
		          if( g_u8Is61FirstSave != 1 )     // 未存储过数据包
				  {
				      return;
				  }
		          l_u32Sd_BaseAddress = g_u32JD61BaseAdr;
				  break;	
	    case 0x62:
		          if( g_u8Is62FirstSave != 1 )     // 未存储过数据包
				  {
				      return;
				  }
				  l_u32Sd_BaseAddress = g_u32JD62BaseAdr;
				  break;
	    case 0x68:
		          if( g_u8Is68FirstSave != 1 )     // 未存储过数据包
				  {
				      return;
				  }
				  l_u32Sd_BaseAddress = g_u32JD68BaseAdr;
				  break;
	      default:
				  break;				  				  	
	}

    if( pSaveID > pSendID )
	{
		   switch(pCmdType)
			{
			    case 0x61:	
				          if(FGetDifference(pSaveID,pSendID) > JDSYSPARAM.m_u32WeightSendThr)
						  {
							  pSendID = FJudgeDifference(pSaveID, JDSYSPARAM.m_u32WeightSendThr);
							  g_u32JD61SendSuccInd = pSendID;
							  FSave61ToTDC(2);
						  }
						  break;	
			    case 0x62: 
				          if(FGetDifference(pSaveID,pSendID) > JDSYSPARAM.m_u32AxleNumSendThr)
						  {
						     pSendID = FJudgeDifference(pSaveID, JDSYSPARAM.m_u32AxleNumSendThr);
							 g_u32JD62SendSuccInd = pSendID;
							 FSave62ToTDC(2);
						  }
						  break;
			    case 0x68:
				          if(FGetDifference(pSaveID,pSendID) > JDSYSPARAM.m_u32AxleWghSendThr)
						  {
						     pSendID = FJudgeDifference(pSaveID, JDSYSPARAM.m_u32AxleWghSendThr);
							 g_u32JD68SendSuccInd = pSendID;
							 FSave68ToTDC(2);
						   }
						  break;
			      default:
						  break;				  				  	
			}

		// 得到将要发送的SD卡序号, Success_Send_data_NUM 的下一个序号
		l_u32NeedSendSDNum = FGetNextSDNum(pSendID,g_u8PeriodTOSave);

		//  通过存储序号，得到日期及时间序号
		pBuf = FGetDateFromNum(l_u32NeedSendSDNum);
		for(i=0;i<6;i++)
		{
		    l_u8DateTimeNum[i] = *(pBuf+i);
		}
		// 将要发送的时间序号
		l_u16NeedSendTimeNum = l_u8DateTimeNum[4] + (l_u8DateTimeNum[5]<<8);

		switch(pCmdType)
		{
		    case 0x61:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd61Num )
					  {
					  	  g_u16RecOrd61Num = 0xffff;
						  // 存储61包成功发送的序号
						  g_u32JD61SendSuccInd = l_u32NeedSendSDNum;
						  FSave61ToTDC(2);
						  return;
					  }							  
					  break;
		    case 0x62:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd62Num )
					  {
					  	  g_u16RecOrd62Num = 0xffff;
						  // 存储62包成功发送的序号
						  g_u32JD62SendSuccInd = l_u32NeedSendSDNum;
						  FSave62ToTDC(2);
						  return;
					  }							  
					  break;			
		    case 0x68:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd68Num )
					  {
					  	  g_u16RecOrd68Num = 0xffff;
						  // 存储68包成功发送的序号
						  g_u32JD68SendSuccInd = l_u32NeedSendSDNum;
						  FSave68ToTDC(2);
						  return;
					  }							  
					  break;			
			  default:
			          break;
		}
		// 得到SD卡扇区地址
		l_u32Raddress = l_u32Sd_BaseAddress + l_u32NeedSendSDNum; 
		// 读SD卡数据
		l_u8ret = SD_ReadBlock(&sd_info, l_u32Raddress, l_u8SendData);
		if( l_u8ret !=0 )
		{
			l_u8SendErr = 101;
//		    SD_Err_Reset_cnt++;
		}
		else
		{
//		    SD_Err_Reset_cnt = 0;
		}
		// 读取数据包长度
		l_u16SendDataLen = (l_u8SendData[510]<<8) + l_u8SendData[511];
		// 从SD卡读出来的时间序号
		switch(pCmdType)
		{
			case 0x61:
			case 0x62:
			case 0x68:
			          l_u16ReadNumFromSD = l_u8SendData[38] + (l_u8SendData[39]<<8);
					  break;
			  default:
			          break;
		}
		
		// 检查数据是否异常
		l_u8DataCheckFlag = FCheckSendData(l_u8SendData, pCmdType);
//		Err_data_flag = Check_data(send_data,SendData_Type);  // 检查命令字、识别码、站点编号

		// 判断数据是否异常
		if( (l_u16ReadNumFromSD != l_u16NeedSendTimeNum)||(l_u16SendDataLen==0)||(l_u16SendDataLen>=1000) )
		{
			l_u8DataCheckFlag = 1;
		}	

		if( l_u8DataCheckFlag == 1 )   // 数据异常 
		{
			BeepON();
			OSTimeDly(10);
			BeepOFF();	
			OSTimeDly(10);
			if( l_u8SendErr == 0 )
			{
			    l_u8SendErr = 100;
			}
			// 数据异常			
		    l_u16SendDataLen = FGetSendDataForErr( pCmdType, l_u8SendData, l_u8DateTimeNum, l_u8SendErr );
			//发送数据包 
			l_u8FeedBack = FSendData(l_u8SendData,l_u16SendDataLen);
		}
		else   // 数据正常
		{
			//发送数据包
			l_u8FeedBack = FSendData(l_u8SendData,l_u16SendDataLen);		
		}

		if( l_u8FeedBack == 1 )    // 发送完成
		{	// 记录日志信息
//			if( SendData_Type == 0x01 )
//		    {
//			    EVENT_0ARev->OSEventCnt = 0;
//			    OSSemPend(EVENT_0ARev,8000,&err);	//等待0A包返回。2000--5s; 8000--20s; 10000--25s
//			}
//			else
//			{
//				EVENT_35Rev->OSEventCnt = 0;
//			    OSSemPend(EVENT_35Rev,8000,&err);	//等待35包返回。2000--5s; 8000--20s; 10000--25s
//			}

			while(g_u8JD6FOrderRev == 0 && l_u16Rec6FCnt < 400)	//180
			{
				OSTimeDly(10);
				l_u16Rec6FCnt ++;
			}
			if( g_u8JD6FOrderRev == 1 )
			{
				g_u8JD6FOrderRev = 0;
				switch(pCmdType)
				{
				    case 0x61:
					     	  if( l_u16NeedSendTimeNum == g_u16RecOrd61Num )
							  {
							  	  g_u16RecOrd61Num = 0xffff;
								  // 存储61包成功发送的序号
								  g_u32JD61SendSuccInd = l_u32NeedSendSDNum;
								  FSave61ToTDC(2);
							  }							  
							  break;
				    case 0x62:
					     	  if( l_u16NeedSendTimeNum == g_u16RecOrd62Num )
							  {
							  	  g_u16RecOrd62Num = 0xffff;
								  // 存储62包成功发送的序号
								  g_u32JD62SendSuccInd = l_u32NeedSendSDNum;
								  FSave62ToTDC(2);
							  }							  
							  break;			
				    case 0x68:
					     	  if( l_u16NeedSendTimeNum == g_u16RecOrd68Num )
							  {
							  	  g_u16RecOrd68Num = 0xffff;
								  // 存储68包成功发送的序号
								  g_u32JD68SendSuccInd = l_u32NeedSendSDNum;
								  FSave68ToTDC(2);
							  }							  
							  break;			
					  default:
					          break;
				} 
			}
			else 				//等待20s
			{
				g_u8JD6FOrderRev = 0;
			}
		}
		else       // 发送失败
		{
		}    
	}
	else if( pSaveID < pSendID )    // 定时周期变化时
	{
		switch(pCmdType)
		{
		    case 0x61:
			     	  g_u32JD61SendSuccInd = g_u32JD61SaveSuccInd;
					  // 存储61包成功发送的序号
					  FSave61ToTDC(2);						  
					  break;
		    case 0x62:
			     	  g_u32JD62SendSuccInd = g_u32JD62SaveSuccInd;
					  // 存储62包成功发送的序号
					  FSave62ToTDC(2);						  
					  break;			
		    case 0x68:
			     	  g_u32JD68SendSuccInd = g_u32JD68SaveSuccInd;
					  // 存储68包成功发送的序号
					  FSave68ToTDC(2);						  
					  break;			
			  default:
			          break;
		} 
	}
	else if( pSaveID == pSendID )
	{
	}

	return;
}

void  FSave61ToTDC(uint8 pCMD)
{
	//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
//			l_u8Buf[0] = (g_u32JDSDSaveInd>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveInd>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveInd>>8);
//			l_u8Buf[3] = g_u32JDSDSaveInd;
			WriteC256(JD61CURNUM, (uint8 * )&g_u32JD61SaveSuccInd, 4);
//			ReadC256(JD01CURNUM, l_u8Buf, 4);
		break;
		case 2:
//			l_u8Buf[0] = (g_u32JDSDSaveIndSuc>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveIndSuc>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveIndSuc>>8);
//			l_u8Buf[3] = g_u32JDSDSaveIndSuc;
			WriteC256(JD61SUCNUM, (uint8 * )&g_u32JD61SendSuccInd, 4);
		break;
		case 3:
//			l_u8Buf[0] = g_u8IsFirstSDSave;
			WriteC256(JD61FIRSTFLAG, (uint8 * )&g_u8Is61FirstSave, 1);
//			ReadC256(JD01FIRSTFLAG, l_u8Buf, 1);
		break;
		default:
		break;
	}
}
void  FSave62ToTDC(uint8 pCMD)
{
	//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
//			l_u8Buf[0] = (g_u32JDSDSaveInd>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveInd>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveInd>>8);
//			l_u8Buf[3] = g_u32JDSDSaveInd;
			WriteC256(JD62CURNUM, (uint8 * )&g_u32JD62SaveSuccInd, 4);
//			ReadC256(JD01CURNUM, l_u8Buf, 4);
		break;
		case 2:
//			l_u8Buf[0] = (g_u32JDSDSaveIndSuc>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveIndSuc>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveIndSuc>>8);
//			l_u8Buf[3] = g_u32JDSDSaveIndSuc;
			WriteC256(JD62SUCNUM, (uint8 * )&g_u32JD62SendSuccInd, 4);
		break;
		case 3:
//			l_u8Buf[0] = g_u8IsFirstSDSave;
			WriteC256(JD62FIRSTFLAG, (uint8 * )&g_u8Is62FirstSave, 1);
//			ReadC256(JD01FIRSTFLAG, l_u8Buf, 1);
		break;
		default:
		break;
	}
}
void  FSave68ToTDC(uint8 pCMD)
{
	//	uint8 l_u8Buf[4];
	switch(pCMD)
	{
		case 1:
//			l_u8Buf[0] = (g_u32JDSDSaveInd>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveInd>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveInd>>8);
//			l_u8Buf[3] = g_u32JDSDSaveInd;
			WriteC256(JD68CURNUM, (uint8 * )&g_u32JD68SaveSuccInd, 4);
//			ReadC256(JD01CURNUM, l_u8Buf, 4);
		break;
		case 2:
//			l_u8Buf[0] = (g_u32JDSDSaveIndSuc>>24);
//			l_u8Buf[1] = (g_u32JDSDSaveIndSuc>>16);
//			l_u8Buf[2] = (g_u32JDSDSaveIndSuc>>8);
//			l_u8Buf[3] = g_u32JDSDSaveIndSuc;
			WriteC256(JD68SUCNUM, (uint8 * )&g_u32JD68SendSuccInd, 4);
		break;
		case 3:
//			l_u8Buf[0] = g_u8IsFirstSDSave;
			WriteC256(JD68FIRSTFLAG, (uint8 * )&g_u8Is68FirstSave, 1);
//			ReadC256(JD01FIRSTFLAG, l_u8Buf, 1);
		break;
		default:
		break;
	}
}
void   FRead61FromTDC(uint8 pCMD)
{
	switch(pCMD)
	{
		case 1:
			ReadC256(JD61CURNUM, (uint8 *)&g_u32JD61SaveSuccInd, 4);
		break;
		case 2:
			ReadC256(JD61SUCNUM, (uint8 *)&g_u32JD61SendSuccInd, 4);
		break;
		case 3:
			ReadC256(JD61FIRSTFLAG, (uint8 *)&g_u8Is61FirstSave, 1);
		break;
		default:
		break;
	}
}
void   FRead62FromTDC(uint8 pCMD)
{
	switch(pCMD)
	{
		case 1:
			ReadC256(JD62CURNUM, (uint8 *)&g_u32JD62SaveSuccInd, 4);
		break;
		case 2:
			ReadC256(JD62SUCNUM, (uint8 *)&g_u32JD62SendSuccInd, 4);
		break;
		case 3:
			ReadC256(JD62FIRSTFLAG, (uint8 *)&g_u8Is62FirstSave, 1);
		break;
		default:
		break;
	}
}
void   FRead68FromTDC(uint8 pCMD)
{
	switch(pCMD)
	{
		case 1:
			ReadC256(JD68CURNUM, (uint8 *)&g_u32JD68SaveSuccInd, 4);
		break;
		case 2:
			ReadC256(JD68SUCNUM, (uint8 *)&g_u32JD68SendSuccInd, 4);
		break;
		case 3:
			ReadC256(JD68FIRSTFLAG, (uint8 *)&g_u8Is68FirstSave, 1);
		break;
		default:
		break;
	}
}
uint8  FJD61ToSaveSend(uint16 pTimeNum)
{
	uint8   i;
	uint8   j;
	uint8   ln;
	uint8   l_u8ret;
	uint8   l_u8FenDuanNum;
	uint16  l_u16Year;
    uint8   l_u8Month;
    uint8   l_u8Day;
	uint8   l_u8FenDuanBuf[18];
	uint8   l_u861Data[512];
	uint16  l_u8Len;
	uint32  l_u32address;
//	SystemTime	l_sDevCurTime;

//	GET8563(&l_sDevCurTime);


	// 

    // 当24时0点的时候，扇区地址按照前一天的处理周期计算
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// 得到当前日期
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// 得到当前车道数
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// 命令字
	l_u861Data[l_u8Len++] = 0x61;
	//设备身份识别码
	for(i=0;i<16;i++)
	{
	    l_u861Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//站点编号
	for(i=0;i<15;i++)
	{
	    l_u861Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//设备硬件错误代码
	l_u861Data[l_u8Len++] = 0x00;
	
	//年
	l_u861Data[l_u8Len++] = l_u16Year;
	l_u861Data[l_u8Len++] = (l_u16Year>>8);
	//月
	l_u861Data[l_u8Len++] = l_u8Month;
	//日
	l_u861Data[l_u8Len++] = l_u8Day;
	// 数据处理周期
    l_u861Data[l_u8Len++] = g_u8PeriodTOSave;
	//时间序号 低位在前
	l_u861Data[l_u8Len++] = pTimeNum&0xff;
	l_u861Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//车道数
	l_u861Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// 将车道号转换成国标车道号
		l_u861Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // 重量数据分段数
	  	for(i=0;i<10;i++)
		{
		    if( g_asAxleLoadData[ln].axle_data[i].che_num!=0 )
		   	{
			   l_u8FenDuanNum++;
			   l_u8FenDuanBuf[j++] = i;
			}
		}
		l_u861Data[l_u8Len++] = (l_u8FenDuanNum&0xFF);
		l_u861Data[l_u8Len++] = (l_u8FenDuanNum>>8);
	
		for(i=0;i<l_u8FenDuanNum;i++)
		{
		    // 重量数据段
			l_u861Data[l_u8Len++] = l_u8FenDuanBuf[i] + 1;
		    // 车辆总数
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].che_num&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].che_num>>8;
		  	// 重量总数
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>8;
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>16;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>24;
			// 当量轴次合计
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>8;
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>16;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>24;
		}
	 }

	 // 清除统计数据缓存
     for(i=0;i<4;i++)
	 {
	     for(j=0;j<10;j++)
		 {
		     g_asAxleLoadData[i].axle_data[j].che_num = 0;
			 g_asAxleLoadData[i].axle_data[j].axis_num = 0;
			 g_asAxleLoadData[i].axle_data[j].allche_wt = 0;
			 g_asAxleLoadData[i].axle_data[j].allaxle_wt = 0;
			 g_asAxleLoadData[i].axle_data[j].acc_equivalent_axles = 0;
		 }
	 }

	 // 数据长度
	 l_u861Data[510] = (l_u8Len>>8);
	 l_u861Data[511] = l_u8Len;
	 // 得到当前SD卡存储序号
	 g_u32JD61SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD61BaseAdr + g_u32JD61SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u861Data);   
	 if( l_u8ret != 0 )							//写SD卡出错  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is61FirstSave != 1 )     // 未存储过61包
	 {
		 // 置位61包存储标志
		 g_u8Is61FirstSave = 1;
		 FSave61ToTDC(3);

		 if( g_u32JD61SaveSuccInd == 0 )
		 {
		     g_u32JD61SendSuccInd = 0;
		 }
		 else
		 {
			 g_u32JD61SendSuccInd = FGetLastSaveNum(g_u32JD61SaveSuccInd,g_u8PeriodTOSave);   
		 }
		 // 存储已发送成功的61包序号
		 FSave61ToTDC(2);;
	 }

	if( g_u32JD61SaveSuccInd < g_u32JD61SendSuccInd )    // 定时周期变化时
	{
 	    g_u32JD61SendSuccInd = FGetLastSaveNum(g_u32JD61SaveSuccInd,g_u8PeriodTOSave);  
	    // 存储61包成功发送的序号
	    FSave61ToTDC(2);
	}

     // 存储当前61包序号
	 FSave61ToTDC(1);

	 return l_u32address;	    // 返回SD卡扇区地址
}
uint8  FJD62ToSaveSend(uint16 pTimeNum)
{
	uint8   i;
	uint8   j;
	uint8   ln;
	uint8   l_u8ret;
	uint8   l_u8FenDuanNum;
	uint16  l_u16Year;
    uint8   l_u8Month;
    uint8   l_u8Day;
	uint8   l_u8FenDuanBuf[18];
	uint8   l_u862Data[512];
	uint16  l_u8Len;
	uint32  l_u32address;
//	SystemTime	l_sDevCurTime;

//	GET8563(&l_sDevCurTime);


	// 

    // 当24时0点的时候，扇区地址按照前一天的处理周期计算
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// 得到当前日期
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// 得到当前车道数
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// 命令字
	l_u862Data[l_u8Len++] = 0x62;
	//设备身份识别码
	for(i=0;i<16;i++)
	{
	    l_u862Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//站点编号
	for(i=0;i<15;i++)
	{
	    l_u862Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//设备硬件错误代码
	l_u862Data[l_u8Len++] = 0x00;
	
	//年
	l_u862Data[l_u8Len++] = l_u16Year;
	l_u862Data[l_u8Len++] = (l_u16Year>>8);
	//月
	l_u862Data[l_u8Len++] = l_u8Month;
	//日
	l_u862Data[l_u8Len++] = l_u8Day;
	// 数据处理周期
    l_u862Data[l_u8Len++] = g_u8PeriodTOSave;
	//时间序号 低位在前
	l_u862Data[l_u8Len++] = pTimeNum&0xff;
	l_u862Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//车道数
	l_u862Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// 将车道号转换成国标车道号
		l_u862Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // 重量数据分段数
	  	for(i=0;i<5;i++)
		{
		    if( g_asAxleLoadData[ln].axis_num_data[i].che_num!=0 )
		   	{
			   l_u8FenDuanNum++;
			   l_u8FenDuanBuf[j++] = i;
			}
		}
		l_u862Data[l_u8Len++] = (l_u8FenDuanNum&0xFF);
		l_u862Data[l_u8Len++] = (l_u8FenDuanNum>>8);
	
		for(i=0;i<l_u8FenDuanNum;i++)
		{
		    // 轴数数据段
			l_u862Data[l_u8Len++] = l_u8FenDuanBuf[i] + 2;
		    // 车辆总数
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].che_num&0xFF;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].che_num>>8;
		  	// 重量总数
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt&0xFF;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>8;
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>16;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>24;
		}
	 }

	 // 清除统计数据缓存
     for(i=0;i<4;i++)
	 {
	     for(j=0;j<5;j++)
		 {
		     g_asAxleLoadData[i].axis_num_data[j].che_num = 0;
			 g_asAxleLoadData[i].axis_num_data[j].allche_wt = 0;
		 }
	 }

	 // 数据长度
	 l_u862Data[510] = (l_u8Len>>8);
	 l_u862Data[511] = l_u8Len;
	 // 得到当前SD卡存储序号
	 g_u32JD62SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD62BaseAdr + g_u32JD62SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u862Data);   
	 if( l_u8ret != 0 )							//写SD卡出错  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is62FirstSave != 1 )     // 未存储过61包
	 {
		 // 置位62包存储标志
		 g_u8Is62FirstSave = 1;
		 FSave62ToTDC(3);

		 if( g_u32JD62SaveSuccInd == 0 )
		 {
		     g_u32JD62SendSuccInd = 0;
		 }
		 else
		 {
			 g_u32JD62SendSuccInd = FGetLastSaveNum(g_u32JD62SaveSuccInd,g_u8PeriodTOSave);   
		 }
		 // 存储已发送成功的62包序号
		 FSave62ToTDC(2);;
	 }

	if( g_u32JD62SaveSuccInd < g_u32JD62SendSuccInd )    // 定时周期变化时
	{
 	    g_u32JD62SendSuccInd = FGetLastSaveNum(g_u32JD62SaveSuccInd,g_u8PeriodTOSave);  
	    // 存储62包成功发送的序号
	    FSave62ToTDC(2);
	}

     // 存储当前62包序号
	 FSave62ToTDC(1);

	 return l_u32address;	    // 返回SD卡扇区地址
}
uint8  FJD68ToSaveSend(uint16 pTimeNum)
{
	uint8   i;
	uint8   j;
	uint8   ln;
	uint8   l_u8ret;
	uint8   l_u8FenDuanNum;
	uint16  l_u16Year;
    uint8   l_u8Month;
    uint8   l_u8Day;
	uint8   l_u8FenDuanBuf[18];
	uint8   l_u868Data[512];
	uint16  l_u8Len;
	uint32  l_u32address;
//	SystemTime	l_sDevCurTime;

//	GET8563(&l_sDevCurTime);


	// 

    // 当24时0点的时候，扇区地址按照前一天的处理周期计算
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// 得到当前日期
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// 得到当前车道数
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// 命令字
	l_u868Data[l_u8Len++] = 0x68;
	//设备身份识别码
	for(i=0;i<16;i++)
	{
	    l_u868Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//站点编号
	for(i=0;i<15;i++)
	{
	    l_u868Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//设备硬件错误代码
	l_u868Data[l_u8Len++] = 0x00;
	
	//年
	l_u868Data[l_u8Len++] = l_u16Year;
	l_u868Data[l_u8Len++] = (l_u16Year>>8);
	//月
	l_u868Data[l_u8Len++] = l_u8Month;
	//日
	l_u868Data[l_u8Len++] = l_u8Day;
	// 数据处理周期
    l_u868Data[l_u8Len++] = g_u8PeriodTOSave;
	//时间序号 低位在前
	l_u868Data[l_u8Len++] = pTimeNum&0xff;
	l_u868Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//车道数
	l_u868Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// 将车道号转换成国标车道号
		l_u868Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // 重量数据分段数
	  	for(i=0;i<4;i++)
		{
		    if( g_asAxleLoadData[ln].axis_num_data[i].che_num!=0 )
		   	{
			   l_u8FenDuanNum++;
			   l_u8FenDuanBuf[j++] = i;
			}
		}
		l_u868Data[l_u8Len++] = (l_u8FenDuanNum&0xFF);
		l_u868Data[l_u8Len++] = (l_u8FenDuanNum>>8);
	
		for(i=0;i<l_u8FenDuanNum;i++)
		{
		    // 轴重数据段
			  l_u868Data[l_u8Len++] = l_u8FenDuanBuf[i] + 1;
			  // 轴总数
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>24;
		  	  // 轴重量总数
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>24;
			  // 当量轴次和计
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>24;
		}
	 }

	 // 清除统计数据缓存
     for(i=0;i<4;i++)
	 {
	     for(j=0;j<4;j++)
		 {
		     g_asAxleLoadData[i].axle_weight_data[j].axis_num   = 0;
			 g_asAxleLoadData[i].axle_weight_data[j].allaxle_wt = 0;
			 g_asAxleLoadData[i].axle_weight_data[j].acc_equivalent_axles = 0;
		 }
	 }

	 // 数据长度
	 l_u868Data[510] = (l_u8Len>>8);
	 l_u868Data[511] = l_u8Len;
	 // 得到当前SD卡存储序号
	 g_u32JD68SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD68BaseAdr + g_u32JD68SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u868Data);   
	 if( l_u8ret != 0 )							//写SD卡出错  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is68FirstSave != 1 )     // 未存储过61包
	 {
		 // 置位68包存储标志
		 g_u8Is68FirstSave = 1;
		 FSave68ToTDC(3);

		 if( g_u32JD68SaveSuccInd == 0 )
		 {
		     g_u32JD68SendSuccInd = 0;
		 }
		 else
		 {
			 g_u32JD68SendSuccInd = FGetLastSaveNum(g_u32JD68SaveSuccInd,g_u8PeriodTOSave);   
		 }
		 // 存储已发送成功的68包序号
		 FSave68ToTDC(2);;
	 }

	if( g_u32JD68SaveSuccInd < g_u32JD68SendSuccInd )    // 定时周期变化时
	{
 	    g_u32JD68SendSuccInd = FGetLastSaveNum(g_u32JD68SaveSuccInd,g_u8PeriodTOSave);  
	    // 存储68包成功发送的序号
	    FSave68ToTDC(2);
	}

     // 存储当前68包序号
	 FSave68ToTDC(1);

	 return l_u32address;	    // 返回SD卡扇区地址
}

