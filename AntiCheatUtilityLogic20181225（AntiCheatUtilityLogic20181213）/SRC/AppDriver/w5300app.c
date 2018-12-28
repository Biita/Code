/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			W5100App.c
** Last modified Date:  20110622
** Last Version:		1.0
** Descriptions:		5100顶层驱动
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
static	NetInfo		m_sniLocal;		//本机网络信息
//static	uint8		m_au8NetBuf[W5100BUFSIZE];



/**********************************************************************
程序名: InitNetSetting
输入: m_sniLocal	本地网络参数
输出: 无
返回: 无
说明：设置网络参数设置
**********************************************************************/
void SetNetSetting(NetInfo p_sniLocal)
{
	memcpy(&m_sniLocal,&p_sniLocal,sizeof(m_sniLocal));
}
/**********************************************************************
程序名: ReadTargetSocktInfo
输入:	SOCKET s:	通信的连接号0~~~3
       
输出: pData	：  返回结构体，包含连接的状态，目标IP地址，目标端口号
返回: 无
说明：获取网络连接对端的IP信息
**********************************************************************/
void ReadTargetSocktInfo(SOCKET s,TargetIPInfo_Struct *pData)
{ 
 	//读状态
//	pData->u8State = Read_W5100(W5300_S0_SSR+s<<16);
	pData->u8State = getSn_SSR(s);
	//读IP地址
	getSn_DIPR(s, pData->u8CnntIP);
//	pData->u8CnntIP[0] = Read_W5100(W5300_S0_DIPR+s*0x100)>>8;
//	pData->u8CnntIP[1] = Read_W5100(W5300_S0_DIPR+s*0x100)&0xFF;
//	pData->u8CnntIP[2] = Read_W5100(W5300_S0_DIPR+s*0x100)>>8;
//	pData->u8CnntIP[3] = Read_W5100(W5300_S0_DIPR+s*0x100)&0xFF;   
	//读端口号 不能读W5300的寄存器
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
程序名: InitNetSetting
输入: m_sniLocal	本地网络参数
输出: 无
返回: 无
说明：初始化网络参数 ，端口号，IP等
**********************************************************************/
void InitNetSetting(NetInfo *p_sniLocal)
{
	unsigned char i;
	
	for(i=0; i<4; i++)
	{
		Gateway_IP[i]	= p_sniLocal->au8GatewayIP[i];		// 加载网关参数
		Sub_Mask[i]		= p_sniLocal->au8SubMask[i];			// 加载子网掩码
		IP_Addr[i]		= p_sniLocal->au8IPAddr[i];			// 加载IP地址  
	}

	/* 加载物理地址 */
	for(i=0; i<6; i++)
		Phy_Addr[i] = p_sniLocal->au8MACAddr[i];

	//端口0
	S0_Port[0] = (p_sniLocal->u32LocalPortNO >> 8) & 0xFF;		//端口号
	S0_Port[1] = p_sniLocal->u32LocalPortNO & 0xFF;
	
	S0_State = 0;
// 	for(i=0; i<4; i++)
// 		S0_DIP[i]		= p_sniLocal->au8ServerIP[i];		// 目的IP地址

// 	S0_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//目的端口号
// 	S0_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S0_Mode = TCP_SERVER;							//设置端口0默认的工作方式为TCP服务器模式

	//端口1
	S1_Port[0] = ((p_sniLocal->u32LocalPortNO+1) >> 8) & 0xFF;		//端口号
	S1_Port[1] = (p_sniLocal->u32LocalPortNO+1) & 0xFF;
	S1_State = 0;
// 	for(i=0; i<4; i++)
// 		S1_DIP[i]		= p_sniLocal->au8ServerIP[i];		// 目的IP地址

// 	S1_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//目的端口号
// 	S1_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S1_Mode = TCP_SERVER;							//设置端口1默认的工作方式为TCP服务器模式 

	//端口2
	S2_Port[0] = ((p_sniLocal->u32LocalPortNO+2)>> 8) & 0xFF;		//端口号
	S2_Port[1] = (p_sniLocal->u32LocalPortNO+2)& 0xFF;
	S2_State = 0;
// 	for(i=0; i<4; i++)
// 		S2_DIP[i]		= p_sniLocal->au8ServerIP[i];		// 目的IP地址

// 	S2_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//目的端口号
// 	S2_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

//  	S2_Mode = TCP_CLIENT;						//设置端口2默认的工作方式为TCP服务器 
	S2_Mode = TCP_SERVER;

	//端口3
	S3_Port[0] = ((p_sniLocal->u32LocalPortNO+3) >> 8) & 0xFF;		//端口号
	S3_Port[1] = (p_sniLocal->u32LocalPortNO+3) & 0xFF;
	S3_State = 0;
// 	for(i=0; i<4; i++)
// 		S3_DIP[i]		= p_sniLocal->au8ServerIP[i];		// 目的IP地址

// 	S3_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//目的端口号
// 	S3_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S3_Mode = TCP_SERVER;						//设置端口3默认的工作方式为TCP服务器 

//端口4
	S4_Port[0] = ((p_sniLocal->u32LocalPortNO +4) >> 8) & 0xFF;		//端口号
	S4_Port[1] = (p_sniLocal->u32LocalPortNO +4) & 0xFF;
	
	S4_State = 0;
// 	for(i=0; i<4; i++)
// 		S0_DIP[i]		= p_sniLocal->au8ServerIP[i];		// 目的IP地址

// 	S0_DPort[0] = (p_sniLocal->u32ServerPortNO >> 8) & 0xFF;	 	//目的端口号
// 	S0_DPort[1] = p_sniLocal->u32ServerPortNO & 0xFF;

	S4_Mode = TCP_SERVER;							//设置端口0默认的工作方式为TCP服务器模式
}


// void Socket_Init(SOCKET s)
// {
// 	unsigned char i;

// 	/*设置分片长度，参考W5100数据手册，该值可以不修改*/
// //	Write_W5100((W5100_S0_MSS+s*0x100),0x05);		/*最大分片字节数=1460*/
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
程序名: InitializeW5300
输入: 无
输出: 无
返回: 无
说明：先对W5300初始化，然后检查网关，最后分别初始化4个端口
*****************************************************************/


void InitializeW5300(NetInfo *p_sniLocal)
{
	//设置引脚
	P0_MUX_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 设置为P0.0 P0.1
	P0_DIR_CLR = (1 << 1) | ( 1 << 0 );			//P0.0 P0.1 设置为输入

	P2_MUX_CLR |= (1 << 5);						//GPIO_05 配置为 GPIO_05
	P2_DIR_CLR |= (1 << 30 );					//GPIO_05 设置为输入
	
	W5300_Reset();
	
//	SetNetSetting(p_sniLocal);
		
	InitNetSetting(p_sniLocal);

	W5300_Init();


	/* 检查网关服务器 */
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
	
	sic2IrqFuncSet(5	, 0	,(unsigned int)IRQ_W5300);      //中断引脚GPIO_02作为W5300中断信号  下降沿触发中断
	
	
}

/*********************************************************************************************************
** Function name:		UdpSendData
** Descriptions:		UDP模式发送数据
** input parameters:	p_u8SendBuf		发送数据指针 
** 						p_u32Len		发送数据长度
** output parameters:	发送数量
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
** Descriptions:		TCP模式发送数据
** input parameters:	p_u8SendBuf		发送数据指针 
** 						p_u32Len		发送数据长度
** output parameters:	发送数量
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
** Descriptions:		UDP模式接收数据
** input parameters:	p_u8SendBuf		接收数据指针 
** output parameters:	接收数量
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
** Descriptions:		Tcp模式接收数据
** input parameters:	p_u8SendBuf		接收数据指针 
** output parameters:	接收数量
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
//W5300中断处理
void IRQ_W5300(void)
{
	
	#define BUFRECSIZE	1000
	
	uint16	l_u16DataSize;
	uint8   l_u8tempi;
	
	
	W5300_Interrupt = 1;
	W5300_Interrupt_Process();				//中断方式

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

