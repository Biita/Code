/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JDProcess.C
** Last modified Date:  20110512
** Last Version:		1.0
** Descriptions:		���ݷ������񣬰��������г�����(����4)�ͼ�������(����6)
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

#define		SETUPALIAS				g_Setup			//���ò����ṹ
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

	 // �������
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
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"JDPAN, Info=%d,%d,%d,%d,%d��\r\n",
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

	// ͳ�����������ֶ�����	
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
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].che_num++;                                                    // ��������
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].allche_wt	+= l_u32TotalWgh;                                  // ������
	g_asAxleLoadData[l_u8LaneNum/2].axle_data[i].acc_equivalent_axles += l_u32DangliangZhouZo;                 // ������κϼ�


	// ͳ�������ֶ�����
	if((l_u8AxleCnt > 1) && (l_u8AxleCnt < 7))
		i = l_u8AxleCnt - 2;
	else
		i = 0;
	g_asAxleLoadData[l_u8LaneNum/2].axis_num_data[i].che_num++;                                                // ��������
	g_asAxleLoadData[l_u8LaneNum/2].axis_num_data[i].allche_wt += l_u32TotalWgh;                                // ������ 

	// ͳ�����طֶ�����
	for(l_u8tempi=0;l_u8tempi<l_u8AxleCnt;l_u8tempi++)
	{
//		if( pItem->au16AxleWeight[l_u8tempi]*10 > JDSYSPARAM.m_u32MaxForAxle )   // ������Ƿ���
//		{
////		      // ���ޱ�־
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
 	    g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].axis_num++;                                               // ������	
		g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].allaxle_wt += pItem->au16AxleWeight[l_u8tempi]*10;							     // ������
        g_asAxleLoadData[l_u8LaneNum/2].axle_weight_data[i].acc_equivalent_axles += l_u32DangliangZhou[l_u8tempi];         // ������κϼ�
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
	                return 1;     // ����쳣��Ĭ��С�ͳ�  
	            }
	
	            if(pTotalDis>8000)  
	            {
	                return 4;     // ����쳣��Ĭ�����ͻ���  
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
		//����� ���ڷ�����
			l_pu8DataBuf[l_u8BufIndex++] = 0xFF;
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			l_pu8DataBuf[l_u8BufIndex++] = 0x08;
			//���ݳ���
			l_pu8DataBuf[l_u8BufIndex++] = (54+32*PROMODPARAM.m_u8LaneNum<<8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (54+32*PROMODPARAM.m_u8LaneNum)&0xff;	//42�ֽڹ������ݣ�5�ֽڵ������������ݣ�ÿ�ֳ���3��
			//���ݰ�����
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			//�豸���ʶ����
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
			}
			//վ����
			for(l_u8tempi=0;l_u8tempi<15;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
			}
			//�豸Ӳ���������
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			//��������
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u16Year;
			l_pu8DataBuf[l_u8BufIndex++] = (l_sDevCurTime.u16Year>>8);
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Month;
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Day;
			
			//��ͨ���ݴ�������
			l_pu8DataBuf[l_u8BufIndex++] = g_u8PeriodTOSave;
			//ʱ����� ��λ��ǰ
			l_pu8DataBuf[l_u8BufIndex++] = pTimeNum&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (pTimeNum>>8)&0xff;

			//������
			l_pu8DataBuf[l_u8BufIndex++] = PROMODPARAM.m_u8LaneNum;
		
			for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)
			{	
				// ��������ת���ɹ��공����
		 	  	l_pu8DataBuf[l_u8BufIndex++] = FChangLaneToBG(l_u8tempi);  
				
				//�����ٷֱ�
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
			
				//ƽ����ͷ���
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = g_asTrafficData[l_u8tempi].mu32TotalDisForVehHead/g_asTrafficData[l_u8tempi].mu32VehNum;
				}
				//����
				if(g_asTrafficData[l_u8tempi].mu32AveDisForVehHead == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 100;
				}
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead>>8)&0xff;
			
				//ʱ��ռ����
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TimeOccupancy*100/60000;

				//��С�ͽ�ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum>>8)&0xff;
					
				//��С��ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeAveSpeed;

				//С����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoNum&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32XiaoHuoNum>>8)&0xff;
							
				//С��ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32XiaoHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32XiaoHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32XiaoHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoAveSpeed;

				//��ͽ�ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum>>8)&0xff;
							
				//���ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32DaKeNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed = g_asTrafficData[l_u8tempi].mu32DaKeTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaKeNum;
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaKeAveSpeed;
							
				//�л���ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum>>8)&0xff;	
						
				//�л�ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32ZhongHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32ZhongHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32ZhongHuoNum;
				
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32ZhongHuoAveSpeed;	
			
				//�����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum>>8)&0xff;
							
				//���ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32DaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32DaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32DaHuoNum;			
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32DaHuoAveSpeed;

				//�ش����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum>>8)&0xff;
							
				//�ش��ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32TeDaHuoNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed = g_asTrafficData[l_u8tempi].mu32TeDaHuoTotalSpeed/g_asTrafficData[l_u8tempi].mu32TeDaHuoNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TeDaHuoAveSpeed;
			
				//��װ�佻ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum>>8)&0xff;
							
				//��װ��ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed = g_asTrafficData[l_u8tempi].mu32JiZhangXiangTotalSpeed/g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum;	
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32JiZhangXiangAveSpeed;
				
				//��������ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum>>8)&0xff;
							
				//������ƽ���ص㳵��
				if(g_asTrafficData[l_u8tempi].mu32TuoLaJiNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = 0;
				}
				else
				{
				  	g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed = g_asTrafficData[l_u8tempi].mu32TuoLaJiTotalSpeed/g_asTrafficData[l_u8tempi].mu32TuoLaJiNum;		
				}
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TuoLaJiAveSpeed;
	
				//Ħ�г���ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum>>8)&0xff;	
						
				//Ħ�г�ƽ���ص㳵��
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
			//�������֡֡����
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
			//���ݳ���
			l_pu8DataBuf[l_u8BufIndex++] = (54+68*PROMODPARAM.m_u8LaneNum<<8)&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (54+68*PROMODPARAM.m_u8LaneNum)&0xff;	//42�ֽڹ������ݣ�5�ֽڵ������������ݣ�ÿ�ֳ���3��
			//���ݰ�����
			l_pu8DataBuf[l_u8BufIndex++] = 0x01;
			//�豸���ʶ����
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
			}
			//վ����
			for(l_u8tempi=0;l_u8tempi<15;l_u8tempi++)
			{
			    l_pu8DataBuf[l_u8BufIndex++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
			}
			//�豸Ӳ���������
			l_pu8DataBuf[l_u8BufIndex++] = 0x00;
			//��������
			l_pu8DataBuf[l_u8BufIndex++] = 0x02;
			
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u16Year;
			l_pu8DataBuf[l_u8BufIndex++] = (l_sDevCurTime.u16Year>>8);
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Month;
			//��
			l_pu8DataBuf[l_u8BufIndex++] = l_sDevCurTime.u8Day;
			
			//��ͨ���ݴ�������
			l_pu8DataBuf[l_u8BufIndex++] = g_u8PeriodTOSave;
			//ʱ����� ��λ��ǰ
			l_pu8DataBuf[l_u8BufIndex++] = pTimeNum&0xff;
			l_pu8DataBuf[l_u8BufIndex++] = (pTimeNum>>8)&0xff;

			//������
			l_pu8DataBuf[l_u8BufIndex++] = PROMODPARAM.m_u8LaneNum;
		
			for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)
			{	
				// ��������ת���ɹ��공����
		 	  	l_pu8DataBuf[l_u8BufIndex++] = FChangLaneToBG(l_u8tempi);  
				
				//�����ٷֱ�
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
			
				//ƽ����ͷ���
				if(g_asTrafficData[l_u8tempi].mu32VehNum == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 0;
				}
				else
				{
					g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = g_asTrafficData[l_u8tempi].mu32TotalDisForVehHead/g_asTrafficData[l_u8tempi].mu32VehNum;
				}
				//����
				if(g_asTrafficData[l_u8tempi].mu32AveDisForVehHead == 0)
				{
				 	g_asTrafficData[l_u8tempi].mu32AveDisForVehHead = 100;
				}
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32AveDisForVehHead>>8)&0xff;
			
				//ʱ��ռ����
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32TimeOccupancy*100/60000;

				//��С�ͽ�ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongXiaoKeNum>>8)&0xff;
					
				//��С��ƽ���ص㳵��
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

				//С����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = g_asTrafficData[l_u8tempi].mu32XiaoHuoNum&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32XiaoHuoNum>>8)&0xff;
							
				//С��ƽ���ص㳵��
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

				//��ͽ�ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaKeNum>>8)&0xff;
							
				//���ƽ���ص㳵��
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
							
				//�л���ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32ZhongHuoNum>>8)&0xff;	
						
				//�л�ƽ���ص㳵��
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
			
				//�����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32DaHuoNum>>8)&0xff;
							
				//���ƽ���ص㳵��
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

				//�ش����ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TeDaHuoNum>>8)&0xff;
							
				//�ش��ƽ���ص㳵��
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
			
				//��װ�佻ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32JiZhangXiangNum>>8)&0xff;
							
				//��װ��ƽ���ص㳵��
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
				
				//��������ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32TuoLaJiNum>>8)&0xff;
							
				//������ƽ���ص㳵��
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
	
				//Ħ�г���ͨ��
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum)&0xff;
				l_pu8DataBuf[l_u8BufIndex++] = (g_asTrafficData[l_u8tempi].mu32MotoNum>>8)&0xff;	
						
				//Ħ�г�ƽ���ص㳵��
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
			//�������֡֡����
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

	///////////////////////////////////////////��SD��
	g_u32JDSDSaveInd	= FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	l_u32SaveAddr = SD_JD_SEC_BGN + g_u32JDSDSaveInd;

	
	// ���ݱ��ݵ�SD���У�����ʱ���������
	l_u32Ret = SD_WriteBlock( &sd_info, l_u32SaveAddr, g_au8JDSDSaveBuf);   
	
	if( l_u32Ret != 0 )		//дSD������ 					 
	{
//		SD_Err_Reset_cnt++;
	}
	else
	{
//	    SD_Err_Reset_cnt = 0;
	}							  

	if( g_u8IsFirstSDSave != 1 )     // δ�洢��01��
	{
		// ��λ01���洢��־
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
		// �洢�ѷ��ͳɹ���01�����
		FSave01ToTDC(2);
	}

	if( g_u32JDSDSaveInd < g_u32JDSDSaveIndSuc )    // ��ʱ���ڱ仯ʱ
	{
 	    g_u32JDSDSaveIndSuc = FGetLastSaveNum(g_u32JDSDSaveInd,g_u8PeriodTOSave);  
	    // �洢01���ɹ����͵����
	    FSave01ToTDC(2);
	}

    // �洢��ǰ01�����
	FSave01ToTDC(1);

	// ����CRC
//	crc_create(frame, len);
//	// ����
//	uart0_sendStr(frame, len+3);	 
	return l_u32SaveAddr;	    // ����SD��������ַ
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
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// ���򳵵�
			   {
                    //������
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
			   else 	                // ˫�򳵵�
			   {
			        //������
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
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// ���򳵵�
			   {
                    //������
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
			   else 	                // ˫�򳵵�
			   {
			        //������
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
		       if( JDSYSPARAM.m_u8LaneDir == 0 )	// ���򳵵�
			   {
                    //������
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
			   else 	                // ˫�򳵵�
			   {
			        //������
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
    uint16 l_u16Totaldays = 0;  //�ܵ�����
    uint32 l_u32NumberID;	       //����õ������к�
    uint32 l_u32NumberID1;	
    uint8  l_u8DaysPerMon[13];
    uint8  l_u8tempi;
    l_u16Year = psTime->u16Year;
    l_u8Month = psTime->u8Month;

    l_u8Day = psTime->u8Day;
    l_u16TimeNum = pTimeNum;
   	  
    if( (l_u16Year%100)== 0 )       // ������
    {
	     if( (l_u16Year%400)==0 )    // ����
		 {
			l_u8DaysPerMon[2]=29;  
	     }
		 else	           
		 {
			l_u8DaysPerMon[2]=28;   // ƽ��
		 }
    }
	else
	{
		if( (l_u16Year%4)==0 )    // ����
		{
			l_u8DaysPerMon[2]=29;  
		}
		else	           
		{
			l_u8DaysPerMon[2]=28;   // ƽ��
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
	    if( (l_u16Year%100)== 0 )       // ������
        {
	        if( (l_u16Year%400)==0 )    // ����
		    {
			    l_u8DaysPerMon[2]=29;  
	     	}
			else	           
			{
			    l_u8DaysPerMon[2]=28;   // ƽ��
			}
        }
	    else
	    {
		    if( (l_u16Year%4)==0 )    // ����
		    {
			    l_u8DaysPerMon[2]=29;  
			}
			else	           
			{
				l_u8DaysPerMon[2]=28;   // ƽ��
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
    uint16  l_u16TotalDays = 0;  //�ܵ�����
    uint32  l_u32NumberID;	    //����õ������к�
    uint8   l_u8DaysPerMon[13];
 
    l_u16Year    = pYear - 1;
//    l_u8Month   = 1;
//    l_u8Day     = 1;
//    l_u16TimeNum = 1;

	while(l_u16Year>=2014)
	{
		if( (l_u16Year%100)== 0 )       // ������
        {
	    	if( (l_u16Year%400)==0 )    // ����
		    {
			    l_u8DaysPerMon[2]=29;  
	        }
		    else	           
		    {
			    l_u8DaysPerMon[2]=28;   // ƽ��
		    }
        }
	    else
	    {
		    if( (l_u16Year%4)==0 )    // ����
		    {
			    l_u8DaysPerMon[2]=29;  
		    }
		    else	           
		    {
			    l_u8DaysPerMon[2]=28;   // ƽ��
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

   if((pSaveNum%1440)!=1)//�жϺ����ڵĴ�С
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
	//����Ŀǰ������һ���ֻ��е����к�	
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
		if( l_u8DataCheckFlag == 1 )   // �����쳣 
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
			// �����쳣			
		    l_u16DataLen = FGetSendDataForErr(1, l_pu8SendBuf, l_pu8Buf, l_u8SendErrFlag);
			//�������ݰ� 
			l_u8Flag = FSendData(l_pu8SendBuf, l_u16DataLen);
			
		}
		else   // ��������
		{
			//�������ݰ�
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
					// �洢01���ɹ����͵����
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
		//�ȴ�����ȷ��֡����ɹ�������д
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
	 uint32 l_u32NeedSendNum = 0;//���������ݰ�����
	 uint32 l_u32LastNum;    //��һ���к�
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

   if((pNum%1440)<(1440/pPeriod))//�жϺ����ڵĴ�С
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

    // �����ּ��
	if( pCMD != pBuf[0] )
	{
		l_u8Result = 1;
	}	
	// ����豸���ʶ����
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
	    if( pBuf[1+l_u8tempi] != JDSYSPARAM.m_au8DeviceID[l_u8tempi] )
		{
		    l_u8Result = 1;
		}	
	}
	// ���վ����
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
				//���ݰ�����
				pDataBuf[l_u16Len++] = 0x01;
				//�豸���ʶ����
				for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
				}
				//վ����
				for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
				}
				//�豸Ӳ���������
				pDataBuf[l_u16Len++] = pErrFlag;
				//��������
				pDataBuf[l_u16Len++] = 0x01;
				//��
				pDataBuf[l_u16Len++] = l_u16Year;
				pDataBuf[l_u16Len++] = (l_u16Year>>8);
				//��
				pDataBuf[l_u16Len++] = l_u8Month;
				//��
				pDataBuf[l_u16Len++] = l_u8Day;
				//��ͨ���ݴ�������
				pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
				//ʱ����� ��λ��ǰ
				pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
				pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
	
				//������
				pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
	
				for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
				{	
					// ��������ת���ɹ��공����
			 	  	pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi);  
					//�����ٷֱ�
					pDataBuf[l_u16Len++] = 0;
					//ƽ����ͷ���
					pDataBuf[l_u16Len++] = 100;
					pDataBuf[l_u16Len++] = 0;
					//ʱ��ռ����
					pDataBuf[l_u16Len++] = 0;
					
					//��С�ͽ�ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;	
					//��С��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					//С����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;			
					//С��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					//��ͽ�ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//���ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
								
					//�л���ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//�л�ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;	
				
					//�����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//���ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					//�ش����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//�ش��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
				
					//��װ�佻ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//��װ��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
					
					//��������ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//������ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
		
					//Ħ�г���ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//Ħ�г�ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
				}
				break;
	
			case 0x02:
				pDataBuf[l_u16Len++] = 0x01;
				//�豸���ʶ����
				for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
				}
				//վ����
				for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
				{
				    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
				}
				//�豸Ӳ���������
				pDataBuf[l_u16Len++] = pErrFlag;
				//��������
				pDataBuf[l_u16Len++] = 0x02;
				//��
				pDataBuf[l_u16Len++] = l_u16Year;
				pDataBuf[l_u16Len++] = (l_u16Year>>8);
				//��
				pDataBuf[l_u16Len++] = l_u8Month;
				//��
				pDataBuf[l_u16Len++] = l_u8Day;
				//��ͨ���ݴ�������
			    pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
				//ʱ����� ��λ��ǰ
				pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
				pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
				//������
				pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
			
				for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
				{
					// ��������ת���ɹ��공����
			 	  	pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi); 
					//�����ٷֱ�
					pDataBuf[l_u16Len++] = 0;
					//ƽ����ͷ���
					pDataBuf[l_u16Len++] = 100;
					pDataBuf[l_u16Len++] = 0;
					//ʱ��ռ����
					pDataBuf[l_u16Len++] = 0;
	
					//��С�ͽ�ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;					
					//��С��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//С����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//С��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//��ͽ�ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//���ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
								
					//�л���ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//�л�ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;	
				
					//�����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;								
					//���ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
	
					//�ش����ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//�ش��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
				
					//��װ�佻ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//��װ��ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					
					//��������ͨ��
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;							
					//������ƽ���ص㳵��
					pDataBuf[l_u16Len++] = 0;
	
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
					pDataBuf[l_u16Len++] = 0;
		
					//Ħ�г���ͨ��
					pDataBuf[l_u16Len++] =0;
					pDataBuf[l_u16Len++] =0;							
					//Ħ�г�ƽ���ص㳵��
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
		// ������
		pDataBuf[l_u16Len++] = pCMD;
		//�豸���ʶ����
		for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
		{
		    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];	
		}
		//վ����
		for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
		{
		    pDataBuf[l_u16Len++] = JDSYSPARAM.m_au8StationID[l_u8tempi];		
		}
		//�豸Ӳ���������
		pDataBuf[l_u16Len++] = pErrFlag;
		//��
		pDataBuf[l_u16Len++] = l_u16Year;
		pDataBuf[l_u16Len++] = (l_u16Year>>8);
		//��
		pDataBuf[l_u16Len++] = l_u8Month;
		//��
		pDataBuf[l_u16Len++] = l_u8Day;
		// ���ݴ�������
		pDataBuf[l_u16Len++] = g_u8PeriodTOSave;
		//ʱ����� ��λ��ǰ
		pDataBuf[l_u16Len++] = l_u16TimeNum&0xff;
		pDataBuf[l_u16Len++] = (l_u16TimeNum>>8)&0xff;
		//������
		pDataBuf[l_u16Len++] = PROMODPARAM.m_u8LaneNum;
	
		for(l_u8tempi=0; l_u8tempi<PROMODPARAM.m_u8LaneNum; l_u8tempi++)
		{				 
			  // ��������ת���ɹ��공����
			  pDataBuf[l_u16Len++] = FChangLaneToBG(l_u8tempi); 
	          // ���������ݷֶ���
			  pDataBuf[l_u16Len++] = 0;//(fenduan_num&0xFF);
			  pDataBuf[l_u16Len++] = 0;//(fenduan_num>>8);
		 }
	}

	return l_u16Len;	    // ���ݰ�����
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

    //��¼�������
	g_u32JD39SaveSuccInd++;
	if( g_u32JD39SaveSuccInd > (g_u32JD61BaseAdr - 1000) )
	{
		g_u32JD39SaveSuccInd = 0;
	}
	l_u8Buf[i++] = g_u32JD39SaveSuccInd&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>8)&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>16)&0xff;
	l_u8Buf[i++] = (g_u32JD39SaveSuccInd>>24)&0xff;
	// �ꡢ�¡���
	l_u8Buf[i++] = g_DevCurTime.u16Year;    //
	l_u8Buf[i++] = g_DevCurTime.u16Year>>8;
	l_u8Buf[i++] = g_DevCurTime.u8Month;     
	l_u8Buf[i++] = g_DevCurTime.u8Day;	   
	// ��¼������
	l_u8Buf[i++] = PROMODPARAM.m_u8LaneNum;   	     
	//��¼������
//	if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//		l_u8Buf[i++] = FChangLaneToBG(pdata->u8psLane);   // ���공����
//	else
		l_u8Buf[i++] = FChangLaneToBG(pdata->u8psLane/2);   // ���공����
	// ʱ���֡���
	l_u8Buf[i++] = g_DevCurTime.u8Hour;
	l_u8Buf[i++] = g_DevCurTime.u8Minute;
	l_u8Buf[i++] = g_DevCurTime.u8Second;
	//��¼����
	l_u8Buf[i++] = pdata->m_u8VehType;   // 
	//��¼����
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
	//��¼��������
	l_u8Buf[i++] = l_u32TotalWeight&0xFF;       // 
	l_u8Buf[i++] = (l_u32TotalWeight>>8)&0xFF;        // 
	l_u8Buf[i++] = (l_u32TotalWeight>>16)&0xFF;        // 
	l_u8Buf[i++] = (l_u32TotalWeight>>24)&0xFF;        // 
	//��¼�ٶ�
   	l_u8Buf[i++] = (pdata->u16speed/10)&0xFF;
	l_u8Buf[i++] = ((pdata->u16speed/10)>>8)&0xFF;

	// ����
	for (j=0; j < pdata->u8AixCnt; j++)
	{
		l_u8Buf[i++] = pdata->au16AxleWeight[j]*10;
		l_u8Buf[i++] = (pdata->au16AxleWeight[j]*10)>>8;
	}
	// ����
	l_u16TotalDis = 0;
	for (j=0; j < pdata->u8AixCnt -1; j++)
	{
		l_u8Buf[i++] = pdata->au16AxleDistance[j]*10;
	    l_u8Buf[i++] = (pdata->au16AxleDistance[j]*10)>>8;
		l_u16TotalDis += pdata->au16AxleDistance[j]*10;
	}
	//��¼�����
   	l_u8Buf[i++] =  l_u16TotalDis&0xFF;
	l_u8Buf[i++] =  (l_u16TotalDis>>8)&0xFF;
	// ��¼���ݳ���
	l_u8Len = i;

	//  ����50���ֽڲ�0
	if( i < 50 )
	{
		for(j=i;j<50;j++)
		{
		    l_u8Buf[i++] = 0;
		}
	}

	// �¶�
//	if((curRst.ln_wendu <= 333) && (curRst.ln_wendu >= 235))
//	{
//		l_cSendTemp = curRst.ln_wendu - 273;
//	}
//	else
//	{
	l_cSendTemp = 102;
//	}
	l_u8Buf[47] = l_cSendTemp;    
	l_u8Buf[48] = l_u8OverFlag;   // ���ޱ�־
	l_u8Buf[49] = l_u8Len; 
	
	if( g_u8Is39FirstSave != 1 )     // δд����������
	{
	    // ���㵥���洢��־
		g_u8Is39FirstSave = 1;
		FSave39ToTDC(3);
		// �洢��ǰ�����������
		FSave39ToTDC(1);
		// �洢�ɹ����͵ĵĵ������
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
	// ���������ݴ���SD��
	l_u32AxleAdr = g_u32JD39BaseAdr + g_u32JD39SaveSuccInd;
	SD_WriteBlock( &sd_info, l_u32AxleAdr, l_u8Buf);
	// ��������Ŵ洢��������
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
	// ������
	l_u8Buf[l_u8Len++] = 0x39; 
	// վ����
	for(i=0;i<15;i++)
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];	
	}
	// �豸���ʶ����
	for(i=0;i<16;i++)					 
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	// �豸Ӳ���������
	l_u8Buf[l_u8Len++] = 0x00;

	if( g_u32JD39SaveSuccInd > g_u32JD39SendSuccInd )		
	{	
		if((g_u32JD39SaveSuccInd - g_u32JD39SendSuccInd) >JDSYSPARAM.m_u16VehNumForSend)
		{
		   g_u32JD39SendSuccInd = g_u32JD39SaveSuccInd - JDSYSPARAM.m_u16VehNumForSend;
		   FSave39ToTDC(2);
		}

		// ������һ������SD�������׵�ַ
		l_u32NeedSendNum = g_u32JD39SendSuccInd + 1;
		l_u32Raddress = g_u32JD39BaseAdr + l_u32NeedSendNum;
		// ��ȡҪ���͵ĵ�������
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
		// ��ȡ�¶�ֵ����Ӳ��������������� 
		l_u8Buf[32]   = l_u8ReadSDbuf[47];
		// ��ȡ���ݰ�����
		l_u16AxleSendLen = l_u8ReadSDbuf[49] + l_u8Len;
		// ��ȡ�ó��Ƿ��ޱ�־
		l_u8OverFlag    = l_u8ReadSDbuf[48]; 
		// ��¼Ҫ���ͳ������ݰ������
		l_u32CarNumSend = l_u8Buf[33] + (l_u8Buf[34]<<8) + (l_u8Buf[35]<<16) + (l_u8Buf[36]<<24);	   
		
		if( (l_u32CarNumSend != l_u32NeedSendNum)||(l_u16AxleSendLen==0)||(l_u16AxleSendLen>512) )
		{
		    l_u8DataErrFlag = 1;     // �����쳣
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
		switch(JDSYSPARAM.m_u8SendVehType)//�������������ж� 
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
					    // ����39���ݰ�
//				        Send_Feedback = SendData(send_data39_err,l_u16AxleSendErrLen);
						l_u16AxleSendErrLen = FGet39OrderForErr(l_u8SendErrData,l_u32NeedSendNum,l_u8err);
						//�������ݰ� 
						l_u8FeedBack = FSendData(l_u8SendErrData, l_u16AxleSendErrLen);
					}
					else  
					{
					    // �洢�ɹ����ͼ���ֵ
			   			g_u32JD39SendSuccInd = l_u32NeedSendNum;
						FSave39ToTDC(2);
						return ; 
					}
				}
				else
				{
					// ����39���ݰ�
				    l_u8FeedBack = FSendData(l_u8Buf,l_u16AxleSendLen);	
				}
				if( l_u8FeedBack == 1 )      // ���ͳɹ�
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
							// �洢01���ɹ����͵����
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
							// �洢01���ɹ����͵����
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
		     if( l_u8OverFlag == 0 )   // �ó�������
			 {
			    // �洢�ɹ����ͼ���ֵ
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
//					    // ����39���ݰ�
//				        Send_Feedback = SendData(send_data39_err,l_u16AxleSendErrLen);
						l_u16AxleSendErrLen = FGet39OrderForErr(l_u8SendErrData,l_u32NeedSendNum,l_u8err);
						//�������ݰ� 
						l_u8FeedBack = FSendData(l_u8SendErrData, l_u16AxleSendErrLen);
					}
					else  
					{
					    // �洢�ɹ����ͼ���ֵ
			   			g_u32JD39SendSuccInd = l_u32NeedSendNum;
						FSave39ToTDC(2);
						return ; 
					}
				}
				else
				{
					// ����39���ݰ�
				    l_u8FeedBack = FSendData(l_u8Buf,l_u16AxleSendLen);	
				}
				if( l_u8FeedBack == 1 )      // ���ͳɹ�
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
							// �洢01���ɹ����͵����
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
							// �洢01���ɹ����͵����
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
	// �õ�����ʱ��

	l_u8Len = 0;
	// ������
	pDataBuf[l_u8Len++] = 0x39; 
	// վ����
	for(i=0;i<15;i++)
	{
		pDataBuf[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];	
	}
	// �豸���ʶ����
	for(i=0;i<16;i++)					 
	{
		pDataBuf[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	// �豸Ӳ���������
	pDataBuf[l_u8Len++] = pErrFlag;

    //��¼�������
	pDataBuf[l_u8Len++] = pSendNum&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>8)&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>16)&0xff;
	pDataBuf[l_u8Len++] = (pSendNum>>24)&0xff;
	// �ꡢ�¡���
	pDataBuf[l_u8Len++] = l_sDevCurTime.u16Year;    //
	pDataBuf[l_u8Len++] = l_sDevCurTime.u16Year>>8;
	pDataBuf[l_u8Len++] = l_sDevCurTime.u8Month;     
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Day >> 4)&0x0F)*10 + (l_sDevCurTime.u8Day&0x0F);	   
	// ��¼������
	pDataBuf[l_u8Len++] = PROMODPARAM.m_u8LaneNum;   	     
	//��¼������
	pDataBuf[l_u8Len++] = 11;   // ���공����
	// ʱ���֡���
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Hour >> 4)&0x0F)*10 + (l_sDevCurTime.u8Hour&0x0F);
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Minute >> 4)&0x0F)*10 + (l_sDevCurTime.u8Minute&0x0F);
	pDataBuf[l_u8Len++] = ((l_sDevCurTime.u8Second >> 4)&0x0F)*10 + (l_sDevCurTime.u8Second&0x0F);
	//��¼����
	pDataBuf[l_u8Len++] = 1;   // С�ͳ�
	//��¼����
	pDataBuf[l_u8Len++] = 2;   // 
	//��¼��������
	pDataBuf[l_u8Len++] = 2000&0xFF;         // 
	pDataBuf[l_u8Len++] = (2000>>8)&0xFF;    // 
	pDataBuf[l_u8Len++] = (2000>>16)&0xFF;   // 
	pDataBuf[l_u8Len++] = (2000>>24)&0xFF;   // 
	//��¼�ٶ�
   	pDataBuf[l_u8Len++] = 60&0xFF;
	pDataBuf[l_u8Len++] = (60>>8)&0xFF;

	// ����
	for (j=0; j < 2; j++)
	{
		pDataBuf[l_u8Len++] = 1000&0xFF;
		pDataBuf[l_u8Len++] = (1000>>8)&0xFF;
	}
	// ����
	for (j=0; j < 1; j++)
	{
		pDataBuf[l_u8Len++] = 2600&0xFF;
	    pDataBuf[l_u8Len++] = (2600>>8)&0xFF;
	}
	//��¼�����
   	pDataBuf[l_u8Len++] =  2600&0xFF;
	pDataBuf[l_u8Len++] =  (2600>>8)&0xFF;

	return l_u8Len;	    // �������ݰ�����
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
		          if( g_u8Is61FirstSave != 1 )     // δ�洢�����ݰ�
				  {
				      return;
				  }
		          l_u32Sd_BaseAddress = g_u32JD61BaseAdr;
				  break;	
	    case 0x62:
		          if( g_u8Is62FirstSave != 1 )     // δ�洢�����ݰ�
				  {
				      return;
				  }
				  l_u32Sd_BaseAddress = g_u32JD62BaseAdr;
				  break;
	    case 0x68:
		          if( g_u8Is68FirstSave != 1 )     // δ�洢�����ݰ�
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

		// �õ���Ҫ���͵�SD�����, Success_Send_data_NUM ����һ�����
		l_u32NeedSendSDNum = FGetNextSDNum(pSendID,g_u8PeriodTOSave);

		//  ͨ���洢��ţ��õ����ڼ�ʱ�����
		pBuf = FGetDateFromNum(l_u32NeedSendSDNum);
		for(i=0;i<6;i++)
		{
		    l_u8DateTimeNum[i] = *(pBuf+i);
		}
		// ��Ҫ���͵�ʱ�����
		l_u16NeedSendTimeNum = l_u8DateTimeNum[4] + (l_u8DateTimeNum[5]<<8);

		switch(pCmdType)
		{
		    case 0x61:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd61Num )
					  {
					  	  g_u16RecOrd61Num = 0xffff;
						  // �洢61���ɹ����͵����
						  g_u32JD61SendSuccInd = l_u32NeedSendSDNum;
						  FSave61ToTDC(2);
						  return;
					  }							  
					  break;
		    case 0x62:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd62Num )
					  {
					  	  g_u16RecOrd62Num = 0xffff;
						  // �洢62���ɹ����͵����
						  g_u32JD62SendSuccInd = l_u32NeedSendSDNum;
						  FSave62ToTDC(2);
						  return;
					  }							  
					  break;			
		    case 0x68:
			     	  if( l_u16NeedSendTimeNum == g_u16RecOrd68Num )
					  {
					  	  g_u16RecOrd68Num = 0xffff;
						  // �洢68���ɹ����͵����
						  g_u32JD68SendSuccInd = l_u32NeedSendSDNum;
						  FSave68ToTDC(2);
						  return;
					  }							  
					  break;			
			  default:
			          break;
		}
		// �õ�SD��������ַ
		l_u32Raddress = l_u32Sd_BaseAddress + l_u32NeedSendSDNum; 
		// ��SD������
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
		// ��ȡ���ݰ�����
		l_u16SendDataLen = (l_u8SendData[510]<<8) + l_u8SendData[511];
		// ��SD����������ʱ�����
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
		
		// ��������Ƿ��쳣
		l_u8DataCheckFlag = FCheckSendData(l_u8SendData, pCmdType);
//		Err_data_flag = Check_data(send_data,SendData_Type);  // ��������֡�ʶ���롢վ����

		// �ж������Ƿ��쳣
		if( (l_u16ReadNumFromSD != l_u16NeedSendTimeNum)||(l_u16SendDataLen==0)||(l_u16SendDataLen>=1000) )
		{
			l_u8DataCheckFlag = 1;
		}	

		if( l_u8DataCheckFlag == 1 )   // �����쳣 
		{
			BeepON();
			OSTimeDly(10);
			BeepOFF();	
			OSTimeDly(10);
			if( l_u8SendErr == 0 )
			{
			    l_u8SendErr = 100;
			}
			// �����쳣			
		    l_u16SendDataLen = FGetSendDataForErr( pCmdType, l_u8SendData, l_u8DateTimeNum, l_u8SendErr );
			//�������ݰ� 
			l_u8FeedBack = FSendData(l_u8SendData,l_u16SendDataLen);
		}
		else   // ��������
		{
			//�������ݰ�
			l_u8FeedBack = FSendData(l_u8SendData,l_u16SendDataLen);		
		}

		if( l_u8FeedBack == 1 )    // �������
		{	// ��¼��־��Ϣ
//			if( SendData_Type == 0x01 )
//		    {
//			    EVENT_0ARev->OSEventCnt = 0;
//			    OSSemPend(EVENT_0ARev,8000,&err);	//�ȴ�0A�����ء�2000--5s; 8000--20s; 10000--25s
//			}
//			else
//			{
//				EVENT_35Rev->OSEventCnt = 0;
//			    OSSemPend(EVENT_35Rev,8000,&err);	//�ȴ�35�����ء�2000--5s; 8000--20s; 10000--25s
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
								  // �洢61���ɹ����͵����
								  g_u32JD61SendSuccInd = l_u32NeedSendSDNum;
								  FSave61ToTDC(2);
							  }							  
							  break;
				    case 0x62:
					     	  if( l_u16NeedSendTimeNum == g_u16RecOrd62Num )
							  {
							  	  g_u16RecOrd62Num = 0xffff;
								  // �洢62���ɹ����͵����
								  g_u32JD62SendSuccInd = l_u32NeedSendSDNum;
								  FSave62ToTDC(2);
							  }							  
							  break;			
				    case 0x68:
					     	  if( l_u16NeedSendTimeNum == g_u16RecOrd68Num )
							  {
							  	  g_u16RecOrd68Num = 0xffff;
								  // �洢68���ɹ����͵����
								  g_u32JD68SendSuccInd = l_u32NeedSendSDNum;
								  FSave68ToTDC(2);
							  }							  
							  break;			
					  default:
					          break;
				} 
			}
			else 				//�ȴ�20s
			{
				g_u8JD6FOrderRev = 0;
			}
		}
		else       // ����ʧ��
		{
		}    
	}
	else if( pSaveID < pSendID )    // ��ʱ���ڱ仯ʱ
	{
		switch(pCmdType)
		{
		    case 0x61:
			     	  g_u32JD61SendSuccInd = g_u32JD61SaveSuccInd;
					  // �洢61���ɹ����͵����
					  FSave61ToTDC(2);						  
					  break;
		    case 0x62:
			     	  g_u32JD62SendSuccInd = g_u32JD62SaveSuccInd;
					  // �洢62���ɹ����͵����
					  FSave62ToTDC(2);						  
					  break;			
		    case 0x68:
			     	  g_u32JD68SendSuccInd = g_u32JD68SaveSuccInd;
					  // �洢68���ɹ����͵����
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

    // ��24ʱ0���ʱ��������ַ����ǰһ��Ĵ������ڼ���
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// �õ���ǰ����
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// �õ���ǰ������
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// ������
	l_u861Data[l_u8Len++] = 0x61;
	//�豸���ʶ����
	for(i=0;i<16;i++)
	{
	    l_u861Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//վ����
	for(i=0;i<15;i++)
	{
	    l_u861Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//�豸Ӳ���������
	l_u861Data[l_u8Len++] = 0x00;
	
	//��
	l_u861Data[l_u8Len++] = l_u16Year;
	l_u861Data[l_u8Len++] = (l_u16Year>>8);
	//��
	l_u861Data[l_u8Len++] = l_u8Month;
	//��
	l_u861Data[l_u8Len++] = l_u8Day;
	// ���ݴ�������
    l_u861Data[l_u8Len++] = g_u8PeriodTOSave;
	//ʱ����� ��λ��ǰ
	l_u861Data[l_u8Len++] = pTimeNum&0xff;
	l_u861Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//������
	l_u861Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// ��������ת���ɹ��공����
		l_u861Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // �������ݷֶ���
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
		    // �������ݶ�
			l_u861Data[l_u8Len++] = l_u8FenDuanBuf[i] + 1;
		    // ��������
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].che_num&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].che_num>>8;
		  	// ��������
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>8;
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>16;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].allche_wt>>24;
			// ������κϼ�
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles&0xFF;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>8;
			l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>16;
		  	l_u861Data[l_u8Len++] = g_asAxleLoadData[ln].axle_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>24;
		}
	 }

	 // ���ͳ�����ݻ���
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

	 // ���ݳ���
	 l_u861Data[510] = (l_u8Len>>8);
	 l_u861Data[511] = l_u8Len;
	 // �õ���ǰSD���洢���
	 g_u32JD61SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD61BaseAdr + g_u32JD61SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u861Data);   
	 if( l_u8ret != 0 )							//дSD������  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is61FirstSave != 1 )     // δ�洢��61��
	 {
		 // ��λ61���洢��־
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
		 // �洢�ѷ��ͳɹ���61�����
		 FSave61ToTDC(2);;
	 }

	if( g_u32JD61SaveSuccInd < g_u32JD61SendSuccInd )    // ��ʱ���ڱ仯ʱ
	{
 	    g_u32JD61SendSuccInd = FGetLastSaveNum(g_u32JD61SaveSuccInd,g_u8PeriodTOSave);  
	    // �洢61���ɹ����͵����
	    FSave61ToTDC(2);
	}

     // �洢��ǰ61�����
	 FSave61ToTDC(1);

	 return l_u32address;	    // ����SD��������ַ
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

    // ��24ʱ0���ʱ��������ַ����ǰһ��Ĵ������ڼ���
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// �õ���ǰ����
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// �õ���ǰ������
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// ������
	l_u862Data[l_u8Len++] = 0x62;
	//�豸���ʶ����
	for(i=0;i<16;i++)
	{
	    l_u862Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//վ����
	for(i=0;i<15;i++)
	{
	    l_u862Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//�豸Ӳ���������
	l_u862Data[l_u8Len++] = 0x00;
	
	//��
	l_u862Data[l_u8Len++] = l_u16Year;
	l_u862Data[l_u8Len++] = (l_u16Year>>8);
	//��
	l_u862Data[l_u8Len++] = l_u8Month;
	//��
	l_u862Data[l_u8Len++] = l_u8Day;
	// ���ݴ�������
    l_u862Data[l_u8Len++] = g_u8PeriodTOSave;
	//ʱ����� ��λ��ǰ
	l_u862Data[l_u8Len++] = pTimeNum&0xff;
	l_u862Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//������
	l_u862Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// ��������ת���ɹ��공����
		l_u862Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // �������ݷֶ���
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
		    // �������ݶ�
			l_u862Data[l_u8Len++] = l_u8FenDuanBuf[i] + 2;
		    // ��������
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].che_num&0xFF;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].che_num>>8;
		  	// ��������
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt&0xFF;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>8;
			l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>16;
		  	l_u862Data[l_u8Len++] = g_asAxleLoadData[ln].axis_num_data[l_u8FenDuanBuf[i]].allche_wt>>24;
		}
	 }

	 // ���ͳ�����ݻ���
     for(i=0;i<4;i++)
	 {
	     for(j=0;j<5;j++)
		 {
		     g_asAxleLoadData[i].axis_num_data[j].che_num = 0;
			 g_asAxleLoadData[i].axis_num_data[j].allche_wt = 0;
		 }
	 }

	 // ���ݳ���
	 l_u862Data[510] = (l_u8Len>>8);
	 l_u862Data[511] = l_u8Len;
	 // �õ���ǰSD���洢���
	 g_u32JD62SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD62BaseAdr + g_u32JD62SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u862Data);   
	 if( l_u8ret != 0 )							//дSD������  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is62FirstSave != 1 )     // δ�洢��61��
	 {
		 // ��λ62���洢��־
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
		 // �洢�ѷ��ͳɹ���62�����
		 FSave62ToTDC(2);;
	 }

	if( g_u32JD62SaveSuccInd < g_u32JD62SendSuccInd )    // ��ʱ���ڱ仯ʱ
	{
 	    g_u32JD62SendSuccInd = FGetLastSaveNum(g_u32JD62SaveSuccInd,g_u8PeriodTOSave);  
	    // �洢62���ɹ����͵����
	    FSave62ToTDC(2);
	}

     // �洢��ǰ62�����
	 FSave62ToTDC(1);

	 return l_u32address;	    // ����SD��������ַ
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

    // ��24ʱ0���ʱ��������ַ����ǰһ��Ĵ������ڼ���
    if( (l_sDevCurTime.u8Hour == 0)&&(l_sDevCurTime.u8Minute == 0) )
	{	   
		FTime_Process(&l_sDevCurTime);
	}
	// �õ���ǰ����
	l_u16Year  = l_sDevCurTime.u16Year;
	l_u8Month  = l_sDevCurTime.u8Month;
	l_u8Day	   = l_sDevCurTime.u8Day;
	// �õ���ǰ������
//	chedao_num = PROMODPARAM.m_u8LaneNum;

	l_u8Len = 0;
	// ������
	l_u868Data[l_u8Len++] = 0x68;
	//�豸���ʶ����
	for(i=0;i<16;i++)
	{
	    l_u868Data[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	//վ����
	for(i=0;i<15;i++)
	{
	    l_u868Data[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];		
	}
	//�豸Ӳ���������
	l_u868Data[l_u8Len++] = 0x00;
	
	//��
	l_u868Data[l_u8Len++] = l_u16Year;
	l_u868Data[l_u8Len++] = (l_u16Year>>8);
	//��
	l_u868Data[l_u8Len++] = l_u8Month;
	//��
	l_u868Data[l_u8Len++] = l_u8Day;
	// ���ݴ�������
    l_u868Data[l_u8Len++] = g_u8PeriodTOSave;
	//ʱ����� ��λ��ǰ
	l_u868Data[l_u8Len++] = pTimeNum&0xff;
	l_u868Data[l_u8Len++] = (pTimeNum>>8)&0xff;
	//������
	l_u868Data[l_u8Len++] = PROMODPARAM.m_u8LaneNum;
	// 

	for(ln=0;ln<PROMODPARAM.m_u8LaneNum;ln++)
	{				 
		// ��������ת���ɹ��공����
		l_u868Data[l_u8Len++] = FChangLaneToBG(ln); 
		
		j=0;
		l_u8FenDuanNum = 0;
        // �������ݷֶ���
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
		    // �������ݶ�
			  l_u868Data[l_u8Len++] = l_u8FenDuanBuf[i] + 1;
			  // ������
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].axis_num>>24;
		  	  // ����������
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].allaxle_wt>>24;
			  // ������κͼ�
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles&0xFF;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>8;
			  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>16;
		  	  l_u868Data[l_u8Len++] = g_asAxleLoadData[ln].axle_weight_data[l_u8FenDuanBuf[i]].acc_equivalent_axles>>24;
		}
	 }

	 // ���ͳ�����ݻ���
     for(i=0;i<4;i++)
	 {
	     for(j=0;j<4;j++)
		 {
		     g_asAxleLoadData[i].axle_weight_data[j].axis_num   = 0;
			 g_asAxleLoadData[i].axle_weight_data[j].allaxle_wt = 0;
			 g_asAxleLoadData[i].axle_weight_data[j].acc_equivalent_axles = 0;
		 }
	 }

	 // ���ݳ���
	 l_u868Data[510] = (l_u8Len>>8);
	 l_u868Data[511] = l_u8Len;
	 // �õ���ǰSD���洢���
	 g_u32JD68SaveSuccInd = FGetSaveNumFromDate(&l_sDevCurTime, pTimeNum);
	 l_u32address = g_u32JD68BaseAdr + g_u32JD68SaveSuccInd;

	 l_u8ret = SD_WriteBlock( &sd_info, l_u32address, l_u868Data);   
	 if( l_u8ret != 0 )							//дSD������  
	 {
//		 SD_Err_Reset_cnt++;
	 }
	 else
	 {
//	     SD_Err_Reset_cnt = 0;
	 }		
	
	 if( g_u8Is68FirstSave != 1 )     // δ�洢��61��
	 {
		 // ��λ68���洢��־
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
		 // �洢�ѷ��ͳɹ���68�����
		 FSave68ToTDC(2);;
	 }

	if( g_u32JD68SaveSuccInd < g_u32JD68SendSuccInd )    // ��ʱ���ڱ仯ʱ
	{
 	    g_u32JD68SendSuccInd = FGetLastSaveNum(g_u32JD68SaveSuccInd,g_u8PeriodTOSave);  
	    // �洢68���ɹ����͵����
	    FSave68ToTDC(2);
	}

     // �洢��ǰ68�����
	 FSave68ToTDC(1);

	 return l_u32address;	    // ����SD��������ַ
}

