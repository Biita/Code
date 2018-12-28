#define __PRTCLWJOLD_C
#include "prtclWJOLD.h"
#include "common.h"

#define     MID2(a, min, max) ((((a)>=(min))&&((a)<=(max)))?1:0)
#define     PROMODPARAM				g_cProModParam

typedef struct _TP_ITEM
{
	uint32 min;
	uint32 max;
} TP_ITEM;

typedef struct _TPGRP_2
{
	uint32   tp;
	TP_ITEM  item1;
} TPGRP_2;

typedef struct _TPGRP_3
{
	uint32   tp;
	TP_ITEM  item1;
	TP_ITEM  item2;
} TPGRP_3;

typedef struct _TPGRP_4
{
	uint32   tp;
	TP_ITEM  item1;
	TP_ITEM  item2;
	TP_ITEM  item3;
} TPGRP_4;

typedef struct _TPGRP_5
{
	uint32   tp;
	TP_ITEM  item1;
	TP_ITEM  item2;
	TP_ITEM  item3;
	TP_ITEM  item4;
} TPGRP_5;

typedef struct _TPGRP_6
{
	uint32   tp;
	TP_ITEM  item1;
	TP_ITEM  item2;
	TP_ITEM  item3;
	TP_ITEM  item4;
	TP_ITEM  item5;
} TPGRP_6;

typedef struct _TPGRP_7
{
	uint32   tp;
	TP_ITEM  item1;
	TP_ITEM  item2;
	TP_ITEM  item3;
	TP_ITEM  item4;
	TP_ITEM  item5;
	TP_ITEM  item6;
} TPGRP_7;

const TPGRP_2 grp2[6] = {
	{21, {1750, 2290}},
	{22, {2290, 2950}},
	{23, {2950, 3670}},
	{24, {3670, 6050}},
	{25, {6050, 6700}},
	{20, {0, 0}}
};

const TPGRP_3 grp3[6] = {
	{31, {1950, 6400}, {900, 1800}},
	{32, {2750, 6100}, {3670, 10000}},
	{33, {2290, 6700}, {1800, 3670}},
	{34, {3670, 6700}, {1800, 3670}},
	{35, {900, 1800}, {3350, 5500}},
	{30, {0, 0}, {0, 0}}
};

const TPGRP_4 grp4[6] = {
	{41, {2750, 6100}, {3300, 10000}, {900, 1800}},
	{42, {2500, 6100}, {900, 1800}, {900, 1800}},
	{43, {900, 1800}, {2500, 6100}, {900, 1800}},
	{44, {2290, 3670}, {1700, 3670}, {1900, 6000}},
	{45, {3670, 6700}, {1800, 3670}, {2100, 6100}},
	{40, {0, 0}, {0, 0}, {0, 0}}
};

const TPGRP_5 grp5[6] = {
	{51, {2400, 6100}, {900, 1800}, {3300, 10000}, {900, 1800}},
	{52, {1950, 6400}, {900, 1800}, {2100, 3670}, {2100, 6100}},
	{53, {2750, 6100}, {3300, 9000}, {900, 1800}, {900, 1800}},
	{54, {2750, 6100}, {3670, 10000}, {1800, 6100}, {2100, 6100}},
	{55, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
	{50, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};

const TPGRP_6 grp6[6] = {
	{61, {2400, 6100}, {900, 1800}, {3300, 9000}, {900, 1800}, {900, 1800}},
	{62, {2400, 6100}, {900, 1800}, {900, 1800}, {3300, 9000}, {900, 1800}},
	{63, {2400, 6100}, {900, 1800}, {3300, 10000}, {1800, 6100}, {2100, 6100}},
	{64, {2750, 6100}, {3300, 9000}, {900, 1800}, {1800, 6100}, {2100, 6100}},
	{65, {900, 1800}, {2500, 6100}, {900, 1800}, {1800, 6100}, {2100, 6100}},
	{60, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};

const TPGRP_7 grp7[6] = {
	{71, {2400, 6100}, {900, 1800}, {3300, 10000}, {900, 1800}, {1800, 6100}, {2100, 6100}},
	{72, {2400, 6100}, {900, 1800}, {900, 1800}, {3300, 9000}, {900, 1800}, {900, 1800}},
	{73, {2750, 6100}, {3300, 9000}, {900, 1800}, {900, 1800}, {1800, 6100}, {2100, 6100}},
	{74, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
	{75, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
	{70, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};
void ToBuff1( Vehicle_Struct *pdata )
{
	//变量定义
	uint8  		i=0;
	uint8 		buf[6];
//	uint8		l_u8PackNO = 0;	// 包数
//	uint8		l_u8AllPackNum = 0;//包总数
	//
	uint8 *		l_pu8Tmp;	   //指针，指向所有数据
//	uint16 *	l_pu16Tmp;	   //指针，指向窄条点数据
//	Channal_Struct *		l_pChlData;	  //指针，指向窄条结构体
//	uint8 *		l_pu8SendBufTmp;	   //指针，指向发送数据结构体数组
	//
//	uint8		l_u8FrameHeadNum = 12;	   //帧头字节数，根据最终协议格式修改
	//uint8		l_u8FrameVehInfoNum = 100;	   //整车重量等信息字节数，根据最终协议格式修改
	//
//	uint16		l_u32FrameLength = 0; // 帧长计数
//	uint32		l_u32tmp = 0;
	//
//	uint8		l_u8AxleListIndex = 0;// 轴序号
//	uint8		l_u8ZhaitiaoListIndex = 0;// 窄条序号
//	uint16		l_u16PtsIndex = 0;//点数序号
	uint16		l_u16Tempi=0;//临时序号
//	uint16		l_u16Index1,l_u16Index2;

	//uint8		l_u8PtsOverFlag = 0;//点数超过最大值（10000），跳出

//	uint16		l_u16LastPackSize = 0;//最后一帧大小
	int16		l_s8Acc = 0;
	uint8		ret;
	uint8   	l_VehType;
	int32 Axisnum;  
  	int32 Axisdis[16]; 
	//
//	uint16	l_u16SendLen = 0;
//	uint8	l_u8Err = 0;


	//将所有数据组成一帧
   	///////////// 旧高速预检兼容
	Axisnum = pdata->u8AixCnt;
	for (i=0; i < Axisnum; i++)
	{
		Axisdis[i] =pdata->au16AxleDistance[i]*10;		        //轴距
								
	}
	pdata->VehicleType = VehicleType_judge(Axisnum,Axisdis);	//
	/////////////
	g_sndNetBuf[(pdata->u8id) & 0xFF].u8Cnt = 1;
	g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[0] = 18+pdata->u8AixCnt*9;
	l_pu8Tmp = g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[0];
	*(l_pu8Tmp++) = 0xff;
//	if(g_cProModParam.m_u8TriggerMode == 1 || g_cProModParam.m_u8TriggerMode == 3)
//	{
//		*(l_pu8Tmp++) = ((pdata->u8psLane)<<4)|(pdata->u8psLane);
//	}
//	else
//	{
//		*(l_pu8Tmp++) = ((pdata->u8psLane/2)<<4)|(pdata->u8psLane/2);	           //说明:具体使用时，车道号规则要按照高速规则进行；     
		*(l_pu8Tmp++) = ((PROMODPARAM.m_au8CHLSET[pdata->u8psLane])<<4)|(PROMODPARAM.m_au8CHLSET[pdata->u8psLane]);
//	}
	*(l_pu8Tmp++) = 0x01;
	// 长度
	*(l_pu8Tmp++) = 0x00;
	*(l_pu8Tmp++) = pdata->VehicleType/10*9+18;	   
	// 速度
	*(l_pu8Tmp++) = pdata->u16speed/10;					
	// 加速度
	for(l_u16Tempi = 0;l_u16Tempi<pdata->u8AixCnt-1;l_u16Tempi++)
	{
		l_s8Acc+=pdata->n8accSpeed[l_u16Tempi];	
	}
//	*(l_pu8Tmp++) = l_s8Acc/(pdata->u8AixCnt-1);		 // 加速度
	Nm2Bcd(l_s8Acc/(pdata->u8AixCnt-1), buf, 6);			
	*(l_pu8Tmp++) = (buf[0] << 4)|buf[1];
	*(l_pu8Tmp++) = (buf[2] << 4)|buf[3];
	*(l_pu8Tmp++) = (buf[4] << 4)|buf[5];
	// 附加信息
	*(l_pu8Tmp++) = 0;
	// 违例码
	*(l_pu8Tmp++) = 0;

	*(l_pu8Tmp++) = (g_DevCurTime.u16Year) & 0xFF; 
	*(l_pu8Tmp++) = g_DevCurTime.u8Month;
	*(l_pu8Tmp++) = g_DevCurTime.u8Day;
	*(l_pu8Tmp++) = g_DevCurTime.u8Hour;
	*(l_pu8Tmp++) = g_DevCurTime.u8Minute;
	*(l_pu8Tmp++) = g_DevCurTime.u8Second;
		// 车型
	l_VehType = 0;
	for(i=0; i<pdata->u8AixGrpCnt; i++)
	{
		l_VehType = l_VehType * 10 + pdata->au8AxleGrpType[i];
	}
	*(l_pu8Tmp++) = pdata->VehicleType;
	// 左轮重
	for (i=0; i<pdata->u8AixCnt; i++)
	{
		Nm2Bcd(pdata->au16AxleWeight[i]*10/2, buf, 6);
		*(l_pu8Tmp++) = (buf[0] << 4)|buf[1];
		*(l_pu8Tmp++) = (buf[2] << 4)|buf[3];
		*(l_pu8Tmp++) = (buf[4] << 4)|buf[5];
	}
	// 右轮重
	for (i=0; i<pdata->u8AixCnt; i++)
	{
		Nm2Bcd(pdata->au16AxleWeight[i]*10/2, buf, 6);
		*(l_pu8Tmp++) = (buf[0] << 4)|buf[1];
		*(l_pu8Tmp++) = (buf[2] << 4)|buf[3];
		*(l_pu8Tmp++) = (buf[4] << 4)|buf[5];
	}
	// 轴距
	for (i=0; i<(pdata->u8AixCnt-1); i++)
	{
		Nm2Bcd(pdata->au16AxleDistance[i], buf, 6);
		*(l_pu8Tmp++) = (buf[0] << 4)|buf[1];
		*(l_pu8Tmp++) = (buf[2] << 4)|buf[3];
		*(l_pu8Tmp++) = (buf[4] << 4)|buf[5];
	}	
//	if((ADJ_SENDCK == p_u8CommandNO)||(ADJ_GSNETCONNECT == p_u8CommandNO)||(ADJ_GSTIMESYNCHRONY == p_u8CommandNO)||(ADJ_GSDEVICESTATUS == p_u8CommandNO))
	{
		Crc3_create(g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[0], g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[0]-3);
	}
//	else
//	{
//		AddCrc16(g_sndNetBuf[(pdata->u8id) & 0xFF].au8data[0],g_sndNetBuf[(pdata->u8id) & 0xFF].au32len[0]-2);
//	}
	OSQPost(g_pQSendPC,&g_sndNetBuf[(pdata->u8id) & 0xFF]);

	if(ret == OS_Q_FULL)
	{
		g_ErrMsgState |= 0x04; 		 	
	}
	
	// 记录车辆编号，用于重发使用
//	g_u8CurSendNetBufIndex = pdata->u8id;
}
uint32 VehicleType_judge(uint32 Axisnum,int32 Axisdis[])	 // 参数为轴数
{
	uint8 i;
//	int32 Axisdis[16];

//	for (i=0; i < Axisnum; i++)
//	{
//		Axisdis[i] = pItem->au16AxleDistance[i];		//轴距	
//	}

	switch (Axisnum)
	{
	case 2:					 // 二轴车
		for (i=0; i<5; i++)
		{
			if (MID2(Axisdis[0], grp2[i].item1.min, grp2[i].item1.max))
			{
				return grp2[i].tp;
			}
		}
		return grp2[5].tp;
	case 3:					// 三轴车
		for (i=0; i<5; i++)
		{
			if (MID2(Axisdis[0], grp3[i].item1.min, grp3[i].item1.max)
				&& MID2(Axisdis[1], grp3[i].item2.min, grp3[i].item2.max))
			{
				return grp3[i].tp;
			}
		}
		return grp3[5].tp;
	case 4:					// 四轴车
		for (i=0; i<5; i++)
		{
			if (MID2(Axisdis[0], grp4[i].item1.min, grp4[i].item1.max)
				&& MID2(Axisdis[1], grp4[i].item2.min, grp4[i].item2.max)
				&& MID2(Axisdis[2], grp4[i].item3.min, grp4[i].item3.max))
			{
				return grp4[i].tp;
			}
		}
		return grp4[5].tp;
	case 5:					// 五轴车
		for (i=0; i<4; i++)
		{
			if (MID2(Axisdis[0], grp5[i].item1.min, grp5[i].item1.max)
				&& MID2(Axisdis[1], grp5[i].item2.min, grp5[i].item2.max)
				&& MID2(Axisdis[2], grp5[i].item3.min, grp5[i].item3.max)
				&& MID2(Axisdis[3], grp5[i].item4.min, grp5[i].item4.max))
			{
				return grp5[i].tp;
			}
		}
		return grp5[5].tp;
	case 6:					// 六轴车
		for (i=0; i<5; i++)
		{
			if (MID2(Axisdis[0], grp6[i].item1.min, grp6[i].item1.max)
				&& MID2(Axisdis[1], grp6[i].item2.min, grp6[i].item2.max)
				&& MID2(Axisdis[2], grp6[i].item3.min, grp6[i].item3.max)
				&& MID2(Axisdis[3], grp6[i].item4.min, grp6[i].item4.max)
				&& MID2(Axisdis[4], grp6[i].item5.min, grp6[i].item5.max))
			{
				return grp6[i].tp;
			}
		}
		return grp6[5].tp;
	case 7:				   // 七轴车
		for (i=0; i<3; i++)
		{
			if (MID2(Axisdis[0], grp7[i].item1.min, grp7[i].item1.max)
				&& MID2(Axisdis[1], grp7[i].item2.min, grp7[i].item2.max)
				&& MID2(Axisdis[2], grp7[i].item3.min, grp7[i].item3.max)
				&& MID2(Axisdis[3], grp7[i].item4.min, grp7[i].item4.max)
				&& MID2(Axisdis[4], grp7[i].item5.min, grp7[i].item5.max)
				&& MID2(Axisdis[5], grp7[i].item6.min, grp7[i].item6.max))
			{
				return grp7[i].tp;
			}
		}
		return grp7[5].tp;
	default:
		return 0;
	}
}
