/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskWave.C
** Last modified Date:  20110518
** Last Version:		1.0
** Descriptions:		发波形
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


//#if	SENDWAVEENABLE > 0		//使能发波形

#define		SETUPALIAS				g_Setup			//设置参数结构
//#define		ADDCRCALIAS				AddCrc16
#define     PROMODPARAM				g_cProModParam




/*********************************************************************************************************
** Function name:		TaskRecWave
** Descriptions:		发送波形
** input parameters:	None 
** output parameters:	none
            接收信号量，数据封装，发送出数据
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


// add by sxh 2014.3.28	  线圈IO状态
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
	
	uint8 l_u8tempi = 0;        // 临时变量
	uint8 l_u8BoardType = 0;    // 根据板卡类型赋值, 1:中航电测采集板 , 2:数字传感器采集板
	uint8 l_u8PackageCnt = 0;    
//	char *pbuf = 0;
	tdata = tdata; 
	
	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// 任务开始运行
	// added by wujinlong 20130805
//	wj_Printf("%04u-%02u-%02u %02u:%02u:%02u TWave Recd TaskBegin。\n",
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
	l_u16SendLen = sprintf((char*)g_cLogInfo.m_au8DataBuf[g_cLogInfo.m_u8In],"%04u-%02u-%02u %02u:%02u:%02u TWave Recd TaskBegin。\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second); 
	g_cLogInfo.m_u16Len[g_cLogInfo.m_u8In] = l_u16SendLen;
	g_cLogInfo.m_u8In = (g_cLogInfo.m_u8In+1) % LOG_NUM;
#endif
	
	m_WaveIndex = 0;   //波形索引
	s_CoilOrLaserGpioIndex = 0;     // add by sxh  2014.3.28
	while(1)
	{
		OSTimeDly(10*OS_TICKS_PER_SEC/1000);
#if    TASKTIMEENABLE   // 统计计时使能	 ？？？？？功能
		if(1==TaskRunTimeStatistics.m_ucTaskWaveFlag ){//1:开始计数状态
			OSSchedLock();
			if(0x01!=T1TCR){ //????
				T1TCR=0x02;
				T1TCR=0x01;
			}			
			TaskRunTimeStatistics.m_ucTaskWaveFlag=2;   //切换到结算状态
			TaskRunTimeStatistics.m_ulTaskWaveDet=0;
			
			TaskRunTimeStatistics.m_ulTaskWave1=T1TC;
			OSSchedUnlock();				
		}
		else if(2==TaskRunTimeStatistics.m_ucTaskWaveFlag){
		    OSSchedLock();
			TaskRunTimeStatistics.m_ulTaskWave1=T1TC; //结算状态中更新
			OSSchedUnlock();
		}
#endif		
	

	   // add by sxh  2014.3.27 在发送波形之前先检查是否有线圈数据需要发送
	   // del wujinlong 20140517
/*	   	if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn)  //有数据
		{
			l_tempIndex = 0x00; 
			memset(&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex],0,sizeof(sndFrame_Struct));
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].u8Cnt = 1;		    //开始组帧
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;  //从机地址
			s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xAA;	 //命令号

			subCommand = g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u8Flag;	  //获取结构体中标志
			switch(subCommand) //0x02;激光位置数据； 0x03：激光IO数据； 0x04：线圈IO数据	 
			{
				case 0x02:
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x02;	 //命令号
					
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //数据帧长度2字节
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x6A;   //数据帧长度2字节

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //当前使用模式
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;					
					for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp -1) * 8) & 0xFF;
					}

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;  //激光标志位
					
					for(l_u16tmp = 0; l_u16tmp < (LASTERCOUNT<<2); l_u16tmp++)  //激光位置数据
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
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 // 预留字段
					}
					AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);	   // add by sxh 2014.3.29

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x6A;
					 
					break;
				}
				case 0x03:  //激光IO
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x03;	 //命令号

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //数据帧长度2字节
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 //

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //当前使用模式
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


				   	for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)   //时间
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp - 1) * 8) & 0xFF;
					}
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // 激光标志IO
					
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
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x04;	 //子命令号

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //数据帧长度2字节
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 // 暂不赋值，最后赋值

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //当前使用模式
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


				   	for(l_u16tmp = 4; l_u16tmp >0; l_u16tmp--)   //时间
					{
						s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
						                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp-1) * 8) & 0xFF;
					}
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // 线圈标志IO
					
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
			g_CoilOrLaserData.m_u8ReadOut = (g_CoilOrLaserData.m_u8ReadOut + 1) % COILGPIODATALEN;   // 读位置自增

			l_err = OSQPost(g_pQNetWavePC,&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex]);  //抛发送波形（IO，激光位置）
			if(OS_Q_FULL == l_err)
				g_ErrMsgState |= 0x200;
			s_CoilOrLaserGpioIndex = (s_CoilOrLaserGpioIndex +1) % WAVE_BUFSIZE; 

		}//if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn) 
*/		
		/**********************************add by sxh for above  *******************************************/
	   	
	 


		//l_pItem = (Channal_Struct *)OSQPend(g_pQWaveMsg,0,&l_err);		//发送波形信号	l_pItem,窄条数据结构
		l_pItem	= (Channal_Struct *)OSQAccept(g_pQWaveMsg);		//过车过程，轴信息
		//if(OS_NO_ERR == l_err) 
		if (l_pItem != (Channal_Struct *)0)
		{
			// wujinlong 20140517，发送IO
			// begin
			if( 0xAB == l_pItem->u8Id )
			{
				m_WaveDataBuf[m_WaveIndex].u8Cnt = 1;
				m_WaveDataBuf[m_WaveIndex].au32len[0] = l_pItem->u16ptCnt;
				for(l_u16tmp = 0;l_u16tmp < l_pItem->u16ptCnt;l_u16tmp++)
				{
					m_WaveDataBuf[m_WaveIndex].au8data[0][l_u16tmp] = ((l_pItem->u16data[0][l_u16tmp]) & 0xFF);
				}
				l_err = OSQPost(g_pQNetWavePC,&m_WaveDataBuf[m_WaveIndex]);  ////发送波形数据队列
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
				l_err = OSQPost(g_pQNetWavePC,&m_WaveDataBuf[m_WaveIndex]);  ////发送波形数据队列
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
				l_u16pkgNum = 0;		// 帧序号清0

				//数据封装
				l_u16Index = 0;
				memset(m_Wavebuf,0,WAVE_TMPSIZE);  

	//#if TEST_NOTWAVE_ZTINFO_EN > 0
	// 测试，发送窄条信息

	//每根窄条的数据结构


				// del by sxh 2014.4.08
				/*m_Wavebuf[l_u16Index++]	= l_pItem->u8Id;  //窄条编号
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u16SelfID>>8) & 0xFF; // 记录本窄条位置，即在数组中的序号，一直不变，下窄条时提供位置信息
				m_Wavebuf[l_u16Index++] = (l_pItem->u16SelfID) & 0xFF;
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>24)& 0xFF;  //窄条积分和
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>16)& 0xFF;  //窄条积分和	
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum>>8) & 0xFF;  //窄条积分和
				m_Wavebuf[l_u16Index++] = (l_pItem->u32chlSum) & 0xFF;     //窄条积分和
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>24)& 0xFF;	   //上称时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>16)& 0xFF;	   //上称时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm>>8) & 0xFF;	   //上称时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32uptm) & 0xFF;       //上称时刻
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>24)& 0xFF;	   //下秤时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>16)& 0xFF;	   //下秤时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm>>8) & 0xFF;	   //下秤时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32dntm) & 0xFF;	   //下秤时刻
				//
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptMaxValue>>8) & 0xFF; //最值
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptMaxValue) & 0xFF;	  //最值  
				*/	 //end  del
	//#else
				// 开始组帧
				//if((g_u8RasterZTRead-2)!=g_u8RasterRead)
				//{
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0xff000000)>>24;	//触发时间 //激光传送来的数据位置
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x00ff0000)>>16;	//触发时刻
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x0000ff00)>>8;    //触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u32tm & 0x000000ff;			//触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]-1+256)&0xff].u8Cnt;// 车辆数
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
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0xff000000)>>24;	//触发时间 //激光传送来的数据位置
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x00ff0000)>>16;	//触发时刻
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x0000ff00)>>8;    //触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[ZTRead[l_pItem->u8Id]].u32tm & 0x000000ff;			//触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[ZTRead[l_pItem->u8Id]].u8Cnt;// 车辆数
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
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0xff000000)>>24;	//触发时间 //激光传送来的数据位置
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x00ff0000)>>16;	//触发时刻
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x0000ff00)>>8;    //触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u32tm & 0x000000ff;			//触发时刻
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(ZTRead[l_pItem->u8Id]+1+256)&0xff].u8Cnt;// 车辆数
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
				
	// 			for( l_u16tmp=0; l_u16tmp<CHANNELNUM; l_u16tmp++ )            //CHANNELNUM,窄条数量
	// 			{
	// 				m_Wavebuf[l_u16Index++] = (g_Setup.an32AxGain[l_u16tmp]>>8) & 0xFF;	 //an32AxGain,增益 ；g_Setup ARM参数
	// 				m_Wavebuf[l_u16Index++] = (g_Setup.an32AxGain[l_u16tmp]) & 0xFF;
	// 			}
	// 			for( l_u16tmp=0; l_u16tmp<CHANNELNUM; l_u16tmp++ )
	// 			{
	// 				//m_Wavebuf[l_u16Index++] = (g_Setup.an32Zero[l_u16tmp]>>8) & 0xFF;
	// 				//m_Wavebuf[l_u16Index++] = (g_Setup.an32Zero[l_u16tmp]) & 0xFF;		   
	// 				m_Wavebuf[l_u16Index++] = (GETCURRENTZERO((l_u16tmp>>3)&0x07,l_u16tmp&0x07)>>8) & 0xFF;  //零点值
	// 				m_Wavebuf[l_u16Index++] = (GETCURRENTZERO((l_u16tmp>>3)&0x07,l_u16tmp&0x07)) & 0xFF;     //零点值
	// 			}
									
				//g_u8RasterDataReadIndex = (g_u8RasterDataWriteIndex - 1 + 256)& 0xFF;    ////读数据信息	用于波形发送读取数据  均未初始化  //根据写位置求得最新的读取位置	
	// 			if(g_u16SystemUseMode == 1 || g_u16SystemUseMode == 3)    // add by sxh  使用模式	 2014.4.01
	// 			{
	// 				m_Wavebuf[l_u16Index++]	 = 1;	//1,有激光位置数据；0，没有激光位置数据
	// 			} else 
	// 			{
	// 				m_Wavebuf[l_u16Index++]	 = 0;	//1,有激光位置数据；0，没有激光位置数据	
	// 			}
			
				
				
				//m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].u8Cnt;// 车辆数
				
				
	// 			for( l_u16tmp=0; l_u16tmp<(LASTERCOUNT<<2); l_u16tmp++ )	// 所有车辆位置信息	；LASTERCOUNT：激光器数量  4个激光器的位置数据
	// 			{
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u8type;   //车辆1事件
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16Ltpos & 0xff00)>>8;//左侧进入点
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16Ltpos & 0x00ff;
	// 				m_Wavebuf[l_u16Index++] = (g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16rtpos & 0xff00)>>8;//右侧进入点
	// 				m_Wavebuf[l_u16Index++] = g_RasterData[(g_u8RasterZTRead-2+256)].headinfo[l_u16tmp].u16rtpos & 0x00ff;
	// 			}
					
				
	 

				//m_Wavebuf[l_u16Index++]	= l_pItem->u8Id;    //窄条编号
			   // m_Wavebuf[l_u16Index++]	= 0x00;  // edit by sxh  窄条编号  --->  预留字段	
				
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>24)& 0xFF;  //上称时刻
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>16)& 0xFF;	  //上称时刻
				m_Wavebuf[l_u16Index++] = ((l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1)>>8) & 0xFF;	  //上称时刻
				m_Wavebuf[l_u16Index++] = (l_pItem->u32FirstDnTime - l_pItem->u32realCnt +1) & 0xFF;	   //上称时刻
				m_Wavebuf[l_u16Index++] = 0x00; //采样标示													 	 
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptCnt >>8) &0xFF;    //窄条存数数据点数
				m_Wavebuf[l_u16Index++] = (l_pItem->u16ptCnt) &0xFF;	   //窄条存数数据点数
				for(l_u16tmp = 0;l_u16tmp < l_pItem->u16ptCnt;l_u16tmp++)
				{
					m_Wavebuf[l_u16Index++] = (l_pItem->u16data[l_u8tempi][l_u16tmp]>>8) & 0xFF;	//窄条点值序列
					m_Wavebuf[l_u16Index++] = (l_pItem->u16data[l_u8tempi][l_u16tmp]) & 0xFF;
				}
	//			AddCrc16(m_Wavebuf,l_u16Index);
	//#endif


				// 开始分包
				if(l_u8PackageCnt == 0)
				{
					memset(&m_WaveDataBuf[m_WaveIndex],0,sizeof(sndFrame_Struct));
				}
				
				l_u16Len = 	(l_u16Index)%SNDFRAMEBESTSIZE;	 ////最佳发送数据帧长度，此长度为数据内容长度
				m_WaveDataBuf[m_WaveIndex].u8Cnt = 	(l_u16Index)/SNDFRAMEBESTSIZE;
				if(l_u16Len ==0)//临时解决采集软件最后一帧满帧导致拼帧后出现波形文件丢窄条或窄条数据异常的问题20171205
				{
					if(m_WaveDataBuf[m_WaveIndex].u8Cnt>0)
					{
						m_WaveDataBuf[m_WaveIndex].u8Cnt--;
						l_u16Len = SNDFRAMEBESTSIZE - 1;
					}
				}

				l_u16Index = 0;
				for(l_u16tmp = l_u8PackageCnt;l_u16tmp<m_WaveDataBuf[m_WaveIndex].u8Cnt+l_u8PackageCnt;l_u16tmp++)	   //分包
				{  			
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][0] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][1] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][2] = 0x00;  //从机地址
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][3] = 0xAA;	 //命令号
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][4] = 0x01;	 //子命令号

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][5] = ((SNDFRAMEBESTSIZE+26)>>8) & 0xFF; //数据帧长度
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][6] = (SNDFRAMEBESTSIZE+26) & 0xFF;		 //数据帧长度

					
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][7] = (g_u16SystemUseMode >>8) & 0xFF; //当前使用模式
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][8] = (g_u16SystemUseMode ) & 0xFF;	   //两个字节使用模式

					  
//					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = l_u16tmp ? COMMSENDWAVETRANS:COMMSENDWAVESTART; //帧标志    //有问题,AB窄条第一个数据点数多超过1包时，第二个窄条将从中间帧开始
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][9] = (l_u16tmp > l_u8PackageCnt) ? COMMSENDWAVETRANS:COMMSENDWAVESTART; //帧标志
					
					/*****************修改帧标识 add by sxh 2014.3.27**************/


					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][10] = (l_u16pkgNum>>8 ) &  0xFF;     //帧序号
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][11] = (l_u16pkgNum ) &  0xFF;		 //帧序号
					l_u16pkgNum++;
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][12] = 0x01;   //通道数量
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][13] = 0x00;   //预留	  // del by sxh  (2014.4.08)
					//m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][14] = 0x00;   //预留
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
					memcpy(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp]+14,m_Wavebuf+l_u16Index,SNDFRAMEBESTSIZE);	 //拷贝波形数据

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
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][SNDFRAMEBESTSIZE+23] = 0x00;	 //10字节预留数据
					
					//
					AddCrc16(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp],SNDFRAMEBESTSIZE+24);
					l_u16Index += SNDFRAMEBESTSIZE;				
				
				}

				if(l_u16Len) //组最后一个数据包
				{
					m_WaveDataBuf[m_WaveIndex].u8Cnt++;	
				
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][0] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][1] = 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][2] = 0x00;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][3] = 0xAA;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][4] = 0x01;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][5] = ((l_u16Len+26)>>8) & 0xFF;
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][6] = (l_u16Len+26) & 0xFF;

					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][7] = (g_u16SystemUseMode >>8) & 0xFF; //当前使用模式
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
					m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp][l_u16Len+23] = 0x00;	 //10字节预留数据

					AddCrc16(m_WaveDataBuf[m_WaveIndex].au8data[l_u16tmp],l_u16Len+24);
				}
				else
				{
					if(l_u16tmp>0)    //只有1包数据在组帧发送时该包为结束帧
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
			l_err = OSQPost(g_pQNetWavePC, &m_WaveDataBuf[m_WaveIndex]);  ////发送波形数据队列
			if(OS_Q_FULL == l_err)
				g_ErrMsgState |= 0x200;
			m_WaveIndex ++;
			m_WaveIndex %= WAVE_BUFSIZE;  // add by sxh 2014.4.01
			//WFlag[l_pItem->u8Id]=0;
		}
		
#if	TASKTIMEENABLE
		if(2==TaskRunTimeStatistics.m_ucTaskWaveFlag){//2:结算状态 ?????
			OSSchedLock();
			T1TCR=0x00;
			TaskRunTimeStatistics.m_ulTaskWaveDet=(RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskWave1)>TaskRunTimeStatistics.m_ulTaskWaveDet)?RUNTIMECOUNT(TaskRunTimeStatistics.m_ulTaskWave1):TaskRunTimeStatistics.m_ulTaskWaveDet; 				
			T1TCR=0x01;	
			OSSchedUnlock();
		}
#endif 	
	}	
}	

//#endif	//#if	SENDWAVEENABLE > 0		//使能发波形

