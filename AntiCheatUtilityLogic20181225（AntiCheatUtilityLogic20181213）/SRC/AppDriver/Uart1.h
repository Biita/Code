/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart1.h
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		Uart1初始化、中断处理
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__UART1_H
#define	__UART1_H

#include "common.h"
#ifdef	__UART1_C
#define	U1_EXT
#else
#define	U1_EXT	extern
#endif


//初始化
U1_EXT	void	UART1Init(int p_iBaudRate);				
/*
#define		UBR_9600	9600		//串口波特率
#define		UBR_19200	19200		//串口波特率
#define		UBR_57600	57600		//串口波特率
#define		UBR_115200	115200		//串口波特率
#define		UBR_230400	230400		//串口波特率
#define		UBR_460800	460800		//串口波特率
*/
U1_EXT	void	U1SendBytes(uint8 * p_u8SendBuf, uint32 p_u32Len);	//发送数据

#endif		//__UART2_H
