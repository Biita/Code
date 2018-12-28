/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			GPIO.h
** Last modified Date:  2011-03-26
** Last Version:		1.0
** Descriptions:		ͨ�ýӿں�����������ȣ�����LCD
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

#define		BEEP				1 << 6					/* GPO_07����BEEP���͵�ƽ����   */

#define		BeepON()			P3_OUTP_SET = BEEP		//������
#define		BeepOFF()			P3_OUTP_CLR = BEEP		//���岻��

#define		LEDBIT				(1<<9)					//GPO_9

#define		LEDON()				P3_OUTP_CLR |= LEDBIT	//LED��
#define		LEDOFF()			P3_OUTP_SET |= LEDBIT	//LED��


#define		BDMODE				(P3_INP_STATE & (1<<2))		//���뿪�أ��Ƿ����궨״̬

#define		LCM_LEDCON			(1<<14)					//���Ʊ��⿪��  GPO14
#define		LCDRST				(1<<1)					//LCD��λ����   GPO01

#define		DELAY_TIME			100						/* �����ӳ�ʱ��					*/
#define		TICKS_PER_SEC		100						/* ÿ����Tick��,��OS_CFG.Hһ��	*/

#define		GSTriggerPIN		(((P3_INP_STATE >> 28) & 1))		//�ڵ���դ�źţ��ж��Ƿ��ڵ���դ	0:�ڵ�  1:�ɿ�
#define		LPTriggerPIN		((P3_INP_STATE >> 16) & 1)		//������Ȧ�ź�
#define		GSErrPIN			((P3_INP_STATE >> 4) & 1)		//��դ�����ź�
#define		LPErrPIN			((P3_INP_STATE >> 5) & 1)		//��Ȧ�����ź�

#define		OUTBGON				((P3_INP_STATE >> 4) & 1)		//�ⲿ������ʾ������

//
#define W5100_NOLINK		((P3_INP_STATE >> 6) & 1)

// ����IO
#define	CoilIN_IO1	((P3_INP_STATE >> 23) & 1)	 //����װ�˿ӡ ������1�� ����ӦLPC3250��GPI_23����Ӧ�̶���P27�Ϸ������水�η�ʽ����
#define CoilIN_IO2	((P3_INP_STATE >> 28) & 1)	 // ����1-5
#define CoilIN_IO3	((P3_INP_STATE >> 16) & 1)	
#define CoilIN_IO4	((P3_INP_STATE >> 22) & 1)	  
#define CoilIN_IO5	((P3_INP_STATE >> 19) & 1)

#define CoilIN_IO6		((P3_INP_STATE >> 4 ) & 1)
#define	CoilIN_IO7		((P3_INP_STATE >> 5 ) & 1)	 // ��Ȧ3��  ����6-10
#define	CoilIN_IO8		((P3_INP_STATE >> 8 ) & 1)
#define	CoilIN_IO9		((P3_INP_STATE >> 9 ) & 1)
#define	CoilIN_IO10		((P3_INP_STATE >> 21) & 1)

//�ɼ����ϵ�
#define SAMPLEPOWER     ( 1<<7 )	//�ɼ����ϵ�
// ���������ͷ��
//#define	Camera_IO1_SET	( 1<<5 )	 //������IO�Ĵ���λ��Ϣ
//#define Camera_IO2_SET	( 1<<20 )
#define	Camera_IO1_SET	( 1<<5 )	 //��� 4��	 ���1-8
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

// ���������β��	  
#define	CameraShouwei_IO1_SET	( 1<<8 )	 //������IO�Ĵ���λ��Ϣ
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

GPIO_EXT	void	Delay(uint32 p_u32TickNumber);				/* ��ʱ����������Ϊ��ʱ��		*/
GPIO_EXT	void    IOInit(void);

#endif		//__GPIO_C
