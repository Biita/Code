/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			DEFConfigErrCode.h
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		错误代码
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

#ifndef	__DEFCONFIGREECODE_H
#define	__DEFCONFIGREECODE_H


// 错误代码 每种类型64个错误种类
// 0x00000000-0x0000003F，全局相关
#define	SUCCESS						0x00000000
#define	FAIL						0xFFFFFFFF
#define	ERR_UNKNOWN					0x00000001


// 0x00000040-0x0000007F，逻辑相关
#define	ERR_LOGIC_SEPVEH			0x00000040		// 1车分2车


// 0x00000080-0x000000BF，算法相关


// 0x000000C0-0x000000FF，出车相关
#define	ERR_VEH_NOTAXLECNT1			0x000000C0		// 不是1轴
#define	ERR_VEH_NOTAXLECNT0			0x000000C1		// 不是0轴
#define	ERR_VEH_DEFAULTVEH			0x000000C2		// 默认车型计算错误
#define	ERR_VEH_NOTWEGHT0			0x000000C3		// 没有重量为0
#define	ERR_VEH_AXLECNT_OVER6		0x000000C4		// 轴数超过6

// 0x00000100-0x0000013F，通信相关
#define	ERR_NET_NOCON				0x00000100

// 0x00000140-0x0000017F，SD相关
#define	ERR_SD_NOTNEW				0x00000140		// 检测SD，不是新的SD或者扇区
#define	ERR_SD_NEW_CARD				0x00000141		// 检测SD，新的SD
#define	ERR_SD_NEW_SECTOR			0x00000142		// 检测SD，新的扇区
#define	ERR_SD_FRAMLEN_OVER			0x00000143		// 组帧超长


// 0x00000180-0x000001BF，FPGA相关




// 0x000001C0-0x000001FF，预留1

// 0x00000200-0x0000023F，预留2

// 0x00000240-0x0000027F，预留3

// 0x00000280-0x000002BF，预留4

// 0x000002C0-0x000002FF，预留5


// 0x00000300-0x0000033F，其他




	
#endif
