/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			FPGA_APP.C
** Last modified Date:  20110523
** Last Version:		1.0
** Descriptions:		��λ��ͨ������
**
**--------------------------------------------------------------------------------------------------------
** Created by:			
** Created date:		
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			zhouhang
** Modified date:		2013.4.15
** Version:			  	1.1
** Descriptions:		���������¶ȴ�����
						��Ӧ�°汾FPGA�߼��޸ļĴ�����ַ
**
*********************************************************************************************************/
#define __FPGA_C

#include "FPGA_APP.h"
#define		SETUPALIAS				g_Setup			//���ò����ṹ
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

/*********************************************************************************************************
** Function name:           Fpga_Err_Alert
**
** Descriptions:            FPGAͨ�Ŵ��󾯱�
**
** input parameters:        uint8 											
** output parameters:      	
** Returned value:          void
**
** Created by:           	zhouhang    
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Fpga_Err_Alert(uint8 p_ucType)
{
	uint8 i;
	for(i=0;i<p_ucType;i++)
	{
		BeepON();
		Delay(1000);
		BeepOFF();
 		Delay(4000);
	}
} 


/*********************************************************************************************************
** Function name:           Fpga_Emc_Check
**
** Descriptions:            ���ARM��FPGA��EMC����״̬�������ж�FPGA�Ƿ�����
**
** input parameters:          							  
** output parameters:        ��
** Returned value:        	0��FPGA����
							1��ż�����������쳣
**							2��
** Created by:              
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 Fpga_Emc_Check(void)
{
	uint8 ret = 0;
	uint32 err_cnt = 0;
	//�������߼��
	do
	{
		FPGA_TEST_1 = 0xAAAA;
		Delay(1);	
		if(0xAAAA == FPGA_TEST_1)
		{
			ret = 0;
			err_cnt = 0;
		}
		else
		{
			ret = 1;
			err_cnt++;
			if(FPGA_CHECK_MAX == err_cnt)
				break;
			else
				;
		}
	}while(1 == ret);

	if(0 == ret)
	{
		do
		{
			FPGA_TEST_1 = 0x5555;
			Delay(1);	
			if(0x5555 == FPGA_TEST_1)
			{
				ret = 0;
				err_cnt = 0;
			}
			else
			{
				ret = 2;
				err_cnt++;
				if(FPGA_CHECK_MAX == err_cnt)
					break;
				else
					;
			}
		}while(2 == ret);		
	}
	else
	{
		;	
	}	

	return ret;
}

/*********************************************************************************************************
** Function name:           FPGAInitParam
**
** Descriptions:            FPGA�����趨
**
** input parameters:        chlSum		ͨ����
							pParam 		��ʼ������ָ��
							  0		----   �ϳ���ֵ
							  1		----   �³���ֵ
							  2		----	�߼��ϳ���ֵ
							  3     ----	�߼��³���ֵ
							  4		----	�ϳӵ��� 
							  5     ----    �³ӵ���
						  
** output parameters:        ��
** Returned value:          ��ȷ�趨���ط���ֵ��ʧ�ܷ�����
**
** Created by:              
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

uint8 FPGAInitParam() 
{
	uint32 unErr_count = 0;
	uint8 ucErr_type = 0;
	sic2IrqFuncSet(1, 0	,(unsigned int)IRQ_FPGA);
	do
	{
		if(unErr_count < FPGA_POWON_MAX)
		{
			FPGA_TEST_1 = 0x1234;
			unErr_count++;	
		}
		else
		{
			ucErr_type = 5;
			break;
		}
	}while(0x1234 != FPGA_TEST_1);
#if FPGA_EMC_CHECK
	ucErr_type = Fpga_Emc_Check();		
#endif 
	if(ucErr_type)
	{
		while(1)
		{
			LEDON();
			Fpga_Err_Alert(ucErr_type);	
			Delay(30000);
		}
		
	}
	else
	{
		if(PROMODPARAM.m_au8AcqBoardFlag[0] == 2)
//			SET_BOARD_TYPE(CFG_1,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_1,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[1] == 2)
//			SET_BOARD_TYPE(CFG_2,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_2,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[2] == 2)
//			SET_BOARD_TYPE(CFG_3,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_3,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[3] == 2)
//			SET_BOARD_TYPE(CFG_4,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_4,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[4] == 2)
//			SET_BOARD_TYPE(CFG_5,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_5,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[5] == 2)
//			SET_BOARD_TYPE(CFG_6,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_6,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[6] == 2)
//			SET_BOARD_TYPE(CFG_7,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_7,XQ_BOARD);
		if(PROMODPARAM.m_au8AcqBoardFlag[7] == 2)
//			SET_BOARD_TYPE(CFG_8,ZT_BOARD);
//		else
			SET_BOARD_TYPE(CFG_8,XQ_BOARD);

//		SET_BOARD_TYPE(CFG_1,XQ_BOARD);
//		SET_BOARD_TYPE(CFG_2,XQ_BOARD);
//		ENABLE_REAL_WAVE(CFG_1);

		SET_FPGA_DOWNVALUE_SE();  
		SET_FPGA_UPVALUE_SE();
		SET_FPGA_UPVALUE();
		SET_FPGA_DNVALUE();
		
		SET_FPGA_DIGITAL_UPVALUE();
		SET_FPGA_DIGITAL_DNVALUE();
		SET_FPGA_DIGITAL_UPVALUE_SE();
		SET_FPGA_DIGITAL_DOWNVALUE_SE();
		
//		SET_FPGA_UPPOINTS();
//		SET_FPGA_DOWNPOINTS();
		//	EnableFPGA_INT();  
		SET_BOARD_TYPE(CFG_1,ZT_BOARD);	

		
	}	 	
	return 1;  
}


/*********************************************************************************************************
** Function name:           Read_Fpga_Status
**
** Descriptions:            ��ȡFPGA״̬��Ŀǰ�����ɼ����DS18B20��״̬��0����δ���ӻ��쳣��1��������
**
** input parameters:        void											
** output parameters:      	
** Returned value:          1
**
** Created by:           	zhouhang    
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint16 Read_Fpga_Status(void)
{
	uint16 ret;
	ret = FPGA_STATUS;
	return ret;
} 

/*********************************************************************************************************
** Function name:           FPGAtemperature
**
** Descriptions:            ��ȡ�¶�ֵ
**
** input parameters:        void											
** output parameters:      	void
** Returned value:          ����float�����¶�ֵ
**
** Created by:           	zhouhang    
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
fp32 FPGA_Temperature(void)
{
	fp32 fTemperature = 0;
	uint16 Temperature_Code;
	Temperature_Code = TEMPERATURE;
	if(Temperature_Code & 0xF800)
	{
		Temperature_Code = ((~Temperature_Code) + 1) & 0x7FF;
		fTemperature = -((fp32)(Temperature_Code >> 4) + (((fp32)(Temperature_Code & 0xF)) / 16));
	}
	else
	{
		Temperature_Code = Temperature_Code & 0x7FF;
		fTemperature = (fp32)(Temperature_Code >> 4) + (((fp32)(Temperature_Code & 0xF)) / 16);		
	}
	return fTemperature;
} 


/*********************************************************************************************************
** Function name:           IRQ_FPGA
**
** Descriptions:			FPGA�ж�
**
** input parameters:		        						
** output parameters:		 
** Returned value:          
**
** Created by:              
** Created Date
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

void  IRQ_FPGA(void)	
{
	uint8  ret = 0;
	uint8  i = 0, temp = 0;
	uint16 l_u16ID = 0,l_u16TriggerID = 0,l_u16Cnt = 0;
	uint16 l_u16IrqInfo = 0;
	uint8  l_u16TempIndex = 0,l_u8Type = 0,l_u8FrameTyle; //ѭ������
	uint8  l_u8BoardType = 0;
	uint8  l_u8BoardNo = 0,l_u8ChannelNo = 0;
// 	uint8 l_u8Err;
// 	uint16 l_u16SendLen;
	uint16 l_u16TypeNew = 0;    //���ִ�������ȡ����ʱʹ��
	uint32 l_u32TimeNow;
	uint8  l_u8DigitZTPos = 0;    //����խ������λ��(��ʱ����)
	uint8  l_u8DigitZTMaxValuePos = 0;   //����խ�����ֵλ��(��ʱ����)
	
	EnterFlagFPGA_INT();// �����жϣ��������ߵ�ƽ
	// խ�����
	l_u16IrqInfo = FPGA_INT_TYPE;
	l_u8BoardType = ((l_u16IrqInfo >> 8) & 0x3);

	switch(l_u8BoardType)			//���ݰ忨���͵Ĳ�ָͬ��ͬ�Ĵ������
	{
		case 0x0:
		case 0x1:
		case 0x2:
			goto ZHDC_SAMPLING_PROCESS;		//�к����ɼ���
		case 0x3:
			goto DigSensor_SAMPLING_PROCESS;//���ִ������ɼ���
		default:
			break;
	}


ZHDC_SAMPLING_PROCESS:				   //�к����խ���ɼ��崦������

	l_u8Type = 0x00FF & l_u16IrqInfo;	
	l_u16ID = SETUPALIAS.u8ArrayForZT[((l_u8Type >> 5)& 0X07)*8+(l_u8Type &0X07)];
	l_u8FrameTyle = GETFRAMESTYLE(l_u8Type);
	l_u8BoardNo = (l_u8Type >> 5)& 0x07;
	l_u8ChannelNo = (l_u8Type &0x07);
	
	if(g_u8IfRunSta == 1)
	{
		if(g_u8RunZTSta[l_u16ID] == 1)
		{
			g_u8RunZTSta[l_u16ID] = 0;
			g_u8ZTStaSendFlag = 1;
		}
		goto IRQ_EXIT;
	}
	if( l_u16ID >= 64 )
	{
		goto IRQ_EXIT;
	}
	if(PROMODPARAM.m_au8ZTIntEn[((l_u8Type >> 5)& 0X07)*8+(l_u8Type &0X07)] == 0)
	{
		goto IRQ_EXIT;
	}

	//
	switch(l_u8FrameTyle)
	{
		case 0x00://��ʼ����֡
			 				 
			// ��֡��ż�¼��խ����һλ�ó�ʼ��
			g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
			g_u16ChlWindex++;
			g_u16ChlWindex &= CHL_BUFMASK;
			memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
			g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
			g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 
			
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{			
				break;	
			}
			l_u16Cnt = GETDATANUM(l_u8Type);
			g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			g_ChlData[l_u16TriggerID].u32realCnt =0;

			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][l_u16TempIndex] = *(GETDATAADDR(l_u8Type)+l_u16TempIndex); 		
			}
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ	
			g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  
			g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
			g_ChlData[l_u16TriggerID].u32uptm = GETSECUPTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u32dntm = GETSECDNTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u16ptMaxValue = GETMAXDATA(l_u8Type);
			g_ChlData[l_u16TriggerID].u32Maxtm  =  GETMAXTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u32chlSum	 = 	GETSUM(l_u8Type);
			g_ChlData[l_u16TriggerID].u32FirstDnTime = GETFITDNTIME(l_u8Type);			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 1;
			g_ChlData[l_u16TriggerID].m_bUpOrDn = 0;
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32FirstDnTime;
			g_ChlData[l_u16TriggerID].u8Id = l_u16ID;
			g_ChlData[l_u16TriggerID].u8BoardNo = l_u8BoardNo;
			g_ChlData[l_u16TriggerID].u8ChannelNo = l_u8ChannelNo;
			g_ChlData[l_u16TriggerID].m_bDirectionFlag = 0;
			g_ChlData[l_u16TriggerID].u8BoardType = 1;

			if(PROMODPARAM.m_au8CHLSET[g_ChlData[l_u16TriggerID].u8Id/PROMODPARAM.m_u8ZTRow] < 10)
			{    // liumingxin �����λ���������ĳ���Ƿ����խ���߼�
				// ��խ��->�߼�����
				ret = OSQPost(g_pQDnMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
					g_ErrMsgState |= 0x10;
			}
			
			// ����if�жϣ����������ʹ�ܣ����Ͳ��� wujinlong 20130111
			if( 1 == g_Setup.u8SendWaveEnable )
			{
				ZTRead[g_ChlData[l_u16TriggerID].u8Id]=g_u8RasterDataWriteIndex-1;
				ret = OSQPost(g_pQWaveMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
				{
					g_ErrMsgState |= 0x100;
				}
			}
			g_ChlID2TriggerID[l_u16ID] = EMPTY_ITEMZERO;
			break;
		case 0x01://��ʼ֡			
			// ��֡��ż�¼��խ����һλ�ó�ʼ��
			g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
			g_u16ChlWindex++;
			g_u16ChlWindex &= CHL_BUFMASK;
			memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
			g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
			g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
			// ��֡����
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			l_u16Cnt = GETDATANUM(l_u8Type);

			g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			g_ChlData[l_u16TriggerID].u32realCnt =0;
			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(GETDATAADDR(l_u8Type)+l_u16TempIndex);				
			}
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ						
			// ��֡��ʼ��
			g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  // revised by wujinlong 20121203
			g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
			g_ChlData[l_u16TriggerID].u32uptm = GETSECUPTIME(l_u8Type);	
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32uptm;
			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 0;		
			break;

		case 0x03: //�м�֡
			// �м�֡���ݿ���
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];							   
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{			
#if 0 == FPGANOFIRSTFRAME
				break;
#endif
#if FPGANOFIRSTFRAME > 0
				// ˵����û����֡������µ�խ��λ��	  wujinlong 20130124
				g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
				g_u16ChlWindex++;
				g_u16ChlWindex &= CHL_BUFMASK;
				memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
				g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
				g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
				//
				l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
				g_ChlData[l_u16TriggerID].u16ptCnt = 0;
				g_ChlData[l_u16TriggerID].u32realCnt =0;	
				
#endif
			}
			l_u16Cnt = GETDATANUM(l_u8Type);
			//g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			if( g_ChlData[l_u16TriggerID].u16ptCnt+l_u16Cnt >= MAXBUFPOINT )
			{
				g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ
				// ���߽��г�����������break
				break;
			}
			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(GETDATAADDR(l_u8Type)+l_u16TempIndex);
			} 
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ
			
			break;

		case 0x02:	//����֡
			// �߼���������
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{
#if 0 == FPGANOFIRSTFRAME 			
				break;
#endif
#if FPGANOFIRSTFRAME > 0 				
				// ˵����û����֡������µ�խ��λ��	  wujinlong 20130124
				g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
				g_u16ChlWindex++;
				g_u16ChlWindex &= CHL_BUFMASK;
				memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
				g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
				g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
				//
				l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
				g_ChlData[l_u16TriggerID].u16ptCnt = 0;
				g_ChlData[l_u16TriggerID].u32realCnt =0;
#endif	
			}
			l_u16Cnt = GETDATANUM(l_u8Type);
			
			g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  
			g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
			g_ChlData[l_u16TriggerID].u32uptm = GETSECUPTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u32dntm = GETSECDNTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u16ptMaxValue = GETMAXDATA(l_u8Type);
			g_ChlData[l_u16TriggerID].u32Maxtm  =  GETMAXTIME(l_u8Type);
			g_ChlData[l_u16TriggerID].u32chlSum	 = 	GETSUM(l_u8Type);
			g_ChlData[l_u16TriggerID].u32FirstDnTime = GETFITDNTIME(l_u8Type);			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 1;
			g_ChlData[l_u16TriggerID].m_bUpOrDn = 0;
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32FirstDnTime;
			// ͨ��խ�����ȷ������32-63���0-31
			g_ChlData[l_u16TriggerID].u8Id = l_u16ID;
			g_ChlData[l_u16TriggerID].u8BoardNo = l_u8BoardNo;
			g_ChlData[l_u16TriggerID].u8ChannelNo = l_u8ChannelNo;
			g_ChlData[l_u16TriggerID].m_bDirectionFlag = 0;
			g_ChlData[l_u16TriggerID].u8BoardType = 1;

			// ����֡���ݿ���			
			if( g_ChlData[l_u16TriggerID].u16ptCnt+l_u16Cnt <= MAXBUFPOINT )
			{
				// ���߽��г�����������break
				//break;			
				for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
				{
					g_ChlData[l_u16TriggerID].u16data[0][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(GETDATAADDR(l_u8Type)+l_u16TempIndex);
				}
				g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;				
			}
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ

//#if LOGICENABLE > 0
		
			if(PROMODPARAM.m_au8CHLSET[g_ChlData[l_u16TriggerID].u8Id/PROMODPARAM.m_u8ZTRow] < 10)
			{    // liumingxin �����λ���������ĳ���Ƿ����խ���߼�
				// ��խ��->�߼�����
				ret = OSQPost(g_pQDnMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
					g_ErrMsgState |= 0x10;
			}
//#endif
			
			// ����if�жϣ����������ʹ�ܣ����Ͳ��� wujinlong 20130111
			if( 1 == g_Setup.u8SendWaveEnable )
			{
				ZTRead[g_ChlData[l_u16TriggerID].u8Id]=g_u8RasterDataWriteIndex-1;
				ret = OSQPost(g_pQWaveMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
				{
					g_ErrMsgState |= 0x100;
				}  
			}

			g_ChlID2TriggerID[l_u16ID] = EMPTY_ITEMZERO;
			break;

		default:
			break;
	} 

	goto IRQ_EXIT;	  //��������

DigSensor_SAMPLING_PROCESS: 	//����խ����������
	
	l_u8Type = 0x00FF & l_u16IrqInfo;
	l_u16ID = SETUPALIAS.u8ArrayForZT[((l_u8Type >> 2)&0x07) *8 +  ((l_u8Type >> 5)&0x07)];
	l_u8BoardNo = (l_u8Type >> 2)&0x07;
	l_u8ChannelNo = (l_u8Type >> 5)&0x07;
	l_u16TypeNew = 	l_u8Type;
	l_u16TypeNew = ((l_u16TypeNew&0x03)|((l_u16IrqInfo&0x0400)>>8)) + ((l_u16TypeNew&0x1C) << 1);    
	l_u8FrameTyle = ABGETFRAMESTYLE(l_u16TypeNew);
	if(g_u8IfRunSta == 1)
	{
		if(g_u8RunZTSta[l_u16ID] == 1)
		{
			g_u8RunZTSta[l_u16ID] = 0;
			g_u8ZTStaSendFlag = 1;
		}
		goto IRQ_EXIT;
	}
	if( l_u16ID >= 64 )
	{
		goto IRQ_EXIT;
	}
	if(PROMODPARAM.m_au8ZTIntEn[((l_u8Type >> 2)&0x07) *8 +  ((l_u8Type >> 5)&0x07)] == 0)
	{
		goto IRQ_EXIT;
	}
	switch(l_u8FrameTyle)
	{
		case 0x00://��ʼ����֡			 				 
			// ��֡��ż�¼��խ����һλ�ó�ʼ��
			g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
			g_u16ChlWindex++;
			g_u16ChlWindex &= CHL_BUFMASK;
			memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
			g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
			g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
			
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{			
				break;	
			}
			l_u16Cnt = ABGETDATANUM(l_u16TypeNew);
			g_ChlData[l_u16TriggerID].u8DataErrorFlag = 0;
			g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			g_ChlData[l_u16TriggerID].u32realCnt =0;
			g_ChlData[l_u16TriggerID].u8packetCnt = 1;
			if(l_u16Cnt == 0)
			{
				g_ChlData[l_u16TriggerID].u8DataErrorFlag++;
			}
			g_ChlData[l_u16TriggerID].u8FrameNo = ABGETFRAMENUM(l_u16TypeNew);

			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][l_u16TempIndex] = *(AGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
				g_ChlData[l_u16TriggerID].u16data[1][l_u16TempIndex] = *(BGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
			}
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ
	
			ReadFPGAGlobalTime(l_u32TimeNow);
			g_ChlData[l_u16TriggerID].u32MainFPGATime = l_u32TimeNow;   // ��ȡ���ذ�FPGA��ʱ��
			g_ChlData[l_u16TriggerID].u32FirstDnTime = ABGETFITDNTIME(l_u16TypeNew);
			if(g_ChlData[l_u16TriggerID].u32MainFPGATime > g_ChlData[l_u16TriggerID].u32FirstDnTime)
			{
				g_ChlData[l_u16TriggerID].u32TimeDiffValue = g_ChlData[l_u16TriggerID].u32MainFPGATime - g_ChlData[l_u16TriggerID].u32FirstDnTime;
				g_ChlData[l_u16TriggerID].n32DiffValue = g_ChlData[l_u16TriggerID].u32TimeDiffValue;
			
				g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  
				g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
				g_ChlData[l_u16TriggerID].u32uptm = ABGETSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32dntm = ABGETSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16ptMaxValue = ABGETMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32Maxtm  =  ABGETMAXTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32chlSum	 = 	ABGETSUM(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32FirstDnTime += g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				/**** Aխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = GETFIRSTZTFITDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = GETFIRSTZTSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = GETFIRSTZTFITUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = GETFIRSTZTSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = GETFIRSTZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = GETFIRSTZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = GETFIRSTZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir == g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir)
// 				{
// 					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt=0;
// 				}

				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8FirstZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32FirstZTSum = GETFIRSTZTSUM(l_u16TypeNew);
				/**** end ****/
				
				/**** Bխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir = GETSECZTFITDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec = GETSECZTSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir = GETSECZTFITUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec = GETSECZTSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16SecondZTMaxValue = GETSECZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = GETSECZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = GETSECZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir==g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec)
// 				{
// 					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt=0;
// 				}
				
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8SecondZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8SecondZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32SecondZTSum = GETSECZTSUM(l_u16TypeNew);
				/**** end ****/
				

				//������խ��A����Ϣ�洢��B��,�����A�е���Ϣ(����ֳ������������ֻ�е�������խ�������)
				if((g_u8A2BFlag == 1 && l_u16ID <= 2) || (g_u8A2BFlag == 2 && l_u16ID >= 3 && l_u16ID <= 5))
				{
					//��Aխ������Ϣ���Ƶ�Bխ����
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec;
					g_ChlData[l_u16TriggerID].u16SecondZTMaxValue     = g_ChlData[l_u16TriggerID].u16FirstZTMaxValue;
					
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt    = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos        = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos;
					g_ChlData[l_u16TriggerID].u32SecondZTSum          = g_ChlData[l_u16TriggerID].u32FirstZTSum;
					
					//���Aխ������Ϣ
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = 0;
					g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = 0;
					
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTSum = 0;
				}
			}
			else
			{
				g_ChlData[l_u16TriggerID].u32TimeDiffValue = g_ChlData[l_u16TriggerID].u32FirstDnTime - g_ChlData[l_u16TriggerID].u32MainFPGATime;
				g_ChlData[l_u16TriggerID].n32DiffValue = 0 - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  
				g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
				g_ChlData[l_u16TriggerID].u32uptm = ABGETSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32dntm = ABGETSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16ptMaxValue = ABGETMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32Maxtm  =  ABGETMAXTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32chlSum	 = 	ABGETSUM(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32FirstDnTime -= g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				/**** Aխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = GETFIRSTZTFITDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = GETFIRSTZTSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = GETFIRSTZTFITUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = GETFIRSTZTSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = GETFIRSTZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = GETFIRSTZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = GETFIRSTZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir == g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir)
// 				{
// 					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt=0;
// 				}

				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8FirstZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32FirstZTSum = GETFIRSTZTSUM(l_u16TypeNew);
				/**** end ****/
				
				/**** Bխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir = GETSECZTFITDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec = GETSECZTSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir = GETSECZTFITUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec = GETSECZTSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16SecondZTMaxValue = GETSECZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = GETSECZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = GETSECZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir==g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec)
// 				{
// 					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt=0;
// 				}
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8SecondZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8SecondZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32SecondZTSum = GETSECZTSUM(l_u16TypeNew);
				/**** end ****/
				
				//������խ��A����Ϣ�洢��B��,�����A�е���Ϣ(����ֳ������������ֻ�е�������խ�������)
				if((g_u8A2BFlag == 1 && l_u16ID <= 2) || (g_u8A2BFlag == 2 && l_u16ID >= 3 && l_u16ID <= 5))
				{
					//��Aխ������Ϣ���Ƶ�Bխ����
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec;
					g_ChlData[l_u16TriggerID].u16SecondZTMaxValue     = g_ChlData[l_u16TriggerID].u16FirstZTMaxValue;
					
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt    = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos        = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos;
					g_ChlData[l_u16TriggerID].u32SecondZTSum          = g_ChlData[l_u16TriggerID].u32FirstZTSum;
					
					//���Aխ������Ϣ
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = 0;
					g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = 0;
					
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTSum = 0;
				}
			}
			
			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 1;
			g_ChlData[l_u16TriggerID].m_bUpOrDn = 0;
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32FirstDnTime;
			// ͨ��խ�����ȷ������32-63���0-31
			g_ChlData[l_u16TriggerID].u8Id = l_u16ID;
			g_ChlData[l_u16TriggerID].u8BoardNo = l_u8BoardNo;
			g_ChlData[l_u16TriggerID].u8ChannelNo = l_u8ChannelNo;
			g_ChlData[l_u16TriggerID].m_bDirectionFlag = 0;
			g_ChlData[l_u16TriggerID].u8BoardType = 2;
					
			if(PROMODPARAM.m_au8CHLSET[g_ChlData[l_u16TriggerID].u8Id/PROMODPARAM.m_u8ZTRow] < 10)
			{    // liumingxin �����λ���������ĳ���Ƿ����խ���߼�
				// ��խ��->�߼�����
				ret = OSQPost(g_pQDnMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
					g_ErrMsgState |= 0x10;
			}					 					
			// ����if�жϣ����������ʹ�ܣ����Ͳ��� wujinlong 20130111
			if( 1 == g_Setup.u8SendWaveEnable )
			{
				//Fag[abc++]=g_ChlData->u8Id;
				ZTRead[g_ChlData[l_u16TriggerID].u8Id]=g_u8RasterDataWriteIndex-1;
				ret = OSQPost(g_pQWaveMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
				{
					g_ErrMsgState |= 0x100;
				}
			}
			g_ChlID2TriggerID[l_u16ID] = EMPTY_ITEMZERO;

			break;

		case 0x01://��ʼ֡			
			// ��֡��ż�¼��խ����һλ�ó�ʼ��
			g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
			g_u16ChlWindex++;
			g_u16ChlWindex &= CHL_BUFMASK;
			memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
			g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
			g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221

			// ��֡����
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			l_u16Cnt = ABGETDATANUM(l_u16TypeNew);

			g_ChlData[l_u16TriggerID].u8DataErrorFlag = 0;
			g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			g_ChlData[l_u16TriggerID].u32realCnt =0;
			g_ChlData[l_u16TriggerID].u8packetCnt = 1;
			if(l_u16Cnt == 0)
			{
				g_ChlData[l_u16TriggerID].u8DataErrorFlag++;
			}
			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][l_u16TempIndex] = *(AGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
				g_ChlData[l_u16TriggerID].u16data[1][l_u16TempIndex] = *(BGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
			}
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ						
			// ��֡��ʼ��
			g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  // revised by wujinlong 20121203
			g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
			g_ChlData[l_u16TriggerID].u32uptm = GETSECUPTIME(l_u16TypeNew);	
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32uptm;
			g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = GETFIRSTZTFITUPTIME(l_u16TypeNew);
			g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = GETFIRSTZTSECUPTIME(l_u16TypeNew);
			g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir = GETSECZTFITUPTIME(l_u16TypeNew);
			g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec = GETSECZTSECUPTIME(l_u16TypeNew);
			g_ChlData[l_u16TriggerID].u8FrameNo = ABGETFRAMENUM(l_u16TypeNew);
			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 0;
			g_ChlData[l_u16TriggerID].m_bUpOrDn = 1;	
			
			break;

		case 0x03: //�м�֡
			// �м�֡���ݿ���
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];							   
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{			
#if 0 == FPGANOFIRSTFRAME
				break;
#endif
#if FPGANOFIRSTFRAME > 0
				// ˵����û����֡������µ�խ��λ��	  wujinlong 20130124
				g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
				g_u16ChlWindex++;
				g_u16ChlWindex &= CHL_BUFMASK;
				memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
				g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
				g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
				//
				l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
				g_ChlData[l_u16TriggerID].u16ptCnt = 0;
				g_ChlData[l_u16TriggerID].u32realCnt =0;	
				
#endif
			}
			l_u16Cnt = ABGETDATANUM(l_u16TypeNew);
			if(l_u16Cnt == 0)
			{
				g_ChlData[l_u16TriggerID].u8DataErrorFlag++;
			}
			g_ChlData[l_u16TriggerID].u8packetCnt++;
			g_ChlData[l_u16TriggerID].u8FrameNo = ABGETFRAMENUM(l_u16TypeNew);
			//g_ChlData[l_u16TriggerID].u16ptCnt = 0;
			if( g_ChlData[l_u16TriggerID].u16ptCnt+l_u16Cnt >= MAXBUFPOINT )
			{
				g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ
				// ���߽��г�����������break
				break;
			}
			for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
			{
				g_ChlData[l_u16TriggerID].u16data[0][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(AGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
				g_ChlData[l_u16TriggerID].u16data[1][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(BGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
			} 
			g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ
			
			break;

		case 0x02:	//����֡
			// �߼���������
			l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
			if(l_u16TriggerID == EMPTY_ITEMZERO)
			{
#if 0 == FPGANOFIRSTFRAME 			
				break;
#endif
#if FPGANOFIRSTFRAME > 0 				
				// ˵����û����֡������µ�խ��λ��	  wujinlong 20130124
				g_ChlID2TriggerID[l_u16ID] = g_u16ChlWindex;
				g_u16ChlWindex++;
				g_u16ChlWindex &= CHL_BUFMASK;
				memset( &g_ChlData[g_ChlID2TriggerID[l_u16ID]], 0, sizeof(Channal_Struct) -2*2*MAXBUFPOINT);
				g_ChlData[g_u16ChlWindex].u16ptCnt = 0;
				g_ChlData[g_u16ChlWindex].u32uptm = (uint32)-1;	 //�߼�����������ֹ���� wujinlong 20121221
				//
				l_u16TriggerID = g_ChlID2TriggerID[l_u16ID];
				g_ChlData[l_u16TriggerID].u16ptCnt = 0;
				g_ChlData[l_u16TriggerID].u32realCnt =0;
#endif	
			}
			l_u16Cnt = ABGETDATANUM(l_u16TypeNew);
			if(l_u16Cnt == 0)
			{
				g_ChlData[l_u16TriggerID].u8DataErrorFlag++;
			}
			
			g_ChlData[l_u16TriggerID].u16SelfID = l_u16TriggerID;  
			g_ChlData[l_u16TriggerID].u16YDMID = l_u16TriggerID;
			
			ReadFPGAGlobalTime(l_u32TimeNow);
			g_ChlData[l_u16TriggerID].u32MainFPGATime = l_u32TimeNow;   // ��ȡ���ذ�FPGA��ʱ��
			g_ChlData[l_u16TriggerID].u32FirstDnTime = ABGETFITDNTIME(l_u16TypeNew);
			if(g_ChlData[l_u16TriggerID].u32MainFPGATime > g_ChlData[l_u16TriggerID].u32FirstDnTime)
			{
				g_ChlData[l_u16TriggerID].u32TimeDiffValue = g_ChlData[l_u16TriggerID].u32MainFPGATime - g_ChlData[l_u16TriggerID].u32FirstDnTime;
				g_ChlData[l_u16TriggerID].n32DiffValue = g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				g_ChlData[l_u16TriggerID].u32uptm = ABGETSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32dntm = ABGETSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16ptMaxValue = ABGETMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32Maxtm  =  ABGETMAXTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32chlSum	 = 	ABGETSUM(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32FirstDnTime += g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				/**** Aխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = GETFIRSTZTFITDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = GETFIRSTZTSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = GETFIRSTZTFITUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = GETFIRSTZTSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = GETFIRSTZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = GETFIRSTZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = GETFIRSTZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir == g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir)
// 				{
// 					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt=0;
// 				}
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8FirstZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32FirstZTSum = GETFIRSTZTSUM(l_u16TypeNew);		
				/**** end ****/
				
				/**** Bխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir = GETSECZTFITDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec = GETSECZTSECDNTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir = GETSECZTFITUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec = GETSECZTSECUPTIME(l_u16TypeNew) + g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16SecondZTMaxValue = GETSECZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = GETSECZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = GETSECZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir==g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec)
// 				{
// 					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt=0;
// 				}
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8SecondZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8SecondZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32SecondZTSum = GETSECZTSUM(l_u16TypeNew);
				/**** end ****/
				

				//������խ��A����Ϣ�洢��B��,�����A�е���Ϣ(����ֳ������������ֻ�е�������խ�������)
				if((g_u8A2BFlag == 1 && l_u16ID <= 2) || (g_u8A2BFlag == 2 && l_u16ID >= 3 && l_u16ID <= 5))
				{
					//��Aխ������Ϣ���Ƶ�Bխ����
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec;
					g_ChlData[l_u16TriggerID].u16SecondZTMaxValue     = g_ChlData[l_u16TriggerID].u16FirstZTMaxValue;
					
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt    = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos        = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos;
					g_ChlData[l_u16TriggerID].u32SecondZTSum          = g_ChlData[l_u16TriggerID].u32FirstZTSum;
					
					//���Aխ������Ϣ
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = 0;
					g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = 0;
					
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTSum = 0;
				}
			}
			else 
			{
				g_ChlData[l_u16TriggerID].u32TimeDiffValue = g_ChlData[l_u16TriggerID].u32FirstDnTime - g_ChlData[l_u16TriggerID].u32MainFPGATime;
				g_ChlData[l_u16TriggerID].n32DiffValue = 0 - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				g_ChlData[l_u16TriggerID].u32uptm = ABGETSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32dntm = ABGETSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16ptMaxValue = ABGETMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32Maxtm  =  ABGETMAXTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32chlSum	 = 	ABGETSUM(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u32FirstDnTime -= g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				
				/**** Aխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = GETFIRSTZTFITDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = GETFIRSTZTSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = GETFIRSTZTFITUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = GETFIRSTZTSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = GETFIRSTZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = GETFIRSTZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = GETFIRSTZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir == g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir)
// 				{
// 					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt=0;
// 				}
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8FirstZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32FirstZTSum = GETFIRSTZTSUM(l_u16TypeNew);	
				/**** end ****/
				
				/**** Bխ��������Ϣ ****/
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir = GETSECZTFITDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec = GETSECZTSECDNTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir = GETSECZTFITUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec = GETSECZTSECUPTIME(l_u16TypeNew) - g_ChlData[l_u16TriggerID].u32TimeDiffValue;
				g_ChlData[l_u16TriggerID].u16SecondZTMaxValue = GETSECZTMAXDATA(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = GETSECZTMAXDATAPOS(l_u16TypeNew);
				g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = GETSECZTUPDNPOS(l_u16TypeNew);
// 				if(g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir==g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec)
// 				{
// 					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos=0;
// 					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt=0;
// 				}
				l_u8DigitZTPos = l_u8DigitZTMaxValuePos = 0;
				if(g_u8ReverseDigitZTPos == 1)    //��������խ���Ĵ���λ�ú����ֵλ��
				{
					temp = g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTPos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt = l_u8DigitZTPos;
					
					temp = g_ChlData[l_u16TriggerID].u8SecondZTMaxPos;
					for(i = 0; i < 4; i++)
					{
						if((temp & (0x08>>i)))
							l_u8DigitZTMaxValuePos |= (0x01<<i);
					}
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos = l_u8DigitZTMaxValuePos;
					
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((l_u8DigitZTMaxValuePos<<4) & 0xF0) + l_u8DigitZTPos;
				}
				else
				{
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = ((g_ChlData[l_u16TriggerID].u8SecondZTMaxPos<<4) & 0xF0) + g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt;
				}
				g_ChlData[l_u16TriggerID].u32SecondZTSum = GETSECZTSUM(l_u16TypeNew);
				/**** end ****/
				
				//������խ��A����Ϣ�洢��B��,�����A�е���Ϣ(����ֳ������������ֻ�е�������խ�������)
				if((g_u8A2BFlag == 1 && l_u16ID <= 2) || (g_u8A2BFlag == 2 && l_u16ID >= 3 && l_u16ID <= 5))
				{
					//��Aխ������Ϣ���Ƶ�Bխ����
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTDnTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmFir      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir;
					g_ChlData[l_u16TriggerID].u32SecondZTUpTmSec      = g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec;
					g_ChlData[l_u16TriggerID].u16SecondZTMaxValue     = g_ChlData[l_u16TriggerID].u16FirstZTMaxValue;
					
					g_ChlData[l_u16TriggerID].u8SecondZTUpDnPosCnt    = g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxPos        = g_ChlData[l_u16TriggerID].u8FirstZTMaxPos;
					g_ChlData[l_u16TriggerID].u8SecondZTMaxAndUpDnPos = g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos;
					g_ChlData[l_u16TriggerID].u32SecondZTSum          = g_ChlData[l_u16TriggerID].u32FirstZTSum;
					
					//���Aխ������Ϣ
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTDnTmSec = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmFir = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTUpTmSec = 0;
					g_ChlData[l_u16TriggerID].u16FirstZTMaxValue = 0;
					
					g_ChlData[l_u16TriggerID].u8FirstZTUpDnPosCnt = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxPos = 0;
					g_ChlData[l_u16TriggerID].u8FirstZTMaxAndUpDnPos = 0;
					g_ChlData[l_u16TriggerID].u32FirstZTSum = 0;
				}
			}

			
			
			g_ChlData[l_u16TriggerID].u8IsDownFlag = 1;
			g_ChlData[l_u16TriggerID].m_bUpOrDn = 0;
			g_ChlData[l_u16TriggerID].m_ulTime = g_ChlData[l_u16TriggerID].u32FirstDnTime;
			// ͨ��խ�����ȷ������32-63���0-31

			g_ChlData[l_u16TriggerID].u8Id = l_u16ID;
			g_ChlData[l_u16TriggerID].u8BoardNo = l_u8BoardNo;
			g_ChlData[l_u16TriggerID].u8ChannelNo = l_u8ChannelNo;
			g_ChlData[l_u16TriggerID].m_bDirectionFlag = 0;
			g_ChlData[l_u16TriggerID].u8BoardType = 2;
			
			

			// ����֡���ݿ���
			g_ChlData[l_u16TriggerID].u8packetCnt++;
			g_ChlData[l_u16TriggerID].u8FrameNo = ABGETFRAMENUM(l_u16TypeNew);
			if( g_ChlData[l_u16TriggerID].u16ptCnt+l_u16Cnt <= MAXBUFPOINT )
			{
				// ���߽��г�����������break
				//break;
			
				for( l_u16TempIndex=0; l_u16TempIndex<l_u16Cnt; l_u16TempIndex++ )
				{
					g_ChlData[l_u16TriggerID].u16data[0][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(AGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
					g_ChlData[l_u16TriggerID].u16data[1][g_ChlData[l_u16TriggerID].u16ptCnt + l_u16TempIndex] = *(BGETDATAADDR(l_u16TypeNew)+l_u16TempIndex);
				}
				g_ChlData[l_u16TriggerID].u16ptCnt += l_u16Cnt;
			}
			g_ChlData[l_u16TriggerID].u32realCnt += l_u16Cnt;  //��ʵ����ֵ

//#if LOGICENABLE > 0
  
			if(PROMODPARAM.m_au8CHLSET[g_ChlData[l_u16TriggerID].u8Id/PROMODPARAM.m_u8ZTRow] < 10)
			{    // liumingxin �����λ���������ĳ���Ƿ����խ���߼�
				// ��խ��->�߼�����
				ret = OSQPost(g_pQDnMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
					g_ErrMsgState |= 0x10;
			}
//#endif
			
//#if	SENDWAVEENABLE > 0
			// ����if�жϣ����������ʹ�ܣ����Ͳ��� wujinlong 20130111
			if( 1 == g_Setup.u8SendWaveEnable )
			{
				//Fag[abc++]=g_ChlData->u8Id;
				ZTRead[g_ChlData[l_u16TriggerID].u8Id]=g_u8RasterDataWriteIndex-1;
				ret = OSQPost(g_pQWaveMsg,&g_ChlData[l_u16TriggerID]);
				if(ret == OS_Q_FULL)
				{
					g_ErrMsgState |= 0x100;
				}  
			}
//#endif

			g_ChlID2TriggerID[l_u16ID] = EMPTY_ITEMZERO;
			
			break;

		default:
			break;
	}
		
	
	goto IRQ_EXIT;		//��������

IRQ_EXIT:
	// ���ж�
	ClearFPGA_INT();
	SIC2_RSR					|= 0x02; // �ǳ���Ҫ������ɾ��
}

