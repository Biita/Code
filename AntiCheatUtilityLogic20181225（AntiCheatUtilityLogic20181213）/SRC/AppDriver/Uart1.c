/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart1.c
** Last modified Date:  20130724
** Last Version:		1.0
** Descriptions:		串口1相关函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:			XZ
** Created date:		20130724
** Version:				1.0
** Descriptions:		Uart1
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
//#define	__UART1_C
#include "UART1.h"

volatile  uint32         Rcv_Len = 0;
volatile  uint32         Rcv_N   = 0;                                   /* 记录接收数据完毕后，需发送数 */
                                                                        /* 据的个数                     */
uint8     SendBuf[8] =   {1,2,3,4,5,6,7,8};  
uint8     U1_Rcv_Buf[64];                                                  /* 字符接收数组                 */
void IRQ_UART1(void);		//中断处理

int sendchar(int ch)
{  
	uint8 Temp;
	Temp = (uint8)ch;
    U1SendBytes(&Temp, 1);
	return ch;   
} 

/*********************************************************************************************************
** 函数名称 ：UART1_SendBuf
** 函数功能 ：向串口发送数据
** 入口参数 ：uint32  Rcv_N：  发送数据的个数。
**            uint8* RcvBufPt：缓存地址  
** 出口参数 ：无
**********************************************************************************************************/
void U1SendBytes (uint8 *RcvBufPt, uint32 Snd_Len)
{
    uint32 i = 0;
    for (i = 0; i < Snd_Len; i++)
	{                                     /* 使用发送FIFO发送数据          */
		while((HSU1_LEVEL>>8) == 64);		//判断当前是否发送完毕
        HSU1_TX = RcvBufPt[i];
    }
    Snd_Len = 0;
}
 
/*********************************************************************************************************
** 函数名称 ：IRQ_UART1
** 函数功能 ：串口1接收中断服务程序
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void IRQ_UART1 (void)
{
    volatile static uint8 i = 0;
    
    /*             
     *  UART1接收中断处理             
     */
   i = HSU1_IIR & 0x06;
   i = 0;
    if ((HSU1_IIR & 0x06) == 0x02)
     {                                    /* 接收FIFO高于触发深度中断     */ 
    
        Rcv_N += (uint32)(HSU1_LEVEL & 0xff) - 1;                       /* 留下一个字节以产生超时中断,未*/
                                                                        /* 保存的数据在中断中处理       */          
        for (; i < Rcv_N; i++)
		{            
            U1_Rcv_Buf[i] = (uint8)(HSU1_RX & 0xff);                       /* 接收数据存入接收缓冲区       */
        }
    } 
	else
	{                                                            /* 接收器超时中断               */
        Rcv_N += (uint32)(HSU1_LEVEL & 0xff);
        Rcv_Len = Rcv_N;                                               /* 保存接收到的数据字节长度     */
       
        for (; i < Rcv_N; i++)
		{  
            U1_Rcv_Buf[i] = (uint8)(HSU1_RX & 0xff);                       /* 处理未保存完的数据           */
        }
        i = 0;
        Rcv_N = 0;                                                      /* 清零接收长度                 */
    }

}

/*********************************************************************************************************
** 函数名称 ：UART1_Init
** 函数功能 ：令串口1完成全部初始化工作,Baund=Fpclk/[14*(HSU1_RATE+1)]
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void UART1Init(int p_iBaudRate)
{
	/* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
	MIC_ER       |= 0xC0000003; 
    HSU1_IIR     |= 0xFF;

	switch (p_iBaudRate)
	{	
		case UBR_4800:
			HSU1_RATE	= 192;
			break;
		
		case UBR_9600:
			HSU1_RATE	= 96;
			break;
		
		case UBR_19200:
			HSU1_RATE	= 47;
			break;
	
		case UBR_38400:
			HSU1_RATE	= 23;
			break;
	
		case UBR_57600:
			HSU1_RATE	= 15;
			break;
	
		case UBR_115200:
			HSU1_RATE	= 7;
			break;
	
		case UBR_230400:
			HSU1_RATE	= 3;
			break;
	
		case UBR_460800:
			HSU1_RATE	= 1;
			break;
	
		default:
			HSU1_RATE	= 7;
			break;
  	}
	
	/*
    *  使能接收中断，设置时钟偏移量和收发触发深度
    */
    HSU1_CTRL    = 0x0001284B;       //1E   20                                   
    
   /* 
    *  初始化UART VIC 中断接口，上升沿触发 
    */
    micIrqFuncSet(26, 1, (unsigned int)IRQ_UART1);  
}
