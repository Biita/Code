
/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JZGlobal.h
** Last modified Date:  2011511
** Last Version:		1.0
** Descriptions:		���س���ȫ�ֱ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:			Wang ZiFeng
** Created date:		20121114
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
#ifndef	__JZGLOBAL_H
#define	__JZGLOBAL_H


#ifdef	__JZGLOBAL
#define	JZG_EXT	
#else
#define	JZG_EXT	extern
#endif

#include	"JZStructure.h"	 

/*
*********************************************************************************************************
*                                        �㷨��صı���
*********************************************************************************************************
*/

JZG_EXT uint8 g_StartWaveFlag;
JZG_EXT uint8 startWave[12];      //ԭ��Ϊ11���ֽ�,���ڷ����ֽڲ�����,�ĳ�12���ֽ�
JZG_EXT uint8 finishWave[12];     //ԭ��Ϊ11���ֽ�,���ڷ����ֽڲ�����,�ĳ�12���ֽ�
JZG_EXT uint8 coilWave[19];
JZG_EXT uint8 laserWave[60];
JZG_EXT uint32 PCSendCnt;
JZG_EXT uint8		g_u8RasterZTRead;	
JZG_EXT uint8		g_u8RasterRead;
JZG_EXT uint8 ZTRead[64];


JZG_EXT	uint32			speed1[CHANNELNUM];	//խ������·������ٶ�1
JZG_EXT	uint32			speed2[CHANNELNUM];	 //խ������·������ٶ�2
JZG_EXT	uint32			speed3[CHANNELNUM];	 //խ������·������ٶ�3



JZG_EXT		CoilOrLaserBuf_Struct     g_CoilOrLaserData;   /*����ȫ����ȦIO������IO������λ�ã�������ʼ�ͽ�����*/
JZG_EXT	uint16  g_u16SystemUseMode;                          /*add by sxh  ����ʹ��ģʽȫ�ֱ��� (3.28)*/ 




/*
*********************************************************************************************************
*                                       ����
*********************************************************************************************************
*/
#define RASTERDATALEN			256
#define BUFSIZE		    		128
#define	UARTRCVBUFSIZE			2048
#define OFFSET					6							  // ������λ��
#define	LASER_RESERVE_BEGIN		55							  // Ԥ����1�ֽ�λ��
 
//JZG_EXT		uint8	    		g_LasterState[LASTERIOCNT];	  //���ⴥ������״̬
//JZG_EXT		uint8	    		g_u8LasterStateAll;		      //���ⴥ������״̬��1λ��ʾ1����			20121108

JZG_EXT		Raster_Struct		g_RasterData[RASTERDATALEN];  //���⴫����������λ��//   20121108
JZG_EXT		uint8				g_u8RasterDataReadIndex;	  //��������Ϣ	���ڲ��η��Ͷ�ȡ����  ��δ��ʼ��
JZG_EXT		uint8				g_u8RasterDataRead;		      //��������Ϣ	�����߼���ȡ��������  ��δ��ʼ��
JZG_EXT		uint8				g_u8RasterDataWriteIndex;	  //д������Ϣ	���ڴ��ڽ���д��

JZG_EXT		uint8               g_u8RasterDaFeiCnt;           //����������ɳ���20����Ĵ�
JZG_EXT		uint8               g_u8RasterNoDaFeiCnt;           //����������ɲ�����20����Ĵ���

extern      uint8	            g_au8Data[BUFSIZE];	          //���⴮������
//JZG_EXT		uint8	g_au8Data[BUFSIZE] = {0};
extern      uint8               g_u8DataLen;
//JZG_EXT		uint8   g_u8DataLen = 0;

/*
*********************************************************************************************************
*                                       ����
*********************************************************************************************************
*/
JZG_EXT	volatile uint8	g_u8KeyValueMapped;
JZG_EXT	volatile uint32	g_u32KeyValueOri;	
JZG_EXT	volatile uint32	g_u32KeyCnt;

/*
*********************************************************************************************************
*                                        SD������
*********************************************************************************************************
*/
#ifdef __STRG_SD

JZG_EXT uint8			g_u8SDInsert; 
JZG_EXT	uint8			g_u8SDEndFlag;
JZG_EXT	uint8			g_u8SDDownFlag;

#endif	

/*
*********************************************************************************************************
*                                        �¶ȱ���
*********************************************************************************************************
*/	
#define TEMPERATURELEN	16  
JZG_EXT	int32			g_n32Temperature; 
JZG_EXT	uint8			g_u8TemperatureIndex;	//index
JZG_EXT	int32			g_an32TemperatureBuf[TEMPERATURELEN];

/*
*********************************************************************************************************
*                                        �������ñ���
*********************************************************************************************************
*/

JZG_EXT	SetupParam		g_Setup;				//���ò���	
//JZG_EXT	NetInfo			g_IPSetLocal;		//������Ϣ
JZG_EXT	LaserSetupParam   g_LaserPara;          //�������
JZG_EXT CProModParam     g_cProModParam;			//wgh 20140303 ģʽѡ�����

/*
*********************************************************************************************************
*                                        ����������
*********************************************************************************************************
*/
JZG_EXT	int32			g_an32MotionZero[CHANNELNUM];	//��̬���
JZG_EXT	uint8			g_u8StaticMotionswitch;			//����ϵͳ������̬�л�:��̬ 1; ��̬ 0 ������
//JZG_EXT  int32			g_s32ADValue[CHANNELNUM]; 	//��ǰһ���ڵľ�ֵ

//#define MOTIONZERO_LEN		 8
//JZG_EXT	uint16			g_au16MotionZeroMiddleValue[CHANNELNUM][MOTIONZERO_LEN];	//��̬����м�ֵ�����ڸ��±Ƚ�ʹ��
//JZG_EXT	uint8			g_au8MotionZeroMiddleValueIndex[CHANNELNUM];	//дλ��
//JZG_EXT	uint8			g_au8MotionZeroMiddleValueCnt[CHANNELNUM];	//��̬����м�ֵ������Ҫ�����
//JZG_EXT	uint8			g_u8MotionZeroZLThresh[CHANNELNUM];	 // ������ֵ	 
//#define MOTIONZERONUM_LEN		 3			// ����3�� ����ֵ�ھ͸������
//#define MOTIONZEROZLThresh		 50			// ����3�� ����ֵ�ھ͸������
/*
*********************************************************************************************************
*                                        ʱ�ӱ���
*********************************************************************************************************
*/ 
//JZG_EXT	uint32			g_u32SN;				//SN��
JZG_EXT	SystemTime		g_DevCurTime;			//��ǰʱ��
//JZG_EXT	SystemTime		g_DevStartTime;			//����ʱ�䣬���ڼ�¼ϵͳ����ʱ��

/*
*********************************************************************************************************
*                                        �ź���
*********************************************************************************************************
*/
#define SENDLISTNUM	128

//JZG_EXT	OS_EVENT *		g_pQVehicle;		//������������ʾ������Ϣ�ź���
//JZG_EXT void	 *		g_apvVehShow[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_psemScreenRefresh;	//��Ļˢ���ź���

JZG_EXT	OS_EVENT *		g_psemKey;				//�����ź���

JZG_EXT	OS_EVENT *		Sem_KeyIn;

//JZG_EXT	OS_EVENT *		g_pSemTime;		//ϵͳʱ���ź���

//JZG_EXT	OS_EVENT *		g_pQAxleShow;			//��ʾ����Ϣ��Ϣ����
//JZG_EXT void	 *		g_apvAxleShow[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pQAxleCalc;			//�����������Ϣ���� 
JZG_EXT void	 *		g_apvAxleInfo[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pQShouWei;			//������β�ź���
JZG_EXT void	 *		g_pQShouWeiList[SENDLISTNUM];	 


JZG_EXT	OS_EVENT *		g_pQSendPC;		//�������ݶ���
JZG_EXT void	 *		g_pSendList[SENDLISTNUM];



//JZG_EXT	OS_EVENT *		g_pQErrMsg;		//����䴫����Ϣʧ��
//JZG_EXT void	 *		g_pQErrList[SENDLISTNUM];

//JZG_EXT	OS_EVENT *		g_pQUpMsg;		//�ϳ���Ϣ֪ͨ
//JZG_EXT void	 *		g_pQUpList[4];

#define DNLISTNUM	(CHANNELNUM*6)
JZG_EXT	OS_EVENT *		g_pQDnMsg;		//�³���Ϣ֪ͨ
JZG_EXT void	 *		g_pQDnList[DNLISTNUM];	 // �ĳ�4->16��wujinlong 20121126
JZG_EXT OS_EVENT *      g_pQSendPCDebug;
JZG_EXT void     *      g_pSendListDebug[DNLISTNUM];

JZG_EXT	OS_EVENT *		g_pSemTask6Param;		//����6��Ҫ�Ĳ����޸�֪ͨ
//
JZG_EXT	OS_EVENT *		g_pQWaveMsg;//���Ͳ����ź�	wzf
JZG_EXT void	 *		g_pQWaveList[DNLISTNUM];

JZG_EXT	OS_EVENT *		g_pQNetWavePC;		//���Ͳ������ݶ��� wzf
JZG_EXT void	 *		g_pNetWaveList[DNLISTNUM];


// test SP
//JZG_EXT	OS_EVENT *		g_pQSendThrouSP;		//�������ݶ���
//JZG_EXT void	 *		g_pSendThrouSPList[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pSemSendThtouSP;		    // ����

//
//JZG_EXT	OS_EVENT *		g_psemLaserInteract;		//���⽻����Ϣ�ź���
JZG_EXT	OS_EVENT *		g_psQLaserInteract;		//���⽻����Ϣ�ź���
#define		RECVLASERDATA_NUM	16
JZG_EXT void	 *		g_pLaserInteractList[RECVLASERDATA_NUM];

#if DEBUGINFO_TASK_SP_EN > 0
#define		COM_NUM		4000
//JZG_EXT OS_EVENT *		g_pEventMutexCOM1;
JZG_EXT	OS_EVENT *		g_pQSendPCCOM1;		//�������ݶ���
JZG_EXT void	 *		g_pSendPCCOM1List[COM_NUM];
#endif

// SD
#define		SD_OFFNET_WRITE_LEN		(16*2)			// ����*2��Ԥ����
JZG_EXT	OS_EVENT *		g_pQSDOffNetWrite;			//SD��д���ݶ���
JZG_EXT void	 *		g_paSDOffNetWriteList[SD_OFFNET_WRITE_LEN];
//
#define		SD_VEH_WRITE_LEN		(16*2)			// ����*2��Ԥ����
JZG_EXT	OS_EVENT *		g_pQSDVehWrite;			//SD��д���ݶ���
JZG_EXT void	 *		g_pQSDVehWriteList[SD_VEH_WRITE_LEN];
//
#define		SD_DBGINFO_WRITE_LEN		COM_NUM		
JZG_EXT	OS_EVENT *		g_pQSDDebugInfoWrite;			//SD��д���ݶ���
JZG_EXT void	 *		g_paSDDebugInfoWriteList[SD_DBGINFO_WRITE_LEN];
//
#define		SD_WAVE_WRITE_LEN		DNLISTNUM		
JZG_EXT	OS_EVENT *		g_pQSDWaveWrite;			//SD��д���ݶ���
JZG_EXT void	 *		g_paSDWaveWriteList[SD_WAVE_WRITE_LEN];

JZG_EXT OS_EVENT *      g_pSDSaveForJD;

#define     JD_SEND_LEN    6
JZG_EXT	OS_EVENT *		g_pQJDSend;			//SD��д���ݶ���
JZG_EXT void	 *		g_paJDSendList[JD_SEND_LEN];
JZG_EXT	OS_EVENT *      g_pSemDisplayCheliangInfo;  //Һ����ʾ��������Ϣ  rp  20151009
JZG_EXT void	 *		g_paDisplayInfo[15];

/*
*********************************************************************************************************
*                                        ������Դͳ�Ʊ���
*********************************************************************************************************
*/
#if	OS_TASK_CREATE_EXT_EN > 0
#define TASK_SUM	10
JZG_EXT	  TaskStatics 	 g_Taskstatics[TASK_SUM];	 
#endif
/*
*********************************************************************************************************
*                                        �豸״̬��
*********************************************************************************************************
*/
#define ERR_LEN		 9
JZG_EXT	uint8			g_u8DeviceERR[ERR_LEN];	//�豸����,��һ�ֽڵ�4λ��ʾ���⣬��һ�ֽڸ�4λ��ʾͨ��.��8�ֽڱ�ʾխ��״̬,
JZG_EXT uint8           g_u8IOERR[2];//ÿ��IO����״̬��ÿλһ���� 0���� 1����
JZG_EXT uint8           g_u8IOStateRe[16];//��¼һ��IO����һ��״̬��ͨ����խ����
JZG_EXT uint8           g_u8ZTStateRe[64];//��¼ÿ��խ����������
//JZG_EXT	uint8			g_u8ERR; 	//��ʾ���豸����,������ǰ�Ĵ�����
JZG_EXT	uint32			g_u32StartupTime;		//��������

/*
*********************************************************************************************************
*                                        ץ����ر���
*********************************************************************************************************
*/
#define CapVehCache    4    //2��ץ�ļ仺��ĳ�����
JZG_EXT uint16 g_u16CapDelayTime;                     //����ץ�ĵļ��ʱ��,��λms,(Ĭ��:80ms) 
JZG_EXT uint8  g_u8CapFlag[CapVehCache];              //0:��ǰ�޳���������ץ��,1:������ɵ�һ��ץ��
JZG_EXT uint32 g_u32FirstCapTime[CapVehCache];        //��¼��һ��ץ��ʱ��
JZG_EXT uint32 g_u32CapInfo[CapVehCache][4];          //��¼ץ����Ϣ,���ڽ��еڶ���ץ��

/*
*********************************************************************************************************
*                                        �������ݱ���
*********************************************************************************************************
*/
#define CHL_BUFLEN				512
#define CHL_BUFMASK				0x1FF
#define CHL_MAXNUM				10	   //wgh 20140305 ���֧�ֵĳ���
#define YAFENGAREASIZE  		40
#define CHL_SAVEVEH     		1      //ÿ���������ɻ��泵��
#define COLUM_SAVEAXlE			4			//ÿ�����ɻ��������
#define AXlE_MAXCOLUM			3    //ÿ�������ѹ����խ��
#define MAXFINDCOLUM			4   //������4�У������������
#define MAXDIFFTICK				2			//��̥��������ܴ���2����
#define HideZTNUM   			24   //����խ����ʼ���
//zhangtonghan

#define XIEZOUSPDRATIOMAX    	150    //б�����������ٶȶα�ֵ����
#define XIEZOUSPDRATIOMIN    	60     //б�����������ٶȶα�ֵ����

JZG_EXT		Channal_Struct		g_ChlData[CHL_BUFLEN];		//խ��������Ϣ		 wujinlong ����
JZG_EXT		uint32				g_ZTTime[64];    //�洢խ��ʱ��,����ɾ��ͬһ��խ�����������򴥷�����ܶ�
// ���Ӽ��ⴥ��ģ��խ����Ϣ��������ʹ�ã��޸� 20131202 
#define		LASERINFO_LEN		64
JZG_EXT		Channal_Struct	 	g_cLaserInfo[LASERINFO_LEN];
JZG_EXT		uint8				g_u8LaserInfoIx;

JZG_EXT		Channal_Struct	 	g_cIOChange[LASERINFO_LEN];
JZG_EXT		uint8				g_u8IOChangeIx;


#define EMPTY_ITEMZERO 			0xFFFF
//JZG_EXT 	uint8 				g_FPGAChlIndex;
JZG_EXT	    uint16				g_ChlID2TriggerID[CHANNELNUM];		//����ID��խ��ID��Ӧmap��
		
//JZG_EXT		uint16				g_u16ChlRindex;		//խ����������Ϣ
JZG_EXT		uint16				g_u16ChlWindex;		//խ���´�д������Ϣ
//JZG_EXT		uint16				g_u16ChlRecvCnt;	//խ���������ݸ���


#define AXLE_BUFLEN				128 
#define AXLE_BUFMASK			0x7F
#define WHEELTRACEZTNUM         36     //�����ֹ켣��խ����
#define WHEELCACHE              4      //�����켣ʱ�����ֵĸ���
JZG_EXT		Axle_Struct				g_AxleInfo[AXLE_BUFLEN];	//��������Ϣ					 20121107
JZG_EXT		uint16					g_u16AxleInfoWriteIndex;		//��������Ϣ�´�д������Ϣ
JZG_EXT		Channal_Struct			g_ChlData[CHL_BUFLEN];		//խ��������Ϣ		 
// JZG_EXT		Wheel_Struct			g_WheelData[CHL_MAXNUM*16][1];	//�����ֵ�������Ϣ
JZG_EXT		Whel_Struct             g_WhelData[CHL_MAXNUM*4];	    //�����ֵ�������Ϣ(���չ켣���ֵĽṹ��)
JZG_EXT		TraceWhel_Struct		g_WheelTrace[WHEELTRACEZTNUM][WHEELCACHE];
JZG_EXT		TraceWhel_Struct		g_ReWheelTrace[WHEELTRACEZTNUM][WHEELCACHE];
//JZG_EXT
JZG_EXT		uint16						g_u16WheelInfoWriteIndex; //��������Ϣ�´�д���ݵ�λ��

JZG_EXT   uint8         g_u8FirstAxleFlag;        //������β��־
JZG_EXT   uint8         g_u8FirstAxleColum;
JZG_EXT   uint8         g_u8CrossingFlag;         //�����β��־
JZG_EXT   uint8         g_u8CrossingColum;


#define  VEHICLE_BUFLEN	 		32
#define  VEHICLE_BUFMASK		0x0F
//zhangtonghan 20170531
#define	MtchZT4Speed			10   //ƥ�䲼��խ�������ٶȲ�С��10Km/h
#define Mtch4SpdThr				20  //ƥ�䲼���ٶ���ֵΪ20Km/h    
#define SpdDiffThr				10  //�ٶȲ�ֵ����ֵ
#define Mtch4LthThr				200  //ƥ��ӵس�����ֵΪ200mm	
#define LthDiffThr1				95		//�ӵس��Ȳ�ֵ95mm
#define LthDiffThr2				70		//�ӵس��Ȳ�ֵ70mm
#define MinLth					25   //��С�ӵس���Ϊ25mm
#define MaxSpeed				120  //�����ٶ�Ϊ120Km/h
#define	HideMxSpeef				1200 //����խ������ٶ�
#define	HideZTSpdDiff			100		//�ٶȲ�
#define ThrMaxSpdDiff   		5   //ѹ�����ٶȲ���С��
#define MaxDiffLth      		4   //���������ӵس��Ȳ�
#define RunWalkMaxTick  		1950//ѹխ����������������Ϊ����ͣͣ
#define MinTick					100  //��С�Ĳ�ֵ����������խ��ƥ��
#define RunWalkDiffTick 		15000 //����ͣͣխ���³Ƶ�����
#define SecSubFriTick			160  //�ڶ��ŵ��ϳӱ�����ڵ�һ���³ӵĵ���
#define MinWheelDiff			4    //ͬ������С������
#define MaxWheelDiff    		12    //ͬ�������������
#define MaxSameWheelDiff 		4    //���ͬ�ֵ�����
#define	SameWheelXieCnt			3
#define MaxSameWheelXieXingDiff	4 //б�����ͬ�ֵ�����
#define DifAxleWheel			4     //ͬһ������ͬ������4����
#define MinTwoRowTimeDiff 		140  //���������ֵ�����
#define NotFindWheelDot   		5  //����������������λ�õ���
#define FriRow2DecRowGap		800  //һ�ֲ������ŵ��ڶ��ŵľ���
#define HideZTRow				2    //����խ��������
#define AxleAccRange			10  //���ٶ�������ֵ
#define HideZTOddDiff			120  
#define DelyTimeSecd     		10
#define RunAndWalkMinSpeed      300    //����ͣͣ�ж���С�ٶ� 
#define CoilDelayDistance 		2  //��������������Ȧ��˸��ʱ�׳�����2m
#define HIDECOLUM_SAVEAXlE		2			//����ÿ�����ɻ��������
#define HideMinTickDif			160  //����խ��������С�³ӵ�����
#define	DigtalZTDotCnt			4 //����խ���ϵĵ���
#define LogicDlyTime			240
#define	LowSpdDlyTime 			1200
#define SUMAVERLITVEH           10000  //С�����ֺ;�ֵ�������ֵ
#define MAXVALUELITVEH          500    //С�����ֵ��ֵ�������ֵ
#define MaxWheelGaugeLitVeh     270    //С�������,��λcm
#define MaxWheelGauge           320    //����ּ��,��λcm
#define MinWheelGauge           70     //��С�ּ��,��λcm
#define XieZouDistance          75     //б�о�����ֵ,��λcm,�ֹ�ĵ�һ��խ�������һ��խ��֮��ľ������ж�б��
#define AxleWgtMinError         100       //��С�����쳣��ֵ,��λkg
#define AxleWgtMaxError         20000     //��������쳣��ֵ,��λkg
#define AXLEMIDDISTANCE         100       //��ƥ��ʱ������켣�ж�ʱ��С���,��λcm
#define MAXVALUETHRED           300       //�������س���խ�����ֵ��ֵ
#define POSTVEHDELAY            200       //�׳�������ʱʱ��,200����

//���ֺ궨��
#define JCLAYOUT                0       //������
#define HIDELAYOUT              1       //���ز���
//end

//�켣���ֵ���ʻ״̬�궨��
#define NORMAL               1    //������ʻ
#define YAFENG               2    //ѹ��
#define XIEZOULEFT           3    //��б
#define XIEZOURIGHT          4    //��б
#define MOVEROUND            5    //��S
//end

#define NORMALLAYOUT        1     //0:ʹ�úϴ����ֵ����ֲ��ֽ��в���,1:�ֳ�ʵ�ʲ���   ����Ӧ����Ϊ1

// JZG_EXT     uint32 EvenColumDelyLth	;   //ż������ʱ15cmƥ��
// JZG_EXT     uint32 OddColumDelyLth	;  //��������Ҫ��ż���м�55cm
JZG_EXT		Vehicle_Struct	    g_vehicleInfo[VEHICLE_BUFLEN];	//����������Ϣ
JZG_EXT		uint8				g_u8vehicleID;				//ά��������ţ�0-256

JZG_EXT     VehicleInfForLoop_Struct 		g_asVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//��ͨ�������д洢������Ϣ�ṹ��
JZG_EXT     VehicleInfForLoop_Struct 		g_asReVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//��ͨ�������д洢������Ϣ�ṹ��
JZG_EXT     VehicleInfForLoop_Struct 		g_HideZTVehicleStruct[2*CHL_MAXNUM][CHL_SAVEVEH]; //����խ���������ݽṹ��
JZG_EXT     VehicleInfForLoop_Struct 		g_HideZTReVehicleStruct[2*CHL_MAXNUM][CHL_SAVEVEH];
JZG_EXT     uint32							g_au32HideDeltZTTime[CHL_MAXNUM];//ɾ������խ��
JZG_EXT     uint8							g_au8HideDeltZTEnable[CHL_MAXNUM];
JZG_EXT     uint32							g_u32PreLoopTime[CHL_MAXNUM];      //��¼ÿ������ǰ����Ȧ��βʱ��(������Ȧ��β��������)
JZG_EXT     uint32							g_u32PostLoopTime[CHL_MAXNUM];     //��¼ÿ������������Ȧ��βʱ��(������Ȧ��β��������)
JZG_EXT     uint8							g_u8PostVehFlag[CHL_MAXNUM];          //ÿ�����������׳���־,0:���׳�,1:�����׳�,default:0
JZG_EXT     uint8							g_u8RePostVehFlag[CHL_MAXNUM];        //ÿ�����������׳���־,0:���׳�,1:�����׳�,default:0
// JZG_EXT		Colum_Struct					Forward_ZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//�����л���
// JZG_EXT		Colum_Struct					Reverse_ZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//�����л���
// JZG_EXT		int16 							g_WonderZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //��������խ��
JZG_EXT		BackVeh_Struct					g_BackVeh[CHL_MAXNUM*2];//������ʶ
JZG_EXT		DeleVeh_Str         			g_DeletVeh[CHL_MAXNUM*2];
JZG_EXT     uint8							g_u8XieLaneFlag[10];//���ڳ���б�б�ʶ
JZG_EXT		HideColum_Struct				Forward_HideZTColum[CHL_MAXNUM*2][HIDECOLUM_SAVEAXlE];//���������л���
JZG_EXT		HideColum_Struct				Reverse_HideZTColum[CHL_MAXNUM*2][HIDECOLUM_SAVEAXlE];//���������л���
// JZG_EXT     VehicleInfForLoop_Struct 	Hideg_asVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//���س�ͨ�������д洢������Ϣ�ṹ��
// JZG_EXT     VehicleInfForLoop_Struct 	Hideg_asReVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//���س�ͨ�������д洢������Ϣ�ṹ��
// JZG_EXT			Colum_Struct				Forward_HideZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//�����л���
// JZG_EXT			Colum_Struct				Reverse_HideZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//�����л���
// JZG_EXT			int16 							g_HideWonderZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //��������խ��
// JZG_EXT			int16 							g_HideWonder2ZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //������2��խ��
JZG_EXT     uint8								g_au8WaitingPostVehFlg[10];
JZG_EXT     uint32              g_au32LoopCaTime[10];//����
JZG_EXT     uint32              g_au32LoopCaTimeUp[10];//�Ϻ���
JZG_EXT     uint32              g_au32LoopCaTimeBack[10];//��ǰ��
JZG_EXT     uint32              g_au32LoopCaTimeUpBack[10];//shangǰ��
JZG_EXT		uint32              g_au32FilterTime[2*CHL_MAXNUM][4];//խ������ʱ���¼
JZG_EXT     uint32              g_au32HideLineLastTime[10];
JZG_EXT     uint32              g_au32ZHLineLastTime[10];
JZG_EXT     uint8               g_au8LoopStatePre[16];	//ǰʱ����Ȧ״̬ 0:δ���� 1:����
JZG_EXT     uint8               g_au8LoopStatePreOnlyCheck[16];//��Ȧ״̬����¼ǰʱ��״̬
JZG_EXT     int16               g_an16YaFengArea[YAFENGAREASIZE];//ѹ��խ���洢
JZG_EXT     uint8               g_u8YaFengAreaIndex;//��һ��ѹ��խ���洢λ��
JZG_EXT     uint8               g_au8ZTNumRecord[2*CHL_MAXNUM][4]; //һ��ģʽ������������Ǵ�������
//JZG_EXT     uint32              g_u32AdjoiningZTTimeThr;//ƥ��խ������ֵ
JZG_EXT     uint32              g_u32WheelWghtCoin;//�����غͶ�
JZG_EXT     uint32				g_OddColumDelyLth;//��ż�о����
JZG_EXT     uint32				g_SymmetricalDiffTick;//���ζԳƵ�����
JZG_EXT     uint32				g_ThrAlgCompenEnble;//ѹ�����㷨����ʹ��
JZG_EXT     uint32				g_ZTWaveOverLap;  //�����غ϶�
JZG_EXT     uint32				g_ZTSecondWaveOverLap;  //���ζ����غ϶�
//JZG_EXT     uint32              g_u32ForcePostAxleThr;//�׳������е�ǿ��������ֵ
JZG_EXT     uint32              g_u32MakeAxleForbeforeThr;//ͬ��խ����ֵ
JZG_EXT     uint32              g_u32YFBelongThr; //ѹ��խ�������ж�
// JZG_EXT     uint32              g_u32Sub1UnUThr; //�쳣1������ֻ��1��ĩ��խ��
// JZG_EXT     uint32              g_u32Sub2UnUThr; //�쳣2������û��ĩ��խ��
JZG_EXT     uint32              g_u32LaserIOUseFlag;// ʹ�ü���ģ��IO��־, 0:��ʹ��, 1:ʹ��, 100:ʹ��ת��ϵͳ(�����жϼ����Ƿ����)(liumingxin)(20170705)
//JZG_EXT     uint32              g_u32OneRowModThr;//һ��խ��ģʽƥ����ֵ
JZG_EXT     uint32              g_u32OneRowFindMatchThr;//һ��խ��ģʽ����ѡһ����ֵ
JZG_EXT     uint32              g_u32XieZouEnable;//б���ж�ʹ��
JZG_EXT     uint32              g_u32U3FCapEnable;//U3��ץ��ʹ��, 0:U2, 1:U3
JZG_EXT     uint32              g_FristAxleDnRange;//��������������ֵ
JZG_EXT     uint32              g_u32PostVehNoLoop;//����Ȧʱ�������ϳ�������ʱ
//JZG_EXT     uint32              g_u32ModChangeTime;//�߼�ת��ʱ��
JZG_EXT     uint32              g_u32AutoResetTime;//�Զ�����ʱ��
JZG_EXT     uint8				g_u8RWNVFlag;//��д����ı�ʶ
JZG_EXT     uint32				g_YiZiLayouTime;//һ�ֲ�����ʱʱ��
JZG_EXT     uint32				g_JointWghtDelyTime; //���Ϲ�����ʱʱ��
JZG_EXT     uint32				g_auxiliaryK;  //������ϵ��


/**** �ֹ켣��ؿ����ò���(������λ����ֵ����) ****/
JZG_EXT     uint8              g_u8PosScoreC;                //λ�ô��ϵ��,ȡֵ:0-255		    					[0]  default:30
JZG_EXT     uint8              g_u8SpdScoreC;                //�ٶȴ��ϵ��,ȡֵ:0-255		    					[1]  default:10
JZG_EXT     uint8              g_u8RadScoreC;                //���ȴ��ϵ��,ȡֵ:0-255		  	  					[2]  default:10
JZG_EXT     uint8              g_u8PosScoreWeight;           //λ�ô��Ȩֵ,ȡֵ:0-10(ȡ����) 			  			[3]  default:4
JZG_EXT     uint8              g_u8SpdScoreWeight;           //�ٶȴ��Ȩֵ,ȡֵ:0-10(ȡ����) 						[4]  default:3
JZG_EXT     uint8              g_u8RadScoreWeight;           //���ȴ��Ȩֵ,ȡֵ:0-10(ȡ����) 						[5]  default:3
JZG_EXT     uint8              g_u8YaFengDiffXPosMax;        //ѹ��խ�����λ�ñ仯,��λcm,ȡֵ:0-80  				[6]  default:60
JZG_EXT     uint8              g_u8XieXingDiffXPosMin;       //б����Сλ�ñ仯,��λcm,ȡֵ:60-255      			[7]  default:60
JZG_EXT     uint16             g_u16TraceSpdChangeMax;       //�켣�ٶȱ仯���ֵ,��λcm/s,ȡֵ:0-555(0-20km/h)     [8]  default:416(15km/h)
JZG_EXT     uint8              g_u8SgOrDbleWhelPoint;        //��˫̥������ֵ,ȡֵ:0-8                     			[9]  default:2(<=2Ϊ��̥)
JZG_EXT     uint8              g_u8BuildTraceScoreMin;       //��С�����켣����,ȡֵ��0-50     			       		[10] default:20
JZG_EXT     uint16			   g_u16TraceDelTime;            //�켣��ʱɾ��ʱ��,��λs                               [11] default:60
JZG_EXT     uint16  		   g_u16RunAndWalkTime;          //����ͣͣʱ��,��λs                                   [12] default:60
JZG_EXT     uint8  		       g_u8RunAndWalkEnable;         //����ͣͣʹ��,0:�ر�,1:��                           [13] default:1
JZG_EXT     uint16			   g_u16EvenColumDelyLth;        //ż������ʱ����,��λcm                                [14] default:20
JZG_EXT     uint8              g_u8TwoBlankUnion;            //0:�����ֳ���,1:�������ͨ�����۳���                [15] default:1
JZG_EXT     uint8              g_u8MinAxleScore;             //����ʱ��С�������,�����������ͬһ����ķ�����С�ڸ÷�����ǿ�Ʋ�����,ȡֵ0-50 [16] default:20
JZG_EXT     uint8              g_u8AxleMultiScore;           //����ʱ��������ۺϵ÷���ֵ,ȡֵ0-100                 [17] default:70
JZG_EXT     uint8              g_u8AxleWgtDiffRatio;         //���ز�ı�ֵ��ֵ,ȡֵ0-100,                          [18] default:20
/**** end ****/

/**** ץ����ؿ����ò���(������λ����ֵ����) ****/
JZG_EXT     uint8              g_u8SecondCapFlag;            //0:���Ʋ����ж���ץ��, 1:���ƽ��ж���ץ��             [21]  default:0
/**** end ****/

/**** ������ؿ����ò���(������λ����ֵ����) ****/
JZG_EXT     uint8              g_u8ReverseDigitZTPos;    //0:������,1:����; ��������խ����λ����Ϣ(�е��ֳ���������խ������ʱ����)    [22]  default:0
JZG_EXT     uint8              g_u8A2BFlag;              //0:���ƶ�,1:�ƶ�;������խ��A����Ϣ�洢��B��,�����A�е���Ϣ(����ֳ������������ֻ�е�������խ�������)    [23]  default:0
/**** end ****/




JZG_EXT     uint8               g_au8ReFrontLoopFlag[CHL_MAXNUM];	//�����ֳ���� 0:��Ч 1:�зֳ�



JZG_EXT     uint8               g_au8ReZTEnPerLie[2*CHL_MAXNUM]; //��ǰխ�������߼��Ƿ����� 0���� 1��
JZG_EXT     uint8               g_au8ZTEnPerLie[2*CHL_MAXNUM];//��ǰխ�������߼��Ƿ�����


JZG_EXT     uint8               g_u8BiaoDEN;//�궨ʹ��
JZG_EXT     uint8               g_u8BiaoDLane;//�궨����

JZG_EXT     uint8               g_u8SDVehBuff[20][512];	 //SD���洢
JZG_EXT     uint8               g_u8SDBuffIndex;	  //  SD���洢ָ��

JZG_EXT     uint8               g_u8SendWave;
JZG_EXT     uint8               g_u8DetVeh;

JZG_EXT     Stu_loop_info       g_asLoopBoard[10];
JZG_EXT     uint8               g_u8IOStateForBoard[16];//��Ȧ��IO��¼
JZG_EXT     uint8               g_u8NumUseLoopBoard;

JZG_EXT     uint8               g_u8DevErr;//�豸���� 0 �� 1 ��Ȧ���� 2 ������� 3 ������
JZG_EXT     int8                g_n8ZTRang[20];//ÿ��խ��������� -1 �޹��� ���� ��ͬ��Ź���һ��
JZG_EXT     uint32              g_u32JGSWTime[10];//������βʱʱ��
JZG_EXT     uint8               g_u8JGSWFlag[10];//������β��� 0 �ó���û��β 1 �ó�����β
JZG_EXT     uint8               g_u8JGSWVehID[10];//������β�ĳ�ID
JZG_EXT     uint8               g_u8NoMatchCnt;//��ƥ�����
JZG_EXT     uint8               g_u8FirstUse;//�����һ��ʹ�ñ�� 1��ʾ��һ��ʹ�� 0����
JZG_EXT     uint8               g_u8IfRunSta;//�Ƿ�ʼ����״̬��ѯ��
JZG_EXT     uint8               g_u8RunZTSta[64];//�������״̬����¼խ������ 0���� 1����
JZG_EXT     uint8               g_u8ZTStaSendFlag;//�Ƿ�խ��״̬�ϴ� 0�� 1��
JZG_EXT     uint8               g_u8RunLoopSta[16];//�������״̬����¼��Ȧ���� 0 ��ʼ 1���� 2�ɴ�����δ����
JZG_EXT     uint8               g_u8CpBuf[17];


//end

//////
#define ZhongXiaoKe          1
#define XiaoHuo              2
#define DaKe                 3
#define ZhongHuo             4
#define DaHuo                5
#define TeDaHuo              6
#define JiZhuangXiang        7
#define TuoLaJi              8
#define Moto               	 9
#define ARRAYNUM             6
JZG_EXT     LANELOOP_INFO       g_asLoopPerLaneInfo[CHL_MAXNUM];
JZG_EXT     LANE_TRAFFIC_DATA	g_asTrafficData[CHL_MAXNUM];
JZG_EXT     LANE_AXLE_DATA		g_asAxleLoadData[CHL_MAXNUM];
JZG_EXT		fp64				jingdu;			// ����
JZG_EXT		fp64				weidu;			// γ��
JZG_EXT		uint8				LOOP_BD_lane;	 //������
JZG_EXT     uint16              g_u16SmallandMid;//С�г����
//JZG_EXT     uint32              g_u32LoopBaseSmallKH;
//JZG_EXT     uint32              g_u32LoopBaseMidKH;
JZG_EXT     uint32              g_u32FollowVehThr;
JZG_EXT     uint16              g_u16TotalMinutesNew;
JZG_EXT     uint16              g_u16TotalMinutesOld;
JZG_EXT     uint8               g_u8PeriodTOSave;
JZG_EXT     uint16              g_u16SaveTimeNum;
JZG_EXT     uint8               g_u8JDInitFlag;
JZG_EXT	    sndFrame_Struct	  	g_cJDSendVehData[ARRAYNUM];
JZG_EXT     uint8               g_u8SendVehDataInd;
JZG_EXT     CJDSysParam			g_cJDSysParam;
JZG_EXT     uint8               g_au8JDUartSendBuf[512];
JZG_EXT     uint16				g_u16JDUartLen;
JZG_EXT     uint8               g_au8JDSDSaveBuf[512];
JZG_EXT     uint16				g_u16JDSDSaveLen;
JZG_EXT     uint32              g_u32JDSDSaveInd;
JZG_EXT     uint32              g_u32JDSDSaveIndSuc;
JZG_EXT     uint8               g_u8IsFirstSDSave;
JZG_EXT     uint8               g_u8Is39FirstSave;
JZG_EXT     uint8               g_u8Is61FirstSave;
JZG_EXT     uint8               g_u8Is62FirstSave;
JZG_EXT     uint8               g_u8Is68FirstSave;
JZG_EXT     uint32              g_u32JDInfo_Addr_Begin;	   //������Ϣ��ʼλ��
JZG_EXT     uint32              g_u32JDInfo_Addr_End;
JZG_EXT     uint8               g_u8JDNetFirstUse;
JZG_EXT     uint8               g_u8JD02OrderRev;
JZG_EXT     uint8               g_u8JD08OrderRev;
JZG_EXT     uint8               g_u8JDSendEna;
JZG_EXT     uint8               g_u8RecOrderCnt;
JZG_EXT     uint16              g_u16RecOrd0ANum;
JZG_EXT     uint32              g_u32RecOrd39Num;
JZG_EXT     uint16              g_u16RecOrd61Num;
JZG_EXT     uint16              g_u16RecOrd62Num;
JZG_EXT     uint16              g_u16RecOrd68Num;
JZG_EXT     uint8               g_u8JD0AOrderRev;
JZG_EXT     uint8               g_u8JD39OrderRev;
JZG_EXT     uint8               g_u8JD6FOrderRev;
JZG_EXT     uint32              g_u32Resend01SDBgNum;
JZG_EXT     uint32              g_u32Resend01SDEndNum;
//JZG_EXT     uint8               g_u8BegTimeRead;
JZG_EXT     sndFrame_Struct     g_u8SendBufDeg[10];
JZG_EXT     uint8               g_u8DegIndex;

JZG_EXT		uint32              g_u32JD39BaseAdr;
JZG_EXT		uint32              g_u32JD61BaseAdr;
JZG_EXT		uint32              g_u32JD62BaseAdr;
JZG_EXT		uint32              g_u32JD68BaseAdr;
JZG_EXT     uint32              g_u32JD39SaveSuccInd;
JZG_EXT     uint32              g_u32JD39SendSuccInd;
JZG_EXT     uint32              g_u32JD61SaveSuccInd;
JZG_EXT     uint32              g_u32JD61SendSuccInd;
JZG_EXT     uint32              g_u32JD62SaveSuccInd;
JZG_EXT     uint32              g_u32JD62SendSuccInd;
JZG_EXT     uint32              g_u32JD68SaveSuccInd;
JZG_EXT     uint32              g_u32JD68SendSuccInd;
JZG_EXT     uint32               g_u32Resend39;
JZG_EXT     uint8               g_u8Resend61Bg;
JZG_EXT     uint8               g_u8Resend61Ed;
JZG_EXT     uint8               g_u8Resend62Bg;
JZG_EXT     uint8               g_u8Resend62Ed;
JZG_EXT     uint8               g_u8Resend68Bg;
JZG_EXT     uint8               g_u8Resend68Ed;
JZG_EXT     SystemTime	        l_sDevCurTime;
JZG_EXT     uint8               g_u839UnRecCnt;
JZG_EXT     uint32              g_u32VehCntForTest;
JZG_EXT     uint8               g_u8LaserPos[300][4];
//JZG_EXT     uint8               g_u8TestFlag;
JZG_EXT      uint8              g_intotimer_cnt;
JZG_EXT      uint16              g_u16DigitalZTNum;
//////
/*
*********************************************************************************************************
*                                        F4���Դ������
*********************************************************************************************************
*/
//#if  SHOWVEHPASSDEBUG > 0
//	#define DBG_CODELEN		 VEHICLE_BUFLEN	 //��ֵ��������Ķ�
//	JZG_EXT DbugCode_Struct		g_BugCode[DBG_CODELEN];		
//	#define	ADDF4CODE(VID,CODE)	( (g_BugCode[VID].DbugCode[g_BugCode[VID].u8NextPos++]) = ((VID)<DBG_CODELEN) ? (CODE) : '*' )
//	#define	RESETF4CODE(VID)	( memset( &g_BugCode[VID], 0, sizeof(DbugCode_Struct) ) )	
//#else
//	#define	ADDF4CODE(VID,CODE)	 
//	#define	RESETF4CODE(VID)
//#endif
/*D0λ������������״̬��
 *D1λ�����������״̬��
 *D2λ�����Ͷ���״̬,

**˵����	λ��1��ʾ�������������Ҫ������пռ�
*/
JZG_EXT	uint16				g_ErrMsgState;	

/*
*********************************************************************************************************
*                                        ͨ�����ݱ���
*********************************************************************************************************
*/
#define	U5DATALEN		32
#define U5ONEDATALEN	512
JZG_EXT	uint8	g_u8UART5RcvBuf[U5DATALEN][U5ONEDATALEN];			//����5���ջ���
JZG_EXT	uint16	g_u16UART5SavIndex;							//����5����λ��
JZG_EXT	uint16	g_u16UART5ReadIndex;						//����5��ȡλ��

#define	NETDATALEN		32
#define	NETDATA_BUFMASK	0x1F
#define NETONEDATALEN	1460		
//���ڽ��յ�AD�ź�
//JZG_EXT	NetFrame		g_anfSPIAD[256];		//����֡
JZG_EXT	uint8			g_anfSPIAD[NETDATALEN][NETONEDATALEN];		//����֡	�����Ϊ300����Э��ȷ��				  20121108
JZG_EXT	uint8			g_u8NetADIn;			//���ڻ���д��λ��
JZG_EXT	uint8			g_u8NetADOut;			//���ڻ������λ��	
JZG_EXT	uint16			g_u16NetADcount;		//���ڻ����еĸ��� 

//JZG_EXT	uint8			g_u8CaptureIndex;		//��¼ץ��ָ��λ�ñ�� 

JZG_EXT	uint8			g_u8NetStatusErr;		//�������״̬

//������Ϣ
JZG_EXT	NetInfo			g_IPSetLocal;

//#if	SENDWAVEBYNET > 0
//JZG_EXT	uint8			g_au8NetSendBuf[SNDFRAMESIZE];					//�������ݰ�����
//JZG_EXT	uint16			g_u16NetBufIndex;						//

//#endif


//����
//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����
//JZG_EXT int32		g_an32WeightBuf[CHANNELNUM][POINTRATE];		//����1s������ֵBuf
//JZG_EXT	volatile	uint16	g_u16WeightBufSavIndex;				//����1s������ֵIndex
//JZG_EXT	volatile	uint16	g_u16WeightBufReadIndex;			//����1s������ֵIndex
//#else		//δʹ�ܷ��Ͳ���
//JZG_EXT int32		g_an32WeightAD[CHANNELNUM];				    //���ڼ��㼴ʹ�������ж��Ƿ���Ҫ����
//#endif	

//JZG_EXT	int32  			g_an32SPIADBuf[CHANNELNUM][256];		     //��SPI�л�ȡ��ADֵ		delete 20121108
//JZG_EXT	uint8			g_u8ADcount;								 //SPI�����еĸ���
//JZG_EXT	uint8			g_u8ADIn;									 //SPI����д��λ��
//JZG_EXT	uint8			g_u8ADOut;									 //SPI�������λ��

JZG_EXT	uint8			g_au8AllNetSendBuf[SENDALLSIZE];					//�������ݰ�����
JZG_EXT	uint8			g_au8JDNetSendBuf[SENDALLSIZE];					//�������ݰ�����
JZG_EXT	sndFrame_Struct	  	g_sndNetBuf[VEHICLE_BUFLEN];						//�������ݻ���			wujinlong
//JZG_EXT uint8			g_u8CurSendNetBufIndex;		  				//��ǰ���������³������  δ��ʼ��

#define TEMP_SIZE	32	
JZG_EXT	sndFrame_Struct	  	g_sndNetOrderBuf[TEMP_SIZE];						//����ָ���			20121107
JZG_EXT uint8			g_u8CurSendNetOrderIndex;		  				//��ǰ����������ָ�����  δ��ʼ��

JZG_EXT	sndFrame_Struct	  	g_cSDSendVehData;

#define NETDEBUG_SIZE  256
JZG_EXT	sndFrame_Struct   g_sndNetDebug[NETDEBUG_SIZE];
JZG_EXT uint8			  g_u8CurNetDebugIndex;

//JZG_EXT uint8			g_u8CurVehNum;		  							//��ǰ�����к��г�����
//JZG_EXT uint8			g_u8IndexLast;		  							//��ǰ���������³������
//JZG_EXT uint8			g_u8IndexFirst;		  							//��ǰ���������糵�����

/*
*********************************************************************************************************
*                                        �߼��������ݱ���
* wujinlong
* 20121120
*********************************************************************************************************
*/
//JZG_EXT		ROUTETABLE		a_CRouteTable1,a_CRouteTable2;
//JZG_EXT		WONDERBUFFER	a_CWonderBuffer1,a_CWonderBuffer2;
//JZG_EXT		LASERLATEBUFFER a_LaserLateBuffer1,a_LaserLateBuffer2;//�����޸�
//
//JZG_EXT uint32			g_u32TimeDelayUpdataLate; //F_UpdataLateLaserZtToRouteTable
//JZG_EXT uint32			g_u32TimeDelayUpdataWbZt; //F_UpdataWbZtRecToRoute
//JZG_EXT uint32			g_u32TimeDelayWonderProcess; //F_WonderProcess	
//JZG_EXT uint32			g_u32TimeDelayDropVehicle; //F_DropVehicle
//JZG_EXT uint32			g_u32TimeDelayForceClear; //F_ForceClear
//
//JZG_EXT uint16			g_u16LaneWidth;		  // �������

/*
*********************************************************************************************************
*                                        ���Է������ݱ���
*********************************************************************************************************
*/
//JZG_EXT uint32 ZtIRQCounter[1000];
//JZG_EXT uint32 ZtIRQCounter2[1000];
//JZG_EXT uint32 ZtIRQSum;
//JZG_EXT uint32 ZtLogicSUM;
//JZG_EXT uint32 csTaskInputRec[1000];

//#define SIM_NET	  0
//#if SIM_NET
//	#define SIM_LASER 0xB6
//	JZG_EXT uint8 sim_net_laser;	
//#endif

//#define SIM_WAVE	1
//#if SIM_WAVE
//JZG_EXT uint32 JTCount;
//JZG_EXT uint16 Sim_frame_id_old;
//#endif



// ϵͳ��ʼ�� 20130805
JZG_EXT int8 		g_u8StartOver;

// �յ�����ץ��IO��ͳ�Ƶ���
//#if DEBUG_CAPTURE_IO_IN_STAT_EN > 0
//JZG_EXT uint32 		g_au16CaptIOInCnt[LASTERIOCNT];				// ��¼ÿ������IO IN�Ĵ���
//#endif

// ֪ͨ���ץ��IO��ͳ�Ƶ���
//#if	DEBUG_CAPTURE_IO_OUT_STAT_EN > 0
//JZG_EXT uint32 		g_au16CaptIOOutCnt[LASTERIOCNT];				// ��¼ÿ������IO OUT�Ĵ���
//#endif

#if DEBUGINFO_TASK_SP_EN > 0
JZG_EXT	CComBuf		g_cDebugInfo[COM_NUM];
JZG_EXT uint16		g_u16CurDebugInd;		  				// ��ǰ����������ָ�����  
#endif


JZG_EXT	CComBuf		g_cRecvLaserData[RECVLASERDATA_NUM];
JZG_EXT uint16		g_u16RecvLaserInd;					  	// ���ռ�����������



//#if NOVEH_CLR_EN > 0
//JZG_EXT uint16		g_u16NoVehClrCnt;		  				// û�г���ռ���
//#endif


#if    TASKTIMEENABLE
#define			RUNTIMECOUNT(LASTTIME)	((UL)T1TC-LASTTIME)
JZG_EXT TASKRUNTIME TaskRunTimeStatistics;	
#endif

JZG_EXT	uint8		g_au8ReCon[NET_PORT_NUM];		// ��������


JZG_EXT	uint16		g_au16HeartTimeCnt[NET_PORT_NUM];		// ��������ʱ�����
JZG_EXT	uint8		g_au8HeartRecvReplyFlag[NET_PORT_NUM];	// �յ������ظ���־
JZG_EXT	uint8		g_au8HeartRecvBeginCntFlag[NET_PORT_NUM];	// ������ջظ���ʼ��־
JZG_EXT	uint16		g_au16HeartRecvTimeCnt[NET_PORT_NUM];	// �������ʧ��ʱ�����
JZG_EXT	uint8		g_au8HeartRecvFailCnt[NET_PORT_NUM];	// ��������ʧ�ܼ���
JZG_EXT	CHeartBuf  	g_cHeartBuf;							// ��������


//#if TEST_CAPTURETIME_EN > 0
//JZG_EXT	uint16		g_u16TestCaptime;						// ����ץ��ʱ��
//JZG_EXT	uint8		g_u8TestSPFlag;							// ����ץ�Ĵ��ڱ�־���յ�������֡��1������Ϊ0
//JZG_EXT	uint8		g_u8TestIOFlag;							// ����ץ��IO��־���յ�IO��1������Ϊ0
//JZG_EXT	uint32		g_u32T1TCBeginTime;						// ��ʼʱ��
//#endif


//#define		EXTDATELEN		40
//JZG_EXT	uint8		g_au8ExtDataBuf[EXTDATELEN];			// ��չָ��buf

//#if TEST_WAVE_EN > 0
//#define		LASERENDNUM		4								// ���������ʱ������0.5sһ��
//JZG_EXT	uint8		g_u8LaserEndFlag;				   		// ���������־
//JZG_EXT	uint8		g_u8LaserEndCnt;						// ������ʱ����
//
//#endif


//JZG_EXT	int	g_n32ZTRightPos[QUATTITY_WbZt_COLOUM];


//#if LOGIC_YAFENG_PROCESS_EN > 0
//#define	YAFENGSTACKLEN	(6*6*2)								// 6��*6��*2Ԥ����������ǵ�������������Ҫ*2
//JZG_EXT	CYaFengStack	g_acYaFengZTStack[YAFENGSTACKLEN];
//#endif	

//JZG_EXT	uint8	g_u8IsLaserorCoilFlag;						// ʹ�ü��⻹����Ȧ��1�����⣬0����Ȧ����ʼ��Ϊ1												



// SD
JZG_EXT	uint8	g_u8IsConnectFlag;							// ���������������1����������0���Ӳ�������Ĭ����1
JZG_EXT	uint32	g_u32NetConCnt;								// ��������ʱ�����	
//
#define	SD_SEND_BUF_NUM		32
JZG_EXT	sndFrame_Struct	  	g_acSDSendVehInfo[SD_SEND_BUF_NUM];	 // ��ȡSD�����ݣ������ڴˣ������͸�ͨ������
JZG_EXT	uint16	g_u16SDSendBufIX;								 // ����
//
JZG_EXT uint8  		g_u8SDRdBuf[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDRdBuf2[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDWrBuf[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDWrBuf2[SD_SECTOR_LEN];


JZG_EXT	uint8		g_u8VehSendFlag;
JZG_EXT	uint8		g_u8DebugInfoSendFlag;
JZG_EXT uint32		g_u32BeginIx ;		   	// ��ʼ���
JZG_EXT uint32		g_u32EndIx;		  		// �������


//#if LASER_UNNORMAL_EN > 0
//JZG_EXT uint8  		g_u8LasUnnFlag;				// �����쳣��־
//JZG_EXT uint32  	g_u32LasUnnTimeCnt;			// �����쳣ʱ�����,30minȡ��
//#define	LAS_UNN_VEH_NUM		10
//JZG_EXT CVehSimInfo	g_cVehSimInfo[LAS_UNN_VEH_NUM];		// �洢������Ϣ

//#endif



// -begin--
// ������� 20140819
//JZG_EXT uint16	g_au16DaoRow23IXRec[8];	// ������2��3��������¼
//JZG_EXT uint16	g_au16TimeThresh;	// �½����͵�2��3��ZT�����ж���ֵ


	// --end--


// test
//JZG_EXT	uint8		g_u8TestCnt;
typedef		SystemTime		RTCDateTime;

#endif	  // end #ifndef	__JZGLOBAL_H

/*
*********************************************************************************************************
*                                        ��Ļ��ʾ���ݱ���
* wangfeng
* 20150715
*********************************************************************************************************
*/

JZG_EXT		uint8 		Locate;
JZG_EXT     uint8       g_u8ChlForShow;
JZG_EXT     uint8       g_u8StopChange;
#define		Logo_Page	0
#define 	Info_Page	1
#define		Func_Page	2
#define		F2_Choose_Chedao_Page 		3
#define		F3_Choose_Chedao_Page 		4
#define		Choose_Tongdao_Page		    5
#define 	Choose_Tongdao_Chedao_Page	6
#define 	Info_Chedao1ZY_Page				7
#define 	Info_Chedao2ZY_Page				8
#define 	Info_Chedao3ZY_Page				9
#define 	Info_Chedao4ZY_Page				10
#define     ProgramVersion              11
#define     Change_Chedao1ZY_Page		12
#define     Change_Chedao2ZY_Page		13
#define     Change_Chedao3ZY_Page		14
#define     Change_Chedao4ZY_Page		15
#define     Change_TongDao1ZY_Page			16
#define     Change_TongDao2ZY_Page			17
#define     Change_TongDao3ZY_Page			18
#define     Change_TongDao4ZY_Page			19
#define     Change_TongDao5ZY_Page			20
#define     Change_TongDao6ZY_Page			21
#define 	Info_Chedao1_TongDaoZY_Page				22
#define 	Info_Chedao2_TongDaoZY_Page				23
#define 	Info_Chedao3_TongDaoZY_Page				24
#define 	Info_Chedao4_TongDaoZY_Page				25
#define 	Chedao_Neima_Page				26
#define     Chezhong_Page                   27


