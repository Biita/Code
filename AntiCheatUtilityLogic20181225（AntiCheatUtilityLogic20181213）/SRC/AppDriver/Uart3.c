/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart3.c
** Last modified Date:  2011-04-12
** Last Version:		1.0
** Descriptions:		����3��غ������������ֽ��ߺ�
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
uint8   g_u8DataLen = 0;			   // ��¼��������ַ�����
uint8	m_au8Data[BUFSIZE] = {0};



//static	uint8	m_u8UART3RcvBuf[UARTRCVBUFSIZE];			//����3���ջ���
static	uint16	m_u16UART3SavIndex = 0;							//����3����λ��
//static	uint16	m_u16UART3ReadIndex = 0;						//����3��ȡλ��
static	uint16	m_u16UART5SavIndex = 0;							//����5����λ��


void IRQ_UART3(void);		//�жϴ���

/*********************************************************************************************************
** Function name:		U3SendBytes
** Descriptions:		�򴮿�3����һ���ֽ�����
** input parameters:	p_u8SendBuf		��������ָ�� 
** 						p_u32Len		�������ݳ���
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

	//�ȴ����ͼĴ�������ǰ���ݷ������
	for (l_u32Tmp = 0; l_u32Tmp < p_u32Len; l_u32Tmp ++)
	{
		while ((U3LSR & 0x40) == 0);	
		
		U3THR = *(p_u8SendBuf+l_u32Tmp);
	}
}

/*********************************************************************************************************
** Function name:     IRQ_Uart3
** Descriptions:      ����3�����жϷ������
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
	 *���ڽ��ռ���Ĵ�������
	*/
	uint32 i = 0;
	uint8 l_u8TempData = 0;
//	uint8 l_u8FindHeadCnt = 0;
//	uint8 l_u8DataValid = 0;//���յ���������Ч�Ա�ʶ 0-��Ч��1-��Ч

	//
//	uint8	l_u8Err;
//	uint16	l_u16SendLen;
	uint32 l_u32Len;
//	uint8  l_u8Buf[256];
//	uint8  l_u8Ind;
	uint8  l_u8tempi;
	static uint16  l_u16LasaerInd = 0;
	static uint8   l_u8LaserFlag = 2;//1��ʾ��AA 2��ʾ��EE

///////

	uint8 	l_u8TempIndex = 0; 
	uint32 l_u32LaserTime = 0;
	 
   	if(PROMODPARAM.m_u8TransmitSysEn == 1)
	{
		i = (uint32)(U3IIR&0x0f);
	   	if (i!= 0x01) 
		{		                  					/* �������жϴ�����             */
			switch (i)
			{		
			    case 0x04:		
			    case 0x0c:		
					i	= (uint32)U3RXLEV;				/* ���ж��ﱣ����i����Ч����*/
					
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
	*  UART3�����жϴ��� 			
	*/
	i = (uint32)(U3IIR&0x0f);                                  
	if (i!= 0x01) 
	{		                  					/* �������жϴ�����             */
		switch (i)
		{
		    case 0x04:		
		    case 0x0c:
				i	= (uint32)U3RXLEV;	// ���ж��ﱣ����i����Ч����
				l_u32Len = i;
//				l_u8Ind = 0;
				m_u16UART3SavIndex = 0;
				for (;i>0;i--) 
				{
					l_u8TempData = (uint8)U3RBR;
//					l_u8Buf[l_u8Ind++] = l_u8TempData;
					m_au8Data[m_u16UART3SavIndex++]	= l_u8TempData;
//					if( 0xAA == l_u8TempData && 0 == m_u16UART3SavIndex)//�ҵ���ͷ��һ��0xFF
//					//if( 0xAA == l_u8TempData )
//					{
//						m_u16UART3SavIndex = 1;
//						m_au8Data[0]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}							
//					else if(0xAA == l_u8TempData && 1 == m_u16UART3SavIndex)//�ҵ���ͷ�ڶ���0xFF
//					{
//						m_u16UART3SavIndex = 2;
//						m_au8Data[1]	= 0xAA;
//						l_u8DataValid = 0;
//						continue;
//					}
//					else if(0xAA != l_u8TempData && 0 == m_u16UART3SavIndex)//û���ҵ���ͷ��һ��0xFF
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
//					else if(0xAA != l_u8TempData && 1 == m_u16UART3SavIndex)//û���ҵ���ͷ�ڶ���0xFF
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
//					//��ͷ����&&��������&&У���ֽ���ȷ
//					if(0 == l_u8DataValid && m_u16UART3SavIndex == (*(m_au8Data+4)))
//					{
//						if(0 != CheckCrc(m_au8Data, *(m_au8Data+4)-2))
//						{			
////					        g_u8DataLen = m_u16UART3SavIndex; 
////							g_psemLaserInteract->OSEventCnt = 0;
////							OSSemPost(g_psemLaserInteract);		//��������˳��Ӧ��û�й�ϵ����Ϊ�����˳��ж�
//				
//							//����
//#if TEST_CAPTURETIME_EN > 0
//							// 0xCA �� ��֡04
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
//									// ˵���ȴ���IO�����ϴ��ڣ����ܣ�ֱ������
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
//							g_u8DeviceERR[0] |= 0x0F;//���¼���״̬��1Ϊ�й���
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
//							|| ( m_u16UART3SavIndex>(*(m_au8Data+4)) && m_u16UART3SavIndex>=5 ) )//û�м�⵽֡ͷ
//					//else if( 1 == l_u8DataValid )//û�м�⵽֡ͷ
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
** Descriptions:      ���3���ȫ����ʼ������,Baund=Fpcld/[16*(DLM:DLL)]*X/Y
** input parameters:  p_iBaudRate	������        
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
    *  ���ò�����
    */
	U3LCR		= 0x83;					/* ʹ�ܲ����ʷ�Ƶ������ķ���   */
	U3DLM		= UARTDLM;				/* ����Ƶ����ֵ�����︳ֵΪ1    */ 
	U3DLL		= UARTDLL; 
	//U3LCR		= 0x3B;					//ǿ����żУ��Ϊ0
	U3LCR        = 0x03;                                                /* ʹ��THR,RBR,IER�Ĵ���        */	  //��

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
    *  ʹ��UART3�жϣ�����������ʱ��
    */	 
//	U3FCR  	     = 0x00;				//��ֹFIFO
	U3FCR  	     = 0x3f;                   					            /* ʹ��FIFO��������FIFO������� */ 	 //��
	U3IER        = 0x01;				/* ʹ��RDA                      */
	UART_CLKMODE |= 0x0020;				/* ѡ��UART3��ʱ��ģʽ          */
	
	//U3IER        = 0x01;		 					                    /* ʹ��RDA                      */
	//UART_CLKMODE |= 0x0200;                                              /* ѡ��UART5��ʱ��ģʽ          */ 
	
	m_u16UART3SavIndex		= 0;
	//m_u16UART3ReadIndex		= 0;
	 
	/* 
    *  ��ʼ��UART VIC �жϽӿڣ������ش��� 
    */
	micIrqFuncSet(7, 1, (unsigned int)IRQ_Uart3 );             
	
}
