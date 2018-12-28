/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**
*************************************************************************************************************/

#define	__JZGLOBAL
#define	__WT_TASK_C

#include "common.h"

//#define		ZTYAFENG_LANE1_2	PROMODPARAM.m_au8LaneBetwWide[0]					// ѹ���ȣ�����1��2
//#define		ZTYAFENG_LANE2_3	PROMODPARAM.m_au8LaneBetwWide[1]					// ѹ���ȣ�����2��3
//#define		ZTYAFENG			(ZTYAFENG_LANE1_2+ZTYAFENG_LANE2_3)/2					// ѹ��ƽ�����
//#define		ZTYAFENGAFFECT_LANE1_2		(ZTYAFENG_LANE1_2+80*2)							// ѹ��1Ӱ������
//#define		ZTYAFENGAFFECT_LANE2_3		(ZTYAFENG_LANE2_3+80*2)							// ѹ��2Ӱ������
//#define		ZTYAFENGAFFECT		(ZTYAFENGAFFECT_LANE1_2+ZTYAFENGAFFECT_LANE2_3)/2		// ѹ��ƽ��Ӱ������
//#define		NEWYAFENGAFFECT		100

#define		SETUPALIAS				g_Setup			//���ò����ṹ
#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam

//#define		LCDPRINTC				PrintCharXY			//��ʾ�ַ���
//#define		LCDPRINTFC				PrintFormatDataXY	//��ʾһ����ʽ���ַ���
//const uint16 	SingleData[] = {29,  30,  31,  34,  37,  37,  39,  45,  45,  47,  48,  52,  55,  55,  60,  62,  65,  66,  69,  74,  72,  78,  79,  82,  87,  89,  92,  95,  96, 100, 105, 108, 109, 113, 110, 120, 121, 123, 128, 131, 134, 137, 141, 143, 147, 152, 152, 158, 160, 162, 170, 170, 173, 175, 176, 180, 184, 191, 191, 198, 199, 200, 202, 207, 209, 215, 218, 219, 223, 225, 229, 236, 240, 240, 247, 250, 254, 254, 257, 261, 262, 267, 270, 272, 274, 280, 283, 286, 288, 293, 292, 298, 302, 303, 306, 307, 313, 315, 318, 322, 322, 328, 329, 331, 336, 337, 343, 345, 344, 347, 351, 353, 355, 354, 359, 361, 365, 368, 367, 373, 376, 377, 374, 380, 384, 381, 387, 390, 390, 389, 393, 394, 398, 400, 401, 403, 404, 407, 409, 409, 410, 411, 412, 415, 419, 422, 420, 422, 424, 426, 428, 429, 428, 430, 431, 433, 432, 436, 436, 435, 437, 440, 442, 442, 440, 444, 446, 447, 446, 448, 446, 445, 451, 452, 451, 452, 454, 457, 455, 455, 456, 454,\
//458, 457, 459, 458, 455, 459, 458, 458, 461, 459, 461, 463, 464, 465, 462, 466, 466, 463, 467, 463, 466, 468, 469, 467, 465, 469, 467, 468, 468, 466, 471, 468, 471, 471, 470, 471, 473, 469, 473, 470, 472, 472, 471, 470, 469, 472, 472, 474, 475, 470, 475, 474, 475, 473, 473, 473, 475, 473, 476, 475, 475, 478, 474, 472, 475, 471, 472, 475, 474, 473, 472, 474, 475, 475, 475, 472, 474, 472, 472, 472, 472, 472, 472, 471, 473, 472, 475, 474, 475, 471, 473, 473, 473, 472, 475, 471, 469, 469, 474, 468, 473, 473, 473, 471, 473, 471, 470, 472, 469, 470, 472, 471, 470, 470, 470, 471, 469, 466, 466, 467, 466, 466, 467, 467, 467, 465, 465, 462, 467, 464, 463, 462, 462, 459, 461, 459, 463, 462, 462, 462, 460, 458, 461, 458, 457, 457, 457, 455, 454, 457, 452, 453, 452, 452, 451, 450, 447, 449, 448, 446, 447, 445, 445, 443, 441, 441, 441, 438, 441, 438, 440, 439, 436, 431, 430, 433, 432, 431, 427, 428, 425, 425, 425, 421, 423, 418, 415,\
//417, 413, 415, 415, 412, 413, 411, 406, 406, 401, 404, 402, 403, 398, 397, 395, 393, 394, 388, 386, 385, 383, 381, 380, 377, 374, 374, 370, 367, 367, 366, 363, 360, 359, 355, 353, 351, 346, 348, 342, 340, 337, 336, 332, 330, 325, 326, 320, 318, 315, 311, 308, 309, 303, 299, 297, 293, 290, 289, 285, 283, 279, 278, 271, 271, 266, 263, 258, 255, 250, 248, 246, 243, 241, 237, 234, 227, 223, 220, 218, 217, 214, 212, 207, 201, 200, 194, 190, 190, 185, 182, 181, 178, 175, 169, 165, 162, 158, 157, 153, 154, 148, 144, 144, 143, 139, 134, 130, 129, 128, 123, 120, 115, 115, 114, 110, 106, 106, 102,  99,  97,  92,  94,  92,  90,  89,  84,  80,  81,  78,  77,  74,  72,  69,  69,  66,  66,  64,  63,  58,  57,  55,  54,  52,  50,  48,  48,  48,  48,  44,  43,  40,  41,  38,  36,  37,  35,  34,  30,  30,  31,  24,  25,  26};
//const uint16    DoubleData[] = {28,  33,  30,  33,  40,  38,  37,  41,  48,  50,  53,  56,  56,  63,  62,  69,  73,  74,  81,  80,  86,  85,  90,  98,  99, 104, 108, 112, 118, 123, 126, 130, 137, 141, 145, 149, 153, 159, 164, 169, 170, 178, 186, 188, 193, 200, 206, 209, 213, 218, 228, 229, 236, 242, 248, 254, 261, 263, 269, 276, 282, 288, 295, 301, 306, 314, 317, 321, 330, 332, 340, 348, 355, 362, 368, 375, 381, 388, 394, 399, 408, 413, 416, 425, 431, 438, 444, 449, 456, 462, 471, 475, 483, 491, 496, 505, 510, 514, 522, 532, 538, 543, 550, 554, 558, 564, 572, 576, 584, 592, 595, 603, 609, 615, 619, 623, 633, 638, 643, 648, 655, 661, 663, 671, 674, 679, 686, 693, 693, 699, 702, 708, 714, 719, 723, 730, 730, 734, 740, 743, 743, 751, 754, 758, 760, 763, 764, 769, 773, 778, 780, 782, 783, 786, 789, 790, 795, 797, 796, 802, 804, 805, 808, 808, 810, 813, 814, 814, 820, 819, 820, 823, 824, 824, 827, 828, 827, 829, 828, 831, 833, 832,\
//832, 835, 836, 834, 835, 834, 836, 837, 837, 834, 837, 835, 836, 837, 837, 837, 835, 835, 832, 834, 830, 832, 833, 831, 830, 830, 827, 826, 824, 824, 824, 824, 822, 818, 821, 820, 818, 817, 816, 812, 806, 805, 805, 803, 799, 797, 796, 796, 793, 790, 786, 784, 780, 779, 775, 769, 764, 762, 762, 757, 755, 752, 746, 746, 740, 737, 732, 731, 723, 722, 717, 711, 706, 702, 701, 694, 690, 685, 682, 676, 667, 665, 660, 657, 652, 649, 640, 637, 632, 627, 621, 615, 606, 601, 600, 590, 584, 581, 571, 566, 562, 557, 549, 545, 539, 533, 527, 522, 515, 509, 503, 499, 492, 485, 479, 472, 467, 462, 454, 451, 445, 439, 432, 424, 418, 412, 408, 398, 395, 387, 382, 372, 367, 360, 355, 350, 343, 335, 330, 324, 318, 313, 308, 300, 298, 289, 285, 275, 269, 266, 259, 252, 251, 244, 236, 232, 224, 219, 212, 207, 200, 200, 193, 187, 181, 175, 170, 166, 160, 157, 151, 148, 139, 135, 131, 131, 123, 119, 117, 112, 109, 103, 101,  94,  89,  87,  82,\
//79,  75,  73,  70,  69,  63,  63,  59,  55,  53,  48,  48,  46,  44,  40,  40,  38,  38,  32,  32,  30,  28,  27,  27,  25};

//uint8 F8Zhi[]={0xFF, 0xFF, 0x00, 0x8F, 0x01, 0x00, 0x0C, 0x01, 0x00, 0x00, 0x00, 0x00};
/*********************************************************************************************************
** Function name:           JZInit
**
** Descriptions:            ��ʼ��ϵͳ��������������Ϣ
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              Wang ZiFeng
** Created Date:            20121026
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void	JZInit(void)
{
	Sys_init();
	LogicMem_init();
	External_Equipment_init();
	Fault_diagnosis_init();  
#if	 OS_TASK_CREATE_EXT_EN > 0
 	memset(g_Taskstatics,0,sizeof(TaskStatics)*TASK_SUM);  	//��Դͳ��
#endif
#if	TASKTIMEENABLE
	memset(&TaskRunTimeStatistics,0,sizeof(TaskRunTimeStatistics));
#endif
	
} 


/*********************************************************************************************************
** Function name:           TaskStart
**
** Descriptions:            �������񣬳�ʼ��ϵͳ����������������
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20110511
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void RunStartTask(void)
{
	// �ó�ʼ��δ������־
	g_u8StartOver = START_NOT_OVER;

#if	 OS_TASK_CREATE_EXT_EN >0
 	OSTaskCreateExt(TaskStart,(void *)0, &TaskStartStk[TASK_START_STACKSIZE-1], TASK_START_PRIO,
					TASK_START_PRIO,&TaskStartStk[0],TASKSTACKSIZE,&g_Taskstatics[8],
					OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
#else
    OSTaskCreate(TaskStart,(void *)0, &TaskStartStk[TASK_START_STACKSIZE-1], TASK_START_PRIO);
#endif
}

/*********************************************************************************************************
** Function name:           TaskStart
**
** Descriptions:            �������񣬳�ʼ��ϵͳ����������������
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              ZHANG Ye
** Created Date:            20110511
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void TaskStart (void *tdata)
{
    uint32	l_u32BR = 0;
	uint8   l_u8UseOne = 0;    //���ڽ���һ���·���ֵ
	
    tdata = tdata;

	FeedExternDog();		//ι�ⲿ��
	
	RESET_FPGA(); // һ���У��ͽ���FPGA��λ��20130922
 
    FeedExternDog();		//ι�ⲿ��

	TargetInit();

	FeedExternDog();		//ι�ⲿ��

    InitAllIRQ();

	FeedExternDog();		//ι�ⲿ��

	IOInit();

	FeedExternDog();		//ι�ⲿ��

	FReadFromTDC();			//��ȡ�������
		 	 	 
	FeedExternDog();		//ι�ⲿ��

// 	InitializeW5100(&g_IPSetLocal);	// ��ʼ������
	InitializeW5300(&g_IPSetLocal);	// ��ʼ������

	FeedExternDog();		//ι�ⲿ��
										  	 
	FPGAInitParam(); // ��ʼ��FPGA
	
#if OS_TASK_STAT_EN > 0
	OSStatInit();
#endif
	
	FeedExternDog();		//ι�ⲿ��

	FTaskBuild(); //��������

	FeedExternDog();		//ι�ⲿ��

	FIOStatusInt(); //IO״̬��ʼ��
		
	if(PROMODPARAM.m_u8EnableFlag[5] == 1) 
	{
		FPGA_LASER_STATUS = 0xFF;  //�����ٿ���
	}

	FeedExternDog();		//ι�ⲿ��

	for( l_u32BR = 0;l_u32BR < 2 ; l_u32BR++ )
	{
		OSTimeDly(OS_TICKS_PER_SEC/2);

		FeedExternDog();		//ι�ⲿ��
		
		OSTimeDly(OS_TICKS_PER_SEC/2);
	
	}
	g_u8StartOver = START_OVER; // �ó�ʼ��������־

	FeedExternDog();		//ι�ⲿ��

	EnableFPGA_INT(); // һ��׼��OK������FPGA�ж�

	FeedExternDog();		//ι�ⲿ��

	AddStartupCnt();	//��¼��������

	FeedExternDog();		//ι�ⲿ��

	FSendCmd2PC_ZONG( 0, ADJ_POWERRUN);	//�ϵ�ָ��

	while(1)
	{						
		if(g_u8IfRunSta == 0 && g_u8FirstUse == 0)
		{
			OSTimeDly(100*OS_TICKS_PER_SEC/1000);
		}
		else
		{
			//OSTimeDly(5000*OS_TICKS_PER_SEC/1000);
			OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
			FeedExternDog();	
			OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
			FeedExternDog();	
			OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
			FeedExternDog();	
			OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
			FeedExternDog();	
			OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
			FeedExternDog();	
		}

		if(g_u8StaticMotionswitch)
		{
			if(l_u8UseOne == 0)
			{
				SET_FPGA_DIGITAL_CMD_SEND_START();    // ������������(20170816)
				l_u8UseOne = 1;
			} 
			g_u8StaticMotionswitch = 0;				   
			LEDON();
		}
		else
		{
	  	  g_u8StaticMotionswitch = 1;
		    LEDOFF();
		}
		FeedExternDog();		//ι�ⲿ��

	}	  
}

/*********************************************************************************************************
** Function name:		CheckStackTaskUse
** Descriptions:		CheckStackTaskUse
** 						
** input parameters:	void 
** output parameters:	void
**
** Created by:			  zhangtonghan	
** Created Date:		 20171122
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

void CheckStackTaskUse(void *tdata)
{	
	#if	 OS_TASK_CREATE_EXT_EN >0
	uint8 err1 =0,err2=0,err3=0;
	uint16  l_u16SendLen;
	uint8   l_u8Err;
  OS_STK_DATA  StackBytes,StackBytes1,StackBytes2;
	#endif
	tdata = tdata;

	// �ȴ���ʼ������
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}
	while(1)
	{
		OSTimeDly(500);
	#if	 OS_TASK_CREATE_EXT_EN >0
		err1=OSTaskStkChk(TASK_LOGIC_PRIO,&StackBytes);
		OSTimeDly(500);
		err2=OSTaskStkChk(TASK_ALG_PRIO,&StackBytes1);
		OSTimeDly(500);
		err3=OSTaskStkChk(TASK_VEH_PRIO,&StackBytes2);
		
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"Task1 F:%d,U:%d;Alg F:%d,U:%d;Veh F:%d,U:%d,%d,%d,%d.\r\n",
									 StackBytes.OSFree,StackBytes.OSUsed,StackBytes1.OSFree,StackBytes1.OSUsed,StackBytes2.OSFree,StackBytes2.OSUsed,err1,err2,err3);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
	}
}

/*********************************************************************************************************
** Function name:		TaskRec1
** Descriptions:		ѭ����⼤��״̬��Ϣ��ÿ������������IO�ڣ�ÿ��IO����һ������. 
** 						priority:		���ȼ� 1��
** input parameters:	void 
** output parameters:	void
**
** Created by:			Chen Wei		  
** Created Date:		20110508	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  TaskRec1(void *tdata)	 
{	
	uint16  l_u16SendLen;
	uint8   l_u8Err;
	// �����ɵ������ʼ��
	g_u8RasterDaFeiCnt = 0;    
	g_u8RasterNoDaFeiCnt = 4;
	// 
    tdata = tdata;

	// �ȴ���ʼ������
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	// ����ʼ����
#if DEBUGINFO_TASK_SP_EN > 0
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u T1 Recd TaskBegin Succ.\r\n",
					       g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
					       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
#endif

	g_u16SystemUseMode =  (PROMODPARAM.m_u8TriggerMode<<8) + 0; // add by sxh  ��ȡϵͳģʽ

	FLogicForLoop();//�˴�Ϊʹ����Ȧ��� wgh 20140303  
}




/*********************************************************************************************************
** Function name:		FLogicForLoop
** Descriptions:		ʹ����Ȧʱ�߼�
** input parameters:	 
** output parameters:	
**
** Created by:			Wgh		  
** Created Date:		20140303	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		 zhangtonghan 		
** Modified date:	 20170531
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
 void  FLogicForLoop(void)
 {
	uint8 l_au8LoopIn[16];
	uint8 l_au8LoopOnlyCheck[16];
	uint8 l_au8LoopState[CHL_MAXNUM];//��ȦIO״̬ 0����������Ȧ�� 1��������Ȧ 2��������Ȧ�� 3��������Ȧ
	uint8 l_u8tempi,l_u8tempj;//,l_u8temp,l_u8temp1;
	uint8 l_u8ZTLocationX,l_u8ZTLocationY;
	uint8 l_au8VehPositionFlag[CHL_MAXNUM];//0����������Ȧ�� 1��������Ȧ 2��������Ȧ�� 3��������Ȧ
	uint8 l_au8ReVehPositionFlag[CHL_MAXNUM];//0����������Ȧ�� 1��������Ȧ 2��������Ȧ�� 3��������Ȧ
//	uint8 l_au8HaveVehPerCHL[2*CHL_MAXNUM-1]; // �г������Ƿ��г��������³�ʱ�ñ�ʶ
//	uint8 l_au8ReHaveVehPerCHL[2*CHL_MAXNUM-1];
	uint32 l_u32TimeNow;//����ǰʱ�䣬��ûд
	int8  l_n8BadPlatPos[8];//���ɼ����λ�� ��ʼ-1 
	uint8 l_u8PosForZT;//��¼����խ������һ������

//	void* l_u32Address[9];//����խ����խ��ƥ�����
	void* l_u32WhelCntAddr[8];
	void* l_u32ZT2WhelAddr[12];
	void* l_u32Whel2AxleAddr[6];
	void* l_u32HideAddress[8];
//	void* l_u32AddressForAxle[9];//����������ƥ�����
	void* l_u32AddressForVeh[3];//������������
	void* l_u32AddressForAbNorVeh[3];//������������
	void* l_u32HideZTPostAxleAddr[5];//����խ������
	void* l_u32HideZTPostVeh[8]; //����խ���׳�
	Channal_Struct * l_pChlItem = (Channal_Struct *)0;//խ������ָ��
	ZTINFRECELEMENT l_pZtInf; //�߼�������խ����Ϣ
	uint8 l_u8whelcnt = 0;

	uint8   l_u8Buf1[16];
	uint8 	l_u8cnt=0;
	uint8   l_u8cnt1 = 0;

	//
	uint16  l_u16SendLen;
	uint8   l_u8Err;
	uint8   l_u8JudgeFlag;    //3��ʾ��Ȧ�ж���β��2��ʾ�����ж���β
	
	uint8 l_u8PostZTFlag = 1;    //0:���׸�խ��,1:�׸�խ��

	uint8 firWhelPos = 0;
	uint8 secWhelPos = 0;
	uint16 Wheel1XPos = 0, Wheel2XPos = 0;    //խ�����ֵ�λ��(����λ��),��λ(cm)
	uint8  LeftPoint = 0, RightPoint = 0;     //����,�����жϵ�˫̥

	//
	// �ȴ���ʼ������
	while ( START_NOT_OVER == g_u8StartOver )
	{
		OSTimeDly(TASK_BEGIN_WAIT_TICKS);
	}

	while(1)
	{
		OSTimeDly(2);
		if(g_u8FirstUse == 0)
		{
			break;
		}
	}

	/***************�ɼ��廵����*************/
	for(l_u8tempi=0; l_u8tempi<8; l_u8tempi++)
	{
		l_n8BadPlatPos[l_u8tempi] = -1;
	}
	for(l_u8tempi=0; l_u8tempi<8; l_u8tempi++)
	{
		if(PROMODPARAM.m_au8AcqBoardFlag[l_u8tempi]==0)
		{
			for(l_u8tempj=l_u8tempi+1; l_u8tempj<8; l_u8tempj++)
			{
				l_n8BadPlatPos[l_u8tempj]++;
			}
		}
	}
	/******************************************/
	///////////////////
	
	for(l_u8tempi = 0; l_u8tempi< 16; l_u8tempi ++)
	{
		g_au8LoopStatePreOnlyCheck[l_u8tempi] = 1;
	}
	while(1)
	{
		OSTimeDly(1);
		///////////////////////////��ѯ��Ȧ״̬
		if(g_u8IfRunSta == 1)
		{
			if(g_u8ZTStaSendFlag == 1)
			{
				g_u8ZTStaSendFlag = 0;
				FSendCmd2PC_ZONG(1,ADJ_USESTAT);
			}
			if(g_u8NumUseLoopBoard == 0)
			{
				l_u8Buf1[0]=CoilIN_IO1;
				l_u8Buf1[1]=CoilIN_IO2;
				l_u8Buf1[2]=CoilIN_IO3;
				l_u8Buf1[3]=CoilIN_IO4;
				l_u8Buf1[4]=CoilIN_IO5;
				l_u8Buf1[5]=CoilIN_IO6;
				l_u8Buf1[6]=CoilIN_IO7;
				l_u8Buf1[7]=CoilIN_IO8;
				l_u8Buf1[8]=CoilIN_IO9;
				l_u8Buf1[9]=CoilIN_IO10;
				l_u8Buf1[10]=0;
				l_u8Buf1[11]=0;
				l_u8Buf1[12]=0;
				l_u8Buf1[13]=0;
				l_u8Buf1[14]=0;
				l_u8Buf1[15]=0;
			}
			else
			{
				l_u8Buf1[0]=g_u8IOStateForBoard[0];
				l_u8Buf1[1]=g_u8IOStateForBoard[1];
				l_u8Buf1[2]=g_u8IOStateForBoard[2];
				l_u8Buf1[3]=g_u8IOStateForBoard[3];
				l_u8Buf1[4]=g_u8IOStateForBoard[4];
				l_u8Buf1[5]=g_u8IOStateForBoard[5];
				l_u8Buf1[6]=g_u8IOStateForBoard[6];
				l_u8Buf1[7]=g_u8IOStateForBoard[7];
				l_u8Buf1[8]=g_u8IOStateForBoard[8];
				l_u8Buf1[9]=g_u8IOStateForBoard[9];
				l_u8Buf1[10]=g_u8IOStateForBoard[10];
				l_u8Buf1[11]=g_u8IOStateForBoard[11];
				l_u8Buf1[12]=g_u8IOStateForBoard[12];
				l_u8Buf1[13]=g_u8IOStateForBoard[13];
				l_u8Buf1[14]=g_u8IOStateForBoard[14];
				l_u8Buf1[15]=g_u8IOStateForBoard[15];
			}
			for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
			{
				if(l_u8Buf1[l_u8tempi] == 0)
				{
					if(g_u8RunLoopSta[l_u8tempi] != 2)
					{
						g_u8RunLoopSta[l_u8tempi] = 1;
					}
				}
				if(l_u8Buf1[l_u8tempi] == 1)
				{
					if(g_u8RunLoopSta[l_u8tempi] == 1)
					{
						g_u8RunLoopSta[l_u8tempi] = 2;
						FSendCmd2PC_ZONG(1,ADJ_USESTAT);
					}
				}
				
			}
		}
		//////////////////////////
#if    TASKTIMEENABLE
		if(1==TaskRunTimeStatistics.m_ucWT_TASKFlag ){//1:��ʼ����״̬
			OSSchedLock();
			if(0x01!=T1TCR){
				T1TCR=0x02;
				T1TCR=0x01;
			}			
			TaskRunTimeStatistics.m_ucWT_TASKFlag=2;   //�л�������״̬
			TaskRunTimeStatistics.m_ulWT_TaskDet=0;
			
			TaskRunTimeStatistics.m_ulWT_Task1=T1TC;
			OSSchedUnlock();				
		}
		else if(2==TaskRunTimeStatistics.m_ucWT_TASKFlag){
		    OSSchedLock();
			TaskRunTimeStatistics.m_ulWT_Task1=T1TC; //����״̬�и���
			OSSchedUnlock();
		}
#endif
		/******************����״̬��ʼ������������������ʼ��******************/
		for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)
		{
			l_au8VehPositionFlag[l_u8tempi]=0;
			l_au8ReVehPositionFlag[l_u8tempi]=0;
			l_au8LoopState[l_u8tempi] = 0;
		}
		
		/**********************************************************************/
		ReadFPGAGlobalTime(l_u32TimeNow);
		
		FCheckDeviceERR();//��ѯ�豸״̬
		
//		g_u8DevErr = 1;

		//// ��¼�������Ȧ��IO״̬(liumingxin)(20170706) ////
		l_u8JudgeFlag = 0;
		if((((g_u8DevErr & 0x02) != 2) && g_u32LaserIOUseFlag == 1 && g_u8RasterNoDaFeiCnt == 4) || (g_u32LaserIOUseFlag == 123))    // g_u8DevErr �豸���� 0:��, 1:��Ȧ����, 2:�������, 3:������
		{
			FRecordIO_Laser(l_au8LoopIn, l_au8LoopOnlyCheck);    // ��ȡ����ģ���IO״̬(liumingxin)(20170705)
			l_u8JudgeFlag = 2;
		}
		else
		{
		    FRecordIO(l_au8LoopIn, l_au8LoopOnlyCheck);         // ��ȡ��ȦIO��Ϣ
			  l_u8JudgeFlag = 3;
		}
		//// end(liumingxin)(20170706) ////
		
		for(l_u8tempi=0; l_u8tempi< 16; l_u8tempi ++)
		{
			if(l_au8LoopOnlyCheck[l_u8tempi] == 0 && g_au8LoopStatePreOnlyCheck[l_u8tempi] == 1)
			{
				l_au8LoopState[l_u8tempi] = 1;
				g_u8IOStateRe[l_u8tempi] = 0;
			}
			else if(l_au8LoopOnlyCheck[l_u8tempi] == 1 && g_au8LoopStatePreOnlyCheck[l_u8tempi] == 0)
			{
				l_au8LoopState[l_u8tempi] = 3;
				g_u8IOStateRe[l_u8tempi] = 0;
			}
			else if(l_au8LoopOnlyCheck[l_u8tempi] == 0 && g_au8LoopStatePreOnlyCheck[l_u8tempi] == 0)
			{
				l_au8LoopState[l_u8tempi] = 2;
			}
			else
			{
				l_au8LoopState[l_u8tempi] = 0;
			}
			g_au8LoopStatePreOnlyCheck[l_u8tempi] = l_au8LoopOnlyCheck[l_u8tempi];
		}
		
// 		FLaserProcess(l_au8LoopIn, l_u32TimeNow);//������Ϣ����

		
		/*****************�����ٿ����͹ر�********************/
		if(PROMODPARAM.m_u8EnableFlag[5] == 1)
		{
			if( l_au8LoopIn[0]==1 && l_au8LoopIn[1]==1 && l_au8LoopIn[2]==1 && l_au8LoopIn[3]==1 && l_au8LoopIn[4]==1
			&& l_au8LoopIn[5]==1 && l_au8LoopIn[6]==1 && l_au8LoopIn[7]==1 && l_au8LoopIn[8]==1 && l_au8LoopIn[9]==1
			&& l_au8LoopIn[10]==1 && l_au8LoopIn[11]==1 && l_au8LoopIn[12]==1 && l_au8LoopIn[13]==1 && l_au8LoopIn[14]==1
			&& l_au8LoopIn[15]==1 )
			{
				// �г�
				FPGA_LASER_STATUS = 0xFF;
			}
			else
			{
				// �޳�
				FPGA_LASER_STATUS = 0;
			}
		}

		if( l_au8LoopIn[0]==1 && l_au8LoopIn[1]==1 && l_au8LoopIn[2]==1 && l_au8LoopIn[3]==1 && l_au8LoopIn[4]==1
			&& l_au8LoopIn[5]==1 && l_au8LoopIn[6]==1 && l_au8LoopIn[7]==1 && l_au8LoopIn[8]==1 && l_au8LoopIn[9]==1
			&& l_au8LoopIn[10]==1 && l_au8LoopIn[11]==1 && l_au8LoopIn[12]==1 && l_au8LoopIn[13]==1 && l_au8LoopIn[14]==1
			&& l_au8LoopIn[15]==1 )
		{
			for(l_u8tempi = 0; l_u8tempi<20; l_u8tempi++)
			{
				g_au8ZTEnPerLie[l_u8tempi] = 1;
				g_au8ReZTEnPerLie[l_u8tempi] = 1;
			}
		}
		/*******************************************************/
		
		FSendWaveBE(l_au8LoopIn, l_u32TimeNow, l_u8JudgeFlag);//����λ�����Ͳ��ο�ʼ�ͽ�����Ϣ
		FVehPassStatus(l_au8LoopIn, l_au8VehPositionFlag, l_au8ReVehPositionFlag);//����IO�жϹ���״̬

		ReadFPGAGlobalTime(l_u32TimeNow);	
		FDeleteValidZTOrColum(l_u32TimeNow, l_au8VehPositionFlag, l_au8ReVehPositionFlag);//���ݵ�ǰʱ��ɾ����Ӧխ����������
		
		l_pChlItem = (Channal_Struct *)OSQAccept(g_pQDnMsg);//����խ���жϽ���խ���߼�����
		if(l_pChlItem !=(Channal_Struct *)0)
		{				
			/*******************************************************/
			/*********���ɼ����г��ֲ���ʹ��ʱ������ɼ���������εݲ�******/
			if(l_n8BadPlatPos[7] != -1)
			{
				l_u8PosForZT = l_pChlItem->u8Id/8;
				if(l_n8BadPlatPos[l_u8PosForZT]!=-1)
				{
					l_pChlItem->u8Id = l_pChlItem->u8Id - l_n8BadPlatPos[l_u8PosForZT]*8;
				}
			}
			
			if(g_u8FirstAxleFlag == 1)  //��һ������Ȧ��β�������ǰ��խ���ٴδ��� ��ȡ����־
			{
				if((l_pChlItem->u8Id/PROMODPARAM.m_u8ZTRow)==g_u8FirstAxleColum)
					g_u8FirstAxleFlag=0;
			}
			if(g_u8CrossingFlag == 1)  //��һ������Ȧ��β�������ǰ��խ���ٴδ��� ��ȡ����־
			{
				if((l_pChlItem->u8Id/PROMODPARAM.m_u8ZTRow)==g_u8CrossingColum)
					g_u8CrossingFlag=0;
			}

			/***************************************************************/
			/*****************************խ����Ϣ��¼****************************/
			l_pZtInf.u16ID = l_pChlItem->u16SelfID;
			l_pZtInf.m_ucTouch = l_pChlItem->u8Id;
			l_pZtInf.m_ulTime = l_pChlItem->m_ulTime;
			l_pZtInf.m_bUpOrDn = l_pChlItem->m_bUpOrDn;
			l_pZtInf.m_bDirectionFlag = l_pChlItem->m_bDirectionFlag;
			l_pZtInf.m_CWbZtInf.m_ulUpTime = l_pChlItem->u32uptm;
			l_pZtInf.m_CWbZtInf.m_ulDownTime = l_pChlItem->u32dntm;
			l_pZtInf.m_CWbZtInf.m_unSum = l_pChlItem->u32chlSum;
			l_pZtInf.m_CWbZtInf.m_unMaxValue = l_pChlItem->u16ptMaxValue;
			/***************************************************************/
			//�ж�������ͬID��խ�������Ƿ�����С
			if((GetDiff(g_ChlData[g_ZTTime[l_pChlItem->u8Id]].u32Maxtm,g_ChlData[l_pChlItem->u16SelfID].u32Mintm) < 200) && (g_ZTTime[l_pChlItem->u8Id]!=0))  //��ͬID��խ���������С��200����(һ���ϳӺ�һ���³�֮��ļ��)
			{
				l_u8PostZTFlag = 0;
			}
			else
			{
				g_ZTTime[l_pChlItem->u8Id] = l_pChlItem->u16SelfID;    //���¸�λ�õ�խ��Pos
				l_u8PostZTFlag = 1;
			}
			FTransLocation(l_pZtInf.m_ucTouch, &l_u8ZTLocationX, &l_u8ZTLocationY,1);	//�õ�խ�������к���
			ReadFPGAGlobalTime(l_u32TimeNow);
			if(tickGetDiff(l_pZtInf.m_ulTime, g_au32FilterTime[l_u8ZTLocationX][l_u8ZTLocationY]) > 0)    
			{
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nI=%d,P=%d,%u,%u,%u,%u,%u,%u,%02x,%02x,%d.",
											 l_pChlItem->u8Id,l_pChlItem->u16SelfID,l_pChlItem->u32chlSum,l_pChlItem->u32uptm,l_pChlItem->u32dntm,									 
											 l_pChlItem->u32realCnt,l_pChlItem->u32FirstDnTime,l_pChlItem->u16ptMaxValue,
											 l_pChlItem->u8FirstZTMaxAndUpDnPos,l_pChlItem->u8SecondZTMaxAndUpDnPos,l_u32TimeNow);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
				if((g_DeletVeh[l_u8ZTLocationX].l_u8DeleteFlag == 2) && ((g_BackVeh[l_u8ZTLocationX].backAxleCnt == 0) || (l_au8ReVehPositionFlag[l_u8ZTLocationX] == 2)))
				{
					memset(&g_DeletVeh[l_u8ZTLocationX],0,sizeof(DeleVeh_Str));	
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
									l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n%d Lane Loop Back Car Delte Concel !!!",l_u8tempi);
									g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
									OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
									g_u16CurDebugInd++;
									g_u16CurDebugInd %= COM_NUM;
									memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
				FCheckIODevice(l_au8LoopState, l_u8ZTLocationX);// IO�����ж� �й���ʱ��IO����һ��ʱ�䲻�仯��ΪIO���ܹ���
				FCheckZTDevice(l_pZtInf, l_u8ZTLocationX, l_u8ZTLocationY); //խ�������ж� �������խ������ͬ��խ�������������ϴ���Ϊխ�����ܹ���

				
				g_au32FilterTime[l_u8ZTLocationX][l_u8ZTLocationY] = l_pZtInf.m_ulTime;//��¼ǰ��խ������ʱ��
				ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
				/****************************խ�������߼��ж�*************************************/
				
				if((l_u8ZTLocationX < 2*PROMODPARAM.m_u8LaneNum) && (l_u8ZTLocationY<PROMODPARAM.m_u8ZTRow) && 
				   (l_pZtInf.m_ucTouch<HideZTNUM) && l_u8PostZTFlag && g_Setup.an8ZTStatus[l_pZtInf.m_ucTouch] == 1)//�ж�խ��λ�ã��������������խ�����������߼���ѹ��խ�������������������խ����ź���
				{	
					/**** ���Դ��� ****/
					if(l_pZtInf.m_ucTouch == 9)
					{
						l_u8cnt1++;
						if(l_u8cnt1 == 4)
						{
							l_u8cnt1 = 0;
						}
					}
					/**** end ****/
					
					l_u8whelcnt	= 0;
					LeftPoint = 0;
					RightPoint = 0;
					l_u32WhelCntAddr[0]	=	&l_pZtInf;
					l_u32WhelCntAddr[1]	=	&l_u8whelcnt;
					l_u32WhelCntAddr[2] =   &Wheel1XPos;
					l_u32WhelCntAddr[3] =   &Wheel2XPos;
					l_u32WhelCntAddr[4] =   &LeftPoint;
					l_u32WhelCntAddr[5] =   &RightPoint;
					l_u32WhelCntAddr[6] =   &firWhelPos;
					l_u32WhelCntAddr[7] =   &secWhelPos;
					FZTWheelCnt(l_u32WhelCntAddr);

					if(l_u8whelcnt > 0)
					{
						FTransLocation(l_pZtInf.m_ucTouch, &l_u8ZTLocationX, &l_u8ZTLocationY,2);
						if(g_au8HideDeltZTEnable[l_u8ZTLocationX/2] == 1)
							g_au8HideDeltZTEnable[l_u8ZTLocationX/2] = 0;
						ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
						l_u32ZT2WhelAddr[0] = &l_pZtInf;
						l_u32ZT2WhelAddr[1] = &l_u8whelcnt;
						l_u32ZT2WhelAddr[2] = &l_u8ZTLocationX;
						l_u32ZT2WhelAddr[3] = &l_u8ZTLocationY;
						l_u32ZT2WhelAddr[4] = &g_WheelTrace[0][0];
						l_u32ZT2WhelAddr[5] = &Wheel1XPos;
						l_u32ZT2WhelAddr[6] = &Wheel2XPos;
						l_u32ZT2WhelAddr[7] = &l_u32TimeNow;
						l_u32ZT2WhelAddr[8] = &LeftPoint;
						l_u32ZT2WhelAddr[9] = &RightPoint;
						FZTMatch2Whel(l_u32ZT2WhelAddr);
					}
					
				}
				else if(l_pZtInf.m_ucTouch >= HideZTNUM)
				{
					if(l_pZtInf.m_ucTouch == 32)
					{
						l_u8cnt++;						
						if(l_u8cnt == 2)
						{
							l_u8cnt =0;
						}
					}
					
					FTransLocation(l_pZtInf.m_ucTouch, &l_u8ZTLocationX, &l_u8ZTLocationY,2);
					g_au32HideLineLastTime[l_u8ZTLocationX] = g_ChlData[l_pZtInf.u16ID].u32Maxtm;				
					if(g_au8HideDeltZTEnable[l_u8ZTLocationX/2] == 1)
						g_au8HideDeltZTEnable[l_u8ZTLocationX/2] = 0;
					
					l_u32HideAddress[0]=&l_pZtInf;
					l_u32HideAddress[1]=&l_u8ZTLocationX;
					l_u32HideAddress[2]=&Forward_HideZTColum[0][0];
					l_u32HideAddress[3]=&Reverse_HideZTColum[0][0];
					l_u32HideAddress[4]=&g_asVehiclePerChannelForLoop[0][0];
					l_u32HideAddress[5]=&g_asReVehiclePerChannelForLoop[0][0];
					l_u32HideAddress[6]=&g_HideZTVehicleStruct[0][0];
					l_u32HideAddress[7]=&g_HideZTReVehicleStruct[0][0];
					
					if(l_u8ZTLocationY == 0)
					{						
						FMatchHideFristRow(l_u32HideAddress);
					}
					else if(l_u8ZTLocationY == 1)
					{

						FMatchHideSecondRow(l_u32HideAddress);
					}
				}
				else
				{
					OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nWrong ZT Num!!!.");
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
					OSSemPost(g_pSemSendThtouSP);
				}
			}
		}

		/**** ���еڶ���ץ�� ****/
		if(g_u8SecondCapFlag == 1)
		{
			FSecondeCapture();
		}
		/**** end ****/

		/**** ���ذ��������ƥ�� ****/
		ReadFPGAGlobalTime(l_u32TimeNow);			//��ȡ��ǰʱ��
		l_u32HideZTPostAxleAddr[0] = &l_u32TimeNow;
		l_u32HideZTPostAxleAddr[1] = &Forward_HideZTColum[0][0];
		l_u32HideZTPostAxleAddr[2] = &Reverse_HideZTColum[0][0];
		l_u32HideZTPostAxleAddr[3] = &g_HideZTVehicleStruct[0][0];
		l_u32HideZTPostAxleAddr[4] = &g_HideZTReVehicleStruct[0][0];
		FMatchHideColumAndCloum(l_u32HideZTPostAxleAddr);
		/**** end ****/
		
		
		/**** ���� ****/
		ReadFPGAGlobalTime(l_u32TimeNow);			//��ȡ��ǰʱ��
		l_u32Whel2AxleAddr[0] = &l_u32TimeNow;
		FPostWheel(l_u32Whel2AxleAddr);
		/**** end ****/
		
		/**** ��ƥ����� ****/
		ReadFPGAGlobalTime(l_u32TimeNow);			//��ȡ��ǰʱ��
		l_u32Whel2AxleAddr[0] = &l_u32TimeNow;
		l_u32Whel2AxleAddr[1] = g_WhelData;
		FWhelMatchAxle(l_u32Whel2AxleAddr);
		/**** end ****/
		

		/**** ����խ��׼���׳� ****/
		ReadFPGAGlobalTime(l_u32TimeNow);			//��ȡ��ǰʱ��
		l_u32HideZTPostVeh[0] = &l_u32TimeNow;
		l_u32HideZTPostVeh[1] = &g_asVehiclePerChannelForLoop[0][0];
		l_u32HideZTPostVeh[2] = &g_asReVehiclePerChannelForLoop[0][0];
		l_u32HideZTPostVeh[3] = &g_HideZTVehicleStruct[0][0];
		l_u32HideZTPostVeh[4] = &g_HideZTReVehicleStruct[0][0];
		l_u32HideZTPostVeh[5] = l_au8VehPositionFlag;
		l_u32HideZTPostVeh[6] = l_au8ReVehPositionFlag;
		l_u32HideZTPostVeh[7] = g_au32LoopCaTimeBack;
		FHideZTPostVehForNormal(l_u32HideZTPostVeh);
		/**** end ****/
		
		/**** �쳣�׳� ****/
		ReadFPGAGlobalTime(l_u32TimeNow);
		l_u32AddressForAbNorVeh[0]=l_au8VehPositionFlag;    //���������Ȧ��Ȧ��״̬
		l_u32AddressForAbNorVeh[1]=l_au8ReVehPositionFlag;  //����ǰ����Ȧ��Ȧ��״̬
		l_u32AddressForAbNorVeh[2]=&l_u32TimeNow;
		FPostVehForAbNormal(l_u32AddressForAbNorVeh);       //�쳣�׳�����
		/**** end ****/
		
		/**** �����׳� *****/
		ReadFPGAGlobalTime(l_u32TimeNow) ;
		l_u32AddressForVeh[0]=l_au8VehPositionFlag;         //���������Ȧ��Ȧ��״̬
		l_u32AddressForVeh[1]=l_au8ReVehPositionFlag;       //����ǰ����Ȧ��Ȧ��״̬
		l_u32AddressForVeh[2]=&l_u32TimeNow;
		FPostVehForNormal(l_u32AddressForVeh);              //�����׳�
		/**** end ****/


#if	TASKTIMEENABLE
		if(2==TaskRunTimeStatistics.m_ucWT_TASKFlag){//2:����״̬
		OSSchedLock();
		T1TCR=0x00;	
		TaskRunTimeStatistics.m_ulWT_TaskDet=(RUNTIMECOUNT(TaskRunTimeStatistics.m_ulWT_Task1)>TaskRunTimeStatistics.m_ulWT_TaskDet)?RUNTIMECOUNT(TaskRunTimeStatistics.m_ulWT_Task1):TaskRunTimeStatistics.m_ulWT_TaskDet;			 		
		T1TCR=0x01;	
		OSSchedUnlock();
		}
#endif 
	}//while end
 }
/*********************************************************************************************************
** Function name:		FInitVehStr
** Descriptions:		��Ȧ�߼�����չ�����Ϣ�ṹ��
** input parameters:	 
** output parameters:	
**
** Created by:			Wgh		  
** Created Date:		20140506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  zhangtonghan	
** Modified date:	  20170606	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FInitVehStr(VehicleInfForLoop_Struct * VehSt ,int8 Flag)
{
	uint8 l_tempi;
	VehSt->m_n8CurrentAxleNum = -1;
	VehSt->m_u8CrossHide=0;
	VehSt->m_u8DireFlag= Flag;
	VehSt->m_BigVehFollowLitVeh=0;
	for(l_tempi=0; l_tempi<ZHOUSIZE; l_tempi++)
		memset(VehSt->m_an16CurrentAxleInf[l_tempi], -1, 2*MAXNUM_AXLE);
	for(l_tempi=0; l_tempi<ZHOUSIZE; l_tempi++)
		memset(VehSt->m_an16HideCurAxleInf[l_tempi], -1, 2*MAXNUM_AXLE);
	memset(VehSt->m_au8AxleDropFlag, 0, ZHOUSIZE);
	memset(VehSt->m_au8NewAxleFlag, 0, ZHOUSIZE);
	memset(VehSt->m_an8AxleProRow, -1, ZHOUSIZE);
	VehSt->m_u8VehicleHaveFlag = 0;
	VehSt->m_u8FollowNewVehFlag = 0;
	VehSt->RunAndWalkFlag = 0;
	VehSt->m_n8XieZouFlag = 0;
	VehSt->m_u8SVSZ = 0;
	VehSt->u8psLane = 0;
	VehSt->u8ViolationCode = 0;    // Υ����
	VehSt->m_LitVehFollowBigVeh = 0;
	VehSt->m_u8VehIDFromLaser = 0;
	VehSt->m_u32TimeNow = 0;
	VehSt->m_u32BuildTime=0;
	VehSt->m_u32EndTime=0;
	memset(VehSt->n8accSpeed, 0, ZHOUSIZE);
	VehSt->m_n8CurrentHideAxleNum = -1;
	VehSt->u8OverFlag = 0;
	VehSt->u16speed = 0;
	memset(VehSt->u16AxleList, 0,2* ZHOUSIZE);
	memset(VehSt->u16HideAxleList, 0,2* ZHOUSIZE);
	VehSt->u8AixCnt = 0;
	VehSt->u8HideAixCnt =0;
	VehSt->u8AixGrpCnt = 0;
	VehSt->u8HideAixGrpCnt = 0;
	memset(VehSt->au8AxleTAIType, 0, ZHOUSIZE);
	memset(VehSt->au8AxleGrpType, 0, ZHOUSIZE);
	memset(VehSt->au16AxleWeight, 0, 2*ZHOUSIZE);
	memset(VehSt->au16AxleGrpWeight, 0, 2*ZHOUSIZE);
	memset(VehSt->au16AxleDistance, 0, 2*(ZHOUSIZE-1));
	memset(VehSt->au8HideAxleTAIType, 0, ZHOUSIZE);
	memset(VehSt->au8HideAxleGrpType, 0, ZHOUSIZE);
	memset(VehSt->au16HideAxleWeight, 0, 2*ZHOUSIZE);
	memset(VehSt->au16HideAxleGrpWeight, 0, 2*ZHOUSIZE);
	memset(VehSt->au16HideAxleDistance, 0, 2*(ZHOUSIZE-1));
	VehSt->m_u16VehWide = 0;
	VehSt->m_u16VehHeight = 0;
	VehSt->m_Capture_LaneNum = 0;   //ץ�ĳ���
	memset(VehSt->AxleStatus,0,7);    //�г��켣
	memset(VehSt->m_au8AxlePoint, 50, ZHOUSIZE);
	VehSt->m_u32LoopTime = 0;
	VehSt->u32HideYiWgt=0;
	VehSt->u32HideJCWgt=0;
	memset(VehSt->NormalAxleScore,0,sizeof(uint8)*7);
	memset(VehSt->HideAxleScore,0,sizeof(uint8)*7);
	VehSt->m_u8NormalLogiclvl = 0;
	VehSt->m_u8HideLogiclvl = 0;
}



/*********************************************************************************************************
** Function name:		FIO2ZT4Wave
** Descriptions:		IOת����ZT����
** input parameters:	 
** output parameters:	
**
** Created by:			Wujinlong		  
** Created Date:		20140517	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint32 FIO2ZT4Wave( void )
{
	uint16		l_u16tmp = 0;
	uint16		l_tempIndex = 0;
	uint8	subCommand = 0x00; 
	uint32		ret = 0;
	
	
	if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn)  //������
	{
		l_tempIndex = 0x00; 
		memset(&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex],0,sizeof(sndFrame_Struct));
		s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].u8Cnt = 1;		    //��ʼ��֡
		s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
		s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xFF;
		s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;  //�ӻ���ַ
		s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xAA;	 //�����

		subCommand = g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u8Flag;	  //��ȡ�ṹ���б�־
		switch(subCommand) //0x02;����λ�����ݣ� 0x03������IO���ݣ� 0x04����ȦIO����	 
		{
			case 0x02:
			{
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x02;	 //�����
				
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x6A;   //����֡����2�ֽ�

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;					
				for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp -1) * 8) & 0xFF;
				}

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;  //�����־λ
				
				for(l_u16tmp = 0; l_u16tmp < (LASTERCOUNT<<2); l_u16tmp++)  //����λ������
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
					                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u8type;
					
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
					                    (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Ltpos & 0xFF00) >>8;
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
					                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Ltpos & 0x00FF;

					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
					                    (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Rtpos & 0xFF00) >>8;
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] =
					                    g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].vehPositionInfo[l_u16tmp].m_u16Rtpos & 0x00FF;
					                  		
				}

				for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 // Ԥ���ֶ�
				}
				AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);	   // add by sxh 2014.3.29

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x6A;
				 
				break;
			}
			case 0x03:  //����IO
			{
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x03;	 //�����

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 //

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


			   	for(l_u16tmp = 4; l_u16tmp > 0; l_u16tmp--)   //ʱ��
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp - 1) * 8) & 0xFF;
				}
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // �����־IO
				
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
				                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState >> 8) & 0xFF;
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
				                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState ) & 0xFF;
				for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xA5;
				}

				AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x1c;
				break;
			}
			case 0x04:
			{
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x04;	 //�������

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x00;	 //����֡����2�ֽ�
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x1C;	 // �ݲ���ֵ�����ֵ

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode >>8) & 0xFF; //��ǰʹ��ģʽ
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = (g_u16SystemUseMode ) & 0xFF;


			   	for(l_u16tmp = 4; l_u16tmp >0; l_u16tmp--)   //ʱ��
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
					                   (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u32Tm >> (l_u16tmp-1) * 8) & 0xFF;
				}
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0x01;   // ��Ȧ��־IO
				
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
				                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState >> 8) & 0xFF;
				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 
				                                       (g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8ReadOut].m_u16GpioState ) & 0xFF;
				for(l_u16tmp = 0; l_u16tmp < 10; l_u16tmp++)
				{
					s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_tempIndex++] = 0xA5;
				}

				AddCrc16(s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0],l_tempIndex);

				s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0] = 0x1c;
				break;
			}
		}
		

		//l_err = OSQPost(g_pQNetWavePC,&s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex]);  //�׷��Ͳ��Σ�IO������λ�ã�
		//if(OS_Q_FULL == l_err)
		//	g_ErrMsgState |= 0x200;

		if( 1 == g_Setup.u8SendWaveEnable )
		{
			g_cLaserInfo[g_u8LaserInfoIx].u8Id = 0xAB;
			g_cLaserInfo[g_u8LaserInfoIx].u16ptCnt = s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au32len[0];
			for(l_u16tmp = 0; l_u16tmp < g_cLaserInfo[g_u8LaserInfoIx].u16ptCnt; l_u16tmp++)
			{
				g_cLaserInfo[g_u8LaserInfoIx].u16data[0][l_u16tmp] = s_CoilOrLaserGpioBuf[s_CoilOrLaserGpioIndex].au8data[0][l_u16tmp];
			}
			
//			ret = OSQPost(g_pQWaveMsg,&g_cLaserInfo[g_u8LaserInfoIx]);
			if(ret == OS_Q_FULL)
			{
				g_ErrMsgState |= 0x100;
			}
			//
			g_u8LaserInfoIx ++;
			g_u8LaserInfoIx %= LASERINFO_LEN;
		}

		g_CoilOrLaserData.m_u8ReadOut = (g_CoilOrLaserData.m_u8ReadOut + 1) % COILGPIODATALEN;   // ��λ������
		s_CoilOrLaserGpioIndex = (s_CoilOrLaserGpioIndex +1) % WAVE_BUFSIZE; 

	}//if (g_CoilOrLaserData.m_u8ReadOut != g_CoilOrLaserData.m_u8WriteIn) 


	return 0;

}
/*********************************************************************************************************
** Function name:		FCheckDeviceERR
** Descriptions:		��ѯ��ǰ�豸״̬
** input parameters:	 
** output parameters:	
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FCheckDeviceERR(void)
{
	uint8 l_u8tempi;
	if((g_u8IOERR[0] == 0) && (g_u8IOERR[1] == 0))
	{
		if((g_u8DevErr == 1) || (g_u8DevErr == 3))
		{
			g_u8DevErr = g_u8DevErr - 1;
		}
		else
		{
//			g_u8DevErr = 0;
		}
	}
	else
	{
		if((g_u8DevErr == 0) || (g_u8DevErr == 2))
		{
			g_u8DevErr ++;
		}
	}
	if((g_u8DeviceERR[0] & 0x0F) == 0)
	{
		if((g_u8DevErr == 2) || (g_u8DevErr == 3))
		{
			g_u8DevErr = g_u8DevErr - 2;
		}
		else
		{
//			g_u8DevErr = 0;
		}
	}
	else
	{
		if((g_u8DevErr != 2) && (g_u8DevErr != 3))
		{
			g_u8DevErr = g_u8DevErr + 2;
		}
	}
	if(g_u8NoMatchCnt > 100)
	{
		if((g_u8DevErr != 2) && (g_u8DevErr != 3))
		{
			g_u8DevErr = g_u8DevErr + 2;
		}
		g_u8NoMatchCnt = 10;
	}
	if(g_u8DevErr == 0)
	{
		g_u8NoMatchCnt = 0;
	}
	if(g_u8DevErr == 2)
	{
		for(l_u8tempi = 0; l_u8tempi<20; l_u8tempi++)
		{
			g_n8ZTRang[l_u8tempi] = -1;
		}
	}
}
/*********************************************************************************************************
** Function name:		FRecordIO
** Descriptions:		��¼IO״̬
** input parameters:	��¼IO״̬���� 
** output parameters:	��¼IO״̬���� 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FRecordIO(uint8* pBuf, uint8* pBuf1)
{
	uint8 l_u8Buf[16];
	uint8 l_u8tempi;
	uint8 l_u8Index;

	l_u8Index = 0;
	
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
		pBuf[l_u8tempi]=1;
		l_u8Buf[l_u8tempi]=1;
	}

	if(g_u8NumUseLoopBoard == 0)
	{
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[0]]=CoilIN_IO1;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[1]]=CoilIN_IO2;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[2]]=CoilIN_IO3;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[3]]=CoilIN_IO4;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[4]]=CoilIN_IO5;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[5]]=CoilIN_IO6;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[6]]=CoilIN_IO7;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[7]]=CoilIN_IO8;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[8]]=CoilIN_IO9;
		l_u8Buf[PROMODPARAM.m_au8IOUseEn[9]]=CoilIN_IO10;
		l_u8Buf[10]= 1;
		l_u8Buf[11]= 1;
		l_u8Buf[12]= 1;
		l_u8Buf[13]= 1;
		l_u8Buf[14]= 1;
		l_u8Buf[15]= 1;
	}
	else
	{
		l_u8Buf[0]=g_u8IOStateForBoard[0];
		l_u8Buf[1]=g_u8IOStateForBoard[1];
		l_u8Buf[2]=g_u8IOStateForBoard[2];
		l_u8Buf[3]=g_u8IOStateForBoard[3];
		l_u8Buf[4]=g_u8IOStateForBoard[4];
		l_u8Buf[5]=g_u8IOStateForBoard[5];
		l_u8Buf[6]=g_u8IOStateForBoard[6];
		l_u8Buf[7]=g_u8IOStateForBoard[7];
		l_u8Buf[8]=g_u8IOStateForBoard[8];
		l_u8Buf[9]=g_u8IOStateForBoard[9];
		l_u8Buf[10]=g_u8IOStateForBoard[10];
		l_u8Buf[11]=g_u8IOStateForBoard[11];
		l_u8Buf[12]=g_u8IOStateForBoard[12];
		l_u8Buf[13]=g_u8IOStateForBoard[13];
		l_u8Buf[14]=g_u8IOStateForBoard[14];
		l_u8Buf[15]=g_u8IOStateForBoard[15];
	}
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
//		if(PROMODPARAM.m_au8IOUseEn[l_u8tempi] == 0 && l_u8tempi<10)
//		{
////			continue;
//		}
//		else
		{
			pBuf1[l_u8Index] = l_u8Buf[l_u8tempi];
			l_u8Index ++;
		}
	}
	if(g_u8DevErr == 1)
	{
		for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
		{
			l_u8Buf[l_u8tempi] = (1-g_au8LoopStatePre[l_u8tempi]);
		}
	}
	l_u8Index = 0;
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
//		if(PROMODPARAM.m_au8IOUseEn[l_u8tempi] == 0 && l_u8tempi<10)
//		{
////			continue;
//		}
//		else
		{
			pBuf[l_u8Index] = l_u8Buf[l_u8tempi];
			l_u8Index ++;
		}
	}
}
/*********************************************************************************************************
** Function name:		FRecordIO_Laser
** Descriptions:		��¼����ģ���IO״̬
** input parameters:	��¼IO״̬���� 
** output parameters:	��¼IO״̬���� 
**
** Created by:			liumingxin	  
** Created Date:		20170705	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FRecordIO_Laser(uint8* pBuf, uint8* pBuf1)
{
    uint8  l_u8Buf[16];
    uint8  l_u8tempi, l_u8tempj;            // ��ʱ����
    uint8  l_u8MatchColumLeft[10] = {0};	// ��¼һ��������ȷ�������խ�������У����10����
    uint8  l_u8MatchColumRight[10] = {0};	// ��¼һ��������ȷ�����Ҳ�խ�������У����10����
    int8   l_u8VehNum = -1;                 // ��¼����ȷ���ĳ�����ţ���0��ʼ
	
    
    if(g_u8RasterDataReadIndex != g_u8RasterDataWriteIndex)
    {
		if(g_RasterData[g_u8RasterDataReadIndex].VehChangeFlag == 1)
		{
			// ��ʼ��IO״̬����
			for(l_u8tempi = 0; l_u8tempi < 16; l_u8tempi++)
			{
				pBuf[l_u8tempi] = 1;
				l_u8Buf[l_u8tempi] = 1;
			}
			/********************���ݼ�����ĳ���λ���ж�����խ���ķ�Χ********************/
			for(l_u8tempi = 0; l_u8tempi < SP_LASER_VEHNUM_MAX; l_u8tempi++)    
			{
				for(l_u8tempj = 0; l_u8tempj < PROMODPARAM.m_u8LaneNum*2; l_u8tempj++)    // ����ȫ���У��ҵ���������λ��������
				{
					// ��������������������λ����������ĳһ��խ����Χ��, ������խ����Ϊ�ó������խ��
					if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16Ltpos + PROMODPARAM.m_u8JGKZ) >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
						&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16Ltpos + PROMODPARAM.m_u8JGKZ) <= PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
						&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type ) & 0x0C) != 0))
					{
						l_u8VehNum ++;
						l_u8MatchColumLeft[l_u8VehNum] = l_u8tempj;    // ��¼�����λ��������
					}
					else if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16Ltpos + PROMODPARAM.m_u8JGKZ) > PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
						    && l_u8tempj == (PROMODPARAM.m_u8LaneNum*2-1))
					{
						l_u8VehNum ++;
						l_u8MatchColumLeft[l_u8VehNum] = l_u8tempj;    // ��¼�����λ��������
					}
					// ��������������������λ����������ĳһ��խ����Χ��, ������խ����Ϊ�ó������խ��
					if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos - PROMODPARAM.m_u8JGKZ) >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
						&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos - PROMODPARAM.m_u8JGKZ) <= PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
						&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type ) & 0x0C) != 0)
					    && g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos >= PROMODPARAM.m_u8JGKZ)
					{
						l_u8MatchColumRight[l_u8VehNum] = l_u8tempj;    // ��¼���Ҳ�λ��������
						break;
					}
					else if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos) >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
						&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos) <= PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
						&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type ) & 0x0C) != 0)
					    && g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u16rtpos < PROMODPARAM.m_u8JGKZ)
					{
						l_u8MatchColumRight[l_u8VehNum] = l_u8tempj;    // ��¼���Ҳ�λ��������
						break;
					}
					else if(l_u8tempj == (PROMODPARAM.m_u8LaneNum*2-1))
					{
						l_u8MatchColumRight[l_u8VehNum] = l_u8tempj;    // ��¼���Ҳ�λ��������
						break;
					}
				}
				// ȷ����������λ�������е������������IO״̬
			   if(((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type) & 0x0C) == 0x04)       // ����
			   {
				   if(l_u8MatchColumRight[l_u8VehNum] < 20)   // ��ֹԽ��
				   {
					   l_u8Buf[(l_u8MatchColumLeft[l_u8VehNum]/2)*2 + 1] = 0;    // ���ж�Ӧ���������IO״̬
					   l_u8Buf[(l_u8MatchColumRight[l_u8VehNum]/2)*2 + 1] = 0;   // ���ж�Ӧ���������IO״̬
				   }
			   }
			   else if(((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type) & 0x0C) == 0x08)  // ��β
			   {
	// 			   if((l_u8MatchColumLeft[l_u8VehNum] >= 0) && (l_u8MatchColumRight[l_u8VehNum] < 20))   // ��ֹԽ��
	// 			   {
	//                    l_u8Buf[(l_u8MatchColumLeft[l_u8VehNum]/2)*2 + 1] = 1;    // ���ж�Ӧ���������IO״̬
	//                    l_u8Buf[(l_u8MatchColumRight[l_u8VehNum]/2)*2 + 1] = 1;   // ���ж�Ӧ���������IO״̬
	// 			   }
			   }
			   else if(((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8tempi].u8type) & 0x0C) == 0x0C)  // ����
			   {
				   if(l_u8MatchColumRight[l_u8VehNum] < 20)   // ��ֹԽ��
				   {
					   l_u8Buf[(l_u8MatchColumLeft[l_u8VehNum]/2)*2 + 1] = 0;    // ���ж�Ӧ���������IO״̬
					   l_u8Buf[(l_u8MatchColumRight[l_u8VehNum]/2)*2 + 1] = 0;   // ���ж�Ӧ���������IO״̬
				   }
			   }
			}
		}
		else
		{
			for(l_u8tempi = 0; l_u8tempi< 16; l_u8tempi ++)
			{
				l_u8Buf[l_u8tempi] = g_au8LoopStatePreOnlyCheck[l_u8tempi];
			}
		}
		g_u8RasterDataReadIndex ++;
    }
	else
	{
		for(l_u8tempi = 0; l_u8tempi< 16; l_u8tempi ++)
		{
			l_u8Buf[l_u8tempi] = g_au8LoopStatePreOnlyCheck[l_u8tempi];
		}
	}
	
    for(l_u8tempi = 0; l_u8tempi < 16; l_u8tempi++)
    {
        pBuf1[l_u8tempi] = l_u8Buf[l_u8tempi];
		pBuf[l_u8tempi] = l_u8Buf[l_u8tempi];
    }
}
/*********************************************************************************************************
** Function name:		FLaserProcess
** Descriptions:		������Ϣ����
** input parameters:	IO״̬����,��ǰʱ�� 
** output parameters:	IO״̬���� 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FLaserProcess(uint8* pBuf, uint32 pTime)
{
	uint8  l_u8MatchLRow[10] = {0};	//��¼һ��������ȷ�������խ�������У����10����
	uint8  l_u8MatchRRow[10] = {0};	//��¼һ��������ȷ�����Ҳ�խ�������У����10����
	int8   l_n8MatchInd = -1; //��¼����ȷ���ĳ���������0��ʼ
	uint8  l_u8MatchRow[10] = {0}; //һ����������ĳ���ID
	uint8  l_u8YFBelong[10] = {0};//��¼ѹ��խ������������������1���Ͳ���¼����
	uint8  l_u8YFVehID[10] = {0};//ѹ��խ�����������³���ID
	uint16 l_au16Tempi;
	uint8  l_u8tempj;
//	uint8  l_u8Err;
//	uint16 l_u16SendLen;
	uint32 l_u32TimeNow;//��¼��ǰʱ��
	uint8  l_u8Flag;//�Ƿ��н�������

//	uint8	l_u8Err;
//	uint16	l_u16SendLen;

	l_u32TimeNow = pTime;
	if(g_u8RasterDataWriteIndex != g_u8RasterDataReadIndex)
	{
		l_n8MatchInd = -1;
		for(l_au16Tempi=0; l_au16Tempi<8; l_au16Tempi++)
		{
			if(g_u8DevErr != 2 && g_u8DevErr != 3)  //�豸����������Ȧ���Ͻ���
			{
				/**************************���ݼ�����ĳ���λ���ж�����խ���ķ�Χ����¼��������խ��������к��Ҳ����Լ���Ӧ�ļ�����ĳ���ID**********************************/
				for(l_u8tempj=0; l_u8tempj<2*PROMODPARAM.m_u8LaneNum; l_u8tempj++)
				{
					if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos-PROMODPARAM.m_u8JGKZ)>PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
					&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos-PROMODPARAM.m_u8JGKZ)<=PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
					&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C)!=0)
					&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos > PROMODPARAM.m_u8JGKZ) )//��������������������λ����������ĳһ��խ����Χ�ڣ�������խ����Ϊ�ó������խ����¼������¼����
					{
						l_n8MatchInd ++;
						l_u8MatchLRow[l_n8MatchInd] = l_u8tempj;
						l_u8MatchRow[l_n8MatchInd] = l_au16Tempi;
					}
					else if(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos<=PROMODPARAM.m_u8JGKZ
					&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C)!=0)
					&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos > PROMODPARAM.m_u16LRLocation[0][l_u8tempj]))//��������������������λ����������ĳһ��խ����Χ�ڣ�������խ����Ϊ�ó������խ����¼������¼����
					{
						l_n8MatchInd ++;
						l_u8MatchLRow[l_n8MatchInd] = 0;
						l_u8MatchRow[l_n8MatchInd] = l_au16Tempi;
					}
					if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16rtpos+PROMODPARAM.m_u8JGKZ)>PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
					&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16rtpos+PROMODPARAM.m_u8JGKZ)<=PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
					&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C)!=0) )//��������������������λ����������ĳһ��խ����Χ�ڣ�������խ����Ϊ�ó����Ҳ�խ����¼
					{
						l_u8MatchRRow[l_n8MatchInd] = l_u8tempj;
					}
				}
				/**********************************************************/
				/*************************�����ѹ��խ������ѹ��խ��������������ÿ����1����1********************************/
				if(PROMODPARAM.m_u8IsYaFeng == 1)
				{
					for(l_u8tempj=0; l_u8tempj<PROMODPARAM.m_u8LaneNum; l_u8tempj++)
					{
						if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos-PROMODPARAM.m_u8JGKZ)>PROMODPARAM.m_u16LRLocation[0][2*PROMODPARAM.m_u8LaneNum+l_u8tempj]
						&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16Ltpos-PROMODPARAM.m_u8JGKZ)<=PROMODPARAM.m_u16LRLocation[1][2*PROMODPARAM.m_u8LaneNum+l_u8tempj]
						&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C)!=0) )
						{
							l_u8YFBelong[l_u8tempj] ++;
							
							l_u8YFVehID[l_u8tempj] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehID;
						}
						if((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16rtpos+PROMODPARAM.m_u8JGKZ)>PROMODPARAM.m_u16LRLocation[0][2*PROMODPARAM.m_u8LaneNum+l_u8tempj]
						&& (g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u16rtpos+PROMODPARAM.m_u8JGKZ)<=PROMODPARAM.m_u16LRLocation[1][2*PROMODPARAM.m_u8LaneNum+l_u8tempj]
						&& (((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C)!=0) )
						{
							l_u8YFBelong[l_u8tempj] ++;

							l_u8YFVehID[l_u8tempj] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehID;
							
						}
					}
				}
				/*********************************************************/
			}
			/*************���ݼ�����Ĵ���״̬���޸ļ�¼��IO״̬��ģ��IO��������βʱ������Ȧ��ʱ��β**************************/
			if(g_u8DevErr == 1 && g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane > 0)
			{
				if(0x08 == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))//����������ĳ��������β�źţ����˫��Ȧģʽǰ����Ȧģ��IO��β��������Ȧ��¼��ǰʱ�������ʱ��β���������Ȧ���¼ʱ����ʱ��β
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
					}
					g_u8JGSWFlag[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
					g_u8JGSWVehID[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehID;
					g_u32JGSWTime[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = l_u32TimeNow;
//					for(l_u8tempj=0; l_u8tempj<2*PROMODPARAM.m_u8LaneNum; l_u8tempj++)
//					{
//						if(g_n8ZTRang[l_u8tempj] == g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehID)
//						{
//							g_n8ZTRang[l_u8tempj] = -1;
//						}
//					}
				}
				else if(0x04 == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))//����������ĳ�����������źţ����۵�˫��Ȧ�ó�������Ȧ��ģ����ȦIO����
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
					}
					else if(PROMODPARAM.m_u8TriggerMode == 2)
					{
						pBuf[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
						g_au8LoopStatePre[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
					}
				}
				else if(0x0C == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))//���������˱����źţ��ó���������Ȧ��ģ��IO���ִ���״̬
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 1;
					}
					else if(PROMODPARAM.m_u8TriggerMode == 2)
					{
						pBuf[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
						g_au8LoopStatePre[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 1;
					}
				}
				else //���������ģ��IOδ����״̬
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 1;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
						g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
					}
					else if(PROMODPARAM.m_u8TriggerMode == 2)
					{
						pBuf[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 1;
						g_au8LoopStatePre[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
					}
				}
			}
			else if(g_u8DevErr == 0 && g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane > 0)
			{
//				if(0x04 == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))//����������ĳ�����������źţ����۵�˫��Ȧ�ó�������Ȧ��ģ����ȦIO����
//				{
//					if(PROMODPARAM.m_u8TriggerMode == 4)
//					{
//						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
////						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
//					}
//					else if(PROMODPARAM.m_u8TriggerMode == 2)
//					{
//						pBuf[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
//					}
//				}
//				else if(0x0C == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))//���������˱����źţ��ó���������Ȧ��ģ��IO���ִ���״̬
//				{
////					if(PROMODPARAM.m_u8TriggerMode == 4)
////					{
////						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 0;
////						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)+1] = 0;
////					}
////					else if(PROMODPARAM.m_u8TriggerMode == 2)
////					{
////						pBuf[g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1] = 0;
////					}
//				}
//				else if(0x08 == ((g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].u8type ) & 0x0C))
//				{
//					g_u8JGSWFlag[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1;
//					g_u8JGSWVehID[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehID;
//					g_u32JGSWTime[(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_au16Tempi].m_u8VehLane-1)] = l_u32TimeNow;
//				}
			}
		}
		/********************************************************/
		/**********************���ݼ���ȷ��������������խ���󣬸��¸ó�խ����Χ**********************************/
		if(l_n8MatchInd > -1)
		{
			/*************************��������������й������֣��������жϹ���*******************************/
			for(l_au16Tempi=0; l_au16Tempi<l_n8MatchInd; l_au16Tempi++)
			{
				l_u8Flag = 0;
				if(l_u8MatchRRow[l_au16Tempi] >= l_u8MatchLRow[l_au16Tempi+1])//����������н�������
				{
					if(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16rtpos>PROMODPARAM.m_u16LRLocation[0][l_u8MatchRRow[l_au16Tempi]]
					&& g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi+1]].u16Ltpos>PROMODPARAM.m_u16LRLocation[1][l_u8MatchRRow[l_au16Tempi]])//�ü������λ�ý����жϣ������೵���Ҳ�λ�����жϵ�խ���ڣ��Ҳ೵����λ�ò����жϵ�խ���ڣ����Ҳ೵���խ������1��
					{
						l_u8MatchLRow[l_au16Tempi+1] = l_u8MatchRRow[l_au16Tempi]+1;
					}
					else if(g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16rtpos<PROMODPARAM.m_u16LRLocation[0][l_u8MatchLRow[l_au16Tempi+1]]
					&& g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi+1]].u16Ltpos<PROMODPARAM.m_u16LRLocation[1][l_u8MatchLRow[l_au16Tempi+1]])//�ü������λ�ý����жϣ������೵���Ҳ�λ�ò����жϵ�խ���ڣ��Ҳ೵����λ�����жϵ�խ���ڣ�����೵�Ҳ�խ������1��
					{
						l_u8MatchRRow[l_au16Tempi] = l_u8MatchLRow[l_au16Tempi+1]-1;
					}
					else if(l_u8MatchRRow[l_au16Tempi] == l_u8MatchLRow[l_au16Tempi+1])//�����೵���Ҳ�խ�����Ҳ೵�����խ��һ��
					{
						l_u8Flag = 1;
					}
					else//������������ܲ������������೵��խ��������������խ��ȡΪ�Ҳ೵���խ����ǰһ�У���֮�Ҳ೵�����ȡΪ��೵�Ҳ�խ���ĺ�һ��
					{
						l_u8Flag = 1;
					}
					/**********************����ȷ���ĳ�������խ����������ÿһ�ж���¼���ڵĳ���ID*************************************/
					for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
					{
//						if(l_u8MatchLRow[l_au16Tempi] == l_u8MatchRRow[l_au16Tempi])
//						{
//							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Left=%d��right=%d��\r\n",
//									           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
//											   g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16Ltpos,
//											   g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16rtpos );
//							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//							g_u16CurDebugInd++;
//							g_u16CurDebugInd %= COM_NUM;
//							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//							OSSemPost(g_pSemSendThtouSP);
//						}
						if(l_u8tempj >= l_u8MatchLRow[l_au16Tempi] && l_u8tempj <= l_u8MatchRRow[l_au16Tempi])
						{
							if(l_u8Flag == 1)
							{
								g_n8ZTRang[l_u8tempj] = -1;
							}
							else
							{
								g_n8ZTRang[l_u8tempj] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].m_u8VehID;
							}
						}
						else if(g_n8ZTRang[l_u8tempj] == g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].m_u8VehID)
						{
//							g_n8ZTRang[l_u8tempj] = -1;
						}
					}
					/************************************************************/
				}
				else
				{
					/**********************����ȷ���ĳ�������խ����������ÿһ�ж���¼���ڵĳ���ID*************************************/
					for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
					{
//						if(l_u8MatchLRow[l_au16Tempi] == l_u8MatchRRow[l_au16Tempi])
//						{
//							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
//							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%04u-%02u-%02u %02u:%02u:%02u TLaser Left=%d��right=%d��\r\n",
//									           g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
//										       g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second, 
//											   g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16Ltpos,
//											   g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].u16rtpos );
//							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
//							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
//							g_u16CurDebugInd++;
//							g_u16CurDebugInd %= COM_NUM;
//							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
//							OSSemPost(g_pSemSendThtouSP);
//						}
						if(l_u8tempj >= l_u8MatchLRow[l_au16Tempi] && l_u8tempj <= l_u8MatchRRow[l_au16Tempi])
						{
							g_n8ZTRang[l_u8tempj] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].m_u8VehID;
						}
						else if(g_n8ZTRang[l_u8tempj] == g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_au16Tempi]].m_u8VehID)
						{
//							g_n8ZTRang[l_u8tempj] = -1;
						}
					}
					/************************************************************/
				}
			}
			/*********************************************************/
			/**********************����ȷ�������һ����������խ����������ÿһ�ж���¼���ڵĳ���ID*************************************/
			for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
			{
				if(l_u8tempj >= l_u8MatchLRow[l_au16Tempi] && l_u8tempj <= l_u8MatchRRow[l_au16Tempi])
				{
					g_n8ZTRang[l_u8tempj] = g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_n8MatchInd]].m_u8VehID;
				}
				else if(g_n8ZTRang[l_u8tempj] == g_RasterData[g_u8RasterDataReadIndex].headinfo[l_u8MatchRow[l_n8MatchInd]].m_u8VehID)
				{
//					g_n8ZTRang[l_u8tempj] = -1;
				}
			}
			/************************************************************/
			/***************************************************************/
		}
		/*********************ȷ��ѹ��խ������**************************/
		if(PROMODPARAM.m_u8IsYaFeng == 1)
		{
			for(l_u8tempj=0; l_u8tempj<PROMODPARAM.m_u8LaneNum; l_u8tempj++)
			{
				if(l_u8YFBelong[l_u8tempj] != 1)
				{
					g_n8ZTRang[2*PROMODPARAM.m_u8LaneNum+l_u8tempj] = -1;
				}
				else
				{
					g_n8ZTRang[2*PROMODPARAM.m_u8LaneNum+l_u8tempj] = l_u8YFVehID[l_u8tempj];
				}
			}
		}
		/************************************************/
		g_u8RasterDataReadIndex ++;
	}
	else
	{
		for(l_au16Tempi=0; l_au16Tempi<8; l_au16Tempi++)
		{
			if(g_u8DevErr == 0 && g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane > 0)
			{
				if(0x04 == ((g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].u8type ) & 0x0C))//����������ĳ�����������źţ����۵�˫��Ȧ�ó�������Ȧ��ģ����ȦIO����
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1-g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1)];
					}
					else if(PROMODPARAM.m_u8TriggerMode == 2)
					{
						pBuf[g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1] = 1-g_au8LoopStatePre[g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1];
					}
				}
				else if(0x0C == ((g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].u8type ) & 0x0C))//���������˱����źţ��ó���������Ȧ��ģ��IO���ִ���״̬
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						pBuf[2*(g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1)] = 1-g_au8LoopStatePre[2*(g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1)];
					}
					else if(PROMODPARAM.m_u8TriggerMode == 2)
					{
						pBuf[g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1] = 1-g_au8LoopStatePre[g_RasterData[g_u8RasterDataReadIndex-1].headinfo[l_au16Tempi].m_u8VehLane-1];
					}
				}
			}
		}
	}
	if(g_u8DevErr == 1)
	{
		for(l_au16Tempi=0; l_au16Tempi<10; l_au16Tempi++)
		{
			if(g_u8JGSWFlag[l_au16Tempi]==1)//�ж���Ȧ�Ƿ��к�����Ȧ��ʱ��β�ģ�����г�����ֵģ��IO��β
			{
				if(tickGetDiff(l_u32TimeNow, g_u32JGSWTime[l_au16Tempi]) > PROMODPARAM.m_au32Thres[15])
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						if(l_au16Tempi < 5)
						{
							pBuf[2*l_au16Tempi+1] = 1;
							g_au8LoopStatePre[2*l_au16Tempi+1] = 1;
						}
						g_u8JGSWFlag[l_au16Tempi] = 0;
						for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
						{
							if(g_n8ZTRang[l_u8tempj] == g_u8JGSWVehID[l_au16Tempi])
							{
								g_n8ZTRang[l_u8tempj] = -1;
							}
						}
						g_u8JGSWVehID[l_au16Tempi] = 0;
					}
					else
					{
						pBuf[l_au16Tempi] = 1;
						g_au8LoopStatePre[l_au16Tempi] = 1;
						g_u8JGSWFlag[l_au16Tempi] = 0;
						for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
						{
							if(g_n8ZTRang[l_u8tempj] == g_u8JGSWVehID[l_au16Tempi])
							{
								g_n8ZTRang[l_u8tempj] = -1;
							}
						}
						g_u8JGSWVehID[l_au16Tempi] = 0;
					}
				}
			}
		}
	}
	else if(g_u8DevErr == 0)
	{
		for(l_au16Tempi=0; l_au16Tempi<10; l_au16Tempi++)
		{
			if(g_u8JGSWFlag[l_au16Tempi]==1)//�ж���Ȧ�Ƿ��к�����Ȧ��ʱ��β�ģ�����г�����ֵģ��IO��β
			{
				if(tickGetDiff(l_u32TimeNow, g_u32JGSWTime[l_au16Tempi]) > PROMODPARAM.m_au32Thres[15])
				{
					if(PROMODPARAM.m_u8TriggerMode == 4)
					{
						if(l_au16Tempi < 5)
						{
//							pBuf[2*l_au16Tempi+1] = 1;
//							g_au8LoopStatePre[2*l_au16Tempi+1] = 1;
						}
						g_u8JGSWFlag[l_au16Tempi] = 0;
						for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
						{
							if(g_n8ZTRang[l_u8tempj] == g_u8JGSWVehID[l_au16Tempi])
							{
								g_n8ZTRang[l_u8tempj] = -1;
							}
						}
						g_u8JGSWVehID[l_au16Tempi] = 0;
					}
					else
					{
						g_u8JGSWFlag[l_au16Tempi] = 0;
						for(l_u8tempj=0; l_u8tempj<20; l_u8tempj++)
						{
							if(g_n8ZTRang[l_u8tempj] == g_u8JGSWVehID[l_au16Tempi])
							{
								g_n8ZTRang[l_u8tempj] = -1;
							}
						}
						g_u8JGSWVehID[l_au16Tempi] = 0;
					}
				}
			}
		}
	}
}
/*********************************************************************************************************
** Function name:		FSendWaveBE
** Descriptions:		���η��Ϳ�ʼ����β
** input parameters:	IO״̬����,��ǰʱ�� 
** output parameters:	 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FSendWaveBE(uint8* pBuf, uint32 pTime, uint8 judgeflag)
{
	uint16 l_au16LoopStateAll = 0x0000;   // add by sxh 2014.4.01
	uint16 l_au16PreLoopStateAll = 0x0000;  // add by sxh 2014.4.01
	uint8*  l_au8LoopIn;
	uint8  l_auLoopSTate1[16];
	uint16 l_au16Tempi;
	uint16  l_u16SendLen;
	uint8   l_u8Err;
	//static uint32 l_u32DelTime=0;
//	uint32  l_u32Ret;
	uint32 l_u32TimeNow;
//#if OS_Q_QUERY_EN > 0
//	OS_Q_DATA 	l_cQData;
//#endif
	memset(l_auLoopSTate1,0x00,sizeof(l_auLoopSTate1));  // add by sxh ;2014.4.21
	l_u32TimeNow = pTime;
	l_au8LoopIn = pBuf;
	for(l_au16Tempi = 0; l_au16Tempi < 16; l_au16Tempi++)
	{
//	 	  l_au8LoopIn[l_au16Tempi] = pBuf[l_au16Tempi];
		  if(l_au8LoopIn[l_au16Tempi] == 0)
		  {
				l_auLoopSTate1[l_au16Tempi] = 1;
		  }
		  else 
		  {
				l_auLoopSTate1[l_au16Tempi] = 0;
		  }
		  
	}
	// add by sxh 2014.4.01
	l_au16PreLoopStateAll = 0x0000;
	for(l_au16Tempi = 0; l_au16Tempi < 16; l_au16Tempi++)   //�洢ǰ��ʱ����Ȧ״̬	 
	{
		l_au16PreLoopStateAll |= ((g_au8LoopStatePre[l_au16Tempi] ^ 0x01)<<l_au16Tempi) ;	
	}

	/* add by sxh 2014.4.1����Ȧ�иı䣬�ͼ�¼��ǰIO״̬���ڲ��������з���*/
	l_au16LoopStateAll = 0x0000;
	//
	for(l_au16Tempi = 0; l_au16Tempi < 16; l_au16Tempi++)
	{
		l_au16LoopStateAll |= ((l_auLoopSTate1[l_au16Tempi] ^ 0x01)<<l_au16Tempi);	//λ��	
	}
	
	if(g_u8SendWave == 0 && PROMODPARAM.m_u8TriggerMode == 4 && SETUPALIAS.u8SendWaveEnable == 1 
			&& l_au16PreLoopStateAll!=l_au16LoopStateAll && l_au16PreLoopStateAll==0xFFFF  )
	{
//#if OS_Q_QUERY_EN > 0
//		// ����ͳ�ƣ������д���ݣ����־��1
//		memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
//		l_u32Ret = OSQQuery( g_pQWaveMsg, &l_cQData );   //g_pQWaveMsg	 g_pQNetWavePC
//	 	if( OS_NO_ERR == l_u32Ret )
//		{
//			if( 0 == l_cQData.OSNMsgs )
//			{
				g_u8SendWave = 1;
				//FSendCmd2PC_ZONG(2,ADJ_EXT);
#if DEBUGINFO_TASK_SP_EN > 0		
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\n**********%04u-%02u-%02u %02u:%02u:%02u,8F02=%d,%d,%04X,%04X.",
							            g_DevCurTime.u16Year, g_DevCurTime.u8Month, g_DevCurTime.u8Day,
										g_DevCurTime.u8Hour, g_DevCurTime.u8Minute, g_DevCurTime.u8Second,
										g_u8SendWave, g_u8DetVeh, l_au16LoopStateAll, l_au16PreLoopStateAll);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
#endif
//			}
//		}
//#endif
//		
	}

	if(l_au16PreLoopStateAll != l_au16LoopStateAll)  //��ȦIO�仯
	{
		g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u8Flag = 0x04;	 //����IO����
		g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u16GpioState = l_au16LoopStateAll;  
		g_CoilOrLaserData.CoilOrLaserData[g_CoilOrLaserData.m_u8WriteIn].m_u32Tm = l_u32TimeNow;
		g_CoilOrLaserData.m_u8WriteIn = (g_CoilOrLaserData.m_u8WriteIn+1) % COILGPIODATALEN;

//		FIO2ZT4Wave();	//20140517

	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nIO XQ=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%04X,%04X.",
					            l_au8LoopIn[0],l_au8LoopIn[1],l_au8LoopIn[2],l_au8LoopIn[3],l_au8LoopIn[4],
								l_au8LoopIn[5],l_au8LoopIn[6],l_au8LoopIn[7],l_au8LoopIn[8],l_au8LoopIn[9],
								l_au8LoopIn[10],l_au8LoopIn[11],l_au8LoopIn[12],l_au8LoopIn[13],l_au8LoopIn[14],
								l_au8LoopIn[15],judgeflag,l_u32TimeNow,l_au16LoopStateAll,l_au16PreLoopStateAll );
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		///////////////////////////////
		g_sndNetDebug[g_u8CurNetDebugIndex].u8Cnt = 1;
		g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0] = 34;				
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][0] = 0xFF;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][1] = 0xFF;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][2] = 0x00;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][3] = 0x70;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][4] = 0x0E;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][5] = 0x00;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][6] = 34;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][7] = ((g_DevCurTime.u16Year>>8)&0xFF);
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][8] = (g_DevCurTime.u16Year&0xFF);
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][9] = g_DevCurTime.u8Month;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][10] = g_DevCurTime.u8Day;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][11] = g_DevCurTime.u8Hour;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][12] = g_DevCurTime.u8Minute;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][13] = g_DevCurTime.u8Second;
		
		for(l_au16Tempi = 0; l_au16Tempi < 16; l_au16Tempi++) 
		{
			g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][14+l_au16Tempi] = l_au8LoopIn[l_au16Tempi];
		}

		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][30] = 0x00;
		g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0][31] = 0x00;
		
		AddCrc16(g_sndNetDebug[g_u8CurNetDebugIndex].au8data[0],g_sndNetDebug[g_u8CurNetDebugIndex].au32len[0]-2);
		OSQPost( g_pQSendPCDebug, &g_sndNetDebug[g_u8CurNetDebugIndex] );
		g_u8CurNetDebugIndex++;
		g_u8CurNetDebugIndex %= NETDEBUG_SIZE;
		//////////////////////////////
		OSSemPost(g_pSemSendThtouSP);
	#endif 
				
	}
	/* add by sxh 2014.4.1����Ȧ�иı䣬�ͼ�¼��ǰIO״̬���ڲ��������з���*/



	if((g_u8SendWave == 1 || g_u8SendWave == 2) && SETUPALIAS.u8SendWaveEnable == 1)
	{
		if( l_au8LoopIn[0]==1 && l_au8LoopIn[1]==1 && l_au8LoopIn[2]==1 && l_au8LoopIn[3]==1 && l_au8LoopIn[4]==1
		 	&& l_au8LoopIn[5]==1 && l_au8LoopIn[6]==1 && l_au8LoopIn[7]==1 && l_au8LoopIn[8]==1 && l_au8LoopIn[9]==1
			&& l_au8LoopIn[10]==1 && l_au8LoopIn[11]==1 && l_au8LoopIn[12]==1 && l_au8LoopIn[13]==1 && l_au8LoopIn[14]==1
			&& l_au8LoopIn[15]==1 && g_u8DetVeh==0)
		 {
//#if OS_Q_QUERY_EN > 0
//			// ����ͳ�ƣ������д���ݣ����־��1
//			memset( &l_cQData, 0, sizeof(OS_Q_DATA) );
//			l_u32Ret = OSQQuery( g_pQWaveMsg, &l_cQData );  
//		 	if( OS_NO_ERR == l_u32Ret )
//			{
//				if( 0 == l_cQData.OSNMsgs )
//				{
					g_u8SendWave = 0;
					//FSendCmd2PC_ZONG(1,ADJ_EXT);
					
// #if DEBUGINFO_TASK_SP_EN > 0
// 				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"@@@@@@@@@@@@@@@8F01=%d, %d, %d, %d��\r\n",
// 							            g_u8SendWave, g_u8DetVeh, l_au16LoopStateAll, l_au16PreLoopStateAll);
// 				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 				g_u16CurDebugInd++;
// 				g_u16CurDebugInd %= COM_NUM;
// 				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 				OSSemPost(g_pSemSendThtouSP);
// #endif
//				}
//			}
//#endif
			
		 }
	}
}
/*********************************************************************************************************
** Function name:		FVehPassStatus
** Descriptions:		��������״̬�ж�
** input parameters:	IO״̬����,�������״̬���������״̬ 
** output parameters:	�������״̬���������״̬ 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:		FVehXieZouStatus
** Descriptions:		����б���ж�
** input parameters:	ָ�����飺խ���ṹ��ָ�롢Forward_ZTColum�ĵ�ַ��Reverse_ZTColum�ĵ�ַ��g_asVehiclePerChannelForLoop�ĵ�ַ��g_asReVehiclePerChannelForLoop�ĵ�ַ 
**                      ֻʹ�����򻺴��к�������խ��
** output parameters:	0:������б��, 1:����б��
**
** Created by:			liumingxin	  
** Created Date:		20170713
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FVehXieZouStatus(void *AddressForAd[4])
{
	VehicleInfForLoop_Struct (*VehForwChannelStruct)[CHL_SAVEVEH]; //����������
	Colum_Struct (*Forw_ZTColum)[COLUM_SAVEAXlE]; 	//�����л���
	//Colum_Struct (*Rev_ZTColum)[COLUM_SAVEAXlE];	//�����л���
	int16 (*WonderZTStruct)[COLUM_SAVEAXlE];        //������խ����
	
	int8   XieZouFlag = 1;    // б�߱�־, -1:����, 1:����������ʻ, 2:2��б��, 3:3��б��
	uint8  l_u8tempi, l_u8tempj;            // ��ʱ����
    uint8  l_u8MatchColumLeft = 0;			// ��¼һ��������ȷ�������խ��������
    uint8  l_u8MatchColumRight = 0;			// ��¼һ��������ȷ�����Ҳ�խ��������
	uint8  l_u8MatchColumTemp = 0;          // ��ʱ��¼�����Ҳ���
	uint8  l_u8ZTHaveFlag = 0;              // �Ƿ���խ��, 0:û��, !0:��
	uint8  l_u8VehHaveFlag = 0;             // �Ƿ��г�, 0:û��, !0:��
	int8   l_u8ColumZTCnt[4][4] = {0};      // ÿ��խ������, 0x00:û��խ��, 0x01:ֻ�е�һ��, 0x02:ֻ�еڶ���, 0x05:��һ����, 0x06:ֻ�ж�����, 0x07:һ������ȫ��
	uint8  l_u8tempCnt = 0;                 // �ж�б�е���������, ����2��Ϊб��
    uint8  l_u8lostZTFlag = 0;              // bit0:ȱ����, bit1:ȱ��, bit2:�е�һ���޵ڶ���  (Ϊ1��Ч)
    uint32 l_u32OddCoumDnTime[3][2];        // ������ǰ�������µ��³�ʱ��
    uint32 l_u32EvenCoumDnTime[3];          // ż����ǰ�������µ��³�ʱ��
  
//    uint8  l_u8NewVehColum = 0;             // ����������
	//uint32 l_u32TimeNow = 0;

	uint8  l_u8Err;
	uint16 l_u16SendLen = 0;
	uint8  l_u8NoneColumFlag = 0;           // ���б�־,0:����, 1:����Ϊ�ǿ���
	
    // ������ʼ��
    VehForwChannelStruct = AddressForAd[0];
	Forw_ZTColum = AddressForAd[1];
	//Rev_ZTColum = AddressForAd[2];
	WonderZTStruct = AddressForAd[3];
	for(l_u8tempi = 0; l_u8tempi < 4; l_u8tempi++)
	{
		l_u8ColumZTCnt[l_u8tempi][0]  = -1;
		l_u8ColumZTCnt[l_u8tempi][1]  = -1;
		l_u8ColumZTCnt[l_u8tempi][2]  = -1;
		l_u8ColumZTCnt[l_u8tempi][3]  = -1;
	}
	for(l_u8tempi = 0; l_u8tempi < 3; l_u8tempi++)
	{
		l_u32EvenCoumDnTime[l_u8tempi] = 0;
		l_u32OddCoumDnTime[l_u8tempi][0] = 0;
		l_u32OddCoumDnTime[l_u8tempi][1] = 0;
	}

	
	
	// �ҵ�����������
	for(l_u8tempj = 0; l_u8tempj < PROMODPARAM.m_u8LaneNum*2; l_u8tempj++)    // ����ȫ���У��ҵ���������λ��������
	{
		// ��������������������λ����������ĳһ��խ����Χ��, ������խ����Ϊ�ó������խ��
		if((g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos - PROMODPARAM.m_u8JGKZ) >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
			&& (g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos - PROMODPARAM.m_u8JGKZ) <= PROMODPARAM.m_u16LRLocation[1][l_u8tempj]
			&& (g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos > PROMODPARAM.m_u8JGKZ) )
		{
			l_u8MatchColumLeft = l_u8tempj;    // ��¼�����λ��������
		}
		else if(g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos <= PROMODPARAM.m_u8JGKZ
			&& (g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16Ltpos >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]))
		{
			l_u8MatchColumLeft = l_u8tempj;    // ��¼�����λ��������
		}
		else if((l_u8tempj == PROMODPARAM.m_u8LaneNum*2 - 1) && (l_u8MatchColumLeft == 0))
		{
			l_u8MatchColumLeft = l_u8tempj;    // ��¼�����λ��������
		}
		// ��������������������λ����������ĳһ��խ����Χ��, ������խ����Ϊ�ó������խ��
		if((g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16rtpos + PROMODPARAM.m_u8JGKZ) >= PROMODPARAM.m_u16LRLocation[0][l_u8tempj]
			&& (g_RasterData[g_u8RasterDataWriteIndex].headinfo[0].u16rtpos + PROMODPARAM.m_u8JGKZ) <= PROMODPARAM.m_u16LRLocation[1][l_u8tempj])
		{
			l_u8MatchColumRight = l_u8tempj;    // ��¼���Ҳ�λ��������
			break;
		}
		else if((l_u8tempj == PROMODPARAM.m_u8LaneNum*2 - 1) && (l_u8MatchColumRight == 0))
		{
			l_u8MatchColumRight = l_u8tempj;    // ��¼���Ҳ�λ��������
		}
	}
	
	// ��������չ��(�����ҵ�û��խ������λ��,���4��(������û��խ������))
	l_u8MatchColumTemp = l_u8MatchColumLeft;
	while(l_u8MatchColumLeft > 0)    // ������չ��,�ҿ���
	{
		for(l_u8tempj = 0; l_u8tempj < COLUM_SAVEAXlE; l_u8tempj++)
		{
			// ������խ��
			if(((Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[0] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[0]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
			    ((Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[1] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[1]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
				((Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[2] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumLeft][l_u8tempj].Colum[2]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
			    ((WonderZTStruct[l_u8MatchColumLeft][l_u8tempj] != -1) && (g_ChlData[WonderZTStruct[l_u8MatchColumLeft][l_u8tempj] != -1].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm))	)    
			{
				break;
			}
		}
		if(l_u8tempj == 4)    // ����Ϊ����
		{
			if((l_u8MatchColumLeft < PROMODPARAM.m_u8LaneNum*2 - 1) && (l_u8MatchColumLeft != l_u8MatchColumTemp))    // ��ֹԽ��
			{
				l_u8MatchColumLeft++;   // ���е��Ҳ���
			}
			break;
		}
		else
		{
			l_u8MatchColumLeft--;
		}
	}
	l_u8MatchColumTemp = l_u8MatchColumRight;
	while(l_u8MatchColumRight < PROMODPARAM.m_u8LaneNum*2)    // ������չ��,�ҿ���
	{
		for(l_u8tempj = 0; l_u8tempj < COLUM_SAVEAXlE; l_u8tempj++)
		{ 
			// ������խ��
			if(((Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[0] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[0]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
			    ((Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[1] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[1]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
				((Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[2] != -1) && (g_ChlData[Forw_ZTColum[l_u8MatchColumRight][l_u8tempj].Colum[2]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)) || \
			    ((WonderZTStruct[l_u8MatchColumRight][l_u8tempj] != -1) && (g_ChlData[WonderZTStruct[l_u8MatchColumRight][l_u8tempj] != -1].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm))	)    
			{
				break;
			}
		}
		if(l_u8tempj == 4)    // ����Ϊ����
		{
			if((l_u8MatchColumRight > 0) && (l_u8MatchColumRight != l_u8MatchColumTemp))    // ��ֹԽ��
			{
				l_u8MatchColumRight--;     // ���е������
			}
			break;
		}
		else
		{
			l_u8MatchColumRight++;
		}
	}
	
	if((l_u8MatchColumRight <= l_u8MatchColumLeft + 3) && (l_u8MatchColumRight >= l_u8MatchColumLeft + 2))    // ���������ܳ���4��
	{
		// �жϼ����Ӧ���Ƿ񽨳�����խ��
		for(l_u8tempi = l_u8MatchColumLeft; l_u8tempi < l_u8MatchColumRight + 1; l_u8tempi++)    // �����жϵĳ�����������
		{
			l_u8NoneColumFlag = 0;
			for(l_u8tempj = 0; l_u8tempj < COLUM_SAVEAXlE; l_u8tempj++)
			{
				if((Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0] != -1) && (g_ChlData[Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm))	
				{
					if((Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0] != -1) || (Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[1] != -1) || (Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[2] != -1) || \
						((WonderZTStruct[l_u8tempi][l_u8tempj] != -1) && (g_ChlData[WonderZTStruct[l_u8tempi][l_u8tempj] != -1].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm)))    
					{
						l_u8ZTHaveFlag++;    // �����Ӧ����խ��
					}
					
					if((WonderZTStruct[l_u8tempi][l_u8tempj] != -1) && 
					(g_ChlData[WonderZTStruct[l_u8tempi][l_u8tempj]].u32uptm + 5*6000 > g_RasterData[g_u8RasterDataWriteIndex].u32tm))
					{
						l_u8NoneColumFlag = 1;
						l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] = 0x02;     // �ڶ�����խ��(������)
					}
				
					if(Forw_ZTColum[l_u8tempi][l_u8tempj].ZTJoinFlag < 3 && Forw_ZTColum[l_u8tempi][l_u8tempj].ZTJoinFlag > 0)    
					{
						l_u8ZTHaveFlag = 0xff;
						l_u8VehHaveFlag = 0xff;
						break;
					}
					else if(Forw_ZTColum[l_u8tempi][l_u8tempj].ZTJoinFlag == 3)    // 2,3�ų���(ȱ����)
					{
						l_u8NoneColumFlag = 1;
						l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] = 0x06;  
					}
					else if(Forw_ZTColum[l_u8tempi][l_u8tempj].ZTJoinFlag == 4)    // 1,2 �ų���
					{
						l_u8NoneColumFlag = 1;
						l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] = 0x03;     
						
						if(l_u8tempi%2 == 1)    // ������
						{
							l_u32OddCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2][0] = g_ChlData[Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0]].u32dntm; // ��һ��խ���³�ʱ��
							l_u32OddCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2][1] = g_ChlData[Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[1]].u32dntm; // �ڶ���խ���³�ʱ��
						}
						else    // ż����
						{
							l_u32EvenCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2] = g_ChlData[Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0]].u32dntm;  // ��һ��խ���³�ʱ��
						}
					}
					else if(Forw_ZTColum[l_u8tempi][l_u8tempj].ZTJoinFlag == 0)    // ֻ������
					{
						l_u8NoneColumFlag = 1;
						l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] = 0x01;     // ������խ��
						
						if(l_u8tempi%2 == 0)    // ż����
						{
							l_u32EvenCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2] = g_ChlData[Forw_ZTColum[l_u8tempi][l_u8tempj].Colum[0]].u32dntm;  // ��һ��խ���³�ʱ��
						}
// 						else
// 						{
// 							l_u32OddCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2][0] = 0; // ��һ��խ���³�ʱ��
// 							l_u32OddCoumDnTime[(l_u8tempi-l_u8MatchColumLeft)/2][1] = 0; // �ڶ���խ���³�ʱ��
// 						}
					}
				}
			}
			if(l_u8NoneColumFlag == 0)
			{
				l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][0] = 0;
			}
			if(l_u8ZTHaveFlag == 0xff && l_u8VehHaveFlag == 0xff)    // ĳ�������������Ϊ����б��
			{
				break;
			}
			if(VehForwChannelStruct[l_u8tempi][0].m_u8VehicleHaveFlag == 1)
			{
				l_u8VehHaveFlag = 1;      // �����Ӧ���г�
			}
		}
		if((l_u8ZTHaveFlag == 0) && (l_u8VehHaveFlag == 0))     // ���ⴥ��ʱ��Ӧ��û��խ������û����, ����
		{
			XieZouFlag = -1;    // ����
		}
		else if((l_u8ZTHaveFlag > 0) && (l_u8VehHaveFlag > 0))   // ���ⴥ��ʱ��Ӧ����խ�����ҽ���, ������ʻ
		{
			XieZouFlag = 1;    // ����������ʻ
		}
		else if((l_u8ZTHaveFlag > 0) && (l_u8VehHaveFlag == 0))  // ���ⴥ��ʱ��Ӧ����խ����û����, ����б���ж�
		{
			for(l_u8tempi = l_u8MatchColumLeft; l_u8tempi < l_u8MatchColumRight + 1; l_u8tempi++)
			{
				for(l_u8tempj = 0; l_u8tempj < COLUM_SAVEAXlE; l_u8tempj++)
				{
					if(l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] == 0)   // ������խ��
					{
						l_u8tempCnt++;
						l_u8lostZTFlag |=  0x02;
					}
					else if((l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] == 0x06) || 
						(l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] == 0x02))  // ȱ����
					{
						l_u8tempCnt++;
						l_u8lostZTFlag |= 0x01;
					}
					else if((l_u8tempi%2 != 0) && (l_u8ColumZTCnt[l_u8tempi-l_u8MatchColumLeft][l_u8tempj] == 0x01))    // ������ֻ�е�һ��
					{
						l_u8tempCnt++;
						l_u8lostZTFlag |=  0x04;
					}
				}
			}
			
			if(l_u32OddCoumDnTime[1][0] > l_u32OddCoumDnTime[0][0])    // ����������
			{
				l_u32OddCoumDnTime[2][0] = l_u32OddCoumDnTime[1][0];
				l_u32OddCoumDnTime[2][1] = l_u32OddCoumDnTime[1][1];
			}	
			else 
			{
				l_u32OddCoumDnTime[2][0] = l_u32OddCoumDnTime[0][0];
				l_u32OddCoumDnTime[2][1] = l_u32OddCoumDnTime[0][1];
			}
			if(l_u32EvenCoumDnTime[0] > l_u32EvenCoumDnTime[1])    // ż��������
			{
				l_u32EvenCoumDnTime[2] = l_u32EvenCoumDnTime[0];
			}
			else
			{
				l_u32EvenCoumDnTime[2] = l_u32EvenCoumDnTime[1];
			}
			
			if((l_u8lostZTFlag & 0x03) != 0)    // �в�����ȱ���ŵ����?
			{
				if((l_u8MatchColumLeft + 2 < l_u8MatchColumRight) && (l_u8tempCnt >= 2))  // �����⳵������Χ����3��
				{
					XieZouFlag = 4;    // ����б��4��
				}
				else if((l_u8MatchColumLeft + 1 < l_u8MatchColumRight) && (l_u8tempCnt >= 2))  // �����⳵������Χ����2��
				{
					XieZouFlag = 3;    // ����б��3��
				}
				else if(l_u8MatchColumLeft + 1 < l_u8MatchColumRight)
				{
					if((l_u32EvenCoumDnTime[2] < l_u32OddCoumDnTime[2][1]) && (l_u32EvenCoumDnTime[2] > l_u32OddCoumDnTime[2][0]))
					{
						if(((l_u32OddCoumDnTime[2][1]-l_u32EvenCoumDnTime[2])*100 < (l_u32EvenCoumDnTime[2]-l_u32OddCoumDnTime[2][0])*120 ||
							(l_u32OddCoumDnTime[2][1]-l_u32EvenCoumDnTime[2])*100 > (l_u32EvenCoumDnTime[2]-l_u32OddCoumDnTime[2][0])*80) &&
							(l_u32OddCoumDnTime[2][1]-l_u32OddCoumDnTime[2][0]>1400))
						{
							XieZouFlag = 5;    // ����б��
						}
					}
				}
				else
				{
					XieZouFlag = 1;    // ��Ϊ������ʻ
				}
			}
			else if(l_u8lostZTFlag ==  0x04)
			{
				if(l_u8tempCnt >= 2)
				{
					XieZouFlag = 6;    // ������ֻ�����ŵ����
				}
			}
			else
			{
				if((l_u32EvenCoumDnTime[2] < l_u32OddCoumDnTime[2][1]) && (l_u32EvenCoumDnTime[2] > l_u32OddCoumDnTime[2][0]))
				{
					if(((l_u32OddCoumDnTime[2][1]-l_u32EvenCoumDnTime[2])*100 < (l_u32EvenCoumDnTime[2]-l_u32OddCoumDnTime[2][0])*120 ||
						(l_u32OddCoumDnTime[2][1]-l_u32EvenCoumDnTime[2])*100 > (l_u32EvenCoumDnTime[2]-l_u32OddCoumDnTime[2][0])*80) &&
						(l_u32OddCoumDnTime[2][1]-l_u32OddCoumDnTime[2][0]>1400))
					{
						XieZouFlag = 7;    // ����б��
					}
				}
			}
		}
		
		if(XieZouFlag > 1)
		{
// 			l_u8NewVehColum = (l_u8MatchColumLeft + l_u8MatchColumRight) / 2;
// 		
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_u8VehicleHaveFlag = 1;
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_n8CurrentAxleNum = -1;
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_u8DireFlag = 1;
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_n8XieZouFlag = XieZouFlag;
// 			ReadFPGAGlobalTime(l_u32TimeNow);
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_u32LoopTime=l_u32TimeNow;
// 			VehForwChannelStruct[l_u8NewVehColum][0].m_Capture_LaneNum = (((PROMODPARAM.m_au8CHLSET[l_u8NewVehColum]& 0x0f)<<4) | (PROMODPARAM.m_au8CHLSET[l_u8NewVehColum+1]& 0x0f));
// 			FCapture(VehForwChannelStruct[l_u8NewVehColum][0].m_Capture_LaneNum, l_u32TimeNow);		
			
#if DEBUGINFO_TASK_SP_EN > 0
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
			l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"XZFlag =%d,L=%d,R=%d,Lt=%02X,N=%d.\r\n",
									XieZouFlag,l_u8MatchColumLeft,l_u8MatchColumRight,l_u8lostZTFlag,l_u8tempCnt);
			g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
			OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
			g_u16CurDebugInd++;
			g_u16CurDebugInd %= COM_NUM;
			memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
#endif
		}
	}
}
 void  FVehPassStatus(uint8* pBuf, uint8* pBuf1, uint8* pBuf2)
{
	uint8* l_au8LoopIn;
	uint8  l_au8LoopState[16];
	uint8* l_au8ReVehPositionFlag;
	uint8* l_au8VehPositionFlag;
	uint8  l_u8tempi;
//	uint8  l_u8tempj;
	uint32 l_u32TimeNow;
	uint16 curCoilStat;
	uint16 preCoilStat=0;
	

	l_au8LoopIn = pBuf;
	l_au8VehPositionFlag = pBuf1;
	l_au8ReVehPositionFlag = pBuf2;
	memset(l_au8LoopState,0x00,sizeof(l_au8LoopState));
	
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
		//curCoilStat=curCoilStat|(g_u8RunLoopSta[l_u8tempi]<<l_u8tempi);
		preCoilStat=preCoilStat^(g_au8LoopStatePre[l_u8tempi]<<l_u8tempi);
		curCoilStat=preCoilStat;
	}
			
	if(PROMODPARAM.m_u8TriggerMode==4)
	{
		for(l_u8tempi=0;l_u8tempi<5;l_u8tempi++)
		{
			if(l_au8LoopIn[2*l_u8tempi]==0)
				l_au8LoopState[2*l_u8tempi]=1;
			else
				l_au8LoopState[2*l_u8tempi]=0;
			if(g_au8LoopStatePre[2*l_u8tempi]==0 && l_au8LoopState[2*l_u8tempi]==1)
			{
				l_au8ReVehPositionFlag[l_u8tempi]=1;
				ReadFPGAGlobalTime(l_u32TimeNow);
				g_au32LoopCaTimeUpBack[l_u8tempi] = l_u32TimeNow;
				if(IsBigFlag(l_u32TimeNow,g_au32LoopCaTimeBack[l_u8tempi]) && (GetDiff(l_u32TimeNow,g_au32LoopCaTimeBack[l_u8tempi]) < 200) && (g_au8WaitingPostVehFlg[l_u8tempi] == 1))
				{
					g_au8WaitingPostVehFlg[l_u8tempi]=0;
				}
				if(((2*l_u8tempi) == g_u8FirstAxleColum) && (g_u8FirstAxleFlag==1)) //��һ������Ȧ��β���������Ȧ�ٴδ��� ��ȡ����־
				{
						g_u8FirstAxleFlag=0;
				}
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					if(g_StartWaveFlag==0)
					{
						g_StartWaveFlag=1;
						TcpSendData( SOCKET_WAVE,startWave,12);
					}	
					coilWave[7]=(l_u32TimeNow& 0xff000000)>>24;
					coilWave[8]=(l_u32TimeNow& 0x00ff0000)>>16;
					coilWave[9]=(l_u32TimeNow& 0x0000ff00)>>8;
					coilWave[10]=l_u32TimeNow& 0xff;
					curCoilStat=curCoilStat|(1<<(2*l_u8tempi));
					coilWave[11]=(preCoilStat>>8)&0xff;
					coilWave[12]=preCoilStat&0xff;
					coilWave[13]=(curCoilStat>>8)&0xff;
					coilWave[14]=curCoilStat&0xff;
					coilWave[15]=0x00;
					coilWave[16]=0x00;
					AddCrc16(coilWave,17);		
					PCSendCnt=TcpSendData( SOCKET_WAVE,coilWave,19);	
					FReConnect( PCSendCnt, SOCKET_WAVE );
// 					if(g_u8RasterRead!=((g_u8RasterDataWriteIndex-1+256)&0xFF))
// 					{
// 						laserWave[5]=((g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)>>8)&0xff;
// 						laserWave[6]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)&0xff;
// 						laserWave[7]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff000000)>>24;
// 						laserWave[8]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x00ff0000)>>16;
// 						laserWave[9]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x0000ff00)>>8;
// 						laserWave[10]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff;
// 						laserWave[11]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;
// 						for(l_u8tempj=0;l_u8tempj<g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;l_u8tempj++)
// 						{
// 							laserWave[12+6*l_u8tempj]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].m_u8VehID;
// 							laserWave[12+6*l_u8tempj+1]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u8type;
// 							laserWave[12+6*l_u8tempj+2]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+3]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos&0xff;
// 							laserWave[12+6*l_u8tempj+4]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+5]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos&0xff;
// 						}
// 						AddCrc16(laserWave,12+6*g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt);
// 						PCSendCnt=SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
// 						FReConnect( PCSendCnt, SOCKET_WAVE );
// 					}
				}
			}
			else if(g_au8LoopStatePre[2*l_u8tempi]==0 && l_au8LoopState[2*l_u8tempi]==0)
			{
				l_au8ReVehPositionFlag[l_u8tempi]=0;
			}
			else if(g_au8LoopStatePre[2*l_u8tempi]==1 && l_au8LoopState[2*l_u8tempi]==1)
			{
				l_au8ReVehPositionFlag[l_u8tempi]=2;
			}
			else if(g_au8LoopStatePre[2*l_u8tempi]==1 && l_au8LoopState[2*l_u8tempi]==0)
			{
				l_au8ReVehPositionFlag[l_u8tempi]=3;
				ReadFPGAGlobalTime(l_u32TimeNow);
				g_au32LoopCaTimeBack[l_u8tempi]=l_u32TimeNow;
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					ReadFPGAGlobalTime(l_u32TimeNow);
					coilWave[7]=(l_u32TimeNow& 0xff000000)>>24;
					coilWave[8]=(l_u32TimeNow& 0x00ff0000)>>16;
					coilWave[9]=(l_u32TimeNow& 0x0000ff00)>>8;
					coilWave[10]=l_u32TimeNow& 0xff;
					curCoilStat=curCoilStat^(1<<(2*l_u8tempi));					
					coilWave[11]=(preCoilStat>>8)&0xff;
					coilWave[12]=preCoilStat&0xff;
					coilWave[13]=(curCoilStat>>8)&0xff;
					coilWave[14]=curCoilStat&0xff;
					coilWave[15]=0x00;
					coilWave[16]=0x00;
					AddCrc16(coilWave,17);
					PCSendCnt=TcpSendData( SOCKET_WAVE,coilWave,19);	
					FReConnect( PCSendCnt, SOCKET_WAVE );		
// 					if(g_u8RasterRead!=((g_u8RasterDataWriteIndex-1+256)&0xFF))
// 					{					
// 						laserWave[5]=((g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)>>8)&0xff;
// 						laserWave[6]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)&0xff;
// 						laserWave[7]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff000000)>>24;
// 						laserWave[8]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x00ff0000)>>16;
// 						laserWave[9]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x0000ff00)>>8;
// 						laserWave[10]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff;
// 						laserWave[11]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;
// 						for(l_u8tempj=0;l_u8tempj<g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;l_u8tempj++)
// 						{
// 							laserWave[12+6*l_u8tempj]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].m_u8VehID;
// 							laserWave[12+6*l_u8tempj+1]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u8type;
// 							laserWave[12+6*l_u8tempj+2]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+3]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos&0xff;
// 							laserWave[12+6*l_u8tempj+4]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+5]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos&0xff;
// 						}
// 						AddCrc16(laserWave,12+6*g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt);
// 						PCSendCnt=SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
// 						FReConnect( PCSendCnt, SOCKET_WAVE );
// 					}
					if(curCoilStat==0x00)   //����
					{
						g_StartWaveFlag=0;
						TcpSendData( SOCKET_WAVE,finishWave,12);
					}	
				}

			}
			g_au8LoopStatePre[2*l_u8tempi] = l_au8LoopState[2*l_u8tempi];
		}
	}
	///////////////////////////////////////////////ǰ����Ȧɨ�裬������Ϊǰ��˫��Ȧʱ����¼ǰ����Ȧ�仯�������ֳ�������������ʻ�׳�����
	///////////////////////////////////////////////������Ȧɨ�裬��¼������Ȧ״̬������������ʻ�׳������зֳ�����
	if(PROMODPARAM.m_u8TriggerMode==4)
	{
		for(l_u8tempi=0;l_u8tempi<5;l_u8tempi++)
		{
			if(l_au8LoopIn[2*l_u8tempi+1]==0)
				l_au8LoopState[2*l_u8tempi+1]=1;
			else
				l_au8LoopState[2*l_u8tempi+1]=0;
			if(g_au8LoopStatePre[2*l_u8tempi+1]==0 && l_au8LoopState[2*l_u8tempi+1]==1)
			{
//				g_au8ReFrontLoopFlag[l_u8tempi]=1;
				l_au8VehPositionFlag[l_u8tempi]=1;
				ReadFPGAGlobalTime(l_u32TimeNow);
				g_au32LoopCaTimeUp[l_u8tempi] = l_u32TimeNow;
				if(((2*l_u8tempi+1) == g_u8CrossingColum) && (g_u8CrossingFlag==1)) //��һ������Ȧ��β���������Ȧ�ٴδ��� ��ȡ����־
				{
						g_u8CrossingFlag=0;
				}
				/////////////////////////������¼��Ȧ��Ϣ ��Ҫʹ���²ſ���
				if(PROMODPARAM.m_u8EnableFlag[2] == 1)
				{
					if(g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOnTime == 0)
					{
						g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOnTime = g_asLoopBoard[2*l_u8tempi+1].trigger_time;
					}
					else
					{
						g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOnTime = g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOnTime;
					}
					g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOnTime = g_asLoopBoard[2*l_u8tempi+1].trigger_time;
				}
				if(g_StartWaveFlag==0)
				{
					g_StartWaveFlag=1;
					TcpSendData( SOCKET_WAVE,startWave,12);
				}	
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					ReadFPGAGlobalTime(l_u32TimeNow);
					coilWave[7]=(l_u32TimeNow& 0xff000000)>>24;
					coilWave[8]=(l_u32TimeNow& 0x00ff0000)>>16;
					coilWave[9]=(l_u32TimeNow& 0x0000ff00)>>8;
					coilWave[10]=l_u32TimeNow& 0xff;
					curCoilStat=curCoilStat|(1<<(2*l_u8tempi+1));
					coilWave[11]=(preCoilStat>>8)&0xff;
					coilWave[12]=preCoilStat&0xff;
					coilWave[13]=(curCoilStat>>8)&0xff;
					coilWave[14]=curCoilStat&0xff;
					coilWave[15]=0x00;
					coilWave[16]=0x00;
					AddCrc16(coilWave,17);
					PCSendCnt=TcpSendData( SOCKET_WAVE,coilWave,19);
					FReConnect( PCSendCnt, SOCKET_WAVE );	
// 					if(g_u8RasterRead!=((g_u8RasterDataWriteIndex-1+256)&0xFF))
// 					{					
// 						laserWave[5]=((g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)>>8)&0xff;
// 						laserWave[6]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)&0xff;
// 						laserWave[7]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff000000)>>24;
// 						laserWave[8]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x00ff0000)>>16;
// 						laserWave[9]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x0000ff00)>>8;
// 						laserWave[10]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff;
// 						laserWave[11]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;
// 						for(l_u8tempj=0;l_u8tempj<g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;l_u8tempj++)
// 						{
// 							laserWave[12+6*l_u8tempj]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].m_u8VehID;
// 							laserWave[12+6*l_u8tempj+1]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u8type;
// 							laserWave[12+6*l_u8tempj+2]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+3]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos&0xff;
// 							laserWave[12+6*l_u8tempj+4]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+5]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos&0xff;
// 						}
// 						AddCrc16(laserWave,12+6*g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt);
// 						PCSendCnt=SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
// 						FReConnect( PCSendCnt, SOCKET_WAVE );	
// 					}
				}
				/////////////////////////
			}
			else if(g_au8LoopStatePre[2*l_u8tempi+1]==0 && l_au8LoopState[2*l_u8tempi+1]==0)
			{
				l_au8VehPositionFlag[l_u8tempi]=0;				
			}
			else if(g_au8LoopStatePre[2*l_u8tempi+1]==1 && l_au8LoopState[2*l_u8tempi+1]==0)
			{
				l_au8VehPositionFlag[l_u8tempi]=3;
				ReadFPGAGlobalTime(l_u32TimeNow);
				g_au32LoopCaTime[l_u8tempi]=l_u32TimeNow;
				/////////////////////////������¼��Ȧ��Ϣ
				if(PROMODPARAM.m_u8EnableFlag[2] == 1)
				{
					if(g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOffTime == 0)
					{
						g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOffTime = g_asLoopBoard[2*l_u8tempi+1].trigger_time;
					}
					else
					{
						g_asLoopPerLaneInfo[l_u8tempi].mu32LastLoopOffTime = g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOnTime;
					}
					g_asLoopPerLaneInfo[l_u8tempi].mu32LoopScope = g_asLoopBoard[2*l_u8tempi+1].frame_data;
					g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOffTime = g_asLoopBoard[2*l_u8tempi+1].trigger_time;
					g_asLoopPerLaneInfo[l_u8tempi].mu32TimeOnOff = tickGetDiff(g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOffTime, g_asLoopPerLaneInfo[l_u8tempi].mu32LoopOnTime);
				}
				if( 1 == g_Setup.u8SendWaveEnable )
				{
					ReadFPGAGlobalTime(l_u32TimeNow);
					coilWave[7]=(l_u32TimeNow& 0xff000000)>>24;
					coilWave[8]=(l_u32TimeNow& 0x00ff0000)>>16;
					coilWave[9]=(l_u32TimeNow& 0x0000ff00)>>8;
					coilWave[10]=l_u32TimeNow& 0xff;
					curCoilStat=curCoilStat^(1<<(2*l_u8tempi+1));
					coilWave[11]=(preCoilStat>>8)&0xff;
					coilWave[12]=preCoilStat&0xff;
					coilWave[13]=(curCoilStat>>8)&0xff;
					coilWave[14]=curCoilStat&0xff;
					coilWave[15]=0x00;
					coilWave[16]=0x00;
					AddCrc16(coilWave,17);
					PCSendCnt=TcpSendData( SOCKET_WAVE,coilWave,19);						
					FReConnect( PCSendCnt, SOCKET_WAVE );	
// 					if(g_u8RasterRead!=((g_u8RasterDataWriteIndex-1+256)&0xFF))
// 					{
// 						laserWave[5]=((g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)>>8)&0xff;
// 						laserWave[6]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt*6+14)&0xff;
// 						laserWave[7]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff000000)>>24;
// 						laserWave[8]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x00ff0000)>>16;
// 						laserWave[9]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0x0000ff00)>>8;
// 						laserWave[10]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u32tm& 0xff;
// 						laserWave[11]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;
// 						for(l_u8tempj=0;l_u8tempj<g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt;l_u8tempj++)
// 						{
// 							laserWave[12+6*l_u8tempj]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].m_u8VehID;
// 							laserWave[12+6*l_u8tempj+1]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u8type;
// 							laserWave[12+6*l_u8tempj+2]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+3]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16Ltpos&0xff;
// 							laserWave[12+6*l_u8tempj+4]=(g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos>>8)&0xff;
// 							laserWave[12+6*l_u8tempj+5]=g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].headinfo[l_u8tempj].u16rtpos&0xff;
// 						}
// 						AddCrc16(laserWave,12+6*g_RasterData[(g_u8RasterDataWriteIndex-1+256)&0xFF].u8Cnt);
// 						PCSendCnt=SendDataNet( SOCKET_WAVE,laserWave,((laserWave[5]<<8)+laserWave[6]));
// 						FReConnect( PCSendCnt, SOCKET_WAVE );	
// 					}
					if(curCoilStat==0x00)   //����
					{
						g_StartWaveFlag=0;
						TcpSendData( SOCKET_WAVE,finishWave,12);
					}	
				}
				/////////////////////////
			}
			else if(g_au8LoopStatePre[2*l_u8tempi+1]==1 && l_au8LoopState[2*l_u8tempi+1]==1)
			{
				l_au8VehPositionFlag[l_u8tempi]=2;
			}
			g_au8LoopStatePre[2*l_u8tempi+1] = l_au8LoopState[2*l_u8tempi+1];
		}
	}

}
/*********************************************************************************************************
** Function name:		FCheckIODevice
** Descriptions:		���IO�Ƿ�����
** input parameters:	�������״̬���������״̬�� խ�������� 
** output parameters:	 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FCheckIODevice(uint8* pBuf, uint8 pDX)
{
	uint8  l_u8ZTLocationX;
//	uint8  l_u8tempi;
	l_u8ZTLocationX = pDX;
	
	if(PROMODPARAM.m_u8TriggerMode==2  && l_u8ZTLocationX/2<PROMODPARAM.m_u8LaneNum)//����Ȧģʽ����
	{
		if(pBuf[l_u8ZTLocationX/2]==0 || pBuf[l_u8ZTLocationX/2]==2)//�����ǰ����״̬Ϊ��Ȧ����������Ȧ�����б�־������1
		{
			g_u8IOStateRe[l_u8ZTLocationX/2]++;
		}
		else if(pBuf[l_u8ZTLocationX/2]==1 ||pBuf[l_u8ZTLocationX/2]==3)//�����ǰ����״̬Ϊ��Ȧ��ʼ��������Ȧ����������־��������
		{
			g_u8IOStateRe[l_u8ZTLocationX/2] = 0;
		}
		if(g_u8IOStateRe[l_u8ZTLocationX/2] >= 60)//�������60������Ϊ��Ȧ����
		{
			g_u8IOStateRe[l_u8ZTLocationX/2] = 60;
			if(l_u8ZTLocationX/2>7)
				g_u8IOERR[1] |=	1<<(l_u8ZTLocationX/2-8);
			else
				g_u8IOERR[0] |=	1<<(l_u8ZTLocationX/2);
		}
		else
		{
			if(l_u8ZTLocationX/2>7)
				g_u8IOERR[1] &=	~(1<<(l_u8ZTLocationX/2-8));
			else
				g_u8IOERR[0] &=	~(1<<(l_u8ZTLocationX/2));
		}
	}
	else if(PROMODPARAM.m_u8TriggerMode==4 && l_u8ZTLocationX/2<PROMODPARAM.m_u8LaneNum)//˫��Ȧģʽ������������������߼���¼�Ĺ���״̬
	{
		if(pBuf[l_u8ZTLocationX/2*2]==0 ||pBuf[l_u8ZTLocationX/2*2]==2)//�����ǰ����״̬Ϊ��Ȧ����������Ȧ�����б�־������1
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2]++;
		}
		else if(pBuf[l_u8ZTLocationX/2*2]==1 ||pBuf[l_u8ZTLocationX/2*2]==3)//�����ǰ����״̬Ϊ��Ȧ��ʼ��������Ȧ����������־��������
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2] = 0;
		}
		if(pBuf[l_u8ZTLocationX/2*2+1]==0 ||pBuf[l_u8ZTLocationX/2*2+1]==2)//�����ǰ����״̬Ϊ��Ȧ����������Ȧ�����б�־������1
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2+1]++;
		}
		else if(pBuf[l_u8ZTLocationX/2*2+1]==1 ||pBuf[l_u8ZTLocationX/2*2+1]==3)//�����ǰ����״̬Ϊ��Ȧ��ʼ��������Ȧ����������־��������
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2+1] = 0;
		}
		if(g_u8IOStateRe[l_u8ZTLocationX/2*2] >= 60)//�������60������Ϊ��Ȧ����
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2] = 60;
			if(l_u8ZTLocationX/2*2>7)
				g_u8IOERR[1] |=	(1<<(l_u8ZTLocationX/2*2-8));
			else
				g_u8IOERR[0] |=	(1<<(l_u8ZTLocationX/2*2));
		}
		else
		{	
			if(l_u8ZTLocationX/2*2>7)
				g_u8IOERR[1] &=	~(1<<(l_u8ZTLocationX/2*2-8));
			else
				g_u8IOERR[0] &=	~(1<<(l_u8ZTLocationX/2*2));
		}
		if(g_u8IOStateRe[l_u8ZTLocationX/2*2+1] >= 60)//�������60������Ϊ��Ȧ����
		{
			g_u8IOStateRe[l_u8ZTLocationX/2*2+1] = 60;
			if(l_u8ZTLocationX/2*2+1>7)
				g_u8IOERR[1] |=	(1<<(l_u8ZTLocationX/2*2+1-8));
			else
				g_u8IOERR[0] |=	(1<<(l_u8ZTLocationX/2*2+1));
		}
		else
		{
			if(l_u8ZTLocationX/2*2+1>7)
				g_u8IOERR[1] &=	~(1<<(l_u8ZTLocationX/2*2+1-8));
			else
				g_u8IOERR[0] &=	~(1<<((l_u8ZTLocationX/2*2+1)));
		}
	}
}
/*********************************************************************************************************
** Function name:		FCheckZTDevice
** Descriptions:		���խ���Ƿ�����
** input parameters:	����խ����խ�������� 
** output parameters:	 
**
** Created by:			Wgh		  
** Created Date:		20141223	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  	
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FCheckZTDevice(ZTINFRECELEMENT l_pZtInf, uint8 pDX, uint8 pDY)
{
	uint8 l_u8ZTLocationY;
	uint8 l_u8ZTLocationX;
	l_u8ZTLocationX = pDX;
	l_u8ZTLocationY = pDY;
	g_u8ZTStateRe[l_pZtInf.m_ucTouch]++;
	if(PROMODPARAM.m_u8ZTRow == 3)//�����3��
	{
		if(l_u8ZTLocationY ==PROMODPARAM.m_u8ZTRow-1)//ĩ��խ���Ĵ���������ǰ����ͬ�в�ֵ����6����Ϊ����
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch-1])>6 
			&& SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch-1], g_u8ZTStateRe[l_pZtInf.m_ucTouch-2])<3)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
		else if(l_u8ZTLocationY ==0)//����խ���Ĵ��������Ⱥ�����ͬ�в�ֵ����6����Ϊ����
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch+1])>6 
			&& SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch+1], g_u8ZTStateRe[l_pZtInf.m_ucTouch+2])<3)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
		else//�м���խ���Ĵ���������ǰ������ͬ�в�ֵ����6����Ϊ����
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch+1])>6 
			&& SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch+1], g_u8ZTStateRe[l_pZtInf.m_ucTouch-1])<3)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
	}
	else if(PROMODPARAM.m_u8ZTRow == 2)//2��խ�������խ��������������ͬ��խ����ֵ6����Ϊ����
	{
		if(l_u8ZTLocationY ==PROMODPARAM.m_u8ZTRow-1)
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch-1])>6)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
		else if(l_u8ZTLocationY ==0)
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch+1])>6)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
	}
	else if(PROMODPARAM.m_u8ZTRow == 1)//1��խ�������խ��������������ͬ����խ����ֵ6����Ϊ����
	{
		if(l_u8ZTLocationX%2 == 0)
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch+1])>6)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
		else if(l_u8ZTLocationX%2 == 1)
		{
			if(SUB(g_u8ZTStateRe[l_pZtInf.m_ucTouch], g_u8ZTStateRe[l_pZtInf.m_ucTouch-1])>6)
			{
				g_u8DeviceERR[(l_pZtInf.m_ucTouch)/8+1] |= 1<<((l_pZtInf.m_ucTouch)%8);
			}
		}
	}
}
/*********************************************************************************************************
** Function name:		FPostVehForAbNormal
** Descriptions:		�쳣�׳�
** input parameters:	
** output parameters:	 
**
** Created by:			zhangtonghan	  
** Created Date:		20170616	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void FPostVehForAbNormal(void* Address[3])
{
	uint8* l_au8VehPositionFlag;//������Ȧ
	uint8* l_au8ReVehPositionFlag;//ǰ����Ȧ
	uint8  l_u8tempi;
	int8 Dir,l_u8pos;
	uint8 LaneNum;
	void* l_u32AddressForPo[3];
	uint32 l_u32timeNow;
	
	l_au8VehPositionFlag = Address[0];
	l_au8ReVehPositionFlag = Address[1];
	l_u32timeNow = *(uint32*)Address[2];
	
// 	for(l_u8tempi=0;l_u8tempi < 2*PROMODPARAM.m_u8LaneNum;l_u8tempi ++)  //��೬��13m�׳�
// 	{
// 		if((g_asVehiclePerChannelForLoop[l_u8tempi][0].m_u8VehicleHaveFlag==1)&&(g_asVehiclePerChannelForLoop[l_u8tempi][0].m_u8PostAxleFlag==1)&&
// 			(g_asVehiclePerChannelForLoop[l_u8tempi][0].m_u32ForcePostTime<=l_u32timeNow))
// 		{
// 			Dir=1;
// 			l_u32AddressForPo[0] = &l_u8tempi;
// 			l_u32AddressForPo[1] = &Dir;
// 			l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[l_u8tempi][0];
// 			FPostVehForLoop(l_u32AddressForPo,30);//�׳�
// 		}
// 		if((g_asReVehiclePerChannelForLoop[l_u8tempi][0].m_u8VehicleHaveFlag==1)&&(g_asReVehiclePerChannelForLoop[l_u8tempi][0].m_u8PostAxleFlag==1)&&
// 			(g_asReVehiclePerChannelForLoop[l_u8tempi][0].m_u32ForcePostTime<=l_u32timeNow))
// 		{
// 			Dir=-1;
// 			l_u32AddressForPo[0] = &l_u8tempi;
// 			l_u32AddressForPo[1] = &Dir;
// 			l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[l_u8tempi][0];
// 			FPostVehForLoop(l_u32AddressForPo,31);//�׳�
// 		}
// 	}
	for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)//����������Ȧ״̬
	{
		Dir = 1;
		
		if((g_au32LoopCaTime[l_u8tempi] > g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime) && (l_au8VehPositionFlag[l_u8tempi]==0) && \
			(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) &&\
			((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0])<=2000))) //by wanglihui 
		{ 
			l_u8pos=F_FindVehEnd(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_an16CurrentAxleInf[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum], MAXNUM_AXLE);
			if((l_u8pos > -1)&&(GetDiff(g_ChlData[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_an16CurrentAxleInf[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum][l_u8pos]].u32FirstDnTime,l_u32timeNow) > 6000))
			{
				LaneNum = 2*l_u8tempi;
				l_u32AddressForPo[0] = &LaneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[LaneNum][0];
				FPostVehForLoop(l_u32AddressForPo,11);//�׳�
			}
		}
// 		else if(((g_au32LoopCaTime[l_u8tempi] > g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime) || (g_au32LoopCaTime[l_u8tempi+1] > g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime))&&\
// 			(l_au8VehPositionFlag[l_u8tempi]==0) && (g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&\
// 			//(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 1) &&
// 		((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[0])<=200)) &&\
// 		(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt-1] == 0x2) && (l_au8VehPositionFlag[l_u8tempi+1]==0))
// 		{
// 			
// 			LaneNum = 2*l_u8tempi+1;
// 			l_u32AddressForPo[0] = &LaneNum;
// 			l_u32AddressForPo[1] = &Dir;
// 			l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[LaneNum][0];
// 			FPostVehForLoop(l_u32AddressForPo,12);//�׳�
// 		}
		else if((g_au32LoopCaTime[l_u8tempi] > g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime) && (l_au8VehPositionFlag[l_u8tempi]==0) && (g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&\
			//(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 1) &&
		((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[0])<=2000)) &&\
		(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt-1] != 0x2))
		{
			l_u8pos=F_FindVehEnd(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_an16CurrentAxleInf[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum], MAXNUM_AXLE);
			if((l_u8pos > -1)&&(GetDiff(g_ChlData[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_an16CurrentAxleInf[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum][l_u8pos]].u32FirstDnTime,l_u32timeNow) > 6000))
			{
				LaneNum = 2*l_u8tempi+1;
				l_u32AddressForPo[0] = &LaneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[LaneNum][0];
				FPostVehForLoop(l_u32AddressForPo,13);//�׳�
			}
		}
	
	}
// 	if((PROMODPARAM.m_u8BwdDirecFlag == 0x01) && (PROMODPARAM.m_au8VehDirecFlag[0] == 0x03))
// 	{
	//����
	for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)//����������Ȧ״̬
	{
		Dir = -1;
		if((g_au32LoopCaTimeBack[l_u8tempi] > g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime) && (l_au8ReVehPositionFlag[l_u8tempi]==0) && \
		(g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&
		((g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 2)||((g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[0])<=2000)))
		//(g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 1)
		{ 
			l_u8pos=F_FindVehEnd(g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_an16CurrentAxleInf[g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum], MAXNUM_AXLE);
			if((l_u8pos > -1)&&(GetDiff(g_ChlData[g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_an16CurrentAxleInf[g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum][l_u8pos]].u32FirstDnTime,l_u32timeNow) > 6000))
			{
				LaneNum = 2*l_u8tempi+1;
				l_u32AddressForPo[0] = &LaneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[LaneNum][0];
				FPostVehForLoop(l_u32AddressForPo,14);//�׳�
			}
		}
		else if((l_u8tempi>0)&&((g_au32LoopCaTimeBack[l_u8tempi] > g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime) || (g_au32LoopCaTimeBack[l_u8tempi-1] > g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime))&&\
			(l_au8ReVehPositionFlag[l_u8tempi]==0) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) &&\
			//(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 1) &&
		((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 2)||((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)&&(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0])<=2000)) &&\
		(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].AxleStatus[g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt-1] == 0x2) && (l_au8ReVehPositionFlag[l_u8tempi-1]==0))
		{				
			LaneNum = 2*l_u8tempi;
			l_u32AddressForPo[0] = &LaneNum;
			l_u32AddressForPo[1] = &Dir;
			l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[LaneNum][0];
			FPostVehForLoop(l_u32AddressForPo,15);//�׳�
		}
		else if((g_au32LoopCaTimeBack[l_u8tempi] > g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime) && (l_au8VehPositionFlag[l_u8tempi]==0) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) &&\
			//(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 1) &&
		((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 2)||((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)&&(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0])<=2000)) &&\
		(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].AxleStatus[g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt-1] != 0x2))
		{
			l_u8pos=F_FindVehEnd(g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_an16CurrentAxleInf[g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum], MAXNUM_AXLE);
			if((l_u8pos > -1)&&(GetDiff(g_ChlData[g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_an16CurrentAxleInf[g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum][l_u8pos]].u32FirstDnTime,l_u32timeNow) > 6000))
			{
				LaneNum = 2*l_u8tempi;
				l_u32AddressForPo[0] = &LaneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[LaneNum][0];
				FPostVehForLoop(l_u32AddressForPo,17);//�׳�
			}
		}		
	}
	//}
}
/*********************************************************************************************************
** Function name:		FPostVehForNormal
** Descriptions:		�����׳�
** input parameters:	
** output parameters:	 
**
** Created by:			zhangtonghan	  
** Created Date:		20170608	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  
**						
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  FPostVehForNormal(void* Address[3])
{
	uint8* l_au8VehPositionFlag;    //������Ȧ
	uint8* l_au8ReVehPositionFlag;  //ǰ����Ȧ

	uint8  l_u8tempi;
	int8 Dir;         //l_columpos,l_columpos1;
	uint8 laneNum;
	static uint32 l_u32DelayTime=0;
	static uint32 l_u32CrossingDelayTime=0;
	uint32 l_u32TimeNow;
	uint16 l_u16tempspeed;
	void* l_u32AddressForPo[3];
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	l_au8VehPositionFlag = Address[0];
	l_au8ReVehPositionFlag = Address[1];
	l_u32TimeNow=*(uint32 *)Address[2];
	
	for(l_u8tempi = 0; l_u8tempi < PROMODPARAM.m_u8LaneNum; l_u8tempi++)//��������ǰ����Ȧ״̬
	{
		if(l_au8ReVehPositionFlag[l_u8tempi] == 1)    //ǰ����Ȧ����
		{
			g_u32PreLoopTime[l_u8tempi] = 0;
			g_u8RePostVehFlag[l_u8tempi] = 0;
		}
		if(l_au8ReVehPositionFlag[l_u8tempi] == 3)    //��ǰ����ǰ����Ȧ��βʱ��¼��ʱ��
		{
			g_u32PreLoopTime[l_u8tempi] = l_u32TimeNow;
		}
		if(g_u32PreLoopTime[l_u8tempi] > 0 && l_u32TimeNow > (g_u32PreLoopTime[l_u8tempi] + POSTVEHDELAY))
		{
			if(l_au8ReVehPositionFlag[l_u8tempi] == 0)
				g_u8RePostVehFlag[l_u8tempi] = 1;     //�����׳���־
		}
	}
	
	for(l_u8tempi = 0; l_u8tempi < PROMODPARAM.m_u8LaneNum; l_u8tempi++)//�������к�����Ȧ״̬
	{
		if(l_au8VehPositionFlag[l_u8tempi] == 1)
		{
			g_u32PostLoopTime[l_u8tempi] = 0;
			g_u8PostVehFlag[l_u8tempi] = 0;
		}
		if(l_au8VehPositionFlag[l_u8tempi] == 3)    //��ǰ����������Ȧ��βʱ��¼��ʱ��
		{
			g_u32PostLoopTime[l_u8tempi] = l_u32TimeNow;
		}
		if(g_u32PostLoopTime[l_u8tempi] > 0 && l_u32TimeNow > (g_u32PostLoopTime[l_u8tempi] + POSTVEHDELAY))
		{
			if(l_au8VehPositionFlag[l_u8tempi] == 0)
				g_u8PostVehFlag[l_u8tempi] = 1;    //�����׳���־
		}
	}
	
	for(l_u8tempi = 0; l_u8tempi < PROMODPARAM.m_u8LaneNum; l_u8tempi++)
	{
		if(g_u8PostVehFlag[l_u8tempi] || g_u8RePostVehFlag[l_u8tempi])
		{
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nPostVehFlag  :Dir=1,t=%d,[%d,%d,%d,%d,%d],[%d,%d,%d,%d,%d].",
									   l_u32TimeNow, g_u8PostVehFlag[0], g_u8PostVehFlag[1], g_u8PostVehFlag[2], g_u8PostVehFlag[3], g_u8PostVehFlag[4],
									   g_u32PostLoopTime[0], g_u32PostLoopTime[1], g_u32PostLoopTime[2], g_u32PostLoopTime[3], g_u32PostLoopTime[4]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			
			OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nRePostVehFlag:Dir=-1,t=%d,[%d,%d,%d,%d,%d],[%d,%d,%d,%d,%d].",
									   l_u32TimeNow, g_u8RePostVehFlag[0], g_u8RePostVehFlag[1], g_u8RePostVehFlag[2], g_u8RePostVehFlag[3], g_u8RePostVehFlag[4],
									   g_u32PreLoopTime[0], g_u32PreLoopTime[1], g_u32PreLoopTime[2], g_u32PreLoopTime[3], g_u32PreLoopTime[4]);
				g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
				OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
				g_u16CurDebugInd++;
				g_u16CurDebugInd %= COM_NUM;
				memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
			OSSemPost(g_pSemSendThtouSP);
			break;
		}
	}
	
	for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)//����������Ȧ״̬
	{
		Dir = 1;//����
			//����ǵ�һ�������һ�����Ѿ����ҹ켣Ϊ���л�������    ����ʻ�Ĺ켣 1:����ѹ����;2:��� 3:���� 4:ѹ���� 5:����  
		//if(l_au8VehPositionFlag[l_u8tempi]==3)
		if(g_u8PostVehFlag[l_u8tempi] == 1)
		{
			g_u8PostVehFlag[l_u8tempi] = 0;
			g_u32PostLoopTime[l_u8tempi] = 0;
			if((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1)  &&\
			((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0]<=2000))))
			{
				if((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum] == 2)&&\
					l_au8VehPositionFlag[l_u8tempi-1]!=2)     //����ҵ�������Ȧ����������� ��Ϊ�ǿ�����ÿ���l_u8tempi-1<0��� wanglihui                                  
				{
					laneNum = 2*l_u8tempi;
					l_u32AddressForPo[0] = &laneNum;
					l_u32AddressForPo[1] = &Dir;
					l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
					FPostVehForLoop(l_u32AddressForPo,16);//�׳�
				}
				else if(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum] != 2)
				{
					laneNum = 2*l_u8tempi;
					l_u32AddressForPo[0] = &laneNum;
					l_u32AddressForPo[1] = &Dir;
					l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
					FPostVehForLoop(l_u32AddressForPo,18);//�׳�
				}		
			}
			else if((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1)  &&\
			((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[0]<=2000))))
			{	
				if((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum] == 2)&&\
					l_au8VehPositionFlag[l_u8tempi+1]!=2)     //������������Ȧ�����������  wanglihui                                  
				{
					if((((l_au8ReVehPositionFlag[l_u8tempi]==2)||(l_au8ReVehPositionFlag[l_u8tempi+1]==2))||  //���������ص����һλ��Ϊ�Ƿ���ǰ����Ȧ�ı�־��
						(g_FristAxleDnRange%100 == 1))&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt < 3)&&
							(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt-1]<=2000))	
					{
						l_u16tempspeed=g_AxleInfo[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u16AxleList[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum]].u16speed;
						l_u32CrossingDelayTime=l_u32TimeNow + CoilDelayDistance*1000/(l_u16tempspeed/216); // �Ӻź����Ǵ�����������Ҫ�ĵ���
						g_u8CrossingFlag=1;
						g_u8CrossingColum=2*l_u8tempi+1;
					}
					else
					{
						laneNum = 2*l_u8tempi+1;
						l_u32AddressForPo[0] = &laneNum;
						l_u32AddressForPo[1] = &Dir;
						l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
						FPostVehForLoop(l_u32AddressForPo,20);//�׳�
					}
				}
				else if(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum] != 2)
				{
					laneNum = 2*l_u8tempi+1;
					l_u32AddressForPo[0] = &laneNum;
					l_u32AddressForPo[1] = &Dir;
					l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
					FPostVehForLoop(l_u32AddressForPo,19);//�׳�
				}
			}
			else if((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)
				&&(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0]>2000))
			{
				l_u16tempspeed=g_AxleInfo[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u16AxleList[g_asVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum]].u16speed;
				l_u32DelayTime=l_u32TimeNow + CoilDelayDistance*1000/(l_u16tempspeed/216); // �Ӻź����Ǵ�����������Ҫ�ĵ���
				g_u8FirstAxleFlag=1;
				g_u8FirstAxleColum=2*l_u8tempi;
			}
			if((l_u8tempi >=1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_u8VehicleHaveFlag == 1)&&
				((g_asVehiclePerChannelForLoop[2*l_u8tempi-1][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi-1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi-1][0].au16AxleWeight[0]<=2000)))
				&&(l_au8VehPositionFlag[l_u8tempi-1] !=2))  //��Կ�������Ȧ���� ����esle if��Ϊ�����ֳ����Ҳ��е��г� �׳�����ҲҪ�׳�
			{
				laneNum = 2*l_u8tempi-1;
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo,21);//�׳�
			}
		}
		else if((l_au8ReVehPositionFlag[l_u8tempi] != 2)&&(g_u8FirstAxleFlag==1)&&((2*l_u8tempi)==g_u8FirstAxleColum)&&(l_u32TimeNow>=l_u32DelayTime))
		{
			laneNum = 2*l_u8tempi;
			l_u32AddressForPo[0] = &laneNum;
			l_u32AddressForPo[1] = &Dir;
			l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
			FPostVehForLoop(l_u32AddressForPo,22);//�׳�
			g_u8FirstAxleFlag=0;
		}
		else if((l_au8ReVehPositionFlag[l_u8tempi] != 2)&&(l_au8ReVehPositionFlag[l_u8tempi+1] != 2)&&(g_u8CrossingFlag==1)&&((2*l_u8tempi+1)==g_u8CrossingColum)
			&&(l_u32TimeNow>=l_u32CrossingDelayTime)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].AxleStatus[g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum] == 2))
		{
			laneNum = 2*l_u8tempi+1;
			l_u32AddressForPo[0] = &laneNum;
			l_u32AddressForPo[1] = &Dir;
			l_u32AddressForPo[2] = &g_asVehiclePerChannelForLoop[laneNum][0];
			g_u8CrossingFlag=0;
			FPostVehForLoop(l_u32AddressForPo,23);//�׳�			
		}
	}
// 	if((PROMODPARAM.m_u8BwdDirecFlag == 0x01)&& (PROMODPARAM.m_au8VehDirecFlag[0] == 0x03))
// 	{
	for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)//����������Ȧ״̬
	{
		Dir = -1;//����
		//if(l_au8ReVehPositionFlag[l_u8tempi]==3)//���ĳһ������Ȧ��ʾ����ͨ�������г����׳�����
		if(g_u8RePostVehFlag[l_u8tempi] == 1)
		{
			g_u32PreLoopTime[l_u8tempi] = 0;
			g_u8RePostVehFlag[l_u8tempi] = 0;
			//����ǵ�һ�������һ�����Ѿ����ҹ켣Ϊ���л�������
			if((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) &&
			   ((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight[0])<=2000)))
			{
				laneNum = 2*l_u8tempi;
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo,24);//�׳�	
			}

			else if((g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&\
					((g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt >= 2)||((g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].u8AixCnt == 1)&&(g_asVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight[0])<=2000)))
			{
				laneNum = 2*l_u8tempi+1;
				l_u32AddressForPo[0] = &laneNum;
				l_u32AddressForPo[1] = &Dir;
				l_u32AddressForPo[2] = &g_asReVehiclePerChannelForLoop[laneNum][0];
				FPostVehForLoop(l_u32AddressForPo,25);//�׳�
			}
		
		}
	}
	//}
	for(l_u8tempi=0;l_u8tempi<PROMODPARAM.m_u8LaneNum;l_u8tempi++)//����������Ȧ״̬
	{
		Dir = -1;//����
		//if(l_au8ReVehPositionFlag[l_u8tempi]==3)//���ĳһ������Ȧ��ʾ����ͨ�������г����׳�����
		if(g_u8RePostVehFlag[l_u8tempi] == 1)
		{
			g_u32PreLoopTime[l_u8tempi] = 0;
			g_u8RePostVehFlag[l_u8tempi] = 0;
			//����ƥ������߼�
			if((g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1)&& (g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) && (g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime))
			{					
				g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum+1;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].RunAndWalkFlag;
				
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8NormalLogiclvl;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8XieZouFlag;
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u16AxleList,	g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight,	g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0],-1);	

				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"1).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt,2*l_u8tempi);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
				
			}			
			else if((l_u8tempi < PROMODPARAM.m_u8LaneNum) && (g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&\
				(g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime))
			{
				g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum+1;	
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].RunAndWalkFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8XieZouFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8NormalLogiclvl;
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u16AxleList,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0],-1);	
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"2).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi+1,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt,2*l_u8tempi);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else if((l_u8tempi > 0) && (g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_u8VehicleHaveFlag == 1) &&\
				(g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime))
			{
				g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_n8CurrentAxleNum+1;	
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].RunAndWalkFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_n8XieZouFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].m_u8NormalLogiclvl;
				
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].u16AxleList,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0].au16AxleWeight,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi-1][0],-1);	
					
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"3).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi-1,g_HideZTReVehicleStruct[2*l_u8tempi][0].u8AixCnt,2*l_u8tempi);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else if(g_HideZTReVehicleStruct[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1)//���������խ��û�����к�խ����
			{
				
			}
				//����խ��+1���г�
			if((g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1)&& (g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8VehicleHaveFlag == 1) && (g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime))
			{
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8CurrentAxleNum+1;	
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].RunAndWalkFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_n8XieZouFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].m_u8NormalLogiclvl;
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].u16AxleList,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0].au16AxleWeight,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi][0],-1);	

				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
					l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"4).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt,2*l_u8tempi+1);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}			
			else if((l_u8tempi < PROMODPARAM.m_u8LaneNum) && (g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) &&\
				(g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime))
			{ 
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8CurrentAxleNum+1;	
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_n8XieZouFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].m_u8NormalLogiclvl;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].RunAndWalkFlag;
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].u16AxleList,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0].au16AxleWeight,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi+1][0],-1);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"5).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi+1,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt,2*l_u8tempi+1);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}

			else if((l_u8tempi < PROMODPARAM.m_u8LaneNum-2) && (g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) && (g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_u8VehicleHaveFlag == 1) &&\
				(g_au32LoopCaTimeBack[l_u8tempi] > g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime))
			{
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_n8CurrentAxleNum+1;	
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u32LoopTime = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_u32LoopTime;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_Capture_LaneNum = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_Capture_LaneNum;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_n8XieZouFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_n8XieZouFlag;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8NormalLogiclvl = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].m_u8NormalLogiclvl;
				g_HideZTReVehicleStruct[2*l_u8tempi+1][0].RunAndWalkFlag = g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].RunAndWalkFlag;
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u16AxleList,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].u16AxleList,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				memcpy(&g_HideZTReVehicleStruct[2*l_u8tempi+1][0].au16AxleWeight,&g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0].au16AxleWeight,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt*sizeof(uint16));
				FInitVehStr(&g_asReVehiclePerChannelForLoop[2*l_u8tempi+2][0],-1);
				
				OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
				l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"6).Rev Match Car ZhongHang=%d %d Hide=%d.\r\n",
											 2*l_u8tempi+2,g_HideZTReVehicleStruct[2*l_u8tempi+1][0].u8AixCnt,2*l_u8tempi+1);
					g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
					OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
					g_u16CurDebugInd++;
					g_u16CurDebugInd %= COM_NUM;
					memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
				OSSemPost(g_pSemSendThtouSP);
			}
			else if(g_HideZTReVehicleStruct[2*l_u8tempi+1][0].m_u8VehicleHaveFlag == 1) //���������խ��û�����к�խ����ɾ��������
			{
				
			}
		}
			
	}
}
void  FIOOutput(uint8* pBuf)
{
//	uint8 l_u8Index=0;
	uint8 l_u8Buf[16];
	uint8 l_u8tempi;
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
//		if(PROMODPARAM.m_au8IOUseEn[10+l_u8tempi] == 1 )//&& l_u8Index<10
		{
			l_u8Buf[l_u8tempi] = pBuf[PROMODPARAM.m_au8IOUseEn[10+l_u8tempi]];
//			l_u8Index ++;
		}

	}
	l_u8Buf[0] == 0? (P3_OUTP_CLR |= Camera_IO1_SET):(P3_OUTP_SET |= Camera_IO1_SET) ;
	l_u8Buf[1] == 0? (P3_OUTP_CLR |= Camera_IO2_SET):(P3_OUTP_SET |= Camera_IO2_SET) ;
	l_u8Buf[2] == 0? (P3_OUTP_CLR |= Camera_IO3_SET):(P3_OUTP_SET |= Camera_IO3_SET) ;
	l_u8Buf[3] == 0? (P3_OUTP_CLR |= Camera_IO4_SET):(P3_OUTP_SET |= Camera_IO4_SET) ;
	l_u8Buf[4] == 0? (P3_OUTP_CLR |= Camera_IO5_SET):(P3_OUTP_SET |= Camera_IO5_SET) ;
	l_u8Buf[5] == 0? (P3_OUTP_CLR |= Camera_IO6_SET):(P3_OUTP_SET |= Camera_IO6_SET) ;
	l_u8Buf[6] == 0? (P3_OUTP_CLR |= Camera_IO7_SET):(P3_OUTP_SET |= Camera_IO7_SET) ;
	l_u8Buf[7] == 0? (P3_OUTP_CLR |= Camera_IO8_SET):(P3_OUTP_SET |= Camera_IO8_SET) ;
	l_u8Buf[8] == 0? (P3_OUTP_CLR |= Camera_IO9_SET):(P3_OUTP_SET |= Camera_IO9_SET) ;
	l_u8Buf[9] == 0? (P3_OUTP_CLR |= Camera_IO10_SET):(P3_OUTP_SET |= Camera_IO10_SET) ;
   	l_u8Buf[10] == 0? (P3_OUTP_CLR |= Camera_IO11_SET):(P3_OUTP_SET |= Camera_IO11_SET) ;
	l_u8Buf[11] == 0? (P3_OUTP_CLR |= Camera_IO12_SET):(P3_OUTP_SET |= Camera_IO12_SET) ;
	l_u8Buf[12] == 0? (P3_OUTP_CLR |= Camera_IO13_SET):(P3_OUTP_SET |= Camera_IO13_SET) ;
	l_u8Buf[13] == 0? (P3_OUTP_CLR |= Camera_IO14_SET):(P3_OUTP_SET |= Camera_IO14_SET) ;
	l_u8Buf[14] == 0? (P3_OUTP_CLR |= Camera_IO15_SET):(P3_OUTP_SET |= Camera_IO15_SET) ;
	l_u8Buf[15] == 0? (P3_OUTP_CLR |= Camera_IO16_SET):(P3_OUTP_SET |= Camera_IO16_SET) ;
}
/************************************
** Function name: FCaptureInfoRec
** Descriptions:  ץ����Ϣ��¼����
*  input parameters:        �����ţ�ץ��ʱ��
** output parameters:       None
** Returned value:          None
**
** Created by:              liumingxin
** Created Date:            2018.08.30
***************************************/
void FCaptureInfoRec(void* Address[4])
{
	uint8 	*lp_u8VehIndex;
	uint32 	*lp_u32CaptureTime;
	int8 	*Dir;
	uint8	*lpheadtail;      //0Ϊͷ,1Ϊβ
	
	uint8 num = CapVehCache;
	uint8 i;
	
	uint8 	l_u8Err;
	uint16 	l_u16SendLen;
	
	lp_u8VehIndex		=	Address[0];
	lp_u32CaptureTime	=	Address[1];
	Dir					=	Address[2];
	lpheadtail			=	Address[3];
	
	for(i = 0; i < num; i++)
	{
		if(g_u8CapFlag[i] == 0)
		{
			g_u8CapFlag[i]= 1;
			g_u32FirstCapTime[i] = *lp_u32CaptureTime;
			
			g_u32CapInfo[i][0] = *lp_u8VehIndex;
			g_u32CapInfo[i][1] = *lp_u32CaptureTime;
			g_u32CapInfo[i][2] = *Dir;
			g_u32CapInfo[i][3] = *lpheadtail;
			break;
		}
	}
	
	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
	l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nFCapRec ID=%08X flag=[%d,%d,%d,%d].",
					  *lp_u32CaptureTime, g_u8CapFlag[0], g_u8CapFlag[1], g_u8CapFlag[2], g_u8CapFlag[3]);
	g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
	OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
	g_u16CurDebugInd++;
	g_u16CurDebugInd %= COM_NUM;
	memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
}

/************************************
** Function name: FSecondeCapture
** Descriptions:  �жϽ��еڶ���ץ�ĺ���
*  input parameters:        �����ţ�ץ��ʱ��
** output parameters:       None
** Returned value:          None
**
** Created by:              liumingxin
** Created Date:            2018.08.30
***************************************/
void FSecondeCapture(void)
{
	uint8 i;
	uint8 cmd = 2;
	uint8 num = CapVehCache;
	uint32 l_u32TimeNow;
	uint32 l_u32TimeDelay;
	void *l_Address[4];
	
	ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
	for(i = 0; i < num; i++)
	{
		l_u32TimeDelay = g_u32FirstCapTime[i] + g_u16CapDelayTime * 6;
		if((g_u8CapFlag[i] == 1) && (l_u32TimeNow > l_u32TimeDelay) && (g_u32FirstCapTime[i] > 0))
		{
			g_u8CapFlag[i] = 0;
			g_u32FirstCapTime[i] = 0;
			
			l_Address[0] = &g_u32CapInfo[i][0];    //VehIndex
			l_Address[1] = &g_u32CapInfo[i][1];    //FirstCaptureTime
			l_Address[2] = &g_u32CapInfo[i][2];    //Dir
			l_Address[3] = &g_u32CapInfo[i][3];    //headtail
			FCapture(l_Address, cmd);
		}
	}
}

/************************************
** Function name: FCapture
** Descriptions:  ץ�ĺ���
*	 input parameters:        �����ţ�ץ��ʱ��
** output parameters:       None
** Returned value:          None
**
** Created by:              zhangtonghan
** Created Date:            2017.06.10
***************************************/
void FCapture(void* Address[4], uint8 cmd)
{
	uint8 	l_u8CJY;
	uint8 	l_u8Err;
	uint16 	l_u16SendLen;
	uint8 	lane,lane1;
	uint8 	*lp_u8VehIndex,p_u8VehIndex;
	uint32 	*lp_u32CaptureTime,p_u32CaptureTime;
	int8 	*Dir;
	uint8	*lpheadtail; //0Ϊͷ,1Ϊβ
	uint8	flag=0;
	uint32 l_u32TimeNow;
	l_u8CJY = 0;
	
	lp_u8VehIndex		=	Address[0];
	lp_u32CaptureTime	=	Address[1];
	Dir					=	Address[2];
	lpheadtail			=	Address[3];
	
	p_u8VehIndex		=	*lp_u8VehIndex;
	p_u32CaptureTime	=	*lp_u32CaptureTime;
	//11-��ͷ  10-��β  01-��ͷ  00-��β
	// U2[11 00], U3[10 01]
	flag 	=	((((*Dir == 1)? 0x01:0x00) << 4) & 0xf0) | (((*lpheadtail == 0) ? 0x01:0x00) & 0x0f);
	
	ReadFPGAGlobalTime(l_u32TimeNow);//��ȡ��ǰʱ��
	lane = (p_u8VehIndex >>4) & 0x0f;
	lane1 = p_u8VehIndex & 0x0f ;
	g_u8CpBuf[0] = 0xee;

	if((flag == 0x11) || (flag == 0x00))
	{
		if(((p_u8VehIndex >>4) & 0x0f) == (p_u8VehIndex & 0x0f))
		{
			g_u8CpBuf[1] = 0xff;
			g_u8CpBuf[2] = (p_u8VehIndex & 0x0f)+1;
			l_u8CJY = (p_u8VehIndex & 0x0f)+1;
			g_u8CpBuf[3] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[3];
			g_u8CpBuf[4] = (p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[4];
			g_u8CpBuf[5] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[5];
			g_u8CpBuf[6] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[6];
			g_u8CpBuf[7] = l_u8CJY;
			g_u8CpBuf[8] = 0xee;
			
			U2SendBytes(&g_u8CpBuf[1], 8);
			
			OSTimeDly(1);
		}
		else
		{
			g_u8CpBuf[1] = 0xff;
			g_u8CpBuf[2] = (p_u8VehIndex & 0x0f)+1;
			l_u8CJY = (p_u8VehIndex & 0x0f)+1;
			g_u8CpBuf[3] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[3];
			g_u8CpBuf[4] = (p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[4];
			g_u8CpBuf[5] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[5];
			g_u8CpBuf[6] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[6];
			g_u8CpBuf[7] = l_u8CJY;
			g_u8CpBuf[8] = 0xee;
			
			l_u8CJY=0;
			g_u8CpBuf[9] = 0xff;
			g_u8CpBuf[10] = ((p_u8VehIndex >> 4) & 0x0f)+1;
			l_u8CJY= ((p_u8VehIndex >> 4) & 0x0f)+1;
			g_u8CpBuf[11] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[11];
			g_u8CpBuf[12]=(p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[12];
			g_u8CpBuf[13] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[13];
			g_u8CpBuf[14] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[14];
			g_u8CpBuf[15] = l_u8CJY;
			g_u8CpBuf[16]=0xee;
			
			U2SendBytes(&g_u8CpBuf[1], 16);
			
			OSTimeDly(1);
		}
	}
	else if((flag == 0x10) || (flag == 0x01))
	{
		if(((p_u8VehIndex >>4) & 0x0f) == (p_u8VehIndex & 0x0f))
		{
			g_u8CpBuf[1] = 0xff;
			g_u8CpBuf[2] = (p_u8VehIndex & 0x0f)+1;
			l_u8CJY = (p_u8VehIndex & 0x0f)+1;
			g_u8CpBuf[3] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[3];
			g_u8CpBuf[4] = (p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[4];
			g_u8CpBuf[5] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[5];
			g_u8CpBuf[6] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[6];
			g_u8CpBuf[7] = l_u8CJY;
			g_u8CpBuf[8] = 0xee;
			
			U3SendBytes(&g_u8CpBuf[1], 8);		
			OSTimeDly(1);
		}
		else
		{
			g_u8CpBuf[1] = 0xff;
			g_u8CpBuf[2] = (p_u8VehIndex & 0x0f)+1;
			l_u8CJY = (p_u8VehIndex & 0x0f)+1;
			g_u8CpBuf[3] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[3];
			g_u8CpBuf[4] = (p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[4];
			g_u8CpBuf[5] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[5];
			g_u8CpBuf[6] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[6];
			g_u8CpBuf[7] = l_u8CJY;
			g_u8CpBuf[8] = 0xee;
			
			l_u8CJY=0;
			g_u8CpBuf[9] = 0xff;
			g_u8CpBuf[10] = ((p_u8VehIndex >> 4) & 0x0f)+1;
			l_u8CJY= ((p_u8VehIndex >> 4) & 0x0f)+1;
			g_u8CpBuf[11] = flag;
			l_u8CJY = l_u8CJY + g_u8CpBuf[11];
			g_u8CpBuf[12]=(p_u32CaptureTime>>8) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[12];
			g_u8CpBuf[13] = (p_u32CaptureTime>>16) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[13];
			g_u8CpBuf[14] = (p_u32CaptureTime>>24) & 0xff;
			l_u8CJY = l_u8CJY + g_u8CpBuf[14];
			g_u8CpBuf[15] = l_u8CJY;
			g_u8CpBuf[16]=0xee;
			
			U3SendBytes(&g_u8CpBuf[1], 16);
			
			OSTimeDly(1);
		}			
	}

	OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nFCap %d,Dir=%d,HeadTail=%d,L=%d,L1=%d,ID=%08X,t=%08x.",
									          cmd, *Dir, *lpheadtail, lane, lane1,p_u32CaptureTime,l_u32TimeNow);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
	OSSemPost(g_pSemSendThtouSP);
}
/************************************
** Function name: FDeleteValidZTOrColum
** Descriptions:  ��ʱɾ��խ���������к���
*	 input parameters:        ��ǰʱ��
** output parameters:       None
** Returned value:          None
**
** Created by:              zhangtonghan
** Created Date:            2017.07.03
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		 zhangtonghan
** Modified date:	  		 20171123
**-------------------------------------------------------------------------------------------------------*/

void  FDeleteValidZTOrColum(uint32 pTime,uint8* pdownloop, uint8* puploop)
{
	uint8 l_u8tempi,l_u8tempj,l_u8tempk;
	uint8 i, id = 0;
	uint8 l_u8Err;
	uint16 l_u16SendLen;
	//uint8 l_u8Flag;
	uint8* l_au8VehPositionFlag;
	uint8* l_au8ReVehPositionFlag;
	uint16 TimeDelay = POINTRATE * g_u16TraceDelTime;    //��ʱʱ��

	l_au8VehPositionFlag 	= pdownloop;
	l_au8ReVehPositionFlag 	= puploop;
	
	for(l_u8tempi=0;l_u8tempi < PROMODPARAM.m_u8LaneNum;l_u8tempi++)
	{
		//PROMODPARAM.m_au8CHLSET[2*l_u8tempi]
		if((l_au8VehPositionFlag[l_u8tempi] == 0) && (l_au8ReVehPositionFlag[l_u8tempi] == 0) && (g_au8HideDeltZTEnable[l_u8tempi] == 0))
		{		
			g_au32HideDeltZTTime[l_u8tempi] = pTime;
			g_au8HideDeltZTEnable[l_u8tempi] = 1;
		}
// 		else
// 		{
// 			g_au8HideDeltZTEnable[l_u8tempi] = 0;
// 		}
	}
	
	for(l_u8tempk=0;l_u8tempk < PROMODPARAM.m_u8LaneNum;l_u8tempk++)
	{
		if(g_au8HideDeltZTEnable[l_u8tempk] == 1)
		{
			if(pTime > (g_au32HideDeltZTTime[l_u8tempk]+TimeDelay))
			{
				for(l_u8tempi=2*l_u8tempk; l_u8tempi <= l_u8tempk*2+1; l_u8tempi++)
				{
					/**** ɾ���켣 ****/
					for(i=0; i<PROMODPARAM.m_u8ZTRow; i++)
					{
						id = l_u8tempi*PROMODPARAM.m_u8ZTRow + i;
						if(g_WheelTrace[id][0].HaveTraceFlag)
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele Trace I=%d,P=%d,[%d,%d,%d,%d]!\r\n",
													g_u16TraceDelTime,id, g_WheelTrace[id][0].LeftWhlZTPos[0][g_WheelTrace[id][0].LeftWhlZTCntPerTrace[0]-1], 
													g_WheelTrace[id][0].HaveTraceFlag,g_WheelTrace[id][1].HaveTraceFlag,
													g_WheelTrace[id][2].HaveTraceFlag,g_WheelTrace[id][3].HaveTraceFlag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							
							memset(&g_WheelTrace[id][0], 0, sizeof(TraceWhel_Struct)*WHEELCACHE);
						}
						if(g_ReWheelTrace[id][0].HaveTraceFlag)
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele ReTrace I=%d,P=%d,[%d,%d,%d,%d]!\r\n",
													g_u16TraceDelTime, id, g_ReWheelTrace[id][0].LeftWhlZTPos[0][g_ReWheelTrace[id][0].LeftWhlZTCntPerTrace[0]-1], 
													g_ReWheelTrace[id][0].HaveTraceFlag,g_ReWheelTrace[id][1].HaveTraceFlag,
													g_ReWheelTrace[id][2].HaveTraceFlag,g_ReWheelTrace[id][3].HaveTraceFlag);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							
							memset(&g_ReWheelTrace[id][0], 0, sizeof(TraceWhel_Struct)*WHEELCACHE);
						}
					}
					/**** end ****/
					
// 					/**** ɾ���� ****/
// 					for(i = 0; i < 2; i++)
// 					{
// 						if(g_WhelData[2*l_u8tempk+i].HaveWheelFlag && (g_WhelData[2*l_u8tempk+i].Dir == -1))
// 						{
// 							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
// 							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"1s Time Dele Wheel C=%d,[%d,%d,%d,%d,%d,%d]!\r\n",
// 													2*l_u8tempk+i, g_WhelData[2*l_u8tempk+i].ZTList[0], g_WhelData[2*l_u8tempk+i].ZTList[1], g_WhelData[2*l_u8tempk+i].ZTList[2],
// 													g_WhelData[2*l_u8tempk+i].ZTList[3], g_WhelData[2*l_u8tempk+i].ZTList[4], g_WhelData[2*l_u8tempk+i].ZTList[5]);
// 							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
// 							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
// 							g_u16CurDebugInd++;
// 							g_u16CurDebugInd %= COM_NUM;
// 							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
// 							OSSemPost(g_pSemSendThtouSP);
// 							
// 							memset(&g_WhelData[2*l_u8tempk+i], 0, sizeof(Whel_Struct));
// 						}
// 					}
// 					/**** end ****/
				
					for(l_u8tempj=0;l_u8tempj<HIDECOLUM_SAVEAXlE;l_u8tempj++)
					{
						if(Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum[0] != -1)
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele HideFC %d,%d,%d,%d!\r\n",
								g_u16TraceDelTime, g_ChlData[Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum[0]].u8Id,Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum[0],
								g_ChlData[Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum[1]].u8Id,Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum[1]);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							memset(&Forward_HideZTColum[l_u8tempi][l_u8tempj],0,sizeof(HideColum_Struct));  //�����г�ʼ��
							memset(&Forward_HideZTColum[l_u8tempi][l_u8tempj].Colum,-1,sizeof(int16)*2); //�����д洢���г�ʼ��Ϊ-1
						}
						if(Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum[0] != -1)
						{
							OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
							l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele HideFC %d,%d,%d,%d!\r\n",
								g_u16TraceDelTime, g_ChlData[Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum[0]].u8Id,Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum[0],
								g_ChlData[Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum[1]].u8Id,Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum[1]);
							g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
							OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
							g_u16CurDebugInd++;
							g_u16CurDebugInd %= COM_NUM;
							memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
							OSSemPost(g_pSemSendThtouSP);
							memset(&Reverse_HideZTColum[l_u8tempi][l_u8tempj],0,sizeof(HideColum_Struct));  //�����г�ʼ��
							memset(&Reverse_HideZTColum[l_u8tempi][l_u8tempj].Colum,-1,sizeof(int16)*2); //�����д洢���г�ʼ��Ϊ-1
						}
						
					}
					if(g_asVehiclePerChannelForLoop[l_u8tempi][0].m_u8VehicleHaveFlag == 1)
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele Veh %d!\r\n",
												g_u16TraceDelTime, l_u8tempi);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						
						FInitVehStr(&g_asVehiclePerChannelForLoop[l_u8tempi][0],1);
					}
					if(g_asReVehiclePerChannelForLoop[l_u8tempi][0].m_u8VehicleHaveFlag == 1)
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele ReVeh %d!\r\n",
												g_u16TraceDelTime, l_u8tempi);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						
						FInitVehStr(&g_asReVehiclePerChannelForLoop[l_u8tempi][0],1);
					}
					if(g_HideZTVehicleStruct[l_u8tempi][0].m_u8VehicleHaveFlag == 1)
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele HideVeh %d!\r\n",
												g_u16TraceDelTime, l_u8tempi);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						
						FInitVehStr(&g_HideZTVehicleStruct[l_u8tempi][0],1);
					}
					if(g_HideZTReVehicleStruct[l_u8tempi][0].m_u8VehicleHaveFlag == 1)
					{
						OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
						l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"%ds Time Dele ReHideVeh %d!\r\n",
												g_u16TraceDelTime, l_u8tempi);
						g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
						OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
						g_u16CurDebugInd++;
						g_u16CurDebugInd %= COM_NUM;
						memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
						OSSemPost(g_pSemSendThtouSP);
						
						FInitVehStr(&g_HideZTReVehicleStruct[l_u8tempi][0],-1);
					}
				}
			}
		}
	}
}

/************************************
** Function name: IsHaveFlag
** Descriptions:  խ���Ա���û�г�
*	 input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              wanglihui
** Created Date:            2017.12.20
**-------------------------------------------------------------------------------------------------------
** Modified by:		zhangtonghan	
** Modified date:	 20180118
** Descriptions:		�޸Ľӿڣ�ʹ����Ӧ˫�����			
**-------------------------------------------------------------------------------------------------------
***************************************/
uint8 IsHaveFlag(void *AddressForAd[9],uint16 *speed)
{
	ZTINFRECELEMENT *lpZTInf;
	Colum_Struct (*Forw_ZTColum)[COLUM_SAVEAXlE];
	Colum_Struct (*Rev_ZTColum)[COLUM_SAVEAXlE];
	VehicleInfForLoop_Struct (*VehiclePerChannelForLoop)[CHL_SAVEVEH];
	VehicleInfForLoop_Struct (*VehicleRevChannelForLoop)[CHL_SAVEVEH];
	uint8 l_u8X,l_u8Y;
	lpZTInf										=	AddressForAd[0];
	Forw_ZTColum							=	AddressForAd[1];
	Rev_ZTColum								=	AddressForAd[2];
	VehiclePerChannelForLoop	=	AddressForAd[4];
	VehicleRevChannelForLoop 	= AddressForAd[5];
	FTransLocation(lpZTInf->m_ucTouch, &l_u8X, &l_u8Y,1);
	switch(l_u8Y)
	{
		case 0x00:		
			if(VehicleRevChannelForLoop[l_u8X][0].m_u8VehicleHaveFlag == 1)
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X][0].u16AxleList[VehicleRevChannelForLoop[l_u8X][0].m_n8CurrentAxleNum]].u16speed;
				return 0x20;
			}
			else if((l_u8X>=1)&&(VehicleRevChannelForLoop[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&((VehicleRevChannelForLoop[l_u8X-1][0].AxleStatus[VehicleRevChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]>=0x04) ||(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(Rev_ZTColum[l_u8X+1][0].Colum[0]==-1))))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X-1][0].u16AxleList[VehicleRevChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x21;
			}
			else if(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(VehicleRevChannelForLoop[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&&((VehicleRevChannelForLoop[l_u8X+1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]!=0x03)||(((l_u8X>0)&&(Rev_ZTColum[l_u8X-1][0].Colum[0]==-1)))))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X+1][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x22;
			}
			else if(((l_u8X+2) < 2*PROMODPARAM.m_u8LaneNum)&&(VehicleRevChannelForLoop[l_u8X+2][0].m_u8VehicleHaveFlag == 1)&&\
				(g_AxleInfo[VehicleRevChannelForLoop[l_u8X+2][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+2][0].m_n8CurrentAxleNum]].AxleStatus<=2)&&((l_u8X <=1)||(Rev_ZTColum[l_u8X-1][0].ZTCnt ==0)))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X+2][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+2][0].m_n8CurrentAxleNum]].u16speed;
				return 0x23;
			}
			else
				return 0;
		case 0x01:
			if(VehicleRevChannelForLoop[l_u8X][0].m_u8VehicleHaveFlag == 1)
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X][0].u16AxleList[VehicleRevChannelForLoop[l_u8X][0].m_n8CurrentAxleNum]].u16speed;
				return 0x20;
			}
			else if(VehiclePerChannelForLoop[l_u8X][0].m_u8VehicleHaveFlag == 1)
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X][0].u16AxleList[VehiclePerChannelForLoop[l_u8X][0].m_n8CurrentAxleNum]].u16speed;
				return 0x10;
			}
			else if((l_u8X>=1)&&(VehiclePerChannelForLoop[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&((VehiclePerChannelForLoop[l_u8X-1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]!=0x03)||(((l_u8X+1)<2*PROMODPARAM.m_u8LaneNum)&&(Forw_ZTColum[l_u8X+1][0].Colum[0]==-1))))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X-1][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x11;
			}
			else if((l_u8X>=1)&&(VehicleRevChannelForLoop[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&((VehicleRevChannelForLoop[l_u8X-1][0].AxleStatus[VehicleRevChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]>=0x04) ||(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(Rev_ZTColum[l_u8X+1][0].Colum[0]==-1))))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X-1][0].u16AxleList[VehicleRevChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x21;
			}
			else if(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(VehiclePerChannelForLoop[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&&((VehiclePerChannelForLoop[l_u8X+1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]>=0x04) ||(((l_u8X>0)&&(Forw_ZTColum[l_u8X-1][0].Colum[0]==-1)))))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X+1][0].u16AxleList[VehiclePerChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x12;
			}
			else if(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(VehicleRevChannelForLoop[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&&((VehicleRevChannelForLoop[l_u8X+1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]!=0x03)||(((l_u8X>0)&&(Rev_ZTColum[l_u8X-1][0].Colum[0]==-1)))))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X+1][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x22;
			}
			else if((l_u8X >= 2)&&(VehiclePerChannelForLoop[l_u8X-2][0].m_u8VehicleHaveFlag == 1)&&
				(g_AxleInfo[VehiclePerChannelForLoop[l_u8X-2][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-2][0].m_n8CurrentAxleNum]].AxleStatus<=2)&&
				((l_u8X+1)<2*PROMODPARAM.m_u8LaneNum)&&(Forw_ZTColum[l_u8X+1][0].Colum[0]==-1))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X-2][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-2][0].m_n8CurrentAxleNum]].u16speed;
				return 0x13;
			}
			else if(((l_u8X+2) < 2*PROMODPARAM.m_u8LaneNum)&&(VehicleRevChannelForLoop[l_u8X+2][0].m_u8VehicleHaveFlag == 1)&&\
				(g_AxleInfo[VehicleRevChannelForLoop[l_u8X+2][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+2][0].m_n8CurrentAxleNum]].AxleStatus<=2)&&((l_u8X <=1)||(Rev_ZTColum[l_u8X-1][0].ZTCnt ==0)))
			{
				*speed=g_AxleInfo[VehicleRevChannelForLoop[l_u8X+2][0].u16AxleList[VehicleRevChannelForLoop[l_u8X+2][0].m_n8CurrentAxleNum]].u16speed;
				return 0x23;
			}
			else
				return 0;
		case 0x02:
			if(VehiclePerChannelForLoop[l_u8X][0].m_u8VehicleHaveFlag == 1)
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X][0].u16AxleList[VehiclePerChannelForLoop[l_u8X][0].m_n8CurrentAxleNum]].u16speed;
				return 0x10;
			}
			else if((l_u8X>=1)&&(VehiclePerChannelForLoop[l_u8X-1][0].m_u8VehicleHaveFlag == 1)&&((VehiclePerChannelForLoop[l_u8X-1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]!=0x03)||(((l_u8X+1)<2*PROMODPARAM.m_u8LaneNum)&&(Forw_ZTColum[l_u8X+1][0].Colum[0]==-1))))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X-1][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x11;
			}
			else if(((l_u8X+1) < 2*PROMODPARAM.m_u8LaneNum)&&(VehiclePerChannelForLoop[l_u8X+1][0].m_u8VehicleHaveFlag == 1)&&((VehiclePerChannelForLoop[l_u8X+1][0].AxleStatus[VehiclePerChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]>=0x04) ||(((l_u8X>0)&&(Forw_ZTColum[l_u8X-1][0].Colum[0]==-1)))))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X+1][0].u16AxleList[VehiclePerChannelForLoop[l_u8X+1][0].m_n8CurrentAxleNum]].u16speed;
				return 0x12;
			}
			else if((l_u8X >= 2)&&(VehiclePerChannelForLoop[l_u8X-2][0].m_u8VehicleHaveFlag == 1)&&
				(g_AxleInfo[VehiclePerChannelForLoop[l_u8X-2][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-2][0].m_n8CurrentAxleNum]].AxleStatus<=2)&&
				((l_u8X+1)<2*PROMODPARAM.m_u8LaneNum)&&(Forw_ZTColum[l_u8X+1][0].Colum[0]==-1))
			{
				*speed=g_AxleInfo[VehiclePerChannelForLoop[l_u8X-2][0].u16AxleList[VehiclePerChannelForLoop[l_u8X-2][0].m_n8CurrentAxleNum]].u16speed;
				return 0x13;
			}
			return 0;
		default:
			return 0;
	}
}

/*********************************************************************************************************
** Function name:     FCoinMatchAdd
** Descriptions:      ����խ��ƥ����������խ��
** input parameters:         
** output parameters: 
**
** Created by: zhangtonghan		  
** Created Date: 20171019
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void 	FCoinMatchAdd(void * Address[6])
{
	HideColum_Struct (*ZTColum)[HIDECOLUM_SAVEAXlE];
	uint8 *pl_u8X,l_u8X;
	uint8 *pl_u8Pos,l_u8Pos,l_u8LCnt=0,l_u8RCnt=0,l_u8LLCnt=0,l_u8RRCnt=0;//
	uint32 LargeRange,LitRange,LargeRange2,LitRange2;
	int8* Dir;
	uint8 l_u8CoinFlg;
	void* l_u32CoinDegAddr[3];
	pl_u8X 		= Address[0];
	pl_u8Pos	=	Address[1];
	ZTColum		=	Address[2];
	Dir				= Address[3];
	l_u8X 	= *pl_u8X;
	l_u8Pos	=	*pl_u8Pos;

	if(l_u8X == 0)
	{
		l_u8RCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
		l_u8RRCnt = FindHideColumEnd(&ZTColum[l_u8X+2][0],HIDECOLUM_SAVEAXlE,1);
	}
	else if(l_u8X == (PROMODPARAM.m_u8LaneNum*2-1))
	{
		l_u8LCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
		l_u8LLCnt = FindHideColumEnd(&ZTColum[l_u8X-2][0],HIDECOLUM_SAVEAXlE,1);
	}
	else
	{
		l_u8LCnt = FindHideColumEnd(&ZTColum[l_u8X-1][0],HIDECOLUM_SAVEAXlE,1);
		l_u8RCnt = FindHideColumEnd(&ZTColum[l_u8X+1][0],HIDECOLUM_SAVEAXlE,1);
		l_u8LLCnt = FindHideColumEnd(&ZTColum[l_u8X-2][0],HIDECOLUM_SAVEAXlE,1);
		l_u8RRCnt = FindHideColumEnd(&ZTColum[l_u8X+2][0],HIDECOLUM_SAVEAXlE,1);
	}
	
	if(l_u8LCnt == 1)
	{
		if((ZTColum[l_u8X-1][0].FristElemRow == 0)&&((g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt))))
		{
			if(IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm))
			{
				if(*Dir == -1)
				{
					ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 7;
					if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 7;
						ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				else
				{
					ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 6;
					if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 6;
						ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
				//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
			else
			{
				if(*Dir == -1)
				{
					ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 6;
					if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 6;
						ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				else
				{
					ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 7;
					if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
					{
						ZTColum[l_u8X-1][0].RightMtchFlag = 7;
						ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
				//ZTColum[l_u8X][l_u8Pos].DueTime +=GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
			
		}
		else if(ZTColum[l_u8X-1][0].FristElemRow == 0)
		{
			LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt)));
			LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt)));				
			
			//�غ϶Ⱥã�����Ϊ��һ����
			if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
			{
				ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 3;
				ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
				if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
				{
					ZTColum[l_u8X-1][0].RightMtchFlag = 3;
					ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
				}
				ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
			else 
			{
				if(IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm))
				{
					if(*Dir == 1)
					{
						ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 6;
						if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 6;
							ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					else
					{
						ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 7;
						if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 7;
							ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
					//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
					ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
				}
				else
				{
					if(*Dir == 1)
					{
						ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 7;
						if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 7;
							ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					else
					{
						ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 6;
						if(ZTColum[l_u8X-1][0].RightMtchFlag == 0)
						{
							ZTColum[l_u8X-1][0].RightMtchFlag = 7;
							ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
					//ZTColum[l_u8X][l_u8Pos].DueTime +=GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
					ZTColum[l_u8X-1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
				}
			}
		}
		else
		{
			l_u8CoinFlg = 0;
			l_u32CoinDegAddr[0] = &ZTColum[l_u8X][l_u8Pos].Colum[1];
			l_u32CoinDegAddr[1] = &ZTColum[l_u8X-1][0].Colum[0];
			l_u32CoinDegAddr[2] = &l_u8CoinFlg;
			TwoZTCoinDegFunc(l_u32CoinDegAddr);
			if(l_u8CoinFlg)
			{
				ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 0x03;
				ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
				//ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 1;
				ZTColum[l_u8X-1][0].RightMtchFlag = 0x03;
				ZTColum[l_u8X-1][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[1];
			}
		}
	}
	else if(l_u8LCnt == 2)
	{
		LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt)));
		LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt)));				
		LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt)));
		LitRange2 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt)));				
			
		if(((g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt))) && \
			((g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt))))
		{
			ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 2;
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4;
		}
		else if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
		{
			ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 3;
			ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][0].Colum[0];
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4 + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][0].Colum[0]].u32Maxtm);
		}
		else if(((LitRange2*100) >= (LargeRange2*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
		{
			ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 3;
			ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-1][1].Colum[0];
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4 + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-1][1].Colum[0]].u32Maxtm);
		}
		else
		{
			ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 2;
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4;
		}
	}
	else if(l_u8LCnt == 0)
	{
		ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 1;
	}
	//�Ҳ�
	if(l_u8RCnt == 1)
	{
		if((ZTColum[l_u8X+1][0].FristElemRow == 0)&&((g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt))))
		{
			if(IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm))
			{
				if(*Dir == 1)
				{
					ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 7;
					if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
						ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				else
				{
					ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 6;
					if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
						ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
				
				//ZTColum[l_u8X][l_u8Pos].DueTime +=GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
			else
			{
				if(*Dir == 1)
				{
					ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 6;
					if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
						ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				else
				{					
					ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 7;
					if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
					{
						ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
						ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
					}
				}
				//ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
				//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
		}
		else if(ZTColum[l_u8X+1][0].FristElemRow == 0)
		{
			LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt)));
			LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt)));				
			
			//�غ϶Ⱥã�����Ϊ��һ����
			if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
			{
				ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 3;
				ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
				
				if(ZTColum[l_u8X+1][0].LeftMtchFlag ==0)
				{
					ZTColum[l_u8X+1][0].LeftMtchFlag = 3;
					ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
				}
				//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
			else 
			{
				if(IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm))
				{
					if(*Dir == 1)
					{
						ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 7;
						if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
							ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					else
					{
						ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 6;
						if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
					//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
					ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
				}
				else
				{
					if(*Dir == 1)
					{
						ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 6;
						if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 6;
							ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					else
					{
						ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 7;
						if(ZTColum[l_u8X+1][0].LeftMtchFlag == 0)
						{
							ZTColum[l_u8X+1][0].LeftMtchFlag = 7;
							ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
						}
					}
					ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
					//ZTColum[l_u8X][l_u8Pos].DueTime +=GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
					ZTColum[l_u8X+1][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
				}
			}
		}
		else
		{
			l_u8CoinFlg = 0;
			l_u32CoinDegAddr[0] = &ZTColum[l_u8X][l_u8Pos].Colum[1];
			l_u32CoinDegAddr[1] = &ZTColum[l_u8X+1][0].Colum[0];
			l_u32CoinDegAddr[2] = &l_u8CoinFlg;
			TwoZTCoinDegFunc(l_u32CoinDegAddr);
			if(l_u8CoinFlg)
			{
				ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 0x03;
				ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
				//ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 1;
				ZTColum[l_u8X+1][0].LeftMtchFlag = 0x03;
				ZTColum[l_u8X+1][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[1];
			}
		}
	}
	else if(l_u8RCnt == 2)
	{
		LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt)));
		LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt)));				
		LargeRange2 = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt)));
		LitRange2 = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt)));				
			
		if(((g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt))) && \
			((g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)) ||\
			(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm < GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt))))
		{
			ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 2;
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4 ;
		}
		else if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
		{
			ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 3;
			ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][0].Colum[0];
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4 + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][0].Colum[0]].u32Maxtm);
		}
		else if(((LitRange2*100) >= (LargeRange2*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
		{
			ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 3;
			ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+1][1].Colum[0];
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4 + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+1][1].Colum[0]].u32Maxtm);
		}
		else
		{
			ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 3;
			ZTColum[l_u8X][l_u8Pos].DueTime =g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm + GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4;
		}
	}
	else if(l_u8RCnt == 0)
	{
		ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 1;
	//	ZTColum[l_u8X][l_u8Pos].DueTime = GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[1]].u32Maxtm)/4;
	}
	//�������е������һ�������
	if((ZTColum[l_u8X][l_u8Pos].LeftMtchFlag == 1)&&(ZTColum[l_u8X][l_u8Pos].RightMtchFlag == 1))
	{
		if((l_u8LLCnt == 1)&&(l_u8RRCnt == 0))
		{
			LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32realCnt)));
			LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32realCnt)));				
			
			//�غ϶Ⱥã�����Ϊ��һ����
			if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
			{
				ZTColum[l_u8X][l_u8Pos].LeftMtchFlag = 8;
				ZTColum[l_u8X][l_u8Pos].WonderLMtchID = ZTColum[l_u8X-2][0].Colum[0];
				
				if(ZTColum[l_u8X-2][0].RightMtchFlag ==0)
				{
					ZTColum[l_u8X-2][0].RightMtchFlag = 8;
					ZTColum[l_u8X-2][0].WonderRMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
				}
			//	ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X-2][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X-2][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
		}
		else if((l_u8LLCnt == 0)&&(l_u8RRCnt == 1))
		{
			LargeRange = GetDiff(IsBig(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsLit(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32realCnt)));
			LitRange = GetDiff(IsLit(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm),IsBig(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32realCnt)));				
			
			//�غ϶Ⱥã�����Ϊ��һ����
			if(((LitRange*100) >= (LargeRange*g_ZTWaveOverLap)) ||\
				((IsBigFlag(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32realCnt))) ||\
			  (IsBigFlag(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm) && IsBigFlag(GetDiff(g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32realCnt),GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32realCnt)))))
			{
				ZTColum[l_u8X][l_u8Pos].RightMtchFlag = 8;
				ZTColum[l_u8X][l_u8Pos].WonderRMtchID = ZTColum[l_u8X+2][0].Colum[0];
				
				if(ZTColum[l_u8X+2][0].LeftMtchFlag ==0)
				{
					ZTColum[l_u8X+2][0].LeftMtchFlag = 8;
					ZTColum[l_u8X+2][0].WonderLMtchID = ZTColum[l_u8X][l_u8Pos].Colum[0];
				}
				//ZTColum[l_u8X][l_u8Pos].DueTime += GetDiff(g_ChlData[ZTColum[l_u8X][l_u8Pos].Colum[0]].u32Maxtm,g_ChlData[ZTColum[l_u8X+2][0].Colum[0]].u32Maxtm);
				ZTColum[l_u8X+2][0].DueTime = ZTColum[l_u8X][l_u8Pos].DueTime;
			}
		}
	}
}


