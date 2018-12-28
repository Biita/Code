/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			UI.c
** Last modified Date:  20110506
** Last Version:		1.0
** Descriptions:		所有界面函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:			ZHANG Ye
** Created date:		20110506
** Version:				1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			ZHANG Ye			
** Modified date:		20110510
** Version:				1.1
** Descriptions:		普通双弯板
**
*********************************************************************************************************/
#define	__UI_C
#include "UI.h"

//压缝位置
//#define		POS_ABS		1			//AB速度
//#define		POS_BCS		2			//BC速度
//#define		POS_ABCS	3			//压缝速度
//#define		POS_VEH		4			//车型整体
//#define		POS_GAP		5			//压缝整体
//#define		POS_2WB		6			//两弯板情况

//static	uint8	m_u8Err;
//static char	m_acTmp[30];		//显示屏一行

#define		SETUPALIAS				g_Setup			//设置参数结构
#define		IPINFOALIAS				g_IPSetLocal			//IP信息别名

//#define		LCDPRINTC				PrintCharXY			//显示字符串
//#define		LCDPRINTFC				PrintFormatDataXY	//显示一定格式的字符串

#define		CRCFunc					CheckCrc			//CRC校验函数名	 
#define		CRCFunc16				AddCrc16			//CRC校验函数名,带赋值

//控制指令:0x01:重画；0x02：退出					
//#define		ResetControlCode(ControlCode)	ControlCode = 0x00
//
//#define		IfReDraw(ControlCode)	ControlCode & 0x01
//#define		ToReDraw(ControlCode)	ControlCode |= 0x01
//#define		NotReDraw(ControlCode)	ControlCode &= ~0x01
//
//#define		IfBreak(ControlCode)	ControlCode & 0x02
//#define		ToBreak(ControlCode)	ControlCode |= 0x02
//#define		NotBreak(ControlCode)	ControlCode &= ~0x02
//
////清除键值
//#define		KeyValue				g_u8KeyValueMapped
//#define		ClearKeyValue()			KeyValue	= 0xff
//
//#define		ClearLCDLine(X)			LCDPRINTC(0		, X	, "                              ")
//
////等待屏幕刷新信号
//#define		WAITSCREENREFRESH()		OSSemPend(g_psemScreenRefresh,0, &m_u8Err)

#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
/*********************************************************************************************************
** Function name:           Display_Chezhong
**
** Descriptions:            车辆重量显示初始界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	rp
** Created Date:            20151009
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Display_Chezhong(Vehicle_Struct *pItem)
{
	uint8 Y = 18;
	uint8 i = 0;
	char l_Cheliang_Info[40];
	uint32 l_Chezhong = 0;		 //总车重
	uint8 l_temp;
	char ZhouXing[10];			 //轴型


	GUI_ClearSCR();
	if(g_u8StopChange == 1)
	{
// 		PrintCharXY(0, 0,"轴序        轴重   速度*");
		PrintCharXY(0, 0,"轴组序        轴组重   速度*");
	}
	else
	{
// 		PrintCharXY(0, 0,"轴序        轴重   速度");
		PrintCharXY(0, 0,"轴组序        轴组重   速度");
	}
	GUI_Line(0,17,240,17, 1	);

// 	//显示轴号和轴重
// 	for(l_temp = 0; l_temp < pItem -> u8AixCnt; l_temp++)
// 	{
// 		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
// 		sprintf(l_Cheliang_Info, "  %d       %dkg", l_temp + 1, pItem ->au16AxleWeight[l_temp] * 10);
// 		PrintCharXY(0, Y, l_Cheliang_Info);
// 		Y += 15;
// 		l_Chezhong += pItem -> au16AxleWeight[l_temp] * 10;

// 	}
	//显示轴号和轴组重
	for(l_temp = 0; l_temp < pItem -> u8AixGrpCnt; l_temp++)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "  %d       %dkg", l_temp + 1, pItem ->au16AxleGrpWeight[l_temp] * 10);
		PrintCharXY(0, Y, l_Cheliang_Info);
		Y += 15;
		l_Chezhong += pItem -> au16AxleGrpWeight[l_temp] * 10;

	}

	//计算总重，显示轴组重
	for(l_temp = 0; l_temp < pItem -> u8AixGrpCnt; l_temp++)
	{
//		l_Chezhong += g_vehicleInfo[(g_u8vehicleID + VEHICLE_BUFMASK - 1) % VEHICLE_BUFMASK].au16AxleGrpWeight[l_temp] * 10;

//		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
//		sprintf(l_Cheliang_Info, "%dkg", g_vehicleInfo[(g_u8vehicleID + VEHICLE_BUFMASK - 1) % VEHICLE_BUFMASK].au16AxleGrpWeight[l_temp] * 10);
//		PrintCharXY(56, i * 15 + 18, l_Cheliang_Info);

		switch(pItem -> au8AxleGrpType[l_temp])
		{
			case 1:
			case 2:
				i += 1;
				break;
			case 3:
			case 4:
			case 5:
				i += 2;
				break;
			case 6:
			case 7:
				i += 3;
				break;
			case 8:
			case 9:
				i += 4;
				break;
			default:
				i += 1;				 
		}

	}
	//显示速度
	if(pItem -> u16speed != 0)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "%dkm/h", pItem -> u16speed / 10);
		PrintCharXY(150, 18, l_Cheliang_Info);
	}
	//显示总重
	PrintCharXY(0, 110, "总重:");
	if(l_Chezhong != 0)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "%dkg", l_Chezhong);
		PrintCharXY(40, 110, l_Cheliang_Info);
	}
	//显示轴型	
	memset(ZhouXing, 0, sizeof(ZhouXing));
	for(l_temp = 0; l_temp < pItem -> u8AixGrpCnt; l_temp++)
	{
		 ZhouXing[l_temp] = pItem -> au8AxleGrpType[l_temp] + 0x30;
	}
	ZhouXing[l_temp] = '\0';
	PrintCharXY(105, 110, "轴型:");
	PrintCharXY(145, 110, ZhouXing);	
	//显示车道号
	PrintCharXY(185, 110, "车道:");
	if(pItem -> u8AixCnt > 1)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "%d", pItem -> u8psLane / 2 + 1);
		PrintCharXY(225 , 110, l_Cheliang_Info);
	}
	
}
/*********************************************************************************************************
** Function name:           DisplayInitLcd
**
** Descriptions:            LCD初始界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern uint8 g_u8CheDaoNum;				 
extern char g_ucTongdaoZY_Display[10];
extern char g_ucTongdaoNM_Display[10];
extern char g_ucTongdaoZero_Display[10];
extern uint16 g_nTongdaoZY_Change;
void DisplayInitLcd(void)
{	
	char l_ucTempCheDaoNum[2];
	GUI_ClearSCR();
	PrintCharXY(40,0,"非现场执法通道信息");// 非现场执法通道信息 	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	PrintCharXY(10,40,"当前车道:  ");	   // 当前车道：
	sprintf(l_ucTempCheDaoNum, "%d", g_u8CheDaoNum);	 
	PrintCharXY(100,40,l_ucTempCheDaoNum);	       
//	PrintCharXY(120,20,"当前通道:  ");  	   // 当前通道
//	PrintCharXY(10,40,"初始内码:  ");	   // 初始内码：
//	PrintCharXY(120,40,"当前内码:  ");	   // 当前内码
//	PrintCharXY(10,60,"当前增益:  ");	   // 当前增益：
	PrintCharXY(10,60,"静态重量:   ");	   // 静态重量: 
	GUI_Line(0,105,240,105, 1	);//	
	PrintCharXY(0,108,"F1-置零");
	PrintCharXY(62,108,"F2-车道选择");
	PrintCharXY(154,108,"F3-增益调整");
//	PrintCharXY(120,108,"F4 查询版本号");
}

/*********************************************************************************************************
** Function name:           DisplayChooseChedao
**
** Descriptions:            F2车道选择界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChooseChedao(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"选择车道");// 选择车道	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(70,20,"1.车道1");	   // 选择车道1  
	PrintCharXY(70,40,"2.车道2");	   // 选择车道2
	PrintCharXY(70,60,"3.车道3");	   // 选择车道3
	PrintCharXY(70,80,"4.车道4");	   // 选择车道4 
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(0,108,"提示:数字键1~4选择车道");
}

/*********************************************************************************************************
** Function name:           DisplayChooseTongdao
**
** Descriptions:            选择对应车道的通道
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void DisplayChooseTongdao(void)
//{
//	GUI_ClearSCR();
//	PrintCharXY(10,0,"选择通道");// 选择通道	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
//	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(40,40,"1.通道1");	   // 选择通道1  
//	PrintCharXY(140,40,"2.通道2");	   // 选择通道2
//	PrintCharXY(40,60,"3.通道3");	   // 选择通道3
//	PrintCharXY(140,60,"4.通道4");	   // 选择通道4
//	PrintCharXY(40,80,"5.通道5");	   // 选择通道5
//	PrintCharXY(140,80,"6.通道6");	   // 选择通道6 
//	GUI_Line(0,105,240,105, 1	);
//	PrintCharXY(0,108,"提示:数字键1~6选择通道");
//}

/*********************************************************************************************************
** Function name:           DisplayFunction
**
** Descriptions:            功能界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayFunction(void)
{
	GUI_ClearSCR();
	PrintCharXY(40,0,"非现场执法静态功能");// 非现场执法静态功能 	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	PrintCharXY(70,20,"1.车道增益  ");	   // 称台增益  
	PrintCharXY(70,40,"2.通道增益  ");	   // 通道增益
	PrintCharXY(70,60,"3.零点跟踪使能");	   // 零点跟踪使能
//	PrintCharXY(70,80,"4.零点查询   ");	   // 零点查询  
	GUI_Line(0,105,240,105, 1	);//	
	PrintCharXY(5,108,"提示:数字键1~3选择功能");
}

/*********************************************************************************************************
** Function name:           DisplayChooseTongdao_Chedao
**
** Descriptions:            选择通道所在车道
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChooseTongdao_Chedao(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"通道增益");// 通道增益	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"1.车道1各通道增益查询及修改");	   
	PrintCharXY(10,40,"2.车道2各通道增益查询及修改");	   
	PrintCharXY(10,60,"3.车道3各通道增益查询及修改");	   
	PrintCharXY(10,80,"4.车道4各通道增益查询及修改");	    
	GUI_Line(0,105,240,105, 1	);
//	PrintCharXY(0,108,"提示:数字键1~4选择通道所在车道");
	PrintCharXY(0,108,"按1~4选择需要修改增益的车道");
}

/*********************************************************************************************************
** Function name:           DisplayChedaox_zengyi
**
** Descriptions:            显示车道增益
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChedao1_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道1增益");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"初始内码:");	    
	PrintCharXY(10,40,"当前内码:");
	PrintCharXY(10,60,"当前增益:");	  
	PrintCharXY(10,80,"静态重量:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-清零");
	PrintCharXY(120,108,"F3-修改增益");
//	PrintCharXY(0,108,"提示:数字键1~4选择车道");
}
void DisplayChedao2_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道2增益");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"初始内码:");	    
	PrintCharXY(10,40,"当前内码:");
	PrintCharXY(10,60,"当前增益:");	  
	PrintCharXY(10,80,"静态重量:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-清零");
	PrintCharXY(120,108,"F3-修改增益");
//	PrintCharXY(0,108,"提示:数字键1~4选择车道");
}
void DisplayChedao3_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道3增益");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"初始内码:");	    
	PrintCharXY(10,40,"当前内码:");
	PrintCharXY(10,60,"当前增益:");	  
	PrintCharXY(10,80,"静态重量:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-清零");
	PrintCharXY(120,108,"F3-修改增益");
//	PrintCharXY(0,108,"提示:数字键1~4选择车道");
}
void DisplayChedao4_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道4增益");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"初始内码:");	    
	PrintCharXY(10,40,"当前内码:");
	PrintCharXY(10,60,"当前增益:");	  
	PrintCharXY(10,80,"静态重量:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-清零");
	PrintCharXY(120,108,"F3-修改增益");
//	PrintCharXY(0,108,"提示:数字键1~4选择车道");
}


/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao1
**
** Descriptions:            显示车道1通道增益
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayTongdaoZY_Chedao1(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道1通道增益");// 车道1增益	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"通道1内码:");	   // 通道1~7内码
//	PrintCharXY(120,20,"通道2内码:");
//	PrintCharXY(10,40,"通道3内码:");
//	PrintCharXY(120,40,"通道4内码:");
//	PrintCharXY(10,60,"通道5内码:");
//	PrintCharXY(120,60,"通道6内码:");			
	PrintCharXY(10,20,"1.通道1:");		// 通道1~6增益
	Seach_TongdaoZY(1, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.通道2:");
	Seach_TongdaoZY(1, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.通道3:");
	Seach_TongdaoZY(1, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.通道4:");
	Seach_TongdaoZY(1, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.通道5:");
	Seach_TongdaoZY(1, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.通道6:");
	Seach_TongdaoZY(1, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.内码查询");		//内码查询提示
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按1-6修改对应增益");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao2
**
** Descriptions:            显示车道2通道增益
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:			    rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayTongdaoZY_Chedao2(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道2通道增益");// 车道1增益	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"通道1内码:");	   // 通道1~7内码
//	PrintCharXY(120,20,"通道2内码:");
//	PrintCharXY(10,40,"通道3内码:");
//	PrintCharXY(120,40,"通道4内码:");
//	PrintCharXY(10,60,"通道5内码:");
//	PrintCharXY(120,60,"通道6内码:");			
	PrintCharXY(10,20,"1.通道1:");		// 通道1~6增益
	Seach_TongdaoZY(2, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.通道2:");
	Seach_TongdaoZY(2, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.通道3:");
	Seach_TongdaoZY(2, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.通道4:");
	Seach_TongdaoZY(2, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.通道5:");
	Seach_TongdaoZY(2, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.通道6:");
	Seach_TongdaoZY(2, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.内码查询");		//内码查询提示
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按1-6修改对应增益");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chengtai3
**
** Descriptions:            显示车道3通道增益
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayTongdaoZY_Chedao3(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道3通道增益");// 车道1增益	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"通道1内码:");	   // 通道1~7内码
//	PrintCharXY(120,20,"通道2内码:");
//	PrintCharXY(10,40,"通道3内码:");
//	PrintCharXY(120,40,"通道4内码:");
//	PrintCharXY(10,60,"通道5内码:");
//	PrintCharXY(120,60,"通道6内码:");			
	PrintCharXY(10,20,"1.通道1:");		// 通道1~6增益
	Seach_TongdaoZY(3, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.通道2:");
	Seach_TongdaoZY(3, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.通道3:");
	Seach_TongdaoZY(3, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.通道4:");
	Seach_TongdaoZY(3, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.通道5:");
	Seach_TongdaoZY(3, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.通道6:");
	Seach_TongdaoZY(3, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.内码查询");		//内码查询提示
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按1-6修改对应增益");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao4
**
** Descriptions:            显示车道4通道增益
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	Wang Feng
** Created Date:            20150714
**--------------------------------------------------------------------------------------------------------
** Modified by:				rp
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayTongdaoZY_Chedao4(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道4通道增益");// 车道1增益	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"通道1内码:");	   // 通道1~6内码
//	PrintCharXY(120,20,"通道2内码:");
//	PrintCharXY(10,40,"通道3内码:");
//	PrintCharXY(120,40,"通道4内码:");
//	PrintCharXY(10,60,"通道5内码:");
//	PrintCharXY(120,60,"通道6内码:");			
	PrintCharXY(10,20,"1.通道1:");		// 通道1~6增益
	Seach_TongdaoZY(4, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.通道2:");
	Seach_TongdaoZY(4, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.通道3:");
	Seach_TongdaoZY(4, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.通道4:");
	Seach_TongdaoZY(4, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.通道5:");
	Seach_TongdaoZY(4, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.通道6:");
	Seach_TongdaoZY(4, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);

	PrintCharXY(10,80,"0.内码查询");		//内码查询提示
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按1-6修改对应增益");
}
/*********************************************************************************************************
** Function name:           DisplayProgramVersion
**
** Descriptions:            显示版本号和程序修改日期
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	rp
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayProgramVersion(void)						   
{	
	GUI_ClearSCR();
	PrintCharXY(10,0, "版本号:        ");
	PrintCharXY(130,0, PROGRAMVERSION);
	PrintCharXY(10,20,"程序修改日期:  ");
	PrintCharXY(130,0, PROGMODIFYDATE);	   
	GUI_Line(0,105,240,105, 1	);//
	PrintCharXY(10,108,"按ESC返回上一级");	
}
/*********************************************************************************************************
** Function name:           DisplayChange_ChedaoxZY
**
** Descriptions:            车道增益修改界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	rp
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChange_Chedao1ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道1增益修改");
	PrintCharXY(10,40,"请输入新的增益:");
//读取键盘输入信息到l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按Enter保存更新");	
}
void DisplayChange_Chedao2ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道2增益修改");
	PrintCharXY(10,40,"请输入新的增益:");
//读取键盘输入信息到l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按Enter保存更新");
}
void DisplayChange_Chedao3ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道3增益修改");
	PrintCharXY(10,40,"请输入新的增益:");
//读取键盘输入信息到l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按Enter保存更新");
}
void DisplayChange_Chedao4ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道4增益修改");
	PrintCharXY(10,40,"请输入新的增益:");
//读取键盘输入信息到l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按Enter保存更新");
}
/*********************************************************************************************************
** Function name:           DisplayChange_TongDaoxZY
**
** Descriptions:            通道增益修改界面
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:             	rp
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChange_TongDao1ZY(void)
{
	//每输入一个字符会重新刷新界面，并将新输入的字符显示出来
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道1增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道1增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
void DisplayChange_TongDao2ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道2增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道2增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
void DisplayChange_TongDao3ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道3增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道3增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
void DisplayChange_TongDao4ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道4增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道4增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
void DisplayChange_TongDao5ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道5增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道5增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
void DisplayChange_TongDao6ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"通道6增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"通道6增益修改");
		PrintCharXY(10,40,"请输入新的增益:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"按Enter保存更新");
	}
}
/*********************************************************************************************************
** Function name:		DisplayChedaox_Neima
** Descriptions:		查询车道x通道内码
** input parameters:	None 
** output parameters:	none
**
** Created by:			rp		  
** Created Date:		20150820	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void DisplayChedao1_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道1内码查询");		  //车道1通道1-6内码
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"通道1:");
	Seach_TongdaoNM(1,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"通道2:");
	Seach_TongdaoNM(1,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"通道3:");
	Seach_TongdaoNM(1,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"通道4:");
	Seach_TongdaoNM(1,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"通道5:");
	Seach_TongdaoNM(1,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"通道6:");
	Seach_TongdaoNM(1,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);

	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按ESC返回上一级");
}
void DisplayChedao2_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道2内码查询");		   //车道2通道1-6内码
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"通道1:");
	Seach_TongdaoNM(2,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"通道2:");
	Seach_TongdaoNM(2,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"通道3:");
	Seach_TongdaoNM(2,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"通道4:");
	Seach_TongdaoNM(2,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"通道5:");
	Seach_TongdaoNM(2,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"通道6:");
	Seach_TongdaoNM(2,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按ESC返回上一级");
}
void DisplayChedao3_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道3内码查询");		    //车道3通道1-6内码
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"通道1:");
	Seach_TongdaoNM(3,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"通道2:");
	Seach_TongdaoNM(3,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"通道3:");
	Seach_TongdaoNM(3,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"通道4:");
	Seach_TongdaoNM(3,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"通道5:");
	Seach_TongdaoNM(3,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"通道6:");
	Seach_TongdaoNM(3,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按ESC返回上一级");
}
void DisplayChedao4_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"车道4内码查询");			 //车道4通道1-6内码
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"通道1:");
	Seach_TongdaoNM(4,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"通道2:");
	Seach_TongdaoNM(4,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"通道3:");
	Seach_TongdaoNM(4,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"通道4:");
	Seach_TongdaoNM(4,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"通道5:");
	Seach_TongdaoNM(4,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"通道6:");
	Seach_TongdaoNM(4,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"按ESC返回上一级");
}

/*********************************************************************************************************
** Function name:		Seach_TongdaoZY
** Descriptions:		查询通道增益
** input parameters:	None 
** output parameters:	none
**
** Created by:			rp		  
** Created Date:		20150820	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Seach_TongdaoZY(uint8 ChedaoNum, uint8 TongdaoNum)
{
//	uint16 l_nTongdaoZY = g_Setup.an32StaGain[((ChedaoNum - 1) * 6 + (TongdaoNum - 1))];
	uint16 l_nTongdaoZY = g_Setup.an32AxGain[((ChedaoNum - 1) * 6 + (TongdaoNum - 1))];
	memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	sprintf(g_ucTongdaoZY_Display, "%d", l_nTongdaoZY);
}
/*********************************************************************************************************
** Function name:		Save_TongdaoZY
** Descriptions:		保存通道增益修改
** input parameters:	None 
** output parameters:	none
**
** Created by:			rp		  
** Created Date:		20150820	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Save_TongdaoZY(uint8 ChedaoNum, uint8 TongdaoNum)
{
//	g_Setup.an32StaGain[((ChedaoNum - 1) * 6 + (TongdaoNum - 1))] = g_nTongdaoZY_Change;
	g_Setup.an32AxGain[((ChedaoNum - 1) * 6 + (TongdaoNum - 1))] = g_nTongdaoZY_Change;
	SaveParams(1);
	g_nTongdaoZY_Change = 0;
}
/*********************************************************************************************************
** Function name:		Seach_TongdaoNM
** Descriptions:		查询通道内码
** input parameters:	None 
** output parameters:	none
**
** Created by:			rp		  
** Created Date:		20150820	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Seach_TongdaoNM(uint8 ChedaoNum, uint8 TongdaoNum)
{
	uint8 l_u8BankaNum;
	uint8 l_u8BankaTongdaoNum;
	uint16 l_nTongdaoNM;
	l_u8BankaNum = (2 * g_cProModParam.m_u8ZTRow * (ChedaoNum - 1) + TongdaoNum - 1) / 8 + 1;
	l_u8BankaTongdaoNum = (ChedaoNum - 1) * 2 * g_cProModParam.m_u8ZTRow + TongdaoNum - (l_u8BankaNum - 1) * 8;
	l_nTongdaoNM = GETCURRENTDATA(l_u8BankaNum, l_u8BankaTongdaoNum);

	memset(g_ucTongdaoNM_Display, 0, sizeof(g_ucTongdaoNM_Display));
	sprintf(g_ucTongdaoNM_Display, "%d", l_nTongdaoNM);
}
/*********************************************************************************************************
** Function name:		Seach_TongdaoZero
** Descriptions:		查询通道零点
** input parameters:	None 
** output parameters:	none
**
** Created by:			rp		  
** Created Date:		20151013	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Seach_TongdaoZero(uint8 ChedaoNum, uint8 TongdaoNum)
{
	uint8 l_u8BankaNum;
	uint8 l_u8BankaTongdaoNum;
	uint16 l_nTongdaoZero;
	l_u8BankaNum = (2 * g_cProModParam.m_u8ZTRow * (ChedaoNum - 1) + TongdaoNum - 1) / 8 + 1;
	l_u8BankaTongdaoNum = (ChedaoNum - 1) * 2 * g_cProModParam.m_u8ZTRow + TongdaoNum - (l_u8BankaNum - 1) * 8;
	l_nTongdaoZero = GETCURRENTZERO(l_u8BankaNum, l_u8BankaTongdaoNum);

	memset(g_ucTongdaoZero_Display, 0, sizeof(g_ucTongdaoZero_Display));
	sprintf(g_ucTongdaoZero_Display, "%d", l_nTongdaoZero);
}
/*********************************************************************************************************
** Function name:		UIStartUp
** Descriptions:		启动画面，不需要循环显示，间隔一段时间后关闭
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIStartUp(void)
//{
//	GUI_ClearSCR();								//清屏
//	DrawPic();									//显示图片
//
//	LCDPRINTC(130	, 72	, PROGRAMVERSION	);	//显示版本信息
//
////	BeepON();			//峰鸣器开
////	
////	OSTimeDly(10);	
////	
////	BeepOFF();			//峰鸣器关	      
////	
//
//	LEDON();
//
//}

/*********************************************************************************************************
** Function name:		UIGeneral
** Descriptions:		标准过程界面，一直显示
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIGeneral(void)
//{
//	#define BUF_LEN	100
//
//	uint8	l_u8ControlCode;		//控制指令
//	uint32	l_u32Temp1;				//临时变量1
//	int32	l_n32MaxWt;				//保存单根窄条中最大值
//	uint32	l_u32Temp2,l_u32Temp3;	//临时变量2,3 
//	uint32	l_u32TempAllWeight = 0;//临时变量，总重 
//	//char 	l_cTaiXing[20];
//	//uint8	l_u8TaiXingIndex;	  
//	char 	l_strOut[BUF_LEN]={0};
//	char  *l_pStr = (char *)0;
//	uint8   l_State = ' '; 
//	Axle_Struct	*l_pAxle;
//	Vehicle_Struct *l_pVeh;
//
//	AddStartupCnt();	//记录启动次数
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	memset(l_strOut, 0, BUF_LEN);  
//	while(1)
//	{  	  	
//		//画界面
//		GUI_ClearSCR();
//		
//		LCDPRINTC(0		, 0		, "序号  "	);
//		LCDPRINTC(40	, 0		, "轴数  "	);
//		LCDPRINTC(80	, 0		, "总重  "	); 
//		LCDPRINTC(136	, 0		, "轴型  "	);
//	   	LCDPRINTC(200	, 0		, "状态  "	); 
//
//		GUI_Line(0		, 16	, 239	, 16	, 1	);
//		GUI_Line(0		, 114	, 239	, 114	, 1	);	
//		BackGroundON();	
//		while (1)		//判断按键并刷新数据
//		{
////			OSTimeDly(10);
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToReDraw(l_u8ControlCode);
//					UISN();
//					ClearKeyValue();
////					l_u8ReShowVeh	= 0x00;		//禁止重新显示
//					break;
//
//				case KB_F1:		//清零
//					NotReDraw(l_u8ControlCode);
//					CLEARZERO();
//					ClearKeyValue();
//					break;				
//				case KB_F2:		//动静态切换
//					NotReDraw(l_u8ControlCode);
////					g_u8StaticMotionswitch	= (g_u8StaticMotionswitch + ENABLE_DYNAMIC) & 0x01;  
//					ClearKeyValue();
//					break;	
//				
//				case KB_F3:		//程序信息
//					ToReDraw(l_u8ControlCode); 
////					l_u8ReShowVeh	|= 0x01;	//允许重新显示
//					UIF3Code();	 
//					ClearKeyValue();				
//					break; 	 				
//				case KB_F4:		//过车代码
//					ToReDraw(l_u8ControlCode);	
////					l_u8ReShowVeh	|= 0x01;	//允许重新显示
//					UIF4Code();
//					ClearKeyValue();
//					break;					
//				case KB_F5:		//轮轴状态
//					ToReDraw(l_u8ControlCode);	 
////					l_u8ReShowVeh	|= 0x01;	//允许重新显示
//					UIF5Code();
//					ClearKeyValue();
//					break;	
//				
//				case KB_F6:		//显示屏背景灯开关
//					NotReDraw(l_u8ControlCode);
//					BackGroundReverse();
//					ClearKeyValue();	
//					break;	
//#if	 OS_TASK_CREATE_EXT_EN >0
//				case KB_F7:		//任务统计情况
//					ToReDraw(l_u8ControlCode);
//				    DisplayStatics();
//					ClearKeyValue();
//				  	break;	
//#endif					
////				case KB_POINT:	//. 手动收尾
////					NotReDraw(l_u8ControlCode);
////					g_u8ShouWeiTag	= 1;
////					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//		
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode))
//				break;
//
//			//刷新通道最值，时间,温度，错误码，待发送车辆数
//			if (OSSemAccept(g_pSemTime)>0)		//有信号量，申请并显示数据
//			{
//				l_n32MaxWt = 0;
//				l_u32Temp2 = 0;
//				for(l_u32Temp1 = 0;l_u32Temp1<CHANNELNUM;l_u32Temp1++)
//				{
//				   if(g_s32ADValue[l_u32Temp1]>l_n32MaxWt)
//				   {
//				  	 l_n32MaxWt = g_s32ADValue[l_u32Temp1];
//					 l_u32Temp2 = l_u32Temp1;
//				   }
//				}
//				 l_n32MaxWt = (l_n32MaxWt - g_an32MotionZero[l_u32Temp2])*SETUPALIAS.an32AxGain[l_u32Temp2]/10000;
//
//				if(g_u8StaticMotionswitch)
//				{
//				  	sprintf(l_strOut,"D%d(%d)  %+02dC E%02X BUF%02d %0.2d:%0.2d:%0.2d",
//								 l_n32MaxWt,l_u32Temp2+1,g_n32Temperature,g_u8ERR,g_vehicleSndCnt, 
//								 g_DevCurTime.u8Hour,g_DevCurTime.u8Minute,g_DevCurTime.u8Second);
//				}
//				else
//				{
//				 	sprintf(l_strOut,"S%d(%d)  %+02dC E%02X BUF%02d %0.2d:%0.2d:%0.2d",
//								 l_n32MaxWt,l_u32Temp2+1,g_n32Temperature,g_u8ERR,g_vehicleSndCnt, 
//								 g_DevCurTime.u8Hour,g_DevCurTime.u8Minute,g_DevCurTime.u8Second);
//				}	   				 
//			
//				LCDPRINTC(0	, 115	, l_strOut);  
//			}
//
//			//LCDPRINTFC( 0		, 20		,  "%02d   "	, g_LasterState[0]	);
//			//LCDPRINTFC( 0		, 36		,  "%02d    "	, g_LasterState[1]	);
//
//			//刷新轴显示数据 
//			l_pAxle	= (Axle_Struct *)OSQAccept(g_pQAxleShow);		//过车过程，轴信息
//			if (l_pAxle!= (Axle_Struct *)0)		//消息队列中有消息，申请并显示数据
//			{
//			//	memcpy(&l_asTmp, (Axle_Struct *)l_pTmp,sizeof(Axle_Struct));	
//				//ClearLCDLine(18 + ((l_pAxle->u8Owner&0x06)<<4));
//				ClearLCDLine(18 + ((l_pAxle->u8Owner%3)<<4));
//				l_u32Temp1 =  g_vehicleInfo[l_pAxle->u8Owner].LasterID;
//				l_State = g_LasterState[l_u32Temp1] !=0 ? '*' :' ';
//				sprintf(l_strOut,"%03d  %02d  %05d  %d  %c",
//				                 l_pAxle->u8Owner+1,
//								 g_vehicleInfo[l_pAxle->u8Owner].u8AixCnt,
//								 l_pAxle->s32AxleWeight,
//								 l_pAxle->u16AixsType,
//								 l_State);
//				//LCDPRINTC(0		, 18 + ((l_pAxle->u8Owner&0x06)<<4)	, l_strOut);
//				LCDPRINTC(0		, 18 + ((l_pAxle->u8Owner%3)<<4)	, l_strOut); 		
//			}  
//
//			l_pStr = (char *)OSQAccept(g_pQErrMsg);
//			if(l_pStr != (char *)0)
//			{
//				LCDPRINTC(0	, 115	, l_pStr);	
//			}
//
//			//显示完整过车数据
//			l_pVeh = (Vehicle_Struct *)OSQAccept(g_pQVehicle);
//			if ((l_pVeh != (Vehicle_Struct *)0))		
//			{  
//				l_u32Temp3 = 0;
//				for(l_u32Temp1=0;l_u32Temp1<l_pVeh->u8AixGrpCnt;l_u32Temp1++)
//				{
//					 l_u32Temp3 = l_u32Temp3*10 + l_pVeh->au8AxleGrpType[l_u32Temp1];	
//				}
//				// test
//				if( 11 == l_u32Temp3 )
//				{
//					l_u32Temp3 = l_u32Temp3;
//				}
//				//
//				l_u32TempAllWeight = 0;
//				for(l_u32Temp1=0;l_u32Temp1<l_pVeh->u8AixCnt;l_u32Temp1++)
//				{
//					l_u32TempAllWeight += l_pVeh->au16AxleWeight[l_u32Temp1];
//				}
//
//				//ClearLCDLine(18 + ((l_pVeh->u8id&0x07)<<4));
//				sprintf(l_strOut,"%03d  %02d  %05d   %d    ",
//				                 l_pVeh->u8id+1,
//								 l_pVeh->u8AixCnt,
//								 l_u32TempAllWeight,
//								 l_u32Temp3);
//				//LCDPRINTC(0		, 18 + ((l_pAxle->u8Owner&0x06)<<4)	, l_strOut);
//				if( 0 == l_pVeh->u8id%3 )
//				{
//					ClearLCDLine(68);
//					LCDPRINTC(0		, 68	, l_strOut);
//				}
//				else
//				{
//					ClearLCDLine(68 + ((l_pVeh->u8id%3)<<4) - 2 );
//					LCDPRINTC(0		, 68 + ((l_pVeh->u8id%3)<<4) - 2, l_strOut);
//				}
//				  							
//			}
//
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//		}												    
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UIBDRoot
** Descriptions:		标定根界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDRoot(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " 系统应用选项");
//		LCDPRINTC(176	, 0		, "Esc 退出");
//		
//		LCDPRINTC(8		, 24	, "1. 动态秤标定");
//		LCDPRINTC(8		, 44	, "2. 静态秤标定");
//		LCDPRINTC(8		, 64	, "3. 系统初始化");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//
//				case KB_1:		//动态标定
//					ToReDraw(l_u8ControlCode);
//					UIBDMain(UI_MOTION);
//					ClearKeyValue();
//					break;	
//				
//				case KB_2:		//静态标定
//					ToReDraw(l_u8ControlCode);
//					UIBDMain(UI_STATIC);
//					ClearKeyValue();
//					break;	
//				
//				case KB_3:		//系统初始化
//					ToReDraw(l_u8ControlCode);
//					UISystemInit();
//					ClearKeyValue();
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}	
//}
/*********************************************************************************************************
** Function name:		UIBDMain
** Descriptions:		动态标定界面
** input parameters:	p_u8Motion 动静态标志 0，静态，1，动态 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDMain(uint8 p_u8Motion)
//{
//	uint8	l_u8ControlCode;//控制指令
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue(); 
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		if (p_u8Motion == UI_MOTION)		//动态
//			LCDPRINTC(0		, 0		, " 动态标定");
//		else 				//静态
//			LCDPRINTC(0		, 0		, " 静态标定");
//
//		LCDPRINTC(112	, 0		, "0 保存  Esc 退出" );
//		
//		LCDPRINTC(8		, 24	, "1. 弯板标定");
//		LCDPRINTC(8		, 44	, "2. 秤台标定");
//		LCDPRINTC(8		, 64	, "3. 分度设定");
//		if (p_u8Motion == UI_MOTION)
//		{
//			LCDPRINTC(8		, 84	, "4. 静态修正");
//			LCDPRINTC(8		, 104	, "5. 动态修正");
//			LCDPRINTC(128	, 24	, "6. 量程设定");
//			LCDPRINTC(128	, 44	, "7. 弹性体高度");
//			LCDPRINTC(128	, 64	, "8. 缓坡长度");
//			LCDPRINTC(128	, 84	, "9. 车型修正");
//			LCDPRINTC(128	, 104	, "F1 路面值高度");
//		}
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			NotReDraw(l_u8ControlCode);
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//弯板标定
//					ToReDraw(l_u8ControlCode);
//					UIBDWanBanChoose(p_u8Motion);
//					break;	
//				
//				case KB_2:		//称台标定
//					ToReDraw(l_u8ControlCode);
//					UIBDChengTai(p_u8Motion);
//					break;	
//				
//				case KB_3:		//分度设定
//					ToReDraw(l_u8ControlCode);
//					UIBDScale(p_u8Motion);
//					break;	
//				
//				case KB_4:		//静态修正
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDStaticModify();
//					}
//					break;	
//				
//				case KB_5:		//动态修正
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDChooseMotion();
//					}
//					break;	
//				
//				case KB_6:		//量程设定
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDFullRange();
//					}
//					break;	
//				
//				case KB_7:		//弹性体高度
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDHeightTanxingti();
//					}
//					break;	
//				
//				case KB_8:		//缓坡长度
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDPoDu();
//					}
//					break;	
//				
//				case KB_9:		//车型修正
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDChooseVehPos();
//					}
//					break;	
//				
//				case KB_0:		//保存设定
//					ToReDraw(l_u8ControlCode);
//					SaveParams();
//					break;	
//				
//				case KB_F1:		//路面值高度
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDRoadPlaneHeight();
//					}
//					break;	
//
//#if	SENDWAVEENABLE > 0		//使能发波形				
//				case KB_F2:		//发波形使能
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDSendWave();
//					}
//					break;	
//#endif				 				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDWanBanChoose
** Descriptions:		弯板动态标定通道选择界面
** input parameters:	p_u8Motion	动静态标志 0表示静态，1表示动态 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  	ZHANG Ye	
** Modified date:	  	20110801
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDWanBanChoose(uint8 p_u8Motion)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Key;
//	uint32  l_u32Value = 0;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 112	, 239	, 112	, 1	);				
//		LCDPRINTFC(0,113,"输入区间[1,%d]",CHANNELNUM);
//		if (p_u8Motion == UI_MOTION)		//动态
//			LCDPRINTC(0		, 0		, "1. 弯板动态标定       Esc-返回");
//		else 				//静态
//			LCDPRINTC(0		, 0		, "1. 弯板静态标定       Esc-返回");  	
//		
//		 while(1)
//		 {
//		   	l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//				case KB_ENTER:
//					if(l_u32Value > 0)
//						ToReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;	
//						if(l_u32Value > CHANNELNUM)	
//							l_u32Value = 0;	
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;					
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//					ClearKeyValue();
//					break;	
//			} 		
//			if(l_u32Value)
//				LCDPRINTFC(0		, 19		, "通道号:%d     ",l_u32Value);
//			else
//			 	LCDPRINTC(0		, 19		, "通道号:       ");
//			if(IfBreak(l_u8ControlCode) ||IfReDraw(l_u8ControlCode))
//				break;		
//		 }
//
//		 if(IfBreak(l_u8ControlCode))
//		 {
//		 	 break;
//		 } 		
//		
//		 if(IfReDraw(l_u8ControlCode))
//		 {
//		 	UIBDWanBan(l_u32Value, p_u8Motion);
//			ResetControlCode(l_u8ControlCode);
//			l_u32Value = 0;
//		 }
//		 WAITSCREENREFRESH();		//等待刷新信号 	
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDChengTai
** Descriptions:		称台动态标定界面
** input parameters:	p_u8Motion	动静态，0表示静态，1表示动态
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDChengTai(uint8 p_u8Motion)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value;		//输入的数字
//	uint8	l_u8InputStatus;	//输入状态；1：
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	l_u8InputStatus	= 0;
//	l_u32Value	= 0;
//	while(1)
//	{	 		
//		
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "2. 秤台标定           Esc-返回");
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 108	, 239	, 108	, 1);	
//		LCDPRINTC(0,109,"F1-修改秤台增益");										   		
//		while(1)
//		{
//			//当前秤台增益	
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//				  	ToBreak(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8InputStatus != 0)
//					{ 
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;					
//				case KB_BKSP:		//回删一个数字
//					if (l_u8InputStatus != 0)
//					{ 
//						l_u32Value	/= 10;
//					}
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//				case KB_ENTER:
//					if(l_u32Value)
//					{
//					 SETUPALIAS.u16PlatformGain =  l_u32Value;
//					 l_u8InputStatus = 3;
//					 l_u32Value = 0;	
//					}						
//					 ClearKeyValue();
//					 break;
//				case KB_F1:
//					l_u8InputStatus = 1;
//					ClearKeyValue();	
//					break;
//				default:
//					break;
//			}
//
//			if(l_u8InputStatus == 1)
//			{
//				if(l_u32Value)
//					LCDPRINTFC(16		, 30	, "秤台增益:%d        ",l_u32Value);
//				else
//				{
//					LCDPRINTC(16		, 30	, "秤台增益:        ");	
//					ClearLCDLine(67);
//				}
//					
//			}
//			else
//			{
//				LCDPRINTFC(16		, 30	, "秤台增益:%d        ",SETUPALIAS.u16PlatformGain);
//				if(l_u8InputStatus == 3)
//					 LCDPRINTC(16		, 67	, "增益值已修改!");			
//					
//			}
//
//		   	LCDPRINTC(16		, 50	, "输入区间:(0,65535)");
//			if (IfBreak(l_u8ControlCode))
//			{
//				break;
//			}
//		}
//
//	 	if (IfBreak(l_u8ControlCode))
//		{
//			break;
//		}	
//
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDScale
** Descriptions:		设置动静态分度界面
** input parameters:	p_u8Motion 动静态标志，0表示静态，1表示动态 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  	ZHANG Ye	
** Modified date:	  	20110801
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDScale(uint8 p_u8Motion)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8 *	l_pu8Scale;
//	switch (p_u8Motion)
//	{
//		case UI_STATIC:	 //静态
//			l_pu8Scale = &SETUPALIAS.u8StaticScale;
//			break;
//			
//		case UI_MOTION:	 //动态
//			l_pu8Scale = &SETUPALIAS.u8MotionScale;
//			break;
//
//		default:
//			return;
//	}
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		if (p_u8Motion == UI_MOTION)		//动态
//			LCDPRINTC(0		, 0		, "动态分度设定         Esc-返回");
//		else 				//静态
//			LCDPRINTC(0		, 0		, "静态分度设定         Esc-返回");
//
//		LCDPRINTC(0		, 19	, "请选择分度值:");
//		
//		LCDPRINTC(64	, 40	, "1--1   kg");
//		
//		LCDPRINTC(64	, 56	, "2--10  kg");
//		LCDPRINTC(64	, 72	, "3--20  kg");
//		LCDPRINTC(64	, 88	, "4--50  kg");
//		LCDPRINTC(64	, 104	, "5--100 kg");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//	
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:
//					*l_pu8Scale		= 1;
//					break;
//
//				case KB_2:
//					*l_pu8Scale		= 10;
//					break;
//
//				case KB_3:
//					*l_pu8Scale		= 20;
//					break;
//
//				case KB_4:
//					*l_pu8Scale		= 50;
//					break;
//
//				case KB_5:
//					*l_pu8Scale		= 100;
//					break;
//	
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (*l_pu8Scale == 0)
//				*l_pu8Scale		= 50;
//
//			LCDPRINTFC(112	, 19	, "%ukg    "	, *l_pu8Scale);
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDStaticModify
** Descriptions:		设置静态修正界面，按照传感器重量修正
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDStaticModify(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Status;		//状态
//	uint8	l_u8Tmp1;		//临时变量
//	uint8	l_u8Tmp2;
//	uint8	l_u8Tmp3;
//
//	ResetControlCode(l_u8ControlCode);
//	l_u8Status	= 0;
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		l_u8Tmp2	= (l_u8Status >> 3)>0 ?	8 : 0;
//		for (l_u8Tmp1 = l_u8Tmp2; l_u8Tmp1 <= l_u8Status; l_u8Tmp1 ++)
//		{
//			l_u8Tmp3	= (l_u8Tmp1 - l_u8Tmp2)<<4;
//			LCDPRINTFC(0		, l_u8Tmp3	, "%d:", l_u8Tmp1);
////			LCDPRINTFC(32		, l_u8Tmp3	, "0- %d"	, SETUPALIAS.au16StaticModify[0][l_u8Tmp1]);
////			LCDPRINTFC(112		, l_u8Tmp3	, "1- %d"	, SETUPALIAS.au16StaticModify[1][l_u8Tmp1]);
//		}
//	
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//				
//				case 0xf1:		//编辑
//					sprintf(m_acTmp, "静态修正%dkm-0", l_u8Status);
////					UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16StaticModify[0][l_u8Status]), 9500, 10500);
//					sprintf(m_acTmp, "静态修正%dkm-1", l_u8Status);
////					UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16StaticModify[1][l_u8Status]), 9500, 10500);
//
//					ToReDraw(l_u8ControlCode);
//					break;
//
//				case 0xff:
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				default:
//					l_u8Status	++;
//					if (l_u8Status == 8)
//						GUI_ClearSCR();
//					else if (l_u8Status == 16)
//					{	
//						ToBreak(l_u8ControlCode);
//						break;
//					}
//
//					//任何按键都显示一条新的数据
//					l_u8Tmp2	= (l_u8Status&0x07) << 4;
//					LCDPRINTFC(0		, l_u8Tmp2	, "%d:", l_u8Status);
////					LCDPRINTFC(32		, l_u8Tmp2	, "0- %d"	, SETUPALIAS.au16StaticModify[0][l_u8Status]);
////					LCDPRINTFC(112		, l_u8Tmp2	, "1- %d"	, SETUPALIAS.au16StaticModify[1][l_u8Status]);
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetValueParamU16
** Descriptions:		通用参数设置界面,设置U16参数
** input parameters:	p_pcName		参数名
**						p_pu16Param 	变量指针
**						p_u32Max		参数上限 
**						p_u32Min		参数下限
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetValueParamU16(char * p_pcName, uint16 * p_pu16Param, uint32 p_u32Min, uint32 p_u32Max)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value	= 0;		//输入数字
//	uint8	l_u8Status;		//功能键状态
//	char	l_acTmp[30];
//	uint8	l_u8Key;
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8Status	= 0;
//
//		//画界面
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s: %u",p_pcName,*p_pu16Param);
//		LCDPRINTC(0		, 0		, l_acTmp);
//		LCDPRINTC(0		, 32	, " Esc退出, F1键修改");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8Status != 0)
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//修改状态，开始输入数字
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= KB_F1;
//						l_u32Value	= 0;
//						memset(l_acTmp, 0, 30);
//						sprintf(l_acTmp," 请输入新值(%u-%u):",p_u32Min,p_u32Max);
//						LCDPRINTC(16		, 56	, l_acTmp);
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					if (l_u8Status	== KB_F1)
//					{
//						if ((p_u32Max == 0 && p_u32Min == 0) || (l_u32Value >= p_u32Min && l_u32Value <= p_u32Max))
//						{
//							*p_pu16Param	= l_u32Value & 0xffff;
//						}
//						l_u32Value	= 0;
//						l_u8Status	= 0;
//						ToReDraw(l_u8ControlCode);
//					}
//					else
//						NotReDraw(l_u8ControlCode);
//					
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (l_u8Status == KB_F1)		//输入状态
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(56		, 80	, "         ");
//				else
//					LCDPRINTFC(56		, 80	, "%u      "	, l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDLineModify
** Descriptions:		线性修正界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDLineModify(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Status;		//功能键状态
//	uint8	l_u8Key;		//按键
//	uint8	l_u8Tmp1,l_u8Tmp2 = 0;
//	char 	l_strOut[30]={0};
//	uint32 	l_u32Value = 0;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		l_u8Status	= 0;
//
//		//画界面
//		GUI_ClearSCR();
//	//	LCDPRINTC(0		, 0		, "请输入速度段n: 1-8  (n-1)*5km");
//	  	sprintf(l_strOut,"请输入速度段[1-%d]:(n-1)*5km",SPEED_CTL_SIZE);	
//		LCDPRINTC(0,0,l_strOut);
//		DrawLineModify(g_Setup.au16Speedmodify);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					if (l_u8Status == 0)
//						ToBreak(l_u8ControlCode);
//					else
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u8Status	= 0;
//					}
//					ClearKeyValue();
//					break;
//			   	case KB_ENTER:
//					if (l_u8Status == 0 && l_u32Value > 0)		//显示对应速度段
//					{
//						l_u8Tmp1	= (l_u32Value - 1)*5;  					
//
//						sprintf(l_strOut,"%dkm-%dkm 当前值: %d",l_u8Tmp1,l_u8Tmp1+5, SETUPALIAS.au16Speedmodify[l_u8Tmp1]);
//						LCDPRINTC(0		, 16	, l_strOut);							
//						LCDPRINTC(0,32,"Esc退出, F1键修改");
//						l_u8Status	= l_u8Tmp1;	
//						l_u32Value = 0;
//					}
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	 
//				case KB_BKSP:
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;	
//				case KB_0:	
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:
//				case KB_9:
//					l_u32Value	*= 10;
//					l_u32Value	+= l_u8Key;
//					if(l_u32Value <= SPEED_CTL_SIZE)
//					{
//						NotReDraw(l_u8ControlCode);
//						sprintf(l_strOut,"请输入速度段[1-%d]:%d(n-1)*5km",SPEED_CTL_SIZE,l_u32Value);	
//						LCDPRINTC(0,0,l_strOut);	
//					}
//					else
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u32Value = 0;
//					}
//					ClearKeyValue();
//					break;					
//				case KB_F1:		//修改状态，开始输入数字
//
//				   	sprintf(l_strOut,"线修正速度%dkm",l_u8Tmp1);
//					UISetValueParamU16(l_strOut, &(SETUPALIAS.au16Speedmodify[l_u8Tmp1]), 8000, 12000); 				
//						
//					if (l_u8Tmp1>1)	//修正左边的点
//					{
//						for(l_u8Tmp2 = 4;l_u8Tmp2 > 0;l_u8Tmp2--)
//						{
//							SETUPALIAS.au16Speedmodify[l_u8Tmp1 - l_u8Tmp2] = SETUPALIAS.au16Speedmodify[l_u8Tmp1 - 5] + (SETUPALIAS.au16Speedmodify[l_u8Tmp1] - SETUPALIAS.au16Speedmodify[l_u8Tmp1-5])*(5 - l_u8Tmp2)/5;	
//						}
//					}
//					if (l_u8Tmp1+1<MAX_SPEED_LIMIT)	//修正右边的点
//					{
//						for(l_u8Tmp2 = 4;l_u8Tmp2 > 0;l_u8Tmp2--)
//						{					
//							SETUPALIAS.au16Speedmodify[l_u8Tmp1 + l_u8Tmp2] = SETUPALIAS.au16Speedmodify[l_u8Tmp1 + 5] + (SETUPALIAS.au16Speedmodify[l_u8Tmp1] - SETUPALIAS.au16Speedmodify[l_u8Tmp1+5])*(5 - l_u8Tmp2)/5;	
//						}
//					}
//					ToReDraw(l_u8ControlCode);
//					l_u8Status	= 0;
//					ClearKeyValue();
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
////			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDPointModify
** Descriptions:		点修正界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDPointModify(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Key;		//按键
//	uint32	l_u32Value;
//	char 	l_strOut[30]={0};	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	l_u32Value	= 0;
//	while(1)
//	{	
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTFC(0		, 0		, "请输入速度 1-%d:",MAX_SPEED_LIMIT - 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					l_u32Value	*= 10;
//					l_u32Value	+= l_u8Key;
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					if ((l_u32Value >0 && l_u32Value < MAX_SPEED_LIMIT))
//					{
//						sprintf(l_strOut,"点修正 %d km",l_u32Value);
//						UISetValueParamU16(l_strOut, &(SETUPALIAS.au16Speedmodify[l_u32Value]), 8000, 12000);
//					}
//					l_u32Value	= 0;
//					ToReDraw(l_u8ControlCode);
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			//刷新屏幕
//			if (l_u32Value == 0)
//				LCDPRINTC(144	, 0		, "         ");
//			else
//				LCDPRINTFC(144	, 0		, "%u    "	, l_u32Value);
//			
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDChooseVeh
** Descriptions:		选择车型界面，根据压缝位置不同，查看的数据不同
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDChooseVeh(uint8 p_u8Pos)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		switch(p_u8Pos)
//		{
//#if	IF3WB
//		case POS_ABS:	//AB
//			LCDPRINTC(0		, 0		, "AB板行驶车型速度点修正");
//			break;
//
//		case POS_BCS:	//BC
//			LCDPRINTC(0		, 0		, "BC板行驶车型速度点修正");
//			break;
//
//		case POS_ABCS:	//压缝
//			LCDPRINTC(0		, 0		, "压缝行驶车型速度点修正");
//			break;
//		
//		case POS_GAP:	//压缝整体
//			LCDPRINTC(0		, 0		, "车型压缝增益修正");
//			break;
//#endif
//		case POS_VEH:	//车型整体
//			LCDPRINTC(0		, 0		, "车型整体修正");
//			break;
//
//		case POS_2WB:	//两弯板
//			LCDPRINTC(0		, 0		, "车型速度修正");
//			break;
//
//		default:
//			ToBreak(l_u8ControlCode);
//			break;
//		}		
//		LCDPRINTC(8		, 24	, "1.11/12");
//		LCDPRINTC(8		, 44	, "2.13/14/15");
//		LCDPRINTC(8		, 64	, "3.112/122");
//		LCDPRINTC(8		, 84	, "4.113/115/114");
//		LCDPRINTC(8		, 104	, "5.123/124/125");
//		LCDPRINTC(124	, 24	, "6.126/127");
//		LCDPRINTC(124	, 44	, "7.155/135/153");
//		LCDPRINTC(124	, 64	, "8.157/156/1127");
//		//LCDPRINTC(124	, 84	, "9.车型修正");
//		LCDPRINTC(124	, 104	, "Esc. 退出设置 ");
//	
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//"1. 11/12"
//				case KB_2:		//"2. 13/14/15"
//				case KB_3:		//"3. 112/122"
//				case KB_4:		//"4. 113/115/114"
//				case KB_5:		//"5. 123/124/125"
//				case KB_6:		//"6. 126/127"
//				case KB_7:		//"7. 155/135/153"
//				case KB_8:		//"8. 157/156/1127"
//					ToReDraw(l_u8ControlCode);
//					switch(p_u8Pos)
//					{
//
//#if	IF3WB
//					case POS_ABS:	//AB
//					case POS_BCS:	//BC
//					case POS_ABCS:	//压缝
//#endif
//					case POS_2WB:	//两弯板
//						UIBDVSModifyParam(p_u8Pos, l_u8Key);
//						break;
//			
//					case POS_VEH:	//车型整体
//						sprintf(m_acTmp, "车型整体-车型%d",l_u8Key);  
//						UISetValueParamU16(&m_acTmp[0], &(SETUPALIAS.au16VehTotalModify[l_u8Key-1]), 8000, 12000);
//						break;
//#if	IF3WB			
//					case POS_GAP:	//压缝整体
//						sprintf(m_acTmp, "压缝整体-车型%d",l_u8Key);  
//						UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16GapModify[l_u8Key-1]), 8000, 12000);
//						break;
//#endif			
//					default:
//						ToBreak(l_u8ControlCode);
//						break;
//					}
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDVSModifyParam
** Descriptions:		参数值界面，根据压缝位置和车型
** input parameters:	p_u8Pos		位置
**						p_u8Veh		车型 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDVSModifyParam(uint8 p_u8Pos, uint8 p_u8Veh)
//{
//	uint8	l_u8ControlCode;		//控制指令
//	uint32	l_u32Value	= 0;		//输入数字
//	uint8	l_u8Key;
//	uint8	l_u8Status;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		switch(p_u8Pos)
//		{
//#if	IF3WB
//		case POS_ABS:	//AB
//			LCDPRINTFC(0		, 0		, "AB板车型速度修正-车型%d"	, p_u8Veh);
//			break;
//
//		case POS_BCS:	//BC
//			LCDPRINTFC(0		, 0		, "BC板车型速度修正-车型%d"	, p_u8Veh);
//			break;
//
//		case POS_ABCS:	//压缝
//			LCDPRINTFC(0		, 0		, "压缝车型速度修正-车型%d"	, p_u8Veh);
//			break;
//#endif
//		case POS_2WB:	//两弯板
//			LCDPRINTFC(0		, 0		, "车型速度修正-车型%d"		, p_u8Veh);
//			break;
//
//		default:
//			ToBreak(l_u8ControlCode);
//			break;
//		}		
//#if	IF3WB
//		LCDPRINTC(0		, 32	, "请输入速度(0-19): ");//20公里以内，编辑对应参数
//#else
//		LCDPRINTFC(0		, 32	, "请输入速度(0-%d): ",MAX_SPEED_LIMIT - 1 );//80公里以内，编辑对应参数
//#endif
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		l_u8Status	= 0xff;
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					l_u32Value	*= 10;
//					l_u32Value	+= l_u8Key;
//					NotReDraw(l_u8ControlCode);
//					l_u8Status	= 0;
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					if (l_u32Value == 0)
//						l_u8Status	= 0xff;
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					if (l_u8Status != 0xff)
//					{
//#if	IF3WB
//						if (l_u32Value < 20)	//20公里以内，编辑对应参数
//#else
//						if (l_u32Value < MAX_SPEED_LIMIT)	//80公里以内，编辑对应参数
//#endif
//						{
//							switch(p_u8Pos)
//							{
//#if	IF3WB		
//							case POS_ABS:	//AB
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "AB-车型%d-速度%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//					
//							case POS_BCS:	//BC
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "BC-车型%d-速度%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//					
//							case POS_ABCS:	//压缝
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "压缝-车型%d-速度%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//#else				
//							case POS_2WB:	//2弯板
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "车型%d-速度%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//#endif				
//							default:
//								ToBreak(l_u8ControlCode);
//								break;
//							}
//						}
//						l_u32Value	= 0;
//						ToReDraw(l_u8ControlCode);
//					}
//					else
//						NotReDraw(l_u8ControlCode);
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (l_u8Status == 0xff)
//				LCDPRINTC(144	, 32	, "         ");
//			else
//				LCDPRINTFC(144	, 32	, "%u       "	, l_u32Value);
//			
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UIBDChooseMotion
** Descriptions:		动态修正选择界面，点修or线修
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDChooseMotion(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " 动态修正");
//		
//		LCDPRINTC(176	, 0		, "Esc 退出");
//		
//		LCDPRINTC(8		, 24	, "1. 线修正");
//		LCDPRINTC(8		, 44	, "2. 点修正");
//	
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//线修正
//					ToReDraw(l_u8ControlCode);
//					UIBDLineModify();
//					break;	
//				
//				case KB_2:		//点修正
//					ToReDraw(l_u8ControlCode);
//					UIBDPointModify();
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDFullRange
** Descriptions:		设置最大量程 
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDFullRange(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "6. 量程设定           Esc-返回");
//		
//		LCDPRINTC(0		, 19	, "请选择最大量程:");
//
//		LCDPRINTC(64	, 40	, "1--10000 kg");
//		LCDPRINTC(64	, 56	, "2--15000 kg");
//		LCDPRINTC(64	, 72	, "3--20000 kg");
//		LCDPRINTC(64	, 88	, "4--30000 kg");
//		LCDPRINTC(64	, 104	, "5--35000 kg");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:
//					SETUPALIAS.u32Full	= 10000;
//					break;
//
//				case KB_2:
//					SETUPALIAS.u32Full	= 15000;
//					break;
//
//				case KB_3:
//					SETUPALIAS.u32Full	= 20000;
//					break;
//
//				case KB_4:
//					SETUPALIAS.u32Full	= 30000;
//					break;
//
//				case KB_5:
//					SETUPALIAS.u32Full	= 35000;
//					break;
//	
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (SETUPALIAS.u32Full == 0)
//				SETUPALIAS.u32Full	= 35000;
//			
//			LCDPRINTFC(128	, 19	, "%ukg      "	, SETUPALIAS.u32Full);
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDHeightTanxingti
** Descriptions:		弹性体高度
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wang ZiFeng		  
** Created Date:		20121107	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDHeightTanxingti(void)
//{
////	UISetEnableParam(" 弹性体高度" , &(SETUPALIAS.u8Genzong));
//	uint8 l_u8Key,l_u8ControlCode,l_u8status = 0x00;
////	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0,l_u32Value2 = 0;
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//画界面
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "弹性体高度设置:");
//		 GUI_Line(0		, 17	, 239	, 17	, 1);
//		 GUI_Line(0		, 127	, 239	, 127	, 1); 	   	 
//		 while(1)
//		 {
//		   	l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					if(l_u8status == 0x01)
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u8status = 0x00;	
//						l_u32Value = 0;
//						l_u32Value2 = 0;
//						ClearLCDLine(35);
//						ClearLCDLine(51);
//					}
//					else
//					{
//						ToBreak(l_u8ControlCode);					
//					}	   
//					ClearKeyValue();				
//					break;
//				case KB_ENTER:
//					ToReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00 && l_u32Value > 0)
//					{
//						l_u8status = 0x01;	
//					}
//					else if(l_u8status == 0x01 && l_u32Value2 > 0)
//					{
//					  SETUPALIAS.an8HeightTanxingti[l_u32Value - 1] = l_u32Value2;
//					  l_u32Value2 = 0;
//					  OSSemPost(g_pSemTask6Param);
//					}
//					ClearKeyValue();
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//						if(l_u8status == 0x00)	 //输入通道号
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //输入设定值	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00)
//						l_u32Value	/= 10;
//					else
//						l_u32Value2	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//			//		ClearKeyValue();
//					break;	
//			}
//
//			if(l_u8status == 0x00)
//			{
//				if(l_u32Value)
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "通道编号:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8HeightTanxingti[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "输入新值:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8HeightTanxingti[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "输入新值:      ");
//				}	
//			}
//			if(IfBreak(l_u8ControlCode) ||IfReDraw(l_u8ControlCode))
//				break;		
//		 }
//
//		 if(IfBreak(l_u8ControlCode))
//		 	break;  
//
//	} 	
//	
//}
/*********************************************************************************************************
** Function name:		UIBDPoDu
** Descriptions:		坡度修正界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDPoDu(void)
//{
////	UISetValueParamU16("坡度修正", &(SETUPALIAS.u16Podu), 9000, 11000);
//	uint8 l_u8Key,l_u8ControlCode,l_u8status = 0x00;
////	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0,l_u32Value2 = 0;
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//画界面
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "缓坡长度设置:");
//		 GUI_Line(0		, 17	, 239	, 17	, 1);
//		 GUI_Line(0		, 127	, 239	, 127	, 1); 	   	 
//		 while(1)
//		 {
//		   	l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					if(l_u8status == 0x01)
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u8status = 0x00;	
//						l_u32Value = 0;
//						l_u32Value2 = 0;
//						ClearLCDLine(35);
//						ClearLCDLine(51);
//					}
//					else
//					{
//						ToBreak(l_u8ControlCode);					
//					}	   
//					ClearKeyValue();				
//					break;
//				case KB_ENTER:
//					ToReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00 && l_u32Value > 0)
//					{
//						l_u8status = 0x01;	
//					}
//					else  if(l_u8status == 0x01)
//					{
//					  SETUPALIAS.an8LengthPodu[l_u32Value - 1] = l_u32Value2;
//					  if(SETUPALIAS.an8ZTStatus > 0)
//					  {
//						  speed1[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*100+15*POINTRATE/10)/(RootFloat((SETUPALIAS.an8ZTStatus[l_u32Value - 1]*POINTRATE/490)));
//						  speed2[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*100)/(RootFloat((SETUPALIAS.an8ZTStatus[l_u32Value - 1]*POINTRATE/490)));
//						  speed3[l_u32Value - 1] = speed2[l_u32Value - 1]/2;
//					  }
//					  else if(SETUPALIAS.an8ZTStatus == 0)
//					  {
//					  	  speed1[l_u32Value - 1] = 10000;
//						  speed2[l_u32Value - 1] = 10000;
//						  speed3[l_u32Value - 1] = 10000;
//					  }
//					  else
//					  {
//					  	  speed1[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*100+15*POINTRATE/10)/(RootFloat(((0-SETUPALIAS.an8ZTStatus[l_u32Value - 1])*POINTRATE/490)));
//						  speed2[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*100)/(RootFloat(((0-SETUPALIAS.an8ZTStatus[l_u32Value - 1])*POINTRATE/490)));
//						  speed3[l_u32Value - 1] = speed2[l_u32Value - 1]/2;
//					  }
//					  l_u32Value2 = 0;
//					  OSSemPost(g_pSemTask6Param);
//					}
//					ClearKeyValue();
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//						if(l_u8status == 0x00)	 //输入通道号
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //输入设定值	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00)
//						l_u32Value	/= 10;
//					else
//						l_u32Value2	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//			//		ClearKeyValue();
//					break;	
//			}
//
//			if(l_u8status == 0x00)
//			{
//				if(l_u32Value)
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "通道编号:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8LengthPodu[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "输入新值:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8LengthPodu[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "输入新值:      ");
//				}	
//			}
//			if(IfBreak(l_u8ControlCode) ||IfReDraw(l_u8ControlCode))
//				break;		
//		 }
//
//		 if(IfBreak(l_u8ControlCode))
//		 	break;  
//
//	} 
//}
/*********************************************************************************************************
** Function name:		UIBDChooseVehPos
** Descriptions:		车型速度修正位置选择
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDChooseVehPos(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " 车型修正");
//		
//		LCDPRINTC(176	, 0		, "Esc 退出");
//		LCDPRINTC(8		, 24	, "1. 车型整体修正");
//#if	IF3WB		
//		LCDPRINTC(8		, 40	, "2. AB板车型速度点修正");
//		LCDPRINTC(8		, 56	, "3. BC板车型速度点修正");	 
//		LCDPRINTC(8		, 72	, "4. 车型压缝修正");
//		LCDPRINTC(8		, 88	, "5. 压缝车型速度点修正");
//#else
//		LCDPRINTC(8		, 40	, "2. 车型速度点修正");
//#endif
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//				
//				case KB_1:		//车型整体修正
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_VEH);
//					break;	
//				
//				case KB_2:		//AB板车型速度修正
//					ToReDraw(l_u8ControlCode);
//#if	IF3WB
//					UIBDChooseVeh(POS_ABS);
//#else
//					UIBDChooseVeh(POS_2WB);
//#endif
//					break;
//						
//#if	IF3WB			
//				case KB_3:		//BC板车型速度修正
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_BCS);
//					break;	
//				
//				case KB_4:		//压缝车型速度修正
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_ABCS);
//					break;	
//				
//				case KB_5:		//车型压缝修正
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_GAP);
//					break;	
//#endif				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		SaveParams
** Descriptions:		保存参数设置
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SaveParams(uint8 num)
{
	uint8 l_au8Tmp[SETUPPARAMSIZE];	  
	uint8 l_u8temp,l_u8tempi,l_u8Err;
	uint16 l_u16SendLen;
	SETUPALIAS.u8Year		= g_DevCurTime.u16Year-2000;	// 取得年值
	SETUPALIAS.u8Month		= g_DevCurTime.u8Month;			// 取得月值
	SETUPALIAS.u8Day		= g_DevCurTime.u8Day;			// 取得日值

	  	
	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{ 	
		l_u8temp=ReadC256(SETUPPARAMADDR, l_au8Tmp, SETUPPARAMSIZE);
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 4);
	else
		g_u8RWNVFlag &=!(1 << 4);
	OSTimeDly(100);
//	WriteC256(HISTORYBDADDR, l_au8Tmp, SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
	
	AddCrc16((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2);
//	RamToFlash();

	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 0);
	else
		g_u8RWNVFlag &=!(1 << 0);

	AddCrc16((uint8 *)&PROMODPARAM, PROMODPARAMSIZE-2);	 
			   
 	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{			
		l_u8temp = WriteC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);	// 在0x00地址处写入32字节数据
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 1);
	else
		g_u8RWNVFlag &=!(1 << 1);
		#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nsaveparams,Num=%d,g_u8RWNVFlag=%d\r",
num,g_u8RWNVFlag);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
}
/*********************************************************************************************************
** Function name:		UIBDRoadPlaneHeight
** Descriptions:		保存参数设置
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDRoadPlaneHeight(void)
//{
//	uint8 l_u8Key,l_u8ControlCode,l_u8status = 0x00;
////	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0,l_u32Value2 = 0;
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//画界面
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "路面值高度设置:");
//		 GUI_Line(0		, 17	, 239	, 17	, 1);
//		 GUI_Line(0		, 127	, 239	, 127	, 1); 	   	 
//		 while(1)
//		 {
//		   	l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					if(l_u8status == 0x01)
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u8status = 0x00;	
//						l_u32Value = 0;
//						l_u32Value2 = 0;
//						ClearLCDLine(35);
//						ClearLCDLine(51);
//					}
//					else
//					{
//						ToBreak(l_u8ControlCode);					
//					}	   
//					ClearKeyValue();				
//					break;
//				case KB_ENTER:
//					ToReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00 && l_u32Value > 0)
//					{
//						l_u8status = 0x01;	
//					}
//					else  if(l_u8status == 0x01)
//					{
//					  SETUPALIAS.an8ZTStatus[l_u32Value - 1] = l_u32Value2;
//					  speed1[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*10+15*(POINTRATE/1000)*10)*1000/(RootFloat((SETUPALIAS.an8ZTStatus[l_u32Value - 1]*POINTRATE*10/49)));
//	  				  speed2[l_u32Value - 1] = (SETUPALIAS.an8LengthPodu[l_u32Value - 1]*10000)/(RootFloat((SETUPALIAS.an8ZTStatus[l_u32Value - 1]*POINTRATE*10/49)));
//	  				  speed3[l_u32Value - 1] = speed2[l_u32Value - 1]/2;
//					  l_u32Value2 = 0;
//					  OSSemPost(g_pSemTask6Param);
//					}
//					ClearKeyValue();
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//						if(l_u8status == 0x00)	 //输入通道号
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //输入设定值	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					if(l_u8status == 0x00)
//						l_u32Value	/= 10;
//					else
//						l_u32Value2	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//			//		ClearKeyValue();
//					break;	
//			}
//
//			if(l_u8status == 0x00)
//			{
//				if(l_u32Value)
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "通道编号:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8ZTStatus[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "输入新值:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "通道编号:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "当前设定值:%d     ",SETUPALIAS.an8ZTStatus[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "输入新值:      ");
//				}	
//			}
//			if(IfBreak(l_u8ControlCode) ||IfReDraw(l_u8ControlCode))
//				break;		
//		 }
//
//		 if(IfBreak(l_u8ControlCode))
//		 	break;  
//
//	} 
//}
/*********************************************************************************************************
** Function name:		UIBDWanBan
** Descriptions:		弯板动态标定通道界面
** input parameters:	p_u8CID		通道号 
**						p_u8Motion	动静态标志 0表示静态，1表示动态 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDWanBan(uint8 p_u8CID, uint8 p_u8Motion)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value;		//输入的数字
//	uint8	l_u8InputStatus;	//输入状态；1：
//	uint8	l_u8Key;
//	int32	l_n32ADTmp;
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		l_u8InputStatus	= 0;
//		l_u32Value	= 0;
//		
//		//画界面
//		GUI_ClearSCR();
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 108	, 239	, 108	, 1);				
//		
//		LCDPRINTC(0		, 0		, "1. 弯板标定           Esc-返回");
//		
//		LCDPRINTFC(8	, 19	, " %u通道   "	, p_u8CID);
//		
//		LCDPRINTC(16	, 40	, "初始零位:");
//		LCDPRINTFC(96	, 40	, "%d    "	, SETUPALIAS.an32Zero[p_u8CID-1]);
//		LCDPRINTC(16	, 56	, "当前内码:");
//		LCDPRINTC(16	, 72	, "当前重量:");
//		LCDPRINTC(16	, 88	, "增益系数:");
//		LCDPRINTFC(96	, 88	, "%d    "	, SETUPALIAS.an32AxGain[p_u8CID-1]);
//		if (p_u8Motion == UI_STATIC)
//			LCDPRINTC(0			, 112	, "F1清零  F2标定  F3增益  F4去皮");
//		else
//			LCDPRINTC(0			, 112	, "F1-清零  F2-标定  F3-输入系数");
//
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8InputStatus != 0)
//					{ 
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					if (l_u8InputStatus != 0)
//					{ 
//						l_u32Value	/= 10;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					if (l_u8InputStatus == KB_F3)	//输入增益系数
//					{
//						if (l_u32Value != 0)
//							SETUPALIAS.an32AxGain[p_u8CID-1]	= l_u32Value;
//					}
//					else if (l_u8InputStatus == KB_F2)		//输入标定重量
//					{
//						if (l_u32Value != 0)
//							SETUPALIAS.an32AxGain[p_u8CID-1]	= l_u32Value * 10000 / (g_an32MotionZero[p_u8CID-1] - SETUPALIAS.an32Zero[p_u8CID-1]);
//					}
//					ToReDraw(l_u8ControlCode);
//					l_u8InputStatus	= 0;
//					break;
//				
////				case KB_F1:			//F1，清零
////					if (l_u8InputStatus == 0)
////					{
////						SETUPALIAS.an32Zero[p_u8CID-1]	= ADAvg[p_u8CID-1];
////						ToReDraw(l_u8ControlCode);
////						
////						//更新动态零点
////						g_an32MotionZero[p_u8CID-1]	= SETUPALIAS.an32Zero[p_u8CID-1];
////					}
////					break;
//
////				case KB_F4:			//F4，静态去皮重
////					if ((l_u8InputStatus == 0) && (p_u8Motion == UI_STATIC))
////					{
////						SETUPALIAS.an32Zero[p_u8CID-1]	= ADAvg[p_u8CID-1];
////						ToReDraw(l_u8ControlCode);
////					}
////					else
////						NotReDraw(l_u8ControlCode);
////					break;
//
//				case KB_F2:			//F2，标定
//					if (l_u8InputStatus == 0)
//					{
//						l_u8InputStatus	= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;
//				
//				case KB_F3:			//F3，输入系数
//					if (l_u8InputStatus == 0)
//					{
//						l_u8InputStatus	= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//			{
//				break;
//			}
//
//			//刷新显示
////			l_n32ADTmp	= ADAvg[p_u8CID-1];
//			l_n32ADTmp = 0;
//			LCDPRINTFC(96		, 56	, "%d    "	, l_n32ADTmp);				//内码
//			
//			if (l_u8InputStatus != KB_F2)		//不是输入标定重量
//			{
//					LCDPRINTFC(96		, 72	, "%d kg    ", (l_n32ADTmp-SETUPALIAS.an32Zero[p_u8CID-1])*SETUPALIAS.an32AxGain[p_u8CID-1]/10000);		//重量
//			}
//			else		//输入标定重量
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(96		, 72	, "         ");
//				else
//					LCDPRINTFC(96		, 72	, "%u      ", l_u32Value);
//			}
//			
//			if (l_u8InputStatus == KB_F3)		//输入增益
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(96		, 88	, "         ");
//				else
//					LCDPRINTFC(96		, 88	, "%u      ", l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}


/*********************************************************************************************************
** Function name:		UIValidate
** Descriptions:		验证界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110614	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIValidate(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value;		//输入数字
//	uint32	l_u32SN;		//SN值
//	uint8	l_u8DigitCnt;	//数字个数
//	uint8	l_u8Tmp1;		//临时变量
//	uint8	l_u8Key;
//	uint32	l_u32SNTimer;	//由Timer生成的SN明码
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	l_u32SN	= 0;
//	l_u32SNTimer	= g_u32SN;
//		
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8DigitCnt= 0;
//		
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTFC(16		, 32	, "SN:%d", l_u32SNTimer);
//		LCDPRINTC(16		, 64	, "请输入口令: ");
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					NotReDraw(l_u8ControlCode);
//					l_u8DigitCnt++;
//					l_u32Value	*= 10;
//					l_u32Value	+= l_u8Key;
//					ClearKeyValue();
//					if (l_u8DigitCnt > 12)
//						return;
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					if (l_u8DigitCnt>0)
//						l_u8DigitCnt	--;
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;	
//				
//				case KB_ENTER:		//系统初始化
//					l_u32SN=bcd(l_u32SNTimer,6)*bcd(l_u32SNTimer,1)*10000+bcd(l_u32SNTimer,5)*bcd(l_u32SNTimer,2)*100+bcd(l_u32SNTimer,4)*bcd(l_u32SNTimer,3) ;
//					l_u32SN=(bcd(l_u32SN,1)<<20)+(bcd(l_u32SN,2)<<16)+(bcd(l_u32SN,3)<<12)+(bcd(l_u32SN,4)<<8)+(bcd(l_u32SN,5)<<4)+bcd(l_u32SN,6);
//					
//					if (l_u32SN == l_u32Value || l_u32Value == SUPERPWD)
//					{
//						UISystemInit();		 						
//						ToBreak(l_u8ControlCode);
//					}
//					else
//					{
//						ToReDraw(l_u8ControlCode);
//					}
//					l_u32Value	= 0;
//					ClearKeyValue();
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			LCDPRINTC(112 + (l_u8DigitCnt<<3) 	, 64	, "  ");
//
//			for (l_u8Tmp1 = 0; l_u8Tmp1 < l_u8DigitCnt; l_u8Tmp1 ++)
//			{
//				LCDPRINTC(112 + (l_u8Tmp1<<3) 	, 64	, "*");
//			}
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		UISystemInit
** Descriptions:		系统初始化界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISystemInit(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8IfPwd;		//是否为第二次输入的密码666666
//	uint32	l_u32Value;		//输入数字
//	uint8	l_u8Operation;	//初始化操作类型
//	uint8	l_u8Tmp1;		//临时变量，循环用
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	while(1)
//	{
//		l_u8IfPwd	= 0;
//		l_u32Value	= 0;
//		l_u8Operation	= 0;
//		//画界面
//		GUI_ClearSCR();
//		
//		LCDPRINTC(0		, 0		, "1. 初始化为系统默认参数");
//		LCDPRINTC(0		, 16	, "2. 保留调校参数并初始化");
//		LCDPRINTC(0		, 32	, "3. 动态修正初始化");
//		LCDPRINTC(0		, 48	, "4. 车型修正初始化");
//		LCDPRINTC(0		, 64	, "5. 静态修正初始化");
//		LCDPRINTC(0		, 80	, "6. 恢复至最近标定参数");
//		LCDPRINTC(0		, 96	, "7. 恢复至历史标定参数");
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//初始化为系统默认参数
//				case KB_2:		//保留调校参数并初始化
//				case KB_3:		//动态修正初始化
//				case KB_4:		//车型修正初始化
//				case KB_5:		//静态修正初始化
//				case KB_6:		//恢复至最近标定参数
//				case KB_7:		//恢复至历史标定参数
//					if (l_u8IfPwd == 0)
//					{
//						l_u8Operation	= l_u8Key;
//						for (l_u8Tmp1 = 0; l_u8Tmp1 < 8; l_u8Tmp1 ++)
//						{
//							if (l_u8Tmp1 != l_u8Operation-1)
//								ClearLCDLine(l_u8Tmp1<<4);
//						}
//						LCDPRINTC(16 	, 112	, "请输入口令:");
//					}
//					else
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//						LCDPRINTC(112 + (l_u8IfPwd<<3) 	, 112	, "*");
//					}
//					l_u8IfPwd	++;		
//					break;	
//				
//				case KB_8:
//				case KB_9:
//				case KB_0:
//					if (l_u8IfPwd == 0)
//					{
//						ToBreak(l_u8ControlCode);
//					}
//					else
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//						LCDPRINTC(112 + (l_u8IfPwd<<3) 	, 112	, "*");
//						l_u8IfPwd ++;
//					}					
//					break;	
//				
//				case KB_ENTER:			//确认
//					NotReDraw(l_u8ControlCode);
//					l_u8IfPwd	= 0xf0;	//确认后，判断密码是否正确
//					break;
//
//				case 0xff:
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				default:		//其他按键，退出
//					ToBreak(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			//刷新
//			if(l_u8IfPwd == 20)	//输入20个字符密码,则退出
//			{
//				ToBreak(l_u8ControlCode);
//				break;
//			}
//			
//			if (l_u8IfPwd == 0xf0)	//输入密码完毕
//			{
//				if (l_u32Value == 666666)		//密码正确
//				{
//					SETUPALIAS.u8DiaoDianTag	= 0;		//取消掉电保护标记
//	
//					switch (l_u8Operation)
//					{
//						case 1:
//							InitSystem();
//							break;
//
//						case 2:
//							InitNonWeight();
//							break;
//						case 3:
//							InitMotionModify();
//							break;
//
//						case 4:
//							InitVehModify();
//							break;
//
//						case 5:
//							InitStaticModify();
//							break;
//
//						case 6:
//							if (RecoverToLast())	//成功
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "最近参数恢复成功!");
//								//OSTimeDly(100);	   
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							else
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "最近参数恢复失败!");
//								//OSTimeDly(100);  
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							break;
//
//						case 7:
//							if (RecoverToHistory())	//成功
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "历史参数恢复成功!");
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							else
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "历史参数恢复失败!");
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							break;
//
//						default:
//							break;
//					}
//					GUI_ClearSCR();
//					LCDPRINTC(40 	, 56	, "初始化完成, 重新启动...  ");
//					InitRestart();
//					OSTimeDly(OS_TICKS_PER_SEC);
//				}
//				ToBreak(l_u8ControlCode);
//				break;	
//			}
//			
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		InitSystem
** Descriptions:		系统初始化
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void InitSystem(uint8 num)
{
	uint8 l_u8Err,l_u8tempi,l_u8temp;
	uint16		l_u16SendLen,l_u16Tmp,l_u16Tmp2,l_u16Tmp3;
//	uint16 *	l_pu16Tmp;	
//	if (CheckCrc((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2) != 0)
//	{
//		WriteC256(HISTORYBDADDR,(uint8 *)&SETUPALIAS, SETUPPARAMSIZE);
//	}

	SETUPALIAS.u8ProgVersion[0]		= PROGVERSION_POS_0;
	SETUPALIAS.u8ProgVersion[1]		= PROGVERSION_POS_1;
	SETUPALIAS.u8ProgVersion[2]		= PROGVERSION_POS_2;
	SETUPALIAS.u8ProgVersion[3]		= PROGVERSION_POS_3;

	SETUPALIAS.u8Version			= YBVERSION;
	SETUPALIAS.u8Year				= 12;
	SETUPALIAS.u8Month				= 1;
	SETUPALIAS.u8Day				= 1;
	SETUPALIAS.u8Protocol			= PROTOL_WJ_NEW;
	SETUPALIAS.u8StaticScale		= 20;
	SETUPALIAS.u8MotionScale		= 100;
	SETUPALIAS.u8DiaoDianTag		= DISABLE_CONFIG;
	//SETUPALIAS.u8DOG				= DISABLE_CONFIG; 
	SETUPALIAS.u8DOG				= ENABLE_CONFIG; 
	SETUPALIAS.u8SendWaveEnable		= DISABLE_CONFIG;
	SETUPALIAS.u8LoopTriggerEnable	= DISABLE_CONFIG;
	SETUPALIAS.u8ComMode			= COM_MODE1;	
	//
	SETUPALIAS.u8SendF4CodeEnable	= DISABLE_CONFIG;
    SETUPALIAS.u8BaudRate			= 5;
	//
	SETUPALIAS.u8VehicleBufSize		= 0;
	//
	SETUPALIAS.u8UPPoints		=	DEFUPPOINT ;
	SETUPALIAS.u8DownPoints		=	DEFDNPOINT ;

	//wzf 2012-08-24 新增初始值
	for(l_u16Tmp = 0; l_u16Tmp < CHANNELNUM; l_u16Tmp++)
	{
// 		for(l_u16Tmp2 = 0; l_u16Tmp2 < 16; l_u16Tmp2++)
// 		{
// //		 	SETUPALIAS.au16StaticModify[l_u16Tmp][l_u16Tmp2]	= 10000;
// 		}
		SETUPALIAS.u8Diszt[l_u16Tmp] = 30;

		
		SETUPALIAS.an8HeightTanxingti[l_u16Tmp] = 10;
		SETUPALIAS.an8ZTStatus[l_u16Tmp] = 1; 
		SETUPALIAS.an8LengthPodu[l_u16Tmp] = 1;
		SETUPALIAS.an32Zero[l_u16Tmp] = 10000; 
		SETUPALIAS.an32AxGain[l_u16Tmp] = 750; 
		SETUPALIAS.an16BDAxGain[l_u16Tmp] = 750;		
	}	

	SETUPALIAS.u16DistBetween[0] = 500;
	SETUPALIAS.u16DistBetween[1] = 500;
	SETUPALIAS.u16DistBetween[2] = 200;
	 
  	for (l_u16Tmp = 0; l_u16Tmp < CHANNALLINESUM; l_u16Tmp ++)	
	{
   	   SETUPALIAS.u16UseToWghFlag[l_u16Tmp] = 0xFFFF;//20130515 浩叔发现bug	
	}
		
	//
	SETUPALIAS.u16UpValue		= 	UPNUM;
	SETUPALIAS.u16DownValue		= 	DNNUM;	 
	SETUPALIAS.u16logicUpValue	=	DEFLUPVALUE;
	SETUPALIAS.u16logicDownValue	= 	DEFLDNVALUE;
	//
	SETUPALIAS.u16PlatformGain	 	= 10000;
//	SETUPALIAS.u16noUsed1 = 0;
	for(l_u16Tmp = 0; l_u16Tmp < 64; l_u16Tmp++)
	{
		SETUPALIAS.u8ArrayForZT[l_u16Tmp] = l_u16Tmp;
	}
	
	for(l_u16Tmp = 0; l_u16Tmp < 10; l_u16Tmp++)
	{
	 	for (l_u16Tmp2= 0; l_u16Tmp2 < 20; l_u16Tmp2 ++)
	  		SETUPALIAS.au16Speedmodify[l_u16Tmp][l_u16Tmp2]	= 10000;
	}
	//
//	SETUPALIAS.u16noUsed2 = 0;
	for (l_u16Tmp = 0; l_u16Tmp < 10; l_u16Tmp ++)
	{
		for (l_u16Tmp2= 0; l_u16Tmp2 < 10; l_u16Tmp2 ++)
		{
			SETUPALIAS.au16VehTotalModify[l_u16Tmp][l_u16Tmp2] = 10000;
		}
	}
	//
	for (l_u16Tmp = 0; l_u16Tmp < 10; l_u16Tmp ++)	
	{
	 	for (l_u16Tmp2= 0; l_u16Tmp2 < 10; l_u16Tmp2 ++)	
		{
		  	for (l_u16Tmp3= 0; l_u16Tmp3 < 20; l_u16Tmp3 ++)
		  		SETUPALIAS.au16VehSpeedModify[l_u16Tmp][l_u16Tmp2][l_u16Tmp3] = VSMSTANDARD;
		}
	}
	//
 	SETUPALIAS.u32Full				= 35000;  		 
  SETUPALIAS.u32Password			= SUPERPWD;
	//
//	FDistBetwInit();
	memset( SETUPALIAS.m_an8HeartBeatEn, 0, NET_PORT_NUM );	  
	 
   	//
	memset(SETUPALIAS.au8ReserveByte,0,RESERVED);
	
	AddCrc16((uint8 *)&SETUPALIAS, SETUPPARAMSIZE-2);	 

	InitNetParam(0);
	//
	//ClearStartupCnt();			   
 //	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 0);
	else
		g_u8RWNVFlag &=!(1 << 0);
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nInitSystem,Num=%d,g_u8RWNVFlag=%d\r",num,g_u8RWNVFlag
					          );
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
}

/*********************************************************************************************************
** Function name:		InitNonWeight
** Descriptions:		初始化非称重设置
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void InitNonWeight(void)
{
	uint8 l_u8Err,l_u8tempi,l_u8temp;
	uint16 l_u16Tmp,l_u16SendLen;
	SETUPALIAS.u32Password			= SUPERPWD;
	SETUPALIAS.u32Full				= 35000;
	for (l_u16Tmp = 0; l_u16Tmp < CHANNELNUM; l_u16Tmp ++)
		SETUPALIAS.an8LengthPodu[l_u16Tmp] = 10;

	SETUPALIAS.u8StaticScale		= 20;
	SETUPALIAS.u8MotionScale		= 100;
//	SETUPALIAS.u8Genzong			= ENABLE_CONFIG;
	SETUPALIAS.u8ComMode			= COM_MODE1;
	SETUPALIAS.u8BaudRate			= 5;
	SETUPALIAS.u8DOG				= 1; 
	SETUPALIAS.u8Year				= 0;
	SETUPALIAS.u8Month				= 0;
	SETUPALIAS.u8Day				= 0;
//	SETUPALIAS.u8PlatWidth			= 75;
	
//	SETUPALIAS.u8FangxiangEnable	= 1;
	SETUPALIAS.u8DiaoDianTag		= 0;
//	SETUPALIAS.u8ZhuapaiEnable		= 0;
	SETUPALIAS.u8LoopTriggerEnable	= 0;

	SETUPALIAS.u8VehicleBufSize		= 0;

//	SETUPALIAS.u8LunZhouERR			= 0x03;
	SETUPALIAS.u8SendWaveEnable		= 1;
	
	AddCrc16((uint8 *)&SETUPALIAS, sizeof(SETUPALIAS) - 2);

//	RamToFlash();
	InitThreshold();

	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 0);
	else
		g_u8RWNVFlag &=!(1 << 0);
//	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nInitNonWeight,g_u8RWNVFlag=%d.",g_u8RWNVFlag
					          );
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
}
/*********************************************************************************************************
** Function name:		InitThreshold
** Descriptions:		初始化阈值参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110513	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void InitThreshold(void)
{
	uint8 l_u8Err,l_u8tempi,l_u8temp;
	uint16 l_u16SendLen;
	SETUPALIAS.u16UpValue		= 	UPNUM;
	SETUPALIAS.u16DownValue		= 	DNNUM;

	SETUPALIAS.u8UPPoints		=	DEFUPPOINT ;
	SETUPALIAS.u8DownPoints		=	DEFDNPOINT ;	 
	SETUPALIAS.u16logicUpValue	=	DEFLUPVALUE;
	SETUPALIAS.u16logicDownValue	= 	DEFLDNVALUE;	

	AddCrc16((uint8 *)&SETUPALIAS, sizeof(SETUPALIAS)-2);
	//WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS, sizeof(SETUPALIAS));
		for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,sizeof(SETUPALIAS));	// 在0x00地址处写入32字节数据
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 0);
	else
		g_u8RWNVFlag &=!(1 << 0);
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nInitThreshold,g_u8RWNVFlag=%d\r",g_u8RWNVFlag
					          );
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
	
	
}
/*********************************************************************************************************
** Function name:		InitMotionModify
** Descriptions:		初始化动态修正参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void InitMotionModify(void)
//{
//	uint16		l_u16Tmp;
//
//	for(l_u16Tmp = 0; l_u16Tmp < MAX_SPEED_LIMIT; l_u16Tmp++)
//	{
//	  SETUPALIAS.au16Speedmodify[l_u16Tmp]	= 10000;
//	}
//	
//	AddCrc16((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2);
//
////	RamToFlash();
//	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据	
//}

/*********************************************************************************************************
** Function name:		InitVehModify
** Descriptions:		初始化车型修正参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void InitVehModify(void)
//{
//	uint16		l_u16Tmp;
//	uint16 *	l_pu16Tmp;
//
//	l_pu16Tmp	= &(SETUPALIAS.au16VehTotalModify[0]);
//	for (l_u16Tmp = 0; l_u16Tmp < 10; l_u16Tmp ++)
//		*(l_pu16Tmp++)	= 10000;
//
//	l_pu16Tmp	= &(SETUPALIAS.au16VehSpeedModify[0][0]);
//	for (l_u16Tmp = 0; l_u16Tmp < 800; l_u16Tmp ++)	   //wzf 2012-09-03原来320，修改成800
//		*(l_pu16Tmp++)	= 10000;
//
//	AddCrc16((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2);
//
////	RamToFlash();
// 	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
//}
/*********************************************************************************************************
** Function name:		InitStaticModify
** Descriptions:		初始化静态修正参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void InitStaticModify(void)
//{
////	uint16		l_u16Tmp,l_u16Tmp2;
//
//// for(l_u16Tmp2 = 0;l_u16Tmp2 < CHANNELNUM;l_u16Tmp2++)
//// {
//// 	for(l_u16Tmp = 0; l_u16Tmp < 16; l_u16Tmp++)
////	{ 
//////		SETUPALIAS.au16StaticModify[l_u16Tmp2][l_u16Tmp]	= 10000; 
////	}	
//// }	
////	AddCrc16((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2);
//
////	RamToFlash();
////	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
//}
/*********************************************************************************************************
** Function name:		RecoverToLast
** Descriptions:		恢复为最近标定参数
** input parameters:	None 
** output parameters:	none
** Return Value:		uint8	是否成功：1，成功     0，失败
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 RecoverToLast(void)
//{
//	ReadC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);
//	OSTimeDly(100);	
//	if(CheckCrc((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2)==0)		//失败
//	{
//		return	0;
//	}
//	else	//成功
//	{
//	//	RamToFlash();
//		return	1;
//	}
//}

/*********************************************************************************************************
** Function name:		RecoverToLast
** Descriptions:		恢复为历史标定参数
** input parameters:	None 
** output parameters:	none
** Return Value:		uint8	是否成功：1，成功     0，失败
** Created by:			ZHANG Ye		  
** Created Date:		20110510	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//uint8 RecoverToHistory(void)
//{
//	ReadC256(HISTORYBDADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);
//	OSTimeDly(100);	
//	if(CheckCrc((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2)==0)		//失败
//	{
//		return	0;
//	}
//	else	//成功
//	{
//	//	RamToFlash();
//		WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// 在0x00地址处写入32字节数据
//		return	1;
//	}
//}

/*********************************************************************************************************
** Function name:		InitNetParam
** Descriptions:		初始化网络参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110623	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void InitNetParam(uint8 num)
{
	IPINFOALIAS.au8SubMask[0]		= 0xff;
	IPINFOALIAS.au8SubMask[1]		= 0xff;
	IPINFOALIAS.au8SubMask[2]		= 0xff;
	IPINFOALIAS.au8SubMask[3]		= 0x00;
	
	IPINFOALIAS.au8GatewayIP[0]		= 192;
	IPINFOALIAS.au8GatewayIP[1]		= 168;
	IPINFOALIAS.au8GatewayIP[2]		= 1;
	IPINFOALIAS.au8GatewayIP[3]		= 1;   

	IPINFOALIAS.au8IPAddr[0]		= 192;
	IPINFOALIAS.au8IPAddr[1]		= 168;
	IPINFOALIAS.au8IPAddr[2]		= 1;
	//IPINFOALIAS.au8IPAddr[3]		= 119;
	IPINFOALIAS.au8IPAddr[3]		= 118;
	IPINFOALIAS.u32LocalPortNO		= 5000;	 

	IPINFOALIAS.au8ServerIP[0]		= 192;
	IPINFOALIAS.au8ServerIP[1]		= 168;
	IPINFOALIAS.au8ServerIP[2]		= 1;
	IPINFOALIAS.au8ServerIP[3]		= 155;								   
	IPINFOALIAS.u32ServerPortNO		= 3136;
	
	IPINFOALIAS.au8MACAddr[0]		= 0x52;
	IPINFOALIAS.au8MACAddr[1]		= 0x54;
	IPINFOALIAS.au8MACAddr[2]		= 0x4c;
	IPINFOALIAS.au8MACAddr[3]		= 0x19;
	IPINFOALIAS.au8MACAddr[4]		= 0xf7;
	//IPINFOALIAS.au8MACAddr[5]		= 0x49;
	IPINFOALIAS.au8MACAddr[5]		= 0x48;
	
	//
	IPINFOALIAS.m_u8ProgVersion[0]		= PROGVERSION_POS_0;
	IPINFOALIAS.m_u8ProgVersion[1]		= PROGVERSION_POS_1;
	IPINFOALIAS.m_u8ProgVersion[2]		= PROGVERSION_POS_2;
	IPINFOALIAS.m_u8ProgVersion[3]		= PROGVERSION_POS_3;
	//
	SaveNetInfo(num);
}
/*********************************************************************************************************
** Function name:		SaveNetInfo
** Descriptions:		保存网络参数
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110624	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SaveNetInfo(uint8 num)
{
	uint8 l_u8Err,l_u8tempi,l_u8temp;
	uint16 l_u16SendLen;
	CRCFunc16((uint8 *)&IPINFOALIAS, sizeof(IPINFOALIAS)-2);
	//WriteC256(NETINFOADDR,(uint8 *)&IPINFOALIAS, sizeof(IPINFOALIAS));
		for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp=WriteC256(NETINFOADDR,(uint8 *)&IPINFOALIAS, sizeof(IPINFOALIAS));
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 2);
	else
		g_u8RWNVFlag &=!(1 << 2);
		#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nSaveNetInfo,Num=%d,g_u8RWNVFlag=%d\r",
					          num,g_u8RWNVFlag);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
}
/*********************************************************************************************************
** Function name:		InitRestart
** Descriptions:		初始化后重启
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110516	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void InitRestart(void)
//{
////	InitVehBufIndex();	 
//	YBRestart();
//}

/*********************************************************************************************************
** Function name:		ClearStartupCnt
** Descriptions:		启动次数清零
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110517	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ClearStartupCnt(void)
{
	g_u32StartupTime	= 0;
	WriteC256(STARTTIMESADDR,(uint8 *)&g_u32StartupTime, 4);
}
/*********************************************************************************************************
** Function name:		AddStartupCnt
** Descriptions:		启动次数累加
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110517	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void AddStartupCnt(void)
{  
	uint16	l_u16RecIndex;
	uint8	l_au8RecordData[8];	  
	//加1
	ReadC256(STARTTIMESADDR, (uint8 *)&g_u32StartupTime, 4);
	OSTimeDly(10);
	g_u32StartupTime += 1;
	WriteC256(STARTTIMESADDR, (uint8 *)&g_u32StartupTime, 4);

	//记录时间
	ReadC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//找到有效记录号
	OSTimeDly(10);
	l_u16RecIndex	+= 1;
	l_u16RecIndex	&= 0x3ff;		//0~1023
	WriteC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//更新记录号
	
	if ((l_u16RecIndex & 0x1ff) == 0)		//写满一个Sector
	{
		EraseSector(NORSTARTREC	+ (l_u16RecIndex << 3));
	}
	
	//生成记录
	l_au8RecordData[0]	= g_DevCurTime.u16Year & 0xff;	//年
	l_au8RecordData[1]	= g_DevCurTime.u16Year >> 8;
	l_au8RecordData[2]	= g_DevCurTime.u8Month;			//月
	l_au8RecordData[3]	= g_DevCurTime.u8Day;			//日
	l_au8RecordData[4]	= g_DevCurTime.u8Hour;			//时
	l_au8RecordData[5]	= g_DevCurTime.u8Minute;		//分
	l_au8RecordData[6]	= g_DevCurTime.u8Second;		//秒
	l_au8RecordData[7]	= 0xaa;
	//
	WriteNORFlash(NORSTARTREC + (l_u16RecIndex<<3), 8, &l_au8RecordData[0]);	

}
/*********************************************************************************************************
** Function name:		YBRestart
** Descriptions:		重启
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20111117	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void YBRestart(void)
{  
	WDTInit();
	WDTIM_COUNTER	= 0x100000;				/* 喂狗	*/ 
}

/*********************************************************************************************************
** Function name:		UISN
** Descriptions:		SN界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISN(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value;		//输入数字
//	uint32	l_u32SN;		//SN值
//	uint8	l_u8DigitCnt;	//数字个数
//	uint8	l_u8Tmp1;		//临时变量
//	uint8	l_u8Key;					  
//	uint32	l_u32SNTimer;	//由Timer生成的SN明码
//	uint8	l_u8SendVeh;	//是否发送车辆
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	BackGroundSave();
//	BackGroundON();
//
//	l_u32SN	= 0;
////	g_u8EnableClearZero	= 0;	//不需要清零
//	l_u32SNTimer	= g_u32SN;
//	
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8DigitCnt= 0;
//		
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTFC(16		, 32	, "SN: %d", l_u32SNTimer);
//		LCDPRINTC(16		, 64	, "请输入口令: ");
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					NotReDraw(l_u8ControlCode);
//					l_u8DigitCnt++;
//					l_u32Value	*= 10;
//					l_u32Value	+= l_u8Key;
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					if (l_u8DigitCnt>0)
//						l_u8DigitCnt	--;
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_ENTER:		//系统初始化
//					l_u32SN=bcd(l_u32SNTimer,6)*bcd(l_u32SNTimer,1)*10000+bcd(l_u32SNTimer,5)*bcd(l_u32SNTimer,2)*100+bcd(l_u32SNTimer,4)*bcd(l_u32SNTimer,3) ;
//					l_u32SN=(bcd(l_u32SN,1)<<20)+(bcd(l_u32SN,2)<<16)+(bcd(l_u32SN,3)<<12)+(bcd(l_u32SN,4)<<8)+(bcd(l_u32SN,5)<<4)+bcd(l_u32SN,6);
//					
//					if (l_u32SN == l_u32Value || l_u32Value == SUPERPWD)
//					{
//						UIBDRoot();
//						ToBreak(l_u8ControlCode);
//						break;
//					}
//					l_u8SendVeh	= 0;
//					switch(l_u32Value)
//					{
//						case 111:
//							UICommonSet();
//							break;
//						
//						case 222:
//							UIViewSetting();
//							break;
//								
//						case 333:
//							UIViewModify();
//							break;
//						
//						case 888:
//							UIViewAuthor();
//							break;
//
////						case 555:
////							UISetRegion();
////							break;
////#if	NETACCESS > 0	//网口传输数据						
//						case 444:
//							UIViewIPInfo();
//							break;
////#endif
//						case 8494:
//							UIViewStartUpTime();
//							break;
//
//						case 8968:
//							UIViewThreshold();
//							break; 
//						
//						//测试发送虚拟数据，9xx
//						case 911:	//发送11车数据
//							g_sviVehicleInfo.u8AixCnt			= 2;
//							g_sviVehicleInfo.u8AixGrpCnt		= 2;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 120;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 160;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 120;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 160;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 25;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 25;   
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 1;    
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x01;							
//							g_sviVehicleInfo.au16AxleDistance[0]	= 250;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8;  							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN;
//							l_u8SendVeh	= 1;
//							break;
//						
//						case 912:	//发送12车数据
//							g_sviVehicleInfo.u8AixCnt			= 2;
//							g_sviVehicleInfo.u8AixGrpCnt		= 2;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 320;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 760;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 320;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 760;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 5;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 5;   
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 2;    
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x12;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 560;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//
//						case 915:	//发送15车数据
//							g_sviVehicleInfo.u8AixCnt			= 3;
//							g_sviVehicleInfo.u8AixGrpCnt		= 2;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 520;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 1060;		
//							g_sviVehicleInfo.au16AxleWeight[2]		= 1160;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 520;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 2220;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 7;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 8;  	
////							g_sviVehicleInfo.au16AxleSpeed[2]		= 9;  
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 5;    
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x35;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 460;	
//							g_sviVehicleInfo.au16AxleDistance[1]	= 130;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[2] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//
//						case 9115:	//发送115车数据
//							g_sviVehicleInfo.u8AixCnt			= 4;
//							g_sviVehicleInfo.u8AixGrpCnt		= 3;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 520;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 1020;	
//							g_sviVehicleInfo.au16AxleWeight[2]		= 1360;		
//							g_sviVehicleInfo.au16AxleWeight[3]		= 1460;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 520;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 1020;	
//							g_sviVehicleInfo.au16AxleGrpWeight[2]	= 2820;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 7;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 7;  	
////							g_sviVehicleInfo.au16AxleSpeed[2]		= 6;   	
////							g_sviVehicleInfo.au16AxleSpeed[3]		= 6; 
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 1; 	
//							g_sviVehicleInfo.au8AxleGrpType[2]			= 5;   
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[2]		= 0x35;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 160;	
//							g_sviVehicleInfo.au16AxleDistance[1]	= 430; 	
//							g_sviVehicleInfo.au16AxleDistance[2]	= 130;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[2] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[3] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//						
//						case 9114:	//发送114车数据
//							g_sviVehicleInfo.u8AixCnt			= 4;
//							g_sviVehicleInfo.u8AixGrpCnt		= 3;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 520;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 1020;	
//							g_sviVehicleInfo.au16AxleWeight[2]		= 1360;		
//							g_sviVehicleInfo.au16AxleWeight[3]		= 2460;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 520;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 1020;	
//							g_sviVehicleInfo.au16AxleGrpWeight[2]	= 3820;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 7;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 7;  	
////							g_sviVehicleInfo.au16AxleSpeed[2]		= 6;   	
////							g_sviVehicleInfo.au16AxleSpeed[3]		= 7; 
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 1; 	
//							g_sviVehicleInfo.au8AxleGrpType[2]			= 4;   
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[2]		= 0x14;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 160;	
//							g_sviVehicleInfo.au16AxleDistance[1]	= 430; 	
//							g_sviVehicleInfo.au16AxleDistance[2]	= 130;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[2] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[3] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//
//						case 9129:	//发送129车数据
//							g_sviVehicleInfo.u8AixCnt			= 5;
//							g_sviVehicleInfo.u8AixGrpCnt		= 3;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 520;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 2020;	
//							g_sviVehicleInfo.au16AxleWeight[2]		= 960;		
//							g_sviVehicleInfo.au16AxleWeight[3]		= 960; 		
//							g_sviVehicleInfo.au16AxleWeight[4]		= 980;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 520;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 2020;	
//							g_sviVehicleInfo.au16AxleGrpWeight[2]	= 2900;
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 7;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 8;  	
////							g_sviVehicleInfo.au16AxleSpeed[2]		= 9;   	
////							g_sviVehicleInfo.au16AxleSpeed[3]		= 9;    	
////							g_sviVehicleInfo.au16AxleSpeed[4]		= 9;
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 2; 	
//							g_sviVehicleInfo.au8AxleGrpType[2]			= 7;   
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x12;	
//							g_sviVehicleInfo.au8AxleTAIType[2]		= 0x79;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 230;	
//							g_sviVehicleInfo.au16AxleDistance[1]	= 630; 	
//							g_sviVehicleInfo.au16AxleDistance[2]	= 130; 	
//							g_sviVehicleInfo.au16AxleDistance[3]	= 130;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[2] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[3] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[4] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//
//						case 9159:	//发送159车数据
//							g_sviVehicleInfo.u8AixCnt			= 6;
//							g_sviVehicleInfo.u8AixGrpCnt		= 3;
//							g_sviVehicleInfo.au16AxleWeight[0]		= 520;	
//							g_sviVehicleInfo.au16AxleWeight[1]		= 2020;	
//							g_sviVehicleInfo.au16AxleWeight[2]		= 2000;	
//							g_sviVehicleInfo.au16AxleWeight[3]		= 1460;		
//							g_sviVehicleInfo.au16AxleWeight[4]		= 1480; 		
//							g_sviVehicleInfo.au16AxleWeight[5]		= 1380;
//							g_sviVehicleInfo.au16AxleGrpWeight[0]	= 520;	
//							g_sviVehicleInfo.au16AxleGrpWeight[1]	= 4020;	
//							g_sviVehicleInfo.au16AxleGrpWeight[2]	= 4320;	 
////							g_sviVehicleInfo.au16AxleSpeed[0]		= 6;	
////							g_sviVehicleInfo.au16AxleSpeed[1]		= 5;  	
////							g_sviVehicleInfo.au16AxleSpeed[2]		= 5;   	
////							g_sviVehicleInfo.au16AxleSpeed[3]		= 5;    	
////							g_sviVehicleInfo.au16AxleSpeed[4]		= 5;    	
////							g_sviVehicleInfo.au16AxleSpeed[5]		= 5;
//							g_sviVehicleInfo.au8AxleGrpType[0]			= 1;	
//							g_sviVehicleInfo.au8AxleGrpType[1]			= 5; 	
//							g_sviVehicleInfo.au8AxleGrpType[2]			= 7;   
//							g_sviVehicleInfo.au8AxleTAIType[0]		= 0x01;	
//							g_sviVehicleInfo.au8AxleTAIType[1]		= 0x35;	
//							g_sviVehicleInfo.au8AxleTAIType[2]		= 0x79;
//							g_sviVehicleInfo.au16AxleDistance[0]	= 230;	
//							g_sviVehicleInfo.au16AxleDistance[1]	= 130; 	
//							g_sviVehicleInfo.au16AxleDistance[2]	= 530;	
//							g_sviVehicleInfo.au16AxleDistance[3]	= 130; 	
//							g_sviVehicleInfo.au16AxleDistance[4]	= 130;	
//							g_sviVehicleInfo.au16AxleDistance[5]	= 130;
//
//							g_AxleInfo[AXLE_BUFLEN].u16ChListSum = 8; 
//							g_AxleInfo[AXLE_BUFLEN+1].u16ChListSum = 8; 							
//							for(l_u8Tmp1 = 0;l_u8Tmp1<8;l_u8Tmp1++)
//							{
//								g_AxleInfo[AXLE_BUFLEN].u16ChList[l_u8Tmp1] = CHL_BUFLEN;	
//								g_AxleInfo[AXLE_BUFLEN+1].u16ChList[l_u8Tmp1] = CHL_BUFLEN+1;								
//							}  							
//							g_sviVehicleInfo.u16AxleList[0] = AXLE_BUFLEN;
//							g_sviVehicleInfo.u16AxleList[1] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[2] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[3] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[4] = AXLE_BUFLEN+1;
//							g_sviVehicleInfo.u16AxleList[5] = AXLE_BUFLEN+1;
//							l_u8SendVeh	= 1;
//							break;
//
//						default:
//							ToReDraw(l_u8ControlCode);
//							break;	
//					}
//					if (l_u8SendVeh	== 1)
//					{
//						ToProtocolBuff(&g_sviVehicleInfo);
//						OSQPost(g_pQVehicle,&g_sviVehicleInfo);
//						OSSemAccept(g_psemScreenRefresh);
//						OSSemPost(g_psemScreenRefresh);
//						l_u8SendVeh	= 0;
//					}
//					l_u32Value	= 0;
//					ToBreak(l_u8ControlCode);
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			LCDPRINTC(112 + (l_u8DigitCnt<<3) 	, 64	, "  ");
//
//			for (l_u8Tmp1 = 0; l_u8Tmp1 < l_u8DigitCnt; l_u8Tmp1 ++)
//				LCDPRINTC(112 + (l_u8Tmp1<<3) 	, 64	, "*");
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
////	g_u8EnableClearZero	= 1;		//恢复自动清零	 
//	BackGroundRecover();
//}
/*********************************************************************************************************
** Function name:		CLEARZERO
** Descriptions:		清零函数，相当于以前的ldc()
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110506	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void CLEARZERO(void)
//{
//	uint8 l_u8tmp = 0;
//
//	for(l_u8tmp = 0;l_u8tmp<CHANNELNUM;l_u8tmp++)	
//	{
//		OS_ENTER_CRITICAL();
//		g_an32MotionZero[l_u8tmp] = g_s32ADValue[l_u8tmp];
//		OS_EXIT_CRITICAL();
//	}
//    
//}

/*********************************************************************************************************
** Function name:		UIF3Code
** Descriptions:		查看F3信息界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIF3Code(void)
//{
//	char 	l_strOut[30]={0};
//	ClearKeyValue();
//	BackGroundSave();
//	BackGroundON();
//	//画界面
//	GUI_ClearSCR();
//	
//	LCDPRINTFC(0	, 0		, "版本号: %2dNT"	, YBVERSION);
// 	LCDPRINTC(104	, 0		, PROGRAMVERSION);
//	
//	//启动时间
//	sprintf(l_strOut, "启动时间: %.4d-%.2d-%.2d %.2d:%.2d:%.2d",
//							g_DevStartTime.u16Year, 
//							g_DevStartTime.u8Month, 
//							g_DevStartTime.u8Day, 
//							g_DevStartTime.u8Hour, 
//							g_DevStartTime.u8Minute, 
//							g_DevStartTime.u8Second);
//	LCDPRINTC(0		, 18	, (char *)l_strOut);
//	
//	sprintf(l_strOut, "启动次数: %-5d 采样率: %-5d ", g_u32StartupTime	, POINTRATE);
//	LCDPRINTC(0		, 36	, (char *)l_strOut);
//	
//	sprintf(l_strOut, "SetupLen: %-5d", sizeof(SETUPALIAS));
//	LCDPRINTC(0		, 54	, (char *)l_strOut);
//								  
//#if ISDEBUG				//调试状态
//	LCDPRINTC(232	, 0		, "*");	
//
//	
//#if	SENDWAVEENABLE > 0		//使能发波形
//
//#if	SENDWAVEBYSP > 0	
//	LCDPRINTC(216	, 0		, "串");
//#endif								  
//#if	SENDWAVEBYNET > 0
//	LCDPRINTC(200	, 0		, "网");
//#endif
//
//#endif	//#if	SENDWAVEENABLE > 0		//使能发波形		
//
//#endif
//	while (KeyValue == 0xff)		//有按键
//	{
//		WAITSCREENREFRESH();		//等待刷新信号
//	}
//	if (KeyValue == KB_F1)
//		ClearStartupCnt();
//
//	ClearKeyValue(); 
//	BackGroundRecover();
//}
/*********************************************************************************************************
** Function name:		UIF4Code
** Descriptions:		查看F4信息界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIF4Code(void)
//{
//
//#if  SHOWVEHPASSDEBUG > 0
//	uint8 l_u8Key,l_u8ControlCode;
//	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0;
//#endif
//
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//画界面
//	GUI_ClearSCR();
//#if  SHOWVEHPASSDEBUG > 0	//显示过车调试代码	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//	  	 
//		 LCDPRINTFC(0		, 0		, "F4代码:(%x)",g_ErrMsgState);
//		 GUI_Line(0		, 17	, 239	, 17	, 1);
//		 GUI_Line(0		, 127	, 239	, 127	, 1); 	   	 
//		 while(1)
//		 { 
//		 	WAITSCREENREFRESH();		//		 	
//		   	l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//				case KB_ENTER:	  					
//					ToReDraw(l_u8ControlCode);
//					break;
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;				
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//					ClearKeyValue();
//					break;	
//			}
//			if(l_u32Value)
//			{
//				ClearLCDLine(19);
//				LCDPRINTFC(0		, 19		, "车辆编号:%d        ",l_u32Value);
//			}
//			else
//			{
//			 	ClearLCDLine(19);
//				LCDPRINTC(0		, 19		, "车辆编号:          ");
//			}
//			if(IfBreak(l_u8ControlCode) ||IfReDraw(l_u8ControlCode))
//				break;		
//		 }
//
//		 if(IfBreak(l_u8ControlCode))
//		 	break;
//
//		  
//		 if(IfReDraw(l_u8ControlCode))
//		 {
//		 	ClearKeyValue();
//			ClearLCDLine(19);
//			if( l_u32Value>=1 && l_u32Value<=DBG_CODELEN )
//			{
//				memcpy(l_strOut, g_BugCode[l_u32Value-1].DbugCode, g_BugCode[l_u32Value-1].u8NextPos );
//		 	}
//			while(KeyValue == 0xFF)
//			{
////				OSTimeDly(10); 				
//				LCDPRINTC(0,19,l_strOut);		 				
//				WAITSCREENREFRESH();		//等待刷新信号	
//			}
//			ResetControlCode(l_u8ControlCode);
//			l_u32Value = 0;
//			ClearKeyValue();
//		 }
//	}
//	
//#else
//	LCDPRINTFC(0		, 0	, "F4代码功能未开启(%x)",g_ErrMsgState);
//	GUI_Line(0		, 17	, 239	, 17	, 1);
//    GUI_Line(0		, 127	, 239	, 127	, 1);
//	while (KeyValue == 0xff)		//有按键
//	{	
//		WAITSCREENREFRESH();		//等待刷新信号
//	}
//	ClearKeyValue();   
//	BackGroundRecover();
//#endif 
//
//}
/*********************************************************************************************************
** Function name:		UIF5Code
** Descriptions:		F5代码界面。查看网络连接的状态信息.
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wang ZiFeng		  
** Created Date:		20121107	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIF5Code(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Tmp1;
//	char 	l_strOut[100] = {0};
//	TargetIPInfo_Struct l_TargetInfo;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();	
//	GUI_ClearSCR();	
//	while(1)
//	{
//		//OSTimeDly(10);	 
//		OSTimeDly(100*OS_TICKS_PER_SEC/1000);
//		//画界面  	
//		LCDPRINTC(0		, 0		, "编号 状态  目标IP       端口");  		
//		GUI_Line(0		, 17	, 239	, 17	, 1);  
//		GUI_Line(0		, 112	, 239	, 112	, 1); 
//		LCDPRINTC(0		, 113		, "F1--查看状态说明");  		
//		for(l_u8Tmp1 = 0; l_u8Tmp1 < 4; l_u8Tmp1++)
//		{
//			ReadTargetSocktInfo(l_u8Tmp1,&l_TargetInfo);
//			sprintf(l_strOut,"%d    %d   %d:%d:%d:%d  %d           ",l_u8Tmp1+1,l_TargetInfo.u8State,
//								l_TargetInfo.u8CnntIP[0],l_TargetInfo.u8CnntIP[1],l_TargetInfo.u8CnntIP[2],l_TargetInfo.u8CnntIP[3],
//								(l_TargetInfo.u8CnntPort[0]<<8)+l_TargetInfo.u8CnntPort[1]);
//
//			LCDPRINTC(0		, 19 +(l_u8Tmp1<<4)	,l_strOut); 	 		
//		}  	  
//	   	//判断按键
//		switch(KeyValue)
//		{
//			case KB_ESC:
//				ToBreak(l_u8ControlCode);
//				ClearKeyValue();
//				break;
//			case KB_F1:
//				GUI_ClearSCR();
//				ClearKeyValue();		
//				 while(KeyValue == 0xFF)
//				 {
//				  	LCDPRINTC(0		, 0		, "网络状态说明");  		
//					GUI_Line(0		, 17	, 239	, 17	, 1);  
//					GUI_Line(0		, 112	, 239	, 112	, 1); 
//					LCDPRINTC(0		, 113		, "ESC--退出"); 
//					LCDPRINTC(0		, 19		, "00 --- 连接已关闭");
//					LCDPRINTC(0		, 35		, "20 --- 等待连接"); 
//					LCDPRINTC(0		, 51		, "22 --- 接收数据");
//					LCDPRINTC(0		, 67		, "23 --- 已建立连接");		  		
//				 }
//				ClearKeyValue();
//				break;				
//			default:				
//				break;
//		}
//		if (IfBreak(l_u8ControlCode))
//			break;
//	} 
//}
/*********************************************************************************************************
** Function name:		DisplayStatics
** Descriptions:		F5代码界面。查看网络连接的状态信息.
** input parameters:	None 
** output parameters:	none
**
** Created by:			Wang ZiFeng		  
** Created Date:		20121107	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//#if	 OS_TASK_CREATE_EXT_EN >0
//void DisplayStatics()
// {
// 	BOOLEAN	  exit = 0;
////	OS_TCB *ptcb = (OS_TCB *)0;
////	TaskStatics *pstatic = (TaskStatics*)0;
//	char l_strOut[100] = {0};
//	uint16 pos = 0,index = 0;
// 	GUI_ClearSCR();	
//   	sprintf(l_strOut,"ID    CPU   栈M   栈C");
//	LCDPRINTC(0,0,l_strOut);
//  	GUI_Line(0		,16			, 239	, 16	, 1	);
//	GUI_Line(0		,114		, 239	, 114	, 1	); 	
//	ClearKeyValue();
//	while(1)
//	{
//		//OSTimeDly(2);		 
//		OSTimeDly(20*OS_TICKS_PER_SEC/1000);
//		switch(KeyValue)
//		{
//		case 0xFF:
//			exit = 0;
//			break;
//		default:
//			exit = 1;
//			break;
//		}
//
//		if(exit == 1)
//			break;
//		pos = 0;
//		index = 0;
//		while(index < 9)
//		{
//			if(g_Taskstatics[index].CPUsage != 0 || g_Taskstatics[index].stkMaxUsage != 0)
//			{
//				sprintf(l_strOut,"T%d    %2d   %d   %d   ",g_Taskstatics[index].TaskID,g_Taskstatics[index].CPUsage,
//									  			g_Taskstatics[index].stkMaxUsage,g_Taskstatics[index].stkCurUsage);	
//			    LCDPRINTC(0,17+(pos<<4),l_strOut);
//				pos++;	
//			}
//			index++;
//				
//		}
////		WAITSCREENREFRESH();		//等待刷新信号量	
//	}
//   	ClearKeyValue();
// }
// #endif

 /*********************************************************************************************************
** Function name:		UICommonSet
** Descriptions:		普通设置界面，111
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UICommonSet(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " 系统设置");
//		LCDPRINTC(112	, 0		, "0 保存  Esc 退出" );
//		LCDPRINTC(8		, 24	, "1. 时钟设置");
//		LCDPRINTC(8		, 40	, "2. 波特率设置");
////		LCDPRINTC(8		, 56	, "3. 命令模式");
////		LCDPRINTC(8		, 72	, "4. 方向使能");
//		LCDPRINTC(8		, 88	, "5. 通讯协议");
////		LCDPRINTC(8		, 104	, "F5 缓存车数");
//		
////		LCDPRINTC(128	, 24	, "6. 台面宽度");
//		LCDPRINTC(128	, 40	, "7. 线圈抓拍");
////		LCDPRINTC(128	, 56	, "8. 轮轴报错");
////		LCDPRINTC(128	, 72	, "9. 抓拍使能");
//#if PROC_DOG_EN > 0
//		LCDPRINTC(128	, 88	, "F1 看门狗设置");
//#endif
//		LCDPRINTC(128	, 104	, "F4 掉电保护");
//
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//时钟设置
//					ToReDraw(l_u8ControlCode);
//					UISetTime();
//					break;	
//				
//				case KB_2:		//波特率设置
//					ToReDraw(l_u8ControlCode);
//					UISetBaudRate();
//					break;	
//				
////				case KB_3:		//命令模式
////					ToReDraw(l_u8ControlCode);
////					UISetCommandMode();
////					break;	
//				
////				case KB_4:		//方向使能
////					ToReDraw(l_u8ControlCode);
////					UISetForwardEnable();
////					break;	
//				
//				case KB_5:		//通讯协议
//					ToReDraw(l_u8ControlCode);
//					UISetProtocol();
//					break;	  				
//	
//				
//				case KB_7:		//线圈抓拍
//					ToReDraw(l_u8ControlCode);
//					UISetLoop();
//					break;	
//				
////				case KB_8:		//轮轴报错
////					ToReDraw(l_u8ControlCode);
////					UISetLunZhouEnable();
////					break;	
//				
////				case KB_9:		//抓拍使能
////					ToReDraw(l_u8ControlCode);
////					UISetCapture();
////					break;	
//				
//				case KB_0:		//保存设置
//					ToReDraw(l_u8ControlCode);
//					SaveParams();
//					break;	
//#if PROC_DOG_EN > 0				
//				case KB_F1:		//看门狗
//					ToReDraw(l_u8ControlCode);
//					UISetDog();
//					break;	
//#endif				
//				case KB_F4:		//掉电保护
//					ToReDraw(l_u8ControlCode);
//					UISetDiaodian();
//					break;	
//				
////				case KB_F5:		//缓存车数
////					ToReDraw(l_u8ControlCode);
////					UISetVehicleCache();
////					break;	
////				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewSetting
** Descriptions:		查看设置参数界面，222
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewSetting(void)
//{
//	SetupParam l_ssTmp;
//	uint16 l_u16tmp = 0;
//	char 	l_strOut[30]={0};
//	uint32 tmpValue = 0;
//	GUI_ClearSCR();
//	ClearKeyValue();
//
//	//画界面
//	ReadC256(SETUPPARAMADDR,(uint8 *)&l_ssTmp, SETUPPARAMSIZE);	
//
//	GUI_Line(0		, 18	, 239	, 18	, 1);
//	GUI_Line(0		, 124	, 239	, 124	, 1);
//	GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//	LCDPRINTC(0		, 0		, PROGRAMVERSION);
//	LCDPRINTC(176	, 0		, "当前设置");
////	memset(l_strOut	, 0		, 30); 
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32Zero[l_u16tmp];
//	}
//	sprintf(l_strOut	, "零位和:%u",tmpValue);
//	LCDPRINTC(0		, 20	, l_strOut);
//	
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32AxGain[l_u16tmp];
//	}
//	sprintf(l_strOut	, "增益和:%u",tmpValue);
//	LCDPRINTC(0		, 36	, l_strOut);
//	/*
//	LCDPRINTFC(0		, 36	, "增  益:%5d,"	, l_ssTmp.an32AxGain[0]);
//	LCDPRINTFC(112		, 36	, "%d  "	, l_ssTmp.an32AxGain[1]);
//	*/	  
//	LCDPRINTFC(0		, 52	, "分度值:%dkg"	, l_ssTmp.u8MotionScale);
//	  
//	LCDPRINTFC(0		, 68	, "满量程:%ukg"	, l_ssTmp.u32Full);
////	LCDPRINTFC(0		, 84	, "跟  踪:%u"	, l_ssTmp.u8Genzong);
//
//	sprintf(l_strOut	, "修改日期:%04d-%02d-%02d",2000+l_ssTmp.u8Year,l_ssTmp.u8Month,l_ssTmp.u8Day);
//	LCDPRINTC(0		, 104	, l_strOut); 
// 	 
//	while (KeyValue == 0xff)		//有按键
//	{
//		WAITSCREENREFRESH();		//等待刷新信号
//	}
//	ClearKeyValue();
//
//	memset((uint8 *)&l_ssTmp, 0, 2048);
//	
//	ReadC256(HISTORYBDADDR,(uint8 *)&l_ssTmp, SETUPPARAMSIZE);
//	GUI_ClearSCR();
//	GUI_Line(0		, 18	, 239	, 18	, 1);
//	GUI_Line(0		, 124	, 239	, 124	, 1);
//	GUI_Line(0		, 127	, 239	, 127	, 1);
//	
//	LCDPRINTC(0		, 0		, PROGRAMVERSION);
//	LCDPRINTC(176	, 0		, "历史设置");
//
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32Zero[l_u16tmp];
//	}
//	sprintf(l_strOut	, "零  位:%u",tmpValue);
//	LCDPRINTC(0		, 20	, l_strOut);
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32AxGain[l_u16tmp];
//	}
//	sprintf(l_strOut	, "增  益:%u",tmpValue);
//	LCDPRINTC(0		, 36	, l_strOut);
//	/*
//	sprintf(l_strOut	, "零  位:%d+%d=%d",l_ssTmp.an32Zero[0],l_ssTmp.an32Zero[1],l_ssTmp.an32Zero[0]+l_ssTmp.an32Zero[1]);
//	LCDPRINTC(0		, 20	, l_strOut);
//	
//	LCDPRINTFC(0	, 36	, "增  益:%5d,"	, l_ssTmp.an32AxGain[0]);
//	LCDPRINTFC(112	, 36	, "%5d  "	, l_ssTmp.an32AxGain[1]);	
//	*/
//		  
//	LCDPRINTFC(0	, 52	, "分度值:%dkg"	, l_ssTmp.u8MotionScale);
//	  
//	LCDPRINTFC(0	, 68	, "满量程:%ukg"	, l_ssTmp.u32Full);
////	LCDPRINTFC(0	, 84	, "跟  踪:%u"	, l_ssTmp.u8Genzong);
//
////	memset(l_strOut, 0, 30);
//	sprintf(l_strOut	, "修改日期:%04u-%02u-%02u",2000+l_ssTmp.u8Year,l_ssTmp.u8Month,l_ssTmp.u8Day);
//	LCDPRINTC(0		, 104	, l_strOut); 
// 	
//	while (KeyValue == 0xff)		//无按键
//	{
//		WAITSCREENREFRESH();		//等待刷新信号
//	}
//	ClearKeyValue();
//}
/*********************************************************************************************************
** Function name:		UIViewModify
** Descriptions:		查看速度修正界面，333，（静态修正和速度修正）
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewModify(void)
//{
//	uint8	l_u8Tmp1,l_u8Tmp2;
//	char 	l_strOut[30]={0};
//	//画界面
//	GUI_ClearSCR();
//	ClearKeyValue();
//	l_u8Tmp2	= 0;
//	for(l_u8Tmp1 = 0; l_u8Tmp1 < 16; l_u8Tmp1 ++)
//	{
////		sprintf(l_strOut, "%2d: 0- %d", l_u8Tmp1, SETUPALIAS.au16StaticModify[0][l_u8Tmp1]);
//		LCDPRINTC(0		, l_u8Tmp2	, l_strOut);
////		LCDPRINTFC(112	, l_u8Tmp2	, "1- %d",SETUPALIAS.au16StaticModify[1][l_u8Tmp1]);
//		
//		l_u8Tmp2	= (l_u8Tmp2 + 16) & 0x7f;
//		
//		if (l_u8Tmp1 % 8 == 7)
//		{
//			while (KeyValue == 0xff)		//有按键
//			{
//				WAITSCREENREFRESH();		//等待刷新信号
//			}
//			if (KeyValue == KB_ESC)
//				return;
//			ClearKeyValue();
//			GUI_ClearSCR();
//		}
//	}
//	
//	l_u8Tmp2	= 0;
//	for(l_u8Tmp1 = 0; l_u8Tmp1 < 32; l_u8Tmp1 ++)
//	{
//		sprintf(l_strOut,"%2dkm %d",l_u8Tmp1,SETUPALIAS.au16Speedmodify[l_u8Tmp1]);
//		LCDPRINTC(0		, l_u8Tmp2	, l_strOut);
//		
//		l_u8Tmp2	= (l_u8Tmp2 + 16) & 0x7f;
//		if (l_u8Tmp1 % 8 == 7)
//		{
//			while (KeyValue == 0xff)		//有按键
//			{
//				WAITSCREENREFRESH();		//等待刷新信号
//			}
//
//			if (KeyValue == KB_ESC)
//				return;
//
//			ClearKeyValue();
//			GUI_ClearSCR();
//		}
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewAuthor
** Descriptions:		查看作者信息界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewAuthor(void)
//{
//	//画界面
//	GUI_ClearSCR();
//	ClearKeyValue();
//
//	LCDPRINTC(16		, 32	, "作者: 田林岩");
//	LCDPRINTC(16		, 64	, "电话: 13801298463");
//	LCDPRINTC(16		, 96	, "tly001@vip.sina.com");
//	
//	GUI_Line(0		, 18	, 239	, 18	, 1);
//	GUI_Line(0		, 124	, 239	, 124	, 1);
//	GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//	while (KeyValue == 0xff)		//有按键
//	{
//		WAITSCREENREFRESH();		//等待刷新信号
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewIPInfo
** Descriptions:		IP参数，444
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110623	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewIPInfo(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//   	char 	l_strOut[30]={0};
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();	  
//		while (1)		//判断按键并刷新数据
//		{
//			GUI_Line(0		, 16	, 239	, 16	, 1);
//			GUI_Line(0		, 128	, 239	, 128	, 1);  			
//			LCDPRINTC(0		, 0		, " 网络配置参数          0-保存 ");  		
//
//			sprintf(l_strOut, "1.MAC地址:%2x-%2x-%2x-%2x-%2x-%2x", g_IPSetLocal.au8MACAddr[0], g_IPSetLocal.au8MACAddr[1], g_IPSetLocal.au8MACAddr[2], g_IPSetLocal.au8MACAddr[3], g_IPSetLocal.au8MACAddr[4], g_IPSetLocal.au8MACAddr[5]); 
//			LCDPRINTC(0	, 17	, l_strOut);
//	
//
//			sprintf(l_strOut, "2.本地IP:%d.%d.%d.%d", g_IPSetLocal.au8IPAddr[0], g_IPSetLocal.au8IPAddr[1], g_IPSetLocal.au8IPAddr[2], g_IPSetLocal.au8IPAddr[3]); 
//			LCDPRINTC(0	, 33	, l_strOut);  	
//
//			sprintf(l_strOut, "3.本地端口%-d,%-d,%-d,%-d", g_IPSetLocal.u32LocalPortNO,g_IPSetLocal.u32LocalPortNO+1,g_IPSetLocal.u32LocalPortNO+2,g_IPSetLocal.u32LocalPortNO+3); 
//			LCDPRINTC(0	, 48	, l_strOut); 	
//
//			sprintf(l_strOut, "4.子网掩码%d.%d.%d.%d", g_IPSetLocal.au8SubMask[0], g_IPSetLocal.au8SubMask[1], g_IPSetLocal.au8SubMask[2], g_IPSetLocal.au8SubMask[3]); 
//			LCDPRINTC(0	, 64	, l_strOut); 			
//
//			sprintf(l_strOut, "5.默认网关:%d.%d.%d.%d", g_IPSetLocal.au8GatewayIP[0], g_IPSetLocal.au8GatewayIP[1], g_IPSetLocal.au8GatewayIP[2], g_IPSetLocal.au8GatewayIP[3]); 
//			LCDPRINTC(0	, 79	, l_strOut);	   	
//			ToReDraw(l_u8ControlCode); 					
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//MAC不可修改
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				case KB_2:		//本地IP
//					UISetValueParamIP("本地IP", g_IPSetLocal.au8IPAddr);
//					break;	
//					
//				case KB_3:		//本地端口
//					UISetValueParamU32("本地端口", &(g_IPSetLocal.u32LocalPortNO), 0, 0);
//					break;	
//					
//				case KB_4:		//子网掩码
//					UISetValueParamIP("子网掩码", g_IPSetLocal.au8SubMask);
//					break;	
//					
//				case KB_5:		//默认网关
//					UISetValueParamIP("默认网关", g_IPSetLocal.au8GatewayIP);
//					break;	
//					
//		//		case KB_6:		//远程IP
//		//			UISetValueParamIP("远程IP", g_IPSetLocal.au8ServerIP);
//		//			break;	
//					
//		//		case KB_7:		//远程端口
//		//			UISetValueParamU32("远程端口", &(g_IPSetLocal.u32ServerPortNO), 0, 0);
//		//			break;	
//					
//				case KB_0:		//保存
//					SaveNetInfo();
//					ToReDraw(l_u8ControlCode);
//					break;
//					
//				case KB_I:		//初始化
//					InitNetParam();
//					ToReDraw(l_u8ControlCode);
//					break;
//					
//				default:
//					NotReDraw(l_u8ControlCode);
//					
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//			//OSTimeDly(LOOPTIME);		//延时，轮询时间
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewStartUpTime
** Descriptions:		查看系统启动时间记录
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110530	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewStartUpTime(void)
//{
//	uint16	l_u16Tmp,l_u16Tmp2;
//	uint16	l_u16RecIndex;
//	uint32	l_u32StartCnt;
//	uint8	l_au8RecData[8];
//	char	l_strOut[30] = {'0'};
//	uint8	l_u8Status;
//	uint8	l_u8Row;
//	SystemTime tm;
//	l_u8Status	= 0x01;
//			
//	//画界面
//	GUI_ClearSCR();
//	ClearKeyValue();
//
//	ReadC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//找到有效记录号
//	l_u16RecIndex	&= 0x3ff;
//	l_u16Tmp		= l_u16RecIndex;
//	l_u16RecIndex	++;
//	l_u16RecIndex	&= 0x3ff;
//	//l_u32StartCnt	= g_u32StartupTime;
//
//	l_u16Tmp2	= 0;		//启动时间计数，由于分屏显示
//	while ((l_u32StartCnt > 0)&&(l_u16Tmp != l_u16RecIndex))
//	{
//		//读取启动时间8B数据
//		ReadNORFlash(NORSTARTREC	+ (l_u16Tmp << 3), 8, &l_au8RecData[0]);
//		
//		//判断是否为有效数据
//		if (l_au8RecData[7] != 0xaa)	//无效
//		{
//			break;	
//		}  
//		//显示启动时间
//		tm.u16Year	= (l_au8RecData[0]	+ (l_au8RecData[1] <<8));
//		tm.u8Month	= l_au8RecData[2];
//		tm.u8Day		= l_au8RecData[3];
//		tm.u8Hour	= l_au8RecData[4];
//		tm.u8Minute	= l_au8RecData[5];
//		tm.u8Second	= l_au8RecData[6];
//
//		l_u8Row	= (l_u16Tmp2 & 0x07)<<4;
//		l_u16Tmp2	++;
//		l_u16Tmp2	&= 0x3ff;
//		LCDPRINTFC(0	, l_u8Row	, "%04d "	,l_u16Tmp2);
//		
//		sprintf(l_strOut, ": %04d-%02d-%02d %02d:%02d:%02d %04d", tm.u16Year, tm.u8Month, 
//			tm.u8Day, tm.u8Hour, tm.u8Minute, tm.u8Second,l_u16Tmp);
//
//		LCDPRINTC(32	, l_u8Row	, l_strOut);
//		
//		l_u16Tmp	--;
//		l_u16Tmp	&= 0x3ff;
//
//		//暂停 		
//		l_u32StartCnt	--;
//		ClearKeyValue();
//		if (l_u16Tmp2 % 8 == 0)
//		{
//			while (KeyValue == 0xff)		//有按键
//			{
//				WAITSCREENREFRESH();		//等待刷新信号量
//			}
//			if (KeyValue == KB_ESC)
//			{
//				l_u8Status	&= ~0x10;
//				return;
//			}
//			ClearKeyValue();
//			GUI_ClearSCR();
//			if (l_u32StartCnt == 0)
//				return;
//		}
//	}
//	
//	if (l_u8Status & 0x01)		//如果终止退出，则等待按键
//	{							
//		ClearKeyValue();
//		while (KeyValue == 0xff)		//有按键
//		{
//			WAITSCREENREFRESH();		//等待刷新信号量
//			//OSTimeDly(LOOPTIME);		//延时，轮询时间
//		}
//		if (KeyValue == KB_ESC)
//			return;
//		ClearKeyValue();
//		GUI_ClearSCR();
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewThreshold
** Descriptions:		阈值参数，8968
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIViewThreshold(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint16	l_u16TempValue = 0;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		LCDPRINTC(0		, 0		, " THRESHOLD");
//		
//		LCDPRINTFC(0		, 20	, "1.UP:  %d        ", SETUPALIAS.u16UpValue);
//		LCDPRINTFC(0		, 36	, "2.DN:  %d        ", SETUPALIAS.u16DownValue);
////		LCDPRINTFC(0		, 52	, "3.FD:  %d        ", SETUPALIAS.u16ForwardWidth);
////		LCDPRINTFC(0		, 68	, "4.RIF: %d        ", SETUPALIAS.u16FilterLevel);
////		LCDPRINTFC(0		, 84	, "5.WT:  %d        ", SETUPALIAS.u16AxleWidth);
//	
//		//wzf 2012-08-24
//		LCDPRINTFC(100		, 20	, "6.UPNUM: %d    ", SETUPALIAS.u8UPPoints);
//		LCDPRINTFC(100		, 36	, "7.DOWNNUM: %d  ", SETUPALIAS.u8DownPoints);
//	  	LCDPRINTFC(100		, 52	, "8.LUP: %d    ", SETUPALIAS.u16logicUpValue);
//		LCDPRINTFC(100		, 68	, "9.LDN: %d   ", SETUPALIAS.u16logicDownValue);
//		LCDPRINTC(0		, 104	, "0.保存");
//				
//		while (1)		//判断按键并刷新数据
//		{
//			ToReDraw(l_u8ControlCode);
//					
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//上称
//					UISetValueParamU16("UP", &(SETUPALIAS.u16UpValue), 10, 100);
//					break;
//
//				case KB_2:		//下称
//					UISetValueParamU16("DN", &(SETUPALIAS.u16DownValue), 10, 100);
//					break;	
//					
////				case KB_3:		//正向判别阈值
////					ClearKeyValue();
////					UISetValueParamU16("FD", &(SETUPALIAS.u16ForwardWidth), 60, 500);
////					break;	
//					
////				case KB_4:		//滤波级别
////					UISetValueParamU16("RIF", &(SETUPALIAS.u16FilterLevel), 3, 128);
////					break;	
//					
////				case KB_5:		//波形最小宽度
////					UISetValueParamU16("WT", &(SETUPALIAS.u16AxleWidth), 50, 500);
////					break;	
//				//wzf 2012-08-24
//				case KB_6:		//上秤点数
//					l_u16TempValue = SETUPALIAS.u8UPPoints;
//					UISetValueParamU16("UPNUM", &l_u16TempValue, 1, 30);
//					SETUPALIAS.u8UPPoints = l_u16TempValue;
//					break;	
//				case KB_7:		//下秤点数
//					l_u16TempValue = SETUPALIAS.u8DownPoints;
//					UISetValueParamU16("DOWNNUM", &l_u16TempValue, 1, 30);
//					SETUPALIAS.u8DownPoints = l_u16TempValue;
//					break;			
//				case KB_8:		//上秤逻辑
//					l_u16TempValue = SETUPALIAS.u16logicUpValue;
//					UISetValueParamU16("LUP", &l_u16TempValue, SETUPALIAS.u16UpValue, 300);
//					SETUPALIAS.u16logicUpValue = l_u16TempValue;
//					break;	
//				case KB_9:		//下秤逻辑
//					l_u16TempValue = SETUPALIAS.u16logicDownValue;
//					UISetValueParamU16("LDN", &l_u16TempValue, SETUPALIAS.u16DownValue, 300);
//					SETUPALIAS.u16logicDownValue = l_u16TempValue;
//					break;													
//				case KB_0:		//保存
//					SaveParams();
//					break;
//					
//				default:
//					NotReDraw(l_u8ControlCode);
//					
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UISetTime
** Descriptions:		设定系统时间
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetTime(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value;		//输入数字
//	uint8	l_u8Status;		//功能键状态
//	uint16	l_u16X;			//坐标X
//	uint8	l_u8Key;   
//	SystemTime tm; 	 
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8Status	= 0;
//
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "时间设定:    Esc退出, F1键修改");
//		
//		LCDPRINTC(16	, 20	, "当前时间:");
//		LCDPRINTC(64	, 36	, "-");
//		LCDPRINTC(88	, 36	, "-");
//		LCDPRINTC(144	, 36	, ":");
//		LCDPRINTC(168	, 36	, ":");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 125	, 239	, 125	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//			
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8Status != 0)
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//修改状态，开始输入数字
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= 1;
//						l_u32Value	= 0;
//						LCDPRINTC(16		, 60	,"设定时间:");
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					NotReDraw(l_u8ControlCode);
//					switch (l_u8Status)
//					{
//						case 1:		//年
//							if (l_u32Value > 1999 && l_u32Value < 2100)
//							{	
//								tm.u16Year	= l_u32Value;
//								LCDPRINTFC(32		, 76	, "%04d"	, l_u32Value);
//								LCDPRINTC(64		, 76	, "-");
//							}
//							else
//								ToReDraw(l_u8ControlCode);
//									
//							break;
//						
//						case 2:		//月
//							if (l_u32Value > 0 && l_u32Value < 13)
//							{
//								tm.u8Month	= l_u32Value & 0xff;
//								LCDPRINTFC(72		, 76	, "%02d"	, l_u32Value);
//								LCDPRINTC(88		, 76	, "-");
//			
//							}
//							else
//								ToReDraw(l_u8ControlCode);
//									
//							break;
//
//						case 3:		//日
//							if (l_u32Value > 0 && l_u32Value < 32)
//							{	
//								tm.u8Day	= l_u32Value & 0xff;
//								LCDPRINTFC(96		, 76	, "%02d"	, l_u32Value);
//							}
//							else
//								ToReDraw(l_u8ControlCode);
//									
//							break;
//						
//						case 4:		//时:0~23
//							if (l_u32Value < 24)
//							{	
//								tm.u8Hour	= l_u32Value & 0xff;
//								LCDPRINTFC(128	, 76	, "%02d"	, l_u32Value);
//								LCDPRINTC(144	, 76	, ":");
//							}
//							else
//								ToReDraw(l_u8ControlCode);	
//							break;
//						
//						case 5:		//分:0~59
//							if (l_u32Value < 60)
//							{	
//								tm.u8Minute	= l_u32Value & 0xff;
//								LCDPRINTFC(152	, 76	, "%02d"	, l_u32Value);	
//								LCDPRINTC(168	, 76	, ":");			
//							}
//							else
//								ToReDraw(l_u8ControlCode);	
//							break;
//						
//						case 6:		//秒:0~59
//							if (l_u32Value < 60)
//							{
//								tm.u8Second	= l_u32Value & 0xff;
//								LCDPRINTFC(176	, 76	, "%02d"	, l_u32Value);
//								LCDPRINTC(32		, 92	, "输入完毕. ");	
//								LCDPRINTC(32		, 108	, "Enter:保存 Esc:取消");
//							}
//							else
//								ToReDraw(l_u8ControlCode);	
//							break;
//
//						case 7:		//确认保存
//							//保存
//							SaveTime(tm);
//							ToReDraw(l_u8ControlCode);
//							break;
//
//						default:
//							ToReDraw(l_u8ControlCode);
//							break;							
//					}
//					l_u8Status	++;
//
//					l_u32Value	= 0;
//					break;	
//				
////				case 0xff:
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			LCDPRINTFC(32		, 36	, "%04d"	, g_DevCurTime.u16Year);
//			LCDPRINTFC(72		, 36	, "%02d"	, g_DevCurTime.u8Month);
//			LCDPRINTFC(96		, 36	, "%02d"	, g_DevCurTime.u8Day);
//
//			LCDPRINTFC(128		, 36	, "%02d"	, g_DevCurTime.u8Hour);
//			LCDPRINTFC(152		, 36	, "%02d"	, g_DevCurTime.u8Minute);	
//			LCDPRINTFC(176		, 36	, "%02d"	, g_DevCurTime.u8Second);
//
//			switch (l_u8Status)		//显示输入的数字
//			{
//				case 1:		//年
//					l_u16X	= 32;		
//					break;
//				
//				case 2:		//月
//					l_u16X	= 72;		
//					break;
//
//				case 3:		//日
//					l_u16X	= 96;		
//					break;
//				
//				case 4:		//时:0~23
//					l_u16X	= 128;		
//					break;
//				
//				case 5:		//分:0~59
//					l_u16X	= 152;		
//					break;
//				
//				case 6:		//秒:0~59
//					l_u16X	= 176;		
//					break;
//
//				default:
//					l_u16X	= 0xff;		
//					break;							
//			}
//			
//			if (l_u16X	!= 0xff)
//			{
//				if (l_u32Value != 0)
//					LCDPRINTFC(l_u16X	, 76	, "%u    "	, l_u32Value);
//				else
//					LCDPRINTC(l_u16X	, 76	, "      ");		
//			}
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		UISetBaudRate
** Descriptions:		设置波特率界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetBaudRate(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Key;
//	uint32	l_au32BR[6] = {4800, 4800, 9600, 38400, 57600, 115200};
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "2. 波特率设定       Esc-返回");
//		
//		LCDPRINTC(0		, 19	, "请选择波特率:");
//		LCDPRINTC(64	, 40	, "1.  4800");
//		LCDPRINTC(64	, 56	, "2.  9600");
//		LCDPRINTC(64	, 72	, "3.  38400");
//		LCDPRINTC(64	, 88	, "4.  57600");
//		LCDPRINTC(64	, 104	, "5.  115200");
//				
//		GUI_Line(0		, 17	, 239	, 17	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//				
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:
//				case KB_2:
//				case KB_3:
//				case KB_4:
//				case KB_5:
//					NotReDraw(l_u8ControlCode);
//					SETUPALIAS.u8BaudRate	= l_u8Key;
//					break;	
//					
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			LCDPRINTFC(112	, 19	, "%u    "	, l_au32BR[SETUPALIAS.u8BaudRate]);
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetProtocol
** Descriptions:		设置协议界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetProtocol(void)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(64	, 0		, "通讯协议:");
//		LCDPRINTC(8		, 24	, "1--新万集");
////		LCDPRINTC(8		, 24	, "0--江苏");
////		LCDPRINTC(8		, 40	, "1--万集");
////		LCDPRINTC(8		, 56	, "2--智运");
////		LCDPRINTC(8		, 72	, "3--安徽");
////		LCDPRINTC(8		, 88	, "4--福建");
////		LCDPRINTC(8		, 104	, "5--四川");
////		LCDPRINTC(128	, 24	, "6--贵州");
////		LCDPRINTC(128	, 40	, "7--陕西");
////		LCDPRINTC(128	, 56	, "8--辽宁");
////		LCDPRINTC(128	, 72	, "9--河南");
//
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//			  case KB_1:
//			  	  SETUPALIAS.u8Protocol	= PROTOL_WJ_NEW;
//				  break; 
//			  case KB_ESC:
//			  case KB_ENTER:
//			  		ToBreak(l_u8ControlCode);
//					break;
//			  default:
//				  	NotReDraw(l_u8ControlCode);
//					break;
//			}
//
////			switch(l_u8Key)
////			{
////				case KB_ESC:
////				case KB_ENTER:
////					ToBreak(l_u8ControlCode);
////					break;
////					
////				case KB_0:
////				case KB_1:
//////				case KB_2:
////				case KB_3:
////				case KB_4:
////				case KB_5:
////				case KB_6:
//////				case KB_7:
//////				case KB_8:
//////				case KB_9:
////					NotReDraw(l_u8ControlCode);
////					SETUPALIAS.u8Protocol	= l_u8Key;
////					break;	
////					
////				case KB_F1:
////					NotReDraw(l_u8ControlCode);
////					SETUPALIAS.u8Protocol	= 10;
////					break;	
////					
////				default:
////					NotReDraw(l_u8ControlCode);
////					break;
////			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
////			LCDPRINTFC(160	, 0		, "%d  ",SETUPALIAS.u8Protocol);
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
 /*********************************************************************************************************
** Function name:		UISetLoop
** Descriptions:		线圈触发使能界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetLoop(void)
//{
//	UISetEnableParam(" 线圈触发使能" , &(SETUPALIAS.u8LoopTriggerEnable));
//}
/*********************************************************************************************************
** Function name:		UISetDog
** Descriptions:		看门狗设置界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetDog(void)
//{
//	UISetEnableParam(" 看门狗使能" , &(SETUPALIAS.u8DOG));
//}
/*********************************************************************************************************
** Function name:		UISetDiaodian
** Descriptions:		掉电保护使能界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetDiaodian(void)
//{
//	UISetEnableParam(" 掉电保护使能" , &(SETUPALIAS.u8DiaoDianTag));
//}
/*********************************************************************************************************
** Function name:		UISetValueParamIP
** Descriptions:		修改IP参数
** input parameters:	p_pcName		参数名
**						p_pu8IP		 	变量指针
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetValueParamIP(char * p_pcName, uint8 * p_pu8IP)
//{
//	uint8	l_u8ControlCode;		//控制指令
//	uint32	l_u32Value	= 0;		//输入数字
//	uint8	l_u8Status;				//功能键状态
//	uint8	l_u8Key;
//	uint8	l_au8TmpIP[4];
//	char	l_acTmp[30];
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8Status	= 0;
//
//		//画界面
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s",p_pcName);
//		LCDPRINTC(0		, 0		, l_acTmp);		   
//		memset(l_acTmp	, 0		, 30);
//		sprintf(l_acTmp	,"%3d.%3d.%3d.%3d"	, *p_pu8IP, *(p_pu8IP+1), *(p_pu8IP+2), *(p_pu8IP+3));
//		LCDPRINTC(24	, 20	, l_acTmp);
//		LCDPRINTC(0		, 36	, " Esc退出, F1键修改");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8Status != 0)
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//修改状态，开始输入数字
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= 1;
//						l_u32Value	= 0;
//						LCDPRINTC(16		, 56	,"请输入新值: ");
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					NotReDraw(l_u8ControlCode);
//					switch (l_u8Status)
//					{
//						case 1:
//						case 2:
//						case 3:
//						case 4:
//							if (l_u32Value <256)
//							{	
//								l_au8TmpIP[l_u8Status-1]	= l_u32Value & 0xff;
//								LCDPRINTFC((l_u8Status<<5) - 8	, 76	, "%3d"	, l_au8TmpIP[l_u8Status-1]);
//								if (l_u8Status == 4)
//								{
//									LCDPRINTC(32		, 92	, "输入完毕. ");	
//									LCDPRINTC(32		, 108	, "Enter:保存 Esc:取消");
//								}
//								else
//								{											
//									LCDPRINTC((l_u8Status<<5) + 16	, 76	, ".");
//								}
//							}
//							else
//								ToReDraw(l_u8ControlCode);
//									
//							break;
//						
//						case 5:		//确认保存
//							//保存
//							*p_pu8IP		= l_au8TmpIP[0];
//							*(p_pu8IP+1)	= l_au8TmpIP[1];
//							*(p_pu8IP+2)	= l_au8TmpIP[2];
//							*(p_pu8IP+3)	= l_au8TmpIP[3];
//
//							ToReDraw(l_u8ControlCode);
//							break;
//
//						default:
//							ToReDraw(l_u8ControlCode);
//							break;							
//					}
//					l_u8Status	++;
//
//					l_u32Value	= 0;
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			if (l_u8Status	> 0)
//			{
//				if (l_u32Value != 0)
//					LCDPRINTFC((l_u8Status<<5) - 8	, 76	, "%u    "	, l_u32Value);
//				else
//					LCDPRINTC((l_u8Status<<5) - 8	, 76	, "      ");		
//			}
//
//			WAITSCREENREFRESH();		//等待刷新信号量
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetValueParamU32
** Descriptions:		通用参数设置界面,设置U32参数
** input parameters:	p_pcName		参数名
**						p_pu32Param 	变量指针
**						p_u32Max		参数上限 
**						p_u32Min		参数下限
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetValueParamU32(char * p_pcName, uint32 * p_pu32Param, uint32 p_u32Min, uint32 p_u32Max)
//{
//	uint8	l_u8ControlCode;//控制指令
//	uint32	l_u32Value	= 0;		//输入数字
//	uint8	l_u8Status;		//功能键状态
//	uint8	l_u8Key;
//	char	l_acTmp[30];
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8Status	= 0;
//
//		//画界面
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s: %u",p_pcName,*p_pu32Param);
//		LCDPRINTC(0		, 0		, l_acTmp);
//		LCDPRINTC(0		, 32	, " Esc退出, F1键修改");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		
//				case KB_2:		
//				case KB_3:		
//				case KB_4:		
//				case KB_5:		
//				case KB_6:		
//				case KB_7:		
//				case KB_8:		
//				case KB_9:		
//				case KB_0:
//					if (l_u8Status != 0)
//					{
//						l_u32Value	*= 10;
//						l_u32Value	+= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_BKSP:		//回删一个数字
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//修改状态，开始输入数字
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= KB_F1;
//						l_u32Value	= 0;
//						memset(l_acTmp, 0, 30);
//						sprintf(l_acTmp," 请输入新值(%u-%u):",p_u32Min,p_u32Max);
//						LCDPRINTC(16		, 56	, l_acTmp);
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//确认输入
//					if (l_u8Status	== KB_F1)
//					{
//						if ((p_u32Max == 0 && p_u32Min == 0) || (l_u32Value >= p_u32Min && l_u32Value <= p_u32Max))
//						{
//							*p_pu32Param	= l_u32Value;
//						}
//						l_u32Value	= 0;
//						l_u8Status	= 0;
//						ToReDraw(l_u8ControlCode);
//					}
//					else
//						NotReDraw(l_u8ControlCode);
//					break;	
//				
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (l_u8Status == KB_F1)		//输入状态
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(56		, 80	, "         ");
//				else
//					LCDPRINTFC(56		, 80	, "%u      "	, l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetEnableParam
** Descriptions:		通用使能参数设置界面
** input parameters:	p_pcName		参数名
**						p_pu8Param 		变量指针
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110509	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UISetEnableParam(char * p_pcName, uint8 * p_pu8Param)
//{
//	uint8	l_u8ControlCode;//控制指令
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//画界面
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, p_pcName);
//		LCDPRINTC(176	, 0		, "Esc 返回");
//		
//		LCDPRINTC(8		, 24	, "是否使能:");
//		LCDPRINTC(8		, 44	, "1.使能  2.禁止");
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//判断按键并刷新数据
//		{
//			//判断按键
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//使能
//					NotReDraw(l_u8ControlCode);
//					* p_pu8Param	= 1;
//					break;	
//						
//				case KB_2:		//禁止		
//					NotReDraw(l_u8ControlCode);
//					* p_pu8Param	= 0;
//					break;	
//					
//				default:
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//是否重画
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//刷新屏幕
//			if (* p_pu8Param)		//为1时，使能
//				LCDPRINTC(120		, 24	, "使能  ");
//			else
//				LCDPRINTC(120		, 24	, "禁止  ");
//			
//			WAITSCREENREFRESH();		//等待刷新信号
//		}
//
//		//是否退出
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		SaveTime
** Descriptions:		保存时间设置
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110517	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SaveTime(SystemTime p_stSet)
{
	SetRTCTime(&p_stSet);
}

//#if	SENDWAVEENABLE > 0		//使能发波形
/*********************************************************************************************************
** Function name:		UIBDSendWave
** Descriptions:		发送波形使能界面
** input parameters:	None 
** output parameters:	none
**
** Created by:			ZHANG Ye		  
** Created Date:		20110507	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void UIBDSendWave(void)
//{
//	uint8	l_u8Status	= 0;
//	l_u8Status	|= ((SETUPALIAS.u8SendWaveEnable	& 0x0f)<<4);
//	UISetEnableParam(" 发送波形使能" , &(SETUPALIAS.u8SendWaveEnable));
//	
//	l_u8Status	|= SETUPALIAS.u8SendWaveEnable	& 0x0f;
//
//	if (l_u8Status == 0x01)
//		OSTaskResume(TASK_WAVE_PRIO);
//	else
//		if (l_u8Status == 0x10)
//			OSTaskSuspend(TASK_WAVE_PRIO);
//}									  
//#endif
/*********************************************************************************************************
** Function name:		ProModInit
** Descriptions:		系统初始化
** input parameters:	None 
** output parameters:	none
**
** Created by:			wgh		  
** Created Date:		20140303	  
**-------------------------------------------------------------------------------------------------------
** Modified by:		  		
** Modified date:	  		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void ProModInit(uint8 num)
{
	uint8 l_u8tempi,l_u8temp,l_u8Err;
	uint16 l_u16SendLen;
	PROMODPARAM.m_u8ZTRow = 3;
	PROMODPARAM.m_u8LaneNum = 3;
	for(l_u8tempi=0; l_u8tempi<10; l_u8tempi++)
	{
		PROMODPARAM.m_au16ZTDist[l_u8tempi][0]=1400;
		PROMODPARAM.m_au16ZTDist[l_u8tempi][1]=1400;
		PROMODPARAM.m_au16ZTDist[l_u8tempi][2]=1400;
	}
	for(l_u8tempi=0;l_u8tempi<64;l_u8tempi++)
	{
		PROMODPARAM.m_au16ZTGap[l_u8tempi]=1400;
	}
	//////////////不同模式下，各位表示的含义不同
	PROMODPARAM.m_au32Thres[0]= 30;   
	PROMODPARAM.m_au32Thres[1]= 10;   
	PROMODPARAM.m_au32Thres[2]=	10;    
	PROMODPARAM.m_au32Thres[3]=	4;    
	PROMODPARAM.m_au32Thres[4]=	3;   
	PROMODPARAM.m_au32Thres[5]=	3;   
	PROMODPARAM.m_au32Thres[6]=	60;    
	PROMODPARAM.m_au32Thres[7]=	60;   
	PROMODPARAM.m_au32Thres[8]=	416;     
	PROMODPARAM.m_au32Thres[9]=	2;    
	PROMODPARAM.m_au32Thres[10]= 20;  
	PROMODPARAM.m_au32Thres[11]= 3;  
	PROMODPARAM.m_au32Thres[12]= 60;    
	PROMODPARAM.m_au32Thres[13]= 1;   
	PROMODPARAM.m_au32Thres[14]= 20;  
	PROMODPARAM.m_au32Thres[15]= 1; 
	PROMODPARAM.m_au32Thres[16]= 20;	
	PROMODPARAM.m_au32Thres[17]= 70;	
	PROMODPARAM.m_au32Thres[18]= 20;	
	PROMODPARAM.m_au32Thres[19]= 30;
	PROMODPARAM.m_au32Thres[20]= 18;
	PROMODPARAM.m_au32Thres[21]= 0;
	PROMODPARAM.m_au32Thres[22]= 0;
	PROMODPARAM.m_au32Thres[23]= 0;
	PROMODPARAM.m_au32Thres[24]= 0;
	PROMODPARAM.m_au32Thres[25]= 0;
	PROMODPARAM.m_au32Thres[26]= 0;
	PROMODPARAM.m_au32Thres[27]= 0;
	PROMODPARAM.m_au32Thres[28]= 0;
	PROMODPARAM.m_au32Thres[29]= 0;
	PROMODPARAM.m_au32Thres[30]= 0;
	PROMODPARAM.m_au32Thres[31]= 0;
	PROMODPARAM.m_au32Thres[32]= 0;
	PROMODPARAM.m_au32Thres[33]= 0;
	PROMODPARAM.m_au32Thres[34]= 0;
	PROMODPARAM.m_au32Thres[35]= 0;
	PROMODPARAM.m_au32Thres[36]= 0;
	PROMODPARAM.m_au32Thres[37]= 0;
	PROMODPARAM.m_au32Thres[38]= 0;
	PROMODPARAM.m_au32Thres[39]= 0;
	//////////////
	//////////////
	for(l_u8tempi=0; l_u8tempi<40; l_u8tempi++)
		PROMODPARAM.m_au8Enable[l_u8tempi]=0;
	for(l_u8tempi=0; l_u8tempi<64; l_u8tempi++)
		PROMODPARAM.m_au8ZTIntEn[l_u8tempi]=1;
	for(l_u8tempi=0; l_u8tempi<20; l_u8tempi++)
	{
		PROMODPARAM.m_au8CHLSET[l_u8tempi] = l_u8tempi/2;
		PROMODPARAM.m_au8CHLSET[l_u8tempi+1] = l_u8tempi/2;
		l_u8tempi ++ ;
	}
	//////////////
	PROMODPARAM.m_u8DebugBaudRate = 5;
	PROMODPARAM.m_u8ZTWide = 175;
	PROMODPARAM.m_u8YaFengZTWide = 175;
	////////////
	for(l_u8tempi=0; l_u8tempi<20; l_u8tempi++)
		PROMODPARAM.m_au8LaneBetwWide[l_u8tempi] = 40;
	////////////
	PROMODPARAM.m_u8TriggerMode = 4;
	PROMODPARAM.m_u8IsYaFeng = 0;
	PROMODPARAM.m_u8UploadDataMode = 1;
	PROMODPARAM.m_u8DataWaveEn = 0;
	PROMODPARAM.m_u8VehWaveSample = 0;
	PROMODPARAM.m_u8CaptureMode = 1;
	PROMODPARAM.m_u8TransmitSysEn = 0;
	PROMODPARAM.m_u8JGKZ = 30;

	PROMODPARAM.m_u16SDSaveEn = 0;
	PROMODPARAM.m_u16ExEquiType = 0;
	for(l_u8tempi=0; l_u8tempi<8; l_u8tempi++)
		PROMODPARAM.m_au8AcqBoardFlag[l_u8tempi] = 1;
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
		memset(PROMODPARAM.m_u8CaptureFlag[l_u8tempi],0,2);
	PROMODPARAM.m_u8BwdDirecFlag = 1;
	for(l_u8tempi=0; l_u8tempi<19; l_u8tempi++)
		PROMODPARAM.m_au8VehDirecFlag[l_u8tempi] = 1;
	for(l_u8tempi=0; l_u8tempi<8; l_u8tempi++)
		PROMODPARAM.m_au8HalfLane[l_u8tempi]=0;
	PROMODPARAM.m_u8SDCapa = 16;
	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
		PROMODPARAM.m_au8SDFuncSpaceper[l_u8tempi]= 0;
	////////////////////////////////////////////
	PROMODPARAM.m_au8SDFuncSpaceper[0] = 1;
	PROMODPARAM.m_au8SDFuncSpaceper[1] = 15;
	PROMODPARAM.m_au8SDFuncSpaceper[2] = 30;
	PROMODPARAM.m_au8SDFuncSpaceper[3] = 25;
	PROMODPARAM.m_au8SDFuncSpaceper[4] = 19;
	PROMODPARAM.m_au8SDFuncSpaceper[5] = 10;
	////////////////////////////////////////////
	PROMODPARAM.m_u8ProgVersion[0] = PROGVERSION_POS_0;
	PROMODPARAM.m_u8ProgVersion[1] = PROGVERSION_POS_1;
	PROMODPARAM.m_u8ProgVersion[2] = PROGVERSION_POS_2;
	PROMODPARAM.m_u8ProgVersion[3] = PROGVERSION_POS_3;

	PROMODPARAM.m_u8StudyAlgEn[0] = 0;					  //学习使能
	PROMODPARAM.m_u8StudyAlgEn[1] = 0;
	PROMODPARAM.m_u8StudyAlgEn[2] = 0;
	PROMODPARAM.m_u8StudyAlgEn[3] = 0;
	PROMODPARAM.m_u8StudyAlgEn[4] = 0;
	PROMODPARAM.m_u16StudyWeight[0][0] = 0;
	PROMODPARAM.m_u16StudyWeight[1][0] = 0;
	PROMODPARAM.m_u16StudyWeight[2][0] = 0;
	PROMODPARAM.m_u16StudyWeight[3][0] = 0;
	PROMODPARAM.m_u16StudyWeight[4][0] = 0;
	PROMODPARAM.m_u16StudyWeight[0][1] = 0;
	PROMODPARAM.m_u16StudyWeight[1][1] = 0;
	PROMODPARAM.m_u16StudyWeight[2][1] = 0;
	PROMODPARAM.m_u16StudyWeight[3][1] = 0;
	PROMODPARAM.m_u16StudyWeight[4][1] = 0;
	PROMODPARAM.m_u8StudyLimit[0][0] = 0;
	PROMODPARAM.m_u8StudyLimit[1][0] = 0;
	PROMODPARAM.m_u8StudyLimit[2][0] = 0;
	PROMODPARAM.m_u8StudyLimit[3][0] = 0;
	PROMODPARAM.m_u8StudyLimit[4][0] = 0;
	PROMODPARAM.m_u8StudyLimit[0][1] = 0;
	PROMODPARAM.m_u8StudyLimit[1][1] = 0;
	PROMODPARAM.m_u8StudyLimit[2][1] = 0;
	PROMODPARAM.m_u8StudyLimit[3][1] = 0;
	PROMODPARAM.m_u8StudyLimit[4][1] = 0;

	PROMODPARAM.m_u8EnableFlag[0] = 0;	// 以激光为准
	PROMODPARAM.m_u8EnableFlag[1] = 1;	// 是否延时抛车使能
	PROMODPARAM.m_u8EnableFlag[2] = 0;	// 交调信息使能，0 不使能 1使能	控制交调功能
	PROMODPARAM.m_u8EnableFlag[3] = 0;	// 无线上传总使能， 控制协议通信
	PROMODPARAM.m_u8EnableFlag[4] = 0;	// 无线上传调试信息使能 控制调试信息是否上传
	PROMODPARAM.m_u8EnableFlag[5] = 0;	// 零点跟踪使能，默认1开
	PROMODPARAM.m_u8EnableFlag[6] = 1;	// 传感器类型 1窄条 2石英
	PROMODPARAM.m_u8EnableFlag[7] = 0;

	for(l_u8tempi=0; l_u8tempi<20; l_u8tempi++)
	{
		PROMODPARAM.m_u16LRLocation[0][l_u8tempi] = 0 + 175*l_u8tempi;
		PROMODPARAM.m_u16LRLocation[1][l_u8tempi] = 174 + 175*l_u8tempi;
	}
	
	for(l_u8tempi=0; l_u8tempi<64; l_u8tempi++)
	{
		PROMODPARAM.m_n8LouDL[l_u8tempi] = 35;
		PROMODPARAM.m_n8CFDian[l_u8tempi] = 25;
	}

	for(l_u8tempi=0; l_u8tempi<10; l_u8tempi++)
	{
		PROMODPARAM.m_au8IOUseEn[l_u8tempi] = l_u8tempi;
	}
	for(l_u8tempi=10; l_u8tempi<26; l_u8tempi++)
	{
		PROMODPARAM.m_au8IOUseEn[l_u8tempi] = l_u8tempi-10;
	}

	memset(PROMODPARAM.m_au8ReserveByte,0,PROMOD_RESERVED);
	
	AddCrc16((uint8 *)&PROMODPARAM, PROMODPARAMSIZE-2);	 
			   
 //	WriteC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);	// 在0x00地址处写入32字节数据
			for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp=WriteC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 1);
	else
		g_u8RWNVFlag &=!(1 << 1);
	#if DEBUGINFO_TASK_SP_EN > 0
		OSSemPend(g_pSemSendThtouSP,0, &l_u8Err);
		l_u16SendLen = sprintf((char*)g_cDebugInfo[g_u16CurDebugInd].m_au8DataBuf,"\r\nProModInit,Num=%d,g_u8RWNVFlag=%d\r",num,g_u8RWNVFlag
);
		g_cDebugInfo[g_u16CurDebugInd].m_u16Len = l_u16SendLen;
		OSQPost( g_pQSendPCCOM1, &g_cDebugInfo[g_u16CurDebugInd] );
		g_u16CurDebugInd++;
		g_u16CurDebugInd %= COM_NUM;
		memset( &g_cDebugInfo[g_u16CurDebugInd], 0, sizeof(CComBuf) );
		OSSemPost(g_pSemSendThtouSP);
	#endif
}

void InitJDParam(void)
{
	uint8 l_u8tempi;
	uint8 *p1;
	uint8 *p2;

	jingdu = 113.55002565147197;
	weidu = 24.040477460757732;
	
	p1 = (uint8 *)&jingdu;
	p2 = (uint8 *)&weidu;

	for(l_u8tempi=0; l_u8tempi<16; l_u8tempi++)
	{
		JDSYSPARAM.m_au8DeviceID[l_u8tempi] = 0;
	}
	for(l_u8tempi=0; l_u8tempi<15; l_u8tempi++)
	{
		JDSYSPARAM.m_au8StationID[l_u8tempi] = 0;
	}
	for(l_u8tempi=0; l_u8tempi<8; l_u8tempi++)
	{
		JDSYSPARAM.m_au8LngBuf[l_u8tempi] = *p1++;
		JDSYSPARAM.m_au8LatBuf[l_u8tempi] = *p2++;
	}
	JDSYSPARAM.m_au16Elevation = 8141;				 //海拔高度
	for(l_u8tempi = 0; l_u8tempi < 10; l_u8tempi++)
	{
		JDSYSPARAM.m_u16MidKeHuoThr[l_u8tempi] = 2600;			   //FZX
	}
	for(l_u8tempi = 0; l_u8tempi < 10; l_u8tempi++)
	{
		JDSYSPARAM.m_u16SmallKeHuoThr[l_u8tempi] = 2200;					   	//FB
	}

	JDSYSPARAM.m_u8LaneDir = 0;  //0 单向 1 双向

	JDSYSPARAM.m_u8OrderMod = 2; //协议模式
	JDSYSPARAM.m_u8SavePeriod = 5;	//上传数据周期
	//JDSYSPARAM.m_u8TransMod = 2;//上传方式 1 网口 2 无线
	
	JDSYSPARAM.m_u32FollowVehTimeThr = 10;//交调跟车时间阈值
	JDSYSPARAM.m_u32DiffSend = 3000; //交调待传上限值
	JDSYSPARAM.m_u32WeightSendThr = 3000;
	JDSYSPARAM.m_u32AxleNumSendThr = 3000;
	JDSYSPARAM.m_u32AxleWghSendThr = 3000;
	JDSYSPARAM.m_u32MaxForAxle = 10000;

	JDSYSPARAM.m_u16SmallandMid = 3700;	//小型和中型车间距界限
	JDSYSPARAM.m_u16VehNumForSend = 10000;
	JDSYSPARAM.m_u8SendVehType = 2;//发车类型  2只发超限车， 1都发
	
	JDSYSPARAM.m_u8ProgVersion[0] = PROGVERSION_POS_0;
	JDSYSPARAM.m_u8ProgVersion[1] = PROGVERSION_POS_1;
	JDSYSPARAM.m_u8ProgVersion[2] = PROGVERSION_POS_2;
	JDSYSPARAM.m_u8ProgVersion[3] = PROGVERSION_POS_3;

	memset(JDSYSPARAM.m_au8ReserveByte,0,JDSYS_RESERVED);
	
	AddCrc16((uint8 *)&JDSYSPARAM, JDSYSPARAMSIZE-2);	 
			   
 	WriteC256(JDSYSADDR,(uint8 *)&JDSYSPARAM,JDSYSPARAMSIZE);
}
