/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			AdjustMacro.h
** Last modified Date:  20110531
** Last Version:		1.0
** Descriptions:		��УЭ������궨��
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110531
** Version:				1.0
** Descriptions:		��УЭ������궨��
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef	__ADJUSTMACRO_H
#define	__ADJUSTMACRO_H

//����֡
//	0xFF 00 [LEN] [COMM] [PARAM1] [PARAM2] [PARAM3] [VALUE1] [VALUE2] [VALUE3] [VALUE4] [CRC1] [CRC2]

//����궨��	COMM
#define		ADJ_GETSDINFO			0xA0	 	// ��ѯSD������	//wgh
#define		ADJ_SETSDINFO			0xA1	 	// ����SD������	//wgh
#define		ADJ_GETIPINFO			0xA2		//��ȡ�Ǳ�IP��ز��� //wgh
#define		ADJ_UPDATEIPINFO		0xA3		//�����Ǳ�IP��ز��� //wgh
#define		ADJ_GETNONEWEIGHT		0xA4		//��ȡ���з�������������//wgh
#define		ADJ_UPDATENONEWEIGHT	0xA5		//�������з������������� //wgh
#define		ADJ_SAVE				0xA6	 	//�����Ǳ����ָ�� //wgh
#define		ADJ_GETVRSION			0xA7		// ��ȡ�汾��  //wgh
#define		ADJ_GETMOTIONWEIGHT		0xA8		//��ȡ���ж�̬��������	//wgh
#define		ADJ_UPDATEMOTIONWEIGHT	0xA9		//�������ж�̬�������� //wgh

#define 	ADJ_SENDRUNTIME 		0xAB		// �������ʱͳ��
#define		ADJ_IPSEARCH		0xAD		//���ػ�IP���� //wgh
#define     ADJ_SUPERCOMD       0xAC        //����ָ��
#define		Para_Read			0xD0		//�����л�ȡ�豸����
#define		Para_Save			0xD1		//�����������豸����
#define		JD_Reset 		    0xD2		//�����г�ʼ��01���ݰ�
#define		JD_Init				0xD3		//����������Ĭ�ϲ���

#define     ADJ_CTRSAMPPOW      0xE1 //�ɼ��幩��ָ��
#define     ADJ_ZTINTENSET      0xE2 //խ��ʹ������
#define     ADJ_ZTINTENASK      0xE3 //խ��ʹ�ܲ�ѯ
#define     ADJ_DEBUGINFO       0xE0 //������Ϣ����

//
#define		ADJ_SENDAXLEGRP		0x80		//������������	//wgh
#define		ADJ_SENDAXLE		0x81		//����������	//wgh
#define		ADJ_TIMESYNCHRONY	0x82		//ʱ��ͬ�� //wgh
#define		ADJ_CAPTURE			0x83		//ץ�� //wgh
#define		ADJ_RESTART	    	0x84		//���� //wgh
#define		ADJ_DEVICESTATUS	0x85		//�豸״̬//wgh
#define		ADJ_HEARTBEAT		0x86		//����	//wgh
#define		ADJ_SETHEARTEN		0x87		// ��������ʹ��//wgh
#define     ADJ_CONHEART        0x88        //���ػ�����ָ�� //wgh
#define     ADJ_POWERRUN        0x89        //�Ǳ��ϵ籨�� //wgh
#define		ADJ_RESEND			0x8A		//�ط� //wgh
#define     ADJ_IOUSEASK        0x8D		//IOʹ�ò�ѯ
#define     ADJ_IOUSESET        0x8C		//IOʹ������
#define     ADJ_TESTCOM         0x8E        //����ָ�� //wgh

#define		ADJ_EXT				0x8F		//��չָ��
#define     ADJ_USESTAT         0x90        //�豸ʹ�ò�ѯ
#define     ADJ_ZTARRAYASK      0x91		//խ�����վ����ѯ
#define     ADJ_ZTARRAYSET      0x92		//խ�����վ�������
#define     ADJ_CHLASK          0x93		//�������ò�ѯ
#define     ADJ_CHLSET          0x94		//������������

#define		ADJ_NETCONNECT		0x95		//���ػ������·����


//�ͼ���ģ�齻��ʱ������
//�����ػ�->ARM->����ģ�飩
#define     LASER_SET_PARA_RQ   0xB0        //���ü������
#define     LASER_READ_PARA_RQ  0xB1        //��ȡ�������
#define     LASER_RESET_RQ      0xB2        //���⸴λ
//������ģ��->ARM->���ػ���
#define     LASER_SET_PARA_RS   0xC0        //���ü������Ӧ��
#define     LASER_READ_PARA_RS  0xC1        //��ȡ�������Ӧ��
#define     LASER_RESET_RS      0xC2        //���⸴λӦ��
#define     LASER_VEHICLE_POS   0xCA        //����λ����Ϣ
#define     LASER_STATE_CC      0xCC        //����״̬��Ϣ������֡��



//////////////////////////////////////////////////////////////		 

#endif	//__ADJUSTMACRO_H
