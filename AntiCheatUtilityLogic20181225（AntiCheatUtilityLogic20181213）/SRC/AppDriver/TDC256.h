/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			TDC256.h
** Last modified Date:  20110517
** Last Version:		1.0
** Descriptions:		铁电
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110517
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
#ifndef __TDC256_H

#define __TDC256_H

#ifdef __TDC256_C
#define	TD_EXT
#include "I2C1.h"
#else
#define	TD_EXT	extern
#endif

#include "config.h"

#define			SETUPPARAMADDR			0x0000	   	//配置参数setup的地址	   6K
//#define			HISTORYBDADDR			0x1000		//出厂参数地址 			   4K
#define         PROMODELPARAMDDR    	0x2000      //选择线圈模式还是激光模式逻辑的参数地址 1K wgh 20140303
#define         HIS_PROMODELPARAM    	0x2400	   	// 出厂参数 1K
#define			NETINFOADDR				0x2800		//网口信息数据	 36  0x10-3F
#define         JDSYSADDR               0x2900		//512

#define         JD01CURNUM              0x2B00
#define         JD01SUCNUM              0x2B04
#define         JD01FIRSTFLAG           0x2B08
#define         JD39CURNUM              0x2B10
#define         JD39SUCNUM              0x2B14
#define         JD39FIRSTFLAG           0x2B18
#define         JD61CURNUM              0x2B20
#define         JD61SUCNUM              0x2B24
#define         JD61FIRSTFLAG           0x2B28
#define         JD62CURNUM              0x2B30
#define         JD62SUCNUM              0x2B34
#define         JD62FIRSTFLAG           0x2B38
#define         JD68CURNUM              0x2B40
#define         JD68SUCNUM              0x2B44
#define         JD68FIRSTFLAG           0x2B48	

#define			SD_ADDR					0x3000		// SD 532*5


#define		STARTTIMESADDR		0x5C00			//启动次数
			

#define		UPDATERECINDEXADDR	0x5C07			//刷程序记录序号
#define		STARTRECINDEXADDR	0x5C08			//程序启动记录序号


TD_EXT	uint8	ReadC256(uint16 p_u16Addr, uint8 * p_pu8ReadBuf, uint16 p_u16Len);
TD_EXT	uint8	WriteC256(uint16 p_u16Addr, uint8 * p_pu8WriteBuf, uint16 p_u16Len);

#endif		//__TDC256_H
