#ifndef _EEPROM_H_
#define _EEPROM_H_

/************************************************************************************/
#ifdef  AP_EPROM_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_EPROM_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_EPROM_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

//#define CMD_TRIG_H	0x46
//#define CMD_TRIG_L  0xB9

#if	 (MCUTYPE == STC12C5616AD)
//STC12C5616AD
#define CMD_TRIG_H	0x46
#define CMD_TRIG_L  0xB9
#elif(MCUTYPE == STC15W1K16S)
//STC15W1K16S
#define CMD_TRIG_H	0x5A
#define CMD_TRIG_L  0xA5
#endif

/*
sfr IAP_DATA    = 0xC2;
sfr IAP_ADDRH   = 0xC3;
sfr IAP_ADDRL   = 0xC4;
sfr IAP_CMD     = 0xC5;
sfr IAP_TRIG    = 0xC6;
sfr IAP_CONTR   = 0xC7;
*/
//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

#define SECTOR_0_ADDR       0
#define SECTOR_1_ADDR       1 
#define SECTOR_2_ADDR       2
#define SECTOR_3_ADDR       3
#define SECTOR_4_ADDR       4
#define SECTOR_5_ADDR       5
#define SECTOR_6_ADDR       6
#define SECTOR_7_ADDR       7

extern UINT16 CODE SECTOR_PAGE[];

#define EEPROM_BLOCK_SIZE   0x0200

UINT8 eeprom_read_byte(const UINT8 * addr);
void  eeprom_write_byte(const UINT8 * addr, UINT8 temp);
void  eeprom_read_block(UINT8 * Buff, UINT8 * beginAddr, UINT8 len);
void  eeprom_write_block(UINT8 * Buff, UINT8 * beginAddr, UINT8 len);
void eeprom_erase_sector(UINT16 pageAddr);

void eeprom_test(void);

#define EE_ADDRESS		0
#define EE_FINISHEDLED 	1
#define EE_WTTIME		2
#define EE_FNDEFINE		3
#define EE_DISPMODE		4
#define EE_KEYPROCLED	5

#define MAX_ADDR        	255      //1~255	  地址
#define MAX_FINISHEDLED     7    	 //3  
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	1    	 //1  
#define MAX_DISPMODE        2    	 //1
#define MAX_KEYPROCLED      8    	 //1

#define DEF_ADDR        	1     	 //1	  默认值
#define DEF_FINISHEDLED     0    	 //0  
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPROCLED      0    	 //0


//================================================================================
// 函数声明
AP_EPROM_EXT void SaveParaToBlock(void);		   // 保存参数，保存前判断参数是否超出范围，如超出则恢复到默认值
AP_EPROM_EXT void LoadParaInEeprom(void);		   // 开机读取参数，如超出则恢复到默认值
#endif









