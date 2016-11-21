#ifndef _Glob_H_
#define _Glob_H_



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
//==================================================================================

//#define MAX_SRCODE_NUM 	20	//最大接收的显示信息
#define MAX_SRCODE_NUM 	12	//最大接收的显示信息
typedef struct 
{
	INT8VU 	  sku;			  			// 库位
	INT16VU   count;		  			// 数量
	INT8VU 	  full;			  			// 满箱操作
	INT8VU 	  srcode_len;	  			// 批次数据长度
	INT8VU 	  srcode[MAX_SRCODE_NUM]; 	// 批次信息
}TASK_INFO;

#define MAX_TASK_NUM 	2	//最大的作业任务数量
typedef struct 
{
	INT8VU 	   num;			  		// 计数
	INT8VU	   total;				// 总任务个数
	INT8VU	   remain;				// 剩余任务个数
	INT8VU	   repeat;				// 重复接收到任务标志
	TASK_INFO  info[MAX_TASK_NUM];	// 任务信息
}TASK_STR;

#define MAX_BACK_NUM	5	//最大回显信息支持
typedef struct 
{
	INT8VU 	   num;			  		// 计数
	INT8VU	   index;				// 索引
	TASK_INFO  info[MAX_BACK_NUM];	// 任务信息
}BACK_TASK_STR;

AP_CPU_EXT 	TASK_STR      XDATA FinishTask;			// 已完成任务信息保存数组
AP_CPU_EXT 	TASK_STR      XDATA UnfinishTask;		// 未完成任务信息保存数组
AP_CPU_EXT 	BACK_TASK_STR XDATA BackTask;			// 已完成任务信息备份回显数组

typedef struct 
{
	INT16VU TaskCurCount;				// 当前任务数量备份，
	INT8VU	LastSoftGetOkCount;			// 上位机软件下发此次命令之前，由0X03号命令采集到的已经OK响应的组数
	INT8VU  CurDispMode ;				// 对应于3种显示模式：0-无内容；1-显示字符串；3-显示分拣任务(1,3实现)
										// 00为非运行状态,01,03为运行状态
	INT8VU  beSettingAddr;				// 0:非设置站址		1:设置站址		2：站址设定已经按下OK键
	INT8VU  SystemLocked;				// 锁定系统、解锁系统
}OS_TASK_STR;

AP_CPU_EXT 	OS_TASK_STR  DATA OsCmdSet;			// 命令设置系统重要参数

//--------------------------------------------------------------------------------------
// LED指示灯
typedef struct 
{
	INT8VU 	light_type;			// 亮灯类型
	INT8VU 	light_type_BK;		// 亮灯类型备份	 
	INT8VU 	flash_mode;			// 闪烁模式
	INT8VU 	flash_status;		// 闪烁状态
	INT16VU	flash_period;		// 闪烁周期基准，可调节
	INT16VU time_refresh;		// 定时刷新时间		系统时钟里自减
}LED_STR;

AP_CPU_EXT 	 LED_STR DATA led;	   // 指示灯组结构体变量
//--------------------------------------------------------------------------------------
// 显示
typedef struct{
	
	INT8VU	f_Refresh;		// 数据刷新标志
	INT8VU	f_Loading;		// 正在载入标志
	INT8VU	f_Rolling;		// 滚屏标志
	INT8VU	f_OffDisp;		// 灭显示标志

	INT8VU	Focus;			// 焦点位
	INT8VU	MenuPage;		// 菜单页
	INT8VU	Shift;			// 移动

	INT8VU	Page;			// 显示页面
	INT8VU	PageBak;		// 显示页面
	INT16VU Value;			// 计数值

	INT16VU TimeRefresh;		 // 定时刷新时间		系统时钟里自减
	INT8VU  RollDelayCnt;		 // 滚屏定时时间		系统时钟里自减
//	INT16VU TatolTimeRefresh16;  // 总的定时刷新时间	系统时钟里自减
	INT8VU  TatolTimeRefresh8;   // 总的定时刷新时间	系统时钟里自减(单位100ms)
//	INT32VU TatolTimeRefresh32;  // 总的定时刷新时间	系统时钟里自减
	INT16VU TatolTimeRefresh16;  // 总的定时刷新时间	系统时钟里自减(单位100ms)
} DISPLAY_STR;

AP_CPU_EXT 	 DISPLAY_STR DATA Disp;	   // 显示结构体变量
//--------------------------------------------------------------------------------------
// 数码管
#if( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
	#define LED_NUMBER		7		  		//数码管数量
#else
	#define LED_NUMBER		6		  		//数码管数量
#endif

typedef struct 
{
	INT8VU 	  Srcode[MAX_SRCODE_NUM];	// 批次信息
	INT8VU 	  SrLen;			 		// 批次信息长度
	INT8VU 	  SrIndex;			 		// 对应批次滚动位置
	INT8VU 	  num[LED_NUMBER];			// 对应数码管显示数据
	INT8VU	  CntLen;					// 数量窗长度
	INT8VU 	  CntIndex;			 		// 对应数量滚动位置
	INT8VU 	  Order;			 		// 对应显示第几位
}DIGELED_STR;

AP_CPU_EXT	DIGELED_STR	IDATA DigiLed;	// 数码管结构体

#define LED_RANKCOUNT	8			//点阵模组显示内容行数



AP_CPU_EXT INT8VU  XDATA SRCodeData[MAX_SRCODE_NUM+1][LED_RANKCOUNT];	//滚屏数据
AP_CPU_EXT INT8VU  XDATA CountCodeData[DIGILED_COUNT+1][LED_RANKCOUNT];	//滚屏数据
AP_CPU_EXT INT8VU  XDATA SKUCodeData[DIGILED_SKU+1][LED_RANKCOUNT];		//滚屏数据
//--------------------------------------------------------------------------------------

AP_CPU_EXT UINT8  IDATA	Buffer[16];		// 数据缓存
//--------------------------------------------------------------------------------------
// 按键定义
typedef struct
{
	INT8VU	f_Short;			//按键释放标志 0释放
	INT8VU	f_Long;				//按键长按标志 1长按 	
	INT8VU	f_LongOnce;			//按键长按执行一次标志
	INT8VU	f_Enable;			//按键扫描使能标志
	INT8VU  f_IsFnKeyPressed;	//功能键是否已经按下（再拍OK键之前）

	INT8VU	SysCnt;				//对系统时钟计时  ，系统时钟里自加
	INT8VU	OnCnt;				//按下计数时间，单位为32ms
	INT8VU	Backup;				//保存上次的键值
	INT8VU	BackProssVal;		//保存上次执行的键值
//	INT8U	ScanCnt;			//扫描计数
	INT8VU  f_LongFunc;			//功能键长按 
	INT8VU  CfgPwdCnt;			//配置密码次数
} KEY_STR;

AP_CPU_EXT	KEY_STR   DATA Key;	  // 按键设置变量的结构体
// 按键设置的模式
typedef struct
{
	INT8VU	f_Config;		// 系统配置标志位,=OFF -> 不在配置状态，=ON ->在配置状态
	INT8VU	f_ChangeValue;	// 工作显示数值状态下，表示是否正在修改焦点位，以区分（修改焦点位下，高位0需要显示，否则不需要显示）	
	INT8VU	f_BackDisp;		// 回显状态标志	   OFF=关
}KEY_MODE_STR;

AP_CPU_EXT	KEY_MODE_STR	 DATA KeyMode;		 // 按键设置的模式 的结构体
//--------------------------------------------------------------------------------------
// 串口
#define UART_BUF_LEN	50//串口缓冲器大小
typedef struct
{
	INT8VU wLen;					// 发送数量
	INT8VU wBuffer[UART_BUF_LEN];	// 发送缓冲区
	INT8VU rIndex;					// 接收缓冲区数量
	INT8VU rBuffer[UART_BUF_LEN];	// 接收缓冲区
	INT8VU rTimer;					// 计时器
	INT8VU rFlag;			 		// 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
}OS_UART;

AP_CPU_EXT  OS_UART XDATA uart;		// 串口发送接收结构体
//--------------------------------------------------------------------------------------
// 参数存储
#define MAX_CFGPARA       	6      	 //配置参数最大个数
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 //地址(1~255)
		INT8VU 	  FinishedLED;			 //完成后的LED亮灯情况(0,1,2,3,4,5,6,7-->N,R,G,B,Y,粉，青，白)
		INT8VU 	  WtTime;				 //进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 //功能键定义(0,1->零拣,满箱操作)
		INT8VU 	  DispMode;				 //显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
		INT8VU 	  KeyProssLED;			 //OK键按下指示灯(0,1,2,3,4,5,6,7,8-->N,R,G,B,Y,粉，青，白,8前一种灯色+1)，（800U 触摸芯片）
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;


AP_CPU_EXT	STORAGE_UNI  IDATA  para;		   // 参数联合
AP_CPU_EXT	STORAGE_UNI  IDATA  para_bk;	   // 备份参数联合
//--------------------------------------------------------------------------------------
// 系统
typedef struct{
	

	INT8VU  Base_100ms;		 // 基准时间100ms		

} OS_TIME;

AP_CPU_EXT 	OS_TIME  DATA os_time;


// 系统显示设置
typedef struct{
	

	INT8VU   DispLed_sku;		 // 库位显示窗位数
	INT8VU   DispLed_count;		 // 数量显示窗位数
	INT32VU  DispMax_count;		 // 最大显示数量
	INT8VU   DispLed_first;		 // 显示起始位
	INT8VU	 Chg_Task_Cnt_first; // 修改任务数量的起始位 
			

} OS_DISPSET;

AP_CPU_EXT 	OS_DISPSET  DATA os_disp_set;

//==================================================================================

#endif
