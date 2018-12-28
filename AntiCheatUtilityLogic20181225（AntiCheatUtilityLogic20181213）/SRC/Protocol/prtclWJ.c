/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			prtclWJ.c
** Last modified Date:  20121031
** Last Version:		1.0
** Descriptions:		非现场执法协议
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wang ZiFeng
** Created date:		20121031
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------	
*********************************************************************************************************/

#define __PRTCLWJ_C
#include "prtclWJ.h"
#include "common.h"
#include "FPGA_APP.h"

#define SENDDEVICESTATUSINTERVAL	200		// 设备状态发送间隔10s，10*20*50ms
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

static		uint8	m_u8TimerNum = 0;		// 时间计数，状态帧时间计数
static		uint8	m_u8SuccessFlag = 1;//0成功，1失败
static      uint8   m_u8LaneNum = 0;

static		sndFrame_Struct	m_SendLastContext;	 //当前发送内容，为收到重发指令使用

static		uint8	m_u8ResendFlag = 0;//重发标志，为1，表示重发，0表示不是重发
static		uint8	m_u8ReSendFrameNum = 0xFF;// 重复帧号
static 		uint16 	m_FPGAInitParam[100] = {0};

//static      sndFrame_Struct   m_u8ZhuaPai; //抓拍指令帧

//SD卡专用
static		uint32	m_u32BeginIx = 0;		   	// 开始序号
static		uint32	m_u32EndIx = 0;		  		// 结束序号
/*********************************************************************************************************
** Function name:           WJ_ProcessCmd
**
** Descriptions:            解析上位机命令，执行操作
**
** input parameters:        uint8 *	pdata: 接收数据内容
**							uint16 	len:   接收数据长度
** output parameters:       none
** Returned value:          none
**
** Created by:              Wang ZiFeng
** Created Date:           	20121031
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void ProcessCmd(NetFrame *pData)
void WJ_ProcessCmd(uint8 *pData)
{
	uint8	l_u8VehID = 0;
	uint8*	l_pu8Tmp;
	uint8	l_u8Tempi = 0;//循环变量
	uint8	l_u8Tempj = 0;
	uint16  l_u16len = 0;
	uint32  l_u32tmpj, l_u32tmpi;//,l_u32tmp1;
	//
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	//
	uint8	l_u8TempVar = 0;		// 临时变量

	uint32  l_u32Ret = 0;
//	uint8   l_u8LaneNum;

//	uint8	l_u8VehSpeedModifyPos = 0;	// 车型速度修正位置，主要区分子命令7、8

	//
	m_u8SuccessFlag = 1;  //初始化为1，表示失败	
	if(*(pData) == 0xFF && *(pData+1) == 0xFF)   
		l_u16len = 	(*(pData+5)<<8) + *(pData+6);
	else if(*(pData) == 0xAA && *(pData+1) == 0xAA)
		l_u16len = *(pData+4);
	if( 0 == CheckCrc(pData,l_u16len-2) )	//CRC校验，revised by wujinlong 20121205
	{
		return;
	}
//	// 通过帧长判断是否为工控机响应,是响应，处理完毕，返回
//	if( 12 == l_u16len )
//	{
//		if( !(*(pData+7)) )		//为0，成功，否则，重传
//		{
//
//		}
//		else
//		{
//			//发送到PC
//			//通信协议发送数据信息
//			OSQPost(g_pQSendPC,&m_SendLastContext);	
//		}
//		return;	
//	}  
	// 
	// 接收到网口信息added by wujinlong 20130731
#if DEBUGINFO_TASK_SP_EN > 0
// 	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Recv Comp Cmd %x [Net]。\r\n",
// 					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
// 					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
// 						   *(pData+3));
// 	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 	g_u16CurDebugInd++;
// 	g_u16CurDebugInd %= COM_NUM;
// 	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 	OSSemPost(g_pSemSendThtouSP);
#endif

	switch(*(pData+3))
	{
		case ADJ_SUPERCOMD:
			m_u8SuccessFlag = 0;
			switch(*(pData+4))
			{
				case 1:
					g_Setup.u8Protocol = *(pData+7);
					SaveParams(2);
					break;
				case 2:
					InitSystem(3);
					InitNetParam(3);
					ProModInit(3);
					InitJDParam();
					OSTimeDly(10);
					YBRestart();
					break;
				case 3:
					InitSystem(4);
					ProModInit(4);
					OSTimeDly(10);
					YBRestart();
					break;
				case 4:
					InitNetParam(4);
					OSTimeDly(10);
					YBRestart();
					break;
				case 5:
					PROMODPARAM.m_u8UploadDataMode = *(pData+7);
					SaveParams(3);
					break;
			}
			SendCommandToPC0(*(pData+4), ADJ_SUPERCOMD);
			break;
		case ADJ_IPSEARCH:
			// 不在W5100中断中，防止g_IPSetLocal耦合
			for(l_u8Tempi=0; l_u8Tempi<4; l_u8Tempi++)
				g_IPSetLocal.au8ServerIP[l_u8Tempi] = S3_DIP[l_u8Tempi];		// 目的IP地址		
			g_IPSetLocal.u32ServerPortNO = (S3_DPort[0]<<8) + S3_DPort[1];	 	//目的端口号
			SaveNetInfo(6);
			//
			SendCommandToPC0(0, ADJ_IPSEARCH);
			break;
		case ADJ_EXT:
			m_u8SuccessFlag = 0;
			switch(*(pData+4))
			{
				case 0x10:
					g_u8BiaoDEN = 1;
					break;
				case 0x11:
					g_u8BiaoDEN = 0;
					break;
				case 0x12:
					if(g_u8BiaoDEN == 1)
					{
						g_u8BiaoDLane = (*(pData+7))*2;
					}
					break;
				case 0x20:
					PROMODPARAM.m_u8StudyAlgEn[0] = *(pData+7);					  //学习使能
					PROMODPARAM.m_u8StudyAlgEn[1] = *(pData+8);
					PROMODPARAM.m_u8StudyAlgEn[2] = *(pData+9);
					PROMODPARAM.m_u8StudyAlgEn[3] = *(pData+10);
					PROMODPARAM.m_u8StudyAlgEn[4] = *(pData+11);

					PROMODPARAM.m_u16StudyWeight[0][0] = (((*(pData+12))<<8) + (*(pData+13)))/10;
					PROMODPARAM.m_u16StudyWeight[0][1] = (((*(pData+14))<<8) + (*(pData+15)))/10;
					PROMODPARAM.m_u8StudyLimit[0][0] = *(pData+16);
					PROMODPARAM.m_u8StudyLimit[0][1] = *(pData+17);
					PROMODPARAM.m_u16StudyWeight[1][0] = (((*(pData+18))<<8) + (*(pData+19)))/10;
					PROMODPARAM.m_u16StudyWeight[1][1] = (((*(pData+20))<<8) + (*(pData+21)))/10;
					PROMODPARAM.m_u8StudyLimit[1][0] = *(pData+22);
					PROMODPARAM.m_u8StudyLimit[1][1] = *(pData+23);
					PROMODPARAM.m_u16StudyWeight[2][0] = (((*(pData+24))<<8) + (*(pData+25)))/10;
					PROMODPARAM.m_u16StudyWeight[2][1] = (((*(pData+26))<<8) + (*(pData+27)))/10;
					PROMODPARAM.m_u8StudyLimit[2][0] = *(pData+28);
					PROMODPARAM.m_u8StudyLimit[2][1] = *(pData+29);
					PROMODPARAM.m_u16StudyWeight[3][0] = (((*(pData+30))<<8) + (*(pData+31)))/10;
					PROMODPARAM.m_u16StudyWeight[3][1] = (((*(pData+32))<<8) + (*(pData+33)))/10;
					PROMODPARAM.m_u8StudyLimit[3][0] = *(pData+34);
					PROMODPARAM.m_u8StudyLimit[3][1] = *(pData+35);
					PROMODPARAM.m_u16StudyWeight[4][0] = (((*(pData+36))<<8) + (*(pData+37)))/10;
					PROMODPARAM.m_u16StudyWeight[4][1] = (((*(pData+38))<<8) + (*(pData+39)))/10;
					PROMODPARAM.m_u8StudyLimit[4][0] = *(pData+40);
					PROMODPARAM.m_u8StudyLimit[4][1] = *(pData+41);
					for(l_u8Tempi = 0; l_u8Tempi<CHANNELNUM; l_u8Tempi++)
					{
						g_Setup.an16BDAxGain[l_u8Tempi] = g_Setup.an32AxGain[l_u8Tempi];	
					}
					SaveParams(4);
					break;
				default:
					break;			
			}
			SendCommandToPC0(*(pData+4), ADJ_EXT);
			break;

	   	// 心跳
		case ADJ_HEARTBEAT:
			l_u8TempVar = *(pData+7);
			
			// 接收回复时间计时>设置阈值（5s）
			if( g_au16HeartRecvTimeCnt[l_u8TempVar]>NET_HEART_RECVINTERVAL )
			{				
				g_au8HeartRecvFailCnt[l_u8TempVar]++;
				if( g_au8HeartRecvFailCnt[l_u8TempVar] > 10 )
				{
					g_au8HeartRecvFailCnt[l_u8TempVar] = 10;
				}
			}
			else
			{
				g_au8HeartRecvFailCnt[l_u8TempVar] = 0;		  // 接收回复失败次数 清零
			}
			g_au8HeartRecvBeginCntFlag[l_u8TempVar] = 0;  // 停止 接收回复时间计数 标志			
			g_au16HeartRecvTimeCnt[l_u8TempVar] = 0;	  // 接收回复时间计数 清零
			
			break;

		case ADJ_SETHEARTEN:
		    switch(*(pData+4))
			{
				case 1:
				{
					m_u8SuccessFlag = 0;
					SendCommandToPC0(1,ADJ_SETHEARTEN);
					break;
				}
			    case 2:
				{
					m_u8SuccessFlag = 0;
					g_Setup.m_an8HeartBeatEn[0] = *(pData+7);
					g_Setup.m_an8HeartBeatEn[1] = *(pData+8);
					g_Setup.m_an8HeartBeatEn[2] = *(pData+9);
					g_Setup.m_an8HeartBeatEn[3] = *(pData+10);
					SaveParams(5);
					SendCommandToPC0(2,ADJ_SETHEARTEN);
					break;
				}
			}
			break;

		case ADJ_CONHEART:
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData+7),ADJ_CONHEART);
#if SD_OFFNET_SAVE_EN > 0
			g_u32NetConCnt = 0;
			g_u8IsConnectFlag = 1;
#endif
			break;
		case ADJ_GETVRSION:
			m_u8SuccessFlag = 0;
			SendCommandToPC0(0, ADJ_GETVRSION);
			break;


#if	TASKTIMEENABLE	
		// 任务耗时统计
		case ADJ_SENDRUNTIME:
			switch (*(pData+4)){
				case 0x00:
			   		SendCommandToPC0(0, ADJ_SENDRUNTIME); //命令模式1
					break;
				case 0x01:
					TaskRunTimeStatistics.m_ucWT_TASKFlag=1;
					TaskRunTimeStatistics.m_ucAnalysisFlag=1;
					TaskRunTimeStatistics.m_ucTaskWaveFlag=1;
					TaskRunTimeStatistics.m_ulTaskProtocolFlag=1;
					break;
				case 0x02:
					TaskRunTimeStatistics.m_ucWT_TASKFlag=0;
					TaskRunTimeStatistics.m_ucAnalysisFlag=0;
					TaskRunTimeStatistics.m_ucTaskWaveFlag=0;
					TaskRunTimeStatistics.m_ulTaskProtocolFlag=0;
					break;
				default:
					break;
			}
			break;
#endif

		case LASER_SET_PARA_RQ://设置激光模块参数
			l_u16len = *(pData+11);
		    if(0 != CheckCrc((pData+7), l_u16len - 2))
			{
			    memcpy((uint8*)(&g_LaserPara), pData + 12, sizeof(LaserSetupParam));
				U3SendBytes(pData+7, l_u16len);
				m_u8SuccessFlag = 0;
				SendCommandToPC0(0, LASER_SET_PARA_RQ);
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x.\r\n",
							       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
								   *(pData+3));
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
			}
			else//命令内容校验失败
			{
				m_u8SuccessFlag = 1;
				SendCommandToPC0(0, LASER_SET_PARA_RQ);
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x.\r\n",
							       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
								   *(pData+3));
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
			}
		    break;

		case LASER_READ_PARA_RQ://读取激光模块参数
		case LASER_RESET_RQ://复位激光模块
			l_u16len = *(pData+11);
		    if(0 != CheckCrc(pData+7, l_u16len - 2))
			{
		        U3SendBytes(pData+7, l_u16len);
				
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x.\r\n",
							       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
							       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
								   *(pData+3));
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
			}
			else//命令内容校验失败
			{
				
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Fail Cmd %x.\r\n",
								       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
								       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
									   *(pData+3));
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
			}
		    break;

//#if	 SIM_NET
//	    case SIM_LASER:
//			sim_net_laser=*(pData+7);			
//			SendCommandToPC0( 0, SIM_LASER);
//			break;		   
//#endif				
		case ADJ_SENDAXLEGRP:	//轴组重 0x00
		case ADJ_SENDAXLE:  // 轴重 0x01
			m_u8SuccessFlag = 0;
			if(*(pData+7) == 1)
		   	{
				l_u8VehID = *(pData+4);//流水号位置，工控机主动读车辆信息
				if( g_Setup.u8ComMode )	   //命令号
					SendCommandToPC0( l_u8VehID, ADJ_SENDAXLE); //命令模式1
				else
					SendCommandToPC0( l_u8VehID, ADJ_SENDAXLEGRP); //命令模式0
			}	   		   		
		   	break;

		case ADJ_TIMESYNCHRONY:  // 时间同步 0x02	
			g_DevCurTime.u16Year	= ((*(pData+7))<<8) + (*(pData+8)); 
			g_DevCurTime.u8Month	= *(pData+9);
			g_DevCurTime.u8Day		= *(pData+10);
			g_DevCurTime.u8Hour	= *(pData+11);
			g_DevCurTime.u8Minute	= *(pData+12);
			g_DevCurTime.u8Second	= *(pData+13);
			
			SaveTime(g_DevCurTime);
			g_u16TotalMinutesNew = g_DevCurTime.u8Hour*60 + g_DevCurTime.u8Minute;
			g_u16TotalMinutesOld = g_u16TotalMinutesNew;
			m_u8SuccessFlag = 0;
			SendCommandToPC0( 0, ADJ_TIMESYNCHRONY);
			break;

		case ADJ_RESEND:   //重发 0x0A
			m_u8SuccessFlag = 0;	
			l_u8VehID = *(pData + 7);	 
			m_u8ReSendFrameNum = *(pData+4); 		
			m_u8ResendFlag = 1;	
			if( g_Setup.u8ComMode )	   //命令号
				SendCommandToPC0( l_u8VehID, ADJ_SENDAXLE); //命令模式1
			else
				SendCommandToPC0( l_u8VehID, ADJ_SENDAXLEGRP); //命令模式0
			break;
		case ADJ_IOUSEASK:
			m_u8SuccessFlag = 0;
		  	SendCommandToPC0( 0, ADJ_IOUSEASK);
			break;
		case ADJ_IOUSESET:
			m_u8SuccessFlag = 0;
			for(l_u8Tempi=0; l_u8Tempi<26; l_u8Tempi++)
			{
				PROMODPARAM.m_au8IOUseEn[l_u8Tempi] = *(pData+7+l_u8Tempi);
			}
			SaveParams(6);
			SendCommandToPC0( 0, ADJ_IOUSESET);
			break;

		case ADJ_TESTCOM:
			switch(*(pData+4))
			{
				case 1:
					break;
				case 2:
					if(*(pData+7)==0)
						P3_OUTP_SET = Camera_IO1_SET;
					else
						P3_OUTP_CLR = Camera_IO1_SET;
					if(*(pData+8)==0)
						P3_OUTP_SET = Camera_IO2_SET;
					else
						P3_OUTP_CLR = Camera_IO2_SET;
					if(*(pData+9)==0)
						P3_OUTP_SET = Camera_IO3_SET;
					else
						P3_OUTP_CLR = Camera_IO3_SET;
					if(*(pData+10)==0)
						P3_OUTP_SET = Camera_IO4_SET;
					else
						P3_OUTP_CLR = Camera_IO4_SET;
					if(*(pData+11)==0)
						P3_OUTP_SET = Camera_IO5_SET;
					else
						P3_OUTP_CLR = Camera_IO5_SET;
					if(*(pData+12)==0)
						P3_OUTP_SET = Camera_IO6_SET;
					else
						P3_OUTP_CLR = Camera_IO6_SET;
					if(*(pData+13)==0)
						P3_OUTP_SET = Camera_IO7_SET;
					else
						P3_OUTP_CLR = Camera_IO7_SET;
					if(*(pData+14)==0)
						P3_OUTP_SET = Camera_IO8_SET;
					else
						P3_OUTP_CLR = Camera_IO8_SET;
					if(*(pData+15)==0)
						P0_OUTP_SET = Camera_IO9_SET;
					else
						P0_OUTP_CLR = Camera_IO9_SET;
					if(*(pData+16)==0)
						P0_OUTP_SET = Camera_IO10_SET;
					else
						P0_OUTP_CLR = Camera_IO10_SET;
					if(*(pData+17)==0)
						P0_OUTP_SET = Camera_IO11_SET;
					else
						P0_OUTP_CLR = Camera_IO11_SET;
					if(*(pData+18)==0)
						P0_OUTP_SET = Camera_IO12_SET;
					else
						P0_OUTP_CLR = Camera_IO12_SET;
					if(*(pData+19)==0)
						P0_OUTP_SET = Camera_IO13_SET;
					else
						P0_OUTP_CLR = Camera_IO13_SET;
					if(*(pData+20)==0)
						P3_OUTP_SET = Camera_IO14_SET;
					else
						P3_OUTP_CLR = Camera_IO14_SET;
					if(*(pData+21)==0)
						P3_OUTP_SET = Camera_IO15_SET;
					else
						P3_OUTP_CLR = Camera_IO15_SET;
					if(*(pData+22)==0)
						P3_OUTP_SET = Camera_IO16_SET;
					else
						P3_OUTP_CLR = Camera_IO16_SET;
					if(*(pData+23)==0)
						P3_OUTP_SET = Camera_IO17_SET;
					else
						P3_OUTP_CLR = Camera_IO17_SET;
					if(*(pData+24)==0)
						P3_OUTP_SET = Camera_IO18_SET;
					else
						P3_OUTP_CLR = Camera_IO18_SET;	
					break;
			}
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData+4), ADJ_TESTCOM);
			break;
	   	case ADJ_DEVICESTATUS:	 //状态帧，0x05，上位机主动查询
			m_u8SuccessFlag = 0;
		  	SendCommandToPC0( 0, ADJ_DEVICESTATUS);
			break;

		case ADJ_RESTART:	 //重启，0x54
			m_u8SuccessFlag = 0;
			SendCommandToPC0( *(pData+4), ADJ_RESTART);
			switch(*(pData+4))
			{
				case 1:
				  	YBRestart();
					break;
				case 2:
					/////////
					memset(g_an16YaFengArea, -1, 2*YAFENGAREASIZE);
					g_u8YaFengAreaIndex = 0;
					for(l_u32tmpj = 0;l_u32tmpj < 2*CHL_MAXNUM-1;l_u32tmpj++)
					{
						for(l_u32tmpi = 0;l_u32tmpi < CHL_SAVEVEH;l_u32tmpi++)
						{
							FInitVehStr(&g_asVehiclePerChannelForLoop[l_u32tmpj][l_u32tmpi],0);
							FInitVehStr(&g_asReVehiclePerChannelForLoop[l_u32tmpj][l_u32tmpi],1);
						}
					}
					for(l_u32tmpj = 0;l_u32tmpj < 2*CHL_MAXNUM;l_u32tmpj++)
					{
// 						for(l_u32tmp1 = 0;l_u32tmp1 < COLUM_SAVEAXlE;l_u32tmp1++)
// 						{
// 							memset(&Forward_ZTColum[l_u32tmpj][l_u32tmp1],0,sizeof(Colum_Struct));  //正向列初始化
// 							memset(&Reverse_ZTColum[l_u32tmpj][l_u32tmp1],0,sizeof(Colum_Struct));  //逆向列初始化
// 							memset(&Forward_ZTColum[l_u32tmpj][l_u32tmp1].Colum,-1,sizeof(int16)*4); //正向列存储的列初始化为-1
// 							memset(&Reverse_ZTColum[l_u32tmpj][l_u32tmp1].Colum,-1,sizeof(int16)*4); //逆向列存储的列初始化为-1
// 							memset(&g_WonderZT[l_u32tmpj][l_u32tmp1],-1,sizeof(int16));
// 						}
						g_au8ReZTEnPerLie[l_u32tmpj]=1;
						g_au8ZTEnPerLie[l_u32tmpj]=1;
					}
					/////////
//					F_CreatRouteTable(LENGTH_ROUTE_TABLE,a_CRouteTable1);
//					F_CreatRouteTable(LENGTH_ROUTE_TABLE,a_CRouteTable2);
//					F_CreatWonderBuffer(20,a_CWonderBuffer1);
//					F_CreatWonderBuffer(20,a_CWonderBuffer2);
//					F_CreatLaserLateBuffer(20,a_LaserLateBuffer1);//浩叔修改
//					F_CreatLaserLateBuffer(20,a_LaserLateBuffer2);//浩叔修改
					break;
				case 4:
					InitSystem(5);
					InitNetParam(5);
					ProModInit(5);
					OSTimeDly(10);
					YBRestart();
					break;
				case 8:
					break;
			}
			break;

		case ADJ_CAPTURE:
			m_u8SuccessFlag = 0;	 //抓拍，0x13，当触发相机抓拍时给上位机发送该命令，通知触发抓拍信号
		   	SendCommandToPC0( 0, ADJ_CAPTURE);
		   	//SendCommandToPC0( g_u8NetADOut, ADJ_CAPTURE);
		   	break;

		case ADJ_USESTAT:
			m_u8SuccessFlag = 0;
			switch(*(pData+4))
			{
				case 0x00:
					g_u8IfRunSta = 1;
					memset(g_u8RunZTSta, 1, 64);//记录窄条触发
					memset(g_u8RunLoopSta, 0, 16);//记录线圈状态
				break;
				case 0x02:
					g_u8FirstUse = 0;
					g_u8IfRunSta = 0;
				break;
				case 0x03:
					g_Setup.u8ProgVersion[0]		= 0;
					g_Setup.u8ProgVersion[1]		= 0;
					g_Setup.u8ProgVersion[2]		= 0;
					g_Setup.u8ProgVersion[3]		= 0;
					SaveParams(7);
				break;
			}
			SendCommandToPC0(*(pData+4), ADJ_USESTAT);
			break;
		case ADJ_ZTARRAYASK:
			m_u8SuccessFlag = 0;
			SendCommandToPC0(0, ADJ_ZTARRAYASK);
			break;
		case ADJ_ZTARRAYSET:
			m_u8SuccessFlag = 0;
			for(l_u8Tempi=0; l_u8Tempi<64; l_u8Tempi++)
			{
				g_Setup.u8ArrayForZT[l_u8Tempi] = *(pData+7+l_u8Tempi);
			}
			SaveParams(8);
			SendCommandToPC0(0, ADJ_ZTARRAYSET);
			break;
		case ADJ_CHLASK:
			m_u8SuccessFlag = 0;
			SendCommandToPC0(0, ADJ_CHLASK);
			break;
		case ADJ_CHLSET:
			m_u8SuccessFlag = 0;
			for(l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++)
			{
				PROMODPARAM.m_au8CHLSET[l_u8Tempi] = *(pData+7+l_u8Tempi);
			}
			SaveParams(9);
			SendCommandToPC0(0, ADJ_CHLSET);
			break;
		/////////////////////////////////////////////
		//SD卡
		case ADJ_GETSDINFO:
			if( FSDCheckAntiShake(&sd_info) )	  	// 检测卡是否完全插入卡座 
			{
				// 插入
				m_u8SuccessFlag = 0;
			}
			else
			{
				m_u8SuccessFlag = 2;
			}
			
			if( 0x02==(*(pData+4)&0x0F) )
			{
				m_u32BeginIx = ((*(pData+8))<<24) + ((*(pData+9))<<16) + ((*(pData+10))<<8) + ((*(pData+11)));
				m_u32EndIx = ((*(pData+12))<<24) + ((*(pData+13))<<16) + ((*(pData+14))<<8) + ((*(pData+15)));
				g_u32BeginIx = m_u32BeginIx;
				g_u32EndIx = m_u32EndIx;
			}
			
			SendCommandToPC0(*(pData+4), ADJ_GETSDINFO);
			break;
		case ADJ_SETSDINFO:
		    m_u8SuccessFlag = 0;
			if( FSDCheckAntiShake(&sd_info) )	  	// 检测卡是否完全插入卡座 
			{
				// 插入
				m_u8SuccessFlag = 0;
			}
			else
			{
				m_u8SuccessFlag = 2;
			}
			switch(*(pData+4))
			{
				case 0x00:
					//格式化SD卡信息
					memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_ZONG, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_OFFNET_SAVE, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					FSysnVehIx();
					FSysnLogIx();
					FSysnWaveIx();
					break;

				case 0x03:
					//清空车辆数
					memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					FSysnVehIx();
					break;

				case 0x13:
					//清空日志数
					memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					FSysnLogIx();
					break;

				case 0x23:
					//清空波形数
					memset( g_u8SDWrBuf, 0, SD_SECTOR_LEN );
					l_u32Ret = SD_WriteBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDWrBuf );		
					if( SD_NO_ERR != l_u32Ret )
					{
					}
					FSysnWaveIx();
					break;

				default:
					break;
			}
			
			SendCommandToPC0(*(pData+4), ADJ_SETSDINFO);
			break;
		///////////
		case ADJ_GETIPINFO:	 //读取仪表IP相关参数,0x20
		   	m_u8SuccessFlag = 0;
		   	SendCommandToPC0( 0, ADJ_GETIPINFO);
		   	break;

		case ADJ_UPDATEIPINFO:	 //更改仪表IP相关参数,0x21
		   	memcpy( &g_IPSetLocal.au8IPAddr[0], pData+7, 4 );
		   	memcpy( &g_IPSetLocal.au8SubMask[0], pData+11, 4 );
		   	memcpy( &g_IPSetLocal.au8GatewayIP[0], pData+15, 4 );
		   	g_IPSetLocal.u32LocalPortNO = ((*(pData+19))<<24) + ((*(pData+20))<<16) + ((*(pData+21))<<8)+(*(pData+22));
		   	memcpy( &g_IPSetLocal.au8MACAddr[0], pData+23, 6 );
		   	m_u8SuccessFlag = 0;
		   	SendCommandToPC0( 0, ADJ_UPDATEIPINFO);	
//		   	YBRestart();	
		   	SaveNetInfo(7);  	
		   	OSTimeDly(500*OS_TICKS_PER_SEC/1000);
//		   	InitializeW5100(&g_IPSetLocal);
		   	break;
		
		case ADJ_NETCONNECT:	 //工控机检查线路连接
			m_u8SuccessFlag = 0;
			SendCommandToPC0( 0, ADJ_NETCONNECT);
#if SD_OFFNET_SAVE_EN > 0
			g_u32NetConCnt = 0;
			g_u8IsConnectFlag = 1;
#endif
		   	break;


		///////////////////////////////////
		// 调校相关命令
		case ADJ_SAVE:	 //保存仪表参数指令,0xA1
		   SaveParams(10);
		   m_u8SuccessFlag = 0;
		   SendCommandToPC0( 0, ADJ_SAVE);
		   break;

		case ADJ_GETNONEWEIGHT:	 //读取所有非重量修正参数,0xA4
		   m_u8SuccessFlag = 0;
		   SendCommandToPC0( 0, ADJ_GETNONEWEIGHT);
		   break;

		case ADJ_UPDATENONEWEIGHT:	 //更新所有非重量修正参数,0xA5
			l_pu8Tmp = pData;
			g_Setup.u32Password = ((*(l_pu8Tmp+7))<<24)+((*(l_pu8Tmp+8))<<16)+((*(l_pu8Tmp+9))<<8)+(*(l_pu8Tmp+10));
			g_Setup.u32Full = ((*(l_pu8Tmp+11))<<24)+((*(l_pu8Tmp+12))<<16)+((*(l_pu8Tmp+13))<<8)+(*(l_pu8Tmp+14));
			l_pu8Tmp = pData + 15;
			g_Setup.u8MotionScale = *(l_pu8Tmp++);
			g_Setup.u8ComMode = *(l_pu8Tmp++);
			g_Setup.u8BaudRate = *(l_pu8Tmp++);
			g_Setup.u8DOG = *(l_pu8Tmp++);
			g_Setup.u8DiaoDianTag = *(l_pu8Tmp++);  
			g_Setup.u8SendWaveEnable = *(l_pu8Tmp++);
			SaveParams(11);
			m_u8SuccessFlag = 0;
			SendCommandToPC0( 0, ADJ_UPDATENONEWEIGHT);
		   	break;

		case ADJ_GETMOTIONWEIGHT:	 //读取所有动态修正参数,0xA8
		   m_u8SuccessFlag = 0;
		   if((*(pData+4) == 7) || (*(pData+4) == 9) || (*(pData+4) == 8))
		   {
		   		m_u8LaneNum = *(pData+7);
		   		if(m_u8LaneNum>9)
		   		{
		   			m_u8LaneNum = 9;
		   		}
		   }
		   SendCommandToPC0( *(pData+4), ADJ_GETMOTIONWEIGHT);
		   break;

		case ADJ_UPDATEMOTIONWEIGHT:	 //更新所有动态修正参数,0xA9
//		   	if(!BDMODE)
			{
				if((*(pData+4) == 7) || (*(pData+4) == 9) || (*(pData+4) == 8))
			   	{
			   		m_u8LaneNum = *(pData+7);
			   		if(m_u8LaneNum>9)
			   		{
			   			m_u8LaneNum = 9;
			   		}
					l_pu8Tmp = pData + 8;
			   	}
				else
				{
					l_pu8Tmp = pData + 7;
				}
				
				switch( *(pData+4) )
				{
					case 1:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							g_Setup.an32AxGain[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;	  //如果用++，防止不同编译器，效果不同
						}
						FPGAInitParam();
						m_u8SuccessFlag = 0;
						break;
	
					case 2:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							g_Setup.an32Zero[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							g_an32MotionZero[l_u8Tempi] = g_Setup.an32Zero[l_u8Tempi];
							l_pu8Tmp += 2;
						}
						FPGAInitParam();
						m_u8SuccessFlag = 0;
						break;
	
					case 3:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							l_pu8Tmp++;
							g_Setup.an8ZTStatus[l_u8Tempi] = *(l_pu8Tmp++);
						}
						OSSemPost(g_pSemTask6Param);
						m_u8SuccessFlag = 0;
						break;
	
					case 4:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							g_Setup.an8LengthPodu[l_u8Tempi] = *(l_pu8Tmp++);
						}
						OSSemPost(g_pSemTask6Param);
						m_u8SuccessFlag = 0;
						break;
	
					case 5:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							g_Setup.an8HeightTanxingti[l_u8Tempi] = *(l_pu8Tmp++);
						}
						OSSemPost(g_pSemTask6Param);
						m_u8SuccessFlag = 0;
						break;
	
					case 6:
						memset(m_FPGAInitParam,0,sizeof(m_FPGAInitParam));
						g_Setup.u16UpValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[0] = g_Setup.u16UpValue;
						l_pu8Tmp += 2;
						g_Setup.u16DownValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[1] = g_Setup.u16DownValue;
						l_pu8Tmp += 2;
						g_Setup.u16logicUpValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[2] = g_Setup.u16logicUpValue;
						l_pu8Tmp += 2;
						g_Setup.u16logicDownValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[3] = g_Setup.u16logicDownValue;
						l_pu8Tmp += 2;
						g_Setup.u8UPPoints = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[4] = g_Setup.u8UPPoints;
						l_pu8Tmp += 2;
						g_Setup.u8DownPoints = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[5] = g_Setup.u8DownPoints;
						l_pu8Tmp += 2;
	
						// 增加胶垫状态（1*16）,更改变量
						//低字节是第一方向，高字节是第二方向
						for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ ) //20130523 浩叔发现bug
						{
	//						g_Setup.u16JiaodianState[l_u8Tempi] = 0;
							g_Setup.u16UseToWghFlag[l_u8Tempi] = (*(l_pu8Tmp)) + 256*(*(l_pu8Tmp+1));//双向车道第二方向各排32-64
							l_pu8Tmp = l_pu8Tmp + 2;
	//						g_Setup.u16JiaodianState[l_u8Tempi] = g_Setup.u16JiaodianState[l_u8Tempi]| (*(l_pu8Tmp++));
						}
						//窄条间距
						for( l_u8Tempi=0; l_u8Tempi<CHL_MAXNUM; l_u8Tempi++ ) //20130523 浩叔发现bug
						{
							for( l_u8Tempj=0; l_u8Tempj<CHANNALLINESUM-1; l_u8Tempj++ )
							{
								g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
								l_pu8Tmp += 2;  
							}
						}
						OSSemPost(g_pSemTask6Param);
						FPGAInitParam();
						m_u8SuccessFlag = 0;
						break;
	
					case 7: 
						for( l_u8Tempi=0; l_u8Tempi<10; l_u8Tempi++ )
						{
							for( l_u8Tempj=0; l_u8Tempj<20; l_u8Tempj++ )
							{
								g_Setup.au16VehSpeedModify[m_u8LaneNum][l_u8Tempi][l_u8Tempj] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
								l_pu8Tmp += 2;
							}
						}
						m_u8SuccessFlag = 0;
						break;
					case 8:
						for( l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++ )
						{
							g_Setup.au16Speedmodify[m_u8LaneNum][l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;
						}
						m_u8SuccessFlag = 0;
					break;
					case 9: 
						for( l_u8Tempi=0; l_u8Tempi<10; l_u8Tempi++ )
						{
							g_Setup.au16VehTotalModify[m_u8LaneNum][l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;
						}
						m_u8SuccessFlag = 0;
						break;
					case 15:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							l_pu8Tmp++;
							g_cProModParam.m_n8LouDL[l_u8Tempi] = *(l_pu8Tmp++);
						}
						m_u8SuccessFlag = 0;
						break;
					case 16:
						for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
						{
							l_pu8Tmp++;
							g_cProModParam.m_n8CFDian[l_u8Tempi] = *(l_pu8Tmp++);
						}
						m_u8SuccessFlag = 0;
						break;
					case 17:
						for( l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++ )
						{
							g_cProModParam.m_u16LRLocation[0][l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;
							g_cProModParam.m_u16LRLocation[1][l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;
						}
						g_cProModParam.m_u8JGKZ = *(l_pu8Tmp + 1);
						l_pu8Tmp += 2;
						m_u8SuccessFlag = 0;
						break;
					case 20:
						m_u8SuccessFlag = 0;
						g_cProModParam.m_u8ZTRow = *(l_pu8Tmp++);
						g_cProModParam.m_u8LaneNum = *(l_pu8Tmp++);
						g_cProModParam.m_u8TriggerMode  = *(l_pu8Tmp++);
						g_cProModParam.m_u8IsYaFeng  = *(l_pu8Tmp++);
						g_cProModParam.m_u8UploadDataMode  = *(l_pu8Tmp++);
						g_cProModParam.m_u8DataWaveEn = *(l_pu8Tmp++);
						g_cProModParam.m_u8VehWaveSample  = *(l_pu8Tmp++);
						g_cProModParam.m_u8CaptureMode  = *(l_pu8Tmp++);
						g_cProModParam.m_u8TransmitSysEn  = *(l_pu8Tmp++);
						g_cProModParam.m_u16SDSaveEn = (*(l_pu8Tmp)<<8) + (*(l_pu8Tmp + 1));
						l_pu8Tmp = l_pu8Tmp + 2;
						g_cProModParam.m_u16ExEquiType = (*(l_pu8Tmp)<<8) + (*(l_pu8Tmp + 1));
						l_pu8Tmp = l_pu8Tmp + 2;
						for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)
							g_cProModParam.m_au8AcqBoardFlag[l_u8Tempi] = *(l_pu8Tmp++);
						for(l_u8Tempi=0; l_u8Tempi<16; l_u8Tempi++)
							for(l_u8Tempj=0; l_u8Tempj<2; l_u8Tempj++)
								g_cProModParam.m_u8CaptureFlag[l_u8Tempi][l_u8Tempj] = *(l_pu8Tmp++);
						g_cProModParam.m_u8BwdDirecFlag  = *(l_pu8Tmp++);
						for(l_u8Tempi=0; l_u8Tempi<19; l_u8Tempi++)
						{
							g_cProModParam.m_au8VehDirecFlag[l_u8Tempi] = *(l_pu8Tmp++);
							if(g_cProModParam.m_au8VehDirecFlag[l_u8Tempi] != 1 && g_cProModParam.m_au8VehDirecFlag[l_u8Tempi] != 2 && g_cProModParam.m_au8VehDirecFlag[l_u8Tempi] != 3)
							{
								g_cProModParam.m_au8VehDirecFlag[l_u8Tempi] = 1;
								m_u8SuccessFlag = 1;
							}
						}
						for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)
						{
							g_cProModParam.m_u8EnableFlag[l_u8Tempi] = *(l_pu8Tmp++);
						}
						/////
						if(g_cProModParam.m_u8LaneNum > 10)
						{
							g_cProModParam.m_u8LaneNum = 10;
						   	m_u8SuccessFlag = 1;
						}
						if(g_cProModParam.m_u8ZTRow * g_cProModParam.m_u8LaneNum * 2 > 64)
						{
							g_cProModParam.m_u8ZTRow = 3;
							m_u8SuccessFlag = 1;
						}
						if(g_cProModParam.m_u8TriggerMode == 0 || g_cProModParam.m_u8TriggerMode > 4)
						{
							g_cProModParam.m_u8TriggerMode = 4;
							m_u8SuccessFlag = 1;
						}
						if(g_cProModParam.m_u8BwdDirecFlag != 0 && g_cProModParam.m_u8BwdDirecFlag != 1)
						{
							g_cProModParam.m_u8BwdDirecFlag = 1;
							m_u8SuccessFlag = 1;	
						}
						//////
						break;
					case 21:
						for(l_u8Tempi=0; l_u8Tempi<40; l_u8Tempi++)
						{
							g_cProModParam.m_au32Thres[l_u8Tempi] = (*(l_pu8Tmp)<<24) + (*(l_pu8Tmp+1)<<16) + (*(l_pu8Tmp+2)<<8) + *(l_pu8Tmp+3);
							l_pu8Tmp = l_pu8Tmp + 4;
						}
						m_u8SuccessFlag = 0;
						break;
					case 22:
						for(l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++)
						{
							g_cProModParam.m_au8LaneBetwWide[l_u8Tempi] = *(l_pu8Tmp);
							l_pu8Tmp ++;
						}
						m_u8SuccessFlag = 0;
						break;
					case 23:
						memset(m_FPGAInitParam,0,sizeof(m_FPGAInitParam));
						g_Setup.u16UpValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[0] = g_Setup.u16UpValue;
						l_pu8Tmp += 2;
						g_Setup.u16DownValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[1] = g_Setup.u16DownValue;
						l_pu8Tmp += 2;
						g_Setup.u16logicUpValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[2] = g_Setup.u16logicUpValue;
						l_pu8Tmp += 2;
						g_Setup.u16logicDownValue = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[3] = g_Setup.u16logicDownValue;
						l_pu8Tmp += 2;
						g_Setup.u8UPPoints = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[4] = g_Setup.u8UPPoints;
						l_pu8Tmp += 2;
						g_Setup.u8DownPoints = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						m_FPGAInitParam[5] = g_Setup.u8DownPoints;
						l_pu8Tmp += 2;
	
						// ??????(1*16),????
						//????????,????????
						for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ ) //20130523 ????bug
						{
	//						g_Setup.u16JiaodianState[l_u8Tempi] = 0;
							g_Setup.u16UseToWghFlag[l_u8Tempi] = (*(l_pu8Tmp)) + 256*(*(l_pu8Tmp+1));//??????????32-64
							l_pu8Tmp = l_pu8Tmp + 2;
	//						g_Setup.u16JiaodianState[l_u8Tempi] = g_Setup.u16JiaodianState[l_u8Tempi]| (*(l_pu8Tmp++));
						}
						//????
						/*
						for( l_u8Tempi=0; l_u8Tempi<CHL_MAXNUM; l_u8Tempi++ ) //20130523 ????bug
						{
							for( l_u8Tempj=0; l_u8Tempj<CHANNALLINESUM-1; l_u8Tempj++ )
							{
								g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
								l_pu8Tmp += 2;  
							}
						}
						*/
						
						for(l_u8Tempi=0; l_u8Tempi<CHANNELNUM;l_u8Tempi++)
						{
							g_cProModParam.m_au16ZTGap[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
							l_pu8Tmp += 2;
						}
						
						OSSemPost(g_pSemTask6Param);
						FPGAInitParam();
						m_u8SuccessFlag = 0;
						break;	
					default:
						return;
	
				}
				SaveParams(12);
	//			m_u8SuccessFlag = 0;
				SendCommandToPC0( *(pData+4), ADJ_UPDATEMOTIONWEIGHT);
			}
		   	break;
		case Para_Read:
			LOOP_BD_lane = *(pData + 7);							//获取车道号
			if(LOOP_BD_lane > 10)				//防止输入车道号大于总的车道数
			{
				return;
			}
			SendCommandToPC0(*(pData + 4),Para_Read);
			break;
		case Para_Save:
//			LOOP_BD_lane = (*(pData + 7) << 24) + (*(pData + 8) << 16) + (*(pData + 9) << 8) + *(pData + 10); 	
			LOOP_BD_lane = *(pData + 7);// 车道号
			g_cJDSysParam.m_au16Elevation = (*(pData + 8) << 8) + *(pData + 9);// 高程	   												
			g_cJDSysParam.m_au8LngBuf[0] = *(pData + 10);//  经度
			g_cJDSysParam.m_au8LngBuf[1] = *(pData + 11);
			g_cJDSysParam.m_au8LngBuf[2] = *(pData + 12);
			g_cJDSysParam.m_au8LngBuf[3] = *(pData + 13);
			g_cJDSysParam.m_au8LngBuf[4] = *(pData + 14);
			g_cJDSysParam.m_au8LngBuf[5] = *(pData + 15);
			g_cJDSysParam.m_au8LngBuf[6] = *(pData + 16);
			g_cJDSysParam.m_au8LngBuf[7] = *(pData + 17);
			g_cJDSysParam.m_au8LatBuf[0] = *(pData + 18);// 	纬度
			g_cJDSysParam.m_au8LatBuf[1] = *(pData + 19);
			g_cJDSysParam.m_au8LatBuf[2] = *(pData + 20);
			g_cJDSysParam.m_au8LatBuf[3] = *(pData + 21);
			g_cJDSysParam.m_au8LatBuf[4] = *(pData + 22);
			g_cJDSysParam.m_au8LatBuf[5] = *(pData + 23);
			g_cJDSysParam.m_au8LatBuf[6] = *(pData + 24);
			g_cJDSysParam.m_au8LatBuf[7] = *(pData + 25);
			for(l_u8Tempi = 0,l_u8Tempj = 26; l_u8Tempi < 15; l_u8Tempi++,l_u8Tempj++)						   		// 站点编号
			{
				g_cJDSysParam.m_au8StationID[l_u8Tempi] = *(pData + l_u8Tempj);
			}
			g_cJDSysParam.m_au8StationID[14] = 0; 
			for(l_u8Tempi = 0,l_u8Tempj = 41; l_u8Tempi < 16; l_u8Tempi++,l_u8Tempj++)						   		// 身份识别码
			{
				g_cJDSysParam.m_au8DeviceID[l_u8Tempi] = *(pData + l_u8Tempj);
			} 
			for(l_u8Tempi = 0,l_u8Tempj = 57; l_u8Tempi < 8; l_u8Tempi++,l_u8Tempj++)						   		//	用户名
			{
				g_cJDSysParam.m_au8UseName[l_u8Tempi] = *(pData + l_u8Tempj);
			}
			for(l_u8Tempi = 0,l_u8Tempj = 65; l_u8Tempi < 8; l_u8Tempi++,l_u8Tempj++)						   		//	密码
			{
				g_cJDSysParam.m_au8KeyNum[l_u8Tempi] = *(pData + l_u8Tempj);
			}
			g_cJDSysParam.m_u16MidKeHuoThr[LOOP_BD_lane - 1] = (*(pData + 73) << 8) + *(pData + 74); 					//	FZX
			g_cJDSysParam.m_u16SmallKeHuoThr[LOOP_BD_lane - 1] = (*(pData + 75) << 8) + *(pData + 76);	// 	FB
			g_cJDSysParam.m_u8SavePeriod = 	(*(pData + 77));
			g_cJDSysParam.m_u16SmallandMid = (*(pData + 78) << 8) + *(pData + 79);
			g_cJDSysParam.m_u8LaneDir =	*(pData + 80);
			g_cJDSysParam.m_u8OrderMod = *(pData + 81);
			g_cJDSysParam.m_u8TransMod = *(pData + 82);
			g_cJDSysParam.m_u8SendVehType =	*(pData + 83);
			AddCrc16((uint8 *)&g_cJDSysParam, JDSYSPARAMSIZE-2);			
			WriteC256(JDSYSADDR, (uint8 *)&g_cJDSysParam, sizeof(g_cJDSysParam));
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData + 4),Para_Save);
			break;

			
		case JD_Reset:
//			g_u32JDSDSaveInd = 0;									  	 //成功存储数据包序号
			WriteC256(JD01CURNUM, (uint8 * )&g_u32JDSDSaveInd, 4);
			g_u32JDSDSaveIndSuc = g_u32JDSDSaveInd;									 //成功发送数据包序号
			WriteC256(JD01SUCNUM, (uint8 * )&g_u32JDSDSaveIndSuc, 4);
			g_u8IsFirstSDSave = 0;										 // 01包首次存铁电标志
			WriteC256(JD01FIRSTFLAG, (uint8 * )&g_u8IsFirstSDSave, 1);

//			g_u32JD39SaveSuccInd = 0;
			WriteC256(JD39CURNUM, (uint8 * )&g_u32JD39SaveSuccInd, 4);
			g_u32JD39SendSuccInd = g_u32JD39SaveSuccInd;		
			WriteC256(JD39SUCNUM, (uint8 * )&g_u32JD39SendSuccInd, 4);
			g_u8Is39FirstSave = 0;
			WriteC256(JD39FIRSTFLAG, (uint8 * )&g_u8Is39FirstSave, 1);

//			g_u32JD61SaveSuccInd = 0;
			WriteC256(JD61CURNUM, (uint8 * )&g_u32JD61SaveSuccInd, 4);
			g_u32JD61SendSuccInd = g_u32JD61SaveSuccInd;		
			WriteC256(JD61SUCNUM, (uint8 * )&g_u32JD61SendSuccInd, 4);
			g_u8Is62FirstSave = 0;
			WriteC256(JD61FIRSTFLAG, (uint8 * )&g_u8Is61FirstSave, 1);

//			g_u32JD62SaveSuccInd = 0;
			WriteC256(JD62CURNUM, (uint8 * )&g_u32JD62SaveSuccInd, 4);
			g_u32JD62SendSuccInd = g_u32JD62SaveSuccInd;		
			WriteC256(JD62SUCNUM, (uint8 * )&g_u32JD62SendSuccInd, 4);
			g_u8Is62FirstSave = 0;
			WriteC256(JD62FIRSTFLAG, (uint8 * )&g_u8Is62FirstSave, 1);

//			g_u32JD68SaveSuccInd = 0;
			WriteC256(JD68CURNUM, (uint8 * )&g_u32JD68SaveSuccInd, 4);
			g_u32JD68SendSuccInd = g_u32JD68SaveSuccInd;		
			WriteC256(JD68SUCNUM, (uint8 * )&g_u32JD68SendSuccInd, 4);
			g_u8Is68FirstSave = 0;
			WriteC256(JD68FIRSTFLAG, (uint8 * )&g_u8Is68FirstSave, 1);
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData + 4),JD_Reset);
			break;
		case JD_Init:
			InitJDParam();
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData + 4),JD_Init);
			break;
		case ADJ_CTRSAMPPOW:
			if(*(pData + 7) == 1) // 1打开
			{
				P3_OUTP_CLR |= SAMPLEPOWER;
			}
			else				   // 0关闭
			{
				P3_OUTP_SET |= SAMPLEPOWER;
			}
			m_u8SuccessFlag = 0;
			SendCommandToPC0(*(pData + 7),ADJ_CTRSAMPPOW);
			break;
		case ADJ_ZTINTENSET:
			m_u8SuccessFlag = 0;
			for(l_u8Tempi=0; l_u8Tempi<64; l_u8Tempi++)
			{
				PROMODPARAM.m_au8ZTIntEn[l_u8Tempi] = *(pData + 7 + l_u8Tempi);
			}
			SaveParams(13);
			SendCommandToPC0(0,ADJ_ZTINTENSET);
			break;
		case ADJ_ZTINTENASK:
			m_u8SuccessFlag = 0;
			SendCommandToPC0(0,ADJ_ZTINTENASK);
			break;
		default:
			break; 
	}	
}

/*********************************************************************************************************
** Function name:		SendCommandToPC0
** Descriptions:		根据协议不同的命令字，发送数据
** input parameters:	uint8	p_u8VehIndex			//buffer数据的序号
						uint8	p_u8CommandNO			//命令字					 
** output parameters:	void
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121105	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SendCommandToPC0(uint8 p_u8VehIndex,uint8 p_u8CommandNO)
{
	//uint8	l_au8TmpData[256];
	uint8	l_u8Tempi = 0;
	uint16 l_u16Tempi=0;
	uint8	l_u8Tempj = 0;
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据

	uint8	l_u8Err;
	uint16	l_u16SendLen;

	uint32	ret = 0;
	uint8   l_u8DeviceErr;

//	uint16	l_u16ZTCurValue = 0;	// 窄条当前瞬时值
	
//	uint8	l_u8VehSpeedModifyPos = 0;
	uint8   l_u8TempVar;

	uint32		l_u32VehNum = 0;					// 车辆信息
	uint32		l_u32VehNext = 0;					// 
	uint32		l_u32VehCur = 0;					// 
	//
	uint32		l_u32LogNum = 0;					// 总日志数
	uint32		l_u32NextLog = 0;					// 下一日志的存储位置
	uint32		l_u32CurLog = 0;					// 当前日志的存储位置
	//
	uint32		l_u32WaveNum = 0;					// 波形信息
	uint32		l_u32WaveNext = 0;					// 
	uint32		l_u32WaveCur = 0;					// 
	//
	uint32		l_u32Ret = 0;						// 返回标志
	uint32		l_u32SDTempIx = 0;					// 循环变量
	//uint32		l_u32SDTempj = 0;					// 循环变量
	uint32		l_u32SDReadIx = 0;					// SD卡读索引
	uint32		l_u32RdAdd = 0;						// 读地址
	uint32		l_u32Num = 0;						// 商
	
	uint32      l_u32TimeNow = 0;
	//uint32	  	l_u32TempNum = 0;
	//uint32		l_u32Remind = 0;					// 余数
	uint32 tempver;
	switch( p_u8CommandNO )
	{
		case ADJ_IPSEARCH:   //工控机IP搜索 //wgh
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 23;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_IPSEARCH;
			*(l_pu8Tmp++) = 2;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 23;
			*(l_pu8Tmp++) = g_IPSetLocal.au8IPAddr[0];
			*(l_pu8Tmp++) = g_IPSetLocal.au8IPAddr[1];
			*(l_pu8Tmp++) = g_IPSetLocal.au8IPAddr[2];
			*(l_pu8Tmp++) = g_IPSetLocal.au8IPAddr[3]; 
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO >> 8)& 0xFF;
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO)& 0xFF;
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[0];
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[1];
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[2];
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[3];
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[4];
			*(l_pu8Tmp++) = g_IPSetLocal.au8MACAddr[5];
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
// 			SendDataNet( SOCKET_UDP, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			TcpSendData( SOCKET_UDP, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			goto ENDOFF;
//			break;
		case ADJ_SUPERCOMD:   //超级指令
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SUPERCOMD;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;			
		case ADJ_EXT:  //扩展指令
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_EXT;
			*(l_pu8Tmp++) = p_u8VehIndex;
			if(p_u8VehIndex == 0x20)
			{
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 17;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 17;
				*(l_pu8Tmp++) = m_u8SuccessFlag; 			// 流水号
				*(l_pu8Tmp++) = PROMODPARAM.m_u8StudyAlgEn[0];
				*(l_pu8Tmp++) = PROMODPARAM.m_u8StudyAlgEn[1];
				*(l_pu8Tmp++) = PROMODPARAM.m_u8StudyAlgEn[2];
				*(l_pu8Tmp++) = PROMODPARAM.m_u8StudyAlgEn[3];
				*(l_pu8Tmp++) = PROMODPARAM.m_u8StudyAlgEn[4];
			}
			else
			{	
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 12;
				*(l_pu8Tmp++) = m_u8SuccessFlag; 			// 流水号

				// test
//				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][2] = g_u8TestCnt;
			} 
			memset( l_pu8Tmp, 0, 2);
			
			break;
		case ADJ_SETHEARTEN:   // 设置心跳使能//wgh
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 16;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SETHEARTEN;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 16;
			*(l_pu8Tmp++) = m_u8SuccessFlag; 
			*(l_pu8Tmp++) = g_Setup.m_an8HeartBeatEn[0];
			*(l_pu8Tmp++) = g_Setup.m_an8HeartBeatEn[1];
			*(l_pu8Tmp++) = g_Setup.m_an8HeartBeatEn[2];
			*(l_pu8Tmp++) = g_Setup.m_an8HeartBeatEn[3]; 
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_CONHEART:  //工控机心跳指令 //wgh
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 13;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_CONHEART;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 13;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = p_u8VehIndex;  //端口号
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_POWERRUN:  //仪表上电报告 //wgh
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 64;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_POWERRUN;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 64;
			*(l_pu8Tmp++) = (g_DevCurTime.u16Year>>8)&0xFF;
			*(l_pu8Tmp++) = (g_DevCurTime.u16Year)&0xFF;
			*(l_pu8Tmp++) = g_DevCurTime.u8Month;
			*(l_pu8Tmp++) = g_DevCurTime.u8Day;
			*(l_pu8Tmp++) = g_DevCurTime.u8Hour;
			*(l_pu8Tmp++) = g_DevCurTime.u8Minute;
			*(l_pu8Tmp++) = g_DevCurTime.u8Second;
			memcpy( l_pu8Tmp, PROGRAMVERSION, 12);
			l_pu8Tmp+= 12;
			memcpy( l_pu8Tmp, PROGMODIFYDATE, 8);
			l_pu8Tmp+= 8;
			*(l_pu8Tmp++) =	PROMODPARAM.m_u8TriggerMode;
			*(l_pu8Tmp++) =	PROMODPARAM.m_u8LaneNum;
			*(l_pu8Tmp++) =	PROMODPARAM.m_u8ZTRow;
			*(l_pu8Tmp++) =	PROMODPARAM.m_u8IsYaFeng;
			memcpy( l_pu8Tmp, g_IPSetLocal.au8SubMask, 4);
			l_pu8Tmp+= 4;
			memcpy( l_pu8Tmp, g_IPSetLocal.au8IPAddr, 4);
			l_pu8Tmp+= 4;
			memcpy( l_pu8Tmp, g_IPSetLocal.au8GatewayIP, 4);
			l_pu8Tmp+= 4;
			memcpy( l_pu8Tmp, g_IPSetLocal.au8MACAddr, 6);
			l_pu8Tmp+= 6;
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO>>24)&0xFF;
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO>>16)&0xFF;
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO>>8)&0xFF;
			*(l_pu8Tmp++) = (g_IPSetLocal.u32LocalPortNO)&0xFF;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
//			SendDataNet( SOCKET_UDP, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			OSTimeDly(2);
			if(PROMODPARAM.m_u8UploadDataMode == 2 || PROMODPARAM.m_u8UploadDataMode == 3)
				U5SendBytes(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			else if(PROMODPARAM.m_u8UploadDataMode == 4)
				U2SendBytes(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			goto ENDOFF;
	
		case ADJ_GETVRSION:  // 获取版本号  //wgh
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 67;
			
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETVRSION;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 67;
			memcpy( l_pu8Tmp, PROGRAMVERSION, 12);
			l_pu8Tmp+= 12;
			memcpy( l_pu8Tmp, PROGMODIFYDATE, 8);
			l_pu8Tmp+= 8;
			tempver=GET_MASTER_FPGA_VERSION();
		
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ver =%x\r\n",
						          tempver
								  );
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			
			*(l_pu8Tmp++)=tempver>>24;
			*(l_pu8Tmp++)=tempver>>16;
			*(l_pu8Tmp++)=tempver>>8;
			*(l_pu8Tmp++)=tempver&0xFF;
			for (l_u8Tempi=0;l_u8Tempi<8;l_u8Tempi++)
			{
				tempver=GET_SLAVE_FPGA_VERSION(l_u8Tempi);
				*(l_pu8Tmp++)=tempver>>24;
				*(l_pu8Tmp++)=tempver>>16;
				*(l_pu8Tmp++)=tempver>>8;
				*(l_pu8Tmp++)=tempver&0xFF;
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ver%d =%x\r\n",
						          l_u8Tempi,tempver
								  );
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			}
			memset( l_pu8Tmp, 0, 2);
			break;
					
#if	TASKTIMEENABLE
		case ADJ_SENDRUNTIME:
		   	l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+48;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SENDRUNTIME;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 00;
			*(l_pu8Tmp++) = 11+48;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucWT_TASKFlag>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucWT_TASKFlag>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucWT_TASKFlag>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ucWT_TASKFlag & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_Task1>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_Task1>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_Task1>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulWT_Task1 & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_TaskDet>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_TaskDet>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulWT_TaskDet>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulWT_TaskDet & 0xFF;
			
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucAnalysisFlag>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucAnalysisFlag>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucAnalysisFlag>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ucAnalysisFlag & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysis1>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysis1>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysis1>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulAnalysis1 & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysisDet>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysisDet>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulAnalysisDet>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulAnalysisDet & 0xFF;
			
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucTaskWaveFlag>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucTaskWaveFlag>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ucTaskWaveFlag>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ucTaskWaveFlag & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWave1>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWave1>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWave1>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulTaskWave1 & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWaveDet>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWaveDet>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskWaveDet>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulTaskWaveDet & 0xFF;
			
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolFlag>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolFlag>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolFlag>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulTaskProtocolFlag & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocol1>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocol1>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocol1>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulTaskProtocol1 & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolDet>>24) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolDet>>16) & 0xFF;
			*(l_pu8Tmp++) = (TaskRunTimeStatistics.m_ulTaskProtocolDet>>8) & 0xFF;
			*(l_pu8Tmp++) = TaskRunTimeStatistics.m_ulTaskProtocolDet & 0xFF;				
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
#endif

		case ADJ_SENDAXLEGRP:	//轴组重 0x00
		case ADJ_SENDAXLE:  // 轴重 0x01
			//通信协议发送数据信息
			if( m_u8ResendFlag )
			{
				//重发指令
				if( 0xFF == m_u8ReSendFrameNum )
				{
					OSQPost(g_pQSendPC,&g_sndNetBuf[(g_vehicleInfo[p_u8VehIndex].u8id) & 0xFF]);
					//if(ret == OS_Q_FULL)
					//	g_ErrMsgState |= 0x10;
				}
				else
				{
					m_SendLastContext.u8Cnt = 1;
					m_SendLastContext.au32len[0] = g_sndNetBuf[g_vehicleInfo[p_u8VehIndex].u8id].au32len[m_u8ReSendFrameNum];
					memcpy( m_SendLastContext.au8data[0], g_sndNetBuf[g_vehicleInfo[p_u8VehIndex].u8id].au8data[m_u8ReSendFrameNum], m_SendLastContext.au32len[0] );
					//
					OSQPost(g_pQSendPC,&m_SendLastContext);
					//if(ret == OS_Q_FULL)
					//	g_ErrMsgState |= 0x10;	
				}
				
				m_u8ResendFlag = 0;
				m_u8ReSendFrameNum = 0xFF;
			}
			else
			{
				OSQPost(g_pQSendPC,&g_sndNetBuf[(g_vehicleInfo[p_u8VehIndex].u8id) & 0xFF]);
				l_pu8Tmp = NULL;
			}
			return;

		case ADJ_TIMESYNCHRONY:  // 时间同步 0x02
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_TIMESYNCHRONY;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;

	   	case ADJ_DEVICESTATUS:	 //状态帧，0x05，上位机主动查询
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 24;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			l_u8DeviceErr = g_u8DevErr;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_DEVICESTATUS;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 24;  
			*(l_pu8Tmp++) = PROMODPARAM.m_u8TriggerMode;
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
			*(l_pu8Tmp++) = l_u8DeviceErr;
			if((g_u8DeviceERR[0] & 0x0F)==0)
				*(l_pu8Tmp++) = 0;
			else
				*(l_pu8Tmp++) = 1;
			*(l_pu8Tmp++) = g_u8IOERR[0];
			*(l_pu8Tmp++) = g_u8IOERR[1];
			for( l_u8Tempi=1; l_u8Tempi<ERR_LEN; l_u8Tempi++ )
			{
				*(l_pu8Tmp++) = g_u8DeviceERR[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		case ADJ_RESTART:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_RESTART;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;  
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_IOUSEASK:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 37;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_IOUSEASK;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 37;
			for(l_u8Tempi = 0; l_u8Tempi<26; l_u8Tempi++)  
			{
				*(l_pu8Tmp++) = PROMODPARAM.m_au8IOUseEn[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_IOUSESET:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_IOUSESET;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_TESTCOM:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			if(p_u8VehIndex==1)
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 22;
			else
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 30;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_TESTCOM;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 00;
			if(p_u8VehIndex==1)
				*(l_pu8Tmp++) = 22;
			else
				*(l_pu8Tmp++) = 30;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			if(p_u8VehIndex==1)
			{
				*(l_pu8Tmp++) = CoilIN_IO1;
				*(l_pu8Tmp++) = CoilIN_IO2;
				*(l_pu8Tmp++) = CoilIN_IO3;
				*(l_pu8Tmp++) = CoilIN_IO4;
				*(l_pu8Tmp++) = CoilIN_IO5;
				*(l_pu8Tmp++) = CoilIN_IO6;
				*(l_pu8Tmp++) = CoilIN_IO7;
				*(l_pu8Tmp++) = CoilIN_IO8;
				*(l_pu8Tmp++) = CoilIN_IO9;
				*(l_pu8Tmp++) = CoilIN_IO10;
			}
			else
			{	
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
				*(l_pu8Tmp++) = 0;
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;


		case ADJ_CAPTURE:	 //抓拍，0x13，当触发相机抓拍时给上位机发送该命令，通知触发抓拍信号
//			m_u8ZhuaPai.u8Cnt = 1;
//			m_u8ZhuaPai.au32len[0] = 12;
//			l_pu8Tmp = m_u8ZhuaPai.au8data[0];
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 13;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_CAPTURE;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 13;
			if(p_u8VehIndex == 0)  
			{
				*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][6];  //流水号 车辆编号  
				*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][7];  //保留  车道号
			}
			else
			{
				*(l_pu8Tmp++) = 0;  //流水号 车辆编号  
				*(l_pu8Tmp++) = 0;  //保留  车道号
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		 /////////////////
		case ADJ_USESTAT:
			switch(p_u8VehIndex)
			{
				case 0:
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;	
					l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0x00;
					*(l_pu8Tmp++) = ADJ_USESTAT;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 12;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 0x00;  //保留
					*(l_pu8Tmp++) = 0x00;  //保留
				break;
				case 1:
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 92;	
					l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0x00;
					*(l_pu8Tmp++) = ADJ_USESTAT;
					*(l_pu8Tmp++) = 1;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 92;
					//
					for(l_u8Tempi = 0; l_u8Tempi<64; l_u8Tempi++) 
					{
						*(l_pu8Tmp++) = g_u8RunZTSta[l_u8Tempi];
					}
					for(l_u8Tempi = 0; l_u8Tempi<16; l_u8Tempi++) 
					{
						if(g_u8RunLoopSta[l_u8Tempi] == 2)
						{
							*(l_pu8Tmp++) = 0;
						}
						else
						{
							*(l_pu8Tmp++) = 1;
						}
					}
					if((g_u8DeviceERR[0] & 0x0F) == 0)
					{
						*(l_pu8Tmp++) = 0;
					}
					else
					{
						*(l_pu8Tmp++) = 1;
					}
					//
					*(l_pu8Tmp++) = 0x00;  //保留
					*(l_pu8Tmp++) = 0x00;  //保留
				break;
				case 2:
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;	
					l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0x00;
					*(l_pu8Tmp++) = ADJ_USESTAT;
					*(l_pu8Tmp++) = 2;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 12;
					*(l_pu8Tmp++) = 0x00;
					*(l_pu8Tmp++) = 0x00;  //保留
					*(l_pu8Tmp++) = 0x00;  //保留
				break;
				case 3:
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;	
					l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0xff;
					*(l_pu8Tmp++) = 0x00;
					*(l_pu8Tmp++) = ADJ_USESTAT;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 12;
					*(l_pu8Tmp++) = 3;
					*(l_pu8Tmp++) = 0x00;  //保留
					*(l_pu8Tmp++) = 0x00;  //保留
				break;
			}
			break;
		case ADJ_ZTARRAYASK:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 75;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_ZTARRAYASK;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 75;
			for(l_u8Tempi = 0; l_u8Tempi<64; l_u8Tempi++)
			{
				*(l_pu8Tmp++) = g_Setup.u8ArrayForZT[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		case ADJ_ZTARRAYSET:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_ZTARRAYSET;
			*(l_pu8Tmp++) = 2;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		case ADJ_CHLASK:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 31;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_CHLASK;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 31;
			for(l_u8Tempi = 0; l_u8Tempi<20; l_u8Tempi++)
			{
				*(l_pu8Tmp++) = PROMODPARAM.m_au8CHLSET[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		case ADJ_CHLSET:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;	
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];									 	
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_CHLSET;
			*(l_pu8Tmp++) = 2;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		case ADJ_GETSDINFO:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETSDINFO;
			*(l_pu8Tmp++) = p_u8VehIndex;
			l_u8TempVar = ((p_u8VehIndex)>>4)&0x0F;
			switch(l_u8TempVar)
			{
				case 0:
					// 车辆信息
					l_u8TempVar = (p_u8VehIndex)&0x0F;
					switch(l_u8TempVar)
					{
						case 1:
							// 车辆数
							g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 23;
							l_u32VehNum = 0;
							l_u32VehNext = 0;			
							l_u32VehCur = 0;
							if( SdHal_CheckCard( &sd_info ) )
							{
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
									if( ERR_SD_NOTNEW == l_u32Ret )
									{
										// 直接赋值
										l_u32VehNum = (g_u8SDRdBuf[SD_CTX_VEH_SUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_SUM+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_VEH_SUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_SUM+3];
										l_u32VehNext = (g_u8SDRdBuf[SD_CTX_VEH_NEXT+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NEXT+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_VEH_NEXT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NEXT+3];
										l_u32VehCur = (g_u8SDRdBuf[SD_CTX_VEH_CUR+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_CUR+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_VEH_CUR+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_CUR+3];
									}
									else
									{
									}
								}
							}
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 23;
							*(l_pu8Tmp++) = (l_u32VehNum>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNum>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNum>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNum) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehCur>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehCur>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehCur>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehCur) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNext>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNext>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNext>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32VehNext) & 0xFF;
							break;

						case 2:
							// 车辆信息
//							if( SdHal_CheckCard( &sd_info ) )
//							{
//								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
//								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_VEHINFO, g_u8SDRdBuf );
//								if( SD_NO_ERR == l_u32Ret )
//								{
//									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
//									if( ERR_SD_NOTNEW == l_u32Ret )
//									{
//										// 直接赋值
//										l_u32VehNum = (g_u8SDRdBuf[SD_CTX_VEH_SUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_SUM+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_VEH_SUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_SUM+3];
//										l_u32VehNext = (g_u8SDRdBuf[SD_CTX_VEH_NEXT+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_NEXT+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_VEH_NEXT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_NEXT+3];
//										l_u32VehCur = (g_u8SDRdBuf[SD_CTX_VEH_CUR+0]<<24) + (g_u8SDRdBuf[SD_CTX_VEH_CUR+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_VEH_CUR+2]<<8 ) + g_u8SDRdBuf[SD_CTX_VEH_CUR+3];
//									
//										// 上传
//										//l_u32Num = m_u32EndIx-m_u32BeginIx+1;
//										l_u32Num = (m_u32EndIx-m_u32BeginIx+1)>l_u32VehNum ? l_u32VehNum : (m_u32EndIx-m_u32BeginIx+1);//借用变量，增加保护	
//										for( l_u32SDTempIx=0; l_u32SDTempIx<l_u32Num; l_u32SDTempIx++ )
//										{
//											l_u32SDReadIx = (l_u32VehCur+(m_u32BeginIx-1)+l_u32SDTempIx) % SD_VEH_MAXNUM;
//											//
//											memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
//											l_u32RdAdd = SD_GETNEXT_SECTOR( SD_VEH_SEC_DATA_BGN, l_u32SDReadIx );
//											l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
//											if( SD_NO_ERR == l_u32Ret )
//											{
//												g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
//												g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = g_u8SDRdBuf[SD_VEH_SEC_DATA+5]*256+g_u8SDRdBuf[SD_VEH_SEC_DATA+6];
//												l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
//												memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_VEH_SEC_DATA, SD_VEH_SEC_DATA_LEN );
//
//												SendDataNet( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);	
//												OSTimeDly(50);
//											}																					
//										}
//									}
//									else
//									{
//									}
//								}
//							}
//							
							g_u8VehSendFlag = 1; 
							goto ENDOFF;

						default:	
							break;
					}
					break;

				case 1:
					// 日志
					l_u8TempVar = (p_u8VehIndex)&0x0F;
					switch(l_u8TempVar)
					{
						case 1:
							// 日志数
							g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 23;
							l_u32LogNum = 0;
							l_u32NextLog = 0;			
							l_u32CurLog = 0;
							if( SdHal_CheckCard( &sd_info ) )
							{
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
									if( ERR_SD_NOTNEW == l_u32Ret )
									{
										// 直接赋值
										l_u32LogNum = (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+3];
										l_u32NextLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+3];
										l_u32CurLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+3];
									}
									else
									{
									}
								}
							}
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 23;
							*(l_pu8Tmp++) = (l_u32LogNum>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32LogNum>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32LogNum>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32LogNum) & 0xFF;
							*(l_pu8Tmp++) = (l_u32CurLog>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32CurLog>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32CurLog>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32CurLog) & 0xFF;
							*(l_pu8Tmp++) = (l_u32NextLog>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32NextLog>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32NextLog>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32NextLog) & 0xFF;
							break;

						case 2:
							// 日志信息
//							if( SdHal_CheckCard( &sd_info ) )
//							{
//								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
//								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_SYSLOG, g_u8SDRdBuf );
//								if( SD_NO_ERR == l_u32Ret )
//								{
//									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
//									if( ERR_SD_NOTNEW == l_u32Ret )
//									{
//										// 直接赋值
//										l_u32LogNum = (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_LOGSUM+3];
//										l_u32NextLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_NEXTLOG+3];
//										l_u32CurLog = (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+0]<<24) + (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+1]<<16) 
//													+ (g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+2]<<8 ) + g_u8SDRdBuf[SD_CTX_DBGINFO_CURLOG+3];
//										
//										// 上传
//										l_u32TempNum = (m_u32EndIx-m_u32BeginIx+1)>l_u32LogNum ? l_u32LogNum : (m_u32EndIx-m_u32BeginIx+1);//借用变量，增加保护
//										l_u32Num = l_u32TempNum/10;						// 商
//										l_u32Remind = l_u32TempNum%10;
//										for( l_u32SDTempIx=0; l_u32SDTempIx<l_u32Num; l_u32SDTempIx++ )
//										{
//											l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
//										   	// 组帧
//											g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
//											g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 1011;
//											*(l_pu8Tmp++) = 0xff;
//											*(l_pu8Tmp++) = 0xff;
//											*(l_pu8Tmp++) = 0x00;
//											*(l_pu8Tmp++) = ADJ_GETSDINFO;
//											*(l_pu8Tmp++) = p_u8VehIndex;
//											*(l_pu8Tmp++) = (1011>>8)&0xFF;
//											*(l_pu8Tmp++) = (1011)&0xFF;
//											//
//											for( l_u32SDTempj=0; l_u32SDTempj<10; l_u32SDTempj++ )
//											{
//												l_u32SDReadIx = (l_u32CurLog+m_u32BeginIx+l_u32SDTempIx*10+l_u32SDTempj) % SD_LOG_MAX_NUM;
//												//
//												memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
//												l_u32RdAdd = SD_GETNEXT_SECTOR( SD_DBGINFO_SEC_DATA_BGN, l_u32SDReadIx );
//												l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
//												if( SD_NO_ERR == l_u32Ret )
//												{
//													memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA, 100 );
//													l_pu8Tmp += 100;
//												}
//												else
//												{
//													memset( l_pu8Tmp, 0, 100 );
//													l_pu8Tmp += 100;
//												}
//											}
//											*(l_pu8Tmp++) = 0;
//											*(l_pu8Tmp++) = 0;
//											AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
//											SendDataNet( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);	
//											OSTimeDly(50);										
//										}
//										if( l_u32Remind )
//										{
//											l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
//										   	// 组帧
//											g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
//											g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 1011;
//											*(l_pu8Tmp++) = 0xff;
//											*(l_pu8Tmp++) = 0xff;
//											*(l_pu8Tmp++) = 0x00;
//											*(l_pu8Tmp++) = ADJ_GETSDINFO;
//											*(l_pu8Tmp++) = p_u8VehIndex;
//											*(l_pu8Tmp++) = (1011>>8)&0xFF;
//											*(l_pu8Tmp++) = (1011)&0xFF;
//											//
//											for( l_u32SDTempj=0; l_u32SDTempj<l_u32Remind; l_u32SDTempj++ )
//											{
//												l_u32SDReadIx = (l_u32CurLog+m_u32BeginIx+l_u32SDTempIx*10+l_u32SDTempj) % SD_LOG_MAX_NUM;
//												//
//												memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
//												l_u32RdAdd = SD_GETNEXT_SECTOR( SD_DBGINFO_SEC_DATA_BGN, l_u32SDReadIx );
//												l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
//												if( SD_NO_ERR == l_u32Ret )
//												{
//													memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_DBGINFO_SEC_LOG_DATA, 100 );
//													l_pu8Tmp += 100;
//												}
//												else
//												{
//													memset( l_pu8Tmp, 0, 100 );
//													l_pu8Tmp += 100;
//												}
//											}
//											memset(l_pu8Tmp,0,100*(10-l_u32Remind));
//											l_pu8Tmp += 100*(10-l_u32Remind);
//											*(l_pu8Tmp++) = 0;
//											*(l_pu8Tmp++) = 0;
//											AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
//											SendDataNet( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);	
//											OSTimeDly(50);
//										}
//									}
//									else
//									{
//									}
//								}
//							}
							g_u8DebugInfoSendFlag = 1;
							goto ENDOFF;

						default:	
							break;
					}
					break;

				case 2:
					// 波形
					l_u8TempVar = (p_u8VehIndex)&0x0F;
					switch(l_u8TempVar)
					{
						case 1:
							// 波形数
							g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 23;
							l_u32WaveNum = 0;
							l_u32WaveNext = 0;			
							l_u32WaveCur = 0;
							if( SdHal_CheckCard( &sd_info ) )
							{
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
									if( ERR_SD_NOTNEW == l_u32Ret )
									{
										// 直接赋值
										l_u32WaveNum = (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+3];
										l_u32WaveNext = (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+3];
										l_u32WaveCur = (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_CURZT+3];
									}
									else
									{
									}
								}
							}
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 23;
							*(l_pu8Tmp++) = (l_u32WaveNum>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNum>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNum>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNum) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNext>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNext>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNext>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveNext) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveCur>>24) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveCur>>16) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveCur>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u32WaveCur) & 0xFF;
							break;
						case 2:
							// 波形信息
							if( SdHal_CheckCard( &sd_info ) )
							{
								memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
								l_u32Ret = SD_ReadBlock( &sd_info, SD_CONTEXT_WAVE, g_u8SDRdBuf );
								if( SD_NO_ERR == l_u32Ret )
								{
									l_u32Ret = FCheckSD( g_u8SDRdBuf, SD_NEW_SECTOR_FLAG_LEN );
									if( ERR_SD_NOTNEW == l_u32Ret )
									{
										// 直接赋值
										l_u32WaveNum = (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_ZTNUM+3];
										l_u32WaveNext = (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_NEXTZT+3];
										l_u32WaveCur = (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+0]<<24) + (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+1]<<16) 
													+ (g_u8SDRdBuf[SD_CTX_WAVE_CURZT+2]<<8 ) + g_u8SDRdBuf[SD_CTX_WAVE_CURZT+3];
									
										// 上传
										l_u32Num = m_u32EndIx-m_u32BeginIx+1;	
										for( l_u32SDTempIx=0; l_u32SDTempIx<l_u32Num; l_u32SDTempIx++ )
										{
											l_u32SDReadIx = (l_u32WaveCur+m_u32BeginIx+l_u32SDTempIx) % SD_WAVE_ZT_MAX_NUM;
											//
											memset( g_u8SDRdBuf, 0, SD_SECTOR_LEN );
											l_u32RdAdd = SD_GETNEXTWAVE_SECTOR( SD_WAVE_SEC_DATA_BGN, l_u32SDReadIx );
											l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd, g_u8SDRdBuf );
											memset( g_u8SDRdBuf2, 0, SD_SECTOR_LEN );
											l_u32Ret = SD_ReadBlock( &sd_info, l_u32RdAdd+1, g_u8SDRdBuf2 );
											if( SD_NO_ERR == l_u32Ret )
											{
												g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
												g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = g_u8SDRdBuf[SD_VEH_SEC_DATA+5]*256+g_u8SDRdBuf[SD_VEH_SEC_DATA+6];
												l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];

												//
												memcpy( l_pu8Tmp, g_u8SDRdBuf+SD_WAVE_SEC_ZT_DATA, SD_WAVE_SEC_DATA_LEN );
												l_pu8Tmp += SD_WAVE_SEC_DATA_LEN;
												memcpy( l_pu8Tmp, g_u8SDRdBuf2+SD_WAVE_SEC_ZT_DATA, SD_WAVE_SEC_DATA_LEN );
// 												SendDataNet( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
												TcpSendData( SOCKET_WT, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);	
												OSTimeDly(50);
											}																					
										}
									}
									else
									{
									}
								}
							}
							goto ENDOFF;;

						default:	
							break;
					}
					break;

				default:	
					break;
			}
			//*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0;
			break;
		case ADJ_SETSDINFO:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SETSDINFO;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			break;
		/////////////////////////////////////////////////////
		case ADJ_GETIPINFO:	 //读取仪表IP相关参数,0x20
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 34;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 34; 
			*(l_pu8Tmp++) = m_u8SuccessFlag; 
			memcpy(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][8], &g_IPSetLocal.au8IPAddr[0], 4 );
			memcpy(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][12], &g_IPSetLocal.au8SubMask[0], 4 );
			memcpy(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][16], &g_IPSetLocal.au8GatewayIP[0], 4 );
			//memcpy(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][20], (uint8*)&g_IPSetLocal.u32LocalPortNO, 4 );
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][20] = (g_IPSetLocal.u32LocalPortNO>>24)&0xFF;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][21] = (g_IPSetLocal.u32LocalPortNO>>16)&0xFF;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][22] = (g_IPSetLocal.u32LocalPortNO>>8)&0xFF;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][23] = (g_IPSetLocal.u32LocalPortNO)&0xFF;
			memcpy(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][24], &g_IPSetLocal.au8MACAddr[0], 6 );
			memset(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][30], 0, 2 );
			break;

		case ADJ_UPDATEIPINFO:	 //更改仪表IP相关参数,0x21
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_UPDATEIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 00;
			*(l_pu8Tmp++) = 12;  
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			memset(l_pu8Tmp, 0, 2 );
			break;

		case ADJ_NETCONNECT:	 //工控机检查线路连接
		    g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_NETCONNECT;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;  //流水号
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
		    break;

		///////////////////////////////////
		// 调校相关命令
		case ADJ_SAVE:	 //保存仪表参数指令,0xA1
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SAVE;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			
			break;

		case ADJ_GETNONEWEIGHT:	 //读取所有非重量修正参数,0xA4
		case ADJ_UPDATENONEWEIGHT:	 //更新所有非重量修正参数,0xA5
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 26;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = p_u8CommandNO;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 0;	   // 数据长度 2字节	
			*(l_pu8Tmp++) = 26;
			*(l_pu8Tmp++) = m_u8SuccessFlag;		
			*(l_pu8Tmp++) = (g_Setup.u32Password & 0xff000000)>>24;
			*(l_pu8Tmp++) = (g_Setup.u32Password & 0x00ff0000)>>16;
			*(l_pu8Tmp++) = (g_Setup.u32Password & 0x0000ff00)>>8;
			*(l_pu8Tmp++) = (g_Setup.u32Password & 0x000000ff);
			*(l_pu8Tmp++) = (g_Setup.u32Full & 0xff000000)>>24;
			*(l_pu8Tmp++) = (g_Setup.u32Full & 0x00ff0000)>>16;
			*(l_pu8Tmp++) = (g_Setup.u32Full & 0x0000ff00)>>8;
			*(l_pu8Tmp++) = (g_Setup.u32Full & 0x000000ff);
			*(l_pu8Tmp++) = g_Setup.u8MotionScale; 
			*(l_pu8Tmp++) = g_Setup.u8ComMode;	 
			*(l_pu8Tmp++) = g_Setup.u8BaudRate;	 
			*(l_pu8Tmp++) = g_Setup.u8DOG; 
			*(l_pu8Tmp++) = g_Setup.u8DiaoDianTag; 
			*(l_pu8Tmp++) = g_Setup.u8SendWaveEnable;
			memset(l_pu8Tmp, 0, 2 );
						
			break;

		case ADJ_GETMOTIONWEIGHT:	 //读取动态修正参数,0xA8
		case ADJ_UPDATEMOTIONWEIGHT:	 //更新动态修正参数,0xA9
		   	//标示类型，1：增益（4*64）
			//2：零点（4*64）
			//3：路面值（1*64）
			//4：缓坡长度（1*64）
			//5：弹性体高度（1*64）
			//6：上秤阈值、下秤阈值、上秤逻辑阈值、下秤逻辑阈值、上秤点数、下秤点数（16*6）、胶垫状态（1*8）、窄条间距（1*3）
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = p_u8CommandNO;
			*(l_pu8Tmp++) = p_u8VehIndex;
			
			if( p_u8CommandNO == ADJ_GETMOTIONWEIGHT )
			{
				if( 1 == p_u8VehIndex || 2 == p_u8VehIndex || 3 == p_u8VehIndex)
				{					
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if( 4==p_u8VehIndex || 5==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+64;
					*(l_pu8Tmp++) = (75>>8) & 0xFF;
					*(l_pu8Tmp++) = 75 & 0xFF;
				}
				else if( 6==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+80;
					*(l_pu8Tmp++) = (91>>8) & 0xFF;
					*(l_pu8Tmp++) = 91 & 0xFF;
				}
				else if( 23==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+148;
					*(l_pu8Tmp++) = (159>>8) & 0xFF;
					*(l_pu8Tmp++) = 159 & 0xFF;
				}
				else if( 7==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+401;
					*(l_pu8Tmp++) = (412>>8) & 0xFF;
					*(l_pu8Tmp++) = 412 & 0xFF;
				}
				else if( 8==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+41;
					*(l_pu8Tmp++) = (52>>8) & 0xFF;
					*(l_pu8Tmp++) = 52 & 0xFF;
				}
				else if( 9==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+21;
					*(l_pu8Tmp++) = (32>>8) & 0xFF;
					*(l_pu8Tmp++) = 32 & 0xFF;
				}
				else if( 14==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+ZERONUM*2*2;
					*(l_pu8Tmp++) = ((11+ZERONUM*2*2)>>8) & 0xFF;
					*(l_pu8Tmp++) = (11+ZERONUM*2*2) & 0xFF;
				}
				else if(15==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if(16==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if(17==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if( 20==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+81;
					*(l_pu8Tmp++) = (92>>8) & 0xFF;
					*(l_pu8Tmp++) = 92 & 0xFF;
				}
				else if( 21==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+160;
					*(l_pu8Tmp++) = (171>>8) & 0xFF;
					*(l_pu8Tmp++) = 171 & 0xFF;
				}
				else if( 22==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+20;
					*(l_pu8Tmp++) = (31>>8) & 0xFF;
					*(l_pu8Tmp++) = 31 & 0xFF;
				}
			}
			else
			{
				if( 1 == p_u8VehIndex || 2 == p_u8VehIndex || 3 == p_u8VehIndex)
				{					
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if( 4==p_u8VehIndex || 5==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+64;
					*(l_pu8Tmp++) = (75>>8) & 0xFF;
					*(l_pu8Tmp++) = 75 & 0xFF;
				}
				else if( 6==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+80;
					*(l_pu8Tmp++) = (91>>8) & 0xFF;
					*(l_pu8Tmp++) = 91 & 0xFF;
				}
				else if( 23==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+148;
					*(l_pu8Tmp++) = (159>>8) & 0xFF;
					*(l_pu8Tmp++) = 159 & 0xFF;
				}
				else if( 7==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+401;
					*(l_pu8Tmp++) = (412>>8) & 0xFF;
					*(l_pu8Tmp++) = 412 & 0xFF;
				}
				else if( 8==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+41;
					*(l_pu8Tmp++) = (52>>8) & 0xFF;
					*(l_pu8Tmp++) = 52 & 0xFF;
				}
				else if( 9==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+21;
					*(l_pu8Tmp++) = (32>>8) & 0xFF;
					*(l_pu8Tmp++) = 32 & 0xFF;
				}
				else if(15==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if(16==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if(17==p_u8VehIndex)
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+128;
					*(l_pu8Tmp++) = (139>>8) & 0xFF;
					*(l_pu8Tmp++) = 139 & 0xFF;
				}
				else if( 20==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+81;
					*(l_pu8Tmp++) = (11+81>>8) & 0xFF;
					*(l_pu8Tmp++) = (11+81) & 0xFF;
				}
				else if( 21==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+160;
					*(l_pu8Tmp++) = (171>>8) & 0xFF;
					*(l_pu8Tmp++) = 171 & 0xFF;
				}
				else if( 22==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+20;
					*(l_pu8Tmp++) = (31>>8) & 0xFF;
					*(l_pu8Tmp++) = 31 & 0xFF;
				}
			}			
			switch( p_u8VehIndex )
			{
				case 1:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						//*(uint16*)(l_pu8Tmp) = g_Setup.an32AxGain[l_u8Tempi];
						//l_pu8Tmp += 2;
						*(l_pu8Tmp++) = (g_Setup.an32AxGain[l_u8Tempi]>>8) & 0xFF;
						*(l_pu8Tmp++) = (g_Setup.an32AxGain[l_u8Tempi]) & 0xFF;
						
					}
					break;

				case 2:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						//*(uint16*)(l_pu8Tmp) = g_Setup.an32Zero[l_u8Tempi];
						//l_pu8Tmp += 2;
						*(l_pu8Tmp++) = (g_Setup.an32Zero[l_u8Tempi]>>8) & 0xFF;
						*(l_pu8Tmp++) = (g_Setup.an32Zero[l_u8Tempi]) & 0xFF;
					}
					break;

				case 3:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = 0;
						*(l_pu8Tmp++) = g_Setup.an8ZTStatus[l_u8Tempi];
						
					} 
//					memset(l_pu8Tmp, 0, 64 );
//					l_pu8Tmp += 64;
					break;

				case 4:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = g_Setup.an8LengthPodu[l_u8Tempi];
						
					}	
					break;

				case 5:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = g_Setup.an8HeightTanxingti[l_u8Tempi];
						
					}
					break;

				case 6:
					*(l_pu8Tmp++) = (g_Setup.u16UpValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16UpValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16DownValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16DownValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicUpValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicUpValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicDownValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicDownValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8UPPoints>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8UPPoints) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8DownPoints>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8DownPoints) & 0xFF;

					// 增加胶垫状态（1*8）,更改变量
					// 增加胶垫状态（1*16）,更改变量
					//低字节是第一方向，高字节是第二方向
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //双向车道第一方向各排0-31
					{
						
						*(l_pu8Tmp++) = 0xFF&g_Setup.u16UseToWghFlag[l_u8Tempi];//20130515 浩叔发现bug
						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16UseToWghFlag[l_u8Tempi]>>8);	   						
					}
//					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //双向车道第二方向各排32-64
//					{ 						
//						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16JiaodianState[l_u8Tempi]>>8);//20130515 浩叔发现bug 						
//					}
					// 增加窄条间距（2*3）,更改变量
					for( l_u8Tempi=0; l_u8Tempi < CHL_MAXNUM; l_u8Tempi++ )
					{
						for( l_u8Tempj=0; l_u8Tempj < CHANNALLINESUM-1; l_u8Tempj++ )
						{
//							*(l_pu8Tmp++) = (g_Setup.u16DistBetween[][l_u8Tempi]>>8)&0xff;//20130515 浩叔发现bug
//							*(l_pu8Tmp++) = g_Setup.u16DistBetween[][l_u8Tempi]&0xff;
							*(l_pu8Tmp++) = (g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj]>>8)&0xff;
							*(l_pu8Tmp++) = (g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj])&0xff;
						}
					}
					break;
				case 23:
					*(l_pu8Tmp++) = (g_Setup.u16UpValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16UpValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16DownValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16DownValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicUpValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicUpValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicDownValue>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u16logicDownValue) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8UPPoints>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8UPPoints) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8DownPoints>>8) & 0xFF;
					*(l_pu8Tmp++) = (g_Setup.u8DownPoints) & 0xFF;

					// ??????(1*8),????
					// ??????(1*16),????
					//????????,????????
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //??????????0-31
					{
						
						*(l_pu8Tmp++) = 0xFF&g_Setup.u16UseToWghFlag[l_u8Tempi];//20130515 ????bug
						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16UseToWghFlag[l_u8Tempi]>>8);	   						
					}
//					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //??????????32-64
//					{ 						
//						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16JiaodianState[l_u8Tempi]>>8);//20130515 ????bug 						
//					}
					// ??????(2*3),????
					/*
					for( l_u8Tempi=0; l_u8Tempi < CHL_MAXNUM; l_u8Tempi++ )
					{
						for( l_u8Tempj=0; l_u8Tempj < CHANNALLINESUM-1; l_u8Tempj++ )
						{
//							*(l_pu8Tmp++) = (g_Setup.u16DistBetween[][l_u8Tempi]>>8)&0xff;//20130515 ????bug
//							*(l_pu8Tmp++) = g_Setup.u16DistBetween[][l_u8Tempi]&0xff;
							*(l_pu8Tmp++) = (g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj]>>8)&0xff;
							*(l_pu8Tmp++) = (g_cProModParam.m_au16ZTDist[l_u8Tempi][l_u8Tempj])&0xff;
						}
					}
					*/
					for( l_u8Tempi=0; l_u8Tempi < 64; l_u8Tempi++ )
					{
						*(l_pu8Tmp++)=(g_cProModParam.m_au16ZTGap[l_u8Tempi]>>8) & 0xff;
						*(l_pu8Tmp++)=g_cProModParam.m_au16ZTGap[l_u8Tempi] & 0xff;
					}
					break;
				case 7:
					*(l_pu8Tmp++) = m_u8LaneNum;
					for( l_u8Tempi=0; l_u8Tempi<10; l_u8Tempi++ )
					{
						for( l_u8Tempj=0; l_u8Tempj<20; l_u8Tempj++ )
						{
							*(l_pu8Tmp++) = (g_Setup.au16VehSpeedModify[m_u8LaneNum][l_u8Tempi][l_u8Tempj]>>8) & 0xFF;
							*(l_pu8Tmp++) = g_Setup.au16VehSpeedModify[m_u8LaneNum][l_u8Tempi][l_u8Tempj] & 0xFF;
						}
					}
					break;
				case 8:
					*(l_pu8Tmp++) = m_u8LaneNum;
					for( l_u8Tempj=0; l_u8Tempj<20; l_u8Tempj++ )
					{
						*(l_pu8Tmp++) = (g_Setup.au16Speedmodify[m_u8LaneNum][l_u8Tempj]>>8) & 0xFF;
						*(l_pu8Tmp++) = g_Setup.au16Speedmodify[m_u8LaneNum][l_u8Tempj] & 0xFF;
					}
					break;
				case 9:
					*(l_pu8Tmp++) = m_u8LaneNum;
					for( l_u8Tempj=0; l_u8Tempj<10; l_u8Tempj++ )
					{
						*(l_pu8Tmp++) = (g_Setup.au16VehTotalModify[m_u8LaneNum][l_u8Tempj]>>8) & 0xFF;
						*(l_pu8Tmp++) = g_Setup.au16VehTotalModify[m_u8LaneNum][l_u8Tempj] & 0xFF;
					}
					break;
				case 14:
				//	DisableFPGA_INT(); // 禁止FPGA中断，再开启
//					g_u8TestFlag = 1;
					for( l_u16Tempi=0; l_u16Tempi<ZERONUM; l_u16Tempi++ ) 
					{
//						if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//						{
//							if( 255!=aFPGALogicNUM2LaneNUM[l_u8Tempi] && 255!=aFPGALogicNUM2ChlNUM[l_u8Tempi] )
//							{
//								
//								l_u16ZTCurValue = GETCURRENTDATA( aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi] );
//								*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
//								*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
//							}
//							else
//							{
//								*(l_pu8Tmp++) = 0;
//								*(l_pu8Tmp++) = 0;
//							}
//						}
//						else
//						{
						//	l_u16ZTCurValue = GETCURRENTDATA( l_u8Tempi/8, l_u8Tempi%8  );
						//	*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
						//	*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
								*(l_pu8Tmp++)=(g_Setup.an32ImmediateValue[l_u16Tempi]>>8)&0xFF;
								*(l_pu8Tmp++)=(g_Setup.an32ImmediateValue[l_u16Tempi])&0xFF;
//						}
						
					}
					for( l_u16Tempi=0; l_u16Tempi<ZERONUM; l_u16Tempi++ ) 
					{
//						if(PROMODPARAM.m_u8TriggerMode == 1 || PROMODPARAM.m_u8TriggerMode == 3)
//						{
//							if( 255!=aFPGALogicNUM2LaneNUM[l_u8Tempi] && 255!=aFPGALogicNUM2ChlNUM[l_u8Tempi] )
//							{
//								l_u16ZTCurValue = GETCURRENTZERO( aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi] );
//								*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
//								*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
//							}
//							else
//							{
//								*(l_pu8Tmp++) = 0;
//								*(l_pu8Tmp++) = 0;
//							}
//						}
//						else
//						{
			//				l_u16ZTCurValue = GETCURRENTZERO( l_u8Tempi/8, l_u8Tempi%8  );
			//				*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
			//				*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
								*(l_pu8Tmp++)=(g_Setup.an32Zero[l_u16Tempi]>>8)&0xFF;
								*(l_pu8Tmp++)=(g_Setup.an32Zero[l_u16Tempi])&0xFF;
//						}
						
					}
				//	EnableFPGA_INT();
					break;
				case 15:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = 0;
						*(l_pu8Tmp++) = g_cProModParam.m_n8LouDL[l_u8Tempi];
						
					}
					break;
				case 16:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = 0;
						*(l_pu8Tmp++) = g_cProModParam.m_n8CFDian[l_u8Tempi];
						
					}
					break;
				case 17:
					for(l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++)
					{
						*(l_pu8Tmp++) =	(g_cProModParam.m_u16LRLocation[0][l_u8Tempi] >> 8) & 0xFF;
						*(l_pu8Tmp++) =	g_cProModParam.m_u16LRLocation[0][l_u8Tempi] & 0xFF;
						*(l_pu8Tmp++) =	(g_cProModParam.m_u16LRLocation[1][l_u8Tempi] >> 8) & 0xFF;
						*(l_pu8Tmp++) =	g_cProModParam.m_u16LRLocation[1][l_u8Tempi] & 0xFF;
					}
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = g_cProModParam.m_u8JGKZ;
					for(l_u8Tempi=41; l_u8Tempi<64; l_u8Tempi++)
					{
						*(l_pu8Tmp++) = 0;
						*(l_pu8Tmp++) = 0;
					}
					break;
				case 20:
					*(l_pu8Tmp++) = g_cProModParam.m_u8ZTRow;
					*(l_pu8Tmp++) = g_cProModParam.m_u8LaneNum;
					*(l_pu8Tmp++) = g_cProModParam.m_u8TriggerMode;
					*(l_pu8Tmp++) = g_cProModParam.m_u8IsYaFeng;
					*(l_pu8Tmp++) = g_cProModParam.m_u8UploadDataMode;
					*(l_pu8Tmp++) = g_cProModParam.m_u8DataWaveEn;
					*(l_pu8Tmp++) = g_cProModParam.m_u8VehWaveSample;
					*(l_pu8Tmp++) = g_cProModParam.m_u8CaptureMode;
					*(l_pu8Tmp++) = g_cProModParam.m_u8TransmitSysEn;
					*(l_pu8Tmp++) = (g_cProModParam.m_u16SDSaveEn>>8) & 0xFF;
					*(l_pu8Tmp++) = g_cProModParam.m_u16SDSaveEn & 0xFF;
					*(l_pu8Tmp++) = (g_cProModParam.m_u16ExEquiType>>8) & 0xFF;
					*(l_pu8Tmp++) = g_cProModParam.m_u16ExEquiType & 0xFF;
					for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)
						*(l_pu8Tmp++) = g_cProModParam.m_au8AcqBoardFlag[l_u8Tempi];
					for(l_u8Tempi=0; l_u8Tempi<16; l_u8Tempi++)
						for(l_u8Tempj=0; l_u8Tempj<2; l_u8Tempj++)
							*(l_pu8Tmp++) = g_cProModParam.m_u8CaptureFlag[l_u8Tempi][l_u8Tempj];
					*(l_pu8Tmp++) = g_cProModParam.m_u8BwdDirecFlag;
					for(l_u8Tempi=0; l_u8Tempi<19; l_u8Tempi++)
						*(l_pu8Tmp++) = g_cProModParam.m_au8VehDirecFlag[l_u8Tempi];
					for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)
					{
						*(l_pu8Tmp++) = g_cProModParam.m_u8EnableFlag[l_u8Tempi];
					}
					break;
				case 21:
					for(l_u8Tempi=0; l_u8Tempi<40; l_u8Tempi++)
					{
						*(l_pu8Tmp++) = (g_cProModParam.m_au32Thres[l_u8Tempi]>>24)&0xFF;
						*(l_pu8Tmp++) = (g_cProModParam.m_au32Thres[l_u8Tempi]>>16)&0xFF;
						*(l_pu8Tmp++) = (g_cProModParam.m_au32Thres[l_u8Tempi]>>8)&0xFF;
						*(l_pu8Tmp++) = g_cProModParam.m_au32Thres[l_u8Tempi]&0xFF;
					}
					break;
				case 22:
					for(l_u8Tempi=0; l_u8Tempi<20; l_u8Tempi++)
					{
						*(l_pu8Tmp++) = (g_cProModParam.m_au8LaneBetwWide[l_u8Tempi])&0xFF;
					}
					break;
				default:
					return;
			}
			
			memset(l_pu8Tmp, 0, 2 );
			break;
		case Para_Read:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 88;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = Para_Read;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
			*(l_pu8Tmp++) = 0x58;				   //长度
			*(l_pu8Tmp++) = LOOP_BD_lane;		   // 车道号
//			*(l_pu8Tmp++) = (LANE_TRAFFIC_DATA.mu32LaneNum >> 24) & 0xFF ;	 
//			*(l_pu8Tmp++) = (LANE_TRAFFIC_DATA.mu32LaneNum >> 16) & 0xFF;
//			*(l_pu8Tmp++) = (LANE_TRAFFIC_DATA.mu32LaneNum >> 8) & 0xFF;
//			*(l_pu8Tmp++) = LANE_TRAFFIC_DATA.mu32LaneNum & 0xFF;
			*(l_pu8Tmp++) = (g_cJDSysParam.m_au16Elevation >> 8) & 0xFF;	 // 高程
			*(l_pu8Tmp++) = g_cJDSysParam.m_au16Elevation & 0xFF;
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[0];	 //  经度
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[1];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[2];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[3];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[4];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[5];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[6];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LngBuf[7];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[0];		//纬度
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[1];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[2];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[3];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[4];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[5];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[6];
			*(l_pu8Tmp++) = g_cJDSysParam.m_au8LatBuf[7];
			for(l_u8Tempi=0; l_u8Tempi<15; l_u8Tempi++)			//站点编号
			{
				*(l_pu8Tmp++) = g_cJDSysParam.m_au8StationID[l_u8Tempi];
			}
			for(l_u8Tempi=0; l_u8Tempi<16; l_u8Tempi++)		   	//身份识别码
			{
				*(l_pu8Tmp++) = g_cJDSysParam.m_au8DeviceID[l_u8Tempi];
			}
			for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)		  	//户名
			{
			    *(l_pu8Tmp++) = g_cJDSysParam.m_au8UseName[l_u8Tempi];
			}
			for(l_u8Tempi=0; l_u8Tempi<8; l_u8Tempi++)			//密码
			{
				*(l_pu8Tmp++) = g_cJDSysParam.m_au8KeyNum[l_u8Tempi];  
			}				
			*(l_pu8Tmp++) = (g_cJDSysParam.m_u16MidKeHuoThr[LOOP_BD_lane - 1] >> 8) & 0xFF;	 	//FZX
			*(l_pu8Tmp++) = g_cJDSysParam.m_u16MidKeHuoThr[LOOP_BD_lane - 1];
			*(l_pu8Tmp++) = (g_cJDSysParam.m_u16SmallKeHuoThr[LOOP_BD_lane - 1] >> 8) & 0xFF;	   		//FB
			*(l_pu8Tmp++) = g_cJDSysParam.m_u16SmallKeHuoThr[LOOP_BD_lane - 1];
			*(l_pu8Tmp++) = g_cJDSysParam.m_u8SavePeriod;
			*(l_pu8Tmp++) =	(g_cJDSysParam.m_u16SmallandMid >> 8) & 0xFF;	
			*(l_pu8Tmp++) =	g_cJDSysParam.m_u16SmallandMid;
			*(l_pu8Tmp++) = g_cJDSysParam.m_u8LaneDir;
			*(l_pu8Tmp++) = g_cJDSysParam.m_u8OrderMod;
			*(l_pu8Tmp++) = g_cJDSysParam.m_u8TransMod;
			*(l_pu8Tmp++) = g_cJDSysParam.m_u8SendVehType;
			*(l_pu8Tmp++) =	0x00;					//预留
			*(l_pu8Tmp++) = 0x00;					//预留
			break;		
		case Para_Save:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = Para_Save;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = 0x0C;			   //长度
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;			  // 保留
			*(l_pu8Tmp++) = 0x00;			  // 保留
			break;
		case JD_Reset:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			ReadFPGAGlobalTime(l_u32TimeNow);
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = p_u8VehIndex;
			*(l_pu8Tmp++) = JD_Reset;
			*(l_pu8Tmp++) = 0x0C;			   //长度
			//*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = l_u32TimeNow&0xFF;
			*(l_pu8Tmp++) = (l_u32TimeNow>>8)&0xFF;
			*(l_pu8Tmp++) = (l_u32TimeNow>>16)&0xFF;
			*(l_pu8Tmp++) = (l_u32TimeNow>>24)&0xFF;
			//*(l_pu8Tmp++) = m_u8SuccessFlag;
			//*(l_pu8Tmp++) = 0x00;			  // 保留
			//*(l_pu8Tmp++) = 0x00;			  // 保留
			break;
		case JD_Init:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = JD_Init;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = 0x0C;			   //长度
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;			  // 保留
			*(l_pu8Tmp++) = 0x00;			  // 保留
			break;
		case ADJ_CTRSAMPPOW:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_CTRSAMPPOW;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = 0x0C;			   //长度
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = p_u8VehIndex;			  // 保留
			*(l_pu8Tmp++) = 0x00;
			break;
		case ADJ_ZTINTENASK:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 73;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_ZTINTENASK;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = 0x49;			   //长度
			for(l_u8Tempi=0; l_u8Tempi<64; l_u8Tempi++)
			{
				*(l_pu8Tmp++) = PROMODPARAM.m_au8ZTIntEn[l_u8Tempi];
			}
			break;
		case ADJ_ZTINTENSET:
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_ZTINTENSET;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x00;				   //长度
		    *(l_pu8Tmp++) = 0x0C;			   //长度
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;			  // 保留
			*(l_pu8Tmp++) = 0x00;
			break;
		default:
			break; 
	}
	
	//发送到PC
	AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
	//通信协议发送数据信息
	
	if( ADJ_EXT == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3] && (2 == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][4] || 1 == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][4]))
	{
		//OSQPost(g_pQNetWavePC,&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
		//	
		g_cIOChange[g_u8IOChangeIx].u8Id = 0xAC;
		g_cIOChange[g_u8IOChangeIx].u16ptCnt = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0];
		for(l_u8Tempi = 0; l_u8Tempi < g_cIOChange[g_u8IOChangeIx].u16ptCnt; l_u8Tempi++)
		{
			g_cIOChange[g_u8IOChangeIx].u16data[0][l_u8Tempi] = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][l_u8Tempi];
		}
		//SendDataNet( SOCKET_WAVE, g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]); 
#if DEBUGINFO_TASK_SP_EN > 0
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"#############8F=%d。\r\n",
						           g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][4] );
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
#endif
		ret = OSQPost(g_pQWaveMsg,&g_cIOChange[g_u8IOChangeIx]);
		if(ret == OS_Q_FULL)
		{
			g_ErrMsgState |= 0x100;
		}
		//
		g_u8IOChangeIx ++;
		g_u8IOChangeIx %= LASERINFO_LEN;
	}
	else
	{
		if(JD_Reset == p_u8CommandNO)
		{
			OSQPost(g_pQJDSend, &g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
		}
		else
		{
			OSQPost(g_pQSendPC,&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
		}
		if(ADJ_SUPERCOMD == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3])
		{
			if(PROMODPARAM.m_u8UploadDataMode == 2 || PROMODPARAM.m_u8UploadDataMode == 3)
				U5SendBytes(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0], g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
			else if(PROMODPARAM.m_u8UploadDataMode == 4)	
				U2SendBytes(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0], g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]);
		}
	}
	


#if DEBUGINFO_TASK_SP_EN > 0
// 	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp FormFrame&Reply Cmd=%x [Net]。\r\n",
// 					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
// 					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
// 						   g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3]);
// 	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 	g_u16CurDebugInd++;
// 	g_u16CurDebugInd %= COM_NUM;
// 	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 	OSSemPost(g_pSemSendThtouSP);
#endif
	//
ENDOFF:
	m_u8SuccessFlag = 1;// 发送完毕，置1，表示失败
	g_u8CurSendNetOrderIndex++;
	g_u8CurSendNetOrderIndex %= TEMP_SIZE;//发送完毕自增
	memset( &g_sndNetOrderBuf[g_u8CurSendNetOrderIndex], 0, sizeof(sndFrame_Struct) );
	l_pu8Tmp = NULL;

}

/*********************************************************************************************************
** Function name:		ToBuff0
** Descriptions:		通过过车后得到的数据，根据协议组帧，然后发送.
**                      先把所有数据保存至大buf中，然后根据分包分别保存至每一帧中
** input parameters:	void					 
** output parameters:	void
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121106	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ToBuff0( Vehicle_Struct *pdata )
{
	//变量定义
	uint8		l_u8PackNO = 0;	// 包数
	uint8		l_u8AllPackNum = 0;//包总数
	//
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据
	uint16 *	l_pu16Tmp;	   //指针，指向窄条点数据
	uint8 *     l_pu8SDtemp;   //SD
	uint16      l_u16SDLen;
	Channal_Struct *		l_pChlData;	  //指针，指向窄条结构体
	uint8 *		l_pu8SendBufTmp;	   //指针，指向发送数据结构体数组
	//
	uint8		l_u8FrameHeadNum = 13;	   //帧头字节数，根据最终协议格式修改
	//uint8		l_u8FrameVehInfoNum = 100;	   //整车重量等信息字节数，根据最终协议格式修改
	//
	uint32		l_u32FrameLength = 0; // 帧长计数
	uint32		l_u32tmp = 0;
	//
	uint8		l_u8AxleListIndex = 0;// 轴序号
	uint8		l_u8ZhaitiaoListIndex = 0;// 窄条序号
	uint16		l_u16PtsIndex = 0;//点数序号
	uint16		l_u16Tempi=0,l_u16Tempi2;//临时序号
	uint16		l_u16Index1,l_u16Index2;

	//uint8		l_u8PtsOverFlag = 0;//点数超过最大值（10000），跳出

	uint16		l_u16LastPackSize = 0;//最后一帧大小
	int16		l_s8Acc = 0;
	uint8		ret;

	//
// 	uint16	l_u16SendLen = 0;
// 	uint8	l_u8Err = 0;

	// 增加保护机制，wujinlong
	if( 0 == pdata->u8AixCnt )
	{
		//return;
	}
	//
	//SD卡存储
	l_pu8SDtemp = g_u8SDVehBuff[g_u8SDBuffIndex];
	// 需增加文件头内容
	*(l_pu8SDtemp++) = 0xFF;	 //帧头
	*(l_pu8SDtemp++) = 0xFF;
	*(l_pu8SDtemp++) = 0x00; //从机地址保留
	if( g_Setup.u8ComMode )	   //命令号
		*(l_pu8SDtemp++) = ADJ_SENDAXLE; //命令模式1
	else
		*(l_pu8SDtemp++) = ADJ_SENDAXLEGRP; //命令模式0
	*(l_pu8SDtemp++) = 0;
	l_u16SDLen = (28+(pdata->u8AixCnt<<1)+pdata->u8AixGrpCnt+((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt+2+(CHANNELNUM<<2)+10+10+2);
	*(l_pu8SDtemp++) = l_u16SDLen>>8;
	*(l_pu8SDtemp++) = l_u16SDLen;
	*(l_pu8SDtemp++) = pdata->u8id;
	*(l_pu8SDtemp++) = 2;
	*(l_pu8SDtemp++) = 0; // 帧序号
	*(l_pu8SDtemp++) = CHANNELNUM; // 通道总数
	*(l_pu8SDtemp++) = 0x00; //	 预留
	*(l_pu8SDtemp++) = 0x00; //	 预留
	
	//
//	*(l_pu8SendBufTmp++)=0;
	//将所有数据组成一帧
    l_pu8Tmp = g_au8AllNetSendBuf;

   //先计算帧长，后进行封装帧操作。
	l_u32tmp = 0;
	if(PROMODPARAM.m_u8DataWaveEn == 1)
	{
		for(l_u16Index1 = 0;l_u16Index1 < pdata->u8AixCnt;l_u16Index1++)  
		{
			l_u16Tempi = pdata->u16AxleList[l_u16Index1];
			for(l_u16Index2 = 0;l_u16Index2< g_AxleInfo[l_u16Tempi].u16ChListSum;l_u16Index2++)
			{
				l_u16Tempi2 = g_AxleInfo[l_u16Tempi].u16ChList[l_u16Index2];
				l_u32tmp += (g_ChlData[l_u16Tempi2].u16ptCnt<<1) + 16;
			}
		}
		l_u32tmp += (CHANNELNUM<<2);
	}

	// 增加F4代码g_Setup.u8SendF4CodeEnable的控制，计算分两步：（1）去除F4代码、CRC的长度，（2）增加F4代码长度
	// added by wujinlong 20130806
   	if( g_Setup.u8ComMode )
	{	//帧长 = 年+月...（15）+轴重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+预留(10) ;
		l_u32FrameLength = 15 + (pdata->u8AixCnt<<1) + pdata->u8AixGrpCnt + ((pdata->u8AixCnt -1)<<1) + pdata->u8AixCnt +2+ l_u32tmp + 2 + 10 + 10 + 2;
	}
	else
	{
		//帧长 = 年+月...（15）+轴组重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+ 预留(10) ; 
		l_u32FrameLength = 15 + (pdata->u8AixGrpCnt<<1) + pdata->u8AixGrpCnt +((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt + 2+ + l_u32tmp + 2 + 10 + 10 + 2;
	}

//#if  SHOWVEHPASSDEBUG > 0 //显示过车调试代码 
//	if( g_Setup.u8SendF4CodeEnable )
//	{
//		l_u16Index1 = 4 + g_BugCode[pdata->u8id].u8NextPos;
//	}
//	else
//	{
//		l_u16Index1 = 4;
//	}
//#else
//	l_u16Index1 = 4;
//#endif
//	l_u32FrameLength += l_u16Index1;
	
	if(l_u32FrameLength > SENDALLSIZE)
	{
		g_ErrMsgState |= 0x10000; 	
		return;
	}
	///////////////////////////////////
	// 增加轴重等信息  根据命令号，添加轴重还是轴组重
//	*(uint16*)(l_pu8Tmp++) = g_DevCurTime.u16Year;	  // 时间
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year>>8) & 0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year) & 0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1); 
	*(l_pu8Tmp++) = g_DevCurTime.u8Month;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = g_DevCurTime.u8Day;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = g_DevCurTime.u8Hour;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = g_DevCurTime.u8Minute;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = g_DevCurTime.u8Second;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	 // 温度
	*(l_pu8Tmp++) = g_n32Temperature & 0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
//	if( 0x01 == PROMODPARAM.m_u8TriggerMode || 0x03 == PROMODPARAM.m_u8TriggerMode)
//	{
//		// 激光模式下
//		*(l_pu8Tmp++) = ((pdata->u8psLane)<<4)&0xF0;
//	}
//	else if( 0x02==PROMODPARAM.m_u8TriggerMode || 0x04==PROMODPARAM.m_u8TriggerMode )
//	{
		// 线圈模式下
//		if(pdata->u8psLane%2==0)		
//			*(l_pu8Tmp++) = (((pdata->u8psLane)<<4)&0xF0) + ((pdata->u8psLane)&0x0F);   	// 车道编号
//		else
//			*(l_pu8Tmp++) = (((pdata->u8psLane/2)<<4)&0xF0) + (((pdata->u8psLane/2+1))&0x0F);
///////////////////////////////////////////////////////////////
	//*(l_pu8Tmp++) = (g_cProModParam.m_au8CHLSET[pdata->u8psLane]<<4)&0xF0 + g_cProModParam.m_au8CHLSET[pdata->u8psLane];
		*(l_pu8Tmp++)=pdata->m_Capture_LaneNum;
////////////////////////////////////////////////////////////
//	}
//	else
//	{
//		*(l_pu8Tmp++) = ((pdata->u8psLane)<<4)&0xF0;
//	}
	
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	//*(l_pu8Tmp++) = 1;
	*(l_pu8Tmp++) = pdata->u8lvl;		 // 可信度：100-0,算法给出
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	//
	//*(uint16*)(l_pu8Tmp++) = pdata->u16speed;		 // 速度
	l_u32tmp = 0;														// 重新计算车速，20131203
	for (l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++)
	{
		l_u32tmp+= g_AxleInfo[pdata->u16AxleList[l_u16Tempi]].u16speed;  				 			
	}
	pdata->u16speed = l_u32tmp/pdata->u8AixCnt;

	//
	if( pdata->u16speed > VEH_OP_MAX_SPEED )
	{
		*(l_pu8Tmp++) = (VEH_OP_DEFINE_SPEED/10) & 0xFF;
	}
	else
	{
		*(l_pu8Tmp++) = ((pdata->u16speed)/10) & 0xFF;
	}
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	for(l_u16Tempi = 0;l_u16Tempi<pdata->u8AixCnt-1;l_u16Tempi++)
	{
		l_s8Acc+=pdata->n8accSpeed[l_u16Tempi];	
	}
	*(l_pu8Tmp++) = l_s8Acc/(pdata->u8AixCnt-1);		 // 加速度
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = 0x00;		 // 违例码	保留
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->u8AixCnt;	 // 轴数
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->u8AixGrpCnt;	 //轴组数
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	if( g_Setup.u8ComMode )
	{
		// 轴重
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
		{ 
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);

		}
	}
	else
	{
		// 轴组重
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
		{	 
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		}
	}

	// 轴组轴型
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
	{
		*(l_pu8Tmp++) = pdata->au8AxleGrpType[l_u16Tempi];
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	}
	// 轴间距
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt-1; l_u16Tempi++ )
	{ 
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])) & 0xFF;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	}
	// 轴速
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
	{
		l_s8Acc = 	pdata->u16AxleList[l_u16Tempi];				
	     *(l_pu8Tmp++) = (g_AxleInfo[l_s8Acc].u16speed/10) & 0xFF;
		 *(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	}  
			
	///////////////////////////////////////
	
	//通道总数
	l_u16LastPackSize = 0;//临时使用
	for( l_u8AxleListIndex=0; l_u8AxleListIndex<pdata->u8AixCnt; l_u8AxleListIndex++ )
	{
		l_u16LastPackSize += g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChListSum;
	}

	if(PROMODPARAM.m_u8DataWaveEn == 1 && PROMODPARAM.m_u8UploadDataMode != 2)
	{
		*(l_pu8Tmp++) = (l_u16LastPackSize>>8) & 0xFF;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		*(l_pu8Tmp++) = (l_u16LastPackSize) & 0xFF;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	}
	else
	{
		*(l_pu8Tmp++) = 0;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		*(l_pu8Tmp++) = 0;
		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	}

	l_u16LastPackSize = 0;
	if(PROMODPARAM.m_u8DataWaveEn == 1 && PROMODPARAM.m_u8UploadDataMode != 2)
	{
		//增益
		for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
		{  
			*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
			*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		}
		//零点
		for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
		{
			*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
			*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])) & 0xFF;
			*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
		}
	}
	///////////////////////////////////////
	// 所有窄条点信息	20140408
	if(PROMODPARAM.m_u8DataWaveEn == 1 && PROMODPARAM.m_u8UploadDataMode != 2)
	{
		for( l_u8AxleListIndex=0; l_u8AxleListIndex<pdata->u8AixCnt; l_u8AxleListIndex++ )
		{
			for( l_u8ZhaitiaoListIndex=0; l_u8ZhaitiaoListIndex<g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChListSum; l_u8ZhaitiaoListIndex++ )
			{
				l_pChlData = &(g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChList[l_u8ZhaitiaoListIndex]]);
				l_pu16Tmp = (*l_pChlData).u16data[0];
				// 增加内容
				*(l_pu8Tmp++) = (*l_pChlData).u8Id;
	
				//
				// test
				if( (*l_pChlData).u8Id >= 64 )
				{
					*(l_pu8Tmp) = (*l_pChlData).u8Id;								
				}
				//
	
				*(l_pu8Tmp++) = (((*l_pChlData).u32uptm)>>24) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32uptm)>>16) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32uptm)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32uptm)) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32dntm)>>24) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32dntm)>>16) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32dntm)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32dntm)) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>24) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>16) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)) & 0xFF;
	
				*(l_pu8Tmp++) = 0;	   //采样标示
	
				*(l_pu8Tmp++) = (((*l_pChlData).u16ptCnt)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u16ptCnt)) & 0xFF;	     			
	
				
				for( l_u16PtsIndex=0; l_u16PtsIndex<(*l_pChlData).u16ptCnt; l_u16PtsIndex++ )
				{
					*(l_pu8Tmp++) = ((*(l_pu16Tmp)) & 0xFF00)>>8;
					*(l_pu8Tmp++) = (*(l_pu16Tmp++)) & 0x00FF;
				}	
			}
	
	 	}
	}				

	// 增加F4代码
//#if  SHOWVEHPASSDEBUG > 0
//	if( g_Setup.u8SendF4CodeEnable )
//	{
//		*(l_pu8Tmp++) = 0xF4; // F4代码标志
//		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
//		*(l_pu8Tmp++) = 0xF4;
//		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
////		*(l_pu8Tmp++) = ((g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0xFF00)>>8;
////		*(l_pu8Tmp++) = (g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0x00FF;
////		memcpy( l_pu8Tmp, g_BugCode[(pdata->u8id) & 0xFF].DbugCode, g_BugCode[(pdata->u8id) & 0xFF].u8NextPos );
////		l_pu8Tmp += g_BugCode[(pdata->u8id) & 0xFF].u8NextPos; 
//	}
//	else
//	{
//		*(l_pu8Tmp++) = 0;// F4代码标志
//		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
////		*(l_pu8Tmp++) = 0;// 长度
////		*(l_pu8Tmp++) = 0;
//	}
//#else	 // 增加没有开启F4代码时的输出，added by wujinlong 20130806
	*(l_pu8Tmp++) = 0;// F4代码标志
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = 0;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
//	*(l_pu8Tmp++) = 0;// 长度
//	*(l_pu8Tmp++) = 0;
//#endif

	
	
	
	*(l_pu8Tmp++) = pdata->m_u16VehLength;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->m_u16VehWide / 10;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->m_u16VehHeight / 10;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->m_u8VehType;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (((pdata->m_u16LoopVal)>>8)&0xFF);
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (((pdata->m_u16LoopVal))&0xFF);
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = pdata->m_u8ProFlag;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32BuildTime>>24)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32BuildTime>>16)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32BuildTime>>8)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32BuildTime)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32EndTime>>24)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32EndTime>>16)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32EndTime>>8)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u32EndTime)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u8PointGet)&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);	
	// 增加预留位置，wujinlong 20130911
	*(l_pu8Tmp++) = (pdata->m_u8LaserPos[0])&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u8LaserPos[1])&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u8LaserPos[2])&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	*(l_pu8Tmp++) = (pdata->m_u8LaserPos[3])&0xFF;
	*(l_pu8SDtemp++) = *(l_pu8Tmp-1);
	AddCrc16( g_u8SDVehBuff[g_u8SDBuffIndex], l_u16SDLen-2);
	///////////////////////////////////
	// 分包保存
															 
	l_u16LastPackSize = l_u32FrameLength % SNDFRAMEBESTSIZE;
	l_u8AllPackNum =  l_u32FrameLength / SNDFRAMEBESTSIZE + (l_u16LastPackSize ? 1:0);
	if( l_u8AllPackNum > 50 )
	{
		l_u8AllPackNum = 50;
	}
	g_sndNetBuf[(pdata->u8id) & 0xFF].u8Cnt = l_u8AllPackNum;

	//逐包添加
	l_pu8Tmp = g_au8AllNetSendBuf;
	for( l_u8PackNO=0; l_u8PackNO<l_u8AllPackNum; l_u8PackNO++ )
	{
		l_pu8SendBufTmp = g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[l_u8PackNO];
		
		// 需增加文件头内容
		*(l_pu8SendBufTmp++) = 0xFF;	 //帧头
		*(l_pu8SendBufTmp++) = 0xFF;
		*(l_pu8SendBufTmp++) = 0x00; //从机地址保留
		if( g_Setup.u8ComMode )	   //命令号
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLE; //命令模式1
		else
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLEGRP; //命令模式0
		*(l_pu8SendBufTmp++)=0;
		// 
		if( l_u8PackNO == l_u8AllPackNum-1 )	//数据长度
		{
			g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u8FrameHeadNum + l_u16LastPackSize;
			*(l_pu8SendBufTmp++) = ((g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO])>>8) & 0xFF;
			*(l_pu8SendBufTmp++) = (g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO]) & 0xFF;
		}
		else
		{
			g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u8FrameHeadNum + SNDFRAMEBESTSIZE + 2; 
			*(l_pu8SendBufTmp++) = ((g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO])>>8) & 0xFF;
			*(l_pu8SendBufTmp++) = (g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO]) & 0xFF;
		}

		*(l_pu8SendBufTmp++) = pdata->u8id; //	 流水号，车号
		//	 
		if( 0 == l_u8PackNO && 1!=l_u8AllPackNum)		 //	 帧标志
			*(l_pu8SendBufTmp++) = 0x00;
		else if( l_u8AllPackNum-1 == l_u8PackNO )
			*(l_pu8SendBufTmp++) = 0x02;
		else
			*(l_pu8SendBufTmp++) = 0x01;

		//
		*(l_pu8SendBufTmp++) = l_u8PackNO; // 帧序号
		*(l_pu8SendBufTmp++) = CHANNELNUM; // 通道总数
		*(l_pu8SendBufTmp++) = 0x00; //	 预留
		*(l_pu8SendBufTmp++) = 0x00; //	 预留

		//
		// 拷贝此包数据
		if( l_u8AllPackNum-1 != l_u8PackNO)
		{
			memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, SNDFRAMEBESTSIZE );
			l_u32tmp  = SNDFRAMEBESTSIZE+13;
		} 		
		else
		{
			memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, l_u16LastPackSize );	
			l_u32tmp = 	 l_u16LastPackSize+13-2;
		}

		//添加CRC
		AddCrc16( g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[l_u8PackNO], l_u32tmp);
	}

	//通信协议发送数据信息
	if( SD_DATA_STORAGE==(PROMODPARAM.m_u16SDSaveEn&SD_DATA_STORAGE) && SD_OFFNET == (PROMODPARAM.m_u16SDSaveEn&SD_OFFNET) )
	{
		// 开启SD，连网发给上位机，断网发给SD卡
		if( g_u8IsConnectFlag )
		{
			OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
		}
		else
		{
			OSQPost(g_pQSDOffNetWrite,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
			OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
		}
	}
	else
	{
		// 没有开启SD卡，直接发送上位机
		OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
	}
	// 保存车辆信息到SD卡
	if( SD_DATA_STORAGE==(PROMODPARAM.m_u16SDSaveEn&SD_DATA_STORAGE) && SD_VEHINFO == (PROMODPARAM.m_u16SDSaveEn&SD_VEHINFO) )
	{
		// 开辟存储空间，512*20的buf，需更改此时变量
		OSQPost(g_pQSDVehWrite,&g_u8SDVehBuff[g_u8SDBuffIndex]);
	}
	g_u8SDBuffIndex++;
	g_u8SDBuffIndex %= 20;
	if(ret == OS_Q_FULL)
	{
		g_ErrMsgState |= 0x04; 		 	
	}
	
	// 记录车辆编号，用于重发使用
//	g_u8CurSendNetBufIndex = pdata->u8id;
}
/*********************************************************************************************************
** Function name:		ToBuff_perWeigh
** Descriptions:		通过过车后得到的数据，根据粗车协议组帧，然后发送.
**                      先把所有数据保存至大buf中，然后根据分包分别保存至每一帧中
** input parameters:	void					 
** output parameters:	void
** Return Value:							
** Created by:			wanglihui		  
** Created Date:		20170731	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Tobuff_preWeigh( Vehicle_Struct *pdata)
{
	//变量定义
	uint8		l_u8PackNO = 0;	// 包数
	uint8		l_u8AllPackNum = 0;//包总数
	//
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据
	
	Channal_Struct *		l_pChlData;	  //指针，指向窄条结构体
	uint8 *		l_pu8SendBufTmp;	   //指针，指向发送数据结构体数组
	//
	//uint8		l_u8FrameVehInfoNum = 100;	   //整车重量等信息字节数，根据最终协议格式修改
	//
	uint32		l_u32FrameLength = 0; // 帧长计数
	uint32		l_u32tmp = 0;
	//
	uint8		l_u8AxleListIndex = 0;// 轴序号
	uint8		l_u8ZhaitiaoListIndex = 0;// 窄条序号
	
	uint16		l_u16Tempi=0;//临时序号
	uint16		l_u16Index1,l_u16Index2;

	//uint8		l_u8PtsOverFlag = 0;//点数超过最大值（10000），跳出

	uint16		l_u16LastPackSize = 0;//最后一帧大小
	int16		l_s8Acc = 0;
	uint8		ret;
	uint16 l_u16VehType = 0;
	uint8  l_u8tempi = 0;
	for( l_u8tempi=0; l_u8tempi<pdata->u8AixGrpCnt; l_u8tempi++ )
	{
		l_u16VehType = l_u16VehType*10 + pdata->au8AxleGrpType[l_u8tempi];
	}

	//
// 	uint16	l_u16SendLen = 0;
// 	uint8	l_u8Err = 0;

	// 增加保护机制，wujinlong
	if( 0 == pdata->u8AixCnt )
	{
		//return;
	}
	
	//将所有数据组成一帧
    l_pu8Tmp = g_au8JDNetSendBuf;

   //先计算帧长，后进行封装帧操作。
	l_u32tmp = 0;
		for(l_u16Index1 = 0;l_u16Index1 < pdata->u8AixCnt;l_u16Index1++)  
		{
			l_u16Tempi = pdata->u16AxleList[l_u16Index1];
			l_u16Index2=g_AxleInfo[l_u16Tempi].RightWheelZTCnt+g_AxleInfo[l_u16Tempi].LeftWheelZTCnt;
			l_u32tmp+=l_u16Index2*11;
			l_u32tmp +=3;
		}
		l_u32tmp =l_u32tmp+13+9;  //校验位+10
	  l_u32FrameLength = l_u32tmp;
		
		if(l_u32FrameLength > SENDALLSIZE)
	{
		g_ErrMsgState |= 0x10000; 	
		return;
	}
	
	*(l_pu8Tmp++) = pdata->m_u32LoopTime & 0xff;   //建车时间
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>8) & 0xff;
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>16) & 0xff;
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>24) & 0xff;
	
	*(l_pu8Tmp++)=pdata->m_Capture_LaneNum;
	
	l_u32tmp = 0;														// 重新计算车速，20131203
	for (l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++)
	{
		l_u32tmp+= g_AxleInfo[pdata->u16AxleList[l_u16Tempi]].u16speed;  				 			
	}
	pdata->u16speed = l_u32tmp/pdata->u8AixCnt;
	if( pdata->u16speed > VEH_OP_MAX_SPEED )
	{	
		*(l_pu8Tmp++) = (VEH_OP_DEFINE_SPEED) & 0xFF;
		*(l_pu8Tmp++) = (VEH_OP_DEFINE_SPEED>>8) & 0xFF;
	}
	else
	{
		*(l_pu8Tmp++) = ((pdata->u16speed)) & 0xFF;
		*(l_pu8Tmp++) = ((pdata->u16speed)>>8) & 0xFF;
	}
	for(l_u16Tempi = 0;l_u16Tempi<pdata->u8AixCnt-1;l_u16Tempi++)
	{
		l_s8Acc+=pdata->n8accSpeed[l_u16Tempi];	
	}
	*(l_pu8Tmp++) = l_s8Acc/(pdata->u8AixCnt-1);		 //加速度
	*(l_pu8Tmp++) = pdata->m_n8Dir;
	*(l_pu8Tmp++) = pdata->u8AixCnt;   //轴数
	
	for( l_u8AxleListIndex=0; l_u8AxleListIndex<pdata->u8AixCnt; l_u8AxleListIndex++ )
		{		
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u8AxleListIndex])) & 0xFF;
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u8AxleListIndex])>>8) & 0xFF;
			*(l_pu8Tmp++) = g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].RightWheelZTCnt+g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].LeftWheelZTCnt;
			
			for( l_u8ZhaitiaoListIndex=0; l_u8ZhaitiaoListIndex<g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].LeftWheelZTCnt; l_u8ZhaitiaoListIndex++ )
			{
				l_pChlData = &(g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16LeftWheeList[l_u8ZhaitiaoListIndex]]);
				*(l_pu8Tmp++) = (*l_pChlData).u8Id;
				*(l_pu8Tmp++) = ((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>16) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>24) & 0xFF;			
				*(l_pu8Tmp++) = ((*l_pChlData).u32realCnt) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32realCnt)>>8) & 0xFF;		
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)) & 0xFF;			
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>8) & 0xFF;	
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>16) & 0xFF;		
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>24) & 0xFF;
			}
			for( l_u8ZhaitiaoListIndex=0; l_u8ZhaitiaoListIndex<g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].RightWheelZTCnt; l_u8ZhaitiaoListIndex++ )
			{
				l_pChlData = &(g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16RightWheeList[l_u8ZhaitiaoListIndex]]);
				*(l_pu8Tmp++) = (*l_pChlData).u8Id;
				*(l_pu8Tmp++) = ((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>8) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>16) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32FirstDnTime-(*l_pChlData).u32realCnt + 1)>>24) & 0xFF;			
				*(l_pu8Tmp++) = ((*l_pChlData).u32realCnt) & 0xFF;
				*(l_pu8Tmp++) = (((*l_pChlData).u32realCnt)>>8) & 0xFF;		
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)) & 0xFF;			
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>8) & 0xFF;	
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>16) & 0xFF;		
				*(l_pu8Tmp++) = (((*l_pChlData).u32chlSum)>>24) & 0xFF;
			}
	
	 	}
		l_u8AllPackNum =0;
		l_u16LastPackSize = l_u32FrameLength % SNDFRAMEBESTSIZE;
		l_u8AllPackNum =  l_u32FrameLength / SNDFRAMEBESTSIZE + (l_u16LastPackSize ? 1:0);
		if( l_u8AllPackNum > 50 )
		{
			l_u8AllPackNum = 50;
		}
		g_cJDSendVehData[(pdata->u8id) & 0xFF].u8Cnt = l_u8AllPackNum;
		l_pu8Tmp = g_au8JDNetSendBuf;
	for( l_u8PackNO=0; l_u8PackNO<l_u8AllPackNum; l_u8PackNO++ )
	{
		l_pu8SendBufTmp = g_cJDSendVehData[(pdata->u8id) & 0xFF].au8data[l_u8PackNO];
		
		// 需增加文件头内容
		*(l_pu8SendBufTmp++) = 0xFF;	 //帧头
		*(l_pu8SendBufTmp++) = 0xFF;
		*(l_pu8SendBufTmp++) = 0x00; //从机地址保留
		if( g_Setup.u8ComMode )	   //命令号
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLE; //命令模式1
			//*(l_pu8SendBufTmp++) = 0x8A; //命令模式1
		else
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLEGRP; //命令模式0
		//*(l_pu8SendBufTmp++)=0;
		// 
		if( l_u8PackNO == l_u8AllPackNum-1 )	//数据长度
		{
			//g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u8FrameHeadNum + l_u16LastPackSize;
			g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u32FrameLength;
			*(l_pu8SendBufTmp++) = (g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO]) & 0xFF;
			*(l_pu8SendBufTmp++) = ((g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO])>>8) & 0xFF;
		}
		else
		{
			//g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u8FrameHeadNum + SNDFRAMEBESTSIZE + 2; 
			g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = SNDFRAMEBESTSIZE + 2;
			*(l_pu8SendBufTmp++) = (g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO]) & 0xFF;
			*(l_pu8SendBufTmp++) = ((g_cJDSendVehData[(pdata->u8id) & 0xFF].au32len[l_u8PackNO])>>8) & 0xFF;
		}
		
		*(l_pu8SendBufTmp++) = l_u8PackNO; // 帧序号
		*(l_pu8SendBufTmp++) = pdata->u8id; //	 流水号，车号
		//	 
// 		if( 0 == l_u8PackNO && 1!=l_u8AllPackNum)		 //	 没有帧标志
// 			*(l_pu8SendBufTmp++) = 0x00;
// 		else if( l_u8AllPackNum-1 == l_u8PackNO )
// 			*(l_pu8SendBufTmp++) = 0x02;
// 		else
// 			*(l_pu8SendBufTmp++) = 0x01;

		*(l_pu8SendBufTmp++) = l_u16VehType & 0xFF;        //	 车型
		*(l_pu8SendBufTmp++) = (l_u16VehType >> 8) & 0xFF; //	 车型

		//
		// 拷贝此包数据
		if( l_u8AllPackNum-1 != l_u8PackNO)
		{
			memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, SNDFRAMEBESTSIZE );
			l_u32tmp  = SNDFRAMEBESTSIZE+10;
		} 		
		else
		{
			memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, l_u16LastPackSize );	
			//memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, l_u32FrameLength );	
			l_u32tmp = 	 l_u16LastPackSize-2;    //源程序还-2 不知道为什么?
		}

		//添加CRC
		AddCrc16( g_cJDSendVehData[(pdata->u8id) & 0xFF].au8data[l_u8PackNO], l_u32tmp);
	}
	OSQPost(g_pQJDSend,&g_cJDSendVehData[(pdata->u8id) & 0xFF]);
	if(ret == OS_Q_FULL)
	{
		g_ErrMsgState |= 0x04; 		 	
	}
}


/*********************************************************************************************************
** Function name:		SendDeviceStatus0
** Descriptions:		非现场执法协议定时10s发送一次设备状态
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121104	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SendDeviceStatus0(void)	   	//每个100ms进入一次函数
{
	if(m_u8TimerNum >= SENDDEVICESTATUSINTERVAL)			//10s发一次
	{
		m_u8TimerNum = 0 ;
		SendCommandToPC0(0,ADJ_DEVICESTATUS);
	}

	m_u8TimerNum++;	
}





/*********************************************************************************************************
** Function name:		FFormHeartFrame
** Descriptions:		心跳
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 FFormHeartFrame(void)	   	
//{
//	uint8	l_u8Tempi = 0;
//	uint8*	l_pu8Tmp;	   //指针，指向所有数据
//	
//	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM; l_u8Tempi++ )
//	{
//	   	g_cHeartBuf.m_u16Len[l_u8Tempi] = 12;
//		l_pu8Tmp = g_cHeartBuf.m_au8DataBuf[l_u8Tempi];
//		*(l_pu8Tmp++) = 0xFF;
//		*(l_pu8Tmp++) = 0xFF;
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 0x86;
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 12;
//		*(l_pu8Tmp++) = l_u8Tempi;
//		memset( l_pu8Tmp, 0, 2 );
//		AddCrc16( g_cHeartBuf.m_au8DataBuf[l_u8Tempi], 10);
//	}
//	
//		
//	return 0;	
//}

/*********************************************************************************************************
** Function name:		FSendHeartbeat
** Descriptions:		心跳
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FSendHeartbeat(void)	   	
{
	uint8	l_u8Tempi = 0;
//	uint32	l_u32Ret = 0;

	//
	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM; l_u8Tempi++ )
	{
		if( g_Setup.m_an8HeartBeatEn[l_u8Tempi] )
		{
			if( g_au16HeartTimeCnt[l_u8Tempi] >= NET_HEART_SENDINTERVAL )			//10s发一次
			{
//				l_u32Ret = SendDataNet( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] ); 
// 				SendDataNet( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] );
				TcpSendData( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] );
				g_au16HeartTimeCnt[l_u8Tempi] = 0;
				g_au16HeartRecvTimeCnt[l_u8Tempi] = 0;
				g_au8HeartRecvReplyFlag[l_u8Tempi] = 0;
				g_au8HeartRecvBeginCntFlag[l_u8Tempi] = 1;
			}
		}
	}

	return 0;	
}

/*********************************************************************************************************
** Function name:		FJudgeUpdate
** Descriptions:		判断和更新
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FJudgeUpdate(void)	   	
{
	uint8	l_u8Tempi = 0;

	//
	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM-1; l_u8Tempi++ )
	{
		if( g_Setup.m_an8HeartBeatEn[l_u8Tempi] )
		{
			// 开启心跳
			// 计时
			g_au16HeartTimeCnt[l_u8Tempi]++;
			if( g_au16HeartTimeCnt[l_u8Tempi] > 1000 )	  // 增加保护
			{
				g_au16HeartTimeCnt[l_u8Tempi] = 1000;
			}

			// 开始计算接收回复时间后，开始计时
			if( g_au8HeartRecvBeginCntFlag[l_u8Tempi] )
			{
				g_au16HeartRecvTimeCnt[l_u8Tempi]++;
				if( g_au16HeartRecvTimeCnt[l_u8Tempi] > 1000 )
				{
					g_au16HeartRecvTimeCnt[l_u8Tempi] = 1000;
				}
			}

			// 接收回复时间计时>设置阈值（5s）
			if( g_au16HeartRecvTimeCnt[l_u8Tempi]>NET_HEART_RECVINTERVAL )
			{				
				g_au8HeartRecvFailCnt[l_u8Tempi]++;
				if( g_au8HeartRecvFailCnt[l_u8Tempi] > 10 )
				{
					g_au8HeartRecvFailCnt[l_u8Tempi] = 10;
				}
				g_au16HeartRecvTimeCnt[l_u8Tempi] = 0;
				g_au8HeartRecvBeginCntFlag[l_u8Tempi] = 0;
			}
				

			
			if( g_au8HeartRecvFailCnt[l_u8Tempi] >= NET_HEART_FAILNUM )
			{
				if(l_u8Tempi < 3)
				{
// 					Socket_Listen(l_u8Tempi);
					listen(l_u8Tempi);
				}
				g_au8HeartRecvFailCnt[l_u8Tempi] = 0;
			}
			
		}
	}
	
	return 0;
}



/*********************************************************************************************************
** Function name:		FFormExtDataFrame
** Descriptions:		组扩展指令帧
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130911	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 FFormExtDataFrame(void)	   	
//{
//	g_au8ExtDataBuf[0] = 0xFF;
//	g_au8ExtDataBuf[1] = 0xFF;
//	g_au8ExtDataBuf[2] = 0;
//	g_au8ExtDataBuf[3] = ADJ_EXT;
//	g_au8ExtDataBuf[4] = 0;
//	g_au8ExtDataBuf[5] = 40;
//	g_au8ExtDataBuf[6] = 0;	 		// 流水号
//	g_au8ExtDataBuf[7] = 0;	 		// 类型
//	memset( &g_au8ExtDataBuf[8], 0, 20 );
//	memset( &g_au8ExtDataBuf[28], 0, 10 );
//	//
//	return SUCCESS;
//}
void ToBuff0ForBD( Vehicle_Struct *pdata )
{
	uint8* l_pu8temp;
	uint8  l_u8tempi,l_u8tempj;
	uint32 l_u32Weight;
	uint8  l_u8speed;
	
	l_u32Weight = 0;
	l_u8speed = 0;
	l_pu8temp = g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[0];
	g_sndNetBuf[(pdata->u8id) & 0xFF].u8Cnt = 1;
	g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[0] = 123*(pdata->u8AixCnt)+17;
	*(l_pu8temp++) = 0xFF;
	*(l_pu8temp++) = 0xFF;
	*(l_pu8temp++) = 0x00;
	*(l_pu8temp++) = 0x8F;
	*(l_pu8temp++) = 0x13;
	*(l_pu8temp++) = ((123*(pdata->u8AixCnt)+17)>>8) & 0xFF;
	*(l_pu8temp++) = (123*(pdata->u8AixCnt)+17) & 0xFF;
	for(l_u8tempi=0; l_u8tempi<pdata->u8AixCnt; l_u8tempi++)
	{
		l_u32Weight += pdata->au16AxleWeight[l_u8tempi];
	}
	*(l_pu8temp++) = (l_u32Weight>>24) & 0xFF;
	*(l_pu8temp++) = (l_u32Weight>>16) & 0xFF;
	*(l_pu8temp++) = (l_u32Weight>>8) & 0xFF;
	*(l_pu8temp++) = (l_u32Weight>>0) & 0xFF;
	*(l_pu8temp++) = pdata->u8AixCnt;
	for(l_u8tempi=0; l_u8tempi<pdata->u8AixCnt; l_u8tempi++)
	{				
		l_u8speed += (g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16speed/10) & 0xFF;
	}
	*(l_pu8temp++) = l_u8speed/pdata->u8AixCnt;
	for(l_u8tempi=0; l_u8tempi<pdata->u8AixCnt; l_u8tempi++)
	{
		*(l_pu8temp++) = l_u8tempi+1;
		*(l_pu8temp++) = (g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16speed >> 8) & 0xFF;
		*(l_pu8temp++) = (g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16speed) & 0xFF;
		for(l_u8tempj=0; l_u8tempj<8; l_u8tempj++)
		{
			if(l_u8tempj < g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChListSum)
			{
				*(l_pu8temp++) = g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u8Id;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32chlSum>>24)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32chlSum>>16)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32chlSum>>8)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32chlSum>>0)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32uptm>>24)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32uptm>>16)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32uptm>>8)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32uptm>>0)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32dntm>>24)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32dntm>>16)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32dntm>>8)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u32dntm>>0)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u16ptCnt>>8)&0xFF;
				*(l_pu8temp++) = (g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8tempi]].u16ChList[l_u8tempj]].u16ptCnt>>0)&0xFF;
			}
			else
			{
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
				*(l_pu8temp++) = 0;
			}
		}
	}
	*(l_pu8temp++) = 0;
	*(l_pu8temp++) = 0;
	AddCrc16( g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[0], g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[0]-2);
	OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
}

