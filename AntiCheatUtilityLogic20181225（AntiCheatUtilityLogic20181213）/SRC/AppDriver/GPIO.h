/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			GPIO.h
** Last modified Date:  2011-03-26
** Last Version:		1.0
** Descriptions:		通用接口函数，如振铃等，包括LCD
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		2011-03-26
** Version:				1.0
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__GPIO_H
#define	__GPIO_H

#include "Config.h"

#ifdef __GPIO_C
#define		GPIO_EXT
#else
#define		GPIO_EXT	extern
#endif

#define		BEEP				1 << 6					/* GPO_07控制BEEP，低电平蜂鸣   */

#define		BeepON()			P3_OUTP_SET = BEEP		//振铃响
#define		BeepOFF()			P3_OUTP_CLR = BEEP		//振铃不响

#define		LEDBIT				(1<<9)					//GPO_9

#define		LEDON()				P3_OUTP_CLR |= LEDBIT	//LED亮
#define		LEDOFF()			P3_OUTP_SET |= LEDBIT	//LED灭


#define		BDMODE				(P3_INP_STATE & (1<<2))		//拨码开关，是否进入标定状态

#define		LCM_LEDCON			(1<<14)					//控制背光开关  GPO14
#define		LCDRST				(1<<1)					//LCD复位引脚   GPO01

#define		DELAY_TIME			100						/* 基本延迟时间					*/
#define		TICKS_PER_SEC		100						/* 每秒钟Tick数,与OS_CFG.H一致	*/

#define		GSTriggerPIN		(((P3_INP_STATE >> 28) & 1))		//遮挡光栅信号：判断是否遮挡光栅	0:遮挡  1:松开
#define		LPTriggerPIN		((P3_INP_STATE >> 16) & 1)		//触发线圈信号
#define		GSErrPIN			((P3_INP_STATE >> 4) & 1)		//光栅故障信号
#define		LPErrPIN			((P3_INP_STATE >> 5) & 1)		//线圈故障信号

#define		OUTBGON				((P3_INP_STATE >> 4) & 1)		//外部点亮显示屏跳线

//
#define W5100_NOLINK		((P3_INP_STATE >> 6) & 1)

// 输入IO
#define	CoilIN_IO1	((P3_INP_STATE >> 23) & 1)	 //按大底板丝印 “输入1” ，对应LPC3250的GPI_23，对应绿端子P27上方，下面按次方式类推
#define CoilIN_IO2	((P3_INP_STATE >> 28) & 1)	 // 输入1-5
#define CoilIN_IO3	((P3_INP_STATE >> 16) & 1)	
#define CoilIN_IO4	((P3_INP_STATE >> 22) & 1)	  
#define CoilIN_IO5	((P3_INP_STATE >> 19) & 1)

#define CoilIN_IO6		((P3_INP_STATE >> 4 ) & 1)
#define	CoilIN_IO7		((P3_INP_STATE >> 5 ) & 1)	 // 线圈3个  输入6-10
#define	CoilIN_IO8		((P3_INP_STATE >> 8 ) & 1)
#define	CoilIN_IO9		((P3_INP_STATE >> 9 ) & 1)
#define	CoilIN_IO10		((P3_INP_STATE >> 21) & 1)

//采集板上电
#define SAMPLEPOWER     ( 1<<7 )	//采集板上电
// 控制相机，头牌
//#define	Camera_IO1_SET	( 1<<5 )	 //待定的IO寄存器位信息
//#define Camera_IO2_SET	( 1<<20 )
#define	Camera_IO1_SET	( 1<<5 )	 //相机 4个	 输出1-8
#define Camera_IO2_SET	( 1<<0 )		
#define Camera_IO3_SET	( 1<<19 )	
#define Camera_IO4_SET	( 1<<8 )
//	  
#define Camera_IO5_SET	( 1<<13 )
#define Camera_IO6_SET	( 1<<10 )
#define Camera_IO7_SET	( 1<<18 )
#define Camera_IO8_SET	( 1<<16 )

#define Camera_IO9_SET	( 1<<5 )
#define Camera_IO10_SET	( 1<<7 )
#define Camera_IO11_SET	( 1<<2 )
#define Camera_IO12_SET	( 1<<3 )
#define Camera_IO13_SET	( 1<<4 )
#define Camera_IO14_SET	( 1<<21 )
#define Camera_IO15_SET	( 1<<12 )
#define Camera_IO16_SET	( 1<<22 )
#define Camera_IO17_SET	( 1<<7 )
#define Camera_IO18_SET	( 1<<20 )

// 控制相机，尾牌	  
#define	CameraShouwei_IO1_SET	( 1<<8 )	 //待定的IO寄存器位信息
#define CameraShouwei_IO2_SET	( 1<<7 )	
#define CameraShouwei_IO3_SET	( 1<<5 )	
#define CameraShouwei_IO4_SET	( 1<<0 )	  
#define CameraShouwei_IO5_SET	( 1<<29 )
#define CameraShouwei_IO6_SET	( 1<<2 )	 
#define CameraShouwei_IO7_SET	( 1<<3 )
#define CameraShouwei_IO8_SET	( 1<<4 )




 
//
#define     GPO_02              (1<<2)
#define     GPO_03              (1<<3)
#define     GPO_07              (1<<7)
#define     GPO_08              (1<<8)

#define     GPI_19              (1<<19)
#define     GPI_22              (1<<22)

GPIO_EXT	void	Delay(uint32 p_u32TickNumber);				/* 延时函数，参数为延时数		*/
GPIO_EXT	void    IOInit(void);

#endif		//__GPIO_C
