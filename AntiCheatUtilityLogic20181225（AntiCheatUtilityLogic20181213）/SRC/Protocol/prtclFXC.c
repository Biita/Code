/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			prtclWJ.c
** Last modified Date:  20121031
** Last Version:		1.0
** Descriptions:		���ֳ�ִ��Э��
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



//����궨��	COMM
//#define		ADJ_SAVE				0xA1	 	//�����Ǳ����ָ��
//#define		ADJ_GETIPINFO			0xA2		//��ȡ�Ǳ�IP��ز���
//#define		ADJ_UPDATEIPINFO		0xA3		//�����Ǳ�IP��ز���
//#define		ADJ_GETNONEWEIGHT		0xA4		//��ȡ���з�������������
//#define		ADJ_UPDATENONEWEIGHT	0xA5		//�������з������������� 
//#define		ADJ_SETHEARTEN			0xA6		// ��������ʹ��
//#define		ADJ_GETVRSION			0xA7		// ��ȡ�汾��
//#define		ADJ_GETMOTIONWEIGHT		0xA8		//��ȡ���ж�̬��������
//#define		ADJ_UPDATEMOTIONWEIGHT	0xA9		//�������ж�̬��������

//#define 	ADJ_SENDRUNTIME 		0xAB		// �������ʱͳ��

//
//#define		ADJ_SENDAXLEGRP		0x80		//������������
//#define		ADJ_SENDAXLE		0x81		//����������
//#define		ADJ_TIMESYNCHRONY	0x82		//ʱ��ͬ��
#define		ADJ_INITNOCACHE		0x84		//�����������ʼ��
//#define		ADJ_DEVICESTATUS	0x85		//�豸״̬
//#define		ADJ_HEARTBEAT		0x86		//����
//#define		ADJ_RESEND			0x8A		//�ط�
#define		ADJ_RESENDOTH		0x8B		//�ط�
//#define		ADJ_EXT				0x8F		//��չָ��

#define		ADJ_CAPTURE			0x83		//ץ��
#define		ADJ_NETCONNECT		0x95		//���ػ������·����

#define     PROMODPARAM				g_cProModParam


/////////////////////////////////////

#define SENDDEVICESTATUSINTERVAL	200		// �豸״̬���ͼ��10s��10*20*50ms

static		uint8	m_u8TimerNum = 0;		// ʱ�������״̬֡ʱ�����
static		uint8	m_u8SuccessFlag = 1;//0�ɹ���1ʧ��


static		sndFrame_Struct	m_SendLastContext;	 //��ǰ�������ݣ�Ϊ�յ��ط�ָ��ʹ��

static		uint8	m_u8ResendFlag = 0;//�ط���־��Ϊ1����ʾ�ط���0��ʾ�����ط�
static		uint8	m_u8ReSendFrameNum = 0xFF;// �ظ�֡��
static 		uint16 	m_FPGAInitParam[100] = {0};
/*********************************************************************************************************
** Function name:           ProcessCmd
**
** Descriptions:            ������λ�����ִ�в���
**
** input parameters:        uint8 *	pdata: ������������
**							uint16 	len:   �������ݳ���
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
	uint8	l_u8Tempi = 0;//ѭ������
//	uint8	l_u8Tempj = 0;
	uint16  l_u16len = 0;
	//
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	//
	uint8	l_u8TempVar = 0;		// ��ʱ����


//	uint8	l_u8VehSpeedModifyPos = 0;	// �����ٶ�����λ�ã���Ҫ����������7��8

	//
	m_u8SuccessFlag = 1;  //��ʼ��Ϊ1����ʾʧ��	   
	l_u16len = 	(*(pData+4)<<8) + *(pData+5);
	if( 0 == CheckCrc(pData,l_u16len-2) )	//CRCУ�飬revised by wujinlong 20121205
	{
		return;
	}
	// ͨ��֡���ж��Ƿ�Ϊ���ػ���Ӧ,����Ӧ��������ϣ�����
	if( 12 == l_u16len )
	{
		if( !(*(pData+7)) )		//Ϊ0���ɹ��������ش�
		{

		}
		else
		{
			//���͵�PC
			//ͨ��Э�鷢��������Ϣ
			OSQPost(g_pQSendPC,&m_SendLastContext);	
		}
		return;	
	}  
	// 
	// ���յ�������Ϣ
	// added by wujinlong 20130731
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u [TaskRec7] Recv Computer NULL NULL Cmd %x [Net]��\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   *(pData+3));
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Recv Comp Cmd %x [Net]��\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   *(pData+3));
#if DEBUGINFO_TASK_SP_EN > 0
// 	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Recv Comp Cmd %x [Net]��\r\n",
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

	   	// ����
		case ADJ_HEARTBEAT:
			l_u8TempVar = *(pData+7);
			
			// ���ջظ�ʱ���ʱ>������ֵ��5s��
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
				g_au8HeartRecvFailCnt[l_u8Tempi] = 0;		  // ���ջظ�ʧ�ܴ��� ����
			}
			g_au8HeartRecvBeginCntFlag[l_u8TempVar] = 0;  // ֹͣ ���ջظ�ʱ����� ��־			
			g_au16HeartRecvTimeCnt[l_u8TempVar] = 0;	  // ���ջظ�ʱ����� ����
			
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
		// �����ʱͳ��
		case ADJ_SENDRUNTIME:
			switch (*(pData+6)){
				case 0x00:
			   		SendCommandToPC_FXC(0, ADJ_SENDRUNTIME); //����ģʽ1
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

		case LASER_SET_PARA_RQ://���ü���ģ�����
		    if(0 != CheckCrc(pData, l_u16len - 2))
			{
			    memcpy((uint8*)(&g_LaserPara), pData + 5, sizeof(LaserSetupParam));
				U3SendBytes(pData, l_u16len);

				// ���յ�������Ϣ
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x��\r\n",
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
			else//��������У��ʧ��
			{
				// ���յ�������Ϣ
				// added by wujinlong 20130731
//				wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Fail Cmd %x��\n",
//			       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//			       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//				   *(pData+3));
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x��\r\n",
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

		case LASER_READ_PARA_RQ://��ȡ����ģ�����
		case LASER_RESET_RQ://��λ����ģ��
		    if(0 != CheckCrc(pData, l_u16len - 2))
			{
		        U3SendBytes(pData, l_u16len);
				// ���յ�������Ϣ
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Succ Cmd %x��\r\n",
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
			else//��������У��ʧ��
			{
				// ���յ�������Ϣ
#if DEBUGINFO_TASK_SP_EN > 0
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Laser CRC Fail Cmd %x��\r\n",
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
		case ADJ_SENDAXLEGRP:	//������ 0x00
		case ADJ_SENDAXLE:  // ���� 0x01
		   	l_u8VehID = *(pData+6);//��ˮ��λ�ã����ػ�������������Ϣ
			if( g_Setup.u8ComMode )	   //�����
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLE); //����ģʽ1
			else
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLEGRP); //����ģʽ0	   		   		
		   	break;

		case ADJ_TIMESYNCHRONY:  // ʱ��ͬ�� 0x02	
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

		case ADJ_RESEND:   //�ط� 0x0A	
			l_u8VehID = *(pData + 6);	 
			m_u8ReSendFrameNum = *(pData+7); 		
			m_u8ResendFlag = 1;	
			if( g_Setup.u8ComMode )	   //�����
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLE); //����ģʽ1
			else
				SendCommandToPC_FXC( l_u8VehID, ADJ_SENDAXLEGRP); //����ģʽ0
			break;
		case ADJ_RESENDOTH:   //�ط� 0x0A
			SendCommandToPC_FXC( 0, ADJ_RESENDOTH);
			break;

	   	case ADJ_DEVICESTATUS:	 //״̬֡��0x05����λ��������ѯ
		  	SendCommandToPC_FXC( 0, ADJ_DEVICESTATUS);
			break;

		case ADJ_INITNOCACHE:	 //�����������ʼ����0x54
		  	YBRestart();
			break;

		case ADJ_CAPTURE:	 //ץ�ģ�0x13�����������ץ��ʱ����λ�����͸����֪ͨ����ץ���ź�
		   	SendCommandToPC_FXC( 0, ADJ_CAPTURE);
		   	//SendCommandToPC_FXC( g_u8NetADOut, ADJ_CAPTURE);
		   	break;

		/////////////////////////////////////////////
		case ADJ_GETIPINFO:	 //��ȡ�Ǳ�IP��ز���,0x20
		   	m_u8SuccessFlag = 0;
		   	SendCommandToPC_FXC( 0, ADJ_GETIPINFO);
		   	break;

		case ADJ_UPDATEIPINFO:	 //�����Ǳ�IP��ز���,0x21
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
		
		case ADJ_NETCONNECT:	 //���ػ������·����
			m_u8SuccessFlag = 0;
			SendCommandToPC_FXC( 0, ADJ_NETCONNECT);
#if SD_OFFNET_SAVE_EN > 0
			g_u32NetConCnt = 0;
			g_u8IsConnectFlag = 1;
#endif
		   	break;


		///////////////////////////////////
		// ��У�������
//		case ADJ_SAVE:	 //�����Ǳ����ָ��,0xA1
//		   SaveParams();
//		   m_u8SuccessFlag = 0;
//		   SendCommandToPC_FXC( 0, ADJ_SAVE);
//		   break;

		case ADJ_GETNONEWEIGHT:	 //��ȡ���з�������������,0xA4
		   SendCommandToPC_FXC( 0, ADJ_GETNONEWEIGHT);
		   break;

		case ADJ_UPDATENONEWEIGHT:	 //�������з�������������,0xA5
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

		case ADJ_GETMOTIONWEIGHT:	 //��ȡ���ж�̬��������,0xA8
		   SendCommandToPC_FXC( *(pData+6), ADJ_GETMOTIONWEIGHT);
		   break;

		case ADJ_UPDATEMOTIONWEIGHT:	 //�������ж�̬��������,0xA9
		   	l_pu8Tmp = pData + 7;
			
			switch( *(pData+6) )
			{
				case 1:
					for( l_u8Tempi=0; l_u8Tempi<CHANNELNUM; l_u8Tempi++ )
					{
						g_Setup.an32AxGain[l_u8Tempi] = ((*(l_pu8Tmp))<<8) + (*(l_pu8Tmp+1));
						l_pu8Tmp += 2;	  //�����++����ֹ��ͬ��������Ч����ͬ
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

					// ���ӽ���״̬��1*16��,���ı���
					//���ֽ��ǵ�һ���򣬸��ֽ��ǵڶ�����
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ ) //20130523 ���巢��bug
					{
						g_Setup.u16UseToWghFlag[l_u8Tempi] = (*(l_pu8Tmp+2)) + 256*(*(l_pu8Tmp+1));  //˫�򳵵���һ�������0-31
						l_pu8Tmp = l_pu8Tmp + 2;
					}
//					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ ) //20130523 ���巢��bug
//					{
//						g_Setup.u16JiaodianState[l_u8Tempi] = g_Setup.u16JiaodianState[l_u8Tempi]| (*(l_pu8Tmp++))<<8;//˫�򳵵��ڶ��������32-64
//					}
					// ����խ����ࣨ1*3��,���ı���
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM - 1; l_u8Tempi++ )	//20130523 ���巢��bug
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
** Descriptions:		����Э�鲻ͬ�������֣���������
** input parameters:	uint8	p_u8VehIndex			//buffer���ݵ����
						uint8	p_u8CommandNO			//������					 
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
	uint8 *		l_pu8Tmp;	   //ָ�룬ָ����������

// 	uint8	l_u8Err;
// 	uint16	l_u16SendLen;

	uint16	l_u16ZTCurValue = 0;	// խ����ǰ˲ʱֵ
	
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
			*(l_pu8Tmp++) = 0x00;  			// ��ˮ��
			switch( p_u8VehIndex )
			{
				case 0x01:
				case 0x02:
					*(l_pu8Tmp++) = p_u8VehIndex;  	// ����
					memset( l_pu8Tmp, 0, 20);	 	// ����
					l_pu8Tmp += 20;
					memset( l_pu8Tmp, 0, 10);	 	// Ԥ��
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
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
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
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
			memcpy(l_pu8Tmp,&TaskRunTimeStatistics,sizeof(TASKRUNTIME));
			l_pu8Tmp+= sizeof(TASKRUNTIME);				
			*(l_pu8Tmp++) = 0x00;  //����
			*(l_pu8Tmp++) = 0x00;  //����
			break;
#endif

		case ADJ_SENDAXLEGRP:	//������ 0x00
		case ADJ_SENDAXLE:  // ���� 0x01
			//ͨ��Э�鷢��������Ϣ
			if( m_u8ResendFlag )
			{
				//�ط�ָ��
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

		case ADJ_TIMESYNCHRONY:  // ʱ��ͬ�� 0x02
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_TIMESYNCHRONY;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //����
			*(l_pu8Tmp++) = 0x00;  //����
			break;

		//case ADJ_RESEND:   //�ط� 0x0A	
		//	break;
		case ADJ_RESENDOTH:   //�ط� 0x0A
			OSQPost(g_pQSendPC,&m_SendLastContext);;
			break;

	   	case ADJ_DEVICESTATUS:	 //״̬֡��0x05����λ��������ѯ
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 20;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_DEVICESTATUS;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 20;
			*(l_pu8Tmp++) = 0x00;  //����
			
			for( l_u8Tempi=0; l_u8Tempi<ERR_LEN; l_u8Tempi++ )
			{
				*(l_pu8Tmp++) = g_u8DeviceERR[l_u8Tempi];
			}
			*(l_pu8Tmp++) = 0x00;  //����
			*(l_pu8Tmp++) = 0x00;  //����
			break;

		//case ADJ_INITNOCACHE:	 //�����������ʼ����0x54
		//	break;

		case ADJ_CAPTURE:	 //ץ�ģ�0x13�����������ץ��ʱ����λ�����͸����֪ͨ����ץ���ź�
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 15;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 0x93;
			*(l_pu8Tmp++) = 0;
			*(l_pu8Tmp++) = 15;
			//*(l_pu8Tmp++) = 0x00;  //��ˮ��  
			*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][6];  //��ˮ�� �������
			//*(l_pu8Tmp++) = g_u8LasterStateAll;  //���� 
//			if(g_cProModParam.m_u8TriggerMode == 1 || g_cProModParam.m_u8TriggerMode == 3) 
//			{
//				*(l_pu8Tmp++) = g_anfSPIAD[g_u8NetADOut][7];  //����  ������
//			}
//			else
//			{
				*(l_pu8Tmp++) = (1<<p_u8VehIndex);
//			}
			*(l_pu8Tmp++) = g_au32LoopCaTime[p_u8VehIndex] & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>8) & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>16) & 0xff;
			*(l_pu8Tmp++) = (g_au32LoopCaTime[p_u8VehIndex]>>24) & 0xff;
			*(l_pu8Tmp++) = 0x00;  //����
			break;
		
		/////////////////////////////////////////////////////
		case ADJ_GETIPINFO:	 //��ȡ�Ǳ�IP��ز���,0x20
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 36;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_GETIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 36;
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
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

		case ADJ_UPDATEIPINFO:	 //�����Ǳ�IP��ز���,0x21
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 14;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_UPDATEIPINFO;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 14;
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			memset(l_pu8Tmp, 0, 4 );
			break;

		case ADJ_NETCONNECT:	 //���ػ������·����
		    g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = ADJ_NETCONNECT;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = 12;
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
			*(l_pu8Tmp++) = m_u8SuccessFlag;
			*(l_pu8Tmp++) = 0x00;  //����
			*(l_pu8Tmp++) = 0x00;  //����
		    break;

		///////////////////////////////////
		// ��У�������
//		case ADJ_SAVE:	 //�����Ǳ����ָ��,0xA1
//		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;
//			g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0] = 12;
//			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
//			*(l_pu8Tmp++) = 0xff;
//			*(l_pu8Tmp++) = 0xff;
//			*(l_pu8Tmp++) = 0x00;
//			*(l_pu8Tmp++) = ADJ_SAVE;
//			*(l_pu8Tmp++) = 0;
//			*(l_pu8Tmp++) = 12;
//			*(l_pu8Tmp++) = 0x00;  //��ˮ��
//			*(l_pu8Tmp++) = m_u8SuccessFlag;
//			*(l_pu8Tmp++) = 0x00;  //����
//			*(l_pu8Tmp++) = 0x00;  //����
//			
//			break;

		case ADJ_GETNONEWEIGHT:	 //��ȡ���з�������������,0xA4
		case ADJ_UPDATENONEWEIGHT:	 //�������з�������������,0xA5
		   	g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].u8Cnt = 1;				
			l_pu8Tmp = g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0];
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0xff;
			*(l_pu8Tmp++) = 0x00;
			*(l_pu8Tmp++) = p_u8CommandNO;
			*(l_pu8Tmp++) = 0;	   // ���ݳ��� 2�ֽ�
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
			*(l_pu8Tmp++) = 0x00;  //��ˮ��
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

		case ADJ_GETMOTIONWEIGHT:	 //��ȡ��̬��������,0xA8
		case ADJ_UPDATEMOTIONWEIGHT:	 //���¶�̬��������,0xA9
		   	//��ʾ���ͣ�1�����棨4*64��
			//2����㣨4*64��
			//3��·��ֵ��1*64��
			//4�����³��ȣ�1*64��
			//5��������߶ȣ�1*64��
			//6���ϳ���ֵ���³���ֵ���ϳ��߼���ֵ���³��߼���ֵ���ϳӵ������³ӵ�����16*6��������״̬��1*8����խ����ࣨ1*3��
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
			*(l_pu8Tmp++) = p_u8VehIndex;  //��ˮ��			

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

					// ���ӽ���״̬��1*8��,���ı���
					// ���ӽ���״̬��1*16��,���ı���
					//���ֽ��ǵ�һ���򣬸��ֽ��ǵڶ�����
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //˫�򳵵���һ�������0-31
					{
						*(l_pu8Tmp++) = 0xFF&g_Setup.u16UseToWghFlag[l_u8Tempi];//20130515 ���巢��bug	   						
						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16UseToWghFlag[l_u8Tempi]>>8);
					}
//					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM; l_u8Tempi++ )  //˫�򳵵��ڶ��������32-64
//					{ 						
//						*(l_pu8Tmp++) = 0xFF&(g_Setup.u16JiaodianState[l_u8Tempi]>>8);//20130515 ���巢��bug 						
//					}
					// ����խ����ࣨ2*3��,���ı���
					for( l_u8Tempi=0; l_u8Tempi<CHANNALLINESUM-1; l_u8Tempi++ )
					{
						*(l_pu8Tmp++) = (g_Setup.u16DistBetween[l_u8Tempi]>>8)&0xff;//20130515 ���巢��bug
						*(l_pu8Tmp++) = g_Setup.u16DistBetween[l_u8Tempi]&0xff;
					}
					memset(l_pu8Tmp, 0, 10 );	   // �˴�С����ó�128-��16*6+16+6��//20130515 ���巢��bug���㲻�ԣ���
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
					DisableFPGA_INT(); // ��ֹFPGA�жϣ��ٿ���
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
	
	//���͵�PC
	AddCrc16(g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0],g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au32len[0]-2);
	//ͨ��Э�鷢��������Ϣ
	
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
	

	// �ظ���λ��ָ��
	// added by wujinlong 20130805
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp FormFrame&Reply Cmd=%x [Net]��\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//	   g_sndNetOrderBuf[g_u8CurSendNetOrderIndex].au8data[0][3]);
#if DEBUGINFO_TASK_SP_EN > 0
// 	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T7 Send Comp FormFrame&Reply Cmd=%x [Net]��\r\n",
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
	m_u8SuccessFlag = 1;// ������ϣ���1����ʾʧ��
	g_u8CurSendNetOrderIndex++;
	g_u8CurSendNetOrderIndex %= TEMP_SIZE;//�����������
	l_pu8Tmp = NULL;

}

/*********************************************************************************************************
** Function name:		ToBuff_FXC
** Descriptions:		ͨ��������õ������ݣ�����Э����֡��Ȼ����.
**                      �Ȱ��������ݱ�������buf�У�Ȼ����ݷְ��ֱ𱣴���ÿһ֡��
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
	//��������
	uint8		l_u8PackNO = 0;	// ����
	uint8		l_u8AllPackNum = 0;//������
	//
	uint8 *		l_pu8Tmp;	   //ָ�룬ָ����������
	uint16 *	l_pu16Tmp;	   //ָ�룬ָ��խ��������
	Channal_Struct *		l_pChlData;	  //ָ�룬ָ��խ���ṹ��
	uint8 *		l_pu8SendBufTmp;	   //ָ�룬ָ�������ݽṹ������
	//
	uint8		l_u8FrameHeadNum = 12;	   //֡ͷ�ֽ�������������Э���ʽ�޸�
	//uint8		l_u8FrameVehInfoNum = 100;	   //������������Ϣ�ֽ�������������Э���ʽ�޸�
	//
	uint16		l_u32FrameLength = 0; // ֡������
	uint32		l_u32tmp = 0;
	//
	uint8		l_u8AxleListIndex = 0;// �����
	uint8		l_u8ZhaitiaoListIndex = 0;// խ�����
	uint16		l_u16PtsIndex = 0;//�������
	uint16		l_u16Tempi=0,l_u16Tempi2;//��ʱ���
	uint16		l_u16Index1,l_u16Index2;

	//uint8		l_u8PtsOverFlag = 0;//�����������ֵ��10000��������

	uint16		l_u16LastPackSize = 0;//���һ֡��С
	int16		l_s8Acc = 0;
	uint8		ret;

	//
// 	uint16	l_u16SendLen = 0;
// 	uint8	l_u8Err = 0;

	// ���ӱ������ƣ�wujinlong
	if( 0 == pdata->u8AixCnt )
	{
		//return;
	}
	//
	
	//�������������һ֡
    l_pu8Tmp = g_au8AllNetSendBuf;

   //�ȼ���֡��������з�װ֡������
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
//	{	//֡�� = ��+��...��17��+����+������+���+����+ͨ����+����+���+<��ͨ���������ݳ���> ...+ F4����+Ԥ��(10)+crc(2) ;
//		l_u16Index1 = 0;
//#if  SHOWVEHPASSDEBUG > 0 //��ʾ�������Դ��� 
//		 l_u16Index1 = 4 + g_BugCode[pdata->u8id].u8NextPos;
//#endif
//		l_u32FrameLength = 17 + (pdata->u8AixCnt<<1) + pdata->u8AixGrpCnt + ((pdata->u8AixCnt -1)<<1) + pdata->u8AixCnt +2+(CHANNELNUM<<2)+ l_u32tmp + l_u16Index1 + 12;
//	}
//	else
//	{
//		//֡�� = ��+��...��17��+������+������+���+����+ͨ����+����+���+<��ͨ���������ݳ���> ...+ Ԥ��(10)+crc(2) ; 
//#if  SHOWVEHPASSDEBUG > 0 //��ʾ�������Դ��� 
//		l_u16Index1 = 4 + g_BugCode[pdata->u8id].u8NextPos;
//#endif
//		l_u32FrameLength = 17 + (pdata->u8AixGrpCnt<<1) + pdata->u8AixGrpCnt +((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt + 2+(CHANNELNUM<<2) + l_u32tmp + l_u16Index1 + 12;
//	}

	// ����F4����g_Setup.u8SendF4CodeEnable�Ŀ��ƣ��������������1��ȥ��F4���롢CRC�ĳ��ȣ���2������F4���볤��
	// added by wujinlong 20130806
   	if( g_Setup.u8ComMode )
	{	//֡�� = ��+��...��17��+����+������+���+����+ͨ����+����+���+<��ͨ���������ݳ���> ...+Ԥ��(10) ;
		l_u32FrameLength = 17 + (pdata->u8AixCnt<<1) + pdata->u8AixGrpCnt + ((pdata->u8AixCnt -1)<<1) + pdata->u8AixCnt +2+(CHANNELNUM<<2)+ l_u32tmp + 10 + 2;
	}
	else
	{
		//֡�� = ��+��...��17��+������+������+���+����+ͨ����+����+���+<��ͨ���������ݳ���> ...+ Ԥ��(10) ; 
		l_u32FrameLength = 17 + (pdata->u8AixGrpCnt<<1) + pdata->u8AixGrpCnt +((pdata->u8AixCnt -1)<<1)+pdata->u8AixCnt + 2+(CHANNELNUM<<2) + l_u32tmp + 10 + 2;
	}

//#if  SHOWVEHPASSDEBUG > 0 //��ʾ�������Դ��� 
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
	// �������ص���Ϣ  ��������ţ�������ػ���������
//	*(uint16*)(l_pu8Tmp++) = g_DevCurTime.u16Year;	  // ʱ��
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year>>8) & 0xFF;
	*(l_pu8Tmp++) = (g_DevCurTime.u16Year) & 0xFF; 
	*(l_pu8Tmp++) = g_DevCurTime.u8Month;
	*(l_pu8Tmp++) = g_DevCurTime.u8Day;
	*(l_pu8Tmp++) = g_DevCurTime.u8Hour;
	*(l_pu8Tmp++) = g_DevCurTime.u8Minute;
	*(l_pu8Tmp++) = g_DevCurTime.u8Second;
	 // �¶�
	*(l_pu8Tmp++) = g_n32Temperature & 0xFF;		
	*(l_pu8Tmp++) = pdata->u8PostVehFlag;			// �������,0~60:��������,>60:������
//	if(g_cProModParam.m_u8TriggerMode == 1|| g_cProModParam.m_u8TriggerMode == 3)
//	{
//		*(l_pu8Tmp++) = pdata->u8psLane;   	// �������
//	}
//	else
//	{
//		*(l_pu8Tmp++) = pdata->u8psLane/2;
		//*(l_pu8Tmp++) = PROMODPARAM.m_au8CHLSET[pdata->m_u8CPID];
		*(l_pu8Tmp++) = pdata->m_Capture_LaneNum;
//	}
	//*(l_pu8Tmp++) = 1;
	*(l_pu8Tmp++) = pdata->u8lvl;		 // ���Ŷȣ�100-0,�㷨����
	//
	//*(uint16*)(l_pu8Tmp++) = pdata->u16speed;		 // �ٶ�
	l_u32tmp = 0;														// ���¼��㳵�٣�20131203
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
	*(l_pu8Tmp++) = l_s8Acc/(pdata->u8AixCnt-1);		 // ���ٶ�
	if((pdata->u8AixCnt == 2) && (pdata->u8ViolationCode == 0x04))   // 2�ᳵ���Ҽ���
	{
		*(l_pu8Tmp++) = 0x20;		         // Υ����,��ɲ
	}
	else
	{
		*(l_pu8Tmp++) = pdata->u8ViolationCode;	   // Υ����
	}
	*(l_pu8Tmp++) = pdata->u8AixCnt;	 // ����
	*(l_pu8Tmp++) = pdata->u8AixGrpCnt;	 //������
	if( g_Setup.u8ComMode )
	{
		// ����
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
		{ 
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8Tmp++) = ((pdata->au16AxleWeight[l_u16Tempi])) & 0xFF;

		}
	}
	else
	{
		// ������
		for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
		{	 
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])>>8) & 0xFF;
			*(l_pu8Tmp++) = ((pdata->au16AxleGrpWeight[l_u16Tempi])) & 0xFF;
		}
	}

	// ��������
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixGrpCnt; l_u16Tempi++ )
	{
		*(l_pu8Tmp++) = pdata->au8AxleGrpType[l_u16Tempi];
	}
	// ����
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt-1; l_u16Tempi++ )
	{ 
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((pdata->au16AxleDistance[l_u16Tempi])) & 0xFF;
	}
	// ����
	for( l_u16Tempi=0; l_u16Tempi<pdata->u8AixCnt; l_u16Tempi++ )
	{
		l_s8Acc = 	pdata->u16AxleList[l_u16Tempi];				
	     *(l_pu8Tmp++) = (g_AxleInfo[l_s8Acc].u16speed/10) & 0xFF;
	}  
			
	///////////////////////////////////////
	
	//ͨ������
	l_u16LastPackSize = 0;//��ʱʹ��
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
	//����
	for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
	{  
		*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((g_Setup.an32AxGain[l_u16Tempi])) & 0xFF;
	}
	//���
	for( l_u16Tempi=0; l_u16Tempi<CHANNELNUM; l_u16Tempi++ )
	{
		*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])>>8) & 0xFF;
		*(l_pu8Tmp++) = ((g_Setup.an32Zero[l_u16Tempi])) & 0xFF;
	}


	///////////////////////////////////////
	// ����խ������Ϣ
	if(PROMODPARAM.m_u8DataWaveEn == 1)
	{
		for( l_u8AxleListIndex=0; l_u8AxleListIndex<pdata->u8AixCnt; l_u8AxleListIndex++ )
		{
			for( l_u8ZhaitiaoListIndex=0; l_u8ZhaitiaoListIndex<g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChListSum; l_u8ZhaitiaoListIndex++ )
			{
				l_pChlData = &(g_ChlData[g_AxleInfo[pdata->u16AxleList[l_u8AxleListIndex]].u16ChList[l_u8ZhaitiaoListIndex]]);
				l_pu16Tmp = (*l_pChlData).u16data[0];
				// ��������
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
	
				*(l_pu8Tmp++) = 0;	   //������ʾ
	
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

	// ����F4����
//#if  SHOWVEHPASSDEBUG > 0
//	if( g_Setup.u8SendF4CodeEnable )
//	{
//		*(l_pu8Tmp++) = 0xF4; // F4�����־
//		*(l_pu8Tmp++) = 0xF4;
//		*(l_pu8Tmp++) = ((g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0xFF00)>>8;
//		*(l_pu8Tmp++) = (g_BugCode[(pdata->u8id) & 0xFF].u8NextPos) & 0x00FF;
//		memcpy( l_pu8Tmp, g_BugCode[(pdata->u8id) & 0xFF].DbugCode, g_BugCode[(pdata->u8id) & 0xFF].u8NextPos );
//		l_pu8Tmp += g_BugCode[(pdata->u8id) & 0xFF].u8NextPos; 
//	}
//	else
//	{
//		*(l_pu8Tmp++) = 0;// F4�����־
//		*(l_pu8Tmp++) = 0;
//		*(l_pu8Tmp++) = 0;// ����
//		*(l_pu8Tmp++) = 0;
//	}
//#else	 // ����û�п���F4����ʱ�������added by wujinlong 20130806
	*(l_pu8Tmp++) = 0;// F4�����־
	*(l_pu8Tmp++) = 0;
	*(l_pu8Tmp++) = 0;// ����
	*(l_pu8Tmp++) = 0;
//#endif

	// ����Ԥ��λ�ã�wujinlong 20130911
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
	// �ְ�����
															 
	l_u16LastPackSize = l_u32FrameLength % SNDFRAMEBESTSIZE;
	l_u8AllPackNum =  l_u32FrameLength / SNDFRAMEBESTSIZE + (l_u16LastPackSize ? 1:0);
	if(l_u8AllPackNum >= 30)
	{
		l_u8AllPackNum = 29;
	}
	g_sndNetBuf[(pdata->u8id) & 0xFF].u8Cnt = l_u8AllPackNum;

	//������
	l_pu8Tmp = g_au8AllNetSendBuf;
	for( l_u8PackNO=0; l_u8PackNO<l_u8AllPackNum; l_u8PackNO++ )
	{
		l_pu8SendBufTmp = g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[l_u8PackNO];
		
		// �������ļ�ͷ����
		*(l_pu8SendBufTmp++) = 0xFF;	 //֡ͷ
		*(l_pu8SendBufTmp++) = 0xFF;
		*(l_pu8SendBufTmp++) = 0x00; //�ӻ���ַ����
		if( g_Setup.u8ComMode )	   //�����
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLE; //����ģʽ1
		else
			*(l_pu8SendBufTmp++) = ADJ_SENDAXLEGRP; //����ģʽ0
		// 
		if( l_u8PackNO == l_u8AllPackNum-1 )	//���ݳ���
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

		*(l_pu8SendBufTmp++) = pdata->u8id; //	 ��ˮ�ţ�����
		//	 
		if( 0 == l_u8PackNO && 1!=l_u8AllPackNum)		 //	 ֡��־
			*(l_pu8SendBufTmp++) = 0x00;
		else if( l_u8AllPackNum-1 == l_u8PackNO )
			*(l_pu8SendBufTmp++) = 0x02;
		else
			*(l_pu8SendBufTmp++) = 0x01;

		//
		*(l_pu8SendBufTmp++) = l_u8PackNO; // ֡���
		*(l_pu8SendBufTmp++) = CHANNELNUM; // ͨ������
		*(l_pu8SendBufTmp++) = 0x00; //	 Ԥ��
		*(l_pu8SendBufTmp++) = 0x00; //	 Ԥ��

		//
		// �����˰�����
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

		//���CRC
		AddCrc16( g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[l_u8PackNO], l_u32tmp);
	}

	//ͨ��Э�鷢��������Ϣ
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
		//�������ݷ���ʧ��
//		OSQPost(g_pQErrMsg,(void *)ErrSndPCMsg);
		g_ErrMsgState |= 0x04; 		 	
	}
	
	// ��¼������ţ������ط�ʹ��
//	g_u8CurSendNetBufIndex = pdata->u8id;
}



/*********************************************************************************************************
** Function name:		SendDeviceStatus_FXC
** Descriptions:		���ֳ�ִ��Э�鶨ʱ10s����һ���豸״̬
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20121104	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SendDeviceStatus_FXC(void)	   	//ÿ��100ms����һ�κ���
{
	if(m_u8TimerNum >= SENDDEVICESTATUSINTERVAL)			//10s��һ��
	{
		m_u8TimerNum = 0 ;
		SendCommandToPC_FXC(0,ADJ_DEVICESTATUS);
	}

	m_u8TimerNum++;	
}





/*********************************************************************************************************
** Function name:		FFormHeartFrame_FXC
** Descriptions:		����
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
//	uint8*	l_pu8Tmp;	   //ָ�룬ָ����������
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
** Descriptions:		����
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
			if( g_au16HeartTimeCnt[l_u8Tempi] >= NET_HEART_SENDINTERVAL )			//10s��һ��
			{
// 				l_u32Ret = SendDataNet( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] ); 
				l_u32Ret = TcpSendData( l_u8Tempi, g_cHeartBuf.m_au8DataBuf[l_u8Tempi], g_cHeartBuf.m_u16Len[l_u8Tempi] ); 
				if( 0 == l_u32Ret )
				{
					// �쳣
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
** Descriptions:		�жϺ͸���
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
			// ��������
			// ��ʱ
			g_au16HeartTimeCnt[l_u8Tempi]++;
			if( g_au16HeartTimeCnt[l_u8Tempi] > 1000 )	  // ���ӱ���
			{
				g_au16HeartTimeCnt[l_u8Tempi] = 1000;
			}

			// ��ʼ������ջظ�ʱ��󣬿�ʼ��ʱ
			if( g_au8HeartRecvBeginCntFlag[l_u8Tempi] )
			{
				g_au16HeartRecvTimeCnt[l_u8Tempi]++;
				if( g_au16HeartRecvTimeCnt[l_u8Tempi] > 1000 )
				{
					g_au16HeartRecvTimeCnt[l_u8Tempi] = 1000;
				}
			}

			// ���ջظ�ʱ���ʱ>������ֵ��5s��
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
** Descriptions:		����չָ��֡
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
//	g_au8ExtDataBuf[6] = 0;	 		// ��ˮ��
//	g_au8ExtDataBuf[7] = 0;	 		// ����
//	memset( &g_au8ExtDataBuf[8], 0, 20 );
//	memset( &g_au8ExtDataBuf[28], 0, 10 );
//	//
//	return SUCCESS;
//}


