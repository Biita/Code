/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			prtclWJJD.c
** Last modified Date:  20121031
** Last Version:		1.0
** Descriptions:		非现场执法协议
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wgh
** Created date:		20121031
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------	
*********************************************************************************************************/

#define __PRTCLWJJD_C
#include "common.h"


#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

void WJJD_ProcessCmd(uint8 *pData)
{
	uint16 l_u16RecDataLen;
//	uint16 l_u16RecCRC;
	uint8  l_u8DataBuf[500];
	uint8*  l_u8Buf;
	//
	uint8	l_u8Err;
	uint16	l_u16SendLen;
	//

	l_u16RecDataLen = (pData[3]<<8) + pData[2];
	if((pData[l_u16RecDataLen+6] == 0xEE) && (pData[l_u16RecDataLen+7] == 0xEE))
	{
		g_u8RecOrderCnt ++;
//		l_u16RecCRC = (pData[l_u16RecDataLen+5]<<8) + pData[l_u16RecDataLen+4];
		if(((pData[l_u16RecDataLen+5]<<8) + pData[l_u16RecDataLen+4]) == JDCRC16(pData+2,l_u16RecDataLen+2))
		{
			memcpy(l_u8DataBuf, pData+4, l_u16RecDataLen);
		}	
		else
			return;	//错误返回		
	}
	else 
		return;	//错误返回
	 
	// 接收到网口信息added by wujinlong 20130731
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf, "%04u-%02u-%02u %02u:%02u:%02u T7 Recv Comp JDCmd %x [Net]。\r\n",
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
	if(l_u8DataBuf[0] == 0x0A)									//0A包
	{
		g_u16RecOrd0ANum = l_u8DataBuf[1] + (l_u8DataBuf[2]<<8);               // 解析时间序号
		g_u8JD0AOrderRev = 1;
	}
	else if(FCheckDeviceID(l_u8DataBuf+1))
	{
		switch(l_u8DataBuf[0])
		{
			case 0x02: //02判断与处理
			     switch(l_u8DataBuf[17])
			     {	    
			       case 0x01:
				   case 0x02:
				   case 0x03:  //设备连接正常
							 g_u8JD02OrderRev = 1;
							 break;
				   case 0x09:
//							 Flag_08_NotReturn = 1;
							 FUpOrderData(l_u8Buf, 8);//更新08包
							 FSendData(l_u8Buf, 65);//发送08包											
							 break;
					 default:
						     break;
			     }
			break;
			case 0x09: // 数据重传与处理
			    FAnalyResendData(l_u8DataBuf);
			break;
			case 0x35:
				FAskAxleLoadData(l_u8DataBuf);
			break;
			default:
				if(FCheckUserKey(l_u8DataBuf))  		//判断用户名和密码
				{
					FProcessPrt(l_u8DataBuf);
				}										
			break;
		}
	}
}

void WJJDSendCommandToPC0(uint8 p_u8VehIndex,uint8 p_u8CommandNO)
{
}

uint8  FSendData(uint8 *pBuf, uint16 pLen)
{
	 if(pLen == 0)
	 {
	 	return 0;
	 }
	 FCompleteData(pBuf, pLen);
	 return 1;
}

uint16  FCompleteData(uint8 *pBuf, uint16 pLen)
{
	uint8*  l_u8Data;
	uint16 l_u16CRC;

	if(pLen == 0)
	{
	   return 0;
	}
	l_u8Data = g_cJDSendVehData[g_u8SendVehDataInd].au8data[0];
	//帧头
	l_u8Data[0] = 0xAA;
	l_u8Data[1] = 0xAA;
	//帧长
	l_u8Data[2] = pLen & 0xFF;
	l_u8Data[3] = (pLen>>8) & 0xFF;
	memcpy(l_u8Data+4, pBuf, pLen);
	//CRC校验
	l_u16CRC = JDCRC16(l_u8Data+2,pLen+2);
	l_u8Data[pLen+4]	= l_u16CRC & 0xFF;
	l_u8Data[pLen+5]	= (l_u16CRC>>8) & 0xFF;

	//帧尾
	l_u8Data[pLen+6]	= 0xEE;
	l_u8Data[pLen+7]	= 0xEE;
	g_cJDSendVehData[g_u8SendVehDataInd].au32len[0] = pLen+8;
	OSQPost(g_pQJDSend, &g_cJDSendVehData[g_u8SendVehDataInd]);
	g_u8SendVehDataInd ++;
	g_u8SendVehDataInd %= ARRAYNUM;
	return 1;
}
void   FUpOrderData(uint8* pBuf, uint8 pCMD)
{
	uint8 l_u8tempi;
	switch(pCMD)
	{
		case 2:
			pBuf[0] = 0x02;
			// 设备身份识别码
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			   pBuf[l_u8tempi+1] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];
			}
			//更新02包时间
			pBuf[17] = 0x01;
		break;
		case 8:
			pBuf[0] = 0x08;
			// 设备身份识别码
			for(l_u8tempi=0;l_u8tempi<16;l_u8tempi++)
			{
			   pBuf[l_u8tempi+1] = JDSYSPARAM.m_au8DeviceID[l_u8tempi];
			}
			//更新08包时间
			pBuf[17] = (g_DevCurTime.u16Year) & 0xFF;
			pBuf[18] = ((g_DevCurTime.u16Year)>> 8) & 0xFF;
			pBuf[19] = g_DevCurTime.u8Month;
			pBuf[20] = g_DevCurTime.u8Day;
			pBuf[21] = g_DevCurTime.u8Hour; 
			pBuf[22] = g_DevCurTime.u8Minute;
			pBuf[23] = g_DevCurTime.u8Second;
			// 数据处理周期
			pBuf[24] = g_u8PeriodTOSave;
		    // 调查内容
			pBuf[25] = JDSYSPARAM.m_u8OrderMod;
			// 服务器IP
			pBuf[26] = 10;
			pBuf[27] = 252;
			pBuf[28] = 0;
			pBuf[29] = 39;
			// 站点编号
			for(l_u8tempi=0;l_u8tempi<15;l_u8tempi++)
			{
			   pBuf[l_u8tempi+30] = JDSYSPARAM.m_au8StationID[l_u8tempi];
			}
			// 鉴别时间
			pBuf[45] = JDSYSPARAM.m_u32FollowVehTimeThr;
			// 传输方式
			pBuf[46] = JDSYSPARAM.m_u8TransMod;
			// 经度坐标
			for(l_u8tempi=0;l_u8tempi<8;l_u8tempi++)
			{
				pBuf[l_u8tempi+47] = JDSYSPARAM.m_au8LngBuf[l_u8tempi];
			}
			// 纬度
			for(l_u8tempi=0;l_u8tempi<8;l_u8tempi++)
			{
				pBuf[l_u8tempi+55] = JDSYSPARAM.m_au8LatBuf[l_u8tempi];
			}
			// 高程
			pBuf[63] = JDSYSPARAM.m_au16Elevation&0xFF; 
			pBuf[64] = (JDSYSPARAM.m_au16Elevation>>8)&0xFF;
		break;
		default:
		break;
	}
}

void  FAnalyResendData(uint8* pBuf)
{
	SystemTime l_sTime;
	uint16 l_u16Num;
	uint8  l_u8ReType;
  	
	//解析年、月、日
    l_sTime.u16Year  = pBuf[33] + (pBuf[34]<<8);
	l_sTime.u8Month = pBuf[35];
	l_sTime.u8Day   = pBuf[36];
	l_u16Num = pBuf[37]+(pBuf[38]<<8); 

	switch(pBuf[0])
	{
	    case 0x09:
				  g_u8JD0AOrderRev = 2;
				  // 计算开始序号（SD卡存储序号）
//				  for(l_u8tempi=0;l_u8tempi<6;l_u8tempi++)
//				  {
//				      l_u8Buf[l_u8tempi] = pBuf[33+l_u8tempi];
//				  }
				  g_u32Resend01SDBgNum = FGetSaveNumFromDate(&l_sTime, l_u16Num);
				  // 计算结束序号（SD卡存储序号）
//				  l_u8Buf[4] = pBuf[39];
//				  l_u8Buf[5] = pBuf[40];
				  l_u16Num = pBuf[39]+(pBuf[40]<<8);
				  g_u32Resend01SDEndNum = FGetSaveNumFromDate(&l_sTime, l_u16Num);;
				  // 如果结束序号小于开始序号，则不处理返回
				  if( g_u32Resend01SDBgNum > g_u32Resend01SDEndNum )
				  {
				      g_u32Resend01SDBgNum = 0;
					  g_u32Resend01SDEndNum = 0;
				  }
				  // 如果为未来序号，则不处理返回
				  if( g_u32Resend01SDEndNum > g_u32JDSDSaveInd )
				  {
				  	  g_u32Resend01SDBgNum = 0;
					  g_u32Resend01SDEndNum = 0;
				  }     	
				  break;
		case 0x69:			  
			      // 判断重传数据类型
			      l_u8ReType = pBuf[37];
			      switch( l_u8ReType )
			      {
			          case 0x01: 	 // 重传单车数据(39)
				                g_u32Resend39 = pBuf[38]+(pBuf[39]<<8)+(pBuf[40]<<16)+(pBuf[41]<<24);
							    if(g_u32Resend39 > g_u32JD39SaveSuccInd)
							    {
							       g_u32Resend39 = 0;
						 	    }
				                break;
				  	  case 0x02:	 // 重量统计数据(61)
							    // 计算开始序号（SD卡存储序号）
								l_u16Num = pBuf[42]+ (pBuf[43]<<8);
							    g_u8Resend61Bg = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 计算结束序号（SD卡存储序号）
								l_u16Num = pBuf[44]+ (pBuf[45]<<8);
						        g_u8Resend61Ed = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 如果结束序号小于开始序号，则不处理返回
						    	if( g_u8Resend61Bg > g_u8Resend61Ed )
							    {
							        g_u8Resend61Bg = 0;
								    g_u8Resend61Ed = 0;
							    }
							    // 如果为未来序号，则不处理返回
							    if( g_u8Resend61Ed > g_u32JD61SaveSuccInd )
							    {
							  	    g_u8Resend61Bg = 0;
								    g_u8Resend61Ed = 0;
							    }     
								break;
				      case 0x03:	 // 轴数统计数据(62)
							    // 计算开始序号（SD卡存储序号）
								l_u16Num = pBuf[42]+ (pBuf[43]<<8);
							    g_u8Resend62Bg = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 计算结束序号（SD卡存储序号）
								l_u16Num = pBuf[44]+ (pBuf[45]<<8);
						        g_u8Resend62Ed = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 如果结束序号小于开始序号，则不处理返回
						    	if( g_u8Resend62Bg > g_u8Resend62Ed )
							    {
							        g_u8Resend62Bg = 0;
								    g_u8Resend62Ed = 0;
							    }
							    // 如果为未来序号，则不处理返回
							    if( g_u8Resend62Ed > g_u32JD62SaveSuccInd )
							    {
							  	    g_u8Resend62Bg = 0;
								    g_u8Resend62Ed = 0;
							    }     
								break;
				      case 0x04:	// 轴重统计数据(68)
					            // 计算开始序号（SD卡存储序号）
								l_u16Num = pBuf[42]+ (pBuf[43]<<8);
							    g_u8Resend68Bg = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 计算结束序号（SD卡存储序号）
								l_u16Num = pBuf[44]+ (pBuf[45]<<8);
						        g_u8Resend68Ed = FGetSaveNumFromDate(&l_sTime, l_u16Num);
						        // 如果结束序号小于开始序号，则不处理返回
						    	if( g_u8Resend68Bg > g_u8Resend68Ed )
							    {
							        g_u8Resend68Bg = 0;
								    g_u8Resend68Ed = 0;
							    }
							    // 如果为未来序号，则不处理返回
							    if( g_u8Resend68Ed > g_u32JD68SaveSuccInd )
							    {
							  	    g_u8Resend68Bg = 0;
								    g_u8Resend68Ed = 0;
							    }     
								break;
				       default:
					           break;        
			       } 	
			    break;
		default:
			    break; 
	
	}
}

uint8  FReSenddata(uint8 l_pCMD, uint32 l_pBgnNum, uint32 l_pEndNum)
{
	uint8   l_u8tempi;
	uint8   l_u8Err;
	uint16 	l_u16Cnt = 0;
	uint8   *l_u8Buf;
	uint8   l_au8DateTimeNum[6];
	uint8   l_u8SendErr = 0;
	uint8   l_u8SDFlag;
	uint8   l_u8SendFeedback;
	uint8   l_u8ErrDataFlag;
	uint8   l_u8Return = 0;
	uint8   l_au8SendData[512];
	uint16  l_u16DataLen;
	uint16  l_u16NeedSendNum;
	uint16  l_u16ReadTimeNumFromSD;
	uint32  l_32SDBaseAddr;
	uint32  l_u32ResendAddr;

	switch(l_pCMD)
	{
	    case 0x01:
		          l_32SDBaseAddr = g_u32JDInfo_Addr_Begin;
				  break;
		case 0x61:
				  l_32SDBaseAddr = g_u32JD61BaseAdr;
				  break;
		case 0x62:
		          l_32SDBaseAddr = g_u32JD61BaseAdr;
				  break;
		case 0x63:
				  l_32SDBaseAddr = g_u32JD61BaseAdr;
				  break;
	      default:
				  break;
	}


    if( l_pBgnNum <= l_pEndNum )
	{
		// 通过存储序号，得到日期及时间序号
		l_u8Buf = FGetDateFromNum(l_pBgnNum);
		for(l_u8tempi=0;l_u8tempi<6;l_u8tempi++)
		{
		   l_au8DateTimeNum[l_u8tempi] = *(l_u8Buf+l_u8tempi);
		}
		// 将要发送的时间序号
		l_u16NeedSendNum = l_au8DateTimeNum[4] + (l_au8DateTimeNum[5]<<8);
		// 得到SD卡扇区地址
		l_u32ResendAddr = l_32SDBaseAddr + l_pBgnNum;
		// 读SD卡数据
		l_u8SDFlag = SD_ReadBlock( &sd_info, l_u32ResendAddr,l_au8SendData );
		if( l_u8SDFlag !=0 )
		{
		    l_u8SendErr = 101;
//			SD_Err_Reset_cnt++;
		}
		else
		{
//		    SD_Err_Reset_cnt = 0;
		}

		// 读取数据包长度
		l_u16DataLen = (l_au8SendData[510]<<8) + l_au8SendData[511];
		// 从SD卡读出来的时间序号
		switch(l_pCMD)
		{
		    case 0x01:
					  l_u16ReadTimeNumFromSD = l_au8SendData[39] + (l_au8SendData[40]<<8);
					  break;
			case 0x61:
			case 0x62:
			case 0x68:
			          l_u16ReadTimeNumFromSD = l_au8SendData[38] + (l_au8SendData[39]<<8);
					  break;
			  default:
			          break;
		}
		// 检查数据是否异常
		l_u8ErrDataFlag = FCheckSendData(l_au8SendData,l_pCMD);  // 检查命令字、识别码、站点编号
		
		// 判断数据是否异常
		if( (l_u16ReadTimeNumFromSD != l_u16NeedSendNum)||(l_u16DataLen==0)||(l_u16DataLen>=1000) )
		{
			l_u8ErrDataFlag = 1;
		}	

		if( l_u8ErrDataFlag == 1 )   // 数据异常 
		{
			BeepON();
			OSTimeDly(10);
			BeepOFF();	
			OSTimeDly(10);
			if( l_u8SendErr == 0 )
			{
			    l_u8SendErr = 100;
			}
			// 数据异常			
		    l_u16DataLen = FGetSendDataForErr( l_pCMD, l_au8SendData, l_au8DateTimeNum, l_u8SendErr );
			//发送数据包 
			l_u8SendFeedback = FSendData(l_au8SendData,l_u16DataLen);
		}
		else
		{
			//发送数据包 
			l_u8SendFeedback = FSendData(l_au8SendData,l_u16DataLen);		
		} 	

		if( l_u8SendFeedback == 1 )    // 发送完成
		{
		    if( l_pCMD == 0x01 )
		    {
			    while(g_u8JD0AOrderRev == 0 && l_u16Cnt<40)
				{
					OSTimeDly(100);
					l_u16Cnt ++;
				} 
				if(g_u8JD0AOrderRev == 1)  	//等待0A包返回。2000--5s; 8000--20s; 10000--25s
				{	
					g_u8JD0AOrderRev = 0;
					l_u8Err = 0;
				}
				else
				{
					g_u8JD0AOrderRev = 0;
					l_u8Err = 1;
				}
			}
			else
		    {
			    while(g_u8JD6FOrderRev == 0 && l_u16Cnt<40)
				{
					OSTimeDly(100);
					l_u16Cnt ++;
				} 
				if(g_u8JD6FOrderRev == 1)  	//等待0A包返回。2000--5s; 8000--20s; 10000--25s
				{	
					g_u8JD6FOrderRev = 0;
					l_u8Err = 0;
				}
				else
				{
					g_u8JD6FOrderRev = 0;
					l_u8Err = 1;
				}
			}
			 
			if( l_u8Err == 0 )
			{
				switch( l_pCMD )
				{
				    case 0x01:
				     	  if( l_u16NeedSendNum == g_u16RecOrd0ANum )
						  {
						  	  g_u16RecOrd0ANum = 0;
							  // 得到SD卡扇区地址, Resend_01_SD_BgnNum 的下一个序号
                              g_u32Resend01SDBgNum = FGetNextSDNum(l_pBgnNum,g_u8PeriodTOSave);
						  }
						  break;
					case 0x61:
				     	  if( l_u16NeedSendNum == g_u16RecOrd61Num )
						  {
						  	  g_u16RecOrd61Num = 0xffff;
							  // 得到SD卡扇区地址, Resend_61_SD_BgnNum 的下一个序号
                              g_u8Resend61Bg = FGetNextSDNum(l_pBgnNum,g_u8PeriodTOSave);
						  }							  
						  break;
				    case 0x62:
				     	  if( l_u16NeedSendNum == g_u16RecOrd62Num )
						  {
						  	  g_u16RecOrd62Num = 0xffff;
							  // 得到SD卡扇区地址, Resend_62_SD_BgnNum 的下一个序号
                              g_u8Resend62Bg = FGetNextSDNum(l_pBgnNum,g_u8PeriodTOSave);
						  }							  
						  break;			
				    case 0x68:
				     	  if( l_u16NeedSendNum == g_u16RecOrd68Num )
						  {
						  	  g_u16RecOrd68Num = 0xffff;
							  // 得到SD卡扇区地址, Resend_68_SD_BgnNum 的下一个序号
                              g_u8Resend68Bg = FGetNextSDNum(l_pBgnNum,g_u8PeriodTOSave);
						  }							  
						  break;			
					  default:
					          break;
				} 
				
				l_u8Return = 1;
			}
			else 				//等待20s
			{
			   	l_u8Return = 0;
			}
		}
		else       // 发送失败
		{
		    l_u8Return = 0;
		}  
	}
	else 
	{
	    switch(l_pCMD)
		{
		    case 0x01:
			     	  g_u32Resend01SDBgNum = 0;
		              g_u32Resend01SDEndNum = 0;
					  break;
			 case 0x61:
			     	  g_u8Resend61Bg = 0;
		              g_u8Resend61Ed = 0;
					  break;
		    case 0x62:
			     	  g_u8Resend62Bg = 0;
		              g_u8Resend62Ed = 0;
					  break;			
		    case 0x68:
			     	  g_u8Resend68Bg = 0;
		              g_u8Resend68Ed = 0;
					  break;
			  default:
			          break;
		} 
		
		l_u8Return = 0;
	}
	return l_u8Return;
}

void  FProcessPrt(uint8* pBuf)
{
	uint8 l_u8tempi;
	switch(pBuf[0])
	{
		case 0x03:
				  for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
				  {
				  	JDSYSPARAM.m_au8StationID[l_u8tempi] = pBuf[33+l_u8tempi];
				  }//Change_RD_Num((struct ASK_03 *)l_u8DataBuf,len);
//				  JDSYSPARAM.m_au8StationID[14] = 0;
			      break;
		case 0x04:
			      //Change_DSC_Ip((struct ASK_04 *)l_u8DataBuf,len);
				  break;
		case 0x05:
			      //Change_Time((struct ASK_05 *)l_u8DataBuf,len);
			      break;
		case 0x06:
			      JDSYSPARAM.m_u8OrderMod = pBuf[33+l_u8tempi];//Change_InvContents((struct ASK_06 *)l_u8DataBuf,len);
			      break;
		case 0x07:
			      JDSYSPARAM.m_u8SavePeriod = pBuf[33+l_u8tempi];
				  //Change_ProCycle((struct ASK_07 *)l_u8DataBuf,len);
			      break;
		case 0x0B:
			      JDSYSPARAM.m_u32FollowVehTimeThr = pBuf[33+l_u8tempi];//Change_DisTime((struct ASK_0B *)l_u8DataBuf,len);
			      break;
		case 0x30:	     // 修改通信模式 jiawei 
				  //Change_Commun_Mode((struct ASK_30 *)l_u8DataBuf,len);
		          break;
		case 0x64:
                  //Change_Car_Sendtype(l_u8DataBuf);
		          break;
        case 0x69:	   // 轴载数据重传
				  //Analytic_Resend_data(l_u8DataBuf);
                  break;
		case 0x70:
		 	      //Flag_78_reset = 1;//设置重启标志
                  break;
	    case 0x71:
		          //Flag_read_weightcoef = 1;
			      //chedaonum = l_u8DataBuf[33];
			      break;	
		case 0x72:
			      //Set_weight_Num((struct ASK_72 *)l_u8DataBuf,len);	
			      //Flag_78_setweight = 1;//设置轴载参数			
			      break;
		 case 0x73:
		          //Flag_read_transmit = 1;
			      break; 	
		case 0x74:
		          JDSYSPARAM.m_u32DiffSend = (pBuf[37])+ ((pBuf[38])<<8)
		                        +((pBuf[39])<<16)+((pBuf[40])<<24);
				  //Flag_78_upperdata = 1;//设置RD待传数据包数量上限值指令标志
			      //Set_RD_Upperdata((struct ASK_74 *)p,len);
			      break;
		case 0x75: 
				  InitJDParam();
//	              fm_writeDefault();	//初始化参数设置
//			      Flag_78_setRD = 1;//设置RD初始化参数指令数据包
			      break;
		 default:
			      break; 
	}
	AddCrc16((uint8 *)&JDSYSPARAM, JDSYSPARAMSIZE-2);	 
			   
 	WriteC256(JDSYSADDR,(uint8 *)&JDSYSPARAM, JDSYSPARAMSIZE);	// 在0x00地址处写入32字节数据
}

uint8  FCheckUserKey(uint8* pBuf)
{
	uint8 l_u8tempi, l_u8Return=1;
	for(l_u8tempi=0;l_u8tempi<8;l_u8tempi++)
	{
		if(pBuf[17+l_u8tempi] != JDSYSPARAM.m_au8UseName[l_u8tempi])
		{
			l_u8Return = 0;
			return l_u8Return;
		}
	}
	for(l_u8tempi=0;l_u8tempi<8;l_u8tempi++)
	{
		if(pBuf[25+l_u8tempi] != JDSYSPARAM.m_au8KeyNum[l_u8tempi])
		{
			l_u8Return = 0;
			return l_u8Return;
		}
	}
	return l_u8Return;
}
void   FAskAxleLoadData(uint8* pBuf)
{
	uint8 l_u8AskType;

	 // 解析35包
     l_u8AskType = pBuf[17];       // 应答类型
     if( l_u8AskType==0x01 )    // 实时过车应答
     {
	     g_u32RecOrd39Num = pBuf[18] + (pBuf[19]<<8) + (pBuf[20]<<16) + (pBuf[21]<<24);
		 g_u8JD39OrderRev = 1;     
     }
     else 
     {
//         g_u8JD6FOrderRev = 1;
		 switch(l_u8AskType)
	     {
	         case 0x02:
		               g_u16RecOrd61Num  = pBuf[22] + (pBuf[23]<<8);
					   g_u8JD6FOrderRev = 1;
				       break;
	    	 case 0x03:
		          	   g_u16RecOrd62Num  = pBuf[22] + (pBuf[23]<<8);
					   g_u8JD6FOrderRev = 1;
				       break;
	    	 case 0x04:
		               g_u16RecOrd68Num  = pBuf[22] + (pBuf[23]<<8);
					   g_u8JD6FOrderRev = 1;
				       break;
			   default: 
		               break;					
	     }
//		 g_u8JD6FOrderRev = 1;
     }
}
void  FReSend39data(void)
{
	uint8   i;
	uint8   l_u8Err = 0;
	uint8   l_u8Len;
	uint8   l_u8ret;
	uint8   l_u8FeedBack;
//	uint8   l_u8LoadFlag;
	uint8   l_u8ReadSDBuf[512];
	uint8   l_u8Buf[512];
	uint8   l_u8SendErrBuf[512];
	uint8   l_u8Errflag = 0;
	uint16  l_u16AxleDataLen;
	uint16  l_u16AxleErrLen; 
	uint32  l_u32CarNumSend;
	uint32  l_u32Raddress;
	uint16  l_u16Rec39Cnt = 0;

	l_u8Len = 0;
	// 命令字
	l_u8Buf[l_u8Len++] = 0x39; 
	// 站点编号
	for(i=0;i<15;i++)
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8StationID[i];	
	}
	// 设备身份识别码
	for(i=0;i<16;i++)					 
	{
		l_u8Buf[l_u8Len++] = JDSYSPARAM.m_au8DeviceID[i];	
	}
	// 设备硬件错误代码
	l_u8Buf[l_u8Len++] = 0x00;

	// 得到SD卡扇区首地址
	l_u32Raddress = g_u32JD39BaseAdr + g_u32Resend39;
	// 读取要发送的单车数据
	l_u8ret = SD_ReadBlock(&sd_info,l_u32Raddress,l_u8ReadSDBuf);
	if( l_u8ret !=0 )
	{
		l_u8Err = 101;
//		SD_Err_Reset_cnt++;
	}
//	else
//	{
//	    SD_Err_Reset_cnt = 0;
//	}

	for(i=0;i<50;i++)
	{
	   l_u8Buf[l_u8Len+i] =	l_u8ReadSDBuf[i];
	}
	// 读取温度值，在硬件错误代码中体现 
	l_u8Buf[32]   = l_u8ReadSDBuf[47];
	// 读取数据包长度
	l_u16AxleDataLen = l_u8ReadSDBuf[49] + l_u8Len;
	// 读取该车是否超限标志
//	l_u8LoadFlag    = l_u8ReadSDBuf[48]; 
	// 记录要发送车辆数据包的序号
	l_u32CarNumSend = l_u8Buf[33] + (l_u8Buf[34]<<8) + (l_u8Buf[35]<<16) + (l_u8Buf[36]<<24);	   
	
	if( (l_u32CarNumSend != g_u32Resend39)||(l_u16AxleDataLen==0)||(l_u16AxleDataLen>512) )
	{
	    l_u8Errflag = 1;     // 数据异常
		if( l_u8Err == 0 )
		{
		   l_u8Err = 100;
		}
	}

	if( l_u8Errflag == 1 )
	{
		BeepON();
		OSTimeDly(10);
		BeepOFF();	
		OSTimeDly(10);

		l_u16AxleErrLen = FGet39OrderForErr(l_u8SendErrBuf, g_u32Resend39, l_u8Err);
		// 发送39数据包
	    l_u8FeedBack = FSendData(l_u8SendErrBuf,l_u16AxleErrLen);
	}
	else
	{
		// 发送39数据包
	    l_u8FeedBack = FSendData(l_u8Buf,l_u16AxleDataLen);	
	}
	
	if( l_u8FeedBack == 1 )      // 发送成功
	{		 
		while(g_u8JD39OrderRev == 0 && l_u16Rec39Cnt < 40)
		{
			OSTimeDly(10);
			l_u16Rec39Cnt ++;
		}
		if(g_u8JD39OrderRev == 1)
		{
			g_u8JD39OrderRev = 0;
			if(g_u32Resend39 == g_u32RecOrd39Num )
			{
			  	g_u32RecOrd39Num = 0;
				g_u32Resend39 = 0;
			}
			return;
		}
		else
		{
			g_u8JD39OrderRev = 0;
			return;
		}	
	}
	else
	{
	}

	return;
}

