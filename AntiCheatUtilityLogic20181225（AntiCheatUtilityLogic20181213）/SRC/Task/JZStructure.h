/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JZStructure.h
** Last modified Date:  2011511
** Last Version:		1.0
** Descriptions:		���س���ṹ
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		2011511
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Ye
** Modified date:		20110722
** Version:				1.1
** Descriptions:		2.2��3.0ͨ��
**---------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Tonghan
** Modified date:		20170904
** Version:				1.2
** Descriptions:		���ֽṹ��
**
*********************************************************************************************************/
#ifndef	__JZSTRUCTURE_H
#define	__JZSTRUCTURE_H

#include "config.h"



/*
*********************************************************************************************************
*                                         �㷨�ṹ��
*********************************************************************************************************
*/	
typedef struct tagTrigerID
{
	uint16	TrigPreID;
	uint16  TrigNextID;
}TrigerID_Struct;

typedef struct tagTrigID4AxleDis
{
	uint16	TrigPreID[3];
	uint16  TrigCurrID[3];
}TrigID4AxleDis;

typedef struct tagSortAxleDist
{
	uint32 l_u32AxleTime;
	uint16	l_u16AxleDist;
}SortAxleDist;

/*
*********************************************************************************************************
*                                         ʱ��ṹ��
*********************************************************************************************************
*/	
typedef	struct tagSystemTime
{
	uint16	u16Year;				//��
	uint8	u8Month;				//��
	uint8	u8Day;					//��
	uint8	u8Week;					//���ڼ�
	uint8	u8Hour;					//ʱ
	uint8	u8Minute;				//��
	uint8	u8Second;				//��
} SystemTime;

/*
*********************************************************************************************************
*                                         ����ṹ��
*********************************************************************************************************
*/	
typedef	struct tagNetInfo
{
	uint8	au8SubMask[4];					//��������
	uint8	au8IPAddr[4];					//IP
	uint8	au8GatewayIP[4];				//Ĭ������
	uint8	au8ServerIP[4];					//������IP��ַ 
	uint32	u32LocalPortNO;					//�˿ں�
	uint32	u32ServerPortNO;				//�������˿ں�
	uint8	au8MACAddr[6];					//MAC
	//
	uint8	m_u8ProgVersion[4];				// ����汾��ʾ
	//
	uint8	au8CRC[2];						//CRC  	
} NetInfo;

typedef	struct tagsndFrame				  // wujinlong
{
	uint8	u8Cnt; 	//���͵İ���
	uint32	au32len[50]; //����֡����
	uint8   au8data[50][SNDFRAMESIZE];	 //���͵���������
} sndFrame_Struct;

typedef struct tagTargetIPInfo
{
	uint8  u8State; //�˿ڵ�״̬
	uint8  u8CnntIP[4]; //�Զ�IP��ַ
	uint8  u8CnntPort[2]; //�Զ˵Ķ˿ں�
}TargetIPInfo_Struct;

//typedef struct
//{
//	uint16 len;
//	uint8 data[MAXBUFPOINT];
//}WaveData_Struct; 

/*
*********************************************************************************************************
*                                         F4����ṹ��
*********************************************************************************************************
*/	
typedef	struct tagDbugCode
{
	uint8	u8NextPos; //���Դ���ĳ���
	uint8   DbugCode[256];	 //���Դ��������
} DbugCode_Struct;


// ��ȦIO�ṹ��	  add by  sxh 2014.3.27
/**********************add by sxh  2014.3.28 ***********************************/
#define COILGPIODATALEN         128  
/*����λ�ýṹ��*/
typedef struct vehPosition
{
	uint8  m_u8type;
	uint16 m_u16Ltpos;	 
	uint16 m_u16Rtpos;
}VehPosition_Struct;
/*��Ȧ/IO״̬�ṹ��*/
typedef struct CoilOrLaser   
{
	uint8 m_u8Flag;                        // ����/��Ȧ     0x02;����λ�����ݣ� 0x03������IO���ݣ� 0x04����ȦIO����
	VehPosition_Struct vehPositionInfo[LASTERCOUNT<<2];	  //����λ����Ϣ(�������������й�ϵ)
	uint16 m_u16GpioState;                // ��һ���ֽ��е�8λ�ɵ͵�������������ʾǰ���Ϊ1-8����Ȧ״̬���ڶ����ֽ��е����λ������ʾ���Ϊ9����
	uint32 m_u32Tm;	                      //����ʱ��

}CoilOrLaser_Struct;

 typedef struct CoilOrLaserBuf   //������ȦIO/����IO/����λ�ýṹ�建��
 {
 	uint8 m_u8WriteIn;
	uint8 m_u8ReadOut;
	CoilOrLaser_Struct CoilOrLaserData[COILGPIODATALEN];
 }CoilOrLaserBuf_Struct;

/*
*********************************************************************************************************
*                                         ��Դͳ�ƽṹ��
*********************************************************************************************************
*/
#if	OS_TASK_CREATE_EXT_EN > 0

typedef struct tagTaskStatics
{
  uint8  TaskID;	//�������ȼ�
  uint32	 CPUsage; 	//CPUʹ����
  uint32  stkMaxUsage;  //ջʹ����
  uint32  stkCurUsage; //ջ��ǰʹ����
  uint32 tmCnt;	    //����ʹ�ú�ʱ,��λms/s  
}TaskStatics;

#endif
/*
*********************************************************************************************************
*                                         �������ýṹ��
*********************************************************************************************************
*/
//ARM����
typedef struct tagSetupParam
{	
	uint8	u8Version;							//�����汾 PARAM_VERSION 3.0
	uint8  	u8Year;							 	//�����޸�����--��
	uint8  	u8Month;							//�����޸�����--��
	uint8  	u8Day;								//�����޸�����--��
//4
	uint8 	u8Protocol;
	uint8 	u8StaticScale;		   				//��̬�ֶ�ֵ��û�ã�����
	uint8 	u8MotionScale;						//��̬�ֶ�ֵ 
	uint8	u8DiaoDianTag;						//���籣��
//8
	uint8 	u8DOG;			 					//���Ź����� 
	uint8 	u8SendWaveEnable;					//���Ͳ���ʹ�� 
	uint8	u8LoopTriggerEnable; 				//��Ȧ����ʹ�ܣ�û�ã�����		 
	uint8 	u8ComMode;			  				//����ģʽ
//12
	uint8 	u8SendF4CodeEnable;					//����F4����ʹ��
	uint8 	u8BaudRate;			  				//����0������ 	 
   	uint8 	u8VehicleBufSize;					//���������С��û�ã�����
	uint8	u8noUsed1;
//16
	uint32 	u32Full;			  				//������
	uint32	u32Password;						//����	  
//24
	uint8	u8UPPoints;						    //�ϳӵ��� wzf 2012-08-24
	uint8 	u8DownPoints;					    //�³ӵ��� wzf 2012-08-24
	uint8	u8noUsed2[2];
//28
	uint16	u16logicUpValue;	 				//�߼��ϳ���ֵ	wzf 2012-08-24
	uint16 	u16logicDownValue;					//�߼��³ӳ���ֵ  wzf 2012-08-24
	uint16 	u16UpValue;	 						//�ϳӷ�ֵ
	uint16 	u16DownValue;	 					//�³ӷ�ֵ	
//36
	uint16  an32ImmediateValue[ZERONUM];//˲ʱֵ
	uint16 	an32Zero[ZERONUM];				//���		   	  
	uint16 	an32AxGain[CHANNELNUM];	 			//����
//292 
   	uint16  u16PlatformGain;				    //��̨���棬wzf 2012-08-13����������û�У�����
	uint16  u16noUsed3;
//296	
	uint8  	u8Diszt[CHANNELNUM];				//խ�����
	int8	an8HeightTanxingti[CHANNELNUM];	    //������߶ȣ�0.1mm����Χ1-16����0.1-1.6��Ĭ��Ϊ10
	uint8   an8ZTStatus[CHANNELNUM];	    //խ��״̬:1Ϊխ��������0:�쳣
	int8 	an8LengthPodu[CHANNELNUM];			//���³��ȣ�խ��ǰ�滺�³��ȣ�
//552
	uint16 	u16DistBetween[CHANNALLINESUM];				//խ�����,300,600,200mm --- ����2012-11-15	 �����ղ��ã��ȱ���
	uint16	u16UseToWghFlag[CHANNALLINESUM];			//ÿһλ��־ÿһ��խ������;����λ����λխ��1-64 1������� 0���������
//568
	uint16 	au16Speedmodify[10][SPEED_CTL_SIZE];  	// �ٶ���������
//968
	uint16  au16VehTotalModify[10][10];				//������������
//1168
	uint16 	au16VehSpeedModify[10][10][SPEED_CTL_SIZE];		 	//�����ٶ�����
//5168	
	int8	m_an8HeartBeatEn[NET_PORT_NUM];		// ����ʹ�ܣ�����ͨ����λ�����ã�1��������Ĭ��1��added by wujinlong 20130905
//5172
	
	// ����������ע�����Ԥ���ֽ���
//	uint8	m_u8LCAddr;							// ��λ����ַ���ӻ���ַ
	uint8	u8ProgVersion[4];					// ����汾��ʾ	
//5176
	uint16 	an16BDAxGain[CHANNELNUM];	 			//���� 
	uint8   u8ArrayForZT[64];					//խ�����վ���
	uint16 	an32StaGain[CHANNELNUM];				//խ����̬����  rp 20150820
//5496  
	uint8 	au8ReserveByte[RESERVED];			//Ԥ���ֽ�	
	uint16	u16CRC;								//CRCУ����
//6144	
} SetupParam;

//����ؼ��궨����
typedef struct tagLaserKeyParam
{
    uint32  m_u32Height;                        //�������߶�
    uint16  m_u32Center;                        //���������ĶԳƵ�
    uint16  m_u32ScanStart;                     //������ɨ����ʼ��
	uint16  m_u32ScanEnd;                       //������ɨ����ֹ��
	uint16  m_u32Port;                          //�������˿ں�
	uint32  m_u32IP;                            //������IP��ַ
	uint32  m_u32OffSet;
	uint32  m_u32MinWide;
	uint32  m_u32EndFrame;
	uint32  m_u32LaneWide;
}LaserKeyParam;

//�������(ע��ýṹ�������Ҫ�޸ģ�һ��Ҫ��֤�ֽڶ��룬��Ϊ�ýṹ���ʹ��memcpy��ֵ)
typedef struct tagLaserSetupParam
{
	uint8   m_u8ComBandRate;				     //���ڲ�����1-4800��3-38400��4-57600��5-115200������-9600��Ĭ��ֵΪ5,��115200
	uint8   m_u8WDTEnable;						 //���Ź�ʹ�ܣ�1-ʹ�ܣ�0-��ֹ
	uint16  m_u16SeverPort;						 //�����շ���ʱ�ķ������˿ں�
	uint32  m_u32SeverIP;						 //�����շ���ʱ�ķ�����IP��ַ
	uint32  m_u32LaserResetCnt;                  //����ģ����������
	uint32  m_u32LaserIP;						 //����IP��ַ
	uint16  m_u16LaserPort;						 //����˿ں�
	uint8   m_a_u8LaserMAC[6];				     //����MAC��ַ
	uint32  m_u32LaserMASK;						 //������������
	uint32  m_u32LaserGateWay;					 //��������
	uint16	m_u16Year;				             //��
	uint8	m_u8Month;				             //��
	uint8	m_u8Day;					         //��
	uint8	m_u8Hour;					         //ʱ
	uint8	m_u8Minute;				             //��
	uint8	m_u8Second;				             //��
	uint8   m_u8LaserNum;						 //����������
	LaserKeyParam m_stLaserKeyPara[LASTERCOUNT]; //�������ؼ�����

	uint8   m_u8Rev[6];							 //�����ֽ�
	uint16  m_CrcChk;							 //У��λ

}LaserSetupParam; 

// wgh 20140303
typedef struct tagProModParam
{
	uint8	m_u8ZTRow;				// խ��������Ĭ��3
	uint8	m_u8LaneNum;			// ��������Ĭ��3
	uint8	m_u8ZTWide;				// ZT��ȣ�Ĭ��175
	uint8	m_u8YaFengZTWide;		// ѹ��ZT��ȣ�Ĭ��175
	
	uint16	m_au16ZTGap[64];
	uint16	m_au16ZTDist[10][3];		// խ����࣬��λmm�����4�ţ����10������ѹ��խ������Ҫ��࣬1���������м����ͬ
	uint32	m_au32Thres[40];		// ���п����õ���ֵ������ɲ����㣬��1s=6000�㣬16λ�����65535����10.9s����ֹ��չ��ʹ�ó���10s������Ԥ���ȹ���20��
//224									// ����ֵ1��ʾͬ��ZTʱ����ֵ60��ȣ��߼�ʱ����ֵ10��
									// ZT�ɶ������ȶ�2������ȷ����ֵ˳��
	uint8	m_au8Enable[40];		// ����ʹ������ [0]�����ڵ�����Ϣ���
									//  			[1]��
	//
		
	uint8	m_au8LaneBetwWide[20];	// �������ȣ�0-255cm��2.55m��0����1-2��������
//284	
	//
	uint8 	m_u8DebugBaudRate;		// ������Ϣ�����ʣ�1-5:4800��9600��38400��57600��115200��Ĭ��115200
	//
	uint8	m_u8TriggerMode;		// ����ģʽ��0x01������/��Ȧ��ǰ�ã���������Ϊ����������Ȧ����������0x02������Ȧ�����ã���0x04��˫��Ȧ��0x08�����⣻Ĭ��0x04
	uint8	m_u8IsYaFeng;			// �Ƿ���ѹ��խ����1���ǣ�0����Ĭ��0
	uint8	m_u8UploadDataMode;		// �����ϴ���ʽ��0x01�����ڣ�0x02��232���ڣ�0x03�����ߴ��䣬4: 422����Ĭ������0x01
	uint8	m_u8DataWaveEn;			// �����ϴ��а����������1��������0����������Ĭ��0
	uint8	m_u8VehWaveSample;		// �������β�����ʶ��0������㣬1����1��ȡ1��(2��ȡ1��)��2����2��ȡ1��(3��ȡ1��)��3����3��ȡ1��(4��ȡ1��)
	uint8	m_u8CaptureMode;		// ץ��ģʽ��0x01��Ӳץ�ģ�0x02����ץ�ģ���λ������ץ�ģ���0x04����ץ�ģ���λ������ץ�ģ���Ĭ��Ӳץ��0x01
	uint8	m_u8TransmitSysEn;		// ת��ϵͳ�������1��������0����������Ĭ��0
	uint16	m_u16SDSaveEn;			// SD��ģʽ��0x01��SD���洢����ʹ�ܣ��ɵ�1-7λ��ʾ15�ִ洢����ʹ�ܣ����������0x02����־������Ϣ0x04��������Ϣ0x08��������Ϣ0x10��
	uint16	m_u16ExEquiType;		// �ⲿ�豸����16�֣�0x01���������⣬0x02�����ߴ���ģ�飬0x04��SD��
	uint8	m_au8AcqBoardFlag[8];	// �ɼ������ޱ�־����0-7�ֽڷֱ��ʾ��1-8���ɼ��壬�޲ɼ���Ϊ0x00���ɼ���Ϊ0x01����Ȧ��0x02��Ŀǰֻ�е�7��8��·��Ч����Ĭ��ȫΪ1��8���ɼ���ͨ��
	uint8	m_u8CaptureFlag[16][2];	// ��Ȧץ�ı�־��ֻ��˫��Ȧģʽ��Ч����1ά��������2ά��1λǰ����Ȧ����2λ������Ȧ��0x00������ץ�ģ�0x01ץ�ģ�����˳�����ų����г�����������������Ϊ1-16������Ĭ��ȫ0x00
	uint8	m_u8BwdDirecFlag;	// ��������ܱ�־��0x00�����򲻳�����0x01�����������Ĭ��0x00
	uint8	m_au8VehDirecFlag[19];	// �׳������־��0-N��ʾ0-N������0x01�����������0x02�����������0x03=0x01|0x02���������������������˳�����ų����г�����������������Ϊ1-N������Ĭ��ȫ0x01����ֵ��m_u8BwdDirecFlag��أ�m_u8BwdDirecFlag���򲻳�����m_au8VehDirecFlag�������ò�������
//356	
	uint8	m_au8HalfLane[8];		// �복�����洢�복���ĳ����������1��5����Ϊ�복������[0]�洢1��[1]�洢5
	
	// SD��
	uint8	m_u8SDCapa;				// SD��������1-1G��4-4G��8-8G��16-16G,Ĭ��8G
	uint8   m_u8JGKZ;				//������չ��Χ
	uint8	m_au8Nouse1[2];
	uint8	m_au8SDFuncSpaceper[16];// ���ܿռ�ٷֱȣ�[0]Ϊ�����������15�ֹ��ܣ���m_u16SDSaveEn���Ӧ��Ĭ��1,10,20,15,15,39
//384	
	// ��չ�ӿ�
	
	//
	
	uint8   m_u8StudyAlgEn[5];					  //ѧϰʹ��
	uint8  	m_u8StudyLimit[5][2];				  //ѧϰ������
	uint8	m_u8Nouse2;
	uint16  m_u16StudyWeight[5][2];				  //ѧϰ����
	
	//
	uint8	m_u8ProgVersion[4];					// ����汾��ʾ
//424											 
	uint8   m_u8EnableFlag[8];					// ʹ��λ ����8��ʹ��
	uint16  m_u16LRLocation[2][20];				// ÿ��խ��������λ�� [0][]��λ�� [1][]��λ��
	int8    m_n8LouDL[64];						// ©��������
	int8    m_n8CFDian[64];						// ��ŵ����
	uint8   m_au8ZTIntEn[64];					// խ���ж��ϴ�ʹ��
	uint8   m_au8IOUseEn[26];					//IOʹ��ʹ��
	uint8   m_au8NoUse3[2];
	uint8   m_au8CHLSET[20];                    //����ʱ��������
	uint8 	m_au8ReserveByte[PROMOD_RESERVED-128];			//Ԥ���ֽ�	
	uint16	m_u16CRC;
} CProModParam;
//end 

/*
*********************************************************************************************************
*                                         �������ݽṹ��
*********************************************************************************************************
*/
# define MAXNUM_AXLE 12 //ÿ������������խ����
//#pragma	pack()

//ÿ��խ�������ݽṹ
typedef struct tagChannal{
	uint16	u16SelfID;		// ��¼��խ��λ�ã����������е���ţ�һֱ���䣬��խ��ʱ�ṩλ����Ϣ
	uint16	u16YDMID; //Ĭ����խ��ID��ͬ
	uint8   u8BoardType;    // �忨���� 1:�к����ɼ���, 2:���ִ������ɼ���
	uint8	u8WheelCnt;  //խ���ϳ��ֵĸ���
	uint8	u8ZTSeam;	//0:һ���ֲ�ѹ��;1:���խ��ѹ��;2:�Ҳ�խ��ѹ��;3:���඼ѹ��;4:���඼��ѹ��;5:��ѹ�Ҳ�ѹ;6:��ѹ��ѹ
	uint8	u8ZTMatchedFlag; //խ����ƥ���ʶ:0:û�б�ƥ��;1:��౻ƥ��;2:�Ҳ౻ƥ��;3:����ƥ��(����ƥ�����3)
	//
	uint32	m_ulTime;					// �ϳ�ʱ����³�ʱ��
	int8	m_bUpOrDn;					// 1���ϳӣ�0 �³�
	int8	m_bDirectionFlag;
	uint8  	u8Id;		//խ�����
	uint8  	u8IsDownFlag; // �����³ӱ�־������³�����1������Ϊ0��writeIndexʱ��ʼ����0  20121108
			
	uint32 	u32chlSum;	//խ�����ֺ�
	uint32 	u32uptm;		//�ϳ�ʱ��
	uint32 	u32dntm;		//�³�ʱ��
	uint32  u32Maxtm;//��ʵ��ֵʱ��
	uint16 	u16ptMaxValue;	//��ֵ
	uint8	TaiType[2]; // 1:��̥;2:˫̥; ����������֣���һ��Ԫ�ر�ʾ����֣��ڶ�����ʾ�Ҳ�;
	uint32 	u32FirstDnTime;		//һ���³�ʱ�� 	
	uint32	u32realCnt;  //��ʵ����
	uint32  u32Mintm;       //һ���ϳ�ʱ��(������ƥ����ʹ��,����λ��δ��ֵ)

	//��һ��խ��������
	uint32  u32FirstZTUpTmFir; //һ���ϳӵ�
	uint32  u32FirstZTUpTmSec; //�����ϳӵ�
	uint32	u32FirstZTDnTmFir; //һ���³ӵ�
	uint32	u32FirstZTDnTmSec; //�����³ӵ�
	uint16 	u16FirstZTMaxValue;	//��ֵ
	uint8	u8FirstZTMaxPos; //��һ��խ�����ֵλ��
	uint8	u8FirstZTUpDnPosCnt;//��һ��խ�����³�λ��
	uint8   u8FirstZTMaxAndUpDnPos;//��һ��խ�����ֵλ�ú����³�λ�ã��ֱ�Ϊ����λ�͵���λ
	uint32	u32FirstZTSum; //��һ��խ�����ֺ�
	
	//�ڶ���խ��������
	uint32  u32SecondZTUpTmFir; //һ���ϳӵ�
	uint32  u32SecondZTUpTmSec; //�����ϳӵ�
	uint32	u32SecondZTDnTmFir; //һ���³ӵ�
	uint32	u32SecondZTDnTmSec; //�����³ӵ�
	uint16 	u16SecondZTMaxValue;	//��ֵ
	uint8	u8SecondZTMaxPos; //�ڶ���խ�����ֵλ��
	uint8	u8SecondZTUpDnPosCnt;//�ڶ���խ�����³�λ��
	uint8   u8SecondZTMaxAndUpDnPos;//�ڶ���խ�����ֵλ�ú����³�λ�ã��ֱ�Ϊ����λ�͵���λ
	uint32	u32SecondZTSum; //�ڶ���խ�����ֺ�
	
	uint32  u32MainFPGATime;      // ���ذ�FPGAʱ��,�����ж����ذ�FPGA�Ͳɼ���FPGAʱ���
	uint32  u32TimeDiffValue;     // ���ִ�������FPGA�����ذ�FPGAϵͳʱ���(ȡ����ֵ)
	int32   n32DiffValue;         // ���ִ�������FPGA�����ذ�FPGAϵͳʱ���(��ȡ����ֵ)
	
	
	uint16 	u16ptCnt; 		//խ���洢���ݵ���
	uint8 	u8packetCnt; 		//��ȡխ���жϴ���
	uint8 	u8DataErrorFlag; 		//0��ʾ��������Ϊ0��ʾ���ݰ�Ϊ0�Ĵ���
	uint8   u8FrameNo;      //֡���
	uint8   u8BoardNo;      //���
	uint8   u8ChannelNo;    //ͨ����
// 	uint16 	u16data[MAXBUFPOINT];	//խ���ĵ�ֵ����//�к�����Aխ��
// 	uint16	u16Bdata[MAXBUFPOINT];	//Bխ���ĵ�ֵ����
	
	uint16  u16data[2][MAXBUFPOINT];    // խ���ĵ�ֵ����, [0][]:�к�����Aխ��, [1][]:Bխ���ĵ�ֵ����
}Channal_Struct; 

typedef struct wheel
{
	uint16 	ZTList[2];//խ��
	uint32 	ZTUpTime;
	uint32 	ZTDnTime;
	uint8	ZTakePos[2];//����խ����λ�� 1:���;2:�Ҳ�;3:����
	uint8  	WheelXPos;//�ֵ�Xλ��
	uint8	TrigCnt;//������
	uint8	SgOrDbleWhel;//��̥����˫̥ 1:��̥; 2:˫̥
	uint8 	TriZTCnt; //������խ����
	uint16 	MaxValue;
	uint8  	WheelYesorNot;//�õ���û����̥
}Wheel_Struct;

typedef struct whel
{
	uint16  ZTList[6];   //խ��
	uint16  WheelXPos[6];//�ֵ�Xλ��    ��λcm
	uint16  WheelYPos[6];//�ֵ�Yλ��    ��λcm
	uint8	ZTakePos[6]; //����խ����λ�� 1:���;2:�Ҳ�;3:����
	int8    Dir;         //1:����,-1:����
	uint8	SgOrDbleWhel;//��̥����˫̥ 1:��̥; 2:˫̥
	
	uint8   Point;       //����(���ںϲ���ʱ�жϵ�˫̥)
	uint8   TriZTCnt;    //������խ����
	uint8   Status;      //��ʻ״̬(���궨��)  Ĭ��Ϊ0 (1:����,2:ѹ��,3:��б,4:��б,5:��S)
	uint8   HaveWheelFlag;         //0:����,1:����
	
	uint32  SumAver;               //���ֺ;�ֵ
	uint32  MaxValueAver;          //���ֵ��ֵ
	
	uint32  DueTime;               //������ĩ����ʱʱ��
	uint32  Spd;                   //���ٶ�,��λcm/s
	uint8   Waiting4MatchFlag;     //��ƥ���ʶ,0:������ƥ��,1:���Խ���ƥ��(ż����ĩ�Ž���ʱ�ñ�־��1,������ĩ�Ž��ֲ���ʱʱ�䵽��ñ�־��1)
	uint8   Score;                 //�ַ���(Ϊ���ֵĹ켣�ķ���)
}Whel_Struct;

//�ֹ켣ƥ��ṹ��
typedef struct _tracewhel
{
	//����Ĭ�ϴ洢�����ֵ�������,˫����ֱ�洢�����ֺ�����������
	uint8  LeftWhlZTID[6][6];           //����խ��ID(�켣x�켣������խ��ID)
	uint16 LeftWhlZTPos[6][6];          //����խ��λ��(�켣x�켣������խ��Pos)
	uint8  LeftWhlZTake[6][6];          //���ִ���խ����λ�� 1:���;2:�Ҳ�;3:����
	
	uint16 LeftWhlZTXPos[6][6];     	//������խ���ϵ�Xλ��,��λcm
	uint16 LeftWhlZTYPos[6][6];    	    //������խ���ϵ�Yλ��,��λcm,ȡֵ:0~350cm(ȡ��ɢֵ:0,70,140,210,280,350)
	uint16 LeftWhlSpd[6][6];            //��켣��ǰ��խ���������ٶ�,cm/s
	int16  LeftWhlDiffXPos[6][6];       //����ÿ���켣Xλ�õĲ�ֵ
	uint8  LeftPoint[6][6];             //�ֵĵ���(�����жϵ�˫̥)  Ĭ��Ϊ0
	
	
	uint8  LeftWhlZTCntPerTrace[6];     //����ÿ���켣������խ����
	uint8  LeftPoScore[6];              //ÿ���켣λ�õ÷�
	uint8  LeftSpdScore[6];             //ÿ���켣�ٶȵ÷�
	uint8  LeftRadScore[6];             //ÿ���켣���ȵ÷�
	uint8  LeftMultiScore[6];           //ÿ���켣�ۺϵ÷�
	uint8  LeftWhlStatus[6];            //��ʻ״̬(���궨��)  Ĭ��Ϊ0 (1:����,2:ѹ��,3:��б,4:��б,5:��S)
	
	uint8  RightWhlZTID[6][6];          //����խ��ID(�켣x�켣������խ��ID)
	uint16 RightWhlZTPos[6][6];         //����խ��λ��(�켣x�켣������խ��Pos)
	uint8  RightWhlZTake[6][6];         //���ִ���խ����λ�� 1:���;2:�Ҳ�;3:����
	
	uint16 RightWhlZTXPos[6][6];        //������խ���ϵ�Xλ��,��λcm
	uint16 RightWhlZTYPos[6][6];        //������խ���ϵ�Yλ��,��λcm,ȡֵ:0~350cm(ȡ��ɢֵ:0,70,140,210,280,350)
	uint16 RightWhlSpd[6][6];           //�ҹ켣��ǰ��խ���������ٶ�
	int16  RightWhlDiffXPos[6][6];      //����ÿ���켣Xλ�õĲ�ֵ
	uint8  RightPoint[6][6];	        //�ֵĵ���(�����жϵ�˫̥)  Ĭ��Ϊ0
	uint8  RightTraceCnt;               //��ǰխ���Ѿ������Ĺ켣����
	uint8  LeftTraceCnt;                //��ǰխ���Ѿ������Ĺ켣����
	uint8  RightWhlZTCntPerTrace[6];    //����ÿ���켣������խ����
	
	uint8  RightPoScore[6];             //ÿ���켣λ�õ÷�
	uint8  RightSpdScore[6];            //ÿ���켣�ٶȵ÷�
	uint8  RightRadScore[6];            //ÿ���켣���ȵ÷�
	uint8  RightMultiScore[6];          //ÿ���켣�ۺϵ÷�
	uint8  RightWhlStatus[6];    	    //��ʻ״̬(���궨��)  Ĭ��Ϊ0 (1:����,2:ѹ��,3:��б,4:��б,5:��S)
	
	int8   Dir;   	             //1:����;-1:����
	int8  WheelCnt;             //��ǰխ�����ֵĸ���
	
	uint16 LastXPos;             //��ǰխ��X��λ��,��λcm
	uint16 LastYPos;             //��ǰխ��Y��λ��,��λcm,ȡֵ:0~350cm(ȡ��ɢֵ:0,70,140,210,280,350)
	
	
	uint32 LeftDueTime;              //������ĩ����ʱʱ��
	uint32 RightDueTime;             //������ĩ����ʱʱ��
	uint8  LeftFirstTracePos;        //��ѡ�켣,�����ź�ʼ��ѡ����ѡ��(����ʱ������δ���ֹ켣ʱʹ��)
	uint8  RightFirstTracePos;
	uint8  LeftStatus;               //Left��ʻ״̬(���ճ��ֵĹ켣����ʻ״̬)   1:����,2:��б,3:��б,4:��S,5ѹ��  Ĭ��Ϊ0
	uint8  RightStatus;
	uint8  HaveTraceFlag;        //���й켣��ʶ,0:û�й켣,1:�й켣,Ĭ��Ϊ0
}TraceWhel_Struct;		         //ÿ��խ���������ֹ켣�ṹ��
//խ���нṹ
typedef struct colum
{
	int16		Colum[4]; 	//���խ����λ��
	uint8		RunAndWalk;//����ͣͣ��ʶ��0Ϊ��
	int8 		Dir;      	//����,1��ʾ����-1��ʾ����
	uint8 		Speed;		// ���ٶ�
	uint8 		ZTCnt; //����խ��������,Ĭ��Ϊ0
	
	uint16 		LaserVehID;//��������ĳ���ID
	uint16 		MaxValue; 	//���ֵ
	
	uint16 		Length;		//�ӵس���
	uint8 		ZTJoinFlag;  //խ��ƴ�ӳ��еı�ʶ��1:���У�2:�޵ڶ��ţ���13�ų���; 3:�޵�һ�ţ���23�ų��У� 4:�޵����ţ���12�ų��У��ñ�ʶ���ڲ������в����Ƿ���©�ģ����ٵ��쳣���
	uint8   	DelaySpeed; //�����ٶ�
	
	uint32 		DueTime;    //��ʱʱ��
	uint8 		WaitingMtchFlag;//�ȴ�ƥ��ı�ʶ
	uint8		HideZTJoinFlag; //����խ�����б�ʶ��1���޵�һ�ţ���234���У�2���޵ڶ��ţ���134���У�3���޵����ţ���124���У�
											//4���޵����ţ���123���У�5����12���У�6����13���У�7����23���У�8:��1����;  9:��34����;     0x55:���У�
	uint8   	TaiTrace[4]; //������̥�켣����¼����ֵλ�ã�0,1,2,3
	uint8		LoRTai[4];  //��խ������խ�����������Ҳ��խ��,0ΪBoth;1Ϊ���խ��;2Ϊ�Ҳ�խ��
	uint8   	TaiType[4];//���е���̥����
	uint8   	SpeedMatchFlag;    //�ٶ�ƥ���ʶ,0:����ƥ��ģʽ(�ٶȺͽӵس���ƥ��),1:�ٶ�ƥ��
}Colum_Struct;

//��ʱδ��
typedef enum trace
{
	Normal=1,    //����ѹ����  1
	CrossRoad,   //���        2
	Single,      //����        3
	ThreColum,   //ѹ����      4
	SpanColum    //����        5
}TagTrace;
//խ���нṹ
typedef struct _colum
{
	int16		Colum[2]; 	//���խ����λ��
	uint16 		MaxValue; 	//���ֵ
	uint16 		Length;		//�ӵس���
	uint8		RunAndWalk;//����ͣͣ��ʶ��0Ϊ��
	int8 		Dir;      	//����,1��ʾ����-1��ʾ����
	uint16 		Speed;		// ���ٶ�
	uint8 		ZTCnt; //����խ��������,Ĭ��Ϊ0
	uint8		LowSpdDlyTimeFlg;
	
	uint8 		LeftMtchFlag;//���ƥ��խ�����Ƶı�ʶ
	uint8		RightMtchFlag;//�Ҳ�ƥ��խ�����Ƶı�ʶ	
	uint16		WonderLMtchID; //����ƥ���խ��ID
	uint16		WonderRMtchID; //����ƥ���խ��ID
	uint32 		DueTime;    //��ʱʱ��
	uint8		FristElemRow;  //Ĭ��0Ϊ����,1Ϊ�ڶ���
	uint8		AddLogicTimeFlg;	//1Ϊ����
	uint8   FirWhelPos;
	uint8   SecWhelPos;
	uint8   WhelCnt;
}HideColum_Struct;


 //�����ݽṹ
typedef struct tagAxle				
{
	uint8 	u8id;					//����
	uint8 	u8Owner;				//������ĳ���
	uint8	u8OrderID;		    	//��¼�����ĵڼ�ֻ��
	uint8   u8ChlNum;               //�Ὠ�����ĸ�������
	
 	uint8	AxleChlPos[3];          //����ѹխ�����У�����ȷ����켣�������ĳ�����
	int8   	Dir;		
	uint16	u16speed;				//���� 
	uint16  WheelSpd;               //���ٶȣ���λcm/s
	
	uint16  LaserVehID;             //�������ڼ���ĳ������
	uint8   u8ValidChlCnt;			//��ƥ��խ���еĸ�������ʼΪ0
	uint8   u8PointsGet;            //�÷�	
	int32	s32AxleWeight;		    //����
	uint16	u16AixsType;			//̥��
	uint16	u16ChListSum;			//���Ӧ��խ����������������
		
	uint16	u16ChList[6];			//���Ӧ��խ���洢λ��
	uint16  u16LeftWheeList[6];    //���ֶ�Ӧ��խ���洢λ��
	uint8	u8LeftWheelTakeZT[6];  //�����ȡխ���ĸ����֣�1Ϊ��ಿ�֣�2Ϊ�Ҳಿ�֣�3Ϊխ������
	uint8	u8RightWheelTakeZT[6];
	uint16  u16RightWheeList[6];   //���ֶ�Ӧ��խ���洢λ��
	uint16  u16LeftWheelMaxValueAver;        //����խ�����ֵ��ֵ
	uint16  u16RightWheelMaxValueAver;       //����խ�����ֵ��ֵ

	uint32  HideZTPostAxleTime;     //����խ�������ʱ��
	uint8   AxleStatus;             //����ʻ״̬   1:����,2:ѹ�� 3:��б,4:��б,5:��S  
	uint8	AxleWhelCnt;			//���ֵĸ�����Ĭ��Ϊ0, 2Ϊ����
	uint8   RunAndWalkFlag;         //����ͣͣ��ʶ 0:û������ͣͣ,1:������ͣͣ
	uint8	ZTLayouType;				//��������  Ĭ��Ϊ0Ϊ�����֣�1Ϊһ�ֲ���
	
	uint16  LeftWheelXRange;        //����ִ���λ�÷�Χ
	uint16	RightWheelXRange;       // �Ҳ��ִ���λ�÷�Χ
	uint16 	BuildLeftWhelXRange;    //������ʱ���������ŵ�λ��
	uint16	BuildRghWhelXRange;
	uint16  LeftWheelAverPos;       //����խ��λ��ƽ��ֵ,��λcm
	uint16  RightWheelAverPos;      //����խ��λ��ƽ��ֵ,��λcm
	uint16  MidXPos;                //���м�λ��,��λcm
	
	uint8   LeftWheelZTCnt;
	uint8	RightWheelZTCnt;
	uint8	WheelType;              //������ 1:��̥; 2:˫̥
	uint8   AxleAccCnt;
	
	uint16	AxleAcc[12];
	uint8   u8AxleErrorType;        //���쳣����,bit0:����һ������խ�����ֺ��쳣,bit1:�ְ�����խ������ʱ������,bit2:ͬһ��խ������2����ͬID��,bit3:��׼�����ƥ���խ����С��5��
								    //bit4:�������ٶȲ����,bit5:������ƥ��խ����С��4��,bit6:��Ϊ����ʱ�жϳ�Ϊ����ͣͣ��б��,bit7:None
	uint8   ZType;                  //խ������  Ĭ��Ϊ0������խ��Ϊ1
	
	uint32  ZTWgt[12];    			//�洢ÿ��խ��������,��λkg
	uint32  WheelWgt[2];  			//0:������,1:������,��λkg
	uint16  ZTPos[12];              //�洢խ��Pos
	uint8   ZTID[12];               ////�洢խ��id
	
	uint32  PostAxleTime;           //����ʱ��
	uint8 HideLeftWhePos;
	uint8 HideRightWhePos;
}Axle_Struct;


//�������ݽṹ
typedef struct tagVehicle				
{
	uint8 	u8id;							//�������
	uint8	u8lvl;							//���Ŷ� 	
	uint8 	LasterID;						//�����ļ�����
	uint8 	u8psLane;						//�����ĳ������
	int8	n8accSpeed[ZHOUSIZE];			//���ٶ�	
	uint8	u8OverFlag;						//���ޱ�ʶ
	uint8   UnionWgtFlg;					//���Ϲ��Ʊ�ʶ
	uint16	u16speed;						//����
	
	//		
			  
	uint16	u16AxleList[ZHOUSIZE];			//��Ӧ������,�������
	uint16	u16HideAxleList[ZHOUSIZE];			//��Ӧ������,�������
	uint16	u16UnionFailedAxleList[ZHOUSIZE];	//���Ϲ���ʧ�����б�,��Ӧ������,�������
	uint8	au8AxleTAIType[ZHOUSIZE];		//̥��	  
	uint8	au8AxleGrpType[ZHOUSIZE];		//��������		  
	uint16	au16AxleWeight[ZHOUSIZE];		//����
	uint16	au16HideAxleWeight[ZHOUSIZE];		//��������
	uint16	au16UnionFailedAxleWeight[ZHOUSIZE];	//���Ϲ���ʧ�ܻ�������
	uint16	au16AxleGrpWeight[ZHOUSIZE];	//������		   wujinlong
	uint16	au16AxleDistance[ZHOUSIZE-1];	//���			   wujinlong
	uint16	au16HideAxleDistance[ZHOUSIZE-1];	//�������
	//
	uint16	m_u16VehWide;				// �������� wujinlong 20131031
	uint16	m_u16VehHeight;
	uint8   m_u8VehType;
	uint8   m_u8CPID;	
	uint16  m_u16VehLength;
	//
	uint16  m_u16LaserCnt;
	uint16  m_u16LoopVal;
	uint32  VehicleType; //	�ɸ���Ԥ���������
	uint8   m_u8ProFlag;
	uint8	m_u8Daoflag;
	uint8   m_u8LaserVehID;
	uint8   m_u8PointGet;//���÷�
	//
	uint32  m_u32BuildTime;
	uint32  m_u32EndTime;
	uint8   m_au8AxlePoint[ZHOUSIZE]; //ÿ��÷�
	uint32  m_u32LoopTime;
	//
	uint8   m_u8LaserPos[4]; //����	
	uint8   m_Capture_LaneNum;  //ץ�ĳ������ߵ���λ�ֱ������
	uint8   m_BigVehFollowLitVeh;//�󳵸�С���ı�ʶ��0��ʶ�ޣ�1��ʶ�У��׳�ʱ����
	uint8   m_LitVehFollowBigVeh;//С�����󳵵ı�ʶ��0��ʶ�ޣ�1��ʶ�У��׳�ʱ����
	uint8	u8AixCnt;						//��ĸ���
	uint8  	u8AixGrpCnt;					//����ĸ���
	uint8	AxleStatus[7]; //����ʻ�Ĺ켣	
	uint32  u32HideYiWgt;// һ�ֲ������ŵ�����
	uint32  u32HideJCWgt;//���ؽ����ֳ�������	
						//���²��ֵĽṹ�壬�����к���״ֵ̬������ʻ״̬   1:����,2:��б,3:��б,4:��S  Ĭ��Ϊ0
	uint8   u8PostVehFlag;    //�׳���ʶ(ϵͳ������ʽ),ȡֵ��Χ:0~255
	uint8   u8ViolationCode;                //Υ����, bit7:����ͣͣ,bit6:���,bit5:��ɲ,bit4:S��,bit3:����,bit2:����,bit1:ѹ����,bit0:���;bitλΪ1��Ч
	uint8   m_n8Dir;
} Vehicle_Struct;

typedef struct tagMColum			
{
	uint32 AverColumWgt;    //��ƽ������
	uint8 mcolum[5];
	uint8 WonderColum;
	uint8 WonderCache;
	uint8 Cache[5];
	uint8 RunAndWalk[5];
	uint8 cnt;	
}MColum_Struct;

typedef struct _backVeh
{
	uint8 backAxleCnt;//��������
	uint8 firstRowCnt;//�������ŵĴ���,���Ϊ3
	//uint16 fixedelaytime;	//�̶���ʱʱ��
	uint16 axleSpeed;
	uint32 firstRowDelyDntime;//��խ���³�ʱ��
	uint32 fristRowDntime;
}BackVeh_Struct;

typedef struct _deteleVeh
{
	uint32 l_u32DeleteTime;
	uint8 l_u8DeleteFlag;//1:Ϊ��;2:Ϊ��
}DeleVeh_Str;

//�������ݽṹ
typedef struct taghead				
{	
	int8 	m_cFlag;									// -1��ʼ��,0��ʾ�³�,2��ʾ����,1�ϳ�
	//
	uint8	u8type;										//�¼�����
	uint16 	u16Ltpos; 									//�������
	uint16 	u16rtpos; 									//�Ҳ�����
	//
	uint8	m_u8VehLane;								// �������������� wujinlong 20131031
	uint16	m_u16VehWide;							  	
	uint16	m_u16VehHeight;	
	uint8 	m_u8VehID;
	
	uint16 m_u16VehCalWide;    // ������
}head_Struct;


typedef struct tagRaster				
{	
	int8 m_ucTouch;  //���������ţ�1λ��ʾ1�����⣬0-3λ��ʾ4������
	uint32	u32tm;//����ʱ��
	uint8	u8Cnt;	//������
	int8 m_bDirectionFlag;//����1-2�������ʾһ������ʹ��1��ʾ��3-4�������ʾ��һ������ʹ��0��ʾ
	uint8 VehChangeFlag;    // �г����仯��ʶ
	head_Struct headinfo[LASTERCOUNT<<2];				   // 20121108
} Raster_Struct;

typedef struct tagVehicleInfForLoop
{
	int8    m_n8CurrentAxleNum;//��ʼ-1
	uint8   m_u8CrossHide; //�����������  0����1����
	int8    m_u8DireFlag;//������������ 1 ���� -1 ����
	uint8   m_BigVehFollowLitVeh;//�󳵸�С���ı�ʶ��0��ʶ�ޣ�1��ʶ�У��׳�ʱ����
	int16   m_an16CurrentAxleInf[ZHOUSIZE][MAXNUM_AXLE];//���浱ǰ�����Чխ���� u16sefID
	int16	m_an16HideCurAxleInf[ZHOUSIZE][MAXNUM_AXLE];//����խ����ǰ�����Чխ��IDλ��	
	uint8   m_au8AxleDropFlag[ZHOUSIZE];//��¼���Ƿ��Ѿ��� ��ʼ0
	uint8   m_au8NewAxleFlag[ZHOUSIZE];//��¼������֮ǰ�Ƿ������� ��ʼ0
	int8    m_an8AxleProRow[ZHOUSIZE];//��¼��ǰ�����Чխ���Ѿ����ڼ��� ��ʼ-1
	uint8   m_u8VehicleHaveFlag;//����Ƿ��г�	  0:�޳� 1:�г�
	uint8   m_u8FollowNewVehFlag;//��Ǻ����Ƿ����³�������0���޳� 1���г�
	uint8   RunAndWalkFlag;//m_u8FollowNewVehFlag-->RunAndWalkFlag
	int8    m_n8XieZouFlag;    //���ֲ���б�߱�־, 0������  1����б  2����б
	uint8   m_u8SVSZ;//�Ƿ񵥳�����һ��ģʽ���խ�� 0 ���� 1��
	uint8 	u8psLane;						//�����ĳ������
	uint8   u8ViolationCode;                //Υ����, bit7:����ͣͣ,bit6:���,bit5:��ɲ,bit4:S��,bit3:����,bit2:����,bit1:ѹ����,bit0:���;bitλΪ1��Ч	
	uint8   m_LitVehFollowBigVeh;//С�����󳵵ı�ʶ��0��ʶ�ޣ�1��ʶ�У��׳�ʱ����
	uint8   m_u8VehIDFromLaser;
	uint32  m_u32TimeNow;//��¼�����������ʱ�� ��ʼֵ0
	uint32  m_u32BuildTime;//����ʱ��
	uint32  m_u32EndTime;//����ʱ��
	int8	n8accSpeed[ZHOUSIZE];			//���ٶ�
	int8    m_n8CurrentHideAxleNum;//��ʼ-1
	uint8	u8OverFlag;						//���ޱ�ʶ
	uint16	u16speed;						//����				
	uint16	u16AxleList[ZHOUSIZE];
	uint16  u16HideAxleList[ZHOUSIZE];
	uint8	u8AixCnt;						//��ĸ���	
	uint8	u8HideAixCnt;
	uint8  	u8AixGrpCnt;					//����ĸ���
	uint8  	u8HideAixGrpCnt;
	uint8	au8AxleTAIType[ZHOUSIZE];		//̥��	  
	uint8	au8AxleGrpType[ZHOUSIZE];		//��������		  
	uint16	au16AxleWeight[ZHOUSIZE];		//����			  
	uint16	au16AxleGrpWeight[ZHOUSIZE];	//������
	uint16	au16AxleDistance[ZHOUSIZE-1];	//���
	uint8	au8HideAxleTAIType[ZHOUSIZE];		//����̥��	  
	uint8	au8HideAxleGrpType[ZHOUSIZE];		//������������		  
	uint16	au16HideAxleWeight[ZHOUSIZE];		//��������			  
	uint16	au16HideAxleGrpWeight[ZHOUSIZE];	//����������
	uint16	au16HideAxleDistance[ZHOUSIZE-1];	//�������
	//
	uint16	m_u16VehWide;				// �������� wujinlong 20131031
	uint16	m_u16VehHeight;
	uint8   m_Capture_LaneNum;  //ץ�ĳ������ߵ���λ�ֱ������
	uint8	AxleStatus[7]; //����ʻ�Ĺ켣	
	uint8   m_au8AxlePoint[ZHOUSIZE]; //ÿ��÷�	
	uint32  m_u32LoopTime;    //ץ��ʱ�̣���485ID
	uint32  u32HideYiWgt;// һ�ֲ������ŵ�����
	uint32  u32HideJCWgt;//���ؽ����ֳ�������  
						//���²��ֵĽṹ�壬�����к���״ֵ̬������ʻ״̬   1:����,2:��б,3:��б,4:��S  Ĭ��Ϊ0
	uint8   NormalAxleScore[7];    //��׼�������ۺϵ÷�(��ʻ״̬,խ��ƥ��,��켣���ۺϵ÷�)
	uint8   HideAxleScore[7];      //���ذ������ۺϵ÷�(��ʻ״̬,խ��ƥ��,��켣���ۺϵ÷�)
	uint8 	m_u8NormalLogiclvl;      //�к��߼����Ŷ�,Ĭ��Ϊ0:����,!0:������(���������ͼ����쳣����)
	uint8 	m_u8HideLogiclvl;    //�����߼����Ŷ�,Ĭ��Ϊ0:����,!0:������
//	uint8   m_u8PointsGet;//���÷�
	uint32  m_u32ForcePostTime;
	uint8   m_u8PostAxleFlag;
} VehicleInfForLoop_Struct;
typedef struct tagVehUnUsForLoop
{
	uint8 m_u8PassThreeWonder;//ѹ���߻��ɱ�־ 0 û�� 1 �л���
	uint8 m_u8WonderChl;
	uint16 m_u16ID;
} VehUnUsForLoop_Struct;

//ͨ��
typedef struct Stu_loop_info
{
	uint8 loop_num;
	uint8 frame_id;
	uint8 frame_type;
	uint8 rsv;
	uint32 frame_data;
	uint32 trigger_time;
}Stu_loop_info;

#define MAX_AXLE_A_VEHICLE 		ZHOUSIZE		//�������16����
#define MAX_ZT_AXLE_AFFECT 		12				//�������Ӱ��խ����

typedef int32 UN;
typedef int32 US;
typedef int32 UL;
typedef int32 UC;


//խ���ź����ݽṹ---------
typedef struct {
	UL m_ulUpTime;
	UL m_ulDownTime;
	UN m_unSum;
	UN m_unMaxValue;	
}ZhitiaoLogicData;
//
typedef struct{					   //խ��һ������
	uint16	u16ID;
	UC m_ucTouch;					// խ�����1-32
	UL m_ulTime;					// �ϳ�ʱ����³�ʱ��
	int8 m_bUpOrDn;					// 1���ϳӣ�0 �³�
	ZhitiaoLogicData m_CWbZtInf;
	int8 m_bDirectionFlag;
} ZTINFRECELEMENT;

// ���ڻ���
#define		COM_LEN		128
typedef struct _CComBuf
{
	uint16	m_u16Len;
	uint8	m_au8DataBuf[COM_LEN];
} CComBuf;



// ��־������in!=out��ʽ����ֱ�Ӷ���Ϊ��ά����
// added by wujinlong 20130819
// ��־����
#define		LOG_NUM		1000
#define		LOG_LEN		128
typedef struct _CLogBuf
{
	uint8	m_u8In;
	uint8	m_u8Out;
	uint16	m_u16Len[LOG_NUM];
	uint8	m_au8DataBuf[LOG_NUM][COM_LEN];
} CLogBuf;

typedef struct	//����ʱ��ͳ��
{
	UL	m_ucWT_TASKFlag;
	UL	m_ulWT_Task1;
	UL	m_ulWT_TaskDet;
	
	UL	m_ucAnalysisFlag;
	UL	m_ulAnalysis1;
	UL	m_ulAnalysisDet;
   	
	UL	m_ucTaskWaveFlag;
	UL	m_ulTaskWave1;
	UL	m_ulTaskWaveDet;

	UL	m_ulTaskProtocolFlag;
	UL	m_ulTaskProtocol1;
	UL	m_ulTaskProtocolDet;
}TASKRUNTIME;

#define		HEARTBEATLEN	100
typedef struct _CHeartBuf
{
	uint16	m_u16Len[NET_PORT_NUM];
	uint8	m_au8DataBuf[NET_PORT_NUM][HEARTBEATLEN];
} CHeartBuf;


//#if LOGIC_YAFENG_PROCESS_EN > 0
typedef struct _CYaFengStack
{
	uint8	m_u8IsValidFlag;		// 1��������Ч��0��������Ч
	uint16	m_u16ChlIx;				// ����������
} CYaFengStack;
//#endif

typedef struct _CSDOffnetIx
{
	uint32		m_u32OffnetVehNum ;					// �ܳ���
	uint32		m_u32OffnetWrSec;					// д����λ��
	uint16		m_u32OffnetWrSub;					// д������λ��
	uint32		m_u32OffnetRdSec;					// ������λ��
	uint16		m_u32OffnetRdSub;					// ��������λ��
	uint32		m_u32VehIx;							// λ�����
} CSDOffnetIx;


//#if LASER_UNNORMAL_EN > 0
typedef struct _CVehSimInfo
{
	uint8	m_u8IsValidFlag;		// 1��������Ч��0��������Ч
	uint32	m_u32PostVehFPGATime;	// �׳�ʱFPGAʱ��
	uint8	m_u8AxleNum;			// ����
	uint32	m_u32Weight;			// ����
} CVehSimInfo;
//#endif

///////////////////////////////////////////��������ʹ�ýṹ��
// �������������ݽṹ
typedef struct _AXLE_DATA
{
	uint32 che_num;		                // ����  
	uint32 axis_num;					// ����
	uint32 allche_wt;				    // ������
	uint32 allaxle_wt; 			        // ������
	uint32 acc_equivalent_axles ;		// ������κϼ�

} AXLE_DATA;
// ���峵�����ط������ݻ���ṹ
typedef struct _LANE_AXLE_DATA
{
	AXLE_DATA axle_data[10];            // ����ͳ��	10��������
	AXLE_DATA axis_num_data[5];         // ����ͳ��	5��������
	AXLE_DATA axle_weight_data[4];      // ����ͳ��	4��������

} LANE_AXLE_DATA;
// ���峵���������ݽṹ
typedef struct _LANE_INFO
{
	// ��Ȧ���
	uint32 mu32LoopOnTime;
	uint32 mu32LoopOffTime;

	uint32 mu32LastLoopOnTime;
	uint32 mu32LastLoopOffTime;

  uint32 mu32TimeOnOff;     // ͨ����Ȧʱ�� 
	uint32 mu32LoopScope;
} LANELOOP_INFO;

typedef struct _LANE_TRAFFIC_DATA
{
	uint32 mu32LaneNum;						// ������
	uint32 mu32VehNum; 					// ����
	uint32 mu32FollowVehNum;					// ������
	uint32 mu32FollowVehPercent;				// �����ٷֱ�
	uint32 mu32TotalDisForVehHead; 			// ��ͷ�ܼ��
	uint32 mu32AveDisForVehHead; 					// ƽ����ͷ���
	uint32 mu32TimeOccupancy;				// ʱ��ռ����
	uint32 mu32XiaoHuoNum;					// С����ͨ��
	uint32 mu32XiaoHuoAveSpeed;			// С��ƽ���ٶ�
	uint32 mu32XiaoHuoTotalSpeed;			// С�����ٶ�
	uint32 mu32ZhongHuoNum;				// �л���ͨ��
	uint32 mu32ZhongHuoAveSpeed;			// �л�ƽ���ٶ�
	uint32 mu32ZhongHuoTotalSpeed;		// �л����ٶ�
	uint32 mu32DaHuoNum;					// �����ͨ��
	uint32 mu32DaHuoAveSpeed;				// ���ƽ���ٶ�
	uint32 mu32DaHuoTotalSpeed;			// ������ٶ�
	uint32 mu32ZhongXiaoKeNum;				// ��С�ͽ�ͨ��
	uint32 mu32ZhongXiaoKeAveSpeed;		// ��С��ƽ���ٶ�
	uint32 mu32ZhongXiaoKeTotalSpeed;		// ��С�����ٶ�
	uint32 mu32DaKeNum;					// ��ͽ�ͨ��
	uint32 mu32DaKeAveSpeed;				// ���ƽ���ٶ�
	uint32 mu32DaKeTotalSpeed;			// ������ٶ�
	uint32 mu32TuoGuaNum;					// �Ϲҳ���ͨ��
	uint32 mu32TuoGuaAveSpeed;				// �Ϲҳ�ƽ���ٶ�
	uint32 mu32TuoGuaTotalSpeed;			// �Ϲҳ����ٶ�
	uint32 mu32TuoLaJiNum;					// ��������ͨ��
	uint32 mu32TuoLaJiAveSpeed;			// ������ƽ���ٶ�
	uint32 mu32TuoLaJiTotalSpeed;			// ���������ٶ�
	uint32 mu32TeDaHuoNum;					// �ش����ͨ��
	uint32 mu32TeDaHuoAveSpeed;			// �ش��ƽ���ٶ�
	uint32 mu32TeDaHuoTotalSpeed;			// �ش�����ٶ�
	uint32 mu32MotoNum;					// Ħ�г���ͨ��
	uint32 mu32MotoAveSpeed;				// Ħ�г�ƽ���ٶ�
	uint32 mu32MotoTotalSpeed;			// Ħ�г����ٶ�
	uint32 mu32JiZhangXiangNum;	  	// ��װ�䳵ƽ���ٶ�
	uint32 mu32JiZhangXiangAveSpeed; 	// ��װ�䳵���ٶ�
	uint32 mu32JiZhangXiangTotalSpeed;		  	// ��װ�䳵���ٶ�

} LANE_TRAFFIC_DATA;

typedef struct tagJDSysParam
{
	uint8   m_au8UseName[8];
	uint8   m_au8KeyNum[8];
	
	uint8   m_au8DeviceID[16];

	uint8   m_au8StationID[15];
	uint8   m_u8LaneDir;  //0 ���� 1 ˫��

	uint8   m_u8OrderMod;
	uint8   m_u8SavePeriod;
	uint8   m_u8TransMod;//�ϴ���ʽ 1 ���� 2 ����
	uint8   m_u8SendVehType;
	 
	uint16  m_u16SmallandMid;  //С�г��������
	uint16  m_au16Elevation;  //�߳�

	uint32  m_u32FollowVehTimeThr;//��������ʱ����ֵ
//	uint32  m_u32MidKeHuoThr;//������Ȧ��ֵ�п��л�����
//	uint32  m_u32SmallKeHuoThr;// ������Ȧ��ֵС��С������
	uint32  m_u32DiffSend; //������������ֵ

	uint8   m_au8LngBuf[8];	//����
	uint8   m_au8LatBuf[8];	//γ��

	uint8	m_u8ProgVersion[4];					// ����汾��ʾ

	uint16  m_u16MidKeHuoThr[10];			   //FZX
	uint16  m_u16SmallKeHuoThr[10];					   	//FB

	uint16  m_u16VehNumForSend;//������������ֵ
	uint16  m_u16UnUse;

	uint32  m_u32WeightSendThr;//������������ֵ
	uint32  m_u32AxleNumSendThr;//������������ֵ
	uint32  m_u32AxleWghSendThr;//���ش�������ֵ

	uint32  m_u32MaxForAxle;//������������ֵ

	uint8 	m_au8ReserveByte[JDSYS_RESERVED];			//Ԥ���ֽ�	
	uint16	m_u16CRC;
} CJDSysParam;

#endif

