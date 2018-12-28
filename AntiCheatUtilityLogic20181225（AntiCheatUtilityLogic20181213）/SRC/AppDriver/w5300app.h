/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			W5100App.h
** Last modified Date:  20110622
** Last Version:		1.0
** Descriptions:		5100Ӧ��
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__W5300APP_H
#define	__W5300APP_H

#include "config.h"	
#include "W5300.h"

#ifdef	__W5300APP_C
#define	W5A_EXT
#include "Common.h"

#else
#define	W5A_EXT	extern
#endif

W5A_EXT	void	SetNetSetting(NetInfo p_sniLocal);		//��������������ã��˿ںţ�IP��
W5A_EXT	void	InitNetSetting(NetInfo *p_sniLocal);					//��ʼ�������������
W5A_EXT	void 	ReadTargetSocktInfo(SOCKET s,TargetIPInfo_Struct *pData);
W5A_EXT	void	InitializeW5300(NetInfo *p_sniLocal);	//��ʼ��5300оƬ
//W5A_EXT	void	SetSocket(void);
W5A_EXT	void	IRQ_W5300(void);						//�����жϴ���
//W5A_EXT	uint32	SendDataNet(uint8 * p_u8SendBuf, uint32 p_u32Len);		//������������
W5A_EXT	uint32	UdpSendData(SOCKET p_s, uint8 * p_u8SendBuf, uint32 p_u32Len);		//������������
W5A_EXT	uint32  TcpSendData(SOCKET p_s, uint8 * p_u8SendBuf, uint32 p_u32Len);
W5A_EXT uint32  UdpRecvData(SOCKET p_s);
W5A_EXT uint32  TcpRecvData(SOCKET p_s);


#endif	// __W5300APP_H
