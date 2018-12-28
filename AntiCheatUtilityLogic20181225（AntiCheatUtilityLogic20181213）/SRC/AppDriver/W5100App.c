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
#define	__W5100APP_C
#include "W5100App.h"

static	NetInfo		m_sniLocal;		//����������Ϣ
//static	uint8		m_au8NetBuf[W5100BUFSIZE];

#define W5100_Disable 	( P3_OUTP_CLR = ( 1 << 11 ) )			//W5100��ʹ��
#define W5100_Enable	( P3_OUTP_SET = ( 1 << 11 ) )				//W5100ʹ��
#define W5100_Reset()	W5100_Disable;W5100_Disable;W5100_Enable;	//W5100��λ

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
	pData->u8State = Read_W5100(W5100_S0_SSR+s*0x100);
	//��IP��ַ
	pData->u8CnntIP[0] = Read_W5100(W5100_S0_DIPR+s*0x100);
	pData->u8CnntIP[1] = Read_W5100(W5100_S0_DIPR+s*0x100 + 1);
	pData->u8CnntIP[2] = Read_W5100(W5100_S0_DIPR+s*0x100 + 2);
	pData->u8CnntIP[3] = Read_W5100(W5100_S0_DIPR+s*0x100 + 3);   
	//���˿ں�
  	pData->u8CnntPort[0] = Read_W5100(W5100_S0_DPORT+s*0x100);
	pData->u8CnntPort[1] = Read_W5100(W5100_S0_DPORT+s*0x100 + 1);

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
//	for(i=0; i<4; i++)
//		S0_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

//	S0_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
//	S0_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S0_Mode = TCP_SERVER;							//���ö˿�0Ĭ�ϵĹ�����ʽΪTCP������ģʽ

	//�˿�1
	S1_Port[0] = ((p_sniLocal->u32LocalPortNO+1) >> 8) & 0xFF;		//�˿ں�
	S1_Port[1] = (p_sniLocal->u32LocalPortNO+1) & 0xFF;
	S1_State = 0;
//	for(i=0; i<4; i++)
//		S1_DIP[i]		= m_sniLocal.au8ServerIP[i];		// Ŀ��IP��ַ

//	S1_DPort[0] = (m_sniLocal.u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
//	S1_DPort[1] = m_sniLocal.u32ServerPortNO & 0xFF;

	S1_Mode = TCP_SERVER;							//���ö˿�1Ĭ�ϵĹ�����ʽΪTCP������ģʽ 

	//�˿�2
	S2_Port[0] = ((p_sniLocal->u32LocalPortNO+2)>> 8) & 0xFF;		//�˿ں�
	S2_Port[1] = (p_sniLocal->u32LocalPortNO+2)& 0xFF;
	S2_State = 0;
	for(i=0; i<4; i++)
		S2_DIP[i]		= p_sniLocal->au8ServerIP[i];		// Ŀ��IP��ַ

	S2_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
	S2_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S2_Mode = TCP_CLIENT;						//���ö˿�2Ĭ�ϵĹ�����ʽΪTCP������ 

	//�˿�3
	S3_Port[0] = ((p_sniLocal->u32LocalPortNO+3) >> 8) & 0xFF;		//�˿ں�
	S3_Port[1] = (p_sniLocal->u32LocalPortNO+3) & 0xFF;
	S3_State = 0;
//	for(i=0; i<4; i++)
//		S3_DIP[i]		= m_sniLocal.au8ServerIP[i];		// Ŀ��IP��ַ

//	S3_DPort[0] = (m_sniLocal.u32ServerPortNO >> 8) & 0xFF;	 	//Ŀ�Ķ˿ں�
//	S3_DPort[1] = m_sniLocal.u32ServerPortNO & 0xFF;

	S3_Mode = UDP_MODE;						//���ö˿�3Ĭ�ϵĹ�����ʽΪTCP������ 
}

/*****************************************************************
������: SetSocket
����: ��
���: ��
����: ��
˵�����ֱ�����4���˿ڣ����ݶ˿ڹ���ģʽ�����˿�����TCP��������TCP�ͻ���
      ��UDPģʽ��
      �Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
*****************************************************************/
void SetSocket(void)
 {
//	uint8 l_u8tempi;
	/* �˿� 0 */
	if(S0_State==0)
	{
		if(S0_Mode==TCP_SERVER)			/* TCP������ģʽ */
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT) 	/* TCP�ͻ���ģʽ */
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else							/* UDPģʽ */
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}

	/* �˿� 1 */
	if(S1_State==0)
	{
		if(S1_Mode==TCP_SERVER)			/* TCP������ģʽ */
		{
			if(Socket_Listen(1)==TRUE)
				S1_State=S_INIT;
			else
				S1_State=0;
		}
		else if(S1_Mode==TCP_CLIENT)	/* TCP�ͻ���ģʽ */
		{
			if(Socket_Connect(1)==TRUE)
				S1_State=S_INIT;
			else
				S1_State=0;
		}
		else							/* UDPģʽ */
		{
			if(Socket_UDP(1)==TRUE)
				S1_State=S_INIT|S_CONN;
			else
				S1_State=0;
		}
	}

	/* �˿� 2 */
	if(S2_State==0)
	{
		if(S2_Mode==TCP_SERVER)			/* TCP������ģʽ */
		{
			if(Socket_Listen(2)==TRUE)
				S2_State=S_INIT;
			else
				S2_State=0;
		}
		else if(S2_Mode==TCP_CLIENT) 	/* TCP�ͻ���ģʽ */
		{
			if(Socket_Connect(2)==TRUE)
				S2_State=S_INIT;
			else
				S2_State=0;
		}
		else							/* UDPģʽ */
		{
			if(Socket_UDP(2)==TRUE)
			S2_State=S_INIT|S_CONN;
			else
				S2_State=0;
		}
	}

	/* �˿� 3 */
	if(S3_State==0)
	{
		if(S3_Mode==TCP_SERVER)			/* TCP������ģʽ */
		{
			if(Socket_Listen(3)==TRUE)
				S3_State=S_INIT;
			else
				S3_State=0;
		}
		else if(S3_Mode==TCP_CLIENT) 	/* TCP�ͻ���ģʽ */
		{
			if(Socket_Connect(3)==TRUE)
				S3_State=S_INIT;
			else
				S3_State=0;
		}
		else							/* UDPģʽ */
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
}

/*****************************************************************
������: InitializeW5100
����: ��
���: ��
����: ��
˵�����ȶ�W5100��ʼ����Ȼ�������أ����ֱ��ʼ��4���˿�
*****************************************************************/
void InitializeW5100(NetInfo *p_sniLocal)
{
	//��������
	P0_MUX_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 ����ΪP0.0 P0.1
	P0_DIR_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 ����Ϊ����

	P2_MUX_CLR |= (1 << 5);						//GPIO_05 ����Ϊ GPIO_05
	P2_DIR_CLR |= (1 << 30 );					//GPIO_05 ����Ϊ����
	
	W5100_Reset();
	
//	SetNetSetting(p_sniLocal);
		
	InitNetSetting(p_sniLocal);

	W5100_Init();

	/* ������ط����� */
//	Detect_Gateway();

	/* �˿�0 */
	Socket_Init(0);

	/* �˿�1 */
	Socket_Init(1);

	/* �˿�2 */
	Socket_Init(2);

	/* �˿�3 */
	Socket_Init(3);

	//��ʼ����
	SetSocket();

	SIC2_ER					|= (1 << 2);
	
	sic2IrqFuncSet(5	, 0	,(unsigned int)IRQ_W5100);      //�ж�����GPIO_02��ΪW5100�ж��ź�  �½��ش����ж�

}
/*********************************************************************************************************
** Function name:		SendDataNet
** Descriptions:		�����ڷ�������
** input parameters:	p_u8SendBuf		��������ָ�� 
** 						p_u32Len		�������ݳ���
** output parameters:	��������
** Created by:			ZHANG Ye		  
** Created Date:		20110622	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		
** Modified date:	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint32 SendDataNet(uint8 * p_u8SendBuf, uint32 p_u32Len)	  
uint32 SendDataNet(SOCKET p_s, uint8 * p_u8SendBuf, uint32 p_u32Len)
{
	uint8  	sk,state;
	uint32	l_u32SendSize = 0,ret = 0;
//	l_u32SendSize = (p_u32Len > W5100BUFSIZE) ? W5100BUFSIZE : p_u32Len;
		
	memcpy(Tx_Buffer, p_u8SendBuf, p_u32Len);
	//for(sk = 0;sk<4;sk++)
	sk = p_s;
	{
		state = Read_W5100(W5100_S0_SSR+sk*0x100);
		if(state == S_SSR_ESTABLISHED || state == S_SSR_UDP)
		{
			ret = S_tx_process(sk, p_u32Len);	
			ret ? l_u32SendSize = p_u32Len : 0;	  
		} 
	}	
	return l_u32SendSize;
}

//W5100�жϴ���
void IRQ_W5100(void)
{
	
	#define BUFRECSIZE	1000
	uint16	l_u16DataSize;
	uint8   l_u8tempi;
	//uint16	l_u16ID;
	//uint8	ret;

//	OSIntEnter();
	W5100_Interrupt = 1;
	W5100_Interrupt_Process();				//�жϷ�ʽ
	
	//return ;
	/* ���Socket0���յ����� */
	if((S0_Data & S_RECEIVE) == S_RECEIVE)
	{
		S0_Data	&= ~S_RECEIVE;
		
	//	l_u16DataSize = S_rx_process(0)-8;	  //UDPЭ�飬ǰ8�ֽ�Ϊ��������ݣ����û�Ϊ��Ч�ֽ�
		l_u16DataSize = S_rx_process(0);		 // 20121107
		if (l_u16DataSize > 0)
		{
			if( 0 != CheckCrc( Rx_Buffer, l_u16DataSize-2) )	   // ����CRCУ�飬wujinlong 20130911
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
		l_u16DataSize = S_rx_process(1);		 // 20121107
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
		

		l_u16DataSize = S_rx_process(2);		 // 20121107
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

	//
   	if((S3_Data & S_RECEIVE) == S_RECEIVE)
	{
		S3_Data	&= ~S_RECEIVE;
		
		l_u16DataSize = S_rx_process(3)-8;		 // 20121107
		for(l_u8tempi=0; l_u8tempi<4; l_u8tempi++)
		{	
			S3_DIP[l_u8tempi]= Rx_Buffer[l_u8tempi]; 
		}
		S3_DPort[0]=Rx_Buffer[4];
		S3_DPort[1]=Rx_Buffer[5];
		for(l_u8tempi=0;l_u8tempi<4;l_u8tempi++)
			Write_W5100(W5100_S3_DIPR+l_u8tempi, S3_DIP[l_u8tempi]);
		Write_W5100(W5100_S3_DPORT, S3_DPort[0]);
		Write_W5100(W5100_S3_DPORT+1, S3_DPort[1]);
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
}

