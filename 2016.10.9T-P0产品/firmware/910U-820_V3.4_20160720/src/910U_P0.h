
// �漰��IO�ڼ�����Ӳ����ͬ�ģ����ڴ˶���
// ����ָʾ��
sbit p_led_red        = P0^2;
sbit p_led_green      = P2^7;
sbit p_led_blue       = P2^6;

#define LED_PWR_ON	0 //LED��Դ����״̬
#define LED_PWR_OFF	1 //LED��Դ����״̬

#define LED_R_ON	(p_led_red   = LED_PWR_ON)
#define LED_R_OFF	(p_led_red   = LED_PWR_OFF)

#define LED_G_ON	(p_led_green = LED_PWR_ON)
#define LED_G_OFF	(p_led_green = LED_PWR_OFF)

#define LED_B_ON	(p_led_blue  = LED_PWR_ON)
#define LED_B_OFF	(p_led_blue  = LED_PWR_OFF)

// ����ܹ�����
//sbit digital_led1_port	=P1^2 ;//��4λ����ܵĵ�ԴIO
//sbit digital_led2_port	=P1^1 ;//��5λ����ܵĵ�ԴIO
//sbit digital_led3_port	=P1^0 ;//��6λ����ܵĵ�ԴIO
//
//sbit digital_led4_port	=P1^4 ;//��4λ����ܵĵ�ԴIO
//sbit digital_led5_port	=P1^3 ;//��5λ����ܵĵ�ԴIO
//sbit digital_led6_port	=P0^3 ;//��6λ����ܵĵ�ԴIO
////sbit digital_led4_port	=P1^2 ;//��4λ����ܵĵ�ԴIO
////sbit digital_led5_port	=P1^1 ;//��5λ����ܵĵ�ԴIO
////sbit digital_led6_port	=P1^0 ;//��6λ����ܵĵ�ԴIO
//
//#define DIGITAL_LED_PWR_ON	0 //����ܵ�Դ����״̬
//#define DIGITAL_LED_PWR_OFF	1 //����ܵ�Դ����״̬
//
//#define DIGITAL_LED1_ON		(digital_led1_port = DIGITAL_LED_PWR_ON)
//#define DIGITAL_LED2_ON		(digital_led2_port = DIGITAL_LED_PWR_ON)
//#define DIGITAL_LED3_ON		(digital_led3_port = DIGITAL_LED_PWR_ON)
//#define DIGITAL_LED4_ON		(digital_led4_port = DIGITAL_LED_PWR_ON)
//#define DIGITAL_LED5_ON		(digital_led5_port = DIGITAL_LED_PWR_ON)
//#define DIGITAL_LED6_ON		(digital_led6_port = DIGITAL_LED_PWR_ON)
//
//#define DIGITAL_LED1_OFF	(digital_led1_port = DIGITAL_LED_PWR_OFF)
//#define DIGITAL_LED2_OFF	(digital_led2_port = DIGITAL_LED_PWR_OFF)
//#define DIGITAL_LED3_OFF	(digital_led3_port = DIGITAL_LED_PWR_OFF)
//#define DIGITAL_LED4_OFF	(digital_led4_port = DIGITAL_LED_PWR_OFF)
//#define DIGITAL_LED5_OFF	(digital_led5_port = DIGITAL_LED_PWR_OFF)
//#define DIGITAL_LED6_OFF	(digital_led6_port = DIGITAL_LED_PWR_OFF)
	
//595 IO��
//sbit p_oe       = P2^0;
//sbit p_sh_clk   = P1^5;
//sbit p_st_clk   = P1^6;
//sbit p_sdata    = P1^7;
//
////595��������
//#define OE_SET	(p_oe = 1)
//#define OE_CLR	(p_oe = 0)
//
//#define SH_CLK_SET	(p_sh_clk = 1)
//#define SH_CLK_CLR	(p_sh_clk = 0)
//
//#define ST_CLK_SET	(p_st_clk = 1)
//#define ST_CLK_CLR	(p_st_clk = 0)
//
//#define SDATA_SET	(p_sdata = 1)
//#define SDATA_CLR	(p_sdata = 0)

//����IO��
//sbit KEY_SUB	  = P3^4;//P3^5;
//sbit KEY_CFG      = P3^5;//P2^4;
//sbit KEY_SHT      = P2^4;
sbit KEY_OK  	  = P3^3;


//����ָʾ�ƺͿ���IO��

//sbit p_led_static = P2^3;//ϵͳ״̬��
sbit p_led_rxd    = P2^2;  // ���ڽ���ָʾ��
sbit p_led_txd    = P2^1;  // ���ڷ���ָʾ��

sbit p_m485_ctrl = P0^0;   // 485оƬ���ͽ��տ����ź�

#define LED_UARTPWR_ON	0 //LED��Դ����״̬
#define LED_UARTPWR_OFF	1 //LED��Դ����״̬

#define LED_RXT_ON	(p_led_rxd   = LED_UARTPWR_ON)
#define LED_RXT_OFF	(p_led_rxd   = LED_UARTPWR_OFF)

#define LED_TXT_ON	(p_led_txd   = LED_UARTPWR_ON)
#define LED_TXT_OFF	(p_led_txd   = LED_UARTPWR_OFF)

#define MAX485_REV_EN	(p_m485_ctrl   = 0)
#define MAX485_SEND_EN	(p_m485_ctrl   = 1)
