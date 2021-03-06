
#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/**********************************************************************************************************
*                    定义与编译器无关的数据类型
**********************************************************************************************************/

typedef unsigned char  BOOLEAN;                 /* 布尔变量                                 */
typedef unsigned char  INT8U;                   /* 无符号8位整型变量                        */
typedef signed   char  INT8S;                   /* 有符号8位整型变量                        */
typedef unsigned short INT16U;                  /* 无符号16位整型变量                       */
typedef signed   short INT16S;                  /* 有符号16位整型变量                       */
typedef unsigned int   INT32U;                  /* 无符号32位整型变量                       */
typedef signed   int   INT32S;                  /* 有符号32位整型变量                       */
typedef float          FP32;                    /* 单精度浮点数（32位长度）                 */
typedef double         FP64;                    /* 双精度浮点数（64位长度）                 */

typedef INT32U         OS_STK;                  /* 堆栈是32位宽度                           */

/* 以下是兼容UC/OS V1.XX的数据类型，在uC/OS-II没有使用  */
#define BYTE           INT8S
#define UBYTE          INT8U
#define WORD           INT16S
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U


/* *********************************************************************************************************
*                    与ARM7体系结构相关的一些定义
**********************************************************************************************************/
#define     OS_CRITICAL_METHOD     2            /* 选择开、关中断的方式 */

__swi(0x01) void OsSwiHandle1(int Handle);
__swi(0x02) void *OsSwiHandle2(int Handle, int Index);

#define OS_TASK_SW() OsSwiHandle1(0)                        /* 任务级任务切换函数          */
#define _OSStartHighRdy() OsSwiHandle1(6)                   /* 运行优先级最高的任务        */
#define OS_ENTER_CRITICAL()  OsSwiHandle1(1)                /* 关中断                      */
#define OS_EXIT_CRITICAL()  OsSwiHandle1(2)                 /* 开中断                      */

#define GetOSFunctionAddr(Index) OsSwiHandle2(10, Index)    /*  获取系统服务函数入口        */
#define GetUsrFunctionAddr(Index) OsSwiHandle2(11, Index)   /*  获取自定义服务函数入口      */
#define OSISRBegin()  OsSwiHandle1(3)                       /*  中断开始处理                */
#define OSISRNeedSwap() OsSwiHandle1(9)                     /*  判断中断是否需要切换        */

#define ChangeToSYSMode() OsSwiHandle1(4)                   /*  任务切换到系统模式          */
#define ChangeToUSRMode() OsSwiHandle1(5)                   /*  任务切换到用户模式          */
#define TaskIsARM(prio) OsSwiHandle2(7, prio)               /*  任务代码是ARM代码           */
#define TaskIsTHUMB(prio) OsSwiHandle2(8, prio)             /*  任务代码是THUMB             */

#define OS_STK_GROWTH    1                      /*  堆栈是从上往下长的          */

#define     USR32Mode       0x10                /*  用户模式                    */
#define     SYS32Mode       0x1f                /*  系统模式                    */
#define     NoInt           0x80

#ifndef USER_USING_MODE
#define	USER_USING_MODE	USR32Mode               /*  任务缺省模式                 */
#endif

#ifndef OS_SELF_EN
#define	OS_SELF_EN     0                        /*  允许返回OS与任务分别编译、固化*/    
#endif


OS_CPU_EXT INT32U OsEnterSum;                   /*  关中断计数器（开关中断的信号量）    */


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
