/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskTimer.C
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
#define	__TASKTIMER_C

#include "common.h"

#define TASKTIMERDLY	500*OS_TICKS_PER_SEC/1000
#define	ONEMINNUM	2	
#define NO_LINKMAXTM	(60*2) 		// 10s

//#define MOTIONZEROTIMERCNT		10
#define MOTIONZEROTIMERCNT		1

#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
//static	uint8	m_u8TimeCnt = 0;
uint8 g_refresh_zero_cnt=0;
extern uint8    check_sendok_flag[MAX_SOCK_NUM];
/*********************************************************************************************************
** Function name:		TaskRec2
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
void  TaskRec2(void *tdata)						
{
   	int32	l_n32TemperatureAll = 0;
	int32	l_n32Temperature = 0;
	int16	l_n16TempCnt = 0;
//	static	uint8	l_u8LinkCnt = 0;
	uint8	l_u8Tempi = 0;//循环变量
	uint8	l_u8Tempj = 0;//循环变量
	uint8	l_u8Err;
	uint16  l_u16Temp;
	uint8  l_u8DigitalID;
	uint8  l_u8Count= 0;
	uint16	l_u16SendLen;
	uint8  l_u8ResetRecd = 0;
	uint8   l_u8ResetFlag = 0;
	uint16  l_u16Order02Cnt = 0;
	uint16  l_u16Order08Cnt = 0;
	uint8   l_u8Send02Flag = 0;
	uint8   l_u8UnRec02Cnt = 0;
	uint8   l_u8Buf[65];
	uint16  l_u16Rec02Cnt = 0;
	uint8   l_u8WiresULinkCnt = 0;
//	uint8   l_u8Buf1[16];
	uint8   l_u8DeviceErr;
	uint8   l_u8TongJiFlag = 0;
//	uint32  l_u32FpgaTime;

	uint16 l_u16BoardStateCheck=0;
	


	TIMER_PIB st_CPUUseOut;
//#if NOVEH_CLR_EN > 0
//	uint8	l_u8PreLasterState = 0;		// 前一时刻激光状态
//	uint16	l_u16CntBenginFlag = 0;		// 计数标志
//#endif

//	uint8 	l_au8Data[10] = {0xFF, 0xFF, 0x01};

	tdata = tdata;

	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// 任务开始运行added by wujinlong 20130805
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T2 Recd TaskBegin[Timer].\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif



	timer_set(&st_CPUUseOut, 5);

	GET8563(&g_DevCurTime);
	g_u16TotalMinutesNew = g_DevCurTime.u8Hour * 60 + g_DevCurTime.u8Minute;
	g_u16TotalMinutesOld = g_u16TotalMinutesNew;
	g_u16SaveTimeNum = g_u16TotalMinutesNew/g_u8PeriodTOSave;
	g_u8JDInitFlag = 1;
	GET8563(&l_sDevCurTime);

	while(1)
	{
		OSTimeDly(TASKTIMERDLY);


		if(timer_expired(&st_CPUUseOut))
		{
#if DEBUGINFO_TASK_SP_EN > 0
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			if(l_u8Count<=11)
			{
				l_u8Count++;
				if(g_u16DigitalZTNum>=36)
				{
					g_u16DigitalZTNum = 0;
				}
				 
				l_u16Temp = 8+(g_u16DigitalZTNum/12)*48+(g_u16DigitalZTNum%12)*4;
				l_u8DigitalID = g_u16DigitalZTNum;
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n%02u:%02u:%02u Zr=%u,%u,%u,%u,%u,%u,%u,%u,%u.",
											 g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
										 l_u8DigitalID, g_Setup.an32Zero[l_u16Temp], g_Setup.an32Zero[l_u16Temp+1], g_Setup.an32Zero[l_u16Temp+2], g_Setup.an32Zero[l_u16Temp+3],
										 g_Setup.an32ImmediateValue[l_u16Temp],g_Setup.an32ImmediateValue[l_u16Temp+1],g_Setup.an32ImmediateValue[l_u16Temp+2],g_Setup.an32ImmediateValue[l_u16Temp+3]);
				g_u16DigitalZTNum++;
			}
			else
			{
				l_u8Count=0;
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n%04u-%02u-%02u %02u:%02u:%02u T2 CPUUsage=%02u, %d, %d, %d, %d, %d, %02X,%d,%d,%02X.",
					               g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
								   OSCPUUsage, g_ErrMsgState, g_u8DevErr, g_u8NumUseLoopBoard, g_u32StartupTime,
								   g_u8DevErr,g_u8DeviceERR[0],g_u8RasterDaFeiCnt,g_u8RasterNoDaFeiCnt,g_u8RWNVFlag); //
			}
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			///////////////////////////////
			g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
			g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 42;				
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x09;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 42;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = OSCPUUsage;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = g_ErrMsgState;
			
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = PROMODPARAM.m_u8TriggerMode;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = g_u8DevErr;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = g_u8NumUseLoopBoard;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][19] = ((g_u32StartupTime>>24)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][20] = ((g_u32StartupTime>>16)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][21] = ((g_u32StartupTime>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][22] = ((g_u32StartupTime)&0xFF);

			for(l_u8Tempj = 0; l_u8Tempj < 15; l_u8Tempj ++)
			{
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][23+l_u8Tempj] = g_cJDSysParam.m_au8StationID[l_u8Tempj];
			}
			
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][38] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][39] = 0x00;
			
			AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
			OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
			g_u8CurNetDebugIndex++;
			g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
			//////////////////////////////
			OSSemPost(g_pSemSendThtouSP);
			
			
			
#endif												 
			timer_restart(&st_CPUUseOut);
		}


//#if NET_KEEPALIVE_EN > 0		
		FNetKeepalive();
		FNetCheckClose();		
//#endif


		//更新SN码
		//GetRTCTime(&g_DevCurTime);
		GET8563(&g_DevCurTime);
		//
//		g_u32SN	= 123456 + (g_DevCurTime.u8Second<<8)+g_DevCurTime.u8Second+((g_DevCurTime.u8Second&0x0f)<<8)+(g_DevCurTime.u8Second&0x0f);
//		g_u32SN	+= g_DevCurTime.u8Second*g_DevCurTime.u8Second;  
//		GET8563(&l_sDevCurTime);

		//更新故障状态(窄条,通信)
		S0_State ? 	(g_u8DeviceERR[0] &=~(1<<7)) : (g_u8DeviceERR[0] |= (1<<7));
		S1_State ? 	(g_u8DeviceERR[0] &=~(1<<6)) : (g_u8DeviceERR[0] |= (1<<6));
		S2_State ? 	(g_u8DeviceERR[0] &=~(1<<5)) : (g_u8DeviceERR[0] |= (1<<5));
		S3_State ? 	(g_u8DeviceERR[0] &=~(1<<4)) : (g_u8DeviceERR[0] |= (1<<4));  
//		g_u8DeviceERR[0] & 0xF0 ? (g_u8ERR |=(1<<4)) : (g_u8ERR &=~(1<<4));	   //通信故障

		//更新温度值		
		l_n32Temperature = ReadTemperature();		
		if (l_n16TempCnt < 16)
		{
			if (((l_n32Temperature<80)&&(l_n32Temperature>-40)))	//前两次温度必须小于80度，防止异常温度
			{
				l_n32TemperatureAll	= l_n32TemperatureAll+l_n32Temperature-g_an32TemperatureBuf[g_u8TemperatureIndex];
				g_an32TemperatureBuf[g_u8TemperatureIndex++] = l_n32Temperature;
				g_u8TemperatureIndex %= TEMPERATURELEN;					 

				l_n16TempCnt ++;								
				g_n32Temperature = l_n32TemperatureAll / l_n16TempCnt; 
			}
		}
		else if(abs(l_n32Temperature - g_n32Temperature) < 15)
		{
			l_n32TemperatureAll	= l_n32TemperatureAll+l_n32Temperature-g_an32TemperatureBuf[g_u8TemperatureIndex];
			g_an32TemperatureBuf[g_u8TemperatureIndex++] = l_n32Temperature;
			g_u8TemperatureIndex	%= 16;
			g_n32Temperature = l_n32TemperatureAll >> 4;	
		} 
		if (g_n32Temperature > 100) 
			g_n32Temperature	= 99;

		g_refresh_zero_cnt++;
		// 更新零点 wujinlong 20121227 //浩叔 20130416 全面简化
		if(g_refresh_zero_cnt==10)//5s从FPGA读一次零点值
		{
			g_refresh_zero_cnt=0;
		l_u16BoardStateCheck=FPGA_STATUS;
		for( l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++ )
		{
			// 更新0点	 //读FPGA零点值，64路值
			if(PROMODPARAM.m_u8TransmitSysEn>0)					
			{
					if(l_u16BoardStateCheck&(0x0100<<(l_u8Tempi/8)))
					{
						g_Setup.an32Zero[l_u8Tempi] = GETCURRENTZERO((l_u8Tempi>>3),(l_u8Tempi%8));	  //zhouhang 20130515
						g_Setup.an32ImmediateValue[l_u8Tempi]=GETCURRENTDATA((l_u8Tempi>>3),(l_u8Tempi%8));
					}
					else
					{
						g_Setup.an32Zero[l_u8Tempi]=0;
						g_Setup.an32ImmediateValue[l_u8Tempi]=0;
					}
			}
			else	 
			{
//					if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//						g_Setup.an32Zero[l_u8Tempi] = GETCURRENTZERO(aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi]);
//					else
						if(l_u16BoardStateCheck&(0x0100<<(l_u8Tempi/8)))
						{
							g_Setup.an32Zero[l_u8Tempi] = GETCURRENTZERO(l_u8Tempi/8, l_u8Tempi%8);  //wgh 20140415
							g_Setup.an32ImmediateValue[l_u8Tempi]=GETCURRENTDATA((l_u8Tempi>>3),(l_u8Tempi%8));
						}
						else
						{
							g_Setup.an32Zero[l_u8Tempi]=0;
							g_Setup.an32ImmediateValue[l_u8Tempi]=0;
						}
			}
		}
		for(l_u8Tempj=0;l_u8Tempj<7;l_u8Tempj++)
		{
			for( l_u8Tempi=0; l_u8Tempi<48; l_u8Tempi++ )
			{
			// 更新0点	 //读FPGA零点值，64路值
				if(PROMODPARAM.m_u8TransmitSysEn>0)		
				{					
					g_Setup.an32Zero[8+l_u8Tempj*48+l_u8Tempi] = GETCURRENTZERO_DIGITAL(1+l_u8Tempj,l_u8Tempi);	  //
					g_Setup.an32ImmediateValue[8+l_u8Tempj*48+l_u8Tempi]=GETCURRENTDATA_DIGITAL(1+l_u8Tempj,l_u8Tempi);
				}
				else	 
				{
//					if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//						g_Setup.an32Zero[l_u8Tempi] = GETCURRENTZERO(aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi]);
//					else
						g_Setup.an32Zero[8+l_u8Tempj*48+l_u8Tempi] = GETCURRENTZERO_DIGITAL(1+l_u8Tempj,l_u8Tempi);  //
						g_Setup.an32ImmediateValue[8+l_u8Tempj*48+l_u8Tempi]=GETCURRENTDATA_DIGITAL(1+l_u8Tempj,l_u8Tempi);
				}
			}
		}
		}
		//	g_intotimer_cnt++;
	//	if(g_intotimer_cnt==10)
		//{
			//g_intotimer_cnt=0;
// 		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Zero1 =%d,%d,%d,%d,%d,%d,%d,%d\r\n",
// 						          g_Setup.an32Zero[0],g_Setup.an32Zero[1],g_Setup.an32Zero[2],g_Setup.an32Zero[3],g_Setup.an32Zero[4],g_Setup.an32Zero[5],g_Setup.an32Zero[6],g_Setup.an32Zero[7]
// 								  );
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 		OSSemPost(g_pSemSendThtouSP);
// 		
// 		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Zero2 =%d,%d,%d,%d,%d,%d,%d,%d\r\n",
// 						          g_Setup.an32Zero[8],g_Setup.an32Zero[9],g_Setup.an32Zero[10],g_Setup.an32Zero[11],g_Setup.an32Zero[12],g_Setup.an32Zero[13],g_Setup.an32Zero[14],g_Setup.an32Zero[15]
// 								  );
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 		OSSemPost(g_pSemSendThtouSP);
// 			
// 		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Zero3 =%d,%d,%d,%d,%d,%d,%d,%d\r\n",
// 						          g_Setup.an32Zero[16],g_Setup.an32Zero[17],g_Setup.an32Zero[18],g_Setup.an32Zero[19],g_Setup.an32Zero[20],g_Setup.an32Zero[21],g_Setup.an32Zero[22],g_Setup.an32Zero[23]
// 								  );
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 		OSSemPost(g_pSemSendThtouSP);
// 		for(l_u8Tempi=0; l_u8Tempi<30; l_u8Tempi++)	
// 		{
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Zero%d-%d =%d,%d,%d,%d,%d,%d,%d,%d\r\n",
// 												l_u8Tempi/6+1,l_u8Tempi%6+1,g_Setup.an32Zero[24+8*l_u8Tempi+0],g_Setup.an32Zero[24+8*l_u8Tempi+1],g_Setup.an32Zero[24+8*l_u8Tempi+2],g_Setup.an32Zero[24+8*l_u8Tempi+3],g_Setup.an32Zero[24+8*l_u8Tempi+4],g_Setup.an32Zero[24+8*l_u8Tempi+5],g_Setup.an32Zero[24+8*l_u8Tempi+6],g_Setup.an32Zero[24+8*l_u8Tempi+7]
// 										);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
// 		}
// 		for(l_u8Tempi=0; l_u8Tempi<30; l_u8Tempi++)	
// 		{
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ImmeValue%d-%d =%d,%d,%d,%d,%d,%d,%d,%d\r\n",
// 												l_u8Tempi/6+1,l_u8Tempi%6+1,g_Setup.an32ImmediateValue[24+8*l_u8Tempi+0],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+1],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+2],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+3],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+4],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+5],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+6],g_Setup.an32ImmediateValue[24+8*l_u8Tempi+7]										);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
// 		}
	//}
		
		
//		for(l_u8Tempi=0;l_u8Tempi<8;l_u8Tempi++)
//		{
//			
//		}
//		if(g_u8TestFlag == 1)
//		{
//			g_u8TestFlag = 0;
//			ReadFPGAGlobalTime(l_u32FpgaTime);
//			#if DEBUGINFO_TASK_SP_EN > 0
//			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u，ID=%d,%d,%d。\r\n",
//						           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
//								   0,
//								   g_Setup.an32Zero[0],
//								   l_u32FpgaTime
//								  );
//			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//			g_u16CurDebugInd++;
//			g_u16CurDebugInd %= COM_NUM;
//			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//			OSSemPost(g_pSemSendThtouSP);
//			#endif
//		}

//#if LASER_UNNORMAL_EN > 0
//		if( (g_u8LasUnnFlag && PROMODPARAM.m_u8EnableFlag[0] == 0) || ((PROMODPARAM.m_u8EnableFlag[0] == 1) && (PROMODPARAM.m_u8TriggerMode == 4)&& g_u32ModChangeTime > 0))
//		{
//			g_u32LasUnnTimeCnt++;
//			if( g_u32LasUnnTimeCnt > 2*60*g_u32ModChangeTime )
//			{
//				g_u32LasUnnTimeCnt = 2*60*g_u32ModChangeTime + 1;
//			}
//			if( g_u32LasUnnTimeCnt > 2*60*g_u32ModChangeTime )
//			{
//				// 到达30min，置0
//				g_u8LasUnnFlag = 0;
//				g_u32LasUnnTimeCnt = 0;
//				g_u8IsLaserorCoilFlag = 1;
//				g_u8DeviceERR[0] = 0;
//				if(PROMODPARAM.m_u8EnableFlag[0] == 1)
//				{
//					//转换到激光模式
//					Loop2LaserConv();
//				}
//			}
//		}
//#endif
//		///////////////////////////查询线圈状态
//		if(g_u8IfRunSta == 1)
//		{
//			if(g_u8ZTStaSendFlag == 1)
//			{
//				g_u8ZTStaSendFlag = 0;
//				FSendCmd2PC_ZONG(1,ADJ_USESTAT);
//			}
//			if(g_u8NumUseLoopBoard == 0)
//			{
//				l_u8Buf1[0]=CoilIN_IO1;
//				l_u8Buf1[1]=CoilIN_IO2;
//				l_u8Buf1[2]=CoilIN_IO3;
//				l_u8Buf1[3]=CoilIN_IO4;
//				l_u8Buf1[4]=CoilIN_IO5;
//				l_u8Buf1[5]=CoilIN_IO6;
//				l_u8Buf1[6]=CoilIN_IO7;
//				l_u8Buf1[7]=CoilIN_IO8;
//				l_u8Buf1[8]=CoilIN_IO9;
//				l_u8Buf1[9]=CoilIN_IO10;
//				l_u8Buf1[10]=0;
//				l_u8Buf1[11]=0;
//				l_u8Buf1[12]=0;
//				l_u8Buf1[13]=0;
//				l_u8Buf1[14]=0;
//				l_u8Buf1[15]=0;
//			}
//			else
//			{
//				l_u8Buf1[0]=g_u8IOStateForBoard[0];
//				l_u8Buf1[1]=g_u8IOStateForBoard[1];
//				l_u8Buf1[2]=g_u8IOStateForBoard[2];
//				l_u8Buf1[3]=g_u8IOStateForBoard[3];
//				l_u8Buf1[4]=g_u8IOStateForBoard[4];
//				l_u8Buf1[5]=g_u8IOStateForBoard[5];
//				l_u8Buf1[6]=g_u8IOStateForBoard[6];
//				l_u8Buf1[7]=g_u8IOStateForBoard[7];
//				l_u8Buf1[8]=g_u8IOStateForBoard[8];
//				l_u8Buf1[9]=g_u8IOStateForBoard[9];
//				l_u8Buf1[10]=g_u8IOStateForBoard[10];
//				l_u8Buf1[11]=g_u8IOStateForBoard[11];
//				l_u8Buf1[12]=g_u8IOStateForBoard[12];
//				l_u8Buf1[13]=g_u8IOStateForBoard[13];
//				l_u8Buf1[14]=g_u8IOStateForBoard[14];
//				l_u8Buf1[15]=g_u8IOStateForBoard[15];
//			}
//			for(l_u8Tempi=0; l_u8Tempi<16; l_u8Tempi++)
//			{
//				if(l_u8Buf1[l_u8Tempi] == 0)
//				{
//					if(g_u8RunLoopSta[l_u8Tempi] != 2)
//					{
//						g_u8RunLoopSta[l_u8Tempi] = 1;
//					}
//				}
//				if(l_u8Buf1[l_u8Tempi] == 1)
//				{
//					if(g_u8RunLoopSta[l_u8Tempi] == 1)
//					{
//						g_u8RunLoopSta[l_u8Tempi] = 2;
//						FSendCmd2PC_ZONG(1,ADJ_USESTAT);
//					}
//				}
//				
//			}
//		}
//		//////////////////////////
		if(PROMODPARAM.m_au32Thres[12]>0 && g_DevCurTime.u8Second%PROMODPARAM.m_au32Thres[12] == 0 && l_u8TongJiFlag == 0)
		{
			l_u8TongJiFlag = 1;
			///////////////////////////////
			g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
			g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 26+ERR_LEN;				
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x1D;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 26+ERR_LEN;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
			
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14] = PROMODPARAM.m_u8TriggerMode;
			l_u8DeviceErr = g_u8DevErr;
			for( l_u8Tempi=1; l_u8Tempi<ERR_LEN; l_u8Tempi++ )
			{
				if(g_u8DeviceERR[l_u8Tempi]!=0)
				{
					if(l_u8DeviceErr != 0)
					{
						l_u8DeviceErr = l_u8DeviceErr+4;	
					}
					else
					{
						l_u8DeviceErr = 3;
					}
					break;
				}
			}
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][15] = l_u8DeviceErr;
			if((g_u8DeviceERR[0] & 0x0F)==0)
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 0;
			else
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][16] = 1;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][17] = g_u8IOERR[0];
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18] = g_u8IOERR[1];
			for( l_u8Tempi=1; l_u8Tempi<ERR_LEN; l_u8Tempi++ )
			{
				g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18+l_u8Tempi] = g_u8DeviceERR[l_u8Tempi];
			}
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][18+ERR_LEN] = ((g_u32VehCntForTest>>24)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][19+ERR_LEN] = ((g_u32VehCntForTest>>16)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][20+ERR_LEN] = ((g_u32VehCntForTest>>8)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][21+ERR_LEN] = ((g_u32VehCntForTest)&0xFF);
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][22+ERR_LEN] = 0x00;
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][23+ERR_LEN] = 0x00;
			
			AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
			OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
			g_u8CurNetDebugIndex++;
			g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
			//////////////////////////////
			g_u32VehCntForTest = 0;
		}
		else if(g_DevCurTime.u8Second%PROMODPARAM.m_au32Thres[12] != 0)
		{
			l_u8TongJiFlag = 0;
		}
		//////////////////////////
		if(g_u32AutoResetTime > 0 && (l_u8ResetFlag == 0)
			&& (g_DevCurTime.u8Hour == 3 && g_DevCurTime.u8Minute == 0 && (g_DevCurTime.u8Second < 5)))
		{
			l_u8ResetRecd ++;
			l_u8ResetFlag = 1;
			if(l_u8ResetRecd == g_u32AutoResetTime)
			{
				l_u8ResetRecd = 0;
				YBRestart();
			}
		}
		if(g_DevCurTime.u8Minute == 1)
		{
			l_u8ResetFlag = 0;
		}
		if((g_DevCurTime.u8Minute%10) == 0 && (g_DevCurTime.u8Second < 5))//每10分钟查一次窄条
		{
			for(l_u8Tempi=0; l_u8Tempi<7; l_u8Tempi++)
			{
				g_u8DeviceERR[l_u8Tempi+1]=0;
			}
			memset(g_u8ZTStateRe, 0, 64);
		}
		///////////////////////////////////// 交调信息01是否上传
		if(PROMODPARAM.m_u8EnableFlag[2] && JDSYSPARAM.m_u8TransMod == 1) //有线网
		{
			if(g_u8JDNetFirstUse == 0)
			{
				FUpOrderData(l_u8Buf, 2);//更新02包
				FSendData(l_u8Buf, 18);//发送02包
				OSTimeDly(100);
				if(g_u8JD02OrderRev == 0 && l_u16Rec02Cnt<40)
				{
					OSTimeDly(100);
					l_u16Rec02Cnt ++;
				}//等待02包返回
				if(g_u8JD02OrderRev)
				{
					l_u16Rec02Cnt = 0;
					g_u8JDSendEna = 1;
					g_u8RecOrderCnt = 0;
					g_u8JD02OrderRev = 0;
					g_u8JDNetFirstUse = 1;
					FUpOrderData(l_u8Buf, 8);//更新08包
					FSendData(l_u8Buf, 65);//发送08包
					OSTimeDly(400);
				}
				else
				{
					l_u16Rec02Cnt = 0;
				}

			}
		}
		else if(PROMODPARAM.m_u8EnableFlag[2] && (JDSYSPARAM.m_u8TransMod == 3 || JDSYSPARAM.m_u8TransMod == 2)) //无线网
		{
			if(g_u8JDSendEna == 0)
			{
				FUpOrderData(l_u8Buf, 2);//更新02包
				FSendData(l_u8Buf, 18);//发送02包
				OSTimeDly(100);
				if(g_u8JD02OrderRev == 0 && l_u16Rec02Cnt<40)//等待02包返回
				{
					OSTimeDly(100);
					l_u16Rec02Cnt++;
				}
				if(g_u8JD02OrderRev)
				{
					l_u16Rec02Cnt = 0;
					g_u8JDSendEna = 1;
					g_u8JD02OrderRev = 0;
					g_u8RecOrderCnt = 0;
					FUpOrderData(l_u8Buf, 8);//更新08包
					FSendData(l_u8Buf, 65);//发送08包
					OSTimeDly(400);
				}
				else
				{
					l_u16Rec02Cnt = 0;
				}
			}
		}
		/////////////////////////////////////
		/////////////////////////////////////交调信息其他指令交互
		if(PROMODPARAM.m_u8EnableFlag[2])
		{
			if( g_u8JDSendEna == 1 )
			{	
			    l_u16Order02Cnt++;
				l_u16Order08Cnt++;
				if( l_u16Order08Cnt >= 180 )
				{  
				    l_u16Order08Cnt = 0;
		 		    FUpOrderData(l_u8Buf, 8);//更新08包
					FSendData(l_u8Buf, 65);//发送08包
				}
				if( l_u16Order02Cnt >= 120 )
				{  
				    l_u16Order02Cnt = 0;
					if( g_u8RecOrderCnt > 0 )
					{
					    g_u8RecOrderCnt = 0;
					}
					else
					{
						FUpOrderData(l_u8Buf, 2);//更新02包
						FSendData(l_u8Buf, 18);//发送02包
		  				l_u8Send02Flag = 1;
								
					}  
				}
				if( l_u8Send02Flag == 1 )
				{
				    if( g_u8JD02OrderRev == 1 )
					{
					    g_u8JD02OrderRev       = 0;
						l_u8UnRec02Cnt   = 0;
						l_u8Send02Flag = 0;	 
						FUpOrderData(l_u8Buf, 8);//更新08包
						FSendData(l_u8Buf, 65);//发送08包 
					}
					else
					{
						l_u8UnRec02Cnt++;
					}
				    if( l_u8UnRec02Cnt >= 20 )	   // 20s 
					{
					    l_u8Send02Flag = 0;
						g_u8JDSendEna = 0;
						l_u8UnRec02Cnt   = 0;
						g_u8JDNetFirstUse = 0;
	//					Flag_NetConnect  = 0;
		                g_u8RecOrderCnt   = 0;
	//					Rx_35_39_CarNum  = 0;
	//					Rx_0A_01_TimeNum = 0;
					}
				}	   
			}
			/////////////////////////无线无连接提示
			if( g_u8JDSendEna == 0 )					 //  无线网络中断
			{
			     if(JDSYSPARAM.m_u8TransMod == 2 || JDSYSPARAM.m_u8TransMod == 3)	       // 无线网络 
				 {
				     l_u8WiresULinkCnt++;
					 if( l_u8WiresULinkCnt>=7 )
					 {
	//				     Relink_cnt1++;
	//					 Relink_cnt2++;
						 l_u8WiresULinkCnt = 0;
					 	 BeepON();
						 OSTimeDly(40);
						 BeepOFF();	
						 OSTimeDly(40);
					 }
				 }
			}
		}
		/////////////////////////


//	  	OSSemAccept(g_pSemTime);
//		OSSemPost(g_pSemTime);
//		OSSemAccept(g_psemScreenRefresh);
//		OSSemPost(g_psemScreenRefresh);	    	
	}  
}


/*********************************************************************************************************
** Function name:		FNetKeepalive
** Descriptions:		
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130910	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FNetKeepalive(void)
{
	uint32 	l_u32Ret = 0;
	uint32 	l_u32Ret2 = SUCCESS;
	int8	l_sk = 0;

  for( l_sk=0; l_sk<NET_PORT_NUM; l_sk++ )
	{
		l_u32Ret = FNetSendKeepalive( l_sk );
		if( ERR_NET_NOCON == l_u32Ret )
		{
			l_u32Ret2 = ERR_NET_NOCON;
// 			Socket_Listen( l_sk );	
// 			socket(l_sk, Sn_MR_TCP, ((S0_Port[0]<<8)|S0_Port[1]), 1);
// 			IINCHIP_WRITE(Sn_MR(l_sk),(uint16)(Sn_MR_TCP|Sn_MR_ND)); // set Sn_MR with protocol & flag
// // 			IINCHIP_WRITE(Sn_IMR(l_sk),0x001F);
// 			setSn_CR(l_sk, Sn_CR_OPEN);      // open s-th SOCKET
			IINCHIP_WRITE(Sn_CR(l_sk), Sn_CR_CLOSE);		/* ????,???????? */
			IINCHIP_WRITE(Sn_CR(l_sk), Sn_CR_OPEN);
// 			IINCHIP_WRITE(Sn_CR(l_sk), Sn_CR_LISTEN);	
			check_sendok_flag[l_sk]=1;
			switch(l_sk)
			{
				case 0:
					if(listen(0)==TRUE)
						S0_State=S_INIT;
					else
						S0_State=0;
					break;
				case 1:
					if(listen(1)==TRUE)
						S1_State=S_INIT;
					else
						S1_State=0;
					break;
				case 2:
					if(listen(2)==TRUE)
						S2_State=S_INIT;
					else
						S2_State=0;
					break;
				case 4:
					if(listen(4)==TRUE)
						S4_State=S_INIT;
					else
						S4_State=0;
					break;
			}
// 				
// 			listen( l_sk );	
		}
	}
	
	return l_u32Ret2;
}

/*********************************************************************************************************
** Function name:		FNetKeepalive
** Descriptions:		
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130910	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FNetSendKeepalive( SOCKET l_sk )
{
	uint32 l_u32Ret = 0;

// 	l_u32Ret = Read_W5100(W5100_S0_SSR+l_sk*0x100);
	l_u32Ret = getSn_SSR(l_sk);
	if( SOCK_ESTABLISHED == l_u32Ret )
	{
// 		Write_W5100(W5100_S0_CR+l_sk*0x100, S_CR_SEND_KEEP);
		Write_W5300(Sn_CR(l_sk), Sn_CR_SEND_KEEP);
	}
	else
	{
		return ERR_NET_NOCON;
	}

	return SUCCESS;
}

/*********************************************************************************************************
** Function name:		FNetCheckClose
** Descriptions:		
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130910	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FNetCheckClose(void)
{
	uint32 	l_u32Ret = 0;
	int8	l_sk = 0;

    for( l_sk=0; l_sk<NET_PORT_NUM; l_sk++ )
	{
// 		l_u32Ret = Read_W5100(W5100_S0_SSR+l_sk*0x100);
		l_u32Ret = getSn_SSR(l_sk);
		if( SOCK_CLOSED == l_u32Ret )
		{
// 			l_u32Ret = Socket_Listen( l_sk );
			l_u32Ret = listen( l_sk );
		}									 
	}
	
	return SUCCESS;
}


