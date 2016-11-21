
#ifndef _KEY_H_
#define _KEY_H_

/************************************************************************************/
#ifdef  AP_KEY_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_KEY_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_KEY_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/


#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1
#define CMD_KEY_ADD		2
#define CMD_KEY_SHT		3
#define CMD_KEY_CFG_L	4
#define CMD_KEY_CFG_S	5

#define KEYFUN_ADD			0x80            // +������
#define KEYFUN_SUB			0x20            // -������
#define KEYFUN_FORWARD		0x08            // ǰ��������
#define KEYFUN_BACK			0x40            // ���˼�����
#define KEYFUN_FUNC			0x10            // ���ܼ�����

#define KEYFUN_OK			0x01            // OK������
#define KEYFUN_FUNC_L		0x02            // ���ܼ�����
#define KEYFUN_NOKEY		0x00            // �ް���

//#define TIMER_COUNT_100mS	100
//#define TIMER_COUNT_1S	500
//#define TIMER_COUNT_3S	800

#define TIMER_COUNT_32ms	3//16//15

//#define	KEYTIME_SHORT		1				//�̰�����
//#define KEYTIME_LONG		30				//��������	16bit
//#define KEYTIME_FREE		1				//�����ͷ�  8bit
#define KEYLONGTIME			200//45//30			//����ʱ�䣬32msΪ��λ
//#define KEYLONGPROTIME		3				//�������ִ��ʱ�䣬32msΪ��λ



// ��������
AP_KEY_EXT void	PutFinishedToStack_Com( TASK_INFO XDATA *pt1,TASK_INFO XDATA *pt2 );	  // ��������
AP_KEY_EXT void Key_Scan(void);															  // ����ɨ�輰����
AP_KEY_EXT void	Exit_Cfg_Mode(void);													  // �˳�����ģʽ


#endif
