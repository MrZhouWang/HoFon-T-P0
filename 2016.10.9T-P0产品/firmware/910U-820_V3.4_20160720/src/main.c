/*=========================================================
// 程序功能说明
// F1:   任务完成亮灯状态，0灭，1红，2绿
// F2：  任务等待间隔时间，
// F3：  功能键长按定义，
// F4：  显示模式，


// 程序更改说明
//  2014年8月6日更改  版本v2.0
//	 1.显示开机版本，显示LED各段方便检测LED好坏。
//	   字库保持原来的基础上把库扩成128的字库，方便以后更改。
//   2.修正910U_P6的F1，F2，F3，F4均显示F1的错误

//  2014年9月12日更改  版本v2.1
//	 1.非显示字符集、非配置状态(显示数值状态)下，货位号为FF，则灭显示。

//  2014年9月22日更改  版本R3G3 v2.1
//	 1.R3G3 版本，3个绿数码管，3个红LED数码管
//     用条件R3G3做编译条件，回传参数，灭显示，焦点位变更，数量最大值，均做对应变更

//  2014年10月10日更改  版本R3G3 v2.2
//   1.PutFinishedToStack()函数超过回显个数的批次数据保存有错
//	 2.增加灯模式0=灭，1=红，2=绿，3=蓝，4=红绿，5=红蓝，6=绿蓝，7=红绿蓝
//     改led_light_ctrl	函数，对应设置上面8种模式
//	 


// 程序更改说明	 2014-11-11 V3.0
1. 把P0，P3，P6，P10整合到一个文件，重新写各模块，优化，变量重新命名，还有宏定义
2. 单片有STC12C5616AD改用STC15W1K16S
3. 灯改成7色的，但开机仍然亮3色，F1参数支持7色，F5参数支持7色
4. 回显存储Bug更改
5. 库位FF改灭显示
6. 等待时，关指示灯
7. 启用F1功能时，因为任务完成后不发清除命令，所以下一次任务有问题，进入不了第一次,改善此
   bug，通过接收任务数码命令置标志，如果该标志为TRUE则认为重发的任务，接收到读取OK键和清除任务信息清此标志

// 程序更改说明	 2014-12-15 V3.1
1. 按键扫描时间缩短为6ms  以前的程序间隔扫描为5ms

// 程序更改说明	 2015-01-05 V3.2
1. 增加820A0，820A2，820A4，820A10S	系列
2. 单片统一用STC15W1K16S，内含晶振、复位模块，RAM 1K，ROM 16K
3. 增加边上的OK灯
4. 取消NULL显示界面

// 程序更改说明	 2015-12-15 V3.3
1.添加组播功能
// 程序更改说明	 2016-1-28 V3.3
1.配置参数的按键在长按FN键基础上增加按4次>键 ，显示----

// 程序更改说明	 2016-7-20 V3.4
1. 进入配置参数的密码状态，不再接收通讯数据，避免要设置单个标签要停掉整条总线的通讯。
2. 新增一个版本P6-910-R6，用F4设置成2，能改成5位显示，取消库位显示。不用此方法
3. P6系列根据F数值，=2则0库位5数量，否则2库位4数量
4. 更改F4 = 1时，显示批次号信息对应数码管位置少了个起始位置，非P6显示不准确。
=========================================================*/


#define AP_CPU_GLOBALS	
#include "extern_symbol.h" 


// 开机显示版本号
#if( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
INT8U	CODE  Version[]={' ',' ',' ','v','3','.','3'};	   //3A=0.；3B=1.；3C=2.；3D=3.；3E=4.；3F=5.；
#else
INT8U	CODE  Version[]={' ',' ',' ','V',0x3D,'4'};	   //3A=0.；3B=1.；3C=2.；3D=3.；3E=4.；3F=5.；
#endif
/************************************************************************
* 编译信息说明
************************************************************************/
#pragma message("Compiled on "__DATE__" at "__TIME__"")			// 提示编译时间及日期

//#pragma message(""RV_MARKET"")
//#pragma message(""RV_FIR"")
//#pragma message(""RV_WIR"")
//#pragma message(""RV_Debug"")
#pragma message(""RV_MACHINE"")								   // 提示编译的机型
#pragma message(""RV_MCU"")									   // 提示编译的MCU
/*******************************************************************************
	 					系统参数复位
*******************************************************************************/
void sys_data_reset(void)
{
    INT8U i;
	
    UnfinishTask.num    = 0; 			 // 未完成任务清0
	UnfinishTask.remain = 0;    		 // 剩余任务清0
    FinishTask.num      = 0;			 // 已完成任务清0
	OsCmdSet.beSettingAddr 	    = 0;	 // 地址设定清0
	OsCmdSet.LastSoftGetOkCount = 0;	 // 上位机已采集到OK按键次数清0
	UnfinishTask.repeat	 = FALSE;		 // 清重复接收到任务标志

    for(i=0; i< MAX_TASK_NUM; i++)
    {	//未完成任务块
//        task_unfinish_NO[i]   = 0;    
//        task_un_goods_numH[i] = 0;
//        task_un_goods_numL[i] = 0; 
		UnfinishTask.info[i].sku = 0;
		UnfinishTask.info[i].count = 0;
        //已完成任务块
//        task_finished_NO[i]   = 0;    
//        task_ed_goods_numH[i] = 0;
//        task_ed_goods_numL[i] = 0;
//		taks_finished_Full[i] = 0; 
		FinishTask.info[i].sku = 0;
		FinishTask.info[i].count = 0;
		FinishTask.info[i].full = 0;
    }
	
	Disp.f_Loading = LOAD_MODE_OFF;	     	   // 进度条等待显示标志清除
//	sys.beWaitNewData 	= OFF;			   // 等待新数据标志清除
}

/*******************************************************************************
	 					系统显示参数设置
*******************************************************************************/
void sys_disp_set(void)
{
#if( (MACHINETYPE == P6_910U)||(MACHINETYPE == P6_910U_G3R3)||(MACHINETYPE == P6_920U)||(MACHINETYPE == P6_920U_G3R3)||(MACHINETYPE == P6_910U_R6) )
	if( para.str.DispMode == 0X02 )	//字符显示型
	{
		os_disp_set.DispLed_sku   		= 0;
		os_disp_set.DispLed_count 		= 5;
		os_disp_set.DispMax_count 		= 65536;
		os_disp_set.DispLed_first 		= 0;
		os_disp_set.Chg_Task_Cnt_first 	= 0;		
	}
	else
#endif
	{
		os_disp_set.DispLed_sku   		= DIGILED_SKU;
		os_disp_set.DispLed_count 		= DIGILED_COUNT;
		os_disp_set.DispMax_count 		= DISPLAY_MAX_NUM;
		os_disp_set.DispLed_first 		= DIGILED_FIRST;
		os_disp_set.Chg_Task_Cnt_first 	= CHG_TASK_COUNT_FIRST;	 
	}
}

/*******************************************************************************
	 					1毫秒延迟函数
*******************************************************************************/
void Delay1Ms() 
{
    INT16U i = 852;

    while (i != 0) {i--;}
}

/*******************************************************************************
	 					毫秒延迟函数
    input   usDelay     延迟时间值
	output  none
*******************************************************************************/

void DelayMs(UINT16 usDelay)
{
    for(; usDelay>0; usDelay--)
    { Delay1Ms(); }   
}

/*******************************************************************************
	 					  端口初始化
STC12C56XXAD系列单片机所有IO口均可由软件配置成4种工作类型之一，4种类型分别为：
准双向口/弱上拉(标准8051输出模式)、强推挽输出/强上拉、仅为输入(高阻)、开漏输出功能。
每个口由2个控制寄存器中的相应位控制每个引脚工作类型。上电复位后默认为准双向口/弱上拉(标准8051输出模式)。
2V以上为高电平，0.8V以下为低电平。每个IO口驱动能力均可达到20mA，但整个芯片最大不得超过55mA。
PXM0[7:0] 和 PXM1[7:0]  2字节设置(X=0,1,2,3..对应P0,P1,P2,P3..) [7:0]对应端口的0到7位
 0				0	准双向口/弱上拉(标准8051输出模式)，灌电流可达20mA，拉电流150uA~250uA
 0				1	强推挽输出(强上拉输出,可达20mA，要加限流电阻)
 1				0	仅为输入（高阻）
 1				1	开漏（Open Drain），内部上拉电阻断开，要外加
*******************************************************************************/
void IO_Init(void)
{
	//寄存器配置P2.6=>	BLUE, P2.7=>GREEN;
//	P2M0 = 0xC0;	
	//寄存器配置P3.7=>RED;  P3.7改成串口 P0.2=>RED
//	P3M0 = 0x80;	
//	P0M0 = 0x04;
	//IO状态初始化
	All_Digital_Led_Pwr_OFF();	  // 关闭所有数码管电源

	// 任务指示灯组初始化
	LED_R_OFF;				//RED_LED    ==> OFF
	LED_G_OFF;				//GREEN_LED  ==> OFF
	LED_B_OFF;				//BLUE_LED   ==> OFF

	//UART 收发指示灯IO配置	
	LED_TXT_OFF;
	LED_RXT_OFF;
}

/***************************************************
		T0定时器初始化
***************************************************/
#define TH0_INIT        0xf8
#define TL0_INIT        0xcc

void InitT0(void)
{
    //定时1ms//默认为12T模式=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  				//这配置是2ms

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0
}

/*******************************************************************************
	定时器T0中断服务程序(2ms中断)
	Input :		中断向量
	Output :	sys.flag 置为有效
*******************************************************************************/

void timer0_int (void) interrupt 1
{ 
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

//	LED_WRN_OFF;
//    sys.led_scan_flag  = ON;			 // 使能数码管刷新
    //串口缓冲区有数据
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        { 
			uart.rFlag  = ON; 
		}
    }
//	//处理主机下发命令
//	Cmd_Process();	

	// 定时产生按键扫描标志
	if( (Key.SysCnt++ >= TIMER_COUNT_32ms ) )
	{  Key.f_Enable = TRUE; Key.SysCnt = 0;	}					

	// 16位定时更新显示缓存计数器，自减1
	if (Disp.TimeRefresh != 0){ Disp.TimeRefresh--; }

	// 16位Led 闪烁时间，自减1
	if (led.time_refresh != 0){ led.time_refresh--; }

	// 8位数码管 滚屏时间，自减1
	if (Disp.RollDelayCnt != 0){ Disp.RollDelayCnt--; }

	// 100ms 计时
	if(os_time.Base_100ms-- == 0)
	{
		os_time.Base_100ms = TIMER_BASE_100MS;

		// 单位为100ms
		// 8位超时递减计算器，自减1	//配置状态下 和 作业时修改数值状态下 共用做为超时退出
	    if (Disp.TatolTimeRefresh8 != 0){ Disp.TatolTimeRefresh8--; }
		
		// 16位超时递减计算器，自减1	// 进度等待时间
	    if (Disp.TatolTimeRefresh16 != 0){ Disp.TatolTimeRefresh16--; } 		
	}

    //挨个扫描数码管
    led_Scan();
//	LED_WRN_ON;
}

/*******************************************************************************
	 					开机自检测程序
    input:   none     
	output:  none
*******************************************************************************/

void Start_Init(void)
{	
	OsCmdSet.CurDispMode  = DISPMODE_NONE;  	// 当前任务状态设为非运行状态
	OsCmdSet.SystemLocked = OFF;				// 锁定系统、解锁系统；默认解锁
	BackTask.num = 0;			   				// 回显备份区的保存总个数
	os_time.Base_100ms    = TIMER_BASE_100MS;	// 基准时间
	Disp.f_Rolling        = ROLL_MODE_OFF;

//	sys.led_scan_flag = ON;		// 使能数码管刷新
	Display_Clear();			// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志

	sys_data_reset();			// 任务信息初始化
}

/*******************************************************************************
	 						main program				
*******************************************************************************/   
int main(void) 
{   

	IO_Init();			   	//IO口初始化，状态灯初始化

	DelayMs(200);    		//等待电源稳定

	EA = 0;
	digital_led_cntrl_Init();// 数码管芯片初始化
	InitT0();				 // 系统时钟，2ms基准

	LoadParaInEeprom();		 // 读EEProm	,判断是否越界，存入Ram	   
	sys_disp_set();			 // 设置显示窗对应库位位数和数量位数
	UART_Init();			 // 串口初始化
	Start_Init();			 // 重要数据初始化
						
	EA = 1;//使能中断

	Start_Display();		 //完成LED显示8和灯亮
	MAX485_REV_EN;			 // 允许接收
	////////////////////////////main()模块函数说明//////////////////////////// 
	//  1)CMD_led_flash():       3组LED指示灯闪烁控制
	//  2)LedDispBuffRefresh():  数码管显示数据更新
	//  3)Key_Scan():   		 4个按键扫描及处理	
	//  4)Cmd_Process():		处理主机通过485下传的命令
	/////////////////////////////////////////////////////////////////////////////////////////
	while(1)
	{	
		// 3 组led指示灯闪烁控制
		CMD_led_flash();
		//按键扫描及处理
		Key_Scan();		
		// 数码管数据更新
		LedDispBuffRefresh();					 	
		//处理主机下发命令
		Cmd_Process();		
	}
}

/*******************************************************************************
								END					
*******************************************************************************/

