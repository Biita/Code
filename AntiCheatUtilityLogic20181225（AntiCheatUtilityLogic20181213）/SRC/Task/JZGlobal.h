
/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JZGlobal.h
** Last modified Date:  2011511
** Last Version:		1.0
** Descriptions:		计重程序全局变量
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
*                                        算法相关的变量
*********************************************************************************************************
*/

JZG_EXT uint8 g_StartWaveFlag;
JZG_EXT uint8 startWave[12];      //原来为11个字节,网口发送字节不对齐,改成12个字节
JZG_EXT uint8 finishWave[12];     //原来为11个字节,网口发送字节不对齐,改成12个字节
JZG_EXT uint8 coilWave[19];
JZG_EXT uint8 laserWave[60];
JZG_EXT uint32 PCSendCnt;
JZG_EXT uint8		g_u8RasterZTRead;	
JZG_EXT uint8		g_u8RasterRead;
JZG_EXT uint8 ZTRead[64];


JZG_EXT	uint32			speed1[CHANNELNUM];	//窄条低于路面参数速度1
JZG_EXT	uint32			speed2[CHANNELNUM];	 //窄条低于路面参数速度2
JZG_EXT	uint32			speed3[CHANNELNUM];	 //窄条低于路面参数速度3



JZG_EXT		CoilOrLaserBuf_Struct     g_CoilOrLaserData;   /*接收全局线圈IO，激光IO，激光位置（触发开始和结束）*/
JZG_EXT	uint16  g_u16SystemUseMode;                          /*add by sxh  加入使用模式全局变量 (3.28)*/ 




/*
*********************************************************************************************************
*                                       激光
*********************************************************************************************************
*/
#define RASTERDATALEN			256
#define BUFSIZE		    		128
#define	UARTRCVBUFSIZE			2048
#define OFFSET					6							  // 车辆数位置
#define	LASER_RESERVE_BEGIN		55							  // 预留第1字节位置
 
//JZG_EXT		uint8	    		g_LasterState[LASTERIOCNT];	  //激光触发器的状态
//JZG_EXT		uint8	    		g_u8LasterStateAll;		      //激光触发器的状态，1位表示1车道			20121108

JZG_EXT		Raster_Struct		g_RasterData[RASTERDATALEN];  //激光传送来的数据位置//   20121108
JZG_EXT		uint8				g_u8RasterDataReadIndex;	  //读数据信息	用于波形发送读取数据  均未初始化
JZG_EXT		uint8				g_u8RasterDataRead;		      //读数据信息	用于逻辑读取激光数据  均未初始化
JZG_EXT		uint8				g_u8RasterDataWriteIndex;	  //写数据信息	用于串口接收写入

JZG_EXT		uint8               g_u8RasterDaFeiCnt;           //激光连续打飞超过20个点的次
JZG_EXT		uint8               g_u8RasterNoDaFeiCnt;           //激光连续打飞不超过20个点的次数

extern      uint8	            g_au8Data[BUFSIZE];	          //激光串口数据
//JZG_EXT		uint8	g_au8Data[BUFSIZE] = {0};
extern      uint8               g_u8DataLen;
//JZG_EXT		uint8   g_u8DataLen = 0;

/*
*********************************************************************************************************
*                                       键盘
*********************************************************************************************************
*/
JZG_EXT	volatile uint8	g_u8KeyValueMapped;
JZG_EXT	volatile uint32	g_u32KeyValueOri;	
JZG_EXT	volatile uint32	g_u32KeyCnt;

/*
*********************************************************************************************************
*                                        SD卡变量
*********************************************************************************************************
*/
#ifdef __STRG_SD

JZG_EXT uint8			g_u8SDInsert; 
JZG_EXT	uint8			g_u8SDEndFlag;
JZG_EXT	uint8			g_u8SDDownFlag;

#endif	

/*
*********************************************************************************************************
*                                        温度变量
*********************************************************************************************************
*/	
#define TEMPERATURELEN	16  
JZG_EXT	int32			g_n32Temperature; 
JZG_EXT	uint8			g_u8TemperatureIndex;	//index
JZG_EXT	int32			g_an32TemperatureBuf[TEMPERATURELEN];

/*
*********************************************************************************************************
*                                        参数配置变量
*********************************************************************************************************
*/

JZG_EXT	SetupParam		g_Setup;				//设置参数	
//JZG_EXT	NetInfo			g_IPSetLocal;		//网口信息
JZG_EXT	LaserSetupParam   g_LaserPara;          //激光参数
JZG_EXT CProModParam     g_cProModParam;			//wgh 20140303 模式选择参数

/*
*********************************************************************************************************
*                                        零点操作变量
*********************************************************************************************************
*/
JZG_EXT	int32			g_an32MotionZero[CHANNELNUM];	//动态零点
JZG_EXT	uint8			g_u8StaticMotionswitch;			//称重系统，动静态切换:动态 1; 静态 0 闪灯用
//JZG_EXT  int32			g_s32ADValue[CHANNELNUM]; 	//当前一秒内的均值

//#define MOTIONZERO_LEN		 8
//JZG_EXT	uint16			g_au16MotionZeroMiddleValue[CHANNELNUM][MOTIONZERO_LEN];	//动态零点中间值，用于更新比较使用
//JZG_EXT	uint8			g_au8MotionZeroMiddleValueIndex[CHANNELNUM];	//写位置
//JZG_EXT	uint8			g_au8MotionZeroMiddleValueCnt[CHANNELNUM];	//动态零点中间值，满足要求计数
//JZG_EXT	uint8			g_u8MotionZeroZLThresh[CHANNELNUM];	 // 增量阈值	 
//#define MOTIONZERONUM_LEN		 3			// 连续3次 在阈值内就更新零点
//#define MOTIONZEROZLThresh		 50			// 连续3次 在阈值内就更新零点
/*
*********************************************************************************************************
*                                        时钟变量
*********************************************************************************************************
*/ 
//JZG_EXT	uint32			g_u32SN;				//SN码
JZG_EXT	SystemTime		g_DevCurTime;			//当前时间
//JZG_EXT	SystemTime		g_DevStartTime;			//启动时间，用于记录系统重启时间

/*
*********************************************************************************************************
*                                        信号量
*********************************************************************************************************
*/
#define SENDLISTNUM	128

//JZG_EXT	OS_EVENT *		g_pQVehicle;		//完整过车后显示过车信息信号量
//JZG_EXT void	 *		g_apvVehShow[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_psemScreenRefresh;	//屏幕刷新信号量

JZG_EXT	OS_EVENT *		g_psemKey;				//键盘信号量

JZG_EXT	OS_EVENT *		Sem_KeyIn;

//JZG_EXT	OS_EVENT *		g_pSemTime;		//系统时间信号量

//JZG_EXT	OS_EVENT *		g_pQAxleShow;			//显示轴信息消息队列
//JZG_EXT void	 *		g_apvAxleShow[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pQAxleCalc;			//轴计算数据消息队列 
JZG_EXT void	 *		g_apvAxleInfo[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pQShouWei;			//车辆收尾信号量
JZG_EXT void	 *		g_pQShouWeiList[SENDLISTNUM];	 


JZG_EXT	OS_EVENT *		g_pQSendPC;		//发送数据队列
JZG_EXT void	 *		g_pSendList[SENDLISTNUM];



//JZG_EXT	OS_EVENT *		g_pQErrMsg;		//任务间传送消息失败
//JZG_EXT void	 *		g_pQErrList[SENDLISTNUM];

//JZG_EXT	OS_EVENT *		g_pQUpMsg;		//上秤消息通知
//JZG_EXT void	 *		g_pQUpList[4];

#define DNLISTNUM	(CHANNELNUM*6)
JZG_EXT	OS_EVENT *		g_pQDnMsg;		//下秤消息通知
JZG_EXT void	 *		g_pQDnList[DNLISTNUM];	 // 改成4->16，wujinlong 20121126
JZG_EXT OS_EVENT *      g_pQSendPCDebug;
JZG_EXT void     *      g_pSendListDebug[DNLISTNUM];

JZG_EXT	OS_EVENT *		g_pSemTask6Param;		//任务6需要的参数修改通知
//
JZG_EXT	OS_EVENT *		g_pQWaveMsg;//发送波形信号	wzf
JZG_EXT void	 *		g_pQWaveList[DNLISTNUM];

JZG_EXT	OS_EVENT *		g_pQNetWavePC;		//发送波形数据队列 wzf
JZG_EXT void	 *		g_pNetWaveList[DNLISTNUM];


// test SP
//JZG_EXT	OS_EVENT *		g_pQSendThrouSP;		//发送数据队列
//JZG_EXT void	 *		g_pSendThrouSPList[SENDLISTNUM];

JZG_EXT	OS_EVENT *		g_pSemSendThtouSP;		    // 启用

//
//JZG_EXT	OS_EVENT *		g_psemLaserInteract;		//激光交互信息信号量
JZG_EXT	OS_EVENT *		g_psQLaserInteract;		//激光交互信息信号量
#define		RECVLASERDATA_NUM	16
JZG_EXT void	 *		g_pLaserInteractList[RECVLASERDATA_NUM];

#if DEBUGINFO_TASK_SP_EN > 0
#define		COM_NUM		4000
//JZG_EXT OS_EVENT *		g_pEventMutexCOM1;
JZG_EXT	OS_EVENT *		g_pQSendPCCOM1;		//发送数据队列
JZG_EXT void	 *		g_pSendPCCOM1List[COM_NUM];
#endif

// SD
#define		SD_OFFNET_WRITE_LEN		(16*2)			// 车数*2（预留）
JZG_EXT	OS_EVENT *		g_pQSDOffNetWrite;			//SD卡写数据队列
JZG_EXT void	 *		g_paSDOffNetWriteList[SD_OFFNET_WRITE_LEN];
//
#define		SD_VEH_WRITE_LEN		(16*2)			// 车数*2（预留）
JZG_EXT	OS_EVENT *		g_pQSDVehWrite;			//SD卡写数据队列
JZG_EXT void	 *		g_pQSDVehWriteList[SD_VEH_WRITE_LEN];
//
#define		SD_DBGINFO_WRITE_LEN		COM_NUM		
JZG_EXT	OS_EVENT *		g_pQSDDebugInfoWrite;			//SD卡写数据队列
JZG_EXT void	 *		g_paSDDebugInfoWriteList[SD_DBGINFO_WRITE_LEN];
//
#define		SD_WAVE_WRITE_LEN		DNLISTNUM		
JZG_EXT	OS_EVENT *		g_pQSDWaveWrite;			//SD卡写数据队列
JZG_EXT void	 *		g_paSDWaveWriteList[SD_WAVE_WRITE_LEN];

JZG_EXT OS_EVENT *      g_pSDSaveForJD;

#define     JD_SEND_LEN    6
JZG_EXT	OS_EVENT *		g_pQJDSend;			//SD卡写数据队列
JZG_EXT void	 *		g_paJDSendList[JD_SEND_LEN];
JZG_EXT	OS_EVENT *      g_pSemDisplayCheliangInfo;  //液晶显示，车辆信息  rp  20151009
JZG_EXT void	 *		g_paDisplayInfo[15];

/*
*********************************************************************************************************
*                                        任务资源统计变量
*********************************************************************************************************
*/
#if	OS_TASK_CREATE_EXT_EN > 0
#define TASK_SUM	10
JZG_EXT	  TaskStatics 	 g_Taskstatics[TASK_SUM];	 
#endif
/*
*********************************************************************************************************
*                                        设备状态码
*********************************************************************************************************
*/
#define ERR_LEN		 9
JZG_EXT	uint8			g_u8DeviceERR[ERR_LEN];	//设备故障,第一字节低4位表示激光，第一字节高4位表示通信.后8字节表示窄条状态,
JZG_EXT uint8           g_u8IOERR[2];//每个IO故障状态，每位一个， 0正常 1故障
JZG_EXT uint8           g_u8IOStateRe[16];//记录一个IO保持一个状态下通过的窄条数
JZG_EXT uint8           g_u8ZTStateRe[64];//记录每个窄条触发次数
//JZG_EXT	uint8			g_u8ERR; 	//显示用设备故障,兼容以前的错误码
JZG_EXT	uint32			g_u32StartupTime;		//启动次数

/*
*********************************************************************************************************
*                                        抓拍相关变量
*********************************************************************************************************
*/
#define CapVehCache    4    //2次抓拍间缓存的车辆数
JZG_EXT uint16 g_u16CapDelayTime;                     //两次抓拍的间隔时间,单位ms,(默认:80ms) 
JZG_EXT uint8  g_u8CapFlag[CapVehCache];              //0:当前无车辆进行了抓拍,1:车辆完成第一次抓拍
JZG_EXT uint32 g_u32FirstCapTime[CapVehCache];        //记录第一次抓拍时刻
JZG_EXT uint32 g_u32CapInfo[CapVehCache][4];          //记录抓拍信息,用于进行第二次抓拍

/*
*********************************************************************************************************
*                                        车辆数据变量
*********************************************************************************************************
*/
#define CHL_BUFLEN				512
#define CHL_BUFMASK				0x1FF
#define CHL_MAXNUM				10	   //wgh 20140305 最多支持的车道
#define YAFENGAREASIZE  		40
#define CHL_SAVEVEH     		1      //每个车道最多可缓存车数
#define COLUM_SAVEAXlE			4			//每列最多可缓存的轴数
#define AXlE_MAXCOLUM			3    //每根轴最多压三列窄条
#define MAXFINDCOLUM			4   //最多查找4列，左右相邻相隔
#define MAXDIFFTICK				2			//轮胎点数最大不能大于2个点
#define HideZTNUM   			24   //隐藏窄条起始编号
//zhangtonghan

#define XIEZOUSPDRATIOMAX    	150    //斜行相邻区间速度段比值上限
#define XIEZOUSPDRATIOMIN    	60     //斜行相邻区间速度段比值下限

JZG_EXT		Channal_Struct		g_ChlData[CHL_BUFLEN];		//窄条数据信息		 wujinlong 以下
JZG_EXT		uint32				g_ZTTime[64];    //存储窄条时间,用于删除同一根窄条连续触发或触发间隔很短
// 增加激光触发模拟窄条信息，发波形使用，修改 20131202 
#define		LASERINFO_LEN		64
JZG_EXT		Channal_Struct	 	g_cLaserInfo[LASERINFO_LEN];
JZG_EXT		uint8				g_u8LaserInfoIx;

JZG_EXT		Channal_Struct	 	g_cIOChange[LASERINFO_LEN];
JZG_EXT		uint8				g_u8IOChangeIx;


#define EMPTY_ITEMZERO 			0xFFFF
//JZG_EXT 	uint8 				g_FPGAChlIndex;
JZG_EXT	    uint16				g_ChlID2TriggerID[CHANNELNUM];		//触发ID与窄条ID对应map表
		
//JZG_EXT		uint16				g_u16ChlRindex;		//窄条读数据信息
JZG_EXT		uint16				g_u16ChlWindex;		//窄条下次写数据信息
//JZG_EXT		uint16				g_u16ChlRecvCnt;	//窄条接收数据个数


#define AXLE_BUFLEN				128 
#define AXLE_BUFMASK			0x7F
#define WHEELTRACEZTNUM         36     //建立轮轨迹的窄条数
#define WHEELCACHE              4      //建立轨迹时缓存轮的个数
JZG_EXT		Axle_Struct				g_AxleInfo[AXLE_BUFLEN];	//轴数据信息					 20121107
JZG_EXT		uint16					g_u16AxleInfoWriteIndex;		//轴数据信息下次写数据信息
JZG_EXT		Channal_Struct			g_ChlData[CHL_BUFLEN];		//窄条数据信息		 
// JZG_EXT		Wheel_Struct			g_WheelData[CHL_MAXNUM*16][1];	//车辆轮的数据信息
JZG_EXT		Whel_Struct             g_WhelData[CHL_MAXNUM*4];	    //车辆轮的数据信息(按照轨迹组轮的结构体)
JZG_EXT		TraceWhel_Struct		g_WheelTrace[WHEELTRACEZTNUM][WHEELCACHE];
JZG_EXT		TraceWhel_Struct		g_ReWheelTrace[WHEELTRACEZTNUM][WHEELCACHE];
//JZG_EXT
JZG_EXT		uint16						g_u16WheelInfoWriteIndex; //车辆轮信息下次写数据的位置

JZG_EXT   uint8         g_u8FirstAxleFlag;        //首轴收尾标志
JZG_EXT   uint8         g_u8FirstAxleColum;
JZG_EXT   uint8         g_u8CrossingFlag;         //跨道收尾标志
JZG_EXT   uint8         g_u8CrossingColum;


#define  VEHICLE_BUFLEN	 		32
#define  VEHICLE_BUFMASK		0x0F
//zhangtonghan 20170531
#define	MtchZT4Speed			10   //匹配布局窄条，轴速度差小于10Km/h
#define Mtch4SpdThr				20  //匹配布局速度阈值为20Km/h    
#define SpdDiffThr				10  //速度差值的阈值
#define Mtch4LthThr				200  //匹配接地长度阈值为200mm	
#define LthDiffThr1				95		//接地长度差值95mm
#define LthDiffThr2				70		//接地长度差值70mm
#define MinLth					25   //最小接地长度为25mm
#define MaxSpeed				120  //最大的速度为120Km/h
#define	HideMxSpeef				1200 //隐藏窄条最大速度
#define	HideZTSpdDiff			100		//速度差
#define ThrMaxSpdDiff   		5   //压三边速度差最小差
#define MaxDiffLth      		4   //左右列最大接地长度差
#define RunWalkMaxTick  		1950//压窄条最大点数，超过即为走走停停
#define MinTick					100  //最小的差值点数，用于窄条匹配
#define RunWalkDiffTick 		15000 //走走停停窄条下称点数差
#define SecSubFriTick			160  //第二排的上秤必须大于第一排下秤的点数
#define MinWheelDiff			4    //同跟轴最小轴距点数
#define MaxWheelDiff    		12    //同跟轴最大轴距点数
#define MaxSameWheelDiff 		4    //最大同轮点数差
#define	SameWheelXieCnt			3
#define MaxSameWheelXieXingDiff	4 //斜行最大同轮点数差
#define DifAxleWheel			4     //同一辆车不同轴最大差4个点
#define MinTwoRowTimeDiff 		140  //两排两个轮点数差
#define NotFindWheelDot   		5  //不能往左侧遍历最大的位置点数
#define FriRow2DecRowGap		800  //一字布局首排到第二排的距离
#define HideZTRow				2    //隐藏窄条的排数
#define AxleAccRange			10  //加速度上限阈值
#define HideZTOddDiff			120  
#define DelyTimeSecd     		10
#define RunAndWalkMinSpeed      300    //走走停停判断最小速度 
#define CoilDelayDistance 		2  //车过称重区域线圈闪烁延时抛车距离2m
#define HIDECOLUM_SAVEAXlE		2			//隐藏每列最多可缓存的轴数
#define HideMinTickDif			160  //隐藏窄条两排最小下秤点数差
#define	DigtalZTDotCnt			4 //数字窄条上的点数
#define LogicDlyTime			240
#define	LowSpdDlyTime 			1200
#define SUMAVERLITVEH           10000  //小车积分和均值的最大阈值
#define MAXVALUELITVEH          500    //小车最大值均值的最大阈值
#define MaxWheelGaugeLitVeh     270    //小车最大间距,单位cm
#define MaxWheelGauge           320    //最大轮间距,单位cm
#define MinWheelGauge           70     //最小轮间距,单位cm
#define XieZouDistance          75     //斜行距离阈值,单位cm,轮轨的第一根窄条和最后一根窄条之间的距离来判断斜行
#define AxleWgtMinError         100       //最小轴重异常阈值,单位kg
#define AxleWgtMaxError         20000     //最大轴重异常阈值,单位kg
#define AXLEMIDDISTANCE         100       //轴匹配时进行轴轨迹判断时最小间距,单位cm
#define MAXVALUETHRED           300       //区分轻重车的窄条最大值阈值
#define POSTVEHDELAY            200       //抛车防抖延时时间,200个点

//布局宏定义
#define JCLAYOUT                0       //交错布局
#define HIDELAYOUT              1       //隐藏布局
//end

//轨迹和轮的行驶状态宏定义
#define NORMAL               1    //正常行驶
#define YAFENG               2    //压缝
#define XIEZOULEFT           3    //左斜
#define XIEZOURIGHT          4    //右斜
#define MOVEROUND            5    //绕S
//end

#define NORMALLAYOUT        1     //0:使用合川布局的数字部分进行测试,1:现场实际布局   正常应设置为1

// JZG_EXT     uint32 EvenColumDelyLth	;   //偶数列延时15cm匹配
// JZG_EXT     uint32 OddColumDelyLth	;  //奇数列需要在偶数列加55cm
JZG_EXT		Vehicle_Struct	    g_vehicleInfo[VEHICLE_BUFLEN];	//车辆数据信息
JZG_EXT		uint8				g_u8vehicleID;				//维护车辆编号，0-256

JZG_EXT     VehicleInfForLoop_Struct 		g_asVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//车通过过程中存储车辆信息结构体
JZG_EXT     VehicleInfForLoop_Struct 		g_asReVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//车通过过程中存储车辆信息结构体
JZG_EXT     VehicleInfForLoop_Struct 		g_HideZTVehicleStruct[2*CHL_MAXNUM][CHL_SAVEVEH]; //隐藏窄条车辆数据结构体
JZG_EXT     VehicleInfForLoop_Struct 		g_HideZTReVehicleStruct[2*CHL_MAXNUM][CHL_SAVEVEH];
JZG_EXT     uint32							g_au32HideDeltZTTime[CHL_MAXNUM];//删除隐藏窄条
JZG_EXT     uint8							g_au8HideDeltZTEnable[CHL_MAXNUM];
JZG_EXT     uint32							g_u32PreLoopTime[CHL_MAXNUM];      //记录每个车道前置线圈收尾时刻(用于线圈收尾防抖处理)
JZG_EXT     uint32							g_u32PostLoopTime[CHL_MAXNUM];     //记录每个车道后置线圈收尾时刻(用于线圈收尾防抖处理)
JZG_EXT     uint8							g_u8PostVehFlag[CHL_MAXNUM];          //每个车道正向抛车标志,0:不抛车,1:正向抛车,default:0
JZG_EXT     uint8							g_u8RePostVehFlag[CHL_MAXNUM];        //每个车道逆向抛车标志,0:不抛车,1:正向抛车,default:0
// JZG_EXT		Colum_Struct					Forward_ZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//正向列缓存
// JZG_EXT		Colum_Struct					Reverse_ZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//逆向列缓存
// JZG_EXT		int16 							g_WonderZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //质疑区的窄条
JZG_EXT		BackVeh_Struct					g_BackVeh[CHL_MAXNUM*2];//倒车标识
JZG_EXT		DeleVeh_Str         			g_DeletVeh[CHL_MAXNUM*2];
JZG_EXT     uint8							g_u8XieLaneFlag[10];//用于车道斜行标识
JZG_EXT		HideColum_Struct				Forward_HideZTColum[CHL_MAXNUM*2][HIDECOLUM_SAVEAXlE];//隐藏正向列缓存
JZG_EXT		HideColum_Struct				Reverse_HideZTColum[CHL_MAXNUM*2][HIDECOLUM_SAVEAXlE];//隐藏逆向列缓存
// JZG_EXT     VehicleInfForLoop_Struct 	Hideg_asVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//隐藏车通过过程中存储车辆信息结构体
// JZG_EXT     VehicleInfForLoop_Struct 	Hideg_asReVehiclePerChannelForLoop[2*CHL_MAXNUM][CHL_SAVEVEH];//隐藏车通过过程中存储车辆信息结构体
// JZG_EXT			Colum_Struct				Forward_HideZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//正向列缓存
// JZG_EXT			Colum_Struct				Reverse_HideZTColum[CHL_MAXNUM*2][COLUM_SAVEAXlE];//逆向列缓存
// JZG_EXT			int16 							g_HideWonderZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //质疑区的窄条
// JZG_EXT			int16 							g_HideWonder2ZT[CHL_MAXNUM*2][COLUM_SAVEAXlE];  //质疑区2的窄条
JZG_EXT     uint8								g_au8WaitingPostVehFlg[10];
JZG_EXT     uint32              g_au32LoopCaTime[10];//后置
JZG_EXT     uint32              g_au32LoopCaTimeUp[10];//上后置
JZG_EXT     uint32              g_au32LoopCaTimeBack[10];//下前置
JZG_EXT     uint32              g_au32LoopCaTimeUpBack[10];//shang前置
JZG_EXT		uint32              g_au32FilterTime[2*CHL_MAXNUM][4];//窄条触发时间记录
JZG_EXT     uint32              g_au32HideLineLastTime[10];
JZG_EXT     uint32              g_au32ZHLineLastTime[10];
JZG_EXT     uint8               g_au8LoopStatePre[16];	//前时刻线圈状态 0:未触发 1:触发
JZG_EXT     uint8               g_au8LoopStatePreOnlyCheck[16];//线圈状态检测记录前时刻状态
JZG_EXT     int16               g_an16YaFengArea[YAFENGAREASIZE];//压缝窄条存储
JZG_EXT     uint8               g_u8YaFengAreaIndex;//下一个压缝窄条存储位置
JZG_EXT     uint8               g_au8ZTNumRecord[2*CHL_MAXNUM][4]; //一排模式单车单条处理记触发次数
//JZG_EXT     uint32              g_u32AdjoiningZTTimeThr;//匹配窄条用阈值
JZG_EXT     uint32              g_u32WheelWghtCoin;//轮重重和度
JZG_EXT     uint32				g_OddColumDelyLth;//奇偶列距离差
JZG_EXT     uint32				g_SymmetricalDiffTick;//波形对称点数差
JZG_EXT     uint32				g_ThrAlgCompenEnble;//压三边算法补偿使能
JZG_EXT     uint32				g_ZTWaveOverLap;  //波形重合度
JZG_EXT     uint32				g_ZTSecondWaveOverLap;  //波形二级重合度
//JZG_EXT     uint32              g_u32ForcePostAxleThr;//抛车处理中的强制抛轴阈值
JZG_EXT     uint32              g_u32MakeAxleForbeforeThr;//同列窄条阈值
JZG_EXT     uint32              g_u32YFBelongThr; //压缝窄条归属判断
// JZG_EXT     uint32              g_u32Sub1UnUThr; //异常1处理中只有1根末排窄条
// JZG_EXT     uint32              g_u32Sub2UnUThr; //异常2处理中没有末排窄条
JZG_EXT     uint32              g_u32LaserIOUseFlag;// 使用激光模拟IO标志, 0:不使用, 1:使用, 100:使用转发系统(无需判断激光是否故障)(liumingxin)(20170705)
//JZG_EXT     uint32              g_u32OneRowModThr;//一排窄条模式匹配阈值
JZG_EXT     uint32              g_u32OneRowFindMatchThr;//一排窄条模式两根选一根阈值
JZG_EXT     uint32              g_u32XieZouEnable;//斜走判断使能
JZG_EXT     uint32              g_u32U3FCapEnable;//U3口抓拍使能, 0:U2, 1:U3
JZG_EXT     uint32              g_FristAxleDnRange;//出车首轴下限阈值
JZG_EXT     uint32              g_u32PostVehNoLoop;//无线圈时两轴以上车抛轴延时
//JZG_EXT     uint32              g_u32ModChangeTime;//逻辑转换时间
JZG_EXT     uint32              g_u32AutoResetTime;//自动重启时间
JZG_EXT     uint8				g_u8RWNVFlag;//读写铁电的标识
JZG_EXT     uint32				g_YiZiLayouTime;//一字布局延时时间
JZG_EXT     uint32				g_JointWghtDelyTime; //联合共称延时时间
JZG_EXT     uint32				g_auxiliaryK;  //轴修正系数


/**** 轮轨迹相关可配置参数(复用上位机阈值参数) ****/
JZG_EXT     uint8              g_u8PosScoreC;                //位置打分系数,取值:0-255		    					[0]  default:30
JZG_EXT     uint8              g_u8SpdScoreC;                //速度打分系数,取值:0-255		    					[1]  default:10
JZG_EXT     uint8              g_u8RadScoreC;                //弧度打分系数,取值:0-255		  	  					[2]  default:10
JZG_EXT     uint8              g_u8PosScoreWeight;           //位置打分权值,取值:0-10(取整数) 			  			[3]  default:4
JZG_EXT     uint8              g_u8SpdScoreWeight;           //速度打分权值,取值:0-10(取整数) 						[4]  default:3
JZG_EXT     uint8              g_u8RadScoreWeight;           //弧度打分权值,取值:0-10(取整数) 						[5]  default:3
JZG_EXT     uint8              g_u8YaFengDiffXPosMax;        //压缝窄条最大位置变化,单位cm,取值:0-80  				[6]  default:60
JZG_EXT     uint8              g_u8XieXingDiffXPosMin;       //斜行最小位置变化,单位cm,取值:60-255      			[7]  default:60
JZG_EXT     uint16             g_u16TraceSpdChangeMax;       //轨迹速度变化最大值,单位cm/s,取值:0-555(0-20km/h)     [8]  default:416(15km/h)
JZG_EXT     uint8              g_u8SgOrDbleWhelPoint;        //单双胎点数阈值,取值:0-8                     			[9]  default:2(<=2为单胎)
JZG_EXT     uint8              g_u8BuildTraceScoreMin;       //最小建立轨迹分数,取值：0-50     			       		[10] default:20
JZG_EXT     uint16			   g_u16TraceDelTime;            //轨迹超时删除时间,单位s                               [11] default:60
JZG_EXT     uint16  		   g_u16RunAndWalkTime;          //走走停停时间,单位s                                   [12] default:60
JZG_EXT     uint8  		       g_u8RunAndWalkEnable;         //走走停停使能,0:关闭,1:打开                           [13] default:1
JZG_EXT     uint16			   g_u16EvenColumDelyLth;        //偶数列延时距离,单位cm                                [14] default:20
JZG_EXT     uint8              g_u8TwoBlankUnion;            //0:交错布局出车,1:两个板块通过评价出车                [15] default:1
JZG_EXT     uint8              g_u8MinAxleScore;             //出车时最小的轴分数,如果两个板块的同一个轴的分数都小于该分数则强制不出车,取值0-50 [16] default:20
JZG_EXT     uint8              g_u8AxleMultiScore;           //出车时计算的轴综合得分阈值,取值0-100                 [17] default:70
JZG_EXT     uint8              g_u8AxleWgtDiffRatio;         //轴重差的比值阈值,取值0-100,                          [18] default:20
/**** end ****/

/**** 抓拍相关可配置参数(复用上位机阈值参数) ****/
JZG_EXT     uint8              g_u8SecondCapFlag;            //0:车牌不进行二次抓拍, 1:车牌进行二次抓拍             [21]  default:0
/**** end ****/

/**** 布局相关可配置参数(复用上位机阈值参数) ****/
JZG_EXT     uint8              g_u8ReverseDigitZTPos;    //0:不反向,1:反向; 反向数字窄条的位置信息(有的现场可能数字窄条接线时反的)    [22]  default:0
JZG_EXT     uint8              g_u8A2BFlag;              //0:不移动,1:移动;将数字窄条A的信息存储到B中,并清除A中的信息(针对现场布局最左侧列只有单根数字窄条的情况)    [23]  default:0
/**** end ****/




JZG_EXT     uint8               g_au8ReFrontLoopFlag[CHL_MAXNUM];	//辅助分车标记 0:无效 1:有分车



JZG_EXT     uint8               g_au8ReZTEnPerLie[2*CHL_MAXNUM]; //当前窄条逆向逻辑是否能用 0不能 1能
JZG_EXT     uint8               g_au8ZTEnPerLie[2*CHL_MAXNUM];//当前窄条正向逻辑是否能用


JZG_EXT     uint8               g_u8BiaoDEN;//标定使能
JZG_EXT     uint8               g_u8BiaoDLane;//标定车道

JZG_EXT     uint8               g_u8SDVehBuff[20][512];	 //SD卡存储
JZG_EXT     uint8               g_u8SDBuffIndex;	  //  SD卡存储指针

JZG_EXT     uint8               g_u8SendWave;
JZG_EXT     uint8               g_u8DetVeh;

JZG_EXT     Stu_loop_info       g_asLoopBoard[10];
JZG_EXT     uint8               g_u8IOStateForBoard[16];//线圈板IO记录
JZG_EXT     uint8               g_u8NumUseLoopBoard;

JZG_EXT     uint8               g_u8DevErr;//设备故障 0 无 1 线圈故障 2 激光故障 3 都故障
JZG_EXT     int8                g_n8ZTRang[20];//每列窄条归属情况 -1 无归属 其他 相同编号归属一起
JZG_EXT     uint32              g_u32JGSWTime[10];//激光收尾时时间
JZG_EXT     uint8               g_u8JGSWFlag[10];//激光收尾标记 0 该车道没收尾 1 该车道收尾
JZG_EXT     uint8               g_u8JGSWVehID[10];//激光收尾的车ID
JZG_EXT     uint8               g_u8NoMatchCnt;//不匹配计数
JZG_EXT     uint8               g_u8FirstUse;//程序第一次使用标记 1表示第一次使用 0不是
JZG_EXT     uint8               g_u8IfRunSta;//是否开始运行状态查询；
JZG_EXT     uint8               g_u8RunZTSta[64];//检查运行状态，记录窄条触发 0正常 1故障
JZG_EXT     uint8               g_u8ZTStaSendFlag;//是否窄条状态上传 0不 1是
JZG_EXT     uint8               g_u8RunLoopSta[16];//检查运行状态，记录线圈触发 0 初始 1触发 2由触发到未触发
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
JZG_EXT		fp64				jingdu;			// 经度
JZG_EXT		fp64				weidu;			// 纬度
JZG_EXT		uint8				LOOP_BD_lane;	 //车道号
JZG_EXT     uint16              g_u16SmallandMid;//小中车间距
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
JZG_EXT     uint32              g_u32JDInfo_Addr_Begin;	   //交调信息开始位置
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
*                                        F4调试代码变量
*********************************************************************************************************
*/
//#if  SHOWVEHPASSDEBUG > 0
//	#define DBG_CODELEN		 VEHICLE_BUFLEN	 //该值不可随意改动
//	JZG_EXT DbugCode_Struct		g_BugCode[DBG_CODELEN];		
//	#define	ADDF4CODE(VID,CODE)	( (g_BugCode[VID].DbugCode[g_BugCode[VID].u8NextPos++]) = ((VID)<DBG_CODELEN) ? (CODE) : '*' )
//	#define	RESETF4CODE(VID)	( memset( &g_BugCode[VID], 0, sizeof(DbugCode_Struct) ) )	
//#else
//	#define	ADDF4CODE(VID,CODE)	 
//	#define	RESETF4CODE(VID)
//#endif
/*D0位代表轴计算队列状态，
 *D1位代表车计算队列状态，
 *D2位代表发送队列状态,

**说明：	位置1表示队列有溢出，需要扩大队列空间
*/
JZG_EXT	uint16				g_ErrMsgState;	

/*
*********************************************************************************************************
*                                        通信数据变量
*********************************************************************************************************
*/
#define	U5DATALEN		32
#define U5ONEDATALEN	512
JZG_EXT	uint8	g_u8UART5RcvBuf[U5DATALEN][U5ONEDATALEN];			//串口5接收缓存
JZG_EXT	uint16	g_u16UART5SavIndex;							//串口5保存位置
JZG_EXT	uint16	g_u16UART5ReadIndex;						//串口5读取位置

#define	NETDATALEN		32
#define	NETDATA_BUFMASK	0x1F
#define NETONEDATALEN	1460		
//网口接收的AD信号
//JZG_EXT	NetFrame		g_anfSPIAD[256];		//数据帧
JZG_EXT	uint8			g_anfSPIAD[NETDATALEN][NETONEDATALEN];		//数据帧	长度最长为300，由协议确定				  20121108
JZG_EXT	uint8			g_u8NetADIn;			//网口缓存写入位置
JZG_EXT	uint8			g_u8NetADOut;			//网口缓存读出位置	
JZG_EXT	uint16			g_u16NetADcount;		//网口缓存中的个数 

//JZG_EXT	uint8			g_u8CaptureIndex;		//记录抓拍指令位置编号 

JZG_EXT	uint8			g_u8NetStatusErr;		//网络故障状态

//网口信息
JZG_EXT	NetInfo			g_IPSetLocal;

//#if	SENDWAVEBYNET > 0
//JZG_EXT	uint8			g_au8NetSendBuf[SNDFRAMESIZE];					//发送数据包长度
//JZG_EXT	uint16			g_u16NetBufIndex;						//

//#endif


//重量
//#if	SENDWAVEENABLE > 0		//使能发波形
//JZG_EXT int32		g_an32WeightBuf[CHANNELNUM][POINTRATE];		//保存1s内重量值Buf
//JZG_EXT	volatile	uint16	g_u16WeightBufSavIndex;				//保存1s内重量值Index
//JZG_EXT	volatile	uint16	g_u16WeightBufReadIndex;			//保存1s内重量值Index
//#else		//未使能发送波形
//JZG_EXT int32		g_an32WeightAD[CHANNELNUM];				    //用于计算即使重量，判断是否需要清零
//#endif	

//JZG_EXT	int32  			g_an32SPIADBuf[CHANNELNUM][256];		     //从SPI中获取的AD值		delete 20121108
//JZG_EXT	uint8			g_u8ADcount;								 //SPI缓存中的个数
//JZG_EXT	uint8			g_u8ADIn;									 //SPI缓存写入位置
//JZG_EXT	uint8			g_u8ADOut;									 //SPI缓存读出位置

JZG_EXT	uint8			g_au8AllNetSendBuf[SENDALLSIZE];					//发送数据包长度
JZG_EXT	uint8			g_au8JDNetSendBuf[SENDALLSIZE];					//发送数据包长度
JZG_EXT	sndFrame_Struct	  	g_sndNetBuf[VEHICLE_BUFLEN];						//发送数据缓存			wujinlong
//JZG_EXT uint8			g_u8CurSendNetBufIndex;		  				//当前缓存中最新车辆序号  未初始化

#define TEMP_SIZE	32	
JZG_EXT	sndFrame_Struct	  	g_sndNetOrderBuf[TEMP_SIZE];						//发送指令缓存			20121107
JZG_EXT uint8			g_u8CurSendNetOrderIndex;		  				//当前缓存中最新指令序号  未初始化

JZG_EXT	sndFrame_Struct	  	g_cSDSendVehData;

#define NETDEBUG_SIZE  256
JZG_EXT	sndFrame_Struct   g_sndNetDebug[NETDEBUG_SIZE];
JZG_EXT uint8			  g_u8CurNetDebugIndex;

//JZG_EXT uint8			g_u8CurVehNum;		  							//当前缓存中含有车辆数
//JZG_EXT uint8			g_u8IndexLast;		  							//当前缓存中最新车辆序号
//JZG_EXT uint8			g_u8IndexFirst;		  							//当前缓存中最早车辆序号

/*
*********************************************************************************************************
*                                        逻辑任务数据变量
* wujinlong
* 20121120
*********************************************************************************************************
*/
//JZG_EXT		ROUTETABLE		a_CRouteTable1,a_CRouteTable2;
//JZG_EXT		WONDERBUFFER	a_CWonderBuffer1,a_CWonderBuffer2;
//JZG_EXT		LASERLATEBUFFER a_LaserLateBuffer1,a_LaserLateBuffer2;//浩叔修改
//
//JZG_EXT uint32			g_u32TimeDelayUpdataLate; //F_UpdataLateLaserZtToRouteTable
//JZG_EXT uint32			g_u32TimeDelayUpdataWbZt; //F_UpdataWbZtRecToRoute
//JZG_EXT uint32			g_u32TimeDelayWonderProcess; //F_WonderProcess	
//JZG_EXT uint32			g_u32TimeDelayDropVehicle; //F_DropVehicle
//JZG_EXT uint32			g_u32TimeDelayForceClear; //F_ForceClear
//
//JZG_EXT uint16			g_u16LaneWidth;		  // 车道宽度

/*
*********************************************************************************************************
*                                        测试发车数据变量
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



// 系统初始化 20130805
JZG_EXT int8 		g_u8StartOver;

// 收到激光抓拍IO等统计调试
//#if DEBUG_CAPTURE_IO_IN_STAT_EN > 0
//JZG_EXT uint32 		g_au16CaptIOInCnt[LASTERIOCNT];				// 记录每个车道IO IN的次数
//#endif

// 通知相机抓拍IO等统计调试
//#if	DEBUG_CAPTURE_IO_OUT_STAT_EN > 0
//JZG_EXT uint32 		g_au16CaptIOOutCnt[LASTERIOCNT];				// 记录每个车道IO OUT的次数
//#endif

#if DEBUGINFO_TASK_SP_EN > 0
JZG_EXT	CComBuf		g_cDebugInfo[COM_NUM];
JZG_EXT uint16		g_u16CurDebugInd;		  				// 当前缓存中最新指令序号  
#endif


JZG_EXT	CComBuf		g_cRecvLaserData[RECVLASERDATA_NUM];
JZG_EXT uint16		g_u16RecvLaserInd;					  	// 接收激光数据索引



//#if NOVEH_CLR_EN > 0
//JZG_EXT uint16		g_u16NoVehClrCnt;		  				// 没有车清空计数
//#endif


#if    TASKTIMEENABLE
#define			RUNTIMECOUNT(LASTTIME)	((UL)T1TC-LASTTIME)
JZG_EXT TASKRUNTIME TaskRunTimeStatistics;	
#endif

JZG_EXT	uint8		g_au8ReCon[NET_PORT_NUM];		// 重连机制


JZG_EXT	uint16		g_au16HeartTimeCnt[NET_PORT_NUM];		// 发送心跳时间计数
JZG_EXT	uint8		g_au8HeartRecvReplyFlag[NET_PORT_NUM];	// 收到心跳回复标志
JZG_EXT	uint8		g_au8HeartRecvBeginCntFlag[NET_PORT_NUM];	// 计算接收回复开始标志
JZG_EXT	uint16		g_au16HeartRecvTimeCnt[NET_PORT_NUM];	// 计算接收失败时间计数
JZG_EXT	uint8		g_au8HeartRecvFailCnt[NET_PORT_NUM];	// 心跳接收失败计数
JZG_EXT	CHeartBuf  	g_cHeartBuf;							// 心跳数据


//#if TEST_CAPTURETIME_EN > 0
//JZG_EXT	uint16		g_u16TestCaptime;						// 测试抓拍时间
//JZG_EXT	uint8		g_u8TestSPFlag;							// 测试抓拍串口标志，收到串口首帧置1，否则为0
//JZG_EXT	uint8		g_u8TestIOFlag;							// 测试抓拍IO标志，收到IO置1，否则为0
//JZG_EXT	uint32		g_u32T1TCBeginTime;						// 开始时间
//#endif


//#define		EXTDATELEN		40
//JZG_EXT	uint8		g_au8ExtDataBuf[EXTDATELEN];			// 扩展指令buf

//#if TEST_WAVE_EN > 0
//#define		LASERENDNUM		4								// 激光接收延时个数，0.5s一个
//JZG_EXT	uint8		g_u8LaserEndFlag;				   		// 激光结束标志
//JZG_EXT	uint8		g_u8LaserEndCnt;						// 激光延时计数
//
//#endif


//JZG_EXT	int	g_n32ZTRightPos[QUATTITY_WbZt_COLOUM];


//#if LOGIC_YAFENG_PROCESS_EN > 0
//#define	YAFENGSTACKLEN	(6*6*2)								// 6根*6轴*2预留，如果考虑到两个方向，则还需要*2
//JZG_EXT	CYaFengStack	g_acYaFengZTStack[YAFENGSTACKLEN];
//#endif	

//JZG_EXT	uint8	g_u8IsLaserorCoilFlag;						// 使用激光还是线圈，1：激光，0：线圈，初始化为1												



// SD
JZG_EXT	uint8	g_u8IsConnectFlag;							// 网络连接正常与否，1连接正常，0连接不正常，默认是1
JZG_EXT	uint32	g_u32NetConCnt;								// 网络连接时间计数	
//
#define	SD_SEND_BUF_NUM		32
JZG_EXT	sndFrame_Struct	  	g_acSDSendVehInfo[SD_SEND_BUF_NUM];	 // 读取SD卡数据，保存在此，并发送给通信任务
JZG_EXT	uint16	g_u16SDSendBufIX;								 // 索引
//
JZG_EXT uint8  		g_u8SDRdBuf[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDRdBuf2[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDWrBuf[SD_SECTOR_LEN];
JZG_EXT uint8  		g_u8SDWrBuf2[SD_SECTOR_LEN];


JZG_EXT	uint8		g_u8VehSendFlag;
JZG_EXT	uint8		g_u8DebugInfoSendFlag;
JZG_EXT uint32		g_u32BeginIx ;		   	// 开始序号
JZG_EXT uint32		g_u32EndIx;		  		// 结束序号


//#if LASER_UNNORMAL_EN > 0
//JZG_EXT uint8  		g_u8LasUnnFlag;				// 激光异常标志
//JZG_EXT uint32  	g_u32LasUnnTimeCnt;			// 激光异常时间计数,30min取消
//#define	LAS_UNN_VEH_NUM		10
//JZG_EXT CVehSimInfo	g_cVehSimInfo[LAS_UNN_VEH_NUM];		// 存储车辆信息

//#endif



// -begin--
// 倒车相关 20140819
//JZG_EXT uint16	g_au16DaoRow23IXRec[8];	// 倒车第2、3排索引记录
//JZG_EXT uint16	g_au16TimeThresh;	// 新建车和第2、3排ZT触发判断阈值


	// --end--


// test
//JZG_EXT	uint8		g_u8TestCnt;
typedef		SystemTime		RTCDateTime;

#endif	  // end #ifndef	__JZGLOBAL_H

/*
*********************************************************************************************************
*                                        屏幕显示数据变量
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


