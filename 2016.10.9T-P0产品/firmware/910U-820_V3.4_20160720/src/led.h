#ifndef _LED_H_
#define _LED_H_

/************************************************************************************/
#ifdef  AP_LED_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_LED_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_LED_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/

//led light ����ģʽ
#define LED_NONE	0				  // ������
#define LED_RED		1				  // �������
#define LED_GREEN	2				  // �����̵�
#define LED_BLUE	3				  // ��������

#define LED_RG		4				  // �������̵�
#define LED_RB		5				  // ����������
#define LED_GB		6				  // ����������
#define LED_RGB		7				  // ������������

//led ����˸����ģʽ
//9������ʱ�����
#define LED_FLASH_MODE_ALWAYS		0			// ����
#define LED_FLASH_MODE_1V1			1			// ����ʱ�䣺 1V1�� ��ʾ�� 1*100ms���� 1*100ms��
#define LED_FLASH_MODE_1V2			2			// ����ʱ�䣺 1V2�� ��ʾ�� 1*100ms���� 2*100ms��
#define LED_FLASH_MODE_1V5			3			// ����ʱ�䣺 1V5,  ��ʾ�� 1*100ms���� 5*100ms��
#define LED_FLASH_MODE_1V10			4			// ����ʱ�䣺 1V10, ��ʾ�� 1*100ms����10*100ms��
#define LED_FLASH_MODE_2V1			5			// ����ʱ�䣺 2V1�� ��ʾ�� 2*100ms���� 1*100ms
#define LED_FLASH_MODE_5V1			6			// ����ʱ�䣺 5V1�� ��ʾ�� 5*100ms���� 1*100ms
#define LED_FLASH_MODE_10V1			7			// ����ʱ�䣺10V1�� ��ʾ��10*100ms���� 1*100ms

//�������ʾ�����ҷֱ�Ϊ��1λ-->��6λ
#define DIGITAL_LED1	0 //��1λ�����
#define DIGITAL_LED2	1 //��2λ�����
#define DIGITAL_LED3	2 //��3λ�����
#define DIGITAL_LED4	3 //��4λ�����
#define DIGITAL_LED5	4 //��5λ�����
#define DIGITAL_LED6	5 //��6λ�����

//#define DIGITAL_LED_PWR_ON	0 //����ܵ�Դ����״̬
//#define DIGITAL_LED_PWR_OFF	1 //����ܵ�Դ����״̬
//
//#define DIGITAL_LED_PWR_ALLOFF	0x1f 	//�����1~5 ����

//====================================================
// ��������ʾģʽ
#define LOAD_MODE_OFF	0		   // ����ģʽ��
#define LOAD_MODE_SHORT	1		   // ����ģʽ��ʱ��
#define LOAD_MODE_LONG	2		   // ����ģʽ��ʱ��


//#define LED_NUMBER		6		  //���������
//#define LED_NOT_DISPLAY 0xff
//#define LED_TBL_MAX     37
#define LED_TBL_MAX     128		  // LED�ֿ��������
#define DIGI_LED_OFF    0x20	  // �ո�
//#define DISPLAY_OFF     0xff	  // LED�������

//��ʾ����ģʽ
#define		PAGE_OFF			0			//��ҳ��

#define		PAGE_RUN			1			//����ҳ��
#define		PAGE_WAIT			2			//�ȴ�����������ҳ��
#define		PAGE_BACKDISPLAY	3			//����ҳ��
#define		PAGE_CFGADDR		4			//���õ�ַҳ��
#define		PAGE_CFGFUNC1	  	5			//���ù���1ҳ��
#define		PAGE_CFGFUNC2		6			//���ù���2ҳ��
#define		PAGE_CFGFUNC3  		7			//���ù���3ҳ��
#define		PAGE_CFGFUNC4  		8			//���ù���4ҳ��
#define		PAGE_START  		9			//�����汾
#define		PAGE_NULL			10			//NU****
#define		PAGE_CFGFUNC5  		11			//���ù���5ҳ��

#define		PAGE_CFGPWD			12          //��������ҳ��
// �������ٶ�
#define		DISP_ROLL_SPEED    40; 	// LED�����ٶ�	 ,ϵͳʱ�����Լ���2msΪ��λ

// ˢ������
#define		SRCODE_AREA_ALL	   0
#define		SRCODE_AREA		   1
#define		SKU_AREA		   2
#define		COUNT_AREA	       3
//====================================================
// ������ʾģʽ
#define ROLL_MODE_OFF			0		   	// �޹���ģʽ
#define ROLL_MODE_SRCINFO		0x80	   	// ����������Ϣ��־
#define ROLL_MODE_SRCINFO_ED	0x40	   	// ��������ˢ�±�־


#define ROLL_MODE_COUNT	    	0x01	   	// ����������Ϣ��־
#define ROLL_MODE_COUNT_ED	    0x02		// ��������ˢ�±�־

//==================================================================================
// ��������
AP_LED_EXT void digital_led_cntrl_Init(void);

AP_LED_EXT void led_Scan(void);				 // ����ܶ�̬ɨ��ˢ��
AP_LED_EXT void CMD_led_flash(void);		 // LEDָʾ�ƹ���״̬
AP_LED_EXT void Start_Display(void);		 // ������ʾ��Ϣ
AP_LED_EXT void All_Digital_Led_Pwr_OFF(void);	 // ����ܵ�Դ����

AP_LED_EXT void	SetDispPage(INT8U Page);	 // ������ʾҳ��
AP_LED_EXT void	LedDispBuffRefresh(void);	 // ��������ݸ���
AP_LED_EXT void Display_Clear(void);		 // ���������ʾ���������ʾ������Ա�־�������ñ�־������ֵ�޸ı�־

AP_LED_EXT void Calc_Value3(INT8U Order,INT8U ASICREL,INT16U Dat);
AP_LED_EXT void Calc_Value4(INT8U Order,INT8U ASICREL,INT16U Dat);
AP_LED_EXT void Calc_Value5(INT8U Order,INT8U ASICREL,INT16U Dat);

#endif

