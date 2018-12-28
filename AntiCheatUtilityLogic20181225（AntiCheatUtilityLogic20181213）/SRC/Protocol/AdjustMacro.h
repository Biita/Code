/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			AdjustMacro.h
** Last modified Date:  20110531
** Last Version:		1.0
** Descriptions:		调校协议命令宏定义
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110531
** Version:				1.0
** Descriptions:		调校协议命令宏定义
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

//命令帧
//	0xFF 00 [LEN] [COMM] [PARAM1] [PARAM2] [PARAM3] [VALUE1] [VALUE2] [VALUE3] [VALUE4] [CRC1] [CRC2]

//命令宏定义	COMM
#define		ADJ_GETSDINFO			0xA0	 	// 查询SD卡内容	//wgh
#define		ADJ_SETSDINFO			0xA1	 	// 设置SD卡内容	//wgh
#define		ADJ_GETIPINFO			0xA2		//读取仪表IP相关参数 //wgh
#define		ADJ_UPDATEIPINFO		0xA3		//更改仪表IP相关参数 //wgh
#define		ADJ_GETNONEWEIGHT		0xA4		//读取所有非重量修正参数//wgh
#define		ADJ_UPDATENONEWEIGHT	0xA5		//更新所有非重量修正参数 //wgh
#define		ADJ_SAVE				0xA6	 	//保存仪表参数指令 //wgh
#define		ADJ_GETVRSION			0xA7		// 获取版本号  //wgh
#define		ADJ_GETMOTIONWEIGHT		0xA8		//读取所有动态修正参数	//wgh
#define		ADJ_UPDATEMOTIONWEIGHT	0xA9		//更新所有动态修正参数 //wgh

#define 	ADJ_SENDRUNTIME 		0xAB		// 各任务计时统计
#define		ADJ_IPSEARCH		0xAD		//工控机IP搜索 //wgh
#define     ADJ_SUPERCOMD       0xAC        //超级指令
#define		Para_Read			0xD0		//交调中获取设备参数
#define		Para_Save			0xD1		//交调中设置设备参数
#define		JD_Reset 		    0xD2		//交调中初始化01数据包
#define		JD_Init				0xD3		//交调中设置默认参数

#define     ADJ_CTRSAMPPOW      0xE1 //采集板供电指令
#define     ADJ_ZTINTENSET      0xE2 //窄条使能设置
#define     ADJ_ZTINTENASK      0xE3 //窄条使能查询
#define     ADJ_DEBUGINFO       0xE0 //调试信息命令

//
#define		ADJ_SENDAXLEGRP		0x80		//发送轴组数据	//wgh
#define		ADJ_SENDAXLE		0x81		//发送轴数据	//wgh
#define		ADJ_TIMESYNCHRONY	0x82		//时间同步 //wgh
#define		ADJ_CAPTURE			0x83		//抓拍 //wgh
#define		ADJ_RESTART	    	0x84		//重启 //wgh
#define		ADJ_DEVICESTATUS	0x85		//设备状态//wgh
#define		ADJ_HEARTBEAT		0x86		//心跳	//wgh
#define		ADJ_SETHEARTEN		0x87		// 设置心跳使能//wgh
#define     ADJ_CONHEART        0x88        //工控机心跳指令 //wgh
#define     ADJ_POWERRUN        0x89        //仪表上电报告 //wgh
#define		ADJ_RESEND			0x8A		//重发 //wgh
#define     ADJ_IOUSEASK        0x8D		//IO使用查询
#define     ADJ_IOUSESET        0x8C		//IO使用设置
#define     ADJ_TESTCOM         0x8E        //测试指令 //wgh

#define		ADJ_EXT				0x8F		//扩展指令
#define     ADJ_USESTAT         0x90        //设备使用查询
#define     ADJ_ZTARRAYASK      0x91		//窄条接收矩阵查询
#define     ADJ_ZTARRAYSET      0x92		//窄条接收矩阵设置
#define     ADJ_CHLASK          0x93		//车道设置查询
#define     ADJ_CHLSET          0x94		//车道设置设置

#define		ADJ_NETCONNECT		0x95		//工控机检查线路连接


//和激光模块交互时的命令
//（工控机->ARM->激光模块）
#define     LASER_SET_PARA_RQ   0xB0        //设置激光参数
#define     LASER_READ_PARA_RQ  0xB1        //读取激光参数
#define     LASER_RESET_RQ      0xB2        //激光复位
//（激光模块->ARM->工控机）
#define     LASER_SET_PARA_RS   0xC0        //设置激光参数应答
#define     LASER_READ_PARA_RS  0xC1        //读取激光参数应答
#define     LASER_RESET_RS      0xC2        //激光复位应答
#define     LASER_VEHICLE_POS   0xCA        //车辆位置信息
#define     LASER_STATE_CC      0xCC        //激光状态信息（心跳帧）



//////////////////////////////////////////////////////////////		 

#endif	//__ADJUSTMACRO_H
