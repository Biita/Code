/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart3.c
** Last modified Date:  2011-04-12
** Last Version:		1.0
** Descriptions:		串口3相关函数，用于数字接线盒
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		2011-04-12
** Version:				1.0
** Descriptions:		Uart3
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Ye			
** Modified date:		20110518		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__UART3_C
#include "Uart3.h"

#include "FPGA_APP.h"
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
uint8	g_au8Data[BUFSIZE] = {0};
uint8   g_u8DataLen = 0;			   // 记录激光接收字符个数
uint8	m_au8Data[BUFSIZE] = {0};



//static	uint8	m_u8UART3RcvBuf[UARTRCVBUFSIZE];			//串口3接收缓存
static	uint16	m_u16UART3SavIndex = 0;							//串口3保存位置
//static	uint16	m_u16UART3ReadIndex = 0;						//串口3读取位置
static	uint16	m_u16UART5SavIndex = 0;							//串口5保存位置


void IRQ_UART3(void);		//中断处理

/*********************************************************************************************************
** Function name:		U3SendBytes
** Descriptions:		向串口3发送一个字节数据
** input parameters:	p_u8SendBuf		发送数据指针 
** 						p_u32Len		发送数据长度
**
** Created by:			ZHANG Ye		  
** Created Date:		20110331	  
**-------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Ye	
** Modified date:		20110517  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void U3SendBytes(uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint32	l_u32Tmp;

	//等待发送寄存器将当前数据发送完毕
	for (l_u32Tmp = 0; l_u32Tmp < p_u32Len; l_u32Tmp ++)
	{
		while ((U3LSR & 0x40) == 0);	
		
		U3THR = *(p_u8SendBuf+l_u32Tmp);
	}
}

/*********************************************************************************************************
** Function name:     IRQ_Uart3
** Descriptions:      串口3接收中断服务程序
** input parameters:  none        
** output parameters: none
**
** Created by:		  
** Created Date:	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void IRQ_Uart3(void)
{
	//static int U32_ad[CHANNELNUM];
	//uint32 td;
	//uint32 i = 0;

	/*
	 *用于接收激光的串口数据
	*/
	uint32 i = 0;
	uint8 l_u8TempData = 0;
//	uint8 l_u8FindHeadCnt = 0;
//	uint8 l_u8DataValid = 0;//接收到的数据有效性标识 0-有效，1-无效

	//
//	uint8	l_u8Err;
//	uint16	l_u16SendLen;
	uint32 l_u32Len;
//	uint8  l_u8Buf[256];
//	uint8  l_u8Ind;
	uint8  l_u8tempi;
	static uint16  l_u16LasaerInd = 0;
	static uint8   l_u8LaserFlag = 2;//1表示有AA 2表示有EE

///////

	uint8 	l_u8TempIndex = 0; 
	uint32 l_u32LaserTime = 0;
	 
   	if(PROMODPARAM.m_u8TransmitSysEn == 1)
	{
		i = (uint32)(U3IIR&0x0f);
	   	if (i!= 0x01) 
		{		                  					/* 若尚有中断待处理             */
			switch (i)
			{		
			    case 0x04:		
			    case 0x0c:		
					i	= (uint32)U3RXLEV;				/* 在中断里保存了i个有效数据*/
					
					for (;i>0;i--) 
					{
						l_u8TempData = (uint8)U3RBR;
						
						if( 0xFF == l_u8TempData )
						{
							m_u16UART5SavIndex = 1;
							m_au8Data[0]	= 0xFF;
							//m_u8StartFlag = 1;
							continue;
						}
						m_au8Data[m_u16UART5SavIndex++]	= l_u8TempData;
						m_u16UART5SavIndex %= BUFSIZE;
					}
					break;
						                                                    
		        default:
			        break;
			}
		}
		
		if( 25 == m_u16UART5SavIndex )
		{
			m_u16UART5SavIndex = 0;
			for(i=0;i<24;i=i+3)
			{
				if((0xFF & (*(m_au8Data+1+i)))>0)
				{
					g_RasterData[g_u8RasterDataWriteIndex].u8Cnt=1;					
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16Ltpos = (*(m_au8Data+1+i)) * 10;
					g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u16rtpos = (*(m_au8Data+1+i+1)) * 10;
					
					ReadFPGAGlobalTime(l_u32LaserTime);
					g_RasterData[g_u8RasterDataWriteIndex].u32tm = l_u32LaserTime; 
					
					switch(0xFF & (*(m_au8Data+1+i+2)))
					{
						case 0x01:
							g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type = 0x01<<2;				
							break;
						case 0x02:
							g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type = 0x02<<2;
							break;
						case 0x03:
							g_RasterData[g_u8RasterDataWriteIndex].headinfo[l_u8TempIndex].u8type = 0x03<<2;
							break;
						default:
							break; 			   
					}
					g_u8RasterDataWriteIndex++;
					memset( &g_RasterData[g_u8RasterDataWriteIndex], 0, sizeof(Raster_Struct));
				}	
			}
		}
	
		return ;
	}

	//////


	
	/* 			
	*  UART3接收中断处理 			
	*/
	i = (uint32)(U3IIR&0x0f);                                  
	if (i!= 0x01) 
	{		                  					/* 若尚有中断待处理             */
		switch (i)
		{
		    case 0x04:		
		    case 0x0c:
				i	= (uint32)U3RXLEV;	// 在中断里保存了i个有效数据
				l_u32Len = i;
//				l_u8Ind = 0;
				m_u16UART3SavIndex = 0;
				for (;i>0;i--) 
				{
					l_u8TempData = (uint8)U3RBR;
//					l_u8Buf[l_u8Ind++] = l_u8TempData;
					m_au8Data[m_u16UART3SavIndex++]	= l_u8TempData;
//					if( 0xAA == l_u8TempData && 0 == m_u16UART3SavIndex)//找到包头第一个0xFF
//					//if( 0xAA == l_u8TempData )
//					{
//						m_u16UART3SavIndex = 1;
//						m_au8Data[0]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}							
//					else if(0xAA == l_u8TempData && 1 == m_u16UART3SavIndex)//找到包头第二个0xFF
//					{
//						m_u16UART3SavIndex = 2;
//						m_au8Data[1]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}
//					else if(0xAA != l_u8TempData && 0 == m_u16UART3SavIndex)//没有找到包头第一个0xFF
//					{
//						if(l_u8FindHeadCnt >3)
//						{
//							l_u8DataValid = 1;
////							break;
//						}
//						else
//						{
//						    l_u8FindHeadCnt++;
//							continue;
//						}
//					}
//					else if(0xAA != l_u8TempData && 1 == m_u16UART3SavIndex)//没有找到包头第二个0xFF
//					{
//						m_u16UART3SavIndex = 2;
//						m_au8Data[1]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}
//					else if(0xAA == l_u8TempData && 1 < m_u16UART3SavIndex && m_au8Data[m_u16UART3SavIndex-1] == 0xAA)
//					{
//						m_u16UART3SavIndex = 2;
//						m_au8Data[0]	= 0xAA;
//						m_au8Data[1]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}
//
//					m_au8Data[m_u16UART3SavIndex++]	= l_u8TempData;
//					m_u16UART3SavIndex %= BUFSIZE;
//					
//					//
//					//包头正常&&长度正常&&校验字节正确
//					if(0 == l_u8DataValid && m_u16UART3SavIndex == (*(m_au8Data+4)))
//					{
//						if(0 != CheckCrc(m_au8Data, *(m_au8Data+4)-2))
//						{			
////					        g_u8DataLen = m_u16UART3SavIndex; 
////							g_psemLaserInteract->OSEventCnt = 0;
////							OSSemPost(g_psemLaserInteract);		//互换两句顺序，应该没有关系，因为必须退出中断
//				
//							//测试
//#if TEST_CAPTURETIME_EN > 0
//							// 0xCA 且 首帧04
//							if( 0xCA==(m_au8Data[3]&0xFF) && 0x04==(m_au8Data[7]&0x0C) )
//							{
//								if( 0==g_u8TestIOFlag )
//								{
//									g_u16TestCaptime = 0;
//									g_u8TestSPFlag = 1;
//									//g_u16TestCaptime = (m_au8Data[55]<<8) + m_au8Data[56];
//									T1TCR=0x02;
//									T1TC = 0;
//									T1TCR=0x01;
//									g_u32T1TCBeginTime = T1TC;
//								}
//								else
//								{
//									// 说明先触发IO，后上串口，不管，直接清零
//									g_u8TestIOFlag = 0;
//									g_u8TestSPFlag = 0;
//									g_u16TestCaptime = 0; 
//								}
//								 
//							}
//#endif
//							//
//							memcpy( &g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf, m_au8Data, m_u16UART3SavIndex );
//							g_cRecvLaserData[g_u16RecvLaserInd].m_u16Len = m_u16UART3SavIndex;
//#if LOGIC_NOT_LASER_EN > 0
//							g_u8DeviceERR[0] |= 0x0F;//更新激光状态，1为有故障
//#else
//							OSQPost( g_psQLaserInteract, &g_cRecvLaserData[g_u16RecvLaserInd] );
//#endif
//							g_u16RecvLaserInd++;
//							g_u16RecvLaserInd %= RECVLASERDATA_NUM;
//							//
//						    m_u16UART3SavIndex = 0;
//							g_u32CeshiCnt ++;
//						}
//						else
//						{
//						    //
//#if DEBUGINFO_TASK_SP_EN > 0
//							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u SP Recd CRC-DataValid=%d,SaveIx=%d,DataLen=%d\r\n",
//										           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//											       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//												   l_u8DataValid,
//												   m_u16UART3SavIndex,
//												   (*(m_au8Data+4)) );
//							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//							g_u16CurDebugInd++;
//							g_u16CurDebugInd %= COM_NUM;
//							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//							OSSemPost(g_pSemSendThtouSP);
//#endif
//							//
//							m_u16UART3SavIndex = 0;
//						}
//					}
//					else if(1 == l_u8DataValid 
//							|| ( m_u16UART3SavIndex>(*(m_au8Data+4)) && m_u16UART3SavIndex>=5 ) )//没有检测到帧头
//					//else if( 1 == l_u8DataValid )//没有检测到帧头
//					{
//					    //
//#if DEBUGINFO_TASK_SP_EN > 0
//						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u SP Recd OTH-DataValid=%d,SaveIx=%d,DataLen=%d\r\n",
//									           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
//											   l_u8DataValid,
//											   m_u16UART3SavIndex,
//											   (*(m_au8Data+4)) );
//						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//						g_u16CurDebugInd++;
//						g_u16CurDebugInd %= COM_NUM;
//						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//						OSSemPost(g_pSemSendThtouSP);
//#endif
//						//
//						m_u16UART3SavIndex = 0;
//					}
					//					
				}
				for(l_u8tempi = 0; l_u8tempi<l_u32Len; l_u8tempi++)
				{
					if(l_u8LaserFlag == 1)
					{
						g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf[l_u16LasaerInd] = m_au8Data[l_u8tempi];
						l_u16LasaerInd ++;
					}
					if(g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf[4] == l_u16LasaerInd
					&& l_u8LaserFlag == 1)
					{
						l_u8LaserFlag = 2;
						g_cRecvLaserData[g_u16RecvLaserInd].m_u16Len = l_u16LasaerInd;
						l_u16LasaerInd = 0;
						OSQPost( g_psQLaserInteract, &g_cRecvLaserData[g_u16RecvLaserInd] );
						g_u16RecvLaserInd++;
						g_u16RecvLaserInd %= RECVLASERDATA_NUM;
					}
					if(l_u8tempi>1 && m_au8Data[l_u8tempi] == 0x00 &&m_au8Data[l_u8tempi-2] == 0xAA && m_au8Data[l_u8tempi-1] == 0xAA)
					{
						g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf[0] = 0xAA;
						g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf[1] = 0xAA;
						g_cRecvLaserData[g_u16RecvLaserInd].m_au8DataBuf[2] = 0x00;
						l_u8LaserFlag = 1;
						l_u16LasaerInd = 3;
					}
				}
				break;
				                                             
	        default:
		        break;
		}
	}
}

/*********************************************************************************************************
** Function name:     UART3Init
** Descriptions:      令串口3完成全部初始化工作,Baund=Fpcld/[16*(DLM:DLL)]*X/Y
** input parameters:  p_iBaudRate	波特率        
** output parameters: none
**
** Created by:		  
** Created Date:	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void UART3Init(int p_iBaudRate)
{	
	/*  
    *  设置波特率
    */
	U3LCR		= 0x83;					/* 使能波特率分频器锁存的访问   */
	U3DLM		= UARTDLM;				/* 给分频器赋值，这里赋值为1    */ 
	U3DLL		= UARTDLL; 
	//U3LCR		= 0x3B;					//强制奇偶校验为0
	U3LCR        = 0x03;                                                /* 使能THR,RBR,IER寄存器        */	  //改

	switch (p_iBaudRate)
	{	
	case UBR_4800:
		U3CLK        = (1 << 8) | (169 << 0);
		break;
	
	case UBR_9600:
		U3CLK        = (3 << 8) | (254 << 0);
		break;
	
	case UBR_19200:
		U3CLK        = (3 << 8) | (127 << 0);
		break;

	case UBR_38400:
		U3CLK        = (6 << 8) | (127 << 0);
		break;

	case UBR_57600:
		U3CLK        = (9 << 8) | (127 << 0);
		break;

	case UBR_115200:
		U3CLK        = (19 << 8) | (134 << 0);
		break;

	case UBR_230400:
		U3CLK        = (19 << 8) | (67 << 0);
		break;

	case UBR_460800:
		U3CLK        = (38 << 8) | (67 << 0);
		break;

	default:
		U3CLK        = (19 << 8) | (134 << 0);
		break;
  	}

	/*
    *  使能UART3中断，开启波特率时钟
    */	 
//	U3FCR  	     = 0x00;				//禁止FIFO
	U3FCR  	     = 0x3f;                   					            /* 使能FIFO，并设置FIFO触发深度 */ 	 //改
	U3IER        = 0x01;				/* 使能RDA                      */
	UART_CLKMODE |= 0x0020;				/* 选择UART3的时钟模式          */
	
	//U3IER        = 0x01;		 					                    /* 使能RDA                      */
	//UART_CLKMODE |= 0x0200;                                              /* 选择UART5的时钟模式          */ 
	
	m_u16UART3SavIndex		= 0;
	//m_u16UART3ReadIndex		= 0;
	 
	/* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
	micIrqFuncSet(7, 1, (unsigned int)IRQ_Uart3 );             
	
}
