/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskDebugInfoOutput.c
** Last modified Date:  20130812
** Last Version:		1.0
** Descriptions:		调试信息输出任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20130812
** Version:				1.0
** Descriptions:		调试信息通过串口输出
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__TASK_DEBUGINFO_OUTPUT_C

#include "common.h"
#define     PROMODPARAM				g_cProModParam
//
#if DEBUGINFO_TASK_SP_EN > 0

/*********************************************************************************************************
** Function name:		TaskDebugInfo
** Descriptions:		串口输出任务
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20130812	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/			   
void  TaskDebugInfo(void *tdata)
{
	// 变量
	CComBuf *l_pcSendPCCOM1 = (CComBuf *)0;	  	// 接收信息
	uint16	l_u16SendLen = 0;					// 格式化长度
	uint8	l_u8Err;
//	uint8 *l_u8Buf;
//	uint32  sndCnt = 0;

	//
	tdata = tdata;

	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u TDebug Recd Task Begin.\r\n", 
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


	while(1)
	{
		OSTimeDly(1000*OS_TICKS_PER_SEC/1000);

		/** 以下添加内容 **/
		while(1)
		{
			l_pcSendPCCOM1 = (CComBuf *)OSQAccept(g_pQSendPCCOM1);	
			if( l_pcSendPCCOM1!=(CComBuf *)0 )
			{
				if( l_pcSendPCCOM1->m_u16Len > 0 )
				{
#if DEBUGINFO_TASK_SP_EN > 0
					U1SendBytes(l_pcSendPCCOM1->m_au8DataBuf, l_pcSendPCCOM1->m_u16Len );
#endif
					/*为了测试网口接收调试信息20170302*/
// 					sndCnt =SendDataNet(SOCKET_RES,l_pcSendPCCOM1->m_au8DataBuf, l_pcSendPCCOM1->m_u16Len);
// 					if(sndCnt != 0)
// 						{
// 							g_u8IsConnectFlag = 1;
// 						}
// 						FReConnect( sndCnt, SOCKET_WT );
						
						
#if SD_DEBUGINFO_EN > 0
					OSQPost( g_pQSDDebugInfoWrite, l_pcSendPCCOM1 );
#endif
					OSTimeDly(2);
//					if((PROMODPARAM.m_u8EnableFlag[4] == 1) && (PROMODPARAM.m_u8EnableFlag[3] == 1))
//					{
//						l_u8Buf = g_u8SendBufDeg[g_u8DegIndex].au8data[0];
//						l_u8Buf[0] = 0xFF;
//						l_u8Buf[1] = 0xFF;
//						l_u8Buf[2] = 0x00;
//						l_u8Buf[3] = 0xE0;
//						l_u8Buf[4] = 0x00;
//						l_u8Buf[5] = ((l_pcSendPCCOM1->m_u16Len+11)>>8)&0xFF;
//						l_u8Buf[6] = (l_pcSendPCCOM1->m_u16Len+11)&0xFF;
//						memcpy(&l_u8Buf[7], l_pcSendPCCOM1->m_au8DataBuf, l_pcSendPCCOM1->m_u16Len);
//						l_u8Buf[l_pcSendPCCOM1->m_u16Len+8] = 0x00;
//						l_u8Buf[l_pcSendPCCOM1->m_u16Len+9] = 0x00;
//						AddCrc16(l_u8Buf, l_pcSendPCCOM1->m_u16Len+9);
//						g_u8SendBufDeg[g_u8DegIndex].au32len[0] = l_pcSendPCCOM1->m_u16Len+11;
//						OSQPost(g_pQJDSend, &g_u8SendBufDeg[g_u8DegIndex]);
//						g_u8DegIndex += 1;
//						g_u8DegIndex = (g_u8DegIndex%10);
////						U2SendBytes(l_u8SendBuf, l_pcSendPCCOM1->m_u16Len+11);
//					}
				}	
			}
			else
			{
				break;
			}
		}



		/** 以上添加内容 **/
	}
}

#endif
