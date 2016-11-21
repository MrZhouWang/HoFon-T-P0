#ifndef _MAIN_H_
#define _MAIN_H_

/********************************************************************************************************
*                                              Global CONSTANS
********************************************************************************************************/
/***********************************************************************************
* 变量声明方法(参考uC/OS_II.H)
*	1、在 extern_symbol.h 中每个全局变量都加上 AP_CPU_EXT 的前缀
* 	2、在 main.c 中产生如下效果：  	
*      	#define OS_CPU_GLOBALS：OS_CPU_EXT 被强制为 ‘ ’(空)
*		#include "Head File/extern_symbol.h"：所有变量被定义 
*	3、在其余文件中产生如下效果：
*		未声明 OS_CPU_GLOBALS：OS_CPU_EXT 被强制为 extern
*		#include "Head File/extern_symbol.h"：所有变量被 ‘extern’ 声明、
*	4、在 main.c 中同时有 #include "Head File/parameter_map.h" 其过程描述为：
*		A、在 extern_symbol.h 中首先定义结构类型，并定义变量及分配空间)
*		B、然后在 parameter_map.h 中对变量进行初始化
* 		C、其他文件中 引用 extern_symbol.h，此时首先定义结构类型，然后再做 extern 声明
*	5、注意：在 main.c 中，必须先 #define OS_CPU_GLOBALS 然后再引用 parameter_map.h
************************************************************************************/
#ifdef  AP_CPU_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_CPU_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_CPU_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

//#define TIMER_DATA_CHANGE	50      //100ms

#define TIMER_BASE_100MS    100    //10ms
#define TIMER_CONFIG        (5000/TIMER_BASE_100MS)    //5s ,单位为100ms
#define TIMER_MODIFY        (5000/TIMER_BASE_100MS)    //5S ,单位为100ms

// 数码管修改数据时焦点位闪烁时间
//#define COUNT_FLICKER_ON	    500				// 闪烁亮的时间
//#define COUNT_FLICKER_OFF		200				// 闪烁灭的时间
#define COUNT_FLICKER_ON	    250				// 闪烁亮的时间
#define COUNT_FLICKER_OFF		150				// 闪烁灭的时间

// 进度条等待模式及时间
#define DIGITAL_LED_LOAD_CNT_PER	100			// 进度条等待定时更新时间
#define DIGITAL_LED_LOAD_TIME_LONG  (800000/TIMER_BASE_100MS)    	// 1=1S

// 显示芯片定时初始化
#define DISP_IC_INI_PER	1000			// 显示芯片定时初始化，单位为100ms

//================================================================================
AP_CPU_EXT 	INT8U	CODE  Version[];
// 函数声明

AP_CPU_EXT	void sys_data_reset(void);		  // 任务信息清除，地址设置标志清除，进度条标志清除
AP_CPU_EXT	void DelayMs(UINT16 usDelay);	  // 延Xms，单位为1ms
AP_CPU_EXT	void sys_disp_set(void);		  // 设置显示的库位位数和数量位数

//编译条件
//*************************************************************************
//MCU型号 做为编译条件
#define	STC12C5616AD	0
#define	STC15W1K16S		1


//产品型号 做为编译条件
#define	P0_910U			0				  //  0个数码管显示（0个库位、0个数量），0个Func键和一个OK键，有指示灯组
#define	P3_910U			1				  //  3个数码管显示（0个库位、3个数量），3个Func键和一个OK键，有指示灯组
#define	P6_910U			2				  //  6个数码管显示（2个库位、4个数量），3个Func键和一个OK键，有指示灯组
#define	P6_910U_G3R3	3				  //  6个数码管显示（3个库位、3个数量），3个Func键和一个OK键，有指示灯组
#define	P10_910U		4				  //  8个数码模组显示（3个批次信息、2个库位、4个数量），3个Func键和一个OK键，有指示灯组
#define	P10_910U_R10	5				  //  8个数码模组显示（3个批次信息、2个库位、4个数量），3个Func键和一个OK键，有指示灯组
#define	P6_910U_R6		6				  //  6个数码管显示（0个库位、5个数量），3个Func键和一个OK键，有指示灯组

#define	A0_820			10				  //  0个数码管显示（0个库位、0个数量），0个Func键和一个OK键，有指示灯组
#define	A2_820			11				  //  2个数码管显示（0个库位、3个数量），3个Func键和一个OK键，有指示灯组
#define	A4_820			12				  //  4个数码管显示（2个库位、2个数量），3个Func键和一个OK键，有指示灯组
#define	A10S_820		13				  //  8个数码模组显示（3个批次信息、2个库位、4个数量），3个Func键和一个OK键，有指示灯组

#define	P0_920U			20				  //  0个数码管显示（0个库位、0个数量），0个Func键和一个OK键，有指示灯组
#define	P3_920U			21				  //  3个数码管显示（0个库位、3个数量），3个Func键和一个OK键，有指示灯组
#define	P6_920U			22				  //  6个数码管显示（2个库位、4个数量），3个Func键和一个OK键，有指示灯组
#define	P6_920U_G3R3	23				  //  6个数码管显示（3个库位、3个数量），3个Func键和一个OK键，有指示灯组
#define	P10_920U		24				  //  8个数码模组显示（3个批次信息、2个库位、4个数量），3个Func键和一个OK键，有指示灯组
#define	P10_920U_R10	25				  //  8个数码模组显示（3个批次信息、2个库位、4个数量），3个Func键和一个OK键，有指示灯组


#define	MACHINETYPE  P0_910U
//#define	MACHINETYPE  P3_910U
//#define	MACHINETYPE  P6_910U
//#define	MACHINETYPE  P6_910U_G3R3	 // 
//#define	MACHINETYPE  P6_910U_R6
//#define	MACHINETYPE  P10_910U
//#define	MACHINETYPE  P10_910U_R10

//#define	MACHINETYPE  A0_820
//#define	MACHINETYPE  A2_820
//#define	MACHINETYPE  A4_820
//#define	MACHINETYPE  A10S_820

//#define	MACHINETYPE  P0_920U
//#define	MACHINETYPE  P3_920U
//#define	MACHINETYPE  P6_920U
//#define	MACHINETYPE  P6_920U_G3R3
//#define	MACHINETYPE  P10_920U
//#define	MACHINETYPE  P10_920U_R10

//#define	MCUTYPE  STC12C5616AD
#define	MCUTYPE  STC15W1K16S

// 根据MCU型号宏定义对应变化
#if	 (MCUTYPE == STC12C5616AD)
	#define	RV_MCU	" STC12C5616AD "
	#include "STC_NEW_8051.H"
#elif(MCUTYPE == STC15W1K16S)
	#define	RV_MCU	" STC15W1K16S "
	#include "STC15Fxxxx.H"
#endif

#define		  COUNT_MAX		5			// 通讯格式里数量为16位数据，最大范围为65535，5位数据

// 根据机型型号宏定义对应变化
#if	 (MACHINETYPE == P0_910U)
	#include "910U_P0.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P0 "			  // 编译后提示信息为P0
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		0			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     0 			  // 最大显示数量
	#define		DIGILED_FIRST		5		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P3_910U)
	#include "910U_P3.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P3 "			  // 编译后提示信息为P3
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		3			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     1000 		  // 最大显示数量
	#define		DIGILED_FIRST		3		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P6_910U_G3R3)
	#include "910U_P6.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P6_G3R3 "	  // 编译后提示信息为P6-G3R3
	#define		DIGILED_SKU			3			  // 库位显示窗位数
	#define		DIGILED_COUNT		3			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     1000 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P6_910U)
	#include "910U_P6.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P6 "			  // 编译后提示信息为P6
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		4			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     10000		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P6_910U_R6)
	#include "910U_P6.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P6_R6 "		// 编译后提示信息为P6
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P10_910U)
	#include "910U_P10.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P10 "		  // 编译后提示信息为P10
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		1		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P10_910U_R10)
	#include "910U_P10.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 910U-P10_R10 "	  // 编译后提示信息为P10_R10
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		1		  	  // 修改任务数量的起始位

// 820系列
#elif (MACHINETYPE == A0_820)
	#include "820A0.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 820A0 "			  // 编译后提示信息为P0
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		0			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     0 			  // 最大显示数量
	#define		DIGILED_FIRST		5		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == A2_820)
	#include "820A2.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 820A2 "			  // 编译后提示信息为P3
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		2			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     100 		  // 最大显示数量
	#define		DIGILED_FIRST		4		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == A4_820)
	#include "820A4.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 820A4 "			  // 编译后提示信息为P6
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		2			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     100		  	  // 最大显示数量
	#define		DIGILED_FIRST		2		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == A10S_820)
	#include "820A10S.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 820A10S "		  // 编译后提示信息为P10
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		1		  	  // 修改任务数量的起始位

//920系列
#elif	 (MACHINETYPE == P0_920U)
	#include "920U_P0.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P0 "			  // 编译后提示信息为P0
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		0			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     0 			  // 最大显示数量
	#define		DIGILED_FIRST		5		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P3_920U)
	#include "920U_P3.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P3 "			  // 编译后提示信息为P3
	#define		DIGILED_SKU			0			  // 库位显示窗位数
	#define		DIGILED_COUNT		3			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     1000 		  // 最大显示数量
	#define		DIGILED_FIRST		3		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P6_920U_G3R3)
	#include "920U_P6.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P6_G3R3 "	  // 编译后提示信息为P6-G3R3
	#define		DIGILED_SKU			3			  // 库位显示窗位数
	#define		DIGILED_COUNT		3			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     1000 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P6_920U)
	#include "920U_P6.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P6 "			  // 编译后提示信息为P6
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		4			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     10000		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		0		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P10_920U)
	#include "920U_P10.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P10 "		  // 编译后提示信息为P10
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		1		  	  // 修改任务数量的起始位

#elif(MACHINETYPE == P10_920U_R10)
	#include "920U_P10.H"						  // 包含对应的机型头文件,涉及硬件
	#define		RV_MACHINE	" 920U-P10_R10 "	  // 编译后提示信息为P10_R10
	#define		DIGILED_SKU			2			  // 库位显示窗位数
	#define		DIGILED_COUNT		5			  // 数量显示窗位数
	#define 	DISPLAY_MAX_NUM     65536 		  // 最大显示数量
	#define		DIGILED_FIRST		0		  	  // 显示起始位
	#define		CHG_TASK_COUNT_FIRST		1		  	  // 修改任务数量的起始位    
#endif


#endif
