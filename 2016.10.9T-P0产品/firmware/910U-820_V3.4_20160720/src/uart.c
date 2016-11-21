
#define AP_UART_GLOBALS	
#include "extern_symbol.h" 


/***************************************************
		T1��ʱ����ʼ��
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       ͬ����λ�Ĵ���		fosc/12
 0   1		   10λ�첽�շ�         �ɶ�ʱ������
 1   0         11λ�첽�շ�  		fosc/32��fosc/64
 1   1         11λ�첽�շ�         �ɶ�ʱ������
P = PSW.0      P=1��   P=0ż
***************************************************/
void InitT1(void)
{
	TMOD = (TMOD & 0x0f) | 0x20;	//8λ�Զ����ض�ʱ��
	SCON = 0xd0;			      	//���пڹ���ģʽ3ѡ��
	IP   = 0x10;				    //PS set the high
	PCON  &= 0x7f;                  //SOMD =0

	//STC15W1K16S
#if(MCUTYPE == STC15W1K16S)
	AUXR1 = 0x40;        		//���ô���1��P3.6�� P3.7��
#endif
	//������ 38400
	AUXR |= 0x40;        		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xfe;       		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TL1 = 0xF7;            		//�趨��ʱ��ֵ
	TH1 = 0xF7;            		//�趨��ʱ����װֵ


	ET1  = 0;                       //close timer1 interrupt
	TR1  = 1;	                    //open timer1
	ES   = 1;				        //Enabled serial port interrrup 
	
    return;
}
/*******************************************************************************
	 						���ڳ�ʼ��
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART_Init(void)
{
	InitT1();			 // T1�������ʷ�����
	uart.rIndex = 0;	 // ���ո�����0
	uart.rFlag  = OFF;	 // ������ɱ�־���
	uart.rTimer = 0;	 // ���ճ�ʱʱ����0
}

/*******************************************************************************
	 					 �����жϺ���
	input:   none     
	output:  none					
*******************************************************************************/
#define RCV_OVER_TIMER	2

void SCON_int (void) interrupt 4
{
//        static UINT8 RX_CHAR;
//        static UINT16 rx_cnt=0;
	if (RI)
	{
		RI = 0;
		if ( uart.rIndex >= UART_BUF_LEN ) 
			return;
		//��ǰ֡δ�������ʱ��������
		if(uart.rFlag == ON)
			return;
		uart.rBuffer[uart.rIndex++] = SBUF;	
		uart.rTimer = RCV_OVER_TIMER;
	}
}
/***********************************************
        ���ڷ��ͳ���
input :     *Ram    
            len     
output :    NONE
***********************************************/
void SendSerialByte(UINT8 *Ram, INT8U len)//UINT16 len)
{ 
	LED_TXT_ON;;			 // ������ָʾ��
	MAX485_SEND_EN;			 // ʹ��485���ͣ��رս���
    for(;len>0;len--)
    {
		ACC = *Ram;
		if(P == 0)
		{
			TB8 = 0;
		}
		else
		{
			TB8 =1;
		}

        SBUF = *Ram;           
        while(!TI){;}             
        TI = 0;
        Ram++;
    }
	MAX485_REV_EN;		   // ʹ��485���գ��رշ���
	LED_TXT_OFF;		   // �ط���ָʾ��
}
/*******************************************************************************
								END					
*******************************************************************************/
