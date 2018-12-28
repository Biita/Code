/**
 * \file    socket.c
 *   Implemetation of WIZnet SOCKET API fucntions
 *
 * This file implements the WIZnet SOCKET API functions that is used in your internat application program.
 * 
 * Revision History :
 * ----------  -------  -----------  ----------------------------
 * Date        Version  Author       Description
 * ----------  -------  -----------  ----------------------------
 * 24/03/2008  1.0.0    MidnightCow  Release with W5300 launching
 * ----------  -------  -----------  ----------------------------
 * 15/05/2008  1.1.0    MidnightCow  Refer to M_15052008.
 *                                   Modify the warning code block in recv(). 
 * ----------  -------  -----------  ---------------------------- 
 * 04/07/2008  1.1.1    MidnightCow  Refer to M_04072008.
 *                                   Modify the warning code block in recv(). 
 * ----------  -------  -----------  ---------------------------- 
 * 08/08/2008  1.2.0    MidnightCow  Refer to M_08082008.
 *                                   Modify close(). 
 * ----------  -------  -----------  ---------------------------- 
 * 11/25/2008  1.2.1    Bongjun      Refer to M_11252008.
 *                                   Modify close().
 * ----------  -------  -----------  ----------------------------  
 * 15/03/2012  1.2.2    Dongeun      Modify a ARP error.
 *                                   Modify connect() & sendto()
 * ----------  -------  -----------  ----------------------------  
 * 12/07/2012  1.2.3    Dongeun      Modify a ARP error.
 *                                   Modify connect() & sendto()
 * ----------  -------  -----------  ----------------------------  
 */
 
 
#include "socket.h"
#include "string.h"

#define SWAP16(A)		((((A << 8 ) & 0xFF00)) | ((A >> 8)& 0x00FF))
/* * Variable for temporary source port number 
 */
uint16   iinchip_source_port;

/** 
 * The flag to check if first send or not.
 */

uint8    check_sendok_flag[MAX_SOCK_NUM];

uint8    socket(SOCKET s, uint8 protocol, uint16 port, uint16 flag)
{
   
	IINCHIP_WRITE(Sn_MR(s),(uint16)(protocol | flag|Sn_MR_ND)); // set Sn_MR with protocol & flag
   if (port != 0) IINCHIP_WRITE(Sn_PORTR(s),port);
   else 
   {
      iinchip_source_port++;     // if don't set the source port, set local_port number.
      IINCHIP_WRITE(Sn_PORTR(s),iinchip_source_port);
   }
	 
	 IINCHIP_WRITE(Sn_IMR(s),0x001F);
   setSn_CR(s, Sn_CR_OPEN);      // open s-th SOCKET 
   
   check_sendok_flag[s] = 1;     // initialize the sendok flag.
   
   return 1;   
}

void     close(SOCKET s)
{
   // M_08082008 : It is fixed the problem that Sn_SSR cannot be changed a undefined value to the defined value.
   //              Refer to Errata of W5300
   //Check if the transmit data is remained or not.
   if( ((getSn_MR(s)& 0x0F) == Sn_MR_TCP) && (getSn_TX_FSR(s) != getIINCHIP_TxMAX(s)) ) 
   { 
      uint16 loop_cnt =0;
      while(getSn_TX_FSR(s) != getIINCHIP_TxMAX(s))
      {
         if(loop_cnt++ > 10)
         {
            uint8 destip[4];
            // M_11252008 : modify dest ip address
            //getSIPR(destip);
            destip[0] = 0;destip[1] = 0;destip[2] = 0;destip[3] = 1;
            socket(s,Sn_MR_UDP,0x3000,0);
            sendto(s,(uint8*)"x",1,destip,0x3000); // send the dummy data to an unknown destination(0.0.0.1).
            break; // M_11252008 : added break statement
         }
         wait_10ms(10);
      }
   };
   ////////////////////////////////   
   setSn_IR(s ,0x00FF);          // Clear the remained interrupt bits.
   setSn_CR(s ,Sn_CR_CLOSE);     // Close s-th SOCKET     
}

uint8    connect(SOCKET s, uint8 * addr, uint16 port)
{
   if
   (
      ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      (port == 0x00) 
   )
   {
      return 0;
   }
   
   // set destination IP 
   IINCHIP_WRITE(Sn_DIPR(s),((uint16)addr[0]<<8)+(uint16)addr[1]);
   IINCHIP_WRITE(Sn_DIPR2(s),((uint16)addr[2]<<8)+(uint16)addr[3]);
   // set destination port number
   IINCHIP_WRITE(Sn_DPORTR(s),port);
   
   // Connect
   ApplySubnet();
   setSn_CR(s,Sn_CR_CONNECT);
   
	while( getSn_SSR(s) != SOCK_SYNSENT )
	{
		if(getSn_SSR(s) == SOCK_ESTABLISHED)
		{
			break;
		}
		if(getSn_IR(s) & Sn_IR_TIMEOUT)
		{
			setSn_IR(s,(Sn_IR_TIMEOUT));
			break;
		}
	}
	
   ClearSubnet();

   return 1;   
}

void     disconnect(SOCKET s)
{
   setSn_CR(s,Sn_CR_DISCON);     // Disconnect
}

uint8    listen(SOCKET s)
{
   if (getSn_SSR(s) != SOCK_INIT)
   {
     
		 return 0;
   }

   setSn_CR(s,Sn_CR_LISTEN);     // listen
   
   return 1;
}  

uint8 Socket_UDP(SOCKET s)
{
	Write_W5300(Sn_MR(s), Sn_MR_UDP);		/*设置Socket为UDP模式*/
	Write_W5300(Sn_CR(s), Sn_CR_OPEN);		/*打开Socket*/

	if(getSn_SSR(s)!=SOCK_UDP)
	{
		Write_W5300(Sn_CR(s), Sn_CR_CLOSE);	/*打开不成功，关闭Socket，然后返回*/
		return FALSE;
	}
	else
		return TRUE;

	/*至此完成了Socket的打开和UDP模式设置，在这种模式下它不需要与远程主机建立连接
	因为Socket不需要建立连接，所以在发送数据前都可以设置目的主机IP和目的Socket的端口号
	如果目的主机IP和目的Socket的端口号是固定的，在运行过程中没有改变，那么也可以在这里设置*/
}
 
uint32   send(SOCKET s, uint8 * buf, uint32 len)
{
   uint8 status=0;
   uint32 ret=0;
   uint32 freesize=0;

   ret = len;
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   
   
   /*
    * \note if you want to use non blocking function, <b>"do{}while(freesize < ret)"</b> code block 
    * can be replaced with the below code. \n
    * \code 
    *       while((freesize = getSn_TX_FSR(s))==0); 
    *       ret = freesize;
    * \endcode
    */
   // -----------------------
   // NOTE : CODE BLOCK START
   do                                   
   {
      freesize = getSn_TX_FSR(s);
      status = getSn_SSR(s);
      if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT)) return 0;
   } while (freesize < ret);
   // NOTE : CODE BLOCK END
   // ---------------------
   
   wiz_write_buf(s,buf,ret);                 // copy data

//    #ifdef __DEF_IINCHIP_DBG__
//       loopcnt=0;
//    #endif   
  
   if(!check_sendok_flag[s])                 // if first send, skip.
   {
      //while (!(getSn_IR(s) & Sn_IR_SENDOK))  // wait previous SEND command completion.
		 switch(s)
		 {
				case 0:
					while (!(S0_Data&S_TRANSMITOK))
					{

						if (getSn_SSR(s) == SOCK_CLOSED)    // check timeout or abnormal closed.
						{
								return 0;
						}
					}
//      	setSn_IR(s, Sn_IR_SENDOK);             // clear Sn_IR_SENDOK	
					S0_Data&=~S_TRANSMITOK;
					break;
				case 1:
					while (!(S1_Data&S_TRANSMITOK))
					{
						if (getSn_SSR(s) == SOCK_CLOSED)    // check timeout or abnormal closed.
						{
								return 0;
						}
					}
//      	setSn_IR(s, Sn_IR_SENDOK);             // clear Sn_IR_SENDOK	
					S1_Data&=~S_TRANSMITOK;
					break;
				case 2:
					while (!(S2_Data&S_TRANSMITOK))
					{
						if (getSn_SSR(s) == SOCK_CLOSED)    // check timeout or abnormal closed.
						{
								return 0;
						}
					}
//      	setSn_IR(s, Sn_IR_SENDOK);             // clear Sn_IR_SENDOK	
					S2_Data&=~S_TRANSMITOK;
					break;
				case 4:
					while (!(S4_Data&S_TRANSMITOK))
					{
						if (getSn_SSR(s) == SOCK_CLOSED)    // check timeout or abnormal closed.
						{
								return 0;
						}
					}
//      	setSn_IR(s, Sn_IR_SENDOK);             // clear Sn_IR_SENDOK	
					S4_Data&=~S_TRANSMITOK;
					break;
		}
   }
   else check_sendok_flag[s] = 0;
   
   // send
   setSn_TX_WRSR(s,ret);   
   setSn_CR(s,Sn_CR_SEND);
   
   return ret;
}

uint32   recv(SOCKET s, uint8 * buf, uint32 len)
{
   uint16 pack_size=0;
   
   
   if(IINCHIP_READ(Sn_MR(s)) & Sn_MR_ALIGN) 
   {
      wiz_read_buf(s, buf, (uint32)len);
      setSn_CR(s,Sn_CR_RECV);
      return len;
   }
   

   wiz_read_buf(s,(uint8*)&pack_size,2);        // extract the PACKET-INFO(DATA packet length)
   if( (*(vint16*)MR) & MR_FS )
      pack_size = ((((pack_size << 8 ) & 0xFF00)) | ((pack_size >> 8)& 0x00FF));
   

   wiz_read_buf(s, buf, (uint32)pack_size);     // copy data   
   
   setSn_CR(s,Sn_CR_RECV);                      // recv
   
   /*
   * \warning  send a packet for updating window size. This code block must be only used when W5300 do only receiving data.
   */
   // ------------------------
   // WARNING CODE BLOCK START 
   
   // M_15052008 : Replace Sn_CR_SEND with Sn_CR_SEND_KEEP.
   //if(!(getSn_IR(s) & Sn_IR_SENDOK))
   //{
   //   setSn_TX_WRSR(s,0);                    // size = 0
   //   setSn_CR(s,Sn_CR_SEND);                // send
   //   while(!(getSn_IR(s) & Sn_IR_SENDOK));  // wait SEND command completion
   //   setSn_IR(s,Sn_IR_SENDOK);              // clear Sn_IR_SENDOK bit
   //}
   
   // M_04072008 : Replace Sn_CR_SEND_KEP with Sn_CR_SEND.
   //if(getSn_RX_RSR(s) == 0)                     // send the window-update packet when the window size is full
   //{
   //   uint8 keep_time = 0;
   //   if((keep_time=getSn_KPALVTR(s)) != 0x00) setSn_KPALVTR(s,0x00); // disables the auto-keep-alive-process
   //   setSn_CR(s,Sn_CR_SEND_KEEP);              // send a keep-alive packet by command
   //   setSn_KPALVTR(s,keep_time);               // restore the previous keep-alive-timer value
   //}
   #if 0
   if(getSn_RX_RSR(s) == 0)                     // check if the window size is full or not
   { /* Sn_RX_RSR can be compared with another value instead of ‘0’,
      according to the host performance of receiving data */
      setSn_TX_WRSR(s,1);                       // size : 1 byte dummy size
      IINCHIP_WRITE(Sn_TX_FIFOR(s),0x0000);     // write dummy data into tx memory
      setSn_CR(s,Sn_CR_SEND);                   // send                         
      while(!(getSn_IR(s) & Sn_IR_SENDOK));     // wait SEND command completion 
      setSn_IR(s,Sn_IR_SENDOK);                 // clear Sn_IR_SENDOK bit       
   }    
   #endif                                                                        
   // WARNING CODE BLOCK END
   // ----------------------
   
   return (uint32)pack_size;
}

uint32   sendto(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16 port)
{
   uint8 status=0;
   uint8 isr=0;
   uint32 ret=0;
   
      
   if
   (
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      ((port == 0x00)) ||(len == 0)
   ) 
   {
      return 0;
   }
   
   
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;
   
   // set destination IP address
   IINCHIP_WRITE(Sn_DIPR(s),(((uint16)addr[0])<<8) + (uint16) addr[1]);
   IINCHIP_WRITE(Sn_DIPR2(s),(((uint16)addr[2])<<8) + (uint16) addr[3]);
   // set destination port number
   IINCHIP_WRITE(Sn_DPORTR(s),port);
   
   wiz_write_buf(s, buf, ret);                              // copy data
   // send
   setSn_TX_WRSR(s,ret);
   ApplySubnet();
   setSn_CR(s, Sn_CR_SEND);
   
   while (!((isr = getSn_IR(s)) & Sn_IR_SENDOK))            // wait SEND command completion
   {
      status = getSn_SSR(s);                                // warning ---------------------------------------
      if ((status == SOCK_CLOSED) || (isr & Sn_IR_TIMEOUT)) // Sn_IR_TIMEOUT causes the decrement of Sn_TX_FSR
      {                                                     // -----------------------------------------------
         setSn_IR(s,Sn_IR_TIMEOUT);
         return 0;
      }
   }

   setSn_IR(s, Sn_IR_SENDOK); // Clear Sn_IR_SENDOK
   ClearSubnet();     
   
   return ret;   
}

uint32   recvfrom(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16  *port)
{
   uint16 head[4];
   uint32 data_len=0;
   uint16 crc[2];
   
   
   if ( len > 0 )
   {
      switch (IINCHIP_READ(Sn_MR(s)) & 0x07)       // check the mode of s-th SOCKET
      {                                            // -----------------------------
         case Sn_MR_UDP :                          // UDP mode 
            wiz_read_buf(s, (uint8*)head, 8);      // extract the PACKET-INFO
            // read peer's IP address, port number.
            if(*((vuint16*)MR) & MR_FS)            // check FIFO swap bit
            {
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
               head[1] = ((((head[1] << 8 ) & 0xFF00)) | ((head[1] >> 8)& 0x00FF));
               head[2] = ((((head[2] << 8 ) & 0xFF00)) | ((head[2] >> 8)& 0x00FF));
               head[3] = ((((head[3] << 8 ) & 0xFF00)) | ((head[3] >> 8)& 0x00FF));
            }
            addr[0] = (uint8)(head[0] >> 8);       // destination IP address
            addr[1] = (uint8)head[0];
            addr[2] = (uint8)(head[1]>>8);
            addr[3] = (uint8)head[1];
            *port = head[2];                       // destination port number
            data_len = (uint32)head[3];            // DATA packet length
            
   
            wiz_read_buf(s, buf, data_len);        // data copy.
            break;
                                                   // -----------------------
         case Sn_MR_IPRAW :                        // IPRAW mode
            wiz_read_buf(s, (uint8*)head, 6);      // extract the PACKET-INFO 
            if(*((vuint16*)MR) & MR_FS)            // check FIFO swap bit
            {
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
               head[1] = ((((head[1] << 8 ) & 0xFF00)) | ((head[1] >> 8)& 0x00FF));
               head[2] = ((((head[2] << 8 ) & 0xFF00)) | ((head[2] >> 8)& 0x00FF));
            }   			
            addr[0] = (uint8)(head[0] >> 8);       // destination IP address
            addr[1] = (uint8)head[0];
            addr[2] = (uint8)(head[1]>>8);
            addr[3] = (uint8)head[1];
            data_len = (uint32)head[2];            // DATA packet length
            
            wiz_read_buf(s, buf, data_len);        // data copy.
            break;                                 
                                                   // -----------------------
         case Sn_MR_MACRAW :                       // MACRAW mode
            wiz_read_buf(s,(uint8*)head,2);        // extract the PACKET-INFO
            if(*((vuint16*)MR) & MR_FS)            // check FIFO swap bit
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
            data_len = (uint32)head[0];            // DATA packet length
            wiz_read_buf(s, buf, data_len);        // data copy.
            wiz_read_buf(s,(uint8*)crc, 4);        // extract CRC data and ignore it.

            break;
         default :
            break;
      }
      setSn_CR(s,Sn_CR_RECV);                      // recv
   }
   
   return data_len;   
}
