/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart5.c
** Last modified Date:  2011-04-12
** Last Version:		1.0
** Descriptions:		串口5相关函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		2011-04-12
** Version:				1.0
** Descriptions:		Uart5
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__UART5_C
#include "UART5.h"

#include "FPGA_APP.h"

//uint8	g_au8Data[BUFSIZE] = {0};
//uint8   g_u8DataLen = 0;			   // 记录激光接收字符个数
//uint8	m_au8Data[BUFSIZE] = {0};



static	uint8	m_u8UART5RcvBuf[UARTRCVBUFSIZE]={0};			//串口5接收缓存
static	uint16	m_u16UART5SavIndex = 0;							//串口5保存位置
static	uint16	m_u16UART5ReadIndex = 0;						//串口5读取位置
static	uint8	m_u8EndFlag = 0;		//帧结束标志
static  uint8   m_u8BufForSt[2]={0};
#define		BUFDATANUM		((m_u16UART5SavIndex + UARTRCVBUFSIZE - m_u16UART5ReadIndex) % UARTRCVBUFSIZE)

void IRQ_UART5(void);		//中断处理

/*********************************************************************************************************
** Function name:		U5SendBytes
** Descriptions:		向串口5发送一个字节数据
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
void U5SendBytes(uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint32	l_u32Tmp;

	for (l_u32Tmp = 0; l_u32Tmp < p_u32Len; l_u32Tmp ++)
	{
		//等待发送寄存器将当前数据发送完毕
		while ((U5LSR & 0x40) == 0);	
		
		U5THR = *(p_u8SendBuf+l_u32Tmp);
	}
}

/*********************************************************************************************************
** Function name:		U5ReciveByte
** Descriptions:		向串口5发送一个字节数据
** input parameters:	*p_pu8RcvDataBuf	接收到的指针					 
** output parameters:	p_u16RcvNum			接收数量(该值为0时，将所有数据传出)
** Return Value:		实际接收数量：
**						当该值小于需要接收数量时，并不接收数据，而是返回Buf中有效字节的数量
**
** Created by:			ZHANG Ye		  
** Created Date:		20110331	  
**-------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Ye	
** Modified date:		20110518	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint16 U5ReciveByte(uint8 *p_pu8RcvDataBuf, uint8 p_u16RcvNum)
{
	uint16	u16Tmp;
	uint16	u16Tmp2;
	u16Tmp	= BUFDATANUM;		//可接收数量
	if ((u16Tmp >= p_u16RcvNum) || (p_u16RcvNum == 0))		//可接收数量满足要求
	{
		if (p_u16RcvNum > 0)
			u16Tmp	= p_u16RcvNum;
		
		for(u16Tmp2 = 0; u16Tmp2 < u16Tmp; u16Tmp2++)
		{
			*(p_pu8RcvDataBuf + u16Tmp2) = m_u8UART5RcvBuf[m_u16UART5ReadIndex++];
			m_u16UART5ReadIndex	%= UARTRCVBUFSIZE;	
		}
	}
	return	u16Tmp;
}

/*********************************************************************************************************
** Function name:     IRQ_UART5
** Descriptions:      串口5接收中断服务程序
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
void IRQ_UART5(void)
{
	/*
	 *用于接收激光的串口数据
	*/
	uint32 i = 0;
	uint8 l_u8TempData = 0;

	i = (uint32)(U5IIR&0x0f);
   	if (i!= 0x01) //若尚有中断待处理
	{		                  					
		switch (i)
		{		
		    case 0x04:		
		    case 0x0c:
				if(i == 0x0c)
					m_u8EndFlag = 1;	
				i	= (uint32)U5RXLEV;	// 在中断里保存了i个有效数据
//				m_u16UART5SavIndex = 0;
				for (;i>0;i--) 
				{
					l_u8TempData = (uint8)(U5RBR & 0xFF);
					if(l_u8TempData == 0xFF && m_u8BufForSt[0] != 0xFF)
					{
						m_u8BufForSt[0]=l_u8TempData;
					}
					else if(l_u8TempData == 0xAA && m_u8BufForSt[0] != 0xAA)
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
					&& g_u8UART5RcvBuf[g_u16UART5SavIndex][0]!=0xFF && g_u8UART5RcvBuf[g_u16UART5SavIndex][1]!=0xFF)
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
						if(g_cJDSysParam.m_u8TransMod == 3)
						{
							if(g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex-2] == 0xEE && g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex-1] == 0xEE)
							{
//								g_u8UART5RcvBuf[g_u16UART5SavIndex][m_u16UART5SavIndex ++ ] =  l_u8TempData;
								m_u16UART5SavIndex = 0;
								g_u16UART5SavIndex ++;
								g_u16UART5SavIndex %= U5DATALEN;
							}
						}	
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
void UART5Init(int p_iBaudRate)
{
	/*  
    *  设置波特率
    */
	U5LCR        = 0x83;                                                /* 使能波特率分频器锁存的访问   */
	U5DLM        = UARTDLM;                                                 /* 给分频器赋值，这里赋值为1    */ 
	U5DLL        = UARTDLL; 
	U5LCR        = 0x03;                                                /* 使能THR,RBR,IER寄存器        */

	switch (p_iBaudRate)
	{	
	case UBR_4800:
		U5CLK        = (1 << 8) | (169 << 0);
		break;
	
	case UBR_9600:
		U5CLK        = (3 << 8) | (254 << 0);
		break;
	
	case UBR_19200:
		U5CLK        = (3 << 8) | (127 << 0);
		break;

	case UBR_38400:
		U5CLK        = (6 << 8) | (127 << 0);
		break;

	case UBR_57600:
		U5CLK        = (9 << 8) | (127 << 0);
		break;

	case UBR_115200:
		U5CLK        = (19 << 8) | (134 << 0);
		break;

	case UBR_230400:
		U5CLK        = (19 << 8) | (67 << 0);
		break;

	case UBR_460800:
		U5CLK        = (38 << 8) | (67 << 0);
		break;

	default:
		U5CLK        = (19 << 8) | (134 << 0);
		break;
  	}
	

	/*
    *  使能UART5中断，开启波特率时钟
    */	 
	U5FCR  	     = 0x3f;                   					            /* 使能FIFO，并设置FIFO触发深度 */
	//U5FCR  	     = 0x00;  
	U5IER        = 0x01;		 					                    /* 使能RDA                      */
	UART_CLKMODE |= 0x0200;                                              /* 选择UART5的时钟模式          */ 
    
	m_u16UART5SavIndex		= 0;
	m_u16UART5ReadIndex		= 0;
	/* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
	micIrqFuncSet(9, 1, (unsigned int)IRQ_UART5 );             
	
}
