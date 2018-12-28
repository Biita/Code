/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			UI.h
** Last modified Date:  20110505
** Last Version:		1.0
** Descriptions:		程序所有界面函数
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

UI_EXT      void    Seach_TongdaoZY(uint8 ChedaoNum, uint8 TongdaoNum);		 //查询通道增益
UI_EXT      void    Seach_TongdaoNM(uint8 ChedaoNum, uint8 TongdaoNum);     //查询通道内码

UI_EXT      void    Display_Chezhong(Vehicle_Struct *pItem);       //车辆重量显示

//UI_EXT	void	UIStartUp(void);			//启动界面	
//UI_EXT	void	UIGeneral(void);			//通用过车界面	
//
//UI_EXT	void	UIBDRoot(void);				//标定根界面 
 

//UI_EXT	void	UIBDMain(uint8 p_u8Motion);	//动静态标定界面 
//UI_EXT	void	UIBDWanBanChoose(uint8 p_u8Motion);	//弯板标定通道选择界面
//UI_EXT	void	UIBDChengTai(uint8 p_u8Motion);					//称台标定界面
//UI_EXT	void	UIBDScale(uint8 p_u8Motion);					//设置分度
//UI_EXT	void	UIBDStaticModify(void);		//设置静态修正界面，按照传感器重量修正
//UI_EXT	void	UIBDChooseMotion(void);		//动态修正选择界面，点修or线修
//UI_EXT	void	UIBDFullRange(void);		//设置最大量程 
//UI_EXT	void	UIBDHeightTanxingti(void);			//弹性体高度界面
//UI_EXT	void	UIBDPoDu(void);				//坡度修正界面
//UI_EXT	void	UIBDChooseVehPos(void);		//车型速度修正位置选择
UI_EXT	void	SaveParams(uint8 num);			//保存参数设置
//UI_EXT	void	UIBDRoadPlaneHeight(void);			//路面值高度界面
//UI_EXT	void	UIBDWanBan(uint8 p_u8CID, uint8 p_u8Motion);	//弯板标定界面
//UI_EXT	void	UISetValueParamU16(char * p_pcName, uint16 * p_pu16Param, uint32 p_u32Min, uint32 p_u32Max);
//UI_EXT	void	UIBDLineModify(void);		//线性修正界面
//UI_EXT	void	UIBDPointModify(void);		//点修正界面
//UI_EXT	void	UIBDChooseVeh(uint8 p_u8Pos);	//选择车型界面，根据压缝位置不同，查看的数据不同
//UI_EXT	void	UIBDVSModifyParam(uint8 p_u8Pos, uint8 p_u8Veh);	//参数值界面，根据压缝位置和车型

//UI_EXT	void	UIValidate(void);			//刷程序后验证界面 
//UI_EXT	void	UISystemInit(void);			//系统初始化界面

UI_EXT	void	InitSystem(uint8 num);			//初始化所有系统参数
UI_EXT	void	InitNonWeight(void);		//初始化非称重设置
//UI_EXT	void 	FDistBetwInit(void);
//UI_EXT	void	InitMotionModify(void);		//初始化动态修正参数
//UI_EXT	void	InitVehModify(void);		//初始化车型修正参数
//UI_EXT	void	InitStaticModify(void);		//初始化静态修正参数
UI_EXT	void	InitThreshold(void);		//初始化阈值参数
//UI_EXT	void	InitVehBufIndex(void);		//初始化铁电缓存车辆信息 
UI_EXT	void	YBRestart(void);			//重启
//UI_EXT	uint8	RecoverToLast(void);		//恢复为最近标定参数
//UI_EXT	uint8	RecoverToHistory(void);		//恢复为历史标定参数
//UI_EXT	void	InitRestart(void);			//初始化后自动重启

UI_EXT	void 	InitNetParam(uint8 num);			//初始化网口信息
UI_EXT	void	SaveNetInfo(uint8 num);			//保存网络信息

UI_EXT	void	ClearStartupCnt(void);		//启动次数清零
UI_EXT	void	AddStartupCnt(void);		//启动次数加1


//UI_EXT	void	UISN(void);					//SN码界面
//UI_EXT	void 	CLEARZERO(void);
//UI_EXT	void	UIF3Code(void);				//查看F3代码界面
//UI_EXT	void	UIF4Code(void);				//查看F4代码界面
//UI_EXT	void	UIF5Code(void);				//查看F5代码界面

//#if	 OS_TASK_CREATE_EXT_EN >0
//UI_EXT	void 	DisplayStatics(void);
//#endif

//UI_EXT	void	UICommonSet(void);			//普通设置界面，111
//UI_EXT	void	UIViewSetting(void);		//查看设置参数界面，222
//UI_EXT	void	UIViewModify(void);			//查看速度修正界面，333
//UI_EXT	void	UIViewAuthor(void);			//查看作者信息界面，888
//UI_EXT	void	UIViewIPInfo(void);			//查看IP信息，444
//UI_EXT	void	UIViewStartUpTime(void);	//查看启动时间信息，8494
//UI_EXT	void	UIViewThreshold(void);		//阈值参数，8968
//
//UI_EXT	void	UISetTime(void);			//设置时间界面
//UI_EXT	void	UISetBaudRate(void);		//设置波特率界面
//UI_EXT	void	UISetProtocol(void);		//协议界面
//UI_EXT	void	UISetLoop(void);			//线圈使能界面
//UI_EXT	void	UISetDog(void);				//设置看门狗界面
//UI_EXT	void	UISetDiaodian(void);		//设置掉电保护界面

//UI_EXT	void	UISetValueParamIP(char * p_pcName, uint8 * p_pu8IP);	//设置IP
//UI_EXT	void 	UISetValueParamU32(char * p_pcName, uint32 * p_pu32Param, uint32 p_u32Min, uint32 p_u32Max);
//UI_EXT	void 	UISetEnableParam(char * p_pcName, uint8 * p_pu8Param);
UI_EXT	void	SaveTime(SystemTime p_stSet);				//保存时间

//#if	SENDWAVEENABLE > 0		//使能发波形
//UI_EXT	void	UIBDSendWave(void);			//发送波形使能界面
//#endif
UI_EXT void ProModInit(uint8 num);//wgh 20140303
UI_EXT void InitJDParam(void);
#endif
