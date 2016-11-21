#ifndef _LED_H_
#define _LED_H_

/************************************************************************************/
#ifdef  AP_LED_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_LED_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_LED_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

//led light 亮灯模式
#define LED_NONE	0				  // 不亮灯
#define LED_RED		1				  // 点亮红灯
#define LED_GREEN	2				  // 点亮绿灯
#define LED_BLUE	3				  // 点亮蓝灯

#define LED_RG		4				  // 点亮红绿灯
#define LED_RB		5				  // 点亮红蓝灯
#define LED_GB		6				  // 点亮绿蓝灯
#define LED_RGB		7				  // 点亮红绿蓝灯

//led 灯闪烁比例模式
//9种亮灭时间比例
#define LED_FLASH_MODE_ALWAYS		0			// 常亮
#define LED_FLASH_MODE_1V1			1			// 亮灭时间： 1V1， 表示亮 1*100ms，灭 1*100ms；
#define LED_FLASH_MODE_1V2			2			// 亮灭时间： 1V2， 表示亮 1*100ms，灭 2*100ms；
#define LED_FLASH_MODE_1V5			3			// 亮灭时间： 1V5,  表示亮 1*100ms，灭 5*100ms；
#define LED_FLASH_MODE_1V10			4			// 亮灭时间： 1V10, 表示亮 1*100ms，灭10*100ms；
#define LED_FLASH_MODE_2V1			5			// 亮灭时间： 2V1， 表示亮 2*100ms，灭 1*100ms
#define LED_FLASH_MODE_5V1			6			// 亮灭时间： 5V1， 表示亮 5*100ms，灭 1*100ms
#define LED_FLASH_MODE_10V1			7			// 亮灭时间：10V1， 表示亮10*100ms，灭 1*100ms

//数码管显示从左到右分别为第1位-->第6位
#define DIGITAL_LED1	0 //第1位数码管
#define DIGITAL_LED2	1 //第2位数码管
#define DIGITAL_LED3	2 //第3位数码管
#define DIGITAL_LED4	3 //第4位数码管
#define DIGITAL_LED5	4 //第5位数码管
#define DIGITAL_LED6	5 //第6位数码管

//#define DIGITAL_LED_PWR_ON	0 //数码管电源开关状态
//#define DIGITAL_LED_PWR_OFF	1 //数码管电源开关状态
//
//#define DIGITAL_LED_PWR_ALLOFF	0x1f 	//数码管1~5 关灭

//====================================================
// 进度条显示模式
#define LOAD_MODE_OFF	0		   // 进度模式关
#define LOAD_MODE_SHORT	1		   // 进度模式短时间
#define LOAD_MODE_LONG	2		   // 进度模式长时间


//#define LED_NUMBER		6		  //数码管数量
//#define LED_NOT_DISPLAY 0xff
//#define LED_TBL_MAX     37
#define LED_TBL_MAX     128		  // LED字库最大数量
#define DIGI_LED_OFF    0x20	  // 空格
//#define DISPLAY_OFF     0xff	  // LED数码管灭

//显示界面模式
#define		PAGE_OFF			0			//灭页面

#define		PAGE_RUN			1			//运行页面
#define		PAGE_WAIT			2			//等待（进度条）页面
#define		PAGE_BACKDISPLAY	3			//回显页面
#define		PAGE_CFGADDR		4			//配置地址页面
#define		PAGE_CFGFUNC1	  	5			//配置功能1页面
#define		PAGE_CFGFUNC2		6			//配置功能2页面
#define		PAGE_CFGFUNC3  		7			//配置功能3页面
#define		PAGE_CFGFUNC4  		8			//配置功能4页面
#define		PAGE_START  		9			//开机版本
#define		PAGE_NULL			10			//NU****
#define		PAGE_CFGFUNC5  		11			//配置功能5页面

#define		PAGE_CFGPWD			12          //配置密码页面
// 滚屏的速度
#define		DISP_ROLL_SPEED    40; 	// LED滚屏速度	 ,系统时钟里自减，2ms为单位

// 刷新区域
#define		SRCODE_AREA_ALL	   0
#define		SRCODE_AREA		   1
#define		SKU_AREA		   2
#define		COUNT_AREA	       3
//====================================================
// 滚屏显示模式
#define ROLL_MODE_OFF			0		   	// 无滚屏模式
#define ROLL_MODE_SRCINFO		0x80	   	// 滚屏批次信息标志
#define ROLL_MODE_SRCINFO_ED	0x40	   	// 滚屏数据刷新标志


#define ROLL_MODE_COUNT	    	0x01	   	// 滚屏数量信息标志
#define ROLL_MODE_COUNT_ED	    0x02		// 滚屏数据刷新标志

//==================================================================================
// 函数声明
AP_LED_EXT void digital_led_cntrl_Init(void);

AP_LED_EXT void led_Scan(void);				 // 数码管动态扫描刷新
AP_LED_EXT void CMD_led_flash(void);		 // LED指示灯工作状态
AP_LED_EXT void Start_Display(void);		 // 开机显示信息
AP_LED_EXT void All_Digital_Led_Pwr_OFF(void);	 // 数码管电源关灭

AP_LED_EXT void	SetDispPage(INT8U Page);	 // 设置显示页面
AP_LED_EXT void	LedDispBuffRefresh(void);	 // 数码管数据更新
AP_LED_EXT void Display_Clear(void);		 // 灭数码管显示和灭灯组显示，清回显标志、清配置标志、清数值修改标志

AP_LED_EXT void Calc_Value3(INT8U Order,INT8U ASICREL,INT16U Dat);
AP_LED_EXT void Calc_Value4(INT8U Order,INT8U ASICREL,INT16U Dat);
AP_LED_EXT void Calc_Value5(INT8U Order,INT8U ASICREL,INT16U Dat);

#endif

