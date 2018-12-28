/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskProtocol.C
** Last modified Date:  20110523
** Last Version:		1.0
** Descriptions:		��λ��ͨ������
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

#define		SETUPALIAS			g_Setup			//���ò����ṹ
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
#define		CMDTOSENDBUFSIZE	4

static		uint8	m_au8CommandToSend[CMDTOSENDBUFSIZE];	//����������
static		uint8	m_u8CTSReadIndex;
static		uint8	m_u8CTSWriteIndex;

//#if	YBVERSION >= 30		//3.0�Ǳ���
//static		void	GetSendBufByVRInfo(uint8 * p_pu8BufAxle, uint8 * p_pu8BufAxleGrp);
//#endif
/*********************************************************************************************************
** Function name:           GetCmdInBuf
**
** Descriptions:            ��ȡ������е�����
**
** input parameters:        p_u8Type����ȡ���ͣ�1��ȡ����2���鿴
** output parameters:       None
** Returned value:          �����
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
	if (m_u8CTSReadIndex != m_u8CTSWriteIndex)	//���������
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
** Descriptions:            ���ô���������
**
** input parameters:        p_u8Cmd	���������
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
** Descriptions:            ����Э�鷢������
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
** Descriptions:            ����Э�鷢������
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
** Descriptions:            ����Э�鷢������
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
** Descriptions:            ��PC����ָ��
**
** input parameters:	uint8	p_u8Datalen	�������ݳ���
						uint8	p_u8Cmd		������������
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
** Descriptions:            ��PC����ָ��
**
** input parameters:        p_u8VehIndex	������
** 							p_u8CommandNO	�����
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
** Descriptions:            ͨ��������Ϣ�����õ������͵��ַ�����
**
** input parameters:        uint8 *	p_pu8BufAxle: ���ػ���
**							uint8 * p_pu8BufAxleGrp: �����ػ���
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
** Descriptions:            ����7����������λ��ͨ��
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
	
	uint32 sndCnt_tmp = 0;     // ���ڲ������ݰ�����ʧ�ܺ��ط�liumingxin

#if OS_Q_QUERY_EN > 0
sndFrame_Struct *pWaveItem = (sndFrame_Struct *)0;
OS_Q_DATA data;
#endif 



	tdata=tdata;
	
	// �ȴ���ʼ������
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

		//SendDeviceStatus0();	//��ʱ������Ϣ
		// ����
		switch(SETUPALIAS.u8Protocol)
		{			   				   
			case PROTOL_WJ_NEW:
//				SendDeviceStatus0();	//��ʱ������Ϣ
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
		if(1==TaskRunTimeStatistics.m_ulTaskProtocolFlag ){//1:��ʼ����״̬
			OSSchedLock();
			if(0x01!=T1TCR){
				T1TCR=0x02;
				T1TCR=0x01;
			}			
			TaskRunTimeStatistics.m_ulTaskProtocolFlag=2;   //�л�������״̬
			TaskRunTimeStatistics.m_ulTaskProtocolDet=0;
			
			TaskRunTimeStatistics.m_ulTaskProtocol1=T1TC;
			OSSchedUnlock();				
		}
		else if(2==TaskRunTimeStatistics.m_ulTaskProtocolFlag){
		    OSSchedLock();
			TaskRunTimeStatistics.m_ulTaskProtocol1=T1TC; //����״̬�и���
			OSSchedUnlock();
		}
#endif

		//�������� 	
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

		//��������
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

						sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
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

						sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
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
					sndCnt = TcpSendData( SOCKET_WT, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
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
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5000 sndCnt=%x Framelen=%d��\r\n",
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

						sndCnt = TcpSendData( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
						
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
// 							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5002 sndCnt=%x Framelen=%d��\r\n",
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
			

			// ��������֡added by wujinlong 20130805
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

#if OS_Q_QUERY_EN > 0  //ʹ��OS_Q_QUERY_ENΪ1 wzf
	 	/****************************************
	  	*���������ݶ���Ϊ��ʱ����ʼ���Ͳ������ݣ�
	  	*���򲻷��Ͳ�������.�������ݶ������ȷ���wzf 
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
// 						sndCnt = SendDataNet( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //���͵�����֡���ȱ���С��1460
						sndCnt = TcpSendData( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //���͵�����֡���ȱ���С��1460
						if(sndCnt != 0)
						{
							g_u8IsConnectFlag = 1;
						}
						FReConnect( sndCnt, SOCKET_WAVE );	  //SOCKET_WAVE
						OSTimeDly(1);
						
						/************����ʧ�����·���һ�θð�����(liumingxin)***********/
						if(sndCnt == 0)    // lmx
						{
// 							sndCnt_tmp = SendDataNet( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //���͵�����֡���ȱ���С��1460
							sndCnt_tmp = TcpSendData( SOCKET_WAVE, pWaveItem->au8data[index],pWaveItem->au32len[index]); //���͵�����֡���ȱ���С��1460
							if(sndCnt_tmp != 0)
							{
								g_u8IsConnectFlag = 1;    // ���������������1����������0���Ӳ�������Ĭ����1(lmx)
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

					// ���Ͳ���֡added by wujinlong 20130805
//#if DEBUGINFO_TASK_SP_EN > 0
//					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp Wave FrameNum=%d��\r\n",
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
	   pItem = (sndFrame_Struct *)OSQAccept(g_pQJDSend); //��Ϊ���ͳ���խ������
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
// 					sndCnt = SendDataNet( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
					sndCnt = TcpSendData( SOCKET_SVI, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
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
// 					sndCnt = SendDataNet( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
					sndCnt = TcpSendData( SOCKET_RES, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
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
// 						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%02u:%02u:%02u T7 5002 sndCnt=%x Framelen=%d��\r\n",
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
// 					sndCnt = SendDataNet( SOCKET_WAVE, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460
					sndCnt = TcpSendData( SOCKET_WAVE, pItem->au8data[index],pItem->au32len[index]); //���͵�����֡���ȱ���С��1460	
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
		if(2==TaskRunTimeStatistics.m_ulTaskProtocolFlag){//2:����״̬
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
** Descriptions:      �����ά�����з�-1�ĸ���
** input parameters:  ��ά����
** output parameters: �����з�-1�ĸ���
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
