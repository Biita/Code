/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			DEFConfig.h
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


#ifndef	__DEFCONFIG_H
#define	__DEFCONFIG_H


/*
*********************************************************************************************************
*                                         �汾��
*********************************************************************************************************
*/
/*
	* 1700Ϊ���ֳ��²������߼���ʼ�汾��������1600��ʼ�ľɲ���Э��ץ�ļ�1500��ʼ�ľɲ��ַ�Э��ץ��
	* 1710Ϊ����խ���߼�,��ʼ�汾Ϊ20170810������1700�� 20170809�汾
	* 1720Ϊ����PK����汾
	* 1800Ϊ�������ֺ�һ���к�����ʼ�汾(���Ž�����+2��һ�����ذ��),�������ذ��δһ���к�+һ������
	*
*/
#define		YBVERSION	30							// �Ǳ�汾��
#define		PROGRAMVERSION		"WJ-WIM-1800"	   	// ����汾�ţ�˵������-��̬����-�汾��(4)
#define		PROGVERSION_POS_0	1					
#define		PROGVERSION_POS_1	5
#define		PROGVERSION_POS_2	0
#define		PROGVERSION_POS_3	2
#define		PROGMODIFYDATE		"20181225"	  		// �����޸�����

/*
*********************************************************************************************************
*                                         ����������ò�����ʹ�ܵ�
*********************************************************************************************************
*/
//#define		DEBUG_NT						1			//����״̬:1 ���ԣ�	0 �ǵ���  
//#define		ISDEBUG							(DEBUG_NT > 0)	//�ǵ���  		  	
//#define		NOTDEBUG						(DEBUG_NT == 0)	//���ǵ���

// ����ʹ��
//#define		SHOWVEHPASSDEBUG 				1			// ��ʾ��̬����״̬ F4����	  
//#define 	PROC_DOG_EN						1			// �������Ź�����
//#define		CAMERACAPTURE					1		 	// ץ��
//#define		SENDWAVEENABLE					1		//���Ͳ���ʹ��	���رգ����޷����Ͳ��Σ���ʹ������Ӧѡ�										//�ǵ���״̬ʱ����ֵ�Զ���Ϊ0
//#define		LOGICENABLE						1		// FPGA�ж��У��������߼���խ��ʹ�ܣ����Ϊ1������Task1��խ����Ϊ0������ wujinlong 20130114


/***********
   ʹ��
***********/
//#define DEBUG_PRINT_EN						0		// ���ڴ�ӡ��Ϣ������غ꣬����
#define	DEBUGINFO_TASK_SP_EN				1		// ������Ϣ���񴮿����ʹ��
#define	DEBUGINFO_LASER_EN					0		// ������Ϣ������������ʹ��
//#define DEBUGINFO_LOGIC_LASER_EN			0		// ������Ϣ���߼������������Ϣʹ��
//#define	DEBUG_SP_OP_WARTING_EN				1		// ������Ϣ�������WaitingArea
//
//
//
//#define	COIL_LOGIC_EN						1		// ��Ȧ�߼�ʹ��
//
//#define	NOVEH_CLR_EN						0		// �޳����ʹ��
// ���ƣ���ʱ��10s���������������һ��ʱ���ڣ�3s��û���յ��ظ����Ҵﵽ3�Σ�������
// ����յ��ظ�����ظ�ʱ�����㣬��־���㣬ʧ�ܴ�������
//#define	NET_HEARTBEAT_EN					0		// ��������ʹ�ܣ�20130904
// 
//#define	NET_KEEPALIVE_EN					1		// ���籣�����ʹ�ܣ�20130910
//
#define TASKTIMEENABLE 						1		// ͳ�Ƽ�ʱʹ�ܣ�����


//#define PRTCL_NO_WAVE_EN					1		// Э����0x81��������



// ����ʹ��Ϊ�ֳ����ӣ����ܴ��ڽ��ڷ���1����Ӷ�����0��δ��ӵ�����
//#define	LOGIC_FILTER_SAMEZT					1		// �˳�һ��ʱ����ͬһխ�����ж� 20131018
//#define	LOGIC_LASER_POS						1		// ����ʱ�����ӳ���λ�ã��ǿ����ǿ��� 20131018

//
//#define	VEH_ADDAXLE_EN						1		// ����ʹ��												 20131019
//#define	VEH_FILTERWEIGHT0_EN				1		// �˳�����Ϊ0����

//#define	LOGIC_FILTER_SAMEZT_INAXLE_EN		1		// ����ʱ���˳���ͬ��խ��								 20131020
//#define	LOGIC_FILTER_ZTDNIME_OVER_EN		0		// ����ʱ���˳��³�ʱ�䳬����Χ��խ��
													// �ù������㷨����ӣ��߼��п�ɾ����20131026
					 

//#define	LOGIC_FILTER_YAFENG_EN				0	    // �˳�С�ڹ�ģʽʹ��			 20131025
//#define LOGIC_COIL_NOTUSING_YAFENG			1		// ��Ȧģʽ�в�ʹ��С�ڹ�


//#define LOGIC_NOT_WONDERPROCESS_EN			0		// �߼��в�ʹ��WonderProcess����1����ʹ�ã�0��ʹ��   20131028
//#define	LOGIC_YAFENG_PROCESS_EN				1		// �µ�ѹ�촦�����

//#define	LOGIC_NOT_LASER_EN					0		// �߼��в�ʹ�ü���ʹ�ܣ�ֱ���ڴ����жϲ��׼��������ź��� 20131107
//#define	VEH_NOT_POST_WT0					0		// ��֡ʱ���������Ϊ0������֡�����ͣ�ֱ�ӷ���

//#define	LOGIC_FTR_LSR_SPR_EN				0		// �߼����˳�����ֳ�ʹ�ܣ�1ʹ�ܣ�0��ʹ�ܣ�20131112

//#define	VEH_ADDAXLE_LIMIT					1		// ����ʱ�������������жϣ�20131212


//#define	LOGIC_POST_AXLE_DELAY				1		// ��ʱ���ᣬ������һ��խ�����³�ʱ��͵�ǰʱ��Ƚϣ�������ֵ����ֱ�����ᣬwujinlong 20140715
//#define	LOGIC_POSTAXLE_DELAYTIME			1000	// ������ʱʱ��




//#define	LASER_UNNORMAL_EN					1		// �����쳣���� wujinlong 20140707
													// ����������쳣��������������ARM���ղ�����������Ϊ�������쳣��תΪ��Ȧģʽ
													// ��Ȧģʽ�£���ʹ��ѹ��խ��������ʱ��ǰһ���Աȣ��ж�������������ʱ���
													// 30min�к������⼤�����������
//#if LASER_UNNORMAL_EN > 0
//#define	LAS_UNN_MATCHTIMR					1000		// ������ʱ�жϣ���ǰʱ��ͳ���ʱ�䳬��1s�����ӵ������׳�ʱ�жϼ���
//#define	LAS_UNN_OVERTIMR					6000		// ������ʱ�жϣ���ǰʱ��ͳ���ʱ�䳬��1s�����ӵ������׳�ʱ�жϼ���
//#define	LAS_UNN_MODELTHRANS_OVERTIME		(30*60*2)	// 30min����ʱ�������У�0.5sһ�Σ���30*60*2��
//#define LAS_UNN_WEIGHT_DOWN					80			// ��������	  70/100
//#define LAS_UNN_WEIGHT_UP					120			// ��������
//#endif


/* SD���洢���� 20131108
 * û�м�⵽��λ������(0x95)����0x81ָ��浽SD��
 * ��⵽��λ������ʱ����SD�����ݷ��ͳ�ȥ   */
#define	SD_DATA_STORAGE_EN					1		// SD�����ݴ洢�ܿ���  
#define	SD_OFFNET_SAVE_EN					1		// �������湦�� ʹ��
#define	SD_DEBUGINFO_EN						1		// ������Ϣ�洢����  ʹ��
#define	SD_WAVE_SAVE_EN						1		// ���δ洢����  ʹ��


/***********
 ���� ʹ��
***********/
// һ������֪ͨ��λ����ʼ�ͽ���
//#define TEST_WAVE_EN 						1		// һ����ʹ�� 20130911

//#define TEST_NOTWAVE_ZTINFO_EN 				0		// �������Σ�����խ����Ϣ��20131021


// ����ʹ��
//#define		TEST_IO_EN			0			// ����ץ��IO
//#define		NET2VEHINFO				0		// ͨ�����ڷ��ͳ�������Ϣ�ͳ���Ϣ	   wujinlong 20130107



/*
*********************************************************************************************************
*                                         ��������
*********************************************************************************************************
*/																																 
#define		SUPERPWD			13801380  	//����Ȩ������

// ϵͳ��ʼ��
#define START_NOT_OVER				0x00
#define START_OVER					0x01 
#define TASK_BEGIN_WAIT_TICKS		(20*OS_TICKS_PER_SEC/1000)		// ����������Ϊ�˷�ֹһֱռ��Ӱ����������Ĵ�����������ʱ




/*
*********************************************************************************************************
*                                         �������������
*********************************************************************************************************
*/
#define		ZHOUSIZE			16			 //���������������������

//#define 	MAX_SPEED_LIMIT		81			//����������ٶ� km/h  
//#define	SPEED_CTL_SIZE		17			//�������ٶȼ���� 5km/h
#define 	MAX_SPEED_LIMIT		100			//����������ٶ� km/h
#define		SPEED_CTL_SIZE		20			//�������ٶȼ���� 5km/h

#define		VSMSTANDARD			10000		// �����ٶ�������׼ֵ������

#define 	ENABLE_CONFIG		1		 	// ʹ�����ù��������
#define		DISABLE_CONFIG		0	

#define 	COM_MODE1			1		 	// ����/����ͨ��ģʽ������ģʽ
#define 	COM_MODE0			0			// ������ģʽ

//����̬
//#define		UI_STATIC			0
//#define		UI_MOTION			1

//#define 	MAXBUFPOINT			2000			//ÿ��խ���洢�ĵ�������ֵ
#define 	MAXBUFPOINT			4000			//ÿ��խ���洢�ĵ�������ֵ 


#define WAVE_BUFSIZE	64
//#define WAVE_BUFMAX		0x1F


// ����
#define 	SNDFRAMESIZE		1440	 	//��������֡�ĳ���
#define		SENDALLSIZE			100000		//���������ܳ���
#define		SNDFRAMEBESTSIZE	900			//��ѷ�������֡���ȣ��˳���Ϊ�������ݳ��ȣ����ǵ��洢SD������֤1֡�洢��2��������
											//������֡ͷ��֡β���ݣ�֡ͷ֡β���ȿ�����20-30�ֽ����ң�20121107

// ����
#define	VEH_AXLE_MIN_WEIGHT			20			 	// ����С������ʵ����20*10Kg
#define	VEH_AXLE_DEF_WT				75				// Ĭ������75*10Kg
#define	VEH_AXLE1_MAX_WT			1000			// ��1�������1000*10Kg��10t
#define	VEH_AXLE1_DEF_WT			150				// ��1Ĭ������150*10Kg��1.5t
//#define	VEH_ADDAXLE_LIMIT_VAL		8000			// ����ʱ�����ӵ�1�������жϣ�����8t�����д���
// �ٶ�
#define	VEH_AXLE_DEF_SPEED			40*10			// Ĭ���ٶ�40*10*0.1Km/h
// ���
#define	VEH_AXLE_DEF_DIST			200				// Ĭ����࣬200cm��2m
#define	VEH_AXLE_DIST_11X			140				// 11X�����

// ʱ��
//#define	LOGIC_SAMEZT_TIMEINTERVAL	50				// ��ͬխ������ʱ�䣬20131018
//
//#define	LOGIC_AXLE_ZTDN_OVERTIME	6000			// ����ʱ���ж�խ��ʱ���
//
//#define	LOGIC_FILTER_YAFENG_TIMEOVER	9000		// ���ѹ��խ��ʱ��		  ����
//#define	LOGIC_MATCH_YAFENG_TIMEOVER		600			// ��ƥ��ѹ��խ��ʱ��
//
//#define	LOGIC_COIL_ON_TIME_MAX			(100*OS_TICKS_PER_SEC/1000)		// ��ʱ100ms


// �ֶ�ֵ
//#define FEN_10						10				// �ֶ�ֵ
//#define FEN_20						20
//#define FEN_50						50
//#define FEN_100						100



// ����ٶ�
#define	VEH_OP_MAX_SPEED			1200			// 120Km/h,1200*0.1Km/h
#define	VEH_OP_DEFINE_SPEED			(VEH_OP_MAX_SPEED*8/10)

//#define	LOGIC_NOTUSINGZT_LEN		6*2				// �߼�����խ��

//#define	LOGIC_POSTAXLE_MAX_CHLNUM	20				// �������խ����

// �����ٶ����������СĬ��ֵ
#define	PARA_VEHTPYE_SPEED_MODIFY_MIN	8000
#define	PARA_VEHTPYE_SPEED_MODIFY_MAX	12000



/*
*********************************************************************************************************
*                                         խ�����
*********************************************************************************************************
*/
#define		CHANNELNUM			64		  			//խ��������
#define   ZERONUM   344                 //8+48*7=344
//#define 	CHANNALROWSUM		8		  			//һ�ŵ�խ����������ʹ��
#define 	CHANNALLINESUM		4 					//������խ��
//#define 	CHANNALLINESUM_LOGIC 3 					//�߼�����

//#define		CHANNELWIDE			175					// խ����ȣ���խ������ 175






/*
*********************************************************************************************************
*                                         �ɼ������
*********************************************************************************************************
*/
#define		POINTRATE			6000				//������

/*
*********************************************************************************************************
*                                         �������
*********************************************************************************************************
*/
#define		LASTERCOUNT			4			//������������
#define		LASTERIOCNT			(LASTERCOUNT<<1)		//������IO�ڵ�����

// ���⴮�ڳ�������
#define	SP_LASER_VEHNUM_MAX			(LASTERCOUNT<<1)
/*
*********************************************************************************************************
*                                         �������
*********************************************************************************************************
*/
// ����������ȼ������������������
//#define	MUTEXCOM1PRIO	 			2				// ����1����������


// �������ȼ������ȼ�0-3 ����	 
#define	TASK_LOGIC_PRIO		 		4				// �߼�����
#define	TASK_TIME_PRIO		 		6				// ʱ������
#define	TASK_ALG_PRIO		 		7				// ���ؼ�������
#define	TASK_VEH_PRIO		 		8				// ��β�׳�����
#define TASK_JDINF_PRIO             9               // ������Ϣ�ϴ�����
#define TASK_UI_PRIO				25				// ��ʾ��������
#define	Task_LASERINTR_PRIO 		10				// ���⽻������

#define	TASK_COMM_PRIO		 		12				// ͨ������
#define	TASK_SD_PRIO 	 			14 				// SD�����������Ƕ������棬��2�������ȼ��൱

#define	TASK_WAVE_PRIO 	 			16				// ����������
#define	TASK_KEY_PRIO		 		18				// �����������ڷ��ֳ���ʹ�ü��̣���ȡ����������
#define TASK_PRI_CHKTASK   			19
#define	TASK_DEBUGINFO_PRIO		 	20				// ������Ϣ�������
//#define TASK_TEST_PRIO	 			22				// ��������ƽʱ�ر�

#define	TASK_START_PRIO	 			26				// �������񣬽�Ϊ����˸�ȹ���

//#define  	ErrAxleMsg			 "�������Ϣ��������!" 
//#define  	ErrSndPCMsg 		 "������Ϣ��������!"
//#define 	ErrCalcVehMsg 		 "����������Ϣ��������!"
//#define 	ErrShowVehMsg 		 "������ʾ��Ϣ��������!"
//#define 	ErrShowAxleMsg 		 "����ʾ��Ϣ��������!"
//���ò����ռ��С 
#define		SETUPPARAMSIZE	 		6756	   
#define 	RESERVED				(10)			//���ò����ռ�Ԥ���ֽ��� 
#define		PROMODPARAMSIZE	 		1024	   
#define 	PROMOD_RESERVED			(270)			//���ò����ռ�Ԥ���ֽ���
#define		JDSYSPARAMSIZE	 		512
#define     JDSYS_RESERVED			366



//����ջ�ռ��С
// �Ȱ�����ǰ���������ͳ������ʹ�ö�ջ���		
#define 	TASK_LOGIC_STACKSIZE		6144		
#define 	TASK_TIME_STACKSIZE			3072
#define 	TASK_ALG_STACKSIZE			4096		
#define 	TASK_VEH_STACKSIZE			3072
#define 	TASK_LASERINTR_STACKSIZE	3072		
#define 	TASK_COMM_STACKSIZE			3072
#define 	TASK_SD_STACKSIZE			3072		
#define 	TASK_WAVE_STACKSIZE			3072
#define 	TASK_KEY_STACKSIZE			3072
#define 	TASK_DEBUGINFO_STACKSIZE	4096
#define 	TASK_TEST_STACKSIZE			3072
#define 	TASK_START_STACKSIZE		3072
#define     TASK_JDINF_STACKSIZE        3072
#define		TASK_UI_STACKSIZE			3072
#define TASK_STK_SIZE_CHKTASK  64
		
#define 	TASKSTACKSIZE				3072		
#define 	TASK6STACKSIZE				4096



/*
*********************************************************************************************************
*                                         �߼����
*********************************************************************************************************
*/
#define	LASER_VEH_MIN_ITV			80					// 80cm��������ĳ�����С���Ҿ���


/*
*********************************************************************************************************
*                                         �㷨���
*********************************************************************************************************
*/
#define		DNNUM				100			//�߼��³��жϣ���������DOWNNUM���㣬��Ϊ�³� //���� 20130516 1000̫�� ��Ϊ200
#define		UPNUM				100			//�߼��ϳ��жϣ���������UPNUM���㣬��Ϊ�³�		//���� 20130516 1000̫�� ��Ϊ200
#define 	WAVEMINSIZE			80			//��С���ο��	
#define 	DEFUPPOINT			7			//�ϳӵ���
#define 	DEFDNPOINT			7			//�³ӵ���
#define		DEFLUPVALUE			40			//�ϳ���ֵ����С
#define		DEFLDNVALUE			40			//�³���ֵ����С



/*
*********************************************************************************************************
*                                         Э��
*********************************************************************************************************
*/
#define 	PROTOL_WJ_OLD		0x00	//�ɸ���Ԥ��Э��
#define 	PROTOL_WJ_NEW		0x01	//����Э��
#define 	PROTOL_FXC			0x02	//���ֳ�ִ��Э��

// ��֡
#define		COMMSENDWAVESTART	0x00		//��֡  
#define		COMMSENDWAVETRANS	0x01		//�м�����֡
#define		COMMSENDWAVEEND		0x02		//β����֡



/*
*********************************************************************************************************
*                                         SD��
*********************************************************************************************************
*/
// ʹ�ܳ���
#define	SD_DATA_STORAGE					0x01
#define	SD_OFFNET						0x02
#define	SD_VEHINFO						0x04
#define	SD_DEBUGINFO					0x08
#define	SD_WAVE							0x10

// ��������
#define	SD_CONTEXT_ZONG					0
#define	SD_CONTEXT_OFFNET_SAVE			1		 // 1-10
#define	SD_CONTEXT_VEHINFO				11		 // 11-20
#define	SD_CONTEXT_SYSLOG				21		 // 21-30
#define	SD_CONTEXT_WAVE					31		 // 31-40
#define SD_CONTEXT_JD                   41       // 41-50
//
#define	SD_TASK_TIMEDLY					2000						// ������ʱ


#define	SD_NET_CONNECT_JUDGE_CNT		(3+1)						// 3�Σ�����ʱ��5s
#define	SD_NET_CON_JUDGE_TIME			(SD_NET_CONNECT_JUDGE_CNT*5/(SD_TASK_TIMEDLY/1000))	 // ʱ������ж�
//
#define	SD_SECTOR_LEN					512							// ��������
#define	SD_NEW_CARD_FLAG_LEN			10			 				// SD���¿�У�����ݳ���
#define	SD_NEW_SECTOR_FLAG_LEN			4			 				// SD��������У�����ݳ���
//
#define	SD_SEC_NUM_FRAM					2							// ÿ֡������
//
#define	SD_ANTISHAKE_CNT				5							// �����������
//
#define	SD_IX_LEN_PER					20							// ÿ����¼����
#define	SD_IX_NUM						25							// �������洢������20�ֽ�һ��


#define	SD_TDC_SIZE						532							// ����ÿ�����ܴ�С532B
#define	SD_TDC2SD_TIME					30							// SD������2sһѭ����5min��300s

/////////////////////////
// �������湦�����
#define	SD_OFFNET_SEC_BGN				m_u32Offnet_Addr_Begin
#define	SD_OFFNET_SEC_DATA_BGN			m_u32Offnet_Data_Begin			
#define	SD_OFFNET_SEC_END				m_u32Offnet_Addr_End
// �������ݵ�ַ
#define	SD_CTX_OFFNET_FLAG				0							// 0-3
#define	SD_CTX_OFFNET_VEHNUM			4							// 4-7
#define	SD_CTX_OFFNET_SEC_BGN			8							// 8-11
#define	SD_CTX_OFFNET_SEC_END			12							// 12-15
#define	SD_CTX_OFFNET_SEC_DATA_BEGIN	14
#define	SD_CTX_OFFNET_WR_SEC			20
#define	SD_CTX_OFFNET_WR_SUB			24
#define	SD_CTX_OFFNET_RD_SEC			26
#define	SD_CTX_OFFNET_RD_SUB			30								
#define	SD_CTX_VEH_NUM					32							// 32-35��������		36- Ԥ��
// �������ݵ�ַ
#define	SD_OFFNET_SEC_FLAG				0							// 0-3
#define	SD_OFFNET_SEC_FRAM_FLAG			4							// 4-7
#define	SD_OFFNET_SEC_FRAM_LEN			8							// 8-9
#define	SD_OFFNET_SEC_FRAM_NUM			10							// 10-11
#define	SD_OFFNET_SEC_VEH_IX			12							// 12-15      16-29 Ԥ��  
#define	SD_OFFNET_SEC_FRAM_DATA			30							// 30- ����
#define	SD_OFFNET_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_OFFNET_SEC_FRAM_DATA)
//
#define	SD_VEH_FRAME_NUM				100							// ÿ�����֡��
#define	SD_VEH_SECTOR_NUM				(SD_VEH_FRAME_NUM*SD_SEC_NUM_FRAM)		// ÿ�����������
#define	SD_VEH_MAX_NUM					((SD_OFFNET_SEC_END-SD_OFFNET_SEC_DATA_BGN+1)/SD_VEH_SECTOR_NUM/SD_IX_NUM*SD_IX_NUM)  // ��֤��������������



////////////////////////////////
// ������Ϣ
#define	SD_VEH_SEC_BGN					m_u32VehInfo_Addr_Begin	
#define	SD_VEH_SEC_DATA_BGN				m_u32VehInfo_Data_Begin		
#define	SD_VEH_SEC_END					m_u32VehInfo_Addr_End
// �������ݵ�ַ
#define	SD_CTX_VEH_FLAG					0							// 0-3
#define	SD_CTX_VEH_SUM					4							// 4-7		
#define	SD_CTX_VEH_SEC_BGN				8							// 8-11
#define	SD_CTX_VEH_SEC_END				12							// 12-15
#define	SD_CTX_VEH_NEXT					16							// 16-19
#define	SD_CTX_VEH_CUR					20							// 20-23      24- Ԥ��
// �������ݵ�ַ
#define	SD_VEH_SEC_FLAG					0							// 0-3
#define	SD_VEH_SEC_VEH_FLAG				4							// 4-7
#define	SD_VEH_SEC_LEN					8							// 8-9      10-29 Ԥ��  
#define	SD_VEH_SEC_DATA					30							// 30- ����
#define	SD_VEH_SEC_DATA_LEN				(SD_SECTOR_LEN-SD_VEH_SEC_DATA)
//
#define	SD_VEH_MAXNUM					(SD_VEH_SEC_END-SD_VEH_SEC_DATA_BGN+1)



////////////////////////////////
// ������Ϣ���湦�����
#define	SD_DBGINFO_SEC_BGN				m_u32Log_Addr_Begin	
#define	SD_DBGINFO_SEC_DATA_BGN			m_u32Log_Data_Begin		
#define	SD_DBGINFO_SEC_END				m_u32Log_Addr_End
// �������ݵ�ַ
#define	SD_CTX_DBGINFO_FLAG				0							// 0-3
#define	SD_CTX_DBGINFO_LOGSUM			4							// 4-7		
#define	SD_CTX_DBGINFO_SEC_BGN			8							// 8-11
#define	SD_CTX_DBGINFO_SEC_END			12							// 12-15
#define	SD_CTX_DBGINFO_NEXTLOG			16							// 16-19
#define	SD_CTX_DBGINFO_CURLOG			20							// 20-23      24- Ԥ��
// �������ݵ�ַ
#define	SD_DBGINFO_SEC_FLAG				0							// 0-3
#define	SD_DBGINFO_SEC_LOG_FLAG			4							// 4-7
#define	SD_DBGINFO_SEC_LOG_LEN			8							// 8-9      10-29 Ԥ��  
#define	SD_DBGINFO_SEC_LOG_DATA			30							// 30- ����
#define	SD_DBGINFO_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_DBGINFO_SEC_LOG_DATA)
//
#define	SD_LOG_MAX_NUM					(SD_DBGINFO_SEC_END-SD_DBGINFO_SEC_DATA_BGN+1)


/////////////////////////
// ���δ洢�������
#define	SD_WAVE_SEC_BGN					m_u32Wave_Addr_Begin
#define	SD_WAVE_SEC_DATA_BGN			m_u32Wave_Data_Begin			
#define	SD_WAVE_SEC_END				    m_u32Wave_Addr_End
// �������ݵ�ַ
#define	SD_CTX_WAVE_FLAG				0							// 0-3
#define	SD_CTX_WAVE_ZTNUM				4							// 4-7
#define	SD_CTX_WAVE_SEC_BGN				8							// 8-11
#define	SD_CTX_WAVE_SEC_END				12							// 12-15
#define	SD_CTX_WAVE_NEXTZT				16							// 16-19
#define	SD_CTX_WAVE_CURZT				20							// 20-23      24- Ԥ��
// �������ݵ�ַ
#define	SD_WAVE_SEC_FLAG				0							// 0-3
#define	SD_WAVE_SEC_ZT_FLAG				4							// 4-7
#define	SD_WAVE_SEC_ZT_LEN				8							// 8-9
#define	SD_WAVE_SEC_ZT_NUM				10							// 10-11
#define	SD_WAVE_SEC_ZT_IX				12							// 12-15      16-29 Ԥ��  
#define	SD_WAVE_SEC_ZT_DATA				30							// 30- ����
#define	SD_WAVE_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_WAVE_SEC_ZT_DATA)
//
#define	SD_WAVE_ZT_MAX_NUM				((SD_WAVE_SEC_END-SD_WAVE_SEC_DATA_BGN+1)/SD_SEC_NUM_FRAM)

/////////////////////////
// �����洢�������
#define	SD_JD_SEC_BGN					g_u32JDInfo_Addr_Begin			
#define	SD_JD_SEC_END				    g_u32JDInfo_Addr_End
// �������ݵ�ַ
#define	SD_CTX_JD_INFNUM				4							// 4-7
#define	SD_CTX_JD_SEC_BGN				8							// 8-11
#define	SD_CTX_JD_SEC_END				12							// 12-15
#define	SD_CTX_JD_NEXTINF   			16							// 16-19
#define	SD_CTX_JD_CURINF				20							//20-23




/* ������Ҫ����ȫ�ֱ��� */
//#define		ZTYAFENG_LANE1_2	50					// ѹ���ȣ�����1��2
//#define		ZTYAFENG_LANE2_3	40					// ѹ���ȣ�����2��3
//#define		ZTYAFENG			(ZTYAFENG_LANE1_2+ZTYAFENG_LANE2_3)/2					// ѹ��ƽ�����
//#define		ZTYAFENGAFFECT_LANE1_2		(ZTYAFENG_LANE1_2+80*2)							// ѹ��1Ӱ������
//#define		ZTYAFENGAFFECT_LANE2_3		(ZTYAFENG_LANE2_3+80*2)							// ѹ��2Ӱ������
//#define		ZTYAFENGAFFECT		(ZTYAFENGAFFECT_LANE1_2+ZTYAFENGAFFECT_LANE2_3)/2		// ѹ��ƽ��Ӱ������
////#define		NEWYAFENGAFFECT		((ZTYAFENG_LANE1_2+ZTYAFENG_LANE1_2)/2+50)				// ѹ�����߼�ƽ��Ӱ������
//#define		NEWYAFENGAFFECT		100




/* ������Ҫ���ü����Բ��� */
//#define		MODELSYSTEST			0		// ģ����Ի������ԣ���Ҫ����Ŀǰ���Ի�������5�����ݣ�added wujinlong 20130731 

//#define		CHANNELYaFengWIDE	175					// ѹ��խ����ȣ���խ������ 175



	







 








	   












 





		


















/////////////////////////////////////////////////////////////
// added by wujinlong 20130805



/***********
    ����
***********/
//#if MODELSYSTEST > 0
//#define		CHANGENUM_MODEL			0x44		// ��������  	����4����  0x40   ˫��4����  0x44
//												//				����3����  0x30   ˫��3����  0x33	
//												//			 	����2����  0x20   ˫��2����  0x22
//#else
//#define		CHANGENUM_MODEL			0x30		// С�ڹ겼��
//#endif
//
#define		NET_PORT_NUM			5			// �˿�����
#define		SOCKET_WT				0			// �˿�5000	  ����֡0x81����PC��������������������õȽ���
#define		SOCKET_WAVE				1			// �˿�5001	  ����0xAA
#define		SOCKET_RES				2			// �˿�5002	  ��־
#define		SOCKET_UDP				3			// �˿�5003	  ����
#define		SOCKET_SVI				4			// �˿�5004	  simple Veh Info ���׳�����Ϣ

#define		RECONNECTCNT			3			// ��������



//
#define	IO_ANTISHAKE_WAIT_CNT		4		// IOץ�ķ�������




// ��־
#define	LOG_UPLOAD_PERRIOD_CNT		10		// ��־�ϴ�����ʱ�����

// һ��ʱ����û�г���ͨ���������������գ���Ҫ���߼��У�g_u8VehPostCnt
#define	NOVEH_CLR_CNT				2*10			// 10s

//
#define	NET_HEART_FAILNUM			3				// ����ʧ�ܴ���������������׼
#define	NET_HEART_SENDINTERVAL		200				// ��������ʱ����10s��10*20*50ms������ͨ��������ѵʱ��ȷ��
#define	NET_HEART_RECVINTERVAL		100				// ���ջظ�ʱ���ʱ���ֵ��ȡǰ��һ���ʱ��






















	
#endif	// end of #ifndef	__DEFCONFIG_H
