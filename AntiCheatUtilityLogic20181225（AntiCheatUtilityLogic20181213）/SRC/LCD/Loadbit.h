/****************************************************************************************
* 文件名：LOADBIT.H
* 功能：显示单色图形及汉字显示。(头文件)
* 作者：黄绍斌
* 日期：2004.02.26
* 备注：
****************************************************************************************/
#ifndef  LOADBIT_H
#define  LOADBIT_H

extern uint8 const hanzi[];
/****************************************************************************
* 名称：GUI_LoadPic()
* 功能：输出单色图形数据，反白显示。
* 入口参数： x		指定显示位置，x坐标
*           y		指定显示位置，y坐标
*           dat		要输出显示的数据
*           hno     要显示此行的点个数
*           lno     要显示此列的点个数
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
extern  void  GUI_LoadPic(uint32 x, uint32 y, uint8 *dat, uint32 hno, uint32 lno);


/****************************************************************************
* 名称：GUI_LoadPic1()
* 功能：输出单色图形数据，反相显示。
* 入口参数： x		指定显示位置，x坐标
*           y		指定显示位置，y坐标
*           dat		要输出显示的数据
*           hno     要显示此行的点个数
*           lno     要显示此列的点个数
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
extern  void  GUI_LoadPic1(uint32 x, uint32 y, uint8 *dat, uint32 hno, uint32 lno);


/****************************************************************************
* 名称：GUI_PutHZ()
* 功能：显示汉字。
* 入口参数： x		指定显示位置，x坐标
*           y		指定显示位置，y坐标
*           dat		要输出显示的汉字点阵数据
*           hno     要显示此行的点个数
*           lno     要显示此列的点个数
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
extern  void  GUI_PutHZ(uint32 x, uint32 y, uint8 *dat, uint8 hno, uint8 lno);


extern void GUI_HZ(uint32 x, uint32 y, char *str);

extern void GUI_PutString8_16(uint32 x, uint32 y, char *str);

extern void GUI_PutString8_16A(uint32 x, uint32 y, char str);
void GUI_hz(uint32 x, uint32 y, char p1,char p2)  ;


#endif
