/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Task_Laser_Interact.C
** Last modified Date:  20130719
** Last Version:		1.0
** Descriptions:		处理与激光模块交互任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			YANG MING
** Created date:		20130719
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
#define	__TASK_LASER_INTERACT_C

#include "common.h"
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
static uint8 s_u8SendRasPar[300]={0};


void  Task_Laser_Interact(void *tdata)
{ 
	//uint8 err;
	uint8 l_u8TempIndex = 0; 
	uint8 l_u8TempRealIndex = 0;
	uint32 l_u32LaserTime = 0;
	uint32 l_u32laserTime=0;
	uint8 temp_i;

	uint8	l_u8Err;
	uint16	l_u16SendLen;
	uint8   l_u8LaserDaFeiPoint = 0;    // 激光打飞点数

	CComBuf* l_pcComBuf = (CComBuf*)0;

	uint16	l_u16NetSendLen = 0;
	
//	void* l_u32Address[9];
	//ZTINFRECELEMENT l_pZtInf; //逻辑用正向窄条信息

	//TIMER_PIB st_LaserHeartOut;

	

#ifdef ARM_LASER_TEST 
	uint8 l_a_u8ReadPara[15] = {0xFF, 0xFF, 0x00, 0xB1, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

	// 增加激光触发模拟窄条信息，发波形使用，20131127
	uint8			l_u8LaserInfoIx = 0;
	uint8			l_u8IsSaveFlag = 0;				// 是否存储标识
	uint16			l_u16Ix = 0;					// 索引下标

	// test
	//int32	l_u32TempLeft = 0;
	//int32	l_u32TempRight = 0;	


	//初始化
	memset( g_cLaserInfo, 0, sizeof(Channal_Struct)*LASERINFO_LEN );

	//
	tdata = tdata;	
    
	//OSTimeDly(100*OS_TICKS_PER_SEC/1000);	
	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}
	


	while(1)
	{
#ifdef ARM_LASER_TEST
		while(1)
		{
			OSTimeDly(5*OS_TICKS_PER_SEC);
			AddCrc16(&l_a_u8ReadPara[0], 13);
			U5SendBytes(&l_a_u8ReadPara[0], 15);

#if DEBUGINFO_TASK_SP_EN > 0
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Send Laser Paras Succ.\r\n",
					               g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
#endif
			OSSemPend(g_psemLaserInteract, 0, &err);
			if(LASER_READ_PARA_RS == *(g_au8Data+3))
			{
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recv Laser Paras Succ.\r\n",
						               g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
				g_au8Data[3] = 0xB0;
				AddCrc16(&g_au8Data[0], 81);
				U5SendBytes(g_au8Data, g_u8DataLen);
			}
			else
			{
			
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recv Laser Paras Fail.\r\n",
						               g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
			}			
		}
#endif
	    //OSSemPend(g_psemLaserInteract, 15*OS_TICKS_PER_SEC, &err);//心跳包10秒一次，判断超时给出5s的冗余
		l_pcComBuf = (CComBuf*)OSQPend( g_psQLaserInteract, 40*OS_TICKS_PER_SEC, &l_u8Err );

//#if LASER_UNNORMAL_EN > 0
//		if( g_u8LasUnnFlag )
//		{
//			// 异常，不进行操作
//			continue;
//		}
//#endif

		if( l_u8Err == OS_TIMEOUT )		//等待超时,激光模块异常
		{
//#if LASER_UNNORMAL_EN > 0
//			if( g_u8LasUnnFlag )
//			{
//				// 异常，不进行操作
//				continue;
//			}
//			else
//			{
				// 正常，同以前
				if(0 == PROMODPARAM.m_u8TransmitSysEn)	 // 模拟环境下，没有激光状态
					g_u8DeviceERR[0] |= 0x0F;//更新激光状态，1为有故障
				continue;			
//			}
//#else
			// 以前的部分
//			if(0 == PROMODPARAM.m_u8TransmitSysEn)	 // 模拟环境下，没有激光状态
//				g_u8DeviceERR[0] |= 0x0F;//更新激光状态，1为有故障
//			continue;
//#endif
		}
		else
		{
			g_u8DeviceERR[0] = g_u8DeviceERR[0] & 0xF0;// wujinlolng 20121227 注意更改激光更新状态，1为有故障
		}

		memset( g_au8Data, 0, BUFSIZE );
		memcpy( g_au8Data, l_pcComBuf->m_au8DataBuf, l_pcComBuf->m_u16Len );
		g_u8DataLen = l_pcComBuf->m_u16Len;
		
	
#if DEBUGINFO_TASK_SP_EN > 0 && DEBUGINFO_LASER_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recv Laser Cmd %x.\r\n",
					           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
						       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
							   *(g_au8Data+3));
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
#endif

		switch( *(g_au8Data+3) )
		{
			case LASER_SET_PARA_RS://设置激光参数应答
				s_u8SendRasPar[0]=0xFF;
				s_u8SendRasPar[1]=0xFF;
				s_u8SendRasPar[2]=0x00;
				s_u8SendRasPar[3]=LASER_SET_PARA_RS;
				s_u8SendRasPar[4]=0x00;
				s_u8SendRasPar[5]=((g_u8DataLen+9)>>8)&0xFF;
				s_u8SendRasPar[6]=(g_u8DataLen+9)&0xFF;
				memcpy(&(s_u8SendRasPar[7]),g_au8Data,g_u8DataLen);
				AddCrc16(s_u8SendRasPar,g_u8DataLen+7);
// 				l_u16NetSendLen = SendDataNet( SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				l_u16NetSendLen = TcpSendData( SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				FReConnect( l_u16NetSendLen, SOCKET_WT );
				if( g_u8DataLen != l_u16SendLen )//发送激光应答到PC失败
				{
					// 设置激光参数应答 失败
#if DEBUGINFO_TASK_SP_EN > 0
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recd Laser SET_PARA_RS Fail.\r\n",
								           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
									       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
#endif
				}
			    break;

			case LASER_READ_PARA_RS://读取激光参数应答
				memcpy((uint8*)(&g_LaserPara), &(g_au8Data[5]), sizeof(LaserSetupParam));
				s_u8SendRasPar[0]=0xFF;
				s_u8SendRasPar[1]=0xFF;
				s_u8SendRasPar[2]=0x00;
				s_u8SendRasPar[3]=LASER_READ_PARA_RS;
				s_u8SendRasPar[4]=0x00;
				s_u8SendRasPar[5]=((g_u8DataLen+9)>>8)&0xFF;
				s_u8SendRasPar[6]=(g_u8DataLen+9)&0xFF;
				memcpy(&(s_u8SendRasPar[7]),g_au8Data,g_u8DataLen);
				AddCrc16(s_u8SendRasPar,g_u8DataLen+7);
// 				l_u16NetSendLen = SendDataNet(SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				l_u16NetSendLen = TcpSendData(SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				FReConnect( l_u16NetSendLen, SOCKET_WT );
				if(g_u8DataLen !=l_u16SendLen )//发送激光应答到PC失败
				{
					// 读取激光参数应答 失败added by wujinlong 20130731
#if DEBUGINFO_TASK_SP_EN > 0
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recd Laser READ_PARA_RS Fail.\r\n",
							           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
#endif
				}
			    break;

			case LASER_RESET_RS://激光复位应答
				s_u8SendRasPar[0]=0xFF;
				s_u8SendRasPar[1]=0xFF;
				s_u8SendRasPar[2]=0x00;
				s_u8SendRasPar[3]=LASER_RESET_RS;
				s_u8SendRasPar[4]=0x00;
				s_u8SendRasPar[5]=((g_u8DataLen+9)>>8)&0xFF;
				s_u8SendRasPar[6]=(g_u8DataLen+9)&0xFF;
				memcpy(&(s_u8SendRasPar[7]),g_au8Data,g_u8DataLen);
				AddCrc16(s_u8SendRasPar,g_u8DataLen+7);
// 				l_u16NetSendLen = SendDataNet(SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				l_u16NetSendLen = TcpSendData(SOCKET_WT, s_u8SendRasPar, g_u8DataLen+9);
				FReConnect( l_u16NetSendLen, SOCKET_WT );
				if(g_u8DataLen != l_u16SendLen)//发送激光应答到PC失败
				{
					// 激光复位应答 失败added by wujinlong 20130731
#if DEBUGINFO_TASK_SP_EN > 0
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recd Laser RESET_RS Fail.\r\n",
								           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
									       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
#endif
				}
			    break;

			case LASER_VEHICLE_POS://车辆位置信息
//				if( !g_u8IsLaserorCoilFlag )
//				{
//					break;
//				}
				
				
				l_u8IsSaveFlag = 0;
				//
				g_RasterData[g_u8RasterDataWriteIndex].u8Cnt = *(g_au8Data+OFFSET);
				l_u8LaserDaFeiPoint = (*(g_au8Data+65));
				//memcpy( (uint8*)g_RasterData[g_u8RasterDataWriteIndex].headinfo, m_au8Data+OFFSET+1, LASTERCOUNT*2*2*5 );
				if(g_RasterData[g_u8RasterDataWriteIndex].u8Cnt > SP_LASER_VEHNUM_MAX)
				{
					g_RasterData[g_u8RasterDataWriteIndex].u8Cnt = SP_LASER_VEHNUM_MAX;
				}
				l_u8TempIndex = 0;
				for( l_u8TempRealIndex=0; l_u8TempRealIndex < SP_LASER_VEHNUM_MAX; l_u8TempRealIndex++ )
				{
					if( SUB( (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7+1))*10, (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7+2))*10 ) < LASER_VEH_MIN_ITV )
					{
						if((*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 0) & 0x0c) != 0)
						{
							g_RasterData[g_u8RasterDataWriteIndex].u8Cnt--;   // 有效车辆数
						}
						continue;
					}
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type 			=  *(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 0);
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos 		= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 1)) * 10;
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos 		= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 2)) * 10;
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehLane 		= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 3));
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehWide 	= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 4)) * 10;
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehHeight 	= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 5)) * 10;
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID		= (*(g_au8Data+OFFSET+1+l_u8TempRealIndex*7 + 6));
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehCalWide  = SUB(g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos, g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos);
					if(g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID == 0)
					{
						g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID = 1;
					}

					/* add by sxh 2014.3.29有激光位置数据后，判断是否为上秤或下称数据，是则保存发送*/
					if ( 0x04 == (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type & 0x0C) )  // 上秤
					{
						g_RasterData[g_u8RasterDataWriteIndex].VehChangeFlag = 1;
						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u8Flag = 0x02;	 //激光IO数据
                       
					    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u8type =	
						                                               g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type ;	//事件类型				

						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u16Ltpos =  
							                                           g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos;	//左位置
						
						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u16Rtpos =  
							                                           g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos;	 //右位置

						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u32Tm = l_u32LaserTime;
						g_CoilOrLaserData.m_u8WriteIn = (g_CoilOrLaserData.m_u8WriteIn+1) % COILGPIODATALEN; 
						
					} 
					else if ( 0x08 == (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type & 0x0C) )  // 下秤
					{
						if((l_u8LaserDaFeiPoint > 20))
						{
							if(g_u8RasterNoDaFeiCnt<4)
							{
								g_u8RasterNoDaFeiCnt = 0;
							}
							if(g_u8RasterDaFeiCnt < 4)
							{
								g_u8RasterDaFeiCnt++;
								if(g_u8RasterDaFeiCnt == 4 && g_RasterData[g_u8RasterDataWriteIndex].u8Cnt == 1)
								{
									g_u8RasterNoDaFeiCnt = 0;
								}
								else if(g_u8RasterDaFeiCnt == 4)
								{
									g_u8RasterDaFeiCnt = 3;
								}
							}	
						}
						else if(l_u8LaserDaFeiPoint <= 20)
						{
							if(g_u8RasterDaFeiCnt<4)
							{
								g_u8RasterDaFeiCnt = 0;
							}
							if(g_u8RasterNoDaFeiCnt < 4)
							{
								g_u8RasterNoDaFeiCnt++;
								if(g_u8RasterNoDaFeiCnt == 4 && g_RasterData[g_u8RasterDataWriteIndex].u8Cnt == 1)
								{
									g_u8RasterDaFeiCnt = 0;
								}
								else if(g_u8RasterNoDaFeiCnt == 4)
								{
									g_u8RasterDaFeiCnt = 3;
								}
							}
						}
						g_RasterData[g_u8RasterDataWriteIndex].VehChangeFlag = 1;
						
						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u8Flag = 0x02;	 //激光IO数据
                       
					    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u8type =	
						                                               g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type;  //时间类型					

						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u16Ltpos =  
							                                           g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos;  //左位置
						
						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].vehPositionInfo[l_u8TempIndex].m_u16Rtpos =  
							                                           g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos;	  //右位置

// 						laserWave[5]=((g_RasterData[g_u8RasterDataWriteIndex].u8Cnt*6+14)>>8)&0xff;
// 						laserWave[6]=(g_RasterData[g_u8RasterDataWriteIndex].u8Cnt*6+14)&0xff;
// 						ReadFPGAGlobalTime(l_u32laserTime);
// 						laserWave[7]=(l_u32laserTime& 0xff000000)>>24;
// 						laserWave[8]=(l_u32laserTime& 0x00ff0000)>>16;
// 						laserWave[9]=(l_u32laserTime& 0x0000ff00)>>8;
// 						laserWave[10]=l_u32laserTime& 0xff;
// 						laserWave[11]=g_RasterData[g_u8RasterDataWriteIndex].u8Cnt;
// 						for(temp_i=0;temp_i<g_RasterData[g_u8RasterDataWriteIndex].u8Cnt;temp_i++)
// 						{
// 							laserWave[12+6*temp_i]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].m_u8VehID;
// 							laserWave[12+6*temp_i+1]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u8type;
// 							laserWave[12+6*temp_i+2]=(g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16Ltpos>>8)&0xff;
// 							laserWave[12+6*temp_i+3]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16Ltpos&0xff;
// 							laserWave[12+6*temp_i+4]=(g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16rtpos>>8)&0xff;
// 							laserWave[12+6*temp_i+5]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16rtpos&0xff;
// 						}
// 						AddCrc16(laserWave,12+6*g_RasterData[g_u8RasterDataWriteIndex].u8Cnt);
// 						SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
						
						g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u32Tm = l_u32LaserTime;
						g_CoilOrLaserData.m_u8WriteIn = (g_CoilOrLaserData.m_u8WriteIn+1) % COILGPIODATALEN; 

					}
					/* add by sxh 2014.3.29有激光位置数据后，判断是否为上秤或下称数据，是则保存发送*/







//#if	SENDWAVEENABLE > 0			
					// 20131127
					if( 1 == g_Setup.u8SendWaveEnable )
					{
						if( 0x04==((g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type) & 0x0C) 
							|| 0x08==((g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type) & 0x0C) )
						{						
							l_u8IsSaveFlag = 1;
						}
					}
//#endif

#if DEBUGINFO_TASK_SP_EN > 0
					if( 0x04 == ((g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type) & 0x0C) )
					{
						g_u8LaserPos[g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID][0] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos/10;
						g_u8LaserPos[g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID][1] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos/10;
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u LS Tg ID=%d,L=%d,R=%d,W=%d,CW=%d,H=%d,N=%d.\r\n",
										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehWide,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehCalWide,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehHeight,
											   g_RasterData[g_u8RasterDataWriteIndex].u8Cnt);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						///////////////////////////////
						g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
						g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 22;				
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x0A;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 22;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos>>8)&0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos &0xFF);
						
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos>>8)&0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos &0xFF);
			
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][29] = 0x00;
						
						AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
						OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
						g_u8CurNetDebugIndex++;
						g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
						//////////////////////////////
						OSSemPost(g_pSemSendThtouSP);
					}
					if( 0x08 == ((g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type) & 0x0C) )
					{
						g_u8LaserPos[g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID][2] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos/10;
						g_u8LaserPos[g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID][3] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos/10;
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u LS End ID=%d,L=%d,R=%d,W=%d,CW=%d,H=%d,N=%d,D=%d.\r\n",
										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehID,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehWide,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehCalWide,
											   g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehHeight,
											   g_RasterData[g_u8RasterDataWriteIndex].u8Cnt,l_u8LaserDaFeiPoint);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						///////////////////////////////
						g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
						g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 22;				
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x0B;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 22;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos>>8)&0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos &0xFF);
						
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos>>8)&0xFF;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = (g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos &0xFF);
			
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = 0x00;
						g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][29] = 0x00;
						
						AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
						OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
						g_u8CurNetDebugIndex++;
						g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
						//////////////////////////////
						OSSemPost(g_pSemSendThtouSP);
					}
#endif
					l_u8TempIndex++;
				}
				

//#if	SENDWAVEENABLE > 0	
				ReadFPGAGlobalTime(l_u32laserTime);
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					if( l_u8IsSaveFlag )
					{
						if(g_StartWaveFlag==0)
						{
							g_StartWaveFlag=1;
							TcpSendData( SOCKET_WAVE,startWave,12);
						}
						laserWave[5]=((g_RasterData[g_u8RasterDataWriteIndex].u8Cnt*6+14)>>8)&0xff;
						laserWave[6]=(g_RasterData[g_u8RasterDataWriteIndex].u8Cnt*6+14)&0xff;
						laserWave[7]=(l_u32laserTime& 0xff000000)>>24;
						laserWave[8]=(l_u32laserTime& 0x00ff0000)>>16;
						laserWave[9]=(l_u32laserTime& 0x0000ff00)>>8;
						laserWave[10]=l_u32laserTime& 0xff;
						laserWave[11]=g_RasterData[g_u8RasterDataWriteIndex].u8Cnt;
						for(temp_i=0;temp_i<g_RasterData[g_u8RasterDataWriteIndex].u8Cnt;temp_i++)
						{
							laserWave[12+6*temp_i]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].m_u8VehID;
							laserWave[12+6*temp_i+1]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u8type;
							laserWave[12+6*temp_i+2]=(g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16Ltpos>>8)&0xff;
							laserWave[12+6*temp_i+3]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16Ltpos&0xff;
							laserWave[12+6*temp_i+4]=(g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16rtpos>>8)&0xff;
							laserWave[12+6*temp_i+5]=g_RasterData[g_u8RasterDataWriteIndex].headinfo[temp_i].u16rtpos&0xff;
						}
						AddCrc16(laserWave,12+6*g_RasterData[g_u8RasterDataWriteIndex].u8Cnt);
// 						SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
						TcpSendData( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
						g_u8RasterRead=g_u8RasterDataWriteIndex;
					}
				}					
				// 20131127
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					if( l_u8IsSaveFlag )
					{
						memset( &g_cLaserInfo[l_u8LaserInfoIx], 0, sizeof(Channal_Struct) );
						if( 1 == g_Setup.u8SendWaveEnable )
						{
							g_cLaserInfo[l_u8LaserInfoIx].u8Id = 0xAA;
							g_cLaserInfo[l_u8LaserInfoIx].u16ptCnt = 6*SP_LASER_VEHNUM_MAX;
							// 转换成16位数据
							l_u16Ix = 0;
							for( l_u8TempIndex=0; l_u8TempIndex<SP_LASER_VEHNUM_MAX; l_u8TempIndex++ )
							{
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type;
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos;
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos;
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u8VehLane;
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehWide;
								g_cLaserInfo[l_u8LaserInfoIx].u16data[0][l_u16Ix++] = g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].m_u16VehHeight;
	
							}
							
							//OSQPost( g_pQWaveMsg,&g_cLaserInfo[l_u8LaserInfoIx] );
						}
	
						//
						l_u8LaserInfoIx = (l_u8LaserInfoIx+1) % LASERINFO_LEN;
					}
				}
//#endif

				// test
//				if( 0x04 == g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u8type )
//				{
//					l_u32TempLeft = g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos - 100;
//					if( l_u32TempLeft <= 10 )
//					{
//						l_u32TempLeft = 10;
//					}
//					l_u32TempRight = g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16rtpos;	
//				}				
//
//				//g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos = 1 * 10;
//				//g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16rtpos = 31 * 10;
//				g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos = l_u32TempLeft;
//				g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16rtpos = l_u32TempRight;		

				// 从FPGA读当前时间
				g_RasterData[g_u8RasterDataWriteIndex].u32tm = l_u32LaserTime;
				
				//// 车辆斜行判断(liumingxin)(20170720) ////
				if((g_u32XieZouEnable == 1) && ((g_u32LaserIOUseFlag == 123) || (g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].m_u16VehHeight) > 220))
				{
					if(0x04 == ((g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u8type) & 0x0C))
					{
						if(g_RasterData[g_u8RasterDataWriteIndex].u8Cnt == 1) 
						{
							/******************************函数形参赋值***********************/
// 							l_u32Address[0]=&g_asVehiclePerChannelForLoop[0][0];
// 							l_u32Address[1]=&Forward_ZTColum[0][0];
// 							l_u32Address[2]=&Reverse_ZTColum[0][0];
// 							l_u32Address[3]=&g_WonderZT[0][0];
							
//							FVehXieZouStatus(l_u32Address);    // 判断斜行
						}
					}
					else if(0x08 == ((g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u8type) & 0x0C)
							&& (g_RasterData[g_u8RasterDataWriteIndex].u8Cnt == 1))
					{
						for(temp_i=0; temp_i< PROMODPARAM.m_u8LaneNum*2; temp_i++)
						{
							if(g_asVehiclePerChannelForLoop[temp_i][0].m_n8XieZouFlag > 1)
							{
								g_asVehiclePerChannelForLoop[temp_i][0].m_u32EndTime = l_u32laserTime;
							}
						}
					}	
				}
				//// end(liumingxin)(20170720) ////
				
				g_u8RasterDataWriteIndex++;
				
// 				if(g_u8RasterDataWriteIndex==256)
// 					WFlag=1;
				memset( &g_RasterData[g_u8RasterDataWriteIndex], 0, sizeof(Raster_Struct));

				// 车辆位置信息added by wujinlong 20130731
#if DEBUGINFO_TASK_SP_EN > 0 && DEBUGINFO_LASER_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recd Laser POS Succ。\r\n",
						           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif

				break;

			case LASER_STATE_CC:	 //更新激光状态
				g_u8DeviceERR[0] = g_u8DeviceERR[0] & 0xF0;// wujinlolng 20121227 注意更改激光更新状态，1为有故障
				// 1故障，0正常
				*(g_au8Data+6) ? (g_u8DeviceERR[0] |= (1   )) : (g_u8DeviceERR[0] &=~(1   ));
				*(g_au8Data+7) ? (g_u8DeviceERR[0] |= (1<<1)) : (g_u8DeviceERR[0] &=~(1<<1));
				*(g_au8Data+8) ? (g_u8DeviceERR[0] |= (1<<2)) : (g_u8DeviceERR[0] &=~(1<<2));
				*(g_au8Data+9) ? (g_u8DeviceERR[0] |= (1<<3)) : (g_u8DeviceERR[0] &=~(1<<3));
				
				  					
#if DEBUGINFO_TASK_SP_EN > 0 && DEBUGINFO_LASER_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Recd Laser STATE Succ。\r\n",
							           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
				break;

			default:
				break;
		}	     
	}
}


