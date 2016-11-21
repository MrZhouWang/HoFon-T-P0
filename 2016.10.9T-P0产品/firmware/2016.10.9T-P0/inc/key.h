
#ifndef _KEY_H_
#define _KEY_H_

/************************************************************************************/
#ifdef  AP_KEY_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_KEY_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_KEY_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/


#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1
#define CMD_KEY_ADD		2
#define CMD_KEY_SHT		3
#define CMD_KEY_CFG_L	4
#define CMD_KEY_CFG_S	5

#define KEYFUN_ADD			0x80            // +键按下
#define KEYFUN_SUB			0x20            // -键按下
#define KEYFUN_FORWARD		0x08            // 前进键按下
#define KEYFUN_BACK			0x40            // 后退键按下
#define KEYFUN_FUNC			0x10            // 功能键按下

#define KEYFUN_OK			0x01            // OK键按下
#define KEYFUN_FUNC_L		0x02            // 功能键长按
#define KEYFUN_NOKEY		0x00            // 无按键

//#define TIMER_COUNT_100mS	100
//#define TIMER_COUNT_1S	500
//#define TIMER_COUNT_3S	800

#define TIMER_COUNT_32ms	3//16//15

//#define	KEYTIME_SHORT		1				//短按触发
//#define KEYTIME_LONG		30				//长按触发	16bit
//#define KEYTIME_FREE		1				//按键释放  8bit
#define KEYLONGTIME			200//45//30			//长按时间，32ms为单位
//#define KEYLONGPROTIME		3				//长按间隔执行时间，32ms为单位



// 函数声明
AP_KEY_EXT void	PutFinishedToStack_Com( TASK_INFO XDATA *pt1,TASK_INFO XDATA *pt2 );	  // 备份数据
AP_KEY_EXT void Key_Scan(void);															  // 按键扫描及处理
AP_KEY_EXT void	Exit_Cfg_Mode(void);													  // 退出配置模式


#endif
