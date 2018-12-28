/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart2.c
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		����2��غ�����������������
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
//static	uint8	m_u8UART5RcvBuf[UARTRCVBUFSIZE]={0};			//����5���ջ���
static	uint16	m_u16UART5SavIndex = 0;							//����5����λ��
//static	uint16	m_u16UART5ReadIndex = 0;						//����5��ȡλ��
static	uint8	m_u8EndFlag = 0;		//֡������־
static  uint8   m_u8BufForSt[2]={0};

void IRQ_UART2(void);		//�жϴ���

/*********************************************************************************************************
** Function name:		U2SendBytes
** Descriptions:		�򴮿�2�����ֽ�����
** input parameters:	p_u8SendBuf		��������ָ�� 
** 						p_u32Len		�������ݳ���
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

	//�ȴ����ͼĴ�������ǰ���ݷ������
	for (l_u32Tmp = 0; l_u32Tmp < p_u32Len; l_u32Tmp ++)
	{
		while((HSU2_LEVEL>>8) == 64);		//�жϵ�ǰ�Ƿ������
		HSU2_TX = *(p_u8SendBuf+l_u32Tmp);
	}
}

/*********************************************************************************************************
** Function name:     IRQ_UART2
** Descriptions:      ����2�����жϷ������
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
** Descriptions:      ���5���ȫ����ʼ������,Baund=Fpcld/[16*(DLM:DLL)]*X/Y
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
    *  ��ʼ��UART VIC �жϽӿڣ������ش��� 
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
    *  ʹ�ܽ����жϣ�����ʱ��ƫ�������շ��������
    */
    HSU2_CTRL    = 0x00012A4B;       //1E   20                                   
    
   	m_u16UART5SavIndex		= 0;
   /* 
    *  ��ʼ��UART VIC �жϽӿڣ������ش��� 
    */
    micIrqFuncSet(25, 1, (unsigned int)IRQ_UART2);  
} 
 
