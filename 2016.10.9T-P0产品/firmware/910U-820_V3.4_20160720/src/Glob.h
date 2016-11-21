#ifndef _Glob_H_
#define _Glob_H_



/***********************************************************************************
* ������������(�ο�uC/OS_II.H)
*	1���� extern_symbol.h ��ÿ��ȫ�ֱ��������� AP_CPU_EXT ��ǰ׺
* 	2���� main.c �в�������Ч����  	
*      	#define OS_CPU_GLOBALS��OS_CPU_EXT ��ǿ��Ϊ �� ��(��)
*		#include "Head File/extern_symbol.h"�����б��������� 
*	3���������ļ��в�������Ч����
*		δ���� OS_CPU_GLOBALS��OS_CPU_EXT ��ǿ��Ϊ extern
*		#include "Head File/extern_symbol.h"�����б����� ��extern�� ������
*	4���� main.c ��ͬʱ�� #include "Head File/parameter_map.h" ���������Ϊ��
*		A���� extern_symbol.h �����ȶ���ṹ���ͣ����������������ռ�)
*		B��Ȼ���� parameter_map.h �жԱ������г�ʼ��
* 		C�������ļ��� ���� extern_symbol.h����ʱ���ȶ���ṹ���ͣ�Ȼ������ extern ����
*	5��ע�⣺�� main.c �У������� #define OS_CPU_GLOBALS Ȼ�������� parameter_map.h
************************************************************************************/
#ifdef  AP_CPU_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_CPU_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_CPU_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/
//==================================================================================

//#define MAX_SRCODE_NUM 	20	//�����յ���ʾ��Ϣ
#define MAX_SRCODE_NUM 	12	//�����յ���ʾ��Ϣ
typedef struct 
{
	INT8VU 	  sku;			  			// ��λ
	INT16VU   count;		  			// ����
	INT8VU 	  full;			  			// �������
	INT8VU 	  srcode_len;	  			// �������ݳ���
	INT8VU 	  srcode[MAX_SRCODE_NUM]; 	// ������Ϣ
}TASK_INFO;

#define MAX_TASK_NUM 	2	//������ҵ��������
typedef struct 
{
	INT8VU 	   num;			  		// ����
	INT8VU	   total;				// ���������
	INT8VU	   remain;				// ʣ���������
	INT8VU	   repeat;				// �ظ����յ������־
	TASK_INFO  info[MAX_TASK_NUM];	// ������Ϣ
}TASK_STR;

#define MAX_BACK_NUM	5	//��������Ϣ֧��
typedef struct 
{
	INT8VU 	   num;			  		// ����
	INT8VU	   index;				// ����
	TASK_INFO  info[MAX_BACK_NUM];	// ������Ϣ
}BACK_TASK_STR;

AP_CPU_EXT 	TASK_STR      XDATA FinishTask;			// �����������Ϣ��������
AP_CPU_EXT 	TASK_STR      XDATA UnfinishTask;		// δ���������Ϣ��������
AP_CPU_EXT 	BACK_TASK_STR XDATA BackTask;			// �����������Ϣ���ݻ�������

typedef struct 
{
	INT16VU TaskCurCount;				// ��ǰ�����������ݣ�
	INT8VU	LastSoftGetOkCount;			// ��λ������·��˴�����֮ǰ����0X03������ɼ������Ѿ�OK��Ӧ������
	INT8VU  CurDispMode ;				// ��Ӧ��3����ʾģʽ��0-�����ݣ�1-��ʾ�ַ�����3-��ʾ�ּ�����(1,3ʵ��)
										// 00Ϊ������״̬,01,03Ϊ����״̬
	INT8VU  beSettingAddr;				// 0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
	INT8VU  SystemLocked;				// ����ϵͳ������ϵͳ
}OS_TASK_STR;

AP_CPU_EXT 	OS_TASK_STR  DATA OsCmdSet;			// ��������ϵͳ��Ҫ����

//--------------------------------------------------------------------------------------
// LEDָʾ��
typedef struct 
{
	INT8VU 	light_type;			// ��������
	INT8VU 	light_type_BK;		// �������ͱ���	 
	INT8VU 	flash_mode;			// ��˸ģʽ
	INT8VU 	flash_status;		// ��˸״̬
	INT16VU	flash_period;		// ��˸���ڻ�׼���ɵ���
	INT16VU time_refresh;		// ��ʱˢ��ʱ��		ϵͳʱ�����Լ�
}LED_STR;

AP_CPU_EXT 	 LED_STR DATA led;	   // ָʾ����ṹ�����
//--------------------------------------------------------------------------------------
// ��ʾ
typedef struct{
	
	INT8VU	f_Refresh;		// ����ˢ�±�־
	INT8VU	f_Loading;		// ���������־
	INT8VU	f_Rolling;		// ������־
	INT8VU	f_OffDisp;		// ����ʾ��־

	INT8VU	Focus;			// ����λ
	INT8VU	MenuPage;		// �˵�ҳ
	INT8VU	Shift;			// �ƶ�

	INT8VU	Page;			// ��ʾҳ��
	INT8VU	PageBak;		// ��ʾҳ��
	INT16VU Value;			// ����ֵ

	INT16VU TimeRefresh;		 // ��ʱˢ��ʱ��		ϵͳʱ�����Լ�
	INT8VU  RollDelayCnt;		 // ������ʱʱ��		ϵͳʱ�����Լ�
//	INT16VU TatolTimeRefresh16;  // �ܵĶ�ʱˢ��ʱ��	ϵͳʱ�����Լ�
	INT8VU  TatolTimeRefresh8;   // �ܵĶ�ʱˢ��ʱ��	ϵͳʱ�����Լ�(��λ100ms)
//	INT32VU TatolTimeRefresh32;  // �ܵĶ�ʱˢ��ʱ��	ϵͳʱ�����Լ�
	INT16VU TatolTimeRefresh16;  // �ܵĶ�ʱˢ��ʱ��	ϵͳʱ�����Լ�(��λ100ms)
} DISPLAY_STR;

AP_CPU_EXT 	 DISPLAY_STR DATA Disp;	   // ��ʾ�ṹ�����
//--------------------------------------------------------------------------------------
// �����
#if( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
	#define LED_NUMBER		7		  		//���������
#else
	#define LED_NUMBER		6		  		//���������
#endif

typedef struct 
{
	INT8VU 	  Srcode[MAX_SRCODE_NUM];	// ������Ϣ
	INT8VU 	  SrLen;			 		// ������Ϣ����
	INT8VU 	  SrIndex;			 		// ��Ӧ���ι���λ��
	INT8VU 	  num[LED_NUMBER];			// ��Ӧ�������ʾ����
	INT8VU	  CntLen;					// ����������
	INT8VU 	  CntIndex;			 		// ��Ӧ��������λ��
	INT8VU 	  Order;			 		// ��Ӧ��ʾ�ڼ�λ
}DIGELED_STR;

AP_CPU_EXT	DIGELED_STR	IDATA DigiLed;	// ����ܽṹ��

#define LED_RANKCOUNT	8			//����ģ����ʾ��������



AP_CPU_EXT INT8VU  XDATA SRCodeData[MAX_SRCODE_NUM+1][LED_RANKCOUNT];	//��������
AP_CPU_EXT INT8VU  XDATA CountCodeData[DIGILED_COUNT+1][LED_RANKCOUNT];	//��������
AP_CPU_EXT INT8VU  XDATA SKUCodeData[DIGILED_SKU+1][LED_RANKCOUNT];		//��������
//--------------------------------------------------------------------------------------

AP_CPU_EXT UINT8  IDATA	Buffer[16];		// ���ݻ���
//--------------------------------------------------------------------------------------
// ��������
typedef struct
{
	INT8VU	f_Short;			//�����ͷű�־ 0�ͷ�
	INT8VU	f_Long;				//����������־ 1���� 	
	INT8VU	f_LongOnce;			//��������ִ��һ�α�־
	INT8VU	f_Enable;			//����ɨ��ʹ�ܱ�־
	INT8VU  f_IsFnKeyPressed;	//���ܼ��Ƿ��Ѿ����£�����OK��֮ǰ��

	INT8VU	SysCnt;				//��ϵͳʱ�Ӽ�ʱ  ��ϵͳʱ�����Լ�
	INT8VU	OnCnt;				//���¼���ʱ�䣬��λΪ32ms
	INT8VU	Backup;				//�����ϴεļ�ֵ
	INT8VU	BackProssVal;		//�����ϴ�ִ�еļ�ֵ
//	INT8U	ScanCnt;			//ɨ�����
	INT8VU  f_LongFunc;			//���ܼ����� 
	INT8VU  CfgPwdCnt;			//�����������
} KEY_STR;

AP_CPU_EXT	KEY_STR   DATA Key;	  // �������ñ����Ľṹ��
// �������õ�ģʽ
typedef struct
{
	INT8VU	f_Config;		// ϵͳ���ñ�־λ,=OFF -> ��������״̬��=ON ->������״̬
	INT8VU	f_ChangeValue;	// ������ʾ��ֵ״̬�£���ʾ�Ƿ������޸Ľ���λ�������֣��޸Ľ���λ�£���λ0��Ҫ��ʾ��������Ҫ��ʾ��	
	INT8VU	f_BackDisp;		// ����״̬��־	   OFF=��
}KEY_MODE_STR;

AP_CPU_EXT	KEY_MODE_STR	 DATA KeyMode;		 // �������õ�ģʽ �Ľṹ��
//--------------------------------------------------------------------------------------
// ����
#define UART_BUF_LEN	50//���ڻ�������С
typedef struct
{
	INT8VU wLen;					// ��������
	INT8VU wBuffer[UART_BUF_LEN];	// ���ͻ�����
	INT8VU rIndex;					// ���ջ���������
	INT8VU rBuffer[UART_BUF_LEN];	// ���ջ�����
	INT8VU rTimer;					// ��ʱ��
	INT8VU rFlag;			 		// ���ջ������Ƿ�����=ON -> ����, =OFF����(�ڶ�ʱ���в�ѯ������״̬)
}OS_UART;

AP_CPU_EXT  OS_UART XDATA uart;		// ���ڷ��ͽ��սṹ��
//--------------------------------------------------------------------------------------
// �����洢
#define MAX_CFGPARA       	6      	 //���ò���������
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 //��ַ(1~255)
		INT8VU 	  FinishedLED;			 //��ɺ��LED�������(0,1,2,3,4,5,6,7-->N,R,G,B,Y,�ۣ��࣬��)
		INT8VU 	  WtTime;				 //�������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 //���ܼ�����(0,1->���,�������)
		INT8VU 	  DispMode;				 //��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
		INT8VU 	  KeyProssLED;			 //OK������ָʾ��(0,1,2,3,4,5,6,7,8-->N,R,G,B,Y,�ۣ��࣬��,8ǰһ�ֵ�ɫ+1)����800U ����оƬ��
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;


AP_CPU_EXT	STORAGE_UNI  IDATA  para;		   // ��������
AP_CPU_EXT	STORAGE_UNI  IDATA  para_bk;	   // ���ݲ�������
//--------------------------------------------------------------------------------------
// ϵͳ
typedef struct{
	

	INT8VU  Base_100ms;		 // ��׼ʱ��100ms		

} OS_TIME;

AP_CPU_EXT 	OS_TIME  DATA os_time;


// ϵͳ��ʾ����
typedef struct{
	

	INT8VU   DispLed_sku;		 // ��λ��ʾ��λ��
	INT8VU   DispLed_count;		 // ������ʾ��λ��
	INT32VU  DispMax_count;		 // �����ʾ����
	INT8VU   DispLed_first;		 // ��ʾ��ʼλ
	INT8VU	 Chg_Task_Cnt_first; // �޸�������������ʼλ 
			

} OS_DISPSET;

AP_CPU_EXT 	OS_DISPSET  DATA os_disp_set;

//==================================================================================

#endif
