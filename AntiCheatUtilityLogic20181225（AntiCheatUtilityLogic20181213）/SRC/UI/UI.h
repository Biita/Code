/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			UI.h
** Last modified Date:  20110505
** Last Version:		1.0
** Descriptions:		�������н��溯��
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110505
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
#ifndef	__UI_H
#define	__UI_H

#include "KBMacro.h"
#include "Config.h"
#include "LCDApp.h"
#include "TDC256.h"
#include "Common.h"
#include "PCF8563.h"
#include "StorageApp.h"
#include "WDT.h"
#include "w5300app.h"
#include "TaskProtocol.h"

#ifdef	__UI_C
#define		UI_EXT
#else
#define		UI_EXT	extern
#endif
UI_EXT		void	DisplayInitLcd(void);
UI_EXT		void 	DisplayChooseChedao(void);
UI_EXT		void 	DisplayChooseTongdao(void);
UI_EXT		void 	DisplayFunction(void);
UI_EXT		void 	DisplayChooseTongdao_Chedao(void);
UI_EXT		void 	DisplayChedao1_zengyi(void);
UI_EXT		void 	DisplayChedao2_zengyi(void);
UI_EXT		void 	DisplayChedao3_zengyi(void);
UI_EXT		void 	DisplayChedao4_zengyi(void);
UI_EXT		void 	DisplayTongdaoZY_Chedao1(void);
UI_EXT		void 	DisplayTongdaoZY_Chedao2(void);
UI_EXT		void 	DisplayTongdaoZY_Chedao3(void);
UI_EXT		void 	DisplayTongdaoZY_Chedao4(void);
UI_EXT		void 	DisplayChange_Chedao1ZY(void);    //rp
UI_EXT		void 	DisplayChange_Chedao2ZY(void);    //rp
UI_EXT		void 	DisplayChange_Chedao3ZY(void);    //rp
UI_EXT		void 	DisplayChange_Chedao4ZY(void);    //rp
UI_EXT		void 	DisplayChange_TongDao1ZY(void);	  //
UI_EXT		void 	DisplayChange_TongDao2ZY(void);	  //
UI_EXT		void 	DisplayChange_TongDao3ZY(void);	  //
UI_EXT		void 	DisplayChange_TongDao4ZY(void);	  //
UI_EXT		void 	DisplayChange_TongDao5ZY(void);	  //
UI_EXT		void 	DisplayChange_TongDao6ZY(void);	  //
UI_EXT		void    DisplayChedao1_Neima(void);
UI_EXT		void    DisplayChedao2_Neima(void);
UI_EXT		void    DisplayChedao3_Neima(void);
UI_EXT		void    DisplayChedao4_Neima(void);

UI_EXT		void    DisplayProgramVersion(void);
UI_EXT      void    Save_TongdaoZY(uint8 ChedaoNum, uint8 TongdaoNum);

UI_EXT      void    Seach_TongdaoZY(uint8 ChedaoNum, uint8 TongdaoNum);		 //��ѯͨ������
UI_EXT      void    Seach_TongdaoNM(uint8 ChedaoNum, uint8 TongdaoNum);     //��ѯͨ������

UI_EXT      void    Display_Chezhong(Vehicle_Struct *pItem);       //����������ʾ

//UI_EXT	void	UIStartUp(void);			//��������	
//UI_EXT	void	UIGeneral(void);			//ͨ�ù�������	
//
//UI_EXT	void	UIBDRoot(void);				//�궨������ 
 

//UI_EXT	void	UIBDMain(uint8 p_u8Motion);	//����̬�궨���� 
//UI_EXT	void	UIBDWanBanChoose(uint8 p_u8Motion);	//���궨ͨ��ѡ�����
//UI_EXT	void	UIBDChengTai(uint8 p_u8Motion);					//��̨�궨����
//UI_EXT	void	UIBDScale(uint8 p_u8Motion);					//���÷ֶ�
//UI_EXT	void	UIBDStaticModify(void);		//���þ�̬�������棬���մ�������������
//UI_EXT	void	UIBDChooseMotion(void);		//��̬����ѡ����棬����or����
//UI_EXT	void	UIBDFullRange(void);		//����������� 
//UI_EXT	void	UIBDHeightTanxingti(void);			//������߶Ƚ���
//UI_EXT	void	UIBDPoDu(void);				//�¶���������
//UI_EXT	void	UIBDChooseVehPos(void);		//�����ٶ�����λ��ѡ��
UI_EXT	void	SaveParams(uint8 num);			//�����������
//UI_EXT	void	UIBDRoadPlaneHeight(void);			//·��ֵ�߶Ƚ���
//UI_EXT	void	UIBDWanBan(uint8 p_u8CID, uint8 p_u8Motion);	//���궨����
//UI_EXT	void	UISetValueParamU16(char * p_pcName, uint16 * p_pu16Param, uint32 p_u32Min, uint32 p_u32Max);
//UI_EXT	void	UIBDLineModify(void);		//������������
//UI_EXT	void	UIBDPointModify(void);		//����������
//UI_EXT	void	UIBDChooseVeh(uint8 p_u8Pos);	//ѡ���ͽ��棬����ѹ��λ�ò�ͬ���鿴�����ݲ�ͬ
//UI_EXT	void	UIBDVSModifyParam(uint8 p_u8Pos, uint8 p_u8Veh);	//����ֵ���棬����ѹ��λ�úͳ���

//UI_EXT	void	UIValidate(void);			//ˢ�������֤���� 
//UI_EXT	void	UISystemInit(void);			//ϵͳ��ʼ������

UI_EXT	void	InitSystem(uint8 num);			//��ʼ������ϵͳ����
UI_EXT	void	InitNonWeight(void);		//��ʼ���ǳ�������
//UI_EXT	void 	FDistBetwInit(void);
//UI_EXT	void	InitMotionModify(void);		//��ʼ����̬��������
//UI_EXT	void	InitVehModify(void);		//��ʼ��������������
//UI_EXT	void	InitStaticModify(void);		//��ʼ����̬��������
UI_EXT	void	InitThreshold(void);		//��ʼ����ֵ����
//UI_EXT	void	InitVehBufIndex(void);		//��ʼ�����绺�泵����Ϣ 
UI_EXT	void	YBRestart(void);			//����
//UI_EXT	uint8	RecoverToLast(void);		//�ָ�Ϊ����궨����
//UI_EXT	uint8	RecoverToHistory(void);		//�ָ�Ϊ��ʷ�궨����
//UI_EXT	void	InitRestart(void);			//��ʼ�����Զ�����

UI_EXT	void 	InitNetParam(uint8 num);			//��ʼ��������Ϣ
UI_EXT	void	SaveNetInfo(uint8 num);			//����������Ϣ

UI_EXT	void	ClearStartupCnt(void);		//������������
UI_EXT	void	AddStartupCnt(void);		//����������1


//UI_EXT	void	UISN(void);					//SN�����
//UI_EXT	void 	CLEARZERO(void);
//UI_EXT	void	UIF3Code(void);				//�鿴F3�������
//UI_EXT	void	UIF4Code(void);				//�鿴F4�������
//UI_EXT	void	UIF5Code(void);				//�鿴F5�������

//#if	 OS_TASK_CREATE_EXT_EN >0
//UI_EXT	void 	DisplayStatics(void);
//#endif

//UI_EXT	void	UICommonSet(void);			//��ͨ���ý��棬111
//UI_EXT	void	UIViewSetting(void);		//�鿴���ò������棬222
//UI_EXT	void	UIViewModify(void);			//�鿴�ٶ��������棬333
//UI_EXT	void	UIViewAuthor(void);			//�鿴������Ϣ���棬888
//UI_EXT	void	UIViewIPInfo(void);			//�鿴IP��Ϣ��444
//UI_EXT	void	UIViewStartUpTime(void);	//�鿴����ʱ����Ϣ��8494
//UI_EXT	void	UIViewThreshold(void);		//��ֵ������8968
//
//UI_EXT	void	UISetTime(void);			//����ʱ�����
//UI_EXT	void	UISetBaudRate(void);		//���ò����ʽ���
//UI_EXT	void	UISetProtocol(void);		//Э�����
//UI_EXT	void	UISetLoop(void);			//��Ȧʹ�ܽ���
//UI_EXT	void	UISetDog(void);				//���ÿ��Ź�����
//UI_EXT	void	UISetDiaodian(void);		//���õ��籣������

//UI_EXT	void	UISetValueParamIP(char * p_pcName, uint8 * p_pu8IP);	//����IP
//UI_EXT	void 	UISetValueParamU32(char * p_pcName, uint32 * p_pu32Param, uint32 p_u32Min, uint32 p_u32Max);
//UI_EXT	void 	UISetEnableParam(char * p_pcName, uint8 * p_pu8Param);
UI_EXT	void	SaveTime(SystemTime p_stSet);				//����ʱ��

//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����
//UI_EXT	void	UIBDSendWave(void);			//���Ͳ���ʹ�ܽ���
//#endif
UI_EXT void ProModInit(uint8 num);//wgh 20140303
UI_EXT void InitJDParam(void);
#endif
