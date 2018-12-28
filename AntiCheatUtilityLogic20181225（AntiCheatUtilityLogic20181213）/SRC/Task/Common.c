/****************************************Copyright (c)****************************************************
**                                     BEIJING  WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Common.c
** Last modified Date:  2011-04-27
** Last Version:		
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Created by:			CHEN Wei
** Created date:		20110427
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
#define	__COMMON_C
#include "common.h"

#define xcrc(crc,cp) (crctab[(crc>>8)^cp]^(crc<<8))
#define		SETUPALIAS				g_Setup			//设置参数结构
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

static	uint16  crctab[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/* CRC  高位字节值表 */  
static unsigned char auchCRCHi[] = { 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 
	0x40 
}; 
/* CRC 低位字节值表*/  
static char auchCRCLo[] = { 
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 
	0x40 
};

/*********************************************************************************************************
** Function name:		ScaleUp
** Descriptions:		分度值化，将数值按分度值转换
** input parameters:	p_n32Value		待转换的值 
** 						p_u8Scale		分度
** return value:		转换后的值
** Created by:			ZHANG Ye		  
** Created Date:		20110519	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int32 ScaleUp(int32 p_n32Value, uint8 p_u8Scale)
{
	int32 l_n32Tmp;
	if ((p_u8Scale <= 100)&&(p_u8Scale > 1))
	{
		l_n32Tmp	= p_n32Value % p_u8Scale;
		p_n32Value	= p_n32Value - l_n32Tmp;
		if(l_n32Tmp >=(p_u8Scale >> 1))
			p_n32Value += p_u8Scale;
	}
	return (p_n32Value);
}

/*********************************************************************************************************
** Function name:		AddCrc16
** Descriptions:		CRC校验，并在后面写入2字节的校验值
** input parameters:	p_pu8Ptr		校验数据指针 
** 						p_u16Len		校验数据长度
** return value:		CRC值
** Created by:			ZHANG Ye		  
** Created Date:		20110519	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint16 AddCrc16( uint8 * p_pu8Ptr,uint16 p_u16Len)
{
	uint16 l_u16CRC;
	uint16 l_u16I,l_u16Cnt;

	l_u16Cnt	= p_u16Len;
	l_u16CRC	= 0;

	for (l_u16I = 0; l_u16I < l_u16Cnt; l_u16I++ )
	{
		l_u16CRC = xcrc(l_u16CRC,*p_pu8Ptr);
		p_pu8Ptr++;
	}

	*p_pu8Ptr	= l_u16CRC >> 8; 
	 p_pu8Ptr	++;
	*p_pu8Ptr	= l_u16CRC & 0xff;

	return	l_u16CRC;
}

/*********************************************************************************************************
** Function name:		CheckCrc
** Descriptions:		CRC校验
** input parameters:	p_pu8Ptr		校验数据指针 
** 						p_u16Len		校验数据长度
** return value:		校验是否成功 0：失败， 1：成功
** Created by:			ZHANG Ye		  
** Created Date:		20110519	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 CheckCrc( uint8 * p_pu8Ptr,uint16 p_u16Len)
{
	uint16 l_u16CRC;
	uint16 l_u16I,l_u16Cnt;

	l_u16Cnt	= p_u16Len;
	l_u16CRC	= 0;

	for (l_u16I = 0; l_u16I < l_u16Cnt; l_u16I++ )
	{
		l_u16CRC = xcrc(l_u16CRC,*p_pu8Ptr);
		p_pu8Ptr++;
	}
	if ((*p_pu8Ptr) == (l_u16CRC >> 8))
	{ 		    
		p_pu8Ptr	++;
		if ((*p_pu8Ptr) == (l_u16CRC & 0xff))
			return	1;				//成功
	}

	return	0;		//失败
}

/*********************************************************************************************************
** Function name:		bcd
** Descriptions:		bcd转换，获取y1值的px位数字
** input parameters:	p_pu8Ptr		校验数据指针 
** 						p_u16Len		校验数据长度
** return value:		数字值
** Created by:			ZHANG Ye		  
** Created Date:		20110519	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 bcd(uint32 p_u32Value,uint8 p_u8Pos)
{
	uint32 l_u32Tmp	= 1;
	uint8  l_u8I;
	for (l_u8I = 1; l_u8I < p_u8Pos; l_u8I ++)	 
	{
		l_u32Tmp	= l_u32Tmp * 10;
	}
	return	((p_u32Value % (l_u32Tmp*10)) / l_u32Tmp);
}

/*********************************************************************************************************
** Function name:		StaticXiuZhengIndex					au16StaticModify
** Descriptions:		对重量进行静态修正,查看当前重量属于静态修正的那个段，主要由于传感器的非线性而造成需要修正；
** input parameters:	int32 p_n32Weight	要修正的重量
** output parameters:	uint8  				需要修正的重量属于那个修正段
**
** Created by:			Chen Wei		  
** Created Date:		20110518	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8 StaticXiuZhengIndex(int32	p_n32Weight)
{
	uint32	l_u32Tmp;
	uint8	u8Return = 0;
	if(p_n32Weight > 0)
	{
		l_u32Tmp = p_n32Weight/1000;
		if(l_u32Tmp < 16)
		{
			if(p_n32Weight < (l_u32Tmp*1000+300))
			{
				if(l_u32Tmp > 0 )
				{
					u8Return = l_u32Tmp - 1;
				} 
			}
		}	
	}
	return u8Return;
}
/*********************************************************************************************************
** Function name:		RootFloat					
** Descriptions:		开平方根快速算法；
** input parameters:		
** output parameters:				
**
** Created by:					  
** Created Date:		20110518	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 RootFloat(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	int yy;
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f375a86 - ( i >> 1 );
	y = * ( float * ) &i;
    y = y*(f-x*y*y);
//	y = y*(f-x*y*y);
        yy = (int)(y * number);
	return yy;
}

//串口格式化输出信息
//int wj_Printf(const char* cmd, ...)
//{
////#if DEBUG_PRINT_EN > 0
//	va_list args;
//	uint8	l_u8Err;
//	//
//	OSMutexPend( g_pEventMutexCOM1, 0, &l_u8Err );
//	//
//	va_start(args, cmd);
//	vprintf(cmd, args);
//	va_end(args);
//	//
//	OSMutexPost( g_pEventMutexCOM1 );
////#endif
//	return 1;
//}

//******************************************************************************************************
// 函数名：timer_set
// 函数功能：设置定时器时间间隔,并且开始计时
// 形参：*t: Timer控制块指针，interval: 控制时间间隔，单位（s）
// 返回值：无
// 全局变量：无
// 注意事项：
//*******************************************************************************************************
void timer_set(TIMER_PIB *t, uint32 interval)
{
	t->interval = interval * OS_TICKS_PER_SEC;
	t->start = OSTimeGet();
}

//******************************************************************************************************
// 函数名：timer_reset
// 函数功能：等下一个时间间隔后再开始计时
// 形参：无
// 返回值：无
// 全局变量：无
// 注意事项：
//*******************************************************************************************************
void timer_reset(TIMER_PIB *t)
{
	t->start += t->interval;
}

//******************************************************************************************************
// 函数名：timer_restart
// 函数功能：重新开始计时
// 形参：无
// 返回值：无
// 全局变量：无
// 注意事项：
//*******************************************************************************************************
void timer_restart(TIMER_PIB *t)
{
	t->start = OSTimeGet();
}

//******************************************************************************************************
// 函数名：timer_expired
// 函数功能：判断是否超时
// 形参：*t: Timer控制块指针
// 返回值：0x00:未超时, 0x01:已超时
// 全局变量：无
// 注意事项：
//*******************************************************************************************************
uint8 timer_expired(TIMER_PIB *t)
{
	uint32 curTick;
	
	curTick = OSTimeGet();
	if (tickGetDiff(curTick, t->start) >= t->interval)
		return 1;
	else
		return 0;
}


/*********************************************************************************************************
** Function name:		FReConnect
** Descriptions:		
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130828	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FReConnect( uint16 p_u16Len, uint8 p_u8Sockketport )
{
	if( !p_u16Len )
	{
		g_au8ReCon[p_u8Sockketport]++;
	}
	else
	{
		g_au8ReCon[p_u8Sockketport] = 0;
	}

	if( g_au8ReCon[p_u8Sockketport] >= RECONNECTCNT )
	{
//		if(p_u8Sockketport != 2)
//			Socket_Listen(p_u8Sockketport);
//		else
//			Socket_Connect(p_u8Sockketport);
		 switch(p_u8Sockketport)
		 {
		 	case 0:
				if(S0_Mode == TCP_SERVER)
				{
// 					Socket_Listen(p_u8Sockketport);
					listen(p_u8Sockketport);
				}
				else
				{
// 					Socket_Connect(p_u8Sockketport);
					connect(p_u8Sockketport,S0_DIP,(S0_DPort[0]<<8)|S0_DPort[1]);
				}
			break;
			case 1:
				if(S1_Mode == TCP_SERVER)
				{
// 					Socket_Listen(p_u8Sockketport);
					listen(p_u8Sockketport);
				}
				else
				{
// 					Socket_Connect(p_u8Sockketport);
					connect(p_u8Sockketport,S1_DIP,(S1_DPort[0]<<8)|S1_DPort[1]);
				}
			break;
			case 2:
				if(S2_Mode == TCP_SERVER)
				{
// 					Socket_Listen(p_u8Sockketport);
					listen(p_u8Sockketport);
				}
				else
				{
// 					Socket_Connect(p_u8Sockketport);
					connect(p_u8Sockketport,S2_DIP,(S2_DPort[0]<<8)|S2_DPort[1]);
				}
			break;
			case 3:
				if(S3_Mode == TCP_SERVER)
				{
// 					Socket_Listen(p_u8Sockketport);
					listen(p_u8Sockketport);
				}
				else
				{
// 					Socket_Connect(p_u8Sockketport);
					connect(p_u8Sockketport,S3_DIP,(S3_DPort[0]<<8)|S3_DPort[1]);
				}
			break;
			case 4:
				if(S4_Mode == TCP_SERVER)
				{
// 					Socket_Listen(p_u8Sockketport);
					listen(p_u8Sockketport);
				}
				else
				{
// 					Socket_Connect(p_u8Sockketport);
					connect(p_u8Sockketport,S4_DIP,(S4_DPort[0]<<8)|S4_DPort[1]);
				}
			break;
		 }

		g_au8ReCon[p_u8Sockketport] = 0;
	}
}

/*********************************************************************************************************
** Function name:		FFendu
** Descriptions:		
** input parameters:						 
** output parameters:
** Return Value:							
** Created by:			Wu Jinlong		  
** Created Date:		20130828	  
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FFendu( uint32 *p_pu16Wet)
{
	uint16	l_u16Fendu = 0;
	uint16	l_u16Remainder = 0;
	uint32 	l_u32WetOri;
	
	l_u32WetOri = *p_pu16Wet; 
	
	l_u16Fendu = g_Setup.u8MotionScale;

	l_u16Remainder = l_u32WetOri % l_u16Fendu;
	*p_pu16Wet = (l_u32WetOri/l_u16Fendu + ((l_u16Remainder>=l_u16Fendu/2)?1:0)) * l_u16Fendu;
	
	return SUCCESS;
}

 /*********************************************************************************************************
** Function name:		Nm2Bcd
** Descriptions:		NUM变成BCD码数组
** input parameters:	b           
** 						
** return value:		
** Created by:					  
** Created Date:			  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Nm2Bcd(uint32 u32_Num, uint8 *u8_buf, uint8 p_u8Len)
{
	uint8 i;
	
	if (p_u8Len == 0)
	{
		return;
	}
	
	// 求十进制值
	for (i=0; i<p_u8Len; i++)
	{
		u8_buf[p_u8Len-1-i] = (u32_Num%10);
		u32_Num = u32_Num/10;
	}
}

 /*********************************************************************************************************
** Function name:		crc_create
** Descriptions:		生成校验码（3个字节校验码）
** input parameters:	p_pu8Ptr		校验数据指针 
** 						p_u16Len		校验数据长度
** return value:		校验是否成功 0：失败， 1：成功
** Created by:					  
** Created Date:			  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  	Fan HongJun	
** Modified date:	  	20130321	
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Crc3_create(uint8 *p_pu8Ptr, uint16 p_u16Len)
{
	uint16 crc;
	uint8 buf[5];
	
	if (p_u16Len == 0)
	{
		return;
	}
	
	// 得到CRC码
	crc = AddCrc16(p_pu8Ptr, p_u16Len);
	// 换算成BCD
	Nm2Bcd(crc, buf, 5);
	
	*(p_pu8Ptr+p_u16Len) = buf[0];
	*(p_pu8Ptr+p_u16Len+1) = (buf[1] << 4)|buf[2];
	*(p_pu8Ptr+p_u16Len+2) = (buf[3] << 4)|buf[4];
}

uint8 FIsRunYear(uint16 pYear)
{
	uint8 l_u8RunYearFlag = 0;
	
	// 判断是平年还是润年
    if( (pYear%100)== 0 )       // 整百年
	{
	    if( (pYear%400)==0 )    // 闰年
		{
			l_u8RunYearFlag = 1;  
		}
		else	           
		{
			l_u8RunYearFlag = 0;   // 平年
		}
	}
	else
	{
		if( (pYear%4)==0 )    // 闰年
		{
			l_u8RunYearFlag = 1;  
		}
		else	           
		{
			l_u8RunYearFlag = 0;   // 平年
		}
	}

	return l_u8RunYearFlag;
}
uint16 FDaysOneYear(uint16 pYear)
{
	if((pYear%4==0 && pYear%100)||pYear%400==0)
		return 366;
	else 
		return 365;
}

uint8 FDaysOneMon(uint16 pYear, uint8 pMonth)
{
	uint8 l_u8Return = 0;

	switch(pMonth)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		        l_u8Return = 31;
				break;
		case 6:
		case 4:
		case 9:
		case 11:
		        l_u8Return = 30;
				break;
		case 2:
		       if(FDaysOneYear(pYear)==366)
		       {
			       l_u8Return = 29;
		       }
			   else
			   {
			       l_u8Return = 28;
			   }
			   break;
		default:
		       l_u8Return = 0;
			   break;
	}
	return l_u8Return;
}

unsigned short JDCRC16(unsigned char *puchMsg, unsigned short usDataLen) 
{  
	unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
	unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */ 
	unsigned uIndex ; /* CRC循环中的索引 */ 
	while (usDataLen--) /* 传输消息缓冲区 */ 
	{ 
		uIndex = uchCRCLo ^ *puchMsg++ ; /* 计算CRC */ 
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ; 
		uchCRCHi = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}
/*********************************************************************************************************
** Function name:		Sys_init
** Descriptions:		系统初始化封装
** input parameters:	none
** return value:	    none
** Created by:		    Chenzhongyuan		  
** Created Date:		2014-12-22	  
*********************************************************************************************************/
void Sys_init(void)//ucos系统、ARM系统及其外设、FPGA系统及其外设等相关系统级初始化
{
   	memset(g_anfSPIAD,0,NETDATALEN*NETONEDATALEN);//网络收到的数据
  	g_u8NetADIn = 0; //收到的数据索引
	g_u8NetADOut = 0; //处理的数据索引
	g_u16NetADcount = 0;//收到网络数据帧计数
	g_u8NetStatusErr = 0;//网络故障状态
	memset( &g_IPSetLocal,0,sizeof(g_IPSetLocal) );// 本地IP地址等
	memset( &g_au8AllNetSendBuf,0,SENDALLSIZE );//网络上传数据组帧总结构
	memset( &g_sndNetBuf,0,VEHICLE_BUFLEN*sizeof(sndFrame_Struct) );//网络分包上传结构
	memset( &g_sndNetOrderBuf,0,TEMP_SIZE*sizeof(sndFrame_Struct) );//网络上传指令数据
	g_u8CurSendNetOrderIndex = 0;//上传指令数据索引
	g_u8CurNetDebugIndex = 0;
	memset( g_au8ReCon, 0, NET_PORT_NUM );		//网口重连
 	memset( g_au16HeartTimeCnt, 0, NET_PORT_NUM*2 ); 	// 发送心跳时间计数
	memset( g_au8HeartRecvReplyFlag, 0, NET_PORT_NUM );	 	// 收到心跳回复标志
	memset( g_au8HeartRecvBeginCntFlag, 0, NET_PORT_NUM );		// 计算接收回复开始标志
	memset( g_au16HeartRecvTimeCnt, 0, NET_PORT_NUM*2 );   	// 计算接收失败时间计数
	memset( g_au8HeartRecvFailCnt, 0, NET_PORT_NUM );	  // 心跳接收失败计数
	memset( &g_cHeartBuf, 0, sizeof(CHeartBuf) );		  	// 心跳数据	

}
// /*********************************************************************************************************
//** Function name:		LogicMem_init
//** Descriptions:		逻辑变量初始化封装
//** input parameters:	none
//** return value:	    none
//** Created by:		    Chenzhongyuan		  
//** Created Date:		2014-12-22	  
//*********************************************************************************************************/
void LogicMem_init(void)//逻辑变量初始化，用于参与逻辑判断的一些全局变量、结构体等信息的初始化
{
    uint32 l_u32tmp, l_u32tmp1;
	g_StartWaveFlag=0;
	startWave[0]=0xff;
	startWave[1]=0xff;
	startWave[2]=0x00;
	startWave[3]=0x8f;
	startWave[4]=0x02;
	startWave[5]=0x00;
	startWave[6]=0x0b;
	startWave[7]=0x00;
	startWave[8]=0x00;
	startWave[9]=0xbd;
	startWave[10]=0x23;
	startWave[11]=0x00;
	
	finishWave[0]=0xff;
	finishWave[1]=0xff;
	finishWave[2]=0x00;
	finishWave[3]=0x8f;
	finishWave[4]=0x01;
	finishWave[5]=0x00;
	finishWave[6]=0x0b;
	finishWave[7]=0x00;
	finishWave[8]=0x00;
	finishWave[9]=0x53;
	finishWave[10]=0xf1;
	finishWave[11]=0x00;
	
	coilWave[0]=0xff;
	coilWave[1]=0xff;
	coilWave[2]=0x00;
	coilWave[3]=0xaa;
	coilWave[4]=0x04;
	coilWave[5]=0x00;
	coilWave[6]=0x13;
	for(l_u32tmp=0;l_u32tmp<6;l_u32tmp++)
		coilWave[7+l_u32tmp]=0x00;
		
	laserWave[0]=0xff;
	laserWave[1]=0xff;
	laserWave[2]=0x00;
	laserWave[3]=0xaa;
	laserWave[4]=0x02;
	for(l_u32tmp=0;l_u32tmp<48;l_u32tmp++)
		laserWave[5+l_u32tmp]=0x00;
	//JDSYSPARAM.m_u8TransMod = 1;  //wanglihui	
	PCSendCnt=0;
	g_u8RasterRead=1;
	memset(&g_Setup,0,sizeof(g_Setup));	   //配置参数结构体
	memset(g_an32MotionZero,0,sizeof(int32)*CHANNELNUM); //零点记录
	g_u8StaticMotionswitch = 0;					//闪灯动作
	memset(&g_DevCurTime,0,sizeof(g_DevCurTime));	//系统当前时间
	g_pSemTask6Param 	= OSSemCreate(0);						 //参数修改指令后任务6进行参数刷新操作
  g_pSemSendThtouSP 	= OSSemCreate(1);						//调试信息调用，保证只有一个调试信息可以调用
	g_pSemDisplayCheliangInfo = OSQCreate(g_paDisplayInfo,15);					//液晶显示用，显示车辆信息	  rp 20151009
#if DEBUGINFO_TASK_SP_EN > 0
	g_pQSendPCCOM1 		= OSQCreate(g_pSendPCCOM1List,COM_NUM);//调试信息输出数据队列
	memset( g_cDebugInfo, 0, COM_NUM*sizeof(CComBuf) );// 调试信息存储
	g_u16CurDebugInd = 0;//调试信息存储指针
#endif
	g_pQWaveMsg			= OSQCreate(g_pQWaveList,DNLISTNUM);		//wzf  发送波形信号消息队列
	g_pQNetWavePC		= OSQCreate(g_pNetWaveList,DNLISTNUM);		 ///波形上传队列
	g_pQSendPC 			= OSQCreate(g_pSendList,SENDLISTNUM);	  //数据发送消息队列
	g_pQSendPCDebug     = OSQCreate(g_pSendListDebug,SENDLISTNUM);
	g_pQDnMsg			= OSQCreate(g_pQDnList,DNLISTNUM);		  //下称消息通知队列，用于FPGA中断服务子函数抛窄条中断信息
	g_pQAxleCalc		= OSQCreate(g_apvAxleInfo,SENDLISTNUM);	  //轴计算数据消息队列
  g_pQShouWei			= OSQCreate(g_pQShouWeiList,SENDLISTNUM); //车辆收尾消息队列

	g_psemKey			= OSSemCreate(0);
	g_psemScreenRefresh	= OSSemCreate(1);
	Sem_KeyIn			= OSSemCreate(0);

  for(l_u32tmp = 0;l_u32tmp < CHL_BUFLEN;l_u32tmp++)
	{
		memset( &g_ChlData[l_u32tmp], -1, sizeof(Channal_Struct) );	 //窄条信息
		g_ChlData[l_u32tmp].u16ptCnt = 0;
	}
	memset(g_ZTTime,0,sizeof(g_ZTTime));   
	memset(g_ChlID2TriggerID,EMPTY_ITEMZERO,CHANNELNUM*sizeof(uint16));//记录FPGA中上传的窄条中断每根窄条的第几次上传
	g_u16ChlWindex = 0;	//窄条第几次上传
	/******************************抓拍**********************/
	g_u16CapDelayTime = 80;
	memset(g_u32FirstCapTime, 0, sizeof(g_u32FirstCapTime));
	memset(g_u8CapFlag, 0, sizeof(g_u8CapFlag));
	memset(g_u32CapInfo, 0, sizeof(g_u32CapInfo));
	
	/******************************轴**********************/
	memset(g_AxleInfo,0,sizeof(Axle_Struct)*AXLE_BUFLEN);	 //轴数据信息
	for(l_u32tmp = 0;l_u32tmp < AXLE_BUFLEN;l_u32tmp++)
	{
		g_AxleInfo[l_u32tmp].u8PointsGet = 100;
		for(l_u32tmp1=0;l_u32tmp1<12;l_u32tmp1++)
			g_AxleInfo[l_u32tmp].u16ChList[l_u32tmp1] = 0xFFFF;
// 		g_AxleInfo[l_u32tmp].s32AxleWeight = 0;
// 		g_AxleInfo[l_u32tmp].u16speed = 0;
// 		g_AxleInfo[l_u32tmp].u8ValidChlCnt = 0;
	}
	g_u16AxleInfoWriteIndex = 0;	   // 轴数据记录索引
	/************************** 车***********************/
	memset(g_vehicleInfo,0,sizeof(Vehicle_Struct)*VEHICLE_BUFLEN); //车辆数据信息
	g_u8vehicleID = 0;											   //维护的车辆编号


	for(l_u32tmp = 0;l_u32tmp < 2*CHL_MAXNUM-1;l_u32tmp++)
	{
		for(l_u32tmp1 = 0;l_u32tmp1 < CHL_SAVEVEH;l_u32tmp1++)
		{
			FInitVehStr(&g_asVehiclePerChannelForLoop[l_u32tmp][l_u32tmp1],1);	//正向车通过过程中存储车辆信息结构体
			FInitVehStr(&g_asReVehiclePerChannelForLoop[l_u32tmp][l_u32tmp1],-1);//逆向车通过过程中存储车辆信息结构体
			FInitVehStr(&g_HideZTVehicleStruct[l_u32tmp][l_u32tmp1],1);
			FInitVehStr(&g_HideZTReVehicleStruct[l_u32tmp][l_u32tmp1],-1);
		}
	}
	for(l_u32tmp = 0;l_u32tmp < 2*CHL_MAXNUM;l_u32tmp++)
	{
		for(l_u32tmp1 = 0;l_u32tmp1 < HIDECOLUM_SAVEAXlE;l_u32tmp1++)
		{
// 			memset(&Forward_ZTColum[l_u32tmp][l_u32tmp1],0,sizeof(Colum_Struct));  //正向列初始化
// 			memset(&Reverse_ZTColum[l_u32tmp][l_u32tmp1],0,sizeof(Colum_Struct));  //逆向列初始化
// 			memset(&Forward_ZTColum[l_u32tmp][l_u32tmp1].Colum,-1,sizeof(int16)*4); //正向列存储的列初始化为-1
// 			memset(&Reverse_ZTColum[l_u32tmp][l_u32tmp1].Colum,-1,sizeof(int16)*4); //逆向列存储的列初始化为-1
// 			memset(&g_WonderZT[l_u32tmp][l_u32tmp1],-1,sizeof(int16));
// 			
			memset(&Forward_HideZTColum[l_u32tmp][l_u32tmp1],0,sizeof(HideColum_Struct));  //正向列初始化
			memset(&Reverse_HideZTColum[l_u32tmp][l_u32tmp1],0,sizeof(HideColum_Struct));  //逆向列初始化
			memset(&Forward_HideZTColum[l_u32tmp][l_u32tmp1].Colum,-1,sizeof(int16)*2); //正向列存储的列初始化为-1
			memset(&Forward_HideZTColum[l_u32tmp][l_u32tmp1].Dir,0,sizeof(int8));
			memset(&Reverse_HideZTColum[l_u32tmp][l_u32tmp1].Colum,-1,sizeof(int16)*2); //逆向列存储的列初始化为-1
			memset(&Reverse_HideZTColum[l_u32tmp][l_u32tmp1].Dir,0,sizeof(int8));
// // 			memset(&g_HideWonderZT[l_u32tmp][l_u32tmp1],-1,sizeof(int16));
// // 			memset(&g_HideWonder2ZT[l_u32tmp][l_u32tmp1],-1,sizeof(int16));
		}
		g_au8ReZTEnPerLie[l_u32tmp]=1;	//当前窄条逆向逻辑是否使用
		g_au8ZTEnPerLie[l_u32tmp]=1;	//当前窄条正向逻辑是否使用
	}
	memset(g_au8HideDeltZTEnable,0,sizeof(uint8)*CHL_MAXNUM);
	memset(g_au32HideDeltZTTime,0,sizeof(uint32)*CHL_MAXNUM);
	memset(g_u32PreLoopTime, 0, sizeof(uint32)*CHL_MAXNUM);
	memset(g_u32PostLoopTime, 0, sizeof(uint32)*CHL_MAXNUM);
	memset(g_u8PostVehFlag, 0, sizeof(uint8)*CHL_MAXNUM);
	memset(g_u8RePostVehFlag, 0, sizeof(uint8)*CHL_MAXNUM);
	memset(g_au32HideLineLastTime,0,10*4);
	memset(g_au32ZHLineLastTime,0,10*4);
	memset(g_BackVeh,0,sizeof(BackVeh_Struct)*2*CHL_MAXNUM);
	memset(g_DeletVeh,0,sizeof(DeleVeh_Str)*2*CHL_MAXNUM);
// 	memset(g_WheelData,0,sizeof(Wheel_Struct)*CHL_MAXNUM*16);
	memset(g_WhelData,0,sizeof(Whel_Struct)*CHL_MAXNUM*4);
	memset(g_WheelTrace,0,sizeof(TraceWhel_Struct)*WHEELTRACEZTNUM*WHEELCACHE);
	memset(g_ReWheelTrace,0,sizeof(TraceWhel_Struct)*WHEELTRACEZTNUM*WHEELCACHE);
	memset(g_au32LoopCaTime, 0, 10*4);
	memset(g_au32LoopCaTimeUp, 0, 10*4);
	memset(g_au32LoopCaTimeBack, 0, 10*4);
	memset(g_au32LoopCaTimeUpBack, 0, 10*4);
	memset(g_au8WaitingPostVehFlg,0,10);
	memset(g_n8ZTRang, -1, 20);	//每列窄条归属情况 -1 无归属 其他 相同编号归属一起
	g_u16UART5SavIndex = 0;							//串口5保存位置
	g_u16UART5ReadIndex = 0;						//串口5数据处理到的位置
	g_u8BiaoDEN = 0;	  //标定使能
	g_u8BiaoDLane = 0;	 //标定车道
	g_u8SendWave= 0;	 //是否上传波形开始存储标记
	g_u8DetVeh=0;		 ///当前是否有车标记
	for(l_u32tmp = 0;l_u32tmp < 16;l_u32tmp++)
	{
		g_u8IOStateForBoard[l_u32tmp] = 1; //线圈板IO记录
	}
	g_u8DevErr = 0;	//设备故障 0 无 1 线圈故障 2 激光故障 3 都故障
	g_u8NoMatchCnt = 0;	//不匹配计数
	g_u16TotalMinutesNew = 0;	   //交调记录的当前分钟
	g_u16TotalMinutesOld = 0;	   //交调记录的上一分钟
	g_u8JDInitFlag = 0;	// 仪表正常启动标记，交调在该标记正常后启动
	g_u8IsFirstSDSave = 0; //01包首次存铁电标志
	g_u8Is39FirstSave = 0; //39包首次存铁电标志
	g_u8Is61FirstSave = 0; //61包首次存铁电标志
	g_u8Is62FirstSave = 0; //62包首次存铁电标志
	g_u8Is68FirstSave = 0;//68包首次存铁电标志
	g_u8JDNetFirstUse = 0; //交调通信是否首次成功
	g_u8JD02OrderRev = 0; //上位机回复的02包标志
	g_u8JD08OrderRev = 0; //上位机回复的08包标志
	g_u8JDSendEna = 0;	 //无线网络连接标志
	g_u8SendVehDataInd = 0;	//交调上传数据索引
	g_u8RecOrderCnt = 0; //交调回复帧计数
	g_u8JD0AOrderRev = 0;//上位机回复的0A包标志
	g_u8JD39OrderRev = 0; //上位机回复的39包标志
	g_u8JD6FOrderRev = 0; //上位机回复的6F包标志
	g_u32Resend01SDBgNum = 0;//上位机要求的重传序号
	g_u8DegIndex = 0;//调试信息无线上传索引
	g_u32Resend39 = 0;//39包重传序号
	g_u8Resend61Bg = 0;//61包重传起始序号
	g_u8Resend61Ed = 0;//61包重传结束序号
	g_u8Resend62Bg = 0;//62包重传起始序号
	g_u8Resend62Ed = 0;//62包重传结束序号
	g_u8Resend68Bg = 0;//68包重传起始序号
	g_u8Resend68Ed = 0;//68包重传结束序号
	g_u8VehSendFlag = 0;//上位机要求SD卡内车辆数据上传
	g_u8DebugInfoSendFlag = 0;//上位机要求SD内调试信息上传
	g_u32BeginIx = 0;//上位机要求SD卡数据上传起始位置
	g_u32EndIx = 0;//上位机要求SD卡数据上传结束位置
	g_u8IfRunSta = 0;//1开始 0不开始 
	g_u8ZTStaSendFlag = 0;
	g_u839UnRecCnt = 0;
	g_u32VehCntForTest = 0;//检测统计周期车数
	g_u16DigitalZTNum = 0;
//	g_u8TestFlag = 0;
//	g_u86FFlag = 0;
	g_ZTSecondWaveOverLap=5; //波形二级拟合度
	memset(g_asLoopPerLaneInfo, 0, sizeof(LANELOOP_INFO)*CHL_MAXNUM);	 ///交调每个车道的线圈信息
	memset(g_asTrafficData, 0, sizeof(LANE_TRAFFIC_DATA)*CHL_MAXNUM); ///交调记录车辆数据
	memset(g_asAxleLoadData, 0, sizeof(LANE_AXLE_DATA)*CHL_MAXNUM);	  //交调记录轴载数据
	memset(g_u8RunZTSta, 1, 64);//记录窄条触发
	memset(g_u8RunLoopSta, 0, 16);//记录线圈状态

}
// /*********************************************************************************************************
//** Function name:		External_Equipment_init
//** Descriptions:		外部设备初始化封装
//** input parameters:	none
//** return value:	    none
//** Created by:		    Chenzhongyuan		  
//** Created Date:		2014-12-22	  
//*********************************************************************************************************/
void External_Equipment_init(void)	//外部设备初始化；激光、线圈、SD卡等外部设备的相关变量初始化
{
	g_u8RasterDataReadIndex = 0;     //读数据信息	用于波形发送读取数据
	g_u8RasterDataWriteIndex = 0; 	  //写数据信息	用于串口接收写入
	g_psQLaserInteract	= OSQCreate(g_pLaserInteractList,RECVLASERDATA_NUM); //激光交互消息队列；
	memset( g_RasterData, 0, RASTERDATALEN * sizeof(Raster_Struct));  //激光传送来的数据位置//   20121108
	memset( g_cLaserInfo, 0, sizeof(Channal_Struct)*LASERINFO_LEN );  //发波形中的激光数据
	g_u8LaserInfoIx = 0;											   //发波形中的激光数据存储指针
   	memset( g_cIOChange, 0, sizeof(Channal_Struct)*LASERINFO_LEN );		//发波形中的IO信息
	g_u8IOChangeIx = 0;													//发波形中的IO信息存储指针
	memset( g_cRecvLaserData, 0, RECVLASERDATA_NUM*sizeof(CComBuf) );  //接收激光的数据结构，串口3中断存储
	g_u16RecvLaserInd = 0;											   //接收激光的数据索引
	memset(g_u8JGSWFlag, 0, 10);//激光收尾标记 0 该车道没收尾 1 该车道收尾
	memset(g_u8JGSWVehID, 0, 10);
#ifdef __STRG_SD 
 	g_u8SDInsert 	= 0; //SD卡是否插入标志
#endif
	g_u8SDBuffIndex = 0;    //  向SD卡存储数据的数据指针			
	g_pQSDOffNetWrite 	= OSQCreate(g_paSDOffNetWriteList, SD_OFFNET_WRITE_LEN); //SD 断网缓存信号量
	g_u8IsConnectFlag = 1;//网络连接标志
	g_u32NetConCnt = 0;	//网络连接时间计数
	memset( g_acSDSendVehInfo, 0, SD_SEND_BUF_NUM*sizeof(sndFrame_Struct) );// 读取SD卡数据，保存在此，并发送给通信任务
	g_u16SDSendBufIX = 0;//读取SD数据索引
	g_pQSDVehWrite 	= OSQCreate(g_pQSDVehWriteList, SD_VEH_WRITE_LEN);//SD卡车辆数据写数据队列
	g_pQSDDebugInfoWrite 	= OSQCreate(g_paSDDebugInfoWriteList, SD_DBGINFO_WRITE_LEN); //SD卡调试信息写数据队列
	g_pQSDWaveWrite 	= OSQCreate(g_paSDWaveWriteList, SD_WAVE_WRITE_LEN);//SD卡波形写数据队列
	g_pSDSaveForJD =OSSemCreate(0);//SD卡存交调数据队列
	g_pQJDSend = OSQCreate(g_paJDSendList, JD_SEND_LEN);   //交调数据上传数据队列

}
// /*********************************************************************************************************
//** Function name:		Fault_diagnosis_init
//** Descriptions:		故障诊断初始化封装
//** input parameters:	none
//** return value:	    none
//** Created by:		    Chenzhongyuan		  
//** Created Date:		2014-12-22	  
//*********************************************************************************************************/
void Fault_diagnosis_init(void)	//故障诊断模块相关变量初始化
{
	memset(g_u8DeviceERR,0,ERR_LEN); // 设备故障,第一字节低4位表示激光，第一字节高4位表示通信.后8字节表示窄条状态,
	memset(g_u8IOERR, 0, 2);//每个IO故障状态，每位一个， 0正常 1故障
	memset(g_u8IOStateRe, 0, 10);//记录一个IO保持一个状态下通过的窄条数
	memset(g_u8ZTStateRe, 0, 64);//记录每个窄条触发次数
// 	g_u32StartupTime = 0;	//启动次数
    g_ErrMsgState = 0;										 //	记录信号量使用是否正常
	g_u8DeviceERR[0] = 0x0F;    // 初始化默认激光故障(liumingxin)(20170707)
}
// /*********************************************************************************************************
//** Function name:		FReadFromTDC
//** Descriptions:		任务启动从铁电读参数
//** input parameters:	none
//** return value:	    none
//** Created by:		wgh		  
//** Created Date:		2014-12-23	  
//*********************************************************************************************************/
void FReadFromTDC(void)
{
	uint32	l_u32BR = 0;
	uint8 l_u8temp,l_u8tempi;
	g_u8RWNVFlag = 0xff;
	//读取铁电中参数，如果参数异常则初始化为系统默认参数
	for(l_u8tempi = 0;l_u8tempi < 3;l_u8tempi++)
	{
	 l_u8temp = ReadC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 4);
	else
		g_u8RWNVFlag &=!(1 << 4);
	//读取铁电中模式参数 wgh 20140303 并初始化
	for(l_u8tempi = 0;l_u8tempi < 3;l_u8tempi++)
	{
		l_u8temp = ReadC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 5);
	else
		g_u8RWNVFlag &=!(1 << 5);
	//读取铁电中网口信息，如果参数异常则初始化为系统默认参数
	for(l_u8tempi = 0;l_u8tempi < 3;l_u8tempi++)
	{
    l_u8temp = ReadC256(NETINFOADDR,(uint8 *)&g_IPSetLocal, sizeof(g_IPSetLocal));
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 6);
	else
		g_u8RWNVFlag &=!(1 << 6);
	//读取铁电中交调信息，如果参数异常则初始化为系统默认参数
	for(l_u8tempi = 0;l_u8tempi < 3;l_u8tempi++)
	{
    l_u8temp = ReadC256(JDSYSADDR, (uint8 *)&g_cJDSysParam, sizeof(g_cJDSysParam));
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 7);
	else
		g_u8RWNVFlag &=!(1 << 7);
	if(PROMODPARAM.m_u8EnableFlag[2] != 0)
	{
		FRead01FromTDC(1);
		FRead01FromTDC(2);
		FRead01FromTDC(3);

		FRead39FromTDC(1);
		FRead39FromTDC(2);
		FRead39FromTDC(3);
		FRead61FromTDC(1);
		FRead61FromTDC(2);
		FRead61FromTDC(3);
		FRead62FromTDC(1);
		FRead62FromTDC(2);
		FRead62FromTDC(3);
		FRead68FromTDC(1);
		FRead68FromTDC(2);
		FRead68FromTDC(3);
	}
	OSTimeDly(500*OS_TICKS_PER_SEC/1000);	
	if(0 == CheckCrc((uint8 *)&PROMODPARAM, PROMODPARAMSIZE-2))
	{
		FeedExternDog();	
		ProModInit(1);
		FeedExternDog();	
	}
	else
	{
			// 增加条件，如果校验通过，但是程序版本号不对，则初始化
		if( PROMODPARAM.m_u8ProgVersion[0] == PROGVERSION_POS_0
			&& PROMODPARAM.m_u8ProgVersion[1]	== PROGVERSION_POS_1
			&& PROMODPARAM.m_u8ProgVersion[2]	== PROGVERSION_POS_2
			&& PROMODPARAM.m_u8ProgVersion[3]	== PROGVERSION_POS_3 )
		{
			
		}
		else
		{
			FeedExternDog();	
			ProModInit(2);
			FeedExternDog();	
		}
	}

	if(0 == CheckCrc((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2))
	{
		ClearStartupCnt();
		FeedExternDog();	
		InitSystem(1);
		FeedExternDog();	
		SETUPALIAS.u8ProgVersion[0] = PROGVERSION_POS_0;
		SETUPALIAS.u8ProgVersion[1]	= PROGVERSION_POS_1;
		SETUPALIAS.u8ProgVersion[2]	= PROGVERSION_POS_2;
		SETUPALIAS.u8ProgVersion[3]	= PROGVERSION_POS_3;
		g_u8FirstUse = 1;
	}
	else
	{
		// 增加条件，如果校验通过，但是程序版本号不对，则初始化
		if( SETUPALIAS.u8ProgVersion[0] == PROGVERSION_POS_0
			&& SETUPALIAS.u8ProgVersion[1]	== PROGVERSION_POS_1
			&& SETUPALIAS.u8ProgVersion[2]	== PROGVERSION_POS_2
			&& SETUPALIAS.u8ProgVersion[3]	== PROGVERSION_POS_3 )
		{
			 g_u8FirstUse = 0;
		}
		else
		{
			FeedExternDog();	
			InitSystem(2);
			FeedExternDog();	
			SETUPALIAS.u8ProgVersion[0] = PROGVERSION_POS_0;
			SETUPALIAS.u8ProgVersion[1]	= PROGVERSION_POS_1;
			SETUPALIAS.u8ProgVersion[2]	= PROGVERSION_POS_2;
			SETUPALIAS.u8ProgVersion[3]	= PROGVERSION_POS_3;
			g_u8FirstUse = 1;
		}
	}
	if(0 == CheckCrc((uint8 *)&g_IPSetLocal,sizeof(g_IPSetLocal)-2))
    {
			    FeedExternDog();	
	        InitNetParam(1);
			    FeedExternDog();	
    }
	else
	{
		// 增加条件，如果校验通过，但是程序版本号不对，则初始化
		if( g_IPSetLocal.m_u8ProgVersion[0] == PROGVERSION_POS_0
			&& g_IPSetLocal.m_u8ProgVersion[1]	== PROGVERSION_POS_1
			&& g_IPSetLocal.m_u8ProgVersion[2]	== PROGVERSION_POS_2
			&& g_IPSetLocal.m_u8ProgVersion[3]	== PROGVERSION_POS_3 )
		{
			
		}
		else
		{
			FeedExternDog();	
			InitNetParam(2);
			FeedExternDog();	
		}
	}
	if(0 == CheckCrc((uint8 *)&g_cJDSysParam,sizeof(g_cJDSysParam)-2))
    {
			   FeedExternDog();	
	       InitJDParam();
			   FeedExternDog();	
    }
	else
	{
		// 增加条件，如果校验通过，但是程序版本号不对，则初始化
		if( g_cJDSysParam.m_u8ProgVersion[0] == PROGVERSION_POS_0
			&& g_cJDSysParam.m_u8ProgVersion[1]	== PROGVERSION_POS_1
			&& g_cJDSysParam.m_u8ProgVersion[2]	== PROGVERSION_POS_2
			&& g_cJDSysParam.m_u8ProgVersion[3]	== PROGVERSION_POS_3 )
		{
			
		}
		else
		{
			FeedExternDog();	
			InitJDParam();
			FeedExternDog();	
		}
	}

	switch(SETUPALIAS.u8BaudRate) 	//串口0波特率
    {
	    case 1:
	        l_u32BR	= 4800;
	        break;
		case 2:
			l_u32BR = 9600;
			break;
	    case 3:
	        l_u32BR	= 38400;
	        break;
	    case 4:
	        l_u32BR	= 57600;
	        break;
	    case 5:
	        l_u32BR	= 115200;
	        break;
	
	    default:
	        l_u32BR	= 115200;
	        break;
    }
	// 初始化串口
// 	if(PROMODPARAM.m_au32Thres[11] != 0)
// 	{
// 		InitAllSP(l_u32BR, 38400, 38400, l_u32BR);  //l_u32BR
// 	}
// 	else
// 	{
// 		InitAllSP(l_u32BR, 38400, l_u32BR, l_u32BR);  //l_u32BR
// 	}
	InitAllSP(l_u32BR, 38400, 38400, l_u32BR);  //l_u32BR
	
	/**** 阈值参数赋值 ****/
	g_u8PosScoreC          = PROMODPARAM.m_au32Thres[0];
	g_u8SpdScoreC          = PROMODPARAM.m_au32Thres[1];
	g_u8RadScoreC          = PROMODPARAM.m_au32Thres[2];
	g_u8PosScoreWeight     = PROMODPARAM.m_au32Thres[3];
	g_u8SpdScoreWeight     = PROMODPARAM.m_au32Thres[4];
	g_u8RadScoreWeight     = PROMODPARAM.m_au32Thres[5];
	g_u8YaFengDiffXPosMax  = PROMODPARAM.m_au32Thres[6];
	g_u8XieXingDiffXPosMin = PROMODPARAM.m_au32Thres[7];
	g_u16TraceSpdChangeMax = PROMODPARAM.m_au32Thres[8];
	g_u8SgOrDbleWhelPoint  = PROMODPARAM.m_au32Thres[9];
	g_u8BuildTraceScoreMin = PROMODPARAM.m_au32Thres[10];
	g_u16TraceDelTime      = PROMODPARAM.m_au32Thres[11];
	g_u16RunAndWalkTime    = PROMODPARAM.m_au32Thres[12];
	g_u8RunAndWalkEnable   = PROMODPARAM.m_au32Thres[13];
	g_u16EvenColumDelyLth  = PROMODPARAM.m_au32Thres[14];
	g_u8TwoBlankUnion      = PROMODPARAM.m_au32Thres[15];
	g_u8MinAxleScore       = PROMODPARAM.m_au32Thres[16];
	g_u8AxleMultiScore     = PROMODPARAM.m_au32Thres[17];
	g_u8AxleWgtDiffRatio   = PROMODPARAM.m_au32Thres[18];
	g_u32WheelWghtCoin     = PROMODPARAM.m_au32Thres[19];
	g_ZTWaveOverLap        = PROMODPARAM.m_au32Thres[20];
	g_u8SecondCapFlag      = PROMODPARAM.m_au32Thres[21];
	g_u8ReverseDigitZTPos  = PROMODPARAM.m_au32Thres[22];
	g_u8A2BFlag            = PROMODPARAM.m_au32Thres[23];
	/**** end ****/

	g_u32FollowVehThr = JDSYSPARAM.m_u32FollowVehTimeThr;//交调跟车时间阈值
	g_u8PeriodTOSave = JDSYSPARAM.m_u8SavePeriod;  //循环存储周期
	g_u16SmallandMid = JDSYSPARAM.m_u16SmallandMid;
	g_u8NumUseLoopBoard = 0;
	g_u32AutoResetTime = 0;
	for(l_u32BR = 0;l_u32BR < 8; l_u32BR++)
	{
		if(PROMODPARAM.m_au8AcqBoardFlag[l_u32BR] == 2)
			g_u8NumUseLoopBoard ++;
	}
	for (l_u32BR = 0;l_u32BR < CHANNELNUM; l_u32BR++)
 	{
		g_an32MotionZero[l_u32BR]	= SETUPALIAS.an32Zero[l_u32BR];	
// 		if(SETUPALIAS.an8RoadPlaneHeight[l_u32BR] > 0)
// 		{
// 			//speed1[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*100+15*POINTRATE/10)/(RootFloat((SETUPALIAS.an8RoadPlaneHeight[l_u32BR]*POINTRATE/490)));
// 			speed1[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*POINTRATE*10+15*POINTRATE/10)*100/(RootFloat((SETUPALIAS.an8RoadPlaneHeight[l_u32BR]*POINTRATE*POINTRATE/49/2)));
// 			//speed2[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*100)/(RootFloat((SETUPALIAS.an8RoadPlaneHeight[l_u32BR]*POINTRATE/490)));
// 			speed2[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*POINTRATE*10)*100/(RootFloat((SETUPALIAS.an8RoadPlaneHeight[l_u32BR]*POINTRATE*POINTRATE/49/2)));
// 			speed3[l_u32BR] = speed2[l_u32BR]/2;
// 		}
// 		else if(SETUPALIAS.an8RoadPlaneHeight[l_u32BR] == 0)
// 		{
// 		  	speed1[l_u32BR] = 10000;
// 			speed2[l_u32BR] = 10000;
// 			speed3[l_u32BR] = 10000;
// 		}
// 		else
// 		{
// 		  	//speed1[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*100+15*POINTRATE/10)/(RootFloat(((0-SETUPALIAS.an8RoadPlaneHeight[l_u32BR])*POINTRATE/490)));
// 			//speed2[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*100)/(RootFloat(((0-SETUPALIAS.an8RoadPlaneHeight[l_u32BR])*POINTRATE/490)));
// 			speed1[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*POINTRATE*10+15*POINTRATE/10)*100/(RootFloat(((0-SETUPALIAS.an8RoadPlaneHeight[l_u32BR])*POINTRATE*POINTRATE/49/2)));
// 			speed2[l_u32BR] = (SETUPALIAS.an8LengthPodu[l_u32BR]*POINTRATE*10)*100/(RootFloat(((0-SETUPALIAS.an8RoadPlaneHeight[l_u32BR])*POINTRATE*POINTRATE/49/2)));
// 			speed3[l_u32BR] = speed2[l_u32BR]/2;
// 		}
 	}
}
// /*********************************************************************************************************
//** Function name:		FTaskBuild
//** Descriptions:		任务创建
//** input parameters:	none
//** return value:	    none
//** Created by:		wgh		  
//** Created Date:		2014-12-23	  
//*********************************************************************************************************/
void FTaskBuild(void)
{
#if	 OS_TASK_CREATE_EXT_EN >0
	OSTaskCreateExt(TaskRec2,(void *)0, &TaskRec2Stk[TASK_TIME_STACKSIZE-1], TASK_TIME_PRIO,
					TASK_TIME_PRIO,&TaskRec2Stk[0],TASKSTACKSIZE,&g_Taskstatics[3],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);	
	OSTaskCreateExt(TaskRec1,(void *)0, &TaskRec1Stk[TASK_LOGIC_STACKSIZE-1], TASK_LOGIC_PRIO,
					TASK_LOGIC_PRIO,&TaskRec1Stk[0],TASKSTACKSIZE,&g_Taskstatics[0],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);	
	OSTaskCreateExt(TaskRec4,(void *)0, &TaskRec4Stk[TASK_VEH_STACKSIZE-1], TASK_VEH_PRIO,
					TASK_VEH_PRIO,&TaskRec4Stk[0],TASKSTACKSIZE,&g_Taskstatics[2],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
  	OSTaskCreateExt(TaskRec6,(void *)0, &TaskRec6Stk[TASK_ALG_STACKSIZE-1], TASK_ALG_PRIO,
					TASK_ALG_PRIO,&TaskRec6Stk[0],TASK6STACKSIZE,&g_Taskstatics[1],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
  	OSTaskCreateExt(TaskRec7,(void *)0, &TaskRec7Stk[TASK_COMM_STACKSIZE-1], TASK_COMM_PRIO,
					TASK_COMM_PRIO,&TaskRec7Stk[0],TASKSTACKSIZE,&g_Taskstatics[4],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(Task_Laser_Interact,(void *)0, &Task_Laser_InteractStk[TASK_LASERINTR_STACKSIZE-1], Task_LASERINTR_PRIO,
					Task_LASERINTR_PRIO,&Task_Laser_InteractStk[0],TASKSTACKSIZE,&g_Taskstatics[7],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	
	OSTaskCreateExt(CheckStackTaskUse,(void *)0,&CheckStackTaskUseSTk[TASK_STK_SIZE_CHKTASK-1], TASK_PRI_CHKTASK, TASK_PRI_CHKTASK, CheckStackTaskUseSTk, TASK_STK_SIZE_CHKTASK, NULL, OS_TASK_OPT_STK_CLR);

#if DEBUGINFO_TASK_SP_EN > 0
	OSTaskCreateExt(TaskDebugInfo,(void *)0, &TaskDebugInfoStk[TASK_DEBUGINFO_STACKSIZE-1], TASK_DEBUGINFO_PRIO,
					TASK_DEBUGINFO_PRIO,&TaskDebugInfoStk[0],TASKSTACKSIZE,&g_Taskstatics[8],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);  
#endif
#if SD_DATA_STORAGE_EN > 0
	OSTaskCreateExt(TaskSDCard,(void *)0, &TaskSDCardStk[TASK_SD_STACKSIZE-1], TASK_SD_PRIO,
					TASK_SD_PRIO,&TaskSDCardStk[0],TASKSTACKSIZE,&g_Taskstatics[9],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);  
#endif
#else
    OSTaskCreate(TaskRec2,(void *)0, &TaskRec2Stk[TASK_TIME_STACKSIZE-1], TASK_TIME_PRIO);	 //时钟显示和接收静态数据	
	OSTaskCreate(TaskRec1,(void *)0, &TaskRec1Stk[TASK_LOGIC_STACKSIZE-1], TASK_LOGIC_PRIO);  //接收数据，做标识
  	OSTaskCreate(TaskRec3,(void *)0, &TaskRec3Stk[TASK_KEY_STACKSIZE-1], TASK_KEY_PRIO);	 //键盘事件处理	
	OSTaskCreate(TaskRec4,(void *)0, &TaskRec4Stk[TASK_VEH_STACKSIZE-1], TASK_VEH_PRIO);	 //车辆收尾后处理
	OSTaskCreate(TaskRec6,(void *)0, &TaskRec6Stk[TASK_ALG_STACKSIZE-1], TASK_ALG_PRIO); //计算轴重
    OSTaskCreate(TaskRec7,(void *)0, &TaskRec7Stk[TASK_COMM_STACKSIZE-1], TASK_COMM_PRIO);	 //协议相关
	OSTaskCreate(Task_Laser_Interact,(void *)0, &Task_Laser_InteractStk[TASK_LASERINTR_STACKSIZE-1], Task_LASERINTR_PRIO);	 //激光模块交互
	OSTaskCreate(TaskRec9,(void *)0, &TaskRec9Stk[TASK_JDINF_STACKSIZE-1], TASK_JDINF_PRIO);	 //交调数据上传
	OSTaskCreate(Task_UI,(void *)0, &Task_UIStk[TASK_UI_STACKSIZE-1], TASK_UI_PRIO);	 //屏幕显示
#if DEBUGINFO_TASK_SP_EN > 0
	OSTaskCreate(TaskDebugInfo,(void *)0, &TaskDebugInfoStk[TASK_DEBUGINFO_STACKSIZE-1], TASK_DEBUGINFO_PRIO);  
#endif
#if SD_DATA_STORAGE_EN > 0
	OSTaskCreate(TaskSDCard,(void *)0, &TaskSDCardStk[TASK_SD_STACKSIZE-1], TASK_SD_PRIO);  
#endif
 #endif	
	
    OSTaskCreate(TaskRecWave,(void *)0, &TaskRecWaveStk[TASK_WAVE_STACKSIZE-1], TASK_WAVE_PRIO);	 //发送波形
	if ( 0 == SETUPALIAS.u8SendWaveEnable )
	{
		 OSTaskSuspend(TASK_WAVE_PRIO);
	}
}
// /*********************************************************************************************************
//** Function name:		FIOStatusInt
//** Descriptions:		IO状态初始化
//** input parameters:	none
//** return value:	    none
//** Created by:		wgh		  
//** Created Date:		2014-12-23	  
//*********************************************************************************************************/
void FIOStatusInt(void)
{
	P3_OUTP_SET |= Camera_IO1_SET; //IO状态初始化
	P3_OUTP_SET |= Camera_IO2_SET; //IO状态初始化
	P3_OUTP_SET |= Camera_IO3_SET; //IO状态初始化
	P3_OUTP_SET |= Camera_IO4_SET; //IO状态初始化
	P3_OUTP_CLR |= SAMPLEPOWER; //采集板上电
}



