/****************************************Copyright (c)****************************************************
**                                     BEIJING  WANJI(WJ)                               
**                                     
**
*********************************************************************************************************/

#define	__LOOPLOGICFUNCTION_C

#include "common.h"
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam


/*********************************************************************************************************
** Function name:     FTransLocation
** Descriptions:      ��խ����ŵõ�խ�������к���
** input parameters:  խ�����        
** output parameters: խ���ڳ����ĺ�������
**
** Created by:	wgh	  
** Created Date:  2014-5-6	  
**-------------------------------------------------------------------------------------------------------
** Modified by:	 zhangtonghan  		
** Modified date: 20180604
** Descriptions: ����ģʽѡ��,1:��ʾ����������;2:��ʾ���ڵ�λ������
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FTransLocation(uint8 ZTID,uint8 *X, uint8 *Y,uint8 mode)
{	
	if(1 == mode)
	{
		if(ZTID < HideZTNUM)
		{
			*X = ZTID/PROMODPARAM.m_u8ZTRow;
			*Y = ZTID%PROMODPARAM.m_u8ZTRow;
		}
		else
		{
			ZTID=ZTID-HideZTNUM;
			*X = ZTID/HideZTRow;
			*Y = ZTID%HideZTRow;
		}
	}
	else if(2 == mode)
	{
		if(ZTID < HideZTNUM)
		{
			*X = ZTID/PROMODPARAM.m_u8ZTRow;
			if(0 == (*X%2))
				*Y = (ZTID%PROMODPARAM.m_u8ZTRow)*2+1;
			else
				*Y = (ZTID%PROMODPARAM.m_u8ZTRow)*2;
		}
		else
		{
			ZTID=ZTID-HideZTNUM;
			*X = ZTID/HideZTRow;
			*Y = ZTID%HideZTRow;
		}
	}
	else
	{
		*X = 0;
		*Y = 0;
	}
}
/*********************************************************************************************************
** Function name:     FTransLocationInvers
** Descriptions:      ��խ�������к��еõ�խ�����
** input parameters:  խ���ڳ����ĺ�������        
** output parameters: խ�����
**
** Created by:	wgh 	  
** Created Date:  2014-5-6	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FTransLocationInvers(uint8 X, uint8 Y, uint8 *ZTID)
{
	if(X < 2*PROMODPARAM.m_u8LaneNum)
	{
		*ZTID = X *	PROMODPARAM.m_u8ZTRow + Y;
	}
	else
	{
		*ZTID = X *	PROMODPARAM.m_u8ZTRow + Y;
	}
}

/*********************************************************************************************************
** Function name:     FPostVehForLoop
** Descriptions:      ��ɳ����׳�����
** input parameters:         
** output parameters: none
**
** Created by:	zhangtonghan 	  
** Created Date:  20170609	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		zhangtonghan  		
** Modified date:	  20170920		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FPostVehForLoop(void *AddressForPo[3],uint8 num)
{
	VehicleInfForLoop_Struct *VehiclePerChannelForLoop;
	uint8* pLaneNum;
	int8 *Dir;
	uint8 LaneNum;
//	void *l_u32AddrFcap[4];
//	uint8  l_u8HeadOrTail = 1; //��ͷΪ0,��βΪ1

	uint16  l_u16SendLen;
	uint8   l_u8Err;
	uint8   l_ret;
	
	uint8  i, j, cnt;              //��ʱ����
	uint16 NormalAxleWgtTemp1 = 0, NormalAxleWgtTemp2 = 0;   //��ʱ����
	uint8  NormalAxleWgtRatioFlag = 0;       //��׼����ж����ر��Ƿ����,0:����1:������,bitλ��Ӧÿ����(ֻ���4���6�ᳵ)
	uint8  PostVehNum = 0;         
	uint32 NormalBlankAllWgt = 0, HideBlankAllWgt = 0, MultiAllWgt = 0;
	uint8  NormalBankAccFlg = 0;       //��׼����Ƿ��мӼ���,0:û�мӼ���,!0:��n���мӼ���(����bitλ����־)
	uint8  HideBankAccFlg = 0;         //���ذ���Ƿ��мӼ���,0:û�мӼ���,!0:��n���мӼ���(����bitλ����־)
	
	int8   ret = -1;      

	uint8  AxleWgtSelectFlag = 0;    //0:��ʹ�ñ�׼�������,1:��ʹ�����ذ������,2:�������ȡƽ��

	uint8  NormalAxleScoreBuff[7], HideAxleScoreBuff[7];
	uint16 NormalAxleWgt = 0, HideAxleWgt = 0;
	uint16 VehScore = 1;
	uint8  NormalVehScore = 100, HideVehScore = 100;
	uint16 AxleWgtRatio = 0;
	
	//ɾ���켣��ر���
// 	uint16 l_u16AxlePos = 0;
// 	uint32 l_u32StartTime = 0, l_u32EndTime = 0;
// 	uint16 l_u16LeftXPos = 0xffff, l_u16RightXPos = 0;
// 	uint8  l_u8StartZTID = 0xff, l_u8EndZTID = 0xff;    
// 	uint8  l_u8ColumTemp = 0;
// 	uint8  temp = 0;    //��ʱ����
// 	void *l_Address[6];

	/**** �������ʼ�� ****/
	pLaneNum = AddressForPo[0];
	Dir = AddressForPo[1];
	VehiclePerChannelForLoop = AddressForPo[2];
	LaneNum = *pLaneNum;

	memset(NormalAxleScoreBuff, 0, sizeof(NormalAxleScoreBuff));
	memset(HideAxleScoreBuff, 0, sizeof(HideAxleScoreBuff));
	/**** end ****/
	
	/**** ���Դ��� ****/
	if(VehiclePerChannelForLoop->u8AixCnt == 3)
	{
		VehiclePerChannelForLoop->u8AixCnt = 3;
	}
	if(VehiclePerChannelForLoop->u8HideAixCnt == 4)
	{
		VehiclePerChannelForLoop->u8AixCnt = 4;
	}
	/**** end ****/
	
	/**** ����2�������Ե����ز���ӡ�����Ϣ ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		NormalBlankAllWgt += VehiclePerChannelForLoop->au16AxleWeight[i];
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		HideBlankAllWgt += VehiclePerChannelForLoop->au16HideAxleWeight[i];
	}
	
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPostVeh,Dr=%d,L=%d,CapL=%02x,485ID=%08X,%d.",
					           *Dir,LaneNum,VehiclePerChannelForLoop->m_Capture_LaneNum,VehiclePerChannelForLoop->m_u32LoopTime,num);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nNormalVeh AxCnt=%d,Wgt=%d,%d,%d,%d,%d,%d,All=%d.",
					           VehiclePerChannelForLoop->u8AixCnt,VehiclePerChannelForLoop->au16AxleWeight[0],VehiclePerChannelForLoop->au16AxleWeight[1],
							   VehiclePerChannelForLoop->au16AxleWeight[2],VehiclePerChannelForLoop->au16AxleWeight[3],VehiclePerChannelForLoop->au16AxleWeight[4],
							   VehiclePerChannelForLoop->au16AxleWeight[5],NormalBlankAllWgt);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nHideVeh AxCnt=%d,Wgt=%d,%d,%d,%d,%d,%d,All=%d.",
					           VehiclePerChannelForLoop->u8HideAixCnt,VehiclePerChannelForLoop->au16HideAxleWeight[0],VehiclePerChannelForLoop->au16HideAxleWeight[1],
							   VehiclePerChannelForLoop->au16HideAxleWeight[2],VehiclePerChannelForLoop->au16HideAxleWeight[3],VehiclePerChannelForLoop->au16HideAxleWeight[4],
							   VehiclePerChannelForLoop->au16HideAxleWeight[5],HideBlankAllWgt);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nVehTrace:%d,%d,%d,%d,%d,%d,%d.",
					           VehiclePerChannelForLoop->AxleStatus[0],VehiclePerChannelForLoop->AxleStatus[1],
							   VehiclePerChannelForLoop->AxleStatus[2],VehiclePerChannelForLoop->AxleStatus[3],
							   VehiclePerChannelForLoop->AxleStatus[4],VehiclePerChannelForLoop->AxleStatus[5],
							   VehiclePerChannelForLoop->AxleStatus[6]);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
	
	//��ӡխ������
	if(VehiclePerChannelForLoop->u8AixCnt == VehiclePerChannelForLoop->u8HideAixCnt && VehiclePerChannelForLoop->u8AixCnt > 2)
	{
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nJCW %d [%d %d %d %d %d %d %d %d %d].",
										i+1,
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[0],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[1],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[2],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[3],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[4],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[5],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[6],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[7],
										g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTWgt[8]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nYZW %d [%d %d %d %d %d %d].",
										i+1,
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[0],
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[1],
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[2],
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[3],
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[4],
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTWgt[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nJCP %d [%d %d %d %d %d %d %d %d %d] s=%d.",
										i+1,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[0]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[1]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[2]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[3]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[4]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[5]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[6]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[7]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].ZTPos[8]].u8Id,
									    g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16speed);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		
		for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nYZP %d [%d %d %d %d %d %d] s=%d.",
										i+1,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[0]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[1]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[2]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[3]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[4]].u8Id,
										g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].ZTPos[5]].u8Id,
										g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].u16speed);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
	}
	
	
// 	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
// 	{
// 		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nWhlWgt %d [%d %d].",
// 								    i+1,
// 									g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].WheelWgt[0],
// 									g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].WheelWgt[1]);
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 		OSSemPost(g_pSemSendThtouSP);
// 	}
	
	/**** end ****/
	
	/**** β�� ****/
//	l_u32AddrFcap[0] = &VehiclePerChannelForLoop->m_Capture_LaneNum;
//	l_u32AddrFcap[1] = &VehiclePerChannelForLoop->m_u32LoopTime;
//	l_u32AddrFcap[2] = Dir;
//	l_u32AddrFcap[3] = &l_u8HeadOrTail;
//				
//	FCapture(l_u32AddrFcap,2);
	/**** end ****/
		
	if(g_u8TwoBlankUnion)     //�������ͨ������ϵͳ���г���,������ʽ��2��(ѡ������һ��������������������������)
	{
		/**** Υ����(���) ****/
		if((NormalBlankAllWgt > 5000) && ((LaneNum == 0) || (LaneNum == 2*PROMODPARAM.m_u8LaneNum - 1)))    //���������ϵ����е����ֵ����ʱ��Ϊ�ǿ��
		{
			for(i=0; i<g_vehicleInfo[g_u8vehicleID].u8AixCnt; i++)   
			{
				if(g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleWhelCnt == 1)   //������
				{
					if(VehiclePerChannelForLoop->u8ViolationCode < 0x40)
					{
						VehiclePerChannelForLoop->u8ViolationCode = 0x40;
					}
					break;
				}
			}
		}
		/**** end ****/
		
		/**** �ж���Щ���мӼ��� ****/
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
			for(j = 0; j < g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAccCnt; j++)
			{
				if((g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAcc[j] > AxleAccRange) || 
				   (g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].RunAndWalkFlag && 
				   (4*g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAcc[j] > AxleAccRange*3)) )
				{
					NormalBankAccFlg |= (0x01<<i);
					break;
				}
			}
		}
		for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
		{
			cnt = 0;
			g_vehicleInfo[g_u8vehicleID].u16HideAxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
			for(j = 0; j < g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].AxleAccCnt; j++)
			{
				if(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].AxleAcc[j] > AxleAccRange)
				{
					cnt++;
				}
			}
			if(cnt > 1)
			{
				HideBankAccFlg |= 0x01<<i;
			}
		}
		/**** end ****/
		
		/**** �ж�4���6�ᳵ���ر��Ƿ���� ****/
		if(VehiclePerChannelForLoop->u8AixCnt == VehiclePerChannelForLoop->u8HideAixCnt)
		{
			if(VehiclePerChannelForLoop->u8AixCnt == 4)
			{
				NormalAxleWgtTemp1 = VehiclePerChannelForLoop->au16AxleWeight[2];
				NormalAxleWgtTemp2 = VehiclePerChannelForLoop->au16AxleWeight[3];
				if((NormalAxleWgtTemp1 * 3 < NormalAxleWgtTemp2) && (NormalAxleWgtTemp1 < VehiclePerChannelForLoop->au16HideAxleWeight[2]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<2;
				}
				else if((NormalAxleWgtTemp2 * 3 < NormalAxleWgtTemp1) && (NormalAxleWgtTemp2 < VehiclePerChannelForLoop->au16HideAxleWeight[3]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<3;
				}
			}
			else if(VehiclePerChannelForLoop->u8AixCnt == 6)
			{
				NormalAxleWgtTemp1 = VehiclePerChannelForLoop->au16AxleWeight[1];
				NormalAxleWgtTemp2 = VehiclePerChannelForLoop->au16AxleWeight[2];
				if((NormalAxleWgtTemp1 * 3 < NormalAxleWgtTemp2) && (NormalAxleWgtTemp1 < VehiclePerChannelForLoop->au16HideAxleWeight[1]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<1;
				}
				else if((NormalAxleWgtTemp2 * 3 < NormalAxleWgtTemp1) && (NormalAxleWgtTemp2 < VehiclePerChannelForLoop->au16HideAxleWeight[2]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<2;
				}
				
				NormalAxleWgtTemp1 = VehiclePerChannelForLoop->au16AxleWeight[3];
				NormalAxleWgtTemp2 = VehiclePerChannelForLoop->au16AxleWeight[4];
				if((NormalAxleWgtTemp1 * 3 < NormalAxleWgtTemp2) && (NormalAxleWgtTemp1 < VehiclePerChannelForLoop->au16HideAxleWeight[3]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<3;
				}
				else if((NormalAxleWgtTemp2 * 3 < NormalAxleWgtTemp1) && (NormalAxleWgtTemp2 < VehiclePerChannelForLoop->au16HideAxleWeight[4]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<4;
				}
				
				NormalAxleWgtTemp1 = VehiclePerChannelForLoop->au16AxleWeight[4];
				NormalAxleWgtTemp2 = VehiclePerChannelForLoop->au16AxleWeight[5];
				if((NormalAxleWgtTemp1 * 3 < NormalAxleWgtTemp2) && (NormalAxleWgtTemp1 < VehiclePerChannelForLoop->au16HideAxleWeight[4]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<4;
				}
				else if((NormalAxleWgtTemp2 * 3 < NormalAxleWgtTemp1) && (NormalAxleWgtTemp2 < VehiclePerChannelForLoop->au16HideAxleWeight[5]))
				{
					NormalAxleWgtRatioFlag |= 0x01<<5;
				}
			}
		}
		/**** end ****/
		
		PostVehNum = FUnPostVeh(VehiclePerChannelForLoop);
		FAxleScore(VehiclePerChannelForLoop);    //����������
		memcpy(NormalAxleScoreBuff, VehiclePerChannelForLoop->NormalAxleScore, sizeof(uint8)*7);
		memcpy(HideAxleScoreBuff, VehiclePerChannelForLoop->HideAxleScore, sizeof(uint8)*7);
		
		/**** ��������ѡ����������복���� ****/
		g_vehicleInfo[g_u8vehicleID].u8id = g_u8vehicleID;//����ID
		g_vehicleInfo[g_u8vehicleID].u8psLane = LaneNum;
		g_vehicleInfo[g_u8vehicleID].m_u8LaserVehID = VehiclePerChannelForLoop->m_u8VehIDFromLaser;
		g_vehicleInfo[g_u8vehicleID].m_u32LoopTime = VehiclePerChannelForLoop->m_u32LoopTime;
		g_vehicleInfo[g_u8vehicleID].m_Capture_LaneNum = VehiclePerChannelForLoop->m_Capture_LaneNum;
		g_vehicleInfo[g_u8vehicleID].u16speed =  VehiclePerChannelForLoop->u16speed;
		g_vehicleInfo[g_u8vehicleID].u8AixCnt = VehiclePerChannelForLoop->u8AixCnt;
		g_vehicleInfo[g_u8vehicleID].m_BigVehFollowLitVeh = VehiclePerChannelForLoop->m_BigVehFollowLitVeh;
		
		
		
		if(VehiclePerChannelForLoop->u8AixCnt == VehiclePerChannelForLoop->u8HideAixCnt)    //�������������ͬ
		{
// 			if(NormalBankAccFlg & HideBankAccFlg)        //���������ͬ�ᶼ�мӼ���,���ϳ���
// 			{
// 				PostVehNum = 1;
// 				g_vehicleInfo[g_u8vehicleID].UnionWgtFlg = 1;
// 			}
			for(i=0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
			{
				ret = AxleWgtJundge(VehiclePerChannelForLoop->au16AxleWeight[i], VehiclePerChannelForLoop->au16HideAxleWeight[i]);
				if(ret == 4)    //����������ض�������,ѡ���
				{
					if(VehiclePerChannelForLoop->au16AxleWeight[i] < VehiclePerChannelForLoop->au16HideAxleWeight[i])
					{
						AxleWgtSelectFlag = 0;
					}
					else
					{
						AxleWgtSelectFlag = 1;
					}
				}
				else if(ret == 2)     //��׼������غ���
				{
					AxleWgtSelectFlag = 0;
				}
				else if(ret == 2)     //���ذ�����غ���
				{
					AxleWgtSelectFlag = 1;
				}
				else    //����������ض�����
				{
					//��ÿ�����������,���ݷ���������
					NormalAxleScoreBuff[i] = VehiclePerChannelForLoop->NormalAxleScore[i];
					HideAxleScoreBuff[i] = VehiclePerChannelForLoop->HideAxleScore[i];
					
					if(NormalAxleScoreBuff[i] > g_u8AxleMultiScore && HideAxleScoreBuff[i] > g_u8AxleMultiScore)
					{
						if(F_Min(VehiclePerChannelForLoop->au16AxleWeight[i], VehiclePerChannelForLoop->au16HideAxleWeight[i]) > 0)
						{
							AxleWgtRatio = GetDiff(VehiclePerChannelForLoop->au16AxleWeight[i], VehiclePerChannelForLoop->au16HideAxleWeight[i]) * 100
										   / F_Min(VehiclePerChannelForLoop->au16AxleWeight[i], VehiclePerChannelForLoop->au16HideAxleWeight[i]);
						}
						if(AxleWgtRatio < 20)
						{
							AxleWgtSelectFlag = 2;
						}
						else    //���ݱ�׼���͵����ؼ����ѡ���Ӧ���(��ʱ�н�����)
						{
							AxleWgtSelectFlag = 0;
						}
					}
					else   //ѡ������ߵ���
					{
						if(NormalAxleScoreBuff[i] > HideAxleScoreBuff[i])
							AxleWgtSelectFlag = 0;
						else if(NormalAxleScoreBuff[i] < HideAxleScoreBuff[i])
							AxleWgtSelectFlag = 1;
						else        //������������ͬ
						{
							NormalAxleWgt = VehiclePerChannelForLoop->au16AxleWeight[i];
							HideAxleWgt = VehiclePerChannelForLoop->au16HideAxleWeight[i];
							
							if( ((GetDiff(NormalAxleWgt, HideAxleWgt)*100 < F_Max(NormalAxleWgt, HideAxleWgt)*30) && (VehiclePerChannelForLoop->u8AixCnt > 2)) ||
								((GetDiff(NormalAxleWgt, HideAxleWgt)*100 < F_Max(NormalAxleWgt, HideAxleWgt)*15) && (VehiclePerChannelForLoop->u8AixCnt <= 2)) )
							{
								AxleWgtSelectFlag = 2;
							}
							else   //��������������ϴ��Ҷ��ں���Χ��,��ȡ�ص�
							{
								if(NormalAxleWgt > HideAxleWgt)
								{
									AxleWgtSelectFlag = 0;
								}
								else
								{
									AxleWgtSelectFlag = 1;
								}
							}
						}
					}
				}
				if(AxleWgtSelectFlag == 0)    //ʹ�ñ�׼�������
				{
					g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = VehiclePerChannelForLoop->au16AxleWeight[i];
					g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
					
					if(g_vehicleInfo[g_u8vehicleID].UnionWgtFlg)   //���ϳ���ʧ��ʹ�����������
					{
						g_vehicleInfo[g_u8vehicleID].au16UnionFailedAxleWeight[i] = VehiclePerChannelForLoop->au16AxleWeight[i];
						g_vehicleInfo[g_u8vehicleID].u16UnionFailedAxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
					}
				}
				else if(AxleWgtSelectFlag == 1)    //ʹ�����ذ������
				{
					g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = VehiclePerChannelForLoop->au16HideAxleWeight[i];
					g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
					
					if(g_vehicleInfo[g_u8vehicleID].UnionWgtFlg)    //���ϳ���ʧ��ʹ�����������
					{ 
						g_vehicleInfo[g_u8vehicleID].au16UnionFailedAxleWeight[i] = VehiclePerChannelForLoop->au16HideAxleWeight[i] ;
						g_vehicleInfo[g_u8vehicleID].u16UnionFailedAxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
					}
				}
				else   //ȡƽ��
				{
					g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = (VehiclePerChannelForLoop->au16AxleWeight[i] + VehiclePerChannelForLoop->au16HideAxleWeight[i]) / 2;
					g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
					
					if(g_vehicleInfo[g_u8vehicleID].UnionWgtFlg)  //���ϳ���ʧ��ʹ�����������
					{
						g_vehicleInfo[g_u8vehicleID].au16UnionFailedAxleWeight[i] = (VehiclePerChannelForLoop->au16AxleWeight[i] + VehiclePerChannelForLoop->au16HideAxleWeight[i]) / 2;
						g_vehicleInfo[g_u8vehicleID].u16UnionFailedAxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
					}
				}
				
				g_vehicleInfo[g_u8vehicleID].au8AxleTAIType[i] = VehiclePerChannelForLoop->au8AxleTAIType[i];
				g_vehicleInfo[g_u8vehicleID].n8accSpeed[i] = VehiclePerChannelForLoop->n8accSpeed[i];
				g_vehicleInfo[g_u8vehicleID].m_au8AxlePoint[i] = VehiclePerChannelForLoop->m_au8AxlePoint[i];
				if(i < g_vehicleInfo[g_u8vehicleID].u8AixCnt-1)
					g_vehicleInfo[g_u8vehicleID].au16AxleDistance[i] = VehiclePerChannelForLoop->au16AxleDistance[i];
			}
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\nPost=%d,[%d,%d,%d,%d,%d,%d],[%d,%d,%d,%d,%d,%d].\r\n",
									PostVehNum,NormalAxleScoreBuff[0],NormalAxleScoreBuff[1],NormalAxleScoreBuff[2],NormalAxleScoreBuff[3],NormalAxleScoreBuff[4],NormalAxleScoreBuff[5],
									HideAxleScoreBuff[0],HideAxleScoreBuff[1],HideAxleScoreBuff[2],HideAxleScoreBuff[3],HideAxleScoreBuff[4],HideAxleScoreBuff[5]);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		else     //�������������ͬ
		{
			VehScore = FVehScore(VehiclePerChannelForLoop);
			NormalVehScore = (uint8)(VehScore&0xff);
			HideVehScore = (uint8)((VehScore&0xff00)>>8);
			
			if(NormalVehScore < HideVehScore)    //���ذ�������
			{
				AxleWgtSelectFlag = 1;
			}
			else if(NormalVehScore > HideVehScore)    //�к���������
			{
				AxleWgtSelectFlag = 0;
			}
			else
			{
				if(VehiclePerChannelForLoop->u8AixCnt > 6)     //�к�����>6,��ѡ���ְ��
				{
					AxleWgtSelectFlag = 1;
				}
				else if(VehiclePerChannelForLoop->u8HideAixCnt > 6)    //��������>6,�к�����<6,ѡ���к�
				{
					AxleWgtSelectFlag = 0;
				}
				else    //������<=6ѡ��������İ��
				{
					if(VehiclePerChannelForLoop->u8AixCnt > VehiclePerChannelForLoop->u8HideAixCnt)
					{
						AxleWgtSelectFlag = 0;
					}
					else
					{
						AxleWgtSelectFlag = 1;
					}
				}
			}
			
			if(AxleWgtSelectFlag == 0)
			{
				g_vehicleInfo[g_u8vehicleID].u8AixCnt = VehiclePerChannelForLoop->u8AixCnt;
			}
			else
			{
				g_vehicleInfo[g_u8vehicleID].u8AixCnt = VehiclePerChannelForLoop->u8HideAixCnt;
			}
			g_vehicleInfo[g_u8vehicleID].m_BigVehFollowLitVeh = 0;
			for(i = 0; i < g_vehicleInfo[g_u8vehicleID].u8AixCnt; i++)
			{
				if(AxleWgtSelectFlag == 0)    //ʹ���к��������
				{
					g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = VehiclePerChannelForLoop->au16AxleWeight[i];
					g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
				}
				else if(AxleWgtSelectFlag == 1)    //ʹ�����ְ������
				{
					g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = VehiclePerChannelForLoop->au16HideAxleWeight[i];
					g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
				}
				g_vehicleInfo[g_u8vehicleID].au8AxleTAIType[i] = VehiclePerChannelForLoop->au8AxleTAIType[i];
				g_vehicleInfo[g_u8vehicleID].n8accSpeed[i] = VehiclePerChannelForLoop->n8accSpeed[i];
				g_vehicleInfo[g_u8vehicleID].m_au8AxlePoint[i] = VehiclePerChannelForLoop->m_au8AxlePoint[i];
				if(i < g_vehicleInfo[g_u8vehicleID].u8AixCnt-1)
					g_vehicleInfo[g_u8vehicleID].au16AxleDistance[i] = VehiclePerChannelForLoop->au16AxleDistance[i];
			}
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\nAxle Diff! Post=%d,Num[%d,%d],Score[%d,%d].\r\n",
										PostVehNum, VehiclePerChannelForLoop->u8AixCnt, VehiclePerChannelForLoop->u8HideAixCnt, NormalVehScore, HideVehScore);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		/**** end ****/
	}
	else
	{
		g_vehicleInfo[g_u8vehicleID].u8id = g_u8vehicleID;//����ID
		g_vehicleInfo[g_u8vehicleID].u8psLane = LaneNum;
		g_vehicleInfo[g_u8vehicleID].u8AixCnt = VehiclePerChannelForLoop->m_n8CurrentAxleNum+1;
		g_vehicleInfo[g_u8vehicleID].m_u8LaserVehID = VehiclePerChannelForLoop->m_u8VehIDFromLaser;
		g_vehicleInfo[g_u8vehicleID].m_u32LoopTime = VehiclePerChannelForLoop->m_u32LoopTime;
		g_vehicleInfo[g_u8vehicleID].m_Capture_LaneNum = VehiclePerChannelForLoop->m_Capture_LaneNum;
		g_vehicleInfo[g_u8vehicleID].u16speed =  VehiclePerChannelForLoop->u16speed;
		g_vehicleInfo[g_u8vehicleID].m_BigVehFollowLitVeh = VehiclePerChannelForLoop->m_BigVehFollowLitVeh;
		for(i=0; i<g_vehicleInfo[g_u8vehicleID].u8AixCnt; i++)
		{
			g_vehicleInfo[g_u8vehicleID].au8AxleTAIType[i] = VehiclePerChannelForLoop->au8AxleTAIType[i];
			g_vehicleInfo[g_u8vehicleID].AxleStatus[i] = VehiclePerChannelForLoop->AxleStatus[i];
			g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
//			g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = VehiclePerChannelForLoop->au16AxleWeight[i];
			g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i] = g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].s32AxleWeight;
			g_vehicleInfo[g_u8vehicleID].n8accSpeed[i] = VehiclePerChannelForLoop->n8accSpeed[i];
//			g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[l_u8tempi]].s32AxleWeight =g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i];
			g_vehicleInfo[g_u8vehicleID].m_au8AxlePoint[i] = VehiclePerChannelForLoop->m_au8AxlePoint[i];
			if(i < g_vehicleInfo[g_u8vehicleID].u8AixCnt-1)
				g_vehicleInfo[g_u8vehicleID].au16AxleDistance[i] = VehiclePerChannelForLoop->au16AxleDistance[i];
		}
		
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nTwo Module weight Disable! Use normal blank."); 
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	}

	/**** ���ݳ�����λ�ú�ʱ����Ϣɾ�������켣 ****/
// 	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)    //ÿ���ᶼѰ���Ƿ������Ʋ���խ��
// 	{
// 		l_u16AxlePos = VehiclePerChannelForLoop->u16AxleList[i];
// 		if(g_AxleInfo[l_u16AxlePos].AxleWhelCnt == 2)    //˫��
// 		{
// 			//��ǰ�����ʼ�ͽ���ʱ��
// 			l_u32StartTime = F_Min(g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[0]].u32dntm, g_ChlData[g_AxleInfo[l_u16AxlePos].u16RightWheeList[0]].u32dntm);
// 			l_u32EndTime = F_Max(g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[g_AxleInfo[l_u16AxlePos].LeftWheelZTCnt-1]].u32dntm,
// 							g_ChlData[g_AxleInfo[l_u16AxlePos].u16RightWheeList[g_AxleInfo[l_u16AxlePos].RightWheelZTCnt-1]].u32dntm);
// 			
// 			//��ǰ������������Xλ��
// 			if(g_AxleInfo[l_u16AxlePos].Dir == 1)    //����
// 			{
// 				l_u16LeftXPos = F_Min(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange, g_AxleInfo[l_u16AxlePos].LeftWheelXRange);
// 				l_u16RightXPos = F_Max(g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange, g_AxleInfo[l_u16AxlePos].RightWheelXRange);
// 			}
// 			else   //����
// 			{
// 				l_u16LeftXPos = F_Min(g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange, g_AxleInfo[l_u16AxlePos].RightWheelXRange);
// 				l_u16RightXPos = F_Max(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange, g_AxleInfo[l_u16AxlePos].LeftWheelXRange);
// 			}	
// 		}
// 		else    //����
// 		{
// 			//��ǰ�����ʼ�ͽ���ʱ��
// 			l_u32StartTime = g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[0]].u32dntm;
// 			l_u32EndTime = g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[g_AxleInfo[l_u16AxlePos].LeftWheelZTCnt-1]].u32dntm;
// 			
// 			//��ǰ������������Xλ��
// 			l_u16LeftXPos = F_Min(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange, g_AxleInfo[l_u16AxlePos].LeftWheelXRange);
// 			l_u16RightXPos = F_Max(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange, g_AxleInfo[l_u16AxlePos].LeftWheelXRange);
// 		}
// 		
// 		//��������36cm(�����߼�������洢Left��Rightֵ)
// 		l_u16LeftXPos = (l_u16LeftXPos > 36) ? (l_u16LeftXPos - 36) : 0;     
// 		l_u16RightXPos += 36;    
// 		
// 		//��������ɾ������խ������ʼխ��ID
// 		l_u8ColumTemp = 0xff;
// 		for(j = 0; j < g_AxleInfo[l_u16AxlePos].LeftWheelZTCnt; j++)
// 		{
// 			temp = g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[j]].u8Id / 3;
// 			if(temp < l_u8ColumTemp)
// 				l_u8ColumTemp = temp;
// 		}
// 		l_u8ColumTemp = (l_u8ColumTemp > 0) ? (l_u8ColumTemp - 1) : 0;
// 		l_u8StartZTID = l_u8ColumTemp * 3;
// 		
// 		l_u8ColumTemp = 0;
// 		for(j = 0; j < g_AxleInfo[l_u16AxlePos].RightWheelZTCnt; j++)
// 		{
// 			temp = g_ChlData[g_AxleInfo[l_u16AxlePos].u16RightWheeList[j]].u8Id / 3;
// 			if(temp > l_u8ColumTemp)
// 				l_u8ColumTemp = temp;
// 		}
// 		l_u8ColumTemp = (l_u8ColumTemp < 2*g_cProModParam.m_u8LaneNum-1) ? (l_u8ColumTemp + 1) : (2*g_cProModParam.m_u8LaneNum-1);
// 		l_u8EndZTID = l_u8ColumTemp * 3 + 2;
// 		
// 		//�������խ��
// 		l_Address[0] = &l_u32StartTime;
// 		l_Address[1] = &l_u32EndTime;
// 		l_Address[2] = &l_u16LeftXPos;
// 		l_Address[3] = &l_u16RightXPos;
// 		l_Address[4] = &l_u8StartZTID;
// 		l_Address[5] = &l_u8EndZTID;
// 		FClearTrace(l_Address, 2);
// 	}
// 	
// 	l_u16AxlePos = VehiclePerChannelForLoop->u16AxleList[0];
// 	l_u32StartTime = F_Min(g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[0]].u32dntm, g_ChlData[g_AxleInfo[l_u16AxlePos].u16RightWheeList[0]].u32dntm);
// 	
// 	l_u16AxlePos = VehiclePerChannelForLoop->u16AxleList[VehiclePerChannelForLoop->u8AixCnt-1];
// 	l_u32EndTime = F_Max(g_ChlData[g_AxleInfo[l_u16AxlePos].u16LeftWheeList[g_AxleInfo[l_u16AxlePos].LeftWheelZTCnt-1]].u32dntm,
// 						 g_ChlData[g_AxleInfo[l_u16AxlePos].u16RightWheeList[g_AxleInfo[l_u16AxlePos].RightWheelZTCnt-1]].u32dntm);
// 	
// 	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
// 	{
// 		l_u16AxlePos = VehiclePerChannelForLoop->u16AxleList[i];
// 		if(g_AxleInfo[l_u16AxlePos].Dir == 1)
// 		{
// 			if(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange < l_u16LeftXPos)
// 				l_u16LeftXPos = g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange;
// 			if(g_AxleInfo[l_u16AxlePos].LeftWheelXRange < l_u16LeftXPos)
// 				l_u16LeftXPos = g_AxleInfo[l_u16AxlePos].LeftWheelXRange;
// 			
// 			if(g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange > l_u16RightXPos)
// 				l_u16RightXPos = g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange;
// 			if(g_AxleInfo[l_u16AxlePos].RightWheelXRange > l_u16RightXPos)
// 				l_u16RightXPos = g_AxleInfo[l_u16AxlePos].RightWheelXRange;
// 		}
// 		else if(g_AxleInfo[l_u16AxlePos].Dir == -1)    //����ʱLeftPosΪ����λ��
// 		{
// 			if(g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange < l_u16LeftXPos)
// 				l_u16LeftXPos = g_AxleInfo[l_u16AxlePos].BuildRghWhelXRange;
// 			if(g_AxleInfo[l_u16AxlePos].LeftWheelXRange < l_u16LeftXPos)
// 				l_u16LeftXPos = g_AxleInfo[l_u16AxlePos].LeftWheelXRange;
// 			
// 			if(g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange > l_u16RightXPos)
// 				l_u16RightXPos = g_AxleInfo[l_u16AxlePos].BuildLeftWhelXRange;
// 			if(g_AxleInfo[l_u16AxlePos].LeftWheelXRange > l_u16RightXPos)
// 				l_u16RightXPos = g_AxleInfo[l_u16AxlePos].LeftWheelXRange;
// 		}
// 	}
// 	
// 	//��������36cm(�����߼�������洢Left��Rightֵ)
// 	l_u16LeftXPos -= 36;      
// 	l_u16RightXPos += 36;    
// 	
// 	l_Address[0] = &l_u32StartTime;
// 	l_Address[1] = &l_u32EndTime;
// 	l_Address[2] = &l_u16LeftXPos;
// 	l_Address[3] = &l_u16RightXPos;
// 	FClearTrace(l_Address, 2);
	/**** end ****/
	/**** �ԱȽ������һ�ְ������ ****/
	if(VehiclePerChannelForLoop->u8AixCnt == VehiclePerChannelForLoop->u8HideAixCnt)
	{
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			MultiAllWgt += g_vehicleInfo[g_u8vehicleID].au16AxleWeight[i];
		}
		if((MultiAllWgt > 0) && (GetDiff(NormalBlankAllWgt, HideBlankAllWgt) / MultiAllWgt)*100 > 30)
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n####[%d]####\n.",
									   (GetDiff(NormalBlankAllWgt, HideBlankAllWgt) / MultiAllWgt)*100);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
	}
	/**** end ****/
	
	if(((*Dir == -1) && (PROMODPARAM.m_u8BwdDirecFlag == 0x01) && (PROMODPARAM.m_au8VehDirecFlag[0] == 0x03)) || (*Dir == 1))
	{
		l_ret = OSQPost( g_pQShouWei, &g_vehicleInfo[g_u8vehicleID]);
	
		if( l_ret == OS_Q_FULL)
		{
			g_ErrMsgState |= 0x02; 	
		}
	}
	FInitVehStr(VehiclePerChannelForLoop,*Dir);
	g_u8vehicleID++;
	g_u8vehicleID = g_u8vehicleID%VEHICLE_BUFLEN;
	memset( &g_vehicleInfo[g_u8vehicleID], 0, sizeof(Vehicle_Struct));	
}

/*********************************************************************************************************
** Function name:     AxleWgtJundge
** Descriptions:      �ж����������Ƿ����(��������ǰ������ѡ��)
** input parameters:  wgt1:�����������,wgt2:���ذ������
** output parameters: ����������������Ƿ��쳣, 1:����������ض�����, 2:��׼�����������,���ذ�������쳣, 
												3:��׼��������쳣,���ذ����������, 4:������鶼�쳣
**
** Created by:	liumingxin
** Created Date:  2018-07-02	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 AxleWgtJundge(uint16 wgt1, uint16 wgt2)
{
	uint16 NormalAxleWgt = 0, HideAxleWgt = 0;
	int8 ret = 1;    //1:����������ض�����, 2:��׼�����������,���ذ�������쳣, 3:��׼��������쳣,���ذ����������, 4:������鶼�쳣
	
	NormalAxleWgt = wgt1;
	HideAxleWgt = wgt2;
	if( ((NormalAxleWgt<AxleWgtMinError) || (NormalAxleWgt>AxleWgtMaxError)) &&
		((HideAxleWgt<AxleWgtMinError) || (HideAxleWgt>AxleWgtMaxError)) )         //����������ض��쳣
	{
		ret = 4;
	}
	else if( ((NormalAxleWgt<AxleWgtMinError) || (NormalAxleWgt>AxleWgtMaxError)) && 
			 ((HideAxleWgt>AxleWgtMinError) && (HideAxleWgt<AxleWgtMaxError)) )    //��׼��������쳣,���ذ����������
	{
		ret = 3;
	}
	else if( ((NormalAxleWgt>AxleWgtMinError) && (NormalAxleWgt<AxleWgtMaxError)) &&
		     ((HideAxleWgt<AxleWgtMinError) || (HideAxleWgt>AxleWgtMaxError)) )    //��׼�����������,���ذ�������쳣
	{
		ret = 2;
	}
	else        //����������ض�����
	{
		ret = 1;
	}
	
	return ret;
}

/*********************************************************************************************************
** Function name:     FAxleScore
** Descriptions:      Ϊÿ��������
** input parameters:  ����Ϣ
** output parameters: None
**
** Created by:	liumingxin
** Created Date:  2018-07-02
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FAxleScore(VehicleInfForLoop_Struct *VehInfo)
{
	VehicleInfForLoop_Struct *VehiclePerChannelForLoop;
	
	uint8  i, j;
	uint8  AxlePos;
	uint8  ScoreBuff[7];
	uint32 NormalAllWgt = 0, HideAllWgt = 0;
	uint8  StatusScore = 100;     //��ʻ״̬�÷�,��ʼ����Ϊ100
	uint8  EvaluateScore = 100;   //���۵÷�
	uint8  TraceScore = 100;      //�켣�÷�
	uint8  a1 = 50, a2 = 20, a3 = 30;    //Ȩֵ(��ʻ״̬�÷�,���۷�,�켣��)
	
	VehiclePerChannelForLoop = VehInfo;
	memset(ScoreBuff, 100, sizeof(uint8)*7);
	
	//�����������ĳ�������
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		NormalAllWgt += VehiclePerChannelForLoop->au16AxleWeight[i];
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		HideAllWgt += VehiclePerChannelForLoop->au16HideAxleWeight[i];
	}
	
	/**** ���б�׼��������� ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		StatusScore = 100;     //��ʻ״̬�÷�,��ʼ����Ϊ100
		EvaluateScore = 100;   //���۵÷�
		TraceScore = 100;      //�켣�÷�
		
		AxlePos = VehiclePerChannelForLoop->u16AxleList[i];
		
		//��ʻ״̬����
		for(j = 0; j < g_AxleInfo[AxlePos].AxleAccCnt; j++)
		{
			if((g_AxleInfo[AxlePos].AxleAcc[j] > AxleAccRange) || 
			   ((g_AxleInfo[AxlePos].RunAndWalkFlag) && (4*g_AxleInfo[AxlePos].AxleAcc[j] > AxleAccRange*3)))
			{
				StatusScore = (StatusScore >= 20)?(StatusScore - 20):0;    //�Ӽ���,-20        
				break;
			}
		}
		if(g_AxleInfo[AxlePos].AxleStatus == XIEZOULEFT || g_AxleInfo[AxlePos].AxleStatus == XIEZOURIGHT)   
		{
			StatusScore = (StatusScore >= 10)?(StatusScore - 10):0;    //б��,-10
		}
		if(g_AxleInfo[AxlePos].RunAndWalkFlag == 0x01)
		{
			StatusScore = (StatusScore >= 25)?(StatusScore - 25):0;    //����ͣͣ,-25
		}
		
		//���۵÷�
		if(g_AxleInfo[AxlePos].u8AxleErrorType & 0x01)
		{
			EvaluateScore = (EvaluateScore >= 5)?(EvaluateScore - 5):0;    //-5
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<1))
		{
			EvaluateScore = (EvaluateScore >= 7)?(EvaluateScore - 7):0;    //-7
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<2))
		{
			EvaluateScore = (EvaluateScore >= 100)?(EvaluateScore - 100):0;    //ͬһ�г���2����ͬID-100
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<3))
		{
			if( (i == 0) && (NormalAllWgt > 5000) )
			{
				EvaluateScore = (EvaluateScore >= 30)?(EvaluateScore - 30):0;    //��11������-30,������-50, 11�����۷�
			}
			else if( (i > 0) && (NormalAllWgt > 5000) )
			{
				EvaluateScore = (EvaluateScore >= 50)?(EvaluateScore - 50):0;    //��11������-30,������-50, 11�����۷�
			}
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<4))
		{
			EvaluateScore = (EvaluateScore >= 15)?(EvaluateScore - 15):0;
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<5))
		{
			//None
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<6))
		{
			EvaluateScore = (EvaluateScore >= 100)?(EvaluateScore - 100):0;
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<7))
		{
			//None
		}
		
		//�켣�÷�
		TraceScore = g_AxleInfo[AxlePos].u8PointsGet;      
		
		//���ۺϵ÷�
		ScoreBuff[i] = a1 * StatusScore / 100 + a2 * EvaluateScore / 100 + a3 * TraceScore / 100;
	}
	memcpy(VehiclePerChannelForLoop->NormalAxleScore, ScoreBuff, sizeof(uint8)*7);
	
	/**** �������ذ�������� ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		StatusScore = 100;     //��ʻ״̬�÷�,��ʼ����Ϊ100
		EvaluateScore = 100;   //���۵÷�
		TraceScore = 100;      //�켣�÷�
		
		AxlePos = VehiclePerChannelForLoop->u16HideAxleList[i];
		//��ʻ״̬����
		if(g_AxleInfo[AxlePos].AxleStatus == XIEZOULEFT || g_AxleInfo[AxlePos].AxleStatus == XIEZOURIGHT)  
		{
			StatusScore = (StatusScore >= 20)?(StatusScore - 20):0;    //б��,-20
		}
		if(g_AxleInfo[AxlePos].RunAndWalkFlag & 0x04)
		{
			StatusScore = (StatusScore >= 30)?(StatusScore - 30):0;    //���ذ������ͣͣ,-30
		}
		
		//���۵÷�
		if(g_AxleInfo[AxlePos].u8AxleErrorType & 0x01)
		{
			EvaluateScore = (EvaluateScore >= 5)?(EvaluateScore - 5):0;    //-5
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<1))
		{
			//None
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<2))
		{
			EvaluateScore = (EvaluateScore >= 100)?(EvaluateScore - 100):0;    //ͬһ�г���2����ͬID-100
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<3))
		{
			//None
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<4))
		{
			//None
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<5))
		{
			EvaluateScore = (EvaluateScore >= 15)?(EvaluateScore - 15):0;
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<6))
		{
			EvaluateScore = (EvaluateScore >= 100)?(EvaluateScore - 100):0;
		}
		if(g_AxleInfo[AxlePos].u8AxleErrorType & (0x01<<7))
		{
			//None
		}
		
		//�켣�÷�
		TraceScore = g_AxleInfo[AxlePos].u8PointsGet;      
		
		//���ۺϵ÷�
		ScoreBuff[i] = a1 * StatusScore / 100 + a2 * EvaluateScore / 100 + a3 * TraceScore / 100;
	}
	memcpy(VehiclePerChannelForLoop->HideAxleScore, ScoreBuff, sizeof(uint8)*7);
}

/*********************************************************************************************************
** Function name:     FVehScore
** Descriptions:      Ϊ������
** input parameters:  ������Ϣ
** output parameters: ��������������,��8λ:���ذ�鳵����, ��8λ:��׼��鳵����
**
** Created by:	liumingxin
** Created Date:  2018-04-24  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint16 FVehScore(VehicleInfForLoop_Struct *VehInfo)
{
	VehicleInfForLoop_Struct *VehiclePerChannelForLoop;
	uint8  i = 0, j = 0;
	uint32 NormalAllWgt = 0, HideAllWgt = 0;
	uint8  AxleDistanceFlag = 0;             //0:�������ĳ���඼����,0x01:��׼�����಻����,0x10:���ذ����಻����,���������඼������(ֻ���������Ȳ���������鶼���쳣��ʻʱ�ж�)
	uint16 AllAxleDistance = 0;              //�к���������
	uint8  l_u8AxleWgtRatioTemp1 = 0, l_u8AxleWgtRatioTemp2 = 0;         //���ر�(����/����),�Ŵ�100��
	uint8  l_u8AxleWgtRatioDiff = 0;         //���ر�֮��
	uint8  NormalAccFlag = 0, HideAccFlag = 0;   //�Ӽ��ٱ�־, 0:û�мӼ���,1:�мӼ���
	uint8  NormalXieFlag = 0, HideXieFlag = 0;   //б�б�־,0:û��б��,1:��б��
	uint8  NormalRunAndWalk = 0, HideRunAndWalk = 0;     //����ͣͣ��־,0:û������ͣͣ,1:ֻ������ͣͣ
	uint8  NormalVehScore = 100, HideVehScore = 100;
	uint16 ret = 1;
	
	VehiclePerChannelForLoop = VehInfo;
	
	//�����������ĳ�������
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		NormalAllWgt += VehiclePerChannelForLoop->au16AxleWeight[i];
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		HideAllWgt += VehiclePerChannelForLoop->au16HideAxleWeight[i];
	}
	
	/**** �жϳ����Ƿ��мӼ��� ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		g_vehicleInfo[g_u8vehicleID].u16AxleList[i] = VehiclePerChannelForLoop->u16AxleList[i];
		for(j = 0; j < g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAccCnt; j++)
		{
			if((g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAcc[j] > AxleAccRange) || 
			   ((g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].RunAndWalkFlag == 0x02) && (4*g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAcc[j] > AxleAccRange*3)))
			{
				NormalAccFlag = 1;
				break;
			}
		}
		if(NormalAccFlag)
		{
			break;
		}
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		g_vehicleInfo[g_u8vehicleID].u16HideAxleList[i] = VehiclePerChannelForLoop->u16HideAxleList[i];
		for(j = 0; j < g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleAccCnt; j++)
		{
			if(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].AxleAcc[j] > AxleAccRange)
			{
				HideAccFlag = 1;
				break;
			}
			
		}
		if(HideAccFlag)
		{
			break;
		}
	}
	/**** end ****/
	
	/**** �жϳ����Ƿ���б�� ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		if(g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleStatus == XIEZOULEFT ||
		   g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].AxleStatus == XIEZOURIGHT)
		{
			NormalXieFlag = 1;
			break;
		}
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		if(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].AxleStatus == XIEZOULEFT ||
		   g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].AxleStatus == XIEZOURIGHT)
		{
			HideXieFlag = 1;
			break;
		}
	}
	/**** end ****/
	
	/**** �жϳ����Ƿ�������ͣͣ ****/
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		if(g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].RunAndWalkFlag)
		{
			NormalRunAndWalk = 1;
			break;
		}
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		if(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].RunAndWalkFlag)
		{
			HideRunAndWalk = 1;
			break;
		}
	}
	/**** end ****/
	
	
	/**** �ж�������������Ƿ���� ****/
	if((VehiclePerChannelForLoop->u8AixCnt != VehiclePerChannelForLoop->u8HideAixCnt) && (NormalXieFlag == 0) && (HideXieFlag == 0) &&
	   (((NormalRunAndWalk == 0) && (HideRunAndWalk == 0)) || (g_u8RunAndWalkEnable == 0)) )
	{
		switch(VehiclePerChannelForLoop->u8AixCnt)
		{
			case 2:
				if((NormalAllWgt > 10000) && (VehiclePerChannelForLoop->au16AxleDistance[0] < 300))
				{
					AxleDistanceFlag |= 0x01;
				}
				break;
			case 3:
				AllAxleDistance = VehiclePerChannelForLoop->au16AxleDistance[0] + VehiclePerChannelForLoop->au16AxleDistance[1];
				if(AllAxleDistance < 400)    
				{
					AxleDistanceFlag |= 0x01;
				}
				break;
			case 4:
				AllAxleDistance = VehiclePerChannelForLoop->au16AxleDistance[0] + VehiclePerChannelForLoop->au16AxleDistance[1] + VehiclePerChannelForLoop->au16AxleDistance[2];
				l_u8AxleWgtRatioTemp1 = (VehiclePerChannelForLoop->au16AxleWeight[2] * 100) / NormalAllWgt;
				l_u8AxleWgtRatioTemp2 = (VehiclePerChannelForLoop->au16AxleWeight[3] * 100) / NormalAllWgt;
				l_u8AxleWgtRatioDiff = GetDiff(l_u8AxleWgtRatioTemp1, l_u8AxleWgtRatioTemp2);
				if((AllAxleDistance < 500) || ((VehiclePerChannelForLoop->au16AxleDistance[1] > 160 || VehiclePerChannelForLoop->au16AxleDistance[1] < 100) && (VehiclePerChannelForLoop->au16AxleDistance[2] > 160 || VehiclePerChannelForLoop->au16AxleDistance[2] < 100) && l_u8AxleWgtRatioDiff > 10))
				{
					AxleDistanceFlag |= 0x01;
				}
				break;
			case 5:
				l_u8AxleWgtRatioTemp1 = (VehiclePerChannelForLoop->au16AxleWeight[3] * 100) / NormalAllWgt;
				l_u8AxleWgtRatioTemp2 = (VehiclePerChannelForLoop->au16AxleWeight[4] * 100) / NormalAllWgt;
				l_u8AxleWgtRatioDiff = GetDiff(l_u8AxleWgtRatioTemp1, l_u8AxleWgtRatioTemp2);
				if((VehiclePerChannelForLoop->au16AxleDistance[3] > 160 || VehiclePerChannelForLoop->au16AxleDistance[3] < 100) && l_u8AxleWgtRatioDiff > 10)
				{
					AxleDistanceFlag |= 0x01;
				}
				break;
			case 6:
				if((VehiclePerChannelForLoop->au16AxleDistance[3] < 160 && VehiclePerChannelForLoop->au16AxleDistance[3] > 100) && VehiclePerChannelForLoop->au16AxleDistance[4] > 160)    //4/5��������,5/6�᲻������
				{
					AxleDistanceFlag |= 0x01;
				}
				else if((VehiclePerChannelForLoop->au16AxleDistance[3] > 160 || VehiclePerChannelForLoop->au16AxleDistance[3] < 100) && (VehiclePerChannelForLoop->au16AxleDistance[2] > 160 || VehiclePerChannelForLoop->au16AxleDistance[2] < 100))
				{
					AxleDistanceFlag |= 0x01;
				}
				break;
			default:
				break;
		}
		switch(VehiclePerChannelForLoop->u8HideAixCnt)
		{
			case 2:
				if((HideAllWgt > 10000) && (VehiclePerChannelForLoop->au16HideAxleDistance[0] < 300))
				{
					AxleDistanceFlag |= 0x10;
				}
				break;
			case 3:
				AllAxleDistance = VehiclePerChannelForLoop->au16HideAxleDistance[0] + VehiclePerChannelForLoop->au16HideAxleDistance[1];
				if(AllAxleDistance < 400)
				{
					AxleDistanceFlag |= 0x10;
				}
				break;
			case 4:
				AllAxleDistance = VehiclePerChannelForLoop->au16HideAxleDistance[0] + VehiclePerChannelForLoop->au16HideAxleDistance[1] + VehiclePerChannelForLoop->au16HideAxleDistance[2];
				l_u8AxleWgtRatioTemp1 = (VehiclePerChannelForLoop->au16HideAxleWeight[2] * 100) / HideAllWgt;
				l_u8AxleWgtRatioTemp2 = (VehiclePerChannelForLoop->au16HideAxleWeight[3] * 100) / HideAllWgt;
				l_u8AxleWgtRatioDiff = GetDiff(l_u8AxleWgtRatioTemp1, l_u8AxleWgtRatioTemp2);
				if((AllAxleDistance < 380) || ((VehiclePerChannelForLoop->au16HideAxleDistance[1] > 160 || VehiclePerChannelForLoop->au16HideAxleDistance[1] < 100) && (VehiclePerChannelForLoop->au16HideAxleDistance[2] > 160 || VehiclePerChannelForLoop->au16HideAxleDistance[2] < 100) && l_u8AxleWgtRatioDiff > 10))
				{
					AxleDistanceFlag |= 0x10;
				}
				break;
			case 5:
				l_u8AxleWgtRatioTemp1 = (VehiclePerChannelForLoop->au16HideAxleWeight[3] * 100) / HideAllWgt;
				l_u8AxleWgtRatioTemp2 = (VehiclePerChannelForLoop->au16HideAxleWeight[4] * 100) / HideAllWgt;
				l_u8AxleWgtRatioDiff = GetDiff(l_u8AxleWgtRatioTemp1, l_u8AxleWgtRatioTemp2);
				if(VehiclePerChannelForLoop->au16HideAxleDistance[3] > 160 && l_u8AxleWgtRatioDiff > 10)
				{
					AxleDistanceFlag |= 0x10;
				}
				break;
			case 6:
				if((VehiclePerChannelForLoop->au16HideAxleDistance[3] < 160  && VehiclePerChannelForLoop->au16HideAxleDistance[3] > 100) && (VehiclePerChannelForLoop->au16HideAxleDistance[4] > 160 || VehiclePerChannelForLoop->au16HideAxleDistance[4] < 100))    //4/5��������,5/6�᲻������
				{
					AxleDistanceFlag |= 0x10;
				}
				else if((VehiclePerChannelForLoop->au16HideAxleDistance[3] > 160 || VehiclePerChannelForLoop->au16HideAxleDistance[3] < 100) && (VehiclePerChannelForLoop->au16HideAxleDistance[2] > 160 || VehiclePerChannelForLoop->au16HideAxleDistance[2] < 100))
				{
					AxleDistanceFlag |= 0x10;
				}
				break;
			default:
				break;
		}
	}
	/**** end ****/
	
	/**** ���������ĳ��������� ****/
	//�������Գ�����,����಻�����ֱ����Ϊ0��
	if((VehiclePerChannelForLoop->u8AixCnt < 2) || (VehiclePerChannelForLoop->u8AixCnt > 6))
	{
		NormalVehScore = 0;
	}
	if((VehiclePerChannelForLoop->u8HideAixCnt < 2) || (VehiclePerChannelForLoop->u8HideAixCnt > 6))
	{
		HideVehScore = 0;
	}
	if(AxleDistanceFlag & 0x0f)    //�к�����಻�����
	{
		NormalVehScore = 0;
	}
	if(AxleDistanceFlag & 0xf0)    //��������в������
	{
		HideVehScore = 0;
	}
	//�������ضԳ���������,�����ز������ֱ����Ϊ0��
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		if( (VehiclePerChannelForLoop->au16AxleWeight[i] < AxleWgtMinError) ||
		    (VehiclePerChannelForLoop->au16AxleWeight[i] > AxleWgtMaxError) )
		{
			NormalVehScore = 0;
			break;
		}
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		if(VehiclePerChannelForLoop->au16HideAxleWeight[i] < AxleWgtMinError ||
			VehiclePerChannelForLoop->au16HideAxleWeight[i] > AxleWgtMaxError)
		{
			HideVehScore = 0;
			break;
		}
	}
	
	if((VehiclePerChannelForLoop->u8AixCnt == 2 && NormalAllWgt > 25000) || 
	   (VehiclePerChannelForLoop->u8AixCnt > 2 && NormalAllWgt < 10000))
	{
		NormalVehScore = 0;
	}
	
	//������ʻ״̬�Գ���������
	if(NormalAccFlag)
	{
		NormalVehScore = (NormalVehScore >= 20)?(NormalVehScore - 20):0;
	}
	if(NormalXieFlag)
	{
		NormalVehScore = (NormalVehScore >= 40)?(NormalVehScore - 40):0;
	}
	if(NormalRunAndWalk)
	{
		NormalVehScore = (NormalVehScore >= 15)?(NormalVehScore - 15):0;
	}
	
	if(HideAccFlag)
	{
		HideVehScore = (HideVehScore >= 20)?(HideVehScore - 20):0;
	}
	if((VehiclePerChannelForLoop->m_n8XieZouFlag > 0))
	{
		HideVehScore = (HideVehScore >= 10)?(HideVehScore - 10):0;
	}
	if(VehiclePerChannelForLoop->RunAndWalkFlag == 0x06)
	{
		HideVehScore = (HideVehScore >= 30)?(HideVehScore - 30):0;     //һ������ͣͣ
	}
	else if((VehiclePerChannelForLoop->RunAndWalkFlag&0x06) == 0x02)
	{
		HideVehScore = (HideVehScore >= 10)?(HideVehScore - 10):0;     //һ��������ͣͣ,����������ͣͣ
	}

	
	//�������۶Գ���������
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & 0x01)
	{
		NormalVehScore = (NormalVehScore >= 5)?(NormalVehScore - 5):0;       //-5
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<1))
	{
		NormalVehScore = (NormalVehScore >= 7)?(NormalVehScore - 7):0;        //-7
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<2))
	{
		NormalVehScore = (NormalVehScore >= 100)?(NormalVehScore - 100):0;    //-100
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<3))
	{
		NormalVehScore = (NormalVehScore >= 50)?(NormalVehScore - 50):0;      //-50
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<4))
	{
		NormalVehScore = (NormalVehScore >= 15)?(NormalVehScore - 15):0;      //-15
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<5))
	{
		NormalVehScore = (NormalVehScore >= 15)?(NormalVehScore - 15):0;      //-15
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<6))
	{
		NormalVehScore = (NormalVehScore >= 100)?(NormalVehScore - 100):0;    //-100
	}
	if(VehiclePerChannelForLoop->m_u8NormalLogiclvl & (0x01<<7))
	{
		NormalVehScore = (NormalVehScore >= 50)?(NormalVehScore - 50):0;      //-50
	}
	
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & 0x01)
	{
		HideVehScore = (HideVehScore >= 5)?(HideVehScore - 5):0;       //-5
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<1))
	{
		HideVehScore = (HideVehScore >= 7)?(HideVehScore - 7):0;        //-7
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<2))
	{
		HideVehScore = (HideVehScore >= 100)?(HideVehScore - 100):0;    //-100
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<3))
	{
		HideVehScore = (HideVehScore >= 50)?(HideVehScore - 50):0;      //-50
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<4))
	{
		HideVehScore = (HideVehScore >= 15)?(HideVehScore - 15):0;      //-15
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<5))
	{
		HideVehScore = (HideVehScore >= 15)?(HideVehScore - 15):0;      //-15
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<6))
	{
		HideVehScore = (HideVehScore >= 100)?(HideVehScore - 100):0;    //-100
	}
	if(VehiclePerChannelForLoop->m_u8HideLogiclvl & (0x01<<7))
	{
		HideVehScore = (HideVehScore >= 50)?(HideVehScore - 50):0;      //-50
	}
	/**** end ****/

	ret = (HideVehScore<<8) | NormalVehScore;
	return ret;
}

/*********************************************************************************************************
** Function name:     FUnPostVeh
** Descriptions:      ǿ�Ʋ��������
** input parameters:  ������Ϣ,���������۷���
** output parameters: 0:�����׳�,����60:���׳����
**
** Created by:	liumingxin
** Created Date:  2018-07-03
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FUnPostVeh(VehicleInfForLoop_Struct *VehInfo)
{
	VehicleInfForLoop_Struct *VehiclePerChannelForLoop;
	uint8 ret = 0;    //0:��������,����60:ǿ�Ʋ��������(Ĭ��Ϊ0)
	uint8 a1 = 70, a2 = 30;    //Ȩֵ
	uint8 NormalScoreTemp = 0, HideScoreTemp = 0;
	
	uint8  i;
	uint32 NormalBlankAllWgt = 0, HideBlankAllWgt = 0;
	uint16 AllAxleDistance = 0;              //�к���������
	uint16 AllHideAxleDistance = 0;          //���ذ�������
	uint8  AxleSpdErrorFlag = 0;             //bit0:��׼��������쳣,bit1:���ذ�������쳣, ��1Ϊ�쳣,Ĭ��Ϊ0
	uint8  AxleScoreErrorFlag = 0;           //bit0:��׼���������쳣,bit1:���ذ��������쳣, ��1Ϊ�쳣,Ĭ��Ϊ0

	uint8  NormalAxleScoreBuff[7], HideAxleScoreBuff[7];
	uint16 NormalAcc = 0, HideAcc = 0;
	uint16 AxleSpdThreshold = 200;        //��λ0.1km/h
	uint16 AxleDiffSpdThreshold = 200;    //��λ0.1km/h
	uint16 AxleAccThreshold = 4;          //��λm/s^2
	 
	VehiclePerChannelForLoop = VehInfo;
	
	//����������鳵������
	for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
	{
		NormalBlankAllWgt += VehiclePerChannelForLoop->au16AxleWeight[i];
	}
	for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
	{
		HideBlankAllWgt += VehiclePerChannelForLoop->au16HideAxleWeight[i];
	}
	
	if(VehiclePerChannelForLoop->u8AixCnt == VehiclePerChannelForLoop->u8HideAixCnt)
	{
		memcpy(NormalAxleScoreBuff, VehiclePerChannelForLoop->NormalAxleScore, sizeof(uint8)*7);
		memcpy(HideAxleScoreBuff, VehiclePerChannelForLoop->HideAxleScore, sizeof(uint8)*7);

		//��ͬ�����ĳ�������೬����ֵ�򲻳���
		switch(VehiclePerChannelForLoop->u8AixCnt)
		{
			case 2:
				AllAxleDistance = VehiclePerChannelForLoop->au16AxleDistance[0];
				AllHideAxleDistance = VehiclePerChannelForLoop->au16HideAxleDistance[0];
				if((AllAxleDistance > 800) && (AllHideAxleDistance > 800))
				{
					ret = 61;
				}
				break;
			case 3:
				for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
				{
					AllAxleDistance += VehiclePerChannelForLoop->au16AxleDistance[i-1];
					AllHideAxleDistance += VehiclePerChannelForLoop->au16HideAxleDistance[i-1];
				}
				if((AllAxleDistance < 400) && (AllHideAxleDistance < 400))
				{
					ret = 62;
				}
				break;
			case 4:
				for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
				{
					AllAxleDistance += VehiclePerChannelForLoop->au16AxleDistance[i-1];
					AllHideAxleDistance += VehiclePerChannelForLoop->au16HideAxleDistance[i-1];
				}
				if((AllAxleDistance < 500) && (AllHideAxleDistance < 500))
				{
					ret = 63;
				}
				break;
			case 5:
				for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
				{
					AllAxleDistance += VehiclePerChannelForLoop->au16AxleDistance[i-1];
					AllHideAxleDistance += VehiclePerChannelForLoop->au16HideAxleDistance[i-1];
				}
				if((AllAxleDistance < 800) && (AllHideAxleDistance < 800))
				{
					ret = 64;
				}
				break;
			case 6:
				for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
				{
					AllAxleDistance += VehiclePerChannelForLoop->au16AxleDistance[i-1];
					AllHideAxleDistance += VehiclePerChannelForLoop->au16HideAxleDistance[i-1];
				}
				if((AllAxleDistance < 900) && (AllHideAxleDistance < 900))
				{
					ret = 65;
				}
				break;
			default:
				break;
		}
		
		if((VehiclePerChannelForLoop->u8AixCnt < 2 || VehiclePerChannelForLoop->u8AixCnt > 6) && 
		   (VehiclePerChannelForLoop->u8HideAixCnt < 2 || VehiclePerChannelForLoop->u8HideAixCnt > 6))    //�����쳣
		{
			ret = 67;
		}
		
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			NormalScoreTemp = a1 * g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u8PointsGet / 100 + a2 * NormalAxleScoreBuff[i] / 100;
			HideScoreTemp = a1 * g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].u8PointsGet / 100 + a2 * HideAxleScoreBuff[i] / 100;
			if(NormalScoreTemp < g_u8MinAxleScore && HideScoreTemp < g_u8MinAxleScore)    //�������������÷ֵ�����ֵ
			{
				ret = 68;
				break;
			}
		}
		for(i = 0; i < (VehiclePerChannelForLoop->u8AixCnt>1?(VehiclePerChannelForLoop->u8AixCnt-1):0); i++)
		{
			//����������ļ��ٶ�
			NormalAcc = (GetDiff(g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16speed, g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i+1]].u16speed) / 36) * 6000 \
						/ GetDiff(g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16LeftWheeList[0]].u32uptm, g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i+1]].u16LeftWheeList[0]].u32uptm);
			HideAcc = (GetDiff(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].u16speed, g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i+1]].u16speed) / 36) * 6000 \
						/ GetDiff(g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16ChList[0]].u32uptm, g_ChlData[g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i+1]].u16ChList[0]].u32uptm);
			if((g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16speed > AxleSpdThreshold) && 
			   ((GetDiff(g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i]].u16speed, g_AxleInfo[VehiclePerChannelForLoop->u16AxleList[i+1]].u16speed) > AxleDiffSpdThreshold) ||
			   (NormalAcc > AxleAccThreshold)))
			{
				AxleSpdErrorFlag |= 0x01;
			}
			if((g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].u16speed > AxleSpdThreshold) && 
			  ((GetDiff(g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i]].u16speed, g_AxleInfo[VehiclePerChannelForLoop->u16HideAxleList[i+1]].u16speed) > AxleDiffSpdThreshold) ||
			  (HideAcc > AxleAccThreshold)))
			{
				AxleSpdErrorFlag |= 0x02;
			}
			if(AxleSpdErrorFlag & 0x03)
			{
				ret = 69;
				break;
			}
		}
	}
	else
	{
		if(VehiclePerChannelForLoop->u8HideAixCnt == 0)
		{
			if(NormalBlankAllWgt > 4000)         //����������ش���4t�������ذ������,����Ϊ�Ƿ��ᵼ�µ�,������
			{
				ret = 75;
			}
			if(VehiclePerChannelForLoop->u8AixCnt < 2)
			{
				ret = 76;
			}
		}
		for(i = 0; i < VehiclePerChannelForLoop->u8AixCnt; i++)
		{
			if(NormalAxleScoreBuff[i] < g_u8MinAxleScore)
			{
				AxleScoreErrorFlag |= 0x01;
				break;
			}
		}
		for(i = 0; i < VehiclePerChannelForLoop->u8HideAixCnt; i++)
		{
			if(HideAxleScoreBuff[i] < g_u8MinAxleScore)
			{
				AxleScoreErrorFlag |= 0x02;
				break;
			}
		}
		if(AxleScoreErrorFlag & 0x03)    //ÿ����鶼����÷ֵ�����ֵ
		{
			//ret = 70;
		}
	}

	
	
	
	return ret;
}




/*********************************************************************************************************
** Function name:     FMatchAxleAndAxle
** Descriptions:      ������ƥ��
** input parameters:   
** output parameters: 
**
** Created by:	zhangtonghan  
** Created Date:  2017-06-06	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void	FMatchAxleAndAxle(void *Address[5])
{
	Axle_Struct *AxleForChannel;
	int8 *Dir;
	uint8 l_u8Err;
	uint16 l_u16SendLen;

	VehicleInfForLoop_Struct (*VehCommStruct)[CHL_SAVEVEH]; //ͳһ�ĳ�����Ϣ�ṹ��
	uint8 l_u8ChlNum;//�����к�
	uint8 l_u8tempi,l_tempos;
	void * AxleAddressed[3];
	void * l_u32AddressForPo[5];
	void * l_u32AddrAxleDist[5];
//	uint8  l_u8HeadOrTail=0; //��ͷΪ0,��βΪ1
//	uint32 l_u32TimeNow;
 	uint16 AxleDistence=0;
	uint32 l_u32AxleAccTim=0;
	uint16 AxleXMidPos_Cur = 0, AxleXMidPos_Match = 0;
	uint16 AxleXLeftPos_Match = 0, AxleXRightPos_Match = 0;
	uint8  AxleStatus_Cur = 0, AxleStatus_Match = 0;
	uint8  AxleMatchFlag = 0;        //������ƥ���־,0:ƥ��ʧ��,!0:ƥ��ɹ�
	uint8  MatchLane = 0;    //ƥ����
	static uint8 cnt = 0;
	
	AxleForChannel 	= Address[0];
// 	p_u8ChlNum		= Address[1];
 	Dir 			= Address[2];
	VehCommStruct 	= Address[3];
	
	if(AxleForChannel->ZTLayouType == HIDELAYOUT) //һ�ֲ���
	{
		switch(AxleForChannel->u8ValidChlCnt)
		{
			case 0x01:
				AxleForChannel->u8ChlNum = AxleForChannel->AxleChlPos[0];
			break;
			case 0x02:
				if((*Dir) == 1)
					AxleForChannel->u8ChlNum = IsLit(AxleForChannel->AxleChlPos[0],AxleForChannel->AxleChlPos[1]);
				else
					AxleForChannel->u8ChlNum = IsBig(AxleForChannel->AxleChlPos[0],AxleForChannel->AxleChlPos[1]);
			break;
			case 0x03:
				AxleForChannel->u8ChlNum = TheMidOne(AxleForChannel->AxleChlPos[0],AxleForChannel->AxleChlPos[1],AxleForChannel->AxleChlPos[2]);
			break;
			default:
			break;
		}
	}
	l_u8ChlNum		= AxleForChannel->u8ChlNum;
	
	/**** ���Դ��� ****/
	if(l_u8ChlNum == 1 && AxleForChannel->ZTLayouType == JCLAYOUT)
	{
		cnt++;
		if(cnt == 3)
		{
			cnt = 3;
		}
	}
	/**** end ****/
	if(VehCommStruct[AxleForChannel->u8ChlNum][0].m_u8VehicleHaveFlag == 1)    //��ǰ���г�
	{
// 		if((AxleForChannel->AxleWhelCnt == 2) || ((AxleForChannel->AxleWhelCnt == 1) && (AxleForChannel->u16LeftWheelMaxValueAver > MAXVALUETHRED)))  //��ǰ��Ϊ�����ֻ�1���ֵ����ֵ�Ƚϴ�
		{
			if(AxleForChannel->ZTLayouType == HIDELAYOUT)
				AxleForChannel->ZTLayouType = HIDELAYOUT;
			
			l_u32AddrAxleDist[0] = AxleForChannel;
			if(AxleForChannel->ZTLayouType == JCLAYOUT)    //������
				l_u32AddrAxleDist[1] = &g_AxleInfo[VehCommStruct[AxleForChannel->u8ChlNum][0].u16AxleList[VehCommStruct[AxleForChannel->u8ChlNum][0].m_n8CurrentAxleNum]];
			else      //���ذ��
				l_u32AddrAxleDist[1] = &g_AxleInfo[VehCommStruct[AxleForChannel->u8ChlNum][0].u16HideAxleList[VehCommStruct[AxleForChannel->u8ChlNum][0].m_n8CurrentAxleNum]];			
			l_u32AddrAxleDist[2] = &(AxleForChannel->ZTLayouType);
			l_u32AddrAxleDist[3] = &AxleDistence;
			l_u32AddrAxleDist[4] = &l_u32AxleAccTim;
			SelectZT4AxleDisCalUPdate(l_u32AddrAxleDist);

			if(AxleDistence > 1400)    //��೬��14mǿ���׳�
			{
				l_u32AddressForPo[0]= &l_u8ChlNum;
				l_u32AddressForPo[1]= Dir;
				l_u32AddressForPo[2]= &VehCommStruct[l_u8ChlNum][0];

				FPostVehForLoop(l_u32AddressForPo,2);//�׳�
			}				
			VehCommStruct[l_u8ChlNum][0].m_u8VehicleHaveFlag = 1;
			VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum++;
			VehCommStruct[l_u8ChlNum][0].AxleStatus[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum]=AxleForChannel->AxleStatus;
			VehCommStruct[l_u8ChlNum][0].m_u8DireFlag = *Dir;
			
			if(AxleForChannel->ZTLayouType == JCLAYOUT)  //����
			{				
				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->LeftWheelZTCnt; l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi]=AxleForChannel->u16LeftWheeList[l_u8tempi];							
				}

				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->RightWheelZTCnt;l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][AxleForChannel->LeftWheelZTCnt+l_u8tempi]=AxleForChannel->u16RightWheeList[l_u8tempi];
				}
			}
			else   //һ�ֲ���
			{
				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->u16ChListSum; l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16HideCurAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi]=AxleForChannel->u16ChList[l_u8tempi];							
				}
			}
		
//			if((VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum == 0) && ((AxleForChannel->ZTLayouType == 0)))
//			{
//				ReadFPGAGlobalTime(l_u32TimeNow);
//				VehCommStruct[l_u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
//				if(*Dir == 1)
//					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f));
//				else
//					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f));
//				
//				l_u32AddrFcap[0] = &(VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum);
//				l_u32AddrFcap[1] = &(l_u32TimeNow);
//				l_u32AddrFcap[2] = Dir;
//				l_u32AddrFcap[3] = &l_u8HeadOrTail;
//				
//				FCapture(l_u32AddrFcap,1);
//				FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
//			}
			AxleAddressed[0] = &VehCommStruct[l_u8ChlNum][0];
			AxleAddressed[1] = &l_u8ChlNum;
			AxleAddressed[2] = Dir;
			FProPostAxle(AxleAddressed,1);//����			
			
		}
// 		else  //��ǰ��Ϊ���ֳ�
// 		{
// 			l_u32AddressForPo[0]= &l_u8ChlNum;
// 			l_u32AddressForPo[1]= Dir;
// 			l_u32AddressForPo[2]= &VehCommStruct[l_u8ChlNum][0];

// 			FPostVehForLoop(l_u32AddressForPo,3);//�׳�
// 			
// 			//�½���
// 			VehCommStruct[l_u8ChlNum][0].m_u8VehicleHaveFlag = 1;
// 			VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum = 0;
// 			VehCommStruct[l_u8ChlNum][0].AxleStatus[0]=AxleForChannel->AxleStatus;
// 			VehCommStruct[l_u8ChlNum][0].m_u8DireFlag = *Dir;
// 			if(AxleForChannel->ZTLayouType == JCLAYOUT)  //����
// 			{
// 				
// 				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->LeftWheelZTCnt; l_u8tempi++)
// 				{
// 					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[0][l_u8tempi]=AxleForChannel->u16LeftWheeList[l_u8tempi];							
// 				}

// 				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->RightWheelZTCnt;l_u8tempi++)
// 				{
// 					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[0][AxleForChannel->LeftWheelZTCnt+l_u8tempi]=AxleForChannel->u16RightWheeList[l_u8tempi];
// 				}
// 			}
// 			else   //һ�ֲ���
// 			{
// 				l_tempos = F_FindEnd8(AxleForChannel->u16ChList,MAXNUM_AXLE);
// 				for(l_u8tempi = 0;l_u8tempi < l_tempos; l_u8tempi++)
// 				{
// 					VehCommStruct[l_u8ChlNum][0].m_an16HideCurAxleInf[0][l_u8tempi]=AxleForChannel->u16ChList[l_u8tempi];							
// 				}
// 			}
// 			
// //			if(AxleForChannel->ZTLayouType == 0)
// //			{
// //				ReadFPGAGlobalTime(l_u32TimeNow);
// //				VehCommStruct[l_u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
// //				if(*Dir == 1)
// //					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f));
// //				else
// //					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum]& 0x0f));
// //				
// //				l_u32AddrFcap[0] = &(VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum);
// //				l_u32AddrFcap[1] = &(l_u32TimeNow);
// //				l_u32AddrFcap[2] = Dir;
// //				l_u32AddrFcap[3] = &l_u8HeadOrTail;
// //				
// //				FCapture(l_u32AddrFcap,1);
// //				FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
// //			}
// 			AxleAddressed[0] = &VehCommStruct[l_u8ChlNum][0];
// 			AxleAddressed[1] = &l_u8ChlNum;
// 			AxleAddressed[2] = Dir;
// 			FProPostAxle(AxleAddressed, 4);//����
// 		}
	}
	else   //��ǰ��û����,Ѱ��+-1,+-2���Ƿ��г�,����������ʻ״̬�ж��Ƿ������ƥ��
	{
		if((l_u8ChlNum < 2*PROMODPARAM.m_u8LaneNum-1) && ((VehCommStruct[AxleForChannel->u8ChlNum+1][0].m_u8VehicleHaveFlag == 1)))    //+1��
		{
			MatchLane = AxleForChannel->u8ChlNum + 1;
			if(AxleForChannel->ZTLayouType == JCLAYOUT)    //������
			{
				if(AxleForChannel->AxleWhelCnt == 2)  //��ǰ��Ϊ������
				{
					AxleXMidPos_Cur = AxleForChannel->MidXPos;
					AxleXMidPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].MidXPos;
					if(GetDiff(AxleXMidPos_Cur, AxleXMidPos_Match) < AXLEMIDDISTANCE)    //���������ľ��λ�ò�С����ֵ����Ϊ������������ͬһ����
					{
						AxleMatchFlag = 5;
					}
					else   //���������ļ��ϴ�ʱ,ֻ�������ᶼΪб��ʱ�Ž���ƥ��
					{
						AxleStatus_Cur = AxleForChannel->AxleStatus;
						AxleStatus_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].AxleStatus;
						if((AxleStatus_Cur == AxleStatus_Match) && (AxleStatus_Cur == XIEZOULEFT || AxleStatus_Cur == XIEZOURIGHT))
						{
							AxleMatchFlag = 6;
						}
					}
				}
				else    //����
				{
					AxleXMidPos_Cur = AxleForChannel->MidXPos;
					AxleXLeftPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].LeftWheelAverPos;
					AxleXRightPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].RightWheelAverPos;
					if((AxleXLeftPos_Match > 0 && GetDiff(AxleXMidPos_Cur, AxleXLeftPos_Match) < AXLEMIDDISTANCE) || 
					   (AxleXRightPos_Match > 0 && GetDiff(AxleXMidPos_Cur, AxleXRightPos_Match) < AXLEMIDDISTANCE))
					{
						AxleMatchFlag = 7;
					}
				}
			}
			else    //���ز���
			{
				AxleMatchFlag = 8;
			}
		}
		if((l_u8ChlNum > 0) && (VehCommStruct[AxleForChannel->u8ChlNum-1][0].m_u8VehicleHaveFlag == 1))    //-1��
		{
			MatchLane = AxleForChannel->u8ChlNum - 1;
			if(AxleForChannel->ZTLayouType == JCLAYOUT)    //������
			{
				if(AxleForChannel->AxleWhelCnt == 2)  //��ǰ��Ϊ������
				{
					AxleXMidPos_Cur = AxleForChannel->MidXPos;
					AxleXMidPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].MidXPos;
					if(GetDiff(AxleXMidPos_Cur, AxleXMidPos_Match) < AXLEMIDDISTANCE)    //���������ľ��λ�ò�С����ֵ����Ϊ������������ͬһ����
					{
						AxleMatchFlag = 9;
					}
					else   //���������ļ��ϴ�ʱ,ֻ�������ᶼΪб��ʱ�Ž���ƥ��
					{
						AxleStatus_Cur = AxleForChannel->AxleStatus;
						AxleStatus_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].AxleStatus;
						if((AxleStatus_Cur == AxleStatus_Match) && (AxleStatus_Cur == XIEZOULEFT || AxleStatus_Cur == XIEZOURIGHT))
						{
							AxleMatchFlag = 10;
						}
					}
				}
				else    //����
				{
					AxleXMidPos_Cur = AxleForChannel->MidXPos;
					AxleXLeftPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].LeftWheelAverPos;
					AxleXRightPos_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].RightWheelAverPos;
					if((AxleXLeftPos_Match > 0 && GetDiff(AxleXMidPos_Cur, AxleXLeftPos_Match) < AXLEMIDDISTANCE) || 
					   (AxleXRightPos_Match > 0 && GetDiff(AxleXMidPos_Cur, AxleXRightPos_Match) < AXLEMIDDISTANCE))
					{
						AxleMatchFlag = 11;
					}
				}
			}
			else    //���ز���
			{
				AxleMatchFlag = 12;
			}
		}
		else if((l_u8ChlNum < 2*PROMODPARAM.m_u8LaneNum-2) && (VehCommStruct[AxleForChannel->u8ChlNum+2][0].m_u8VehicleHaveFlag == 1))    //+2��
		{
			MatchLane = AxleForChannel->u8ChlNum + 2;
			if(AxleForChannel->ZTLayouType == JCLAYOUT)    //������
			{
				AxleStatus_Cur = AxleForChannel->AxleStatus;
				AxleStatus_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].AxleStatus;
				if((AxleStatus_Cur == AxleStatus_Match) && ((AxleStatus_Cur == XIEZOULEFT) || (AxleStatus_Cur == XIEZOURIGHT)))
				{
					if(AxleForChannel->AxleWhelCnt == 2)  //��ǰ��Ϊ������
						AxleMatchFlag = 13;
				}
			}
			else    //���ز���
			{
				if( (((AxleForChannel->AxleStatus >= 3)&&(*Dir == 1)) || (((AxleForChannel->AxleStatus ==3)||(AxleForChannel->AxleStatus ==1))&&(*Dir == -1))) &&
					(VehCommStruct[MatchLane][0].AxleStatus[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum] != 0) )
				{
					AxleMatchFlag = 14;
				}
			}
		}
		else if((l_u8ChlNum > 1) && (VehCommStruct[AxleForChannel->u8ChlNum-2][0].m_u8VehicleHaveFlag == 1))    //+2��
		{
			MatchLane = AxleForChannel->u8ChlNum - 2;
			if(AxleForChannel->ZTLayouType == JCLAYOUT)    //������
			{
				AxleStatus_Cur = AxleForChannel->AxleStatus;
				AxleStatus_Match = g_AxleInfo[VehCommStruct[MatchLane][0].u16AxleList[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum]].AxleStatus;
				if((AxleStatus_Cur == AxleStatus_Match) && ((AxleStatus_Cur == XIEZOULEFT) || (AxleStatus_Cur == XIEZOURIGHT)))
				{
					if(AxleForChannel->AxleWhelCnt == 2)  //��ǰ��Ϊ������
						AxleMatchFlag = 15;
				}
			}
			else    //���ز���
			{
				if( (((AxleForChannel->AxleStatus >= 3) && (*Dir == 1)) || (((AxleForChannel->AxleStatus ==3) || (AxleForChannel->AxleStatus ==1)) && (*Dir == -1))) &&
					(VehCommStruct[MatchLane][0].AxleStatus[VehCommStruct[MatchLane][0].m_n8CurrentAxleNum] != 0) )
				{
					AxleMatchFlag = 16;
				}
			}
		}
		
		if(AxleMatchFlag > 0)    //������ƥ��ɹ�
		{
			memcpy(&VehCommStruct[l_u8ChlNum][0], &VehCommStruct[MatchLane][0], sizeof(VehicleInfForLoop_Struct));
			FInitVehStr(&VehCommStruct[MatchLane][0], *Dir);
			
			VehCommStruct[l_u8ChlNum][0].m_u8VehicleHaveFlag = 1;
			VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum++;
			VehCommStruct[l_u8ChlNum][0].AxleStatus[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum] = AxleForChannel->AxleStatus;
			VehCommStruct[l_u8ChlNum][0].m_u8DireFlag = *Dir;
			
			if(AxleForChannel->ZTLayouType == JCLAYOUT)  //����
			{				
				for(l_u8tempi = 0;l_u8tempi < AxleForChannel->LeftWheelZTCnt; l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi] = AxleForChannel->u16LeftWheeList[l_u8tempi];							
				}

				for(l_u8tempi = 0; l_u8tempi < AxleForChannel->RightWheelZTCnt; l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][AxleForChannel->LeftWheelZTCnt+l_u8tempi] = AxleForChannel->u16RightWheeList[l_u8tempi];
				}
			}
			else   //һ�ֲ���
			{
				l_tempos = F_FindEnd8(AxleForChannel->u16ChList, MAXNUM_AXLE);
				for(l_u8tempi = 0; l_u8tempi < l_tempos; l_u8tempi++)
				{
					VehCommStruct[l_u8ChlNum][0].m_an16HideCurAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi] = AxleForChannel->u16ChList[l_u8tempi];							
				}
			}
		
//			if((VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum == 0) && (AxleForChannel->ZTLayouType == JCLAYOUT))
//			{
//				ReadFPGAGlobalTime(l_u32TimeNow);
//				VehCommStruct[l_u8ChlNum][0].m_u32LoopTime = l_u32TimeNow;
//				if(*Dir == 1)
//					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum] & 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum] & 0x0f));
//				else
//					VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum] & 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[AxleForChannel->u8ChlNum] & 0x0f));
//				
//				l_u32AddrFcap[0] = &(VehCommStruct[l_u8ChlNum][0].m_Capture_LaneNum);
//				l_u32AddrFcap[1] = &(l_u32TimeNow);
//				l_u32AddrFcap[2] = Dir;
//				l_u32AddrFcap[3] = &l_u8HeadOrTail;
//				
//				FCapture(l_u32AddrFcap, 1);
//				FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
//			}
			AxleAddressed[0] = &VehCommStruct[l_u8ChlNum][0];
			AxleAddressed[1] = &l_u8ChlNum;
			AxleAddressed[2] = Dir;
			FProPostAxle(AxleAddressed, AxleMatchFlag);//����
		}
		else    //��ƥ��ʧ��,˫���ڵ�ǰ���½���,������ɾ������
		{
			if(AxleForChannel->AxleWhelCnt == 2)  //��ǰ��Ϊ������
			{
				FInitVehStr(&VehCommStruct[l_u8ChlNum][0], *Dir);
				VehCommStruct[l_u8ChlNum][0].m_u8VehicleHaveFlag = 1;
				VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum++;
				VehCommStruct[l_u8ChlNum][0].AxleStatus[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum]=AxleForChannel->AxleStatus;
				VehCommStruct[l_u8ChlNum][0].m_u8DireFlag = *Dir;
				if(AxleForChannel->ZTLayouType == JCLAYOUT)  //����
				{				
					for(l_u8tempi = 0;l_u8tempi < AxleForChannel->LeftWheelZTCnt; l_u8tempi++)
					{
						VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi]=AxleForChannel->u16LeftWheeList[l_u8tempi];							
					}

					for(l_u8tempi = 0;l_u8tempi < AxleForChannel->RightWheelZTCnt;l_u8tempi++)
					{
						VehCommStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][AxleForChannel->LeftWheelZTCnt+l_u8tempi]=AxleForChannel->u16RightWheeList[l_u8tempi];
					}
				}
				else   //һ�ֲ���
				{
					l_tempos = F_FindEnd8(AxleForChannel->u16ChList,MAXNUM_AXLE);
					for(l_u8tempi = 0;l_u8tempi < l_tempos; l_u8tempi++)
					{
						VehCommStruct[l_u8ChlNum][0].m_an16HideCurAxleInf[VehCommStruct[l_u8ChlNum][0].m_n8CurrentAxleNum][l_u8tempi]=AxleForChannel->u16ChList[l_u8tempi];							
					}
				}

				AxleAddressed[0] = &VehCommStruct[l_u8ChlNum][0];
				AxleAddressed[1] = &l_u8ChlNum;
				AxleAddressed[2] = Dir;
				FProPostAxle(AxleAddressed, AxleMatchFlag);//����
			}
			else
			{
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nClrSglW D=%d,L=%d,n=%d,[%d,%d,%d,%d,%d,%d],[%d,%d,%d,%d,%d,%d].",
										    *Dir, l_u8ChlNum, AxleForChannel->LeftWheelZTCnt, 
											AxleForChannel->u8LeftWheelTakeZT[0], AxleForChannel->u8LeftWheelTakeZT[1], AxleForChannel->u8LeftWheelTakeZT[2],
											AxleForChannel->u8LeftWheelTakeZT[3], AxleForChannel->u8LeftWheelTakeZT[4], AxleForChannel->u8LeftWheelTakeZT[5],
											AxleForChannel->u16LeftWheeList[0], AxleForChannel->u16LeftWheeList[1], AxleForChannel->u16LeftWheeList[2],
											AxleForChannel->u16LeftWheeList[3], AxleForChannel->u16LeftWheeList[4], AxleForChannel->u16LeftWheeList[5]);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				memset(AxleForChannel, 0, sizeof(Axle_Struct));
			}
		}
	}
}
/*********************************************************************************************************
** Function name:     FProPostAxle
** Descriptions:      ����
** input parameters:  ��ţ������ţ����ţ���������        
** output parameters: none
**
** Created by: zhangtonghan			  
** Created Date: 2017-06-07  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FProPostAxle(void *Address[3],uint8 num)
{
	int8 l_n8ArrayPos;
	uint8 l_ret;
	uint8 l_u8temp;
	int8 AxleNum;
	uint8 *ChlNum1;
	int8	*VehDir;
	VehicleInfForLoop_Struct *VehiclePerChannelForLoop;
	
	void*  l_u32AddrFcap[4];
	uint8  l_u8HeadOrTail=0; //��ͷΪ0,��βΪ1
	
	uint16  l_u16SendLen;
	uint8   l_u8Err;
	uint8 	Num,ChlNum;
	uint32 l_u32TimeNow;
	VehiclePerChannelForLoop = Address[0];
	ChlNum1 = Address[1];
	VehDir = Address[2];
	
	AxleNum = VehiclePerChannelForLoop->m_n8CurrentAxleNum;
	ChlNum = *ChlNum1;
	Num = num;
	///////
	
	///////////����ץ��//////////////////	
	if((AxleNum == 0) && (g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType == 0))
		{
			ReadFPGAGlobalTime(l_u32TimeNow);
			VehiclePerChannelForLoop->m_u32LoopTime=l_u32TimeNow;
			if(*VehDir == 1)
				VehiclePerChannelForLoop->m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[ChlNum]& 0x0f));
			else
				VehiclePerChannelForLoop->m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[ChlNum]& 0x0f));
			l_u8HeadOrTail=0; //��ͷΪ0,��βΪ1
			l_u32AddrFcap[0] = &(VehiclePerChannelForLoop->m_Capture_LaneNum);
			l_u32AddrFcap[1] = &(l_u32TimeNow);
			l_u32AddrFcap[2] = VehDir;
			l_u32AddrFcap[3] = &l_u8HeadOrTail;
			
			FCapture(l_u32AddrFcap,1);
			FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
			VehiclePerChannelForLoop->m_u32LoopTime=l_u32TimeNow;
		}
		
		///////////�ڶ���β��//////////////////	
	if((AxleNum == 1) && (g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType == 0))
		{
			l_u8HeadOrTail=1; //��ͷΪ0,��βΪ1
			l_u32AddrFcap[0] = &(VehiclePerChannelForLoop->m_Capture_LaneNum);
			l_u32AddrFcap[1] = &VehiclePerChannelForLoop->m_u32LoopTime;
			l_u32AddrFcap[2] = VehDir;
			l_u32AddrFcap[3] = &l_u8HeadOrTail;
			
			FCapture(l_u32AddrFcap,2);
// 			FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
		}
	
	
	ReadFPGAGlobalTime(l_u32TimeNow);
	
	//������ṹ�帳ֵ
	g_AxleInfo[g_u16AxleInfoWriteIndex].u8id = g_u16AxleInfoWriteIndex;
	g_AxleInfo[g_u16AxleInfoWriteIndex].u8OrderID = AxleNum+1;
	g_AxleInfo[g_u16AxleInfoWriteIndex].u8ChlNum = ChlNum;
	g_AxleInfo[g_u16AxleInfoWriteIndex].Dir = *VehDir;
	g_AxleInfo[g_u16AxleInfoWriteIndex].RunAndWalkFlag = 0;
	VehiclePerChannelForLoop->m_au8AxleDropFlag[AxleNum] = 1;
	VehiclePerChannelForLoop->m_u8CrossHide = g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType;
	if(JCLAYOUT == g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType)
	{
		VehiclePerChannelForLoop->u16AxleList[AxleNum]= g_u16AxleInfoWriteIndex;
		VehiclePerChannelForLoop->u8AixCnt++;
	}
	else
	{
		VehiclePerChannelForLoop->u16HideAxleList[AxleNum]= g_u16AxleInfoWriteIndex;
		VehiclePerChannelForLoop->u8HideAixCnt++;
	}

	if(g_AxleInfo[g_u16AxleInfoWriteIndex].u8OrderID == 0)
		return;

	l_n8ArrayPos = F_FindVehEnd(VehiclePerChannelForLoop->m_an16CurrentAxleInf[AxleNum], MAXNUM_AXLE);
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPostAxle,Dr=%d,L=%x,N=%d,Z=%d,St=%d,P=%d,%d,pos[%d,%d].",
					           *VehDir,*ChlNum1,AxleNum,l_n8ArrayPos+1,g_AxleInfo[g_u16AxleInfoWriteIndex].AxleStatus,Num,l_u32TimeNow,
											g_AxleInfo[g_u16AxleInfoWriteIndex].HideLeftWhePos,g_AxleInfo[g_u16AxleInfoWriteIndex].HideRightWhePos);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
	
	
	l_ret = OSQPost(g_pQAxleCalc, &g_AxleInfo[g_u16AxleInfoWriteIndex]);
	if( l_ret == OS_Q_FULL)
	{
		g_ErrMsgState |= 0x01; 	
	}
	
	
	
	g_u16AxleInfoWriteIndex++;
	g_u16AxleInfoWriteIndex %= AXLE_BUFLEN;
	memset(&g_AxleInfo[g_u16AxleInfoWriteIndex],0,sizeof(Axle_Struct));

	for(l_u8temp=0;l_u8temp<12;l_u8temp++)
		memset(&g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChList[l_u8temp],0xFFFF,sizeof(uint16));
	g_AxleInfo[g_u16AxleInfoWriteIndex].u8PointsGet = 100;
}

/*********************************************************************************************************
** Function name:     F_FindEnd8
** Descriptions:      ������������ЧԪ�صĸ���
** input parameters:  ���顢���鳤��        
** output parameters: ������ʵ��Ԫ�ظ���
**
** Created by: wgh		  
** Created Date: 2014-5-6	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 F_FindEnd8(uint16 lp_nArray[],uint8 cnArrayLength){
	static uint8 nArrayEnd;
	uint8	i,nArrayLength;
	uint16 *ap_nArray;
	ap_nArray = lp_nArray;
	nArrayLength = cnArrayLength;
	nArrayEnd = 0;
	for( i=0;i<nArrayLength;i++){
		if(0xFFFF==ap_nArray[i]){
			nArrayEnd=i;
			break;
		}
	}
	if( i == nArrayLength )
	{
		nArrayEnd = nArrayLength - 1;
	}
	return  nArrayEnd;
}
/*********************************************************************************************************
** Function name:     F_FindVehEnd
** Descriptions:      ������������ЧԪ�صĸ���
** input parameters:  ���顢���鳤��        
** output parameters: ������ʵ��Ԫ�ظ���
**
** Created by: zhangtonghan		  
** Created Date: 2017-06-15  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int8 F_FindVehEnd(int16 lp_nArray[],uint8 cnArrayLength)
{
	int8 nArrayEnd;
	uint8	i,nArrayLength;
	int16 *ap_nArray;
	ap_nArray = lp_nArray;
	nArrayLength = cnArrayLength;
	nArrayEnd = 0;
	for( i=0;i<nArrayLength;i++){
		if(-1 ==ap_nArray[i]){
			nArrayEnd=i-1;
			break;
		}
	}
	if( i == nArrayLength )
	{
		nArrayEnd = nArrayLength - 1;
	}
	return  nArrayEnd;
}
/*********************************************************************************************************
** Function name:     FindCurAxleOrPreVeh
** Descriptions:      �ж�������һ��ֵǿ���׳�
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 2017-06-15  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FindCurAxleOrPreVeh(void * Address[3])
{
	ZTINFRECELEMENT *lpZTInf;
	VehicleInfForLoop_Struct (*VehiclePerChannelForLoop)[CHL_SAVEVEH];
	int8 * Dir;
	uint8 l_u8X,l_u8Y;
	//uint8 l_u8Err;
	uint8 l_u8ChlNum,l_n8ArrayPos;
	uint32 l_u32temptime;
	uint16 l_u16tempspeed;
	//uint16 l_u16SendLen;
	void *AddressForPo[3];
	
	lpZTInf = Address[0];
	Dir = Address[1];
	VehiclePerChannelForLoop=Address[2];
		
	FTransLocation(lpZTInf->m_ucTouch, &l_u8X, &l_u8Y,1);
		
	l_u8ChlNum = l_u8X;
	if(*Dir == 1)
	{
		if(((VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum == 1) &&\
					(VehiclePerChannelForLoop[l_u8ChlNum][0].m_BigVehFollowLitVeh != 1)) || \
					(VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum >= 2)) 
		{
			l_n8ArrayPos = F_FindVehEnd(VehiclePerChannelForLoop[l_u8ChlNum][0].m_an16CurrentAxleInf[VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum], MAXNUM_AXLE);
			l_u32temptime=GetDiff(g_ChlData[VehiclePerChannelForLoop[l_u8ChlNum][0].m_an16CurrentAxleInf[VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum][l_n8ArrayPos]].u32FirstDnTime,lpZTInf->m_ulTime);
			l_u16tempspeed=g_AxleInfo[g_asVehiclePerChannelForLoop[l_u8ChlNum][0].u16AxleList[VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum]].u16speed;
			if((l_u16tempspeed > 100) && ((l_u16tempspeed*l_u32temptime/216) > 13000))
			{
				VehiclePerChannelForLoop[l_u8ChlNum][0].m_u8FollowNewVehFlag = 1;
				if(VehiclePerChannelForLoop[l_u8ChlNum][0].m_au8AxleDropFlag[VehiclePerChannelForLoop[l_u8ChlNum][0].m_n8CurrentAxleNum]==1)
				{							
					AddressForPo[0]= &l_u8ChlNum;
					AddressForPo[1]= Dir;
					AddressForPo[2]= &VehiclePerChannelForLoop[l_u8ChlNum][0];
					
					FPostVehForLoop(AddressForPo,4);
				}					
			}
		}
	}
}

/*********************************************************************************************************
** Function name:     FMatchHideFristRow
** Descriptions:      ���ص�һ��ƥ�亯��
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 2017-09-04
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
 void 	FMatchHideFristRow(void * Address[8])
{
	ZTINFRECELEMENT *lpZTInf;
	HideColum_Struct (*Forw_ZTColum)[HIDECOLUM_SAVEAXlE]; 	//�����л���
	HideColum_Struct (*Rev_ZTColum)[HIDECOLUM_SAVEAXlE];		//�����л���
 	//VehicleInfForLoop_Struct (*VehForwStruct)[CHL_SAVEVEH]; //����������
 	VehicleInfForLoop_Struct (*VehReveStruct)[CHL_SAVEVEH];  //����������
	VehicleInfForLoop_Struct (*VehHideReveStruct)[CHL_SAVEVEH];
	VehicleInfForLoop_Struct (*VehHideForwStruct)[CHL_SAVEVEH];
	void* l_u32WhelCntAddr[8];
	uint8 pfirWhelPos = 0;
	uint8 psecWhelPos = 0;
	uint8 pWhelCnt = 0;
	uint8 *pl_u8X,l_u8X;	
	uint8 l_u8Pos;
	int8 Dir;
	void* l_u32CommMtchAddr[9];
	
	uint16 Wheel1XPos = 0, Wheel2XPos = 0;    //խ�����ֵ�λ��(����λ��),��λ(cm)
	uint8  LeftPoint = 0, RightPoint = 0; 
	
	lpZTInf 			= Address[0];
	pl_u8X				= Address[1];
	Forw_ZTColum		= Address[2];
	Rev_ZTColum			= Address[3];
 	//VehForwStruct		= Address[4];
 	VehReveStruct		= Address[5];
	VehHideForwStruct 	= Address[6];
	VehHideReveStruct 	= Address[7];
	
	
	l_u8X	= *pl_u8X;
	 
	if((l_u8X > 0)&&(l_u8X < PROMODPARAM.m_u8LaneNum*2-1) &&(VehHideForwStruct[l_u8X][0].m_u8VehicleHaveFlag == 0)&& ((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1)  || ((VehReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1) && (VehHideForwStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 0)) || ((VehReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&(VehHideForwStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 0)) ||\
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || ((VehHideReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&&(VehHideForwStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 0)) || ((VehHideReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&((VehHideForwStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 0)))||\
		((l_u8X+2 < PROMODPARAM.m_u8LaneNum*2)&&(VehReveStruct[l_u8X+2][0].m_u8VehicleHaveFlag == 1) &&(VehReveStruct[l_u8X+2][0].AxleStatus[VehReveStruct[l_u8X+2][0].m_n8CurrentAxleNum] != 0x03))))
	{
		Dir = -1;
		l_u32CommMtchAddr[0] = &Dir;
		l_u32CommMtchAddr[1] = &Rev_ZTColum[0][0];
		l_u32CommMtchAddr[2] = Address[5];
		l_u32CommMtchAddr[3] = Address[0];
		l_u32CommMtchAddr[4] = Address[1];
		FCommMtchZTFunc(l_u32CommMtchAddr);
		
	}
	else if((l_u8X == 0) &&(VehHideForwStruct[l_u8X][0].m_u8VehicleHaveFlag == 0)&& ((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || ((VehReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&& (VehHideForwStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 0))|| \
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || ((VehHideReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&& (VehHideForwStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 0))||\
	((VehReveStruct[l_u8X+2][0].m_u8VehicleHaveFlag == 1) &&(VehReveStruct[l_u8X+2][0].AxleStatus[VehReveStruct[l_u8X+2][0].m_n8CurrentAxleNum] != 0x03))))
	{
		Dir = -1;
		l_u32CommMtchAddr[0] = &Dir;
		l_u32CommMtchAddr[1] = &Rev_ZTColum[0][0];
		l_u32CommMtchAddr[2] = Address[5];
		l_u32CommMtchAddr[3] = Address[0];
		l_u32CommMtchAddr[4] = Address[1];
		FCommMtchZTFunc(l_u32CommMtchAddr);
	}
	else if((l_u8X == PROMODPARAM.m_u8LaneNum*2-1) &&(VehHideForwStruct[l_u8X][0].m_u8VehicleHaveFlag == 0)&& ((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || ((VehReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&& (VehHideForwStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 0))||\
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || ((VehHideReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&& (VehHideForwStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 0))))
	{
		Dir = -1;
		l_u32CommMtchAddr[0] = &Dir;
		l_u32CommMtchAddr[1] = &Rev_ZTColum[0][0];
		l_u32CommMtchAddr[2] = Address[5];
		l_u32CommMtchAddr[3] = Address[0];
		l_u32CommMtchAddr[4] = Address[1];
		FCommMtchZTFunc(l_u32CommMtchAddr);
	}
	else
	{
		l_u32WhelCntAddr[0]	=	lpZTInf;
		l_u32WhelCntAddr[1]	=	&pWhelCnt;
		l_u32WhelCntAddr[2] =   &Wheel1XPos;
		l_u32WhelCntAddr[3] =   &Wheel2XPos;
		l_u32WhelCntAddr[4] =   &LeftPoint;
		l_u32WhelCntAddr[5] =   &RightPoint;
		l_u32WhelCntAddr[6] =   &pfirWhelPos;
		l_u32WhelCntAddr[7] =   &psecWhelPos;
		FZTWheelCnt(l_u32WhelCntAddr);
		
		l_u8Pos = FindHideColumEnd(&Forw_ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
		Forw_ZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
		Forw_ZTColum[l_u8X][l_u8Pos].ZTCnt++;
		Forw_ZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
		Forw_ZTColum[l_u8X][l_u8Pos].Dir = 1;
		Forw_ZTColum[l_u8X][l_u8Pos].FirWhelPos = pfirWhelPos;
		Forw_ZTColum[l_u8X][l_u8Pos].SecWhelPos = psecWhelPos;
		Forw_ZTColum[l_u8X][l_u8Pos].WhelCnt = pWhelCnt;
		
	}
 	
}
/*********************************************************************************************************
** Function name:     FMatchHideSecondRow
** Descriptions:      ���صڶ���ƥ�亯��
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 2017-09-04
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void 	FMatchHideSecondRow(void * Address[8])
{
	ZTINFRECELEMENT *lpZTInf;
//	HideColum_Struct (*Forw_ZTColum)[HIDECOLUM_SAVEAXlE]; 	//�����л���
	HideColum_Struct (*Rev_ZTColum)[HIDECOLUM_SAVEAXlE];		//�����л���
// 	VehicleInfForLoop_Struct (*VehForwStruct)[CHL_SAVEVEH]; //����������
 	VehicleInfForLoop_Struct (*VehReveStruct)[CHL_SAVEVEH];  //����������
	VehicleInfForLoop_Struct (*VehHideReveStruct)[CHL_SAVEVEH];
	uint8 *pl_u8X,l_u8X;
	uint8 l_u8Pos;
	void *l_u32CommMtchAddr[9];
	int8 Dir;
//	uint32 WheelWght,Wait4WheelWght,Wait4WheelWght2,LargeRange,LitRange,LargeRange2,LitRange2;
	
	lpZTInf 				= Address[0];
	pl_u8X					=	Address[1];
//	Forw_ZTColum			=	Address[2];
 	Rev_ZTColum				= Address[3];
// 	VehForwStruct			= Address[4];
 	VehReveStruct			= Address[5];
	VehHideReveStruct 		= Address[7];
	
	l_u8X	= *pl_u8X;
	if((l_u8X > 0)&&(l_u8X < PROMODPARAM.m_u8LaneNum*2-1) && (Forward_HideZTColum[l_u8X][0].Colum[0] == -1) && ((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1) || (VehReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)||\
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehHideReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1) || (VehHideReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)))
	{
		l_u8Pos = FindHideColumEnd(&Rev_ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
		Rev_ZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
		Rev_ZTColum[l_u8X][l_u8Pos].ZTCnt++;
		Rev_ZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
		Rev_ZTColum[l_u8X][l_u8Pos].Dir = -1;	
	}
	else if((l_u8X == 0) && (Forward_HideZTColum[l_u8X][0].Colum[0] == -1) &&((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1)||\
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehHideReveStruct[l_u8X+1][0].m_u8VehicleHaveFlag == 1)))
	{
		l_u8Pos = FindHideColumEnd(&Rev_ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
		Rev_ZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
		Rev_ZTColum[l_u8X][l_u8Pos].ZTCnt++;
		Rev_ZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
		Rev_ZTColum[l_u8X][l_u8Pos].Dir = -1;
	}
	else if((l_u8X == PROMODPARAM.m_u8LaneNum*2-1) && (Forward_HideZTColum[l_u8X][0].Colum[0] == -1) &&((VehReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)||\
		(VehHideReveStruct[l_u8X][0].m_u8VehicleHaveFlag == 1) || (VehHideReveStruct[l_u8X-1][0].m_u8VehicleHaveFlag == 1)))
	{
		l_u8Pos = FindHideColumEnd(&Rev_ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
		Rev_ZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
		Rev_ZTColum[l_u8X][l_u8Pos].ZTCnt++;
		Rev_ZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
		Rev_ZTColum[l_u8X][l_u8Pos].Dir = -1;
	}
	else
	{
		Dir = 1;
		l_u32CommMtchAddr[0] = &Dir;
		l_u32CommMtchAddr[1] = Address[2];
		l_u32CommMtchAddr[2] = Address[4];
		l_u32CommMtchAddr[3] = Address[0];
		l_u32CommMtchAddr[4] = Address[1];
		FCommMtchZTFunc(l_u32CommMtchAddr);
	}
}


/*******************************************************************************************************
** Function name:  		FZTWheelCnt
** Descriptions:   		խ�����ֵĸ���
** input parameters:	խ�����
** output parameters:	խ�����ֵĸ���
**
** Created by: zhangtonghan		  
** Created Date: 20180604
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FZTWheelCnt(void * Address[8])
{
	ZTINFRECELEMENT *lpZTInf;
	uint8 *pWheelCnt;
	uint8 l_u8temp,ZTACnt=0,ZTAPos=0,ZTBPos=0,ZTBCnt=0;
	uint16 *pWheel1XPos, *pWheel2XPos;   //ÿ��խ���������ֵľ���λ��,��λ(cm)
	uint8  *pLeftPoint, *pRightPoint;
	uint8 *pAPos,*pBPos;
	uint16  l_u16SendLen;
	uint8   l_u8Err;
	
	lpZTInf = Address[0];
	pWheelCnt = Address[1];
	pWheel1XPos = Address[2];
	pWheel2XPos = Address[3];
	pLeftPoint = Address[4];
	pRightPoint = Address[5];
	pAPos = Address[6];
	pBPos = Address[7];
	
	for(l_u8temp = 0; l_u8temp < 4;l_u8temp++)
	{
		if(g_ChlData[lpZTInf->u16ID].u8FirstZTUpDnPosCnt & (1 << l_u8temp))
			ZTACnt++;
		if(g_ChlData[lpZTInf->u16ID].u8SecondZTUpDnPosCnt & (1 << l_u8temp))
			ZTBCnt++;
		if( g_ChlData[lpZTInf->u16ID].u8FirstZTMaxPos & (1 << l_u8temp))
			ZTAPos = l_u8temp;
		if( g_ChlData[lpZTInf->u16ID].u8SecondZTMaxPos & (1 << l_u8temp))
			ZTBPos = l_u8temp;
	}
	
	//2����ʱ�ж�A��B�Ƿ���û�ϳӵ�
	if((ZTACnt > 0) && (ZTBCnt > 0) && (GetDiff(ZTAPos,(4+ZTBPos)) >= 3))
	{
		if(g_ChlData[lpZTInf->u16ID].u32FirstZTDnTmFir == g_ChlData[lpZTInf->u16ID].u32FirstZTUpTmFir)
		{
			g_ChlData[lpZTInf->u16ID].u8FirstZTMaxPos=0;
			g_ChlData[lpZTInf->u16ID].u8FirstZTUpDnPosCnt=0;
			ZTACnt = 0;
			ZTAPos = 0;
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nZTA UpTm=DnTm,Pos=%d.",
									lpZTInf->u16ID);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
		if(g_ChlData[lpZTInf->u16ID].u32SecondZTDnTmFir==g_ChlData[lpZTInf->u16ID].u32SecondZTUpTmFir)
		{
			g_ChlData[lpZTInf->u16ID].u8SecondZTMaxPos=0;
			g_ChlData[lpZTInf->u16ID].u8SecondZTUpDnPosCnt=0;
			ZTBCnt = 0;
			ZTBPos = 0;
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nZTB UpTm=DnTm,Pos=%d.",
									lpZTInf->u16ID);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		}
	}
	
	if((ZTACnt > 0) && (ZTBCnt > 0) && (GetDiff(ZTAPos,(4+ZTBPos)) >= 3))
	{
		*pWheelCnt = 2;
		*pWheel1XPos = (lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + (ZTAPos+1)*18;
		*pWheel2XPos = (lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + 90 + (ZTBPos+1)*18;
		
		*pLeftPoint = ZTACnt;
		*pRightPoint = ZTBCnt;
		
		*pAPos = 8*((lpZTInf->m_ucTouch-24)/2) + ZTAPos; //����
		*pBPos = 8*((lpZTInf->m_ucTouch-24)/2) + 4+ ZTBPos;  //����	
	}
	else if((ZTACnt > 0) || (ZTBCnt > 0))
	{
		*pWheelCnt = 1;
		if((ZTACnt > 0) && (ZTBCnt == 0))
		{
			*pWheel1XPos = (lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + (ZTAPos+1)*18;
			*pLeftPoint = ZTACnt;
			*pAPos = ((lpZTInf->m_ucTouch-24)/2)*8 + ZTAPos;
		}
		else if((ZTACnt == 0) && (ZTBCnt > 0))
		{
			*pWheel1XPos = (lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + 90 + (ZTBPos+1)*18;
			*pLeftPoint = ZTBCnt;
			*pAPos = ((lpZTInf->m_ucTouch-24)/2)*8 + ZTBPos + 4;
		}
		else if((ZTACnt > 0) && (ZTBCnt > 0) && (GetDiff(ZTAPos,(4+ZTBPos)) < 3))
		{
			*pWheel1XPos = (((lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + (ZTAPos+1)*18) + ((lpZTInf->m_ucTouch/PROMODPARAM.m_u8ZTRow)*180 + 90 + (ZTBPos+1)*18)) / 2;
			*pLeftPoint = ZTACnt + ZTBCnt;
			if(IsBigFlag(g_ChlData[lpZTInf->m_ucTouch].u16FirstZTMaxValue,g_ChlData[lpZTInf->m_ucTouch].u16SecondZTMaxValue))
			{
				*pAPos = ((lpZTInf->m_ucTouch-24)/2)*8 + ZTAPos;
			}
			else
			{
				*pAPos = ((lpZTInf->m_ucTouch-24)/2)*8 + 4 + ZTBPos;
			}
		}
	}
	else
	{
		*pWheelCnt = 0;
	}
}

/*******************************************************************************************************
** Function name:  		FZTMatch2Whel
** Descriptions:   		խ��ƥ����
** input parameters:	խ����š�խ��
** output parameters:	
**
** Created by: zhangtonghan		  
** Created Date: 20180604
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FZTMatch2Whel(void * Address[10])
{
	uint8 i, j, k;
	ZTINFRECELEMENT *lpZTInf;
	uint8 *pWheelCnt;
	uint8 *X;
	uint8 *Y;
	uint16 *pWheel1XPos, *pWheel2XPos;
	TraceWhel_Struct (*WheelTrace)[4];
	void* l_Address[12];

	uint8 NewTraceInitScore = 100;    //�½��켣��ʼ����0��1�ų�ʼΪ100��,2�ų�ʼ����Ϊ70��(�ݶ��˷���)
	uint8 WheelCache = WHEELCACHE;         //ÿ��խ�������ֵĸ���
	uint8 l_u8MergeCnt = 0;       //��¼�켣�ϲ�����   
	uint8 l_u8SearchID = 0xff;    
	uint32 *pTimeNow;
	uint32 l_u32TimeNow = 0;//hjp181110
	uint8  *pLeftPoint, *pRightPoint;
	uint8 SearchWheelCacheCnt = 3;    //���ҵĻ����ָ���,�ɵ�(Ŀǰ������3��������)
	uint8 SearchZTYPos, SearchZTXPos;
	uint8 ZTErrorFlag = 0;    //0:խ������, 1:խ������
	int8 Dir = 1;     //�����켣����,1:����,-1:����
	uint8 FirstRowBuildTraceFlag = 1;    //���Ž��켣��־,0:�������켣,1:��������켣,2:��������켣 Ĭ��Ϊ1
	uint8 AxlePos = 0;
	uint8 ExtendDis = 30;    //��չ30cm
	uint8 temp = 0;    //��ʱ����
	uint8 CurrentAxleNum = 0;
	
	uint8  ZTBuildTaceFlag = 0;  //խ�����켣��־ 0:δ�����켣,!0:�����켣 Ĭ��Ϊ0
	uint8  l_u8Err;
	uint16 l_u16SendLen;
	
	static uint8 l_u8cnt1 = 0;

	lpZTInf = Address[0];
	pWheelCnt = Address[1];
	X = Address[2];
	Y = Address[3];
	WheelTrace = Address[4];
	pWheel1XPos = Address[5];
	pWheel2XPos = Address[6];
    pTimeNow = Address[7];
	pLeftPoint = Address[8];
	pRightPoint = Address[9];

	/**** ���Դ��� ****/
	if(lpZTInf->m_ucTouch == 10)
	{
		l_u8cnt1++;
		if(l_u8cnt1 == 4)
		{
			l_u8cnt1 = 0;
		}
	}
	/**** end ****/
						
	//�½��켣��ϲ����й켣
	for(i=0; i<WheelCache; i++)
	{
		//����ƥ��
		if(WheelTrace[lpZTInf->m_ucTouch][i].HaveTraceFlag==0)   
		{
			Dir = 1;
			FirstRowBuildTraceFlag = 1;
			//��ǰխ����������ȡ+-1�ж�û������,�������խ��������켣,+-1��������,��λ���Ƿ����
			if(*Y == 0 || *Y == 1)
			{
				if(g_asReVehiclePerChannelForLoop[*X][0].m_u8VehicleHaveFlag == 1)    //��ǰ�������г�
				{
					FirstRowBuildTraceFlag = 0;
				}
				else if((*X < 2*PROMODPARAM.m_u8LaneNum-1) && (g_asReVehiclePerChannelForLoop[*X+1][0].m_u8VehicleHaveFlag == 1))    //+1��������
				{
					CurrentAxleNum = g_asReVehiclePerChannelForLoop[*X+1][0].m_n8CurrentAxleNum;
					for(k = CurrentAxleNum; k < CurrentAxleNum; k--)
					{
						AxlePos = g_asReVehiclePerChannelForLoop[*X+1][0].u16AxleList[g_asReVehiclePerChannelForLoop[*X+1][0].m_n8CurrentAxleNum];
						if(g_AxleInfo[AxlePos].AxleWhelCnt == 2)
						{
							break;
						}
					}
					
					if(*pWheel1XPos > g_AxleInfo[AxlePos].RightWheelXRange - ExtendDis)
					{
						FirstRowBuildTraceFlag = 0;
					}
				}
				else if((*X > 0) && (g_asReVehiclePerChannelForLoop[*X-1][0].m_u8VehicleHaveFlag == 1))    //-1��������
				{
					CurrentAxleNum = g_asReVehiclePerChannelForLoop[*X-1][0].m_n8CurrentAxleNum;
					for(k = CurrentAxleNum; k < CurrentAxleNum; k--)
					{
						AxlePos = g_asReVehiclePerChannelForLoop[*X-1][0].u16AxleList[g_asReVehiclePerChannelForLoop[*X-1][0].m_n8CurrentAxleNum];
						if(g_AxleInfo[AxlePos].AxleWhelCnt == 2)
						{
							break;
						}
					}
					
					if(*pWheel1XPos < g_AxleInfo[AxlePos].LeftWheelXRange + ExtendDis)
					{
						FirstRowBuildTraceFlag = 0;
					}
				}
			}
			//����խ��û�������½��켣 
			if((*Y == 0 || *Y == 1) && g_Setup.an8ZTStatus[lpZTInf->m_ucTouch] == 1 && FirstRowBuildTraceFlag > 0)    
			{
				l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
				l_Address[1] = &Dir;
				l_Address[2] = &i;    //������
				l_Address[3] = &NewTraceInitScore;
				l_Address[4] = pWheelCnt;
				l_Address[5] = pWheel1XPos;
				l_Address[6] = pWheel2XPos;
				l_Address[7] = pLeftPoint;
				l_Address[8] = pRightPoint;
				FCreatNewTrace(l_Address);
				ZTBuildTaceFlag |= 0x01;
			}
			
			//�ҵ�ǰ��,-1,+1�н��й켣�ϲ�,�����3��������
			//��ǰ��
			l_u8MergeCnt = 0;
			if(*Y > 1)
			{
				l_u8SearchID = lpZTInf->m_ucTouch-1;
				
				FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
				if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 1)    //��խ������,�Ҹ���ǰһ��խ��
				{
					l_u8SearchID--;
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
					{
						l_u8SearchID = 0xff;
					}
				}
				else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 1)    //����խ���������򲻼�����ǰ��
				{
					l_u8SearchID = 0xff;
				}
				if(l_u8SearchID < CHANNELNUM && (*Y == 4 || *Y == 5))
				{
					if((WheelTrace[l_u8SearchID][0].HaveTraceFlag == 0) && 
					   (g_ReWheelTrace[l_u8SearchID][0].HaveTraceFlag == 0))   //ǰһ��խ����û�н������������켣
					{
						l_u8SearchID--;
					}
				}
				if(l_u8SearchID < CHANNELNUM)
				{
					for(j=0; j<WheelCache; j++)
					{
						if(WheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
						{
							l_Address[0] = &WheelTrace[l_u8SearchID][j];
							l_Address[1] = &WheelTrace[lpZTInf->m_ucTouch][i];
							l_Address[2] = lpZTInf;
							l_Address[3] = pWheelCnt;
							l_Address[4] = &l_u8SearchID;
							l_Address[5] = pWheel1XPos;
							l_Address[6] = pWheel2XPos;
							l_Address[7] = pTimeNow;
							l_Address[8] = pLeftPoint;
							l_Address[9] = pRightPoint;
							l_Address[10] = &Dir;
							FWheelTraceMerge(l_Address);     //��Ϲ켣
							
							ZTBuildTaceFlag |= 0x01;
							l_u8MergeCnt++;
							if(l_u8MergeCnt >= SearchWheelCacheCnt)
								break;
						}
					}
					if(l_u8MergeCnt == 0 && (*Y == 2 || *Y == 3)  && (g_ReWheelTrace[lpZTInf->m_ucTouch+1][0].HaveTraceFlag == 0))
					{
						l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
						l_Address[1] = &Dir;
						l_Address[2] = &i;    //������
						l_Address[3] = &NewTraceInitScore;
						l_Address[4] = pWheelCnt;
						l_Address[5] = pWheel1XPos;
						l_Address[6] = pWheel2XPos;
						l_Address[7] = pLeftPoint;
						l_Address[8] = pRightPoint;
						FCreatNewTrace(l_Address);
						ZTBuildTaceFlag |= 0x01;
					}
				}
				else if(l_u8SearchID >= CHANNELNUM)    //��ǰ�ҵ�խ�������������ڸ�խ�����½��켣
				{
					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
					l_Address[1] = &Dir;
					l_Address[2] = &i;    //������
					l_Address[3] = &NewTraceInitScore;
					l_Address[4] = pWheelCnt;
					l_Address[5] = pWheel1XPos;
					l_Address[6] = pWheel2XPos;
					l_Address[7] = pLeftPoint;
					l_Address[8] = pRightPoint;
					FCreatNewTrace(l_Address);
					ZTBuildTaceFlag |= 0x01;
				}
			}
			
			//-1��
			l_u8MergeCnt = 0;
			if(lpZTInf->m_ucTouch > 3 && *Y > 0)
			{
				if(*X%2 == 0)
					l_u8SearchID = lpZTInf->m_ucTouch-3;
				else
					l_u8SearchID = lpZTInf->m_ucTouch-4;
				
				FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
				if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 1)    //��խ������,�Ҹ���ǰһ��խ��
				{
					l_u8SearchID--;
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
					{
						l_u8SearchID = 0xff;
					}
				}
				else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 1)    //����խ���������򲻼�����ǰ��
				{
					l_u8SearchID = 0xff;
				}
				if(l_u8SearchID < CHANNELNUM)
				{
					for(j=0; j<WheelCache; j++)
					{
						if(WheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
						{
							l_Address[0] = &WheelTrace[l_u8SearchID][j];
							l_Address[1] = &WheelTrace[lpZTInf->m_ucTouch][i];
							l_Address[2] = lpZTInf;
							l_Address[3] = pWheelCnt;
							l_Address[4] = &l_u8SearchID;
							l_Address[5] = pWheel1XPos;
							l_Address[6] = pWheel2XPos;
							l_Address[7] = pTimeNow;
							l_Address[8] = pLeftPoint;
							l_Address[9] = pRightPoint;
							l_Address[10] = &Dir;
							FWheelTraceMerge(l_Address);     //��Ϲ켣
							ZTBuildTaceFlag |= 0x01;
							l_u8MergeCnt++;
							if(l_u8MergeCnt >= SearchWheelCacheCnt)
								break;
						}
					}
				}
				else    //��ǰ�ҵ�խ�������������ڸ�խ�����½��켣
				{
// 					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
// 					l_Address[1] = &Dir;
// 					l_Address[2] = &i;    //������
// 					l_Address[3] = &NewTraceInitScore;
// 					l_Address[4] = pWheelCnt;
// 					l_Address[5] = pWheel1XPos;
// 					l_Address[6] = pWheel2XPos;
// 					l_Address[7] = pLeftPoint;
// 					l_Address[8] = pRightPoint;
// 					FCreatNewTrace(l_Address);
// 					ZTBuildTaceFlag |= 0x01;
				}
			}
			
			//+1��
			l_u8MergeCnt = 0;
			if(lpZTInf->m_ucTouch < WHEELTRACEZTNUM && *Y > 0)
			{
				if(*X%2 == 0)
					l_u8SearchID = lpZTInf->m_ucTouch+3;
				else
					l_u8SearchID = lpZTInf->m_ucTouch+2;
				
				FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
				if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 1)    //��խ������,�Ҹ���ǰһ��խ��
				{
					l_u8SearchID--;
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
					{
						l_u8SearchID = 0xff;
					}
				}
				else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 1)    //����խ���������򲻼�����ǰ��
				{
					l_u8SearchID = 0xff;
				}
				if(l_u8SearchID < CHANNELNUM)
				{
					for(j=0; j<WheelCache; j++)
					{
						if(WheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
						{
							l_Address[0] = &WheelTrace[l_u8SearchID][j];
							l_Address[1] = &WheelTrace[lpZTInf->m_ucTouch][i];
							l_Address[2] = lpZTInf;
							l_Address[3] = pWheelCnt;
							l_Address[4] = &l_u8SearchID;
							l_Address[5] = pWheel1XPos;
							l_Address[6] = pWheel2XPos;
							l_Address[7] = pTimeNow;
							l_Address[8] = pLeftPoint;
							l_Address[9] = pRightPoint;
							l_Address[10] = &Dir;
							FWheelTraceMerge(l_Address);    //��Ϲ켣
							ZTBuildTaceFlag |= 0x01;
							l_u8MergeCnt++;
							if(l_u8MergeCnt >= SearchWheelCacheCnt)
								break;
						}
					}
				}
				else    //��ǰ�ҵ�խ�������������ڸ�խ�����½��켣
				{
// 					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
// 					l_Address[1] = &Dir;
// 					l_Address[2] = &i;    //������
// 					l_Address[3] = &NewTraceInitScore;
// 					l_Address[4] = pWheelCnt;
// 					l_Address[5] = pWheel1XPos;
// 					l_Address[6] = pWheel2XPos;
// 					l_Address[7] = pLeftPoint;
// 					l_Address[8] = pRightPoint;
// 					FCreatNewTrace(l_Address);
// 					ZTBuildTaceFlag |= 0x01;
				}
			}
			//������ѡ�켣
			l_Address[0] = &WheelTrace[lpZTInf->m_ucTouch][i];//0ori?hjp
			FFindFirstTracePos(l_Address);
			
			//����ĩ����ʱʱ�䣬HJP
			if((*Y > 3) && (WheelTrace[lpZTInf->m_ucTouch][i].LeftWhlZTCntPerTrace[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos] > 0))
			{
				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
				if(*Y == 4)
				{
					if(WheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0] > 0)
						WheelTrace[lpZTInf->m_ucTouch][i].LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0];
						//������ĩ����ʱʱ��
				}
				else
				{
					WheelTrace[lpZTInf->m_ucTouch][i].LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0];   //ż����ĩ����ʱʱ��
				       //ż����ĩ����ʱʱ��
				}
			}
			if((*Y > 3) && (WheelTrace[lpZTInf->m_ucTouch][i].RightWhlZTCntPerTrace[WheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos] > 0))
			{
				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
				if(*Y == 4)
				{
					if(WheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0] > 0)
						WheelTrace[lpZTInf->m_ucTouch][i].RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0];
					    //������ĩ����ʱʱ��
				}
				else
				{
					WheelTrace[lpZTInf->m_ucTouch][i].RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0];   //ż����ĩ����ʱʱ��
				        //ż����ĩ����ʱʱ��
				}
			}
			
			break;
		}
	}
	temp = i;     //�����жϸ�խ���Ƿ�ɹ������켣,�����ڴ�ӡ������Ϣ
	
	for(i=0; i<WheelCache; i++)
	{
		//����ƥ��
		if(g_ReWheelTrace[lpZTInf->m_ucTouch][i].HaveTraceFlag==0)   
		{
			Dir = -1;
			FirstRowBuildTraceFlag = 2;
			//��ǰխ����������ȡ+-1�ж�û������,�������խ��������켣,+-1��������,��λ���Ƿ����
			//if((*Y == 2*PROMODPARAM.m_u8ZTRow-1) || (*Y == 2*PROMODPARAM.m_u8ZTRow-2))
			{
				//if((g_asVehiclePerChannelForLoop[*X][0].m_u8VehicleHaveFlag == 1) || (g_HideZTVehicleStruct[*X][0].m_u8VehicleHaveFlag == 1))    //��ǰ�������г�
				if(g_asVehiclePerChannelForLoop[*X][0].m_u8VehicleHaveFlag == 1)    //��ǰ�������г�
				{
					FirstRowBuildTraceFlag = 0;
				}
				else if((*X < 2*PROMODPARAM.m_u8LaneNum-1) && (g_asVehiclePerChannelForLoop[*X+1][0].m_u8VehicleHaveFlag == 1))    //+1��������
				{
					CurrentAxleNum = g_asVehiclePerChannelForLoop[*X+1][0].m_n8CurrentAxleNum;
					for(k = CurrentAxleNum; k < CurrentAxleNum; k--)
					{
						AxlePos = g_asVehiclePerChannelForLoop[*X+1][0].u16AxleList[g_asVehiclePerChannelForLoop[*X+1][0].m_n8CurrentAxleNum];
						if(g_AxleInfo[AxlePos].AxleWhelCnt == 2)
						{
							break;
						}
					}
					if(*pWheel1XPos > g_AxleInfo[AxlePos].LeftWheelXRange - ExtendDis)
					{
						FirstRowBuildTraceFlag = 0;
					}
				}
				else if((*X > 0) && (g_asVehiclePerChannelForLoop[*X-1][0].m_u8VehicleHaveFlag == 1))    //-1��������
				{
					CurrentAxleNum = g_asVehiclePerChannelForLoop[*X-1][0].m_n8CurrentAxleNum;
					for(k = CurrentAxleNum; k < CurrentAxleNum; k--)
					{
						AxlePos = g_asVehiclePerChannelForLoop[*X-1][0].u16AxleList[g_asVehiclePerChannelForLoop[*X-1][0].m_n8CurrentAxleNum];
						if(g_AxleInfo[AxlePos].AxleWhelCnt == 2)
						{
							break;
						}
					}
					
					if(*pWheel1XPos < g_AxleInfo[AxlePos].RightWheelXRange + ExtendDis)
					{
						FirstRowBuildTraceFlag = 0;
					}
				}
// 				if(*Y == 0 || *Y == 1)
// 				{
// 					if((g_WheelTrace[lpZTInf->m_ucTouch+PROMODPARAM.m_u8ZTRow-1][0].HaveTraceFlag==1) && (g_Setup.an8ZTStatus[lpZTInf->m_ucTouch+1] == ZTErrorFlag))
// 					{
// 						FirstRowBuildTraceFlag = 0;
// 					}
// 				}
			}
			if(FirstRowBuildTraceFlag > 0)
			{
				if(((*Y == 2*PROMODPARAM.m_u8ZTRow-1) || (*Y == 2*PROMODPARAM.m_u8ZTRow-2)) && 
				   (g_Setup.an8ZTStatus[lpZTInf->m_ucTouch] == 1))   
				{
					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
					l_Address[1] = &Dir;
					l_Address[2] = &i;    //������
					l_Address[3] = &NewTraceInitScore;
					l_Address[4] = pWheelCnt;
					l_Address[5] = pWheel1XPos;
					l_Address[6] = pWheel2XPos;
					l_Address[7] = pLeftPoint;
					l_Address[8] = pRightPoint;
					FCreatNewTrace(l_Address);
					ZTBuildTaceFlag |= 0x02;
				}
				
				//�ҵ�ǰ��,-1,+1�н��й켣�ϲ�,�����3��������
				//��ǰ��
				l_u8MergeCnt = 0;
				if(*Y < 2*PROMODPARAM.m_u8ZTRow-2)
				{
					l_u8SearchID = lpZTInf->m_ucTouch+1;
					FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 3)    //��խ������,�Ҹ���ǰһ��խ��
					{
						l_u8SearchID++;
						if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
						{
							l_u8SearchID = 0xff;
						}
					}
					else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 3)    //ĩ��խ���������򲻼�����ǰ��
					{
						l_u8SearchID = 0xff;
					}
					if(l_u8SearchID < CHANNELNUM && (*Y == 0 || *Y == 1))
					{
						if((g_ReWheelTrace[l_u8SearchID][0].HaveTraceFlag == 0) && 
						   (g_WheelTrace[l_u8SearchID][0].HaveTraceFlag == 0))    //ǰһ��խ����û�н�������켣������켣������ǰ��խ��
						{
							l_u8SearchID++;
						}
					}
					if(l_u8SearchID < CHANNELNUM)
					{
						for(j=0; j<WheelCache; j++)
						{
							if(g_ReWheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
							{
								l_Address[0] = &g_ReWheelTrace[l_u8SearchID][j];
								l_Address[1] = &g_ReWheelTrace[lpZTInf->m_ucTouch][i];
								l_Address[2] = lpZTInf;
								l_Address[3] = pWheelCnt;
								l_Address[4] = &l_u8SearchID;
								l_Address[5] = pWheel1XPos;
								l_Address[6] = pWheel2XPos;
								l_Address[7] = pTimeNow;
								l_Address[8] = pLeftPoint;
								l_Address[9] = pRightPoint;
								l_Address[10] = &Dir;
								FWheelTraceMerge(l_Address);     //��Ϲ켣
								ZTBuildTaceFlag |= 0x02;
								l_u8MergeCnt++;
								if(l_u8MergeCnt >= SearchWheelCacheCnt)
									break;
							}
						}
						if(l_u8MergeCnt == 0 && (*Y == 2 || *Y == 3) && (g_WheelTrace[lpZTInf->m_ucTouch-1][0].HaveTraceFlag == 0) &&
						   (g_ReWheelTrace[lpZTInf->m_ucTouch+1][0].HaveTraceFlag == 1))
						{
							l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
							l_Address[1] = &Dir;
							l_Address[2] = &i;    //������
							l_Address[3] = &NewTraceInitScore;
							l_Address[4] = pWheelCnt;
							l_Address[5] = pWheel1XPos;
							l_Address[6] = pWheel2XPos;
							l_Address[7] = pLeftPoint;
							l_Address[8] = pRightPoint;
							FCreatNewTrace(l_Address);
							ZTBuildTaceFlag |= 0x02;
						}
					}
					else    //��ǰ�ҵ�խ�������������ڸ�խ�����½��켣
					{
						l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
						l_Address[1] = &Dir;
						l_Address[2] = &i;    //������
						l_Address[3] = &NewTraceInitScore;
						l_Address[4] = pWheelCnt;
						l_Address[5] = pWheel1XPos;
						l_Address[6] = pWheel2XPos;
						l_Address[7] = pLeftPoint;
						l_Address[8] = pRightPoint;
						FCreatNewTrace(l_Address);
						ZTBuildTaceFlag |= 0x02;
					}
				}
				
				//-1��
				l_u8MergeCnt = 0;
				if(lpZTInf->m_ucTouch > 3 && *Y < 2*PROMODPARAM.m_u8ZTRow-1)
				{
					if(*X%2 == 0)
						l_u8SearchID = lpZTInf->m_ucTouch-2;
					else
						l_u8SearchID = lpZTInf->m_ucTouch-3;
					FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 3)    //��խ������,�Ҹ���ǰһ��խ��
					{
						l_u8SearchID++;
						if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
						{
							l_u8SearchID = 0xff;
						}
					}
					else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 3)    //����խ���������򲻼�����ǰ��
					{
						l_u8SearchID = 0xff;
					}
					
					if(l_u8SearchID < CHANNELNUM)
					{
						for(j=0; j<WheelCache; j++)
						{
							if(g_ReWheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
							{
								l_Address[0] = &g_ReWheelTrace[l_u8SearchID][j];
								l_Address[1] = &g_ReWheelTrace[lpZTInf->m_ucTouch][i];
								l_Address[2] = lpZTInf;
								l_Address[3] = pWheelCnt;
								l_Address[4] = &l_u8SearchID;
								l_Address[5] = pWheel1XPos;
								l_Address[6] = pWheel2XPos;
								l_Address[7] = pTimeNow;
								l_Address[8] = pLeftPoint;
								l_Address[9] = pRightPoint;
								l_Address[10] = &Dir;
								FWheelTraceMerge(l_Address);     //��Ϲ켣
								ZTBuildTaceFlag |= 0x02;
								l_u8MergeCnt++;
								if(l_u8MergeCnt >= SearchWheelCacheCnt)
									break;
							}
						}
					}
					else
					{
	// 					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
	// 					l_Address[1] = &Dir;
	// 					l_Address[2] = &i;    //������
	// 					l_Address[3] = &NewTraceInitScore;
	// 					l_Address[4] = pWheelCnt;
	// 					l_Address[5] = pWheel1XPos;
	// 					l_Address[6] = pWheel2XPos;
	// 					l_Address[7] = pLeftPoint;
	// 					l_Address[8] = pRightPoint;
	// 					FCreatNewTrace(l_Address);
	// 					ZTBuildTaceFlag |= 0x02;
					}
				}
				
				//+1��
				l_u8MergeCnt = 0;
				if(lpZTInf->m_ucTouch < WHEELTRACEZTNUM && *Y < 2*PROMODPARAM.m_u8ZTRow-1)
				{
					if(*X%2 == 0)
						l_u8SearchID = lpZTInf->m_ucTouch+4;
					else
						l_u8SearchID = lpZTInf->m_ucTouch+3;
					FTransLocation(l_u8SearchID, &SearchZTXPos, &SearchZTYPos, 2);
					if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos <= 3)    //��խ������,�Ҹ���ǰһ��խ��
					{
						l_u8SearchID++;
						if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag)     //��ǰ2�Ŷ��������򲻽���ƥ��
						{
							l_u8SearchID = 0xff;
						}
					}
					else if(g_Setup.an8ZTStatus[l_u8SearchID] == ZTErrorFlag && SearchZTYPos > 3)    //����խ���������򲻼�����ǰ��
					{
						l_u8SearchID = 0xff;
					}
					
					if(l_u8SearchID < CHANNELNUM)
					{
						for(j=0; j<WheelCache; j++)
						{
							if(g_ReWheelTrace[l_u8SearchID][j].HaveTraceFlag==1)
							{
								l_Address[0] = &g_ReWheelTrace[l_u8SearchID][j];
								l_Address[1] = &g_ReWheelTrace[lpZTInf->m_ucTouch][i];
								l_Address[2] = lpZTInf;
								l_Address[3] = pWheelCnt;
								l_Address[4] = &l_u8SearchID;
								l_Address[5] = pWheel1XPos;
								l_Address[6] = pWheel2XPos;
								l_Address[7] = pTimeNow;
								l_Address[8] = pLeftPoint;
								l_Address[9] = pRightPoint;
								l_Address[10] = &Dir;
								FWheelTraceMerge(l_Address);     //��Ϲ켣
								ZTBuildTaceFlag |= 0x02;
								l_u8MergeCnt++;
								if(l_u8MergeCnt >= SearchWheelCacheCnt)
									break;
							}
						}
					}
					else
					{
	// 					l_Address[0] = &lpZTInf->u16ID;    //��ǰխ����Pos
	// 					l_Address[1] = &Dir;
	// 					l_Address[2] = &i;    //������
	// 					l_Address[3] = &NewTraceInitScore;
	// 					l_Address[4] = pWheelCnt;
	// 					l_Address[5] = pWheel1XPos;
	// 					l_Address[6] = pWheel2XPos;
	// 					l_Address[7] = pLeftPoint;
	// 					l_Address[8] = pRightPoint;
	// 					FCreatNewTrace(l_Address);
	// 					ZTBuildTaceFlag |= 0x02;
					}
				}
				//������ѡ�켣
				l_Address[0] = &g_ReWheelTrace[lpZTInf->m_ucTouch][0];
				FFindFirstTracePos(l_Address);
				
				//����ĩ����ʱʱ�䣬HJP
				if((*Y < 2) && (g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftWhlZTCntPerTrace[g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos] > 0))
				{
					ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
					if(*Y == 1)
					{
						if(g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0] > 0)
							g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]/2/10 + g_u16EvenColumDelyLth)*6000 / g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0];
							//������ĩ����ʱʱ��
					}
					else
					{
						g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftWhlSpd[g_ReWheelTrace[lpZTInf->m_ucTouch][i].LeftFirstTracePos][0];   //ż����ĩ����ʱʱ��
						   //ż����ĩ����ʱʱ��
					}
				}
				if((*Y < 2) && (g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightWhlZTCntPerTrace[g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos] > 0))
				{
					ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
					if(*Y == 1)
					{
						if(g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[WheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0] > 0)
							g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]/2/10 + g_u16EvenColumDelyLth)*6000 / g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0];
							//������ĩ����ʱʱ��
					}
					else
					{
						g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightWhlSpd[g_ReWheelTrace[lpZTInf->m_ucTouch][i].RightFirstTracePos][0];   //ż����ĩ����ʱʱ��
							//ż����ĩ����ʱʱ��
					}
				}
				break;
			}
		}
	}
	
	if(ZTBuildTaceFlag == 0 || (g_WheelTrace[lpZTInf->m_ucTouch][temp].HaveTraceFlag == 0 && g_ReWheelTrace[lpZTInf->m_ucTouch][i].HaveTraceFlag == 0))    //խ�������켣ʧ��(��ӡ����)
	{
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nBuildTrace Failed: %02x,%d,ID=%d,Pos=%d",
								ZTBuildTaceFlag, FirstRowBuildTraceFlag, lpZTInf->m_ucTouch, lpZTInf->u16ID);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	}
}
/*******************************************************************************************************
** Function name:  		FFindFirstTracePos
** Descriptions:   		Ѱ����ѡ�켣
** input parameters:	խ���켣��Ϣ
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180605
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FFindFirstTracePos(void * Address[1])
{
	TraceWhel_Struct *WheelTrace;
	uint8 i;
	uint8 l_u8ThrPassTracePos = 0xff;
	uint8 l_u8MaxScoreTracePos = 0;
	uint8 l_u8MaxScore = 0;
	
	WheelTrace = Address[0];
	
	//Left
	l_u8ThrPassTracePos = 0xff;
	l_u8MaxScoreTracePos = 0;
	for(i=0; i<WheelTrace->LeftTraceCnt; i++)
	{
		//ѹ��켣��Ѱ�����ŵ�
		if(WheelTrace->LeftWhlStatus[i] == YAFENG && l_u8ThrPassTracePos != 0xff)
		{
			if((WheelTrace->LeftWhlZTCntPerTrace[i] > WheelTrace->LeftWhlZTCntPerTrace[l_u8ThrPassTracePos]))   //��ǰ�켣խ������,���շ�������ѡ��
			{
				if((WheelTrace->LeftMultiScore[i] > 60) || 
				   (WheelTrace->LeftMultiScore[i] > WheelTrace->LeftMultiScore[l_u8ThrPassTracePos]))  //��ǰ�켣��������50���ǰһ����ѡ�켣�ķ�����, �������ѡѹ��켣
				{
					l_u8ThrPassTracePos = i;    //ѹ��켣λ��
				}
			}
			else if((WheelTrace->LeftWhlZTCntPerTrace[i] < WheelTrace->LeftWhlZTCntPerTrace[l_u8ThrPassTracePos]))  //��ǰ�켣խ������
			{
				if((WheelTrace->LeftMultiScore[l_u8ThrPassTracePos] <= 60) && 
				   (WheelTrace->LeftMultiScore[i] > WheelTrace->LeftMultiScore[l_u8ThrPassTracePos]))
				{
					l_u8ThrPassTracePos = i;    //ѹ��켣λ��
				}
			}
			else    //խ������ͬ
			{
				if(WheelTrace->LeftMultiScore[i] > WheelTrace->LeftMultiScore[l_u8ThrPassTracePos])
				{
					l_u8ThrPassTracePos = i;    //ѹ��켣λ��
				}
			}
		}
		else if(WheelTrace->LeftWhlStatus[i] == YAFENG && l_u8ThrPassTracePos == 0xff)
		{
			l_u8ThrPassTracePos = i;    //ѹ��켣λ��
		}
		
		//Ѱ�ҷ�����ߵĹ켣
		if((WheelTrace->LeftMultiScore[i] > ((l_u8MaxScore>5)?(l_u8MaxScore+5):l_u8MaxScore)) || 
		   (GetDiff(WheelTrace->LeftMultiScore[i], l_u8MaxScore) < 5 && WheelTrace->LeftWhlStatus[i] == NORMAL && WheelTrace->LeftWhlZTCntPerTrace[i] > WheelTrace->LeftWhlZTCntPerTrace[l_u8MaxScoreTracePos]))
		{
			l_u8MaxScore = WheelTrace->LeftMultiScore[i];    //ȡ�����й켣�е���߷�,��������5�����ڵ���ǰ��Ĺ켣��խ������Ĺ켣
			l_u8MaxScoreTracePos = i;    //��߷ֹ켣��λ��
		}
	}
	if(l_u8ThrPassTracePos < 6)   //��ѹ��켣,������ѹ��켣����
		WheelTrace->LeftFirstTracePos = l_u8ThrPassTracePos;
	else
		WheelTrace->LeftFirstTracePos = l_u8MaxScoreTracePos;
	
	//Right
	l_u8ThrPassTracePos = 0xff;
	l_u8MaxScoreTracePos = 0;
	for(i=0; i<WheelTrace->RightTraceCnt; i++)
	{
		//ѹ��켣��Ѱ�����ŵ�
		if(WheelTrace->RightWhlStatus[i] == YAFENG && l_u8ThrPassTracePos != 0xff)
		{
			if((WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[l_u8ThrPassTracePos]))   //��ǰ�켣խ������,���շ�������ѡ��
			{
				if((WheelTrace->RightMultiScore[i] > 50) || 
				   (WheelTrace->RightMultiScore[i] > WheelTrace->RightMultiScore[l_u8ThrPassTracePos]))  //��ǰ�켣��������50���ǰһ����ѡ�켣�ķ�����, �������ѡѹ��켣
				{
					l_u8ThrPassTracePos = i;    //ѹ��켣λ��
				}
			}
			else    //��ǰ�켣խ������
			{
				if((WheelTrace->RightMultiScore[l_u8ThrPassTracePos] <= 50) && 
				   (WheelTrace->RightMultiScore[i] > WheelTrace->RightMultiScore[l_u8ThrPassTracePos]))
				{
					l_u8ThrPassTracePos = i;    //ѹ��켣λ��
				}
			}
		}
		else if(WheelTrace->RightWhlStatus[i] == YAFENG && l_u8ThrPassTracePos == 0xff)
		{
			l_u8ThrPassTracePos = i;    //ѹ��켣λ��
		}
		
		//Ѱ�ҷ�����ߵĹ켣
		if((WheelTrace->RightMultiScore[i] > ((l_u8MaxScore>5)?(l_u8MaxScore+5):l_u8MaxScore)) ||
		   (GetDiff(WheelTrace->RightMultiScore[i], l_u8MaxScore) < 5 && WheelTrace->RightWhlStatus[i] == NORMAL && WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[l_u8MaxScoreTracePos]))
		{
			l_u8MaxScore = WheelTrace->RightMultiScore[i];    //ȡ�����й켣�е���߷�,��������5�����ڵ���ǰ��Ĺ켣��խ������Ĺ켣
			l_u8MaxScoreTracePos = i;    //��߷ֹ켣��λ��
		}
	}
	if(l_u8ThrPassTracePos < 6)   //��ѹ��켣,������ѹ��켣����
		WheelTrace->RightFirstTracePos = l_u8ThrPassTracePos;
	else
		WheelTrace->RightFirstTracePos = l_u8MaxScoreTracePos;
}

/*******************************************************************************************************
** Function name:  		FCreatNewTrace
** Descriptions:   		�����¹켣
** input parameters:	խ��,�켣�ṹ��,�ڼ�����
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180605
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FCreatNewTrace(void * Address[10])
{
	TraceWhel_Struct *WheelTrace;
	uint8 *pZTCache;   //��ǰխ������ĸ���
	uint8 l_u8ZTLocationX, l_u8ZTLocationY;
	uint16 *pWheel1XPos, *pWheel2XPos;
	uint8 *pNewTraceInitScore;
	uint8 *pWheelCnt;
	uint8 *pLeftPoint, *pRightPoint;
	uint16 YStep = 70;     //y����ľ�������(Ĭ��Ϊ1400mm����һ��)
	uint16 *pCurZTPos;
	uint8  curZTID;
	int8   *pDir;
	
	pCurZTPos = Address[0];
	pDir = Address[1];
	pZTCache = Address[2];
    pNewTraceInitScore= Address[3];
	pWheelCnt = Address[4];
	pWheel1XPos = Address[5];
	pWheel2XPos = Address[6];
	pLeftPoint = Address[7];
	pRightPoint = Address[8];
	
	curZTID = g_ChlData[*pCurZTPos].u8Id;
	YStep = PROMODPARAM.m_au16ZTGap[curZTID] / 2 / 10;   //��λcm
	FTransLocation(curZTID, &l_u8ZTLocationX, &l_u8ZTLocationY, 2);
	
	if(*pDir == 1)    //����
	{
		WheelTrace = &g_WheelTrace[curZTID][*pZTCache];
	}
	else if(*pDir == -1)    //����
	{
		WheelTrace = &g_ReWheelTrace[curZTID][*pZTCache];
	}

	//���ֽ���һ���켣,����Left������
	WheelTrace->LeftWhlZTID[0][0] = curZTID;
	WheelTrace->LeftWhlZTPos[0][0] = *pCurZTPos;
	if(*pWheelCnt == 1)
	{
		WheelTrace->LeftWhlZTake[0][0] = 3;
	}
	else
	{
		WheelTrace->LeftWhlZTake[0][0] = 1;
	}
	WheelTrace->LeftWhlZTCntPerTrace[0] = 1;
	WheelTrace->LeftWhlZTXPos[0][0] = *pWheel1XPos;
	WheelTrace->LeftWhlZTYPos[0][0] = l_u8ZTLocationY*YStep;
	WheelTrace->LeftWhlSpd[0][0] = 0;
	WheelTrace->LeftWhlDiffXPos[0][0] = 0;
	WheelTrace->LeftPoint[0][0] = *pLeftPoint;
	WheelTrace->LeftPoScore[0] = *pNewTraceInitScore;
	WheelTrace->LeftSpdScore[0] = *pNewTraceInitScore;
	WheelTrace->LeftRadScore[0] = *pNewTraceInitScore;
	WheelTrace->LeftMultiScore[0] = *pNewTraceInitScore;
	WheelTrace->LeftTraceCnt = 1;
	WheelTrace->LastXPos = *pWheel1XPos;
	WheelTrace->LastYPos = l_u8ZTLocationY*70;
	WheelTrace->LeftWhlStatus[0] = 0;
	WheelTrace->Dir = *pDir;
	WheelTrace->WheelCnt = *pWheelCnt;
	WheelTrace->HaveTraceFlag = 1;
	WheelTrace->LeftDueTime = 0;
	WheelTrace->LeftFirstTracePos = 0;
	WheelTrace->LeftStatus = 0;
	
	//˫�ֽ��������켣,�ֱ����Letf��Right������
	if(*pWheelCnt == 2)   
	{
		WheelTrace->RightWhlZTID[0][0] = curZTID;
		WheelTrace->RightWhlZTPos[0][0] = *pCurZTPos;
		WheelTrace->RightWhlZTake[0][0] = 2;
		WheelTrace->RightWhlZTCntPerTrace[0] = 1;
		WheelTrace->RightWhlZTXPos[0][0] = *pWheel2XPos;
		WheelTrace->RightWhlZTYPos[0][0] = l_u8ZTLocationY*YStep;
		WheelTrace->RightWhlSpd[0][0] = 0;
		WheelTrace->RightWhlDiffXPos[0][0] = 0;
		WheelTrace->RightPoint[0][0] = *pRightPoint;
		WheelTrace->RightPoScore[0] = *pNewTraceInitScore;
		WheelTrace->RightSpdScore[0] = *pNewTraceInitScore;
		WheelTrace->RightRadScore[0] = *pNewTraceInitScore;
		WheelTrace->RightMultiScore[0] = *pNewTraceInitScore;
		WheelTrace->RightTraceCnt = 1;
		WheelTrace->LastXPos = *pWheel2XPos;
		WheelTrace->LastYPos = l_u8ZTLocationY*70;
		WheelTrace->RightWhlStatus[0] = 0;
		WheelTrace->Dir = *pDir;
		WheelTrace->WheelCnt = *pWheelCnt;
		WheelTrace->HaveTraceFlag = 1;
		WheelTrace->RightDueTime = 0;
		WheelTrace->RightFirstTracePos = 0;
		WheelTrace->RightStatus = 0;
	}
}

/*******************************************************************************************************
** Function name:  		FAppendTrace
** Descriptions:   		׷�ӹ켣
** input parameters:	��׷�ӹ켣����Ҫ��Ϣ,׷��λ��
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180607
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FInsertTrace(void * Address[15])
{
	TraceWhel_Struct *WheelTrace;
	uint16 *l_u16TraceList;    //�켣խ��Pos
	uint16 *ZTXpos;     //������ÿ��խ����Xλ��(��λ:cm)
	uint16 *ZTYpos;     //������ÿ��խ����Yλ��(��λ:cm)
	uint16 *Spd;        //������ÿ�������ٶ�(��λ:cm/s)
	uint8  *pZTCnt;
	uint8  *l_u8Score;   	     //λ�õ÷�,�ٶȵ÷�,���ȵ÷�,�ۺϵ÷�
	uint8  *l_u8ZTIDPerTrace;    //�켣��խ��ID
	uint8  *pInsertPos;
	uint8  *pTraceDir;
	uint8  *pWheelCnt;
	uint8  *pWheelZTake;
	uint8  *pZTPoint;
	int8   *pDir;

	uint8  l_u8InsertPos = 0;     //�����λ��
	uint8  TraceDir = 0;          //0:�켣����Left��,1:�켣����Right��
	uint8  ZTCnt = 0;
	uint8  WheelCnt = 0;
	
	uint8 i = 0;
	uint8 ZTID = 0;
	uint8 X = 0, Y = 0;
//	uint32 l_u32TimeNow = 0;
//	uint8 MaxScore = 0;
//	uint8 MaxScorePos = 0;
//	uint8 ThrPassPos = 0xff;   //ѹ��켣λ��
//	uint8 DueTimePos = 0;
//	uint32 DueTime = 0;    //��ʱ�洢��ʱʱ��
	
	l_u16TraceList   = Address[0];
	ZTXpos           = Address[1];
	ZTYpos           = Address[2];
	Spd              = Address[3];
	pZTCnt           = Address[4];
	l_u8Score        = Address[5];
	l_u8ZTIDPerTrace = Address[6];
	WheelTrace       = Address[7];
	pInsertPos       = Address[8];
	pTraceDir        = Address[9];
	pWheelCnt        = Address[10];
	pWheelZTake      = Address[11];
	pZTPoint         = Address[12];
	pDir             = Address[13];
	
	ZTCnt = *pZTCnt;
	l_u8InsertPos = *pInsertPos;
	TraceDir = *pTraceDir;
	WheelCnt = *pWheelCnt;

	ZTID = l_u8ZTIDPerTrace[ZTCnt-1];    //��ǰ�켣�����һ��խ��
	FTransLocation(ZTID, &X, &Y, 2);
	

	//���켣���뵱ǰխ���ṹ�����Ӧλ��
	if(TraceDir == 0)   //����Left��
	{
		memcpy(WheelTrace->LeftWhlZTID[l_u8InsertPos], l_u8ZTIDPerTrace, sizeof(uint8)*6);
		memcpy(WheelTrace->LeftWhlZTPos[l_u8InsertPos], l_u16TraceList, sizeof(uint16)*6);
		WheelTrace->LeftWhlZTCntPerTrace[l_u8InsertPos] = ZTCnt;
		memcpy(WheelTrace->LeftWhlZTXPos[l_u8InsertPos], ZTXpos, sizeof(uint16)*6);
		memcpy(WheelTrace->LeftWhlZTYPos[l_u8InsertPos], ZTYpos, sizeof(uint16)*6);
		memcpy(WheelTrace->LeftWhlSpd[l_u8InsertPos], Spd, sizeof(uint16)*6);
		memcpy(WheelTrace->LeftWhlZTake[l_u8InsertPos], pWheelZTake, sizeof(uint8)*6);
		memcpy(WheelTrace->LeftPoint[l_u8InsertPos], pZTPoint, sizeof(uint8)*6);
		
		for(i=0; i<ZTCnt-1; i++)
		{
			WheelTrace->LeftWhlDiffXPos[l_u8InsertPos][i] = ZTXpos[i+1] - ZTXpos[i];
		}
		WheelTrace->LeftTraceCnt++;
		if(WheelTrace->LeftTraceCnt > 6)
		{
			WheelTrace->LeftTraceCnt = 6;
		}
		WheelTrace->LastXPos = ZTXpos[ZTCnt-1];
		WheelTrace->LastYPos = ZTYpos[ZTCnt-1];
		WheelTrace->Dir = *pDir;
		WheelTrace->WheelCnt = WheelCnt;
		WheelTrace->HaveTraceFlag = 1;
		WheelTrace->LeftPoScore[l_u8InsertPos] = l_u8Score[0];
		WheelTrace->LeftSpdScore[l_u8InsertPos] = l_u8Score[1];
		WheelTrace->LeftRadScore[l_u8InsertPos] = l_u8Score[2];
		WheelTrace->LeftMultiScore[l_u8InsertPos] = l_u8Score[3];
		WheelTrace->LeftWhlStatus[l_u8InsertPos] = l_u8Score[4];
/*		
//		//����ĩ����ʱʱ��
//		if(*pDir == 1)    //����
//		{
//			if(Y > 1 && ZTCnt > 1)    //ĩ��
//			{
//				for(i=0; i<WheelTrace->LeftTraceCnt; i++)
//				{
//					//��߷�λ��(���ڼ�����ʱʱ��)
//					if(WheelTrace->LeftMultiScore[i] > MaxScore)
//					{
//						MaxScore = WheelTrace->LeftMultiScore[i];
//						MaxScorePos = i;
//					}
//					//�׸�խ��������ѹ��λ��(���ڼ�����ʱʱ��)
//					if(i == 0)
//					{
//						if(WheelTrace->LeftWhlStatus[i] == YAFENG)
//						{
//							ThrPassPos = i;
//						}
//					}
//					else
//					{
//						if(((WheelTrace->RightWhlStatus[i] == YAFENG) && (WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[ThrPassPos])) ||
//							(ThrPassPos == 0xff))
//						{
//							ThrPassPos = i;
//						}
//					}
//				}
//				if(ThrPassPos < 6)
//					DueTimePos = ThrPassPos;
//				else
//					DueTimePos = MaxScorePos;
//				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
//				
//				//����ĩ����ʱʱ��
//				if(WheelTrace->LeftWhlSpd[DueTimePos][0] > 0)
//				{
//					//ĩ��խ�����������ĩ��խ������ʱʱ��,ĩ��խ�������������ǰһ��խ������ʱʱ��
//					if(Y > 3)
//					{
//#if NORMALLAYOUT
//						//�ԳƲ��ּ�����ʱʱ��
//		 				if(Y == 4)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->LeftWhlSpd[DueTimePos][0] > 0)
//									DueTime = (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][0];
//								WheelTrace->LeftDueTime = l_u32TimeNow + DueTime;   //������ĩ����ʱʱ��
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 							}
//						}
//		 				else if(Y == 5)
//		 					WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][0];   //ż����ĩ����ʱʱ��
//#else
//						//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��(������ĩ�ż��1.2m)
//						if(Y == 4)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									DueTime = (120 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];
//								WheelTrace->LeftDueTime = l_u32TimeNow + DueTime;    //������ĩ����ʱʱ��
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 							}
//						}
//						else if(Y == 5)
//						{
//							if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//						}
//#endif
//					}
//					else if(Y == 3)    //ż���еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID+1] == 0)    //��ǰ��ĩ��խ������
//						{
//							//������ĩ��խ����������
//							if((ZTID > 2 && g_Setup.an8ZTStatus[ZTID-2] == 1) || (g_Setup.an8ZTStatus[ZTID+4] == 1))
//							{
//#if NORMALLAYOUT
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + (40 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//#endif
//							}
//							else    //������ĩ��խ��������
//							{
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//							}
//						}
//					}
//					else if(Y == 2)    //�����еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID+1] == 0)    //��ǰ��ĩ��խ������
//						{
//							//������ĩ��խ����������
//							if((ZTID > 3 && g_Setup.an8ZTStatus[ZTID-3] == 1) || ((ZTID+3) < 2*g_cProModParam.m_u8LaneNum*3-1 && g_Setup.an8ZTStatus[ZTID+3] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (3*PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (140 + 120 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 								}
//#endif
//							}
//							else     //������ĩ��խ��������
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//								else
//								{
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//								}
//#endif
//							}
//						}
//					}
//				}
//			}
//		}
//		else if(*pDir == -1)    //����
//		{
//			if(Y < 2*PROMODPARAM.m_u8ZTRow-2 && ZTCnt > 1)    
//			{
//				for(i=0; i<WheelTrace->LeftTraceCnt; i++)
//				{
//					//��߷�λ��(���ڼ�����ʱʱ��)
//					if(WheelTrace->LeftMultiScore[i] > MaxScore)
//					{
//						MaxScore = WheelTrace->LeftMultiScore[i];
//						MaxScorePos = i;
//					}
//					//�׸�խ��������ѹ��λ��(���ڼ�����ʱʱ��)
//					if(i == 0)
//					{
//						if(WheelTrace->LeftWhlStatus[i] == YAFENG)
//						{
//							ThrPassPos = i;
//						}
//					}
//					else
//					{
//						if(((WheelTrace->RightWhlStatus[i] == YAFENG) && (WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[ThrPassPos])) ||
//							(ThrPassPos == 0xff))
//						{
//							ThrPassPos = i;
//						}
//					}
//				}
//				if(ThrPassPos < 6)
//					DueTimePos = ThrPassPos;
//				else
//					DueTimePos = MaxScorePos;
//				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
//				
//				//����ĩ����ʱʱ��
//				if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//				{
//					//����խ���������������խ������ʱʱ��,����խ�������������ǰһ��խ������ʱʱ��
//					if(Y < 2)
//					{
//#if NORMALLAYOUT
//						//�ԳƲ��ּ�����ʱʱ��
//		 				if(Y == 1)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if( WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000;   
//// 							}
//						}
//		 				else if(Y == 0)
//						{
//							if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//						}
//#else
//						//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��(���������ż��1.2m)
//						if(Y == 1)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + (80 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   //������������ʱʱ��
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 							}
//						}
//						else if(Y == 0)
//						{
//							if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//						}
//#endif
//					}
//					else if(Y == 2)    //�����еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID-1] == 0)    //��ǰ������խ������
//						{
//							//����������խ����������
//							if((ZTID > 4 && g_Setup.an8ZTStatus[ZTID-4] == 1) || (g_Setup.an8ZTStatus[ZTID+2] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (60 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//							else    //����������խ��������
//							{
//								if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//							}
//						}
//					}
//					else if(Y == 3)    //ż���еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID-1] == 0)    //��ǰ������խ������
//						{
//							//����������խ����������
//							if((ZTID > 3 && g_Setup.an8ZTStatus[ZTID-3] == 1) || ((ZTID+3) < 2*g_cProModParam.m_u8LaneNum*3-1 && g_Setup.an8ZTStatus[ZTID+3] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (3*PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (140 + 60 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//							else     //����������խ��������
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->LeftWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->LeftDueTime = l_u32TimeNow + (100 + g_u16EvenColumDelyLth)*6000 / WheelTrace->LeftWhlSpd[DueTimePos][WheelTrace->LeftWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->LeftDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//						}
//					}
//				}
//			}
//		}
*/
	}
	else    //����Right
	{
		memcpy(WheelTrace->RightWhlZTID[l_u8InsertPos], l_u8ZTIDPerTrace, sizeof(uint8)*6);
		memcpy(WheelTrace->RightWhlZTPos[l_u8InsertPos], l_u16TraceList, sizeof(uint16)*6);
		WheelTrace->RightWhlZTCntPerTrace[l_u8InsertPos] = ZTCnt;
		memcpy(WheelTrace->RightWhlZTXPos[l_u8InsertPos], ZTXpos, sizeof(uint16)*6);
		memcpy(WheelTrace->RightWhlZTYPos[l_u8InsertPos], ZTYpos, sizeof(uint16)*6);
		memcpy(WheelTrace->RightWhlSpd[l_u8InsertPos], Spd, sizeof(uint16)*6);
		memcpy(WheelTrace->RightWhlZTake[l_u8InsertPos], pWheelZTake, sizeof(uint8)*6);
		memcpy(WheelTrace->RightPoint[l_u8InsertPos], pZTPoint, sizeof(uint8)*6);
		
		for(i=0; i<ZTCnt-1; i++)
		{
			WheelTrace->RightWhlDiffXPos[l_u8InsertPos][i] = ZTXpos[i+1] - ZTXpos[i];
		}
		WheelTrace->RightTraceCnt++;
		if(WheelTrace->RightTraceCnt > 6)
		{
			WheelTrace->RightTraceCnt = 6;
		}
		WheelTrace->LastXPos = ZTXpos[ZTCnt-1];
		WheelTrace->LastYPos = ZTYpos[ZTCnt-1];
		WheelTrace->Dir = *pDir;
		WheelTrace->WheelCnt = WheelCnt;
		WheelTrace->HaveTraceFlag = 1;
		WheelTrace->RightPoScore[l_u8InsertPos] = l_u8Score[0];
		WheelTrace->RightSpdScore[l_u8InsertPos] = l_u8Score[1];
		WheelTrace->RightRadScore[l_u8InsertPos] = l_u8Score[2];
		WheelTrace->RightMultiScore[l_u8InsertPos] = l_u8Score[3];
		WheelTrace->RightWhlStatus[l_u8InsertPos] = l_u8Score[4];
/*		
//		if(*pDir == 1)    //����
//		{
//			//����������ĩ����ʱʱ��
//			if(Y > 3 && ZTCnt > 1)    //������ĩ��
//			{
//				for(i=0; i<WheelTrace->RightTraceCnt; i++)
//				{
//					//��߷�λ��(���ڼ�����ʱʱ��)
//					if(WheelTrace->RightMultiScore[i] > MaxScore)
//					{
//						MaxScore = WheelTrace->RightMultiScore[i];
//						MaxScorePos = i;
//					}
//					//�׸�խ��������ѹ��λ��(���ڼ�����ʱʱ��)
//					if(i == 0)
//					{
//						if(WheelTrace->RightWhlStatus[i] == 5)
//						{
//							ThrPassPos = i;
//						}
//					}
//					else
//					{
//						if(((WheelTrace->RightWhlStatus[i] == YAFENG) && (WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[ThrPassPos])) ||
//							(ThrPassPos == 0xff))
//						{
//							ThrPassPos = i;
//						}
//					}
//				}
//				if(ThrPassPos < 6)
//					DueTimePos = ThrPassPos;
//				else
//					DueTimePos = MaxScorePos;
//				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
//				//����ĩ����ʱʱ��
//				if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//				{
//					//ĩ��խ�����������ĩ��խ������ʱʱ��,ĩ��խ�������������ǰһ��խ������ʱʱ��
//					if(Y > 3)
//					{
//#if NORMALLAYOUT
//						//�ԳƲ��ּ�����ʱʱ��
//		 				if(Y == 4)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									DueTime = (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];
//								WheelTrace->RightDueTime = l_u32TimeNow + DueTime;   //������ĩ����ʱʱ��
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 							}
//						}
//		 				else if(Y == 5)
//						{
//							if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//						}
//#else
//						//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��(������ĩ�ż��1.2m)
//						if(Y == 4)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									DueTime = (120 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];
//								WheelTrace->RightDueTime = l_u32TimeNow + DueTime;    //������ĩ����ʱʱ��
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 							}
//						}
//						else if(Y == 5)
//						{
//							if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//						}
//#endif
//					}
//					else if(Y == 3)    //ż���еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID+1] == 0)    //��ǰ��ĩ��խ������
//						{
//							//������ĩ��խ����������
//							if((ZTID > 2 && g_Setup.an8ZTStatus[ZTID-2] == 1) || (g_Setup.an8ZTStatus[ZTID+4] == 1))
//							{
//#if NORMALLAYOUT
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + (40 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//#endif
//							}
//							else    //������ĩ��խ��������
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //ż����ĩ����ʱʱ��
//							}
//						}
//					}
//					else if(Y == 2)    //�����еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID+1] == 0)    //��ǰ��ĩ��խ������
//						{
//							//������ĩ��խ����������
//							if((ZTID > 3 && g_Setup.an8ZTStatus[ZTID-3] == 1) || ((ZTID+3) < 2*g_cProModParam.m_u8LaneNum*3-1 && g_Setup.an8ZTStatus[ZTID+3] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (3*PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (140 + 120 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//								}
//#endif
//							}
//							else     //������ĩ��խ��������
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������ĩ����ʱʱ��
//								}
//								else
//								{
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   //������ĩ����ʱʱ��
//								}
//#endif
//							}
//						}
//					}
//				}
//			}
//		}
//		else if(*pDir == -1)    //����
//		{
//			if(Y < 2*PROMODPARAM.m_u8ZTRow-2 && ZTCnt > 1)    
//			{
//				for(i=0; i<WheelTrace->RightTraceCnt; i++)
//				{
//					//��߷�λ��(���ڼ�����ʱʱ��)
//					if(WheelTrace->RightMultiScore[i] > MaxScore)
//					{
//						MaxScore = WheelTrace->RightMultiScore[i];
//						MaxScorePos = i;
//					}
//					//�׸�խ��������ѹ��λ��(���ڼ�����ʱʱ��)
//					if(i == 0)
//					{
//						if(WheelTrace->RightWhlStatus[i] == YAFENG)
//						{
//							ThrPassPos = i;
//						}
//					}
//					else
//					{
//						if(((WheelTrace->RightWhlStatus[i] == YAFENG) && (WheelTrace->RightWhlZTCntPerTrace[i] > WheelTrace->RightWhlZTCntPerTrace[ThrPassPos])) ||
//							(ThrPassPos == 0xff))
//						{
//							ThrPassPos = i;
//						}
//					}
//				}
//				if(ThrPassPos < 6)
//					DueTimePos = ThrPassPos;
//				else
//					DueTimePos = MaxScorePos;
//				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
//				
//				//����ĩ����ʱʱ��
//				if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//				{
//					//����խ���������������խ������ʱʱ��,����խ�������������ǰһ��խ������ʱʱ��
//					if(Y < 2)
//					{
//#if NORMALLAYOUT
//						//�ԳƲ��ּ�����ʱʱ��
//		 				if(Y == 1)
//						{
//// 							if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   
//							}
//// 							else    //��������ͣͣʱ�������ʱ
//// 							{
//// 								WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 							}
//						}
//		 				else if(Y == 0)
//						{
//							if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];  
//						}							
//#else
//						//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��(���������ż��1.2m)
//						if(Y == 1)
//						{
//							if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + (80 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   //������������ʱʱ��
//							}
//							else    //��������ͣͣʱ�������ʱ
//							{
//								WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//							}
//						}
//						else if(Y == 0)
//						{
//							if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//								WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2]; 
//						}							
//#endif
//					}
//					else if(Y == 2)    //�����еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID-1] == 0)    //��ǰ������խ������
//						{
//							//����������խ����������
//							if((ZTID > 4 && g_Setup.an8ZTStatus[ZTID-4] == 1) || (g_Setup.an8ZTStatus[ZTID+2] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (60 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//							else    //����������խ��������
//							{
//								if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16EvenColumDelyLth*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   
//							}
//						}
//					}
//					else if(Y == 3)    //ż���еڶ���
//					{
//						if(g_Setup.an8ZTStatus[ZTID-1] == 0)    //��ǰ������խ������
//						{
//							//����������խ����������
//							if((ZTID > 3 && g_Setup.an8ZTStatus[ZTID-3] == 1) || ((ZTID+3) < 2*g_cProModParam.m_u8LaneNum*3-1 && g_Setup.an8ZTStatus[ZTID+3] == 1))
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (3*PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (140 + 60 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//							else     //����������խ��������
//							{
//#if NORMALLAYOUT
//// 								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (PROMODPARAM.m_au16ZTGap[ZTID-1]/2/10 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];   
//								}
//// 								else    //��������ͣͣʱ�������ʱ
//// 								{
//// 									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//// 								}
//#else
//								//�ϴ�����ģ��,���ڼ�಻�Գ�,������ʱʱ����Ҫ�������ü��
//								if((WheelTrace->WheelCnt == 2 && WheelTrace->RightWhlStatus[DueTimePos] != YAFENG) || g_u8RunAndWalkEnable == 0)    //˫�ֻ�������ͣͣ�ر�ʱ����������ʱ
//								{
//									if(WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2] > 0)
//										WheelTrace->RightDueTime = l_u32TimeNow + (100 + g_u16EvenColumDelyLth)*6000 / WheelTrace->RightWhlSpd[DueTimePos][WheelTrace->RightWhlZTCntPerTrace[DueTimePos]-2];  
//								}
//								else    //��������ͣͣʱ�������ʱ
//								{
//									WheelTrace->RightDueTime = l_u32TimeNow + g_u16RunAndWalkTime*6000;   
//								}
//#endif
//							}
//						}
//					}
//				}
//			}
//		}
*/
	}
}

/*******************************************************************************************************
** Function name:  		FDeleteTrace
** Descriptions:   		ɾ���¹켣
** input parameters:	
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180612
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FDeleteTrace(void * Address[4])
{
	TraceWhel_Struct *WheelTrace;
	TraceWhel_Struct tempWheelTrace;
	TraceWhel_Struct (*pWheelTrace)[4];

	uint8 *pTraceDir;    
	void *l_Address[5];
	uint8 TracePos;
	uint8 i, j, k, m, n, temp;
	uint8 l_u8temp;
	uint8 ColumMin=0xff, ColumMax=0, Colum=0;

	uint8 ZTID = 0;
	uint8 WheelCache = WHEELCACHE;
	uint8 TraceCnt = 0;    //�켣��
	uint8 ZTCnt = 0;
	uint8 TraceZTCnt = 0, tempTraceZTCnt = 0;  //�켣����խ����
	uint8 DelFlag = 0;   
	uint8 LeftDelPos[6][6];     //ÿ��������ɾ���Ĺ켣λ��
	uint8 RightDelPos[6][6];     //ÿ��������ɾ���Ĺ켣λ��
	
	WheelTrace = Address[0];
	pTraceDir = Address[1];
	
	memcpy(&tempWheelTrace, WheelTrace, sizeof(TraceWhel_Struct));   //�ݴ��Ѿ����ֵ�խ���Ĺ켣��Ϣ
	if(*pTraceDir == 0)
	{
		TracePos = WheelTrace->LeftFirstTracePos;
		ZTCnt = WheelTrace->LeftWhlZTCntPerTrace[TracePos];
	}
	else
	{
		TracePos = WheelTrace->RightFirstTracePos;
		ZTCnt = WheelTrace->RightWhlZTCntPerTrace[TracePos];
	}
	
	//�ҳ���ǰ�켣������������к����Ҳ���
	for(i=0; i<((ZTCnt<=6)?ZTCnt:6); i++)
	{
		if(*pTraceDir == 0)
			Colum = g_ChlData[WheelTrace->LeftWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
		else
			Colum = g_ChlData[WheelTrace->RightWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
		if(Colum > ColumMax)
		{
			ColumMax = Colum;
		}
		if(Colum < ColumMin)
		{
			ColumMin = Colum;
		}
	}

	//������չ1��,ɾ�������Ѿ����ֵ�խ���Ĺ켣
	for(l_u8temp=0; l_u8temp<2; l_u8temp++)
	{
		if(l_u8temp == 0)    //����
		{
			pWheelTrace = g_WheelTrace;
		}
		else    //����
		{
			pWheelTrace = g_ReWheelTrace;
		}
		
		for(temp=0; temp<4; temp++)
		{
			switch(temp)
			{
				case 0:
					Colum = ColumMin-1;
					break;
				case 1:
					Colum = ColumMax+1;
					break;
				case 2:
					Colum = ColumMin;
					break;
				case 3:
					Colum = ColumMax;
					break;
				default:
					break;
			}
			if(Colum >= PROMODPARAM.m_u8LaneNum*2)
			{
				continue;
			}
			for(i=0; i<PROMODPARAM.m_u8ZTRow; i++)
			{
				ZTID = Colum*PROMODPARAM.m_u8ZTRow+i;
				memset(LeftDelPos, -1, sizeof(uint8)*6*6);
				memset(RightDelPos, -1, sizeof(uint8)*6*6);
				
				//Left���ֱ���
				for(j=0; j<WheelCache; j++)
				{
					TraceCnt = pWheelTrace[ZTID][j].LeftTraceCnt;
					for(k=0; k<((TraceCnt<=6)?TraceCnt:6); k++)   //�켣��
					{
						TraceZTCnt = pWheelTrace[ZTID][j].LeftWhlZTCntPerTrace[k];
						for(m=0; m<((TraceZTCnt<=6)?TraceZTCnt:6); m++)  //ÿ���켣խ����
						{
							tempTraceZTCnt = (*pTraceDir == 0) ? tempWheelTrace.LeftWhlZTCntPerTrace[TracePos] : tempWheelTrace.RightWhlZTCntPerTrace[TracePos];
							for(n=0; n<((tempTraceZTCnt<=6)?tempTraceZTCnt:6); n++)
							{
								if(*pTraceDir == 0)
								{
									if((pWheelTrace[ZTID][j].LeftWhlZTPos[k][m] == tempWheelTrace.LeftWhlZTPos[TracePos][n]) &&
									   (pWheelTrace[ZTID][j].LeftWhlZTake[k][m] == tempWheelTrace.LeftWhlZTake[TracePos][n]))
									{
										//��¼ɾ���켣��λ��
										LeftDelPos[j][k] = 1;
										DelFlag = 1;
									}
								}
								else if(*pTraceDir == 1)
								{
									if((pWheelTrace[ZTID][j].LeftWhlZTPos[k][m] == tempWheelTrace.RightWhlZTPos[TracePos][n]) &&
									   (pWheelTrace[ZTID][j].LeftWhlZTake[k][m] == tempWheelTrace.RightWhlZTake[TracePos][n]))
									{
										//��¼ɾ���켣��λ��
										LeftDelPos[j][k] = 1;
										DelFlag = 1;
									}
								}
								if(DelFlag)
								{
									DelFlag = 0;
									break;
								}
							}
						}
					}
					
					//Right���ֱ���
					TraceCnt = pWheelTrace[ZTID][j].RightTraceCnt;
					for(k=0; k<((TraceCnt<=6)?TraceCnt:6); k++)   //�켣��
					{
						TraceZTCnt = pWheelTrace[ZTID][j].RightWhlZTCntPerTrace[k];
						for(m=0; m<((TraceZTCnt<=6)?TraceZTCnt:6); m++)  //ÿ���켣խ����
						{
							tempTraceZTCnt = (*pTraceDir == 0) ? tempWheelTrace.LeftWhlZTCntPerTrace[TracePos] : tempWheelTrace.RightWhlZTCntPerTrace[TracePos];
							for(n=0; n<((tempTraceZTCnt<=6)?tempTraceZTCnt:6); n++)
							{
								if(*pTraceDir == 0)
								{
									if((pWheelTrace[ZTID][j].RightWhlZTPos[k][m] == tempWheelTrace.LeftWhlZTPos[TracePos][n]) &&
									   (pWheelTrace[ZTID][j].RightWhlZTake[k][m] == tempWheelTrace.LeftWhlZTake[TracePos][n]))
									{
										//��¼ɾ���켣��λ��
										RightDelPos[j][k] = 1;
										DelFlag = 1;
									}
								}
								else if(*pTraceDir == 1)
								{
									if((pWheelTrace[ZTID][j].RightWhlZTPos[k][m] == tempWheelTrace.RightWhlZTPos[TracePos][n]) &&
									   (pWheelTrace[ZTID][j].RightWhlZTake[k][m] == tempWheelTrace.RightWhlZTake[TracePos][n]))
									{
										//��¼ɾ���켣��λ��
										RightDelPos[j][k] = 1;
										DelFlag = 1;
									}
								}
								if(DelFlag)
								{
									DelFlag = 0;
									break;
								}
							}
						}
					}
				}
				
				//ɾ��������켣
				l_Address[0] = &pWheelTrace[ZTID][0];
				l_Address[1] = LeftDelPos;
				l_Address[2] = RightDelPos;
				FArrangeTrace(l_Address);
			}
			if(ColumMin == ColumMax && temp==2)
			{
				break;
			}
		}
	}
}

/*******************************************************************************************************
** Function name:  		FClearTrace
** Descriptions:   		����켣
** input parameters:	cmd--1:��ʱɾ�켣ģʽ,2:�����׳�����Ϣɾ�켣
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180613
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FClearTrace(void * Address[6], uint8 cmd)
{
	TraceWhel_Struct (*WheelTrace)[WHEELCACHE];
	uint8 i, j, k;
	uint32 l_u32TimeNow;
	uint16 ZTPos;
	uint8  DelRecord[WHEELCACHE];    //��¼��Ҫɾ���Ļ���켣��λ��, 0:δ��ʱ,����Ҫɾ��, 1:��Ҫɾ��
	uint16 ZTPosBuff[WHEELCACHE];
	uint8  DelFlag = 0;     //0:û�г�ʱ��,1:�г�ʱ
	uint8  temp = 0;
	uint32 *pStartTime = NULL, *pEndTime = NULL;
	uint16 *pLeftXPos = NULL, *pRightXPos = NULL;
	uint8  *pStartZTID = NULL, *pEndZTID = NULL;
	uint8  X, Y;
	uint8 StartZTID = 0, EndZTID = 0;
	uint8 DelZTake = 0;   //0x01:Left,0x02:Right,0x03:ȫ��, Ĭ��Ϊ0
	uint16 LeftXPos = 0, RightXPos = 0;
	
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	
	switch(cmd)
	{
		case 1:
			StartZTID = 0;
			EndZTID = WHEELTRACEZTNUM;
			break;
		case 2:
			pStartTime = Address[0];   //��һ��խ�����³�ʱ��
			pEndTime = Address[1];     //�����һ��խ�����³�ʱ��
			pLeftXPos = Address[2];    //�������Xλ��
			pRightXPos = Address[3];   //�����Ҳ�Xλ��
			pStartZTID = Address[4];   
			pEndZTID = Address[5];   
		
			StartZTID = *pStartZTID;
			EndZTID = *pEndZTID;
			break;
		default :
			break;
	}

	memset(DelRecord, 0, sizeof(DelRecord));
	memset(ZTPosBuff, 0, sizeof(ZTPosBuff));
	ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
	
	//����ȫ���켣,ɾ����ʱ�켣,������ʣ�µĹ켣(��������򶼱���)
	for(k = 0; k < 2; k++)
	{
		if(k == 0)
			WheelTrace = g_WheelTrace;
		else
			WheelTrace = g_ReWheelTrace;
		for(i = StartZTID; i < EndZTID; i++)
		{
			DelFlag = 0;
			memset(DelRecord, 0, sizeof(DelRecord));
			memset(ZTPosBuff, 0, sizeof(ZTPosBuff));
			for(j = 0; j < WHEELCACHE; j++)
			{
				if(WheelTrace[i][j].HaveTraceFlag)
				{
					ZTPos = WheelTrace[i][j].LeftWhlZTPos[0][WheelTrace[i][j].LeftWhlZTCntPerTrace[0]-1];
					if(cmd == 1)    //��ʱɾ��
					{
						if(l_u32TimeNow > g_ChlData[ZTPos].u32dntm + g_u16TraceDelTime*6000)   //��ǰխ���Ĺ켣��ʱ
						{
							DelRecord[j] = 1;
							ZTPosBuff[j] = ZTPos;
							DelFlag = 1;
							DelZTake = 3;
						}
					}
					else if(cmd == 2)    //���ݴ�����׳���Ϣɾ�켣
					{
						FTransLocation(g_ChlData[ZTPos].u8Id, &X, &Y, 2);
						//ĩ��խ��ֻҪ������ϼ���ɾ��,����խ����Ҫʱ��;���ͬʱ�����ɾ��
						if(WheelTrace[i][j].WheelCnt == 1)
						{
							if(((Y == 5 && k == 1) || (Y == 0 && k == 0)) &&
								(WheelTrace[i][j].LastXPos >= *pLeftXPos && WheelTrace[i][j].LastXPos <= *pRightXPos))    
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 2;
								DelZTake = 3;
							}
							else if((WheelTrace[i][j].LastXPos >= *pLeftXPos && WheelTrace[i][j].LastXPos <= *pRightXPos) &&
								(g_ChlData[ZTPos].u32dntm >= *pStartTime && g_ChlData[ZTPos].u32dntm <= *pEndTime))
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 3;
								DelZTake = 3;
							}
						}
						else if(WheelTrace[i][j].WheelCnt == 2)
						{
							LeftXPos = WheelTrace[i][j].LeftWhlZTXPos[0][WheelTrace[i][j].LeftWhlZTCntPerTrace[0]-1];
							RightXPos = WheelTrace[i][j].RightWhlZTXPos[0][WheelTrace[i][j].RightWhlZTCntPerTrace[0]-1];
							//Left
							if(((Y == 5 && k == 1) || (Y == 0 && k == 0)) && (LeftXPos >= *pLeftXPos && LeftXPos <= *pRightXPos))    
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 4;
								DelZTake |= 0x01;
							}
							else if((LeftXPos >= *pLeftXPos && LeftXPos <= *pRightXPos) &&
								(g_ChlData[ZTPos].u32dntm >= *pStartTime && g_ChlData[ZTPos].u32dntm <= *pEndTime))
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 5;
								DelZTake |= 0x01;
							}
							//Right
							if(((Y == 5 && k == 1) || (Y == 0 && k == 0)) && (RightXPos >= *pLeftXPos && RightXPos <= *pRightXPos))    
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 6;
								DelZTake |= 0x02;
							}
							else if((RightXPos >= *pLeftXPos && RightXPos <= *pRightXPos) &&
								(g_ChlData[ZTPos].u32dntm >= *pStartTime && g_ChlData[ZTPos].u32dntm <= *pEndTime))
							{
								DelRecord[j] = 1;
								ZTPosBuff[j] = ZTPos;
								DelFlag = 7;
								DelZTake |= 0x02;
							}
						}
					}
				}
			}
			if(DelFlag)    //����Ҫɾ���Ļ���켣
			{
				//ɾ����ʱ�Ļ���켣
				for(j = 0; j < WHEELCACHE; j++)
				{
					if(DelRecord[j])
					{
						FDelSingleTrace(&WheelTrace[i][j], DelZTake);   //����ù켣��Ϣ
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nDelTrace: %d,ID=%d,Pos=%d,Take=%02x,t1=%d,t2=%d",
										cmd, g_ChlData[ZTPosBuff[j]].u8Id,ZTPosBuff[j], DelZTake, g_ChlData[ZTPosBuff[j]].u32dntm, l_u32TimeNow);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
					}
				}
				//����ʣ�µĻ���켣
				temp = 0;
				for(j = 0; j < WHEELCACHE; j++)
				{
					if(WheelTrace[i][j].HaveTraceFlag)
					{
						if(j != temp)
						{
							memcpy(&WheelTrace[i][temp], &WheelTrace[i][j], sizeof(TraceWhel_Struct));
							memset(&WheelTrace[i][j], 0, sizeof(TraceWhel_Struct));
						}
						temp++;
					}
				}
			}
		}
	}
}

/*******************************************************************************************************
** Function name:  		FDelSingleTrace
** Descriptions:   		��������켣
** input parameters:	trace:�켣ָ��,dir:1-Left,2-Right,3-All
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180716
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FDelSingleTrace(void *trace, uint8 dir)
{
	TraceWhel_Struct *WheelTrace;
	WheelTrace = trace;
	
	switch(dir)
	{
		case 1:
			memset(WheelTrace->LeftWhlZTID, 0xff, sizeof(WheelTrace->LeftWhlZTID));
			memset(WheelTrace->LeftWhlZTPos, 0xff, sizeof(WheelTrace->LeftWhlZTPos));
			memset(WheelTrace->LeftWhlZTake, 0xff, sizeof(WheelTrace->LeftWhlZTake));
			memset(WheelTrace->LeftWhlZTXPos, 0, sizeof(WheelTrace->LeftWhlZTXPos));
			memset(WheelTrace->LeftWhlZTYPos, 0, sizeof(WheelTrace->LeftWhlZTYPos));
			memset(WheelTrace->LeftWhlSpd, 0, sizeof(WheelTrace->LeftWhlSpd));
			memset(WheelTrace->LeftWhlDiffXPos, 0, sizeof(WheelTrace->LeftWhlDiffXPos));
			memset(WheelTrace->LeftPoint, 0, sizeof(WheelTrace->LeftPoint));
			memset(WheelTrace->LeftWhlZTCntPerTrace, 0, sizeof(WheelTrace->LeftWhlZTCntPerTrace));
		
			memset(WheelTrace->LeftPoScore, 0, sizeof(WheelTrace->LeftPoScore));
			memset(WheelTrace->LeftSpdScore, 0, sizeof(WheelTrace->LeftSpdScore));
			memset(WheelTrace->LeftRadScore, 0, sizeof(WheelTrace->LeftRadScore));
			memset(WheelTrace->LeftMultiScore, 0, sizeof(WheelTrace->LeftMultiScore));
			memset(WheelTrace->LeftWhlStatus, 0, sizeof(WheelTrace->LeftWhlStatus));
		
			WheelTrace->LeftTraceCnt = 0;
			if(WheelTrace->WheelCnt > 0)
				WheelTrace->WheelCnt--;
			WheelTrace->LeftDueTime = 0;
			WheelTrace->LeftFirstTracePos = 0xff;
			WheelTrace->LeftStatus = 0;
			break;
		case 2:
			memset(WheelTrace->RightWhlZTID, 0xff, sizeof(WheelTrace->RightWhlZTID));
			memset(WheelTrace->RightWhlZTPos, 0xff, sizeof(WheelTrace->RightWhlZTPos));
			memset(WheelTrace->RightWhlZTake, 0xff, sizeof(WheelTrace->RightWhlZTake));
			memset(WheelTrace->RightWhlZTXPos, 0, sizeof(WheelTrace->RightWhlZTXPos));
			memset(WheelTrace->RightWhlZTYPos, 0, sizeof(WheelTrace->RightWhlZTYPos));
			memset(WheelTrace->RightWhlSpd, 0, sizeof(WheelTrace->RightWhlSpd));
			memset(WheelTrace->RightWhlDiffXPos, 0, sizeof(WheelTrace->RightWhlDiffXPos));
			memset(WheelTrace->RightPoint, 0, sizeof(WheelTrace->RightPoint));
			memset(WheelTrace->RightWhlZTCntPerTrace, 0, sizeof(WheelTrace->RightWhlZTCntPerTrace));
		
			memset(WheelTrace->RightPoScore, 0, sizeof(WheelTrace->RightPoScore));
			memset(WheelTrace->RightSpdScore, 0, sizeof(WheelTrace->RightSpdScore));
			memset(WheelTrace->RightRadScore, 0, sizeof(WheelTrace->RightRadScore));
			memset(WheelTrace->RightMultiScore, 0, sizeof(WheelTrace->RightMultiScore));
			memset(WheelTrace->RightWhlStatus, 0, sizeof(WheelTrace->RightWhlStatus));
		
			WheelTrace->RightTraceCnt = 0;
			if(WheelTrace->WheelCnt > 0)
				WheelTrace->WheelCnt--;
			WheelTrace->RightDueTime = 0;
			WheelTrace->RightFirstTracePos = 0xff;
			WheelTrace->RightStatus = 0;
			break;
		case 3:
			memset(WheelTrace, 0, sizeof(TraceWhel_Struct));
			break;
		default:
			break;
	}
}
/*******************************************************************************************************
** Function name:  		FMoveTrace
** Descriptions:   		����켣
** input parameters:	ĩ�ų��ֺ�����ʣ�µĹ켣
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180606
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FArrangeTrace(void * Address[6])
{
	TraceWhel_Struct *WheelTrace;
	uint8 *pLeftDelPos;
	uint8 *pRightDelPos;

	uint8 i, j, cnt;
	uint8 WheelCache = WHEELCACHE;
	uint8 l_u8Pos4Del = 0xff;   //�ƶ�����Ҫmemset�Ĺ켣λ��(��ʱ����)
// 	uint8 l_u8Err;
// 	uint16 l_u16SendLen;
	void *l_Address[3];
	
	WheelTrace = Address[0];
	pLeftDelPos = Address[1];
	pRightDelPos = Address[2];

	//ɾ���켣
	for(i=0; i<WheelCache; i++)
	{
		for(j=0; j<6; j++)
		{
			if(WheelTrace[i].HaveTraceFlag)
			{
				if(pLeftDelPos[i*6+j] == 1)
				{
// 					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Arr Left Trace P=%d,%d,%d,%d,%d,%d!!!.\r\n",
// 					WheelTrace[i].LeftWhlZTPos[j][0],WheelTrace[i].LeftWhlZTPos[j][1],WheelTrace[i].LeftWhlZTPos[j][2],
// 					WheelTrace[i].LeftWhlZTPos[j][3],WheelTrace[i].LeftWhlZTPos[j][4],WheelTrace[i].LeftWhlZTPos[j][5]);
// 					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 					g_u16CurDebugInd++;
// 					g_u16CurDebugInd %= COM_NUM;
// 					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 					OSSemPost(g_pSemSendThtouSP);
					
					memset(WheelTrace[i].LeftWhlZTID[j], -1, sizeof(uint8)*6);
					memset(WheelTrace[i].LeftWhlZTPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].LeftWhlZTake[j], -1, sizeof(uint8)*6);
					WheelTrace[i].LeftWhlZTCntPerTrace[j] = 0;
					memset(WheelTrace[i].LeftWhlZTXPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].LeftWhlZTYPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].LeftWhlSpd[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].LeftWhlDiffXPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].LeftPoint[j], 0, sizeof(uint8)*6);
					if(WheelTrace[i].LeftTraceCnt > 0)
						WheelTrace[i].LeftTraceCnt--;
					else
						WheelTrace[i].LeftTraceCnt = 0;
					WheelTrace[i].LeftPoScore[j] = 0;
					WheelTrace[i].LeftSpdScore[j] = 0;
					WheelTrace[i].LeftRadScore[j] = 0;
					WheelTrace[i].LeftMultiScore[j] = 0;
					WheelTrace[i].LeftWhlStatus[j] = 0;
				}
				
				if(pRightDelPos[i*6+j] == 1)
				{
// 					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Arr Right Trace P=%d,%d,%d,%d,%d,%d!!!.\r\n",
// 					WheelTrace[i].RightWhlZTPos[j][0],WheelTrace[i].RightWhlZTPos[j][1],WheelTrace[i].RightWhlZTPos[j][2],
// 					WheelTrace[i].RightWhlZTPos[j][3],WheelTrace[i].RightWhlZTPos[j][4],WheelTrace[i].RightWhlZTPos[j][5]);
// 					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 					g_u16CurDebugInd++;
// 					g_u16CurDebugInd %= COM_NUM;
// 					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 					OSSemPost(g_pSemSendThtouSP);
					
					memset(WheelTrace[i].RightWhlZTID[j], -1, sizeof(uint8)*6);
					memset(WheelTrace[i].RightWhlZTPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].RightWhlZTake[j], -1, sizeof(uint8)*6);
					WheelTrace[i].RightWhlZTCntPerTrace[j] = 0;
					memset(WheelTrace[i].RightWhlZTXPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].RightWhlZTYPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].RightWhlSpd[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].RightWhlDiffXPos[j], -1, sizeof(uint16)*6);
					memset(WheelTrace[i].RightPoint[j], 0, sizeof(uint8)*6);
					if(WheelTrace[i].RightTraceCnt > 0)
						WheelTrace[i].RightTraceCnt--;
					else
						WheelTrace[i].RightTraceCnt = 0;
					WheelTrace[i].RightPoScore[j] = 0;
					WheelTrace[i].RightSpdScore[j] = 0;
					WheelTrace[i].RightRadScore[j] = 0;
					WheelTrace[i].RightMultiScore[j] = 0;
					WheelTrace[i].RightWhlStatus[j] = 0;
				}
				if(WheelTrace[i].LeftTraceCnt==0 && WheelTrace[i].RightTraceCnt>0)
				{
					WheelTrace[i].WheelCnt = 1;
					WheelTrace[i].LeftDueTime = 0;
					WheelTrace[i].LeftFirstTracePos = 0;
					WheelTrace[i].LeftStatus = 0;
				}
				else if(WheelTrace[i].LeftTraceCnt>01 && WheelTrace[i].RightTraceCnt==0)
				{
					WheelTrace[i].WheelCnt = 1;
					WheelTrace[i].RightDueTime = 0;
					WheelTrace[i].RightFirstTracePos = 0;
					WheelTrace[i].RightStatus = 0;
				}
				if(WheelTrace[i].LeftTraceCnt==0 && WheelTrace[i].RightTraceCnt==0)
				{
					WheelTrace[i].LastXPos = 0;
					WheelTrace[i].LastYPos = 0;
					WheelTrace[i].Dir = 0;
					WheelTrace[i].WheelCnt = 0;
					WheelTrace[i].HaveTraceFlag = 0;
					WheelTrace[i].LeftDueTime = 0;
					WheelTrace[i].RightDueTime = 0;
					WheelTrace[i].LeftFirstTracePos = 0;
					WheelTrace[i].RightFirstTracePos = 0;
					WheelTrace[i].LeftStatus = 0;
					WheelTrace[i].RightStatus = 0;
				}
			}
		}
	}

	
	//����켣
	for(i=0; i<WheelCache; i++)
	{
		cnt = 0;
		l_u8Pos4Del = 0xff;
		for(j=0; j<6; j++)
		{
			if((pLeftDelPos[i*6+j] != 1) && (WheelTrace[i].LeftWhlZTCntPerTrace[j] > 0))
			{
				if(j != cnt)
				{
					//�����ƶ�
					memcpy(WheelTrace[i].LeftWhlZTID[cnt], WheelTrace[i].LeftWhlZTID[j], sizeof(uint8)*6);
					memcpy(WheelTrace[i].LeftWhlZTPos[cnt], WheelTrace[i].LeftWhlZTPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].LeftWhlZTake[cnt], WheelTrace[i].LeftWhlZTake[j], sizeof(uint8)*6);
					WheelTrace[i].LeftWhlZTCntPerTrace[cnt] = WheelTrace[i].LeftWhlZTCntPerTrace[j];
					memcpy(WheelTrace[i].LeftWhlZTXPos[cnt], WheelTrace[i].LeftWhlZTXPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].LeftWhlZTYPos[cnt], WheelTrace[i].LeftWhlZTYPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].LeftWhlSpd[cnt], WheelTrace[i].LeftWhlSpd[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].LeftWhlDiffXPos[cnt], WheelTrace[i].LeftWhlDiffXPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].LeftPoint[cnt], WheelTrace[i].LeftPoint[j], sizeof(uint8)*6);
					WheelTrace[i].LeftPoScore[cnt] = WheelTrace[i].LeftPoScore[j];
					WheelTrace[i].LeftSpdScore[cnt] = WheelTrace[i].LeftSpdScore[j];
					WheelTrace[i].LeftRadScore[cnt] = WheelTrace[i].LeftRadScore[j];
					WheelTrace[i].LeftMultiScore[cnt] = WheelTrace[i].LeftMultiScore[j];
					WheelTrace[i].LeftWhlStatus[cnt] = WheelTrace[i].LeftWhlStatus[j];
					
					//��¼���һ���ƶ���ɺ���Ҫmemset�Ĺ켣λ��
					l_u8Pos4Del = j;
				}
				cnt++;
			}
		}
		if(l_u8Pos4Del < 6)
		{
			memset(WheelTrace[i].LeftWhlZTID[l_u8Pos4Del], -1, sizeof(uint8)*6);
			memset(WheelTrace[i].LeftWhlZTPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].LeftWhlZTake[l_u8Pos4Del], -1, sizeof(uint8)*6);
			WheelTrace[i].LeftWhlZTCntPerTrace[l_u8Pos4Del] = 0;
			memset(WheelTrace[i].LeftWhlZTXPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].LeftWhlZTYPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].LeftWhlSpd[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].LeftWhlDiffXPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].LeftPoint[l_u8Pos4Del], 0, sizeof(uint8)*6);
			WheelTrace[i].LeftPoScore[l_u8Pos4Del] = 0;
			WheelTrace[i].LeftSpdScore[l_u8Pos4Del] = 0;
			WheelTrace[i].LeftRadScore[l_u8Pos4Del] = 0;
			WheelTrace[i].LeftMultiScore[l_u8Pos4Del] = 0;
			WheelTrace[i].LeftWhlStatus[l_u8Pos4Del] = 0;
		}
	}
	
	for(i=0; i<WheelCache; i++)
	{
		cnt = 0;
		l_u8Pos4Del = 0xff;
		for(j=0; j<6; j++)
		{
			if((pRightDelPos[i*6+j] != 1) && (WheelTrace[i].RightWhlZTCntPerTrace[j] > 0))
			{
				if(j != cnt)
				{
					//�����ƶ�
					memcpy(WheelTrace[i].RightWhlZTID[cnt], WheelTrace[i].RightWhlZTID[j], sizeof(uint8)*6);
					memcpy(WheelTrace[i].RightWhlZTPos[cnt], WheelTrace[i].RightWhlZTPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].RightWhlZTake[cnt], WheelTrace[i].RightWhlZTake[j], sizeof(uint8)*6);
					WheelTrace[i].RightWhlZTCntPerTrace[cnt] = WheelTrace[i].RightWhlZTCntPerTrace[j];
					memcpy(WheelTrace[i].RightWhlZTXPos[cnt], WheelTrace[i].RightWhlZTXPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].RightWhlZTYPos[cnt], WheelTrace[i].RightWhlZTYPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].RightWhlSpd[cnt], WheelTrace[i].RightWhlSpd[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].RightWhlDiffXPos[cnt], WheelTrace[i].RightWhlDiffXPos[j], sizeof(uint16)*6);
					memcpy(WheelTrace[i].RightPoint[cnt], WheelTrace[i].RightPoint[j], sizeof(uint8)*6);
					WheelTrace[i].RightPoScore[cnt] = WheelTrace[i].RightPoScore[j];
					WheelTrace[i].RightSpdScore[cnt] = WheelTrace[i].RightSpdScore[j];
					WheelTrace[i].RightRadScore[cnt] = WheelTrace[i].RightRadScore[j];
					WheelTrace[i].RightMultiScore[cnt] = WheelTrace[i].RightMultiScore[j];
					WheelTrace[i].RightWhlStatus[cnt] = WheelTrace[i].RightWhlStatus[j];
					
					//��¼���һ���ƶ���ɺ���Ҫmemset�Ĺ켣λ��
					l_u8Pos4Del = j;
				}
				cnt++;
			}
		}
		if(l_u8Pos4Del < 6)
		{
			memset(WheelTrace[i].RightWhlZTID[l_u8Pos4Del], -1, sizeof(uint8)*6);
			memset(WheelTrace[i].RightWhlZTPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].RightWhlZTake[l_u8Pos4Del], -1, sizeof(uint8)*6);
			WheelTrace[i].RightWhlZTCntPerTrace[l_u8Pos4Del] = 0;
			memset(WheelTrace[i].RightWhlZTXPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].RightWhlZTYPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].RightWhlSpd[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].RightWhlDiffXPos[l_u8Pos4Del], -1, sizeof(uint16)*6);
			memset(WheelTrace[i].RightPoint[l_u8Pos4Del], 0, sizeof(uint8)*6);
			WheelTrace[i].RightPoScore[l_u8Pos4Del] = 0;
			WheelTrace[i].RightSpdScore[l_u8Pos4Del] = 0;
			WheelTrace[i].RightRadScore[l_u8Pos4Del] = 0;
			WheelTrace[i].RightMultiScore[l_u8Pos4Del] = 0;
			WheelTrace[i].RightWhlStatus[l_u8Pos4Del] = 0;
		}
	}
	
	//խ��������ǰ��
	cnt = 0;
	for(i=0; i<WheelCache; i++)
	{
		if(WheelTrace[i].HaveTraceFlag>0)
		{
			cnt = i;
			break;
		}
	}
	if(cnt > 0)
	{
		for(i=0; i<WheelCache-cnt; i++)
		{
			if(WheelTrace[cnt+i].HaveTraceFlag>0)
			{
				memcpy(&WheelTrace[i], &WheelTrace[cnt+i], sizeof(TraceWhel_Struct));
				l_Address[0] = &WheelTrace[cnt+i];
				FInitTrace(l_Address);
			}
		}
	}
}

/*******************************************************************************************************
** Function name:  		FInitTrace
** Descriptions:   		��ʼ��ĳ��խ����ĳ���켣
** input parameters:	խ������
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180614
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FInitTrace(void * Address[2])
{
	TraceWhel_Struct *WheelTrace;
	
	WheelTrace = Address[0];
	
	memset(WheelTrace->LeftWhlZTID, -1, sizeof(uint8)*6*6);
	memset(WheelTrace->LeftWhlZTPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->LeftWhlZTake, -1, sizeof(uint8)*6*6);
	memset(WheelTrace->LeftWhlZTCntPerTrace, 0, sizeof(uint8)*6);
	memset(WheelTrace->LeftWhlZTXPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->LeftWhlZTYPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->LeftWhlSpd, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->LeftWhlDiffXPos, -1, sizeof(int16)*6*6);
	memset(WheelTrace->LeftPoint, 0, sizeof(uint8)*6*6);
	WheelTrace->LeftTraceCnt = 0;
	memset(WheelTrace->LeftPoScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->LeftSpdScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->LeftRadScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->LeftMultiScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->LeftWhlStatus, 0, sizeof(uint8)*6);
	
	memset(WheelTrace->RightWhlZTID, -1, sizeof(uint8)*6*6);
	memset(WheelTrace->RightWhlZTPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->RightWhlZTake, -1, sizeof(uint8)*6*6);
	memset(WheelTrace->RightWhlZTCntPerTrace, 0, sizeof(uint8)*6);
	memset(WheelTrace->RightWhlZTXPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->RightWhlZTYPos, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->RightWhlSpd, -1, sizeof(uint16)*6*6);
	memset(WheelTrace->RightWhlDiffXPos, -1, sizeof(int16)*6*6);
	memset(WheelTrace->RightPoint, 0, sizeof(uint8)*6*6);
	WheelTrace->RightTraceCnt = 0;
	memset(WheelTrace->RightPoScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->RightSpdScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->RightRadScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->RightMultiScore, 0, sizeof(uint8)*6);
	memset(WheelTrace->RightWhlStatus, 0, sizeof(uint8)*6);
	
	WheelTrace->LastXPos = 0;
	WheelTrace->LastYPos = 0;
	WheelTrace->Dir = 0;
	WheelTrace->WheelCnt = 0;
	WheelTrace->HaveTraceFlag = 0;
	WheelTrace->LeftDueTime = 0;
	WheelTrace->RightDueTime = 0;
	WheelTrace->LeftFirstTracePos = 0;
	WheelTrace->RightFirstTracePos = 0;
	WheelTrace->LeftStatus = 0;
	WheelTrace->RightStatus = 0;
}

/*******************************************************************************************************
** Function name:  		FTraceScore
** Descriptions:   		Ϊ�켣���
** input parameters:	խ������,խ��������ÿ��խ�����ֵ�X��Yλ��(ÿ��ֻ�ᴫһ����)
** output parameters:	�ù켣���ۺϵ÷�
**
** Created by: liumingxin  
** Created Date: 20180606
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
uint8 FTraceScore(void * Address[6])
{
	uint8 i, j;
	uint16 *ZTTrace;    //խ������
	uint16 *ZTXpos;     //������ÿ��խ����Xλ��(��λ:cm)
	uint16 *ZTYpos;     //������ÿ��խ����Yλ��(��λ:cm)
	uint16 *Spd;
	uint8  *l_u8Score;    //ָ��������λ�õ÷�,�ٶȵ÷�,���ȵ÷�(���)
	uint8 *pZTCnt;
	uint8  ZTCnt;         //��ǰ����խ����
	
	uint16 delta_x = 0;
	uint16 delta_y = 1;
	uint32 delta_spd = 0; //��λmm/s
	int16 deltaX[5];      //�켣��խ�����е�λ�ñ仯ֵ(���ڻ��ȴ�ֺ�����ʻ֤״̬���ж�)
	uint16 deltaY[5];
	uint8  yafeng_buff[6];
	
	//ϵ��
	uint8 c_pos = g_u8PosScoreC;
	uint8 c_spd = g_u8SpdScoreC;
	uint8 c_rad = g_u8RadScoreC;
	//Ȩֵ
	uint8 a_pos = g_u8PosScoreWeight;
	uint8 a_spd = g_u8SpdScoreWeight;
	uint8 a_rad = g_u8RadScoreWeight;    

	uint16 PosScore[6];   //��6��λ�ô洢λ���ܵ÷�
	uint16 SpdScore[6];
	uint16 RadScore[6];
	uint8  ScoreTemp = 0;
	uint8  Score = 0;     //�ۺϵ÷�
	uint8  LeftXieCnt = 0, RightXieCnt = 0;    //��б����,��б����
	uint8  Status = 0;        //��ʻ״̬  1:����,2:��б,3:��б,4:��S  Ĭ��Ϊ0

	ZTTrace = Address[0];
	ZTXpos = Address[1];
	ZTYpos = Address[2];
	Spd = Address[3];
	l_u8Score = Address[5];
	pZTCnt = Address[4];
	
	ZTCnt = *pZTCnt;
	memset(deltaX, 0, sizeof(int16)*5);
	memset(deltaY, 0, sizeof(uint16)*5);
	memset(yafeng_buff, 0, sizeof(uint8)*6);

	for(i =0; i<6; i++)
	{
		PosScore[i] = 100;
		SpdScore[i] = 100;
		RadScore[i] = 100;
	}
	
	//�ж��Ƿ���ѹ��
	for(i=0; i<ZTCnt-1; i++)
	{
		if((GetDiff(g_ChlData[ZTTrace[i]].u8Id/PROMODPARAM.m_u8ZTRow, g_ChlData[ZTTrace[i+1]].u8Id/PROMODPARAM.m_u8ZTRow) == 1) && (GetDiff(ZTXpos[i], ZTXpos[i+1])<g_u8YaFengDiffXPosMax) && 
		   ((g_ChlData[ZTTrace[i]].u8Id < g_ChlData[ZTTrace[i+1]].u8Id && (((g_ChlData[ZTTrace[i]].u8SecondZTUpDnPosCnt&0x0c) && (g_ChlData[ZTTrace[i+1]].u8FirstZTUpDnPosCnt&0x01)) || ((g_ChlData[ZTTrace[i]].u8SecondZTUpDnPosCnt&0x08) && (g_ChlData[ZTTrace[i+1]].u8FirstZTUpDnPosCnt&0x03)))) ||
		    (g_ChlData[ZTTrace[i]].u8Id > g_ChlData[ZTTrace[i+1]].u8Id && (((g_ChlData[ZTTrace[i+1]].u8SecondZTUpDnPosCnt&0x0c) && (g_ChlData[ZTTrace[i]].u8FirstZTUpDnPosCnt&0x01)) || ((g_ChlData[ZTTrace[i+1]].u8SecondZTUpDnPosCnt&0x08) && (g_ChlData[ZTTrace[i]].u8FirstZTUpDnPosCnt&0x03)) ))))   //ѹ����봥��խ���߽�ĵ�
		{
			Status = YAFENG;    //ѹ��
			yafeng_buff[i] = 1;
			//break;
		}
	}
	
	/**** ����λ�õ÷� ****/
	for(i=0; i<ZTCnt-1; i++)
	{
		delta_x = GetDiff(ZTXpos[i], ZTXpos[i+1]);
		delta_y = GetDiff(ZTYpos[i], ZTYpos[i+1]);
		if(delta_y > 0 && delta_x*c_pos/delta_y < 100 && yafeng_buff[i] == 0)   //��ѹ�������λ������
			PosScore[i] = 100 - delta_x*c_pos/delta_y;    //ÿһ�ε�λ�õ÷�
		else if(delta_y > 0 && delta_x*c_pos/delta_y >= 100)
			PosScore[i] = 0;
	}
	//�������е���λ�õ÷�
	PosScore[5] = PosScore[0];
	for(j=1; j<i; j++)
	{
		PosScore[5] = PosScore[5] * PosScore[j] / 100;
	}

	/**** �����ٶȵ÷� ****/
	if(ZTCnt == 2)
	{
		SpdScore[5] = 100;
	}
	else if(ZTCnt > 2)
	{
		for(i=0; i<ZTCnt-2; i++)
		{
			if(Status == 0)    //��ѹ��������ٶ�����(��ֹѹ��ʱ�ٶȼ��㲻׼)
			{
				delta_spd = GetDiff(Spd[i], Spd[i+1]);
				if(delta_spd*36/1000 * c_spd < 100)   //�����ٶ�����
					SpdScore[i] = 100 - delta_spd*36/1000 * c_spd;
				else if(delta_spd*36/1000 * c_spd >= 100)
					SpdScore[i] = 0;
			}
		}
		SpdScore[5] = SpdScore[0];
		for(j=1; j<i; j++)
		{
			SpdScore[5] = SpdScore[5] * SpdScore[j] / 100;
		}
	}
	for(i=0; i<ZTCnt-1; i++)
	{
		if(Spd[i] > 2700)    //�ٶȳ���2700cm/s����Ϊ������,������0
			SpdScore[5] = 0;
	}
	//�ٶȱ仯����15km/h��Ϊ����,���ٶȷ���0
	for(i=0; i<ZTCnt-2; i++)
	{
		delta_spd = GetDiff(Spd[i], Spd[i+1]);
		if(delta_spd > g_u16TraceSpdChangeMax)
		{
			SpdScore[5] = 0;
			break;
		}
	}

	/**** ���㻡�ȵ÷� ****/
	for(i=0; i<ZTCnt-1; i++)
	{
		deltaX[i] = ZTXpos[i+1] - ZTXpos[i];    //��λcm
		deltaY[i] = GetDiff(ZTYpos[i], ZTYpos[i+1]);
	}
	for(i=0; i<ZTCnt-2; i++)
	{
		if((F_Nabs(deltaX[i])>g_u8YaFengDiffXPosMax) && (F_Nabs(deltaX[i+1])>g_u8YaFengDiffXPosMax))    //λ�ñ仯����ѹ����С������л��ȴ��
			ScoreTemp += F_Nabs(GetDiff(deltaX[i], deltaX[i+1])) * c_rad / deltaY[i];
	}
	RadScore[5] = (ScoreTemp<100) ? (100-ScoreTemp) : 0;     //���ȵ÷�
	
	//����λ�ñ仯�ж�����ʻ״̬
	for(i=0; i<ZTCnt-1; i++)
	{
		if(F_Nabs(deltaX[i]) > g_u8XieXingDiffXPosMin)
		{
			if(deltaX[i] < 0)
			{
				LeftXieCnt++;
			}
			else if(deltaX[i] > 0)
			{
				RightXieCnt++;
			}
		}
	}
	if((LeftXieCnt > 0) && (RightXieCnt > 0))
	{
		Status = MOVEROUND;    //��S
	}
	else if((LeftXieCnt > 0) && (RightXieCnt == 0))
	{
		Status = XIEZOULEFT;    //��б
	}
	else if((LeftXieCnt == 0) && (RightXieCnt > 0))
	{
		Status = XIEZOURIGHT;    //��б
	}
	else
	{
		if(Status != YAFENG)   //û��ѹ��
		{
			Status = NORMAL;    //����
		}
	}
	
	
	
	/**** �����ۺϵ÷� ****/
	if(PosScore[5] == 0 || SpdScore[5] == 0 || RadScore[5] == 0)
		Score = 0;
	else
		Score = (a_pos * PosScore[5] / 10) + (a_spd * SpdScore[5] / 10) + (a_rad * RadScore[5] / 10);
	
	//�洢�������
	l_u8Score[0] = PosScore[5];
	l_u8Score[1] = SpdScore[5];
	l_u8Score[2] = RadScore[5];
	l_u8Score[3] = Score;
	l_u8Score[4] = Status;
	
	return Score;
}

/*******************************************************************************************************
** Function name:  		FWheelTraceMerge
** Descriptions:   		�ֹ켣�ϲ�
** input parameters:	ǰһխ���Ĺ켣�ṹ��,��ǰխ���Ĺ켣�ṹ��,
						ǰһ��խ����Ϣ,��ǰխ����Ϣ,��ǰխ��������
** output parameters:	
**
** Created by: liumingxin  
** Created Date: 20180606
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FWheelTraceMerge(void * Address[11])
{
	TraceWhel_Struct *WheelTracePre, *WheelTraceCur;
	ZTINFRECELEMENT *lpZTInf;
	uint8 *pWheelCnt;
	uint8 *pZTIDPre;
	uint8 preZTID;
	uint16 *pWheel1XPos, *pWheel2XPos;
	uint8  *pLeftPoint, *pRightPoint;
	void *l_Address[15];
	
	uint8 i, j;
	uint16 YStep = 70;     //y����ľ�������(Ĭ��Ϊ1400mm����һ��)
	uint8 l_u8ZTLocationX, l_u8ZTLocationY;
	uint8 curZTWhlCnt = 0;       //��ǰխ���ϵ��ֵĸ���
	uint8  l_u8ZTIDPerTrace[6];
	uint16 l_u16TraceList[6];    //��ʱ�洢�켣,���ڼ���켣����
	uint16 ZTXpos[6];     		 //������ÿ��խ����Xλ��(��λ:cm)
	uint16 ZTYpos[6];    		 //������ÿ��խ����Yλ��(��λ:cm)
	uint16 Spd[6];       		 //������ÿ�������ٶ�(��λ:mm/s)
	uint8  WheelZTake[6];        //�켣�д���խ����λ�� 1:���;2:�Ҳ�;3:����
	uint8  WheelZTPoint[6];      //����
	uint8  l_u8Score[5];   	     //λ�õ÷�,�ٶȵ÷�,���ȵ÷�,�ۺϵ÷�,����ʻ״̬
	uint8  l_u8MultiScore = 0;   //��ʱ�洢ÿ�����е��ۺϵ÷� 
	uint8  ZTCnt = 0;
	uint8  MinScore = 0xff;
	uint8  MinScorePos = 0;      //��ͷ����Ĺ켣λ��
	uint8  TraceDir = 0;         //0:�켣����Left��,1:�켣����Right��
	uint8  ZTMergeScoreMin = g_u8BuildTraceScoreMin;    //�����켣����С����
//	uint32 *pTimeNow;
	int8   *pDir;                //�켣����,1:����,-1:����

	WheelTracePre = Address[0];
	WheelTraceCur = Address[1];
	lpZTInf       = Address[2];    //��ǰխ����Ϣ
	pWheelCnt     = Address[3];    //��ǰխ�����ֵĸ���
	pZTIDPre      = Address[4];
	pWheel1XPos   = Address[5];
	pWheel2XPos   = Address[6];
	//pTimeNow      = Address[7];
	pLeftPoint    = Address[8];
	pRightPoint   = Address[9];
	pDir          = Address[10];
	
	//��ʼ���ȹز���
	curZTWhlCnt = *pWheelCnt;
	preZTID = *pZTIDPre;
	memset(l_u16TraceList, -1, sizeof(uint16)*6);
	memset(ZTXpos, -1, sizeof(uint16)*6);
	memset(ZTYpos, -1, sizeof(uint16)*6);
	memset(Spd, -1, sizeof(uint16)*6);
	memset(l_u8Score, -1, sizeof(uint8)*4);
	YStep = PROMODPARAM.m_au16ZTGap[preZTID]/2/10;
	FTransLocation(lpZTInf->m_ucTouch, &l_u8ZTLocationX, &l_u8ZTLocationY, 2);
	
	if(curZTWhlCnt == 1)    //��ǰխ����ֻ��һ����,��ǰһխ���������ֵĹ켣������һ������
	{
		TraceDir = 0;   //�켣�ϲ���Left��
		for(i=0; i<WheelTracePre->LeftTraceCnt; i++)    //����ǰһխ����Left���й켣
		{
			//��ʼ��ÿ�α�������Ҫ�Ĳ���
			MinScore = 0xff;
//			memset(l_u16TraceList, -1, sizeof(uint16)*6);
//			memset(ZTXpos, -1, sizeof(uint16)*6);
//			memset(ZTYpos, -1, sizeof(uint16)*6);
//			memset(Spd, -1, sizeof(uint16)*6);
			memset(l_u8Score, -1, sizeof(uint8)*4);
//			memset(WheelZTake, -1, sizeof(uint8)*6);
			
			//ȡ��һ���켣���������������
			memcpy(l_u16TraceList, WheelTracePre->LeftWhlZTPos[i], sizeof(uint16)*6);
			memcpy(ZTXpos, WheelTracePre->LeftWhlZTXPos[i], sizeof(uint16)*6);
			memcpy(ZTYpos, WheelTracePre->LeftWhlZTYPos[i], sizeof(uint16)*6);
			memcpy(Spd, WheelTracePre->LeftWhlSpd[i], sizeof(uint16)*6);
			memcpy(WheelZTake, WheelTracePre->LeftWhlZTake[i], sizeof(uint8)*6);
			memcpy(WheelZTPoint, WheelTracePre->LeftPoint[i], sizeof(uint8)*6);
			
			//����ǰխ�����������������
			l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->u16ID;   //խ��Pos
			ZTXpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pWheel1XPos;
			ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;   //��λcm

			WheelZTPoint[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pLeftPoint;
			WheelZTake[WheelTracePre->LeftWhlZTCntPerTrace[i]] = 3;
			Spd[WheelTracePre->LeftWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]) *6000 /
												GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]].u32uptm);	//��λcm/s
			ZTCnt = WheelTracePre->LeftWhlZTCntPerTrace[i]+1;
			
			l_Address[0] = l_u16TraceList;
			l_Address[1] = ZTXpos;
			l_Address[2] = ZTYpos;
			l_Address[3] = Spd;
			l_Address[4] = &ZTCnt;
			l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
			l_u8MultiScore = FTraceScore(l_Address);
			
			if(l_u8MultiScore > ZTMergeScoreMin)    //��ǰ�켣�ķ���������ֵ�����򽫹켣���뵽��Ӧλ��
			{
				memcpy(l_u8ZTIDPerTrace, WheelTracePre->LeftWhlZTID[i], sizeof(uint8)*6);	
				l_u8ZTIDPerTrace[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
				l_Address[6] = l_u8ZTIDPerTrace;    //�켣��խ��IDList
				l_Address[7] = WheelTraceCur;       //���켣���뵱ǰ��խ���켣�ṹ��
				l_Address[9] = &TraceDir;
				l_Address[10] = &curZTWhlCnt;
				l_Address[11] = WheelZTake;
				l_Address[12] = WheelZTPoint;
				l_Address[13] = pDir;
				if(WheelTraceCur->LeftTraceCnt < 6)    //���켣���뵽��ǰխ����
				{
					l_Address[8] = &WheelTraceCur->LeftTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
					FInsertTrace(l_Address);
				}
				else if(WheelTraceCur->LeftTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣,�����ǰ�켣Ϊѹ���򸲸����һ����ѹ��켣
				{
					for(j=0; j<6; j++)
					{
						if(WheelTraceCur->LeftMultiScore[j] < MinScore)
						{
							MinScore = WheelTraceCur->LeftMultiScore[j];
							MinScorePos = j;
						}
					}
					if(l_u8MultiScore > MinScore)
					{
						l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
						FInsertTrace(l_Address);
					}
					else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
					{
						for(j=0; j<6; j++)
						{
							if(WheelTraceCur->LeftWhlStatus[j] != YAFENG ||
							   ZTCnt > WheelTraceCur->LeftWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
							{
								MinScorePos = j;
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
								break;
							}
						}	
					}
				}
			}
		}
		
		for(i=0; i<WheelTracePre->RightTraceCnt; i++)    //����ǰһխ����Right���й켣
		{
			//��ʼ��ÿ�α����Ĳ���
			MinScore = 0xff;
//			memset(l_u16TraceList, -1, sizeof(uint16)*6);
//			memset(ZTXpos, -1, sizeof(uint16)*6);
//			memset(ZTYpos, -1, sizeof(uint16)*6);
//			memset(Spd, -1, sizeof(uint16)*6);
			memset(l_u8Score, -1, sizeof(uint8)*4);
//			memset(WheelZTake, -1, sizeof(uint8)*6);
			
			//ȡ��һ���켣���������������
			memcpy(l_u16TraceList, WheelTracePre->RightWhlZTPos[i], sizeof(uint16)*6);
			memcpy(ZTXpos, WheelTracePre->RightWhlZTXPos[i], sizeof(uint16)*6);
			memcpy(ZTYpos, WheelTracePre->RightWhlZTYPos[i], sizeof(uint16)*6);
			memcpy(Spd, WheelTracePre->RightWhlSpd[i], sizeof(uint16)*6);
			memcpy(WheelZTake, WheelTracePre->RightWhlZTake[i], sizeof(uint8)*6);
			memcpy(WheelZTPoint, WheelTracePre->RightPoint[i], sizeof(uint8)*6);
			
			//����ǰխ����������խ������
			l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
			ZTXpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pWheel1XPos;
			ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

			WheelZTPoint[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pLeftPoint;
			WheelZTake[WheelTracePre->RightWhlZTCntPerTrace[i]] = 3;
			Spd[WheelTracePre->RightWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]-1]) *6000 /
												GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]-1]].u32uptm);	
			ZTCnt = WheelTracePre->RightWhlZTCntPerTrace[i]+1;
			
			l_Address[0] = l_u16TraceList;
			l_Address[1] = ZTXpos;
			l_Address[2] = ZTYpos;
			l_Address[3] = Spd;
			l_Address[4] = &ZTCnt;
			l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
			l_u8MultiScore = FTraceScore(l_Address);
			
			if(l_u8MultiScore > ZTMergeScoreMin)
			{
				memcpy(l_u8ZTIDPerTrace, WheelTracePre->RightWhlZTID[i], sizeof(uint8)*6);
				l_u8ZTIDPerTrace[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
				l_Address[6] = l_u8ZTIDPerTrace;
				l_Address[7] = WheelTraceCur;
				l_Address[9] = &TraceDir;
				l_Address[10] = &curZTWhlCnt;
				l_Address[11] = WheelZTake;
				l_Address[12] = WheelZTPoint;
				l_Address[13] = pDir;
				if(WheelTraceCur->LeftTraceCnt < 6)    //���켣���뵽��ǰխ����
				{
					l_Address[8] = &WheelTraceCur->LeftTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
					FInsertTrace(l_Address);
				}
				else if(WheelTraceCur->LeftTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
				{
					for(j=0; j<6; j++)
					{
						if(WheelTraceCur->RightMultiScore[j] < MinScore)
						{
							MinScore = WheelTraceCur->LeftMultiScore[j];
							MinScorePos = j;
						}
					}
					if(l_u8MultiScore > MinScore)
					{
						l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
						FInsertTrace(l_Address);
					}
					else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
					{
						for(j=0; j<6; j++)
						{
							if(WheelTraceCur->LeftWhlStatus[j] != YAFENG ||
							   ZTCnt > WheelTraceCur->LeftWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
							{
								MinScorePos = j;
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
								break;
							}
						}
					}
				}
			}
		}
	}
	else if(curZTWhlCnt == 2)    //��ǰխ������2����,ѡ���Ե���ǰһխ���ϵ��ֹ켣��������
	{
		if(WheelTracePre->WheelCnt == 1)
		{
			for(i=0; i<WheelTracePre->LeftTraceCnt; i++)    //���ֱ���ǰһխ�������й켣
			{
				TraceDir = 0;
				//��ʼ��ÿ�α����Ĳ���
				MinScore = 0xff;
//				memset(l_u16TraceList, -1, sizeof(uint16)*6);
//				memset(ZTXpos, -1, sizeof(uint16)*6);
//				memset(ZTYpos, -1, sizeof(uint16)*6);
//				memset(Spd, -1, sizeof(uint16)*6);
				memset(l_u8Score, -1, sizeof(uint8)*4);
			
				//ȡ��һ���켣���������������
				memcpy(l_u16TraceList, WheelTracePre->LeftWhlZTPos[i], sizeof(uint16)*6);
				memcpy(ZTXpos, WheelTracePre->LeftWhlZTXPos[i], sizeof(uint16)*6);
				memcpy(ZTYpos, WheelTracePre->LeftWhlZTYPos[i], sizeof(uint16)*6);
				memcpy(Spd, WheelTracePre->LeftWhlSpd[i], sizeof(uint16)*6);
				memcpy(WheelZTake, WheelTracePre->LeftWhlZTake[i], sizeof(uint8)*6);
				memcpy(WheelZTPoint, WheelTracePre->LeftPoint[i], sizeof(uint8)*6);
				
				//����ǰխ����������խ������
				l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
				ZTXpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pWheel1XPos;
				ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

				WheelZTPoint[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pLeftPoint;
				WheelZTake[WheelTracePre->LeftWhlZTCntPerTrace[i]] = 1;
				Spd[WheelTracePre->LeftWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]) *6000 /
													GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]].u32uptm);	
				ZTCnt = WheelTracePre->LeftWhlZTCntPerTrace[i]+1;
				
				l_Address[0] = l_u16TraceList;
				l_Address[1] = ZTXpos;
				l_Address[2] = ZTYpos;
				l_Address[3] = Spd;
				l_Address[4] = &ZTCnt;
				l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
				l_u8MultiScore = FTraceScore(l_Address);
				
				if(l_u8MultiScore > ZTMergeScoreMin)
				{
					memcpy(l_u8ZTIDPerTrace, WheelTracePre->LeftWhlZTID[i], sizeof(uint8)*6);
					l_u8ZTIDPerTrace[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
					l_Address[6] = l_u8ZTIDPerTrace;
					l_Address[7] = WheelTraceCur;
					l_Address[9] = &TraceDir;
					l_Address[10] = &curZTWhlCnt;
					l_Address[11] = WheelZTake;
					l_Address[12] = WheelZTPoint;
					l_Address[13] = pDir;
					if(WheelTraceCur->LeftTraceCnt < 6)    //���켣���뵽��ǰխ����
					{
						l_Address[8] = &WheelTraceCur->LeftTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
						FInsertTrace(l_Address);
					}
					else if(WheelTraceCur->LeftTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
					{
						for(j=0; j<6; j++)
						{
							if(WheelTraceCur->LeftMultiScore[j] < MinScore)
							{
								MinScore = WheelTraceCur->LeftMultiScore[j];
								MinScorePos = j;
							}
						}
						if(l_u8MultiScore > MinScore)
						{
							l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->LeftWhlStatus[j] != YAFENG ||
								   ZTCnt > WheelTraceCur->LeftWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
								{
									MinScorePos = j;
									l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
									FInsertTrace(l_Address);
									break;
								}
							}
						}
					}
				}
			}
			
			for(i=0; i<WheelTracePre->LeftTraceCnt; i++)    //���ֱ���ǰһխ�������й켣
			{
				TraceDir = 1;
				//��ʼ��ÿ�α����Ĳ���
				MinScore = 0xff;
//				memset(l_u16TraceList, -1, sizeof(uint16)*6);
//				memset(ZTXpos, -1, sizeof(uint16)*6);
//				memset(ZTYpos, -1, sizeof(uint16)*6);
//				memset(Spd, -1, sizeof(uint16)*6);
				memset(l_u8Score, -1, sizeof(uint8)*4);
			
				//ȡ��һ���켣���������������
				memcpy(l_u16TraceList, WheelTracePre->LeftWhlZTPos[i], sizeof(uint16)*6);
				memcpy(ZTXpos, WheelTracePre->LeftWhlZTXPos[i], sizeof(uint16)*6);
				memcpy(ZTYpos, WheelTracePre->LeftWhlZTYPos[i], sizeof(uint16)*6);
				memcpy(Spd, WheelTracePre->LeftWhlSpd[i], sizeof(uint16)*6);
				memcpy(WheelZTake, WheelTracePre->LeftWhlZTake[i], sizeof(uint8)*6);
				memcpy(WheelZTPoint, WheelTracePre->LeftPoint[i], sizeof(uint8)*6);
				//����ǰխ����������խ������
				l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
				ZTXpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pWheel2XPos;;
				ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

				WheelZTPoint[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pRightPoint;
				WheelZTake[WheelTracePre->LeftWhlZTCntPerTrace[i]] = 2;
				Spd[WheelTracePre->LeftWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]) *6000 /
													GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]].u32uptm);	
				ZTCnt = WheelTracePre->LeftWhlZTCntPerTrace[i]+1;
				
				l_Address[0] = l_u16TraceList;
				l_Address[1] = ZTXpos;
				l_Address[2] = ZTYpos;
				l_Address[3] = Spd;
				l_Address[4] = &ZTCnt;
				l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
				l_u8MultiScore = FTraceScore(l_Address);
				
				if(l_u8MultiScore > ZTMergeScoreMin)
				{
					memcpy(l_u8ZTIDPerTrace, WheelTracePre->LeftWhlZTID[i], sizeof(uint8)*6);
					l_u8ZTIDPerTrace[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
					l_Address[6] = l_u8ZTIDPerTrace;
					l_Address[7] = WheelTraceCur;
					l_Address[9] = &TraceDir;
					l_Address[10] = &curZTWhlCnt;
					l_Address[11] = WheelZTake;
					l_Address[12] = WheelZTPoint;
					l_Address[13] = pDir;
					if(WheelTraceCur->RightTraceCnt < 6)    //���켣���뵽��ǰխ����
					{
						l_Address[8] = &WheelTraceCur->RightTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
						FInsertTrace(l_Address);
					}
					else if(WheelTraceCur->RightTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
					{
						for(j=0; j<6; j++)
						{
							if(WheelTraceCur->RightMultiScore[j] < MinScore)
							{
								MinScore = WheelTraceCur->RightMultiScore[j];
								MinScorePos = j;
							}
						}
						if(l_u8MultiScore > MinScore)
						{
							l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->RightWhlStatus[j] != YAFENG ||
							       ZTCnt > WheelTraceCur->RightWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
								{
									MinScorePos = j;
									l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
									FInsertTrace(l_Address);
									break;
								}
							}
						}
					}
				}
			}
		}
		else if(WheelTracePre->WheelCnt == 2)
		{
			if(l_u8ZTLocationX == preZTID/PROMODPARAM.m_u8ZTRow)   //��ͬ��
			{
				for(i=0; i<WheelTracePre->LeftTraceCnt; i++)    //����ǰһխ��������Left�켣
				{
					TraceDir = 0;
					//��ʼ��ÿ�α����Ĳ���
					MinScore = 0xff;
//					memset(l_u16TraceList, -1, sizeof(uint16)*6);
//					memset(ZTXpos, -1, sizeof(uint16)*6);
//					memset(ZTYpos, -1, sizeof(uint16)*6);
//					memset(Spd, -1, sizeof(uint16)*6);
					memset(l_u8Score, -1, sizeof(uint8)*4);
				
					//ȡ��һ���켣���������������
					memcpy(l_u16TraceList, WheelTracePre->LeftWhlZTPos[i], sizeof(uint16)*6);
					memcpy(ZTXpos, WheelTracePre->LeftWhlZTXPos[i], sizeof(uint16)*6);
					memcpy(ZTYpos, WheelTracePre->LeftWhlZTYPos[i], sizeof(uint16)*6);
					memcpy(Spd, WheelTracePre->LeftWhlSpd[i], sizeof(uint16)*6);
					memcpy(WheelZTake, WheelTracePre->LeftWhlZTake[i], sizeof(uint8)*6);
					memcpy(WheelZTPoint, WheelTracePre->LeftPoint[i], sizeof(uint8)*6);
					//����ǰխ����������խ������
					l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
					ZTXpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pWheel1XPos;
					ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;
				
					WheelZTPoint[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pLeftPoint;
					WheelZTake[WheelTracePre->LeftWhlZTCntPerTrace[i]] = 1;
					Spd[WheelTracePre->LeftWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]) *6000 /
														GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]].u32uptm);	
					ZTCnt = WheelTracePre->LeftWhlZTCntPerTrace[i]+1;
					
					l_Address[0] = l_u16TraceList;
					l_Address[1] = ZTXpos;
					l_Address[2] = ZTYpos;
					l_Address[3] = Spd;
					l_Address[4] = &ZTCnt;
					l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
					l_u8MultiScore = FTraceScore(l_Address);
					
					if(l_u8MultiScore > ZTMergeScoreMin)
					{
						memcpy(l_u8ZTIDPerTrace, WheelTracePre->LeftWhlZTID[i], sizeof(uint8)*6);
						l_u8ZTIDPerTrace[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
						l_Address[6] = l_u8ZTIDPerTrace;
						l_Address[7] = WheelTraceCur;
						l_Address[9] = &TraceDir;
						l_Address[10] = &curZTWhlCnt;
						l_Address[11] = WheelZTake;
						l_Address[12] = WheelZTPoint;
						l_Address[13] = pDir;
						if(WheelTraceCur->LeftTraceCnt < 6)    //���켣���뵽��ǰխ����
						{
							l_Address[8] = &WheelTraceCur->LeftTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(WheelTraceCur->LeftTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->LeftMultiScore[j] < MinScore)
								{
									MinScore = WheelTraceCur->LeftMultiScore[j];
									MinScorePos = j;
								}
							}
							if(l_u8MultiScore > MinScore)
							{
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
							}
							else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
							{
								for(j=0; j<6; j++)
								{
									if(WheelTraceCur->LeftWhlStatus[j] != YAFENG ||
							           ZTCnt > WheelTraceCur->LeftWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
									{
										MinScorePos = j;
										l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
										FInsertTrace(l_Address);
										break;
									}
								}
							}
						}
					}
				}
				
				for(i=0; i<WheelTracePre->RightTraceCnt; i++)    //���ֱ���ǰһխ�������й켣
				{
					TraceDir = 1;
					//��ʼ��ÿ�α����Ĳ���
					MinScore = 0xff;
//					memset(l_u16TraceList, -1, sizeof(uint16)*6);
//					memset(ZTXpos, -1, sizeof(uint16)*6);
//					memset(ZTYpos, -1, sizeof(uint16)*6);
//					memset(Spd, -1, sizeof(uint16)*6);
					memset(l_u8Score, -1, sizeof(uint8)*4);
				
					//ȡ��һ���켣���������������
					memcpy(l_u16TraceList, WheelTracePre->RightWhlZTPos[i], sizeof(uint16)*6);
					memcpy(ZTXpos, WheelTracePre->RightWhlZTXPos[i], sizeof(uint16)*6);
					memcpy(ZTYpos, WheelTracePre->RightWhlZTYPos[i], sizeof(uint16)*6);
					memcpy(Spd, WheelTracePre->RightWhlSpd[i], sizeof(uint16)*6);
					memcpy(WheelZTake, WheelTracePre->RightWhlZTake[i], sizeof(uint8)*6);
					memcpy(WheelZTPoint, WheelTracePre->RightPoint[i], sizeof(uint8)*6);
					//����ǰխ����������խ������
					l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
					ZTXpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pWheel2XPos;
					ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

					WheelZTPoint[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pRightPoint;
					WheelZTake[WheelTracePre->RightWhlZTCntPerTrace[i]] = 2;
					Spd[WheelTracePre->RightWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]-1]) *6000 /
														GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]-1]].u32uptm);	
					ZTCnt = WheelTracePre->RightWhlZTCntPerTrace[i]+1;
					
					l_Address[0] = l_u16TraceList;
					l_Address[1] = ZTXpos;
					l_Address[2] = ZTYpos;
					l_Address[3] = Spd;
					l_Address[4] = &ZTCnt;
					l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
					l_u8MultiScore = FTraceScore(l_Address);
					
					if(l_u8MultiScore > ZTMergeScoreMin)
					{
						memcpy(l_u8ZTIDPerTrace, WheelTracePre->RightWhlZTID[i], sizeof(uint8)*6);
						l_u8ZTIDPerTrace[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
						l_Address[6] = l_u8ZTIDPerTrace;
						l_Address[7] = WheelTraceCur;
						l_Address[9] = &TraceDir;
						l_Address[10] = &curZTWhlCnt;
						l_Address[11] = WheelZTake;
						l_Address[12] = WheelZTPoint;
						l_Address[13] = pDir;
						if(WheelTraceCur->RightTraceCnt < 6)    //���켣���뵽��ǰխ����
						{
							l_Address[8] = &WheelTraceCur->RightTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(WheelTraceCur->RightTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->RightMultiScore[j] < MinScore)
								{
									MinScore = WheelTraceCur->RightMultiScore[j];
									MinScorePos = j;
								}
							}
							if(l_u8MultiScore > MinScore)
							{
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
							}
							else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
							{
								for(j=0; j<6; j++)
								{
									if(WheelTraceCur->RightWhlStatus[j] != YAFENG ||
									   ZTCnt > WheelTraceCur->RightWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
									{
										MinScorePos = j;
										l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
										FInsertTrace(l_Address);
										break;
									}
								}
							}
						}
					}
				}
			}
			else if(preZTID/PROMODPARAM.m_u8ZTRow < l_u8ZTLocationX)    //�����
			{
				for(i=0; i<WheelTracePre->RightTraceCnt; i++)    //����ǰһխ�����ֵ����й켣
				{
					TraceDir = 0;
					//��ʼ��ÿ�α����Ĳ���
					MinScore = 0xff;
//					memset(l_u16TraceList, -1, sizeof(uint16)*6);
//					memset(ZTXpos, -1, sizeof(uint16)*6);
//					memset(ZTYpos, -1, sizeof(uint16)*6);
//					memset(Spd, -1, sizeof(uint16)*6);
					memset(l_u8Score, -1, sizeof(uint8)*4);
				
					//ȡ��һ���켣���������������
					memcpy(l_u16TraceList, WheelTracePre->RightWhlZTPos[i], sizeof(uint16)*6);
					memcpy(ZTXpos, WheelTracePre->RightWhlZTXPos[i], sizeof(uint16)*6);
					memcpy(ZTYpos, WheelTracePre->RightWhlZTYPos[i], sizeof(uint16)*6);
					memcpy(Spd, WheelTracePre->RightWhlSpd[i], sizeof(uint16)*6);
					memcpy(WheelZTake, WheelTracePre->RightWhlZTake[i], sizeof(uint8)*6);
					memcpy(WheelZTPoint, WheelTracePre->RightPoint[i], sizeof(uint8)*6);
					//����ǰխ����������խ������
					l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
					ZTXpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pWheel1XPos;
					ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

					WheelZTPoint[WheelTracePre->RightWhlZTCntPerTrace[i]] = *pLeftPoint;
					WheelZTake[WheelTracePre->RightWhlZTCntPerTrace[i]] = 1;
					Spd[WheelTracePre->RightWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->RightWhlZTCntPerTrace[i]-1]) *6000 /
														GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->RightWhlZTCntPerTrace[i]-1]].u32uptm);	
					ZTCnt = WheelTracePre->RightWhlZTCntPerTrace[i]+1;
					
					l_Address[0] = l_u16TraceList;
					l_Address[1] = ZTXpos;
					l_Address[2] = ZTYpos;
					l_Address[3] = Spd;
					l_Address[4] = &ZTCnt;
					l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
					l_u8MultiScore = FTraceScore(l_Address);
					
					if(l_u8MultiScore > ZTMergeScoreMin)
					{
						memcpy(l_u8ZTIDPerTrace, WheelTracePre->RightWhlZTID[i], sizeof(uint8)*6);
						l_u8ZTIDPerTrace[WheelTracePre->RightWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
						l_Address[6] = l_u8ZTIDPerTrace;
						l_Address[7] = WheelTraceCur;
						l_Address[9] = &TraceDir;
						l_Address[10] = &curZTWhlCnt;
						l_Address[11] = WheelZTake;
						l_Address[12] = WheelZTPoint;
						l_Address[13] = pDir;
						if(WheelTraceCur->LeftTraceCnt < 6)    //���켣���뵽��ǰխ����
						{
							l_Address[8] = &WheelTraceCur->LeftTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(WheelTraceCur->LeftTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->LeftMultiScore[j] < MinScore)
								{
									MinScore = WheelTraceCur->LeftMultiScore[j];
									MinScorePos = j;
								}
							}
							if(l_u8MultiScore > MinScore)
							{
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
							}
							else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
							{
								for(j=0; j<6; j++)
								{
									if(WheelTraceCur->LeftWhlStatus[j] != YAFENG ||
									   ZTCnt > WheelTraceCur->LeftWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
									{
										MinScorePos = j;
										l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
										FInsertTrace(l_Address);
										break;
									}
								}
							}
						}
					}
				}
			}
			else    //�Ҳ���
			{
				for(i=0; i<WheelTracePre->LeftTraceCnt; i++)    //����ǰһխ�������й켣
				{
					TraceDir = 1;
					//��ʼ��ÿ�α����Ĳ���
					MinScore = 0xff;
//					memset(l_u16TraceList, -1, sizeof(uint16)*6);
//					memset(ZTXpos, -1, sizeof(uint16)*6);
//					memset(ZTYpos, -1, sizeof(uint16)*6);
//					memset(Spd, -1, sizeof(uint16)*6);
					memset(l_u8Score, -1, sizeof(uint8)*4);
				
					//ȡ��һ���켣���������������
					memcpy(l_u16TraceList, WheelTracePre->LeftWhlZTPos[i], sizeof(uint16)*6);
					memcpy(ZTXpos, WheelTracePre->LeftWhlZTXPos[i], sizeof(uint16)*6);
					memcpy(ZTYpos, WheelTracePre->LeftWhlZTYPos[i], sizeof(uint16)*6);
					memcpy(Spd, WheelTracePre->LeftWhlSpd[i], sizeof(uint16)*6);
					memcpy(WheelZTake, WheelTracePre->LeftWhlZTake[i], sizeof(uint8)*6);
					memcpy(WheelZTPoint, WheelTracePre->LeftPoint[i], sizeof(uint8)*6);
					//����ǰխ����������խ������
					l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->u16ID;
					ZTXpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pWheel2XPos;
					ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]] = l_u8ZTLocationY*YStep;

					WheelZTPoint[WheelTracePre->LeftWhlZTCntPerTrace[i]] = *pRightPoint;
					WheelZTake[WheelTracePre->LeftWhlZTCntPerTrace[i]] = 2;
					Spd[WheelTracePre->LeftWhlZTCntPerTrace[i]-1] = GetDiff(ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]], ZTYpos[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]) *6000 /
														GetDiff(g_ChlData[lpZTInf->u16ID].u32uptm, g_ChlData[l_u16TraceList[WheelTracePre->LeftWhlZTCntPerTrace[i]-1]].u32uptm);	
					ZTCnt = WheelTracePre->LeftWhlZTCntPerTrace[i]+1;
					
					l_Address[0] = l_u16TraceList;
					l_Address[1] = ZTXpos;
					l_Address[2] = ZTYpos;
					l_Address[3] = Spd;
					l_Address[4] = &ZTCnt;
					l_Address[5] = l_u8Score;    //�洢����õ��ĸ������
					l_u8MultiScore = FTraceScore(l_Address);
					
					if(l_u8MultiScore > ZTMergeScoreMin)
					{
						memcpy(l_u8ZTIDPerTrace, WheelTracePre->LeftWhlZTID[i], sizeof(uint8)*6);
						l_u8ZTIDPerTrace[WheelTracePre->LeftWhlZTCntPerTrace[i]] = lpZTInf->m_ucTouch;
						l_Address[6] = l_u8ZTIDPerTrace;
						l_Address[7] = WheelTraceCur;
						l_Address[9] = &TraceDir;
						l_Address[10] = &curZTWhlCnt;
						l_Address[11] = WheelZTake;
						l_Address[12] = WheelZTPoint;
						l_Address[13] = pDir;
						if(WheelTraceCur->RightTraceCnt < 6)    //���켣���뵽��ǰխ����
						{
							l_Address[8] = &WheelTraceCur->RightTraceCnt;   //��Ҫ��ӵ��Ĺ켣λ��
							FInsertTrace(l_Address);
						}
						else if(WheelTraceCur->RightTraceCnt == 6)   //��ǰ�ṹ�еĹ켣����, ����ͷ�,��������ȵ�ǰ�������򸲸Ǹù켣
						{
							for(j=0; j<6; j++)
							{
								if(WheelTraceCur->RightMultiScore[j] < MinScore)
								{
									MinScore = WheelTraceCur->RightMultiScore[j];
									MinScorePos = j;
								}
							}
							if(l_u8MultiScore > MinScore)
							{
								l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
								FInsertTrace(l_Address);
							}
							else if(l_u8Score[4] == YAFENG)    //��ǰ�켣Ϊѹ��
							{
								for(j=0; j<6; j++)
								{
									if(WheelTraceCur->RightWhlStatus[j] != YAFENG ||
									   ZTCnt > WheelTraceCur->RightWhlZTCntPerTrace[j])  //���Ƿ�ѹ��켣��ѹ�쵫��խ�����ٵĹ켣
									{
										MinScorePos = j;
										l_Address[8] = &MinScorePos;   //��Ҫ��ӵ��Ĺ켣λ��
										FInsertTrace(l_Address);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
/*******************************************************************************************************
** Function name:  		FPostWheel
** Descriptions:   		����켣��ѡ��һ���߷ֵĹ켣������
** input parameters:	None
** output parameters:	
**
** Created by: liumingxin		  
** Created Date: 20180604
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FPostWheel(void * Address[2])
{
	TraceWhel_Struct (*WheelTrace)[4];
	uint32 *pTimeNow;
	void *l_Address[10];
	uint8 i;
	uint8 cmd = 1;
	uint8 ZTID4Delete;
	uint8 TraceDir;
	uint8 l_u8BuildWheelTracePos;
	uint8  LeftPostWheelFlag = 0;     //0:������,1:����
	uint8  RightPostWheelFlag = 0;    //0:������,1:����
	
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	uint8 l_u8PostWhlFlag = 0;
	
	pTimeNow = Address[0];
	
	//����ÿһ��,��ʱʱ�䵽����,����
	WheelTrace = g_WheelTrace;
	for(i=0; i<2*PROMODPARAM.m_u8LaneNum; i++) 
	{
		LeftPostWheelFlag = 0;
		RightPostWheelFlag = 0;
		l_u8PostWhlFlag = 0;
		
		ZTID4Delete = 3*i+2;    //ĩ��խ��ID
		if(i%2 == 0)   //ż����
		{
			if(g_Setup.an8ZTStatus[ZTID4Delete] == 1)    //ĩ��խ������,��ֱ�ӽ���
			{
				if(WheelTrace[ZTID4Delete][0].LeftTraceCnt > 0)
				{
					LeftPostWheelFlag = 1;
				}
				if(WheelTrace[ZTID4Delete][0].RightTraceCnt > 0)
				{
					RightPostWheelFlag = 1;
				}
				l_u8PostWhlFlag = 1;
			}
			else    //ĩ��խ����������ǰһ��խ������
			{
				if((ZTID4Delete > 2 && g_Setup.an8ZTStatus[ZTID4Delete-2] == 1) || (g_Setup.an8ZTStatus[ZTID4Delete+3] == 1))    //������ĩ��խ��������
				{
					ZTID4Delete--;
					if((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0) && (WheelTrace[ZTID4Delete][0].LeftTraceCnt>0))    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
					{
						LeftPostWheelFlag = 1;
					}
					if((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)  && (WheelTrace[ZTID4Delete][0].RightTraceCnt>0))
					{
						RightPostWheelFlag = 1;
					}
					l_u8PostWhlFlag = 2;
				}
				else    //������ĩ��խ��������
				{
					ZTID4Delete--;
					if(WheelTrace[ZTID4Delete][0].LeftTraceCnt > 0)
					{
						LeftPostWheelFlag = 1;
					}
					if(WheelTrace[ZTID4Delete][0].RightTraceCnt > 0)
					{
						RightPostWheelFlag = 1;
					}
					l_u8PostWhlFlag = 3;
				}
			}
		}
		else   //������
		{
			if(g_Setup.an8ZTStatus[ZTID4Delete] == 1)    //ĩ��խ������,����ʱʱ�䵽����
			{
				if(WheelTrace[ZTID4Delete][0].LeftTraceCnt>0)
				{
					if(((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0)) || (WheelTrace[ZTID4Delete][1].HaveTraceFlag==1))    //��ʱʱ�䵽,���һ���ᴥ����խ��
					{
						LeftPostWheelFlag = 1;
					}
				}
				if(WheelTrace[ZTID4Delete][0].RightTraceCnt>0)
				{
					if(((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)) || (WheelTrace[ZTID4Delete][1].HaveTraceFlag==1))
					{
						RightPostWheelFlag = 1;
					}
				}
				l_u8PostWhlFlag = 4;
			}
			else    //ĩ��խ����������ǰһ��խ������
			{
				ZTID4Delete--;
				if(WheelTrace[ZTID4Delete][0].LeftTraceCnt>0)
				{
					if(((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0)) || (WheelTrace[ZTID4Delete][1].HaveTraceFlag == 1))    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
					{
						LeftPostWheelFlag = 1;
					}
				}
				if(WheelTrace[ZTID4Delete][0].RightTraceCnt>0)
				{
					if(((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)) || (WheelTrace[ZTID4Delete][1].HaveTraceFlag == 1))
					{
						RightPostWheelFlag = 1;
					}
				}
				l_u8PostWhlFlag = 5;
			}
			
		}
		
		if(LeftPostWheelFlag == 1)    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
		{
			//�ҳ���ѡ�켣
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			FFindFirstTracePos(l_Address);   
			
			TraceDir = 0;
			l_u8BuildWheelTracePos = WheelTrace[ZTID4Delete][0].LeftFirstTracePos;
			
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPostWhl: 1,flag=%d,I=%d,P=%d,%d,n=%d,select=%d.",
							l_u8PostWhlFlag, ZTID4Delete, WheelTrace[ZTID4Delete][0].LeftWhlZTPos[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].LeftPoint[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1],
							WheelTrace[ZTID4Delete][0].LeftTraceCnt, l_u8BuildWheelTracePos);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);

			/**** ����ʹ�� ****/
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nS=[%d,%d,%d,%d,%d,%d],Sta=[%d,%d,%d,%d,%d,%d],Cnt=[%d,%d,%d,%d,%d,%d].",
							WheelTrace[ZTID4Delete][0].LeftMultiScore[0], WheelTrace[ZTID4Delete][0].LeftMultiScore[1], WheelTrace[ZTID4Delete][0].LeftMultiScore[2], 
							WheelTrace[ZTID4Delete][0].LeftMultiScore[3], WheelTrace[ZTID4Delete][0].LeftMultiScore[4], WheelTrace[ZTID4Delete][0].LeftMultiScore[5],
							WheelTrace[ZTID4Delete][0].LeftWhlStatus[0], WheelTrace[ZTID4Delete][0].LeftWhlStatus[1], WheelTrace[ZTID4Delete][0].LeftWhlStatus[2], 
							WheelTrace[ZTID4Delete][0].LeftWhlStatus[3], WheelTrace[ZTID4Delete][0].LeftWhlStatus[4], WheelTrace[ZTID4Delete][0].LeftWhlStatus[5],
							WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[0],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[1],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[2],
							WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[3],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[4],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"0.ZTPos=[%d,%d,%d,%d,%d,%d]\r\n",
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][0], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][1], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][2], 
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][3], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][4], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[0][5]);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
// 			
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"1.ZTPos=[%d,%d,%d,%d,%d,%d]\r\n",
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][0], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][1], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][2], 
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][3], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][4], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[1][5]);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
// 			
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"2.ZTPos=[%d,%d,%d,%d,%d,%d]\r\n",
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][0], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][1], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][2], 
// 							WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][3], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][4], WheelTrace[ZTID4Delete][0].LeftWhlZTPos[2][5]);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
			/**** end ****/
			
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = g_WhelData;
			l_Address[2] = &TraceDir;
			l_Address[3] = &cmd;
			FBuildWheel(l_Address);
			
			//ɾ���Ѿ�ƥ��õ�խ��
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = &TraceDir;
			FDeleteTrace(l_Address);    //ɾ�������Ѿ����ֵ�խ�������й켣
		}
		if(RightPostWheelFlag == 1)    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
		{
			//�ҳ���ѡ�켣
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			FFindFirstTracePos(l_Address);   
			
			TraceDir = 1;
			l_u8BuildWheelTracePos = WheelTrace[ZTID4Delete][0].RightFirstTracePos;
			
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPostWhl: 2,flg=%d,I=%d,P=%d,%d,n=%d,select=%d.",
							l_u8PostWhlFlag, ZTID4Delete, WheelTrace[ZTID4Delete][0].RightWhlZTPos[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].RightPoint[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].RightTraceCnt, l_u8BuildWheelTracePos);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);

			/**** ����ʹ�� ****/
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"S=[%d,%d,%d,%d,%d,%d],Sta=[%d,%d,%d,%d,%d,%d],Cnt=[%d,%d,%d,%d,%d,%d]\r\n",
							WheelTrace[ZTID4Delete][0].RightMultiScore[0], WheelTrace[ZTID4Delete][0].RightMultiScore[1], WheelTrace[ZTID4Delete][0].RightMultiScore[2], 
							WheelTrace[ZTID4Delete][0].RightMultiScore[3], WheelTrace[ZTID4Delete][0].RightMultiScore[4], WheelTrace[ZTID4Delete][0].RightMultiScore[5],
							WheelTrace[ZTID4Delete][0].RightWhlStatus[0], WheelTrace[ZTID4Delete][0].RightWhlStatus[1], WheelTrace[ZTID4Delete][0].RightWhlStatus[2], 
							WheelTrace[ZTID4Delete][0].RightWhlStatus[3], WheelTrace[ZTID4Delete][0].RightWhlStatus[4], WheelTrace[ZTID4Delete][0].RightWhlStatus[5],
							WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[0],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[1],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[2],
							WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[3],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[4],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			/**** end ****/
			
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = g_WhelData;
			l_Address[2] = &TraceDir;
			l_Address[3] = &cmd;
			FBuildWheel(l_Address);
			
			//ɾ���Ѿ�ƥ��õ�խ��
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = &TraceDir;
			FDeleteTrace(l_Address);    //ɾ�������Ѿ����ֵ�խ�������й켣
		}
	}
	
	//����ÿһ��,��ʱʱ�䵽����,����
	WheelTrace = g_ReWheelTrace;
	for(i=0; i<2*PROMODPARAM.m_u8LaneNum; i++) 
	{
		LeftPostWheelFlag = 0;
		RightPostWheelFlag = 0;
		l_u8PostWhlFlag = 0;
		
		ZTID4Delete = 3*i;    //����խ��ID
		if(i%2 == 1)   //������
		{
			if(g_Setup.an8ZTStatus[ZTID4Delete] == 1)    //����խ������,��ֱ�ӽ���
			{
				if(WheelTrace[ZTID4Delete][0].LeftTraceCnt > 0)
				{
					LeftPostWheelFlag = 1;
				}
				if(WheelTrace[ZTID4Delete][0].RightTraceCnt > 0)
				{
					RightPostWheelFlag = 1;
				}
				l_u8PostWhlFlag = 1;
			}
			else    //����խ����������ǰһ��խ������
			{
				if((g_Setup.an8ZTStatus[ZTID4Delete-3] == 1) || ((ZTID4Delete+3) < WHEELTRACEZTNUM && g_Setup.an8ZTStatus[ZTID4Delete+3] == 1))    //������ĩ��խ��������
				{
					ZTID4Delete++;
					if((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0) && (WheelTrace[ZTID4Delete][0].LeftTraceCnt>0))    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
					{
						LeftPostWheelFlag = 1;
					}
					if((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)  && (WheelTrace[ZTID4Delete][0].RightTraceCnt>0))
					{
						RightPostWheelFlag = 1;
					}
					l_u8PostWhlFlag = 2;
				}
				else    //������ĩ��խ��������
				{
					ZTID4Delete++;
					if(WheelTrace[ZTID4Delete][0].LeftTraceCnt > 0)
					{
						LeftPostWheelFlag = 1;
					}
					if(WheelTrace[ZTID4Delete][0].RightTraceCnt > 0)
					{
						RightPostWheelFlag = 1;
					}
					l_u8PostWhlFlag = 3;
				}
			}
		}
		else   //ż����
		{
			if(g_Setup.an8ZTStatus[ZTID4Delete] == 1)    //����խ������,����ʱʱ�䵽����
			{
				if((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0) && (WheelTrace[ZTID4Delete][0].LeftTraceCnt>0))    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
				{
					LeftPostWheelFlag = 1;
				}
				if((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)  && (WheelTrace[ZTID4Delete][0].RightTraceCnt>0))
				{
					RightPostWheelFlag = 1;
				}
				l_u8PostWhlFlag = 4;
			}
			else    //����խ����������ǰһ��խ������
			{
				ZTID4Delete++;
				if((*pTimeNow >= WheelTrace[ZTID4Delete][0].LeftDueTime) && (WheelTrace[ZTID4Delete][0].LeftDueTime > 0) && (WheelTrace[ZTID4Delete][0].LeftTraceCnt>0))    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
				{
					LeftPostWheelFlag = 1;
				}
				if((*pTimeNow >= WheelTrace[ZTID4Delete][0].RightDueTime) && (WheelTrace[ZTID4Delete][0].RightDueTime > 0)  && (WheelTrace[ZTID4Delete][0].RightTraceCnt>0))
				{
					RightPostWheelFlag = 1;
				}
				l_u8PostWhlFlag = 5;
			}
		}
		
		if(LeftPostWheelFlag == 1)    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
		{
			//�ҳ���ѡ�켣
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			FFindFirstTracePos(l_Address);   
			
			TraceDir = 0;
			l_u8BuildWheelTracePos = WheelTrace[ZTID4Delete][0].LeftFirstTracePos;
			
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRePostWhl: 1,flg=%d,I=%d,P=%d,%d,n=%d,select=%d.",
							l_u8PostWhlFlag, ZTID4Delete, WheelTrace[ZTID4Delete][0].LeftWhlZTPos[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].LeftPoint[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].LeftTraceCnt, l_u8BuildWheelTracePos);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			
			/**** ����ʹ�� ****/
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nS=[%d,%d,%d,%d,%d,%d],Sta=[%d,%d,%d,%d,%d,%d],ZTCnt=[%d,%d,%d,%d,%d,%d].",
							WheelTrace[ZTID4Delete][0].LeftMultiScore[0], WheelTrace[ZTID4Delete][0].LeftMultiScore[1], WheelTrace[ZTID4Delete][0].LeftMultiScore[2], 
							WheelTrace[ZTID4Delete][0].LeftMultiScore[3], WheelTrace[ZTID4Delete][0].LeftMultiScore[4], WheelTrace[ZTID4Delete][0].LeftMultiScore[5],
							WheelTrace[ZTID4Delete][0].LeftWhlStatus[0], WheelTrace[ZTID4Delete][0].LeftWhlStatus[1], WheelTrace[ZTID4Delete][0].LeftWhlStatus[2], 
							WheelTrace[ZTID4Delete][0].LeftWhlStatus[3], WheelTrace[ZTID4Delete][0].LeftWhlStatus[4], WheelTrace[ZTID4Delete][0].LeftWhlStatus[5],
							WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[0],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[1],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[2],
							WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[3],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[4],WheelTrace[ZTID4Delete][0].LeftWhlZTCntPerTrace[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			/**** end ****/

			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = g_WhelData;
			l_Address[2] = &TraceDir;
			l_Address[3] = &cmd;
			FBuildWheel(l_Address);
			
			//ɾ���Ѿ�ƥ��õ�խ��
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = &TraceDir;
			FDeleteTrace(l_Address);    //ɾ�������Ѿ����ֵ�խ�������й켣
		}
		if(RightPostWheelFlag == 1)    //��ʱʱ�䵽,ѡ��������ߵĹ켣,ɾ�������켣����Ӧխ���Ĺ켣��Ϣ
		{
			//�ҳ���ѡ�켣
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			FFindFirstTracePos(l_Address);   
			
			TraceDir = 1;
			l_u8BuildWheelTracePos = WheelTrace[ZTID4Delete][0].RightFirstTracePos;
			
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRePostWhl: 2,flg=%d,I=%d,P=%d,%d,n=%d,select=%d.",
							l_u8PostWhlFlag, ZTID4Delete, WheelTrace[ZTID4Delete][0].RightWhlZTPos[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].RightPoint[l_u8BuildWheelTracePos][WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[l_u8BuildWheelTracePos]-1], 
							WheelTrace[ZTID4Delete][0].RightTraceCnt, l_u8BuildWheelTracePos);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			
			/**** ����ʹ�� ****/
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nS=[%d,%d,%d,%d,%d,%d],Sta=[%d,%d,%d,%d,%d,%d],ZTCnt=[%d,%d,%d,%d,%d,%d].",
							WheelTrace[ZTID4Delete][0].RightMultiScore[0], WheelTrace[ZTID4Delete][0].RightMultiScore[1], WheelTrace[ZTID4Delete][0].RightMultiScore[2], 
							WheelTrace[ZTID4Delete][0].RightMultiScore[3], WheelTrace[ZTID4Delete][0].RightMultiScore[4], WheelTrace[ZTID4Delete][0].RightMultiScore[5],
							WheelTrace[ZTID4Delete][0].RightWhlStatus[0], WheelTrace[ZTID4Delete][0].RightWhlStatus[1], WheelTrace[ZTID4Delete][0].RightWhlStatus[2], 
							WheelTrace[ZTID4Delete][0].RightWhlStatus[3], WheelTrace[ZTID4Delete][0].RightWhlStatus[4], WheelTrace[ZTID4Delete][0].RightWhlStatus[5],
							WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[0],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[1],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[2],
							WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[3],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[4],WheelTrace[ZTID4Delete][0].RightWhlZTCntPerTrace[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			/**** end ****/

			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = g_WhelData;
			l_Address[2] = &TraceDir;
			l_Address[3] = &cmd;
			FBuildWheel(l_Address);
			
			//ɾ���Ѿ�ƥ��õ�խ��
			l_Address[0] = &WheelTrace[ZTID4Delete][0];
			l_Address[1] = &TraceDir;
			FDeleteTrace(l_Address);    //ɾ�������Ѿ����ֵ�խ�������й켣
		}
	}
}



/*******************************************************************************************************
** Function name:  		FBuildWheel
** Descriptions:   		����ѡ�����Ĺ켣����
** input parameters:	None
** output parameters:	
**
** Created by: liumingxin		  
** Created Date: 20180614
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FBuildWheel(void * Address[5])
{
	TraceWhel_Struct *WheelTrace;
	Whel_Struct *Wheel;
	uint8 *TraceDir;
	uint8  TracePos;
	uint16 buildWheelPos = 0;    //����λ��(0~������*4)
	uint8 i;
	uint8 WheelCnt;
	uint8 *pCmd;
	
	void *l_Address[4];
	int8 ret = -1;
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	
	uint16 pos = 0;
	uint32 sum_aver = 0;
	uint32 maxValue_aver = 0;

	uint8 ColumMin = 0xff;
	uint8 ColumMax = 0;
	uint8 Colum;
	uint8 PointTemp[2];
	uint8 ZTCntPerColum[2];
	
	WheelTrace = Address[0];
	Wheel = Address[1];
	TraceDir = Address[2];
	pCmd = Address[3];
	
	//���㽨��λ��
	if(*TraceDir == 0)
	{
		TracePos = WheelTrace->LeftFirstTracePos;
		for(i=0; i<WheelTrace->LeftWhlZTCntPerTrace[TracePos]; i++)
		{
			buildWheelPos += WheelTrace->LeftWhlZTXPos[TracePos][i];
		}
		if(WheelTrace->LeftWhlZTCntPerTrace[TracePos] > 0)
		{
			buildWheelPos = buildWheelPos / WheelTrace->LeftWhlZTCntPerTrace[TracePos] / 90;   //���㽨��λ��
		}		

		//������ֺ͵ľ�ֵ�����ֵ�ľ�ֵ
		sum_aver = 0;
		maxValue_aver = 0;
		for(i=0; i<WheelTrace->LeftWhlZTCntPerTrace[TracePos]; i++)
		{
			pos = WheelTrace->LeftWhlZTPos[TracePos][i];
			switch(WheelTrace->LeftWhlZTake[TracePos][i])
			{
				case 1:
					sum_aver += g_ChlData[pos].u32FirstZTSum;
					maxValue_aver += g_ChlData[pos].u16FirstZTMaxValue;
					break;
				case 2:
					sum_aver += g_ChlData[pos].u32SecondZTSum;
					maxValue_aver += g_ChlData[pos].u16SecondZTMaxValue;
					break;
				case 3:
					sum_aver += g_ChlData[pos].u32chlSum;
					maxValue_aver += g_ChlData[pos].u16ptMaxValue;
					break;
				default:break;
			}
		}
		if(WheelTrace->LeftWhlZTCntPerTrace[TracePos] > 0)
		{
			sum_aver = sum_aver / WheelTrace->LeftWhlZTCntPerTrace[TracePos];
			maxValue_aver = maxValue_aver / WheelTrace->LeftWhlZTCntPerTrace[TracePos];
		}
	}
	else
	{
		TracePos = WheelTrace->RightFirstTracePos;
		for(i=0; i<WheelTrace->RightWhlZTCntPerTrace[TracePos]; i++)
		{
			buildWheelPos += WheelTrace->RightWhlZTXPos[TracePos][i];
		}
		if(WheelTrace->RightWhlZTCntPerTrace[TracePos] > 0)
		{
			buildWheelPos = buildWheelPos / WheelTrace->RightWhlZTCntPerTrace[TracePos] / 90;   //���㽨��λ��
		}
		
		//������ֺ͵ľ�ֵ�����ֵ�ľ�ֵ
		sum_aver = 0;
		maxValue_aver = 0;
		for(i=0; i<WheelTrace->RightWhlZTCntPerTrace[TracePos]; i++)
		{
			pos = WheelTrace->RightWhlZTPos[TracePos][i];
			switch(WheelTrace->RightWhlZTake[TracePos][i])
			{
				case 1:
					sum_aver += g_ChlData[pos].u32FirstZTSum;
					maxValue_aver += g_ChlData[pos].u16FirstZTMaxValue;
					break;
				case 2:
					sum_aver += g_ChlData[pos].u32SecondZTSum;
					maxValue_aver += g_ChlData[pos].u16SecondZTMaxValue;
					break;
				case 3:
					sum_aver += g_ChlData[pos].u32chlSum;
					maxValue_aver += g_ChlData[pos].u16ptMaxValue;
					break;
				default:break;
			}
		}
		if(WheelTrace->RightWhlZTCntPerTrace[TracePos] > 0)
		{
			sum_aver = sum_aver / WheelTrace->RightWhlZTCntPerTrace[TracePos];
			maxValue_aver = maxValue_aver / WheelTrace->RightWhlZTCntPerTrace[TracePos];
		}
	}
	/**** ÿ�ν���ʱ��鱾�ν���λ���Ƿ��Ѿ�������,�������Ƚ����ֽ���һ��ƥ��,ƥ��ʧ���׶��� ****/
	if((Wheel[buildWheelPos].HaveWheelFlag == 1) && (Wheel[buildWheelPos].TriZTCnt > 0))
	{
		if(*pCmd == 1)
		{
			l_Address[0] = &Wheel[buildWheelPos];
			l_Address[1] = &buildWheelPos;    //��Pos
			ret = FMatchWheel(l_Address);
		}
		if((ret == 0) || (*pCmd == 2))    //ƥ��ʧ���׶���
		{
			//������Ϣ��������
			WheelCnt = 1;    
			l_Address[0] = &Wheel[buildWheelPos];
			l_Address[1] = &Wheel[buildWheelPos];
			l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
			l_Address[3] = &WheelCnt;
			FWheelBelong2Axle(l_Address);
				
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nMatchWhl Failed: 1 Single Wheel! %d[%d,%d],Dr=%d,[%d,%d,%d,%d,%d,%d].",
							buildWheelPos, Wheel[buildWheelPos].Point, Wheel[buildWheelPos].SgOrDbleWhel, Wheel[buildWheelPos].Dir, 
							Wheel[buildWheelPos].ZTList[0], Wheel[buildWheelPos].ZTList[1], Wheel[buildWheelPos].ZTList[2],
							Wheel[buildWheelPos].ZTList[3], Wheel[buildWheelPos].ZTList[4], Wheel[buildWheelPos].ZTList[5]);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
		
			//����Ѿ��׵��ֵ���Ϣ
			memset(&Wheel[buildWheelPos], 0, sizeof(Whel_Struct)); 
			//����
			l_Address[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
			l_Address[1] = &g_AxleInfo[g_u16AxleInfoWriteIndex].u8ChlNum;
			l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex].Dir;
			if(g_AxleInfo[g_u16AxleInfoWriteIndex].Dir == -1)
				l_Address[3] = &g_asReVehiclePerChannelForLoop[0][0];
			else
				l_Address[3] = &g_asVehiclePerChannelForLoop[0][0];
			FMatchAxleAndAxle(l_Address);
		}
	}
	else if((Wheel[buildWheelPos].HaveWheelFlag == 1) && (Wheel[buildWheelPos].TriZTCnt == 0))
	{
		//�����խ�������б�־�Ļ�����
		memset(&Wheel[buildWheelPos], 0, sizeof(Whel_Struct)); 
	}
	/**** end ****/	
	
	//����
	if(*TraceDir == 0)    //Left
	{
		//�����ֹ켣�а���������к���С��
		ColumMax = 0;
		ColumMin = 0xff;
		memset(PointTemp, 0, sizeof(PointTemp));
		memset(ZTCntPerColum, 0, sizeof(ZTCntPerColum));
		//������ѹ����
		for(i=0; i<WheelTrace->LeftWhlZTCntPerTrace[TracePos]; i++)
		{
			Colum = g_ChlData[WheelTrace->LeftWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
			if(Colum > ColumMax)
			{
				ColumMax = Colum;
			}
			if(Colum < ColumMin)
			{
				ColumMin = Colum;
			}
		}
		
		//����켣��ƽ������(�����жϵ�˫̥)
		for(i=0; i<WheelTrace->LeftWhlZTCntPerTrace[TracePos]; i++)
		{
			Colum = g_ChlData[WheelTrace->LeftWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
			//����ÿ�е�խ�������ܵ���
			if(Colum == ColumMax)
			{
				ZTCntPerColum[1]++;
				PointTemp[1] += WheelTrace->LeftPoint[TracePos][i];
			}
			else if(Colum == ColumMin && ColumMax != ColumMin)	
			{
				ZTCntPerColum[0]++;
				PointTemp[0] += WheelTrace->LeftPoint[TracePos][i];
			}
		}
		PointTemp[0] = (ZTCntPerColum[0] > 0) ? (PointTemp[0]*10 / ZTCntPerColum[0]) : 0;
		PointTemp[1] = (ZTCntPerColum[1] > 0) ? (PointTemp[1]*10 / ZTCntPerColum[1]) : 0;
		if(PointTemp[0] < 100 && PointTemp[0] > 0)
		{
			Wheel[buildWheelPos].Point = PointTemp[0] / 10 + (((PointTemp[0] % 10)<5) ? 0:1);
		}
		if(PointTemp[1] < 100 && PointTemp[1] > 0)
		{
			Wheel[buildWheelPos].Point += PointTemp[1] / 10 + (((PointTemp[1] % 10)<5) ? 0:1);
		}
		if(Wheel[buildWheelPos].Point <= g_u8SgOrDbleWhelPoint)
		{
			Wheel[buildWheelPos].SgOrDbleWhel = 1;    //1:��̥,2:˫̥,Ĭ��Ϊ0
		}
		else
		{
			Wheel[buildWheelPos].SgOrDbleWhel = 2;    //1:��̥,2:˫̥,Ĭ��Ϊ0
		}
		
		memcpy(Wheel[buildWheelPos].ZTList, WheelTrace->LeftWhlZTPos[TracePos], sizeof(uint16)*6);
		memcpy(Wheel[buildWheelPos].ZTakePos, WheelTrace->LeftWhlZTake[TracePos], sizeof(uint8)*6);
		memcpy(Wheel[buildWheelPos].WheelXPos, WheelTrace->LeftWhlZTXPos[TracePos], sizeof(uint16)*6);
		memcpy(Wheel[buildWheelPos].WheelYPos, WheelTrace->LeftWhlZTYPos[TracePos], sizeof(uint16)*6);
		Wheel[buildWheelPos].TriZTCnt = WheelTrace->LeftWhlZTCntPerTrace[TracePos];
		Wheel[buildWheelPos].Status = WheelTrace->LeftWhlStatus[TracePos];
		Wheel[buildWheelPos].HaveWheelFlag = 1;
		Wheel[buildWheelPos].SumAver = sum_aver;
		Wheel[buildWheelPos].MaxValueAver = maxValue_aver;
		Wheel[buildWheelPos].DueTime = WheelTrace->LeftDueTime;
		Wheel[buildWheelPos].Waiting4MatchFlag = 0;
		Wheel[buildWheelPos].Score = WheelTrace->LeftMultiScore[TracePos];
		
		
		//�ж��ֵ���ʻ״̬(�����ж�б��״̬)
		if(GetDiff(Wheel[buildWheelPos].WheelXPos[0], Wheel[buildWheelPos].WheelXPos[Wheel[buildWheelPos].TriZTCnt-1]) > XieZouDistance)
		{
			if(Wheel[buildWheelPos].WheelXPos[0] > Wheel[buildWheelPos].WheelXPos[Wheel[buildWheelPos].TriZTCnt-1])  //��б
				Wheel[buildWheelPos].Status = XIEZOULEFT;
			else      //��б
				Wheel[buildWheelPos].Status = XIEZOURIGHT;
		}
		else
		{
			Wheel[buildWheelPos].Status = WheelTrace->LeftWhlStatus[TracePos];
		}
			
		Wheel[buildWheelPos].Dir = WheelTrace->Dir;
		for(i=0; i<WheelTrace->LeftWhlZTCntPerTrace[TracePos]-2; i++)
		{
			Wheel[buildWheelPos].Spd += WheelTrace->LeftWhlSpd[TracePos][i];
		}
		if(WheelTrace->LeftWhlZTCntPerTrace[TracePos] > 1)
			Wheel[buildWheelPos].Spd /= (WheelTrace->LeftWhlZTCntPerTrace[TracePos]-1);    //�켣��ƽ���ٶ���Ϊ�ֵ��ٶ�
		
		/****���Դ���****/
		if(Wheel[buildWheelPos].ZTList[Wheel[buildWheelPos].TriZTCnt-1] == 48)
		{
			Wheel[buildWheelPos].ZTList[Wheel[buildWheelPos].TriZTCnt-1] = 48;
		}
		/**** end ****/
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPos=%d,ID=%d,tL=%d.", 
							Wheel[buildWheelPos].ZTList[Wheel[buildWheelPos].TriZTCnt-1], 
							WheelTrace->LeftWhlZTID[TracePos][WheelTrace->LeftWhlZTCntPerTrace[TracePos]-1], 
							WheelTrace->LeftDueTime);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	}
	else    //Right
	{
		//�����ֹ켣�а���������к���С��
		ColumMax = 0;
		ColumMin = 0xff;
		memset(PointTemp, 0, sizeof(PointTemp));
		memset(ZTCntPerColum, 0, sizeof(ZTCntPerColum));
		for(i=0; i<WheelTrace->RightWhlZTCntPerTrace[TracePos]; i++)
		{
			Colum = g_ChlData[WheelTrace->RightWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
			if(Colum > ColumMax)
			{
				ColumMax = Colum;
			}
			if(Colum < ColumMin)
			{
				ColumMin = Colum;
			}
		}
		//����켣��ƽ������(�����жϵ�˫̥)
		for(i=0; i<WheelTrace->RightWhlZTCntPerTrace[TracePos]; i++)
		{
			Colum = g_ChlData[WheelTrace->RightWhlZTPos[TracePos][i]].u8Id / PROMODPARAM.m_u8ZTRow;
			//����ÿ�е�խ�������ܵ���
			if(Colum == ColumMax)
			{
				ZTCntPerColum[1]++;
				PointTemp[1] += WheelTrace->RightPoint[TracePos][i];
			}
			else if(Colum == ColumMin && ColumMax != ColumMin)	
			{
				ZTCntPerColum[0]++;
				PointTemp[0] += WheelTrace->RightPoint[TracePos][i];
			}
		}
		PointTemp[0] = (ZTCntPerColum[0] > 0) ? (PointTemp[0]*10 / ZTCntPerColum[0]) : 0;
		PointTemp[1] = (ZTCntPerColum[1] > 0) ? (PointTemp[1]*10 / ZTCntPerColum[1]) : 0;
		if(PointTemp[0] < 100 && PointTemp[0] > 0)
		{
			Wheel[buildWheelPos].Point = PointTemp[0] / 10 + (((PointTemp[0] % 10)<5) ? 0:1);
		}
		if(PointTemp[1] < 100 && PointTemp[1] > 0)
		{
			Wheel[buildWheelPos].Point += PointTemp[1] / 10 + (((PointTemp[1] % 10)<5) ? 0:1);
		}
		if(Wheel[buildWheelPos].Point <= g_u8SgOrDbleWhelPoint)
		{
			Wheel[buildWheelPos].SgOrDbleWhel = 1;    //1:��̥,2:˫̥,Ĭ��Ϊ0
		}
		else
		{
			Wheel[buildWheelPos].SgOrDbleWhel = 2;    //1:��̥,2:˫̥,Ĭ��Ϊ0
		}
		
		memcpy(Wheel[buildWheelPos].ZTList, WheelTrace->RightWhlZTPos[TracePos], sizeof(uint16)*6);
		memcpy(Wheel[buildWheelPos].ZTakePos, WheelTrace->RightWhlZTake[TracePos], sizeof(uint8)*6);
		memcpy(Wheel[buildWheelPos].WheelXPos, WheelTrace->RightWhlZTXPos[TracePos], sizeof(uint16)*6);
		memcpy(Wheel[buildWheelPos].WheelYPos, WheelTrace->RightWhlZTYPos[TracePos], sizeof(uint16)*6);
		Wheel[buildWheelPos].TriZTCnt = WheelTrace->RightWhlZTCntPerTrace[TracePos];
		Wheel[buildWheelPos].HaveWheelFlag = 1;
		Wheel[buildWheelPos].SumAver = sum_aver;
		Wheel[buildWheelPos].MaxValueAver = maxValue_aver;
		Wheel[buildWheelPos].DueTime = WheelTrace->RightDueTime;
		Wheel[buildWheelPos].Waiting4MatchFlag = 0;
		Wheel[buildWheelPos].Score = WheelTrace->RightMultiScore[TracePos];
		//�ж��ֵ���ʻ״̬(�����ж�б��״̬)
		if(GetDiff(Wheel[buildWheelPos].WheelXPos[0], Wheel[buildWheelPos].WheelXPos[Wheel[buildWheelPos].TriZTCnt-1]) > XieZouDistance)
		{
			if(Wheel[buildWheelPos].WheelXPos[0] > Wheel[buildWheelPos].WheelXPos[Wheel[buildWheelPos].TriZTCnt-1])  //��б
				Wheel[buildWheelPos].Status = XIEZOULEFT;
			else      //��б
				Wheel[buildWheelPos].Status = XIEZOURIGHT;
		}
		else
			Wheel[buildWheelPos].Status = WheelTrace->RightWhlStatus[TracePos];
		Wheel[buildWheelPos].Dir = WheelTrace->Dir;

		for(i=0; i<WheelTrace->RightWhlZTCntPerTrace[TracePos]-2; i++)
		{
			Wheel[buildWheelPos].Spd += WheelTrace->RightWhlSpd[TracePos][i];
		}
		if(WheelTrace->RightWhlZTCntPerTrace[TracePos] > 1)
			Wheel[buildWheelPos].Spd /= (WheelTrace->RightWhlZTCntPerTrace[TracePos]-1);    //�켣��ƽ���ٶ���Ϊ�ֵ��ٶ�
		
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPos=%d,ID=%d,tR=%d.", 
							Wheel[buildWheelPos].ZTList[Wheel[buildWheelPos].TriZTCnt-1], 
							WheelTrace->RightWhlZTID[TracePos][WheelTrace->RightWhlZTCntPerTrace[TracePos]-1], 
							WheelTrace->RightDueTime);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	}
}

/*******************************************************************************************************
** Function name:  		FWhelMatchAxle
** Descriptions:   		������ƥ����ᣬ���Եó��������ʻ״̬
** input parameters:	�ֵĻ���
** output parameters:	
**
** Created by: liumingxin
** Created Date: 20180615
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FWhelMatchAxle(void * Address[2])
{
	Whel_Struct *Wheel;
// 	Whel_Struct *LeftWheel, *RightWheel;
	void *l_Address[4];
	uint32 *pTimeNow;
	uint8 i;
	int8 ret = 0xff;
	uint8 WheelCnt = 0;
	
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	
	pTimeNow = Address[0];
	Wheel = Address[1];
	
	//������ǰ������,�������ֳ���
	for(i=0; i<PROMODPARAM.m_u8LaneNum*4; i++)
	{
		ret = 0xff;
		//��ʱʱ�䵽,��λ�ֵĴ�ƥ���ʶ
		if(Wheel[i].HaveWheelFlag && (*pTimeNow > Wheel[i].DueTime) && (Wheel[i].DueTime > 0))    //������չ3���������ƥ��
		{
			Wheel[i].Waiting4MatchFlag = 1;
		}
		
		//��ƥ�����,�������������������(��չ3����Pos)
		if(Wheel[i].Waiting4MatchFlag == 1)
		{
			if(g_ChlData[Wheel[i].ZTList[2]].u8Id == 14)
				g_ChlData[Wheel[i].ZTList[2]].u8Id = 14;
			
			l_Address[0] = &Wheel[i];
			l_Address[1] = &i;    //��Pos
			ret = FMatchWheel(l_Address);
			if(ret == 0)    //ƥ��ʧ���׶���
			{
				//������Ϣ��������
				WheelCnt = 1;    
				l_Address[0] = &Wheel[i];
				l_Address[1] = &Wheel[i];
				l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				l_Address[3] = &WheelCnt;
				FWheelBelong2Axle(l_Address);
					
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nMatchWhl Failed: 2 SingleWheel! [%d,%d],Dr=%d,[%d,%d,%d,%d,%d,%d].",
								Wheel[i].Point, Wheel[i].SgOrDbleWhel, Wheel[i].Dir, 
								Wheel[i].ZTList[0], Wheel[i].ZTList[1], Wheel[i].ZTList[2],
								Wheel[i].ZTList[3], Wheel[i].ZTList[4], Wheel[i].ZTList[5]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			
				//����Ѿ��׵��ֵ���Ϣ
				memset(&Wheel[i], 0, sizeof(Whel_Struct)); 
				//����
				l_Address[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				l_Address[1] = &g_AxleInfo[g_u16AxleInfoWriteIndex].u8ChlNum;
				l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex].Dir;
				if(g_AxleInfo[g_u16AxleInfoWriteIndex].Dir == -1)
					l_Address[3] = &g_asReVehiclePerChannelForLoop[0][0];
				else
					l_Address[3] = &g_asVehiclePerChannelForLoop[0][0];
				FMatchAxleAndAxle(l_Address);
			}
		}
		
		//��Χû���ֻ�ƥ��ʧ������չ3����Posƥ��δ���ֵĹ켣
		
	}
}
/*******************************************************************************************************
** Function name:  		FMatchWheel
** Descriptions:   		�����ֽ���ƥ��
** input parameters:	��ǰ�ֺʹ�ƥ���ֵ���Ϣ
** output parameters:	
**
** Created by: liumingxin
** Created Date: 20180615
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
int8 FMatchWheel(void * Address[4])
{
	TraceWhel_Struct (*WheelTrace)[4];
	Whel_Struct *curWheel, *matchWheel;
	Whel_Struct *LeftWheel, *RightWheel;
	
	int8 i, j;
	uint8 X, Y;
	int8 ret = 0;            //-1:ʱ��ƥ��ʧ��, 1:ʱ��ƥ��ɹ�
	uint16 curWheelAverXPos = 0, matchWheelAverXPos = 0;
// 	uint16 buildWheelPos = 0;
	uint8 *pCurWheelPos;   
	int8 WheelMatchFlag = 0;     //0:��ƥ�����ʧ��,1:������ƥ�����ɹ�,����켣ƥ��ɹ�(��Ϊ����ֵ)
	uint8 ColumCnt;
	void* l_Address[7];
	uint8 ColumMin=0xff, ColumMax=0, Colum;
	uint8 temp;
	uint8 ZTID, matchTraceZTCnt;
	uint8 FirstTracePos = 0;
	uint8 TraceDir = 0;
	uint8 WheelCnt = 0;    //����
	int8 Dir = 0;
	uint8 *pMatchTraceZTake;
	uint8 cmd = 0;
	uint16 l_u16MaxWheelGauge = 0;
	uint16 l_u16MinWheelGauge = 0;
	
	
	uint8 MatchTraceZTID[6];    //��ƥ��켣ʱ�洢   -1��,��ǰ��(min, max),+1��
	int8 MatchTraceFlag[6];    //��ƥ��켣ʱƥ���ʶ,-1:ƥ��ʧ��,1:ƥ��ɹ�
	uint8 MatchTraceYPos[6];
	uint8 MatchTraceDir[6];
	uint16 MatchTraceAverXPos[6];
	uint8 MatchTraceFirstZTID[3];    //0:�洢������Ź켣��ZTID,1:�洢�Ҳ����Ź켣��ZTID,2:�洢����ѡ�������й켣��ZTID
	uint8 MatchTraceTemp[2];
	
	uint16 MatchTraceAverXPosTemp[2];
	
	uint8 l_u8Err;
	uint16 l_u16SendLen;
//	uint32 l_u32TimeNow;
	
	static uint8 l_u8cnt = 0;
	
	curWheel = Address[0];
	pCurWheelPos = Address[1];
	
	memset(MatchTraceZTID, -1, sizeof(uint8)*6);
	memset(MatchTraceFlag, -1, sizeof(int8)*6);
	memset(MatchTraceYPos, 0, sizeof(uint8)*6);
	memset(MatchTraceFirstZTID, -1, sizeof(uint8)*3);
	memset(MatchTraceDir, 0, sizeof(uint8)*6);
	memset(MatchTraceTemp, 0, sizeof(uint8)*2);
	memset(MatchTraceAverXPos, 0, sizeof(uint16)*6);
	memset(MatchTraceAverXPosTemp, 0, sizeof(uint16)*2);
	
	//����ּ�����С�ּ��
	l_u16MinWheelGauge = MinWheelGauge;
	if(curWheel->SumAver < SUMAVERLITVEH && curWheel->MaxValueAver < MAXVALUELITVEH)   //С��,�ּ����ֵ��󳵲�ͬ
	{
		l_u16MaxWheelGauge = MaxWheelGaugeLitVeh;
	}
	else
		l_u16MaxWheelGauge = MaxWheelGauge;
	
	for(i=0; i<curWheel->TriZTCnt; i++)
	{
		curWheelAverXPos += curWheel->WheelXPos[i];
	}
	curWheelAverXPos = curWheelAverXPos / curWheel->TriZTCnt;    //��ǰ�ֵ��ֹ켣ƽ��Xλ��(��λcm)
	
	//�������3����Pos������ƥ��
	if(*pCurWheelPos > 0)
	{
		ColumCnt = (*pCurWheelPos<3)?0:(*pCurWheelPos-3);
		for(i=*pCurWheelPos-1; i >= ColumCnt; i--)
		{
			matchWheel = &g_WhelData[i];
			
			if((matchWheel->HaveWheelFlag == 1) && (matchWheel->Dir == curWheel->Dir))
			{
				for(j=0; j<matchWheel->TriZTCnt; j++)
				{
					matchWheelAverXPos += matchWheel->WheelXPos[j];
				}
				matchWheelAverXPos = matchWheelAverXPos / matchWheel->TriZTCnt;   //��ƥ���ֵ��ֹ켣ƽ��Xλ��(��λcm)
				
				if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־�������ֵ�Ž���ƥ��
				{
					Dir = curWheel->Dir;
					l_Address[0] = curWheel->ZTList;
					l_Address[1] = matchWheel->ZTList;
					l_Address[2] = &curWheel->TriZTCnt;
					l_Address[3] = &matchWheel->TriZTCnt;
					l_Address[4] = curWheel->ZTakePos;
					l_Address[5] = matchWheel->ZTakePos;
					l_Address[6] = &Dir;
					ret = FWheelTimeMatch(l_Address);
// 					if(ret == 1)
// 					{
// 						l_Address[0] = curWheel->ZTList;
// 						l_Address[1] = matchWheel->ZTList;
// 						l_Address[2] = &curWheel->TriZTCnt;
// 						l_Address[3] = &matchWheel->TriZTCnt;
// 						l_Address[4] = curWheel->ZTakePos;
// 						l_Address[5] = matchWheel->ZTakePos;
// 						ret = FWheelWgtMatch(l_Address);
// 					}
				}
			}
			if(ret == 1)
			{
				WheelMatchFlag = 2;
				break;
			}
		}
	}
	
	//���ұ���3����Pos������ƥ��
	if(WheelMatchFlag == 0)
	{
		if(*pCurWheelPos+3 < 2*2*PROMODPARAM.m_u8LaneNum)
			ColumCnt = *pCurWheelPos + 3;
		else
			ColumCnt = 2*2*PROMODPARAM.m_u8LaneNum - 1;
		for(i=*pCurWheelPos+1; i <= ColumCnt; i++)
		{
			matchWheel = &g_WhelData[i];
			
			if((matchWheel->HaveWheelFlag == 1) && (matchWheel->Dir == curWheel->Dir))
			{
				for(j=0; j<matchWheel->TriZTCnt; j++)
				{
					matchWheelAverXPos += matchWheel->WheelXPos[j];
				}
				matchWheelAverXPos = matchWheelAverXPos / matchWheel->TriZTCnt;   //��ƥ���ֵ��ֹ켣ƽ��Xλ��(��λcm)
				
				if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־�������ֵ�Ž���ƥ��
				{
					Dir = curWheel->Dir;
					l_Address[0] = curWheel->ZTList;
					l_Address[1] = matchWheel->ZTList;
					l_Address[2] = &curWheel->TriZTCnt;
					l_Address[3] = &matchWheel->TriZTCnt;
					l_Address[4] = curWheel->ZTakePos;
					l_Address[5] = matchWheel->ZTakePos;
					l_Address[6] = &Dir;
					ret = FWheelTimeMatch(l_Address);
// 					if(ret == 1)
// 					{
// 						l_Address[0] = curWheel->ZTList;
// 						l_Address[1] = matchWheel->ZTList;
// 						l_Address[2] = &curWheel->TriZTCnt;
// 						l_Address[3] = &matchWheel->TriZTCnt;
// 						l_Address[4] = curWheel->ZTakePos;
// 						l_Address[5] = matchWheel->ZTakePos;
// 						ret = FWheelWgtMatch(l_Address);
// 					}
				}
			}
			if(ret == 1)
			{
				WheelMatchFlag = 1;
				break;
			}
		}
	}
	

	
	//��ƥ��ʧ�ܽ�������켣ƥ��
	if(WheelMatchFlag == 0)
	{
		Dir = curWheel->Dir;
		if(Dir == 1)
			WheelTrace = g_WheelTrace;
		else if(Dir == -1)
			WheelTrace = g_ReWheelTrace;
		
		/**** ���Դ��� ****/
		if(curWheel->ZTList[2] == 33)
		{
			curWheel->ZTList[2] = 33;
		}
		/**** end ****/
		
		//��ǰ�ְ��������Ҹ���1��Ѱ���Ƿ��й켣������ƥ��
		for(i=0; i<curWheel->TriZTCnt; i++)
		{
			Colum = g_ChlData[curWheel->ZTList[i]].u8Id / PROMODPARAM.m_u8ZTRow;
			if(Colum > ColumMax)
			{
				ColumMax = Colum;
			}
			if(Colum < ColumMin)
			{
				ColumMin = Colum;
			}
		}
		for(temp=0; temp<6; temp++)
		{
			switch(temp)
			{
				case 0:
					Colum = ColumMin-1;
					break;
				case 1:
					Colum = ColumMin;
					break;
				case 2:
					Colum = ColumMax;
					break;
				case 3:
					Colum = ColumMax+1;
					break;
				case 4:
					if((MatchTraceFlag[0] != 1) && (MatchTraceFlag[1] != 1) && 
					   (MatchTraceFlag[2] != 1) && (MatchTraceFlag[3] != 1))   //��ǰ�к�+-1��ûƥ���Ϲ켣����-2��
					{
						Colum = ColumMin-2;
					}
					break;
				case 5:
					if((MatchTraceFlag[0] != 1) && (MatchTraceFlag[1] != 1) && 
					   (MatchTraceFlag[2] != 1) && (MatchTraceFlag[3] != 1))   //��ǰ�к�+-1��ûƥ���Ϲ켣����+2��
					{
						Colum = ColumMin+2;
					}
					break;
				default:
					break;
			}

			if((temp > 3) && ((MatchTraceFlag[0] == 1) || (MatchTraceFlag[1] == 1) || 
			   (MatchTraceFlag[2] == 1) || (MatchTraceFlag[3] == 1)))    //+-1�к͵�ǰ����ƥ��ɹ����򲻼�����+-2��
			{
				break;
			}
			if((Colum >= PROMODPARAM.m_u8LaneNum*2) || 
			   (ColumMin == ColumMax && temp == 2))
			{
				continue;
			}
			//�ҳ���Ҫƥ����е��й켣�����խ��ID
			if(Dir == 1)   //��������ĩ��
			{
				for(i=PROMODPARAM.m_u8ZTRow-1; i>=0; i--)
				{
					ZTID = Colum*PROMODPARAM.m_u8ZTRow + i;
					if(WheelTrace[ZTID][0].HaveTraceFlag==1)
					{
						FTransLocation(ZTID, &X, &Y, 2);
						MatchTraceZTID[temp] = ZTID;
						MatchTraceYPos[temp] = Y;
						break;
					}
				}
			}
			else    //������������
			{
				for(i=0; i<PROMODPARAM.m_u8ZTRow; i++)
				{
					ZTID = Colum*PROMODPARAM.m_u8ZTRow + i;
					if(WheelTrace[ZTID][0].HaveTraceFlag==1)
					{
						FTransLocation(ZTID, &X, &Y, 2);
						MatchTraceZTID[temp] = ZTID;
						MatchTraceYPos[temp] = Y;
						break;
					}
				}
			}
			
			//���Ѿ��ҳ���խ���켣����ƥ��,���洢ƥ���־
			if(WheelTrace[ZTID][0].WheelCnt == 2 && WheelTrace[ZTID][0].HaveTraceFlag==1 && WheelTrace[ZTID][0].Dir == Dir)
			{
				if(Colum < ColumMin || Colum == ColumMax)
				{
					l_Address[0] = &WheelTrace[ZTID][0];
					FFindFirstTracePos(l_Address);     //������ѡ�켣λ��
					
					FirstTracePos = WheelTrace[ZTID][0].RightFirstTracePos;
					matchTraceZTCnt = WheelTrace[ZTID][0].RightWhlZTCntPerTrace[FirstTracePos];
					pMatchTraceZTake = WheelTrace[ZTID][0].RightWhlZTake[FirstTracePos];
					matchWheelAverXPos = 0;
					for(j=0; j<matchTraceZTCnt; j++)
					{
						matchWheelAverXPos += WheelTrace[ZTID][0].RightWhlZTXPos[FirstTracePos][j];
					}
					if(matchTraceZTCnt > 0)
					{
						matchWheelAverXPos = matchWheelAverXPos / matchTraceZTCnt;
						MatchTraceAverXPos[temp] = matchWheelAverXPos;
					}
					if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־����120cm�Ž���ƥ��
					{
						l_Address[0] = curWheel->ZTList;
						l_Address[1] = &WheelTrace[ZTID][0].RightWhlZTPos[FirstTracePos];
						l_Address[2] = &curWheel->TriZTCnt;
						l_Address[3] = &matchTraceZTCnt;
						l_Address[4] = curWheel->ZTakePos;
						l_Address[5] = pMatchTraceZTake;
						l_Address[6] = &Dir;
						ret = FWheelTimeMatch(l_Address);
						MatchTraceFlag[temp] = ret;
						MatchTraceDir[temp] = 1;
					}
				}
				else if(Colum > ColumMax || Colum == ColumMin)
				{
					l_Address[0] = &WheelTrace[ZTID][0];
					FFindFirstTracePos(l_Address);     //������ѡ�켣λ��
					
					FirstTracePos = WheelTrace[ZTID][0].LeftFirstTracePos;
					matchTraceZTCnt = WheelTrace[ZTID][0].LeftWhlZTCntPerTrace[FirstTracePos];
					pMatchTraceZTake = WheelTrace[ZTID][0].LeftWhlZTake[FirstTracePos];
					matchWheelAverXPos = 0;
					for(j=0; j<matchTraceZTCnt; j++)
					{
						matchWheelAverXPos += WheelTrace[ZTID][0].LeftWhlZTXPos[FirstTracePos][j];
					}
					if(matchTraceZTCnt > 0)
					{
						matchWheelAverXPos = matchWheelAverXPos / matchTraceZTCnt;
						MatchTraceAverXPos[temp] = matchWheelAverXPos;
					}
					if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־����120cm�Ž���ƥ��
					{
						l_Address[0] = curWheel->ZTList;
						l_Address[1] = &WheelTrace[ZTID][0].LeftWhlZTPos[FirstTracePos];
						l_Address[2] = &curWheel->TriZTCnt;
						l_Address[3] = &matchTraceZTCnt;
						l_Address[4] = curWheel->ZTakePos;
						l_Address[5] = pMatchTraceZTake;
						l_Address[6] = &Dir;
						ret = FWheelTimeMatch(l_Address);
						MatchTraceFlag[temp] = ret;
						MatchTraceDir[temp] = 0;
					}
				}
			}
			else if(WheelTrace[ZTID][0].WheelCnt == 1 && WheelTrace[ZTID][0].HaveTraceFlag==1 && WheelTrace[ZTID][0].Dir == Dir)
			{
				l_Address[0] = &WheelTrace[ZTID][0];
				FFindFirstTracePos(l_Address);     //������ѡ�켣λ��
				
				if(WheelTrace[ZTID][0].LeftTraceCnt > 0)
				{
					FirstTracePos = WheelTrace[ZTID][0].LeftFirstTracePos;
					matchTraceZTCnt = WheelTrace[ZTID][0].LeftWhlZTCntPerTrace[FirstTracePos];
					pMatchTraceZTake = WheelTrace[ZTID][0].LeftWhlZTake[FirstTracePos];
					matchWheelAverXPos = 0;
					for(j=0; j<matchTraceZTCnt; j++)
					{
						matchWheelAverXPos += WheelTrace[ZTID][0].LeftWhlZTXPos[FirstTracePos][j];
					}
					if(matchTraceZTCnt > 0)
					{
						matchWheelAverXPos = matchWheelAverXPos / matchTraceZTCnt;
						MatchTraceAverXPos[temp] = matchWheelAverXPos;
					}
					if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־����120cm�Ž���ƥ��
					{
						l_Address[0] = curWheel->ZTList;
						l_Address[1] = &WheelTrace[ZTID][0].LeftWhlZTPos[FirstTracePos];
						l_Address[2] = &curWheel->TriZTCnt;
						l_Address[3] = &matchTraceZTCnt;
						l_Address[4] = curWheel->ZTakePos;
						l_Address[5] = pMatchTraceZTake;
						l_Address[6] = &Dir;
						ret = FWheelTimeMatch(l_Address);
						MatchTraceFlag[temp] = ret;
						MatchTraceDir[temp] = 0;
					}
				}
				else if(WheelTrace[ZTID][0].RightTraceCnt > 0)
				{
					FirstTracePos = WheelTrace[ZTID][0].RightFirstTracePos;
					matchTraceZTCnt = WheelTrace[ZTID][0].RightWhlZTCntPerTrace[FirstTracePos];
					pMatchTraceZTake = WheelTrace[ZTID][0].RightWhlZTake[FirstTracePos];
					matchWheelAverXPos = 0;
					for(j=0; j<matchTraceZTCnt; j++)
					{
						matchWheelAverXPos += WheelTrace[ZTID][0].RightWhlZTXPos[FirstTracePos][j];
					}
					if(matchTraceZTCnt > 0)
					{
						matchWheelAverXPos = matchWheelAverXPos / matchTraceZTCnt;
						MatchTraceAverXPos[temp] = matchWheelAverXPos;
					}
					if(GetDiff(curWheelAverXPos, matchWheelAverXPos) > l_u16MinWheelGauge && GetDiff(curWheelAverXPos, matchWheelAverXPos) < l_u16MaxWheelGauge)   //�־����120cm�Ž���ƥ��
					{
						l_Address[0] = curWheel->ZTList;
						l_Address[1] = &WheelTrace[ZTID][0].RightWhlZTPos[FirstTracePos];
						l_Address[2] = &curWheel->TriZTCnt;
						l_Address[3] = &matchTraceZTCnt;
						l_Address[4] = curWheel->ZTakePos;
						l_Address[5] = pMatchTraceZTake;
						l_Address[6] = &Dir;
						ret = FWheelTimeMatch(l_Address);
						MatchTraceFlag[temp] = ret;
						MatchTraceDir[temp] = 1;
					}
				}
			}
		}
		
		//ѡ��һ�����ŵĹ켣���������
		if(MatchTraceFlag[0]==1 && MatchTraceFlag[1]==1)
		{
			if(MatchTraceYPos[0] > MatchTraceYPos[1])    //-1�е�Yλ�ô��ڵ�ǰ��(min)
			{
				FirstTracePos = WheelTrace[MatchTraceZTID[0]][0].LeftFirstTracePos;
				for(i=0; i<WheelTrace[MatchTraceZTID[0]][0].LeftWhlZTCntPerTrace[FirstTracePos]; i++)
				{
					if(WheelTrace[MatchTraceZTID[0]][0].LeftWhlZTID[FirstTracePos][i] == MatchTraceZTID[1])   //-1�й켣�е�խ��������ǰ�е���ѡ�켣խ��
					{
						MatchTraceFirstZTID[0] = MatchTraceZTID[0];
						MatchTraceTemp[0] = MatchTraceDir[0];
						MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[0];
						break;
					}
				}
				if(i >= WheelTrace[MatchTraceZTID[0]][0].LeftWhlZTCntPerTrace[FirstTracePos])   //-1�й켣�е�խ����������ǰ�е���ѡ�켣խ��
				{
					MatchTraceFirstZTID[0] = MatchTraceZTID[1];
					MatchTraceTemp[0] = MatchTraceDir[1];
					MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[1];
				}
			}
			else if(MatchTraceYPos[0] < MatchTraceYPos[1])
			{
				MatchTraceFirstZTID[0] = MatchTraceZTID[1];
				MatchTraceTemp[0] = MatchTraceDir[1];
				MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[1];
			}
		}
		else if(MatchTraceFlag[0]==1 && MatchTraceFlag[1]!=1)
		{
			MatchTraceFirstZTID[0] = MatchTraceZTID[0];
			MatchTraceTemp[0] = MatchTraceDir[0];
			MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[0];
		}
		else if(MatchTraceFlag[0]!=1 && MatchTraceFlag[1]==1)
		{
			MatchTraceFirstZTID[0] = MatchTraceZTID[1];
			MatchTraceTemp[0] = MatchTraceDir[1];
			MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[1];
		}
		
		if(MatchTraceFlag[3]==1 && MatchTraceFlag[2]==1)
		{
			if(MatchTraceYPos[3] > MatchTraceYPos[2])    //+1�е�Yλ�ô��ڵ�ǰ��(min)
			{
				FirstTracePos = WheelTrace[MatchTraceZTID[3]][0].LeftFirstTracePos;
				for(i=0; i<WheelTrace[MatchTraceZTID[3]][0].LeftWhlZTCntPerTrace[FirstTracePos]; i++)
				{
					if(WheelTrace[MatchTraceZTID[3]][0].LeftWhlZTID[FirstTracePos][i] == MatchTraceZTID[1])   //-1�й켣�е�խ��������ǰ�е���ѡ�켣խ��
					{
						MatchTraceFirstZTID[1] = MatchTraceZTID[3];
						MatchTraceTemp[1] = MatchTraceDir[3];
						MatchTraceAverXPosTemp[1] = MatchTraceAverXPos[3];
						break;
					}
				}
				if(i >= WheelTrace[MatchTraceZTID[3]][0].LeftWhlZTCntPerTrace[FirstTracePos])   //-1�й켣�е�խ����������ǰ�е���ѡ�켣խ��
				{
					MatchTraceFirstZTID[1] = MatchTraceZTID[2];
					MatchTraceTemp[1] = MatchTraceDir[2];
					MatchTraceAverXPosTemp[1] = MatchTraceAverXPos[2];
				}
			}
			else if(MatchTraceYPos[3] < MatchTraceYPos[2])
			{
				MatchTraceFirstZTID[1] = MatchTraceZTID[2];
				MatchTraceTemp[1] = MatchTraceDir[2];
				MatchTraceAverXPosTemp[1] = MatchTraceAverXPos[2];
			}
		}
		else if(MatchTraceFlag[3]==1 && MatchTraceFlag[2]!=1)
		{
			MatchTraceFirstZTID[1] = MatchTraceZTID[3];
			MatchTraceTemp[1] = MatchTraceDir[3];
			MatchTraceAverXPosTemp[1] = MatchTraceAverXPos[3];
		}
		else if(MatchTraceFlag[3]!=1 && MatchTraceFlag[2]==1)
		{
			MatchTraceFirstZTID[1] = MatchTraceZTID[2];
			MatchTraceTemp[1] = MatchTraceDir[2];
			MatchTraceAverXPosTemp[1] = MatchTraceAverXPos[2];
		}
		
		if(MatchTraceFlag[5]==1 && MatchTraceFlag[4]==1)
		{
			MatchTraceFirstZTID[0] = MatchTraceZTID[5];
			MatchTraceTemp[0] = MatchTraceDir[5];
			MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[5];
		}
		else if(MatchTraceFlag[5]==1 && MatchTraceFlag[4]!=1)
		{
			MatchTraceFirstZTID[0] = MatchTraceZTID[5];
			MatchTraceTemp[0] = MatchTraceDir[5];
			MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[5];
		}
		else if(MatchTraceFlag[5]!=1 && MatchTraceFlag[4]==1)
		{
			MatchTraceFirstZTID[0] = MatchTraceZTID[4];
			MatchTraceTemp[0] = MatchTraceDir[4];
			MatchTraceAverXPosTemp[0] = MatchTraceAverXPos[4];
		}
		
		//ѡ��������ƥ���������Ź켣
		if(MatchTraceFirstZTID[0] != 0xff && MatchTraceFirstZTID[1] != 0xff)
		{
			//����б�з���ѡ�����Ź켣,����б��ѡ�Ҳ�Ĺ켣,����б��ѡ�����켣
			if(curWheel->Status == XIEZOULEFT || curWheel->Status == XIEZOURIGHT)    //��б��
			{
				if(curWheel->Status == XIEZOULEFT)    //��б
				{
					MatchTraceFirstZTID[2] = MatchTraceFirstZTID[0];
					TraceDir = MatchTraceTemp[0];
					matchWheelAverXPos = MatchTraceAverXPosTemp[0];
				}
				else if(curWheel->Status == XIEZOURIGHT)   //��б
				{
					MatchTraceFirstZTID[2] = MatchTraceFirstZTID[1];
					TraceDir = MatchTraceTemp[1];
					matchWheelAverXPos = MatchTraceAverXPosTemp[1];
				}
			}
			else    //��б��
			{
				if((curWheelAverXPos/180)%2 == 0) //ż�����������Ҳ�,���������������
				{
					MatchTraceFirstZTID[2] = MatchTraceFirstZTID[1];   
					TraceDir = MatchTraceTemp[1];
					matchWheelAverXPos = MatchTraceAverXPosTemp[1];
				}
				else
				{
					MatchTraceFirstZTID[2] = MatchTraceFirstZTID[0];   
					TraceDir = MatchTraceTemp[0];
					matchWheelAverXPos = MatchTraceAverXPosTemp[0];
				}
			}
		}
		else if(MatchTraceFirstZTID[0] != 0xff && MatchTraceFirstZTID[1] == 0xff)
		{
			MatchTraceFirstZTID[2] = MatchTraceFirstZTID[0];
			TraceDir = MatchTraceTemp[0];
			matchWheelAverXPos = MatchTraceAverXPosTemp[0];
		}
		else if(MatchTraceFirstZTID[0] == 0xff && MatchTraceFirstZTID[1] != 0xff)
		{
			MatchTraceFirstZTID[2] = MatchTraceFirstZTID[1];
			TraceDir = MatchTraceTemp[1];
			matchWheelAverXPos = MatchTraceAverXPosTemp[1];
		}
		
		
		if(MatchTraceFirstZTID[2] != 0xff)
		{
			l_u8cnt++;
			if(l_u8cnt == 2)
			{
				l_u8cnt = 2;
			}
			WheelMatchFlag = 2;
			
			l_Address[0] = &WheelTrace[MatchTraceFirstZTID[2]][0];
			FFindFirstTracePos(l_Address);     //������ѡ�켣λ��
			
			cmd = 2;
			l_Address[0] = &WheelTrace[MatchTraceFirstZTID[2]][0];
			l_Address[1] = g_WhelData;
			l_Address[2] = &TraceDir;
			l_Address[3] = &cmd;
			FBuildWheel(l_Address);
			
			matchWheel = &g_WhelData[matchWheelAverXPos/90];
			//......//
			
			//ɾ���Ѿ�ƥ��õ�խ��
			l_Address[0] = &WheelTrace[MatchTraceFirstZTID[2]][0];
			l_Address[1] = &TraceDir;
			FDeleteTrace(l_Address);    //ɾ�������Ѿ����ֵ�խ�������й켣
			
// 			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"trace [%d,%d][%d,%d,%d,%d][%d,%d,%d,%d][%d,%d,%d,%d,%d,%d][%d,%d,%d]%d\r\n",
// 							ColumMin, ColumMax, MatchTraceZTID[0], MatchTraceZTID[1], MatchTraceZTID[2], MatchTraceZTID[3],
// 							MatchTraceDir[0],MatchTraceDir[1],MatchTraceDir[2],MatchTraceDir[3],curWheel->ZTList[0],curWheel->ZTList[1],
// 							curWheel->ZTList[2],curWheel->ZTList[3],curWheel->ZTList[4],curWheel->ZTList[5],MatchTraceFirstZTID[0],MatchTraceFirstZTID[1],MatchTraceFirstZTID[2],
// 							TraceDir);
// 			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 			g_u16CurDebugInd++;
// 			g_u16CurDebugInd %= COM_NUM;
// 			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 			OSSemPost(g_pSemSendThtouSP);
		}
	}
	
	if(WheelMatchFlag > 0)
	{
		//����ʹ��
		if(g_u16CurDebugInd==4)
		{
			g_u16CurDebugInd = 2;
		}
		//ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��

		WheelCnt = 2;    //ƥ��ɹ�,��������Ϊ2
		//������Ϣ��������
		if(curWheelAverXPos < matchWheelAverXPos)
		{
			if(curWheel->Dir == 1)
			{
				LeftWheel = curWheel;
				RightWheel = matchWheel;
			}
			else if(curWheel->Dir == -1)
			{
				LeftWheel = matchWheel;
				RightWheel = curWheel;
			}
		}
		else
		{
			if(curWheel->Dir == 1)
			{
				LeftWheel = matchWheel;
				RightWheel = curWheel;
			}
			else if(curWheel->Dir == -1)
			{
				LeftWheel = curWheel;
				RightWheel = matchWheel;
			}
		}
		
		l_Address[0] = LeftWheel;
		l_Address[1] = RightWheel;
		l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
		l_Address[3] = &WheelCnt;
		FWheelBelong2Axle(l_Address);
		
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nMatchWhl: %d,[%d,%d],[%d,%d],Dr=%d.",
						WheelMatchFlag, LeftWheel->Point, LeftWheel->SgOrDbleWhel, RightWheel->Point, RightWheel->SgOrDbleWhel, LeftWheel->Dir);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
		
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nL=[%d,%d,%d,%d,%d,%d],R=[%d,%d,%d,%d,%d,%d].",
						LeftWheel->ZTList[0],LeftWheel->ZTList[1],LeftWheel->ZTList[2],LeftWheel->ZTList[3],LeftWheel->ZTList[4],LeftWheel->ZTList[5],
						RightWheel->ZTList[0],RightWheel->ZTList[1],RightWheel->ZTList[2],RightWheel->ZTList[3],RightWheel->ZTList[4],RightWheel->ZTList[5]);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
		
		//����Ѿ�ƥ�����������ֵ���Ϣ
		memset(curWheel, 0, sizeof(Whel_Struct));    
		memset(matchWheel, 0, sizeof(Whel_Struct));
		
		//����
		if(g_u16AxleInfoWriteIndex == 1)
			g_u16AxleInfoWriteIndex = 1;
		l_Address[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
		l_Address[1] = &g_AxleInfo[g_u16AxleInfoWriteIndex].u8ChlNum;
		l_Address[2] = &g_AxleInfo[g_u16AxleInfoWriteIndex].Dir;
		if(g_AxleInfo[g_u16AxleInfoWriteIndex].Dir == -1)
			l_Address[3] = &g_asReVehiclePerChannelForLoop[0][0];
		else
			l_Address[3] = &g_asVehiclePerChannelForLoop[0][0];
		FMatchAxleAndAxle(l_Address);
		
		//�����һ����ṹ��
// 		g_u16AxleInfoWriteIndex++;
// 		g_u16AxleInfoWriteIndex %= AXLE_BUFLEN;
// 		memset(&g_AxleInfo[g_u16AxleInfoWriteIndex],0,sizeof(Axle_Struct));
	}

	return WheelMatchFlag;
}

/*******************************************************************************************************
** Function name:  		FWheelBelong2Axle
** Descriptions:   		������Ϣ��������
** input parameters:	����Ϣ(���ֻ�˫��)
** output parameters:	
**
** Created by: liumingxin
** Created Date: 20180626
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
void FWheelBelong2Axle(void * Address[4])
{
	Whel_Struct *LeftWheel, *RightWheel;
	Axle_Struct *AxleForChannel;
	uint8 i;
	uint8 *pWheelCnt;
	uint16 LeftWheelAverXPos = 0, RightWheelAverXPos = 0;
	uint32 l_u32TimeNow;
	
	LeftWheel = Address[0];
	RightWheel = Address[1];
	AxleForChannel = Address[2];
	pWheelCnt = Address[3];
	
	ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
	if(*pWheelCnt == 2)    //˫��
	{
		for(i=0; i<LeftWheel->TriZTCnt; i++)
		{
			LeftWheelAverXPos += LeftWheel->WheelXPos[i];
		}
		LeftWheelAverXPos = LeftWheelAverXPos / LeftWheel->TriZTCnt;    //��ǰ�ֵ��ֹ켣ƽ��Xλ��(��λcm)
		for(i=0; i<RightWheel->TriZTCnt; i++)
		{
			RightWheelAverXPos += RightWheel->WheelXPos[i];
		}
		RightWheelAverXPos = RightWheelAverXPos / RightWheel->TriZTCnt;    //��ǰ�ֵ��ֹ켣ƽ��Xλ��(��λcm)
		
		//����ɵ������ֵ���Ϣ��������
		AxleForChannel->ZType = 1;    //����խ��
//		AxleForChannel->u8id = g_u16AxleInfoWriteIndex;
		AxleForChannel->u8ChlNum = (LeftWheelAverXPos + RightWheelAverXPos) / 2 / 180;   //����������
		AxleForChannel->MidXPos = (LeftWheelAverXPos + RightWheelAverXPos) / 2;   //���м�λ��,��λcm
		AxleForChannel->LeftWheelAverPos = LeftWheelAverXPos;
		AxleForChannel->RightWheelAverPos = RightWheelAverXPos;
		AxleForChannel->WheelType = LeftWheel->SgOrDbleWhel;

		for(i = 0; i < LeftWheel->TriZTCnt; i++) //����
		{
			AxleForChannel->u16LeftWheeList[i] = LeftWheel->ZTList[i];;
			AxleForChannel->u8LeftWheelTakeZT[i] = LeftWheel->ZTakePos[i];	
			AxleForChannel->u16LeftWheelMaxValueAver += g_ChlData[AxleForChannel->u16LeftWheeList[i]].u16ptMaxValue;	
		}
		if(LeftWheel->TriZTCnt > 0)
			AxleForChannel->u16LeftWheelMaxValueAver = AxleForChannel->u16LeftWheelMaxValueAver / LeftWheel->TriZTCnt;
		for(i = 0; i < RightWheel->TriZTCnt; i++) //����
		{
			AxleForChannel->u16RightWheeList[i] = RightWheel->ZTList[i];
			AxleForChannel->u8RightWheelTakeZT[i] = RightWheel->ZTakePos[i];	
			AxleForChannel->u16RightWheelMaxValueAver += g_ChlData[AxleForChannel->u16RightWheeList[i]].u16ptMaxValue;			
		}
		if(RightWheel->TriZTCnt > 0)
			AxleForChannel->u16RightWheelMaxValueAver = AxleForChannel->u16RightWheelMaxValueAver / RightWheel->TriZTCnt;
		
		AxleForChannel->BuildLeftWhelXRange = LeftWheel->WheelXPos[0];
		AxleForChannel->BuildRghWhelXRange = RightWheel->WheelXPos[0];
		AxleForChannel->LeftWheelXRange = LeftWheel->WheelXPos[LeftWheel->TriZTCnt-1];
		AxleForChannel->RightWheelXRange = RightWheel->WheelXPos[RightWheel->TriZTCnt-1];
		AxleForChannel->LeftWheelZTCnt = LeftWheel->TriZTCnt;
		AxleForChannel->RightWheelZTCnt = RightWheel->TriZTCnt;
		AxleForChannel->AxleWhelCnt = *pWheelCnt;
		AxleForChannel->u8PointsGet = (LeftWheel->Score + RightWheel->Score) / 2;
		AxleForChannel->Dir = LeftWheel->Dir;
		AxleForChannel->AxleStatus = F_Max(LeftWheel->Status, RightWheel->Status);
		AxleForChannel->ZTLayouType = JCLAYOUT;
		AxleForChannel->PostAxleTime = l_u32TimeNow;
	}
	else if(*pWheelCnt == 1)    //����
	{
		for(i=0; i<LeftWheel->TriZTCnt; i++)
		{
			LeftWheelAverXPos += LeftWheel->WheelXPos[i];
		}
		LeftWheelAverXPos = LeftWheelAverXPos / LeftWheel->TriZTCnt;    //��ǰ�ֵ��ֹ켣ƽ��Xλ��(��λcm)
		//����ɵ������ֵ���Ϣ��������
		AxleForChannel->ZType = 1;    //����խ��
//		AxleForChannel->u8id = g_u16AxleInfoWriteIndex;
		AxleForChannel->u8ChlNum = LeftWheelAverXPos / 180;   //����������
		AxleForChannel->MidXPos = LeftWheelAverXPos;
		AxleForChannel->LeftWheelAverPos = LeftWheelAverXPos;
		AxleForChannel->RightWheelAverPos = 0;
		AxleForChannel->WheelType = LeftWheel->SgOrDbleWhel;
		
		for(i = 0; i < LeftWheel->TriZTCnt; i++) //����
		{
			AxleForChannel->u16LeftWheeList[i] = LeftWheel->ZTList[i];;
			AxleForChannel->u8LeftWheelTakeZT[i] = LeftWheel->ZTakePos[i];	
			AxleForChannel->u16LeftWheelMaxValueAver += g_ChlData[AxleForChannel->u16LeftWheeList[i]].u16ptMaxValue;	
		}
		if(LeftWheel->TriZTCnt > 0)
			AxleForChannel->u16LeftWheelMaxValueAver = AxleForChannel->u16LeftWheelMaxValueAver / LeftWheel->TriZTCnt;
		
		AxleForChannel->BuildLeftWhelXRange = LeftWheel->WheelXPos[0];
		AxleForChannel->LeftWheelXRange = LeftWheel->WheelXPos[LeftWheel->TriZTCnt-1];
	    AxleForChannel->LeftWheelZTCnt = LeftWheel->TriZTCnt;
		AxleForChannel->AxleWhelCnt = *pWheelCnt;
		AxleForChannel->u8PointsGet = LeftWheel->Score;
		AxleForChannel->Dir = LeftWheel->Dir;
		AxleForChannel->AxleStatus = LeftWheel->Status;
		AxleForChannel->ZTLayouType = JCLAYOUT;
		AxleForChannel->PostAxleTime = l_u32TimeNow;
	}
}

/*******************************************************************************************************
** Function name:  		FWheelTimeMatch
** Descriptions:   		�����ֽ���ʱ��ƥ��
** input parameters:	��ǰ�ֺʹ�ƥ���ֵ���Ϣ
** output parameters:	
**
** Created by: liumingxin
** Created Date: 20180625
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
int8 FWheelTimeMatch(void * Address[7])
{
	uint16 *curWheelZTList, *matchWheelZTList;
// 	uint8 *curWheelZTTake, *matchWheelZTTake;
	uint8  *curWheelZTCnt, *matchWheelZTCnt;
	uint8 curWheelZTRow, matchWheelZTRow;    //�ְ�����խ��������(0~5)
	uint32 curWheelZTUpTime, matchWheelZTUpTime;    //�����ϳ�ʱ��
	uint32 curWheelZTDnTime, matchWheelZTDnTime;    //�����³�ʱ��
	uint8  curZTID, matchZTID;
	uint16 curZTPos, matchZTPos;
	uint8 overlap = 0;    //�غ϶�0~100
	uint8 i, j;
	uint8 X, Y;
	int8 *pDir;
	int8 ret = 1;    //1:ƥ��ɹ�,-1:ƥ��ʧ��  (Ĭ��Ϊ1,ƥ���������ƥ��ʧ�ܵ���������-1)
	
	uint8  l_u8Err;
	uint16 l_u16SendLen;
	
	curWheelZTList = Address[0];
	matchWheelZTList = Address[1];
	curWheelZTCnt = Address[2];
	matchWheelZTCnt = Address[3];
// 	curWheelZTTake = Address[4];
// 	matchWheelZTTake = Address[5];
	pDir = Address[6];
	
	//����խ�������Ž���ʱ��ƥ��
	for(i=0; i<*curWheelZTCnt; i++)
	{
		curZTPos = curWheelZTList[i];
		curZTID = g_ChlData[curZTPos].u8Id;
		curWheelZTUpTime = g_ChlData[curZTPos].u32uptm;
		curWheelZTDnTime = g_ChlData[curZTPos].u32dntm;
		
		FTransLocation(curZTID, &X, &Y, 2);
		curWheelZTRow = Y;
		for(j=0; j<*matchWheelZTCnt; j++)
		{
			matchZTPos = matchWheelZTList[j];
			matchZTID = g_ChlData[matchZTPos].u8Id;
			matchWheelZTUpTime = g_ChlData[matchZTPos].u32uptm;
			matchWheelZTDnTime = g_ChlData[matchZTPos].u32dntm;
			
			FTransLocation(matchZTID, &X, &Y, 2);
			matchWheelZTRow = Y;
			
			//����С���ȴ���,������ĺ󴥷�;����ƥ��ʧ��,��ͬխ����Takeλ�ò�����ͬ
			if(*pDir == 1)    //����
			{
				if((matchWheelZTRow > curWheelZTRow) && (matchWheelZTUpTime < curWheelZTUpTime))
					ret = -1;
				if((matchWheelZTRow < curWheelZTRow) && (matchWheelZTUpTime > curWheelZTUpTime))
					ret = -1;
// 				if((matchZTID == curZTID) && (curWheelZTTake[i] == matchWheelZTTake[j]))
// 					ret = -1;
				
				if(matchWheelZTRow == curWheelZTRow && matchZTID != curZTID)    //������ͬ��խ��
				{
					overlap = FOverlapRatio(curWheelZTUpTime, curWheelZTDnTime, matchWheelZTUpTime, matchWheelZTDnTime);
					if(overlap < g_ZTWaveOverLap)
					{
						//ret = -1;
					}
					
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nTime match overlap=%d,id=[%d,%d],pos=[%d,%d].",
										overlap, curZTID, matchZTID, curWheelZTList[i], matchWheelZTList[j]);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
			}
			else if(*pDir == -1)    //����
			{
				if((matchWheelZTRow < curWheelZTRow) && (matchWheelZTUpTime < curWheelZTUpTime))
					ret = -1;
				if((matchWheelZTRow > curWheelZTRow) && (matchWheelZTUpTime > curWheelZTUpTime))
					ret = -1;
// 				if((matchZTID == curZTID) && (curWheelZTTake[i] == matchWheelZTTake[j]))
// 					ret = -1;
				
				if(matchWheelZTRow == curWheelZTRow && matchZTID != curZTID)    //������ͬ��խ��
				{
					overlap = FOverlapRatio(curWheelZTUpTime, curWheelZTDnTime, matchWheelZTUpTime, matchWheelZTDnTime);
					if(overlap < g_ZTWaveOverLap)
					{
						//ret = -1;
					}
					
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nTime match overlap=%d,id=[%d,%d],pos=[%d,%d].",
										overlap, curZTID, matchZTID, curWheelZTList[i], matchWheelZTList[j]);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
			}
			else
			{
				ret = -1;
			}
			
			if(ret == -1)
				break;
		}
		if(ret == -1)
			break;
	}
	
	return ret;
}

/*******************************************************************************************************
** Function name:  		FOverlapRatio
** Descriptions:   		��������խ�����³�ʱ����غ϶�
** input parameters:	����խ�������³�ʱ��
** output parameters:	�غ϶�
**
** Created by: liumingxin
** Created Date: 20181129
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
uint8 FOverlapRatio(uint32 up1, uint32 dn1, uint32 up2, uint32 dn2)
{
	uint8 OverlapRatio = 0;    //�غ϶�0~100
	
	//�����غ϶�
	if(up1 <= up2)
	{
		if(dn2 <= dn1)
		{
			OverlapRatio = 100;
		}
		else
		{
			if(up2 > dn1)
			{
				OverlapRatio = 0;
			}
			else
			{
				OverlapRatio = GetDiff(dn1, up2) * 100 / GetDiff(up1, dn2);
			}
		}
	}
	else
	{
		if(dn1 < dn2)
		{
			OverlapRatio = 100;
		}
		else
		{
			if(up1 > dn2)
			{
				OverlapRatio = 0;
			}
			else
			{
				OverlapRatio = GetDiff(up1, dn2) * 100 / GetDiff(up2, dn1);
			}
		}
	}
	return OverlapRatio;
}

/*******************************************************************************************************
** Function name:  		FWheelWgtMatch
** Descriptions:   		�����ֽ�������ƥ��
** input parameters:	��ǰ�ֺʹ�ƥ���ֵ���Ϣ
** output parameters:	
**
** Created by: liumingxin
** Created Date: 20180808
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:
*******************************************************************************************************/
int8 FWheelWgtMatch(void * Address[6])
{
	uint16 *curWheelZTList, *matchWheelZTList;
	uint8  *curWheelZTCnt, *matchWheelZTCnt;
	uint8  *curWheelZTake, *matchWheelZTake;
	uint16 curWheelWgt, matchWheelWgt, WheelWgtTemp;
	uint8 curColumZTPos[2][3], matchColumZTPos[2][3];
	uint8 curColumZTake[2][3], matchColumZTake[2][3];
	uint8 curColumZTCnt[2], matchColumZTCnt[2];
	uint8 CoulumMax = 0, ColumMin = 0xff, Colum = 0;   //�洢ÿ���ֵ���
	uint8 i, ZTID;
	uint32 spd = 0, spdMax = 0;    //�ٶ�,��λmm/s
	uint32 sum = 0;    //���ֺ�
	uint8 WheelWgtRatio;    //���ر���ֵ
	uint32 MaxSpdThred = 100;   //��λmm/s
	int8 ret = 1;    //1:ƥ��ɹ�,-1:ƥ��ʧ��,2:������ٶȹ�С
	
	//������Ϣʹ��
	uint32 curSpd = 0, matchSpd = 0;
	uint32 dt1 = 0, dt2 = 0, dt3 = 0;
	uint32 sum1 = 0, sum2 = 0, sum3 = 0;
	
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	
	curWheelZTList = Address[0];
	matchWheelZTList = Address[1];
	curWheelZTCnt = Address[2];
	matchWheelZTCnt = Address[3];
	curWheelZTake = Address[4];
	matchWheelZTake = Address[5];

	spd = 0;
	curWheelWgt = 0;
	matchWheelWgt = 0;

	memset(curColumZTCnt, 0, sizeof(curColumZTCnt));
	memset(matchColumZTCnt, 0, sizeof(matchColumZTCnt));
	memset(curColumZTPos, 0, sizeof(curColumZTPos));
	memset(matchColumZTPos, 0, sizeof(matchColumZTPos));
	memset(curColumZTake, 0, sizeof(curColumZTake));
	memset(matchColumZTake, 0, sizeof(matchColumZTake));
	
	/**** ����ÿ���ֵ����� ****/
	//���㵱ǰ������
	CoulumMax = 0;
	ColumMin = 0xff;
	for(i = 0; i < *curWheelZTCnt; i++)
	{
		ZTID = g_ChlData[curWheelZTList[i]].u8Id;
		Colum = g_ChlData[curWheelZTList[i]].u8Id / PROMODPARAM.m_u8ZTRow;
		if(Colum >= CoulumMax)
		{
			CoulumMax = Colum;
		}
		if(Colum <= ColumMin)
		{
			ColumMin = Colum;
		}
	}
	for(i = 0; i < *curWheelZTCnt; i++)
	{
		ZTID = g_ChlData[curWheelZTList[i]].u8Id;
		Colum = g_ChlData[curWheelZTList[i]].u8Id / PROMODPARAM.m_u8ZTRow;
		if(Colum == CoulumMax)
		{
			curColumZTPos[1][curColumZTCnt[1]] = curWheelZTList[i];
			curColumZTake[1][curColumZTCnt[1]] = curWheelZTake[i];
			curColumZTCnt[1]++;
		}
		if(Colum == ColumMin)
		{
			curColumZTPos[0][curColumZTCnt[0]] = curWheelZTList[i];
			curColumZTake[0][curColumZTCnt[0]] = curWheelZTake[i];
			curColumZTCnt[0]++;
		}
	}
	if(CoulumMax == ColumMin)   //����
	{
		spdMax = 0;
		WheelWgtTemp = 0;
		//ʹ�ø�������ٶ������������
		for(i = 0; i < curColumZTCnt[1]-1; i++)
		{
			ZTID = g_ChlData[curColumZTPos[1][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[curWheelZTList[i]].u32uptm, g_ChlData[curWheelZTList[i+1]].u32uptm);   //mm/s
			
			if(spd > spdMax)
			{
				dt3 = GetDiff(g_ChlData[curWheelZTList[i]].u32uptm, g_ChlData[curWheelZTList[i+1]].u32uptm);
				spdMax = spd;
				switch(curColumZTake[1][i])
				{
					case 1:
						sum = g_ChlData[curColumZTPos[1][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[curColumZTPos[1][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[curColumZTPos[1][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				sum3 = sum;
				WheelWgtTemp = sum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		curWheelWgt = WheelWgtTemp;
	}
	else    //����
	{
		spdMax = 0;
		WheelWgtTemp = 0;
		//ʹ��ÿ������ٶ������������
		for(i = 0; i < curColumZTCnt[0]-1; i++)
		{
			ZTID = g_ChlData[curColumZTPos[0][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[curWheelZTList[i]].u32uptm, g_ChlData[curWheelZTList[i+1]].u32uptm);   //mm/s
			
			if(spd > spdMax)
			{
				spdMax = spd;
				switch(curColumZTake[0][i])
				{
					case 1:
						sum = g_ChlData[curColumZTPos[0][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[curColumZTPos[0][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[curColumZTPos[0][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				WheelWgtTemp = sum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		curWheelWgt = WheelWgtTemp;
		
		spdMax = 0;
		WheelWgtTemp = 0;
		for(i = 0; i < curColumZTCnt[1]-1; i++)
		{
			ZTID = g_ChlData[curColumZTPos[1][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[curWheelZTList[i]].u32uptm, g_ChlData[curWheelZTList[i+1]].u32uptm);   //mm/s
			
			if(spd > spdMax)
			{
				spdMax = spd;
				switch(curColumZTake[1][i])
				{
					case 1:
						sum = g_ChlData[curColumZTPos[1][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[curColumZTPos[1][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[curColumZTPos[1][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				WheelWgtTemp = sum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		curWheelWgt += WheelWgtTemp;
	}
	curSpd = spdMax;
	
	//�����ƥ��������
	CoulumMax = 0;
	ColumMin = 0xff;
	for(i = 0; i < *matchWheelZTCnt; i++)
	{
		ZTID = g_ChlData[matchWheelZTList[i]].u8Id;
		Colum = g_ChlData[matchWheelZTList[i]].u8Id / PROMODPARAM.m_u8ZTRow;
		if(Colum >= CoulumMax)
		{
			CoulumMax = Colum;
		}
		if(Colum <= ColumMin)
		{
			ColumMin = Colum;
		}
	}
	for(i = 0; i < *matchWheelZTCnt; i++)
	{
		ZTID = g_ChlData[matchWheelZTList[i]].u8Id;
		Colum = g_ChlData[matchWheelZTList[i]].u8Id / PROMODPARAM.m_u8ZTRow;
		if(Colum == CoulumMax)
		{
			matchColumZTPos[1][matchColumZTCnt[1]] = matchWheelZTList[i];
			matchColumZTake[1][matchColumZTCnt[1]] = matchWheelZTake[i];
			matchColumZTCnt[1]++;
		}
		if(Colum == ColumMin)
		{
			matchColumZTPos[0][matchColumZTCnt[0]] = matchWheelZTList[i];
			matchColumZTake[0][matchColumZTCnt[0]] = matchWheelZTake[i];
			matchColumZTCnt[0]++;
		}
	}
	if(CoulumMax == ColumMin)   //����
	{
		spdMax = 0;
		WheelWgtTemp = 0;
		matchWheelWgt = 0;
		//ʹ�ø�������ٶ������������
		for(i = 0; i < matchColumZTCnt[1]-1; i++)
		{
			ZTID = g_ChlData[matchColumZTPos[1][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[matchWheelZTList[i]].u32uptm, g_ChlData[matchWheelZTList[i+1]].u32uptm);   //mm/s
			if(spd > spdMax)
			{
				spdMax = spd;
				switch(matchColumZTake[1][i])
				{
					case 1:
						sum = g_ChlData[matchColumZTPos[1][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[matchColumZTPos[1][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[matchColumZTPos[1][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				WheelWgtTemp = sum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		matchWheelWgt = WheelWgtTemp;
	}
	else    //����
	{
		spdMax = 0;
		WheelWgtTemp = 0;
		matchWheelWgt = 0;
		//ʹ��ÿ������ٶ������������
		for(i = 0; i < matchColumZTCnt[0]-1; i++)
		{
			ZTID = g_ChlData[matchColumZTPos[0][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[matchWheelZTList[i]].u32uptm, g_ChlData[matchWheelZTList[i+1]].u32uptm);   //mm/s
			if(spd > spdMax)
			{
				spdMax = spd;
				dt1 = GetDiff(g_ChlData[matchWheelZTList[i]].u32uptm, g_ChlData[matchWheelZTList[i+1]].u32uptm);
				switch(matchColumZTake[0][i])
				{
					case 1:
						sum = g_ChlData[matchColumZTPos[0][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[matchColumZTPos[0][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[matchColumZTPos[0][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				sum1 = sum; 
				WheelWgtTemp = sum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		matchWheelWgt = WheelWgtTemp;
		
		spdMax = 0;
		WheelWgtTemp = 0;
		for(i = 0; i < matchColumZTCnt[1]-1; i++)
		{
			ZTID = g_ChlData[matchColumZTPos[1][i]].u8Id;
			spd = PROMODPARAM.m_au16ZTGap[ZTID] * 6000 / GetDiff(g_ChlData[matchWheelZTList[i]].u32uptm, g_ChlData[matchWheelZTList[i+1]].u32uptm);   //mm/s
			if(spd > spdMax)
			{
				dt2 = (g_ChlData[matchWheelZTList[i]].u8Id<<8) | g_ChlData[matchWheelZTList[i+1]].u8Id;
				spdMax = spd;
				
				switch(matchColumZTake[1][i])
				{
					case 1:
						sum = g_ChlData[matchColumZTPos[1][i]].u32FirstZTSum;
						break;
					case 2:
						sum = g_ChlData[matchColumZTPos[1][i]].u32SecondZTSum;
						break;
					case 3:
						sum = g_ChlData[matchColumZTPos[1][i]].u32chlSum;
						break;
					default:
						break;
					
				}
				sum2 = sum;
				WheelWgtTemp = g_ChlData[matchColumZTPos[1][i]].u32chlSum * spd /1000 / 300;
			}
		}
		if(spdMax < MaxSpdThred)
		{
			return 2;
		}
		matchWheelWgt += WheelWgtTemp;
	}
	matchSpd = spdMax;
	
	// if(curWheelWgt < 1000 && matchWheelWgt < 1000)    //��С��1t�Ĳ��������ر��ж�
	// {
		// return 1;
	// }
	//���ر��ж�
	WheelWgtRatio = GetDiff(curWheelWgt, matchWheelWgt) * 100 / F_Max(curWheelWgt, matchWheelWgt);
	if(WheelWgtRatio < g_u32WheelWghtCoin)
		ret = 1;
	else
		ret = -1;
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"WhlWtMtch,ret=%d,rat=%d,[%d,%d],[%d,%d],[%d,%d],[%d,%d]\r\n",
						ret, WheelWgtRatio, sum1,sum2,dt1,dt2,dt3,sum3,curSpd,matchSpd);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
	
	return ret;
}


/*********************************************************************************************************
** Function name:  FHideZTPostVehForNormal
** Descriptions: ����խ���׳�
** input parameters:   
** output parameters:  
**
** Created by: zhangtonghan		  
** Created Date: 2017-10-30
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------*/
void  FHideZTPostVehForNormal(void* Address[9])
{
	//uint32 *pTime;
	VehicleInfForLoop_Struct (*VehForwChannelStruct)[CHL_SAVEVEH]; //����������
//	VehicleInfForLoop_Struct (*VehReveChannelStruct)[CHL_SAVEVEH]; //����
	VehicleInfForLoop_Struct (*VehHideZTForStruct)[CHL_SAVEVEH]; //����խ������������
	VehicleInfForLoop_Struct (*VehHideZTForRevStruct)[CHL_SAVEVEH]; //����
	uint8 *l_au8VehPositionFlag;//��Ȧ״̬
	uint8 *l_au8ReVehPositionFlag;
	uint32 *l_DownBackLoopTime; //��ǰ����Ȧ��ʱ��
	uint8 l_u8temp,l_u8tempi,l_u8Cnt=0,laneNum;
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	void*l_u32AddressForPo[3];
	int8 Dir;
//	pTime									= Address[0];
	VehForwChannelStruct		= Address[1];
//	VehReveChannelStruct		= Address[2];
	VehHideZTForStruct			= Address[3];
	VehHideZTForRevStruct		= Address[4];
	l_au8VehPositionFlag		= Address[5];
	l_au8ReVehPositionFlag		= Address[6];
	l_DownBackLoopTime			= Address[7];

	for(l_u8temp = 0;l_u8temp < PROMODPARAM.m_u8LaneNum;l_u8temp++)
	{
		//if(l_au8VehPositionFlag[l_u8temp] == 0x03)
		if(l_au8ReVehPositionFlag[l_u8temp] == 0x03)
		{
			if((VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) && (VehForwChannelStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) && 
			   (l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				//��Ͷ�Ӧ���˳�	
				VehForwChannelStruct[2*l_u8temp][0].u8HideAixCnt = l_u8Cnt;	
				VehForwChannelStruct[2*l_u8temp][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 0;
				}	

				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n1.Forw Car Mat Zhonghang=%d,Hide=%d,%d.",
											 2*l_u8temp,2*l_u8temp,l_u8Cnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
			}			
			else if((l_u8temp < PROMODPARAM.m_u8LaneNum) && (VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) && (VehForwChannelStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1) &&\
				(l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp+1][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				VehForwChannelStruct[2*l_u8temp+1][0].u8HideAixCnt = l_u8Cnt;	
				VehForwChannelStruct[2*l_u8temp+1][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp+1][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp+1][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 0;
				}
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n2.Forw Car Mat Zhonghang=%d,Hide=%d,%d.",
											 2*l_u8temp+1,2*l_u8temp,l_u8Cnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
					
			}
			else if((l_u8temp > 0) && (VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) && (VehForwChannelStruct[2*l_u8temp-1][0].m_u8VehicleHaveFlag == 1) &&\
				(l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp-1][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				VehForwChannelStruct[2*l_u8temp-1][0].u8HideAixCnt = l_u8Cnt;	
				VehForwChannelStruct[2*l_u8temp-1][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp-1][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp-1][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp][0].m_n8CurrentHideAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag = 0;
				}
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n3.Forw Car Mat Zhonghang=%d,Hide=%d,%d.",
											 2*l_u8temp-1,2*l_u8temp,l_u8Cnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
			}
			else if(VehHideZTForStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) //���������խ��û�����к�խ����
			{
				FInitVehStr(&VehHideZTForStruct[2*l_u8temp][0],1);
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nThe Hide Lane Nun %d No Match ZhongHang.", 2*l_u8temp);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
				//����խ��+1���г�
			if((VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1)&& (VehForwChannelStruct[2*l_u8temp][0].m_u8VehicleHaveFlag == 1) && (l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				//��Ͷ�Ӧ���˳�	
				VehForwChannelStruct[2*l_u8temp][0].u8HideAixCnt = l_u8Cnt;	
				VehForwChannelStruct[2*l_u8temp][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp+1][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 0;
				}
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n4.Forw Car Mat Zhonghang=%d,Hide=%d,%d.",
											 2*l_u8temp,2*l_u8temp+1,l_u8Cnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}			
			else if((l_u8temp < PROMODPARAM.m_u8LaneNum) && (VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1) && (VehForwChannelStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1) &&\
				(l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp+1][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				VehForwChannelStruct[2*l_u8temp+1][0].u8HideAixCnt = l_u8Cnt;
				VehForwChannelStruct[2*l_u8temp+1][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp+1][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp+1][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp+1][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 0;
				}
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n5.Forw Car Mat Zhonghang=%d,Hide=%d,%d.",
											 2*l_u8temp+1,2*l_u8temp+1,l_u8Cnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else if((l_u8temp > 0) && (VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1) && (VehForwChannelStruct[2*l_u8temp+2][0].m_u8VehicleHaveFlag == 1) &&\
				(l_DownBackLoopTime[l_u8temp] > VehForwChannelStruct[2*l_u8temp+2][0].m_u32LoopTime))
			{
				for(l_u8tempi=0;l_u8tempi <= VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum;l_u8tempi++)
				{
					if(VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] == 1)
					{
						l_u8Cnt++;
					}
				}
				VehForwChannelStruct[2*l_u8temp+2][0].u8HideAixCnt = l_u8Cnt;	
				VehForwChannelStruct[2*l_u8temp+2][0].m_u8HideLogiclvl = VehHideZTForStruct[2*l_u8temp+1][0].m_u8HideLogiclvl;
				memcpy(&VehForwChannelStruct[2*l_u8temp+2][0].u16HideAxleList,&VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList,l_u8Cnt*sizeof(uint16));
				memcpy(&VehForwChannelStruct[2*l_u8temp+2][0].au16HideAxleWeight,&VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight,l_u8Cnt*sizeof(uint16));
				for(l_u8tempi=0;l_u8tempi < 12-l_u8Cnt;l_u8tempi++)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].u16HideAxleList[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi] 	= VehHideZTForStruct[2*l_u8temp+1][0].m_au8AxleDropFlag[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi] 		= VehHideZTForStruct[2*l_u8temp+1][0].m_an8AxleProRow[l_u8tempi+l_u8Cnt];
					VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi]	=	VehHideZTForStruct[2*l_u8temp+1][0].au16HideAxleWeight[l_u8tempi+l_u8Cnt];
				}
				VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum = VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentAxleNum-l_u8Cnt;
				if(VehHideZTForStruct[2*l_u8temp+1][0].m_n8CurrentHideAxleNum >= 0)
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 1;
				}
				else
				{
					VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag = 0;
				}
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n6.Forw Car Mat Zhonghang=%d,Hide=%d,AXleNum=%d,%d.",
											 2*l_u8temp+2,2*l_u8temp+1,l_u8Cnt,VehForwChannelStruct[2*l_u8temp+2][0].u8HideAixCnt);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else if(VehHideZTForStruct[2*l_u8temp+1][0].m_u8VehicleHaveFlag == 1)  //���������խ��û�����к�խ����ɾ��������
			{
				FInitVehStr(&VehHideZTForStruct[2*l_u8temp+1][0],1);
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"The Hide Lane Nun %d No Match ZhongHang.\r\n", 2*l_u8temp+1);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
		 
		}
	}
	
	
	if(l_au8ReVehPositionFlag[3] == 0x03)//һ����(ʹ��ǰ����Ȧ���)
	{
		Dir = -1;
		for(l_u8temp = 0; l_u8temp <= 2; l_u8temp++)
		{
			laneNum = l_u8temp;
			if((VehHideZTForRevStruct[laneNum][0].m_u8VehicleHaveFlag == 1) && (VehHideZTForRevStruct[laneNum][0].u8AixCnt > 0))
			{
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &VehHideZTForRevStruct[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo,laneNum+30);//�׳�
			}
		}
	}
	if(l_au8VehPositionFlag[3] == 0x03)//������(ʹ�ú�����Ȧ���)
	{
		Dir = -1;
		for(l_u8temp = 1; l_u8temp <= 3; l_u8temp++)
		{
			laneNum = l_u8temp;
			if((VehHideZTForRevStruct[laneNum][0].m_u8VehicleHaveFlag == 1) && (VehHideZTForRevStruct[laneNum][0].u8AixCnt > 0))
			{
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &VehHideZTForRevStruct[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo, l_u8temp + 40);//�׳�
			}
		}
	}
	if(l_au8ReVehPositionFlag[4] == 0x03)//������(ʹ��ǰ����Ȧ���)
	{
		Dir = -1;
		for(l_u8temp = 2; l_u8temp <= 4; l_u8temp++)
		{
			laneNum = l_u8temp;
			if((VehHideZTForRevStruct[laneNum][0].m_u8VehicleHaveFlag == 1) && (VehHideZTForRevStruct[laneNum][0].u8AixCnt > 0))
			{
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &VehHideZTForRevStruct[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo, l_u8temp + 40);//�׳�
			}
		}
	}
}


/*********************************************************************************************************
** Function name:     BuildNewVeh
** Descriptions:      С������ʱ�½���
** input parameters:      
** output parameters:   
**
** Created by:	wanglihui  
** Created Date:  2018-01-04	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void BuildNewVeh(void *AddressForPo[3])
{
	uint8 l_u8ChlNum,*Dir;
	VehicleInfForLoop_Struct (*VehForwChannelStruct)[CHL_SAVEVEH]; 
	Axle_Struct *AxleForChannel;
	uint8 l_u8tempi,l_tempos;
	//uint8 l_u8ZTid;
	void * AxleAddressed[3];
// 	void *	l_u32AddrFcap[4];
	uint32 l_u32TimeNow;
//	uint8 l_u8headortail=0;
	
	AxleForChannel = AddressForPo[0];
	Dir = AddressForPo[1];
	VehForwChannelStruct = AddressForPo[2];
	l_u8ChlNum = AxleForChannel->u8ChlNum;
	//�½���
	VehForwChannelStruct[l_u8ChlNum][0].m_u8VehicleHaveFlag = 1;
	VehForwChannelStruct[l_u8ChlNum][0].m_n8CurrentAxleNum = 0;
	VehForwChannelStruct[l_u8ChlNum][0].AxleStatus[0]=AxleForChannel->AxleStatus;
	VehForwChannelStruct[l_u8ChlNum][0].m_u8DireFlag = *Dir;
	l_tempos=F_FindEnd8(AxleForChannel->u16ChList,MAXNUM_AXLE);
	for(l_u8tempi = 0;l_u8tempi < l_tempos; l_u8tempi++)
	{
		VehForwChannelStruct[l_u8ChlNum][0].m_an16CurrentAxleInf[0][l_u8tempi]=AxleForChannel->u16ChList[l_u8tempi];							
	}
	ReadFPGAGlobalTime(l_u32TimeNow);
	VehForwChannelStruct[l_u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
	VehForwChannelStruct[l_u8ChlNum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[l_u8ChlNum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[l_u8ChlNum]& 0x0f));
	
// 	l_u32AddrFcap[0]	= &(VehForwChannelStruct[l_u8ChlNum][0].m_Capture_LaneNum);
// 	l_u32AddrFcap[1]	= &l_u32TimeNow;
// 	l_u32AddrFcap[2]	= Dir;
// 	l_u32AddrFcap[3]	= &l_u8headortail;
// 	FCapture(l_u32AddrFcap,1);
// 	FCaptureInfoRec(l_u32AddrFcap);   //��¼��һ��ץ�ĵ���Ϣ
// 	VehForwChannelStruct[l_u8ChlNum][0].m_u32LoopTime=l_u32TimeNow;
	
	AxleAddressed[0] = &VehForwChannelStruct[l_u8ChlNum][0];
	AxleAddressed[1] = &l_u8ChlNum;
	AxleAddressed[2] = Dir;
	FProPostAxle(AxleAddressed,8);//����
}

/*********************************************************************************************************
** Function name:     ColumInit
** Descriptions:      ��ʼ��ĳһ��
** input parameters:  �нṹ�����飬λ��     
** output parameters:   
**
** Created by:	zhangtonghan  
** Created Date:  2017-10-18	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ColumHideInit(HideColum_Struct* l_pArrStr,uint8 Pos)
{
	HideColum_Struct* pdata;
	pdata=l_pArrStr;
	memset((pdata+Pos),0,sizeof(HideColum_Struct));
	memset((pdata+Pos)->Colum,-1,2*sizeof(int16));	
}

/*********************************************************************************************************
** Function name:     MoveHideColumElem
** Descriptions:      �ƶ��е�Ԫ��
** input parameters:  �нṹ�����飬����         
** output parameters:   
**
** Created by:	zhangtonghan  
** Created Date:  2017-10-18	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void MoveHideColumElem(HideColum_Struct* l_pArrStr,uint8 length)
{
	uint8 l_u8temp;
	
	for(l_u8temp=0;l_u8temp < length-1; l_u8temp++)
	{
		*(l_pArrStr+l_u8temp)=*(l_pArrStr+l_u8temp+1);
	}
	ColumHideInit(l_pArrStr,length-1);
}

/*********************************************************************************************************
** Function name:     FindHideColumEnd
** Descriptions:      
** input parameters:  �нṹ�����飬����  ,����      
** output parameters:   �յ�λ��
**
** Created by:	zhangtonghan  
** Created Date:  2017-10-18	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 FindHideColumEnd(HideColum_Struct* l_pArrStr,uint8 length,uint8 cmd)
{
	uint8 l_u8temp=0,l_u8lenth;
	HideColum_Struct *p_Arr;
	
	p_Arr=l_pArrStr;
	l_u8lenth=length;
	for(l_u8temp=0; l_u8temp < l_u8lenth; l_u8temp++)
	{
		if((p_Arr+l_u8temp)->Colum[0] == -1) 
			break;
	}
	if((l_u8temp == l_u8lenth) && (cmd == 0))
	{
		MoveHideColumElem(p_Arr,l_u8lenth);
		return (l_u8lenth-1);
	}
	else
		return l_u8temp;
}
/*********************************************************************************************************
** Function name:     FCommMtchZTFunc
** Descriptions:      ͨ��ƥ��խ������
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 20171031
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FCommMtchZTFunc(void * Address[9])
{
	ZTINFRECELEMENT *lpZTInf;
	HideColum_Struct (*ZTColum)[HIDECOLUM_SAVEAXlE]; 
// 	VehicleInfForLoop_Struct (*VehStruct)[CHL_SAVEVEH]; 
	uint8 *pl_u8X,l_u8X;
	uint8 l_u8Pos,l_u8tempi,l_u8MtchCnt,l_u8MtchLCnt,l_u8MtchRCnt,l_u8MtchPos,cmd;
	uint32 l_u32difUpTime = 0;
//	uint8 l_u8WaitRowCnt,l_u8Wait2RowCnt,l_u8NoMtchCnt;
	void *l_u32CoinMatchAddr[6];
	void* l_u32LftAndRghtMtch[6];
	int8 *Dir;
	uint32 WheelWght,Wait4WheelWght,Wait4WheelWght2;
// uint32 LargeRange,LitRange,LargeRange2,LitRange2
	Dir			 	= Address[0];
	ZTColum		= Address[1];
	//VehStruct	= Address[2];
	lpZTInf		= Address[3];
	pl_u8X		= Address[4];
	
	l_u8X		=	*pl_u8X;
	
	
	l_u8MtchCnt=0;
	l_u8MtchPos=0;
	l_u8MtchLCnt=l_u8MtchRCnt=0;
	l_u8Pos= 0;
//	l_u8MtchCnt=l_u8WaitRowCnt=l_u8Wait2RowCnt=l_u8NoMtchCnt=0;
	l_u8Pos = FindHideColumEnd(&ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,1);
	for(l_u8tempi = 0;l_u8tempi < l_u8Pos;l_u8tempi++)
	{
		if(ZTColum[l_u8X][l_u8tempi].ZTCnt == 1)
		{
			l_u8MtchCnt++;
			l_u8MtchPos = l_u8tempi;
		}
	}
	if((l_u8Pos == 1) && (l_u8MtchCnt == 0))
	{
		ZTColum[l_u8X][1].Colum[0] = lpZTInf->u16ID;
		ZTColum[l_u8X][1].FristElemRow = 1;
		ZTColum[l_u8X][1].Dir = *Dir;
		ZTColum[l_u8X][1].ZTCnt++;
		ZTColum[l_u8X][1].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
		return;
	}
	else
	{
		if((l_u8Pos == 0)&&(l_u8MtchCnt == 0))//��Ӧ������û��խ��
		{
			//Cmd: 1:���;2:�Ҳ�;3:����
			if((l_u8X > 0) && (l_u8X < (2*PROMODPARAM.m_u8LaneNum-1))) //��Ҫ�鿴���������խ��
			{
				l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);			
				l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
				if((l_u8MtchLCnt == 0) && (l_u8MtchRCnt >0))
				{
					cmd = 2;
					l_u32LftAndRghtMtch[0] = &cmd;
					l_u32LftAndRghtMtch[1] = Address[0];
					l_u32LftAndRghtMtch[2] = Address[1];
					l_u32LftAndRghtMtch[3] = Address[3];
					l_u32LftAndRghtMtch[4] = Address[4];
					
					FMtchLeftAndRghtColum(l_u32LftAndRghtMtch);
					
				}
				else if((l_u8MtchLCnt > 0) && (l_u8MtchRCnt == 0))
				{
					cmd = 1;
					l_u32LftAndRghtMtch[0] = &cmd;
					l_u32LftAndRghtMtch[1] = Address[0];
					l_u32LftAndRghtMtch[2] = Address[1];
					l_u32LftAndRghtMtch[3] = Address[3];
					l_u32LftAndRghtMtch[4] = Address[4];
					
					FMtchLeftAndRghtColum(l_u32LftAndRghtMtch);
					
				}
				else if((l_u8MtchLCnt > 0) && (l_u8MtchRCnt > 0))
				{
					cmd = 3;
					l_u32LftAndRghtMtch[0] = &cmd;
					l_u32LftAndRghtMtch[1] = Address[0];
					l_u32LftAndRghtMtch[2] = Address[1];
					l_u32LftAndRghtMtch[3] = Address[3];
					l_u32LftAndRghtMtch[4] = Address[4];
					
					FMtchLeftAndRghtColum(l_u32LftAndRghtMtch);				
				}
				else
				{
					if(*Dir == 1)
					{
						l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;	
						g_u8XieLaneFlag[l_u8X]=1;
					}
					else if(*Dir == -1)
					{
						l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;	
					}
				}
				//if
			}	
			else if(l_u8X == 0)
			{
				l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
				if(l_u8MtchRCnt > 0)
				{
					cmd = 2;
					l_u32LftAndRghtMtch[0] = &cmd;
					l_u32LftAndRghtMtch[1] = Address[0];
					l_u32LftAndRghtMtch[2] = Address[1];
					l_u32LftAndRghtMtch[3] = Address[3];
					l_u32LftAndRghtMtch[4] = Address[4];
					
					FMtchLeftAndRghtColum(l_u32LftAndRghtMtch);			
				}
				else
				{
					if(*Dir == 1)
					{
						l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;				
					}
					else if(*Dir == -1)
					{
						l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;	
					}
				}
			}
			else if(l_u8X == (2*PROMODPARAM.m_u8LaneNum-1))
			{
				l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
				if(l_u8MtchLCnt > 0)
				{
					l_u32LftAndRghtMtch[0] = &cmd;
					l_u32LftAndRghtMtch[1] = Address[0];
					l_u32LftAndRghtMtch[2] = Address[1];
					l_u32LftAndRghtMtch[3] = Address[3];
					l_u32LftAndRghtMtch[4] = Address[4];
					
					FMtchLeftAndRghtColum(l_u32LftAndRghtMtch);
					
				}
				else
				{
					if(*Dir == 1)
					{
						l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;				
					}
					else if(*Dir == -1)
					{
						l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
						Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;	
					}
				}
			}
			
	}
		
		else if((l_u8Pos == 1)&&(l_u8MtchCnt == 1))//��Ӧ��������һ��խ��
		{
			if(ZTColum[l_u8X][0].FristElemRow == 0)
			{
				l_u32difUpTime = g_ChlData[lpZTInf->u16ID].u32uptm - g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32uptm;
				if(g_ChlData[lpZTInf->u16ID].u8BoardType == 1)
				{ //������ʽ�����ֺ�*����*խ�����/խ����ȣ��ϳ�1-�ϳ�2��
					WheelWght = g_ChlData[lpZTInf->u16ID].u32chlSum*1400/l_u32difUpTime*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/1000/30;
				}
				else if(g_ChlData[lpZTInf->u16ID].u8BoardType == 2)
					WheelWght = g_ChlData[lpZTInf->u16ID].u32chlSum*1400/l_u32difUpTime*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/1000/50;
				if(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8BoardType == 1)
					Wait4WheelWght = g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32chlSum*1400/l_u32difUpTime*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8Id]/1000/30;
				else if(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8BoardType == 1)
					Wait4WheelWght = g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32chlSum*1400/l_u32difUpTime*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8Id]/1000/50;
				
				if(((GetDiff(WheelWght,Wait4WheelWght)*100) <= (IsBig(WheelWght,Wait4WheelWght)*g_u32WheelWghtCoin)) && (GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm) >= HideMinTickDif))
				{
					ZTColum[l_u8X][l_u8MtchPos].Colum[1] = lpZTInf->u16ID;
					ZTColum[l_u8X][l_u8MtchPos].ZTCnt++;
					ZTColum[l_u8X][l_u8MtchPos].Dir = *Dir;
					ZTColum[l_u8X][l_u8MtchPos].MaxValue = IsBig(ZTColum[l_u8X][l_u8MtchPos].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
					ZTColum[l_u8X][l_u8MtchPos].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
					ZTColum[l_u8X][l_u8MtchPos].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));
					//
					if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime = POINTRATE*g_u16RunAndWalkTime+GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
					}
					else
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
					}
					
					
					
					l_u32CoinMatchAddr[0] = &l_u8X;
					l_u32CoinMatchAddr[1] = &l_u8MtchPos;
					l_u32CoinMatchAddr[2] = &ZTColum[0][0];
					l_u32CoinMatchAddr[3] = Dir;
					FCoinMatchAdd(l_u32CoinMatchAddr);
				}
				else//��б�еĿ��ܣ���Ҫ����������խ������ƥ��������ֻ��Ϊһ��խ��
				{
					l_u8MtchLCnt=l_u8MtchRCnt=0;
					if((l_u8X > 0) && (l_u8X < (2*PROMODPARAM.m_u8LaneNum-1)))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);			
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X == 0)
					{
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X < (2*PROMODPARAM.m_u8LaneNum-1))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
					}
					if((l_u8MtchLCnt==0) && (l_u8MtchRCnt == 1))
					{
						//if((ZTColum[l_u8X][0].FristElemRow == 0)&&(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm))))
						{
							ZTColum[l_u8X][l_u8MtchPos].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][l_u8MtchPos].ZTCnt++;
							ZTColum[l_u8X][l_u8MtchPos].Dir = *Dir;
							ZTColum[l_u8X][l_u8MtchPos].MaxValue = IsBig(ZTColum[l_u8X][l_u8MtchPos].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][l_u8MtchPos].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][l_u8MtchPos].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 0))
					{
						//if(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm)))
						{
							ZTColum[l_u8X][l_u8MtchPos].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][l_u8MtchPos].ZTCnt++;
							ZTColum[l_u8X][l_u8MtchPos].Dir = *Dir;
							ZTColum[l_u8X][l_u8MtchPos].MaxValue = IsBig(ZTColum[l_u8X][l_u8MtchPos].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][l_u8MtchPos].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][l_u8MtchPos].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][l_u8MtchPos].DueTime))
								{
									ZTColum[l_u8X][l_u8MtchPos].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8MtchPos].DueTime;
								}
							}
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 1))
					{
						if((IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm))) &&\
							(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm))))
						{
							ZTColum[l_u8X][l_u8MtchPos].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][l_u8MtchPos].ZTCnt++;
							ZTColum[l_u8X][l_u8MtchPos].Dir = *Dir;
							ZTColum[l_u8X][l_u8MtchPos].MaxValue = IsBig(ZTColum[l_u8X][l_u8MtchPos].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][l_u8MtchPos].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][l_u8MtchPos].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][l_u8MtchPos].DueTime))
								{
									ZTColum[l_u8X][l_u8MtchPos].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8MtchPos].DueTime;
								}
							}
							if(ZTColum[l_u8X+1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X+1][0].DueTime,ZTColum[l_u8X][l_u8MtchPos].DueTime))
								{
									ZTColum[l_u8X][l_u8MtchPos].DueTime = ZTColum[l_u8X+1][0].DueTime;
									if(ZTColum[l_u8X-1][0].ZTCnt == 2)
										ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X+1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8MtchPos].DueTime;
								}
							}
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
					else
					{
						if(*Dir == -1)
						{
							l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Forward_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
						else
						{
							l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Reverse_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
					}
				}		
			}
			else
			{
				ZTColum[l_u8X][1].Colum[0] = lpZTInf->u16ID;
				ZTColum[l_u8X][1].Dir = *Dir;
				ZTColum[l_u8X][1].FristElemRow = 1;
				ZTColum[l_u8X][1].ZTCnt++;
				ZTColum[l_u8X][1].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
			}
		}
		else if((l_u8Pos == 2)&&(l_u8MtchCnt == 2)) //��Ӧ������������խ��
		{
			WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/g_ChlData[lpZTInf->u16ID].u32realCnt)*(g_Setup.an32AxGain[lpZTInf->m_ucTouch]/100);
			Wait4WheelWght = (g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32chlSum/g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)*(g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8Id]/100);
			Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32chlSum/g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)*(g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X][1].Colum[0]].u8Id]/100);
			if((ZTColum[l_u8X][0].FristElemRow	== 0)	&& (ZTColum[l_u8X][1].FristElemRow	== 0))
			{
				if((GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif) && \
					(GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif))
				{
					ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
					ZTColum[l_u8X][1].ZTCnt++;
					ZTColum[l_u8X][1].Dir = *Dir;
					ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
					ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
					ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
					
					l_u8MtchPos = 1;
					if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
					}
					else
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
					}
					
					
					l_u32CoinMatchAddr[0] = &l_u8X;
					l_u32CoinMatchAddr[1] = &l_u8MtchPos;
					l_u32CoinMatchAddr[2] = &ZTColum[0][0];
					l_u32CoinMatchAddr[3] = Dir;
					FCoinMatchAdd(l_u32CoinMatchAddr);
				}
				else if((GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif) && \
					(GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif))
				{
					ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
					ZTColum[l_u8X][0].ZTCnt++;
					ZTColum[l_u8X][0].Dir = *Dir;
					ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
					ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
					ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
					
					l_u8MtchPos = 0;
					
					if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
					}
					else
					{
						ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
					}
					
					
					l_u32CoinMatchAddr[0] = &l_u8X;
					l_u32CoinMatchAddr[1] = &l_u8MtchPos;
					l_u32CoinMatchAddr[2] = &ZTColum[0][0];
					l_u32CoinMatchAddr[3] = Dir;
					FCoinMatchAdd(l_u32CoinMatchAddr);
				}
				//����ʱ���Ƚϴ�,����Ҫ�鿴һ�����ص���ֵ
				else
				{
		// 			if(((WheelWght*100 >= g_u32WheelWghtCoin*Wait4WheelWght) && (WheelWght*g_u32WheelWghtCoin <= 100*Wait4WheelWght)) &&\
		// 				((WheelWght*100 < g_u32WheelWghtCoin*Wait4WheelWght2) && (WheelWght*g_u32WheelWghtCoin > 100*Wait4WheelWght2)))
					if(((GetDiff(WheelWght,Wait4WheelWght)*100) <= (WheelWght*g_u32WheelWghtCoin)) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) > (WheelWght*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].Dir = *Dir;
						ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
						ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
						ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
						
						l_u8MtchPos = 0;
						if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
						{
							ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
						}
						else
						{
							ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						}
						
						
						l_u32CoinMatchAddr[0] = &l_u8X;
						l_u32CoinMatchAddr[1] = &l_u8MtchPos;
						l_u32CoinMatchAddr[2] = &ZTColum[0][0];
						l_u32CoinMatchAddr[3] = Dir;
						FCoinMatchAdd(l_u32CoinMatchAddr);
					}
		// 			else if(((WheelWght*100 < g_u32WheelWghtCoin*Wait4WheelWght) && (WheelWght*g_u32WheelWghtCoin > 100*Wait4WheelWght)) &&\
		// 				((WheelWght*100 >= g_u32WheelWghtCoin*Wait4WheelWght2) && (WheelWght*g_u32WheelWghtCoin <= 100*Wait4WheelWght2)))
					else if(((GetDiff(WheelWght,Wait4WheelWght)*100) > (WheelWght*g_u32WheelWghtCoin)) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) <= (WheelWght*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
						ZTColum[l_u8X][1].ZTCnt++;
						ZTColum[l_u8X][1].Dir = *Dir;
						ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
						ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
						ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
						
						l_u8MtchPos = 1;
						if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
						{
							ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
						}
						else
						{
							ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						}
						
						
						l_u32CoinMatchAddr[0] = &l_u8X;
						l_u32CoinMatchAddr[1] = &l_u8MtchPos;
						l_u32CoinMatchAddr[2] = &ZTColum[0][0];
						l_u32CoinMatchAddr[3] = Dir;
						FCoinMatchAdd(l_u32CoinMatchAddr);
					}
					else
					{
						if(IsBigFlag(GetDiff(WheelWght,Wait4WheelWght),GetDiff(WheelWght,Wait4WheelWght2)))
						{
							ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][1].ZTCnt++;
							ZTColum[l_u8X][1].Dir = *Dir;
							ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
							
							l_u8MtchPos = 1;
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
						else
						{
							ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].Dir = *Dir;
							ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
							
							l_u8MtchPos = 0;
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][l_u8MtchPos].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
				}
			}
			else if((ZTColum[l_u8X][0].FristElemRow	== 0)	&& (ZTColum[l_u8X][1].FristElemRow	== 1))
			{
				//WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/g_ChlData[lpZTInf->u16ID].u32realCnt)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/1000;
				//Wait4WheelWght = (g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32chlSum/g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X][0].Colum[0]].u8Id]/1000;
				if(((GetDiff(WheelWght,Wait4WheelWght)*100) <= (WheelWght*g_u32WheelWghtCoin)) && (GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm) >= HideMinTickDif))
				{
					ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
					ZTColum[l_u8X][0].ZTCnt++;
					ZTColum[l_u8X][0].Dir = *Dir;
					ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
					ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
					ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
					//
					if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
					{
						ZTColum[l_u8X][0].DueTime = POINTRATE*g_u16RunAndWalkTime+GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						ZTColum[l_u8X][0].RunAndWalk = 1;
					}
					else
					{
						ZTColum[l_u8X][0].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
					}
					
					
					l_u8MtchPos = 0;
					l_u32CoinMatchAddr[0] = &l_u8X;
					l_u32CoinMatchAddr[1] = &l_u8MtchPos;
					l_u32CoinMatchAddr[2] = &ZTColum[0][0];
					l_u32CoinMatchAddr[3] = Dir;
					FCoinMatchAdd(l_u32CoinMatchAddr);
				}
				else//��б�еĿ��ܣ���Ҫ����������խ������ƥ��������ֻ��Ϊһ��խ��
				{
					l_u8MtchLCnt=l_u8MtchRCnt=0;
					if((l_u8X > 0) && (l_u8X < (2*PROMODPARAM.m_u8LaneNum-1)))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);			
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X == 0)
					{
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X < (2*PROMODPARAM.m_u8LaneNum-1))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
					}
					if((l_u8MtchLCnt==0) && (l_u8MtchRCnt == 1))
					{
						if(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm)))
						{
							ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].Dir = *Dir;
							ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][0].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][0].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][0].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							l_u8MtchPos = 0;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 0))
					{
						if(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm)))
						{
							ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].Dir = *Dir;
							ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][0].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][0].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][0].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][0].DueTime))
								{
									ZTColum[l_u8X][0].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][0].DueTime;
								}
							}
							l_u8MtchPos = 0;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 1))
					{
						if((IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm))) &&\
							(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm))))
						{
							ZTColum[l_u8X][0].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].Dir = *Dir;
							ZTColum[l_u8X][0].MaxValue = IsBig(ZTColum[l_u8X][0].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][0].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][0].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][0].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][0].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][0].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][0].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][0].DueTime))
								{
									ZTColum[l_u8X][0].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][0].DueTime;
								}
							}
							if(ZTColum[l_u8X+1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X+1][0].DueTime,ZTColum[l_u8X][0].DueTime))
								{
									ZTColum[l_u8X][0].DueTime = ZTColum[l_u8X+1][0].DueTime;
									if(ZTColum[l_u8X-1][0].ZTCnt == 2)
										ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X+1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][0].DueTime;
								}
							}
							l_u8MtchPos  = 0;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
					else
					{
						if(*Dir == -1)
						{
							l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Forward_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
						else
						{
							l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Reverse_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
					}
				}		
			}
			else if((ZTColum[l_u8X][0].FristElemRow	== 1)	&& (ZTColum[l_u8X][1].FristElemRow	== 0))
			{
				if(((GetDiff(WheelWght,Wait4WheelWght2)*100) <= (WheelWght*g_u32WheelWghtCoin)) && (GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm) >= HideMinTickDif))
				{
					ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
					ZTColum[l_u8X][1].ZTCnt++;
					ZTColum[l_u8X][1].Dir = *Dir;
					ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
					ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
					ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
					//
					if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
					{
						ZTColum[l_u8X][1].DueTime = POINTRATE*g_u16RunAndWalkTime+GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
						ZTColum[l_u8X][1].RunAndWalk = 1;
					}
					else
					{
						ZTColum[l_u8X][1].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
					}
					
					
					l_u8MtchPos = 1;
					l_u32CoinMatchAddr[0] = &l_u8X;
					l_u32CoinMatchAddr[1] = &l_u8MtchPos;
					l_u32CoinMatchAddr[2] = &ZTColum[0][0];
					l_u32CoinMatchAddr[3] = Dir;
					FCoinMatchAdd(l_u32CoinMatchAddr);
				}
				else//��б�еĿ��ܣ���Ҫ����������խ������ƥ��������ֻ��Ϊһ��խ��
				{
					l_u8MtchLCnt=l_u8MtchRCnt=0;
					if((l_u8X > 0) && (l_u8X < (2*PROMODPARAM.m_u8LaneNum-1)))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);			
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X == 0)
					{
						l_u8MtchRCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
					}
					else if(l_u8X < (2*PROMODPARAM.m_u8LaneNum-1))
					{
						l_u8MtchLCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
					}
					if((l_u8MtchLCnt==0) && (l_u8MtchRCnt == 1))
					{
						if(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm)))
						{
							ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][1].ZTCnt++;
							ZTColum[l_u8X][1].Dir = *Dir;
							ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][1].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][1].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][1].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							l_u8MtchPos = 1;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 0))
					{
						if(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/2,GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm)))
						{
							ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][1].ZTCnt++;
							ZTColum[l_u8X][1].Dir = *Dir;
							ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][1].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][1].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][1].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][1].DueTime))
								{
									ZTColum[l_u8X][1].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][0].DueTime;
								}
							}
							l_u8MtchPos = 1;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					
					}
					else if((l_u8MtchLCnt==1) && (l_u8MtchRCnt == 1))
					{
						if((IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm))) &&\
							(IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm),GetDiff(g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm))))
						{
							ZTColum[l_u8X][1].Colum[1] = lpZTInf->u16ID;
							ZTColum[l_u8X][1].ZTCnt++;
							ZTColum[l_u8X][1].Dir = *Dir;
							ZTColum[l_u8X][1].MaxValue = IsBig(ZTColum[l_u8X][1].MaxValue,g_ChlData[lpZTInf->u16ID].u16ptMaxValue);
							ZTColum[l_u8X][1].Speed  = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*216/(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));//�ٶȷŴ�10��
							ZTColum[l_u8X][1].Length = PROMODPARAM.m_au16ZTGap[lpZTInf->m_ucTouch]*(g_ChlData[lpZTInf->u16ID].u32realCnt + g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32realCnt)/(2*GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm));
							//ZTColum[l_u8X][l_u8MtchPos].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8MtchPos].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							
							if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32uptm) > (2*POINTRATE)) && (g_u8RunAndWalkEnable == 1))
							{
								ZTColum[l_u8X][1].DueTime =POINTRATE*g_u16RunAndWalkTime + GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
								ZTColum[l_u8X][1].RunAndWalk = 1;
							}
							else
							{
								ZTColum[l_u8X][1].DueTime = GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X][1].Colum[0]].u32Maxtm)/4 + g_ChlData[lpZTInf->u16ID].u32Maxtm;
							}
							
							if(ZTColum[l_u8X-1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X-1][0].DueTime,ZTColum[l_u8X][1].DueTime))
								{
									ZTColum[l_u8X][1].DueTime = ZTColum[l_u8X-1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][1].DueTime;
								}
							}
							if(ZTColum[l_u8X+1][0].ZTCnt == 2)
							{
								if(IsBigFlag(ZTColum[l_u8X+1][0].DueTime,ZTColum[l_u8X][1].DueTime))
								{
									ZTColum[l_u8X][1].DueTime = ZTColum[l_u8X+1][0].DueTime;
									if(ZTColum[l_u8X-1][0].ZTCnt == 2)
										ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X+1][0].DueTime;
								}
								else
								{
									ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][1].DueTime;
								}
							}
							l_u8MtchPos  = 1;
							l_u32CoinMatchAddr[0] = &l_u8X;
							l_u32CoinMatchAddr[1] = &l_u8MtchPos;
							l_u32CoinMatchAddr[2] = &ZTColum[0][0];
							l_u32CoinMatchAddr[3] = Dir;
							FCoinMatchAdd(l_u32CoinMatchAddr);
						}
					}
					else
					{
						if(*Dir == -1)
						{
							l_u8Pos = FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Forward_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Forward_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
						else
						{
							l_u8Pos = FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8Pos].ZTCnt++;
							Reverse_HideZTColum[l_u8X][l_u8Pos].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
							Reverse_HideZTColum[l_u8X][l_u8Pos].Dir = -1;
						}
					}
				}		
			
			}
			else
			{
				
			}
		}

		else
		{
			if(l_u8Pos == 2)
			{
				if(ZTColum[l_u8X][0].ZTCnt == 1)
				{
					MoveHideColumElem(&ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE);
					ZTColum[l_u8X][1].Colum[0] = lpZTInf->u16ID;
					ZTColum[l_u8X][1].Dir = *Dir;
					ZTColum[l_u8X][1].FristElemRow = 1;
					ZTColum[l_u8X][1].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
				}
				else if(ZTColum[l_u8X][1].ZTCnt == 1)
				{
					ZTColum[l_u8X][1].Colum[0] = lpZTInf->u16ID;
					ZTColum[l_u8X][1].Dir = *Dir;
					ZTColum[l_u8X][1].FristElemRow = 1;
					ZTColum[l_u8X][1].MaxValue = g_ChlData[lpZTInf->u16ID].u16ptMaxValue;
				}
			}
		}
	}
}
/*********************************************************************************************************
** Function name:  FMtchLeftAndRghtColum
** Descriptions: ����ƥ��խ��
** input parameters:   
** output parameters:  
**
** Created by: zhangtonghan		  
** Created Date: 2017-11-03
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------*/
void  FMtchLeftAndRghtColum(void * Address[6])
{
	int8 *Dir,*Cmd;
	HideColum_Struct (*ZTColum)[HIDECOLUM_SAVEAXlE];
	ZTINFRECELEMENT *lpZTInf;
	uint8 *pl_u8X,l_u8X,l_u8MatchRowCnt,l_u8MatchRghtRowCnt,l_u8Flag,l_u8ThreeFlag = 0,l_u8Elem=0;
	uint32 LargeRange,LitRange,WheelWght,Wait4WheelWght,Wait4WheelWght2,LargeRange2,LitRange2;
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	uint8 l_u8tempi,l_u8tempj,l_u8CoinFlg,l_u8Pos;
	void*	l_u32CoinDegAddr[3];
	void* l_u32CrossAddr[5];
	
										
	Cmd 		= Address[0];
	Dir			= Address[1];
	ZTColum = Address[2];
	lpZTInf = Address[3];
	pl_u8X 	= Address[4];
	
	l_u8X = *pl_u8X;
	//Cmd: 1:���;2:�Ҳ�;3:����
	switch(*Cmd)
	{
		case 1:
			l_u8MatchRowCnt=FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
		  if((l_u8MatchRowCnt == 1) && (ZTColum[l_u8X-1][0].ZTCnt == 1)) 
			{
				
				ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
				ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
				//ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
				ZTColum[l_u8X][0].ZTCnt++;
				ZTColum[l_u8X][0].FristElemRow = 1;
				if(*Dir == 1)
				{
					ZTColum[l_u8X][0].LeftMtchFlag = 5;
					ZTColum[l_u8X-1][0].RightMtchFlag = 5;
				}
				else
				{
					ZTColum[l_u8X][0].LeftMtchFlag = 4;
					ZTColum[l_u8X-1][0].RightMtchFlag = 4;
				}
				ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
			}
			else if((l_u8MatchRowCnt == 1) && (ZTColum[l_u8X-1][0].ZTCnt == 2))//ѹ��
			{
				l_u8CoinFlg = 0;
				l_u32CoinDegAddr[0] = &(lpZTInf->u16ID);
				l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][0].Colum[1];
				l_u32CoinDegAddr[2] = &l_u8CoinFlg;
				TwoZTCoinDegFunc(l_u32CoinDegAddr);
				
				l_u8Elem = 1;
				l_u8ThreeFlag= 0;
				l_u32CrossAddr[0] = &ZTColum[l_u8X-1][0].Colum[0];
				l_u32CrossAddr[1] = &ZTColum[l_u8X-1][0].Colum[1];
				l_u32CrossAddr[2] = &(lpZTInf->u16ID);
				l_u32CrossAddr[3] = &l_u8Elem;
				l_u32CrossAddr[4] = &l_u8ThreeFlag;
				WhetherCrossThreeLane(l_u32CrossAddr);
				l_u8ThreeFlag=1;
// 				if((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt))))||\
// 					(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm) && (IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))
				if((l_u8CoinFlg == 1) && (l_u8ThreeFlag == 1))
				{
					ZTColum[l_u8X-1][0].RightMtchFlag = 3;
					ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
					ZTColum[l_u8X][0].Colum[0]=lpZTInf->u16ID;
					ZTColum[l_u8X][0].LeftMtchFlag = 3;
					ZTColum[l_u8X][0].ZTCnt++;
					ZTColum[l_u8X][0].FristElemRow = 1;
				}
				else
				{
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
											 lpZTInf->u16ID,lpZTInf->m_ucTouch);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
					if(*Dir == 1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
					else if(*Dir == -1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
				}
			}
			else if(l_u8MatchRowCnt == 2)
			{
				if((ZTColum[l_u8X-1][0].ZTCnt == 2) && (ZTColum[l_u8X-1][1].ZTCnt == 2))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange2   = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));				
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt;
					Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt;
					//�غ϶���������������
					if((((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght)*100) < (Wait4WheelWght*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 3;
						ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((((LitRange2*100) > (LargeRange2*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X-1][1].RightMtchFlag = 3;
						ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
				else if((ZTColum[l_u8X-1][0].ZTCnt == 1) && (ZTColum[l_u8X-1][1].ZTCnt == 1))
				{
					if((GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif) && \
					((GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif) &&(ZTColum[l_u8X-1][1].FristElemRow == 0)))
					{
						Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						WheelWght= (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
						if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 3;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 6;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
					}
					else if(((GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif)  && (ZTColum[l_u8X-1][0].FristElemRow == 0))&& \
					(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif))
					{
						Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						WheelWght= (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
						if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 3;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 6;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
					}
				}
				else if((ZTColum[l_u8X-1][0].ZTCnt == 2) && (ZTColum[l_u8X-1][1].ZTCnt == 1))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
				//	Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
					Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
					
					if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)))
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 3;
						ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;	
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((ZTColum[l_u8X-1][1].FristElemRow == 1)&&(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X-1][1].RightMtchFlag = 5;
						ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
				else if((ZTColum[l_u8X-1][0].ZTCnt == 1) && (ZTColum[l_u8X-1][1].ZTCnt == 2))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
					//Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
					
					if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)))
					{
						ZTColum[l_u8X-1][1].RightMtchFlag = 3;
						ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;	
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 5;
						ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
			}
			break;
		case 2:
			l_u8MatchRowCnt=FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
			if((l_u8MatchRowCnt == 1) && (ZTColum[l_u8X+1][0].ZTCnt == 1)) 
			{
				
				ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
				ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
				ZTColum[l_u8X][0].ZTCnt++;
				ZTColum[l_u8X][0].FristElemRow = 1;
				if(*Dir == 1)
				{
					ZTColum[l_u8X][0].RightMtchFlag = 4;
					ZTColum[l_u8X+1][0].LeftMtchFlag = 4;
				}
				else
				{
					ZTColum[l_u8X][0].RightMtchFlag = 5;
					ZTColum[l_u8X+1][0].LeftMtchFlag = 5;
				}
				//ZTColum[l_u8X][0].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
			}
			else if((l_u8MatchRowCnt == 1) && (ZTColum[l_u8X+1][0].ZTCnt == 2))//ѹ��
			{
// 				LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
// 				LitRange	 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
				l_u8CoinFlg = 0;
				l_u32CoinDegAddr[0] = &(lpZTInf->u16ID);
				l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][0].Colum[1];
				l_u32CoinDegAddr[2] = &l_u8CoinFlg;
				TwoZTCoinDegFunc(l_u32CoinDegAddr);
				
				l_u8Elem = 1;
				l_u8ThreeFlag= 0;
				l_u32CrossAddr[0] = &ZTColum[l_u8X+1][0].Colum[0];
				l_u32CrossAddr[1] = &ZTColum[l_u8X+1][0].Colum[1];
				l_u32CrossAddr[2] = &(lpZTInf->u16ID);
				l_u32CrossAddr[3] = &l_u8Elem;
				l_u32CrossAddr[4] = &l_u8ThreeFlag;
				WhetherCrossThreeLane(l_u32CrossAddr);
				l_u8ThreeFlag=1;
// 				if((IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt))))||\
// 					(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm) && (IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))
				if((l_u8CoinFlg==1) && (l_u8ThreeFlag == 1))
				{
					ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
					ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
					ZTColum[l_u8X][0].Colum[0]=lpZTInf->u16ID;
					ZTColum[l_u8X][0].RightMtchFlag = 3;	
					ZTColum[l_u8X][0].ZTCnt++;
					ZTColum[l_u8X][0].FristElemRow = 1;
				}
				else
				{
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
											 lpZTInf->u16ID,lpZTInf->m_ucTouch);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
					if(*Dir == 1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
					else if(*Dir == -1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
				}
			}
			else if(l_u8MatchRowCnt == 2)
			{
				if((ZTColum[l_u8X+1][0].ZTCnt == 2) && (ZTColum[l_u8X+1][1].ZTCnt == 2))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange2   = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));				
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt;
					Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt;
					//�غ϶���������������
					if((((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght)*100) < (Wait4WheelWght*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
						ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((((LitRange2*100) > (LargeRange2*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X+1][1].LeftMtchFlag = 3;
						ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
				else if((ZTColum[l_u8X+1][0].ZTCnt == 1) && (ZTColum[l_u8X+1][1].ZTCnt == 1))
				{
					if((GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif) && \
					((GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif) && (ZTColum[l_u8X+1][1].FristElemRow ==0)))
					{
						Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						WheelWght= (g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt;
						if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
						{
							ZTColum[l_u8X+1][1].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							ZTColum[l_u8X+1][1].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
					}
					else if(((GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif) && (ZTColum[l_u8X+1][0].FristElemRow ==0)) && \
					(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif))
					{
						Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						WheelWght= (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
						if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
					}
				}
				else if((ZTColum[l_u8X+1][0].ZTCnt == 2) && (ZTColum[l_u8X+1][1].ZTCnt == 1))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
				//	Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
					Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt;
					
					if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)))
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
						ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;	
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X+1][1].LeftMtchFlag = 5;
						ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
				else if((ZTColum[l_u8X+1][0].ZTCnt == 1) && (ZTColum[l_u8X+1][1].ZTCnt == 2))
				{
					LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
					WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
					//Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
					
					if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
						(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)))
					{
						ZTColum[l_u8X+1][1].RightMtchFlag = 3;
						ZTColum[l_u8X+1][1].WonderRMtchID = lpZTInf->u16ID;	
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
					{
						ZTColum[l_u8X+1][0].RightMtchFlag = 5;
						ZTColum[l_u8X+1][0].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						ZTColum[l_u8X][0].FristElemRow = 1;
					}
					else
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						if(*Dir == 1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
						else if(*Dir == -1)
						{
							l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
							Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
						}
					}
				}
			}
		break;
		case 3:
			l_u8MatchRowCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1); //�������
			l_u8MatchRghtRowCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1); //�Ҳ�����
			
			l_u8Flag = ((l_u8MatchRowCnt&0x0F) << 4) | (l_u8MatchRghtRowCnt & 0x0F) ;
			switch(l_u8Flag)
			{
				case 0x00:
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
											 lpZTInf->u16ID,lpZTInf->m_ucTouch);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
					if(*Dir == 1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
					else if(*Dir == -1)
					{
						l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
						Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
					}
				break;
				case 0x10:
					ZTColum[l_u8X][0].FristElemRow = 1;
					if(ZTColum[l_u8X-1][0].ZTCnt == 1) //ѹ��
					{
						
						ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].FristElemRow = 1;
						ZTColum[l_u8X][0].ZTCnt++;
						if(*Dir == 1)
						{
							ZTColum[l_u8X][0].LeftMtchFlag = 4;
							ZTColum[l_u8X-1][0].RightMtchFlag = 4;
						}
						else
						{
							ZTColum[l_u8X][0].LeftMtchFlag = 5;
							ZTColum[l_u8X-1][0].RightMtchFlag = 5;
						}
						ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
					}
					else if(ZTColum[l_u8X-1][0].ZTCnt == 2)
					{					
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &(lpZTInf->u16ID);
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][0].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
// 						if((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt))))||\
// 							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm) && (IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))
						if(l_u8CoinFlg)
						{
							ZTColum[l_u8X-1][0].LeftMtchFlag = 3;
							ZTColum[l_u8X-1][0].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0]=lpZTInf->u16ID;
							ZTColum[l_u8X][0].RightMtchFlag = 3;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							ZTColum[l_u8X][0].FristElemRow = 0;
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					
				break;
				case 0x20:
					ZTColum[l_u8X][0].FristElemRow = 1;
					if((ZTColum[l_u8X-1][0].ZTCnt == 2) && (ZTColum[l_u8X-1][1].ZTCnt == 2))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange2   = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));				
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt;
						Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt;
						//�غ϶���������������
						if((((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght)*100) < (Wait4WheelWght*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 3;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else if((((LitRange2*100) > (LargeRange2*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 3;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							ZTColum[l_u8X][0].FristElemRow = 0;
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					else if((ZTColum[l_u8X-1][0].ZTCnt == 1) && (ZTColum[l_u8X-1][1].ZTCnt == 1))
					{
						if((GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif) && \
						((GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif)&&(ZTColum[l_u8X-1][1].FristElemRow == 0)))
						{
							Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
							WheelWght= (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
							if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
							{
								ZTColum[l_u8X-1][1].RightMtchFlag = 3;
								ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
							}
							else
							{
								ZTColum[l_u8X-1][1].RightMtchFlag = 6;
								ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
							}
						}
						else if(((GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif)&& ZTColum[l_u8X-1][0].FristElemRow == 0) && \
						(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif))
						{
							Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
							WheelWght= (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
							if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
							{
								ZTColum[l_u8X-1][0].RightMtchFlag = 3;
								ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
							}
							else
							{
								ZTColum[l_u8X-1][0].RightMtchFlag = 6;
								ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
							}
						}
					}
					else if((ZTColum[l_u8X-1][0].ZTCnt == 2) && (ZTColum[l_u8X-1][1].ZTCnt == 1))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					//	Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
						Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
						
						if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm)))
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 3;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;	
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 5;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							ZTColum[l_u8X][0].FristElemRow = 0;
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					else if((ZTColum[l_u8X-1][0].ZTCnt == 1) && (ZTColum[l_u8X-1][1].ZTCnt == 2))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						Wait4WheelWght = (g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt;
						//Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
						
						if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm)))
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 3;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;	
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 5;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							ZTColum[l_u8X][0].FristElemRow = 0;
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
				
				break;
				case 0x01:
					ZTColum[l_u8X][0].FristElemRow = 1;
					if(ZTColum[l_u8X+1][0].ZTCnt == 1) //ѹ��
					{
						
						ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
						ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].FristElemRow = 1;
						//ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
						ZTColum[l_u8X][0].ZTCnt++;
						if(*Dir == 1)
						{
							ZTColum[l_u8X][0].RightMtchFlag = 5;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 5;
						}
						else
						{
							ZTColum[l_u8X][0].RightMtchFlag = 4;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 4;
						}
						ZTColum[l_u8X][0].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
					}
					else if(ZTColum[l_u8X+1][0].ZTCnt == 2)
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &(lpZTInf->u16ID);
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][0].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
// 						if((IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt))))||\
// 							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm) && (IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0]=lpZTInf->u16ID;
							ZTColum[l_u8X][0].RightMtchFlag = 3;
							ZTColum[l_u8X][0].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[1];
							ZTColum[l_u8X][0].ZTCnt++;							
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							ZTColum[l_u8X][0].FristElemRow = 0;
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					
				break;
				case 0x11:
					if(IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm))
					{
						if(*Dir == 1)
						{
							ZTColum[l_u8X][0].RightMtchFlag = 6;
							ZTColum[l_u8X][0].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
							ZTColum[l_u8X-1][0].RightMtchFlag = 6;
						}
						else
						{
							ZTColum[l_u8X][0].RightMtchFlag = 7;
							ZTColum[l_u8X][0].LeftMtchFlag = 7;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
							ZTColum[l_u8X-1][0].RightMtchFlag = 7;
						}					
					}
					else
					{
						if(*Dir == 1)
						{
							ZTColum[l_u8X][0].RightMtchFlag = 7;
							ZTColum[l_u8X][0].LeftMtchFlag = 7;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
							ZTColum[l_u8X-1][0].RightMtchFlag = 7;
						}
						else
						{
							ZTColum[l_u8X][0].RightMtchFlag = 6;
							ZTColum[l_u8X][0].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
							ZTColum[l_u8X-1][0].RightMtchFlag = 6;
						}	
					}
					ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
					ZTColum[l_u8X][0].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
					ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
					ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
					ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
					ZTColum[l_u8X][0].FristElemRow = 1;
					ZTColum[l_u8X][0].ZTCnt++;
				break;
				case 0x21:
					//ZTColum[l_u8X][0].FristElemRow = 1;
					if(ZTColum[l_u8X-1][0].ZTCnt == 2)
					{
						LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
				
						if((100*LitRange > g_ZTWaveOverLap*LargeRange) || (IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt)) ||\
							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))))
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 3;
							ZTColum[l_u8X-1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].LeftMtchFlag = 3;
							ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else if(ZTColum[l_u8X+1][0].FristElemRow == 0)
						{
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Disable Case!!! %d,%d,%02x.\r\n",
													 lpZTInf->m_ucTouch,ZTColum[l_u8X-1][0].Colum[1],l_u8Flag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
						}
					}
					else if(ZTColum[l_u8X-1][1].ZTCnt == 2)
					{
						LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
				
						if((100*LitRange > g_ZTWaveOverLap*LargeRange) || (IsBigFlag(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) ||\
							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))))
						{
							ZTColum[l_u8X-1][1].RightMtchFlag = 3;
							ZTColum[l_u8X-1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].LeftMtchFlag = 3;
							ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X-1][1].Colum[0];
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else if(ZTColum[l_u8X+1][0].FristElemRow == 0)
						{
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Disable Case!!! %d,%d,%02x.\r\n",
													 lpZTInf->m_ucTouch,ZTColum[l_u8X-1][1].Colum[1],l_u8Flag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
						}
					}
				break;
				case 0x02:
					if((ZTColum[l_u8X+1][0].ZTCnt == 2) && (ZTColum[l_u8X+1][1].ZTCnt == 2))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange2   = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));				
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt;
						Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt;
						//�غ϶���������������
						if((((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght)*100) < (Wait4WheelWght*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else if((((LitRange2*100) > (LargeRange2*g_ZTWaveOverLap)) || (((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)) ||((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) <= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm >= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)))) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X+1][1].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					else if((ZTColum[l_u8X+1][0].ZTCnt == 1) && (ZTColum[l_u8X+1][1].ZTCnt == 1))
					{
						if((GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif) && \
						((GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif) && (ZTColum[l_u8X+1][1].FristElemRow == 0)))
						{
							Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
							WheelWght= (g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt;
							if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
							{
								ZTColum[l_u8X+1][1].LeftMtchFlag = 3;
								ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
								ZTColum[l_u8X][0].FristElemRow = 1;
							}
							else
							{
								ZTColum[l_u8X+1][1].LeftMtchFlag = 6;
								ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
								ZTColum[l_u8X][0].FristElemRow = 1;
							}
						}
						else if(((GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) > HideMinTickDif)&&(ZTColum[l_u8X+1][0].FristElemRow == 0)) && \
						(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) < HideMinTickDif))
						{
							Wait4WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
							WheelWght= (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
							if((GetDiff(WheelWght,Wait4WheelWght)*100) < (WheelWght*g_u32WheelWghtCoin))
							{
								ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
								ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
								ZTColum[l_u8X][0].FristElemRow = 1;
							}
							else
							{
								ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
								ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
								ZTColum[l_u8X][0].FristElemRow = 1;
							}
						}
					}
					else if((ZTColum[l_u8X+1][0].ZTCnt == 2) && (ZTColum[l_u8X+1][1].ZTCnt == 1))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
					//	Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
						Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt;
						
						if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm)))
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
							ZTColum[l_u8X+1][0].WonderLMtchID = lpZTInf->u16ID;	
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm) > HideMinTickDif)&&(ZTColum[l_u8X+1][1].FristElemRow == 0) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X+1][1].LeftMtchFlag = 5;
							ZTColum[l_u8X+1][1].WonderLMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
					else if((ZTColum[l_u8X+1][0].ZTCnt == 1) && (ZTColum[l_u8X+1][1].ZTCnt == 2))
					{
						LargeRange  = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	  = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						WheelWght = (g_ChlData[lpZTInf->u16ID].u32chlSum/100)*g_Setup.an32AxGain[lpZTInf->m_ucTouch]/g_ChlData[lpZTInf->u16ID].u32realCnt;
						Wait4WheelWght = (g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt;
						//Wait4WheelWght2 = (g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32chlSum/100)*g_Setup.an32AxGain[g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u8Id]/g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt;
						
						if(((LitRange*100) > (LargeRange*g_ZTWaveOverLap)) || ((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt) >= GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt)) &&\
							(g_ChlData[lpZTInf->u16ID].u32Maxtm <= g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm)))
						{
							ZTColum[l_u8X+1][1].RightMtchFlag = 3;
							ZTColum[l_u8X+1][1].WonderRMtchID = lpZTInf->u16ID;	
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else if((GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm) > HideMinTickDif) && ((GetDiff(WheelWght,Wait4WheelWght2)*100) < (Wait4WheelWght2*g_u32WheelWghtCoin)))
						{
							ZTColum[l_u8X+1][0].RightMtchFlag = 5;
							ZTColum[l_u8X+1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							ZTColum[l_u8X][0].FristElemRow = 1;
						}
						else
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch failed,Go oppose colum %d,%d.\r\n",
													 lpZTInf->u16ID,lpZTInf->m_ucTouch);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							if(*Dir == 1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Reverse_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Reverse_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
							else if(*Dir == -1)
							{
								l_u8MatchRowCnt=FindHideColumEnd(&Forward_HideZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].Colum[0] = lpZTInf->u16ID;
								Forward_HideZTColum[l_u8X][l_u8MatchRowCnt].ZTCnt++;
							}
						}
					}
				
				break;
				case 0x12:
					ZTColum[l_u8X][0].FristElemRow = 1;
					if(ZTColum[l_u8X+1][0].ZTCnt == 2)
					{
						LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
				
						if((100*LitRange > g_ZTWaveOverLap*LargeRange) || (IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt)) ||\
							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))))
						{
							ZTColum[l_u8X+1][0].RightMtchFlag = 3;
							ZTColum[l_u8X+1][0].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].LeftMtchFlag = 3;
							ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X+1][0].Colum[0];
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							//ZTColum[l_u8X][0].FristElemRow = 0;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Disable Case!!! %d,%d,%02x.\r\n",
													 lpZTInf->m_ucTouch,ZTColum[l_u8X+1][0].Colum[1],l_u8Flag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
						}
					}
					else if(ZTColum[l_u8X+1][1].ZTCnt == 2)
					{
						LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
						LitRange	 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)));
				
						if((100*LitRange > g_ZTWaveOverLap*LargeRange) || (IsBigFlag(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[1]].u32realCnt)) ||\
							(IsBigFlag(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm) && (IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[1]].u32realCnt),GetDiff(g_ChlData[lpZTInf->u16ID].u32Maxtm,g_ChlData[lpZTInf->u16ID].u32realCnt)))))))
						{
							ZTColum[l_u8X+1][1].RightMtchFlag = 3;
							ZTColum[l_u8X+1][1].WonderRMtchID = lpZTInf->u16ID;
							ZTColum[l_u8X][0].FristElemRow = 1;
							ZTColum[l_u8X][0].LeftMtchFlag = 3;
							ZTColum[l_u8X][0].WonderLMtchID = ZTColum[l_u8X+1][1].Colum[0];
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
						}
						else
						{
							//ZTColum[l_u8X][0].FristElemRow = 0;
							ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
							ZTColum[l_u8X][0].ZTCnt++;
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Disable Case!!! %d,%d,%02x.\r\n",
													 lpZTInf->m_ucTouch,ZTColum[l_u8X+1][1].Colum[1],l_u8Flag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
						}
					}
				break;
				case 0x22:
					ZTColum[l_u8X][0].FristElemRow = 1;
					for(l_u8tempi=0;l_u8tempi < HIDECOLUM_SAVEAXlE;l_u8tempi++)
					{
						for(l_u8tempj=0;l_u8tempj < HIDECOLUM_SAVEAXlE;l_u8tempj++)
						{
							l_u8CoinFlg = 0;
							l_u32CoinDegAddr[0] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
							l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][l_u8tempj].Colum[0];
							l_u32CoinDegAddr[2] = &l_u8CoinFlg;
							TwoZTCoinDegFunc(l_u32CoinDegAddr);
							if(l_u8CoinFlg == 1)
							{
								ZTColum[l_u8X-1][l_u8tempi].RightMtchFlag = 3;
								ZTColum[l_u8X-1][l_u8tempi].WonderRMtchID = lpZTInf->u16ID;
								ZTColum[l_u8X+1][l_u8tempj].LeftMtchFlag = 3;
								ZTColum[l_u8X+1][l_u8tempj].WonderRMtchID = lpZTInf->u16ID;
								l_u8Pos = FindHideColumEnd(&ZTColum[l_u8X][0],HIDECOLUM_SAVEAXlE,0);
								ZTColum[l_u8X][l_u8Pos].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 3;
								ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 3;
								//ZTColum[l_u8X][0].Colum[0] = lpZTInf->u16ID;
								ZTColum[l_u8X][0].ZTCnt++;
								break;
							}
						}
						if(l_u8CoinFlg == 1)
						{
							break;
						}
					}
					if(l_u8CoinFlg == 1)
					{
						l_u8CoinFlg = 0;
					}
					else
					{
						ZTColum[l_u8X][0].FristElemRow = 0;
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"ZT Mtch No Such Situation!!!%d,%d.\r\n",
												 lpZTInf->u16ID,lpZTInf->m_ucTouch);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
					}
				break;
			}
		break;
	}
}

/*********************************************************************************************************
** Function name:     TwoZTCoinDegFunc
** Descriptions:      ����խ���غ϶�ƥ��̶ȵĺ���
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 20171025
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void 	TwoZTCoinDegFunc(void * Address[6])
{
	uint16 *pFristID,*pWait4ID,FristID,Wait4ID;
	uint8 *MtchFlg;
	uint32 LargeRange,LitRange;
	
	pFristID		= Address[0];
	pWait4ID		= Address[1];
	MtchFlg			= Address[2];
	
	FristID			= *pFristID;
	Wait4ID			= *pWait4ID;
	
	
	LargeRange = GetDiff(IsBig(g_ChlData[FristID].u32Maxtm,g_ChlData[Wait4ID].u32Maxtm),IsLit(GetDiff(g_ChlData[FristID].u32Maxtm,g_ChlData[FristID].u32realCnt),GetDiff(g_ChlData[Wait4ID].u32Maxtm,g_ChlData[Wait4ID].u32realCnt)));
	LitRange	 = GetDiff(IsLit(g_ChlData[FristID].u32Maxtm,g_ChlData[Wait4ID].u32Maxtm),IsBig(GetDiff(g_ChlData[FristID].u32Maxtm,g_ChlData[FristID].u32realCnt),GetDiff(g_ChlData[Wait4ID].u32Maxtm,g_ChlData[Wait4ID].u32realCnt)));
	
	
	if((((LitRange*100) > (LargeRange*g_ZTWaveOverLap))&&(IsInMid(g_ChlData[FristID].u32Maxtm,GetDiff(g_ChlData[Wait4ID].u32Maxtm,g_ChlData[Wait4ID].u32realCnt),g_ChlData[Wait4ID].u32Maxtm) ||\
		IsInMid(g_ChlData[Wait4ID].u32Maxtm,GetDiff(g_ChlData[FristID].u32Maxtm,g_ChlData[FristID].u32realCnt),g_ChlData[FristID].u32Maxtm))) \
		|| ((( g_ChlData[FristID].u32Maxtm >= g_ChlData[Wait4ID].u32Maxtm) && ( GetDiff(g_ChlData[FristID].u32Maxtm,g_ChlData[FristID].u32realCnt) <= g_ChlData[Wait4ID].u32Maxtm)) ||\
		((g_ChlData[FristID].u32Maxtm <= g_ChlData[Wait4ID].u32Maxtm) && (g_ChlData[FristID].u32Maxtm >= GetDiff(g_ChlData[Wait4ID].u32Maxtm,g_ChlData[Wait4ID].u32realCnt)))))
	{
		*MtchFlg = 1;
	}
	else
	{
		*MtchFlg = 0;
	}
}

/*********************************************************************************************************
** Function name:  WhetherCrossThreeLane
** Descriptions: ѹ�����ж�
** input parameters:   
** output parameters:  
**
** Created by: zhangtonghan		  
** Created Date: 2018-01-16
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------*/
void  WhetherCrossThreeLane(void* Address[5])
{
	uint16 *pFristRowZT,*pSecRowZT,*pWonderRowZT;
	uint8 *pElem,*pFlag;
	uint32 FirWht,SecWht,WonWht;
	pFristRowZT 	= Address[0];
	pSecRowZT			= Address[1];
	pWonderRowZT	= Address[2];
	pElem					= Address[3];
	pFlag					= Address[4];
	
	FirWht 	= g_ChlData[*pFristRowZT].u32chlSum/g_ChlData[*pFristRowZT].u32realCnt*g_Setup.an32AxGain[g_ChlData[*pFristRowZT].u8Id]/1000;
	SecWht	=	g_ChlData[*pSecRowZT].u32chlSum/g_ChlData[*pSecRowZT].u32realCnt*g_Setup.an32AxGain[g_ChlData[*pFristRowZT].u8Id]/1000;
	WonWht	= g_ChlData[*pWonderRowZT].u32chlSum/g_ChlData[*pWonderRowZT].u32realCnt*g_Setup.an32AxGain[g_ChlData[*pWonderRowZT].u8Id]/1000;
	
	if(*pElem == 0)
	{
		if(GetDiff(SecWht,(FirWht+WonWht))*100 < g_u32WheelWghtCoin*SecWht)
		{
			*pFlag = 1; 
		}
		else
		{
			*pFlag = 0; 
		}
	}
	else
	{
		if(GetDiff(FirWht,(SecWht+WonWht))*100 < g_u32WheelWghtCoin*FirWht)
		{
			*pFlag = 1; 
		}
		else
		{
			*pFlag = 0; 
		}
	}
}

/*********************************************************************************************************
** Function name:     FMatchHideColumAndCloum
** Descriptions:      ����խ��������ƥ�����
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 20171023
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void 	FMatchHideColumAndCloum(void * Address[6])
{
	uint32 *pl_u32Time,l_u32Time=0;
	HideColum_Struct (*ForwZTColum)[HIDECOLUM_SAVEAXlE];
	HideColum_Struct (*RevrZTColum)[HIDECOLUM_SAVEAXlE];
// 	VehicleInfForLoop_Struct (*VehForwChannelStruct)[CHL_SAVEVEH]; //����������
// 	VehicleInfForLoop_Struct (*VehReveChannelStruct)[CHL_SAVEVEH];  //����������
	uint8 l_u8tempi,l_u8tempj,l_u8tempk;
	int8 Dir;
	//l_columpos;
	void* HideZTColumMtchAddr[6];
	void* AxleAddress[4];
	void* l_u32InitAddr[3];
	pl_u32Time  					= Address[0];
	ForwZTColum 					= Address[1];
	RevrZTColum 					= Address[2];
// 	VehForwChannelStruct	= Address[3];
// 	VehReveChannelStruct	= Address[4];
	
	l_u32Time	= *pl_u32Time;
	
	for(l_u8tempi=0;l_u8tempi<(2*PROMODPARAM.m_u8LaneNum);l_u8tempi++)
	{
		//l_columpos=-1;
		for(l_u8tempj=0;l_u8tempj<HIDECOLUM_SAVEAXlE;l_u8tempj++)
		{
			
			Dir=1;
			if((ForwZTColum[l_u8tempi][l_u8tempj].ZTCnt == 2) && (((g_au32HideLineLastTime[l_u8tempi] < g_au32ZHLineLastTime[l_u8tempi]) && (ForwZTColum[l_u8tempi][l_u8tempj].RunAndWalk !=0))||\
				(l_u32Time >= ForwZTColum[l_u8tempi][l_u8tempj].DueTime) || ((l_u8tempj == 0) && (ForwZTColum[l_u8tempi][1].Colum[0] >= 0))))
			{
				if((ForwZTColum[l_u8tempi][l_u8tempj].AddLogicTimeFlg == 0) &&(ForwZTColum[l_u8tempi][l_u8tempj].RunAndWalk == 0))
				{
					ForwZTColum[l_u8tempi][l_u8tempj].DueTime += LogicDlyTime;
					ForwZTColum[l_u8tempi][l_u8tempj].AddLogicTimeFlg = 1;
					break;
				}
//				g_AxleInfo[g_u16AxleInfoWriteIndex].u8id = g_u16AxleInfoWriteIndex;
				
				for(l_u8tempk=0;l_u8tempk < ForwZTColum[l_u8tempi][l_u8tempj].ZTCnt;l_u8tempk++)
					g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChList[g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum+l_u8tempk] = ForwZTColum[l_u8tempi][l_u8tempj].Colum[l_u8tempk];
				g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum += ForwZTColum[l_u8tempi][l_u8tempj].ZTCnt;
				g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum = g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum;
				g_AxleInfo[g_u16AxleInfoWriteIndex].RunAndWalkFlag |= ForwZTColum[l_u8tempi][l_u8tempj].RunAndWalk;
				g_AxleInfo[g_u16AxleInfoWriteIndex].AxleChlPos[0] = l_u8tempi;
				g_AxleInfo[g_u16AxleInfoWriteIndex].u8ValidChlCnt++;
				g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType = HIDELAYOUT;
				g_AxleInfo[g_u16AxleInfoWriteIndex].AxleWhelCnt = 2;
				
				HideZTColumMtchAddr[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				HideZTColumMtchAddr[1] = &Dir;
				HideZTColumMtchAddr[2] = &l_u8tempi;//��
				HideZTColumMtchAddr[3] = &l_u8tempj;//�ڼ���
				HideZTColumMtchAddr[4] = Address[1];
				FHideZTColumMtchFunc(HideZTColumMtchAddr);
				
				
				
	
				
				l_u32InitAddr[0] = &ForwZTColum[0][0];
				l_u32InitAddr[1] = &l_u8tempi;
				l_u32InitAddr[2] = &l_u8tempj;
				ColumHideInitSolo(l_u32InitAddr);
				
				AxleAddress[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				AxleAddress[1] = &l_u8tempi;//��
				AxleAddress[2] = &Dir;
				AxleAddress[3] = Address[3];
				FMatchAxleAndAxle(AxleAddress);
			}
			else
			{
				break;
			}
		}
		
	}
	for(l_u8tempi=0;l_u8tempi<(2*PROMODPARAM.m_u8LaneNum);l_u8tempi++)
	{	
		for(l_u8tempj=0;l_u8tempj<HIDECOLUM_SAVEAXlE;l_u8tempj++)
		{
			Dir=-1;
			if((RevrZTColum[l_u8tempi][l_u8tempj].ZTCnt == 2) && ((l_u32Time >= RevrZTColum[l_u8tempi][l_u8tempj].DueTime) || ((l_u8tempj == 0) && (RevrZTColum[l_u8tempi][1].Colum[0] >= 0))))
			{
				if((RevrZTColum[l_u8tempi][l_u8tempj].AddLogicTimeFlg == 0) && (RevrZTColum[l_u8tempi][l_u8tempj].RunAndWalk == 0))
				{
					RevrZTColum[l_u8tempi][l_u8tempj].DueTime += LogicDlyTime;
					RevrZTColum[l_u8tempi][l_u8tempj].AddLogicTimeFlg = 1;
					break;
				}
				
//				g_AxleInfo[g_u16AxleInfoWriteIndex].u8id = g_u16AxleInfoWriteIndex;
				
				for(l_u8tempk=0;l_u8tempk < RevrZTColum[l_u8tempi][l_u8tempj].ZTCnt;l_u8tempk++)
					g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChList[g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum+l_u8tempk] = RevrZTColum[l_u8tempi][l_u8tempj].Colum[l_u8tempk];
				g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum += RevrZTColum[l_u8tempi][l_u8tempj].ZTCnt;
				g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum = g_AxleInfo[g_u16AxleInfoWriteIndex].u16ChListSum;
				g_AxleInfo[g_u16AxleInfoWriteIndex].RunAndWalkFlag |= RevrZTColum[l_u8tempi][l_u8tempj].RunAndWalk;
				g_AxleInfo[g_u16AxleInfoWriteIndex].AxleChlPos[0] = l_u8tempi;
				g_AxleInfo[g_u16AxleInfoWriteIndex].u8ValidChlCnt++;
				g_AxleInfo[g_u16AxleInfoWriteIndex].ZTLayouType = HIDELAYOUT;
				g_AxleInfo[g_u16AxleInfoWriteIndex].AxleWhelCnt = 2;
				
				HideZTColumMtchAddr[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				HideZTColumMtchAddr[1] = &Dir;
				HideZTColumMtchAddr[2] = &l_u8tempi;//��
				HideZTColumMtchAddr[3] = &l_u8tempj;//�ڼ���
				HideZTColumMtchAddr[4] = Address[2];
				FHideZTColumMtchFunc(HideZTColumMtchAddr);
				
				for(l_u8tempk=0;l_u8tempk<g_AxleInfo[g_u16AxleInfoWriteIndex].u8ValidChlCnt;l_u8tempk++)
				{
					g_u8XieLaneFlag[g_AxleInfo[g_u16AxleInfoWriteIndex].AxleChlPos[l_u8tempk]] = 0;
				}
				l_u32InitAddr[0] = &RevrZTColum[0][0];
				l_u32InitAddr[1] = &l_u8tempi;
				l_u32InitAddr[2] = &l_u8tempj;
				ColumHideInitSolo(l_u32InitAddr);
				
				AxleAddress[0] = &g_AxleInfo[g_u16AxleInfoWriteIndex];
				AxleAddress[1] = &l_u8tempi;//��
				AxleAddress[2] = &Dir;
				AxleAddress[3] = Address[4];
				FMatchAxleAndAxle(AxleAddress);
			}
			else
			{
				break;
			}
	}
		
	}
}

/*********************************************************************************************************
** Function name:     FHideZTColumMtchFunc
** Descriptions:      ����խ��ƥ��
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 20171024
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FHideZTColumMtchFunc(void * Address[6])
{
	Axle_Struct *MtchAxlePt;
//	int8 *Dir;
	uint8 *FristColumL;
	uint8 *MtchColumPos;
	uint8 noMatch2Col=0; //��ƥ��-2�л��2��
	HideColum_Struct (*ZTColum)[HIDECOLUM_SAVEAXlE];
//	uint16 l_u16Speed,l_u16Length;
	uint8 l_u8tempi,l_u8tempj,l_u8tempk,l_u8temp,l_u8MtchFlag=0,l_u8X,l_u8CoinFlg=0,l_u8ThreeFlag=0;
//	void* l_u32SpdAndLthMthAdd[6];
	void* l_u32CoinDegAddr[6];
	void* l_u32InitAddr[3];
	void* l_u32CrossAddr[5];
	uint8 l_u8InitLine,l_u8ChlNum;
	uint8 MtchDirFlg=0; //1����2���ң�3��ƥ���,����Ѱ��;4:ƥ�䲻��,����Ѱ��
//	uint32 WhelWght=0,WhelWght1=0;
	
	MtchAxlePt		= Address[0];
//	Dir						=	Address[1];
	FristColumL		= Address[2];
	MtchColumPos 	= Address[3];
	ZTColum				= Address[4];
	l_u8X					= *FristColumL;
	
// 	l_u16Speed = ZTColum[l_u8X][*MtchColumPos].Speed;
// 	l_u16Length = ZTColum[l_u8X][*MtchColumPos].Length;
	
	if(((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 3) || ((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 8)) ) && (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag != 3))
	{
		MtchDirFlg = 1;
	}
	else if((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 3) && (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 3))
	{
		MtchDirFlg = 3;
	}
	else if((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag != 3) && ((ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 3) || (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 8)))
	{
		MtchDirFlg = 2;
	}
	else if(((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag != 3) && (ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag > 1)) || ((ZTColum[l_u8X][*MtchColumPos].RightMtchFlag != 3)&& (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag > 1)))
	{
		MtchDirFlg = 4;
	}
	else if((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag <= 1) && (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag <= 1))
	{
		l_u8InitLine=*MtchColumPos;
		l_u8ChlNum = l_u8X;
		l_u32InitAddr[0] = &ZTColum[0][0];
		l_u32InitAddr[1] = &l_u8ChlNum;
		l_u32InitAddr[2] = &l_u8InitLine;
		ColumHideInitSolo(l_u32InitAddr);
	}
	
	l_u8MtchFlag=0;
	switch(MtchDirFlg)
	{
		case 1:
			for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
			{
				if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-1][l_u8tempi].Colum[0])
				{
					if(ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 2) //�ڶ���խ����Ҫ����ѡ���غ϶�ƥ��
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][l_u8tempi].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							//�ж������ǲ���һ���֣����������ֻ�еڶ���խ����Ĭ������Ϊ0
							if((ZTColum[*FristColumL][*MtchColumPos].FristElemRow ==1) || (ZTColum[l_u8X-1][l_u8tempi].FristElemRow ==1))
							{
								//MtchAxlePt->AxleWhelCnt += ZTColum[l_u8X-1][l_u8tempi].WhelCnt;
							}
							else
							{
								if((ZTColum[*FristColumL][*MtchColumPos].WhelCnt == 1)&& (ZTColum[l_u8X-1][l_u8tempi].WhelCnt == 1))
								{
									if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[*FristColumL][*MtchColumPos].FirWhelPos) <4)
									{
										//��������
										MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].FirWhelPos;
									}
									else
									{
										//MtchAxlePt->AxleWhelCnt++;
										MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
										MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].FirWhelPos;
									}
								}
								else if((ZTColum[*FristColumL][*MtchColumPos].WhelCnt == 2)&& (ZTColum[l_u8X-1][l_u8tempi].WhelCnt == 1))
								{
									if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[*FristColumL][*MtchColumPos].FirWhelPos) <4)
									{
										//MtchAxlePt->AxleWhelCnt++;
										MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
										MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].SecWhelPos;
										noMatch2Col = 1;
									}
									else
									{
										MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
										MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].SecWhelPos;
										break;
									}										
								}
								else if((ZTColum[*FristColumL][*MtchColumPos].WhelCnt == 1)&& (ZTColum[l_u8X-1][l_u8tempi].WhelCnt == 2))
								{
									if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].SecWhelPos,ZTColum[*FristColumL][*MtchColumPos].FirWhelPos) <4)
									{
										//MtchAxlePt->AxleWhelCnt++;
										MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][*MtchColumPos].FirWhelPos;
										MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].FirWhelPos;
									}
									else
									{
										MtchAxlePt->HideLeftWhePos = ZTColum[*FristColumL][*MtchColumPos].FirWhelPos;
										break;
									}
								}
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
							
							
							if(noMatch2Col !=1)
							{
								if(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 3)
								{
									for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
									{
										if(ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempj].Colum[0])
										{
											if(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 1)
											{
												//if
												l_u8ThreeFlag = 0;
												l_u32CrossAddr[0] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
												l_u32CrossAddr[1] = &ZTColum[l_u8X-1][l_u8tempi].Colum[1];
												l_u32CrossAddr[2] = &ZTColum[l_u8X-2][l_u8tempj].Colum[0];
												l_u32CrossAddr[3] = &ZTColum[l_u8X-2][l_u8tempj].FristElemRow;
												l_u32CrossAddr[4] = &l_u8ThreeFlag;
												WhetherCrossThreeLane(l_u32CrossAddr);
												if(l_u8ThreeFlag)
												{
													if((ZTColum[l_u8X-2][l_u8tempj].FristElemRow == 1) || (ZTColum[l_u8X-1][l_u8tempi].FristElemRow))
													{
															//ֻ�еڶ���խ��������ƥ�������
													}
													else 
													{
														if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 2) || (IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >= 4))
															break;  //-2�в����������ֻ���-2���ֺ�-1����ߵ���Ϊͬһ����
														else if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 1)&&(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) < 4))
														{
															MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-2][l_u8tempi].FirWhelPos;
														}
													}
													
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum] = ZTColum[l_u8X-2][l_u8tempj].Colum[0];
													MtchAxlePt->u16ChListSum++;
													MtchAxlePt->u16ChListSum=MtchAxlePt->u16ChListSum;
													MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
													MtchAxlePt->u8ValidChlCnt++;
													MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
													MtchAxlePt->AxleWhelCnt += ZTColum[l_u8X-2][l_u8tempi].WhelCnt;
													//��ʼ��
													l_u8InitLine=l_u8tempj;
													l_u8ChlNum = l_u8X-2;
													l_u32InitAddr[0] = &ZTColum[0][0];
													l_u32InitAddr[1] = &l_u8ChlNum;
													l_u32InitAddr[2] = &l_u8InitLine;
													ColumHideInitSolo(l_u32InitAddr);
												}
												break;
											}
											else if(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 2)
											{
												l_u8CoinFlg = 0;
												l_u32CoinDegAddr[0] = &ZTColum[l_u8X-1][l_u8tempi].Colum[1];
												l_u32CoinDegAddr[1] = &ZTColum[l_u8X-2][l_u8tempj].Colum[1];
												l_u32CoinDegAddr[2] = &l_u8CoinFlg;
												TwoZTCoinDegFunc(l_u32CoinDegAddr);
												if(l_u8CoinFlg)
												{
													if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 2) || (IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >= 4))
															break;  //-2�в����������ֻ���-2���ֺ�-1����ߵ���Ϊͬһ����
													if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 1)&&(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) < 4))
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-2][l_u8tempi].FirWhelPos;
													}
													for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
														MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
													MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
													MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
													MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
													MtchAxlePt->u8ValidChlCnt++;
													MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
													//��ʼ��																								
													l_u8InitLine=l_u8tempj;
													l_u8ChlNum = l_u8X-2;
													l_u32InitAddr[0] = &ZTColum[0][0];
													l_u32InitAddr[1] = &l_u8ChlNum;
													l_u32InitAddr[2] = &l_u8InitLine;
													ColumHideInitSolo(l_u32InitAddr);
													break;
												}	
											}							
										}
									}								
								}		
							}	
							noMatch2Col = 0;	
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X-1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
						}
						else//����ƥ�䶼���ã�����Ҫ�������ƥ����
						{						
							if(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 3)
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 2)
									{
										l_u8CoinFlg = 0;
										l_u32CoinDegAddr[0] = &ZTColum[l_u8X-1][l_u8tempi].Colum[1];
										l_u32CoinDegAddr[1] = &ZTColum[l_u8X-2][l_u8tempj].Colum[1];
										l_u32CoinDegAddr[2] = &l_u8CoinFlg;
										TwoZTCoinDegFunc(l_u32CoinDegAddr);
										if(l_u8CoinFlg)
										{
											l_u8CoinFlg = 0;
											l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
											l_u32CoinDegAddr[1] = &ZTColum[l_u8X-2][l_u8tempj].Colum[1];
											l_u32CoinDegAddr[2] = &l_u8CoinFlg;
											TwoZTCoinDegFunc(l_u32CoinDegAddr);
											if(l_u8CoinFlg)
											{
												if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 1)&&(ZTColum[l_u8X][l_u8tempi].WhelCnt == 1)&& \
													(IsBigTwo(ZTColum[l_u8X-2][l_u8tempi].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) < 4))
												{
													MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-2][l_u8tempi].FirWhelPos;
													MtchAxlePt->HideRightWhePos = ZTColum[*FristColumL][*MtchColumPos].FirWhelPos;
												}
												else if((ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 2)||(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2))
													break;
												for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
												MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
												MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
												for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
												MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
												MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
												//��ʼ��
// 												l_u8InitLine=*MtchColumPos;
// 												l_u8ChlNum = l_u8X;
// 												l_u32InitAddr[0] = &ZTColum[0][0];
// 												l_u32InitAddr[1] = &l_u8ChlNum;
// 												l_u32InitAddr[2] = &l_u8InitLine;
// 												ColumHideInitSolo(l_u32InitAddr);
												
												l_u8InitLine=l_u8tempi;
												l_u8ChlNum = l_u8X-1;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
												
												l_u8InitLine=l_u8tempj;
												l_u8ChlNum = l_u8X-2;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
												break;
											}
											else
											{
												//�غ϶Ȳ���,���Ҳ��ң��Ƿ�б��
											}
										}
										else
										{
											//û��ƥ����Ϊʲô��Ҫ�ӽ����أ�û�������ε���
// 											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
// 													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
// 											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
// 											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
// 											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
// 											MtchAxlePt->u8ValidChlCnt++;
// 											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
// // 											l_u8InitLine=*MtchColumPos;
// // 											l_u8ChlNum = l_u8X;
// // 											l_u32InitAddr[0] = &ZTColum[0][0];
// // 											l_u32InitAddr[1] = &l_u8ChlNum;
// // 											l_u32InitAddr[2] = &l_u8InitLine;
// // 											ColumHideInitSolo(l_u32InitAddr);
// 											
// 											l_u8InitLine=l_u8tempi;
// 											l_u8ChlNum = l_u8X-1;
// 											l_u32InitAddr[0] = &ZTColum[0][0];
// 											l_u32InitAddr[1] = &l_u8ChlNum;
// 											l_u32InitAddr[2] = &l_u8InitLine;
// 											ColumHideInitSolo(l_u32InitAddr);
											
//											l_u8InitLine=l_u8tempi;
//											l_u8ChlNum = l_u8X-1;
//											l_u32InitAddr[0] = &ZTColum[0][0];
//											l_u32InitAddr[1] = &l_u8ChlNum;
//											l_u32InitAddr[2] = &l_u8InitLine;
//											ColumHideInitSolo(l_u32InitAddr);
//											break;
											//��ʼ��
										}										
									}
									else if(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 1)   
									{	  //0->2  -1->2  -2->1
										if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
										{
											if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X-1][l_u8tempj].FirWhelPos) > 4)
												break;
											else
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
												noMatch2Col = 1;
											}
										}										
										else if(ZTColum[l_u8X-1][l_u8tempi].WhelCnt == 2)
										{
											if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].SecWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos) < 4)
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											}
										}
										else if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)&&(ZTColum[l_u8X-1][l_u8tempi].WhelCnt == 1))
										{
											if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].SecWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos) >= 4)
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											}
											else
											{
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											}
										}
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
										if(noMatch2Col == 1)
										{
											l_u8ThreeFlag = 0;
											l_u32CrossAddr[0] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
											l_u32CrossAddr[1] = &ZTColum[l_u8X-1][l_u8tempi].Colum[1];
											l_u32CrossAddr[2] = &ZTColum[l_u8X-2][l_u8tempj].Colum[0];
											l_u32CrossAddr[3] = &ZTColum[l_u8X-2][l_u8tempj].FristElemRow;
											l_u32CrossAddr[4] = &l_u8ThreeFlag;
											WhetherCrossThreeLane(l_u32CrossAddr);
											if(l_u8ThreeFlag)
											{
												if(ZTColum[l_u8X-2][l_u8tempj].FristElemRow == 1)
												{
													
												}
												else if(ZTColum[l_u8X-2][l_u8tempj].WhelCnt == 2)
													break;
												else if((ZTColum[l_u8X-2][l_u8tempj].WhelCnt == 1) &&(IsBigTwo(ZTColum[l_u8X-2][l_u8tempj].FirWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >= 4))
													break;
												for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
														MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
												MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
												MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
												
												l_u8InitLine=l_u8tempj;
												l_u8ChlNum = l_u8X-2;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
											}
										}
										noMatch2Col = 0;
										//��ʼ��
// 										l_u8InitLine=*MtchColumPos;
// 										l_u8ChlNum = l_u8X;
// 										l_u32InitAddr[0] = &ZTColum[0][0];
// 										l_u32InitAddr[1] = &l_u8ChlNum;
// 										l_u32InitAddr[2] = &l_u8InitLine;
// 										ColumHideInitSolo(l_u32InitAddr);
										
										l_u8InitLine=l_u8tempi;
										l_u8ChlNum = l_u8X-1;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
							else
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
								for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
									MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
								MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
								MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
								MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
								MtchAxlePt->u8ValidChlCnt++;
								MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
								//��ʼ��
// 								l_u8InitLine=*MtchColumPos;
// 								l_u8ChlNum = l_u8X;
// 								l_u32InitAddr[0] = &ZTColum[0][0];
// 								l_u32InitAddr[1] = &l_u8ChlNum;
// 								l_u32InitAddr[2] = &l_u8InitLine;
// 								ColumHideInitSolo(l_u32InitAddr);
								
								l_u8InitLine=l_u8tempi;
								l_u8ChlNum = l_u8X-1;
								l_u32InitAddr[0] = &ZTColum[0][0];
								l_u32InitAddr[1] = &l_u8ChlNum;
								l_u32InitAddr[2] = &l_u8InitLine;
								ColumHideInitSolo(l_u32InitAddr);
								break;
							}
						}
					}
					else if(ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 1)//ֻ��һ��,��Ҫ��������ƥ��
					{
						//
						//if(ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 0) 
						if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
						{
							if(ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 1)
							{ //-1��û��λ����Ϣ�����������������������λ�ø�����
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
							}
							else if(ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 0)
							{
								if(IsBigTwo(ZTColum[l_u8X-1][l_u8tempi].FirWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos) < 4)
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}
								else
									break;
							}
						}
						else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
						{
							if(ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 1)
							{ //-1��û��λ����Ϣ�����������������������λ�ø�����
								//MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
							}
							else if(ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 0)
							{							
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							}
						}
							
						MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum] = ZTColum[l_u8X-1][l_u8tempi].Colum[0];
						MtchAxlePt->u16ChListSum++;
						MtchAxlePt->u16ChListSum=MtchAxlePt->u16ChListSum;
						MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt] = l_u8X-1;
						MtchAxlePt->u8ValidChlCnt++;
						MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
						if(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 3)
						{
							for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
							{
								if(ZTColum[l_u8X-2][l_u8tempj].Colum[0] == ZTColum[l_u8X-1][0].WonderLMtchID)
								{
									if(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 2)
									{
										l_u8CoinFlg = 0;
										l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
										l_u32CoinDegAddr[1] = &ZTColum[l_u8X-2][l_u8tempj].Colum[1];
										l_u32CoinDegAddr[2] = &l_u8CoinFlg;
										TwoZTCoinDegFunc(l_u32CoinDegAddr);
										if(l_u8CoinFlg)
										{   //2-1-2
											if(ZTColum[l_u8X-2][l_u8tempj].WhelCnt == 2)
												break;
											if(IsBigTwo(ZTColum[l_u8X-2][l_u8tempj].FirWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos) >12)
												break;
											else
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-2][l_u8tempj].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											}
											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
												MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
											MtchAxlePt->u8ValidChlCnt++;
											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
											//��ʼ��
											l_u8InitLine=l_u8tempj;
											l_u8ChlNum = l_u8X-2;
											l_u32InitAddr[0] = &ZTColum[0][0];
											l_u32InitAddr[1] = &l_u8ChlNum;
											l_u32InitAddr[2] = &l_u8InitLine;
											ColumHideInitSolo(l_u32InitAddr);
											l_u8MtchFlag=1;
											break;
										}
										else
										{
											l_u8MtchFlag=1;
											//��ʼ��ZTColum[l_u8X-1][0]
										}
									}
									else if(ZTColum[l_u8X-2][0].ZTCnt == 1)
									{
										//��ʼ��ZTColum[l_u8X-1][0]
										l_u8MtchFlag=1;
										break;
									}
								}
							}
							if(l_u8MtchFlag)
							{
								l_u8MtchFlag=0;
// 								l_u8InitLine=*MtchColumPos;
// 								l_u8ChlNum = l_u8X;
// 								l_u32InitAddr[0] = &ZTColum[0][0];
// 								l_u32InitAddr[1] = &l_u8ChlNum;
// 								l_u32InitAddr[2] = &l_u8InitLine;
// 								ColumHideInitSolo(l_u32InitAddr);
							}
						}
						else if((ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag > 1) && (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag < 2))
						{
							for(l_u8tempk=0;l_u8tempk<2;l_u8tempk++)
							{
								if(ZTColum[l_u8X-2][l_u8tempk].Colum[0] == ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID)
								{
									if(ZTColum[l_u8X-2][l_u8tempk].LeftMtchFlag < 2)
									{
										for(l_u8temp=0;l_u8temp<ZTColum[l_u8X-2][l_u8tempk].ZTCnt;l_u8temp++)
										{
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8temp] = ZTColum[l_u8X-2][l_u8tempk].Colum[l_u8temp];
										}
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempk].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempk].RunAndWalk;
//										MtchAxlePt->u8XieFlag = 1;
										
										l_u8InitLine=l_u8tempk;
										l_u8ChlNum = l_u8X-2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
						}
						l_u8InitLine=l_u8tempi;
						l_u8ChlNum = l_u8X-1;
						l_u32InitAddr[0] = &ZTColum[0][0];
						l_u32InitAddr[1] = &l_u8ChlNum;
						l_u32InitAddr[2] = &l_u8InitLine;
						ColumHideInitSolo(l_u32InitAddr);
						break;
							
						
					}
				}
				else if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempi].Colum[0])
				{
					if(ZTColum[l_u8X-2][l_u8tempi].ZTCnt == 2) //�ڶ���խ����Ҫ����ѡ���غ϶�ƥ��
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-2][l_u8tempi].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2) || (ZTColum[l_u8X-2][l_u8tempi].WhelCnt == 2))
								break;
							else 
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X-2][l_u8tempi].FirWhelPos) <=12)
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-2][l_u8tempi].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								}
								else
									break;
							}								
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempi].RunAndWalk;
							
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X-2;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
				}
			}
			for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
			{
				if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
				{
					if( ZTColum[l_u8X+1][l_u8tempi].FristElemRow == 0)
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[0];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][l_u8tempi].Colum[0];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
							
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X+1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
					else if( ZTColum[l_u8X+1][l_u8tempi].FristElemRow == 1)
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][l_u8tempi].Colum[0];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;						
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X+1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
				}
			
			}
		break;
		case 2:
			for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
			{
				if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
				{
					if(ZTColum[l_u8X+1][l_u8tempi].ZTCnt == 2) //�ڶ���խ����Ҫ����ѡ���غ϶�ƥ��
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][l_u8tempi].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
							if(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 3)
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0])
									{
										if(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 1)
										{
											l_u8ThreeFlag = 0;
											l_u32CrossAddr[0] = &ZTColum[l_u8X+1][l_u8tempi].Colum[0];
											l_u32CrossAddr[1] = &ZTColum[l_u8X+1][l_u8tempi].Colum[1];
											l_u32CrossAddr[2] = &ZTColum[l_u8X+2][l_u8tempj].Colum[0];
											l_u32CrossAddr[3] = &ZTColum[l_u8X+2][l_u8tempj].FristElemRow;
											l_u32CrossAddr[4] = &l_u8ThreeFlag;
											WhetherCrossThreeLane(l_u32CrossAddr);
											if(l_u8ThreeFlag)
											{ //2-2-1
												if(ZTColum[l_u8X+2][l_u8tempj].FristElemRow == 0)
												{
													if((ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2)||(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2))
														break;
													else
													{
														if(IsBigTwo(ZTColum[l_u8X+2][l_u8tempj].FirWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos)>12)
															break;
														else
														{
															MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
															MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
														}
													}
												}
												else if(ZTColum[l_u8X+2][l_u8tempj].FristElemRow == 1)
												{
													if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
														break;
													else if(ZTColum[l_u8X+1][l_u8tempi].WhelCnt == 1)
													{
														if(IsBigTwo(ZTColum[l_u8X+1][l_u8tempi].FirWhelPos,8*l_u8X+8) >= 4)
															break;
														else
														{
															MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
															MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
														}
													}
													else if(ZTColum[l_u8X+1][l_u8tempi].WhelCnt == 2)
													{
														if(IsBigTwo(ZTColum[l_u8X+1][l_u8tempi].SecWhelPos,8*l_u8X+8) >= 4)
															break;
														else
														{
															MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
															MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].SecWhelPos;
														}
													}
												}
												MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum] = ZTColum[l_u8X+2][l_u8tempj].Colum[0];
												MtchAxlePt->u16ChListSum++;
												MtchAxlePt->u16ChListSum=MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
												l_u8InitLine=l_u8tempj;
												l_u8ChlNum = l_u8X+2;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
											}
											//��ʼ��
											l_u8MtchFlag = 1;
											break;
										}
										else if(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 2)
										{
											l_u8CoinFlg = 0;
											l_u32CoinDegAddr[0] = &ZTColum[l_u8X+1][l_u8tempi].Colum[1];
											l_u32CoinDegAddr[1] = &ZTColum[l_u8X+2][l_u8tempj].Colum[1];
											l_u32CoinDegAddr[2] = &l_u8CoinFlg;
											//TwoZTCoinDegFunc(l_u32CoinDegAddr);
											//if(l_u8CoinFlg)
											{
												if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)||(ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2))
													break;
												else
												{
													if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X+2][l_u8tempj].FirWhelPos)>12)
														break;
													else
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
														MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
													}
												}
												for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
												MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
												MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
												l_u8InitLine=l_u8tempj;
												l_u8ChlNum = l_u8X+2;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
												//��ʼ��
												l_u8MtchFlag = 1;
												break;
											}	
										}							
									}
								}
// 								if(l_u8MtchFlag)
// 								{
									l_u8MtchFlag=0;
// 									l_u8InitLine=*MtchColumPos;
// 									l_u8ChlNum = l_u8X;
// 									l_u32InitAddr[0] = &ZTColum[0][0];
// 									l_u32InitAddr[1] = &l_u8ChlNum;
// 									l_u32InitAddr[2] = &l_u8InitLine;
// 									ColumHideInitSolo(l_u32InitAddr);
									
							//	}
							}
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X+1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
						else//����ƥ�䶼���ã�����Ҫ�������ƥ����
						{						
							if(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 3)
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if((ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 2) && (ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0]))
									{
										l_u8CoinFlg = 0;
										l_u32CoinDegAddr[0] = &ZTColum[l_u8X+1][l_u8tempi].Colum[1];
										l_u32CoinDegAddr[1] = &ZTColum[l_u8X+2][l_u8tempj].Colum[1];
										l_u32CoinDegAddr[2] = &l_u8CoinFlg;
										TwoZTCoinDegFunc(l_u32CoinDegAddr);
										if(l_u8CoinFlg)
										{
											if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
											{
												if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
													break;
												else
												{
													MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
													MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
												}								
											}
											else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
											}
											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
												MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
											MtchAxlePt->u8ValidChlCnt++;
											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
											
											l_u8CoinFlg = 0;
											l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
											l_u32CoinDegAddr[1] = &ZTColum[l_u8X+2][l_u8tempj].Colum[1];
											l_u32CoinDegAddr[2] = &l_u8CoinFlg;
											TwoZTCoinDegFunc(l_u32CoinDegAddr);
											if(l_u8CoinFlg)
											{				
												if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)||(ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2))
													break;
												else
												{
													if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X+2][l_u8tempj].FirWhelPos)>12)
														break;
													else
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
														MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
													}
												}
												for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
												MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
												MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
												MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
												MtchAxlePt->u8ValidChlCnt++;
												MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
												//��ʼ��
// 												l_u8InitLine=*MtchColumPos;
// 												l_u8ChlNum = l_u8X;
// 												l_u32InitAddr[0] = &ZTColum[0][0];
// 												l_u32InitAddr[1] = &l_u8ChlNum;
// 												l_u32InitAddr[2] = &l_u8InitLine;
// 												ColumHideInitSolo(l_u32InitAddr);
												
												l_u8InitLine=l_u8tempj;
												l_u8ChlNum = l_u8X+2;
												l_u32InitAddr[0] = &ZTColum[0][0];
												l_u32InitAddr[1] = &l_u8ChlNum;
												l_u32InitAddr[2] = &l_u8InitLine;
												ColumHideInitSolo(l_u32InitAddr);
											}
											else
											{
												//�غ϶Ȳ���,���Ҳ��ң��Ƿ�б��
												
											}
											l_u8InitLine=l_u8tempi;
											l_u8ChlNum = l_u8X+1;
											l_u32InitAddr[0] = &ZTColum[0][0];
											l_u32InitAddr[1] = &l_u8ChlNum;
											l_u32InitAddr[2] = &l_u8InitLine;
											ColumHideInitSolo(l_u32InitAddr);
											break;
										}
										else
										{
											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
											{
													MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
											}
											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
											MtchAxlePt->u8ValidChlCnt++;
											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
											//��ʼ��
// 											l_u8InitLine=*MtchColumPos;
// 											l_u8ChlNum = l_u8X;
// 											l_u32InitAddr[0] = &ZTColum[0][0];
// 											l_u32InitAddr[1] = &l_u8ChlNum;
// 											l_u32InitAddr[2] = &l_u8InitLine;
// 											ColumHideInitSolo(l_u32InitAddr);
// 											
											l_u8InitLine=l_u8tempi;
											l_u8ChlNum = l_u8X+1;
											l_u32InitAddr[0] = &ZTColum[0][0];
											l_u32InitAddr[1] = &l_u8ChlNum;
											l_u32InitAddr[2] = &l_u8InitLine;
											ColumHideInitSolo(l_u32InitAddr);
											break;	
										}										
									}
									else if((ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 1)&& (ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0]))
									{			
										if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
										{
											if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
												break;
											else
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
											}								
										}
										else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
										{
											MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
										}
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
										l_u8ThreeFlag = 0;
										l_u32CrossAddr[0] = &ZTColum[l_u8X+1][l_u8tempi].Colum[0];
										l_u32CrossAddr[1] = &ZTColum[l_u8X+1][l_u8tempi].Colum[1];
										l_u32CrossAddr[2] = &ZTColum[l_u8X+2][l_u8tempj].Colum[0];
										l_u32CrossAddr[3] = &ZTColum[l_u8X+2][l_u8tempj].FristElemRow;
										l_u32CrossAddr[4] = &l_u8ThreeFlag;
										WhetherCrossThreeLane(l_u32CrossAddr);
										{
											if(ZTColum[l_u8X+2][l_u8tempj].FristElemRow == 0)
											{
												if((ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2)||(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2))
													break;
												else
												{
													if(IsBigTwo(ZTColum[l_u8X+2][l_u8tempj].FirWhelPos,ZTColum[l_u8X][*MtchColumPos].FirWhelPos)>12)
														break;
													else
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
														MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
													}
												}
											}
											else if(ZTColum[l_u8X+2][l_u8tempj].FristElemRow == 1)
											{
												if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
													break;
												else if(ZTColum[l_u8X+1][l_u8tempi].WhelCnt == 1)
												{
													if(IsBigTwo(ZTColum[l_u8X+1][l_u8tempi].FirWhelPos,8*l_u8X+8) >= 4)
														break;
													else
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
														MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
													}
												}
												else if(ZTColum[l_u8X+1][l_u8tempi].WhelCnt == 2)
												{
													if(IsBigTwo(ZTColum[l_u8X+1][l_u8tempi].SecWhelPos,8*l_u8X+8) >= 4)
														break;
													else
													{
														MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
														MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].SecWhelPos;
													}
												}
											}
											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
												MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
											MtchAxlePt->u8ValidChlCnt++;
											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
											//��ʼ��
											l_u8InitLine=l_u8tempj;
											l_u8ChlNum = l_u8X+2;
											l_u32InitAddr[0] = &ZTColum[0][0];
											l_u32InitAddr[1] = &l_u8ChlNum;
											l_u32InitAddr[2] = &l_u8InitLine;
											ColumHideInitSolo(l_u32InitAddr);
										}
// 										
										l_u8InitLine=l_u8tempi;
										l_u8ChlNum = l_u8X+1;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
							else
							{
								if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
								{
									if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
										break;
									else
									{
										MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
										MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
									}								
								}
								else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
								}
								for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
									MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
								MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
								MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
								MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
								MtchAxlePt->u8ValidChlCnt++;
								MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
								//��ʼ��
// 								l_u8InitLine=*MtchColumPos;
// 								l_u8ChlNum = l_u8X;
// 								l_u32InitAddr[0] = &ZTColum[0][0];
// 								l_u32InitAddr[1] = &l_u8ChlNum;
// 								l_u32InitAddr[2] = &l_u8InitLine;
// 								ColumHideInitSolo(l_u32InitAddr);
								
								l_u8InitLine=l_u8tempi;
								l_u8ChlNum = l_u8X+1;
								l_u32InitAddr[0] = &ZTColum[0][0];
								l_u32InitAddr[1] = &l_u8ChlNum;
								l_u32InitAddr[2] = &l_u8InitLine;
								ColumHideInitSolo(l_u32InitAddr);
								break;
							}
						}
					}
					else if(ZTColum[l_u8X+1][l_u8tempi].ZTCnt == 1)//ֻ��һ��,��Ҫ��������ƥ��
					{
						if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
						{
							MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
						}
						else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
						{
							if(ZTColum[l_u8X+1][l_u8tempi].FristElemRow == 1)
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							else
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
							}
						}
						MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum] = ZTColum[l_u8X+1][l_u8tempi].Colum[0];
						MtchAxlePt->u16ChListSum++;
						MtchAxlePt->u16ChListSum=MtchAxlePt->u16ChListSum;
						MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
						MtchAxlePt->u8ValidChlCnt++;
						MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
						if(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 3)
						{
							for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
							{
								if(ZTColum[l_u8X+2][l_u8tempj].Colum[0] == ZTColum[l_u8X+1][0].WonderRMtchID)
								{
									if(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 2)
									{
										l_u8CoinFlg = 0;
										l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
										l_u32CoinDegAddr[1] = &ZTColum[l_u8X+2][l_u8tempj].Colum[1];
										l_u32CoinDegAddr[2] = &l_u8CoinFlg;
										TwoZTCoinDegFunc(l_u32CoinDegAddr);
										if(l_u8CoinFlg)
										{
											if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2) ||(ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2))
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												break;
											}
											else
											{
												if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X+2][l_u8tempj].FirWhelPos) > 12)
												{
													MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
													break;
												}
												else
												{
													MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
													MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
												}
											}
											for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
												MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
											MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
											MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
											//��ʼ��
											MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
											MtchAxlePt->u8ValidChlCnt++;
											MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
											l_u8InitLine=l_u8tempj;
											l_u8ChlNum = l_u8X+2;
											l_u32InitAddr[0] = &ZTColum[0][0];
											l_u32InitAddr[1] = &l_u8ChlNum;
											l_u32InitAddr[2] = &l_u8InitLine;
											ColumHideInitSolo(l_u32InitAddr);
											l_u8MtchFlag=1;
											break;
										}
										else
										{
											l_u8MtchFlag=1;
											//��ʼ��ZTColum[l_u8X+1][0]
										}
									}
									else if(ZTColum[l_u8X+2][0].ZTCnt == 1)
									{
										//��ʼ��ZTColum[l_u8X+1][0]
										l_u8MtchFlag=1;
										break;
									}
								}
							}
							if(l_u8MtchFlag)
							{
								l_u8MtchFlag=0;
// 								l_u8InitLine=*MtchColumPos;
// 								l_u8ChlNum = l_u8X;
// 								l_u32InitAddr[0] = &ZTColum[0][0];
// 								l_u32InitAddr[1] = &l_u8ChlNum;
// 								l_u32InitAddr[2] = &l_u8InitLine;
// 								ColumHideInitSolo(l_u32InitAddr);
							}
						}
						else if((ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag > 1) && (ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag < 2))
						{
							for(l_u8tempk=0;l_u8tempk<2;l_u8tempk++)
							{
								if(ZTColum[l_u8X+2][l_u8tempk].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
								{
									if(ZTColum[l_u8X+2][l_u8tempk].RightMtchFlag < 2)
									{
										if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2) ||(ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2))
										{
											MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
											break;
										}
										else
										{
											if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X+2][l_u8tempj].FirWhelPos) > 12)
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												break;
											}
											else
											{
												MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
												MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
											}
										}
										for(l_u8temp=0;l_u8temp<ZTColum[l_u8X+2][l_u8tempk].ZTCnt;l_u8temp++)
										{
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8temp] = ZTColum[l_u8X+2][l_u8tempk].Colum[l_u8temp];
										}
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempk].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempk].RunAndWalk;
//										MtchAxlePt->u8XieFlag = 1;
										
										l_u8InitLine=l_u8tempk;
										l_u8ChlNum = l_u8X+2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
						}
						l_u8InitLine=l_u8tempi;
						l_u8ChlNum = l_u8X+1;
						l_u32InitAddr[0] = &ZTColum[0][0];
						l_u32InitAddr[1] = &l_u8ChlNum;
						l_u32InitAddr[2] = &l_u8InitLine;
						ColumHideInitSolo(l_u32InitAddr);
						
						break;
					}
				}
				else if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempi].Colum[0])
				{
					if(ZTColum[l_u8X+2][l_u8tempi].ZTCnt == 2) //�ڶ���խ����Ҫ����ѡ���غ϶�ƥ��
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X+2][l_u8tempi].Colum[1];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						if(l_u8CoinFlg)
						{
							if((ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2) ||(ZTColum[l_u8X+2][l_u8tempj].WhelCnt == 2))
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								break;
							}
							else
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].FirWhelPos,ZTColum[l_u8X+2][l_u8tempj].FirWhelPos) > 12)
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									break;
								}
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+2][l_u8tempj].FirWhelPos;
								}
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempi].RunAndWalk;
							
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X+2;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
				}
			}
			for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
			{
				if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-1][l_u8tempi].Colum[0])
				{
					if( ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 0)
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[0];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						
						l_u8ThreeFlag = 0;
						if(ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 1)
						{							
							l_u32CrossAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[0];
							l_u32CrossAddr[1] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
							l_u32CrossAddr[2] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
							l_u32CrossAddr[3] = &ZTColum[l_u8X-1][l_u8tempi].FristElemRow;
							l_u32CrossAddr[4] = &l_u8ThreeFlag;
							WhetherCrossThreeLane(l_u32CrossAddr);
						}
						if((l_u8CoinFlg== 1) &&(((ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 1) &&(l_u8ThreeFlag==1)) || (ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 2)))
						{
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X-1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
					else if( ZTColum[l_u8X-1][l_u8tempi].FristElemRow == 1)
					{
						l_u8CoinFlg = 0;
						l_u32CoinDegAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
						l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
						l_u32CoinDegAddr[2] = &l_u8CoinFlg;
						TwoZTCoinDegFunc(l_u32CoinDegAddr);
						
						l_u8ThreeFlag = 0;
						if(ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 1)
						{							
							l_u32CrossAddr[0] = &ZTColum[l_u8X][*MtchColumPos].Colum[0];
							l_u32CrossAddr[1] = &ZTColum[l_u8X][*MtchColumPos].Colum[1];
							l_u32CrossAddr[2] = &ZTColum[l_u8X-1][l_u8tempi].Colum[0];
							l_u32CrossAddr[3] = &ZTColum[l_u8X-1][l_u8tempi].FristElemRow;
							l_u32CrossAddr[4] = &l_u8ThreeFlag;
							WhetherCrossThreeLane(l_u32CrossAddr);
						}
						if((l_u8CoinFlg== 1) &&(((ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 1) &&(l_u8ThreeFlag==1)) || (ZTColum[l_u8X-1][l_u8tempi].ZTCnt == 2)))
						{
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X-1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}
					}
					
				}
			
			}
		break;
		case 3:
			for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
			{
				if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-1][l_u8tempi].Colum[0])
				{	
					if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
					{
						if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >=4)
							break;
						else
						{
							MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
						}								
					}
					else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
					{
						MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
						MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
					}
					for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
						MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
					MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
					MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
					MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
					MtchAxlePt->u8ValidChlCnt++;
					MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
					
					l_u8InitLine=l_u8tempi;
					l_u8ChlNum = l_u8X-1;
					l_u32InitAddr[0] = &ZTColum[0][0];
					l_u32InitAddr[1] = &l_u8ChlNum;
					l_u32InitAddr[2] = &l_u8InitLine;
					ColumHideInitSolo(l_u32InitAddr);
					
				}
				if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
				{
					if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
					{
						if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
							break;
						else
						{
							MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
							MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
						}								
					}
					else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
					{
						MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
						MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
					}
					for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
						MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
					MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
					MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
					MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
					MtchAxlePt->u8ValidChlCnt++;
					MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
					l_u8InitLine=l_u8tempi;
					l_u8ChlNum = l_u8X+1;
					l_u32InitAddr[0] = &ZTColum[0][0];
					l_u32InitAddr[1] = &l_u8ChlNum;
					l_u32InitAddr[2] = &l_u8InitLine;
					ColumHideInitSolo(l_u32InitAddr);
					
				}											
			}
		break;
								
		case 4:
			if((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 4) || (ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 6))
			{
				for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
				{
					if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-1][l_u8tempi].Colum[0])
					{
						//if((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][l_u8tempi].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm)))
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
							MtchAxlePt->u8ValidChlCnt++;
//							MtchAxlePt->u8XieFlag = 1;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
							if((ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 4)||(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 6)||(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 3))
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(((ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 2) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X-2][l_u8tempj].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm)) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X-2][l_u8tempj].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[1]].u32Maxtm)))||\
									((ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 1)))
									{
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
										//��ʼ��

										l_u8InitLine=l_u8tempj;
										l_u8ChlNum = l_u8X-2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);										
										break;
									}
								}
							}						
						}
						l_u8InitLine=l_u8tempi;
						l_u8ChlNum = l_u8X-1;
						l_u32InitAddr[0] = &ZTColum[0][0];
						l_u32InitAddr[1] = &l_u8ChlNum;
						l_u32InitAddr[2] = &l_u8InitLine;
						ColumHideInitSolo(l_u32InitAddr);
					}
				}
			}
			else if((ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 5) || (ZTColum[l_u8X][*MtchColumPos].LeftMtchFlag == 7))
			{
				for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
				{
					if(ZTColum[l_u8X][*MtchColumPos].WonderLMtchID == ZTColum[l_u8X-1][l_u8tempi].Colum[0])
					{
					//	if(IsBigFlag(g_ChlData[ZTColum[l_u8X+1][l_u8tempi].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm))
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X-1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X-1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-1][l_u8tempi].RunAndWalk;
//							MtchAxlePt->u8XieFlag = 2;
							if((ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 5)||(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 7)||(ZTColum[l_u8X-1][l_u8tempi].LeftMtchFlag == 3))
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(((ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 2) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X-2][l_u8tempj].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm)) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X-2][l_u8tempj].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[1]].u32Maxtm)))||\
									((ZTColum[l_u8X-1][l_u8tempi].WonderLMtchID == ZTColum[l_u8X-2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X-2][l_u8tempj].ZTCnt == 1)))
									{
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X-2][l_u8tempj].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X-2][l_u8tempj].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X-2][l_u8tempj].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X-2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X-2][l_u8tempj].RunAndWalk;
										//��ʼ��
										
										l_u8InitLine=l_u8tempi;
										l_u8ChlNum = l_u8X-1;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										
										l_u8InitLine=l_u8tempj;
										l_u8ChlNum = l_u8X-2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
							else
							{
								
								l_u8InitLine=l_u8tempi;
								l_u8ChlNum = l_u8X-1;
								l_u32InitAddr[0] = &ZTColum[0][0];
								l_u32InitAddr[1] = &l_u8ChlNum;
								l_u32InitAddr[2] = &l_u8InitLine;
								ColumHideInitSolo(l_u32InitAddr);
								break;
							}
						}
					}
				}
			}
		
			
			if((ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 4) || (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 6))
			{
				for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
				{
					if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
					{
						//if((IsBigFlag(g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][l_u8tempi].Colum[0]].u32Maxtm)))
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
//							MtchAxlePt->u8XieFlag = 1;
							if((ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 4)||(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 6) || (ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 3))
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(((ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 2) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X+2][l_u8tempj].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm)) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X+2][l_u8tempj].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[1]].u32Maxtm)))||\
									((ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 1)))
									{
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
										//��ʼ��
																				
// 										l_u8InitLine=l_u8tempi;
// 										l_u8ChlNum = l_u8X+1;
// 										l_u32InitAddr[0] = &ZTColum[0][0];
// 										l_u32InitAddr[1] = &l_u8ChlNum;
// 										l_u32InitAddr[2] = &l_u8InitLine;
// 										ColumHideInitSolo(l_u32InitAddr);
										
										l_u8InitLine=l_u8tempj;
										l_u8ChlNum = l_u8X+2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										l_u8MtchFlag = 1;
										break;
									}
								}
								if(l_u8MtchFlag == 1)
								{
									l_u8MtchFlag = 0;
								}
															
									
							}
							l_u8InitLine=l_u8tempi;
							l_u8ChlNum = l_u8X+1;
							l_u32InitAddr[0] = &ZTColum[0][0];
							l_u32InitAddr[1] = &l_u8ChlNum;
							l_u32InitAddr[2] = &l_u8InitLine;
							ColumHideInitSolo(l_u32InitAddr);
							break;
						}				
					}
				}
			}
			
			else if((ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 5) || (ZTColum[l_u8X][*MtchColumPos].RightMtchFlag == 7))
			{
				for(l_u8tempi=0;l_u8tempi<2;l_u8tempi++)
				{
					if(ZTColum[l_u8X][*MtchColumPos].WonderRMtchID == ZTColum[l_u8X+1][l_u8tempi].Colum[0])
					{
					//	if(IsBigFlag(g_ChlData[ZTColum[l_u8X+1][l_u8tempi].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm))
						{
							if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 2)
							{
								if(IsBigTwo(ZTColum[l_u8X][*MtchColumPos].SecWhelPos,ZTColum[l_u8X+1][l_u8tempi].FirWhelPos) >=4)
									break;
								else
								{
									MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
									MtchAxlePt->HideRightWhePos = ZTColum[l_u8X][*MtchColumPos].SecWhelPos;
								}								
							}
							else if(ZTColum[l_u8X][*MtchColumPos].WhelCnt == 1)
							{
								MtchAxlePt->HideLeftWhePos = ZTColum[l_u8X][*MtchColumPos].FirWhelPos;
								MtchAxlePt->HideRightWhePos = ZTColum[l_u8X+1][l_u8tempi].FirWhelPos;
							}
							for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+1][l_u8tempi].ZTCnt;l_u8tempk++)
								MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+1][l_u8tempi].Colum[l_u8tempk];
							MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+1][l_u8tempi].ZTCnt;
							MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
							MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+1;
							MtchAxlePt->u8ValidChlCnt++;
							MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+1][l_u8tempi].RunAndWalk;
//							MtchAxlePt->u8XieFlag = 2;
							if((ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 5)||(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 7)||(ZTColum[l_u8X+1][l_u8tempi].RightMtchFlag == 3))
							{
								for(l_u8tempj=0;l_u8tempj<2;l_u8tempj++)
								{
									if(((ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 2) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X+2][l_u8tempj].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[0]].u32Maxtm)) &&\
										(IsBigFlag(g_ChlData[ZTColum[l_u8X+2][l_u8tempj].Colum[1]].u32Maxtm,g_ChlData[ZTColum[l_u8X][*MtchColumPos].Colum[1]].u32Maxtm)))||\
									((ZTColum[l_u8X+1][l_u8tempi].WonderRMtchID == ZTColum[l_u8X+2][l_u8tempj].Colum[0])&&(ZTColum[l_u8X+2][l_u8tempj].ZTCnt == 1)))
									{
										for(l_u8tempk=0;l_u8tempk<ZTColum[l_u8X+2][l_u8tempj].ZTCnt;l_u8tempk++)
											MtchAxlePt->u16ChList[MtchAxlePt->u16ChListSum + l_u8tempk] = ZTColum[l_u8X+2][l_u8tempj].Colum[l_u8tempk];
										MtchAxlePt->u16ChListSum +=ZTColum[l_u8X+2][l_u8tempj].ZTCnt;
										MtchAxlePt->u16ChListSum = MtchAxlePt->u16ChListSum;
										MtchAxlePt->AxleChlPos[MtchAxlePt->u8ValidChlCnt]= l_u8X+2;
										MtchAxlePt->u8ValidChlCnt++;
										MtchAxlePt->RunAndWalkFlag += ZTColum[l_u8X+2][l_u8tempj].RunAndWalk;
										//��ʼ��
										
										l_u8InitLine=l_u8tempi;
										l_u8ChlNum = l_u8X+1;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										
										l_u8InitLine=l_u8tempj;
										l_u8ChlNum = l_u8X+2;
										l_u32InitAddr[0] = &ZTColum[0][0];
										l_u32InitAddr[1] = &l_u8ChlNum;
										l_u32InitAddr[2] = &l_u8InitLine;
										ColumHideInitSolo(l_u32InitAddr);
										break;
									}
								}
							}
							else
							{
								
								l_u8InitLine=l_u8tempi;
								l_u8ChlNum = l_u8X+1;
								l_u32InitAddr[0] = &ZTColum[0][0];
								l_u32InitAddr[1] = &l_u8ChlNum;
								l_u32InitAddr[2] = &l_u8InitLine;
								ColumHideInitSolo(l_u32InitAddr);
								break;
							}
						}
					}
				}
			}
			break;
		default:
		break;
	}
}
/*********************************************************************************************************
** Function name:     ColumHideInitSolo
** Descriptions:      ��ʼ���еĵ���һ��
** input parameters:  �нṹ�����飬λ��     
** output parameters:   
**
** Created by:	zhangtonghan  
** Created Date:  2017-10-27	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ColumHideInitSolo(void* Addressed[4])
{
	HideColum_Struct (*ZTColum)[HIDECOLUM_SAVEAXlE]; 	

	uint8 *Line,*Pos;

	uint8 l_u8temp;

	ZTColum = Addressed[0];	
	Line = Addressed[1]; 
	Pos = Addressed[2];

	for(l_u8temp = *Pos;(l_u8temp + 1) < HIDECOLUM_SAVEAXlE;l_u8temp++)
	{
		memcpy(&ZTColum[*Line][l_u8temp],&ZTColum[*Line][l_u8temp + 1],sizeof(HideColum_Struct));
	}
	ColumHideInit(&ZTColum[*Line][0],1);
}
