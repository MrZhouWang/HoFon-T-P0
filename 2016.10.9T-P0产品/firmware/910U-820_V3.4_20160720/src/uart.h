#ifndef _UART_H_
#define _UART_H_

/************************************************************************************/
#ifdef  AP_UART_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_UART_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_UART_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/

#define BD_4800		0
#define BD_9600		1
#define BD_19200	2
#define BD_38400	3

#define BAUDRATE_MAX  BD_38400

//sbit p_led_static = P2^3;//ϵͳ״̬��
//sbit p_led_rxd = P2^2;
//sbit p_led_txd = P2^1;
//
//sbit p_m485_ctrl = P0^0;



//#define UART_BUF_LEN	50//���ڻ�������С
//typedef struct
//{
//	INT8VU wBuffer[UART_BUF_LEN];	//д������
//	INT8VU wLen;
//	INT8VU rIndex;					//���ջ���������
//	INT8VU rBuffer[UART_BUF_LEN];
//	INT8VU rTimer;					// ��ʱ��
//	INT8VU rFlag;			 		// ���ջ������Ƿ�����=ON -> ����, =OFF����(�ڶ�ʱ���в�ѯ������״̬)
//}OS_UART;
//
//AP_UART_EXT  OS_UART XDATA uart;

AP_UART_EXT	void   UART_Init(void);
AP_UART_EXT	UINT8  SerialGetChar(void);
AP_UART_EXT void   SendSerialByte(UINT8 *Ram, INT8U len);//UINT16 len);
AP_UART_EXT INT16U CRC16 ( INT8U XDATA * puchMsg, INT8U usDataLen ) ;
#endif

