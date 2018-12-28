/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			FPGA_APP.h
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
** Modified by:		zhouhang	
** Modified date:	2013.2.26	
** Version:			1.0.1
** Descriptions:	����ʽ�汾 ��ӦFPGA��verilog�汾
**
*********************************************************************************************************/

#ifndef __FPGA_APP_H__
#define __FPGA_APP_H__

#include "config.h"
#include "gpio.h"

#ifdef	__FPGA_C
#define		FPGA_EXT
#else
#define		FPGA_EXT	extern	
#endif

//ת������
static INT8U  const  aFPGAChannelNUM2LogicNUMForLoop[8][8] = {		 //???????,????????
    0,  4, 1, 5, 2, 6, 3, 7,   
    8,  12,	9, 13,	10, 14, 11, 15,
    16,  20, 17, 21, 18, 22, 19, 23,   
    24,  28,  25,  29, 26, 30, 37, 31,   
    32, 36, 33, 37, 34, 38, 35, 39,       
    40, 44, 41, 45, 42, 46, 43, 47,      
    48, 52, 49, 53, 50, 54, 51, 55,     
    56, 60, 57, 61, 58, 62, 59, 63
};	 
/////////////////////////////////////////
//static INT8U  const  LaserNum2LoopNumFor2[16] = {		 //???????,????????
//    0, 2, 12, 4, 6, 14, 8, 10, 1, 3, 13, 5, 7, 15, 9, 11
//};	
//static INT8U  const  LaserNum2LoopNumFor3[24] = {		 //???????,????????
//    0, 3, 18, 6, 9, 21, 12, 15, 1, 4, 19, 7, 10, 22, 13, 16, 2, 5, 20, 8, 11, 23, 14, 17
//};
///////////////////////////////////////////
//static INT8U  const  LoopNum2LaserNumFor2[16] = {		 //???????,????????
//    0, 8, 1, 9, 3, 11, 4, 12, 6, 14, 7, 15, 2, 10, 5, 13
//};	
//static INT8U  const  LoopNum2LaserNumFor3[24] = {		 //???????,????????
//    0, 8, 16, 1, 9, 17, 3, 11, 19, 4, 12, 20, 6, 14, 22, 7, 15, 23, 2, 10, 18, 5, 13, 21
//};
/////////////////////////////////////////////
//static INT8U  const  LaserForRe3[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 4, 5, 0, 7, 8};
//static INT8U  const  LaserForRe2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 4, 5, 0, 7, 8};
/////////////////////////////////////////////
static INT8U  const	 LoopBoardNum[4][4] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10,11,
	12,13,14,15
};
//#if 0x44 == CHANGENUM_MODEL
//static INT8U  const  aFPGAChannelNUM2LogicNUM[8][8] = {		 //???????,????????
//     0,  1,  8,  9, 16, 17, 24, 25,   
//     2,  3,	10, 11,	18, 19, 26, 27,
//     4,  5, 12, 13, 20, 21, 28, 29,   
//     6,  7, 14, 15, 22, 23, 30, 31,
//    38, 39, 46, 47, 54, 55, 62, 63,
//	36, 37, 44, 45, 52, 53, 60, 61,  
//	34, 35, 42, 43, 50, 51, 58, 59,
//	32, 33, 40, 41, 48, 49, 56, 57  
//};
//static INT8U  const  aFPGALogicNUM2ChannelNUM[64] = {		 //???2???
//    25, 24, 17, 16,  9,  8,  1, 0,   
//    27, 26, 19, 18, 11, 10,  3, 2,
//    29, 28, 21, 20, 13, 12,  5, 4,   
//    31, 30, 23, 22, 15, 14,  7, 6,   
//    32, 33, 40, 41, 48, 49, 56, 57,       
//    34, 35, 42, 43, 50, 51, 58, 59,      
//    36, 37, 44, 45, 52, 53, 60, 61,     
//    38, 39, 46, 47, 54, 55, 62, 63
//};
//static INT8U  const  aFPGALogicNUM2LaneNUM[64] = {		 //???2???
//     0,  0,  3,  1,  1,  3,  2, 2,   
//     0,  0,  3,  1,  1,  3,  2, 2,
//     0,  0,  3,  1,  1,  3,  2, 2,   
//   255,255,255,255,255,255,255,255,
//   255,255,255,255,255,255,255,255,      
//   255,255,255,255,255,255,255,255,     
//   255,255,255,255,255,255,255,255,   
//   255,255,255,255,255,255,255,255,
//};
//static INT8U  const  aFPGALogicNUM2ChlNUM[64] = {		 //???2?????
//     0,  1,  0,  0,  1,  1,  0, 1,   
//     2,  3,  2,  2,  3,  3,  2, 3,
//     4,  5,  4,  4,  5,  5,  4, 5,   
//   255,255,255,255,255,255,255,255,
//   255,255,255,255,255,255,255,255,      
//   255,255,255,255,255,255,255,255,     
//   255,255,255,255,255,255,255,255,   
//   255,255,255,255,255,255,255,255,
//};
//#elif 0x40 == CHANGENUM_MODEL
//#elif 0x33 == CHANGENUM_MODEL
//#elif 0x30 == CHANGENUM_MODEL
//static INT8U  const  aFPGAChannelNUM2LogicNUM[8][8] = {		 //???????,????????
//     0,  1,  8,  9, 16, 17,255,255,   
//     3,  4,	11, 12,	19, 20,255,255,
//     6,  7, 14, 15, 22, 23,255,255,   
//     2,  5, 10, 13, 18, 21,255,255,
//   255,255,255,255,255,255,255,255,
//   255,255,255,255,255,255,255,255,  
//   255,255,255,255,255,255,255,255,
//   255,255,255,255,255,255,255,255  
//};
static INT8U  const  aFPGALogicNUM2LaneNUM[64] = {		 //???2???
     0,  0,  3,  1,  1,  3,  2, 2,   
     0,  0,  3,  1,  1,  3,  2, 2,
     0,  0,  3,  1,  1,  3,  2, 2,   
   255,255,255,255,255,255,255,255,
   255,255,255,255,255,255,255,255,      
   255,255,255,255,255,255,255,255,     
   255,255,255,255,255,255,255,255,   
   255,255,255,255,255,255,255,255,
};
static INT8U  const  aFPGALogicNUM2ChlNUM[64] = {		 //???2?????
     0,  1,  0,  0,  1,  1,  0, 1,   
     2,  3,  2,  2,  3,  3,  2, 3,
     4,  5,  4,  4,  5,  5,  4, 5,   
   255,255,255,255,255,255,255,255,
   255,255,255,255,255,255,255,255,      
   255,255,255,255,255,255,255,255,     
   255,255,255,255,255,255,255,255,   
   255,255,255,255,255,255,255,255,
};

static INT8U  const  aFPGALogicNUM2ChlNUMFANGSHAN[8][8] = {		 //???2?????
     0,  3,  1,  4, 2, 5,12,13,   
     6,  9,	7, 10,	8, 11,14,255,
     255,  255, 255, 255, 255, 255,255,255,   
     255,  255, 255, 255, 255, 255,255,255,
   255,255,255,255,255,255,255,255,
   255,255,255,255,255,255,255,255,  
   255,255,255,255,255,255,255,255,
   255,255,255,255,255,255,255,255 
};
//#elif 0x22 == CHANGENUM_MODEL
//#elif 0x20 == CHANGENUM_MODEL
//#else
//#endif

//static int ZtNumRec111[100]={0};

static INT8U const aChannelNUM2RoadNUM[64]={   //Channel???2???     ????? wujinlong 20130815
	0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7
};

//	�����趨
#define FPGA_EMC_CHECK 		1	//�Ƿ���EMC���
#define FPGA_POWON_MAX		200000 //FPGA�ϵ�ȴ�ʱ�䣬������������ֵ����
#define FPGA_CHECK_MAX		10	//���EMC���ߴ�������ֵ������������ֵ��ΪFPGA��ARMͨ�Ŵ�������


// ����ַ���																									 
#define FPGA_CS3_BASE		( 0xE3000000 )	 //CS3��ַ

// �ж����
#define FPGA_INT_CLEAR		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008100<<1)) )	 //���ж�
#define FPGA_INT_TYPE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008101<<1)) )	 //�ж�Դ
#define FPGA_INT_ENABLE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008102<<1)) )	 //�ж�ʹ��
#define FPGA_INT_UNENABLE	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008103<<1)) )	 //�жϽ���
#define FPGA_INT_ENTERFLAG	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008104<<1)) )	 //���жϱ�־λ���������ߵ�ƽ
#define FPGA_RESET			(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008110<<1)) )	 //FPGA��λ

//���üĴ���
#define FPGA_CONFIG_X(cfg_num)	(*(volatile uint16 *)(FPGA_CS3_BASE + ((0x008200+(cfg_num))<<1)))		  //�忨���üĴ��� ��д
#define CFG_1		0x0
#define CFG_2		0x1
#define CFG_3		0x2
#define CFG_4		0x3
#define CFG_5		0x4
#define CFG_6		0x5
#define CFG_7		0x6
#define CFG_8		0x7

#define SET_BOARD_MASK(cfg_num,ch)	(FPGA_CONFIG_X(cfg_num) |= (1<<(4+(ch))))			//����ͨ�������������ͨ����BITȡֵ0~7
#define CLR_BOARD_MASK(cfg_num,ch)	(FPGA_CONFIG_X(cfg_num) &= (~(1<<(4+(ch)))))		//����ͨ�������������ͨ��
#define CH1	0x0
#define CH2	0x1
#define CH3	0x2
#define CH4	0x3
#define CH5	0x4
#define CH6	0x5
#define CH7	0x6
#define CH8	0x7


#define ENABLE_REAL_WAVE(cfg_num)		(FPGA_CONFIG_X(cfg_num) |= 0x4)				//����ʹ��ʵʱ����
#define DISABLE_REAL_WAVE(cfg_num)		(FPGA_CONFIG_X(cfg_num) &= 0xFFFB)			//����ʵʱ����
#define SET_WAVE_MODE1(cfg_num)			(FPGA_CONFIG_X(cfg_num) |= 0x8)				//��������1 	����ֵ
#define SET_WAVE_MODE2(cfg_num)			(FPGA_CONFIG_X(cfg_num) &= 0xFFF7)			//��������2	    �������������ֵ

#define ZT_BOARD	0x0			 //խ���ɼ���
#define SY_BOARD	0x1			 //ʯӢ�ɼ���
#define XQ_BOARD	0x3			 //��Ȧ��
#define SET_BOARD_TYPE(cfg_num,type)	(FPGA_CONFIG_X(cfg_num) = ((FPGA_CONFIG_X(cfg_num) & 0xFFFC) | (type)))			 //ѡ��忨���� 

// ʱ���
#define FPGA_GLABLETIMELOCK				( (*(volatile uint16 *)(FPGA_CS3_BASE + (0x008108<<1))) = 1 )		  // ��������
#define FPGA_GLOBALTIME					(( (*(volatile uint16 *) (FPGA_CS3_BASE + ((0x008106 )<<1))) & 0xFFFF)	\
											+	(((*(volatile uint16 *) (FPGA_CS3_BASE + ((0x008105 )<<1))) & 0xFFFF)<<16))	  	// ʱ��

#define FPGA_GLABLETIMECLEAR			( (*(volatile uint16 *)(FPGA_CS3_BASE + (0x008107<<1))) = 0x0101 )		  // ʱ������

// ���ԼĴ���
#define FPGA_TEST_1			(*(volatile uint16 *)(FPGA_CS3_BASE + (0x00AAAA<<1)))
#define FPGA_TEST_2			(*(volatile uint16 *)(FPGA_CS3_BASE + (0x00D555<<1)))
//���� 20130416 TASK1  ��FPGA��дһ��
#define FPGA_LASER_STATUS		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008600<<1)))

//ģ���üĴ���
#define FPGA_SIM_LASER_STATUS		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008601<<1)))
#define FPGA_SIM_FRAME_ID		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008602<<1)))


//���ݼ������ݣ�дFPGA���ݣ�֪ͨFPGA��ʼ���Ͳ���
#define NOTIFYFPGASENDDATA1		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008240<<1)))
#define NOTIFYFPGASENDDATA2		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008241<<1)))
#define NOTIFYFPGASENDDATA3		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008242<<1)))
#define NOTIFYFPGASENDDATA4		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008243<<1)))
#define NOTIFYFPGASENDDATA5		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008244<<1)))
#define NOTIFYFPGASENDDATA6		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008245<<1)))
#define NOTIFYFPGASENDDATA7		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008246<<1)))
#define NOTIFYFPGASENDDATA8		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008247<<1)))

#define WRITENOTIFY1()			NOTIFYFPGASENDDATA1 = 0x1234;
#define WRITENOTIFY2()			NOTIFYFPGASENDDATA2 = 0x1234;
#define WRITENOTIFY3()			NOTIFYFPGASENDDATA3 = 0x1234;
#define WRITENOTIFY4()			NOTIFYFPGASENDDATA4 = 0x1234;
#define WRITENOTIFY5()			NOTIFYFPGASENDDATA5 = 0x1234;
#define WRITENOTIFY6()			NOTIFYFPGASENDDATA6 = 0x1234;
#define WRITENOTIFY7()			NOTIFYFPGASENDDATA7 = 0x1234;
#define WRITENOTIFY8()			NOTIFYFPGASENDDATA8 = 0x1234;


// �������
#define FPGA_UPVALUE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008300<<1)) )	 //�ϳ���ֵ
#define FPGA_DNVALUE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008301<<1)) )	 // �³���ֵ
#define FPGA_UPVALUE_SE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008302<<1)) )	 //�ڶ��ϳ���ֵ
#define FPGA_DOWNVALUE_SE	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008303<<1)) )	 //�ڶ��³���ֵ
#define FPGA_UP_NUM			(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008304<<1)) )	 //�ϳ��жϵ��� ֻд
#define FPGA_DOWN_NUM		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008305<<1)) )	 //�³��жϵ��� ֻд

#define SET_FPGA_DOWNVALUE_SE()	FPGA_DOWNVALUE_SE = g_Setup.u16DownValue
#define SET_FPGA_UPVALUE_SE()	FPGA_UPVALUE_SE = g_Setup.u16UpValue	
#define SET_FPGA_UPVALUE()	    FPGA_UPVALUE = g_Setup.u16logicUpValue
#define SET_FPGA_DNVALUE()	    FPGA_DNVALUE = g_Setup.u16logicDownValue
#define	SET_FPGA_UPPOINTS()	    FPGA_UP_NUM = g_Setup.u8UPPoints
#define	SET_FPGA_DOWNPOINTS()   FPGA_DOWN_NUM = g_Setup.u8DownPoints

// ���ִ�������ز���
#define FPGA_DIGITAL_UPVALUE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008003<<1)) )	 //�ϳ���ֵ
#define FPGA_DIGITAL_DNVALUE		(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008004<<1)) )	 //�³���ֵ
#define FPGA_DIGITAL_UPVALUE_SE	    (*(volatile uint16 *)(FPGA_CS3_BASE + (0x008005<<1)) )	 //�ڶ��ϳ���ֵ
#define FPGA_DIGITAL_DOWNVALUE_SE	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008006<<1)) )	 //�ڶ��³���ֵ
// #define FPGA_DIGITAL_CMD_SEND_START	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008000<<1)) )
#define FPGA_DIGITAL_CMD_SEND_START	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008AA9<<1)) )

#define SET_FPGA_DIGITAL_UPVALUE()	       FPGA_DIGITAL_UPVALUE = g_Setup.u16logicUpValue*11/10
#define SET_FPGA_DIGITAL_DNVALUE()	       FPGA_DIGITAL_DNVALUE = g_Setup.u16logicDownValue*11/10
#define SET_FPGA_DIGITAL_UPVALUE_SE()	   FPGA_DIGITAL_UPVALUE_SE = g_Setup.u16UpValue*11/10
#define SET_FPGA_DIGITAL_DOWNVALUE_SE()	   FPGA_DIGITAL_DOWNVALUE_SE = g_Setup.u16DownValue*11/10
// #define SET_FPGA_DIGITAL_CMD_SEND_START()  FPGA_DIGITAL_CMD_SEND_START |= 0x0001
#define SET_FPGA_DIGITAL_CMD_SEND_START()  FPGA_DIGITAL_CMD_SEND_START = 0x0001


#define MASTER_FPGA_VERSION_H	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008400<<1)))	//���ذ�FPGA�汾�Ÿ�16λ
#define MASTER_FPGA_VERSION_L	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008401<<1)))	//���ذ�FPGA�汾�ŵ�16λ

#define SLAVE_FPGA_VERSION_H(board_id) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + 0x1A0 + 0x3F)<<1))) & 0xFFFF) 	// ˲ʱֵ

#define SLAVE_FPGA_VERSION_L(board_id) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + 0x1A0 + 0x3E)<<1))) & 0xFFFF) 	// ˲ʱֵ

#define TEMPERATURE	 (*(volatile uint16 *)(FPGA_CS3_BASE + (0x008500<<1))) //�¶ȴ�������ֵ

#define	FPGA_STATUS	(*(volatile uint16 *)(FPGA_CS3_BASE + (0x008410<<1)))	//FPGA״̬

// �жϿ���
#define ClearFPGA_INT()		FPGA_INT_CLEAR = 0x1357;
#define EnableFPGA_INT()	FPGA_INT_ENABLE = 0x5678;
#define DisableFPGA_INT()	FPGA_INT_UNENABLE = 0;
#define EnterFlagFPGA_INT()	FPGA_INT_ENTERFLAG = 0x0101;

//��λ
#define RESET_FPGA()	FPGA_RESET = 0x55AA;	
																							  
// ���ݵ�ַ


// ��ȡ�����к����
#define GETDATAADDR(l_u16ID)   	((volatile uint16 *)( FPGA_CS3_BASE + ((l_u16ID << 6)<<1)) )
#define	GETFRAMESTYLE(l_u16ID)	( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x32 )<<1))) & 0x03) 	   	// ֡����
#define	GETFRAMENUM(l_u16ID)	( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x34 )<<1))) & 0xFFFF)	  	 // ֡���
#define	GETDATANUM(l_u16ID)		( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x35 )<<1))) & 0x3F) 	   	// ����Ŀ
#define	GETMAXDATA(l_u16ID)		( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x36 )<<1))) & 0xFFFF) 	  	// ���ֵ
#define	GETFITDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x37 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x38 )<<1))) & 0xFFFF)<<16))	  	// һ���³Ƶ�
#define	GETSUM(l_u16ID)			(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x39 )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x3A )<<1))) & 0xFFFF)<<16  ))			// ���ֺ�
#define	GETSECUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x3B )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x3C )<<1))) & 0xFFFF)<<16  ))			// �ϳ�ʱ��
#define	GETSECDNTIME(l_u16ID)	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x3D )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x3E )<<1))) & 0xFFFF)<<16  ))		// �³�ʱ��
#define	GETMAXTIME(l_u16ID)	((( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x32 )<<1))) & 0xFFFC)>>2)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x33 )<<1))) & 0xFF00) << 6) \
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 6) + 0x35 )<<1))) & 0xFFC0) <<16))		// ��ֵʱ��

//����խ����ͬ���õ�ֵ
#define AGETDATAADDR(l_u16ID)   	((volatile uint16 *)( FPGA_CS3_BASE + ((l_u16ID << 8)<<1)))
#define	ABGETFRAMESTYLE(l_u16ID)	( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x32 )<<1))) & 0x03) 	   	// ֡����
#define	ABGETFRAMENUM(l_u16ID)	( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x34 )<<1))) & 0xFFFF)	  	 // ֡���
#define	ABGETDATANUM(l_u16ID)		( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x35 )<<1))) & 0x3F) 	   	// ����Ŀ
#define	ABGETMAXDATA(l_u16ID)		( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x36 )<<1))) & 0xFFFF) 	  	// ���ֵ
#define	ABGETFITDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x37 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x38 )<<1))) & 0xFFFF)<<16))	  	// һ���³Ƶ�
#define	ABGETSUM(l_u16ID)			(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x39 )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x3A )<<1))) & 0xFFFF)<<16  ))			// ���ֺ�
#define	ABGETSECUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x3B )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x3C )<<1))) & 0xFFFF)<<16  ))			// �ϳ�ʱ��
#define	ABGETSECDNTIME(l_u16ID)	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x3D )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x3E )<<1))) & 0xFFFF)<<16  ))		// �³�ʱ��
#define	ABGETMAXTIME(l_u16ID)	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x40 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x41 )<<1))) & 0xFFFF)<<16 ))		// ��ֵʱ��

//��һ��խ����������Ϣ
#define	GETFIRSTZTFITUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x42 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x43 )<<1))) & 0xFFFF)<<16 ))		// ��һ��խ��һ���ϳ�									
#define	GETFIRSTZTSECUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x44 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x45 )<<1))) & 0xFFFF)<<16 ))		// ��һ��խ�������ϳ�
#define	GETFIRSTZTFITDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x46 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x47 )<<1))) & 0xFFFF)<<16 ))		// ��һ��խ��һ���³�
#define	GETFIRSTZTSECDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x48 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x49 )<<1))) & 0xFFFF)<<16 ))		// ��һ��խ�������³�
#define	GETFIRSTZTMAXDATA(l_u16ID)		((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4A )<<1))) & 0xFFFF)	// ��һ��խ����ֵ
#define	GETFIRSTZTMAXDATAPOS(l_u16ID)  ((((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4B )<<1))) & 0xFF)>>4)&0x0F)	// ��һ��խ����ֵ��λ��
#define	GETFIRSTZTUPDNPOS(l_u16ID)  ((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4B )<<1)))&0x0F)	// ��һ��խ�����ļ���Ӧ��Ƭ����
#define	GETFIRSTZTSUM(l_u16ID)  (( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4C )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4D )<<1))) & 0xFFFF)<<16  ))			// ��һ��խ�����ֺ�


//�ڶ���խ����������Ϣ
#define	GETSECZTFITUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4E )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x4F )<<1))) & 0xFFFF)<<16 ))		// �ڶ���խ��һ���ϳ�									
#define	GETSECZTSECUPTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x50 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x51 )<<1))) & 0xFFFF)<<16 ))		// �ڶ���խ�������ϳ�
#define	GETSECZTFITDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x52 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x53 )<<1))) & 0xFFFF)<<16 ))		// �ڶ���խ��һ���³�
#define	GETSECZTSECDNTIME(l_u16ID)		(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x54 )<<1))) & 0xFFFF)	\
									+	(((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x55 )<<1))) & 0xFFFF)<<16 ))		// �ڶ���խ�������³�
#define	GETSECZTMAXDATA(l_u16ID)		((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x56 )<<1))) & 0xFFFF)	// �ڶ���խ����ֵ
#define	GETSECZTMAXDATAPOS(l_u16ID)  ((((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x57 )<<1))) & 0xFF)>>4)&0x0F)	// �ڶ���խ����ֵ��λ��
#define	GETSECZTUPDNPOS(l_u16ID)  ((*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x57 )<<1)))&0x0F)	// �ڶ���խ�����ļ���Ӧ��Ƭ����
#define	GETSECZTSUM(l_u16ID)  (( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x58 )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + (((l_u16ID << 8) + 0x59 )<<1))) & 0xFFFF)<<16  ))			// �ڶ���խ�����ֺ�
#define BGETDATAADDR(l_u16ID)   	((volatile uint16 *)( FPGA_CS3_BASE + (((l_u16ID << 8) + 0x60)<<1)))//ȡ��������

#define GETCURRENTDATA(board_id,channel_id) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + ((channel_id) << 6) + 0x3F )<<1))) & 0xFFFF) 	   	// ˲ʱֵ
#define GETCURRENTZERO(board_id,channel_id) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + (0x1 << 9) + ((channel_id) << 6) + 0x3F )<<1))) & 0xFFFF) 	// ���ֵ
#define GETCURRENTDATA_DIGITAL(board_id,offset) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + 0x1A0 + offset )<<1))) & 0xFFFF) 	// ˲ʱֵ
#define GETCURRENTZERO_DIGITAL(board_id,offset) ( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((board_id) << 11) + 0xA0 + offset )<<1))) & 0xFFFF) 	// ���ֵ

#define GET_MASTER_FPGA_VERSION()	(((uint32)MASTER_FPGA_VERSION_H << 16) | ((uint32)MASTER_FPGA_VERSION_L))
#define GET_SLAVE_FPGA_VERSION(board_id) (((uint32)SLAVE_FPGA_VERSION_H(board_id)  << 16) | ((uint32)SLAVE_FPGA_VERSION_L(board_id)))

//�������ݽ����趨�忨Ϊ��Ȧ����Ե���
#define GET_LOOP_FRAME_ID(l_u8type)	(((*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x0 )<<1))) >>4 )& 0x000F) 	//��ȡ��Ȧ֡�����Ϣ	
#define GET_LOOP_STYLE(l_u8type)	( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x1 )<<1))) & 0x00FF)	//��ȡ��Ȧ֡������Ϣ
#define GET_LOOP_DATA(l_u8type)	(( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x3 )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x2 )<<1))) & 0xFFFF)<<16  ))		// ��ȡ��Ȧ֡����
#define GET_LOOP_TIME(l_u8type)	(( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x5 )<<1))) & 0xFFFF)	\
									+	(( (*(volatile uint16 *) (FPGA_CS3_BASE + ((((l_u8type) << 6) + 0x4 )<<1))) & 0xFFFF)<<16  ))		//��ȡ֡����ʱ��			
///////////////////////////////////////

// ������խ����Ϣ
#define GetLane(l_u16ID)		( ((l_u16ID) & 0xE0)>>5 )
#define GetZtNum(l_u16ID)		( (l_u16ID) & 0x07 )


///////////����////////////////
#define	ReadFPGAGlobalTime(l_u32Time)	FPGA_GLABLETIMELOCK;\
										l_u32Time = FPGA_GLOBALTIME;								// ��ʱ��
#define	ClearFPGAGlobalTime()			FPGA_GLABLETIMECLEAR;									// ʱ������

//FPGA_EXT uint8 FPGAInitParam(uint16 *pParam); //�״γ�ʼ��
FPGA_EXT uint8 FPGAInitParam(void); 

FPGA_EXT uint8 FPGAGetZeroValue(uint16 *pretData,uint16 chlSum); //��ȡ����ͨ�����ֵ

FPGA_EXT uint8 FPGASetZeroValue(uint16 *pSetData,uint16 chlSum); //�趨����ͨ�����ֵ

FPGA_EXT uint8 FPGAEnableZeroCheck(uint8 FlagSet);  //����������,FlagSet =1��ʾ������㣬FlagSet = 0��ֹͣ������ʱ������

FPGA_EXT uint8 FPGAGetChlCurValue(uint16 *pSetData,uint16 chlSum); //��ȡ����ͨ��˲ʱֵ

FPGA_EXT void  IRQ_FPGA(void);	 //

FPGA_EXT uint16 Read_Fpga_Status(void);

FPGA_EXT fp32 FPGA_Temperature(void);


#endif
