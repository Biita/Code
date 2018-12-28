/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			JZStructure.h
** Last modified Date:  2011511
** Last Version:		1.0
** Descriptions:		计重程序结构
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
** Descriptions:		2.2和3.0通用
**---------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Tonghan
** Modified date:		20170904
** Version:				1.2
** Descriptions:		非现结构体
**
*********************************************************************************************************/
#ifndef	__JZSTRUCTURE_H
#define	__JZSTRUCTURE_H

#include "config.h"



/*
*********************************************************************************************************
*                                         算法结构体
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
*                                         时间结构体
*********************************************************************************************************
*/	
typedef	struct tagSystemTime
{
	uint16	u16Year;				//年
	uint8	u8Month;				//月
	uint8	u8Day;					//日
	uint8	u8Week;					//星期几
	uint8	u8Hour;					//时
	uint8	u8Minute;				//分
	uint8	u8Second;				//秒
} SystemTime;

/*
*********************************************************************************************************
*                                         网络结构体
*********************************************************************************************************
*/	
typedef	struct tagNetInfo
{
	uint8	au8SubMask[4];					//子网掩码
	uint8	au8IPAddr[4];					//IP
	uint8	au8GatewayIP[4];				//默认网关
	uint8	au8ServerIP[4];					//服务器IP地址 
	uint32	u32LocalPortNO;					//端口号
	uint32	u32ServerPortNO;				//服务器端口号
	uint8	au8MACAddr[6];					//MAC
	//
	uint8	m_u8ProgVersion[4];				// 程序版本标示
	//
	uint8	au8CRC[2];						//CRC  	
} NetInfo;

typedef	struct tagsndFrame				  // wujinlong
{
	uint8	u8Cnt; 	//发送的包数
	uint32	au32len[50]; //数据帧长度
	uint8   au8data[50][SNDFRAMESIZE];	 //发送的数据内容
} sndFrame_Struct;

typedef struct tagTargetIPInfo
{
	uint8  u8State; //端口的状态
	uint8  u8CnntIP[4]; //对端IP地址
	uint8  u8CnntPort[2]; //对端的端口号
}TargetIPInfo_Struct;

//typedef struct
//{
//	uint16 len;
//	uint8 data[MAXBUFPOINT];
//}WaveData_Struct; 

/*
*********************************************************************************************************
*                                         F4代码结构体
*********************************************************************************************************
*/	
typedef	struct tagDbugCode
{
	uint8	u8NextPos; //调试代码的长度
	uint8   DbugCode[256];	 //调试代码的内容
} DbugCode_Struct;


// 线圈IO结构体	  add by  sxh 2014.3.27
/**********************add by sxh  2014.3.28 ***********************************/
#define COILGPIODATALEN         128  
/*车辆位置结构体*/
typedef struct vehPosition
{
	uint8  m_u8type;
	uint16 m_u16Ltpos;	 
	uint16 m_u16Rtpos;
}VehPosition_Struct;
/*线圈/IO状态结构体*/
typedef struct CoilOrLaser   
{
	uint8 m_u8Flag;                        // 激光/线圈     0x02;激光位置数据； 0x03：激光IO数据； 0x04：线圈IO数据
	VehPosition_Struct vehPositionInfo[LASTERCOUNT<<2];	  //车辆位置信息(个激光器数量有关系)
	uint16 m_u16GpioState;                // 第一个字节中的8位由低到高依次用来表示前编号为1-8的线圈状态；第二个字节中的最低位用来表示编号为9的线
	uint32 m_u32Tm;	                      //触发时间

}CoilOrLaser_Struct;

 typedef struct CoilOrLaserBuf   //接收线圈IO/激光IO/激光位置结构体缓存
 {
 	uint8 m_u8WriteIn;
	uint8 m_u8ReadOut;
	CoilOrLaser_Struct CoilOrLaserData[COILGPIODATALEN];
 }CoilOrLaserBuf_Struct;

/*
*********************************************************************************************************
*                                         资源统计结构体
*********************************************************************************************************
*/
#if	OS_TASK_CREATE_EXT_EN > 0

typedef struct tagTaskStatics
{
  uint8  TaskID;	//任务优先级
  uint32	 CPUsage; 	//CPU使用率
  uint32  stkMaxUsage;  //栈使用率
  uint32  stkCurUsage; //栈当前使用率
  uint32 tmCnt;	    //任务使用耗时,单位ms/s  
}TaskStatics;

#endif
/*
*********************************************************************************************************
*                                         参数配置结构体
*********************************************************************************************************
*/
//ARM参数
typedef struct tagSetupParam
{	
	uint8	u8Version;							//参数版本 PARAM_VERSION 3.0
	uint8  	u8Year;							 	//程序修改日期--年
	uint8  	u8Month;							//程序修改日期--月
	uint8  	u8Day;								//程序修改日期--日
//4
	uint8 	u8Protocol;
	uint8 	u8StaticScale;		   				//静态分度值，没用，保留
	uint8 	u8MotionScale;						//动态分度值 
	uint8	u8DiaoDianTag;						//掉电保护
//8
	uint8 	u8DOG;			 					//看门狗设置 
	uint8 	u8SendWaveEnable;					//发送波形使能 
	uint8	u8LoopTriggerEnable; 				//线圈触发使能，没用，保留		 
	uint8 	u8ComMode;			  				//命令模式
//12
	uint8 	u8SendF4CodeEnable;					//发送F4代码使能
	uint8 	u8BaudRate;			  				//串口0波特率 	 
   	uint8 	u8VehicleBufSize;					//车辆缓存大小，没用，保留
	uint8	u8noUsed1;
//16
	uint32 	u32Full;			  				//满量程
	uint32	u32Password;						//密码	  
//24
	uint8	u8UPPoints;						    //上秤点数 wzf 2012-08-24
	uint8 	u8DownPoints;					    //下秤点数 wzf 2012-08-24
	uint8	u8noUsed2[2];
//28
	uint16	u16logicUpValue;	 				//逻辑上秤阈值	wzf 2012-08-24
	uint16 	u16logicDownValue;					//逻辑下秤秤阈值  wzf 2012-08-24
	uint16 	u16UpValue;	 						//上秤阀值
	uint16 	u16DownValue;	 					//下秤阀值	
//36
	uint16  an32ImmediateValue[ZERONUM];//瞬时值
	uint16 	an32Zero[ZERONUM];				//零点		   	  
	uint16 	an32AxGain[CHANNELNUM];	 			//增益
//292 
   	uint16  u16PlatformGain;				    //秤台增益，wzf 2012-08-13新增变量，没有，保留
	uint16  u16noUsed3;
//296	
	uint8  	u8Diszt[CHANNELNUM];				//窄条宽度
	int8	an8HeightTanxingti[CHANNELNUM];	    //弹性体高度，0.1mm，范围1-16，即0.1-1.6，默认为10
	uint8   an8ZTStatus[CHANNELNUM];	    //窄条状态:1为窄条正常，0:异常
	int8 	an8LengthPodu[CHANNELNUM];			//缓坡长度（窄条前面缓坡长度）
//552
	uint16 	u16DistBetween[CHANNALLINESUM];				//窄条间距,300,600,200mm --- 新增2012-11-15	 ，最终不用，先保留
	uint16	u16UseToWghFlag[CHANNALLINESUM];			//每一位标志每一根窄条的用途，低位到高位窄条1-64 1参与称重 0不参与称重
//568
	uint16 	au16Speedmodify[10][SPEED_CTL_SIZE];  	// 速度整体修正
//968
	uint16  au16VehTotalModify[10][10];				//车型整体修正
//1168
	uint16 	au16VehSpeedModify[10][10][SPEED_CTL_SIZE];		 	//车型速度修正
//5168	
	int8	m_an8HeartBeatEn[NET_PORT_NUM];		// 心跳使能，可以通过上位机设置，1：开启，默认1，added by wujinlong 20130905
//5172
	
	// 新增变量，注意更改预留字节数
//	uint8	m_u8LCAddr;							// 下位机地址，从机地址
	uint8	u8ProgVersion[4];					// 程序版本标示	
//5176
	uint16 	an16BDAxGain[CHANNELNUM];	 			//增益 
	uint8   u8ArrayForZT[64];					//窄条接收矩阵
	uint16 	an32StaGain[CHANNELNUM];				//窄条静态增益  rp 20150820
//5496  
	uint8 	au8ReserveByte[RESERVED];			//预留字节	
	uint16	u16CRC;								//CRC校验码
//6144	
} SetupParam;

//激光关键标定参数
typedef struct tagLaserKeyParam
{
    uint32  m_u32Height;                        //激光器高度
    uint16  m_u32Center;                        //激光器中心对称点
    uint16  m_u32ScanStart;                     //激光器扫描起始点
	uint16  m_u32ScanEnd;                       //激光器扫描终止点
	uint16  m_u32Port;                          //激光器端口号
	uint32  m_u32IP;                            //激光器IP地址
	uint32  m_u32OffSet;
	uint32  m_u32MinWide;
	uint32  m_u32EndFrame;
	uint32  m_u32LaneWide;
}LaserKeyParam;

//激光参数(注意该结构体如果需要修改，一定要保证字节对齐，因为该结构体会使用memcpy赋值)
typedef struct tagLaserSetupParam
{
	uint8   m_u8ComBandRate;				     //串口波特率1-4800；3-38400；4-57600；5-115200；其他-9600；默认值为5,即115200
	uint8   m_u8WDTEnable;						 //看门狗使能：1-使能；0-禁止
	uint16  m_u16SeverPort;						 //网口收发数时的服务器端口号
	uint32  m_u32SeverIP;						 //网口收发数时的服务器IP地址
	uint32  m_u32LaserResetCnt;                  //激光模块重启次数
	uint32  m_u32LaserIP;						 //激光IP地址
	uint16  m_u16LaserPort;						 //激光端口号
	uint8   m_a_u8LaserMAC[6];				     //激光MAC地址
	uint32  m_u32LaserMASK;						 //激光子网掩码
	uint32  m_u32LaserGateWay;					 //激光网关
	uint16	m_u16Year;				             //年
	uint8	m_u8Month;				             //月
	uint8	m_u8Day;					         //日
	uint8	m_u8Hour;					         //时
	uint8	m_u8Minute;				             //分
	uint8	m_u8Second;				             //秒
	uint8   m_u8LaserNum;						 //激光器数量
	LaserKeyParam m_stLaserKeyPara[LASTERCOUNT]; //激光器关键参数

	uint8   m_u8Rev[6];							 //保留字节
	uint16  m_CrcChk;							 //校验位

}LaserSetupParam; 

// wgh 20140303
typedef struct tagProModParam
{
	uint8	m_u8ZTRow;				// 窄条排数，默认3
	uint8	m_u8LaneNum;			// 车道数，默认3
	uint8	m_u8ZTWide;				// ZT宽度，默认175
	uint8	m_u8YaFengZTWide;		// 压缝ZT宽度，默认175
	
	uint16	m_au16ZTGap[64];
	uint16	m_au16ZTDist[10][3];		// 窄条间距，单位mm，最多4排，最多10车道，压缝窄条不需要间距，1车道内两列间距相同
	uint32	m_au32Thres[40];		// 所有可设置的阈值，换算成采样点，如1s=6000点，16位最大是65535，即10.9s，防止扩展等使用超过10s，包括预留等共计20个
//224									// 如阈值1表示同排ZT时间阈值60点等，逻辑时间阈值10个
									// ZT成对重量比对2个，需确定阈值顺序
	uint8	m_au8Enable[40];		// 参数使能数组 [0]：串口调试信息输出
									//  			[1]：
	//
		
	uint8	m_au8LaneBetwWide[20];	// 车道间宽度，0-255cm，2.55m，0：第1-2车道间宽度
//284	
	//
	uint8 	m_u8DebugBaudRate;		// 调试信息波特率，1-5:4800、9600、38400、57600、115200，默认115200
	//
	uint8	m_u8TriggerMode;		// 触发模式：0x01：激光/线圈（前置），激光作为主触发，线圈辅助触发；0x02：单线圈（后置）；0x04：双线圈；0x08：激光；默认0x04
	uint8	m_u8IsYaFeng;			// 是否有压缝窄条，1：是，0：否，默认0
	uint8	m_u8UploadDataMode;		// 数据上传方式，0x01：网口，0x02：232串口，0x03：无线传输，4: 422串口默认网口0x01
	uint8	m_u8DataWaveEn;			// 数据上传中包含波形与否，1：包含，0：不包含，默认0
	uint8	m_u8VehWaveSample;		// 车辆波形采样标识，0：不抽点，1：隔1点取1点(2点取1点)，2：隔2点取1点(3点取1点)，3：隔3点取1点(4点取1点)
	uint8	m_u8CaptureMode;		// 抓拍模式，0x01：硬抓拍，0x02：软抓拍（下位机触发抓拍），0x04：软抓拍（上位机触发抓拍），默认硬抓拍0x01
	uint8	m_u8TransmitSysEn;		// 转发系统开启与否，1：开启，0：不开启，默认0
	uint16	m_u16SDSaveEn;			// SD卡模式，0x01：SD卡存储功能使能，由第1-7位表示15种存储功能使能，如断网缓存0x02、日志调试信息0x04、车辆信息0x08，波形信息0x10等
	uint16	m_u16ExEquiType;		// 外部设备，共16种，0x01：交调激光，0x02：无线传输模块，0x04：SD卡
	uint8	m_au8AcqBoardFlag[8];	// 采集板有无标志，第0-7字节分别表示第1-8个采集板，无采集板为0x00，采集板为0x01，线圈板0x02（目前只有第7、8两路有效），默认全为1，8个采集板通道
	uint8	m_u8CaptureFlag[16][2];	// 线圈抓拍标志，只有双线圈模式有效，第1维车道，第2维第1位前置线圈，第2位后置线圈，0x00不进行抓拍；0x01抓拍；车道顺序，沿着车道行车方向自左至右依次为1-16车道，默认全0x00
	uint8	m_u8BwdDirecFlag;	// 逆向出车总标志，0x00：逆向不出车；0x01：逆向出车；默认0x00
	uint8	m_au8VehDirecFlag[19];	// 抛车方向标志，0-N表示0-N车道，0x01：正向出车；0x02：逆向出车；0x03=0x01|0x02，正向逆向均出车，车道顺序，沿着车道行车方向自左至右依次为1-N车道，默认全0x01，该值和m_u8BwdDirecFlag相关，m_u8BwdDirecFlag逆向不出车，m_au8VehDirecFlag逆向设置不起作用
//356	
	uint8	m_au8HalfLane[8];		// 半车道，存储半车道的车道数，如第1、5车道为半车道，则[0]存储1，[1]存储5
	
	// SD卡
	uint8	m_u8SDCapa;				// SD卡容量，1-1G，4-4G，8-8G，16-16G,默认8G
	uint8   m_u8JGKZ;				//激光扩展范围
	uint8	m_au8Nouse1[2];
	uint8	m_au8SDFuncSpaceper[16];// 功能空间百分比，[0]为索引区，最多15种功能，和m_u16SDSaveEn相对应，默认1,10,20,15,15,39
//384	
	// 扩展接口
	
	//
	
	uint8   m_u8StudyAlgEn[5];					  //学习使能
	uint8  	m_u8StudyLimit[5][2];				  //学习控制线
	uint8	m_u8Nouse2;
	uint16  m_u16StudyWeight[5][2];				  //学习重量
	
	//
	uint8	m_u8ProgVersion[4];					// 程序版本标示
//424											 
	uint8   m_u8EnableFlag[8];					// 使能位 可用8个使能
	uint16  m_u16LRLocation[2][20];				// 每列窄条的左右位置 [0][]左位置 [1][]右位置
	int8    m_n8LouDL[64];						// 漏电流参数
	int8    m_n8CFDian[64];						// 充放电参数
	uint8   m_au8ZTIntEn[64];					// 窄条中断上传使能
	uint8   m_au8IOUseEn[26];					//IO使用使能
	uint8   m_au8NoUse3[2];
	uint8   m_au8CHLSET[20];                    //出车时车道设置
	uint8 	m_au8ReserveByte[PROMOD_RESERVED-128];			//预留字节	
	uint16	m_u16CRC;
} CProModParam;
//end 

/*
*********************************************************************************************************
*                                         车辆数据结构体
*********************************************************************************************************
*/
# define MAXNUM_AXLE 12 //每个轴最多包含的窄条数
//#pragma	pack()

//每根窄条的数据结构
typedef struct tagChannal{
	uint16	u16SelfID;		// 记录本窄条位置，即在数组中的序号，一直不变，下窄条时提供位置信息
	uint16	u16YDMID; //默认与窄条ID相同
	uint8   u8BoardType;    // 板卡类型 1:中航电测采集板, 2:数字传感器采集板
	uint8	u8WheelCnt;  //窄条上车轮的个数
	uint8	u8ZTSeam;	//0:一个轮不压缝;1:左侧窄条压缝;2:右侧窄条压缝;3:两侧都压缝;4:两侧都不压缝;5:左压右不压;6:右压左不压
	uint8	u8ZTMatchedFlag; //窄条被匹配标识:0:没有被匹配;1:左侧被匹配;2:右侧被匹配;3:都被匹配(单轮匹配后置3)
	//
	uint32	m_ulTime;					// 上秤时间或下秤时间
	int8	m_bUpOrDn;					// 1：上秤，0 下秤
	int8	m_bDirectionFlag;
	uint8  	u8Id;		//窄条编号
	uint8  	u8IsDownFlag; // 增加下秤标志，如果下秤则置1，否则为0，writeIndex时初始化置0  20121108
			
	uint32 	u32chlSum;	//窄条积分和
	uint32 	u32uptm;		//上称时刻
	uint32 	u32dntm;		//下秤时刻
	uint32  u32Maxtm;//真实最值时刻
	uint16 	u16ptMaxValue;	//最值
	uint8	TaiType[2]; // 1:单胎;2:双胎; 如果有两个轮，第一个元素表示左侧轮，第二个表示右侧;
	uint32 	u32FirstDnTime;		//一级下称时刻 	
	uint32	u32realCnt;  //真实点数
	uint32  u32Mintm;       //一级上秤时刻(列与列匹配中使用,其他位置未赋值)

	//第一根窄条的数据
	uint32  u32FirstZTUpTmFir; //一级上秤点
	uint32  u32FirstZTUpTmSec; //二级上秤点
	uint32	u32FirstZTDnTmFir; //一级下秤点
	uint32	u32FirstZTDnTmSec; //二级下秤点
	uint16 	u16FirstZTMaxValue;	//最值
	uint8	u8FirstZTMaxPos; //第一根窄条最大值位置
	uint8	u8FirstZTUpDnPosCnt;//第一根窄条上下秤位置
	uint8   u8FirstZTMaxAndUpDnPos;//第一根窄条最大值位置和上下秤位置，分别为高四位和低四位
	uint32	u32FirstZTSum; //第一根窄条积分和
	
	//第二根窄条的数据
	uint32  u32SecondZTUpTmFir; //一级上秤点
	uint32  u32SecondZTUpTmSec; //二级上秤点
	uint32	u32SecondZTDnTmFir; //一级下秤点
	uint32	u32SecondZTDnTmSec; //二级下秤点
	uint16 	u16SecondZTMaxValue;	//最值
	uint8	u8SecondZTMaxPos; //第二根窄条最大值位置
	uint8	u8SecondZTUpDnPosCnt;//第二根窄条上下秤位置
	uint8   u8SecondZTMaxAndUpDnPos;//第二根窄条最大值位置和上下秤位置，分别为高四位和低四位
	uint32	u32SecondZTSum; //第二根窄条积分和
	
	uint32  u32MainFPGATime;      // 主控板FPGA时间,用于判断主控板FPGA和采集板FPGA时间差
	uint32  u32TimeDiffValue;     // 数字传感器的FPGA与主控板FPGA系统时间差(取绝对值)
	int32   n32DiffValue;         // 数字传感器的FPGA与主控板FPGA系统时间差(不取绝对值)
	
	
	uint16 	u16ptCnt; 		//窄条存储数据点数
	uint8 	u8packetCnt; 		//读取窄条中断次数
	uint8 	u8DataErrorFlag; 		//0表示正常，不为0表示数据包为0的次数
	uint8   u8FrameNo;      //帧编号
	uint8   u8BoardNo;      //板号
	uint8   u8ChannelNo;    //通道号
// 	uint16 	u16data[MAXBUFPOINT];	//窄条的点值序列//中航或者A窄条
// 	uint16	u16Bdata[MAXBUFPOINT];	//B窄条的点值序列
	
	uint16  u16data[2][MAXBUFPOINT];    // 窄条的点值序列, [0][]:中航或者A窄条, [1][]:B窄条的点值序列
}Channal_Struct; 

typedef struct wheel
{
	uint16 	ZTList[2];//窄条
	uint32 	ZTUpTime;
	uint32 	ZTDnTime;
	uint8	ZTakePos[2];//触发窄条的位置 1:左侧;2:右侧;3:整体
	uint8  	WheelXPos;//轮的X位置
	uint8	TrigCnt;//触发数
	uint8	SgOrDbleWhel;//单胎或者双胎 1:单胎; 2:双胎
	uint8 	TriZTCnt; //触发的窄条数
	uint16 	MaxValue;
	uint8  	WheelYesorNot;//该点有没有轮胎
}Wheel_Struct;

typedef struct whel
{
	uint16  ZTList[6];   //窄条
	uint16  WheelXPos[6];//轮的X位置    单位cm
	uint16  WheelYPos[6];//轮的Y位置    单位cm
	uint8	ZTakePos[6]; //触发窄条的位置 1:左侧;2:右侧;3:整体
	int8    Dir;         //1:正向,-1:逆向
	uint8	SgOrDbleWhel;//单胎或者双胎 1:单胎; 2:双胎
	
	uint8   Point;       //点数(用于合并轮时判断单双胎)
	uint8   TriZTCnt;    //触发的窄条数
	uint8   Status;      //行驶状态(见宏定义)  默认为0 (1:正常,2:压缝,3:左斜,4:右斜,5:绕S)
	uint8   HaveWheelFlag;         //0:无轮,1:有轮
	
	uint32  SumAver;               //积分和均值
	uint32  MaxValueAver;          //最大值均值
	
	uint32  DueTime;               //奇数列末排延时时间
	uint32  Spd;                   //轮速度,单位cm/s
	uint8   Waiting4MatchFlag;     //待匹配标识,0:不进行匹配,1:可以进行匹配(偶数列末排建轮时该标志置1,奇数列末排建轮并延时时间到后该标志置1)
	uint8   Score;                 //轮分数(为组轮的轨迹的分数)
}Whel_Struct;

//轮轨迹匹配结构体
typedef struct _tracewhel
{
	//单轮默认存储在左轮的数组中,双轮则分别存储在左轮和右轮数组中
	uint8  LeftWhlZTID[6][6];           //左轮窄条ID(轨迹x轨迹包含的窄条ID)
	uint16 LeftWhlZTPos[6][6];          //左轮窄条位置(轨迹x轨迹包含的窄条Pos)
	uint8  LeftWhlZTake[6][6];          //左轮触发窄条的位置 1:左侧;2:右侧;3:整体
	
	uint16 LeftWhlZTXPos[6][6];     	//左轮在窄条上的X位置,单位cm
	uint16 LeftWhlZTYPos[6][6];    	    //左轮在窄条上的Y位置,单位cm,取值:0~350cm(取离散值:0,70,140,210,280,350)
	uint16 LeftWhlSpd[6][6];            //左轨迹中前后窄条的区间速度,cm/s
	int16  LeftWhlDiffXPos[6][6];       //左轮每条轨迹X位置的差值
	uint8  LeftPoint[6][6];             //轮的点数(用来判断单双胎)  默认为0
	
	
	uint8  LeftWhlZTCntPerTrace[6];     //左轮每条轨迹包含的窄条数
	uint8  LeftPoScore[6];              //每条轨迹位置得分
	uint8  LeftSpdScore[6];             //每条轨迹速度得分
	uint8  LeftRadScore[6];             //每条轨迹弧度得分
	uint8  LeftMultiScore[6];           //每条轨迹综合得分
	uint8  LeftWhlStatus[6];            //行驶状态(见宏定义)  默认为0 (1:正常,2:压缝,3:左斜,4:右斜,5:绕S)
	
	uint8  RightWhlZTID[6][6];          //右轮窄条ID(轨迹x轨迹包含的窄条ID)
	uint16 RightWhlZTPos[6][6];         //右轮窄条位置(轨迹x轨迹包含的窄条Pos)
	uint8  RightWhlZTake[6][6];         //右轮触发窄条的位置 1:左侧;2:右侧;3:整体
	
	uint16 RightWhlZTXPos[6][6];        //左轮在窄条上的X位置,单位cm
	uint16 RightWhlZTYPos[6][6];        //左轮在窄条上的Y位置,单位cm,取值:0~350cm(取离散值:0,70,140,210,280,350)
	uint16 RightWhlSpd[6][6];           //右轨迹中前后窄条的区间速度
	int16  RightWhlDiffXPos[6][6];      //右轮每条轨迹X位置的差值
	uint8  RightPoint[6][6];	        //轮的点数(用来判断单双胎)  默认为0
	uint8  RightTraceCnt;               //当前窄条已经建立的轨迹个数
	uint8  LeftTraceCnt;                //当前窄条已经建立的轨迹个数
	uint8  RightWhlZTCntPerTrace[6];    //右轮每条轨迹包含的窄条数
	
	uint8  RightPoScore[6];             //每条轨迹位置得分
	uint8  RightSpdScore[6];            //每条轨迹速度得分
	uint8  RightRadScore[6];            //每条轨迹弧度得分
	uint8  RightMultiScore[6];          //每条轨迹综合得分
	uint8  RightWhlStatus[6];    	    //行驶状态(见宏定义)  默认为0 (1:正常,2:压缝,3:左斜,4:右斜,5:绕S)
	
	int8   Dir;   	             //1:正向;-1:逆向
	int8  WheelCnt;             //当前窄条上轮的个数
	
	uint16 LastXPos;             //当前窄条X的位置,单位cm
	uint16 LastYPos;             //当前窄条Y的位置,单位cm,取值:0~350cm(取离散值:0,70,140,210,280,350)
	
	
	uint32 LeftDueTime;              //奇数列末排延时时间
	uint32 RightDueTime;             //奇数列末排延时时间
	uint8  LeftFirstTracePos;        //优选轨迹,第三排后开始评选出优选列(建轮时或轮找未建轮轨迹时使用)
	uint8  RightFirstTracePos;
	uint8  LeftStatus;               //Left行驶状态(最终出轮的轨迹的行驶状态)   1:正常,2:左斜,3:右斜,4:绕S,5压缝  默认为0
	uint8  RightStatus;
	uint8  HaveTraceFlag;        //含有轨迹标识,0:没有轨迹,1:有轨迹,默认为0
}TraceWhel_Struct;		         //每根窄条建立的轮轨迹结构体
//窄条列结构
typedef struct colum
{
	int16		Colum[4]; 	//存放窄条的位置
	uint8		RunAndWalk;//走走停停标识，0为非
	int8 		Dir;      	//方向,1表示正向，-1表示逆向
	uint8 		Speed;		// 轴速度
	uint8 		ZTCnt; //该列窄条的数量,默认为0
	
	uint16 		LaserVehID;//激光给出的车辆ID
	uint16 		MaxValue; 	//最大值
	
	uint16 		Length;		//接地长度
	uint8 		ZTJoinFlag;  //窄条拼接成列的标识，1:满列；2:无第二排，用13排出列; 3:无第一排，用23排出列； 4:无第三排，用12排出列，该标识用于不满三列查找是否有漏的，较少的异常情况
	uint8   	DelaySpeed; //最后的速度
	
	uint32 		DueTime;    //超时时刻
	uint8 		WaitingMtchFlag;//等待匹配的标识
	uint8		HideZTJoinFlag; //隐藏窄条成列标识，1：无第一排，用234出列；2：无第二排，用134出列；3：无第三排，用124出列；
											//4：无第四排，用123出列；5：用12出列；6：用13出列；7：用23出列；8:用1出列;  9:用34出列;     0x55:满列；
	uint8   	TaiTrace[4]; //该列轮胎轨迹，记录极大值位置，0,1,2,3
	uint8		LoRTai[4];  //该窄条代表窄条的左侧或者右侧半窄条,0为Both;1为左侧窄条;2为右侧窄条
	uint8   	TaiType[4];//该列的轮胎车型
	uint8   	SpeedMatchFlag;    //速度匹配标识,0:正常匹配模式(速度和接地长度匹配),1:速度匹配
}Colum_Struct;

//暂时未用
typedef enum trace
{
	Normal=1,    //正常压两列  1
	CrossRoad,   //跨道        2
	Single,      //单列        3
	ThreColum,   //压三列      4
	SpanColum    //跨列        5
}TagTrace;
//窄条列结构
typedef struct _colum
{
	int16		Colum[2]; 	//存放窄条的位置
	uint16 		MaxValue; 	//最大值
	uint16 		Length;		//接地长度
	uint8		RunAndWalk;//走走停停标识，0为非
	int8 		Dir;      	//方向,1表示正向，-1表示逆向
	uint16 		Speed;		// 轴速度
	uint8 		ZTCnt; //该列窄条的数量,默认为0
	uint8		LowSpdDlyTimeFlg;
	
	uint8 		LeftMtchFlag;//左侧匹配窄条疑似的标识
	uint8		RightMtchFlag;//右侧匹配窄条疑似的标识	
	uint16		WonderLMtchID; //疑似匹配的窄条ID
	uint16		WonderRMtchID; //疑似匹配的窄条ID
	uint32 		DueTime;    //超时时刻
	uint8		FristElemRow;  //默认0为首排,1为第二排
	uint8		AddLogicTimeFlg;	//1为增加
	uint8   FirWhelPos;
	uint8   SecWhelPos;
	uint8   WhelCnt;
}HideColum_Struct;


 //轴数据结构
typedef struct tagAxle				
{
	uint8 	u8id;					//轴编号
	uint8 	u8Owner;				//轴归属的车辆
	uint8	u8OrderID;		    	//记录本车的第几只轴
	uint8   u8ChlNum;               //轴建立在哪个车道上
	
 	uint8	AxleChlPos[3];          //轴中压窄条的列，用于确定其轨迹及建立的车道列
	int8   	Dir;		
	uint16	u16speed;				//轴速 
	uint16  WheelSpd;               //轮速度，单位cm/s
	
	uint16  LaserVehID;             //该轴属于激光的车辆编号
	uint8   u8ValidChlCnt;			//轴匹配窄条列的个数，初始为0
	uint8   u8PointsGet;            //得分	
	int32	s32AxleWeight;		    //轴重
	uint16	u16AixsType;			//胎型
	uint16	u16ChListSum;			//轴对应的窄条总数计算重量用
		
	uint16	u16ChList[6];			//轴对应的窄条存储位置
	uint16  u16LeftWheeList[6];    //左轮对应的窄条存储位置
	uint8	u8LeftWheelTakeZT[6];  //左侧轮取窄条哪个部分，1为左侧部分，2为右侧部分，3为窄条整体
	uint8	u8RightWheelTakeZT[6];
	uint16  u16RightWheeList[6];   //右轮对应的窄条存储位置
	uint16  u16LeftWheelMaxValueAver;        //左轮窄条最大值均值
	uint16  u16RightWheelMaxValueAver;       //右轮窄条最大值均值

	uint32  HideZTPostAxleTime;     //隐藏窄条抛轴的时刻
	uint8   AxleStatus;             //轴行驶状态   1:正常,2:压缝 3:左斜,4:右斜,5:绕S  
	uint8	AxleWhelCnt;			//轴轮的个数，默认为0, 2为两个
	uint8   RunAndWalkFlag;         //走走停停标识 0:没有走走停停,1:有走走停停
	uint8	ZTLayouType;				//布局类型  默认为0为交错布局，1为一字布局
	
	uint16  LeftWheelXRange;        //左侧轮处在位置范围
	uint16	RightWheelXRange;       // 右侧轮处在位置范围
	uint16 	BuildLeftWhelXRange;    //建立轴时左轮在首排的位置
	uint16	BuildRghWhelXRange;
	uint16  LeftWheelAverPos;       //左轮窄条位置平均值,单位cm
	uint16  RightWheelAverPos;      //右轮窄条位置平均值,单位cm
	uint16  MidXPos;                //轴中间位置,单位cm
	
	uint8   LeftWheelZTCnt;
	uint8	RightWheelZTCnt;
	uint8	WheelType;              //轮类型 1:单胎; 2:双胎
	uint8   AxleAccCnt;
	
	uint16	AxleAcc[12];
	uint8   u8AxleErrorType;        //轴异常类型,bit0:出现一个或多个窄条积分和异常,bit1:轮包含的窄条序列时间差过大,bit2:同一列窄条出现2个相同ID号,bit3:标准板块轴匹配的窄条数小于5个
								    //bit4:两个轮速度差过大,bit5:隐藏轮匹配窄条数小于4个,bit6:轴为跨列时判断出为走走停停或斜行,bit7:None
	uint8   ZType;                  //窄条类型  默认为0，数字窄条为1
	
	uint32  ZTWgt[12];    			//存储每根窄条的重量,单位kg
	uint32  WheelWgt[2];  			//0:左轮重,1:右轮重,单位kg
	uint16  ZTPos[12];              //存储窄条Pos
	uint8   ZTID[12];               ////存储窄条id
	
	uint32  PostAxleTime;           //抛轴时刻
	uint8 HideLeftWhePos;
	uint8 HideRightWhePos;
}Axle_Struct;


//车辆数据结构
typedef struct tagVehicle				
{
	uint8 	u8id;							//车辆编号
	uint8	u8lvl;							//可信度 	
	uint8 	LasterID;						//触发的激光编号
	uint8 	u8psLane;						//经过的车道编号
	int8	n8accSpeed[ZHOUSIZE];			//加速度	
	uint8	u8OverFlag;						//超限标识
	uint8   UnionWgtFlg;					//联合共称标识
	uint16	u16speed;						//车速
	
	//		
			  
	uint16	u16AxleList[ZHOUSIZE];			//对应的轴编号,触发编号
	uint16	u16HideAxleList[ZHOUSIZE];			//对应的轴编号,触发编号
	uint16	u16UnionFailedAxleList[ZHOUSIZE];	//联合共秤失败轴列表,对应的轴编号,触发编号
	uint8	au8AxleTAIType[ZHOUSIZE];		//胎型	  
	uint8	au8AxleGrpType[ZHOUSIZE];		//轴组轴型		  
	uint16	au16AxleWeight[ZHOUSIZE];		//轴重
	uint16	au16HideAxleWeight[ZHOUSIZE];		//隐藏轴重
	uint16	au16UnionFailedAxleWeight[ZHOUSIZE];	//联合共秤失败缓存轴重
	uint16	au16AxleGrpWeight[ZHOUSIZE];	//轴组重		   wujinlong
	uint16	au16AxleDistance[ZHOUSIZE-1];	//轴距			   wujinlong
	uint16	au16HideAxleDistance[ZHOUSIZE-1];	//隐藏轴距
	//
	uint16	m_u16VehWide;				// 车宽，车高 wujinlong 20131031
	uint16	m_u16VehHeight;
	uint8   m_u8VehType;
	uint8   m_u8CPID;	
	uint16  m_u16VehLength;
	//
	uint16  m_u16LaserCnt;
	uint16  m_u16LoopVal;
	uint32  VehicleType; //	旧高速预检软件兼容
	uint8   m_u8ProFlag;
	uint8	m_u8Daoflag;
	uint8   m_u8LaserVehID;
	uint8   m_u8PointGet;//车得分
	//
	uint32  m_u32BuildTime;
	uint32  m_u32EndTime;
	uint8   m_au8AxlePoint[ZHOUSIZE]; //每轴得分
	uint32  m_u32LoopTime;
	//
	uint8   m_u8LaserPos[4]; //分米	
	uint8   m_Capture_LaneNum;  //抓拍车道，高低四位分别代表车道
	uint8   m_BigVehFollowLitVeh;//大车跟小车的标识，0标识无，1标识有，抛车时清零
	uint8   m_LitVehFollowBigVeh;//小车跟大车的标识，0标识无，1标识有，抛车时清零
	uint8	u8AixCnt;						//轴的个数
	uint8  	u8AixGrpCnt;					//轴组的个数
	uint8	AxleStatus[7]; //轴行驶的轨迹	
	uint32  u32HideYiWgt;// 一字布局两排的重量
	uint32  u32HideJCWgt;//隐藏交错布局车的重量	
						//最新布局的结构体，复用中航的状态值，轴行驶状态   1:正常,2:左斜,3:右斜,4:绕S  默认为0
	uint8   u8PostVehFlag;    //抛车标识(系统出车方式),取值范围:0~255
	uint8   u8ViolationCode;                //违例码, bit7:走走停停,bit6:跨界,bit5:点刹,bit4:S弯,bit3:加速,bit2:减速,bit1:压三边,bit0:跨道;bit位为1有效
	uint8   m_n8Dir;
} Vehicle_Struct;

typedef struct tagMColum			
{
	uint32 AverColumWgt;    //列平均重量
	uint8 mcolum[5];
	uint8 WonderColum;
	uint8 WonderCache;
	uint8 Cache[5];
	uint8 RunAndWalk[5];
	uint8 cnt;	
}MColum_Struct;

typedef struct _backVeh
{
	uint8 backAxleCnt;//倒车轴数
	uint8 firstRowCnt;//经过首排的次数,最大为3
	//uint16 fixedelaytime;	//固定延时时间
	uint16 axleSpeed;
	uint32 firstRowDelyDntime;//首窄条下秤时间
	uint32 fristRowDntime;
}BackVeh_Struct;

typedef struct _deteleVeh
{
	uint32 l_u32DeleteTime;
	uint8 l_u8DeleteFlag;//1:为正;2:为倒
}DeleVeh_Str;

//激光数据结构
typedef struct taghead				
{	
	int8 	m_cFlag;									// -1初始化,0表示下秤,2表示保持,1上秤
	//
	uint8	u8type;										//事件类型
	uint16 	u16Ltpos; 									//左侧进入点
	uint16 	u16rtpos; 									//右侧进入点
	//
	uint8	m_u8VehLane;								// 车道、车宽、车高 wujinlong 20131031
	uint16	m_u16VehWide;							  	
	uint16	m_u16VehHeight;	
	uint8 	m_u8VehID;
	
	uint16 m_u16VehCalWide;    // 计算宽度
}head_Struct;


typedef struct tagRaster				
{	
	int8 m_ucTouch;  //触发激光编号，1位表示1个激光，0-3位表示4个激光
	uint32	u32tm;//触发时间
	uint8	u8Cnt;	//车辆数
	int8 m_bDirectionFlag;//方向，1-2个激光表示一个方向，使用1表示，3-4个激光表示另一个方向，使用0表示
	uint8 VehChangeFlag;    // 有车辆变化标识
	head_Struct headinfo[LASTERCOUNT<<2];				   // 20121108
} Raster_Struct;

typedef struct tagVehicleInfForLoop
{
	int8    m_n8CurrentAxleNum;//初始-1
	uint8   m_u8CrossHide; //交错或者隐藏  0交错，1隐藏
	int8    m_u8DireFlag;//正向还是逆向标记 1 正向 -1 逆向
	uint8   m_BigVehFollowLitVeh;//大车跟小车的标识，0标识无，1标识有，抛车时清零
	int16   m_an16CurrentAxleInf[ZHOUSIZE][MAXNUM_AXLE];//保存当前轴的有效窄条号 u16sefID
	int16	m_an16HideCurAxleInf[ZHOUSIZE][MAXNUM_AXLE];//隐藏窄条当前轴的有效窄条ID位置	
	uint8   m_au8AxleDropFlag[ZHOUSIZE];//记录轴是否已经抛 初始0
	uint8   m_au8NewAxleFlag[ZHOUSIZE];//记录该轴抛之前是否有新轴 初始0
	int8    m_an8AxleProRow[ZHOUSIZE];//记录当前轴的有效窄条已经到第几排 初始-1
	uint8   m_u8VehicleHaveFlag;//标记是否有车	  0:无车 1:有车
	uint8   m_u8FollowNewVehFlag;//标记后面是否有新车跟进，0：无车 1：有车
	uint8   RunAndWalkFlag;//m_u8FollowNewVehFlag-->RunAndWalkFlag
	int8    m_n8XieZouFlag;    //数字布局斜走标志, 0：正常  1：左斜  2：右斜
	uint8   m_u8SVSZ;//是否单车单条一排模式左侧窄条 0 不是 1是
	uint8 	u8psLane;						//经过的车道编号
	uint8   u8ViolationCode;                //违例码, bit7:走走停停,bit6:跨界,bit5:点刹,bit4:S弯,bit3:加速,bit2:减速,bit1:压三边,bit0:跨道;bit位为1有效	
	uint8   m_LitVehFollowBigVeh;//小车跟大车的标识，0标识无，1标识有，抛车时清零
	uint8   m_u8VehIDFromLaser;
	uint32  m_u32TimeNow;//记录处理轴的最新时间 初始值0
	uint32  m_u32BuildTime;//建车时间
	uint32  m_u32EndTime;//结束时间
	int8	n8accSpeed[ZHOUSIZE];			//加速度
	int8    m_n8CurrentHideAxleNum;//初始-1
	uint8	u8OverFlag;						//超限标识
	uint16	u16speed;						//车速				
	uint16	u16AxleList[ZHOUSIZE];
	uint16  u16HideAxleList[ZHOUSIZE];
	uint8	u8AixCnt;						//轴的个数	
	uint8	u8HideAixCnt;
	uint8  	u8AixGrpCnt;					//轴组的个数
	uint8  	u8HideAixGrpCnt;
	uint8	au8AxleTAIType[ZHOUSIZE];		//胎型	  
	uint8	au8AxleGrpType[ZHOUSIZE];		//轴组轴型		  
	uint16	au16AxleWeight[ZHOUSIZE];		//轴重			  
	uint16	au16AxleGrpWeight[ZHOUSIZE];	//轴组重
	uint16	au16AxleDistance[ZHOUSIZE-1];	//轴距
	uint8	au8HideAxleTAIType[ZHOUSIZE];		//隐藏胎型	  
	uint8	au8HideAxleGrpType[ZHOUSIZE];		//隐藏轴组轴型		  
	uint16	au16HideAxleWeight[ZHOUSIZE];		//隐藏轴重			  
	uint16	au16HideAxleGrpWeight[ZHOUSIZE];	//隐藏轴组重
	uint16	au16HideAxleDistance[ZHOUSIZE-1];	//隐藏轴距
	//
	uint16	m_u16VehWide;				// 车宽，车高 wujinlong 20131031
	uint16	m_u16VehHeight;
	uint8   m_Capture_LaneNum;  //抓拍车道，高低四位分别代表车道
	uint8	AxleStatus[7]; //轴行驶的轨迹	
	uint8   m_au8AxlePoint[ZHOUSIZE]; //每轴得分	
	uint32  m_u32LoopTime;    //抓拍时刻，即485ID
	uint32  u32HideYiWgt;// 一字布局两排的重量
	uint32  u32HideJCWgt;//隐藏交错布局车的重量  
						//最新布局的结构体，复用中航的状态值，轴行驶状态   1:正常,2:左斜,3:右斜,4:绕S  默认为0
	uint8   NormalAxleScore[7];    //标准板块轴的综合得分(行驶状态,窄条匹配,轴轨迹的综合得分)
	uint8   HideAxleScore[7];      //隐藏板块轴的综合得分(行驶状态,窄条匹配,轴轨迹的综合得分)
	uint8 	m_u8NormalLogiclvl;      //中航逻辑可信度,默认为0:可信,!0:不可信(不可信类型见轴异常类型)
	uint8 	m_u8HideLogiclvl;    //隐藏逻辑可信度,默认为0:可信,!0:不可信
//	uint8   m_u8PointsGet;//车得分
	uint32  m_u32ForcePostTime;
	uint8   m_u8PostAxleFlag;
} VehicleInfForLoop_Struct;
typedef struct tagVehUnUsForLoop
{
	uint8 m_u8PassThreeWonder;//压三边怀疑标志 0 没有 1 有怀疑
	uint8 m_u8WonderChl;
	uint16 m_u16ID;
} VehUnUsForLoop_Struct;

//通用
typedef struct Stu_loop_info
{
	uint8 loop_num;
	uint8 frame_id;
	uint8 frame_type;
	uint8 rsv;
	uint32 frame_data;
	uint32 trigger_time;
}Stu_loop_info;

#define MAX_AXLE_A_VEHICLE 		ZHOUSIZE		//单车最多16根轴
#define MAX_ZT_AXLE_AFFECT 		12				//单轴最多影响窄条数

typedef int32 UN;
typedef int32 US;
typedef int32 UL;
typedef int32 UC;


//窄条信号数据结构---------
typedef struct {
	UL m_ulUpTime;
	UL m_ulDownTime;
	UN m_unSum;
	UN m_unMaxValue;	
}ZhitiaoLogicData;
//
typedef struct{					   //窄条一条数据
	uint16	u16ID;
	UC m_ucTouch;					// 窄条编号1-32
	UL m_ulTime;					// 上秤时间或下秤时间
	int8 m_bUpOrDn;					// 1：上秤，0 下秤
	ZhitiaoLogicData m_CWbZtInf;
	int8 m_bDirectionFlag;
} ZTINFRECELEMENT;

// 串口缓存
#define		COM_LEN		128
typedef struct _CComBuf
{
	uint16	m_u16Len;
	uint8	m_au8DataBuf[COM_LEN];
} CComBuf;



// 日志，采用in!=out方式，则直接定义为二维数组
// added by wujinlong 20130819
// 日志缓存
#define		LOG_NUM		1000
#define		LOG_LEN		128
typedef struct _CLogBuf
{
	uint8	m_u8In;
	uint8	m_u8Out;
	uint16	m_u16Len[LOG_NUM];
	uint8	m_au8DataBuf[LOG_NUM][COM_LEN];
} CLogBuf;

typedef struct	//任务时间统计
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
	uint8	m_u8IsValidFlag;		// 1：数据有效，0：数据无效
	uint16	m_u16ChlIx;				// 传感器索引
} CYaFengStack;
//#endif

typedef struct _CSDOffnetIx
{
	uint32		m_u32OffnetVehNum ;					// 总车数
	uint32		m_u32OffnetWrSec;					// 写扇区位置
	uint16		m_u32OffnetWrSub;					// 写扇区内位置
	uint32		m_u32OffnetRdSec;					// 读扇区位置
	uint16		m_u32OffnetRdSub;					// 读扇区内位置
	uint32		m_u32VehIx;							// 位置序号
} CSDOffnetIx;


//#if LASER_UNNORMAL_EN > 0
typedef struct _CVehSimInfo
{
	uint8	m_u8IsValidFlag;		// 1：数据有效，0：数据无效
	uint32	m_u32PostVehFPGATime;	// 抛车时FPGA时间
	uint8	m_u8AxleNum;			// 轴数
	uint32	m_u32Weight;			// 总重
} CVehSimInfo;
//#endif

///////////////////////////////////////////交调程序使用结构体
// 总重重量段数据结构
typedef struct _AXLE_DATA
{
	uint32 che_num;		                // 车数  
	uint32 axis_num;					// 轴数
	uint32 allche_wt;				    // 车总重
	uint32 allaxle_wt; 			        // 轴总重
	uint32 acc_equivalent_axles ;		// 当量轴次合计

} AXLE_DATA;
// 定义车道轴载发送数据缓存结构
typedef struct _LANE_AXLE_DATA
{
	AXLE_DATA axle_data[10];            // 总重统计	10个重量段
	AXLE_DATA axis_num_data[5];         // 轴数统计	5个轴数段
	AXLE_DATA axle_weight_data[4];      // 轴重统计	4个重量段

} LANE_AXLE_DATA;
// 定义车道缓冲数据结构
typedef struct _LANE_INFO
{
	// 线圈相关
	uint32 mu32LoopOnTime;
	uint32 mu32LoopOffTime;

	uint32 mu32LastLoopOnTime;
	uint32 mu32LastLoopOffTime;

  uint32 mu32TimeOnOff;     // 通过线圈时间 
	uint32 mu32LoopScope;
} LANELOOP_INFO;

typedef struct _LANE_TRAFFIC_DATA
{
	uint32 mu32LaneNum;						// 车道号
	uint32 mu32VehNum; 					// 车数
	uint32 mu32FollowVehNum;					// 跟车数
	uint32 mu32FollowVehPercent;				// 跟车百分比
	uint32 mu32TotalDisForVehHead; 			// 车头总间距
	uint32 mu32AveDisForVehHead; 					// 平均车头间距
	uint32 mu32TimeOccupancy;				// 时间占有率
	uint32 mu32XiaoHuoNum;					// 小货交通量
	uint32 mu32XiaoHuoAveSpeed;			// 小货平均速度
	uint32 mu32XiaoHuoTotalSpeed;			// 小货总速度
	uint32 mu32ZhongHuoNum;				// 中货交通量
	uint32 mu32ZhongHuoAveSpeed;			// 中货平均速度
	uint32 mu32ZhongHuoTotalSpeed;		// 中货总速度
	uint32 mu32DaHuoNum;					// 大货交通量
	uint32 mu32DaHuoAveSpeed;				// 大货平均速度
	uint32 mu32DaHuoTotalSpeed;			// 大货总速度
	uint32 mu32ZhongXiaoKeNum;				// 中小客交通量
	uint32 mu32ZhongXiaoKeAveSpeed;		// 中小客平均速度
	uint32 mu32ZhongXiaoKeTotalSpeed;		// 中小客总速度
	uint32 mu32DaKeNum;					// 大客交通量
	uint32 mu32DaKeAveSpeed;				// 大客平均速度
	uint32 mu32DaKeTotalSpeed;			// 大客总速度
	uint32 mu32TuoGuaNum;					// 拖挂车交通量
	uint32 mu32TuoGuaAveSpeed;				// 拖挂车平均速度
	uint32 mu32TuoGuaTotalSpeed;			// 拖挂车总速度
	uint32 mu32TuoLaJiNum;					// 拖拉机交通量
	uint32 mu32TuoLaJiAveSpeed;			// 拖拉机平均速度
	uint32 mu32TuoLaJiTotalSpeed;			// 拖拉机总速度
	uint32 mu32TeDaHuoNum;					// 特大货交通量
	uint32 mu32TeDaHuoAveSpeed;			// 特大货平均速度
	uint32 mu32TeDaHuoTotalSpeed;			// 特大货总速度
	uint32 mu32MotoNum;					// 摩托车交通量
	uint32 mu32MotoAveSpeed;				// 摩托车平均速度
	uint32 mu32MotoTotalSpeed;			// 摩托车总速度
	uint32 mu32JiZhangXiangNum;	  	// 集装箱车平均速度
	uint32 mu32JiZhangXiangAveSpeed; 	// 集装箱车总速度
	uint32 mu32JiZhangXiangTotalSpeed;		  	// 集装箱车总速度

} LANE_TRAFFIC_DATA;

typedef struct tagJDSysParam
{
	uint8   m_au8UseName[8];
	uint8   m_au8KeyNum[8];
	
	uint8   m_au8DeviceID[16];

	uint8   m_au8StationID[15];
	uint8   m_u8LaneDir;  //0 单向 1 双向

	uint8   m_u8OrderMod;
	uint8   m_u8SavePeriod;
	uint8   m_u8TransMod;//上传方式 1 网口 2 无线
	uint8   m_u8SendVehType;
	 
	uint16  m_u16SmallandMid;  //小中车轴间距界限
	uint16  m_au16Elevation;  //高程

	uint32  m_u32FollowVehTimeThr;//交调跟车时间阈值
//	uint32  m_u32MidKeHuoThr;//交调线圈赋值中客中货界限
//	uint32  m_u32SmallKeHuoThr;// 交调线圈赋值小客小货界限
	uint32  m_u32DiffSend; //交调待传上限值

	uint8   m_au8LngBuf[8];	//经度
	uint8   m_au8LatBuf[8];	//纬度

	uint8	m_u8ProgVersion[4];					// 程序版本标示

	uint16  m_u16MidKeHuoThr[10];			   //FZX
	uint16  m_u16SmallKeHuoThr[10];					   	//FB

	uint16  m_u16VehNumForSend;//单车待传数量值
	uint16  m_u16UnUse;

	uint32  m_u32WeightSendThr;//重量待传上限值
	uint32  m_u32AxleNumSendThr;//轴数待传上限值
	uint32  m_u32AxleWghSendThr;//轴重待传上限值

	uint32  m_u32MaxForAxle;//单轴轴重上限值

	uint8 	m_au8ReserveByte[JDSYS_RESERVED];			//预留字节	
	uint16	m_u16CRC;
} CJDSysParam;

#endif

