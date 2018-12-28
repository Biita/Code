/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			DEFConfig.h
** Last modified Date:  20140324
** Last Version:		1.0
** Descriptions:		宏常量
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
*                                         版本号
*********************************************************************************************************
*/
/*
	* 1700为非现场新布局新逻辑起始版本，区别于1600起始的旧布局协议抓拍及1500起始的旧布局非协议抓拍
	* 1710为四排窄条逻辑,起始版本为20170810，基于1700的 20170809版本
	* 1720为德清PK程序版本
	* 1800为四排数字和一排中航的起始版本(三排交错板块+2排一字隐藏板块),其中隐藏板块未一排中航+一排数字
	*
*/
#define		YBVERSION	30							// 仪表版本号
#define		PROGRAMVERSION		"WJ-WIM-1800"	   	// 程序版本号，说明：万集-动态称重-版本号(4)
#define		PROGVERSION_POS_0	1					
#define		PROGVERSION_POS_1	5
#define		PROGVERSION_POS_2	0
#define		PROGVERSION_POS_3	2
#define		PROGMODIFYDATE		"20181225"	  		// 程序修改日期

/*
*********************************************************************************************************
*                                         程序调试配置参数、使能等
*********************************************************************************************************
*/
//#define		DEBUG_NT						1			//调试状态:1 调试；	0 非调试  
//#define		ISDEBUG							(DEBUG_NT > 0)	//是调试  		  	
//#define		NOTDEBUG						(DEBUG_NT == 0)	//不是调试

// 功能使能
//#define		SHOWVEHPASSDEBUG 				1			// 显示动态过车状态 F4代码	  
//#define 	PROC_DOG_EN						1			// 启动看门狗功能
//#define		CAMERACAPTURE					1		 	// 抓拍
//#define		SENDWAVEENABLE					1		//发送波形使能	若关闭，则无法发送波形，即使设置相应选项。										//非调试状态时，该值自动设为0
//#define		LOGICENABLE						1		// FPGA中断中，控制向逻辑抛窄条使能，如果为1，则向Task1抛窄条，为0，不抛 wujinlong 20130114


/***********
   使能
***********/
//#define DEBUG_PRINT_EN						0		// 串口打印信息输出开关宏，杨明
#define	DEBUGINFO_TASK_SP_EN				1		// 调试信息任务串口输出使能
#define	DEBUGINFO_LASER_EN					0		// 调试信息，激光接收输出使能
//#define DEBUGINFO_LOGIC_LASER_EN			0		// 调试信息：逻辑中输出激光信息使能
//#define	DEBUG_SP_OP_WARTING_EN				1		// 调试信息串口输出WaitingArea
//
//
//
//#define	COIL_LOGIC_EN						1		// 线圈逻辑使能
//
//#define	NOVEH_CLR_EN						0		// 无车清空使能
// 机制：定时（10s）发送心跳，如果一定时间内（3s）没有收到回复，且达到3次，则重置
// 如果收到回复，则回复时间清零，标志清零，失败次数清零
//#define	NET_HEARTBEAT_EN					0		// 网络心跳使能，20130904
// 
//#define	NET_KEEPALIVE_EN					1		// 网络保活机制使能，20130910
//
#define TASKTIMEENABLE 						1		// 统计计时使能，浩叔


//#define PRTCL_NO_WAVE_EN					1		// 协议中0x81不发波形



// 以下使能为现场所加，可能存在仅在方向1中添加而反向0中未添加的现象
//#define	LOGIC_FILTER_SAMEZT					1		// 滤除一定时间内同一窄条的中断 20131018
//#define	LOGIC_LASER_POS						1		// 抛轴时，增加车辆位置，是靠左还是靠右 20131018

//
//#define	VEH_ADDAXLE_EN						1		// 补轴使能												 20131019
//#define	VEH_FILTERWEIGHT0_EN				1		// 滤除重量为0的轴

//#define	LOGIC_FILTER_SAMEZT_INAXLE_EN		1		// 抛轴时，滤除相同的窄条								 20131020
//#define	LOGIC_FILTER_ZTDNIME_OVER_EN		0		// 抛轴时，滤除下秤时间超过范围的窄条
													// 该功能在算法中添加，逻辑中可删除，20131026
					 

//#define	LOGIC_FILTER_YAFENG_EN				0	    // 滤除小乌龟模式使能			 20131025
//#define LOGIC_COIL_NOTUSING_YAFENG			1		// 线圈模式中不使用小乌龟


//#define LOGIC_NOT_WONDERPROCESS_EN			0		// 逻辑中不使用WonderProcess处理，1：不使用，0：使用   20131028
//#define	LOGIC_YAFENG_PROCESS_EN				1		// 新的压缝处理机制

//#define	LOGIC_NOT_LASER_EN					0		// 逻辑中不使用激光使能，直接在串口中断不抛激光数据信号量 20131107
//#define	VEH_NOT_POST_WT0					0		// 组帧时，如果重量为0，则不组帧不发送，直接返回

//#define	LOGIC_FTR_LSR_SPR_EN				0		// 逻辑中滤除激光分车使能，1使能，0不使能，20131112

//#define	VEH_ADDAXLE_LIMIT					1		// 出车时增轴增加轴重判断，20131212


//#define	LOGIC_POST_AXLE_DELAY				1		// 延时抛轴，检测最后一排窄条的下秤时间和当前时间比较，大于阈值，则直接抛轴，wujinlong 20140715
//#define	LOGIC_POSTAXLE_DELAYTIME			1000	// 抛轴延时时间




//#define	LASER_UNNORMAL_EN					1		// 激光异常处理 wujinlong 20140707
													// 激光控制器异常，不发送心跳，ARM接收不到心跳，认为激光是异常，转为线圈模式
													// 线圈模式下，不使用压缝窄条，出车时与前一车对比，判断轴数、重量、时间等
													// 30min中后继续检测激光控制器心跳
//#if LASER_UNNORMAL_EN > 0
//#define	LAS_UNN_MATCHTIMR					1000		// 车辆超时判断，当前时间和车辆时间超过1s，则扔掉，在抛车时判断即可
//#define	LAS_UNN_OVERTIMR					6000		// 车辆超时判断，当前时间和车辆时间超过1s，则扔掉，在抛车时判断即可
//#define	LAS_UNN_MODELTHRANS_OVERTIME		(30*60*2)	// 30min，在时钟任务中，0.5s一次，共30*60*2次
//#define LAS_UNN_WEIGHT_DOWN					80			// 重量下限	  70/100
//#define LAS_UNN_WEIGHT_UP					120			// 重量上限
//#endif


/* SD卡存储机制 20131108
 * 没有检测到上位机心跳(0x95)，将0x81指令保存到SD卡
 * 检测到上位机心跳时，将SD卡内容发送出去   */
#define	SD_DATA_STORAGE_EN					1		// SD卡数据存储总开关  
#define	SD_OFFNET_SAVE_EN					1		// 断网缓存功能 使能
#define	SD_DEBUGINFO_EN						1		// 调试信息存储功能  使能
#define	SD_WAVE_SAVE_EN						1		// 波形存储功能  使能


/***********
 测试 使能
***********/
// 一波车，通知上位机开始和结束
//#define TEST_WAVE_EN 						1		// 一波车使能 20130911

//#define TEST_NOTWAVE_ZTINFO_EN 				0		// 不发波形，发送窄条信息，20131021


// 测试使用
//#define		TEST_IO_EN			0			// 测试抓拍IO
//#define		NET2VEHINFO				0		// 通过网口发送车辆轴信息和车信息	   wujinlong 20130107



/*
*********************************************************************************************************
*                                         程序总体
*********************************************************************************************************
*/																																 
#define		SUPERPWD			13801380  	//超级权限密码

// 系统初始化
#define START_NOT_OVER				0x00
#define START_OVER					0x01 
#define TASK_BEGIN_WAIT_TICKS		(20*OS_TICKS_PER_SEC/1000)		// 任务启动后，为了防止一直占用影响其他任务的创建，需先延时




/*
*********************************************************************************************************
*                                         参数、常量相关
*********************************************************************************************************
*/
#define		ZHOUSIZE			16			 //单个车辆包含的最大轴数

//#define 	MAX_SPEED_LIMIT		81			//修正的最大速度 km/h  
//#define	SPEED_CTL_SIZE		17			//修正的速度间隔段 5km/h
#define 	MAX_SPEED_LIMIT		100			//修正的最大速度 km/h
#define		SPEED_CTL_SIZE		20			//修正的速度间隔段 5km/h

#define		VSMSTANDARD			10000		// 车型速度修正基准值，常量

#define 	ENABLE_CONFIG		1		 	// 使能配置功能项，常量
#define		DISABLE_CONFIG		0	

#define 	COM_MODE1			1		 	// 串口/网口通信模式，轴重模式
#define 	COM_MODE0			0			// 轴组重模式

//动静态
//#define		UI_STATIC			0
//#define		UI_MOTION			1

//#define 	MAXBUFPOINT			2000			//每根窄条存储的点个数最大值
#define 	MAXBUFPOINT			4000			//每根窄条存储的点个数最大值 


#define WAVE_BUFSIZE	64
//#define WAVE_BUFMAX		0x1F


// 网络
#define 	SNDFRAMESIZE		1440	 	//发送数据帧的长度
#define		SENDALLSIZE			100000		//发送数据总长度
#define		SNDFRAMEBESTSIZE	900			//最佳发送数据帧长度，此长度为数据内容长度，考虑到存储SD卡，保证1帧存储在2个扇区中
											//不包括帧头、帧尾数据，帧头帧尾长度控制在20-30字节左右，20121107

// 重量
#define	VEH_AXLE_MIN_WEIGHT			20			 	// 轴最小重量，实际是20*10Kg
#define	VEH_AXLE_DEF_WT				75				// 默认轴重75*10Kg
#define	VEH_AXLE1_MAX_WT			1000			// 轴1最大轴重1000*10Kg，10t
#define	VEH_AXLE1_DEF_WT			150				// 轴1默认轴重150*10Kg，1.5t
//#define	VEH_ADDAXLE_LIMIT_VAL		8000			// 增轴时，增加第1轴重量判断，超过8t，进行处理
// 速度
#define	VEH_AXLE_DEF_SPEED			40*10			// 默认速度40*10*0.1Km/h
// 轴距
#define	VEH_AXLE_DEF_DIST			200				// 默认轴距，200cm，2m
#define	VEH_AXLE_DIST_11X			140				// 11X型轴距

// 时间
//#define	LOGIC_SAMEZT_TIMEINTERVAL	50				// 相同窄条触发时间，20131018
//
//#define	LOGIC_AXLE_ZTDN_OVERTIME	6000			// 抛轴时，判断窄条时间差
//
//#define	LOGIC_FILTER_YAFENG_TIMEOVER	9000		// 清除压缝窄条时限		  点数
//#define	LOGIC_MATCH_YAFENG_TIMEOVER		600			// 轴匹配压缝窄条时限
//
//#define	LOGIC_COIL_ON_TIME_MAX			(100*OS_TICKS_PER_SEC/1000)		// 延时100ms


// 分度值
//#define FEN_10						10				// 分度值
//#define FEN_20						20
//#define FEN_50						50
//#define FEN_100						100



// 最大速度
#define	VEH_OP_MAX_SPEED			1200			// 120Km/h,1200*0.1Km/h
#define	VEH_OP_DEFINE_SPEED			(VEH_OP_MAX_SPEED*8/10)

//#define	LOGIC_NOTUSINGZT_LEN		6*2				// 逻辑不用窄条

//#define	LOGIC_POSTAXLE_MAX_CHLNUM	20				// 抛轴最大窄条数

// 车型速度修正最大最小默认值
#define	PARA_VEHTPYE_SPEED_MODIFY_MIN	8000
#define	PARA_VEHTPYE_SPEED_MODIFY_MAX	12000



/*
*********************************************************************************************************
*                                         窄条相关
*********************************************************************************************************
*/
#define		CHANNELNUM			64		  			//窄条的数量
#define   ZERONUM   344                 //8+48*7=344
//#define 	CHANNALROWSUM		8		  			//一排的窄条数，激光使用
#define 	CHANNALLINESUM		4 					//共几排窄条
//#define 	CHANNALLINESUM_LOGIC 3 					//逻辑排数

//#define		CHANNELWIDE			175					// 窄条宽度，即窄条长度 175






/*
*********************************************************************************************************
*                                         采集板相关
*********************************************************************************************************
*/
#define		POINTRATE			6000				//采样率

/*
*********************************************************************************************************
*                                         激光相关
*********************************************************************************************************
*/
#define		LASTERCOUNT			4			//激光器的数量
#define		LASTERIOCNT			(LASTERCOUNT<<1)		//激光器IO口的数量

// 激光串口车辆总数
#define	SP_LASER_VEHNUM_MAX			(LASTERCOUNT<<1)
/*
*********************************************************************************************************
*                                         任务相关
*********************************************************************************************************
*/
// 互斥对象优先级，必须高于所有任务
//#define	MUTEXCOM1PRIO	 			2				// 串口1输出互斥对象


// 任务优先级，优先级0-3 保留	 
#define	TASK_LOGIC_PRIO		 		4				// 逻辑任务
#define	TASK_TIME_PRIO		 		6				// 时钟任务
#define	TASK_ALG_PRIO		 		7				// 轴重计算任务
#define	TASK_VEH_PRIO		 		8				// 收尾抛车任务
#define TASK_JDINF_PRIO             9               // 交调信息上传任务
#define TASK_UI_PRIO				25				// 显示界面任务
#define	Task_LASERINTR_PRIO 		10				// 激光交互任务

#define	TASK_COMM_PRIO		 		12				// 通信任务
#define	TASK_SD_PRIO 	 			14 				// SD卡任务，由于是断网缓存，此2任务优先级相当

#define	TASK_WAVE_PRIO 	 			16				// 发波形任务
#define	TASK_KEY_PRIO		 		18				// 键盘任务，由于非现场不使用键盘，故取消创建任务
#define TASK_PRI_CHKTASK   			19
#define	TASK_DEBUGINFO_PRIO		 	20				// 调试信息输出任务
//#define TASK_TEST_PRIO	 			22				// 测试任务，平时关闭

#define	TASK_START_PRIO	 			26				// 界面任务，仅为灯闪烁等功能

//#define  	ErrAxleMsg			 "轴计算消息队列已满!" 
//#define  	ErrSndPCMsg 		 "发送消息队列已满!"
//#define 	ErrCalcVehMsg 		 "车辆计算消息队列已满!"
//#define 	ErrShowVehMsg 		 "车辆显示消息队列已满!"
//#define 	ErrShowAxleMsg 		 "轴显示消息队列已满!"
//配置参数空间大小 
#define		SETUPPARAMSIZE	 		6756	   
#define 	RESERVED				(10)			//配置参数空间预留字节数 
#define		PROMODPARAMSIZE	 		1024	   
#define 	PROMOD_RESERVED			(270)			//配置参数空间预留字节数
#define		JDSYSPARAMSIZE	 		512
#define     JDSYS_RESERVED			366



//任务栈空间大小
// 先按照以前的来，最后统计任务使用堆栈情况		
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
*                                         逻辑相关
*********************************************************************************************************
*/
#define	LASER_VEH_MIN_ITV			80					// 80cm，激光给的车辆最小左右距离


/*
*********************************************************************************************************
*                                         算法相关
*********************************************************************************************************
*/
#define		DNNUM				100			//逻辑下称判断，连续超过DOWNNUM个点，认为下称 //浩叔 20130516 1000太高 改为200
#define		UPNUM				100			//逻辑上称判断，连续超过UPNUM个点，认为下称		//浩叔 20130516 1000太高 改为200
#define 	WAVEMINSIZE			80			//最小波形宽度	
#define 	DEFUPPOINT			7			//上秤点数
#define 	DEFDNPOINT			7			//下秤点数
#define		DEFLUPVALUE			40			//上秤阈值，较小
#define		DEFLDNVALUE			40			//下秤阈值，较小



/*
*********************************************************************************************************
*                                         协议
*********************************************************************************************************
*/
#define 	PROTOL_WJ_OLD		0x00	//旧高速预检协议
#define 	PROTOL_WJ_NEW		0x01	//新万集协议
#define 	PROTOL_FXC			0x02	//非现场执法协议

// 组帧
#define		COMMSENDWAVESTART	0x00		//首帧  
#define		COMMSENDWAVETRANS	0x01		//中间数据帧
#define		COMMSENDWAVEEND		0x02		//尾数据帧



/*
*********************************************************************************************************
*                                         SD卡
*********************************************************************************************************
*/
// 使能常量
#define	SD_DATA_STORAGE					0x01
#define	SD_OFFNET						0x02
#define	SD_VEHINFO						0x04
#define	SD_DEBUGINFO					0x08
#define	SD_WAVE							0x10

// 索引扇区
#define	SD_CONTEXT_ZONG					0
#define	SD_CONTEXT_OFFNET_SAVE			1		 // 1-10
#define	SD_CONTEXT_VEHINFO				11		 // 11-20
#define	SD_CONTEXT_SYSLOG				21		 // 21-30
#define	SD_CONTEXT_WAVE					31		 // 31-40
#define SD_CONTEXT_JD                   41       // 41-50
//
#define	SD_TASK_TIMEDLY					2000						// 任务延时


#define	SD_NET_CONNECT_JUDGE_CNT		(3+1)						// 3次，心跳时间5s
#define	SD_NET_CON_JUDGE_TIME			(SD_NET_CONNECT_JUDGE_CNT*5/(SD_TASK_TIMEDLY/1000))	 // 时间计数判断
//
#define	SD_SECTOR_LEN					512							// 扇区长度
#define	SD_NEW_CARD_FLAG_LEN			10			 				// SD卡新卡校验数据长度
#define	SD_NEW_SECTOR_FLAG_LEN			4			 				// SD卡新扇区校验数据长度
//
#define	SD_SEC_NUM_FRAM					2							// 每帧扇区数
//
#define	SD_ANTISHAKE_CNT				5							// 防抖处理计数
//
#define	SD_IX_LEN_PER					20							// 每条记录长度
#define	SD_IX_NUM						25							// 索引区存储个数，20字节一个


#define	SD_TDC_SIZE						532							// 铁电每个功能大小532B
#define	SD_TDC2SD_TIME					30							// SD卡任务2s一循环，5min是300s

/////////////////////////
// 断网缓存功能相关
#define	SD_OFFNET_SEC_BGN				m_u32Offnet_Addr_Begin
#define	SD_OFFNET_SEC_DATA_BGN			m_u32Offnet_Data_Begin			
#define	SD_OFFNET_SEC_END				m_u32Offnet_Addr_End
// 索引内容地址
#define	SD_CTX_OFFNET_FLAG				0							// 0-3
#define	SD_CTX_OFFNET_VEHNUM			4							// 4-7
#define	SD_CTX_OFFNET_SEC_BGN			8							// 8-11
#define	SD_CTX_OFFNET_SEC_END			12							// 12-15
#define	SD_CTX_OFFNET_SEC_DATA_BEGIN	14
#define	SD_CTX_OFFNET_WR_SEC			20
#define	SD_CTX_OFFNET_WR_SUB			24
#define	SD_CTX_OFFNET_RD_SEC			26
#define	SD_CTX_OFFNET_RD_SUB			30								
#define	SD_CTX_VEH_NUM					32							// 32-35车辆总数		36- 预留
// 扇区内容地址
#define	SD_OFFNET_SEC_FLAG				0							// 0-3
#define	SD_OFFNET_SEC_FRAM_FLAG			4							// 4-7
#define	SD_OFFNET_SEC_FRAM_LEN			8							// 8-9
#define	SD_OFFNET_SEC_FRAM_NUM			10							// 10-11
#define	SD_OFFNET_SEC_VEH_IX			12							// 12-15      16-29 预留  
#define	SD_OFFNET_SEC_FRAM_DATA			30							// 30- 数据
#define	SD_OFFNET_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_OFFNET_SEC_FRAM_DATA)
//
#define	SD_VEH_FRAME_NUM				100							// 每车最大帧数
#define	SD_VEH_SECTOR_NUM				(SD_VEH_FRAME_NUM*SD_SEC_NUM_FRAM)		// 每车最大扇区数
#define	SD_VEH_MAX_NUM					((SD_OFFNET_SEC_END-SD_OFFNET_SEC_DATA_BGN+1)/SD_VEH_SECTOR_NUM/SD_IX_NUM*SD_IX_NUM)  // 保证索引是整扇区数



////////////////////////////////
// 车辆信息
#define	SD_VEH_SEC_BGN					m_u32VehInfo_Addr_Begin	
#define	SD_VEH_SEC_DATA_BGN				m_u32VehInfo_Data_Begin		
#define	SD_VEH_SEC_END					m_u32VehInfo_Addr_End
// 索引内容地址
#define	SD_CTX_VEH_FLAG					0							// 0-3
#define	SD_CTX_VEH_SUM					4							// 4-7		
#define	SD_CTX_VEH_SEC_BGN				8							// 8-11
#define	SD_CTX_VEH_SEC_END				12							// 12-15
#define	SD_CTX_VEH_NEXT					16							// 16-19
#define	SD_CTX_VEH_CUR					20							// 20-23      24- 预留
// 扇区内容地址
#define	SD_VEH_SEC_FLAG					0							// 0-3
#define	SD_VEH_SEC_VEH_FLAG				4							// 4-7
#define	SD_VEH_SEC_LEN					8							// 8-9      10-29 预留  
#define	SD_VEH_SEC_DATA					30							// 30- 数据
#define	SD_VEH_SEC_DATA_LEN				(SD_SECTOR_LEN-SD_VEH_SEC_DATA)
//
#define	SD_VEH_MAXNUM					(SD_VEH_SEC_END-SD_VEH_SEC_DATA_BGN+1)



////////////////////////////////
// 调试信息保存功能相关
#define	SD_DBGINFO_SEC_BGN				m_u32Log_Addr_Begin	
#define	SD_DBGINFO_SEC_DATA_BGN			m_u32Log_Data_Begin		
#define	SD_DBGINFO_SEC_END				m_u32Log_Addr_End
// 索引内容地址
#define	SD_CTX_DBGINFO_FLAG				0							// 0-3
#define	SD_CTX_DBGINFO_LOGSUM			4							// 4-7		
#define	SD_CTX_DBGINFO_SEC_BGN			8							// 8-11
#define	SD_CTX_DBGINFO_SEC_END			12							// 12-15
#define	SD_CTX_DBGINFO_NEXTLOG			16							// 16-19
#define	SD_CTX_DBGINFO_CURLOG			20							// 20-23      24- 预留
// 扇区内容地址
#define	SD_DBGINFO_SEC_FLAG				0							// 0-3
#define	SD_DBGINFO_SEC_LOG_FLAG			4							// 4-7
#define	SD_DBGINFO_SEC_LOG_LEN			8							// 8-9      10-29 预留  
#define	SD_DBGINFO_SEC_LOG_DATA			30							// 30- 数据
#define	SD_DBGINFO_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_DBGINFO_SEC_LOG_DATA)
//
#define	SD_LOG_MAX_NUM					(SD_DBGINFO_SEC_END-SD_DBGINFO_SEC_DATA_BGN+1)


/////////////////////////
// 波形存储功能相关
#define	SD_WAVE_SEC_BGN					m_u32Wave_Addr_Begin
#define	SD_WAVE_SEC_DATA_BGN			m_u32Wave_Data_Begin			
#define	SD_WAVE_SEC_END				    m_u32Wave_Addr_End
// 索引内容地址
#define	SD_CTX_WAVE_FLAG				0							// 0-3
#define	SD_CTX_WAVE_ZTNUM				4							// 4-7
#define	SD_CTX_WAVE_SEC_BGN				8							// 8-11
#define	SD_CTX_WAVE_SEC_END				12							// 12-15
#define	SD_CTX_WAVE_NEXTZT				16							// 16-19
#define	SD_CTX_WAVE_CURZT				20							// 20-23      24- 预留
// 扇区内容地址
#define	SD_WAVE_SEC_FLAG				0							// 0-3
#define	SD_WAVE_SEC_ZT_FLAG				4							// 4-7
#define	SD_WAVE_SEC_ZT_LEN				8							// 8-9
#define	SD_WAVE_SEC_ZT_NUM				10							// 10-11
#define	SD_WAVE_SEC_ZT_IX				12							// 12-15      16-29 预留  
#define	SD_WAVE_SEC_ZT_DATA				30							// 30- 数据
#define	SD_WAVE_SEC_DATA_LEN			(SD_SECTOR_LEN-SD_WAVE_SEC_ZT_DATA)
//
#define	SD_WAVE_ZT_MAX_NUM				((SD_WAVE_SEC_END-SD_WAVE_SEC_DATA_BGN+1)/SD_SEC_NUM_FRAM)

/////////////////////////
// 交调存储功能相关
#define	SD_JD_SEC_BGN					g_u32JDInfo_Addr_Begin			
#define	SD_JD_SEC_END				    g_u32JDInfo_Addr_End
// 索引内容地址
#define	SD_CTX_JD_INFNUM				4							// 4-7
#define	SD_CTX_JD_SEC_BGN				8							// 8-11
#define	SD_CTX_JD_SEC_END				12							// 12-15
#define	SD_CTX_JD_NEXTINF   			16							// 16-19
#define	SD_CTX_JD_CURINF				20							//20-23




/* 以下需要设置全局变量 */
//#define		ZTYAFENG_LANE1_2	50					// 压缝宽度，车道1、2
//#define		ZTYAFENG_LANE2_3	40					// 压缝宽度，车道2、3
//#define		ZTYAFENG			(ZTYAFENG_LANE1_2+ZTYAFENG_LANE2_3)/2					// 压缝平均宽度
//#define		ZTYAFENGAFFECT_LANE1_2		(ZTYAFENG_LANE1_2+80*2)							// 压缝1影响区域
//#define		ZTYAFENGAFFECT_LANE2_3		(ZTYAFENG_LANE2_3+80*2)							// 压缝2影响区域
//#define		ZTYAFENGAFFECT		(ZTYAFENGAFFECT_LANE1_2+ZTYAFENGAFFECT_LANE2_3)/2		// 压缝平均影响区域
////#define		NEWYAFENGAFFECT		((ZTYAFENG_LANE1_2+ZTYAFENG_LANE1_2)/2+50)				// 压缝新逻辑平均影响区域
//#define		NEWYAFENGAFFECT		100




/* 以下需要设置兼容性参数 */
//#define		MODELSYSTEST			0		// 模拟测试环境测试，主要兼容目前测试环境串口5收数据，added wujinlong 20130731 

//#define		CHANNELYaFengWIDE	175					// 压缝窄条宽度，即窄条长度 175



	







 








	   












 





		


















/////////////////////////////////////////////////////////////
// added by wujinlong 20130805



/***********
    常量
***********/
//#if MODELSYSTEST > 0
//#define		CHANGENUM_MODEL			0x44		// 表征车道  	单向4车道  0x40   双向4车道  0x44
//												//				单向3车道  0x30   双向3车道  0x33	
//												//			 	单向2车道  0x20   双向2车道  0x22
//#else
//#define		CHANGENUM_MODEL			0x30		// 小乌龟布局
//#endif
//
#define		NET_PORT_NUM			5			// 端口数量
#define		SOCKET_WT				0			// 端口5000	  重量帧0x81，与PC交互，包括激光参数设置等交互
#define		SOCKET_WAVE				1			// 端口5001	  波形0xAA
#define		SOCKET_RES				2			// 端口5002	  日志
#define		SOCKET_UDP				3			// 端口5003	  保留
#define		SOCKET_SVI				4			// 端口5004	  simple Veh Info 简易车辆信息

#define		RECONNECTCNT			3			// 重连次数



//
#define	IO_ANTISHAKE_WAIT_CNT		4		// IO抓拍防抖计数




// 日志
#define	LOG_UPLOAD_PERRIOD_CNT		10		// 日志上传周期时间计数

// 一定时间内没有车辆通过，则相关内容清空，主要是逻辑中，g_u8VehPostCnt
#define	NOVEH_CLR_CNT				2*10			// 10s

//
#define	NET_HEART_FAILNUM			3				// 心跳失败次数，网络重连标准
#define	NET_HEART_SENDINTERVAL		200				// 心跳发送时间间隔10s，10*20*50ms，根据通信任务轮训时间确定
#define	NET_HEART_RECVINTERVAL		100				// 接收回复时间计时最大值，取前者一半的时间






















	
#endif	// end of #ifndef	__DEFCONFIG_H
