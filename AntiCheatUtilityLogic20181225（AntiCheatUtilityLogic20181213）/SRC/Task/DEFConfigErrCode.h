/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			DEFConfigErrCode.h
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		�������
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


// ������� ÿ������64����������
// 0x00000000-0x0000003F��ȫ�����
#define	SUCCESS						0x00000000
#define	FAIL						0xFFFFFFFF
#define	ERR_UNKNOWN					0x00000001


// 0x00000040-0x0000007F���߼����
#define	ERR_LOGIC_SEPVEH			0x00000040		// 1����2��


// 0x00000080-0x000000BF���㷨���


// 0x000000C0-0x000000FF���������
#define	ERR_VEH_NOTAXLECNT1			0x000000C0		// ����1��
#define	ERR_VEH_NOTAXLECNT0			0x000000C1		// ����0��
#define	ERR_VEH_DEFAULTVEH			0x000000C2		// Ĭ�ϳ��ͼ������
#define	ERR_VEH_NOTWEGHT0			0x000000C3		// û������Ϊ0
#define	ERR_VEH_AXLECNT_OVER6		0x000000C4		// ��������6

// 0x00000100-0x0000013F��ͨ�����
#define	ERR_NET_NOCON				0x00000100

// 0x00000140-0x0000017F��SD���
#define	ERR_SD_NOTNEW				0x00000140		// ���SD�������µ�SD��������
#define	ERR_SD_NEW_CARD				0x00000141		// ���SD���µ�SD
#define	ERR_SD_NEW_SECTOR			0x00000142		// ���SD���µ�����
#define	ERR_SD_FRAMLEN_OVER			0x00000143		// ��֡����


// 0x00000180-0x000001BF��FPGA���




// 0x000001C0-0x000001FF��Ԥ��1

// 0x00000200-0x0000023F��Ԥ��2

// 0x00000240-0x0000027F��Ԥ��3

// 0x00000280-0x000002BF��Ԥ��4

// 0x000002C0-0x000002FF��Ԥ��5


// 0x00000300-0x0000033F������




	
#endif
