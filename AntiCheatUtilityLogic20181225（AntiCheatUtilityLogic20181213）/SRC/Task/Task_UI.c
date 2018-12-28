/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			Task_UI.C
** Last modified Date:  20150715
** Last Version:		1.0
** Descriptions:		??????,????????
**
**--------------------------------------------------------------------------------------------------------
** Created by:			wangfeng
** Created date:		20150715
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
#define _TASK_UI_C


#include "common.h"


uint8 g_u8CheDaoNum = 1;
char g_ucTongdaoZY_Display[10];
char g_ucTongdaoNM_Display[10];
char g_ucTongdaoZero_Display[10];
uint16 g_nTongdaoZY_Change;
uint8 g_u8LCDBackGroundOn = 1;
void Task_UI(void *tdata)
{
//	uint8   l_u8StopChange;
//	uint8	l_u8Err;
	uint8   l_u8flag;
	Vehicle_Struct *pItem = (Vehicle_Struct *)0;

	Locate = 0;
	g_u8ChlForShow = 0;
	g_u8StopChange = 0;
	l_u8flag = 0;
	DrawPic();
	OSTimeDly(1000*OS_TICKS_PER_SEC/1000);
	if(BDMODE)							//拨码2为ON时，键盘不可用
	{
		pItem = &g_vehicleInfo[(g_u8vehicleID + 14)%15];
		Display_Chezhong(pItem);
		while(1)
		{
			pItem = OSQAccept(g_pSemDisplayCheliangInfo); 
			if(pItem != 0 && (Locate == Chezhong_Page) && (g_u8StopChange == 0))
			{	
				Display_Chezhong(pItem);
			}
			OSTimeDly(50*OS_TICKS_PER_SEC/1000);	
		}
	}
	else								//拨码2为OFF时，键盘可用
	{
		for(;;)
		{ 		
			pItem = OSQAccept(g_pSemDisplayCheliangInfo); 
			if((pItem != 0) && (Locate == Chezhong_Page) && (g_u8StopChange == 0))
			{
				Display_Chezhong(pItem);
			}
			if(OSSemAccept(Sem_KeyIn) > 0) 
	//	 	OSSemPend(Sem_KeyIn,0,&l_u8Err);
			{
//				 if(l_u8Err == OS_NO_ERR)
				 {
				 	switch (g_u8KeyValueMapped)
					{
						case KB_ESC:
							l_u8flag = 0;
							DoKey_ESC();
							break;
						case KB_ENTER:
							DoKey_ENTER();
							break;
						case KB_F1:
							DoKey_F1();
							break;
						case KB_F2:
							DoKey_F2();
							break;
						case KB_F3:
							DoKey_F3();
							break;
						case KB_F4:		   //
							DoKey_F4();	   //
							break;		   //
						case KB_F10:
							if(g_u8StopChange == 0)
							{
								g_u8StopChange = 1;
							}
							else
							{
								g_u8StopChange = 0;
							}
							if(g_u8StopChange == 1)
							{
								PrintCharXY(0, 0,"轴序        轴重   速度*");
							}
							else
							{
								PrintCharXY(0, 0,"轴序        轴重   速度 ");
							}
							break;
						case KB_F11:
							DoKey_F11();
							pItem = &g_vehicleInfo[(g_u8vehicleID + 14)%15];
							
							break;
						case KB_F12:
							DoKey_F12();
							break;
						case KB_0:
							l_u8flag = 1;
//							DoKey_0();
							break;
						case KB_1:
							DoKey_1();
							break;
						case KB_2:
							DoKey_2();
							break;
						case KB_3:
							DoKey_3();
							break;
						case KB_4:
							DoKey_4();
							break;
						case KB_5:
							DoKey_5();
							break;
						case KB_6:
							DoKey_6();
							break;
						case KB_7:
							DoKey_7();
							break;
						case KB_8:
							DoKey_8();
							break;
						case KB_9:
							DoKey_9();
							break;
					}
				 }
//				 Display_Chezhong();
			}
			if(l_u8flag != 0)
			{
				DoKey_0();
			}			
			OSTimeDly(50*OS_TICKS_PER_SEC/1000);  
		}
	}
}

uint8 DoKey_ESC(void)
{
	switch(Locate)
	{
		case Info_Page:
			 Display_Chezhong(&g_vehicleInfo[(g_u8vehicleID + 14)%15]);
			 Locate = Chezhong_Page;
			 break;
		case Func_Page:
		 	 DisplayInitLcd();
			 Locate = Info_Page;
			 break;
		case Chezhong_Page:
			 DrawPic();
			 Locate = Logo_Page;
			 break;			 
		case F2_Choose_Chedao_Page:
			 DisplayInitLcd();
			 Locate = Info_Page;
			 break;
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:
			 DisplayChooseTongdao_Chedao();
			 Locate = Choose_Tongdao_Chedao_Page;
			 break;			 
		case F3_Choose_Chedao_Page:
			 DisplayFunction();
			 Locate = Func_Page;
			 break;
		case Choose_Tongdao_Chedao_Page:
			 DisplayFunction();
			 Locate = Func_Page;
			 break;
		case Info_Chedao1ZY_Page:
		case Info_Chedao2ZY_Page:
		case Info_Chedao3ZY_Page:
		case Info_Chedao4ZY_Page:
			 DisplayChooseChedao();
			 Locate = F3_Choose_Chedao_Page;
			 break;
		case ProgramVersion:			 //		   rp
			 DisplayInitLcd();			 //
			 Locate = Info_Page;		 //
			 break;						 //
		case Change_Chedao1ZY_Page:
		case Change_Chedao2ZY_Page:
		case Change_Chedao3ZY_Page:
		case Change_Chedao4ZY_Page:
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					DisplayChedao1_zengyi();
					Locate = Info_Chedao1ZY_Page;
					break;
				case 2:
					DisplayChedao2_zengyi();
					Locate = Info_Chedao2ZY_Page;
					break;
				case 3:
					DisplayChedao3_zengyi();
					Locate = Info_Chedao3ZY_Page;
					break;
				case 4:
					DisplayChedao4_zengyi();
					Locate = Info_Chedao4ZY_Page;
					break;
			}
		}
		break;
		case Change_TongDao1ZY_Page:					//	  rp
		case Change_TongDao2ZY_Page:					 //
		case Change_TongDao3ZY_Page:					 //
		case Change_TongDao4ZY_Page:					 //
		case Change_TongDao5ZY_Page:
		case Change_TongDao6ZY_Page:
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
			g_nTongdaoZY_Change = 0;
		}
		break;
		case Chedao_Neima_Page:
		{
			switch (g_u8CheDaoNum)
			{
				case 1:
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
		}
		break;
	}			
	return 0;
}

uint8 DoKey_ENTER(void)
{
	switch(Locate)
	{
		case Logo_Page:
		     Display_Chezhong(&g_vehicleInfo[(g_u8vehicleID + 14)%15]);	 		 
			 Locate = Chezhong_Page;
			 break;
		case Chezhong_Page:
			 DisplayInitLcd();
			 Locate = Info_Page;
			 break;
		case Change_TongDao1ZY_Page:					//保存通道增益，保存后返回上一级	  rp
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 1);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 1);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 1);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 1);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
		}
		break;	
		case Change_TongDao2ZY_Page:					 //
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 2);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 2);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 2);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 2);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
		}
		break;
		case Change_TongDao3ZY_Page:					 //
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 3);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 3);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 3);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 3);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
		}
		break;
		case Change_TongDao4ZY_Page:					 //
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 4);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 4);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 4);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 4);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}

		}
		break;
		case Change_TongDao5ZY_Page:
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 5);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 5);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 5);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 5);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}	
		}
		break;
		case Change_TongDao6ZY_Page:
		{
			switch(g_u8CheDaoNum)
			{
				case 1:
					Save_TongdaoZY(1, 6);
					DisplayTongdaoZY_Chedao1();
					Locate = Info_Chedao1_TongDaoZY_Page;
					break;
				case 2:
					Save_TongdaoZY(2, 6);
					DisplayTongdaoZY_Chedao2();
					Locate = Info_Chedao2_TongDaoZY_Page;
					break;
				case 3:
					Save_TongdaoZY(3, 6);
					DisplayTongdaoZY_Chedao3();
					Locate = Info_Chedao3_TongDaoZY_Page;
					break;
				case 4:
					Save_TongdaoZY(4, 6);
					DisplayTongdaoZY_Chedao4();
					Locate = Info_Chedao4_TongDaoZY_Page;
					break;
			}
		}
		break;
//		switch(g_u8CheDaoNum)								//保存结束返回上一级
//			{
//			case 1:
//				DisplayTongdaoZY_Chedao1();
//				Locate = Info_Chedao1_TongDaoZY_Page;
//				break;
//			case 2:
//				DisplayTongdaoZY_Chedao2();
//				Locate = Info_Chedao2_TongDaoZY_Page;
//				break;
//			case 3:
//				DisplayTongdaoZY_Chedao3();
//				Locate = Info_Chedao3_TongDaoZY_Page;
//				break;
//			case 4:
//				DisplayTongdaoZY_Chedao4();
//				Locate = Info_Chedao4_TongDaoZY_Page;
//				break;
//			}
					 
	}
	return 0;
}

uint8  DoKey_F1(void)
{
	
	return 0;
}

uint8  DoKey_F2(void)
{
	if(Locate == Info_Page)
	{
	 	DisplayChooseChedao();
		Locate = F2_Choose_Chedao_Page;
		return 0;	
	}
	else
	{return 0;}
}

uint8  DoKey_F3(void)
{
	switch(Locate)
	{
		case Info_Page:
	 		 DisplayFunction();
			 Locate = Func_Page;
			 break;	
		case Info_Chedao1ZY_Page:
		// 修改车道1增益
			 DisplayChange_Chedao1ZY();				  //rp
			 g_u8CheDaoNum = 1;
			 Locate = Change_Chedao1ZY_Page;			  //
			 break;
		case Info_Chedao2ZY_Page:
		// 修改车道2增益
			 DisplayChange_Chedao2ZY();				  //rp
			 g_u8CheDaoNum = 2;
			 Locate = Change_Chedao2ZY_Page;			  //
			 break;
		case Info_Chedao3ZY_Page:
		// 修改车道3增益
			 DisplayChange_Chedao3ZY();				  //rp
			 g_u8CheDaoNum = 3;
			 Locate = Change_Chedao3ZY_Page;			  //
			 break;
		case Info_Chedao4ZY_Page:
		// 修改车道4增益
			 DisplayChange_Chedao4ZY();				  //rp
			 g_u8CheDaoNum = 4;
			 Locate = Change_Chedao4ZY_Page;			  //
			 break;
	}
	return 0;
}
	
uint8  DoKey_F4(void)			  //			rp
{								  //
	if(Locate == Info_Page)		  //
	{							  //
		 DisplayProgramVersion(); //
		 Locate = ProgramVersion; //
		 return 0;				  //
	}							  //
	else						  //
	{							  //
		return 0;				  //
	}							  //
}								  //

 uint8   DoKey_F11(void)
 {
 	uint8 l_u8break;
	l_u8break = 1;
	GUI_ClearSCR();
	PrintCharXY(20,20,"显示车道选择:");
	PrintFormatDataXY(160, 20, "%d", g_u8ChlForShow);
	while(1)
	{
		OSTimeDly(50*OS_TICKS_PER_SEC/1000);
		if(OSSemAccept(Sem_KeyIn) > 0)
		{
			switch(g_u8KeyValueMapped)
			{
				case 0:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 1:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 2:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 3:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 4:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 5:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 6:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 7:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case 8:
					g_u8ChlForShow = g_u8KeyValueMapped;
				break;
				case KB_ESC:
					l_u8break = 0;
				break;
			}
			if(l_u8break == 1)
			{	
				PrintFormatDataXY(160, 20, "%d", g_u8ChlForShow);
			}
		}
		if(l_u8break == 0)
		{
			break;
		}
	}
	return 0;
 }

uint8 DoKey_F12(void)
{
	if(g_u8LCDBackGroundOn == 1)
	{
		BackGroundOFF();
		g_u8LCDBackGroundOn = 0;
	}
	else
	{
		BackGroundON();
		g_u8LCDBackGroundOn = 1;
	}
	return 0;
}
uint8  DoKey_0(void)
{
	switch(Locate)
	{
		case Change_Chedao1ZY_Page:
			 //save车道1增益;
			 break;
		case Change_Chedao2ZY_Page:
			 //save车道2增益;
			 break;
		case Change_Chedao3ZY_Page:
			 //save车道3增益;
			 break;
		case Change_Chedao4ZY_Page:
			 //save车道4增益;
			 break;
		case Info_Chedao1_TongDaoZY_Page:			 //内码查询
			 DisplayChedao1_Neima();
			 g_u8CheDaoNum = 1;
			 Locate = Chedao_Neima_Page;
			 break;
		case Info_Chedao2_TongDaoZY_Page:
			 DisplayChedao2_Neima();
			 g_u8CheDaoNum = 2;
			 Locate = Chedao_Neima_Page;
			 break;
		case Info_Chedao3_TongDaoZY_Page:
			 DisplayChedao3_Neima();
			 g_u8CheDaoNum = 3;
			 Locate = Chedao_Neima_Page;
			 break;
		case Info_Chedao4_TongDaoZY_Page:
			 DisplayChedao4_Neima();
			 g_u8CheDaoNum = 4;
			 Locate = Chedao_Neima_Page;
			 break;
		case Change_TongDao1ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 0;
			 DisplayChange_TongDao6ZY();
			 break;
	}
return 0;
}

uint8  DoKey_1(void)
{
	switch(Locate)
	{
		case Func_Page:								   
			 DisplayChooseChedao();
			 Locate = F3_Choose_Chedao_Page;
			 break;
		case F3_Choose_Chedao_Page:
			 DisplayChedao1_zengyi();
			 g_u8CheDaoNum = 1;
			 Locate = Info_Chedao1ZY_Page;
			 break;
		case F2_Choose_Chedao_Page:					  //rp
//			 DisplayChooseTongdao();
			 g_u8CheDaoNum = 1;					  	 //*
			 DisplayInitLcd();
//		 	 Locate = Choose_Tongdao_Page;
			 Locate = Info_Page;
		 	 break;
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:						   //rp
			 DisplayChange_TongDao1ZY();
			 Locate = Change_TongDao1ZY_Page;
			 break;
		case Choose_Tongdao_Chedao_Page:
			 DisplayTongdaoZY_Chedao1();
			 g_u8CheDaoNum = 1;								  
			 Locate = Info_Chedao1_TongDaoZY_Page;
			 break;
		case Change_TongDao1ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 1;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}
uint8  DoKey_2(void)
{
	switch(Locate)
	{
		case Func_Page:
			 DisplayChooseTongdao_Chedao();
			 Locate = Choose_Tongdao_Chedao_Page;
			 break;
		case F3_Choose_Chedao_Page:
			 DisplayChedao2_zengyi();
			 g_u8CheDaoNum = 2;
			 Locate = Info_Chedao2ZY_Page;
			 break;
		case F2_Choose_Chedao_Page:					  //rp
//			 DisplayChooseTongdao();
			 g_u8CheDaoNum = 2;					  	  //*
			 DisplayInitLcd();
//		 	 Locate = Choose_Tongdao_Page;
			 Locate = Info_Page;
		 	 break;
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:						   //rp
			 DisplayChange_TongDao2ZY();
			 Locate = Change_TongDao2ZY_Page;
			 break;
		case Choose_Tongdao_Chedao_Page:
			 DisplayTongdaoZY_Chedao2();
			 g_u8CheDaoNum = 2;
			 Locate = Info_Chedao2_TongDaoZY_Page;
			 break;
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 2;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}

uint8  DoKey_3(void)
{
	switch(Locate)
	{
		case Func_Page:
			 //????
			 break;
		case F3_Choose_Chedao_Page:
			 DisplayChedao3_zengyi();
			 g_u8CheDaoNum = 3;
			 Locate = Info_Chedao3ZY_Page;
			 break;
		case F2_Choose_Chedao_Page:					  //rp
//			 DisplayChooseTongdao();
			 g_u8CheDaoNum = 3;					  	  //*
			 DisplayInitLcd();
//		 	 Locate = Choose_Tongdao_Page;
			 Locate = Info_Page;
		 	 break;
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:						   //rp
			 DisplayChange_TongDao3ZY();
			 Locate = Change_TongDao3ZY_Page;
			 break;
		case Choose_Tongdao_Chedao_Page:
			 DisplayTongdaoZY_Chedao3();
			 g_u8CheDaoNum = 3;
			 Locate = Info_Chedao3_TongDaoZY_Page;
			 break;
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 3;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}

uint8  DoKey_4(void)
{
	switch(Locate)
	{
		case F3_Choose_Chedao_Page:
			 DisplayChedao4_zengyi();
			 g_u8CheDaoNum = 4;
			 Locate = Info_Chedao4ZY_Page;
			 break;
		case F2_Choose_Chedao_Page:					  //rp
//			 DisplayChooseTongdao();
			 g_u8CheDaoNum = 4;					  	  //*
			 DisplayInitLcd();
//		 	 Locate = Choose_Tongdao_Page;
			 Locate = Info_Page;
		 	 break;
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:						   //rp
			 DisplayChange_TongDao4ZY();
			 Locate = Change_TongDao4ZY_Page;
			 break;
		case Choose_Tongdao_Chedao_Page:
			 DisplayTongdaoZY_Chedao4();
			 g_u8CheDaoNum = 4;
			 Locate = Info_Chedao4_TongDaoZY_Page;
			 break;
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 4;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}


uint8  DoKey_5(void)
{
	switch(Locate)
	{
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:
			 DisplayChange_TongDao5ZY();
			 Locate = Change_TongDao5ZY_Page;
			 break;
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 5;
			 DisplayChange_TongDao6ZY();
			 break;	
	}
return 0;
}
uint8  DoKey_6(void)
{
	switch(Locate)
	{
		case Info_Chedao1_TongDaoZY_Page:
		case Info_Chedao2_TongDaoZY_Page:
		case Info_Chedao3_TongDaoZY_Page:
		case Info_Chedao4_TongDaoZY_Page:
			 DisplayChange_TongDao6ZY();
			 Locate = Change_TongDao6ZY_Page;
			 break;
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 6;
			 DisplayChange_TongDao6ZY();
			 break;
		

	}
return 0;
}
uint8  DoKey_7(void)
{
	switch(Locate)
	{
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 7;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}
uint8  DoKey_8(void)
{
	switch(Locate)
	{
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 8;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}
uint8  DoKey_9(void)
{
	switch(Locate)
	{
		case Change_TongDao1ZY_Page:									 //???????
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao1ZY();
			 break;
		case Change_TongDao2ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao2ZY();
			 break;
		case Change_TongDao3ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao3ZY();
			 break;
		case Change_TongDao4ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao4ZY();
			 break;
		case Change_TongDao5ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao5ZY();
			 break;
		case Change_TongDao6ZY_Page:
			 g_nTongdaoZY_Change = g_nTongdaoZY_Change * 10 + 9;
			 DisplayChange_TongDao6ZY();
			 break;
	}
	return 0;
}

