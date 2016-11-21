#ifndef _EEPROM_H_
#define _EEPROM_H_

/************************************************************************************/
#ifdef  AP_EPROM_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_EPROM_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_EPROM_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
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
//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

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

#define MAX_ADDR        	255      //1~255	  ��ַ
#define MAX_FINISHEDLED     7    	 //3  
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	1    	 //1  
#define MAX_DISPMODE        2    	 //1
#define MAX_KEYPROCLED      8    	 //1

#define DEF_ADDR        	1     	 //1	  Ĭ��ֵ
#define DEF_FINISHEDLED     0    	 //0  
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPROCLED      0    	 //0


//================================================================================
// ��������
AP_EPROM_EXT void SaveParaToBlock(void);		   // �������������ǰ�жϲ����Ƿ񳬳���Χ���糬����ָ���Ĭ��ֵ
AP_EPROM_EXT void LoadParaInEeprom(void);		   // ������ȡ�������糬����ָ���Ĭ��ֵ
#endif









