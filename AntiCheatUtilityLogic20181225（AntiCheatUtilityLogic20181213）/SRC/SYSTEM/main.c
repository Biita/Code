/****************************************Copyright (c)****************************************************
**                         			BEIJING	WANJI(WJ)                               
**                                     
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			main.c
** Last modified Date:  2011-03-31
** Last Version:		1.0
** Descriptions:		�������ļ�
**
** ����ѡ��궨��˵����
**	C/C++:
**		USEMODE:	�û�ģʽ��������ʹ�á���ESAMģ���У�����У��ESAM����EEP��
**		PUBMODE��	����ģʽ������������ʹ�á���ESAMģ���У�����дCSK��дEEP��
**		UPDATEMODE������ģʽ��������ˢ������ESAMģ���У����Զ�SN����EEP��дEEP	
**		LCD��		ʹ�ñ�׼LCD�����壬���ر�׼LCD������Ӧ��
** 		LCDNEW��	��LCD������3250�������ϵ���ʾ��������Ӧ��
**		__STRG_NAND:	NANDFlash�洢����������NAND���������Ӧ��
**		__STRG_NOR:	NORFlash�洢����������NOR���������Ӧ��
**		__STRG_SD��	SD������������SD�����������Ӧ��
**		__UCOS��	UCOSģʽ��д����Flashʱ����ж�
**
**	ASM:
**		__FLASHMODE��	�ڴ�ģʽ����Flash�м��س�����˳����ٴ򿪻��档
**						����״̬ʱ��ɾ���ú궨�塣
**
**
**--------------------------------------------------------------------------------------------------------
** Created by:		
** Created date:		
** Version:			
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define  EXT_GLOBAL

#include "common.h"

 int main(void)
 {
//#if PROC_DOG_EN > 0	 	 	 
	FeedExternDog();		//ι�ⲿ��
//#endif
	
	OSInit();                   	//��ʼ�� uC/OS-II
	FeedExternDog();		//ι�ⲿ��
	JZInit();	
	FeedExternDog();		//ι�ⲿ��
	RunStartTask();					//��ʼ�������� 
	
  OSStart();                 		//��ʼ������

	return(1);
 }
