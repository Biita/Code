/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			DEFConfigConst.h
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		�곣��
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


#ifndef	__DEFCONFIGCONST_H
#define	__DEFCONFIGCONST_H


//��������д������Բ�����صĲ��ֺ궨�壺
#define		CPTB_TRIGGER_LASER			0x01
#define		CPTB_TRIGGER_SCOIL			0x02
#define		CPTB_TRIGGER_DCOIL			0x04
//
#define		CPTB_UPLOAD_NET				0x01
#define		CPTB_UPLOAD_SP				0x02
#define		CPTB_UPLOAD_WIRELESS		0x04
//
#define		CPTB_CAPTURE_HARD			0x01
#define		CPTB_CAPTURE_SOFT_L			0x02
#define		CPTB_CAPTURE_SOFT_U			0x04
//
#define		CPTB_SDSAVE_MODE_EN			0x01
#define		CPTB_SDSAVE_OFFNET			0x02
#define		CPTB_SDSAVE_DEBUGINFO		0x04
#define		CPTB_SDSAVE_VEHINFO			0x08
#define		CPTB_SDSAVE_WAVE			0x10
//
#define		CPTB_EXEQUI_LASERSURVEY		0x01
#define		CPTB_EXEQUI_DTU				0x02
#define		CPTB_EXEQUI_SD				0x04
//
#define		CPTB_ACQBOD_NO				0x00
#define		CPTB_ACQBOD_HAV				0x01
#define		CPTB_ACQBOD_COIL			0x02
//
#define		CPTB_VEHPOST_FWD			0x01
#define		CPTB_VEHPOST_BWD			0x02

//��Ȧģʽ�£�խ����ż���������ݵĺ꣺
#define		ZTNumSLane(row)					(row*2)		// ������խ��������246
#define		ZTLane(ix, row)					(ix/(row*2))	// խ�����ڳ�����0123
#define		ZTRow(ix, row)					(ix%row)		// խ������������012
#define		ZTLR(ix, row)					((ix%(row*2))/row)	// խ������λ�ã�0��1��
#define		ZTYafengIxBegin(row,lane)			(row*2*lane)	// ��1��ѹ��խ��������

//����
// �����ʾ
#define		VEH_DIREC_FWD				0x01
#define		VEH_DIREC_BWD				0x02



	
#endif // end of #ifndef	__DEFCONFIGCONST_H
