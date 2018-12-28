/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskProtocol.C
** Last modified Date:  20110523
** Last Version:		1.0
** Descriptions:		上位机通信任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110523
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
#define	__TASKPROTOCOL_C

#include "common.h"

#define		SETUPALIAS			g_Setup			//设置参数结构
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
#define		CMDTOSENDBUFSIZE	4

static		uint8	m_au8CommandToSend[CMDTOSENDBUFSIZE];	//待发送命令
static		uint8	m_u8CTSReadIndex;
static		uint8	m_u8CTSWriteIndex;

//#if	YBVERSION >= 30		//3.0仪表功能
//static		void	GetSendBufByVRInfo(uint8 * p_pu8BufAxle, uint8 * p_pu8BufAxleGrp);
//#endif
/*********************************************************************************************************
** Function name:           GetCmdInBuf
**
** Descriptions:            获取命令缓存中的命令
**
** input parameters:        p_u8Type，获取类型，1：取出；2：查看
** output parameters:       None
** Returned value:          命令号
**
** Created by:              ZHANG Ye
** Created Date:            20110523
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 GetCmdInBuf(uint8 p_u8Type)
{
	uint8	l_u8TmpCmd	= 0xff;
	if (m_u8CTSReadIndex != m_u8CTSWriteIndex)	//有命令待发
	{
		l_u8TmpCmd	= m_au8CommandToSend[m_u8CTSReadIndex];
		if (p_u8Type == 1)
		{
			m_au8CommandToSend[m_u8CTSReadIndex]	= 0xff;
			m_u8CTSReadIndex	++;		  
			m_u8CTSReadIndex	%= CMDTOSENDBUFSIZE; 	
		}
	}
	return	l_u8TmpCmd;	
}

/*********************************************************************************************************
** Function name:           AddCmdToSend
**
** Descriptions:            设置待发送命令
**
** input parameters:        p_u8Cmd	插入命令号
** output parameters:       None
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20110523
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void AddCmdToSend(uint8 p_u8Cmd)
//{
//	m_au8CommandToSend[m_u8CTSWriteIndex]	= p_u8Cmd;
//	m_u8CTSWriteIndex	++;
//	m_u8CTSWriteIndex	%= CMDTOSENDBUFSIZE; 	
//}							

/*********************************************************************************************************
** Function name:           ToProtocolBuff
**
** Descriptions:            按照协议发送数据
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20110523
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ToProtocolBuff(Vehicle_Struct *pdata)
{
	if((PROMODPARAM.m_u8EnableFlag[2]) && FSDCheckAntiShake(&sd_info))
	{
		FSave39ToSD(pdata);
	}
	switch(g_Setup.u8Protocol)
	{
		case PROTOL_WJ_OLD:
			ToBuff1(pdata);
			break;
		
		case PROTOL_WJ_NEW:
			if((g_u8BiaoDEN == 1) && (g_u8BiaoDLane == pdata->u8psLane))			
			{
				ToBuff0ForBD(pdata);
			}
			else
			{
				ToBuff0(pdata);
				Tobuff_preWeigh(pdata);
			}
			break;	
		
		case PROTOL_FXC:
			ToBuff_FXC(pdata);
			Tobuff_preWeigh(pdata);
			break;
				
		default:
			break;
	}
}

/*********************************************************************************************************
** Function name:           FSendCmd2PC_ZONG
**
** Descriptions:            按照协议发送数据
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              wujinlong
** Created Date:            20140709
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FSendCmd2PC_ZONG(uint8 p_u8VehIndex,uint8 p_u8CommandNO)
{
	switch(g_Setup.u8Protocol)
	{
		case PROTOL_WJ_OLD:

			break;
		
		case PROTOL_WJ_NEW:
			SendCommandToPC0(p_u8VehIndex,p_u8CommandNO);
			break;	
		
		case PROTOL_FXC:
			SendCommandToPC_FXC(p_u8VehIndex,p_u8CommandNO);
			break;
				
		default:
			break;
	}
}

/*********************************************************************************************************
** Function name:           FProcessCmd_ZONG
**
** Descriptions:            按照协议发送数据
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              wujinlong
** Created Date:            20140709
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FProcessCmd_ZONG(uint8 *pData)
{
	if((pData[0] == 0xAA) && (pData[1] == 0xAA))
	{
		WJJD_ProcessCmd(pData);	
	}
	else
	{
		switch(g_Setup.u8Protocol)
		{
			case PROTOL_WJ_OLD:
				if(PROMODPARAM.m_u8EnableFlag[3] == 1)
				{
					WJ_ProcessCmd(pData);
				}
				break;
			
			case PROTOL_WJ_NEW:
				WJ_ProcessCmd(pData);
				break;	
			
			case PROTOL_FXC:
				ProcessCmd_FXC(pData);
				if(PROMODPARAM.m_u8EnableFlag[3] == 1)
				{
					WJ_ProcessCmd(pData);
				}
				break;
					
			default:
				break;
		}
		
	}
}

/*********************************************************************************************************
** Function name:           ExecutePCCommand
**
** Descriptions:            向PC发送指令
**
** input parameters:	uint8	p_u8Datalen	接收数据长度
						uint8	p_u8Cmd		接收数据命令
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20111122
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ExecutePCCommand(uint8 p_u8Datalen,uint8 p_u8Cmd)
{

	switch(SETUPALIAS.u8Protocol)
	{
	default:
		break;
	}
}

/*********************************************************************************************************
** Function name:           CommondToPC
**
** Descriptions:            向PC发送指令
**
** input parameters:        p_u8VehIndex	车辆号
** 							p_u8CommandNO	命令号
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20110523
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void CommondToPC(uint8 p_u8VehIndex,uint8 p_u8CommandNO)
{

	switch(SETUPALIAS.u8Protocol)
	{
	default:
		break;
	}
}

/*********************************************************************************************************
** Function name:           GetSendBufByVRInfo
**
** Descriptions:            通过过车信息分析得到待发送的字符数组
**
** input parameters:        uint8 *	p_pu8BufAxle: 轴重缓存
**							uint8 * p_pu8BufAxleGrp: 轴组重缓存
** output parameters:       none
** Returned value:          none
**
** Created by:              ZHANG Ye
** Created Date:            20110704
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void GetSendBufByVRInfo(uint8 * p_pu8BufAxle, uint8 * p_pu8BufAxleGrp)
//{
//	switch(SETUPALIAS.u8Protocol)
//	{  				   
//	default:
//		break;
//	}
//}  

/*********************************************************************************************************
** Function name:           TaskRec7
**
** Descriptions:            任务7，负责与上位机通信
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              Wang ZiFeng
** Created Date:            20121031
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  TaskRec7(void *tdata)
{  
	uint16  CmdCnt = 0;//,ret = 0;
	//uint16 FIFO_count=0;
	//uint16 i;
	uint8	index = 0;
//	uint8 ZTCount=0;
	uint8	ret;
	uint32  sndCnt = 0;
	sndFrame_Struct *pItem = (sndFrame_Struct *)0;
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	//uint16  l_timeBreak  = 0;
	
	uint32 sndCnt_tmp = 0;     // 用于波形数据包发送失败后重发liumingxin

#if OS_Q_QUERY_EN > 0
sndFrame_Struct *pWaveItem = (sndFrame_Struct *)0;
OS_Q_DATA data;
#endif 



	tdata=tdata;
	
	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}


#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Recd TaskBegin [Communicate].\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif

// Log

			   
	m_u8CTSReadIndex	= 0;
	m_u8CTSWriteIndex	= 0;


	while(1)
	{
		//OSTimeDly(4); 
		//OSTimeDly(10);	  
		OSTimeDly(20*OS_TICKS_PER_SEC/1000);

		//SendDeviceStatus0();	//定时发送信息
		// 心跳
		switch(SETUPALIAS.u8Protocol)
		{			   				   
			case PROTOL_WJ_NEW:
//				SendDeviceStatus0();	//定时发送信息
				FSendHeartbeat();
				FJudgeUpdate();
				break;

			case PROTOL_FXC:
				FSendHeartbeat_FXC();
				FJudgeUpdate_FXC();
				break;
	
			default:
				break;
		}


#if	TASKTIMEENABLE
		if(1==TaskRunTimeStatistics.m_ulTaskProtocolFlag ){//1:开始计数状态
			OSSchedLock();
			if(0x01!=T1TCR){
				T1TCR=0x02;
				T1TCR=0x01;
			}			
			TaskRunTimeStatistics.m_ulTaskProtocolFlag=2;   //切换到结算状态
			TaskRunTimeStatistics.m_ulTaskProtocolDet=0;
			
			TaskRunTimeStatistics.m_ulTaskProtocol1=T1TC;
			OSSchedUnlock();				
		}
		else if(2==TaskRunTimeStatistics.m_ulTaskProtocolFlag){
		    OSSchedLock();
			TaskRunTimeStatistics.m_ulTaskProtocol1=T1TC; //结算状态中更新
			OSSchedUnlock();
		}
#endif

		//接收数据 	
		if(g_u16NetADcount || (g_u16UART5SavIndex != g_u16UART5ReadIndex))		
		{	
			if(g_u16NetADcount)
			{
				OS_ENTER_CRITICAL();	
				CmdCnt = g_u16NetADcount;
				g_u16NetADcount = 0;
				OS_EXIT_CRITICAL();
				while(CmdCnt)
			   	{
				   	if((g_anfSPIAD[g_u8NetADOut][0]==0xFF) && (g_anfSPIAD[g_u8NetADOut][1]==0xFF) && (g_anfSPIAD[g_u8NetADOut][3]==ADJ_SUPERCOMD))
					{
						WJ_ProcessCmd(g_anfSPIAD[g_u8NetADOut]);
					}
					else if(PROMODPARAM.m_u8UploadDataMode == 1)
					{
						FProcessCmd_ZONG(g_anfSPIAD[g_u8NetADOut]);
				  	}
				   	CmdCnt--;
				 	g_u8NetADOut++;
					g_u8NetADOut %= NETDATA_BUFMASK;
			   	}
			}
			else
			{
				while(g_u16UART5SavIndex != g_u16UART5ReadIndex)
			   	{
				   	if((g_u8UART5RcvBuf[g_u16UART5ReadIndex][0]==0xFF) && (g_u8UART5RcvBuf[g_u16UART5ReadIndex][1]==0xFF) && (g_u8UART5RcvBuf[g_u16UART5ReadIndex][3]==ADJ_SUPERCOMD))
					{
						WJ_ProcessCmd(g_u8UART5RcvBuf[g_u16UART5ReadIndex]);
					}
					else if((PROMODPARAM.m_u8UploadDataMode == 2) || (PROMODPARAM.m_u8UploadDataMode == 3) || (PROMODPARAM.m_u8UploadDataMode == 4)
					|| (PROMODPARAM.m_u8EnableFlag[3] == 1) || (PROMODPARAM.m_u8EnableFlag[2] != 0))
					{
						FProcessCmd_ZONG(g_u8UART5RcvBuf[g_u16UART5ReadIndex]);
					}
					g_u16UART5ReadIndex++;
					g_u16UART5ReadIndex %= U5DATALEN;
				}
			}
		
		}

		//发送数据
		pItem = (sndFrame_Struct *)OSQAccept(g_pQSendPC);	
		if(pItem != (sndFrame_Struct *)0)
		{  		
			index = 0;
			do
			{
				if((PROMODPARAM.m_u8UploadDataMode == 2) || (PROMODPARAM.m_u8UploadDataMode == 3))
				{
					U5SendBytes(pItem->au8data[index], pItem->au32len[index]);

					OSTimeDly(1);

					if((pItem->au8data[index][0]==0xFF) && (pItem->au8data[index][1]==0xFF) && (pItem->au8data[index][3]==ADJ_SUPERCOMD))
					{
						U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

						OSTimeDly(1);

						sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
						if(sndCnt != 0)
						{
							g_u8IsConnectFlag = 1;
						}
						FReConnect( sndCnt, SOCKET_WT );
		
						OSTimeDly(1);

					}
				}
				else if(PROMODPARAM.m_u8UploadDataMode == 4)
				{
					U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

					OSTimeDly(1);

					if((pItem->au8data[index][0]==0xFF) && (pItem->au8data[index][1]==0xFF) && (pItem->au8data[index][3]==ADJ_SUPERCOMD))
					{
						U5SendBytes(pItem->au8data[index], pItem->au32len[index]);

						OSTimeDly(1);

						sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
						if(sndCnt != 0)
						{
							g_u8IsConnectFlag = 1;
						}
						FReConnect( sndCnt, SOCKET_WT );
		
						OSTimeDly(1);

					}
				}
				else if(PROMODPARAM.m_u8UploadDataMode == 1)
				{
					sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					if(sndCnt != 0)
					{
						g_u8IsConnectFlag = 1;
					}
					FReConnect( sndCnt, SOCKET_WT );
	
					OSTimeDly(1);

					if((pItem->au8data[index][0]==0xFF) && (pItem->au8data[index][1]==0xFF) && (pItem->au8data[index][3]==ADJ_SUPERCOMD))
					{
						U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

						OSTimeDly(1);

						U5SendBytes(pItem->au8data[index], pItem->au32len[index]);

						OSTimeDly(1);
					}
					//OSTimeDly(5);
					if( (0==sndCnt)
						|| (sndCnt!=pItem->au32len[index]) )
					{
#if DEBUGINFO_TASK_SP_EN > 0
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5000 sndCnt=%x Framelen=%d。\r\n",
										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
											   sndCnt, pItem->au32len[index]);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
#endif
					}
//					if(0 == PROMODPARAM.m_u8EnableFlag[2])
//					{

						sndCnt = TcpSendData( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
						
						if(sndCnt != 0)
						{
							g_u8IsConnectFlag = 1;
						}
						FReConnect( sndCnt, SOCKET_RES );
						OSTimeDly(1);
						if( (0==sndCnt)
							|| (sndCnt!=pItem->au32len[index]) )
						{
// 	#if DEBUGINFO_TASK_SP_EN > 0
// 							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5002 sndCnt=%x Framelen=%d。\r\n",
// 											       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
// 												   sndCnt, pItem->au32len[index]);
// 							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 							g_u16CurDebugInd++;
// 							g_u16CurDebugInd %= COM_NUM;
// 							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 							OSSemPost(g_pSemSendThtouSP);
// 	#endif
						}
//					}

				}
				if(PROMODPARAM.m_u8EnableFlag[3] == 1)
				{
					U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

					OSTimeDly(1);
				}	 
				
				index++;	
			}while(index < pItem->u8Cnt); 
			

			// 发送重量帧added by wujinlong 20130805
#if DEBUGINFO_TASK_SP_EN > 0
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp cmd=%x FrameNum=%d. \r\n",
// 							       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
// 							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
// 								   pItem->au8data[0][3], pItem->u8Cnt);
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
#endif
	
		} 

#if OS_Q_QUERY_EN > 0  //使能OS_Q_QUERY_EN为1 wzf
	 	/****************************************
	  	*当车辆数据队列为空时，开始发送波形数据，
	  	*否则不发送波形数据.车辆数据队列优先发送wzf 
	  	****************************************/
		ret = OSQQuery(g_pQSendPC,&data);  
	 	if(OS_NO_ERR == ret)
		{
			if(data.OSNMsgs == 0)
			{
				pWaveItem = (sndFrame_Struct *)OSQAccept(g_pQNetWavePC);
#if SD_WAVE_SAVE_EN > 0
				OSQPost( g_pQSDWaveWrite, pWaveItem );
#endif
				if(pWaveItem != (sndFrame_Struct *)0)
				{  		
					index = 0;
					if( (pWaveItem->au8data[index][3]==0x8F) && ((pWaveItem->au8data[index][4]==0x02) || (pWaveItem->au8data[index][4]==0x01)))
					{
					#if DEBUGINFO_TASK_SP_EN > 0
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"$$$$$$$$$$$$$$8F=%d.\r\n",
									           pWaveItem->au8data[index][4]);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
					#endif
					}

					do
					{
// 						sndCnt = SendDataNet( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //发送的数据帧长度必须小于1460
						sndCnt = TcpSendData( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //发送的数据帧长度必须小于1460
						if(sndCnt != 0)
						{
							g_u8IsConnectFlag = 1;
						}
						FReConnect( sndCnt, SOCKET_WAVE );	  //SOCKET_WAVE
						OSTimeDly(1);
						
						/************发送失败重新发送一次该包数据(liumingxin)***********/
						if(sndCnt == 0)    // lmx
						{
// 							sndCnt_tmp = SendDataNet( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //发送的数据帧长度必须小于1460
							sndCnt_tmp = TcpSendData( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //发送的数据帧长度必须小于1460
							if(sndCnt_tmp != 0)
							{
								g_u8IsConnectFlag = 1;    // 网络连接正常与否，1连接正常，0连接不正常，默认是1(lmx)
							}
							FReConnect( sndCnt_tmp, SOCKET_WAVE );	  //SOCKET_WAVE
							OSTimeDly(1);
						}
						//printf("i:%d, F:%d, Z:%d, s:%d, st:%d\r\n", index, pWaveItem->au8data[index][9], pWaveItem->au8data[index][12], sndCnt, sndCnt_tmp);    // lmx
						if(sndCnt_tmp != 0)    // lmx
						{
							sndCnt_tmp = 0;           
						}
						/************end(liumingxin)***********/
						index++;	
					}while(index < pWaveItem->u8Cnt);

					// 发送波形帧added by wujinlong 20130805
//#if DEBUGINFO_TASK_SP_EN > 0
//					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp Wave FrameNum=%d。\r\n",
//									       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//									       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//										   pWaveItem->u8Cnt); 
//					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//					g_u16CurDebugInd++;
//					g_u16CurDebugInd %= COM_NUM;
//					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//					OSSemPost(g_pSemSendThtouSP);
//#endif

		
				} 
			}
		} 
#endif
	   pItem = (sndFrame_Struct *)OSQAccept(g_pQJDSend); //改为发送车辆窄条数据
	   if(pItem != (sndFrame_Struct *)0)
	   {
	   		index = 0;
			do
			{
				//JDSYSPARAM.m_u8TransMod = 2;
				//if(JDSYSPARAM.m_u8TransMod == 2)
				{
// 					U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

// 					OSTimeDly(1);
// 					sndCnt = SendDataNet( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					sndCnt = TcpSendData( SOCKET_SVI, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					if(sndCnt != 0)
					{
						g_u8IsConnectFlag = 1;
					}
					FReConnect( sndCnt, SOCKET_SVI );
	
					OSTimeDly(1);
				}
				//else if(JDSYSPARAM.m_u8TransMod == 3)
				if(JDSYSPARAM.m_u8TransMod == 3)
				{
					U5SendBytes(pItem->au8data[index], pItem->au32len[index]);

					OSTimeDly(1);
				}
				else if(JDSYSPARAM.m_u8TransMod == 1)
				{
// 					sndCnt = SendDataNet( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					sndCnt = TcpSendData( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					if(sndCnt != 0)
					{
						g_u8IsConnectFlag = 1;
					}
					FReConnect( sndCnt, SOCKET_RES );
	
					OSTimeDly(1);
					//OSTimeDly(5);
					if( (0==sndCnt)
						|| (sndCnt!=pItem->au32len[index]) )
					{
#if DEBUGINFO_TASK_SP_EN > 0
// 						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5002 sndCnt=%x Framelen=%d。\r\n",
// 										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
// 											   sndCnt, pItem->au32len[index]);
// 						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 						g_u16CurDebugInd++;
// 						g_u16CurDebugInd %= COM_NUM;
// 						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 						OSSemPost(g_pSemSendThtouSP);
#endif
					}
				}	 
				index++;	
			}while(index < pItem->u8Cnt);
	   }
	   pItem = (sndFrame_Struct *)OSQAccept(g_pQSendPCDebug);
	   if(pItem != (sndFrame_Struct *)0)
	   {
	   		index = 0;
			do
			{
				
				if(0 == g_Setup.u8SendWaveEnable)
				{
// 					sndCnt = SendDataNet( SOCKET_WAVE, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460
					sndCnt = TcpSendData( SOCKET_WAVE, pItem->au8data[index],pItem->au32len[index]); //发送的数据帧长度必须小于1460	
					FReConnect( sndCnt, SOCKET_WAVE );
					OSTimeDly(1);
				}
				if(PROMODPARAM.m_u8EnableFlag[4] == 1)
				{
					U2SendBytes(pItem->au8data[index], pItem->au32len[index]);

					OSTimeDly(1);
				}
			 
				index++;	
			}while(index < pItem->u8Cnt);
	   }


#if	TASKTIMEENABLE
		if(2==TaskRunTimeStatistics.m_ulTaskProtocolFlag){//2:结算状态
			OSSchedLock();
			T1TCR=0x00;
			TaskRunTimeStatistics.m_ulTaskProtocolDet=(RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskProtocol1)>TaskRunTimeStatistics.m_ulTaskProtocolDet)?RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskProtocol1):TaskRunTimeStatistics.m_ulTaskProtocolDet; 				
			T1TCR=0x01;	
			OSSchedUnlock();
		}
#endif 
	}
}

/*********************************************************************************************************
** Function name:     F_FindZTCount
** Descriptions:      计算二维数组中非-1的个数
** input parameters:  二维数组
** output parameters: 数组中非-1的个数
**
** Created by: wanglihui		  
** Created Date: 20170603	  
**-------------------------------------------------------------------------------------------------------

** Modified by:		NONE	   
** Modified date:	 
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

uint8 F_FindZTCount(int16 (*ptr)[4])  
{
	int i,j;
	uint8 count=0;
	for(i=0;i<2*CHL_MAXNUM;i++)
	{
		for(j=0;j<4;j++)
		{
			if(*(*(ptr+i)+j)!=-1)
			{
				count++;
			}
		}
	}
	
	return count;
}
