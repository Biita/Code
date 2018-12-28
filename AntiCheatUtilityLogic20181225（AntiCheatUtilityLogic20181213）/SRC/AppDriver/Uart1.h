/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Uart1.h
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		Uart1��ʼ�����жϴ���
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


//��ʼ��
U1_EXT	void	UART1Init(int p_iBaudRate);				
/*
#define		UBR_9600	9600		//���ڲ�����
#define		UBR_19200	19200		//���ڲ�����
#define		UBR_57600	57600		//���ڲ�����
#define		UBR_115200	115200		//���ڲ�����
#define		UBR_230400	230400		//���ڲ�����
#define		UBR_460800	460800		//���ڲ�����
*/
U1_EXT	void	U1SendBytes(uint8 * p_u8SendBuf, uint32 p_u32Len);	//��������

#endif		//__UART2_H
