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

#define __PRTCLFXC_C
#include "prtclFXC.h"
#include "common.h"
#include "FPGA_APP.h"



//命令宏定义	COMM
//#define		ADJ_SAVE				0xA1	 	//保存仪表参数指令
//#define		ADJ_GETIPINFO			0xA2		//读取仪表IP相关参数
//#define		ADJ_UPDATEIPINFO		0xA3		//更改仪表IP相关参数
//#define		ADJ_GETNONEWEIGHT		0xA4		//读取所有非重量修正参数
//#define		ADJ_UPDATENONEWEIGHT	0xA5		//更新所有非重量修正参数 
//#define		ADJ_SETHEARTEN			0xA6		// 设置心跳使能
//#define		ADJ_GETVRSION			0xA7		// 获取版本号
//#define		ADJ_GETMOTIONWEIGHT		0xA8		//读取所有动态修正参数
//#define		ADJ_UPDATEMOTIONWEIGHT	0xA9		//更新所有动态修正参数

//#define 	ADJ_SENDRUNTIME 		0xAB		// 各任务计时统计

//
//#define		ADJ_SENDAXLEGRP		0x80		//发送轴组数据
//#define		ADJ_SENDAXLE		0x81		//发送轴数据
//#define		ADJ_TIMESYNCHRONY	0x82		//时间同步
#define		ADJ_INITNOCACHE		0x84		//不保留缓存初始化
//#define		ADJ_DEVICESTATUS	0x85		//设备状态
//#define		ADJ_HEARTBEAT		0x86		//心跳
//#define		ADJ_RESEND			0x8A		//重发
#define		ADJ_RESENDOTH		0x8B		//重发
//#define		ADJ_EXT				0x8F		//扩展指令

#define		ADJ_CAPTURE			0x83		//抓拍
#define		ADJ_NETCONNECT		0x95		//工控机检查线路连接

#define     PROMODPARAM				g_cProModParam


/////////////////////////////////////

#define SENDDEVICESTATUSINTERVAL	200		// 设备状态发送间隔10s，10*20*50ms

static		uint8	m_u8TimerNum = 0;		// 时间计数，状态帧时间计数
static		uint8	m_u8SuccessFlag = 1;//0成功，1失败


static		sndFrame_Struct	m_SendLastContext;	 //当前发送内容，为收到重发指令使用

static		uint8	m_u8ResendFlag = 0;//重发标志，为1，表示重发，0表示不是重发
static		uint8	m_u8ReSendFrameNum = 0xFF;// 重复帧号
static 		uint16 	m_FPGAInitParam[100] = {0};
/*********************************************************************************************************
** Function name:           ProcessCmd
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
void ProcessCmd_FXC(uint8 *pData)
{
	uint8	l_u8VehID = 0;
	uint8*	l_pu8Tmp;
	uint8	l_u8Tempi = 0;//循环变量
//	uint8	l_u8Tempj = 0;
	uint16  l_u16len = 0;
	//
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	//
	uint8	l_u8TempVar = 0;		// 临时变量


//	uint8	l_u8VehSpeedModifyPos = 0;	// 车型速度修正位置，主要区分子命令7、8

	//
	m_u8SuccessFlag = 1;  //初始化为1，表示失败	   
	l_u16len = 	(*(pData+4)<<8) + *(pData+5);
	if( 0 == CheckCrc(pData,l_u16len-2) )	//CRC校验，revised by wujinlong 20121205
	{
		return;
	}
	// 通过帧长判断是否为工控机响应,是响应，处理完毕，返回
	if( 12 == l_u16len )
	{
		if( !(*(pData+7)) )		//为0，成功，否则，重传
		{

		}
		else
		{
			//发送到PC
			//通信协议发送数据信息
			OSQPost(g_pQSendPC,&m_SendLastContext);	
		}
		return;	
	}  
	// 
	// 接收到网口信息
	// added by wujinlong 20130731
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u [TaskRec7] Recv Computer NULL NULL Cmd %x [Net]。\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   *(pData+3));
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Recv Comp Cmd %x [Net]。\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   *(pData+3));
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
		case ADJ_EXT:
			switch( *(pData+7) )
			{
				case 0x01:
				case 0x02:
					break;

				default:
					break;			
			}
			SendCommandToPC_FXC(*(pData+7), ADJ_EXT);
			break;

	   	// 心跳
		case ADJ_HEARTBEAT:
			l_u8TempVar = *(pData+7);
			
			// 接收回复时间计时>设置阈值（5s）
			if( g_au16HeartRecvTimeCnt[l_u8Tempi]>NET_HEART_RECVINTERVAL )
			{				
				g_au8HeartRecvFailCnt[l_u8Tempi]++;
				if( g_au8HeartRecvFailCnt[l_u8Tempi] > 10 )
				{
					g_au8HeartRecvFailCnt[l_u8Tempi] = 10;
				}
			}
			else
			{
				g_au8HeartRecvFailCnt[l_u8Tempi] = 0;		  // 接收回复失败次数 清零
			}
			g_au8HeartRecvBeginCntFlag[l_u8TempVar] = 0;  // 停止 接收回复时间计数 标志			
			g_au16HeartRecvTimeCnt[l_u8TempVar] = 0;	  // 接收回复时间计数 清零
			
			break;
			case JD_Reset: 
				SendCommandToPC0(*(pData + 2),JD_Reset);
			break;

//		case ADJ_SETHEARTEN:
//			g_Setup.m_an8HeartBeatEn[0] = *(pData+7);
//			g_Setup.m_an8HeartBeatEn[1] = *(pData+8);
//			g_Setup.m_an8HeartBeatEn[2] = *(pData+9);
//			g_Setup.m_an8HeartBeatEn[3] = *(pData+10);
//			SaveParams();
//			break;

		case ADJ_GETVRSION:
			SendCommandToPC_FXC(0, ADJ_GETVRSION);
			break;


#if	TASKTIMEENABLE	
		// 任务耗时统计
		case ADJ_SENDRUNTIME:
			switch (*(pData+6)){
				case 0x00:
			   		SendCommandToPC_FXC(0, ADJ_SENDRUNTIME); //命令模式1
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
		    if(0 != CheckCrc(pData, l_u16len - 2))
			{
			    memcpy((uint8*)(&g_LaserPara), pData + 5, sizeof(LaserSetupParam));
				U3SendBytes(pData, l_u16len);

				// 接收到网口信息
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x。\r\n",
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
				// 接收到网口信息
				// added by wujinlong 20130731
//				wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Fail Cmd %x。\n",
//			       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//			       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//				   *(pData+3));
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x。\r\n",
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
		    if(0 != CheckCrc(pData, l_u16len - 2))
			{
		        U3SendBytes(pData, l_u16len);
				// 接收到网口信息
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x。\r\n",
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
				// 接收到网口信息
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Fail Cmd %x。\r\n",
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
//			SendCommandToPC_FXC( 0, SIM_LASER);
//			break;		   
//#endif				
		case ADJ_SENDAXLEGRP:	//轴组重 0x00
		case ADJ_SENDAXLE:  // 轴重 0x01
		   	l_u8VehID = *(pData+6);//流水号位置，工控机主动读车辆信息
			if( g_Setup.u8ComMode )	   //命令号
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLE); //命令模式1
			else
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLEGRP); //命令模式0	   		   		
		   	break;

		case ADJ_TIMESYNCHRONY:  // 时间同步 0x02	
			g_DevCurTime.u16Year	= ((*(pData+7))<<8) + (*(pData+8)); 
			g_DevCurTime.u8Month	= *(pData+9);
			g_DevCurTime.u8Day		= *(pData+10);
			g_DevCurTime.u8Hour	= *(pData+11);
			g_DevCurTime.u8Minute	= *(pData+12);
			g_DevCurTime.u8Second	= *(pData+13);
			
			SaveTime(g_DevCurTime);
			m_u8SuccessFlag = 0;
			SendCommandToPC_FXC( 0, ADJ_TIMESYNCHRONY);
			break;

		case ADJ_RESEND:   //重发 0x0A	
			l_u8VehID = *(pData + 6);	 
			m_u8ReSendFrameNum = *(pData+7); 		
			m_u8ResendFlag = 1;	
			if( g_Setup.u8ComMode )	   //命令号
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLE); //命令模式1
			else
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLEGRP); //命令模式0
			break;
		case ADJ_RESENDOTH:   //重发 0x0A
			SendCommandToPC_FXC( 0, ADJ_RESENDOTH);
			break;

	   	case ADJ_DEVICESTATUS:	 //状态帧，0x05，上位机主动查询
		  	SendCommandToPC_FXC( 0, ADJ_DEVICESTATUS);
			break;

		case ADJ_INITNOCACHE:	 //不保留缓存初始化，0x54
		  	YBRestart();
			break;

		case ADJ_CAPTURE:	 //抓拍，0x13，当触发相机抓拍时给上位机发送该命令，通知触发抓拍信号
		   	SendCommandToPC_FXC( 0, ADJ_CAPTURE);
		   	//SendCommandToPC_FXC( g_u8NetADOut, ADJ_CAPTURE);
		   	break;

		/////////////////////////////////////////////
		case ADJ_GETIPINFO:	 //读取仪表IP相关参数,0x20
		   	m_u8SuccessFlag = 0;
		   	SendCommandToPC_FXC( 0, ADJ_GETIPINFO);
		   	break;

		case ADJ_UPDATEIPINFO:	 //更改仪表IP相关参数,0x21
		   	memcpy( &g_IPSetLocal.au8IPAddr[0], pData+7, 4 );
		   	memcpy( &g_IPSetLocal.au8SubMask[0], pData+11, 4 );
		   	memcpy( &g_IPSetLocal.au8GatewayIP[0], pData+15, 4 );
		   	//memcpy( &g_IPSetLocal.u32LocalPortNO, (uint32*)(pData+19), 1 );
		   	g_IPSetLocal.u32LocalPortNO = ((*(pData+19))<<24) + ((*(pData+20))<<16) + ((*(pData+21))<<8)+(*(pData+22));
		   	memcpy( &g_IPSetLocal.au8MACAddr[0], pData+23, 6 );
		   	m_u8SuccessFlag = 0;
//		   	SendCommandToPC_FXC( 0, ADJ_UPDATEIPINFO);	
//		   	YBRestart();	
		   	SaveNetInfo(8);  
		   	//OSTimeDly(50); 	
		   	OSTimeDly(500*OS_TICKS_PER_SEC/1000);
// 		   	InitializeW5100(&g_IPSetLocal);
				InitializeW5300(&g_IPSetLocal);
		   	break;
		
		case ADJ_NETCONNECT:	 //工控机检查线路连接
			m_u8SuccessFlag = 0;
			SendCommandToPC_FXC( 0, ADJ_NETCONNECT);
#if SD_OFFNET_SAVE_EN > 0
			g_u32NetConCnt = 0;
			g_u8IsConnectFlag = 1;
#endif
		   	break;


		///////////////////////////////////
		// 调校相关命令
//		case ADJ_SAVE:	 //保存仪表参数指令,0xA1
//		   SaveParams();
//		   m_u8SuccessFlag = 0;
//		   SendCommandToPC_FXC( 0, ADJ_SAVE);
//		   break;

		case ADJ_GETNONEWEIGHT:	 //读取所有非重量修正参数,0xA4
		   SendCommandToPC_FXC( 0, ADJ_GETNONEWEIGHT);
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
			SaveParams(14);
			m_u8SuccessFlag = 0;
			SendCommandToPC_FXC( 0, ADJ_UPDATENONEWEIGHT);
		   	break;

		case ADJ_GETMOTIONWEIGHT:	 //读取所有动态修正参数,0xA8
		   SendCommandToPC_FXC( *(pData+6), ADJ_GETMOTIONWEIGHT);
		   break;

		case ADJ_UPDATEMOTIONWEIGHT:	 //更新所有动态修正参数,0xA9
		   	l_pu8Tmp = pData + 7;
			
			switch( *(pData+6) )
			{
				case 1:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an32AxGain[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						l_pu8Tmp += 2;	  //如果用++，防止不同编译器，效果不同
					}
					m_FPGAInitParam[0] = g_Setup.u16UpValue;
					m_FPGAInitParam[1] = g_Setup.u16DownValue;
					m_FPGAInitParam[2] = g_Setup.u16logicUpValue;
					m_FPGAInitParam[3] = g_Setup.u16logicDownValue;
					m_FPGAInitParam[4] = g_Setup.u8UPPoints;
					m_FPGAInitParam[5] = g_Setup.u8DownPoints;
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32AxGain[l_u8Tempi];	
					}
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32Zero[l_u8Tempi];	
					}
					//FPGAInitParam(m_FPGAInitParam,CHANNELNUM);
					FPGAInitParam();
					break;

				case 2:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an32Zero[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						g_an32MotionZero[l_u8Tempi] = g_Setup.an32Zero[l_u8Tempi];
						l_pu8Tmp += 2;
					}
					m_FPGAInitParam[0] = g_Setup.u16UpValue;
					m_FPGAInitParam[1] = g_Setup.u16DownValue;
					m_FPGAInitParam[2] = g_Setup.u16logicUpValue;
					m_FPGAInitParam[3] = g_Setup.u16logicDownValue;
					m_FPGAInitParam[4] = g_Setup.u8UPPoints;
					m_FPGAInitParam[5] = g_Setup.u8DownPoints;
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32AxGain[l_u8Tempi];	
					}
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32Zero[l_u8Tempi];	
					}
					//FPGAInitParam(m_FPGAInitParam,CHANNELNUM);
					FPGAInitParam();
					break;

				case 3:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an8ZTStatus[l_u8Tempi] = *(l_pu8Tmp++);
					}
					OSSemPost(g_pSemTask6Param);
					break;

				case 4:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an8LengthPodu[l_u8Tempi] = *(l_pu8Tmp++);
					}
					OSSemPost(g_pSemTask6Param);
					break;

				case 5:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an8HeightTanxingti[l_u8Tempi] = *(l_pu8Tmp++);
					}
					OSSemPost(g_pSemTask6Param);
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
						g_Setup.u16UseToWghFlag[l_u8Tempi] = (*(l_pu8Tmp+2)) + 256*(*(l_pu8Tmp+1));  //双向车道第一方向各排0-31
						l_pu8Tmp = l_pu8Tmp + 2;
					}
//					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ ) //20130523 浩叔发现bug
//					{
//						g_Setup.u16JiaodianState[l_u8Tempi] = g_Setup.u16JiaodianState[l_u8Tempi]| (*(l_pu8Tmp++))<<8;//双向车道第二方向各排32-64
//					}
					// 增加窄条间距（1*3）,更改变量
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM - 1; l_u8Tempi++ )	//20130523 浩叔发现bug
					{
						g_Setup.u16DistBetween[l_u8Tempi] = (*(l_pu8Tmp++)<<8);
						g_Setup.u16DistBetween[l_u8Tempi] = g_Setup.u16DistBetween[l_u8Tempi] | (*(l_pu8Tmp++));
					}
					OSSemPost(g_pSemTask6Param);
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32AxGain[l_u8Tempi];	
					}
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						m_FPGAInitParam[6+l_u8Tempi] = g_Setup.an32Zero[l_u8Tempi];	
					}
					//FPGAInitParam(m_FPGAInitParam,CHANNELNUM);
					FPGAInitParam();
					break;

				case 7:
				case 8:
					// 20131121
//					if( 7 == (*(pData+6)) )
//					{
//						l_u8VehSpeedModifyPos = 0;
//					}
//					else if( 8 == (*(pData+6)) )
//					{
//						l_u8VehSpeedModifyPos = 5;
//					}
//					else
//					{
//						break;
//					}
//					for( l_u8Tempi=0; l_u8Tempi<5; l_u8Tempi++ )
//					{
//						for( l_u8Tempj=0; l_u8Tempj<80; l_u8Tempj++ )
//						{
//							g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
//							if( g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] < PARA_VEHTPYE_SPEED_MODIFY_MIN )
//							{
//								g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] = PARA_VEHTPYE_SPEED_MODIFY_MIN;
//							}
//							else if( g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] > PARA_VEHTPYE_SPEED_MODIFY_MAX )
//							{
//								g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] = PARA_VEHTPYE_SPEED_MODIFY_MAX;
//							}
//							else
//							{
//							}
//							l_pu8Tmp += 2;
//						}
//					}
					break;
				
				default:
					break;

			}
			SaveParams(15);
			m_u8SuccessFlag = 0;
			SendCommandToPC_FXC( *(pData+6), ADJ_UPDATEMOTIONWEIGHT);
		   	break;

		default:
			break; 
	}	
}

/*********************************************************************************************************
** Function name:		SendCommandToPC_FXC
** Descriptions:		根据协议不同的命令字，发送数据
** input parameters:	uint8	p_u8VehIndex			//buffer数据的序号
						uint8	p_u8CommandNO			//命令字					 
** output parameters:	void
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121105	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SendCommandToPC_FXC(uint8 p_u8VehIndex,uint8 p_u8CommandNO)
{
	//uint8	l_au8TmpData[256];
	uint8	l_u8Tempi = 0;
//	uint8	l_u8Tempj = 0;
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据

// 	uint8	l_u8Err;
// 	uint16	l_u16SendLen;

	uint16	l_u16ZTCurValue = 0;	// 窄条当前瞬时值
	
//	uint8	l_u8VehSpeedModifyPos = 0;
	uint32 tempver;
	switch( p_u8CommandNO )
	{
		case ADJ_EXT:
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 40;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_EXT;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 40;
			*(l_pu8Tmp++) = 0x00;  			// 流水号
			switch( p_u8VehIndex )
			{
				case 0x01:
				case 0x02:
					*(l_pu8Tmp++) = p_u8VehIndex;  	// 类型
					memset( l_pu8Tmp, 0, 20);	 	// 数据
					l_pu8Tmp += 20;
					memset( l_pu8Tmp, 0, 10);	 	// 预留
					break;

				default:
					break;			
			}
			
			break;
	
	
		case ADJ_GETVRSION:
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 75;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETVRSION;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 75;
			*(l_pu8Tmp++) = 0x00;  //流水号
			memcpy( l_pu8Tmp, PROGRAMVERSION, 12);
			l_pu8Tmp+= 12;
			memcpy( l_pu8Tmp, PROGMODIFYDATE, 8);
			l_pu8Tmp+= 8;
			
			tempver=GET_MASTER_FPGA_VERSION();
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
			}
			memset( l_pu8Tmp, 0, 10);
			break;
					
#if	TASKTIMEENABLE
		case ADJ_SENDRUNTIME:
		   	l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 11+sizeof(TASKRUNTIME);
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_SENDRUNTIME;
			*(l_pu8Tmp++) = (11+sizeof(TASKRUNTIME))>>8;
			*(l_pu8Tmp++) = 0xFF&(11+sizeof(TASKRUNTIME));
			*(l_pu8Tmp++) = 0x00;  //流水号
			memcpy(l_pu8Tmp,&TaskRunTimeStatistics,sizeof(TASKRUNTIME));
			l_pu8Tmp+= sizeof(TASKRUNTIME);				
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
				}
				else
				{
					m_SendLastContext.u8Cnt = 1;
					m_SendLastContext.au32len[0] = g_sndNetBuf[g_vehicleInfo[p_u8VehIndex].u8id].au32len[m_u8ReSendFrameNum];
					memcpy( m_SendLastContext.au8data[0], g_sndNetBuf[g_vehicleInfo[p_u8VehIndex].u8id].au8data[0], m_SendLastContext.au32len[0] );
					//
					OSQPost(g_pQSendPC,&m_SendLastContext);	
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
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;  //流水号
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;

		//case ADJ_RESEND:   //重发 0x0A	
		//	break;
		case ADJ_RESENDOTH:   //重发 0x0A
			OSQPost(g_pQSendPC,&m_SendLastContext);;
			break;

	   	case ADJ_DEVICESTATUS:	 //状态帧，0x05，上位机主动查询
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 20;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_DEVICESTATUS;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 20;
			*(l_pu8Tmp++) = 0x00;  //保留
			
			for( l_u8Tempi=0; l_u8Tempi<ERR_LEN; l_u8Tempi++ )
			{
				*(l_pu8Tmp++) = g_u8DeviceERR[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0x00;  //保留
			*(l_pu8Tmp++) = 0x00;  //保留
			break;

		//case ADJ_INITNOCACHE:	 //不保留缓存初始化，0x54
		//	break;

		case ADJ_CAPTURE:	 //抓拍，0x13，当触发相机抓拍时给上位机发送该命令，通知触发抓拍信号
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 15;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x93;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 15;
			//*(l_pu8Tmp++) = 0x00;  //流水号  
			*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][6];  //流水号 车辆编号
			//*(l_pu8Tmp++) = g_u8LasterStateAll;  //保留 
//			if(g_cProModParam.m_u8TriggerMode == 1 || g_cProModParam.m_u8TriggerMode == 3) 
//			{
//				*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][7];  //保留  车道号
//			}
//			else
//			{
				*(l_pu8Tmp++) = (1<<p_u8VehIndex);
//			}
			*(l_pu8Tmp++) = g_au32LoopCaTime[p_u8VehIndex] & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>8) & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>16) & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>24) & 0xff;
			*(l_pu8Tmp++) = 0x00;  //保留
			break;
		
		/////////////////////////////////////////////////////
		case ADJ_GETIPINFO:	 //读取仪表IP相关参数,0x20
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 36;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 36;
			*(l_pu8Tmp++) = 0x00;  //流水号
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
			memset(&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][30], 0, 4 );
			break;

		case ADJ_UPDATEIPINFO:	 //更改仪表IP相关参数,0x21
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 14;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_UPDATEIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 14;
			*(l_pu8Tmp++) = 0x00;  //流水号
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			memset(l_pu8Tmp, 0, 4 );
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
//		case ADJ_SAVE:	 //保存仪表参数指令,0xA1
//		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
//			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
//			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
//			*(l_pu8Tmp++) = 0xff;
//			*(l_pu8Tmp++) = 0xff;
//			*(l_pu8Tmp++) = 0x00;
//			*(l_pu8Tmp++) = ADJ_SAVE;
//			*(l_pu8Tmp++) = 0;
//			*(l_pu8Tmp++) = 12;
//			*(l_pu8Tmp++) = 0x00;  //流水号
//			*(l_pu8Tmp++) = m_u8SuccessFlag;
//			*(l_pu8Tmp++) = 0x00;  //保留
//			*(l_pu8Tmp++) = 0x00;  //保留
//			
//			break;

		case ADJ_GETNONEWEIGHT:	 //读取所有非重量修正参数,0xA4
		case ADJ_UPDATENONEWEIGHT:	 //更新所有非重量修正参数,0xA5
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = p_u8CommandNO;
			*(l_pu8Tmp++) = 0;	   // 数据长度 2字节
			if( p_u8CommandNO == ADJ_GETNONEWEIGHT )
			{
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 33;
				*(l_pu8Tmp++) = 33;
			}
			else
			{
				g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 26;
				*(l_pu8Tmp++) = 26;	
			}		
			*(l_pu8Tmp++) = 0x00;  //流水号
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
			if( p_u8CommandNO == ADJ_GETNONEWEIGHT )
			{
				memset(l_pu8Tmp, 0, 10 );
			}
			else
			{
				*(l_pu8Tmp++) = m_u8SuccessFlag;
				memset(l_pu8Tmp, 0, 2 );
			}
						
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
			
			if( p_u8CommandNO == ADJ_GETMOTIONWEIGHT )
			{
				if( 14 == p_u8VehIndex )
				{					
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 275;
					*(l_pu8Tmp++) = (275>>8) & 0xFF;
					*(l_pu8Tmp++) = 275 & 0xFF;
				}
				else if( 7==p_u8VehIndex || 8==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 819;
					*(l_pu8Tmp++) = (819>>8) & 0xFF;
					*(l_pu8Tmp++) = 819 & 0xFF;
				}
				else
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 147;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 147;
				}
			}
			else
			{
				if( 7==p_u8VehIndex || 8==p_u8VehIndex )
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 812;
					*(l_pu8Tmp++) = (812>>8) & 0xFF;
					*(l_pu8Tmp++) = 812 & 0xFF;
				}
				else
				{
					g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 140;
					*(l_pu8Tmp++) = 0;
					*(l_pu8Tmp++) = 140;
				}
			}
			*(l_pu8Tmp++) = p_u8VehIndex;  //流水号			

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
						*(l_pu8Tmp++) = g_Setup.an8ZTStatus[l_u8Tempi];
						
					} 
					memset(l_pu8Tmp, 0, 64 );
					l_pu8Tmp += 64;
					break;

				case 4:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = g_Setup.an8LengthPodu[l_u8Tempi];
						
					}	
					memset(l_pu8Tmp, 0, 64 );
					l_pu8Tmp += 64;
					break;

				case 5:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = g_Setup.an8HeightTanxingti[l_u8Tempi];
						
					}	 
					memset(l_pu8Tmp, 0, 64 );
					l_pu8Tmp += 64;
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
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM-1; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = (g_Setup.u16DistBetween[l_u8Tempi]>>8)&0xff;//20130515 浩叔发现bug
						*(l_pu8Tmp++) = g_Setup.u16DistBetween[l_u8Tempi]&0xff;
					}
					memset(l_pu8Tmp, 0, 10 );	   // 此大小计算得出128-（16*6+16+6）//20130515 浩叔发现bug计算不对！！
					l_pu8Tmp += 10;
					break;

				case 7:
				case 8:
					// 20131121
//					if( 7 == p_u8VehIndex )
//					{
//						l_u8VehSpeedModifyPos = 0;
//					}
//					else if( 8 == p_u8VehIndex )
//					{
//						l_u8VehSpeedModifyPos = 5;
//					}
//					else
//					{
//						break;
//					}
//					for( l_u8Tempi=0; l_u8Tempi<5; l_u8Tempi++ )
//					{
//						for( l_u8Tempj=0; l_u8Tempj<80; l_u8Tempj++ )
//						{
//							*(l_pu8Tmp++) = (g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj]>>8) & 0xFF;
//							*(l_pu8Tmp++) = g_Setup.au16VehSpeedModify[l_u8VehSpeedModifyPos+l_u8Tempi][l_u8Tempj] & 0xFF;
//						}
//					}
					break;

				case 14:
					DisableFPGA_INT(); // 禁止FPGA中断，再开启
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ ) 
					{
						//*(l_pu8Tmp++) = ((GETCURRENTDATA((l_u8Tempi>>3)&0x07,l_u8Tempi&0x07))>>8) & 0xFF;
						//*(l_pu8Tmp++) = ((GETCURRENTDATA((l_u8Tempi>>3)&0x07,l_u8Tempi&0x07))) & 0xFF;
						if( 255!=aFPGALogicNUM2LaneNUM[l_u8Tempi] && 255!=aFPGALogicNUM2ChlNUM[l_u8Tempi] )
						{
							l_u16ZTCurValue = GETCURRENTDATA( aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi] );
							*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
						}
						else
						{
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 0;
						}
						
					}
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ ) 
					{
						if( 255!=aFPGALogicNUM2LaneNUM[l_u8Tempi] && 255!=aFPGALogicNUM2ChlNUM[l_u8Tempi] )
						{
							l_u16ZTCurValue = GETCURRENTZERO( aFPGALogicNUM2LaneNUM[l_u8Tempi], aFPGALogicNUM2ChlNUM[l_u8Tempi] );
							*(l_pu8Tmp++) = (l_u16ZTCurValue>>8) & 0xFF;
							*(l_pu8Tmp++) = (l_u16ZTCurValue) & 0xFF;
						}
						else
						{
							*(l_pu8Tmp++) = 0;
							*(l_pu8Tmp++) = 0;
						}
						
					}
					EnableFPGA_INT();
					break;

				default:
					break;
			}
			if( p_u8CommandNO == ADJ_GETMOTIONWEIGHT )
			{
				memset(l_pu8Tmp, 0, 10 );
			}
			else
			{
				*(l_pu8Tmp++) = m_u8SuccessFlag;
				memset(l_pu8Tmp, 0, 2 );
			}
			break;

		default:
			break; 
	}
	
	//发送到PC
	AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
	//通信协议发送数据信息
	
	if( ADJ_EXT == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3] )
	{
		if( 0x01 == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][7]
			|| 0x02 == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][7] )
		{
			OSQPost(g_pQNetWavePC,&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
		}
		else if( 0x03 == g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][7] )
		{
			OSQPost(g_pQNetWavePC,&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
		}
		else
		{}
		
	}
	else
	{
		OSQPost(g_pQSendPC,&g_sndNetOrderBuf[g_u8CurSendNetOrderIndex]);
	}
	

	// 回复上位机指令
	// added by wujinlong 20130805
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp FormFrame&Reply Cmd=%x [Net]。\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3]);
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
	m_u8SuccessFlag = 1;// 发送完毕，置1，表示失败
	g_u8CurSendNetOrderIndex++;
	g_u8CurSendNetOrderIndex %= TEMP_SIZE;//发送完毕自增
	l_pu8Tmp = NULL;

}

/*********************************************************************************************************
** Function name:		ToBuff_FXC
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
void ToBuff_FXC( Vehicle_Struct *pdata )
{
	//变量定义
	uint8		l_u8PackNO = 0;	// 包数
	uint8		l_u8AllPackNum = 0;//包总数
	//
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据
	uint16 *	l_pu16Tmp;	   //指针，指向窄条点数据
	Channal_Struct *		l_pChlData;	  //指针，指向窄条结构体
	uint8 *		l_pu8SendBufTmp;	   //指针，指向发送数据结构体数组
	//
	uint8		l_u8FrameHeadNum = 12;	   //帧头字节数，根据最终协议格式修改
	//uint8		l_u8FrameVehInfoNum = 100;	   //整车重量等信息字节数，根据最终协议格式修改
	//
	uint16		l_u32FrameLength = 0; // 帧长计数
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
	}


//	if( g_Setup.u8ComMode )
//	{	//帧长 = 年+月...（17）+轴重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+ F4代码+预留(10)+crc(2) ;
//		l_u16Index1 = 0;
//#if  SHOWVEHPASSDEBUG > 0 //显示过车调试代码 
//		 l_u16Index1 = 4 + g_BugCode[pdata->u8id].u8NextPos;
//#endif
//		l_u32FrameLength = 17 + (pdata->u8AixCnt<<1) + pdata->u8AixGrpCnt + ((pdata->u8AixCnt -1)<<1) + pdata->u8AixCnt +2+(CHANNELNUM<<2)+ l_u32tmp + l_u16Index1 + 12;
//	}
//	else
//	{
//		//帧长 = 年+月...（17）+轴组重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+ 预留(10)+crc(2) ; 
//#if  SHOWVEHPASSDEBUG > 0 //显示过车调试代码 
//		l_u16Index1 = 4 + g_BugCode[pdata->u8id].u8NextPos;
//#endif
//		l_u32FrameLength = 17 + (pdata->u8AixGrpCnt<<1) + pdata->u8AixGrpCnt +((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt + 2+(CHANNELNUM<<2) + l_u32tmp + l_u16Index1 + 12;
//	}

	// 增加F4代码g_Setup.u8SendF4CodeEnable的控制，计算分两步：（1）去除F4代码、CRC的长度，（2）增加F4代码长度
	// added by wujinlong 20130806
   	if( g_Setup.u8ComMode )
	{	//帧长 = 年+月...（17）+轴重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+预留(10) ;
		l_u32FrameLength = 17 + (pdata->u8AixCnt<<1) + pdata->u8AixGrpCnt + ((pdata->u8AixCnt -1)<<1) + pdata->u8AixCnt +2+(CHANNELNUM<<2)+ l_u32tmp + 10 + 2;
	}
	else
	{
		//帧长 = 年+月...（17）+轴组重+轴组型+轴距+轴速+通道数+增益+零点+<各通道数据内容长度> ...+ 预留(10) ; 
		l_u32FrameLength = 17 + (pdata->u8AixGrpCnt<<1) + pdata->u8AixGrpCnt +((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt + 2+(CHANNELNUM<<2) + l_u32tmp + 10 + 2;
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
	l_u16Index1 = 4;
//#endif
	l_u32FrameLength += l_u16Index1;
	
	if(l_u32FrameLength > SENDALLSIZE)
	{
//		OSQPost(g_pQErrMsg,ErrAxleMsg);
		g_ErrMsgState |= 0x80; 	
		return;
	}
	///////////////////////////////////
	// 增加轴重等信息  根据命令号，添加轴重还是轴组重
//	*(uint16*)(l_pu8Tmp++) = g_DevCurTime.u16Year;	  // 时间
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year>>8) & 0xFF;
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year) & 0xFF; 
	*(l_pu8Tmp++) = g_DevCurTime.u8Month;
	*(l_pu8Tmp++) = g_DevCurTime.u8Day;
	*(l_pu8Tmp++) = g_DevCurTime.u8Hour;
	*(l_pu8Tmp++) = g_DevCurTime.u8Minute;
	*(l_pu8Tmp++) = g_DevCurTime.u8Second;
	 // 温度
	*(l_pu8Tmp++) = g_n32Temperature & 0xFF;		
	*(l_pu8Tmp++) = pdata->u8PostVehFlag;			// 出车情况,0~60:正常出车,>60:不出车
//	if(g_cProModParam.m_u8TriggerMode == 1|| g_cProModParam.m_u8TriggerMode == 3)
//	{
//		*(l_pu8Tmp++) = pdata->u8psLane;   	// 车道编号
//	}
//	else
//	{
//		*(l_pu8Tmp++) = pdata->u8psLane/2;
		//*(l_pu8Tmp++) = PROMODPARAM.m_au8CHLSET[pdata->m_u8CPID];
		*(l_pu8Tmp++) = pdata->m_Capture_LaneNum;
//	}
	//*(l_pu8Tmp++) = 1;
	*(l_pu8Tmp++) = pdata->u8lvl;		 // 可信度：100-0,算法给出
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
		*(l_pu8Tmp++) = (VEH_OP_DEFINE_SPEED>>8) & 0xFF;
		*(l_pu8Tmp++) = (VEH_OP_DEFINE_SPEED) & 0xFF;
	}
	else
	{
		*(l_pu8Tmp++) = ((pdata->u16speed)>>8) & 0xFF;
		*(l_pu8Tmp++) = ((pdata->u16speed)) & 0xFF;
	}
	for(l_u16Tempi = 0;l_u16Tempi<pdata->u8AixCnt-1;l_u16Tempi++)
	{
		l_s8Acc+=pdata->n8accSpeed[l_u16Tempi];	
	}
	*(l_pu8Tmp++) = l_s8Acc/(pdata->u8AixCnt-1);		 // 加速度
	if((pdata->u8AixCnt == 2) && (pdata->u8ViolationCode == 0x04))   // 2轴车并且减速
	{
		*(l_pu8Tmp++) = 0x20;		         // 违例码,点刹
	}
	else
	{
		*(l_pu8Tmp++) = pdata->u8ViolationCode;	   // 违例码
	}
	*(l_pu8Tmp++) = pdata->u8AixCnt;	 // 轴数
	*(l_pu8Tmp++) = pdata->u8AixGrpCnt;	 //轴组数
	if( g_Setup.u8ComMode )
	{
		// 轴重
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
		{ 
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])) & 0xFF;

		}
	}
	else
	{
		// 轴组重
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
		{	 
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])) & 0xFF;
		}
	}

	// 轴组轴型
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
	{
		*(l_pu8Tmp++) = pdata->au8AxleGrpType[l_u16Tempi];
	}
	// 轴间距
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt-1; l_u16Tempi++ )
	{ 
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])) & 0xFF;
	}
	// 轴速
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
	{
		l_s8Acc = 	pdata->u16AxleList[l_u16Tempi];				
	     *(l_pu8Tmp++) = (g_AxleInfo[l_s8Acc].u16speed/10) & 0xFF;
	}  
			
	///////////////////////////////////////
	
	//通道总数
	l_u16LastPackSize = 0;//临时使用
	for( l_u8AxleListIndex=0; l_u8AxleListIndex<pdata->u8AixCnt; l_u8AxleListIndex++ )
	{
		l_u16LastPackSize += g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChListSum;
	}
	if(PROMODPARAM.m_u8DataWaveEn == 0)
	{
		*(l_pu8Tmp++) = 0;
		*(l_pu8Tmp++) = 0;
	}
	else
	{
		*(l_pu8Tmp++) = (l_u16LastPackSize>>8) & 0xFF;
		*(l_pu8Tmp++) = (l_u16LastPackSize) & 0xFF;
	}

	l_u16LastPackSize = 0;
	//增益
	for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
	{  
		*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])) & 0xFF;
	}
	//零点
	for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
	{
		*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])) & 0xFF;
	}


	///////////////////////////////////////
	// 所有窄条点信息
	if(PROMODPARAM.m_u8DataWaveEn == 1)
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
//		*(l_pu8Tmp++) = 0xF4;
//		*(l_pu8Tmp++) = ((g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0xFF00)>>8;
//		*(l_pu8Tmp++) = (g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0x00FF;
//		memcpy( l_pu8Tmp, g_BugCode[(pdata->u8id) & 0xFF].DbugCode, g_BugCode[(pdata->u8id) & 0xFF].u8NextPos );
//		l_pu8Tmp += g_BugCode[(pdata->u8id) & 0xFF].u8NextPos; 
//	}
//	else
//	{
//		*(l_pu8Tmp++) = 0;// F4代码标志
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 0;// 长度
//		*(l_pu8Tmp++) = 0;
//	}
//#else	 // 增加没有开启F4代码时的输出，added by wujinlong 20130806
	*(l_pu8Tmp++) = 0;// F4代码标志
	*(l_pu8Tmp++) = 0;
	*(l_pu8Tmp++) = 0;// 长度
	*(l_pu8Tmp++) = 0;
//#endif

	// 增加预留位置，wujinlong 20130911
	memset( l_pu8Tmp, 0, 12 );
	*(l_pu8Tmp++) = pdata->m_u16VehLength;
	*(l_pu8Tmp++) = pdata->m_u16VehWide / 10;
	*(l_pu8Tmp++) = pdata->m_u16VehHeight / 10;
	*(l_pu8Tmp++) = pdata->m_u8VehType;
	*(l_pu8Tmp++) = pdata->m_u8LaserPos[0];
	*(l_pu8Tmp++) = pdata->m_u8LaserPos[1];
	*(l_pu8Tmp++) = pdata->m_u8LaserPos[2];
	*(l_pu8Tmp++) = pdata->m_u8LaserPos[3];
	*(l_pu8Tmp++) = pdata->m_u32LoopTime & 0xff;
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>8) & 0xff;
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>16) & 0xff;
	*(l_pu8Tmp++) = (pdata->m_u32LoopTime>>24) & 0xff;
	if(pdata->u8psLane == 2)
	{
		pdata->u8psLane = 2;
	}

	///////////////////////////////////
	// 分包保存
															 
	l_u16LastPackSize = l_u32FrameLength % SNDFRAMEBESTSIZE;
	l_u8AllPackNum =  l_u32FrameLength / SNDFRAMEBESTSIZE + (l_u16LastPackSize ? 1:0);
	if(l_u8AllPackNum >= 30)
	{
		l_u8AllPackNum = 29;
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
		// 
		if( l_u8PackNO == l_u8AllPackNum-1 )	//数据长度
		{
			g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[l_u8PackNO] = l_u8FrameHeadNum + l_u16LastPackSize + 2;
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
			l_u32tmp  = SNDFRAMEBESTSIZE+12;
		} 		
		else
		{
			memcpy( l_pu8SendBufTmp, l_pu8Tmp+SNDFRAMEBESTSIZE*l_u8PackNO, l_u16LastPackSize );	
			l_u32tmp = 	 l_u16LastPackSize+12;
		}

		//添加CRC
		AddCrc16( g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[l_u8PackNO], l_u32tmp);
	}

	//通信协议发送数据信息
#if SD_DATA_STORAGE_EN>0 && SD_OFFNET_SAVE_EN>0
//	if( g_u8IsConnectFlag )
	{
		OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
	}
//	else
	{
		OSQPost(g_pQSDOffNetWrite,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
	}
#else
	OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);
#endif
	if(ret == OS_Q_FULL)
	{
		//车辆数据发送失败
//		OSQPost(g_pQErrMsg,(void *)ErrSndPCMsg);
		g_ErrMsgState |= 0x04; 		 	
	}
	
	// 记录车辆编号，用于重发使用
//	g_u8CurSendNetBufIndex = pdata->u8id;
}



/*********************************************************************************************************
** Function name:		SendDeviceStatus_FXC
** Descriptions:		非现场执法协议定时10s发送一次设备状态
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121104	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SendDeviceStatus_FXC(void)	   	//每个100ms进入一次函数
{
	if(m_u8TimerNum >= SENDDEVICESTATUSINTERVAL)			//10s发一次
	{
		m_u8TimerNum = 0 ;
		SendCommandToPC_FXC(0,ADJ_DEVICESTATUS);
	}

	m_u8TimerNum++;	
}





/*********************************************************************************************************
** Function name:		FFormHeartFrame_FXC
** Descriptions:		心跳
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 FFormHeartFrame_FXC(void)	   	
//{
//	uint8	l_u8Tempi = 0;
//	uint8*	l_pu8Tmp;	   //指针，指向所有数据
//	
//	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM; l_u8Tempi++ )
//	{
//	   	g_cHeartBuf.m_u16Len[l_u8Tempi] = 20;
//		l_pu8Tmp = g_cHeartBuf.m_au8DataBuf[l_u8Tempi];
//		*(l_pu8Tmp++) = 0xFF;
//		*(l_pu8Tmp++) = 0xFF;
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 0x86;
//		*(l_pu8Tmp++) = 20;
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = l_u8Tempi;
//		memset( l_pu8Tmp, 0, 10 );
//		AddCrc16( g_cHeartBuf.m_au8DataBuf[l_u8Tempi], 18);
//	}
//	
//		
//	return 0;	
//}

/*********************************************************************************************************
** Function name:		FSendHeartbeat_FXC
** Descriptions:		心跳
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FSendHeartbeat_FXC(void)	   	
{
	uint8	l_u8Tempi = 0;
	uint32	l_u32Ret = 0;

	//
	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM; l_u8Tempi++ )
	{
		if( g_Setup.m_an8HeartBeatEn[l_u8Tempi] )
		{
			if( g_au16HeartTimeCnt[l_u8Tempi] >= NET_HEART_SENDINTERVAL )			//10s发一次
			{
// 				l_u32Ret = SendDataNet( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] ); 
				l_u32Ret = TcpSendData( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] ); 
				if( 0 == l_u32Ret )
				{
					// 异常
				}
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
** Function name:		FJudgeUpdate_FXC
** Descriptions:		判断和更新
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130904	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FJudgeUpdate_FXC(void)	   	
{
	uint8	l_u8Tempi = 0;

	//
	for( l_u8Tempi=0; l_u8Tempi<NET_PORT_NUM; l_u8Tempi++ )
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
// 				Socket_Listen(l_u8Tempi);
				listen(l_u8Tempi);
				g_au8HeartRecvFailCnt[l_u8Tempi] = 0;
			}
			
		}
	}
	
	return 0;
}



/*********************************************************************************************************
** Function name:		FFormExtDataFrame_FXC
** Descriptions:		组扩展指令帧
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130911	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 FFormExtDataFrame_FXC(void)	   	
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


