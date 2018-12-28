/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TaskTest.c
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wu Jinlong
** Created date:		20140324
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
#define _TASKTEST_C_

#include "common.h"

#define DS18B20TEST 		0
#define FM24W256TEST 		0
#define SDCARDTEST 			1

#define TEST_BLOCKNUM 		4
#define TEST_CYCLE			1000000
#define TEST_BASE_ADDR		0x0000

//OS_STK	TaskTestStk[TASKSTACKSIZE];


void TaskTest(void)
{
	uint16 i;
	uint8 status;

	uint32 total_cnt = 0;
	uint32 write_err_cnt = 0;
	uint32 read_err_cnt = 0;
	uint32 check_err_cnt = 0;
	uint32 success_cnt = 0;

	uint8 tx_buf[2048] = {0};
	uint8 rx_buf[2048] = {0};

	uint8	l_u8Err;
	uint16	l_u16SendLen;

	// 等待初始化结束
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// 任务开始运行added by wujinlong 20130805
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TTest Recd TaskBegin。\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif



	for(i=0;i<2048;i++)
	{
		tx_buf[i] = (uint8)((i+8) & 0xff);
	} 

#if DS18B20TEST	
	temp = FPGA_Temperature();
	temp_old = FPGA_Temperature();

	//++++++++++++++DS18B20 test code++++++++++++++++++++++++ add by zhouhang
	while(1)
	{
		temp = FPGA_Temperature();
		if(temp > temp_old)
			if(temp - temp_old > 5)
				unErrorCnt++;
		else if(temp < temp_old)
			if(temp_old - temp > 5)
				unErrorCnt++;
		temp_old = temp;
		if(unErrorCnt == 10)
		{
			while(1);
		}
	}
 	//-------------------------------------------------------
#endif


#if FM24W256TEST
	while(1)
	{
  	//++++++++++++++FM24W256 test code++++++++++++++++++++++++ add by zhouhang
		status =  WriteC256(0x1000, tx_buf, 256);
		status =  ReadC256(0x1000, rx_buf, 256);
	//-------------------------------------------------------
	}
#endif

#if SDCARDTEST
	i=0;

	while(1)
	{
		total_cnt++;
		//status = WriteSDCardBlock(0x8000+i, tx_buf);	 //写入地址为一个扇区的地址
    	status = WriteSDCardMultiBlock(TEST_BASE_ADDR+i, TEST_BLOCKNUM, tx_buf);
		if(status!=SD_NO_ERR)
		{
			write_err_cnt++;
		}
		else
		{
	   		//status = ReadSDCardBlock(0x8000+i, rx_buf);
	  		status = ReadSDCardMultiBlock(TEST_BASE_ADDR+i, TEST_BLOCKNUM, rx_buf);
			if(status!=SD_NO_ERR)
			{
				read_err_cnt++;
			}
			else
			{
	  			if(memcmp(tx_buf, rx_buf, (TEST_BLOCKNUM*512)) != 0x00) 			/* 校对数据 */
				{
					check_err_cnt++;
				}
				else
				{
					success_cnt++;
				}				
			}
		}
		i += TEST_BLOCKNUM;				

		while(total_cnt == TEST_CYCLE)
		{
			OSTimeDly(200);
		}
		OSTimeDly(200);

		memset(rx_buf,0x00,2048);
	}
#endif
	
}

