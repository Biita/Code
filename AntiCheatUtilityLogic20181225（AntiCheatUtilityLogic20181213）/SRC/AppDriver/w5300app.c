/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			W5100App.c
** Last modified Date:  20110622
** Last Version:		1.0
** Descriptions:		5100��������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
//#define	__W5300APP_C

#include "common.h"
static	NetInfo		m_sniLocal;		//����������Ϣ
//static	uint8		m_au8NetBuf[W5100BUFSIZE];



/**********************************************************************
������: InitNetSetting
����: m_sniLocal	�����������
���: ��
����: ��
˵�������������������
**********************************************************************/
void SetNetSetting(NetInfo p_sniLocal)
{
	memcpy(&m_sniLocal,&p_sniLocal,sizeof(m_sniLocal));
}
/**********************************************************************
������: ReadTargetSocktInfo
����:	SOCKET s:	ͨ�ŵ����Ӻ�0~~~3
       
���: pData	��  ���ؽṹ�壬�������ӵ�״̬��Ŀ��IP��ַ��Ŀ��˿ں�
����: ��
˵������ȡ�������ӶԶ˵�IP��Ϣ
**********************************************************************/
void ReadTargetSocktInfo(SOCKET s,TargetIPInfo_Struct *pData)
{ 
 	//��״̬
//	pData->u8State = Read_W5100(W5300_S0_SSR+s<<16);
	pData->u8State = getSn_SSR(s);
	//��IP��ַ
	getSn_DIPR(s, pData->u8CnntIP);
//	pData->u8CnntIP[0] = Read_W5100(W5300_S0_DIPR+s*0x100)>>8;
//	pData->u8CnntIP[1] = Read_W5100(W5300_S0_DIPR+s*0x100)&0xFF;
//	pData->u8CnntIP[2] = Read_W5100(W5300_S0_DIPR+s*0x100)>>8;
//	pData->u8CnntIP[3] = Read_W5100(W5300_S0_DIPR+s*0x100)&0xFF;   
	//���˿ں� ���ܶ�W5300�ļĴ���
	switch(s)
	{
		case 0:
		  	pData->u8CnntPort[0] = S0_DPort[0];
			pData->u8CnntPort[1] = S0_DPort[1];
		break;
		case 1:
		  	pData->u8CnntPort[0] = S1_DPort[0];
			pData->u8CnntPort[1] = S1_DPort[1];
		break;
		case 2:
		  	pData->u8CnntPort[0] = S2_DPort[0];
			pData->u8CnntPort[1] = S2_DPort[1];
		break;
		case 3:
		  	pData->u8CnntPort[0] = S3_DPort[0];
			pData->u8CnntPort[1] = S3_DPort[1];
		break;
		case 4:
		  	pData->u8CnntPort[0] = S4_DPort[0];
			pData->u8CnntPort[1] = S4_DPort[1];
		break;
		case 5:
		  	pData->u8CnntPort[0] = S5_DPort[0];
			pData->u8CnntPort[1] = S5_DPort[1];
		break;
		case 6:
		  	pData->u8CnntPort[0] = S6_DPort[0];
			pData->u8CnntPort[1] = S6_DPort[1];
		break;
		case 7:
		  	pData->u8CnntPort[0] = S7_DPort[0];
			pData->u8CnntPort[1] = S7_DPort[1];
		break;
	}
//  	pData->u8CnntPort[0] = Read_W5100(W5300_S0_DPORT+s*0x100)>>8;
//	pData->u8CnntPort[1] = Read_W5100(W5300_S0_DPORT+s*0x100)&0xFF;

}
/**********************************************************************
������: InitNetSetting
����: m_sniLocal	�����������
���: ��
����: ��
˵������ʼ��������� ���˿ںţ�IP��
**********************************************************************/
void InitNetSetting(NetInfo *p_sniLocal)
{
	unsigned char i;
	
	for(i=0; i<4; i++)
	{
		Gateway_IP[i]	= p_sniLocal->au8GatewayIP[i];		// �������ز���
		Sub_Mask[i]		= p_sniLocal->au8SubMask[i];			// ������������
		IP_Addr[i]		= p_sniLocal->au8IPAddr[i];			// ����IP��ַ  
	}

	/* ���������ַ */
	for(i=0; i<6; i++)
		Phy_Addr[i] = p_sniLocal->au8MACAddr[i];

	//�˿�0
	S0_Port[0] = (p_sniLocal->u32LocalPortNO >> 8) & 0xFF;		//�˿ں�
	S0_Port[1] = p_sniLocal->u32LocalPortNO & 0xFF;
	
	S0_State = 0;
// 	for(i=0; i<4; i++)
// 		S0_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

// 	S0_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
// 	S0_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S0_Mode = TCP_SERVER;							//���ö˿�0Ĭ�ϵĹ�����ʽΪTCP������ģʽ

	//�˿�1
	S1_Port[0] = ((p_sniLocal->u32LocalPortNO+1) >> 8) & 0xFF;		//�˿ں�
	S1_Port[1] = (p_sniLocal->u32LocalPortNO+1) & 0xFF;
	S1_State = 0;
// 	for(i=0; i<4; i++)
// 		S1_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

// 	S1_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
// 	S1_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S1_Mode = TCP_SERVER;							//���ö˿�1Ĭ�ϵĹ�����ʽΪTCP������ģʽ 

	//�˿�2
	S2_Port[0] = ((p_sniLocal->u32LocalPortNO+2)>> 8) & 0xFF;		//�˿ں�
	S2_Port[1] = (p_sniLocal->u32LocalPortNO+2)& 0xFF;
	S2_State = 0;
// 	for(i=0; i<4; i++)
// 		S2_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

// 	S2_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
// 	S2_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

//  	S2_Mode = TCP_CLIENT;						//���ö˿�2Ĭ�ϵĹ�����ʽΪTCP������ 
	S2_Mode = TCP_SERVER;

	//�˿�3
	S3_Port[0] = ((p_sniLocal->u32LocalPortNO+3) >> 8) & 0xFF;		//�˿ں�
	S3_Port[1] = (p_sniLocal->u32LocalPortNO+3) & 0xFF;
	S3_State = 0;
// 	for(i=0; i<4; i++)
// 		S3_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

// 	S3_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
// 	S3_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S3_Mode = TCP_SERVER;						//���ö˿�3Ĭ�ϵĹ�����ʽΪTCP������ 

//�˿�4
	S4_Port[0] = ((p_sniLocal->u32LocalPortNO +4) >> 8) & 0xFF;		//�˿ں�
	S4_Port[1] = (p_sniLocal->u32LocalPortNO +4) & 0xFF;
	
	S4_State = 0;
// 	for(i=0; i<4; i++)
// 		S0_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

// 	S0_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
// 	S0_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S4_Mode = TCP_SERVER;							//���ö˿�0Ĭ�ϵĹ�����ʽΪTCP������ģʽ
}


// void Socket_Init(SOCKET s)
// {
// 	unsigned char i;

// 	/*���÷�Ƭ���ȣ��ο�W5100�����ֲᣬ��ֵ���Բ��޸�*/
// //	Write_W5100((W5100_S0_MSS+s*0x100),0x05);		/*����Ƭ�ֽ���=1460*/
// //	Write_W5100((W5100_S0_MSS+s*0x100+1),0xb4);

// 	/* Set Socket Port number */
// 	switch(s)
// 	{
// 		case 0:
// 			IINCHIP_WRITE(Sn_PORTR(0),((S0_Port[0]<<8)|S0_Port[1]));	/* Set Local Socket Port number */
// 			

// //			Write_W5100(W5100_S0_DPORT,S0_DPort[0]);	/* Set Destination port number */
// //			Write_W5100(W5100_S0_DPORT+1,S0_DPort[1]);

// //			for(i=0;i<4;i++)
// //				Write_W5100(W5100_S0_DIPR+i,S0_DIP[i]);	/* Set Destination IP Address */
// 			break;
// 		case 1:
// 			IINCHIP_WRITE(Sn_PORTR(1),((S1_Port[0]<<8)|S1_Port[1]));	/* Set Local Socket Port number */

// //			Write_W5100(W5100_S1_DPORT,S1_DPort[0]);	/* Set Destination port number */
// //			Write_W5100(W5100_S1_DPORT+1,S1_DPort[1]);

// //			for(i=0;i<4;i++)
// //				Write_W5100(W5100_S1_DIPR+i,S1_DIP[i]);	/* Set Destination IP Address */
// 			break;
// 		case 2:
// 			IINCHIP_WRITE(Sn_PORTR(2),((S2_Port[0]<<8)|S2_Port[1]));	/* Set Local Socket Port number */

// 			IINCHIP_WRITE(Sn_DPORTR(2),((S2_DPort[0]<<8)|S2_DPort[1]));	/* Set Destination port number */
// 			
// 			setSn_DIPR(2,S2_DIP);
// 			
// 			break;
// 		case 3:
// 			IINCHIP_WRITE(Sn_PORTR(3),((S3_Port[0]<<8)|S3_Port[1]));	/* Set Local Socket Port number */

// //			Write_W5100(W5100_S3_DPORT,S3_DPort[0]);	/* Set Destination port number */
// //			Write_W5100(W5100_S3_DPORT+1,S3_DPort[1]);
// //
// //			for(i=0;i<4;i++)
// //				Write_W5100(W5100_S3_DIPR+i,S3_DIP[i]);	/* Set Destination IP Address */
// 			break;
// 		default:
// 			break;
// 	}
// }



void SetSocket(void)
{
//	uint8 l_u8tempi;
	/* ?? 0 */
	if(S0_State==0)
	{
		if(S0_Mode==TCP_SERVER)			/* TCP????? */
		{
			if(listen(0)==TRUE)
				S0_State=S_INIT;	
			else
				S0_State=0;
			
		}
		else if(S0_Mode==TCP_CLIENT) 	/* TCP????? */
		{
			if(connect(0,S0_DIP,(S0_DPort[0]<<8)|(S0_DPort[1]))==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else							/* UDP?? */
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}

	/* ?? 1 */
	if(S1_State==0)
	{
		if(S1_Mode==TCP_SERVER)			/* TCP????? */
		{
			if(listen(1)==TRUE)
				S1_State=S_INIT;
			else
				S1_State=0;
			
		}
		else if(S1_Mode==TCP_CLIENT)	/* TCP????? */
		{
			if(connect(1,S1_DIP,(S1_DPort[0]<<8)|(S1_DPort[1]))==TRUE)
				S1_State=S_INIT;
			else
				S1_State=0;
		}
		else							/* UDP?? */
		{
			if(Socket_UDP(1)==TRUE)
				S1_State=S_INIT|S_CONN;
			else
				S1_State=0;
		}
	}

	/* ?? 2 */
	if(S2_State==0)
	{
		if(S2_Mode==TCP_SERVER)			/* TCP????? */
		{
			if(listen(2)==TRUE)
				S2_State=S_INIT;
			else
				S2_State=0;
			
			
		}
		else if(S2_Mode==TCP_CLIENT) 	/* TCP????? */
		{
			if(connect(2,S2_DIP,(S2_DPort[0]<<8)|(S2_DPort[1]))==TRUE)
				S2_State=S_INIT;
			else
				S2_State=0;
			
		}
		else							/* UDP?? */
		{
			if(Socket_UDP(2)==TRUE)
			S2_State=S_INIT|S_CONN;
			else
				S2_State=0;
		}
	}

	/* ?? 3 */
	if(S3_State==0)
	{
		if(S3_Mode==TCP_SERVER)			/* TCP????? */
		{
			if(listen(3)==TRUE)
				
				S3_State=S_INIT;
			else
				S3_State=0;
			
		}
		else if(S3_Mode==TCP_CLIENT) 	/* TCP????? */
		{
			if(connect(3,S3_DIP,(S3_DPort[0]<<8)|(S3_DPort[1]))==TRUE)
				S3_State=S_INIT;
			else
				S3_State=0;
		}
		else							/* UDP?? */
		{
			if(Socket_UDP(3)==TRUE)
			{
				S3_State=S_INIT|S_CONN;
					
//				S3_DIP[0]= 192; 
//				S3_DIP[1]= 168;
//				S3_DIP[2]= 3;
//				S3_DIP[3]= 24;
//				S3_DPort[0]=0x13;
//				S3_DPort[1]=0x88;
//				for(l_u8tempi=0;l_u8tempi<4;l_u8tempi++)
//					Write_W5100(W5100_S3_DIPR+l_u8tempi, S3_DIP[l_u8tempi]);
//				Write_W5100(W5100_S3_DPORT, S3_DPort[0]);
//				Write_W5100(W5100_S3_DPORT+1, S3_DPort[1]);
			}
			else
				S3_State=0;
			
		}
	}

	if(S4_State==0)
	{
		if(S4_Mode==TCP_SERVER)			/* TCP????? */
		{
			if(listen(4)==TRUE)
				S4_State=S_INIT;	
			else
				S4_State=0;
			
		}
		else if(S4_Mode==TCP_CLIENT) 	/* TCP????? */
		{
			if(connect(4,S4_DIP,(S4_DPort[0]<<8)|(S4_DPort[1]))==TRUE)
				S4_State=S_INIT;
			else
				S4_State=0;
		}
		else							/* UDP?? */
		{
			if(Socket_UDP(4)==TRUE)
				S4_State=S_INIT|S_CONN;
			else
				S4_State=0;
		}
	}
}
/*****************************************************************
������: InitializeW5300
����: ��
���: ��
����: ��
˵�����ȶ�W5300��ʼ����Ȼ�������أ����ֱ��ʼ��4���˿�
*****************************************************************/


void InitializeW5300(NetInfo *p_sniLocal)
{
	//��������
	P0_MUX_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 ����ΪP0.0 P0.1
	P0_DIR_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 ����Ϊ����

	P2_MUX_CLR |= (1 << 5);						//GPIO_05 ����Ϊ GPIO_05
	P2_DIR_CLR |= (1 << 30 );					//GPIO_05 ����Ϊ����
	
	W5300_Reset();
	
//	SetNetSetting(p_sniLocal);
		
	InitNetSetting(p_sniLocal);

	W5300_Init();


	/* ������ط����� */
//	Detect_Gateway();
// 	Socket_Init(0);
// 	Socket_Init(1);
// 	Socket_Init(2);
// 	Socket_Init(3);
	socket(0, Sn_MR_TCP, ((S0_Port[0]<<8)|S0_Port[1]), 1);
	
	socket(1, Sn_MR_TCP, ((S1_Port[0]<<8)|S1_Port[1]), 1);
	
	socket(2, Sn_MR_TCP, ((S2_Port[0]<<8)|S2_Port[1]), 1);
	socket(3, Sn_MR_TCP, ((S3_Port[0]<<8)|S3_Port[1]), 0);
	socket(4, Sn_MR_TCP, ((S4_Port[0]<<8)|S4_Port[1]), 1);
	
	SetSocket();


	SIC2_ER					|= (1 << 2);
	
	sic2IrqFuncSet(5	, 0	,(unsigned int)IRQ_W5300);      //�ж�����GPIO_02��ΪW5300�ж��ź�  �½��ش����ж�
	
	
}

/*********************************************************************************************************
** Function name:		UdpSendData
** Descriptions:		UDPģʽ��������
** input parameters:	p_u8SendBuf		��������ָ�� 
** 						p_u32Len		�������ݳ���
** output parameters:	��������
** Created by:			WangZheng		  
** Created Date:		20171208	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 UdpSendData(SOCKET p_s, uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint32 l_u32SendSize = 0,ret = 0;
//	memcpy(Tx_Buffer, p_u8SendBuf, p_u32Len);
	switch(p_s)
	{
		case 0:
			l_u32SendSize = sendto(0, p_u8SendBuf, p_u32Len, S0_DIP, (S0_DPort[0]<<8)|S0_DPort[1]);
		break;
		case 1:
			l_u32SendSize = sendto(1, p_u8SendBuf, p_u32Len, S1_DIP, (S1_DPort[0]<<8)|S1_DPort[1]);
		break;
		case 2:
			l_u32SendSize = sendto(2, p_u8SendBuf, p_u32Len, S2_DIP, (S2_DPort[0]<<8)|S2_DPort[1]);
		break;
		case 3:
			l_u32SendSize = sendto(3, p_u8SendBuf, p_u32Len, S3_DIP, (S3_DPort[0]<<8)|S3_DPort[1]);
		break;
		case 4:
			l_u32SendSize = sendto(4, p_u8SendBuf, p_u32Len, S4_DIP, (S4_DPort[0]<<8)|S4_DPort[1]);
		break;
		case 5:
			l_u32SendSize = sendto(5, p_u8SendBuf, p_u32Len, S5_DIP, (S5_DPort[0]<<8)|S5_DPort[1]);
		break;
		case 6:
			l_u32SendSize = sendto(6, p_u8SendBuf, p_u32Len, S6_DIP, (S6_DPort[0]<<8)|S6_DPort[1]);
		break;
		case 7:
			l_u32SendSize = sendto(7, p_u8SendBuf, p_u32Len, S7_DIP, (S7_DPort[0]<<8)|S7_DPort[1]);
		break;
	}
	
	ret ? l_u32SendSize = p_u32Len : 0;	
	return l_u32SendSize;
	
}
/*********************************************************************************************************
** Function name:		TcpSendData
** Descriptions:		TCPģʽ��������
** input parameters:	p_u8SendBuf		��������ָ�� 
** 						p_u32Len		�������ݳ���
** output parameters:	��������
** Created by:			WangZheng		  
** Created Date:		20171208	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 TcpSendData(SOCKET p_s, uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint32 l_u32SendSize = 0,ret = 0;
	ret = send(p_s, p_u8SendBuf, p_u32Len);
	ret ? l_u32SendSize = p_u32Len : 0;	
	return l_u32SendSize;
	
}
/*********************************************************************************************************
** Function name:		UdpRecvData
** Descriptions:		UDPģʽ��������
** input parameters:	p_u8SendBuf		��������ָ�� 
** output parameters:	��������
** Created by:			WangZheng		  
** Created Date:		20171208	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 UdpRecvData(SOCKET p_s)
{	
	uint16 l_u16DataSize = 0;
	switch(p_s)
	{
		case 0:
			if((S0_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(0);    

			l_u16DataSize = recvfrom(0, Rx_Buffer,l_u16DataSize, S0_DIP, (uint16*)((S0_DPort[0]<<8)|S0_DPort[1]));	
			S0_State &=(~Sn_IR_RECV);	
		break;
		case 1:
			if((S1_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(1);    

			l_u16DataSize = recvfrom(1, Rx_Buffer,l_u16DataSize, S1_DIP, (uint16*)((S1_DPort[0]<<8)|S1_DPort[1]));	
			S1_State &=(~Sn_IR_RECV);	
		break;
		case 2:
			if((S2_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(2);    

			l_u16DataSize = recvfrom(2, Rx_Buffer,l_u16DataSize, S2_DIP, (uint16*)((S2_DPort[0]<<8)|S2_DPort[1]));	
			S2_State &=(~Sn_IR_RECV);	
		break;
		case 3:
			if((S3_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(3);    

			l_u16DataSize = recvfrom(3, Rx_Buffer,l_u16DataSize, S3_DIP, (uint16*)((S3_DPort[0]<<8)|S3_DPort[1]));	
			S3_State &=(~Sn_IR_RECV);	
		break;
		case 4:
			if((S4_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(4);    

			l_u16DataSize = recvfrom(4, Rx_Buffer,l_u16DataSize, S4_DIP, (uint16*)((S4_DPort[0]<<8)|S4_DPort[1]));	
			S4_State &=(~Sn_IR_RECV);	
		break;
		case 5:
			if((S5_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(5);    

			l_u16DataSize = recvfrom(5, Rx_Buffer,l_u16DataSize, S5_DIP, (uint16*)((S5_DPort[0]<<8)|S5_DPort[1]));	
			S5_State &=(~Sn_IR_RECV);	
		break;
		case 6:
			if((S6_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(6);    
			
			l_u16DataSize = recvfrom(6, Rx_Buffer,l_u16DataSize, S6_DIP, (uint16*)((S6_DPort[0]<<8)|S6_DPort[1]));	
			S6_State &=(~Sn_IR_RECV);	
		break;
		case 7:
			if((S7_State & Sn_IR_RECV)== Sn_IR_RECV)
				l_u16DataSize=getSn_RX_RSR(7);    

			l_u16DataSize = recvfrom(7, Rx_Buffer,l_u16DataSize, S7_DIP, (uint16*)((S7_DPort[0]<<8)|S7_DPort[1]));	
			S7_State &=(~Sn_IR_RECV);	
		break;
	}

	if (l_u16DataSize > 0)
	{
		if( 0 != CheckCrc( Rx_Buffer, l_u16DataSize-2) )	   
		{
			memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	  
			g_u8NetADIn	++;
			g_u8NetADIn %= NETDATA_BUFMASK;	
			g_u16NetADcount	++;
			g_u16NetADcount %= NETDATA_BUFMASK;
		}
		
	}  
	g_u8NetStatusErr	= 0;

	return l_u16DataSize;

}
/*********************************************************************************************************
** Function name:		TcpRecvData
** Descriptions:		Tcpģʽ��������
** input parameters:	p_u8SendBuf		��������ָ�� 
** output parameters:	��������
** Created by:			WangZheng		  
** Created Date:		20171208	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 TcpRecvData(SOCKET p_s)
{	
	uint16 l_u16DataSize = 0;
	l_u16DataSize=getSn_RX_RSR(p_s);
	if((l_u16DataSize>0)&&(l_u16DataSize<=1460))
	{
		switch(p_s)
		{
			case 0:	
				if((S0_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(0);    						
				l_u16DataSize = recv(0, Rx_Buffer,l_u16DataSize);	
				S0_State &=(~Sn_IR_RECV);					
			break;
			case 1:
				if((S1_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(1); 			  			
				l_u16DataSize = recv(1, Rx_Buffer,l_u16DataSize);	
				S1_State &=(~Sn_IR_RECV);	
			break;
			case 2:
				if((S2_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(2); 
				l_u16DataSize = recv(2, Rx_Buffer,l_u16DataSize);	
				S2_State &=(~Sn_IR_RECV);	
			break;
			case 3:
				if((S3_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(3); 

				l_u16DataSize = recv(3, Rx_Buffer,l_u16DataSize);	
				S3_State &=(~Sn_IR_RECV);	
			break;
			case 4:
				if((S4_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(4); 

				l_u16DataSize = recv(4, Rx_Buffer,l_u16DataSize);	
				S4_State &=(~Sn_IR_RECV);	
			break;
			case 5:
				if((S6_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(5); 

				l_u16DataSize = recv(5, Rx_Buffer,l_u16DataSize);	
				S5_State &=(~Sn_IR_RECV);	
			break;
			case 6:
				if((S6_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(6); 

				l_u16DataSize = recv(6, Rx_Buffer,l_u16DataSize);	
				S6_State &=(~Sn_IR_RECV);	
			break;
			case 7:
				if((S7_State & Sn_IR_RECV)== Sn_IR_RECV)
					l_u16DataSize=getSn_RX_RSR(7); 

				l_u16DataSize = recv(7, Rx_Buffer,l_u16DataSize);	
				S7_State &=(~Sn_IR_RECV);	
			break;
		}

		if (l_u16DataSize > 0)
		{
			if( 0 != CheckCrc( Rx_Buffer, l_u16DataSize-2) )	   
			{
				memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	 
				g_u8NetADIn	++;
				g_u8NetADIn %= NETDATA_BUFMASK;	
				g_u16NetADcount	++;
				g_u16NetADcount %= NETDATA_BUFMASK;
			}
			
		}  
		g_u8NetStatusErr	= 0;

		return l_u16DataSize;	
	}
	else
		return FALSE;

}
//W5300�жϴ���
void IRQ_W5300(void)
{
	
	#define BUFRECSIZE	1000
	
	uint16	l_u16DataSize;
	uint8   l_u8tempi;
	
	
	W5300_Interrupt = 1;
	W5300_Interrupt_Process();				//�жϷ�ʽ

	if((S0_Data & S_RECEIVE) == S_RECEIVE)
	{
		S0_Data	&= ~S_RECEIVE;
		
	//	l_u16DataSize = S_rx_process(0)-8;	  //UDP??,?8????????,????????
		l_u16DataSize=getSn_RX_RSR(0);    						
		l_u16DataSize = recv(0, Rx_Buffer,l_u16DataSize);	
		
		if (l_u16DataSize > 0)
		{
			if( 0 != CheckCrc( Rx_Buffer, l_u16DataSize-2) )	   // ??CRC??,wujinlong 20130911
			{
				memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	   //20121107
				g_u8NetADIn	++;
				g_u8NetADIn %= NETDATA_BUFMASK;	
				g_u16NetADcount	++;
				g_u16NetADcount %= NETDATA_BUFMASK;
			}
			
		}  
//		g_u8DeviceERR		&= 0xef;
		g_u8NetStatusErr	= 0;
	}

  if((S1_Data & S_RECEIVE) == S_RECEIVE)
	{
		S1_Data	&= ~S_RECEIVE;	 		
		l_u16DataSize=getSn_RX_RSR(1);    						
		l_u16DataSize = recv(1, Rx_Buffer,l_u16DataSize);	
		if (l_u16DataSize > 0)
		{
			memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	   //20121107
			g_u8NetADIn	++;
			g_u8NetADIn %= NETDATA_BUFMASK;	
			g_u16NetADcount	++;
			g_u16NetADcount %= NETDATA_BUFMASK;
		}  
		g_u8NetStatusErr	= 0;
	}

	if((S2_Data & S_RECEIVE) == S_RECEIVE)
	{
		S2_Data	&= ~S_RECEIVE;
		

		l_u16DataSize=getSn_RX_RSR(2);    						
		l_u16DataSize = recv(2, Rx_Buffer,l_u16DataSize);	
		if (l_u16DataSize > 0)
		{
			if(Rx_Buffer[3]==0xAC)
			{
				;
			}
			memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	   //20121107
			g_u8NetADIn	++;
			g_u8NetADIn %= NETDATA_BUFMASK;	
			g_u16NetADcount	++;
			g_u16NetADcount %= NETDATA_BUFMASK;
		}  
		g_u8NetStatusErr	= 0;
	}

  if((S3_Data & S_RECEIVE) == S_RECEIVE)
	{
		S3_Data	&= ~S_RECEIVE;
		
		l_u16DataSize=getSn_RX_RSR(3);    						
		l_u16DataSize = recv(3, Rx_Buffer,l_u16DataSize)-8;
		
		for(l_u8tempi=0; l_u8tempi<4; l_u8tempi++)
		{	
			S3_DIP[l_u8tempi]= Rx_Buffer[l_u8tempi]; 
		}
		S3_DPort[0]=Rx_Buffer[4];
		S3_DPort[1]=Rx_Buffer[5];
		setSn_DIPR(3, S3_DIP);
		setSn_DPORTR(3,((S3_DPort[0]<<8)|S3_DPort[1]));
		if (l_u16DataSize > 0)
		{
			memcpy(g_anfSPIAD[g_u8NetADIn], &Rx_Buffer[8], l_u16DataSize);	   //20121107
			g_u8NetADIn	++;
			g_u8NetADIn %= NETDATA_BUFMASK;	
			g_u16NetADcount	++;
			g_u16NetADcount %= NETDATA_BUFMASK;
		}  
		g_u8NetStatusErr	= 0;
	}

//	OSIntExit();
	if((S4_Data & S_RECEIVE) == S_RECEIVE)
	{
		S4_Data	&= ~S_RECEIVE;
		
	//	l_u16DataSize = S_rx_process(0)-8;	  //UDP??,?8????????,????????
		l_u16DataSize=getSn_RX_RSR(4);    						
		l_u16DataSize = recv(4, Rx_Buffer,l_u16DataSize);	
		
		if (l_u16DataSize > 0)
		{
			if( 0 != CheckCrc( Rx_Buffer, l_u16DataSize-2) )	   // ??CRC??,wujinlong 20130911
			{
				memcpy(g_anfSPIAD[g_u8NetADIn], Rx_Buffer, l_u16DataSize);	   //20121107
				g_u8NetADIn	++;
				g_u8NetADIn %= NETDATA_BUFMASK;	
				g_u16NetADcount	++;
				g_u16NetADcount %= NETDATA_BUFMASK;
			}
			
		}  
//		g_u8DeviceERR		&= 0xef;
		g_u8NetStatusErr	= 0;
	}
}

