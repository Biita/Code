/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			UI.c
** Last modified Date:  20110506
** Last Version:		1.0
** Descriptions:		���н��溯��
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
** Descriptions:		��ͨ˫���
**
*********************************************************************************************************/
#define	__UI_C
#include "UI.h"

//ѹ��λ��
//#define		POS_ABS		1			//AB�ٶ�
//#define		POS_BCS		2			//BC�ٶ�
//#define		POS_ABCS	3			//ѹ���ٶ�
//#define		POS_VEH		4			//��������
//#define		POS_GAP		5			//ѹ������
//#define		POS_2WB		6			//��������

//static	uint8	m_u8Err;
//static char	m_acTmp[30];		//��ʾ��һ��

#define		SETUPALIAS				g_Setup			//���ò����ṹ
#define		IPINFOALIAS				g_IPSetLocal			//IP��Ϣ����

//#define		LCDPRINTC				PrintCharXY			//��ʾ�ַ���
//#define		LCDPRINTFC				PrintFormatDataXY	//��ʾһ����ʽ���ַ���

#define		CRCFunc					CheckCrc			//CRCУ�麯����	 
#define		CRCFunc16				AddCrc16			//CRCУ�麯����,����ֵ

//����ָ��:0x01:�ػ���0x02���˳�					
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
////�����ֵ
//#define		KeyValue				g_u8KeyValueMapped
//#define		ClearKeyValue()			KeyValue	= 0xff
//
//#define		ClearLCDLine(X)			LCDPRINTC(0		, X	, "                              ")
//
////�ȴ���Ļˢ���ź�
//#define		WAITSCREENREFRESH()		OSSemPend(g_psemScreenRefresh,0, &m_u8Err)

#define     PROMODPARAM				g_cProModParam
#define     JDSYSPARAM              g_cJDSysParam
/*********************************************************************************************************
** Function name:           Display_Chezhong
**
** Descriptions:            ����������ʾ��ʼ����
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
	uint32 l_Chezhong = 0;		 //�ܳ���
	uint8 l_temp;
	char ZhouXing[10];			 //����


	GUI_ClearSCR();
	if(g_u8StopChange == 1)
	{
// 		PrintCharXY(0, 0,"����        ����   �ٶ�*");
		PrintCharXY(0, 0,"������        ������   �ٶ�*");
	}
	else
	{
// 		PrintCharXY(0, 0,"����        ����   �ٶ�");
		PrintCharXY(0, 0,"������        ������   �ٶ�");
	}
	GUI_Line(0,17,240,17, 1	);

// 	//��ʾ��ź�����
// 	for(l_temp = 0; l_temp < pItem -> u8AixCnt; l_temp++)
// 	{
// 		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
// 		sprintf(l_Cheliang_Info, "  %d       %dkg", l_temp + 1, pItem ->au16AxleWeight[l_temp] * 10);
// 		PrintCharXY(0, Y, l_Cheliang_Info);
// 		Y += 15;
// 		l_Chezhong += pItem -> au16AxleWeight[l_temp] * 10;

// 	}
	//��ʾ��ź�������
	for(l_temp = 0; l_temp < pItem -> u8AixGrpCnt; l_temp++)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "  %d       %dkg", l_temp + 1, pItem ->au16AxleGrpWeight[l_temp] * 10);
		PrintCharXY(0, Y, l_Cheliang_Info);
		Y += 15;
		l_Chezhong += pItem -> au16AxleGrpWeight[l_temp] * 10;

	}

	//�������أ���ʾ������
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
	//��ʾ�ٶ�
	if(pItem -> u16speed != 0)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "%dkm/h", pItem -> u16speed / 10);
		PrintCharXY(150, 18, l_Cheliang_Info);
	}
	//��ʾ����
	PrintCharXY(0, 110, "����:");
	if(l_Chezhong != 0)
	{
		memset(l_Cheliang_Info, 0, sizeof(l_Cheliang_Info));
		sprintf(l_Cheliang_Info, "%dkg", l_Chezhong);
		PrintCharXY(40, 110, l_Cheliang_Info);
	}
	//��ʾ����	
	memset(ZhouXing, 0, sizeof(ZhouXing));
	for(l_temp = 0; l_temp < pItem -> u8AixGrpCnt; l_temp++)
	{
		 ZhouXing[l_temp] = pItem -> au8AxleGrpType[l_temp] + 0x30;
	}
	ZhouXing[l_temp] = '\0';
	PrintCharXY(105, 110, "����:");
	PrintCharXY(145, 110, ZhouXing);	
	//��ʾ������
	PrintCharXY(185, 110, "����:");
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
** Descriptions:            LCD��ʼ����
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
	PrintCharXY(40,0,"���ֳ�ִ��ͨ����Ϣ");// ���ֳ�ִ��ͨ����Ϣ 	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	PrintCharXY(10,40,"��ǰ����:  ");	   // ��ǰ������
	sprintf(l_ucTempCheDaoNum, "%d", g_u8CheDaoNum);	 
	PrintCharXY(100,40,l_ucTempCheDaoNum);	       
//	PrintCharXY(120,20,"��ǰͨ��:  ");  	   // ��ǰͨ��
//	PrintCharXY(10,40,"��ʼ����:  ");	   // ��ʼ���룺
//	PrintCharXY(120,40,"��ǰ����:  ");	   // ��ǰ����
//	PrintCharXY(10,60,"��ǰ����:  ");	   // ��ǰ���棺
	PrintCharXY(10,60,"��̬����:   ");	   // ��̬����: 
	GUI_Line(0,105,240,105, 1	);//	
	PrintCharXY(0,108,"F1-����");
	PrintCharXY(62,108,"F2-����ѡ��");
	PrintCharXY(154,108,"F3-�������");
//	PrintCharXY(120,108,"F4 ��ѯ�汾��");
}

/*********************************************************************************************************
** Function name:           DisplayChooseChedao
**
** Descriptions:            F2����ѡ�����
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
	PrintCharXY(10,0,"ѡ�񳵵�");// ѡ�񳵵�	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(70,20,"1.����1");	   // ѡ�񳵵�1  
	PrintCharXY(70,40,"2.����2");	   // ѡ�񳵵�2
	PrintCharXY(70,60,"3.����3");	   // ѡ�񳵵�3
	PrintCharXY(70,80,"4.����4");	   // ѡ�񳵵�4 
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ�񳵵�");
}

/*********************************************************************************************************
** Function name:           DisplayChooseTongdao
**
** Descriptions:            ѡ���Ӧ������ͨ��
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
//	PrintCharXY(10,0,"ѡ��ͨ��");// ѡ��ͨ��	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
//	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(40,40,"1.ͨ��1");	   // ѡ��ͨ��1  
//	PrintCharXY(140,40,"2.ͨ��2");	   // ѡ��ͨ��2
//	PrintCharXY(40,60,"3.ͨ��3");	   // ѡ��ͨ��3
//	PrintCharXY(140,60,"4.ͨ��4");	   // ѡ��ͨ��4
//	PrintCharXY(40,80,"5.ͨ��5");	   // ѡ��ͨ��5
//	PrintCharXY(140,80,"6.ͨ��6");	   // ѡ��ͨ��6 
//	GUI_Line(0,105,240,105, 1	);
//	PrintCharXY(0,108,"��ʾ:���ּ�1~6ѡ��ͨ��");
//}

/*********************************************************************************************************
** Function name:           DisplayFunction
**
** Descriptions:            ���ܽ���
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
	PrintCharXY(40,0,"���ֳ�ִ����̬����");// ���ֳ�ִ����̬���� 	 PrintCharXY(uint8 p_u8X, uint8 p_u8Y, char * p_cStr)
	PrintCharXY(70,20,"1.��������  ");	   // ��̨����  
	PrintCharXY(70,40,"2.ͨ������  ");	   // ͨ������
	PrintCharXY(70,60,"3.������ʹ��");	   // ������ʹ��
//	PrintCharXY(70,80,"4.����ѯ   ");	   // ����ѯ  
	GUI_Line(0,105,240,105, 1	);//	
	PrintCharXY(5,108,"��ʾ:���ּ�1~3ѡ����");
}

/*********************************************************************************************************
** Function name:           DisplayChooseTongdao_Chedao
**
** Descriptions:            ѡ��ͨ�����ڳ���
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
	PrintCharXY(10,0,"ͨ������");// ͨ������	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"1.����1��ͨ�������ѯ���޸�");	   
	PrintCharXY(10,40,"2.����2��ͨ�������ѯ���޸�");	   
	PrintCharXY(10,60,"3.����3��ͨ�������ѯ���޸�");	   
	PrintCharXY(10,80,"4.����4��ͨ�������ѯ���޸�");	    
	GUI_Line(0,105,240,105, 1	);
//	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ��ͨ�����ڳ���");
	PrintCharXY(0,108,"��1~4ѡ����Ҫ�޸�����ĳ���");
}

/*********************************************************************************************************
** Function name:           DisplayChedaox_zengyi
**
** Descriptions:            ��ʾ��������
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
	PrintCharXY(10,0,"����1����");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"��ʼ����:");	    
	PrintCharXY(10,40,"��ǰ����:");
	PrintCharXY(10,60,"��ǰ����:");	  
	PrintCharXY(10,80,"��̬����:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-����");
	PrintCharXY(120,108,"F3-�޸�����");
//	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ�񳵵�");
}
void DisplayChedao2_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����2����");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"��ʼ����:");	    
	PrintCharXY(10,40,"��ǰ����:");
	PrintCharXY(10,60,"��ǰ����:");	  
	PrintCharXY(10,80,"��̬����:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-����");
	PrintCharXY(120,108,"F3-�޸�����");
//	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ�񳵵�");
}
void DisplayChedao3_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����3����");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"��ʼ����:");	    
	PrintCharXY(10,40,"��ǰ����:");
	PrintCharXY(10,60,"��ǰ����:");	  
	PrintCharXY(10,80,"��̬����:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-����");
	PrintCharXY(120,108,"F3-�޸�����");
//	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ�񳵵�");
}
void DisplayChedao4_zengyi(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����4����");	 
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"��ʼ����:");	    
	PrintCharXY(10,40,"��ǰ����:");
	PrintCharXY(10,60,"��ǰ����:");	  
	PrintCharXY(10,80,"��̬����:");	   
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"F1-����");
	PrintCharXY(120,108,"F3-�޸�����");
//	PrintCharXY(0,108,"��ʾ:���ּ�1~4ѡ�񳵵�");
}


/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao1
**
** Descriptions:            ��ʾ����1ͨ������
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
	PrintCharXY(10,0,"����1ͨ������");// ����1����	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"ͨ��1����:");	   // ͨ��1~7����
//	PrintCharXY(120,20,"ͨ��2����:");
//	PrintCharXY(10,40,"ͨ��3����:");
//	PrintCharXY(120,40,"ͨ��4����:");
//	PrintCharXY(10,60,"ͨ��5����:");
//	PrintCharXY(120,60,"ͨ��6����:");			
	PrintCharXY(10,20,"1.ͨ��1:");		// ͨ��1~6����
	Seach_TongdaoZY(1, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.ͨ��2:");
	Seach_TongdaoZY(1, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.ͨ��3:");
	Seach_TongdaoZY(1, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.ͨ��4:");
	Seach_TongdaoZY(1, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.ͨ��5:");
	Seach_TongdaoZY(1, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.ͨ��6:");
	Seach_TongdaoZY(1, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.�����ѯ");		//�����ѯ��ʾ
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��1-6�޸Ķ�Ӧ����");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao2
**
** Descriptions:            ��ʾ����2ͨ������
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
	PrintCharXY(10,0,"����2ͨ������");// ����1����	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"ͨ��1����:");	   // ͨ��1~7����
//	PrintCharXY(120,20,"ͨ��2����:");
//	PrintCharXY(10,40,"ͨ��3����:");
//	PrintCharXY(120,40,"ͨ��4����:");
//	PrintCharXY(10,60,"ͨ��5����:");
//	PrintCharXY(120,60,"ͨ��6����:");			
	PrintCharXY(10,20,"1.ͨ��1:");		// ͨ��1~6����
	Seach_TongdaoZY(2, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.ͨ��2:");
	Seach_TongdaoZY(2, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.ͨ��3:");
	Seach_TongdaoZY(2, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.ͨ��4:");
	Seach_TongdaoZY(2, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.ͨ��5:");
	Seach_TongdaoZY(2, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.ͨ��6:");
	Seach_TongdaoZY(2, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.�����ѯ");		//�����ѯ��ʾ
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��1-6�޸Ķ�Ӧ����");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chengtai3
**
** Descriptions:            ��ʾ����3ͨ������
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
	PrintCharXY(10,0,"����3ͨ������");// ����1����	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"ͨ��1����:");	   // ͨ��1~7����
//	PrintCharXY(120,20,"ͨ��2����:");
//	PrintCharXY(10,40,"ͨ��3����:");
//	PrintCharXY(120,40,"ͨ��4����:");
//	PrintCharXY(10,60,"ͨ��5����:");
//	PrintCharXY(120,60,"ͨ��6����:");			
	PrintCharXY(10,20,"1.ͨ��1:");		// ͨ��1~6����
	Seach_TongdaoZY(3, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.ͨ��2:");
	Seach_TongdaoZY(3, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.ͨ��3:");
	Seach_TongdaoZY(3, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.ͨ��4:");
	Seach_TongdaoZY(3, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.ͨ��5:");
	Seach_TongdaoZY(3, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.ͨ��6:");
	Seach_TongdaoZY(3, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);
	PrintCharXY(10,80,"0.�����ѯ");		//�����ѯ��ʾ
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��1-6�޸Ķ�Ӧ����");
}

/*********************************************************************************************************
** Function name:           DisplayTongdaoZY_Chedao4
**
** Descriptions:            ��ʾ����4ͨ������
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
	PrintCharXY(10,0,"����4ͨ������");// ����1����	 
	GUI_Line(0,17,240,17, 1	);
//	PrintCharXY(10,20,"ͨ��1����:");	   // ͨ��1~6����
//	PrintCharXY(120,20,"ͨ��2����:");
//	PrintCharXY(10,40,"ͨ��3����:");
//	PrintCharXY(120,40,"ͨ��4����:");
//	PrintCharXY(10,60,"ͨ��5����:");
//	PrintCharXY(120,60,"ͨ��6����:");			
	PrintCharXY(10,20,"1.ͨ��1:");		// ͨ��1~6����
	Seach_TongdaoZY(4, 1);
	PrintCharXY(75,20,g_ucTongdaoZY_Display);

	PrintCharXY(125,20,"2.ͨ��2:");
	Seach_TongdaoZY(4, 2);
	PrintCharXY(190,20,g_ucTongdaoZY_Display);

	PrintCharXY(10,40,"3.ͨ��3:");
	Seach_TongdaoZY(4, 3);
	PrintCharXY(75,40,g_ucTongdaoZY_Display);
	  
	PrintCharXY(125,40,"4.ͨ��4:");
	Seach_TongdaoZY(4, 4);
	PrintCharXY(190,40,g_ucTongdaoZY_Display);

	PrintCharXY(10,60,"5.ͨ��5:");
	Seach_TongdaoZY(4, 5);
	PrintCharXY(75,60,g_ucTongdaoZY_Display);

	PrintCharXY(125,60,"6.ͨ��6:");
	Seach_TongdaoZY(4, 6);
	PrintCharXY(190,60,g_ucTongdaoZY_Display);

	PrintCharXY(10,80,"0.�����ѯ");		//�����ѯ��ʾ
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��1-6�޸Ķ�Ӧ����");
}
/*********************************************************************************************************
** Function name:           DisplayProgramVersion
**
** Descriptions:            ��ʾ�汾�źͳ����޸�����
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
	PrintCharXY(10,0, "�汾��:        ");
	PrintCharXY(130,0, PROGRAMVERSION);
	PrintCharXY(10,20,"�����޸�����:  ");
	PrintCharXY(130,0, PROGMODIFYDATE);	   
	GUI_Line(0,105,240,105, 1	);//
	PrintCharXY(10,108,"��ESC������һ��");	
}
/*********************************************************************************************************
** Function name:           DisplayChange_ChedaoxZY
**
** Descriptions:            ���������޸Ľ���
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
	PrintCharXY(10,0,"����1�����޸�");
	PrintCharXY(10,40,"�������µ�����:");
//��ȡ����������Ϣ��l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��Enter�������");	
}
void DisplayChange_Chedao2ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"����2�����޸�");
	PrintCharXY(10,40,"�������µ�����:");
//��ȡ����������Ϣ��l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��Enter�������");
}
void DisplayChange_Chedao3ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"����3�����޸�");
	PrintCharXY(10,40,"�������µ�����:");
//��ȡ����������Ϣ��l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��Enter�������");
}
void DisplayChange_Chedao4ZY(void) 						   
{
//	uint16 l_nZY;
	GUI_ClearSCR();
	PrintCharXY(10,0,"����4�����޸�");
	PrintCharXY(10,40,"�������µ�����:");
//��ȡ����������Ϣ��l_nZY
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��Enter�������");
}
/*********************************************************************************************************
** Function name:           DisplayChange_TongDaoxZY
**
** Descriptions:            ͨ�������޸Ľ���
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
	//ÿ����һ���ַ�������ˢ�½��棬������������ַ���ʾ����
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��1�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��1�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
void DisplayChange_TongDao2ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��2�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��2�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
void DisplayChange_TongDao3ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��3�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��3�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
void DisplayChange_TongDao4ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��4�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��4�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
void DisplayChange_TongDao5ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��5�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��5�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
void DisplayChange_TongDao6ZY(void)
{
	if((Locate == Info_Chedao1_TongDaoZY_Page) || (Locate == Info_Chedao2_TongDaoZY_Page) || (Locate == Info_Chedao3_TongDaoZY_Page) || (Locate == Info_Chedao4_TongDaoZY_Page))
	{
		GUI_ClearSCR();
		PrintCharXY(10,0,"ͨ��6�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
		memset(g_ucTongdaoZY_Display, 0, sizeof(g_ucTongdaoZY_Display));
	}
	else
	{
		PrintCharXY(10,0,"ͨ��6�����޸�");
		PrintCharXY(10,40,"�������µ�����:");
		sprintf(g_ucTongdaoZY_Display, "%d", g_nTongdaoZY_Change);
		PrintCharXY(130,40,g_ucTongdaoZY_Display);
		GUI_Line(0,105,240,105, 1	);
		PrintCharXY(10,108,"��Enter�������");
	}
}
/*********************************************************************************************************
** Function name:		DisplayChedaox_Neima
** Descriptions:		��ѯ����xͨ������
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
	PrintCharXY(10,0,"����1�����ѯ");		  //����1ͨ��1-6����
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"ͨ��1:");
	Seach_TongdaoNM(1,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"ͨ��2:");
	Seach_TongdaoNM(1,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"ͨ��3:");
	Seach_TongdaoNM(1,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"ͨ��4:");
	Seach_TongdaoNM(1,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"ͨ��5:");
	Seach_TongdaoNM(1,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"ͨ��6:");
	Seach_TongdaoNM(1,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);

	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��ESC������һ��");
}
void DisplayChedao2_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����2�����ѯ");		   //����2ͨ��1-6����
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"ͨ��1:");
	Seach_TongdaoNM(2,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"ͨ��2:");
	Seach_TongdaoNM(2,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"ͨ��3:");
	Seach_TongdaoNM(2,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"ͨ��4:");
	Seach_TongdaoNM(2,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"ͨ��5:");
	Seach_TongdaoNM(2,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"ͨ��6:");
	Seach_TongdaoNM(2,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��ESC������һ��");
}
void DisplayChedao3_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����3�����ѯ");		    //����3ͨ��1-6����
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"ͨ��1:");
	Seach_TongdaoNM(3,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"ͨ��2:");
	Seach_TongdaoNM(3,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"ͨ��3:");
	Seach_TongdaoNM(3,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"ͨ��4:");
	Seach_TongdaoNM(3,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"ͨ��5:");
	Seach_TongdaoNM(3,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"ͨ��6:");
	Seach_TongdaoNM(3,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��ESC������һ��");
}
void DisplayChedao4_Neima(void)
{
	GUI_ClearSCR();
	PrintCharXY(10,0,"����4�����ѯ");			 //����4ͨ��1-6����
	GUI_Line(0,17,240,17, 1	);
	PrintCharXY(10,20,"ͨ��1:");
	Seach_TongdaoNM(4,1);
	PrintCharXY(65,20,g_ucTongdaoNM_Display);

	PrintCharXY(120,20,"ͨ��2:");
	Seach_TongdaoNM(4,2);
	PrintCharXY(175,20,g_ucTongdaoNM_Display);

	PrintCharXY(10,40,"ͨ��3:");
	Seach_TongdaoNM(4,3);
	PrintCharXY(65,40,g_ucTongdaoNM_Display);

	PrintCharXY(120,40,"ͨ��4:");
	Seach_TongdaoNM(4,4);
	PrintCharXY(175,40,g_ucTongdaoNM_Display);

	PrintCharXY(10,60,"ͨ��5:");
	Seach_TongdaoNM(4,5);
	PrintCharXY(65,60,g_ucTongdaoNM_Display);

	PrintCharXY(120,60,"ͨ��6:");
	Seach_TongdaoNM(4,6);
	PrintCharXY(175,60,g_ucTongdaoNM_Display);
	GUI_Line(0,105,240,105, 1	);
	PrintCharXY(10,108,"��ESC������һ��");
}

/*********************************************************************************************************
** Function name:		Seach_TongdaoZY
** Descriptions:		��ѯͨ������
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
** Descriptions:		����ͨ�������޸�
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
** Descriptions:		��ѯͨ������
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
** Descriptions:		��ѯͨ�����
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
** Descriptions:		�������棬����Ҫѭ����ʾ�����һ��ʱ���ر�
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
//	GUI_ClearSCR();								//����
//	DrawPic();									//��ʾͼƬ
//
//	LCDPRINTC(130	, 72	, PROGRAMVERSION	);	//��ʾ�汾��Ϣ
//
////	BeepON();			//��������
////	
////	OSTimeDly(10);	
////	
////	BeepOFF();			//��������	      
////	
//
//	LEDON();
//
//}

/*********************************************************************************************************
** Function name:		UIGeneral
** Descriptions:		��׼���̽��棬һֱ��ʾ
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
//	uint8	l_u8ControlCode;		//����ָ��
//	uint32	l_u32Temp1;				//��ʱ����1
//	int32	l_n32MaxWt;				//���浥��խ�������ֵ
//	uint32	l_u32Temp2,l_u32Temp3;	//��ʱ����2,3 
//	uint32	l_u32TempAllWeight = 0;//��ʱ���������� 
//	//char 	l_cTaiXing[20];
//	//uint8	l_u8TaiXingIndex;	  
//	char 	l_strOut[BUF_LEN]={0};
//	char  *l_pStr = (char *)0;
//	uint8   l_State = ' '; 
//	Axle_Struct	*l_pAxle;
//	Vehicle_Struct *l_pVeh;
//
//	AddStartupCnt();	//��¼��������
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	memset(l_strOut, 0, BUF_LEN);  
//	while(1)
//	{  	  	
//		//������
//		GUI_ClearSCR();
//		
//		LCDPRINTC(0		, 0		, "���  "	);
//		LCDPRINTC(40	, 0		, "����  "	);
//		LCDPRINTC(80	, 0		, "����  "	); 
//		LCDPRINTC(136	, 0		, "����  "	);
//	   	LCDPRINTC(200	, 0		, "״̬  "	); 
//
//		GUI_Line(0		, 16	, 239	, 16	, 1	);
//		GUI_Line(0		, 114	, 239	, 114	, 1	);	
//		BackGroundON();	
//		while (1)		//�жϰ�����ˢ������
//		{
////			OSTimeDly(10);
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToReDraw(l_u8ControlCode);
//					UISN();
//					ClearKeyValue();
////					l_u8ReShowVeh	= 0x00;		//��ֹ������ʾ
//					break;
//
//				case KB_F1:		//����
//					NotReDraw(l_u8ControlCode);
//					CLEARZERO();
//					ClearKeyValue();
//					break;				
//				case KB_F2:		//����̬�л�
//					NotReDraw(l_u8ControlCode);
////					g_u8StaticMotionswitch	= (g_u8StaticMotionswitch + ENABLE_DYNAMIC) & 0x01;  
//					ClearKeyValue();
//					break;	
//				
//				case KB_F3:		//������Ϣ
//					ToReDraw(l_u8ControlCode); 
////					l_u8ReShowVeh	|= 0x01;	//����������ʾ
//					UIF3Code();	 
//					ClearKeyValue();				
//					break; 	 				
//				case KB_F4:		//��������
//					ToReDraw(l_u8ControlCode);	
////					l_u8ReShowVeh	|= 0x01;	//����������ʾ
//					UIF4Code();
//					ClearKeyValue();
//					break;					
//				case KB_F5:		//����״̬
//					ToReDraw(l_u8ControlCode);	 
////					l_u8ReShowVeh	|= 0x01;	//����������ʾ
//					UIF5Code();
//					ClearKeyValue();
//					break;	
//				
//				case KB_F6:		//��ʾ�������ƿ���
//					NotReDraw(l_u8ControlCode);
//					BackGroundReverse();
//					ClearKeyValue();	
//					break;	
//#if	 OS_TASK_CREATE_EXT_EN >0
//				case KB_F7:		//����ͳ�����
//					ToReDraw(l_u8ControlCode);
//				    DisplayStatics();
//					ClearKeyValue();
//				  	break;	
//#endif					
////				case KB_POINT:	//. �ֶ���β
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode))
//				break;
//
//			//ˢ��ͨ����ֵ��ʱ��,�¶ȣ������룬�����ͳ�����
//			if (OSSemAccept(g_pSemTime)>0)		//���ź��������벢��ʾ����
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
//			//ˢ������ʾ���� 
//			l_pAxle	= (Axle_Struct *)OSQAccept(g_pQAxleShow);		//�������̣�����Ϣ
//			if (l_pAxle!= (Axle_Struct *)0)		//��Ϣ����������Ϣ�����벢��ʾ����
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
//			//��ʾ������������
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
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//		}												    
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UIBDRoot
** Descriptions:		�궨������
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
//	uint8	l_u8ControlCode;//����ָ��
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " ϵͳӦ��ѡ��");
//		LCDPRINTC(176	, 0		, "Esc �˳�");
//		
//		LCDPRINTC(8		, 24	, "1. ��̬�ӱ궨");
//		LCDPRINTC(8		, 44	, "2. ��̬�ӱ궨");
//		LCDPRINTC(8		, 64	, "3. ϵͳ��ʼ��");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					ClearKeyValue();
//					break;
//
//				case KB_1:		//��̬�궨
//					ToReDraw(l_u8ControlCode);
//					UIBDMain(UI_MOTION);
//					ClearKeyValue();
//					break;	
//				
//				case KB_2:		//��̬�궨
//					ToReDraw(l_u8ControlCode);
//					UIBDMain(UI_STATIC);
//					ClearKeyValue();
//					break;	
//				
//				case KB_3:		//ϵͳ��ʼ��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}	
//}
/*********************************************************************************************************
** Function name:		UIBDMain
** Descriptions:		��̬�궨����
** input parameters:	p_u8Motion ����̬��־ 0����̬��1����̬ 
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
//	uint8	l_u8ControlCode;//����ָ��
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue(); 
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		if (p_u8Motion == UI_MOTION)		//��̬
//			LCDPRINTC(0		, 0		, " ��̬�궨");
//		else 				//��̬
//			LCDPRINTC(0		, 0		, " ��̬�궨");
//
//		LCDPRINTC(112	, 0		, "0 ����  Esc �˳�" );
//		
//		LCDPRINTC(8		, 24	, "1. ���궨");
//		LCDPRINTC(8		, 44	, "2. ��̨�궨");
//		LCDPRINTC(8		, 64	, "3. �ֶ��趨");
//		if (p_u8Motion == UI_MOTION)
//		{
//			LCDPRINTC(8		, 84	, "4. ��̬����");
//			LCDPRINTC(8		, 104	, "5. ��̬����");
//			LCDPRINTC(128	, 24	, "6. �����趨");
//			LCDPRINTC(128	, 44	, "7. ������߶�");
//			LCDPRINTC(128	, 64	, "8. ���³���");
//			LCDPRINTC(128	, 84	, "9. ��������");
//			LCDPRINTC(128	, 104	, "F1 ·��ֵ�߶�");
//		}
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			NotReDraw(l_u8ControlCode);
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//���궨
//					ToReDraw(l_u8ControlCode);
//					UIBDWanBanChoose(p_u8Motion);
//					break;	
//				
//				case KB_2:		//��̨�궨
//					ToReDraw(l_u8ControlCode);
//					UIBDChengTai(p_u8Motion);
//					break;	
//				
//				case KB_3:		//�ֶ��趨
//					ToReDraw(l_u8ControlCode);
//					UIBDScale(p_u8Motion);
//					break;	
//				
//				case KB_4:		//��̬����
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDStaticModify();
//					}
//					break;	
//				
//				case KB_5:		//��̬����
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDChooseMotion();
//					}
//					break;	
//				
//				case KB_6:		//�����趨
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDFullRange();
//					}
//					break;	
//				
//				case KB_7:		//������߶�
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDHeightTanxingti();
//					}
//					break;	
//				
//				case KB_8:		//���³���
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDPoDu();
//					}
//					break;	
//				
//				case KB_9:		//��������
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDChooseVehPos();
//					}
//					break;	
//				
//				case KB_0:		//�����趨
//					ToReDraw(l_u8ControlCode);
//					SaveParams();
//					break;	
//				
//				case KB_F1:		//·��ֵ�߶�
//					if (p_u8Motion == UI_MOTION)
//					{
//						ToReDraw(l_u8ControlCode);
//						UIBDRoadPlaneHeight();
//					}
//					break;	
//
//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����				
//				case KB_F2:		//������ʹ��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDWanBanChoose
** Descriptions:		��嶯̬�궨ͨ��ѡ�����
** input parameters:	p_u8Motion	����̬��־ 0��ʾ��̬��1��ʾ��̬ 
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Key;
//	uint32  l_u32Value = 0;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 112	, 239	, 112	, 1	);				
//		LCDPRINTFC(0,113,"��������[1,%d]",CHANNELNUM);
//		if (p_u8Motion == UI_MOTION)		//��̬
//			LCDPRINTC(0		, 0		, "1. ��嶯̬�궨       Esc-����");
//		else 				//��̬
//			LCDPRINTC(0		, 0		, "1. ��徲̬�궨       Esc-����");  	
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;
//				default:
//					ClearKeyValue();
//					break;	
//			} 		
//			if(l_u32Value)
//				LCDPRINTFC(0		, 19		, "ͨ����:%d     ",l_u32Value);
//			else
//			 	LCDPRINTC(0		, 19		, "ͨ����:       ");
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
//		 WAITSCREENREFRESH();		//�ȴ�ˢ���ź� 	
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDChengTai
** Descriptions:		��̨��̬�궨����
** input parameters:	p_u8Motion	����̬��0��ʾ��̬��1��ʾ��̬
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value;		//���������
//	uint8	l_u8InputStatus;	//����״̬��1��
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	l_u8InputStatus	= 0;
//	l_u32Value	= 0;
//	while(1)
//	{	 		
//		
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "2. ��̨�궨           Esc-����");
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 108	, 239	, 108	, 1);	
//		LCDPRINTC(0,109,"F1-�޸ĳ�̨����");										   		
//		while(1)
//		{
//			//��ǰ��̨����	
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
//				case KB_BKSP:		//��ɾһ������
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
//					LCDPRINTFC(16		, 30	, "��̨����:%d        ",l_u32Value);
//				else
//				{
//					LCDPRINTC(16		, 30	, "��̨����:        ");	
//					ClearLCDLine(67);
//				}
//					
//			}
//			else
//			{
//				LCDPRINTFC(16		, 30	, "��̨����:%d        ",SETUPALIAS.u16PlatformGain);
//				if(l_u8InputStatus == 3)
//					 LCDPRINTC(16		, 67	, "����ֵ���޸�!");			
//					
//			}
//
//		   	LCDPRINTC(16		, 50	, "��������:(0,65535)");
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
** Descriptions:		���ö���̬�ֶȽ���
** input parameters:	p_u8Motion ����̬��־��0��ʾ��̬��1��ʾ��̬ 
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8 *	l_pu8Scale;
//	switch (p_u8Motion)
//	{
//		case UI_STATIC:	 //��̬
//			l_pu8Scale = &SETUPALIAS.u8StaticScale;
//			break;
//			
//		case UI_MOTION:	 //��̬
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
//		//������
//		GUI_ClearSCR();
//		if (p_u8Motion == UI_MOTION)		//��̬
//			LCDPRINTC(0		, 0		, "��̬�ֶ��趨         Esc-����");
//		else 				//��̬
//			LCDPRINTC(0		, 0		, "��̬�ֶ��趨         Esc-����");
//
//		LCDPRINTC(0		, 19	, "��ѡ��ֶ�ֵ:");
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
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (*l_pu8Scale == 0)
//				*l_pu8Scale		= 50;
//
//			LCDPRINTFC(112	, 19	, "%ukg    "	, *l_pu8Scale);
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDStaticModify
** Descriptions:		���þ�̬�������棬���մ�������������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Status;		//״̬
//	uint8	l_u8Tmp1;		//��ʱ����
//	uint8	l_u8Tmp2;
//	uint8	l_u8Tmp3;
//
//	ResetControlCode(l_u8ControlCode);
//	l_u8Status	= 0;
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
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
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//				
//				case 0xf1:		//�༭
//					sprintf(m_acTmp, "��̬����%dkm-0", l_u8Status);
////					UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16StaticModify[0][l_u8Status]), 9500, 10500);
//					sprintf(m_acTmp, "��̬����%dkm-1", l_u8Status);
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
//					//�κΰ�������ʾһ���µ�����
//					l_u8Tmp2	= (l_u8Status&0x07) << 4;
//					LCDPRINTFC(0		, l_u8Tmp2	, "%d:", l_u8Status);
////					LCDPRINTFC(32		, l_u8Tmp2	, "0- %d"	, SETUPALIAS.au16StaticModify[0][l_u8Status]);
////					LCDPRINTFC(112		, l_u8Tmp2	, "1- %d"	, SETUPALIAS.au16StaticModify[1][l_u8Status]);
//					NotReDraw(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetValueParamU16
** Descriptions:		ͨ�ò������ý���,����U16����
** input parameters:	p_pcName		������
**						p_pu16Param 	����ָ��
**						p_u32Max		�������� 
**						p_u32Min		��������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value	= 0;		//��������
//	uint8	l_u8Status;		//���ܼ�״̬
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
//		//������
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s: %u",p_pcName,*p_pu16Param);
//		LCDPRINTC(0		, 0		, l_acTmp);
//		LCDPRINTC(0		, 32	, " Esc�˳�, F1���޸�");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//�޸�״̬����ʼ��������
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= KB_F1;
//						l_u32Value	= 0;
//						memset(l_acTmp, 0, 30);
//						sprintf(l_acTmp," ��������ֵ(%u-%u):",p_u32Min,p_u32Max);
//						LCDPRINTC(16		, 56	, l_acTmp);
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//ȷ������
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (l_u8Status == KB_F1)		//����״̬
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(56		, 80	, "         ");
//				else
//					LCDPRINTFC(56		, 80	, "%u      "	, l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDLineModify
** Descriptions:		������������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Status;		//���ܼ�״̬
//	uint8	l_u8Key;		//����
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
//		//������
//		GUI_ClearSCR();
//	//	LCDPRINTC(0		, 0		, "�������ٶȶ�n: 1-8  (n-1)*5km");
//	  	sprintf(l_strOut,"�������ٶȶ�[1-%d]:(n-1)*5km",SPEED_CTL_SIZE);	
//		LCDPRINTC(0,0,l_strOut);
//		DrawLineModify(g_Setup.au16Speedmodify);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//					if (l_u8Status == 0 && l_u32Value > 0)		//��ʾ��Ӧ�ٶȶ�
//					{
//						l_u8Tmp1	= (l_u32Value - 1)*5;  					
//
//						sprintf(l_strOut,"%dkm-%dkm ��ǰֵ: %d",l_u8Tmp1,l_u8Tmp1+5, SETUPALIAS.au16Speedmodify[l_u8Tmp1]);
//						LCDPRINTC(0		, 16	, l_strOut);							
//						LCDPRINTC(0,32,"Esc�˳�, F1���޸�");
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
//						sprintf(l_strOut,"�������ٶȶ�[1-%d]:%d(n-1)*5km",SPEED_CTL_SIZE,l_u32Value);	
//						LCDPRINTC(0,0,l_strOut);	
//					}
//					else
//					{
//						ToReDraw(l_u8ControlCode);
//						l_u32Value = 0;
//					}
//					ClearKeyValue();
//					break;					
//				case KB_F1:		//�޸�״̬����ʼ��������
//
//				   	sprintf(l_strOut,"�������ٶ�%dkm",l_u8Tmp1);
//					UISetValueParamU16(l_strOut, &(SETUPALIAS.au16Speedmodify[l_u8Tmp1]), 8000, 12000); 				
//						
//					if (l_u8Tmp1>1)	//������ߵĵ�
//					{
//						for(l_u8Tmp2 = 4;l_u8Tmp2 > 0;l_u8Tmp2--)
//						{
//							SETUPALIAS.au16Speedmodify[l_u8Tmp1 - l_u8Tmp2] = SETUPALIAS.au16Speedmodify[l_u8Tmp1 - 5] + (SETUPALIAS.au16Speedmodify[l_u8Tmp1] - SETUPALIAS.au16Speedmodify[l_u8Tmp1-5])*(5 - l_u8Tmp2)/5;	
//						}
//					}
//					if (l_u8Tmp1+1<MAX_SPEED_LIMIT)	//�����ұߵĵ�
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
////			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDPointModify
** Descriptions:		����������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Key;		//����
//	uint32	l_u32Value;
//	char 	l_strOut[30]={0};	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	l_u32Value	= 0;
//	while(1)
//	{	
//		//������
//		GUI_ClearSCR();
//		LCDPRINTFC(0		, 0		, "�������ٶ� 1-%d:",MAX_SPEED_LIMIT - 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_ENTER:		//ȷ������
//					if ((l_u32Value >0 && l_u32Value < MAX_SPEED_LIMIT))
//					{
//						sprintf(l_strOut,"������ %d km",l_u32Value);
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			//ˢ����Ļ
//			if (l_u32Value == 0)
//				LCDPRINTC(144	, 0		, "         ");
//			else
//				LCDPRINTFC(144	, 0		, "%u    "	, l_u32Value);
//			
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDChooseVeh
** Descriptions:		ѡ���ͽ��棬����ѹ��λ�ò�ͬ���鿴�����ݲ�ͬ
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		switch(p_u8Pos)
//		{
//#if	IF3WB
//		case POS_ABS:	//AB
//			LCDPRINTC(0		, 0		, "AB����ʻ�����ٶȵ�����");
//			break;
//
//		case POS_BCS:	//BC
//			LCDPRINTC(0		, 0		, "BC����ʻ�����ٶȵ�����");
//			break;
//
//		case POS_ABCS:	//ѹ��
//			LCDPRINTC(0		, 0		, "ѹ����ʻ�����ٶȵ�����");
//			break;
//		
//		case POS_GAP:	//ѹ������
//			LCDPRINTC(0		, 0		, "����ѹ����������");
//			break;
//#endif
//		case POS_VEH:	//��������
//			LCDPRINTC(0		, 0		, "������������");
//			break;
//
//		case POS_2WB:	//�����
//			LCDPRINTC(0		, 0		, "�����ٶ�����");
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
//		//LCDPRINTC(124	, 84	, "9.��������");
//		LCDPRINTC(124	, 104	, "Esc. �˳����� ");
//	
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//					case POS_ABCS:	//ѹ��
//#endif
//					case POS_2WB:	//�����
//						UIBDVSModifyParam(p_u8Pos, l_u8Key);
//						break;
//			
//					case POS_VEH:	//��������
//						sprintf(m_acTmp, "��������-����%d",l_u8Key);  
//						UISetValueParamU16(&m_acTmp[0], &(SETUPALIAS.au16VehTotalModify[l_u8Key-1]), 8000, 12000);
//						break;
//#if	IF3WB			
//					case POS_GAP:	//ѹ������
//						sprintf(m_acTmp, "ѹ������-����%d",l_u8Key);  
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDVSModifyParam
** Descriptions:		����ֵ���棬����ѹ��λ�úͳ���
** input parameters:	p_u8Pos		λ��
**						p_u8Veh		���� 
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
//	uint8	l_u8ControlCode;		//����ָ��
//	uint32	l_u32Value	= 0;		//��������
//	uint8	l_u8Key;
//	uint8	l_u8Status;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		switch(p_u8Pos)
//		{
//#if	IF3WB
//		case POS_ABS:	//AB
//			LCDPRINTFC(0		, 0		, "AB�峵���ٶ�����-����%d"	, p_u8Veh);
//			break;
//
//		case POS_BCS:	//BC
//			LCDPRINTFC(0		, 0		, "BC�峵���ٶ�����-����%d"	, p_u8Veh);
//			break;
//
//		case POS_ABCS:	//ѹ��
//			LCDPRINTFC(0		, 0		, "ѹ�쳵���ٶ�����-����%d"	, p_u8Veh);
//			break;
//#endif
//		case POS_2WB:	//�����
//			LCDPRINTFC(0		, 0		, "�����ٶ�����-����%d"		, p_u8Veh);
//			break;
//
//		default:
//			ToBreak(l_u8ControlCode);
//			break;
//		}		
//#if	IF3WB
//		LCDPRINTC(0		, 32	, "�������ٶ�(0-19): ");//20�������ڣ��༭��Ӧ����
//#else
//		LCDPRINTFC(0		, 32	, "�������ٶ�(0-%d): ",MAX_SPEED_LIMIT - 1 );//80�������ڣ��༭��Ӧ����
//#endif
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		l_u8Status	= 0xff;
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					if (l_u32Value == 0)
//						l_u8Status	= 0xff;
//					break;	
//				
//				case KB_ENTER:		//ȷ������
//					if (l_u8Status != 0xff)
//					{
//#if	IF3WB
//						if (l_u32Value < 20)	//20�������ڣ��༭��Ӧ����
//#else
//						if (l_u32Value < MAX_SPEED_LIMIT)	//80�������ڣ��༭��Ӧ����
//#endif
//						{
//							switch(p_u8Pos)
//							{
//#if	IF3WB		
//							case POS_ABS:	//AB
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "AB-����%d-�ٶ�%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//					
//							case POS_BCS:	//BC
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "BC-����%d-�ٶ�%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//					
//							case POS_ABCS:	//ѹ��
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "ѹ��-����%d-�ٶ�%dkm",p_u8Veh,l_u32Value);  
//								UISetValueParamU16(m_acTmp, &(SETUPALIAS.au16VehSpeedModify[p_u8Veh-1][l_u32Value]), (VSMSTANDARD*8)/10, (VSMSTANDARD*12)/10);
//								break;
//#else				
//							case POS_2WB:	//2���
////								memset(m_acTmp, 0, 30);
//								sprintf(m_acTmp, "����%d-�ٶ�%dkm",p_u8Veh,l_u32Value);  
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (l_u8Status == 0xff)
//				LCDPRINTC(144	, 32	, "         ");
//			else
//				LCDPRINTFC(144	, 32	, "%u       "	, l_u32Value);
//			
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UIBDChooseMotion
** Descriptions:		��̬����ѡ����棬����or����
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
//	uint8	l_u8ControlCode;//����ָ��
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " ��̬����");
//		
//		LCDPRINTC(176	, 0		, "Esc �˳�");
//		
//		LCDPRINTC(8		, 24	, "1. ������");
//		LCDPRINTC(8		, 44	, "2. ������");
//	
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//������
//					ToReDraw(l_u8ControlCode);
//					UIBDLineModify();
//					break;	
//				
//				case KB_2:		//������
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDFullRange
** Descriptions:		����������� 
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
//	uint8	l_u8ControlCode;//����ָ��
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "6. �����趨           Esc-����");
//		
//		LCDPRINTC(0		, 19	, "��ѡ���������:");
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
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (SETUPALIAS.u32Full == 0)
//				SETUPALIAS.u32Full	= 35000;
//			
//			LCDPRINTFC(128	, 19	, "%ukg      "	, SETUPALIAS.u32Full);
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIBDHeightTanxingti
** Descriptions:		������߶�
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
////	UISetEnableParam(" ������߶�" , &(SETUPALIAS.u8Genzong));
//	uint8 l_u8Key,l_u8ControlCode,l_u8status = 0x00;
////	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0,l_u32Value2 = 0;
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//������
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "������߶�����:");
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
//						if(l_u8status == 0x00)	 //����ͨ����
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //�����趨ֵ	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//��ɾһ������
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
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "ͨ�����:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8HeightTanxingti[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "������ֵ:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8HeightTanxingti[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "������ֵ:      ");
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
** Descriptions:		�¶���������
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
////	UISetValueParamU16("�¶�����", &(SETUPALIAS.u16Podu), 9000, 11000);
//	uint8 l_u8Key,l_u8ControlCode,l_u8status = 0x00;
////	char 	l_strOut[256]={0};
//	uint32 l_u32Value = 0,l_u32Value2 = 0;
//	ClearKeyValue();   	
//	BackGroundSave();
//	BackGroundON();
//	//������
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "���³�������:");
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
//						if(l_u8status == 0x00)	 //����ͨ����
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //�����趨ֵ	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//��ɾһ������
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
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "ͨ�����:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8LengthPodu[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "������ֵ:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8LengthPodu[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "������ֵ:      ");
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
** Descriptions:		�����ٶ�����λ��ѡ��
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
//	uint8	l_u8ControlCode;//����ָ��
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " ��������");
//		
//		LCDPRINTC(176	, 0		, "Esc �˳�");
//		LCDPRINTC(8		, 24	, "1. ������������");
//#if	IF3WB		
//		LCDPRINTC(8		, 40	, "2. AB�峵���ٶȵ�����");
//		LCDPRINTC(8		, 56	, "3. BC�峵���ٶȵ�����");	 
//		LCDPRINTC(8		, 72	, "4. ����ѹ������");
//		LCDPRINTC(8		, 88	, "5. ѹ�쳵���ٶȵ�����");
//#else
//		LCDPRINTC(8		, 40	, "2. �����ٶȵ�����");
//#endif
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//				
//				case KB_1:		//������������
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_VEH);
//					break;	
//				
//				case KB_2:		//AB�峵���ٶ�����
//					ToReDraw(l_u8ControlCode);
//#if	IF3WB
//					UIBDChooseVeh(POS_ABS);
//#else
//					UIBDChooseVeh(POS_2WB);
//#endif
//					break;
//						
//#if	IF3WB			
//				case KB_3:		//BC�峵���ٶ�����
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_BCS);
//					break;	
//				
//				case KB_4:		//ѹ�쳵���ٶ�����
//					ToReDraw(l_u8ControlCode);
//					UIBDChooseVeh(POS_ABCS);
//					break;	
//				
//				case KB_5:		//����ѹ������
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		SaveParams
** Descriptions:		�����������
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
	SETUPALIAS.u8Year		= g_DevCurTime.u16Year-2000;	// ȡ����ֵ
	SETUPALIAS.u8Month		= g_DevCurTime.u8Month;			// ȡ����ֵ
	SETUPALIAS.u8Day		= g_DevCurTime.u8Day;			// ȡ����ֵ

	  	
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
//	WriteC256(HISTORYBDADDR, l_au8Tmp, SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
	
	AddCrc16((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2);
//	RamToFlash();

	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
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
		l_u8temp = WriteC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
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
** Descriptions:		�����������
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
//	//������
//	GUI_ClearSCR();
//	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//		 LCDPRINTC(0		, 0		, "·��ֵ�߶�����:");
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
//						if(l_u8status == 0x00)	 //����ͨ����
//						{
//						   l_u32Value	*= 10;
//						   l_u32Value	+= l_u8Key;	
//						   if(l_u32Value > CHANNELNUM)
//								l_u32Value = 0;	
//						}
//						else
//						{
//						  //�����趨ֵ	 
//						   l_u32Value2	*= 10;
//						   l_u32Value2	+= l_u8Key;	
//						}		
//					NotReDraw(l_u8ControlCode);
//					ClearKeyValue();
//					break;	
//				
//				case KB_BKSP:		//��ɾһ������
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
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//				else
//				 	 LCDPRINTC(0		, 19		, "ͨ�����:       ");	
//			}
//			else if(l_u8status == 0x01 && l_u32Value !=0x00)
//			{
//				if(l_u32Value2)
//				{
//				  	LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8ZTStatus[l_u32Value - 1]);
//					LCDPRINTFC(0		, 51		, "������ֵ:%d     ",l_u32Value2);
//				}
//				else
//				{
//					LCDPRINTFC(0		, 19		, "ͨ�����:%d     ",l_u32Value);
//					LCDPRINTFC(0		, 35		, "��ǰ�趨ֵ:%d     ",SETUPALIAS.an8ZTStatus[l_u32Value - 1]);
//					LCDPRINTC(0			, 51		, "������ֵ:      ");
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
** Descriptions:		��嶯̬�궨ͨ������
** input parameters:	p_u8CID		ͨ���� 
**						p_u8Motion	����̬��־ 0��ʾ��̬��1��ʾ��̬ 
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value;		//���������
//	uint8	l_u8InputStatus;	//����״̬��1��
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
//		//������
//		GUI_ClearSCR();
//		GUI_Line(0		, 17	, 239	, 17	, 1);				
//		GUI_Line(0		, 108	, 239	, 108	, 1);				
//		
//		LCDPRINTC(0		, 0		, "1. ���궨           Esc-����");
//		
//		LCDPRINTFC(8	, 19	, " %uͨ��   "	, p_u8CID);
//		
//		LCDPRINTC(16	, 40	, "��ʼ��λ:");
//		LCDPRINTFC(96	, 40	, "%d    "	, SETUPALIAS.an32Zero[p_u8CID-1]);
//		LCDPRINTC(16	, 56	, "��ǰ����:");
//		LCDPRINTC(16	, 72	, "��ǰ����:");
//		LCDPRINTC(16	, 88	, "����ϵ��:");
//		LCDPRINTFC(96	, 88	, "%d    "	, SETUPALIAS.an32AxGain[p_u8CID-1]);
//		if (p_u8Motion == UI_STATIC)
//			LCDPRINTC(0			, 112	, "F1����  F2�궨  F3����  F4ȥƤ");
//		else
//			LCDPRINTC(0			, 112	, "F1-����  F2-�궨  F3-����ϵ��");
//
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					if (l_u8InputStatus != 0)
//					{ 
//						l_u32Value	/= 10;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//ȷ������
//					if (l_u8InputStatus == KB_F3)	//��������ϵ��
//					{
//						if (l_u32Value != 0)
//							SETUPALIAS.an32AxGain[p_u8CID-1]	= l_u32Value;
//					}
//					else if (l_u8InputStatus == KB_F2)		//����궨����
//					{
//						if (l_u32Value != 0)
//							SETUPALIAS.an32AxGain[p_u8CID-1]	= l_u32Value * 10000 / (g_an32MotionZero[p_u8CID-1] - SETUPALIAS.an32Zero[p_u8CID-1]);
//					}
//					ToReDraw(l_u8ControlCode);
//					l_u8InputStatus	= 0;
//					break;
//				
////				case KB_F1:			//F1������
////					if (l_u8InputStatus == 0)
////					{
////						SETUPALIAS.an32Zero[p_u8CID-1]	= ADAvg[p_u8CID-1];
////						ToReDraw(l_u8ControlCode);
////						
////						//���¶�̬���
////						g_an32MotionZero[p_u8CID-1]	= SETUPALIAS.an32Zero[p_u8CID-1];
////					}
////					break;
//
////				case KB_F4:			//F4����̬ȥƤ��
////					if ((l_u8InputStatus == 0) && (p_u8Motion == UI_STATIC))
////					{
////						SETUPALIAS.an32Zero[p_u8CID-1]	= ADAvg[p_u8CID-1];
////						ToReDraw(l_u8ControlCode);
////					}
////					else
////						NotReDraw(l_u8ControlCode);
////					break;
//
//				case KB_F2:			//F2���궨
//					if (l_u8InputStatus == 0)
//					{
//						l_u8InputStatus	= l_u8Key;
//					}
//					NotReDraw(l_u8ControlCode);
//					break;
//				
//				case KB_F3:			//F3������ϵ��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//			{
//				break;
//			}
//
//			//ˢ����ʾ
////			l_n32ADTmp	= ADAvg[p_u8CID-1];
//			l_n32ADTmp = 0;
//			LCDPRINTFC(96		, 56	, "%d    "	, l_n32ADTmp);				//����
//			
//			if (l_u8InputStatus != KB_F2)		//��������궨����
//			{
//					LCDPRINTFC(96		, 72	, "%d kg    ", (l_n32ADTmp-SETUPALIAS.an32Zero[p_u8CID-1])*SETUPALIAS.an32AxGain[p_u8CID-1]/10000);		//����
//			}
//			else		//����궨����
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(96		, 72	, "         ");
//				else
//					LCDPRINTFC(96		, 72	, "%u      ", l_u32Value);
//			}
//			
//			if (l_u8InputStatus == KB_F3)		//��������
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(96		, 88	, "         ");
//				else
//					LCDPRINTFC(96		, 88	, "%u      ", l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}


/*********************************************************************************************************
** Function name:		UIValidate
** Descriptions:		��֤����
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value;		//��������
//	uint32	l_u32SN;		//SNֵ
//	uint8	l_u8DigitCnt;	//���ָ���
//	uint8	l_u8Tmp1;		//��ʱ����
//	uint8	l_u8Key;
//	uint32	l_u32SNTimer;	//��Timer���ɵ�SN����
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
//		//������
//		GUI_ClearSCR();
//		LCDPRINTFC(16		, 32	, "SN:%d", l_u32SNTimer);
//		LCDPRINTC(16		, 64	, "���������: ");
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					if (l_u8DigitCnt>0)
//						l_u8DigitCnt	--;
//					l_u32Value	/= 10;
//					ClearKeyValue();
//					break;	
//				
//				case KB_ENTER:		//ϵͳ��ʼ��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			LCDPRINTC(112 + (l_u8DigitCnt<<3) 	, 64	, "  ");
//
//			for (l_u8Tmp1 = 0; l_u8Tmp1 < l_u8DigitCnt; l_u8Tmp1 ++)
//			{
//				LCDPRINTC(112 + (l_u8Tmp1<<3) 	, 64	, "*");
//			}
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		UISystemInit
** Descriptions:		ϵͳ��ʼ������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8IfPwd;		//�Ƿ�Ϊ�ڶ������������666666
//	uint32	l_u32Value;		//��������
//	uint8	l_u8Operation;	//��ʼ����������
//	uint8	l_u8Tmp1;		//��ʱ������ѭ����
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	while(1)
//	{
//		l_u8IfPwd	= 0;
//		l_u32Value	= 0;
//		l_u8Operation	= 0;
//		//������
//		GUI_ClearSCR();
//		
//		LCDPRINTC(0		, 0		, "1. ��ʼ��ΪϵͳĬ�ϲ���");
//		LCDPRINTC(0		, 16	, "2. ������У��������ʼ��");
//		LCDPRINTC(0		, 32	, "3. ��̬������ʼ��");
//		LCDPRINTC(0		, 48	, "4. ����������ʼ��");
//		LCDPRINTC(0		, 64	, "5. ��̬������ʼ��");
//		LCDPRINTC(0		, 80	, "6. �ָ�������궨����");
//		LCDPRINTC(0		, 96	, "7. �ָ�����ʷ�궨����");
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			l_u8Key	= KeyValue;
//			switch(l_u8Key)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//��ʼ��ΪϵͳĬ�ϲ���
//				case KB_2:		//������У��������ʼ��
//				case KB_3:		//��̬������ʼ��
//				case KB_4:		//����������ʼ��
//				case KB_5:		//��̬������ʼ��
//				case KB_6:		//�ָ�������궨����
//				case KB_7:		//�ָ�����ʷ�궨����
//					if (l_u8IfPwd == 0)
//					{
//						l_u8Operation	= l_u8Key;
//						for (l_u8Tmp1 = 0; l_u8Tmp1 < 8; l_u8Tmp1 ++)
//						{
//							if (l_u8Tmp1 != l_u8Operation-1)
//								ClearLCDLine(l_u8Tmp1<<4);
//						}
//						LCDPRINTC(16 	, 112	, "���������:");
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
//				case KB_ENTER:			//ȷ��
//					NotReDraw(l_u8ControlCode);
//					l_u8IfPwd	= 0xf0;	//ȷ�Ϻ��ж������Ƿ���ȷ
//					break;
//
//				case 0xff:
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				default:		//�����������˳�
//					ToBreak(l_u8ControlCode);
//					break;
//			}	//switch(KeyValue)
//			ClearKeyValue();
//					
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//			
//			//ˢ��
//			if(l_u8IfPwd == 20)	//����20���ַ�����,���˳�
//			{
//				ToBreak(l_u8ControlCode);
//				break;
//			}
//			
//			if (l_u8IfPwd == 0xf0)	//�����������
//			{
//				if (l_u32Value == 666666)		//������ȷ
//				{
//					SETUPALIAS.u8DiaoDianTag	= 0;		//ȡ�����籣�����
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
//							if (RecoverToLast())	//�ɹ�
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "��������ָ��ɹ�!");
//								//OSTimeDly(100);	   
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							else
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "��������ָ�ʧ��!");
//								//OSTimeDly(100);  
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							break;
//
//						case 7:
//							if (RecoverToHistory())	//�ɹ�
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "��ʷ�����ָ��ɹ�!");
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							else
//							{
//								GUI_ClearSCR();
//								LCDPRINTC(40 	, 56	, "��ʷ�����ָ�ʧ��!");
//								OSTimeDly(OS_TICKS_PER_SEC);
//							}
//							break;
//
//						default:
//							break;
//					}
//					GUI_ClearSCR();
//					LCDPRINTC(40 	, 56	, "��ʼ�����, ��������...  ");
//					InitRestart();
//					OSTimeDly(OS_TICKS_PER_SEC);
//				}
//				ToBreak(l_u8ControlCode);
//				break;	
//			}
//			
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		InitSystem
** Descriptions:		ϵͳ��ʼ��
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

	//wzf 2012-08-24 ������ʼֵ
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
   	   SETUPALIAS.u16UseToWghFlag[l_u16Tmp] = 0xFFFF;//20130515 ���巢��bug	
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
 //	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
	for(l_u8tempi = 0; l_u8tempi<3 ;l_u8tempi++)
	{
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
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
** Descriptions:		��ʼ���ǳ�������
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
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
		if(l_u8temp == 1)
			break;
	}
	if(l_u8temp == 1)
		g_u8RWNVFlag |= (1 << 0);
	else
		g_u8RWNVFlag &=!(1 << 0);
//	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
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
** Descriptions:		��ʼ����ֵ����
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
		l_u8temp = WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,sizeof(SETUPALIAS));	// ��0x00��ַ��д��32�ֽ�����
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
** Descriptions:		��ʼ����̬��������
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
//	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����	
//}

/*********************************************************************************************************
** Function name:		InitVehModify
** Descriptions:		��ʼ��������������
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
//	for (l_u16Tmp = 0; l_u16Tmp < 800; l_u16Tmp ++)	   //wzf 2012-09-03ԭ��320���޸ĳ�800
//		*(l_pu16Tmp++)	= 10000;
//
//	AddCrc16((uint8 *)&SETUPALIAS, SETUPPARAMSIZE - 2);
//
////	RamToFlash();
// 	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
//}
/*********************************************************************************************************
** Function name:		InitStaticModify
** Descriptions:		��ʼ����̬��������
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
////	WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
//}
/*********************************************************************************************************
** Function name:		RecoverToLast
** Descriptions:		�ָ�Ϊ����궨����
** input parameters:	None 
** output parameters:	none
** Return Value:		uint8	�Ƿ�ɹ���1���ɹ�     0��ʧ��
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
//	if(CheckCrc((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2)==0)		//ʧ��
//	{
//		return	0;
//	}
//	else	//�ɹ�
//	{
//	//	RamToFlash();
//		return	1;
//	}
//}

/*********************************************************************************************************
** Function name:		RecoverToLast
** Descriptions:		�ָ�Ϊ��ʷ�궨����
** input parameters:	None 
** output parameters:	none
** Return Value:		uint8	�Ƿ�ɹ���1���ɹ�     0��ʧ��
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
//	if(CheckCrc((uint8 *)&SETUPALIAS,SETUPPARAMSIZE - 2)==0)		//ʧ��
//	{
//		return	0;
//	}
//	else	//�ɹ�
//	{
//	//	RamToFlash();
//		WriteC256(SETUPPARAMADDR,(uint8 *)&SETUPALIAS,SETUPPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
//		return	1;
//	}
//}

/*********************************************************************************************************
** Function name:		InitNetParam
** Descriptions:		��ʼ���������
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
** Descriptions:		�����������
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
** Descriptions:		��ʼ��������
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
** Descriptions:		������������
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
** Descriptions:		���������ۼ�
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
	//��1
	ReadC256(STARTTIMESADDR, (uint8 *)&g_u32StartupTime, 4);
	OSTimeDly(10);
	g_u32StartupTime += 1;
	WriteC256(STARTTIMESADDR, (uint8 *)&g_u32StartupTime, 4);

	//��¼ʱ��
	ReadC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//�ҵ���Ч��¼��
	OSTimeDly(10);
	l_u16RecIndex	+= 1;
	l_u16RecIndex	&= 0x3ff;		//0~1023
	WriteC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//���¼�¼��
	
	if ((l_u16RecIndex & 0x1ff) == 0)		//д��һ��Sector
	{
		EraseSector(NORSTARTREC	+ (l_u16RecIndex << 3));
	}
	
	//���ɼ�¼
	l_au8RecordData[0]	= g_DevCurTime.u16Year & 0xff;	//��
	l_au8RecordData[1]	= g_DevCurTime.u16Year >> 8;
	l_au8RecordData[2]	= g_DevCurTime.u8Month;			//��
	l_au8RecordData[3]	= g_DevCurTime.u8Day;			//��
	l_au8RecordData[4]	= g_DevCurTime.u8Hour;			//ʱ
	l_au8RecordData[5]	= g_DevCurTime.u8Minute;		//��
	l_au8RecordData[6]	= g_DevCurTime.u8Second;		//��
	l_au8RecordData[7]	= 0xaa;
	//
	WriteNORFlash(NORSTARTREC + (l_u16RecIndex<<3), 8, &l_au8RecordData[0]);	

}
/*********************************************************************************************************
** Function name:		YBRestart
** Descriptions:		����
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
	WDTIM_COUNTER	= 0x100000;				/* ι��	*/ 
}

/*********************************************************************************************************
** Function name:		UISN
** Descriptions:		SN����
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value;		//��������
//	uint32	l_u32SN;		//SNֵ
//	uint8	l_u8DigitCnt;	//���ָ���
//	uint8	l_u8Tmp1;		//��ʱ����
//	uint8	l_u8Key;					  
//	uint32	l_u32SNTimer;	//��Timer���ɵ�SN����
//	uint8	l_u8SendVeh;	//�Ƿ��ͳ���
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//	
//	BackGroundSave();
//	BackGroundON();
//
//	l_u32SN	= 0;
////	g_u8EnableClearZero	= 0;	//����Ҫ����
//	l_u32SNTimer	= g_u32SN;
//	
//	while(1)
//	{
//		l_u32Value	= 0;
//		l_u8DigitCnt= 0;
//		
//		//������
//		GUI_ClearSCR();
//		LCDPRINTFC(16		, 32	, "SN: %d", l_u32SNTimer);
//		LCDPRINTC(16		, 64	, "���������: ");
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					if (l_u8DigitCnt>0)
//						l_u8DigitCnt	--;
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_ENTER:		//ϵͳ��ʼ��
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
////#if	NETACCESS > 0	//���ڴ�������						
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
//						//���Է����������ݣ�9xx
//						case 911:	//����11������
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
//						case 912:	//����12������
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
//						case 915:	//����15������
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
//						case 9115:	//����115������
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
//						case 9114:	//����114������
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
//						case 9129:	//����129������
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
//						case 9159:	//����159������
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			LCDPRINTC(112 + (l_u8DigitCnt<<3) 	, 64	, "  ");
//
//			for (l_u8Tmp1 = 0; l_u8Tmp1 < l_u8DigitCnt; l_u8Tmp1 ++)
//				LCDPRINTC(112 + (l_u8Tmp1<<3) 	, 64	, "*");
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
////	g_u8EnableClearZero	= 1;		//�ָ��Զ�����	 
//	BackGroundRecover();
//}
/*********************************************************************************************************
** Function name:		CLEARZERO
** Descriptions:		���㺯�����൱����ǰ��ldc()
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
** Descriptions:		�鿴F3��Ϣ����
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
//	//������
//	GUI_ClearSCR();
//	
//	LCDPRINTFC(0	, 0		, "�汾��: %2dNT"	, YBVERSION);
// 	LCDPRINTC(104	, 0		, PROGRAMVERSION);
//	
//	//����ʱ��
//	sprintf(l_strOut, "����ʱ��: %.4d-%.2d-%.2d %.2d:%.2d:%.2d",
//							g_DevStartTime.u16Year, 
//							g_DevStartTime.u8Month, 
//							g_DevStartTime.u8Day, 
//							g_DevStartTime.u8Hour, 
//							g_DevStartTime.u8Minute, 
//							g_DevStartTime.u8Second);
//	LCDPRINTC(0		, 18	, (char *)l_strOut);
//	
//	sprintf(l_strOut, "��������: %-5d ������: %-5d ", g_u32StartupTime	, POINTRATE);
//	LCDPRINTC(0		, 36	, (char *)l_strOut);
//	
//	sprintf(l_strOut, "SetupLen: %-5d", sizeof(SETUPALIAS));
//	LCDPRINTC(0		, 54	, (char *)l_strOut);
//								  
//#if ISDEBUG				//����״̬
//	LCDPRINTC(232	, 0		, "*");	
//
//	
//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����
//
//#if	SENDWAVEBYSP > 0	
//	LCDPRINTC(216	, 0		, "��");
//#endif								  
//#if	SENDWAVEBYNET > 0
//	LCDPRINTC(200	, 0		, "��");
//#endif
//
//#endif	//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����		
//
//#endif
//	while (KeyValue == 0xff)		//�а���
//	{
//		WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//	}
//	if (KeyValue == KB_F1)
//		ClearStartupCnt();
//
//	ClearKeyValue(); 
//	BackGroundRecover();
//}
/*********************************************************************************************************
** Function name:		UIF4Code
** Descriptions:		�鿴F4��Ϣ����
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
//	//������
//	GUI_ClearSCR();
//#if  SHOWVEHPASSDEBUG > 0	//��ʾ�������Դ���	
//	ResetControlCode(l_u8ControlCode);
//	while(1)
//	{
//	  	 
//		 LCDPRINTFC(0		, 0		, "F4����:(%x)",g_ErrMsgState);
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
//				case KB_BKSP:		//��ɾһ������
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
//				LCDPRINTFC(0		, 19		, "�������:%d        ",l_u32Value);
//			}
//			else
//			{
//			 	ClearLCDLine(19);
//				LCDPRINTC(0		, 19		, "�������:          ");
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
//				WAITSCREENREFRESH();		//�ȴ�ˢ���ź�	
//			}
//			ResetControlCode(l_u8ControlCode);
//			l_u32Value = 0;
//			ClearKeyValue();
//		 }
//	}
//	
//#else
//	LCDPRINTFC(0		, 0	, "F4���빦��δ����(%x)",g_ErrMsgState);
//	GUI_Line(0		, 17	, 239	, 17	, 1);
//    GUI_Line(0		, 127	, 239	, 127	, 1);
//	while (KeyValue == 0xff)		//�а���
//	{	
//		WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//	}
//	ClearKeyValue();   
//	BackGroundRecover();
//#endif 
//
//}
/*********************************************************************************************************
** Function name:		UIF5Code
** Descriptions:		F5������档�鿴�������ӵ�״̬��Ϣ.
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
//	uint8	l_u8ControlCode;//����ָ��
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
//		//������  	
//		LCDPRINTC(0		, 0		, "��� ״̬  Ŀ��IP       �˿�");  		
//		GUI_Line(0		, 17	, 239	, 17	, 1);  
//		GUI_Line(0		, 112	, 239	, 112	, 1); 
//		LCDPRINTC(0		, 113		, "F1--�鿴״̬˵��");  		
//		for(l_u8Tmp1 = 0; l_u8Tmp1 < 4; l_u8Tmp1++)
//		{
//			ReadTargetSocktInfo(l_u8Tmp1,&l_TargetInfo);
//			sprintf(l_strOut,"%d    %d   %d:%d:%d:%d  %d           ",l_u8Tmp1+1,l_TargetInfo.u8State,
//								l_TargetInfo.u8CnntIP[0],l_TargetInfo.u8CnntIP[1],l_TargetInfo.u8CnntIP[2],l_TargetInfo.u8CnntIP[3],
//								(l_TargetInfo.u8CnntPort[0]<<8)+l_TargetInfo.u8CnntPort[1]);
//
//			LCDPRINTC(0		, 19 +(l_u8Tmp1<<4)	,l_strOut); 	 		
//		}  	  
//	   	//�жϰ���
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
//				  	LCDPRINTC(0		, 0		, "����״̬˵��");  		
//					GUI_Line(0		, 17	, 239	, 17	, 1);  
//					GUI_Line(0		, 112	, 239	, 112	, 1); 
//					LCDPRINTC(0		, 113		, "ESC--�˳�"); 
//					LCDPRINTC(0		, 19		, "00 --- �����ѹر�");
//					LCDPRINTC(0		, 35		, "20 --- �ȴ�����"); 
//					LCDPRINTC(0		, 51		, "22 --- ��������");
//					LCDPRINTC(0		, 67		, "23 --- �ѽ�������");		  		
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
** Descriptions:		F5������档�鿴�������ӵ�״̬��Ϣ.
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
//   	sprintf(l_strOut,"ID    CPU   ջM   ջC");
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
////		WAITSCREENREFRESH();		//�ȴ�ˢ���ź���	
//	}
//   	ClearKeyValue();
// }
// #endif

 /*********************************************************************************************************
** Function name:		UICommonSet
** Descriptions:		��ͨ���ý��棬111
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
//	uint8	l_u8ControlCode;//����ָ��
//	
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, " ϵͳ����");
//		LCDPRINTC(112	, 0		, "0 ����  Esc �˳�" );
//		LCDPRINTC(8		, 24	, "1. ʱ������");
//		LCDPRINTC(8		, 40	, "2. ����������");
////		LCDPRINTC(8		, 56	, "3. ����ģʽ");
////		LCDPRINTC(8		, 72	, "4. ����ʹ��");
//		LCDPRINTC(8		, 88	, "5. ͨѶЭ��");
////		LCDPRINTC(8		, 104	, "F5 ���泵��");
//		
////		LCDPRINTC(128	, 24	, "6. ̨����");
//		LCDPRINTC(128	, 40	, "7. ��Ȧץ��");
////		LCDPRINTC(128	, 56	, "8. ���ᱨ��");
////		LCDPRINTC(128	, 72	, "9. ץ��ʹ��");
//#if PROC_DOG_EN > 0
//		LCDPRINTC(128	, 88	, "F1 ���Ź�����");
//#endif
//		LCDPRINTC(128	, 104	, "F4 ���籣��");
//
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//					ToBreak(l_u8ControlCode);
//					break;
//
//				case KB_1:		//ʱ������
//					ToReDraw(l_u8ControlCode);
//					UISetTime();
//					break;	
//				
//				case KB_2:		//����������
//					ToReDraw(l_u8ControlCode);
//					UISetBaudRate();
//					break;	
//				
////				case KB_3:		//����ģʽ
////					ToReDraw(l_u8ControlCode);
////					UISetCommandMode();
////					break;	
//				
////				case KB_4:		//����ʹ��
////					ToReDraw(l_u8ControlCode);
////					UISetForwardEnable();
////					break;	
//				
//				case KB_5:		//ͨѶЭ��
//					ToReDraw(l_u8ControlCode);
//					UISetProtocol();
//					break;	  				
//	
//				
//				case KB_7:		//��Ȧץ��
//					ToReDraw(l_u8ControlCode);
//					UISetLoop();
//					break;	
//				
////				case KB_8:		//���ᱨ��
////					ToReDraw(l_u8ControlCode);
////					UISetLunZhouEnable();
////					break;	
//				
////				case KB_9:		//ץ��ʹ��
////					ToReDraw(l_u8ControlCode);
////					UISetCapture();
////					break;	
//				
//				case KB_0:		//��������
//					ToReDraw(l_u8ControlCode);
//					SaveParams();
//					break;	
//#if PROC_DOG_EN > 0				
//				case KB_F1:		//���Ź�
//					ToReDraw(l_u8ControlCode);
//					UISetDog();
//					break;	
//#endif				
//				case KB_F4:		//���籣��
//					ToReDraw(l_u8ControlCode);
//					UISetDiaodian();
//					break;	
//				
////				case KB_F5:		//���泵��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewSetting
** Descriptions:		�鿴���ò������棬222
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
//	//������
//	ReadC256(SETUPPARAMADDR,(uint8 *)&l_ssTmp, SETUPPARAMSIZE);	
//
//	GUI_Line(0		, 18	, 239	, 18	, 1);
//	GUI_Line(0		, 124	, 239	, 124	, 1);
//	GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//	LCDPRINTC(0		, 0		, PROGRAMVERSION);
//	LCDPRINTC(176	, 0		, "��ǰ����");
////	memset(l_strOut	, 0		, 30); 
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32Zero[l_u16tmp];
//	}
//	sprintf(l_strOut	, "��λ��:%u",tmpValue);
//	LCDPRINTC(0		, 20	, l_strOut);
//	
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32AxGain[l_u16tmp];
//	}
//	sprintf(l_strOut	, "�����:%u",tmpValue);
//	LCDPRINTC(0		, 36	, l_strOut);
//	/*
//	LCDPRINTFC(0		, 36	, "��  ��:%5d,"	, l_ssTmp.an32AxGain[0]);
//	LCDPRINTFC(112		, 36	, "%d  "	, l_ssTmp.an32AxGain[1]);
//	*/	  
//	LCDPRINTFC(0		, 52	, "�ֶ�ֵ:%dkg"	, l_ssTmp.u8MotionScale);
//	  
//	LCDPRINTFC(0		, 68	, "������:%ukg"	, l_ssTmp.u32Full);
////	LCDPRINTFC(0		, 84	, "��  ��:%u"	, l_ssTmp.u8Genzong);
//
//	sprintf(l_strOut	, "�޸�����:%04d-%02d-%02d",2000+l_ssTmp.u8Year,l_ssTmp.u8Month,l_ssTmp.u8Day);
//	LCDPRINTC(0		, 104	, l_strOut); 
// 	 
//	while (KeyValue == 0xff)		//�а���
//	{
//		WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
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
//	LCDPRINTC(176	, 0		, "��ʷ����");
//
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32Zero[l_u16tmp];
//	}
//	sprintf(l_strOut	, "��  λ:%u",tmpValue);
//	LCDPRINTC(0		, 20	, l_strOut);
//	tmpValue = 0;
//	for(l_u16tmp = 0;l_u16tmp<CHANNELNUM;l_u16tmp++)
//	{
//		tmpValue +=	l_ssTmp.an32AxGain[l_u16tmp];
//	}
//	sprintf(l_strOut	, "��  ��:%u",tmpValue);
//	LCDPRINTC(0		, 36	, l_strOut);
//	/*
//	sprintf(l_strOut	, "��  λ:%d+%d=%d",l_ssTmp.an32Zero[0],l_ssTmp.an32Zero[1],l_ssTmp.an32Zero[0]+l_ssTmp.an32Zero[1]);
//	LCDPRINTC(0		, 20	, l_strOut);
//	
//	LCDPRINTFC(0	, 36	, "��  ��:%5d,"	, l_ssTmp.an32AxGain[0]);
//	LCDPRINTFC(112	, 36	, "%5d  "	, l_ssTmp.an32AxGain[1]);	
//	*/
//		  
//	LCDPRINTFC(0	, 52	, "�ֶ�ֵ:%dkg"	, l_ssTmp.u8MotionScale);
//	  
//	LCDPRINTFC(0	, 68	, "������:%ukg"	, l_ssTmp.u32Full);
////	LCDPRINTFC(0	, 84	, "��  ��:%u"	, l_ssTmp.u8Genzong);
//
////	memset(l_strOut, 0, 30);
//	sprintf(l_strOut	, "�޸�����:%04u-%02u-%02u",2000+l_ssTmp.u8Year,l_ssTmp.u8Month,l_ssTmp.u8Day);
//	LCDPRINTC(0		, 104	, l_strOut); 
// 	
//	while (KeyValue == 0xff)		//�ް���
//	{
//		WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//	}
//	ClearKeyValue();
//}
/*********************************************************************************************************
** Function name:		UIViewModify
** Descriptions:		�鿴�ٶ��������棬333������̬�������ٶ�������
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
//	//������
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
//			while (KeyValue == 0xff)		//�а���
//			{
//				WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
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
//			while (KeyValue == 0xff)		//�а���
//			{
//				WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
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
** Descriptions:		�鿴������Ϣ����
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
//	//������
//	GUI_ClearSCR();
//	ClearKeyValue();
//
//	LCDPRINTC(16		, 32	, "����: ������");
//	LCDPRINTC(16		, 64	, "�绰: 13801298463");
//	LCDPRINTC(16		, 96	, "tly001@vip.sina.com");
//	
//	GUI_Line(0		, 18	, 239	, 18	, 1);
//	GUI_Line(0		, 124	, 239	, 124	, 1);
//	GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//	while (KeyValue == 0xff)		//�а���
//	{
//		WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewIPInfo
** Descriptions:		IP������444
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
//	uint8	l_u8ControlCode;//����ָ��
//   	char 	l_strOut[30]={0};
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();	  
//		while (1)		//�жϰ�����ˢ������
//		{
//			GUI_Line(0		, 16	, 239	, 16	, 1);
//			GUI_Line(0		, 128	, 239	, 128	, 1);  			
//			LCDPRINTC(0		, 0		, " �������ò���          0-���� ");  		
//
//			sprintf(l_strOut, "1.MAC��ַ:%2x-%2x-%2x-%2x-%2x-%2x", g_IPSetLocal.au8MACAddr[0], g_IPSetLocal.au8MACAddr[1], g_IPSetLocal.au8MACAddr[2], g_IPSetLocal.au8MACAddr[3], g_IPSetLocal.au8MACAddr[4], g_IPSetLocal.au8MACAddr[5]); 
//			LCDPRINTC(0	, 17	, l_strOut);
//	
//
//			sprintf(l_strOut, "2.����IP:%d.%d.%d.%d", g_IPSetLocal.au8IPAddr[0], g_IPSetLocal.au8IPAddr[1], g_IPSetLocal.au8IPAddr[2], g_IPSetLocal.au8IPAddr[3]); 
//			LCDPRINTC(0	, 33	, l_strOut);  	
//
//			sprintf(l_strOut, "3.���ض˿�%-d,%-d,%-d,%-d", g_IPSetLocal.u32LocalPortNO,g_IPSetLocal.u32LocalPortNO+1,g_IPSetLocal.u32LocalPortNO+2,g_IPSetLocal.u32LocalPortNO+3); 
//			LCDPRINTC(0	, 48	, l_strOut); 	
//
//			sprintf(l_strOut, "4.��������%d.%d.%d.%d", g_IPSetLocal.au8SubMask[0], g_IPSetLocal.au8SubMask[1], g_IPSetLocal.au8SubMask[2], g_IPSetLocal.au8SubMask[3]); 
//			LCDPRINTC(0	, 64	, l_strOut); 			
//
//			sprintf(l_strOut, "5.Ĭ������:%d.%d.%d.%d", g_IPSetLocal.au8GatewayIP[0], g_IPSetLocal.au8GatewayIP[1], g_IPSetLocal.au8GatewayIP[2], g_IPSetLocal.au8GatewayIP[3]); 
//			LCDPRINTC(0	, 79	, l_strOut);	   	
//			ToReDraw(l_u8ControlCode); 					
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//MAC�����޸�
//					NotReDraw(l_u8ControlCode);
//					break;
//
//				case KB_2:		//����IP
//					UISetValueParamIP("����IP", g_IPSetLocal.au8IPAddr);
//					break;	
//					
//				case KB_3:		//���ض˿�
//					UISetValueParamU32("���ض˿�", &(g_IPSetLocal.u32LocalPortNO), 0, 0);
//					break;	
//					
//				case KB_4:		//��������
//					UISetValueParamIP("��������", g_IPSetLocal.au8SubMask);
//					break;	
//					
//				case KB_5:		//Ĭ������
//					UISetValueParamIP("Ĭ������", g_IPSetLocal.au8GatewayIP);
//					break;	
//					
//		//		case KB_6:		//Զ��IP
//		//			UISetValueParamIP("Զ��IP", g_IPSetLocal.au8ServerIP);
//		//			break;	
//					
//		//		case KB_7:		//Զ�̶˿�
//		//			UISetValueParamU32("Զ�̶˿�", &(g_IPSetLocal.u32ServerPortNO), 0, 0);
//		//			break;	
//					
//				case KB_0:		//����
//					SaveNetInfo();
//					ToReDraw(l_u8ControlCode);
//					break;
//					
//				case KB_I:		//��ʼ��
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//			//OSTimeDly(LOOPTIME);		//��ʱ����ѯʱ��
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewStartUpTime
** Descriptions:		�鿴ϵͳ����ʱ���¼
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
//	//������
//	GUI_ClearSCR();
//	ClearKeyValue();
//
//	ReadC256(STARTRECINDEXADDR,(uint8 *)&l_u16RecIndex,2);		//�ҵ���Ч��¼��
//	l_u16RecIndex	&= 0x3ff;
//	l_u16Tmp		= l_u16RecIndex;
//	l_u16RecIndex	++;
//	l_u16RecIndex	&= 0x3ff;
//	//l_u32StartCnt	= g_u32StartupTime;
//
//	l_u16Tmp2	= 0;		//����ʱ����������ڷ�����ʾ
//	while ((l_u32StartCnt > 0)&&(l_u16Tmp != l_u16RecIndex))
//	{
//		//��ȡ����ʱ��8B����
//		ReadNORFlash(NORSTARTREC	+ (l_u16Tmp << 3), 8, &l_au8RecData[0]);
//		
//		//�ж��Ƿ�Ϊ��Ч����
//		if (l_au8RecData[7] != 0xaa)	//��Ч
//		{
//			break;	
//		}  
//		//��ʾ����ʱ��
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
//		//��ͣ 		
//		l_u32StartCnt	--;
//		ClearKeyValue();
//		if (l_u16Tmp2 % 8 == 0)
//		{
//			while (KeyValue == 0xff)		//�а���
//			{
//				WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
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
//	if (l_u8Status & 0x01)		//�����ֹ�˳�����ȴ�����
//	{							
//		ClearKeyValue();
//		while (KeyValue == 0xff)		//�а���
//		{
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//			//OSTimeDly(LOOPTIME);		//��ʱ����ѯʱ��
//		}
//		if (KeyValue == KB_ESC)
//			return;
//		ClearKeyValue();
//		GUI_ClearSCR();
//	}
//}
/*********************************************************************************************************
** Function name:		UIViewThreshold
** Descriptions:		��ֵ������8968
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint16	l_u16TempValue = 0;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
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
//		LCDPRINTC(0		, 104	, "0.����");
//				
//		while (1)		//�жϰ�����ˢ������
//		{
//			ToReDraw(l_u8ControlCode);
//					
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//�ϳ�
//					UISetValueParamU16("UP", &(SETUPALIAS.u16UpValue), 10, 100);
//					break;
//
//				case KB_2:		//�³�
//					UISetValueParamU16("DN", &(SETUPALIAS.u16DownValue), 10, 100);
//					break;	
//					
////				case KB_3:		//�����б���ֵ
////					ClearKeyValue();
////					UISetValueParamU16("FD", &(SETUPALIAS.u16ForwardWidth), 60, 500);
////					break;	
//					
////				case KB_4:		//�˲�����
////					UISetValueParamU16("RIF", &(SETUPALIAS.u16FilterLevel), 3, 128);
////					break;	
//					
////				case KB_5:		//������С���
////					UISetValueParamU16("WT", &(SETUPALIAS.u16AxleWidth), 50, 500);
////					break;	
//				//wzf 2012-08-24
//				case KB_6:		//�ϳӵ���
//					l_u16TempValue = SETUPALIAS.u8UPPoints;
//					UISetValueParamU16("UPNUM", &l_u16TempValue, 1, 30);
//					SETUPALIAS.u8UPPoints = l_u16TempValue;
//					break;	
//				case KB_7:		//�³ӵ���
//					l_u16TempValue = SETUPALIAS.u8DownPoints;
//					UISetValueParamU16("DOWNNUM", &l_u16TempValue, 1, 30);
//					SETUPALIAS.u8DownPoints = l_u16TempValue;
//					break;			
//				case KB_8:		//�ϳ��߼�
//					l_u16TempValue = SETUPALIAS.u16logicUpValue;
//					UISetValueParamU16("LUP", &l_u16TempValue, SETUPALIAS.u16UpValue, 300);
//					SETUPALIAS.u16logicUpValue = l_u16TempValue;
//					break;	
//				case KB_9:		//�³��߼�
//					l_u16TempValue = SETUPALIAS.u16logicDownValue;
//					UISetValueParamU16("LDN", &l_u16TempValue, SETUPALIAS.u16DownValue, 300);
//					SETUPALIAS.u16logicDownValue = l_u16TempValue;
//					break;													
//				case KB_0:		//����
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}

/*********************************************************************************************************
** Function name:		UISetTime
** Descriptions:		�趨ϵͳʱ��
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value;		//��������
//	uint8	l_u8Status;		//���ܼ�״̬
//	uint16	l_u16X;			//����X
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
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "ʱ���趨:    Esc�˳�, F1���޸�");
//		
//		LCDPRINTC(16	, 20	, "��ǰʱ��:");
//		LCDPRINTC(64	, 36	, "-");
//		LCDPRINTC(88	, 36	, "-");
//		LCDPRINTC(144	, 36	, ":");
//		LCDPRINTC(168	, 36	, ":");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 125	, 239	, 125	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//			
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//�޸�״̬����ʼ��������
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= 1;
//						l_u32Value	= 0;
//						LCDPRINTC(16		, 60	,"�趨ʱ��:");
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//ȷ������
//					NotReDraw(l_u8ControlCode);
//					switch (l_u8Status)
//					{
//						case 1:		//��
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
//						case 2:		//��
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
//						case 3:		//��
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
//						case 4:		//ʱ:0~23
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
//						case 5:		//��:0~59
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
//						case 6:		//��:0~59
//							if (l_u32Value < 60)
//							{
//								tm.u8Second	= l_u32Value & 0xff;
//								LCDPRINTFC(176	, 76	, "%02d"	, l_u32Value);
//								LCDPRINTC(32		, 92	, "�������. ");	
//								LCDPRINTC(32		, 108	, "Enter:���� Esc:ȡ��");
//							}
//							else
//								ToReDraw(l_u8ControlCode);	
//							break;
//
//						case 7:		//ȷ�ϱ���
//							//����
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			LCDPRINTFC(32		, 36	, "%04d"	, g_DevCurTime.u16Year);
//			LCDPRINTFC(72		, 36	, "%02d"	, g_DevCurTime.u8Month);
//			LCDPRINTFC(96		, 36	, "%02d"	, g_DevCurTime.u8Day);
//
//			LCDPRINTFC(128		, 36	, "%02d"	, g_DevCurTime.u8Hour);
//			LCDPRINTFC(152		, 36	, "%02d"	, g_DevCurTime.u8Minute);	
//			LCDPRINTFC(176		, 36	, "%02d"	, g_DevCurTime.u8Second);
//
//			switch (l_u8Status)		//��ʾ���������
//			{
//				case 1:		//��
//					l_u16X	= 32;		
//					break;
//				
//				case 2:		//��
//					l_u16X	= 72;		
//					break;
//
//				case 3:		//��
//					l_u16X	= 96;		
//					break;
//				
//				case 4:		//ʱ:0~23
//					l_u16X	= 128;		
//					break;
//				
//				case 5:		//��:0~59
//					l_u16X	= 152;		
//					break;
//				
//				case 6:		//��:0~59
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
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		UISetBaudRate
** Descriptions:		���ò����ʽ���
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Key;
//	uint32	l_au32BR[6] = {4800, 4800, 9600, 38400, 57600, 115200};
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, "2. �������趨       Esc-����");
//		
//		LCDPRINTC(0		, 19	, "��ѡ������:");
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
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			LCDPRINTFC(112	, 19	, "%u    "	, l_au32BR[SETUPALIAS.u8BaudRate]);
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetProtocol
** Descriptions:		����Э�����
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint8	l_u8Key;
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(64	, 0		, "ͨѶЭ��:");
//		LCDPRINTC(8		, 24	, "1--����");
////		LCDPRINTC(8		, 24	, "0--����");
////		LCDPRINTC(8		, 40	, "1--��");
////		LCDPRINTC(8		, 56	, "2--����");
////		LCDPRINTC(8		, 72	, "3--����");
////		LCDPRINTC(8		, 88	, "4--����");
////		LCDPRINTC(8		, 104	, "5--�Ĵ�");
////		LCDPRINTC(128	, 24	, "6--����");
////		LCDPRINTC(128	, 40	, "7--����");
////		LCDPRINTC(128	, 56	, "8--����");
////		LCDPRINTC(128	, 72	, "9--����");
//
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
////			LCDPRINTFC(160	, 0		, "%d  ",SETUPALIAS.u8Protocol);
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
 /*********************************************************************************************************
** Function name:		UISetLoop
** Descriptions:		��Ȧ����ʹ�ܽ���
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
//	UISetEnableParam(" ��Ȧ����ʹ��" , &(SETUPALIAS.u8LoopTriggerEnable));
//}
/*********************************************************************************************************
** Function name:		UISetDog
** Descriptions:		���Ź����ý���
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
//	UISetEnableParam(" ���Ź�ʹ��" , &(SETUPALIAS.u8DOG));
//}
/*********************************************************************************************************
** Function name:		UISetDiaodian
** Descriptions:		���籣��ʹ�ܽ���
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
//	UISetEnableParam(" ���籣��ʹ��" , &(SETUPALIAS.u8DiaoDianTag));
//}
/*********************************************************************************************************
** Function name:		UISetValueParamIP
** Descriptions:		�޸�IP����
** input parameters:	p_pcName		������
**						p_pu8IP		 	����ָ��
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
//	uint8	l_u8ControlCode;		//����ָ��
//	uint32	l_u32Value	= 0;		//��������
//	uint8	l_u8Status;				//���ܼ�״̬
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
//		//������
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s",p_pcName);
//		LCDPRINTC(0		, 0		, l_acTmp);		   
//		memset(l_acTmp	, 0		, 30);
//		sprintf(l_acTmp	,"%3d.%3d.%3d.%3d"	, *p_pu8IP, *(p_pu8IP+1), *(p_pu8IP+2), *(p_pu8IP+3));
//		LCDPRINTC(24	, 20	, l_acTmp);
//		LCDPRINTC(0		, 36	, " Esc�˳�, F1���޸�");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//�޸�״̬����ʼ��������
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= 1;
//						l_u32Value	= 0;
//						LCDPRINTC(16		, 56	,"��������ֵ: ");
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//ȷ������
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
//									LCDPRINTC(32		, 92	, "�������. ");	
//									LCDPRINTC(32		, 108	, "Enter:���� Esc:ȡ��");
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
//						case 5:		//ȷ�ϱ���
//							//����
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
//			//�Ƿ��ػ�
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
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź���
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetValueParamU32
** Descriptions:		ͨ�ò������ý���,����U32����
** input parameters:	p_pcName		������
**						p_pu32Param 	����ָ��
**						p_u32Max		�������� 
**						p_u32Min		��������
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
//	uint8	l_u8ControlCode;//����ָ��
//	uint32	l_u32Value	= 0;		//��������
//	uint8	l_u8Status;		//���ܼ�״̬
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
//		//������
//		GUI_ClearSCR();
//		memset(l_acTmp, 0, 30);
//		sprintf(l_acTmp,"%s: %u",p_pcName,*p_pu32Param);
//		LCDPRINTC(0		, 0		, l_acTmp);
//		LCDPRINTC(0		, 32	, " Esc�˳�, F1���޸�");
//		
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
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
//				case KB_BKSP:		//��ɾһ������
//					NotReDraw(l_u8ControlCode);
//					l_u32Value	/= 10;
//					break;	
//				
//				case KB_F1:		//�޸�״̬����ʼ��������
//					if (l_u8Status == 0)
//					{
//						l_u8Status	= KB_F1;
//						l_u32Value	= 0;
//						memset(l_acTmp, 0, 30);
//						sprintf(l_acTmp," ��������ֵ(%u-%u):",p_u32Min,p_u32Max);
//						LCDPRINTC(16		, 56	, l_acTmp);
//					}
//					NotReDraw(l_u8ControlCode);
//					break;	
//				
//				case KB_ENTER:		//ȷ������
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (l_u8Status == KB_F1)		//����״̬
//			{
//				if (l_u32Value == 0)
//					LCDPRINTC(56		, 80	, "         ");
//				else
//					LCDPRINTFC(56		, 80	, "%u      "	, l_u32Value);
//			}
//
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//	}
//}
/*********************************************************************************************************
** Function name:		UISetEnableParam
** Descriptions:		ͨ��ʹ�ܲ������ý���
** input parameters:	p_pcName		������
**						p_pu8Param 		����ָ��
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
//	uint8	l_u8ControlCode;//����ָ��
//
//	ResetControlCode(l_u8ControlCode);
//	ClearKeyValue();
//
//	while(1)
//	{
//		//������
//		GUI_ClearSCR();
//		LCDPRINTC(0		, 0		, p_pcName);
//		LCDPRINTC(176	, 0		, "Esc ����");
//		
//		LCDPRINTC(8		, 24	, "�Ƿ�ʹ��:");
//		LCDPRINTC(8		, 44	, "1.ʹ��  2.��ֹ");
//		GUI_Line(0		, 18	, 239	, 18	, 1);
//		GUI_Line(0		, 124	, 239	, 124	, 1);
//		GUI_Line(0		, 127	, 239	, 127	, 1);
//		
//		while (1)		//�жϰ�����ˢ������
//		{
//			//�жϰ���
//			switch(KeyValue)
//			{
//				case KB_ESC:
//				case KB_ENTER:
//					ToBreak(l_u8ControlCode);
//					break;
//					
//				case KB_1:		//ʹ��
//					NotReDraw(l_u8ControlCode);
//					* p_pu8Param	= 1;
//					break;	
//						
//				case KB_2:		//��ֹ		
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
//			//�Ƿ��ػ�
//			if (IfReDraw(l_u8ControlCode)||IfBreak(l_u8ControlCode))
//				break;
//
//			//ˢ����Ļ
//			if (* p_pu8Param)		//Ϊ1ʱ��ʹ��
//				LCDPRINTC(120		, 24	, "ʹ��  ");
//			else
//				LCDPRINTC(120		, 24	, "��ֹ  ");
//			
//			WAITSCREENREFRESH();		//�ȴ�ˢ���ź�
//		}
//
//		//�Ƿ��˳�
//		if (IfBreak(l_u8ControlCode))
//			break;
//
//	}
//}
/*********************************************************************************************************
** Function name:		SaveTime
** Descriptions:		����ʱ������
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

//#if	SENDWAVEENABLE > 0		//ʹ�ܷ�����
/*********************************************************************************************************
** Function name:		UIBDSendWave
** Descriptions:		���Ͳ���ʹ�ܽ���
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
//	UISetEnableParam(" ���Ͳ���ʹ��" , &(SETUPALIAS.u8SendWaveEnable));
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
** Descriptions:		ϵͳ��ʼ��
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
	//////////////��ͬģʽ�£���λ��ʾ�ĺ��岻ͬ
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

	PROMODPARAM.m_u8StudyAlgEn[0] = 0;					  //ѧϰʹ��
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

	PROMODPARAM.m_u8EnableFlag[0] = 0;	// �Լ���Ϊ׼
	PROMODPARAM.m_u8EnableFlag[1] = 1;	// �Ƿ���ʱ�׳�ʹ��
	PROMODPARAM.m_u8EnableFlag[2] = 0;	// ������Ϣʹ�ܣ�0 ��ʹ�� 1ʹ��	���ƽ�������
	PROMODPARAM.m_u8EnableFlag[3] = 0;	// �����ϴ���ʹ�ܣ� ����Э��ͨ��
	PROMODPARAM.m_u8EnableFlag[4] = 0;	// �����ϴ�������Ϣʹ�� ���Ƶ�����Ϣ�Ƿ��ϴ�
	PROMODPARAM.m_u8EnableFlag[5] = 0;	// ������ʹ�ܣ�Ĭ��1��
	PROMODPARAM.m_u8EnableFlag[6] = 1;	// ���������� 1խ�� 2ʯӢ
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
			   
 //	WriteC256(PROMODELPARAMDDR,(uint8 *)&PROMODPARAM,PROMODPARAMSIZE);	// ��0x00��ַ��д��32�ֽ�����
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
	JDSYSPARAM.m_au16Elevation = 8141;				 //���θ߶�
	for(l_u8tempi = 0; l_u8tempi < 10; l_u8tempi++)
	{
		JDSYSPARAM.m_u16MidKeHuoThr[l_u8tempi] = 2600;			   //FZX
	}
	for(l_u8tempi = 0; l_u8tempi < 10; l_u8tempi++)
	{
		JDSYSPARAM.m_u16SmallKeHuoThr[l_u8tempi] = 2200;					   	//FB
	}

	JDSYSPARAM.m_u8LaneDir = 0;  //0 ���� 1 ˫��

	JDSYSPARAM.m_u8OrderMod = 2; //Э��ģʽ
	JDSYSPARAM.m_u8SavePeriod = 5;	//�ϴ���������
	//JDSYSPARAM.m_u8TransMod = 2;//�ϴ���ʽ 1 ���� 2 ����
	
	JDSYSPARAM.m_u32FollowVehTimeThr = 10;//��������ʱ����ֵ
	JDSYSPARAM.m_u32DiffSend = 3000; //������������ֵ
	JDSYSPARAM.m_u32WeightSendThr = 3000;
	JDSYSPARAM.m_u32AxleNumSendThr = 3000;
	JDSYSPARAM.m_u32AxleWghSendThr = 3000;
	JDSYSPARAM.m_u32MaxForAxle = 10000;

	JDSYSPARAM.m_u16SmallandMid = 3700;	//С�ͺ����ͳ�������
	JDSYSPARAM.m_u16VehNumForSend = 10000;
	JDSYSPARAM.m_u8SendVehType = 2;//��������  2ֻ�����޳��� 1����
	
	JDSYSPARAM.m_u8ProgVersion[0] = PROGVERSION_POS_0;
	JDSYSPARAM.m_u8ProgVersion[1] = PROGVERSION_POS_1;
	JDSYSPARAM.m_u8ProgVersion[2] = PROGVERSION_POS_2;
	JDSYSPARAM.m_u8ProgVersion[3] = PROGVERSION_POS_3;

	memset(JDSYSPARAM.m_au8ReserveByte,0,JDSYS_RESERVED);
	
	AddCrc16((uint8 *)&JDSYSPARAM, JDSYSPARAMSIZE-2);	 
			   
 	WriteC256(JDSYSADDR,(uint8 *)&JDSYSPARAM,JDSYSPARAMSIZE);
}
