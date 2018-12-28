/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskWave.C
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		������
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110518
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define	__TASKWAVE_C

#include "common.h"


//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����

#define		SETUPALIAS				g_Setup			//���ò����ṹ
//#define		ADDCRCALIAS				AddCrc16
#define     PROMODPARAM				g_cProModParam




/*********************************************************************************************************
** Function name:		TaskRecWave
** Descriptions:		���Ͳ���
** input parameters:	None 
** output parameters:	none
            �����ź��������ݷ�װ�����ͳ�����
** Created by:					  
** Created Date:		20121226	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/



static sndFrame_Struct	m_WaveDataBuf[WAVE_BUFSIZE];
#define WAVE_TMPSIZE	(MAXBUFPOINT*2+100)	
static uint8  m_Wavebuf[WAVE_TMPSIZE] = {0};
static uint8	m_WaveIndex;


// add by sxh 2014.3.28	  ��ȦIO״̬
/******************add by sxh 2014.3.28*****************************/
sndFrame_Struct s_CoilOrLaserGpioBuf[WAVE_BUFSIZE];
uint8           s_CoilOrLaserGpioIndex;
/******************add by sxh 2014.3.28*****************************/


void  TaskRecWave(void *tdata)
{
	uint8	l_err;

//	uint8	subCommand = 0x00; // add by sxh 2014.3.28
//    uint16 l_tempIndex = 0x00; // add by sxh 2014.3.28

	Channal_Struct *l_pItem = 0;
	uint16  l_u16Index = 0,l_u16Len = 0,l_u16tmp;
	static  uint16 l_u16pkgNum = 0;

	uint8	l_u8Err;
	uint16	l_u16SendLen;
	
	uint8 l_u8tempi = 0;        // ��ʱ����
	uint8 l_u8BoardType = 0;    // ���ݰ忨���͸�ֵ, 1:�к����ɼ��� , 2:���ִ������ɼ���
	uint8 l_u8PackageCnt = 0;    
//	char *pbuf = 0;
	tdata = tdata; 
	
	// �ȴ���ʼ������
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// ����ʼ����
	// added by wujinlong 20130805
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u TWave Recd TaskBegin��\n",
//       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TWave Recd TaskBegin.\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );	 //g_cDebugInfo
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif

	// Log
#if LOG_UPLOAD_EN > 0
	l_u16SendLen = sprintf((char*)g_cLogInfo.m_au8DataBuf[g_cLogInfo.m_u8In],"%04u-%02u-%02u %02u:%02u:%02u TWave Recd TaskBegin��\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second); 
	g_cLogInfo.m_u16Len[g_cLogInfo.m_u8In] = l_u16SendLen;
	g_cLogInfo.m_u8In = (g_cLogInfo.m_u8In+1) % LOG_NUM;
#endif
	
	m_WaveIndex = 0;   //��������
	s_CoilOrLaserGpioIndex = 0;     // add by sxh  2014.3.28
	while(1)
	{
		OSTimeDly(10*OS_TICKS_PER_SEC/1000);
#if    TASKTIMEENABLE   // ͳ�Ƽ�ʱʹ��	 ��������������
		if(1==TaskRunTimeStatistics.m_ucTaskWaveFlag ){//1:��ʼ����״̬
			OSSchedLock();
			if(0x01!=T1TCR){ //????
				T1TCR=0x02;
				T1TCR=0x01;
			}			
			TaskRunTimeStatistics.m_ucTaskWaveFlag=2;   //�л�������״̬
			TaskRunTimeStatistics.m_ulTaskWaveDet=0;
			
			TaskRunTimeStatistics.m_ulTaskWave1=T1TC;
			OSSchedUnlock();				
		}
		else if(2==TaskRunTimeStatistics.m_ucTaskWaveFlag){
		    OSSchedLock();
			TaskRunTimeStatistics.m_ulTaskWave1=T1TC; //����״̬�и���
			OSSchedUnlock();
		}
#endif		
	

	   // add by sxh  2014.3.27 �ڷ��Ͳ���֮ǰ�ȼ���Ƿ�����Ȧ������Ҫ����
	   // del wujinlong 20140517
/*	   	if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn)  //������
		{
			l_tempIndex = 0x00; 
			memset(&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex],0,sizeof(sndFrame_Struct));
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].u8Cnt = 1;		    //��ʼ��֡
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;  //�ӻ���ַ
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xAA;	 //�����

			subCommand = g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u8Flag;	  //��ȡ�ṹ���б�־
			switch(subCommand) //0x02;����λ�����ݣ� 0x03������IO���ݣ� 0x04����ȦIO����	 
			{
				case 0x02:
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x02;	 //�����
					
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x6A;   //����֡����2�ֽ�

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;					
					for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp -1) * 8) & 0xFF;
					}

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;  //�����־λ
					
					for(l_u16tmp = 0; l_u16tmp < (LASTERCOUNT<<2); l_u16tmp++)  //����λ������
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
						                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u8type;
						
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
						                    (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Ltpos & 0xFF00) >>8;
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
						                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Ltpos & 0x00FF;

						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
						                    (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Rtpos & 0xFF00) >>8;
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
						                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Rtpos & 0x00FF;
						                  		
					}

					for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 // Ԥ���ֶ�
					}
					AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);	   // add by sxh 2014.3.29

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x6A;
					 
					break;
				}
				case 0x03:  //����IO
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x03;	 //�����

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 //

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


				   	for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)   //ʱ��
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp - 1) * 8) & 0xFF;
					}
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // �����־IO
					
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState >> 8) & 0xFF;
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState ) & 0xFF;
					for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xA5;
					}

					AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x1c;
					break;
				}
				case 0x04:
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x04;	 //�������

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 // �ݲ���ֵ�����ֵ

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


				   	for(l_u16tmp = 4; l_u16tmp >0; l_u16tmp--)   //ʱ��
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp-1) * 8) & 0xFF;
					}
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // ��Ȧ��־IO
					
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState >> 8) & 0xFF;
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState ) & 0xFF;
					for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xA5;
					}

					AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x1c;
					break;
				}
			}
			g_CoilOrLaserData.m_u8ReadOut = (g_CoilOrLaserData.m_u8ReadOut + 1) % COILGPIODATALEN;   // ��λ������

			l_err = OSQPost(g_pQNetWavePC,&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex]);  //�׷��Ͳ��Σ�IO������λ�ã�
			if(OS_Q_FULL == l_err)
				g_ErrMsgState |= 0x200;
			s_CoilOrLaserGpioIndex = (s_CoilOrLaserGpioIndex +1) % WAVE_BUFSIZE; 

		}//if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn) 
*/		
		/**********************************add by sxh for above  *******************************************/
	   	
	 


		//l_pItem = (Channal_Struct *)OSQPend(g_pQWaveMsg,0,&l_err);		//���Ͳ����ź�	l_pItem,խ�����ݽṹ
		l_pItem	= (Channal_Struct *)OSQAccept(g_pQWaveMsg);		//�������̣�����Ϣ
		//if(OS_NO_ERR == l_err) 
		if (l_pItem != (Channal_Struct *)0)
		{
			// wujinlong 20140517������IO
			// begin
			if( 0xAB == l_pItem->u8Id )
			{
				m_WaveDataBuf[m_WaveIndex].u8Cnt = 1;
				m_WaveDataBuf[m_WaveIndex].au32len[0] = l_pItem->u16ptCnt;
				for(l_u16tmp = 0;l_u16tmp < l_pItem->u16ptCnt;l_u16tmp++)
				{
					m_WaveDataBuf[m_WaveIndex].au8data[0][l_u16tmp] = ((l_pItem->u16data[0][l_u16tmp]) & 0xFF);
				}
				l_err = OSQPost(g_pQNetWavePC,&m_WaveDataBuf[m_WaveIndex]);  ////���Ͳ������ݶ���
				if(OS_Q_FULL == l_err)
					g_ErrMsgState |= 0x200;
				m_WaveIndex ++;
				m_WaveIndex %= WAVE_BUFSIZE; 
				//
				continue;
			}
			else if( 0xAC == l_pItem->u8Id )
			{
				m_WaveDataBuf[m_WaveIndex].u8Cnt = 1;
				m_WaveDataBuf[m_WaveIndex].au32len[0] = l_pItem->u16ptCnt;
				for(l_u16tmp = 0;l_u16tmp < l_pItem->u16ptCnt;l_u16tmp++)
				{
					m_WaveDataBuf[m_WaveIndex].au8data[0][l_u16tmp] = ((l_pItem->u16data[0][l_u16tmp]) & 0xFF);
				}
				l_err = OSQPost(g_pQNetWavePC,&m_WaveDataBuf[m_WaveIndex]);  ////���Ͳ������ݶ���
				if(OS_Q_FULL == l_err)
					g_ErrMsgState |= 0x200;
				m_WaveIndex ++;
				m_WaveIndex %= WAVE_BUFSIZE; 
				//
				continue;
			}
			else
			{
			}
			// end
			
			switch(l_pItem->u8BoardType)
			{
				case 2: 
					l_u8BoardType = 2;
					break;
				default :
					l_u8BoardType = 1;
					break;
			}
			l_u8PackageCnt = 0;
			for(l_u8tempi = 0; l_u8tempi < l_u8BoardType; l_u8tempi++)
			{
				l_u16pkgNum = 0;		// ֡�����0

				//���ݷ�װ
				l_u16Index = 0;
				memset(m_Wavebuf,0,WAVE_TMPSIZE);  

	//#if TEST_NOTWAVE_ZTINFO_EN > 0
	// ���ԣ�����խ����Ϣ

	//ÿ��խ�������ݽṹ


				// del by sxh 2014.4.08
				/*m_Wavebuf[l_u16Index++]	= l_pItem->u8Id;  //խ�����
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u16SelfID>>8) & 0xFF; // ��¼��խ��λ�ã����������е���ţ�һֱ���䣬��խ��ʱ�ṩλ����Ϣ
				m_Wavebuf[l_u16Index++] = (l_pItem->u16SelfID) & 0xFF;
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>24)& 0xFF;  //խ�����ֺ�
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>16)& 0xFF;  //խ�����ֺ�	
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>8) & 0xFF;  //խ�����ֺ�
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum) & 0xFF;     //խ�����ֺ�
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>24)& 0xFF;	   //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>16)& 0xFF;	   //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>8) & 0xFF;	   //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm) & 0xFF;       //�ϳ�ʱ��
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>24)& 0xFF;	   //�³�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>16)& 0xFF;	   //�³�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>8) & 0xFF;	   //�³�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm) & 0xFF;	   //�³�ʱ��
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptMaxValue>>8) & 0xFF; //��ֵ
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptMaxValue) & 0xFF;	  //��ֵ  
				*/	 //end  del
	//#else
				// ��ʼ��֡
				//if((g_u8RasterZTRead-2)!=g_u8RasterRead)
				//{
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0xff000000)>>24;	//����ʱ�� //���⴫����������λ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x00ff0000)>>16;	//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x0000ff00)>>8;    //����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x000000ff;			//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u8Cnt;// ������
	// 				for(l_u16tmp=0;l_u16tmp<g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u8Cnt;l_u16tmp++)
	// 				{
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].m_u8VehID;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].u8type;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].u16Ltpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].u16Ltpos&0xff;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].u16rtpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].headinfo[l_u16tmp].u16rtpos&0xff;
	// 				}

					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
				//}
				//if((g_u8RasterZTRead-1)!=g_u8RasterRead)
				//{
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0xff000000)>>24;	//����ʱ�� //���⴫����������λ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x00ff0000)>>16;	//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x0000ff00)>>8;    //����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x000000ff;			//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[ZTRead[l_pItem->u8Id]].u8Cnt;// ������
	// 				for(l_u16tmp=0;l_u16tmp<g_RasterData[ZTRead[l_pItem->u8Id]].u8Cnt;l_u16tmp++)
	// 				{
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].m_u8VehID;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].u8type;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].u16Ltpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].u16Ltpos&0xff;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].u16rtpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[ZTRead[l_pItem->u8Id]].headinfo[l_u16tmp].u16rtpos&0xff;
	// 				}

					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
				//}
				//if((((ZTRead[l_pItem->u8Id]+1)<=(g_u8RasterDataWriteIndex-1))&&(ZTRead[l_pItem->u8Id]+1!=255))||(WFlag[l_pItem->u8Id]==1))
	// 				if(((g_u8RasterDataWriteIndex-1+256)&0xFF)!=ZTRead[l_pItem->u8Id])
	// 			{
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0xff000000)>>24;	//����ʱ�� //���⴫����������λ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x00ff0000)>>16;	//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x0000ff00)>>8;    //����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x000000ff;			//����ʱ��
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u8Cnt;// ������
	// 				for(l_u16tmp=0;l_u16tmp<g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u8Cnt;l_u16tmp++)
	// 				{
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].m_u8VehID;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].u8type;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].u16Ltpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].u16Ltpos&0xff;
	// 					m_Wavebuf[l_u16Index++]=(g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].u16rtpos>>8)&0xff;
	// 					m_Wavebuf[l_u16Index++]=g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].headinfo[l_u16tmp].u16rtpos&0xff;
	// 				}
	// 			}
	// 			else
	// 			{
	// 				m_Wavebuf[l_u16Index++] =0;
	// 				m_Wavebuf[l_u16Index++] =0;
	// 				m_Wavebuf[l_u16Index++] =0;
	// 				m_Wavebuf[l_u16Index++] =0;
	// 				m_Wavebuf[l_u16Index++] =0;
	// 			}

					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
					m_Wavebuf[l_u16Index++] =0;
				
	// 			for( l_u16tmp=0; l_u16tmp<CHANNELNUM; l_u16tmp++ )            //CHANNELNUM,խ������
	// 			{
	// 				m_Wavebuf[l_u16Index++] = (g_Setup.an32AxGain[l_u16tmp]>>8) & 0xFF;	 //an32AxGain,���� ��g_Setup ARM����
	// 				m_Wavebuf[l_u16Index++] = (g_Setup.an32AxGain[l_u16tmp]) & 0xFF;
	// 			}
	// 			for( l_u16tmp=0; l_u16tmp<CHANNELNUM; l_u16tmp++ )
	// 			{
	// 				//m_Wavebuf[l_u16Index++] = (g_Setup.an32Zero[l_u16tmp]>>8) & 0xFF;
	// 				//m_Wavebuf[l_u16Index++] = (g_Setup.an32Zero[l_u16tmp]) & 0xFF;		   
	// 				m_Wavebuf[l_u16Index++] = (GETCURRENTZERO((l_u16tmp>>3)&0x07,l_u16tmp&0x07)>>8) & 0xFF;  //���ֵ
	// 				m_Wavebuf[l_u16Index++] = (GETCURRENTZERO((l_u16tmp>>3)&0x07,l_u16tmp&0x07)) & 0xFF;     //���ֵ
	// 			}
									
				//g_u8RasterDataReadIndex = (g_u8RasterDataWriteIndex - 1 + 256)& 0xFF;    ////��������Ϣ	���ڲ��η��Ͷ�ȡ����  ��δ��ʼ��  //����дλ��������µĶ�ȡλ��	
	// 			if(g_u16SystemUseMode == 1 || g_u16SystemUseMode == 3)    // add by sxh  ʹ��ģʽ	 2014.4.01
	// 			{
	// 				m_Wavebuf[l_u16Index++]	 = 1;	//1,�м���λ�����ݣ�0��û�м���λ������
	// 			} else 
	// 			{
	// 				m_Wavebuf[l_u16Index++]	 = 0;	//1,�м���λ�����ݣ�0��û�м���λ������	
	// 			}
			
				
				
				//m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].u8Cnt;// ������
				
				
	// 			for( l_u16tmp=0; l_u16tmp<(LASTERCOUNT<<2); l_u16tmp++ )	// ���г���λ����Ϣ	��LASTERCOUNT������������  4����������λ������
	// 			{
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u8type;   //����1�¼�
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16Ltpos & 0xff00)>>8;//�������
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16Ltpos & 0x00ff;
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16rtpos & 0xff00)>>8;//�Ҳ�����
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16rtpos & 0x00ff;
	// 			}
					
				
	 

				//m_Wavebuf[l_u16Index++]	= l_pItem->u8Id;    //խ�����
			   // m_Wavebuf[l_u16Index++]	= 0x00;  // edit by sxh  խ�����  --->  Ԥ���ֶ�	
				
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>24)& 0xFF;  //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>16)& 0xFF;	  //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>8) & 0xFF;	  //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = (l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1) & 0xFF;	   //�ϳ�ʱ��
				m_Wavebuf[l_u16Index++] = 0x00; //������ʾ													 	 
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptCnt >>8) &0xFF;    //խ���������ݵ���
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptCnt) &0xFF;	   //խ���������ݵ���
				for(l_u16tmp = 0;l_u16tmp < l_pItem->u16ptCnt;l_u16tmp++)
				{
					m_Wavebuf[l_u16Index++] = (l_pItem->u16data[l_u8tempi][l_u16tmp]>>8) & 0xFF;	//խ����ֵ����
					m_Wavebuf[l_u16Index++] = (l_pItem->u16data[l_u8tempi][l_u16tmp]) & 0xFF;
				}
	//			AddCrc16(m_Wavebuf,l_u16Index);
	//#endif


				// ��ʼ�ְ�
				if(l_u8PackageCnt == 0)
				{
					memset(&m_WaveDataBuf[m_WaveIndex],0,sizeof(sndFrame_Struct));
				}
				
				l_u16Len = 	(l_u16Index)%SNDFRAMEBESTSIZE;	 ////��ѷ�������֡���ȣ��˳���Ϊ�������ݳ���
				m_WaveDataBuf[m_WaveIndex].u8Cnt = 	(l_u16Index)/SNDFRAMEBESTSIZE;
				if(l_u16Len ==0)//��ʱ����ɼ�������һ֡��֡����ƴ֡����ֲ����ļ���խ����խ�������쳣������20171205
				{
					if(m_WaveDataBuf[m_WaveIndex].u8Cnt>0)
					{
						m_WaveDataBuf[m_WaveIndex].u8Cnt--;
						l_u16Len = SNDFRAMEBESTSIZE - 1;
					}
				}

				l_u16Index = 0;
				for(l_u16tmp = l_u8PackageCnt;l_u16tmp<m_WaveDataBuf[m_WaveIndex].u8Cnt+l_u8PackageCnt;l_u16tmp++)	   //�ְ�
				{  			
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][0] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][1] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][2] = 0x00;  //�ӻ���ַ
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][3] = 0xAA;	 //�����
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][4] = 0x01;	 //�������

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][5] = ((SNDFRAMEBESTSIZE+26)>>8) & 0xFF; //����֡����
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][6] = (SNDFRAMEBESTSIZE+26) & 0xFF;		 //����֡����

					
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][7] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][8] = (g_u16SystemUseMode ) & 0xFF;	   //�����ֽ�ʹ��ģʽ

					  
//					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = l_u16tmp ? COMMSENDWAVETRANS:COMMSENDWAVESTART; //֡��־    //������,ABխ����һ�����ݵ����೬��1��ʱ���ڶ���խ�������м�֡��ʼ
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = (l_u16tmp > l_u8PackageCnt) ? COMMSENDWAVETRANS:COMMSENDWAVESTART; //֡��־
					
					/*****************�޸�֡��ʶ add by sxh 2014.3.27**************/


					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][10] = (l_u16pkgNum>>8 ) &  0xFF;     //֡���
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][11] = (l_u16pkgNum ) &  0xFF;		 //֡���
					l_u16pkgNum++;
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = 0x01;   //ͨ������
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][13] = 0x00;   //Ԥ��	  // del by sxh  (2014.4.08)
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][14] = 0x00;   //Ԥ��
					if(l_u8BoardType == 2 && l_pItem->u8BoardNo > 0)
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = 8+(l_pItem->u8BoardNo-1)*12+l_pItem->u8ChannelNo*2+l_u8tempi;
					}
					else
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = l_pItem->u8ChannelNo;
					}

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][13]=PROMODPARAM.m_au8CHLSET[l_pItem->u8Id/PROMODPARAM.m_u8ZTRow];
			
					m_WaveDataBuf[m_WaveIndex].au32len[l_u16tmp] = SNDFRAMEBESTSIZE+26;     //????????
					//
					memcpy(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp]+14,m_Wavebuf+l_u16Index,SNDFRAMEBESTSIZE);	 //������������

					if((l_u8BoardType == 2) && (l_pItem->u8BoardNo > 0) && (l_u8tempi == 0))
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+14] = l_pItem->u8FirstZTMaxPos;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+15] = l_pItem->u8FirstZTUpDnPosCnt;
					}
					else if((l_u8BoardType == 2) && (l_pItem->u8BoardNo > 0) && (l_u8tempi == 1))
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+14] = l_pItem->u8SecondZTMaxPos;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+15] = l_pItem->u8SecondZTUpDnPosCnt;
					}
					else
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+14] = 0x00;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+15] = 0x00;
					}
					
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+16] = 0x00;
					
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+17] = 0x00;	 
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+18] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+19] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+20] = 0x00;	
					
					
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+21] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+22] = 0x00;	
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+23] = 0x00;	 //10�ֽ�Ԥ������
					
					//
					AddCrc16(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp],SNDFRAMEBESTSIZE+24);
					l_u16Index += SNDFRAMEBESTSIZE;				
				
				}

				if(l_u16Len) //�����һ�����ݰ�
				{
					m_WaveDataBuf[m_WaveIndex].u8Cnt++;	
				
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][0] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][1] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][2] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][3] = 0xAA;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][4] = 0x01;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][5] = ((l_u16Len+26)>>8) & 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][6] = (l_u16Len+26) & 0xFF;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][7] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][8] = (g_u16SystemUseMode ) & 0xFF;
					

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = COMMSENDWAVEEND;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][10] = (l_u16pkgNum>>8 ) &  0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][11] = (l_u16pkgNum ) &  0xFF;
					l_u16pkgNum++;
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = 0x01;   // del by sxh 2014.4.08
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][13] = 0x00;   // del by sxh 2014.4.03
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][14] = 0x00;	 // del by sxh 2014.4.03
					
					if(l_u8BoardType == 2 && l_pItem->u8BoardNo > 0)
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = 8+(l_pItem->u8BoardNo-1)*12+l_pItem->u8ChannelNo*2+l_u8tempi;
					}
					else
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = l_pItem->u8ChannelNo;
					}
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][13]=PROMODPARAM.m_au8CHLSET[l_pItem->u8Id/PROMODPARAM.m_u8ZTRow];
					
					m_WaveDataBuf[m_WaveIndex].au32len[l_u16tmp] = l_u16Len + 26;
					memcpy(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp]+14,m_Wavebuf+l_u16Index,l_u16Len);
					if((l_u8BoardType == 2) && (l_pItem->u8BoardNo > 0) && (l_u8tempi == 0))
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+14] = l_pItem->u8FirstZTMaxPos;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+15] = l_pItem->u8FirstZTUpDnPosCnt;
					}
					else if((l_u8BoardType == 2) && (l_pItem->u8BoardNo > 0) && (l_u8tempi == 1))
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+14] = l_pItem->u8SecondZTMaxPos;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+15] = l_pItem->u8SecondZTUpDnPosCnt;
					}
					else
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+14] = 0x00;
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+15] = 0x00;
					}
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+16] = 0x00;
												
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+17] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+18] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+19] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+20] = 0x00;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+21] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+22] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+23] = 0x00;	 //10�ֽ�Ԥ������

					AddCrc16(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp],l_u16Len+24);
				}
				else
				{
					if(l_u16tmp>0)    //ֻ��1����������֡����ʱ�ð�Ϊ����֡
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp-1][9] = COMMSENDWAVEEND;
					}
					else
					{
						m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = COMMSENDWAVEEND;
					}
				}
				l_u8PackageCnt += m_WaveDataBuf[m_WaveIndex].u8Cnt;	
			}
			m_WaveDataBuf[m_WaveIndex].u8Cnt = l_u8PackageCnt;
			l_err = OSQPost(g_pQNetWavePC, &m_WaveDataBuf[m_WaveIndex]);  ////���Ͳ������ݶ���
			if(OS_Q_FULL == l_err)
				g_ErrMsgState |= 0x200;
			m_WaveIndex ++;
			m_WaveIndex %= WAVE_BUFSIZE;  // add by sxh 2014.4.01
			//WFlag[l_pItem->u8Id]=0;
		}
		
#if	TASKTIMEENABLE
		if(2==TaskRunTimeStatistics.m_ucTaskWaveFlag){//2:����״̬ ?????
			OSSchedLock();
			T1TCR=0x00;
			TaskRunTimeStatistics.m_ulTaskWaveDet=(RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskWave1)>TaskRunTimeStatistics.m_ulTaskWaveDet)?RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskWave1):TaskRunTimeStatistics.m_ulTaskWaveDet; 				
			T1TCR=0x01;	
			OSSchedUnlock();
		}
#endif 	
	}	
}	

//#endif	//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����

