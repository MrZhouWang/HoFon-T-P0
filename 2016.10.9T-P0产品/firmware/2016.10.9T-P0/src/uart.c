
#define AP_UART_GLOBALS	
#include "extern_symbol.h" 


/***************************************************
		T1定时器初始化
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       同步移位寄存器		fosc/12
 0   1		   10位异步收发         由定时器控制
 1   0         11位异步收发  		fosc/32或fosc/64
 1   1         11位异步收发         由定时器控制
P = PSW.0      P=1奇   P=0偶
***************************************************/
void InitT1(void)
{
	TMOD = (TMOD & 0x0f) | 0x20;	//8位自动重载定时器
	SCON = 0xd0;			      	//串行口工作模式3选择
	IP   = 0x10;				    //PS set the high
	PCON  &= 0x7f;                  //SOMD =0

	//STC15W1K16S
#if(MCUTYPE == STC15W1K16S)
	AUXR1 = 0x40;        		//设置串口1在P3.6和 P3.7中
#endif
	//波特率 38400
	AUXR |= 0x40;        		//定时器1时钟为Fosc,即1T
	AUXR &= 0xfe;       		//串口1选择定时器1为波特率发生器
	TL1 = 0xF7;            		//设定定时初值
	TH1 = 0xF7;            		//设定定时器重装值


	ET1  = 0;                       //close timer1 interrupt
	TR1  = 1;	                    //open timer1
	ES   = 1;				        //Enabled serial port interrrup 
	
    return;
}
/*******************************************************************************
	 						串口初始化
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART_Init(void)
{
	InitT1();			 // T1做波特率发生器
	uart.rIndex = 0;	 // 接收个数清0
	uart.rFlag  = OFF;	 // 接收完成标志清除
	uart.rTimer = 0;	 // 接收超时时间清0
}

/*******************************************************************************
	 					 串口中断函数
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
		//当前帧未处理完毕时，不接收
		if(uart.rFlag == ON)
			return;
		uart.rBuffer[uart.rIndex++] = SBUF;	
		uart.rTimer = RCV_OVER_TIMER;
	}
}
/***********************************************
        串口发送程序
input :     *Ram    
            len     
output :    NONE
***********************************************/
void SendSerialByte(UINT8 *Ram, INT8U len)//UINT16 len)
{ 
	LED_TXT_ON;;			 // 开发送指示灯
	MAX485_SEND_EN;			 // 使能485发送，关闭接收
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
	MAX485_REV_EN;		   // 使能485接收，关闭发送
	LED_TXT_OFF;		   // 关发送指示灯
}
/*******************************************************************************
								END					
*******************************************************************************/
