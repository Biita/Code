/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskJDInf.C
** Last modified Date:  20110512
** Last Version:		1.0
** Descriptions:		时钟任务
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
#define	__TASKJDINF_C

#include "common.h"

#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam


/*********************************************************************************************************
** Function name:		TaskRec9
** Descriptions:		任务2，时钟任务
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
void  TaskRec9(void *tdata)						
{
	uint8	l_u8Err;
	uint16	l_u16SendLen; 


	tdata = tdata;

	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// 任务开始运行added by wujinlong 20130805
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T9 Recd TaskBegin[Timer]。\r\n",
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
		if(PROMODPARAM.m_u8EnableFlag[2] == 1)
		{
//			if( AxleloadType !=0 )
//			{					
//			}
			if((g_u8JDSendEna == 1) && FSDCheckAntiShake(&sd_info))
			{
				if(JDSYSPARAM.m_u8SendVehType)
				{
					if(g_u32JD39SaveSuccInd != g_u32JD39SendSuccInd)
					{
						FSend39Info(g_u32JD39SaveSuccInd, g_u32JD39SendSuccInd);
						OSTimeDly(200);
					}
				}
				FSend01Info(g_u32JDSDSaveInd, g_u32JDSDSaveIndSuc);
				OSTimeDly(200);
				FSend6FInfo(0x61,g_u32JD61SaveSuccInd, g_u32JD61SendSuccInd);
				OSTimeDly(200);
				FSend6FInfo(0x62,g_u32JD62SaveSuccInd, g_u32JD62SendSuccInd);
				OSTimeDly(200);
				FSend6FInfo(0x68,g_u32JD68SaveSuccInd, g_u32JD68SendSuccInd);
				OSTimeDly(200);
					
			}
			/////////////////////////////////////重传指令
			if(g_u32Resend01SDBgNum)
			{	
				FReSenddata(0x01, g_u32Resend01SDBgNum, g_u32Resend01SDEndNum);
				OSTimeDly(200);
			}
			if(g_u8Resend61Bg)
			{
				FReSenddata(0x61, g_u8Resend61Bg, g_u8Resend61Ed);
				OSTimeDly(200);
			}
			if(g_u8Resend62Bg)
			{
				FReSenddata(0x62, g_u8Resend62Bg, g_u8Resend62Ed);
				OSTimeDly(200);
			}
			if(g_u8Resend68Bg)
			{
				FReSenddata(0x68, g_u8Resend68Bg, g_u8Resend68Ed);
				OSTimeDly(200);
			}
			if( g_u32Resend39 )
			{
			    FReSend39data();
			}
			/////////////////////////////////////
		}
		OSTimeDly(100);	    	
	}  
}
