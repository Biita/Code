/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart2.c
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		串口2相关函数，接收轮轴数据
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110518
** Version:				1.0
** Descriptions:		Uart2
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__UART2_C
#include "UART2.h"

//#define OFFSET		17
//static	uint8		m_au8Data[BUFSIZE] = {0};
//static	uint8	m_u8UART5RcvBuf[UARTRCVBUFSIZE]={0};			//串口5接收缓存
static	uint16	m_u16UART5SavIndex = 0;							//串口5保存位置
//static	uint16	m_u16UART5ReadIndex = 0;						//串口5读取位置
static	uint8	m_u8EndFlag = 0;		//帧结束标志
static  uint8   m_u8BufForSt[2]={0};

void IRQ_UART2(void);		//中断处理

/*********************************************************************************************************
** Function name:		U2SendBytes
** Descriptions:		向串口2发送字节数据
** input parameters:	p_u8SendBuf		发送数据指针 
** 						p_u32Len		发送数据长度
**
** Created by:			ZHANG Ye		  
** Created Date:		20110517	  
**-------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:		  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void U2SendBytes(uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint32	l_u32Tmp;

	//等待发送寄存器将当前数据发送完毕
	for (l_u32Tmp = 0; l_u32Tmp < p_u32Len; l_u32Tmp ++)
	{
		while((HSU2_LEVEL>>8) == 64);		//判断当前是否发送完毕
		HSU2_TX = *(p_u8SendBuf+l_u32Tmp);
	}
}

/*********************************************************************************************************
** Function name:     IRQ_UART2
** Descriptions:      串口2接收中断服务程序
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
void IRQ_UART2(void)
{ 	
	
	uint32 i = 0;
	uint8 l_u8TempData = 0;
	 
//	OSIntEnter();
	i = HSU2_IIR;
	i = HSU2_IIR&0x06;
	if(i)
	{
		switch(i)
		{
			case 0x02:
			case 0x04:
			case 0x06:
				if(i == 0x04)
					m_u8EndFlag = 1;
				if(i == 0x02)
				{
					i = (uint32)(HSU2_LEVEL & 0xff)-1;
				}
				else
					i = (uint32)(HSU2_LEVEL & 0xff);
				for (;i>0;i--) 
				{
					l_u8TempData = (uint8)(HSU2_RX & 0xFF);
					if(l_u8TempData == 0xFF && m_u8BufForSt[0] != 0xFF && m_u8BufForSt[0] != 0xAA)
					{
						m_u8BufForSt[0]=l_u8TempData;
					}
					else if(l_u8TempData == 0xAA && m_u8BufForSt[0] != 0xAA && m_u8BufForSt[0] != 0xFF)
					{
						m_u8BufForSt[0]=l_u8TempData;
					}
					else if(l_u8TempData == 0xFF && m_u8BufForSt[0] == 0xFF
					&& g_u8UART5RcvBuf[g_u16UART5SavIndex][0]!=0xAA && g_u8UART5RcvBuf[g_u16UART5SavIndex][1]!=0xAA)
					{
//						m_u16UART5SavIndex = 0;
//						g_u16UART5SavIndex ++;
//						g_u16UART5SavIndex %= U5DATALEN;
						m_u8BufForSt[1]=l_u8TempData;
						m_u16UART5SavIndex = 2;
						g_u8UART5RcvBuf[g_u16UART5SavIndex][0] = 0xFF;
						g_u8UART5RcvBuf[g_u16UART5SavIndex][1] = 0xFF;
						m_u8BufForSt[0]=0;
						m_u8BufForSt[1]=0;
					}
					else if(l_u8TempData == 0xAA && m_u8BufForSt[0] == 0xAA
					&& g_u8UART5RcvBuf[g_u16UART5SavIndex][0]!=0xFF && g_u8UART5RcvBuf[g_u16UART5SavIndex][1]!=0xFF
					&& ((m_u16UART5SavIndex < 2) || (m_u16UART5SavIndex >1 && g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex-1]==0xEE && g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex-2]==0xEE)))
					{
						m_u8BufForSt[1]=l_u8TempData;
						m_u16UART5SavIndex = 2;
						g_u8UART5RcvBuf[g_u16UART5SavIndex][0] = 0xAA;
						g_u8UART5RcvBuf[g_u16UART5SavIndex][1] = 0xAA;
						m_u8BufForSt[0]=0;
						m_u8BufForSt[1]=0;
					}
					else
					{
						if(m_u8BufForSt[0] == 0xFF)
						{
							g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] = 0xFF;
							m_u16UART5SavIndex %= U5ONEDATALEN;
						}
						if(m_u8BufForSt[0] == 0xAA)
						{
							g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] = 0xAA;
							m_u16UART5SavIndex %= U5ONEDATALEN;
						}
						m_u8BufForSt[0]=0;
						m_u8BufForSt[1]=0;
						g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] = l_u8TempData;
						m_u16UART5SavIndex %= U5ONEDATALEN;	
					}				
				}
				if(m_u8EndFlag == 1)
				{
					m_u8EndFlag = 0;
					if(m_u8BufForSt[0] == 0xFF && m_u8BufForSt[1] == 0x00)
					{
						g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] = m_u8BufForSt[0];
						m_u16UART5SavIndex %= U5ONEDATALEN;
					}
					else if(m_u8BufForSt[0] == 0xAA && m_u8BufForSt[1] == 0x00)
					{
						g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] = m_u8BufForSt[0];
						m_u16UART5SavIndex %= U5ONEDATALEN;
					}
					m_u8BufForSt[0] = 0x00;
					m_u16UART5SavIndex = 0;
					g_u16UART5SavIndex ++;
					g_u16UART5SavIndex %= U5DATALEN;
					g_u8UART5RcvBuf[g_u16UART5SavIndex][0] = 0x00;
					g_u8UART5RcvBuf[g_u16UART5SavIndex][1] = 0x00;
				}
//				m_u16UART5SavIndex = 0;
				break;
					                                                    
	        default:
		        break;
		}		
	}
//	HSU2_IIR     |= 0xFF;       
 // 	OSIntExit();
}

/*********************************************************************************************************
** Function name:     UART5Init
** Descriptions:      令串口5完成全部初始化工作,Baund=Fpcld/[16*(DLM:DLL)]*X/Y
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
void UART2Init(int p_iBaudRate)
{
	/* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
	MIC_ER       |= 0xC0000003; 
    HSU2_IIR     |= 0xFF;

	switch (p_iBaudRate)
	{	
		case UBR_4800:
			HSU2_RATE	= 192;
			break;
		
		case UBR_9600:
			HSU2_RATE	= 96;
			break;
		
		case UBR_19200:
			HSU2_RATE	= 47;
			break;
	
		case UBR_38400:
			HSU2_RATE	= 23;
			break;
	
		case UBR_57600:
			HSU2_RATE	= 15;
			break;
	
		case UBR_115200:
			HSU2_RATE	= 7;
			break;
	
		case UBR_230400:
			HSU2_RATE	= 3;
			break;
	
		case UBR_460800:
			HSU2_RATE	= 1;
			break;
	
		default:
			HSU2_RATE	= 7;
			break;
  	}
	
	/*
    *  使能接收中断，设置时钟偏移量和收发触发深度
    */
    HSU2_CTRL    = 0x00012A4B;       //1E   20                                   
    
   	m_u16UART5SavIndex		= 0;
   /* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
    micIrqFuncSet(25, 1, (unsigned int)IRQ_UART2);  
} 
 
