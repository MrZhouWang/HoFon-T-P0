


// 涉及到IO口及其他硬件不同的，均在此定义
// 灯组指示灯
#if	 (MCUTYPE == STC12C5616AD)
sbit p_led_red        = P2^6;
sbit p_led_green      = P2^7;
sbit p_led_blue       = P3^7;
#else  
sbit p_led_red        = P0^2;
sbit p_led_green      = P2^7;
sbit p_led_blue       = P2^6;
#endif

#define LED_PWR_ON	0 //LED电源开关状态
#define LED_PWR_OFF	1 //LED电源开关状态

#define LED_R_ON	(p_led_red   = LED_PWR_ON)
#define LED_R_OFF	(p_led_red   = LED_PWR_OFF)

#define LED_G_ON	(p_led_green = LED_PWR_ON)
#define LED_G_OFF	(p_led_green = LED_PWR_OFF)

#define LED_B_ON	(p_led_blue  = LED_PWR_ON)
#define LED_B_OFF	(p_led_blue  = LED_PWR_OFF)

// 数码点阵芯片
// SPI引脚定义
#if	 (MCUTYPE == STC12C5616AD)
sbit ht_dat = 	P1^5;
sbit ht_wr  = 	P1^7;
sbit ht_rd	=   P1^6;
sbit ht_cs0 = 	P2^4;
sbit ht_cs1 = 	P2^5;
#else
sbit ht_dat = 	P1^3;
sbit ht_wr  = 	P1^5;
sbit ht_rd	=   P1^4;
sbit ht_cs0 = 	P1^0;
sbit ht_cs1 = 	P1^1;
#endif

//spi配置
#define HT1632_CS1_H	(ht_cs0 = 1)
#define HT1632_CS1_L	(ht_cs0 = 0)

#define HT1632_CS2_H	(ht_cs1 = 1)
#define HT1632_CS2_L	(ht_cs1 = 0)

#define HT1632_RD_H		(ht_rd  = 1)
#define HT1632_RD_L		(ht_rd  = 0)

#define HT1632_WR_H		(ht_wr  = 1)
#define HT1632_WR_L		(ht_wr  = 0)

#define HT1632_DATA_H	(ht_dat = 1)
#define HT1632_DATA_L	(ht_dat = 0)

#define	HT1632_CS1	0
#define	HT1632_CS2	1

//按键IO口
#if	 (MCUTYPE == STC12C5616AD)
sbit KEY_SUB	  = P0^1;//P3^5;
sbit KEY_CFG      = P3^5;//P2^4;
sbit KEY_SHT      = P3^4;
sbit KEY_OK  	  = P1^3;
#else
sbit KEY_SUB	  = P3^5;
sbit KEY_CFG      = P2^4;
sbit KEY_SHT      = P3^4;
sbit KEY_OK  	  = P3^3;
#endif

//串口指示灯和控制IO口
sbit p_led_static = P2^3;//系统状态灯
sbit p_led_rxd    = P2^2;  // 串口接收指示灯
sbit p_led_txd    = P2^1;  // 串口发送指示灯

sbit p_m485_ctrl = P0^0;   // 485芯片发送接收控制信号

#define LED_UARTPWR_ON	0 //LED电源开关状态
#define LED_UARTPWR_OFF	1 //LED电源开关状态

#define LED_RXT_ON	(p_led_rxd   = LED_UARTPWR_ON)
#define LED_RXT_OFF	(p_led_rxd   = LED_UARTPWR_OFF)

#define LED_TXT_ON	(p_led_txd   = LED_UARTPWR_ON)
#define LED_TXT_OFF	(p_led_txd   = LED_UARTPWR_OFF)

#define MAX485_REV_EN	(p_m485_ctrl   = 0)
#define MAX485_SEND_EN	(p_m485_ctrl   = 1)

 //-----------------------------定义HT1632各项参数
#define C_COMMAND1632_SYSEN      0x01  //SYSTEM ENABLE
#define C_COMMAND1632_N_16COM    0x24  //N-MOS OUTPUT AND 16 COMMON OPTION
#define C_COMMAND1632_P_16COM    0x2c  //P-MOS OUTPUT AND 16 COMMON OPTION
#define C_COMMAND1632_N_8COM     0x20  //N-MOS OUTPUT AND 8 COMMON OPTION
#define C_COMMAND1632_P_8COM     0x28  //P-MOS OUTPUT AND 8 COMMON OPTION

#define C_COMMAND1632_PWMDUTY    0xaF //PWM DUTY SET(0xa0~0xaf),亮度设置
#define C_COMMAND1632_LEDON      0x03  //LED ON
#define C_COMMAND1632_LEDOFF     0x02  //LED OFF
#define C_COMMAND1632_BLINKOFF   0x08  //BLINK OFF
#define C_COMMAND1632_BLINKON    0x09  //BLINK OFF

#define C_COMMAND1632_SLV_M    	0x10  //SLAVE MODE
#define C_COMMAND1632_MST_M    	0x18  //MOSTER MODE