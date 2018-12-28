/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskSDCard.c
** Last modified Date:  20131108
** Last Version:		1.0
** Descriptions:		SD����
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20131108
** Version:				1.0
** Descriptions:		SD���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__TASK_SDCARD_C

#include "common.h"

#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
#define		SDCAPA					((PROMODPARAM.m_u8SDCapa)*1024*1024*2*8/10) // SD������	 1024M*1024K*1024B/512B


uint8  		m_au8SDRdIxBuf[SD_SECTOR_LEN] = {0};
uint8  		m_au8SDWrIxBuf[SD_SECTOR_LEN] = {0};

//
uint32	m_u32Ix_Addr_Begin = 0;	// ��������ʼλ��
uint32	m_u32Ix_Addr_End = 0;	// ����������λ��

//
uint32	m_u32Offnet_Addr_Begin = 0;	// �������濪ʼλ��
uint32	m_u32Offnet_Data_Begin = 0;	// �����������ݿ�ʼλ��
uint32	m_u32Offnet_Addr_End = 0;	// �����������λ��

//	
uint32	m_u32VehInfo_Addr_Begin = 0;	// ������Ϣ��ʼλ��
uint32	m_u32VehInfo_Data_Begin = 0;
uint32	m_u32VehInfo_Addr_End = 0;


//	
uint32	m_u32Log_Addr_Begin = 0;	   // ��־��ʼλ��
uint32	m_u32Log_Data_Begin = 0;
uint32	m_u32Log_Addr_End = 0;


//
uint32	m_u32Wave_Addr_Begin = 0;	   // ���ο�ʼλ��
uint32	m_u32Wave_Data_Begin = 0;
uint32	m_u32Wave_Addr_End = 0;	

//




/*********************************************************************************************************
** Function name:		TaskSDCard
** Descriptions:		SD������
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/			   
void  TaskSDCard(void *tdata)
{	
	// ����
	uint8		l_u8Tempi = 0;						// ѭ������
	//uint32		l_u32Tempi = 0;
	uint32		l_u32Ret = 0;						// ���ر�־

	uint8		l_u8SDInsert = 0;					// SD������״̬��1���룬0δ����
	uint8		l_u8SDInsertPre = 0;				// ��һʱ��SD������״̬��1���룬0δ����
	//
	uint16		l_u16SendLen = 0;						// ��ʽ������
	uint8		l_u8Err;

	// ��������
	uint32		l_u32WrAdd = 0;						// д��ַ
	uint32		l_u32RdAdd = 0;						// ����ַ
	uint8		l_u8IsWrFlag = 0;					// �Ƿ�д��־
#if OS_Q_QUERY_EN > 0
	OS_Q_DATA 	l_cQData;
#endif

	// ��������
	sndFrame_Struct *l_pcWTFrame = (sndFrame_Struct *)0;	
	CSDOffnetIx	l_cOffnetIx;
	uint32		l_u32FrameNum = 0;					// ����֡��

	// ������Ϣ
	//CComBuf    *l_pcVehData = (CComBuf *)0;
	uint8	   *l_pcVehData = (uint8 *)0;
	uint32		l_u32VehNum = 0;					
	uint32		l_u32VehNext = 0;				
	uint32		l_u32VehCur = 0;				

	// ������Ϣ����־
	CComBuf    *l_pcLogData = (CComBuf *)0;
	uint32		l_u32LogNum = 0;					// ����־��
	uint32		l_u32NextLog = 0;					// ��һ��־�Ĵ洢λ��
	uint32		l_u32CurLog = 0;					// ��ǰ��־�Ĵ洢λ��

	
	// ����
	sndFrame_Struct    *l_pcZTData = (sndFrame_Struct *)0;
	uint32		l_u32ZTNum = 0;					// �ܲ�����
	uint32		l_u32NextZT = 0;					// ��һ���εĴ洢λ��
	uint32		l_u32CurZT = 0;					// ��ǰ���εĴ洢λ��
	
	// ʱ���¼��������浽SD��
//	uint32		l_u32OffnetTimer = 0;
	uint32		l_u32VehTimer = 0;
	uint32		l_u32LogTimer = 0;				// ��־ʱ���¼
	uint32		l_u32WaveTimer = 0;

	//
	uint8		*l_pu8Tmp;
	//
	uint32	   	l_u32TempVar;
	uint32		l_u32SDTempIx = 0;					// ѭ������
	uint32		l_u32SDTempj = 0;					// ѭ������
	uint32		l_u32SDReadIx = 0;					// SD��������
	//uint32		l_u32RdAdd = 0;						// ����ַ
	uint32		l_u32Num = 0;						// ��	
	uint32		l_u32Remind = 0;					// ����

	

	//
	tdata = tdata;

	// ��ʼ��
	memset( &l_cOffnetIx, 0, sizeof(CSDOffnetIx) );




	// �ȴ���ʼ������
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}
	// ����ʼ����
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u TSD Recd Task Begin��\r\n", 
						g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
       					g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif


	// ����Ƿ����¿�
	FSDNewCardInit();

	// ͬ��������
	if( SD_DATA_STORAGE == (PROMODPARAM.m_u16SDSaveEn&SD_DATA_STORAGE) )
	{
		// SD��ʹ��
		if( FSDCheckAntiShake(&sd_info) )						
		{
			// ��⿨�Ƿ���ȫ���뿨��
			FSysnVehIx();
			FSysnLogIx();
			FSysnWaveIx();
			FSysnJDIx();
		}
	}


	while(1)
	{
		OSTimeDly( SD_TASK_TIMEDLY*OS_TICKS_PER_SEC/1000 );

		// ����SD������ʱ�����
		l_u32VehTimer++;
		if( l_u32VehTimer > SD_TDC2SD_TIME*2 )
		{
			l_u32VehTimer = SD_TDC2SD_TIME*2;
		}
		l_u32LogTimer++;
		if( l_u32LogTimer > SD_TDC2SD_TIME*2 )
		{
			l_u32LogTimer = SD_TDC2SD_TIME*2;
		}
		l_u32WaveTimer++;
		if( l_u32WaveTimer > SD_TDC2SD_TIME*2 )
		{
			l_u32WaveTimer = SD_TDC2SD_TIME*2;
		}

		/** ����������� **/
		// SD������״̬��⣬���δ������ֱ��continue��20131126
		l_u8SDInsertPre = l_u8SDInsert;
		l_u8SDInsert = FSDCheckAntiShake(&sd_info);
		if( !l_u8SDInsert )							/* ��⿨�Ƿ���ȫ���뿨�� */
		{
			continue;
		}


		// ʵʱ���SD��״̬����������²��룬�����SD����ʼ��
		if( (!l_u8SDInsertPre) && l_u8SDInsert )
		{
			SDCardInit();
			OSTimeDly( OS_TICKS_PER_SEC );
			// ÿ�μ�����²����SD����Ҫ����Ƿ����¿�
			memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
			l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_ZONG, g_u8SDRdBuf );
			if( SD_NO_ERR == l_u32Ret )
			{
				l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_CARD_FLAG_LEN );
				if( ERR_SD_NEW_CARD == l_u32Ret )
				{
					// Ϊ�¿�
					memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
					for( l_u8Tempi = 0; l_u8Tempi<SD_NEW_CARD_FLAG_LEN; l_u8Tempi++ )
					{
						g_u8SDWrBuf[l_u8Tempi] = 0xAA;
					}
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_ZONG, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}		
				}
			}

			// ��⿨�Ƿ���ȫ���뿨��
			FSysnVehIx();
			FSysnLogIx();
			FSysnWaveIx();
			FSysnJDIx();
		}

		
		
	

		// ��������Ƿ�����
		// �������湦��
		// �������״̬��������������״̬
		g_u32NetConCnt++;
		if( g_u32NetConCnt > SD_NET_CON_JUDGE_TIME*2 )
		{
			g_u32NetConCnt = SD_NET_CON_JUDGE_TIME*2;
		}
		if( g_u32NetConCnt > SD_NET_CON_JUDGE_TIME )
		{
			g_u8IsConnectFlag = 0;
		}

		// �������ݷ���
		if( g_u8VehSendFlag )
		{
			if( SdHal_CheckCard( &sd_info ) )
			{
				memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
				l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDRdBuf );
				if( SD_NO_ERR == l_u32Ret )
				{
					l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
					if( ERR_SD_NOTNEW == l_u32Ret )
					{
						// ֱ�Ӹ�ֵ
						l_u32VehNum = (g_u8SDRdBuf[SD_CTX_VEH_SUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_SUM+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_VEH_SUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_SUM+3];
						l_u32VehNext = (g_u8SDRdBuf[SD_CTX_VEH_NEXT+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NEXT+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_VEH_NEXT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NEXT+3];
						l_u32VehCur = (g_u8SDRdBuf[SD_CTX_VEH_CUR+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_CUR+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_VEH_CUR+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_CUR+3];
					
						// �ϴ�
						//l_u32Num = m_u32EndIx-m_u32BeginIx+1;
						l_u32Num = (g_u32EndIx-g_u32BeginIx+1)>l_u32VehNum ? l_u32VehNum : (g_u32EndIx-g_u32BeginIx+1);//���ñ��������ӱ���	
						for( l_u32SDTempIx=0; l_u32SDTempIx<l_u32Num; l_u32SDTempIx++ )
						{
							l_u32SDReadIx = (l_u32VehCur+(g_u32BeginIx-1)+l_u32SDTempIx) % SD_VEH_MAXNUM;
							//
							memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
							l_u32RdAdd = SD_GETNEXT_SECTOR( SD_VEH_SEC_DATA_BGN, l_u32SDReadIx );
							l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
							if( SD_NO_ERR == l_u32Ret )
							{
								g_cSDSendVehData.u8Cnt = 1;
								g_cSDSendVehData.au32len[0] = g_u8SDRdBuf[SD_VEH_SEC_DATA+5]*256+g_u8SDRdBuf[SD_VEH_SEC_DATA+6];
								l_pu8Tmp = g_cSDSendVehData.au8data[0];
								memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_VEH_SEC_DATA, SD_VEH_SEC_DATA_LEN );

								TcpSendData( SOCKET_WT, g_cSDSendVehData.au8data[0],g_cSDSendVehData.au32len[0]);
								OSTimeDly(40);
							}																					
						}
					}
					else
					{
					}
				}
			}

			// ��Ϊ0
			g_u8VehSendFlag = 0;
			g_u32BeginIx = 0;
			g_u32EndIx = 0;
		}

		if( g_u8DebugInfoSendFlag )
		{
			if( SdHal_CheckCard( &sd_info ) )
			{
				memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
				l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDRdBuf );
				if( SD_NO_ERR == l_u32Ret )
				{
					l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
					if( ERR_SD_NOTNEW == l_u32Ret )
					{
						// ֱ�Ӹ�ֵ
						l_u32LogNum = (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+3];
						l_u32NextLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+3];
						l_u32CurLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+1]<<16) 
									+ (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+3];
						
						// �ϴ�
						l_u32TempVar = (g_u32EndIx-g_u32BeginIx+1)>l_u32LogNum ? l_u32LogNum : (g_u32EndIx-g_u32BeginIx+1);//���ñ��������ӱ���
						l_u32Num = l_u32TempVar/10;						// ��
						l_u32Remind = l_u32TempVar%10;
						for( l_u32SDTempIx=0; l_u32SDTempIx<l_u32Num; l_u32SDTempIx++ )
						{
							l_pu8Tmp = g_cSDSendVehData.au8data[0];
						   	// ��֡
							g_cSDSendVehData.u8Cnt = 1;
							g_cSDSendVehData.au32len[0] = 1011;
							*(l_pu8Tmp++) = 0xff;
							*(l_pu8Tmp++) = 0xff;
							*(l_pu8Tmp++) = 0x00;
							*(l_pu8Tmp++) = ADJ_GETSDINFO;
							*(l_pu8Tmp++) = 0x12;
							*(l_pu8Tmp++) = (1011>>8)&0xFF;
							*(l_pu8Tmp++) = (1011)&0xFF;
							//
							for( l_u32SDTempj=0; l_u32SDTempj<10; l_u32SDTempj++ )
							{
								l_u32SDReadIx = (l_u32CurLog+g_u32BeginIx+l_u32SDTempIx*10+l_u32SDTempj) % SD_LOG_MAX_NUM;
								//
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32RdAdd = SD_GETNEXT_SECTOR( SD_DBGINFO_SEC_DATA_BGN, l_u32SDReadIx );
								l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									if( 0x00!=(*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+99)) )
									{
										*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+98) = 0x0D;
										*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+99) = 0x0A;
									}
									memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA, 100 );
									l_pu8Tmp += 100;
								}
								else
								{
									memset( l_pu8Tmp, 0, 100 );
									l_pu8Tmp += 100;
								}
							}
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 0;
							AddCrc16(g_cSDSendVehData.au8data[0],g_cSDSendVehData.au32len[0]-2);
							TcpSendData( SOCKET_WT, g_cSDSendVehData.au8data[0],g_cSDSendVehData.au32len[0]);	
							OSTimeDly(80);										
						}
						if( l_u32Remind )
						{
							l_pu8Tmp = g_cSDSendVehData.au8data[0];
						   	// ��֡
							g_cSDSendVehData.u8Cnt = 1;
							g_cSDSendVehData.au32len[0] = 1011;
							*(l_pu8Tmp++) = 0xff;
							*(l_pu8Tmp++) = 0xff;
							*(l_pu8Tmp++) = 0x00;
							*(l_pu8Tmp++) = ADJ_GETSDINFO;
							*(l_pu8Tmp++) = 0x12;
							*(l_pu8Tmp++) = (1011>>8)&0xFF;
							*(l_pu8Tmp++) = (1011)&0xFF;
							//
							for( l_u32SDTempj=0; l_u32SDTempj<l_u32Remind; l_u32SDTempj++ )
							{
								l_u32SDReadIx = (l_u32CurLog+g_u32BeginIx+l_u32SDTempIx*10+l_u32SDTempj) % SD_LOG_MAX_NUM;
								//
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32RdAdd = SD_GETNEXT_SECTOR( SD_DBGINFO_SEC_DATA_BGN, l_u32SDReadIx );
								l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									if( 0x00!=(*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+99)) )
									{
										*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+98) = 0x0D;
										*(g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA+99) = 0x0A;
									}
									memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA, 100 );
									l_pu8Tmp += 100;
								}
								else
								{
									memset( l_pu8Tmp, 0, 100 );
									l_pu8Tmp += 100;
								}
							}
							memset(l_pu8Tmp,0,100*(10-l_u32Remind));
							l_pu8Tmp += 100*(10-l_u32Remind);
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 0;
							AddCrc16(g_cSDSendVehData.au8data[0],g_cSDSendVehData.au32len[0]-2);
							TcpSendData( SOCKET_WT, g_cSDSendVehData.au8data[0],g_cSDSendVehData.au32len[0]);	
							OSTimeDly(80);
						}
					}
					else
					{
					}
				}
			}

			// ��Ϊ0
			g_u8DebugInfoSendFlag = 0;
			g_u32BeginIx = 0;
			g_u32EndIx = 0;
		}


		// SD��ʹ��
		if( SD_DATA_STORAGE == (PROMODPARAM.m_u16SDSaveEn&SD_DATA_STORAGE) )
		{
			// �������湦��
			if(SD_OFFNET == (PROMODPARAM.m_u16SDSaveEn&SD_OFFNET))
			{
			 	// �ж��Ƿ���д�����ݣ�����У����ж��������������򲻽��в�������ֹÿ�ζ���
				l_u8IsWrFlag = 1;
#if OS_Q_QUERY_EN > 0
				// ����ͳ�ƣ������д���ݣ����־��1
				memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
				l_u32Ret = OSQQuery( g_pQSDOffNetWrite, &l_cQData );  
			 	if( OS_NO_ERR == l_u32Ret )
				{
					if( l_cQData.OSNMsgs > 0 )
					{
						l_u8IsWrFlag = 1;
					}
					else
					{
						l_u8IsWrFlag = 0;
					}
				}
#else
				// ��������ֱ����1��ÿ�ζ����ж�
				l_u8IsWrFlag = 1;
#endif
				// 
				if( l_u8IsWrFlag )
				{
					// ����������
					// �жϹ������������Ϊ�µ����������ʼ���������д����ֱ�Ӹ�ֵ			
					memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, g_u8SDRdBuf );
					if( SD_NO_ERR == l_u32Ret )
					{
						l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
						if( ERR_SD_NOTNEW == l_u32Ret )
						{
							// ֱ�Ӹ�ֵ
							l_cOffnetIx.m_u32OffnetVehNum = (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+3];			
							l_cOffnetIx.m_u32OffnetWrSec = (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+3];		
							l_cOffnetIx.m_u32OffnetWrSub = (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SUB+0]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_WR_SUB+1];
							l_cOffnetIx.m_u32OffnetRdSec = (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+3];		
							l_cOffnetIx.m_u32OffnetRdSub = (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SUB+0]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_RD_SUB+1];
							l_cOffnetIx.m_u32VehIx = (g_u8SDRdBuf[SD_CTX_VEH_NUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NUM+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_VEH_NUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NUM+3];		
							 
						}
						else if( ERR_SD_NEW_SECTOR == l_u32Ret )
						{
							memset( &l_cOffnetIx, 0, sizeof(CSDOffnetIx) );
							//
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							FSDFormOffnetCtx( m_au8SDWrIxBuf, &l_cOffnetIx );
							l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
							}					
						}
						else
						{
						}
					}
					while(1)
					{
						OSTimeDly(10);
						//
						l_pcWTFrame = (sndFrame_Struct *)OSQAccept(g_pQSDOffNetWrite);	
						if( l_pcWTFrame != (sndFrame_Struct *)0 )
						{  		
							// ��������
							memset( m_au8SDRdIxBuf, 0, SD_SECTOR_LEN );
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							l_u32RdAdd = SD_GETNEXT_SECTOR( SD_OFFNET_SEC_BGN, l_cOffnetIx.m_u32OffnetWrSec);
							l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, m_au8SDRdIxBuf );
							memcpy( m_au8SDWrIxBuf, m_au8SDRdIxBuf, SD_SECTOR_LEN );
							if( SD_NO_ERR == l_u32Ret )
							{
								l_u32Ret = FCheckSD( m_au8SDWrIxBuf, SD_NEW_SECTOR_FLAG_LEN );
								if( ERR_SD_NEW_SECTOR == l_u32Ret )
								{
									m_au8SDWrIxBuf[0] = 0xAA;
									m_au8SDWrIxBuf[1] = 0xAA;
									m_au8SDWrIxBuf[2] = 0xAA;
									m_au8SDWrIxBuf[3] = 0xAA;
								}
								// �������
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER) = (l_cOffnetIx.m_u32VehIx>>24) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+1) = (l_cOffnetIx.m_u32VehIx>>16) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+2) = (l_cOffnetIx.m_u32VehIx>>8) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+3) = (l_cOffnetIx.m_u32VehIx) & 0xFF;
								// ����ʱ��
								GET8563(&g_DevCurTime);
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+4) = (g_DevCurTime.u16Year>>8) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+5) = (g_DevCurTime.u16Year) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+6) = (g_DevCurTime.u8Month) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+7) = (g_DevCurTime.u8Day) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+8) = (g_DevCurTime.u8Hour) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+9) = (g_DevCurTime.u8Minute) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+10) = (g_DevCurTime.u8Second) & 0xFF;
								//
								l_u32WrAdd = SD_GETNEXTVEH_SECTOR(l_cOffnetIx.m_u32OffnetWrSec,l_cOffnetIx.m_u32OffnetWrSub,0); 
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+11) = (l_u32WrAdd>>24) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+12) = (l_u32WrAdd>>16) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+13) = (l_u32WrAdd>>8) & 0xFF;
								*(m_au8SDWrIxBuf+4+l_cOffnetIx.m_u32OffnetWrSub*SD_IX_LEN_PER+14) = (l_u32WrAdd) & 0xFF;

								l_u32WrAdd = SD_GETNEXT_SECTOR( SD_OFFNET_SEC_BGN, l_cOffnetIx.m_u32OffnetWrSec);
								l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd, m_au8SDWrIxBuf );		
								if( SD_NO_ERR != l_u32Ret )
								{
									continue;
								}
								//
								
									
							}
							else
							{
								continue;
							}

							// ��������
							l_u32FrameNum = l_pcWTFrame->u8Cnt;
							if( l_u32FrameNum>100 )
							{
								l_u32FrameNum = 100;
							}
							l_u8Tempi = 0;
							for( l_u8Tempi = 0; l_u8Tempi<l_u32FrameNum; l_u8Tempi++ )
							{			
								memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
								memset( g_u8SDWrBuf2, 0, SD_SECTOR_LEN );
		
								l_u32Ret = FSDFormOffnetBuf( g_u8SDWrBuf, g_u8SDWrBuf2, l_pcWTFrame->au8data[l_u8Tempi], l_pcWTFrame->au32len[l_u8Tempi] );
								if( SUCCESS != l_u32Ret )
								{
									continue;
								}

								//

								g_u8SDWrBuf[SD_OFFNET_SEC_FRAM_NUM+0] = (l_u32FrameNum>>8) & 0xFF;	 // ���ֶ���Ч
								g_u8SDWrBuf[SD_OFFNET_SEC_FRAM_NUM+1] = (l_u32FrameNum) & 0xFF;
								g_u8SDWrBuf[SD_OFFNET_SEC_VEH_IX+0] = (l_cOffnetIx.m_u32VehIx>>24) & 0xFF;
								g_u8SDWrBuf[SD_OFFNET_SEC_VEH_IX+1] = (l_cOffnetIx.m_u32VehIx>>16) & 0xFF;
								g_u8SDWrBuf[SD_OFFNET_SEC_VEH_IX+2] = (l_cOffnetIx.m_u32VehIx>>8) & 0xFF;
								g_u8SDWrBuf[SD_OFFNET_SEC_VEH_IX+3] = (l_cOffnetIx.m_u32VehIx) & 0xFF;
								
								//		
								l_u32WrAdd = SD_GETNEXTVEH_SECTOR( l_cOffnetIx.m_u32OffnetWrSec, l_cOffnetIx.m_u32OffnetWrSub, l_u8Tempi );		
								l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd, g_u8SDWrBuf );		
								if( SD_NO_ERR != l_u32Ret )
								{
									continue;
								}

								if( l_pcWTFrame->au32len[l_u8Tempi] > SD_OFFNET_SEC_DATA_LEN )
								{
									g_u8SDWrBuf2[SD_OFFNET_SEC_FRAM_NUM+0] = 0x00;	 // ���ֶ���Ч
									g_u8SDWrBuf2[SD_OFFNET_SEC_FRAM_NUM+1] = 0x00;
									g_u8SDWrBuf2[SD_OFFNET_SEC_VEH_IX+0] = (l_cOffnetIx.m_u32VehIx>>24) & 0xFF;
									g_u8SDWrBuf2[SD_OFFNET_SEC_VEH_IX+1] = (l_cOffnetIx.m_u32VehIx>>16) & 0xFF;
									g_u8SDWrBuf2[SD_OFFNET_SEC_VEH_IX+2] = (l_cOffnetIx.m_u32VehIx>>8) & 0xFF;
									g_u8SDWrBuf2[SD_OFFNET_SEC_VEH_IX+3] = (l_cOffnetIx.m_u32VehIx) & 0xFF;
									l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd+1, g_u8SDWrBuf2 );		
									if( SD_NO_ERR != l_u32Ret )
									{
										continue;
									} 
								}
								OSTimeDly(1);          	
							}

							// ������ŵ�
							if( l_cOffnetIx.m_u32OffnetWrSub<SD_IX_NUM-1 )
							{
								l_cOffnetIx.m_u32OffnetWrSub++;								
							}
							else
							{
								l_cOffnetIx.m_u32OffnetWrSub = 0;
								l_cOffnetIx.m_u32OffnetWrSec++;	
							}
							
							// ���泵��++
							if( l_cOffnetIx.m_u32OffnetVehNum<SD_VEH_MAX_NUM )
							{								
								l_cOffnetIx.m_u32OffnetVehNum++;
							}
							else
							{
								l_cOffnetIx.m_u32OffnetVehNum = SD_VEH_MAX_NUM;
								if( l_cOffnetIx.m_u32OffnetRdSub<SD_IX_NUM-1 )
								{
									l_cOffnetIx.m_u32OffnetRdSub++;								
								}
								else
								{
									l_cOffnetIx.m_u32OffnetRdSub = 0;
									l_cOffnetIx.m_u32OffnetRdSec++;	
								}									
							}
							// �±�����
							if( l_cOffnetIx.m_u32VehIx<SD_VEH_MAX_NUM-1 )  // ���0~MAX-1
							{
								l_cOffnetIx.m_u32VehIx++;
							}
							else
							{
								l_cOffnetIx.m_u32VehIx = 0;
								l_cOffnetIx.m_u32OffnetWrSec = 0;
								l_cOffnetIx.m_u32OffnetWrSub = 0;
							}
						}
						else
						{
							// ����������
							// ���ոĳ���д���磬��дSD

							// ���湦����������
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							FSDFormOffnetCtx( m_au8SDWrIxBuf, &l_cOffnetIx );
							l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
							}
							break;
						}
						
					}//end while(1)
				}

				// ������������
				if( g_u8IsConnectFlag )
				//if( 1 )
				{
					memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, g_u8SDRdBuf );
					if( SD_NO_ERR == l_u32Ret )
					{
						l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
						if( ERR_SD_NOTNEW == l_u32Ret )
						{
							// ֱ�Ӹ�ֵ
							l_cOffnetIx.m_u32OffnetVehNum = (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_VEHNUM+3];			
							l_cOffnetIx.m_u32OffnetWrSec = (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_WR_SEC+3];		
							l_cOffnetIx.m_u32OffnetWrSub = (g_u8SDRdBuf[SD_CTX_OFFNET_WR_SUB+0]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_WR_SUB+1];
							l_cOffnetIx.m_u32OffnetRdSec = (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+0]<<24) + (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+2]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_RD_SEC+3];		
							l_cOffnetIx.m_u32OffnetRdSub = (g_u8SDRdBuf[SD_CTX_OFFNET_RD_SUB+0]<<8 ) + g_u8SDRdBuf[SD_CTX_OFFNET_RD_SUB+1];
							l_cOffnetIx.m_u32VehIx = (g_u8SDRdBuf[SD_CTX_VEH_NUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NUM+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_VEH_NUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NUM+3];		
							 
						}
						else if( ERR_SD_NEW_SECTOR == l_u32Ret )
						{
							memset( &l_cOffnetIx, 0, sizeof(CSDOffnetIx) );
							// ��֡��д������
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							FSDFormOffnetCtx( m_au8SDWrIxBuf, &l_cOffnetIx );

							l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
							}					
						}
						else
						{
						}
					}
		
					// 
					if( l_cOffnetIx.m_u32OffnetVehNum > 0 )
					{
						while( l_cOffnetIx.m_u32OffnetVehNum )
						//while( 0 )
						{  		
							if( !g_u8IsConnectFlag )
							{
								// ���湦����������
								memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
								FSDFormOffnetCtx( m_au8SDWrIxBuf, &l_cOffnetIx );
								l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, m_au8SDWrIxBuf );		
								if( SD_NO_ERR != l_u32Ret )
								{
								}
								break;
							}
							OSTimeDly(50);
							//
							l_u32FrameNum = 0;
							l_u32RdAdd = SD_GETNEXTVEH_SECTOR( l_cOffnetIx.m_u32OffnetRdSec, l_cOffnetIx.m_u32OffnetRdSub, 0 );
							memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
							l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
							if( SD_NO_ERR == l_u32Ret )
							{
								l_u32FrameNum = (g_u8SDRdBuf[SD_OFFNET_SEC_FRAM_NUM]<<8)+g_u8SDRdBuf[SD_OFFNET_SEC_FRAM_NUM+1];	
							}
							//
							l_u8Tempi = 0;
							g_acSDSendVehInfo[g_u16SDSendBufIX].u8Cnt = l_u32FrameNum;
							for( l_u8Tempi = 0; l_u8Tempi<l_u32FrameNum; l_u8Tempi++ )
							{
								l_u32RdAdd = SD_GETNEXTVEH_SECTOR( l_cOffnetIx.m_u32OffnetRdSec, l_cOffnetIx.m_u32OffnetRdSub, l_u8Tempi );
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								memset( g_u8SDRdBuf2, 0, SD_SECTOR_LEN );
								l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );						
								l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd+1, g_u8SDRdBuf2 );
								//
								g_acSDSendVehInfo[g_u16SDSendBufIX].au32len[l_u8Tempi] = (g_u8SDRdBuf[SD_OFFNET_SEC_FRAM_LEN]<<8) + g_u8SDRdBuf[SD_OFFNET_SEC_FRAM_LEN+1];
								memcpy( g_acSDSendVehInfo[g_u16SDSendBufIX].au8data[l_u8Tempi], 
										&g_u8SDRdBuf[SD_OFFNET_SEC_FRAM_DATA], 
										SD_OFFNET_SEC_DATA_LEN );
								if( g_acSDSendVehInfo[g_u16SDSendBufIX].au32len[l_u8Tempi]>SD_OFFNET_SEC_DATA_LEN 
									&& g_acSDSendVehInfo[g_u16SDSendBufIX].au32len[l_u8Tempi]-SD_OFFNET_SEC_DATA_LEN<SD_OFFNET_SEC_DATA_LEN)
								{
									memcpy( &g_acSDSendVehInfo[g_u16SDSendBufIX].au8data[l_u8Tempi][SD_OFFNET_SEC_DATA_LEN], 
											&g_u8SDRdBuf2[SD_OFFNET_SEC_FRAM_DATA], 
											g_acSDSendVehInfo[g_u16SDSendBufIX].au32len[l_u8Tempi]-SD_OFFNET_SEC_DATA_LEN );
								}
								//
								OSTimeDly(1);

								
							}

							// �����������־�����¼���CRC
							g_acSDSendVehInfo[g_u16SDSendBufIX].au8data[0][11] = 0x01;
							AddCrc16(g_acSDSendVehInfo[g_u16SDSendBufIX].au8data[0],g_acSDSendVehInfo[g_u16SDSendBufIX].au32len[0]-2);
	
							//
							l_u32Ret = OSQPost( g_pQSendPC, &g_acSDSendVehInfo[g_u16SDSendBufIX] );
							if( l_u32Ret == OS_Q_FULL)
							{
								g_ErrMsgState |= 0x04; 		 	
							}
//							SendDataNet( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
							
		
							// ��λ�ø���
							if( l_cOffnetIx.m_u32OffnetRdSub<SD_IX_NUM-1 )
							{
								l_cOffnetIx.m_u32OffnetRdSub++;	
							}
							else
							{
								l_cOffnetIx.m_u32OffnetRdSub = 0;
								l_cOffnetIx.m_u32OffnetRdSec++;
								
							}
							
							// ���泵��--
							if( l_cOffnetIx.m_u32OffnetVehNum > 0 )
							{
								l_cOffnetIx.m_u32OffnetVehNum--;
							}
							if( 0 == l_cOffnetIx.m_u32OffnetVehNum )
							{
								l_cOffnetIx.m_u32OffnetRdSec = l_cOffnetIx.m_u32OffnetWrSec;
								l_cOffnetIx.m_u32OffnetRdSub = l_cOffnetIx.m_u32OffnetWrSub;
							}
							// �ܳ�������
							if( l_cOffnetIx.m_u32OffnetRdSec*SD_IX_NUM+l_cOffnetIx.m_u32OffnetRdSub>SD_VEH_MAX_NUM )
							{
								l_cOffnetIx.m_u32OffnetRdSec = 0;
								l_cOffnetIx.m_u32OffnetRdSub = 0;
							}
							else
							{
							
							}
							
							// �׻��泵����
							g_u16SDSendBufIX++;
							g_u16SDSendBufIX = g_u16SDSendBufIX % SD_SEND_BUF_NUM;
							//
							if( 0 == l_cOffnetIx.m_u32OffnetVehNum )
							{
								// ���湦����������
								memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
								FSDFormOffnetCtx( m_au8SDWrIxBuf, &l_cOffnetIx );
								l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, m_au8SDWrIxBuf );		
								if( SD_NO_ERR != l_u32Ret )
								{
								}
							}
						}							
					}
				} // end of if( g_u8IsConnectFlag )

			}

			// ������Ϣ����
			if(SD_VEHINFO == (PROMODPARAM.m_u16SDSaveEn&SD_VEHINFO))
			{
				// ����10min�����������ݴ���SD��
				if( l_u32VehTimer > SD_TDC2SD_TIME )
				{
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+4,(uint8 *)&l_u32VehNum, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+8,(uint8 *)&l_u32VehNext, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+12,(uint8 *)&l_u32VehCur, 4);
					//
					memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
					FSDFormVehCtx( m_au8SDWrIxBuf, l_u32VehNum, l_u32VehNext, l_u32VehCur );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_VEHINFO, m_au8SDWrIxBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					//
					l_u32VehTimer = 0;
				}
				
				l_u8IsWrFlag = 1;
#if OS_Q_QUERY_EN > 0
				// ����ͳ�ƣ������д���ݣ����־��1
				memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
				l_u32Ret = OSQQuery( g_pQSDVehWrite, &l_cQData );  
			 	if( OS_NO_ERR == l_u32Ret )
				{
					if( l_cQData.OSNMsgs > 0 )
					{
						l_u8IsWrFlag = 1;
					}
					else
					{
						l_u8IsWrFlag = 0;
					}
				}
#else
				// ��������ֱ����1��ÿ�ζ����ж�
				l_u8IsWrFlag = 1;
#endif
				
				if( l_u8IsWrFlag )
				{
					// ����������
					// �жϹ������������Ϊ�µ����������ʼ���������д����ֱ�Ӹ�ֵ			
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+4,(uint8 *)&l_u32VehNum, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+8,(uint8 *)&l_u32VehNext, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*1+12,(uint8 *)&l_u32VehCur, 4);
		
					while(1)
					{
						OSTimeDly(1);
						//
						l_pcVehData = (uint8 *)OSQAccept(g_pQSDVehWrite);	
						if( l_pcVehData != (uint8 *)0 )
						{  		
							memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );		
							//l_u32Ret = FSDFormVehBuf( g_u8SDWrBuf, l_pcVehData->m_au8DataBuf, l_pcVehData->m_u16Len );
							l_u32Ret = FSDFormVehBuf( g_u8SDWrBuf, l_pcVehData, (*(l_pcVehData+5))*256+(*(l_pcVehData+6)) );
							if( SUCCESS != l_u32Ret )
							{
								continue;
							}
							//		
							l_u32WrAdd = SD_GETNEXT_SECTOR( SD_VEH_SEC_DATA_BGN, l_u32VehNext );		
							l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd, g_u8SDWrBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
								continue;
							}
							//
							l_u32VehNext++;
							l_u32VehNext = l_u32VehNext % SD_VEH_MAXNUM;
							//
							if( l_u32VehNum < SD_VEH_MAXNUM )
							{
								l_u32VehNum++;
							}
							else
							{
								l_u32VehNum = SD_VEH_MAXNUM;
								l_u32VehCur++;
								l_u32VehCur = l_u32VehCur % SD_VEH_MAXNUM;
							}			
										
						}
						else
						{
							// ���湦����������
							WriteC256( SD_ADDR+SD_TDC_SIZE*1+4,(uint8 *)&l_u32VehNum, 4);
							WriteC256( SD_ADDR+SD_TDC_SIZE*1+8,(uint8 *)&l_u32VehNext, 4);
							WriteC256( SD_ADDR+SD_TDC_SIZE*1+12,(uint8 *)&l_u32VehCur, 4);
							break;
						}						
					}		
				} // end of if( l_u8IsWrFlag )
			}

			// ��־������Ϣ����
			if(SD_DEBUGINFO == (PROMODPARAM.m_u16SDSaveEn&SD_DEBUGINFO))
			{
				// ����10min�����������ݴ���SD��
				if( l_u32LogTimer > SD_TDC2SD_TIME )
				{
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+4,(uint8 *)&l_u32LogNum, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+8,(uint8 *)&l_u32NextLog, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+12,(uint8 *)&l_u32CurLog, 4);
					//
					memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
					FSDFormLogCtx( m_au8SDWrIxBuf, l_u32LogNum, l_u32NextLog, l_u32CurLog );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_SYSLOG, m_au8SDWrIxBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					//
					l_u32LogTimer = 0;
				}
			
				
				
				l_u8IsWrFlag = 1;
#if OS_Q_QUERY_EN > 0
				// ����ͳ�ƣ������д���ݣ����־��1
				memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
				l_u32Ret = OSQQuery( g_pQSDDebugInfoWrite, &l_cQData );  
			 	if( OS_NO_ERR == l_u32Ret )
				{
					if( l_cQData.OSNMsgs > 0 )
					{
						l_u8IsWrFlag = 1;
					}
					else
					{
						l_u8IsWrFlag = 0;
					}
				}
#else
				// ��������ֱ����1��ÿ�ζ����ж�
				l_u8IsWrFlag = 1;
#endif
				
				if( l_u8IsWrFlag )
				{
					// ����������
					// �жϹ������������Ϊ�µ����������ʼ���������д����ֱ�Ӹ�ֵ			
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+4,(uint8 *)&l_u32LogNum, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+8,(uint8 *)&l_u32NextLog, 4);
					ReadC256( SD_ADDR+SD_TDC_SIZE*2+12,(uint8 *)&l_u32CurLog, 4);
		
					while(1)
					{
						OSTimeDly(1);
						//
						l_pcLogData = (CComBuf *)OSQAccept(g_pQSDDebugInfoWrite);	
						if( l_pcLogData != (CComBuf *)0 )
						{  		
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
		
							l_u32Ret = FSDFormLogBuf( m_au8SDWrIxBuf, l_pcLogData->m_au8DataBuf, l_pcLogData->m_u16Len );
							if( SUCCESS != l_u32Ret )
							{
								continue;
							}
		
							l_u32WrAdd = SD_GETNEXT_SECTOR( SD_DBGINFO_SEC_DATA_BGN, l_u32NextLog );		
							l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
								continue;
							}
							//
							l_u32NextLog++;
							l_u32NextLog = l_u32NextLog % SD_LOG_MAX_NUM;
							//
							if( l_u32LogNum < SD_LOG_MAX_NUM )
							{
								l_u32LogNum++;
							}
							else
							{
								l_u32LogNum = SD_LOG_MAX_NUM;
								l_u32CurLog++;
								l_u32CurLog = l_u32CurLog % SD_LOG_MAX_NUM;
							}			
										
						}
						else
						{
							// ���湦����������
							WriteC256(SD_ADDR+SD_TDC_SIZE*2+4,(uint8 *)&l_u32LogNum,4);
							WriteC256(SD_ADDR+SD_TDC_SIZE*2+8,(uint8 *)&l_u32NextLog,4);
							WriteC256(SD_ADDR+SD_TDC_SIZE*2+12,(uint8 *)&l_u32CurLog,4);
							break;
						}
						
					}
		
				} // end of if( l_u8IsWrFlag )
			}

			// ���ι���
			if(SD_WAVE == (PROMODPARAM.m_u16SDSaveEn&SD_WAVE))
			{
				l_u8IsWrFlag = 1;
#if OS_Q_QUERY_EN > 0
				// ����ͳ�ƣ������д���ݣ����־��1
				memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
				l_u32Ret = OSQQuery( g_pQSDWaveWrite, &l_cQData );  
			 	if( OS_NO_ERR == l_u32Ret )
				{
					if( l_cQData.OSNMsgs > 0 )
					{
						l_u8IsWrFlag = 1;
					}
					else
					{
						l_u8IsWrFlag = 0;
					}
				}
#else
				// ��������ֱ����1��ÿ�ζ����ж�
				l_u8IsWrFlag = 1;
#endif
				
				if( l_u8IsWrFlag )
				{
					// ����������
					// �жϹ������������Ϊ�µ����������ʼ���������д����ֱ�Ӹ�ֵ			
					memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDRdBuf );
					if( SD_NO_ERR == l_u32Ret )
					{
						l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
						if( ERR_SD_NOTNEW == l_u32Ret )
						{
							// ֱ�Ӹ�ֵ
							l_u32ZTNum = (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+3];
							l_u32NextZT = (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+3];
							l_u32CurZT = (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+1]<<16) 
										+ (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_CURZT+3];
						}
						else if( ERR_SD_NEW_SECTOR == l_u32Ret )
						{
							l_u32ZTNum = 0;
							l_u32NextZT = 0;			
							l_u32CurZT = 0;	
							// ����
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							FSDFormWaveCtx( m_au8SDWrIxBuf, l_u32ZTNum, l_u32NextZT, l_u32CurZT );
							l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
							}									
						}
						else
						{
						}
					}
		
					while(1)
					{
						OSTimeDly(10);
						//
						l_pcZTData = (sndFrame_Struct *)OSQAccept(g_pQSDWaveWrite);	
						if( l_pcZTData != (sndFrame_Struct *)0 )
						{  		
							l_u8Tempi = 0;
							for( l_u8Tempi = 0; l_u8Tempi<l_pcZTData->u8Cnt; l_u8Tempi++ )
							{			
								memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
								memset( g_u8SDWrBuf2, 0, SD_SECTOR_LEN );
		
								l_u32Ret = FSDFormWaveBuf( g_u8SDWrBuf, g_u8SDWrBuf2, l_pcZTData->au8data[l_u8Tempi], l_pcZTData->au32len[l_u8Tempi] );
								if( SUCCESS != l_u32Ret )
								{
									continue;
								}
								//
								l_u32WrAdd = SD_GETNEXTWAVE_SECTOR(SD_WAVE_SEC_DATA_BGN,l_u32NextZT);
								l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd, g_u8SDWrBuf );		
								if( SD_NO_ERR != l_u32Ret )
								{
									continue;
								}
								//
								l_u32Ret = SD_WriteBlock( &sd_info, l_u32WrAdd+1, g_u8SDWrBuf2 );		
								if( SD_NO_ERR != l_u32Ret )
								{
									continue;
								}
								//
								l_u32NextZT = (l_u32NextZT+1) % SD_WAVE_ZT_MAX_NUM;
								if( l_u32ZTNum < SD_WAVE_ZT_MAX_NUM )
								{
									l_u32ZTNum++;
								}
								else
								{
									l_u32CurZT = (l_u32CurZT+1) % SD_WAVE_ZT_MAX_NUM;
								}
								// 
								OSTimeDly(1);          	
							}			
						}
						else
						{
							// ���湦����������
							memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
							FSDFormWaveCtx( m_au8SDWrIxBuf, l_u32ZTNum, l_u32NextZT, l_u32CurZT );
							l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, m_au8SDWrIxBuf );		
							if( SD_NO_ERR != l_u32Ret )
							{
							}
							break;
						}
						
					}
		
		
				}
			}// end of if(SD_WAVE == (PROMODPARAM.m_u16SDSaveEn&SD_WAVE))

			
			
			// ��������
		
		}
		/////////////////////////��SD��д������Ϣ
		if(OSSemAccept(g_pSDSaveForJD) > 0)
		{
			if(l_u8SDInsert) //��SD��д��
			{
				GET8563(&l_sDevCurTime);
				FJDInfToSaveSend(g_u16SaveTimeNum);
				FJD61ToSaveSend(g_u16SaveTimeNum);
				FJD62ToSaveSend(g_u16SaveTimeNum);
				FJD68ToSaveSend(g_u16SaveTimeNum);
			}
			else
			{
			}
		}
		/////////////////////////

		/** ����������� **/		
	}
}

/*********************************************************************************************************
** Function name:		FCheckSD
** Descriptions:		���SD��
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FCheckSD( uint8 *p_pu8Data, uint32 p_u32Len )
{
	uint32	l_u32Ret = 0;		  	// ����ֵ
	uint8	l_u8Tempi = 0;		  	// ѭ������
	uint32	l_u32IsNew = 0;		// 1��ʾ�µ�SD��

	//
	for( l_u8Tempi=0; l_u8Tempi<p_u32Len; l_u8Tempi++ )
	{
		if( 0xAA != (*(p_pu8Data+l_u8Tempi)) )
		{
			l_u32IsNew = 1;
			break;
		}
	}

	//
	if( l_u32IsNew )
	{
		if( p_u32Len == SD_NEW_CARD_FLAG_LEN )
		{
			l_u32Ret = ERR_SD_NEW_CARD;
		}
		else if( p_u32Len == SD_NEW_SECTOR_FLAG_LEN )
		{
			l_u32Ret = ERR_SD_NEW_SECTOR;
		}
		else
		{
		
		}
	}
	else
	{
		l_u32Ret = ERR_SD_NOTNEW;
	}
	//
	return l_u32Ret;
}

/*********************************************************************************************************
** Function name:		FSDFormOffnetBuf
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormOffnetBuf( uint8 *p_pu8Des1, uint8 *p_pu8Des2, uint8 *p_pu8FrameData, uint32 p_u32Len )
{
	 uint8	*l_pu8Des1 = p_pu8Des1;
	 uint8	*l_pu8Des2 = p_pu8Des2;
	 uint8	*l_pu8Ori = p_pu8FrameData;

	 if( p_u32Len > SD_OFFNET_SEC_DATA_LEN*2 )
	 {
	 	return ERR_SD_FRAMLEN_OVER;
	 }
	 
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 //
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = *(l_pu8Ori+7);
	 //
	 *(l_pu8Des1++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des1++) = p_u32Len & 0xFF;
	 // ֡���ͳ�����������渳ֵ
	 
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 //
	 *(l_pu8Des2++) = 0xBB;
	 *(l_pu8Des2++) = 0xBB;
	 *(l_pu8Des2++) = 0xBB;
	 *(l_pu8Des2++) = *(l_pu8Ori+7);
	 //
	 *(l_pu8Des2++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des2++) = p_u32Len & 0xFF;
	 // ֡���ͳ�����������渳ֵ

	 //
	 l_pu8Des1 = p_pu8Des1+SD_OFFNET_SEC_FRAM_DATA;
	 l_pu8Des2 = p_pu8Des2+SD_OFFNET_SEC_FRAM_DATA;
	 if( p_u32Len <= SD_OFFNET_SEC_DATA_LEN )
	 {
	 	memcpy( l_pu8Des1, l_pu8Ori, p_u32Len );
	 }
	 else
	 {
	 	memcpy( l_pu8Des1, l_pu8Ori, SD_OFFNET_SEC_DATA_LEN );
		memcpy( l_pu8Des2, l_pu8Ori+SD_OFFNET_SEC_DATA_LEN, p_u32Len-SD_OFFNET_SEC_DATA_LEN );
	 }


	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormVehBuf
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormVehBuf( uint8 *p_pu8Des1, uint8 *p_pu8FrameData, uint32 p_u32Len )
{
	 uint8	*l_pu8Des1 = p_pu8Des1;
	 uint8	*l_pu8Ori = p_pu8FrameData;

	 if( p_u32Len > SD_OFFNET_SEC_DATA_LEN )
	 {
	 	return ERR_SD_FRAMLEN_OVER;
	 }
	 
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 //
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = 0xBB;
	 *(l_pu8Des1++) = 0xBB;
	 //
	 *(l_pu8Des1++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des1++) = p_u32Len & 0xFF;
	 

	 l_pu8Des1 = p_pu8Des1+SD_OFFNET_SEC_FRAM_DATA;
	 memcpy( l_pu8Des1, l_pu8Ori, p_u32Len );


	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormLogBuf
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormLogBuf( uint8 *p_pu8Des1, uint8 *p_pu8FrameData, uint32 p_u32Len )
{
	 uint8	*l_pu8Des1 = p_pu8Des1;
	 uint8	*l_pu8Ori = p_pu8FrameData;

	 if( p_u32Len > SD_OFFNET_SEC_DATA_LEN )
	 {
	 	return ERR_SD_FRAMLEN_OVER;
	 }
	 
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 //
	 *(l_pu8Des1++) = 0xCC;
	 *(l_pu8Des1++) = 0xCC;
	 *(l_pu8Des1++) = 0xCC;
	 *(l_pu8Des1++) = 0xCC;
	 //
	 *(l_pu8Des1++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des1++) = p_u32Len & 0xFF;
	 

	 l_pu8Des1 = p_pu8Des1+SD_DBGINFO_SEC_LOG_DATA;
	 memcpy( l_pu8Des1, l_pu8Ori, p_u32Len );


	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormWaveBuf
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormWaveBuf( uint8 *p_pu8Des1, uint8 *p_pu8Des2, uint8 *p_pu8FrameData, uint32 p_u32Len )
{
	 uint8	*l_pu8Des1 = p_pu8Des1;
	 uint8	*l_pu8Des2 = p_pu8Des2;
	 uint8	*l_pu8Ori = p_pu8FrameData;

	 if( p_u32Len > SD_OFFNET_SEC_DATA_LEN*2 )
	 {
	 	return ERR_SD_FRAMLEN_OVER;
	 }
	 
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 *(l_pu8Des1++) = 0xAA;
	 //
	 *(l_pu8Des1++) = 0xDD;
	 *(l_pu8Des1++) = 0xDD;
	 *(l_pu8Des1++) = 0xDD;
	 *(l_pu8Des1++) = 0xDD;
	 //
	 *(l_pu8Des1++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des1++) = p_u32Len & 0xFF;
	 
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 *(l_pu8Des2++) = 0xAA;
	 //
	 *(l_pu8Des2++) = 0xDD;
	 *(l_pu8Des2++) = 0xDD;
	 *(l_pu8Des2++) = 0xDD;
	 *(l_pu8Des2++) = 0xDD;
	 //
	 *(l_pu8Des2++) = (p_u32Len>>8) & 0xFF;
	 *(l_pu8Des2++) = p_u32Len & 0xFF;

	 //
	 l_pu8Des1 = p_pu8Des1+SD_WAVE_SEC_ZT_DATA;
	 l_pu8Des2 = p_pu8Des2+SD_WAVE_SEC_ZT_DATA;
	 if( p_u32Len <= SD_WAVE_SEC_DATA_LEN )
	 {
	 	memcpy( l_pu8Des1, l_pu8Ori, p_u32Len );
	 }
	 else
	 {
	 	memcpy( l_pu8Des1, l_pu8Ori, SD_WAVE_SEC_DATA_LEN );
		memcpy( l_pu8Des2, l_pu8Ori+SD_OFFNET_SEC_DATA_LEN, p_u32Len-SD_WAVE_SEC_DATA_LEN );
	 }


	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormOffnetCtx
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormOffnetCtx( uint8 *p_pu8Des, CSDOffnetIx*	p_cOffnetIx )
{
	uint8	*l_pu8Des = p_pu8Des;

	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	//
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetVehNum)>>24) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetVehNum)>>16) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetVehNum)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetVehNum)    ) & 0xFF;
	
	//
	*(l_pu8Des++) = (SD_OFFNET_SEC_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_BGN    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_OFFNET_SEC_END>>24) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_END>>16) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_END>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_END    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_OFFNET_SEC_DATA_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_DATA_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_DATA_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_OFFNET_SEC_DATA_BGN    ) & 0xFF;
	
	//
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSec)>>24) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSec)>>16) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSec)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSec)    ) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSub)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetWrSub)    ) & 0xFF;
	//
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSec)>>24) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSec)>>16) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSec)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSec)    ) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSub)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32OffnetRdSub)    ) & 0xFF;

	//
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32VehIx)>>24) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32VehIx)>>16) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32VehIx)>> 8) & 0xFF;
	*(l_pu8Des++) = ((p_cOffnetIx->m_u32VehIx)    ) & 0xFF;
	//
	return SUCCESS;
}



/*********************************************************************************************************
** Function name:		FSDFormVehCtx
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormVehCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur )
{
	uint8	*l_pu8Des = p_pu8Des;

	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	//
	*(l_pu8Des++) = (p_u32Num>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Num    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_VEH_SEC_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_BGN    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_VEH_SEC_END>>24) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_END>>16) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_END>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_VEH_SEC_END    ) & 0xFF;

	
	//
	*(l_pu8Des++) = (p_u32Next>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Next    ) & 0xFF;
	//
	*(l_pu8Des++) = (p_u32Cur>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur    ) & 0xFF;
	
	//
	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormLogCtx
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormLogCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur )
{
	uint8	*l_pu8Des = p_pu8Des;

	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	//
	*(l_pu8Des++) = (p_u32Num>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Num    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_DBGINFO_SEC_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_BGN    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_DBGINFO_SEC_END>>24) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_END>>16) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_END>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_DBGINFO_SEC_END    ) & 0xFF;

	
	//
	*(l_pu8Des++) = (p_u32Next>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Next    ) & 0xFF;
	//
	*(l_pu8Des++) = (p_u32Cur>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur    ) & 0xFF;
	
	//
	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDFormWaveCtx
** Descriptions:		��֡
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131108	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDFormWaveCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur )
{
	uint8	*l_pu8Des = p_pu8Des;

	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	//
	*(l_pu8Des++) = (p_u32Num>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Num    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_WAVE_SEC_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_BGN    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_WAVE_SEC_END>>24) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_END>>16) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_END>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_WAVE_SEC_END    ) & 0xFF;

	
	//
	*(l_pu8Des++) = (p_u32Next>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Next    ) & 0xFF;
	//
	*(l_pu8Des++) = (p_u32Cur>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur    ) & 0xFF;
	
	//
	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FSDCheckAntiShake
** Descriptions:		�������
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20131127	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDCheckAntiShake( sd_struct *sds )
{
	uint8	l_u8Tempi = 0;
	//uint8	l_u8SDInsertStatus = 0;			// ����״̬
	uint8	l_u8AntiShakeCnt = 0;			// ��������
	uint8	l_u8SDInsert;

	for( l_u8Tempi=0; l_u8Tempi<SD_ANTISHAKE_CNT; l_u8Tempi++ )
	{
		l_u8SDInsert = SdHal_CheckCard( sds );
		if( l_u8SDInsert )
		{
			l_u8AntiShakeCnt++;
		}
	}
	//
	if( l_u8AntiShakeCnt >= SD_ANTISHAKE_CNT-1 )
	{
		return 1;	 // ����
	}
	else
	{
		return 0;	 // δ����
	}
}

/*********************************************************************************************************
** Function name:		FSDCptAddr
** Descriptions:		���ݿռ���䣬������ʼ��ַ
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140422	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDCptAddr( uint8*buf, int len )
{
	uint8	l_u8Tempi = 0; // ѭ������
	uint16	l_u16Sum = 0;  // ��
	uint16	l_u16TempSum = 0;	 // ��ʱ���
	

	for( l_u8Tempi=0; l_u8Tempi<len; l_u8Tempi++ )
	{
		l_u16Sum += buf[l_u8Tempi];	
	}
	//
	if( 100==l_u16Sum )
	{
		// ������100%
	}
	else
	{
		// ����100%����ֵΪĬ��ֵ 1,15,30,25,19,10
		memset(buf, 0, len);
		buf[0] = 1;
		buf[1] = 15;
		buf[2] = 30;
		buf[3] = 25;
		buf[4] = 19;
		buf[5] = 10;
	}
	
	//
	if(PROMODPARAM.m_u8SDCapa == 8)
	{
		l_u16TempSum = 0;
		m_u32Ix_Addr_Begin = 0;
		l_u16TempSum += buf[0];
		m_u32Offnet_Addr_Begin = SDCAPA*l_u16TempSum/100;	// �������濪ʼλ��
		m_u32Offnet_Data_Begin = SDCAPA*(l_u16TempSum+1)/100;	// 1%�Ŀռ���������
		l_u16TempSum += buf[1];
		m_u32VehInfo_Addr_Begin = SDCAPA*l_u16TempSum/100;
		m_u32VehInfo_Data_Begin = m_u32VehInfo_Addr_Begin;	   //  ������������Ϊ1��������һ��
		l_u16TempSum += buf[2];
		m_u32Log_Addr_Begin = SDCAPA*l_u16TempSum/100;
		m_u32Log_Data_Begin = m_u32Log_Addr_Begin;
		l_u16TempSum += buf[3];
		m_u32Wave_Addr_Begin = SDCAPA*l_u16TempSum/100;
		m_u32Wave_Data_Begin = m_u32Wave_Addr_Begin;
		l_u16TempSum += buf[4];
		g_u32JDInfo_Addr_Begin = SDCAPA*l_u16TempSum/100;
		l_u16TempSum += buf[5];
		//
		m_u32Ix_Addr_End = m_u32Offnet_Addr_Begin-1;
		m_u32Offnet_Addr_End = m_u32VehInfo_Addr_Begin-1;	// �����������λ��
		m_u32VehInfo_Addr_End = m_u32Log_Addr_Begin-1;
		m_u32Log_Addr_End = m_u32Wave_Addr_Begin-1;
		m_u32Wave_Addr_End = g_u32JDInfo_Addr_Begin-1;
		g_u32JDInfo_Addr_End = SDCAPA*l_u16TempSum/100-1;
	}
	else if(PROMODPARAM.m_u8SDCapa > 8)
	{
		l_u16TempSum = 0;
		m_u32Ix_Addr_Begin = 0;
		l_u16TempSum += buf[0];
		m_u32Offnet_Addr_Begin = SDCAPA/2*l_u16TempSum/100;	// �������濪ʼλ��
		m_u32Offnet_Data_Begin = SDCAPA/2*(l_u16TempSum+1)/100;	// 1%�Ŀռ���������
		l_u16TempSum += buf[1];
		m_u32VehInfo_Addr_Begin = SDCAPA/2*l_u16TempSum/100;
		m_u32VehInfo_Data_Begin = m_u32VehInfo_Addr_Begin;	   //  ������������Ϊ1��������һ��
		l_u16TempSum += buf[2];
		m_u32Log_Addr_Begin = SDCAPA/2*l_u16TempSum/100;
		m_u32Log_Data_Begin = m_u32Log_Addr_Begin;
		l_u16TempSum += buf[3];
		m_u32Wave_Addr_Begin = SDCAPA/2*l_u16TempSum/100;
		m_u32Wave_Data_Begin = m_u32Wave_Addr_Begin;
		l_u16TempSum += buf[4];
		g_u32JDInfo_Addr_Begin = SDCAPA/2*l_u16TempSum/100;
		l_u16TempSum += buf[5];
		//
		m_u32Ix_Addr_End = m_u32Offnet_Addr_Begin-1;
		m_u32Offnet_Addr_End = m_u32VehInfo_Addr_Begin-1;	// �����������λ��
		m_u32VehInfo_Addr_End = m_u32Log_Addr_Begin-1;
		m_u32Log_Addr_End = m_u32Wave_Addr_Begin-1;
		m_u32Wave_Addr_End = g_u32JDInfo_Addr_Begin-1;
		g_u32JDInfo_Addr_End = SDCAPA/2*l_u16TempSum/100-1;

		g_u32JD39BaseAdr = SDCAPA/2+5000;
		g_u32JD61BaseAdr = SDCAPA/2+8088000;
		g_u32JD62BaseAdr = SDCAPA/2+10066000;
		g_u32JD68BaseAdr = SDCAPA/2+11778000;


	}
	
	//
	return 0;
}

/*********************************************************************************************************
** Function name:		FSDNewCardInit
** Descriptions:		���ݳ�ʼ��
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140422	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSDNewCardInit( void )
{
	uint32	l_u32Ret = 0; // ����ֵ
	uint8	l_u8Tempi = 0;// ѭ������
	
	// ����⵽���¿�ʱ������SD�����ݳ�ʼ�����������㹦�ܷ�������ʼ����������
	memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
	l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_ZONG, g_u8SDRdBuf );
	if( SD_NO_ERR == l_u32Ret )
	{
		l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_CARD_FLAG_LEN );
		if( ERR_SD_NEW_CARD == l_u32Ret )
		{
			// Ϊ�¿�
			memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
			for( l_u8Tempi = 0; l_u8Tempi<SD_NEW_CARD_FLAG_LEN; l_u8Tempi++ )
			{
				g_u8SDWrBuf[l_u8Tempi] = 0xAA;
			}
			l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_ZONG, g_u8SDWrBuf );		
			if( SD_NO_ERR != l_u32Ret )
			{
			}
			
				
		}
	}

	FSDCptAddr( PROMODPARAM.m_au8SDFuncSpaceper, 16);
	
	return 0;
}


/*********************************************************************************************************
** Function name:		FSysn
** Descriptions:		��������ͬ��SD���������������
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140422	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSysnVehIx( void )
{
	uint32		l_u32VehNum = 0;					// ����־��
	uint32		l_u32VehNext = 0;					// ��һ��־�Ĵ洢λ��
	uint32		l_u32VehCur = 0;					// ��ǰ��־�Ĵ洢λ��

	uint32		l_u32Ret = 0;						// ���ر�־
	
	memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
	l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDRdBuf );
	if( SD_NO_ERR == l_u32Ret )
	{
		l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
		if( ERR_SD_NOTNEW == l_u32Ret )
		{
			// ֱ�Ӹ�ֵ
			l_u32VehNum = (g_u8SDRdBuf[SD_CTX_VEH_SUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_SUM+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_VEH_SUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_SUM+3];
			l_u32VehNext = (g_u8SDRdBuf[SD_CTX_VEH_NEXT+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NEXT+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_VEH_NEXT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NEXT+3];
			l_u32VehCur = (g_u8SDRdBuf[SD_CTX_VEH_CUR+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_CUR+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_VEH_CUR+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_CUR+3];
		}
		else if( ERR_SD_NEW_SECTOR == l_u32Ret )
		{
			l_u32VehNum = 0;
			l_u32VehNext = 0;			
			l_u32VehCur = 0;
			
			// ��֡��д������
			memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
			FSDFormLogCtx( m_au8SDWrIxBuf, l_u32VehNum, l_u32VehNext, l_u32VehCur );
			l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_VEHINFO, m_au8SDWrIxBuf );		
			if( SD_NO_ERR != l_u32Ret )
			{
			}			
		}
		else
		{
		}
	}

	WriteC256(SD_ADDR+SD_TDC_SIZE*1+4,(uint8 *)&l_u32VehNum,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*1+8,(uint8 *)&l_u32VehNext,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*1+12,(uint8 *)&l_u32VehCur,4);


	return 0;	
}

/*********************************************************************************************************
** Function name:		FSysn
** Descriptions:		��������ͬ��SD���������������
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140422	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSysnLogIx( void )
{
	uint32		l_u32LogNum = 0;					// ����־��
	uint32		l_u32NextLog = 0;					// ��һ��־�Ĵ洢λ��
	uint32		l_u32CurLog = 0;					// ��ǰ��־�Ĵ洢λ��

	uint32		l_u32Ret = 0;						// ���ر�־
	
	memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
	l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDRdBuf );
	if( SD_NO_ERR == l_u32Ret )
	{
		l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
		if( ERR_SD_NOTNEW == l_u32Ret )
		{
			// ֱ�Ӹ�ֵ
			l_u32LogNum = (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+3];
			l_u32NextLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+3];
			l_u32CurLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+3];
		}
		else if( ERR_SD_NEW_SECTOR == l_u32Ret )
		{
			l_u32LogNum = 0;
			l_u32NextLog = 0;			
			l_u32CurLog = 0;
			
			// ��֡��д������
			memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
			FSDFormLogCtx( m_au8SDWrIxBuf, l_u32LogNum, l_u32NextLog, l_u32CurLog );
			l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_SYSLOG, m_au8SDWrIxBuf );		
			if( SD_NO_ERR != l_u32Ret )
			{
			}			
		}
		else
		{
		}
	}

	WriteC256(SD_ADDR+SD_TDC_SIZE*2+4,(uint8 *)&l_u32LogNum,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*2+8,(uint8 *)&l_u32NextLog,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*2+12,(uint8 *)&l_u32CurLog,4);


	return 0;	
}

/*********************************************************************************************************
** Function name:		FSysn
** Descriptions:		��������ͬ��SD���������������
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140422	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FSysnWaveIx( void )
{
	uint32		l_u32WaveNum = 0;					// ����־��
	uint32		l_u32WaveNext = 0;					// ��һ��־�Ĵ洢λ��
	uint32		l_u32WaveCur = 0;					// ��ǰ��־�Ĵ洢λ��

	uint32		l_u32Ret = 0;						// ���ر�־
	
	memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
	l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDRdBuf );
	if( SD_NO_ERR == l_u32Ret )
	{
		l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
		if( ERR_SD_NOTNEW == l_u32Ret )
		{
			// ֱ�Ӹ�ֵ
			l_u32WaveNum = (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+3];
			l_u32WaveNext = (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+3];
			l_u32WaveCur = (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_CURZT+3];
		}
		else if( ERR_SD_NEW_SECTOR == l_u32Ret )
		{
			l_u32WaveNum = 0;
			l_u32WaveNext = 0;			
			l_u32WaveCur = 0;
			
			// ��֡��д������
			memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
			FSDFormLogCtx( m_au8SDWrIxBuf, l_u32WaveNum, l_u32WaveNext, l_u32WaveCur );
			l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, m_au8SDWrIxBuf );		
			if( SD_NO_ERR != l_u32Ret )
			{
			}			
		}
		else
		{
		}
	}

	WriteC256(SD_ADDR+SD_TDC_SIZE*3+4,(uint8 *)&l_u32WaveNum,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*3+8,(uint8 *)&l_u32WaveNext,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*3+12,(uint8 *)&l_u32WaveCur,4);


	return 0;	
}

uint32 FSysnJDIx(void)
{
	uint32		l_u32JDNum = 0;					// ����־��
	uint32		l_u32JDNext = 0;					// ��һ��־�Ĵ洢λ��
	uint32		l_u32JDCur = 0;					// ��ǰ��־�Ĵ洢λ��

	uint32		l_u32Ret = 0;						// ���ر�־
	
	memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
	l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_JD, g_u8SDRdBuf );
	if( SD_NO_ERR == l_u32Ret )
	{
		l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
		if( ERR_SD_NOTNEW == l_u32Ret )
		{
			// ֱ�Ӹ�ֵ
			l_u32JDNum = (g_u8SDRdBuf[SD_CTX_JD_INFNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_JD_INFNUM+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_JD_INFNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_JD_INFNUM+3];
			l_u32JDNext = (g_u8SDRdBuf[SD_CTX_JD_NEXTINF+0]<<24) + (g_u8SDRdBuf[SD_CTX_JD_NEXTINF+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_JD_NEXTINF+2]<<8 ) + g_u8SDRdBuf[SD_CTX_JD_NEXTINF+3];
			l_u32JDCur = (g_u8SDRdBuf[SD_CTX_JD_CURINF+0]<<24) + (g_u8SDRdBuf[SD_CTX_JD_CURINF+1]<<16) 
						+ (g_u8SDRdBuf[SD_CTX_JD_CURINF+2]<<8 ) + g_u8SDRdBuf[SD_CTX_JD_CURINF+3];
		}
		else if( ERR_SD_NEW_SECTOR == l_u32Ret )
		{
			l_u32JDNum = 0;
			l_u32JDNext = 0;			
			l_u32JDCur = 0;
			
			// ��֡��д������
			memset( m_au8SDWrIxBuf, 0, SD_SECTOR_LEN );
			FSDFormJDCtx( m_au8SDWrIxBuf, l_u32JDNum, l_u32JDNext, l_u32JDCur );
			l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, m_au8SDWrIxBuf );		
			if( SD_NO_ERR != l_u32Ret )
			{
			}			
		}
		else
		{
		}
	}

	WriteC256(SD_ADDR+SD_TDC_SIZE*4+4,(uint8 *)&l_u32JDNum,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*4+8,(uint8 *)&l_u32JDNext,4);
	WriteC256(SD_ADDR+SD_TDC_SIZE*4+12,(uint8 *)&l_u32JDCur,4);

	return 0;
}

uint32 FSDFormJDCtx( uint8 *p_pu8Des, uint32 p_u32Num, uint32 p_u32Next, uint32 p_u32Cur )
{
	uint8	*l_pu8Des = p_pu8Des;

	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	*(l_pu8Des++) = 0xAA;
	//
	*(l_pu8Des++) = (p_u32Num>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Num>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Num    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_JD_SEC_BGN>>24) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_BGN>>16) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_BGN>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_BGN    ) & 0xFF;
	//
	*(l_pu8Des++) = (SD_JD_SEC_END>>24) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_END>>16) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_END>> 8) & 0xFF;
	*(l_pu8Des++) = (SD_JD_SEC_END    ) & 0xFF;

	
	//
	*(l_pu8Des++) = (p_u32Next>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Next>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Next    ) & 0xFF;
	//
	*(l_pu8Des++) = (p_u32Cur>>24) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>>16) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur>> 8) & 0xFF;
	*(l_pu8Des++) = (p_u32Cur    ) & 0xFF;
	
	//
	return SUCCESS;
}



