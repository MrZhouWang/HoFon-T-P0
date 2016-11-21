
#define AP_LED_GLOBALS	
#include "extern_symbol.h" 
#include "ASCII_Led.h"						   // ASCII ����Ӧ�����

// ��������
void	Digi_DotLed_Disp(void);
void  	DispBuffrefresh_AD(void);
void 	DispBuffrefresh_F1(void);
void 	DispBuffrefresh_F2(void);
void 	DispBuffrefresh_F3(void);
void 	DispBuffrefresh_F4(void);
void 	DispBuffrefresh_F5(void);
void 	DispBuffrefresh_FnCom(INT8U fn);
void 	DispBuffrefresh_OFF(void);

// �����Ͷ�Ӧ��ͬ�ĵط�
//==============================================================================
// ���������ʾ
#if	( (MACHINETYPE == P0_910U)||(MACHINETYPE == A0_820) )
// �������ɨ��
void led_Scan(void)
{ return; }
// ������ܳ�ʼ��
void digital_led_cntrl_Init(void)
{ return; }
// �ް汾��ʾ
void	DispVersion(void)
{ return; }
// ������ܶ�λ��ʾ
void	DispLedBit(void)
{ return; }
// �޿���������ʾ
void Display_Para_AD_Fn(void)
{ return; }
// ������ܵ�Դ�ر�
void All_Digital_Led_Pwr_OFF(void)
{ return; }
// ����ʾоƬˢ��
void	Digi_DotLed_Disp(void)
{ return; }

//void	Disp_Roll_Process(void)
//{ return; }
//==============================================================================
// ���������ʾ
#else
/*******************************************************************************
	 					��ʾaddress��F1~F4����
    input:   none     
	output:  none
*******************************************************************************/
void Display_Para_AD_Fn(void)
{
//	Disp.DatSel = 0;
	INT8U	i ;

	DispBuffrefresh_AD();
	Digi_DotLed_Disp();
	DelayMs(1000);						// ��ʱ1000ms
	
	for (i = 1; i < MAX_CFGPARA; i++)
	{
		DispBuffrefresh_FnCom(i);
		Digi_DotLed_Disp();
		DelayMs(1000);						// ��ʱ1000ms
	}
//	DispBuffrefresh_F1();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// ��ʱ1000ms
//	
//	DispBuffrefresh_F2();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// ��ʱ1000ms
//					
// 	DispBuffrefresh_F3();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// ��ʱ1000ms
//	
//	DispBuffrefresh_F4();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// ��ʱ1000ms
//
// 	DispBuffrefresh_F5();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// ��ʱ1000ms
}	
	
// ��ʽ�������ʾ	
	#if( (MACHINETYPE == P3_910U)||(MACHINETYPE == P6_910U)||(MACHINETYPE == P6_910U_G3R3)||(MACHINETYPE == A2_820)||(MACHINETYPE == A4_820)||(MACHINETYPE == P3_920U)||(MACHINETYPE == P6_920U)||(MACHINETYPE == P6_920U_G3R3)||(MACHINETYPE == P6_910U_R6) )

// ����ˢ��,�ڶ�ʱ����  led_Scan()��ˢ����ʾоƬ
void	Digi_DotLed_Disp(void)
{ return; }

//void	Disp_Roll_Process(void)
//{ return; }
/*******************************************************************************
	 					��ʾ�汾
    input:   none     
	output:  none
*******************************************************************************/
void	DispVersion(void)
{
 	INT8U	i;
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = Version[i]; }
	DelayMs(800);					  // ��ʱ800ms
}

/*******************************************************************************
	 					��ʾ������
    input:   none     
	output:  none
*******************************************************************************/
INT8U  CODE   LEDSecBit[7] = {37,38,39,40,41,42,36};		//�����Ӧ��LED���
void	DispLedBit(void)
{
 	INT8U	i,j;
	for(i = 0; i < 7; i++)
	{
		for(j = DIGILED_FIRST; j < LED_NUMBER; j++)
		{ DigiLed.num[j]   = LEDSecBit[i]; }		 //������
		 		    
		DelayMs(300);    //��ʱ150��ʾ
	}
}


/*******************************************************************************
	                    LED�����SPI����IO��ʼ��
    Input :	 None
    Output:  None	
*******************************************************************************/
void digital_led_cntrl_Init(void)
{	
	OE_SET;			//DISABLE
	SH_CLK_CLR;
	ST_CLK_CLR;
	OE_CLR;			//ENALBE

}

/*******************************************************************************
	                    LED����� �ر����е�Դ
    Input :	 None
    Output:  None	
*******************************************************************************/
void All_Digital_Led_Pwr_OFF(void)
{
	DIGITAL_LED1_OFF;		//LED 0 ==> OFF
//	LED_PORT_OUT |= 0x1f;  							//LED 1-LED5 ==> OFF
	DIGITAL_LED2_OFF;		//LED 1 ==> OFF
	DIGITAL_LED3_OFF;		//LED 2 ==> OFF
	DIGITAL_LED4_OFF;		//LED 3 ==> OFF
	DIGITAL_LED5_OFF;		//LED 4 ==> OFF
	DIGITAL_LED6_OFF;		//LED 5 ==> OFF
}
/*******************************************************************************
	                    ��ʾ��ǰɨ���led����          
    Input :	 UINT8 ucValue   //��ǰҪ��ʾ����ֵ
    Output:  None	
*******************************************************************************/
void led_Show(UINT8 ucValue)
{
	UINT8 i;

	ST_CLK_CLR;
	for (i = 0; i < 8; i++)
	{
		if (ucValue & 0x80)
			SDATA_SET;
		else
			SDATA_CLR;
		SH_CLK_CLR;
		NOP; NOP; NOP; NOP;
		SH_CLK_SET;
		ucValue = ucValue << 1; 
	}
//	led_SupplyPower();						      //����
	ST_CLK_SET;
}

/*******************************************************************************
	                    ��ʾһ��digital led         
    Input :	          None
    Output:         None
    Description:  ��ʾһ����������֣�ɨ��ʱ��: 72us         
*******************************************************************************/

void led_Scan(void)
{
    UINT8 ucValue;

//    if (sys.led_scan_flag == OFF) return;
//    sys.led_scan_flag = OFF;

    if( (++DigiLed.Order) >= LED_NUMBER ) 
    { DigiLed.Order = DIGILED_FIRST; }

	ucValue = DigiLed.num[DigiLed.Order];
	if(ucValue >= LED_TBL_MAX )
	{ ucValue = LED_TBL_MAX-1; }

	// ȫ�ر�������ʾ��Դ
	All_Digital_Led_Pwr_OFF();
	// ��Ӧ��ʾ��������595
    led_Show(LED_TBL[ucValue]);						//ȡ���ֵ ����595
	//��Ӧ����ܹ���
//	led_SupplyPower();						      	//��Ӧ����ܹ���
	switch(DigiLed.Order)
	{
		case DIGITAL_LED1:  DIGITAL_LED1_ON; break;		  //��λ����ܵ�Դ
		case DIGITAL_LED2:  DIGITAL_LED2_ON; break;		  //ʮλ����ܵ�Դ
		case DIGITAL_LED3:  DIGITAL_LED3_ON; break;		  //��λ����ܵ�Դ
		case DIGITAL_LED4:  DIGITAL_LED4_ON; break;		  //ǧλ����ܵ�Դ
		case DIGITAL_LED5:  DIGITAL_LED5_ON; break;		  //��λ����ܵ�Դ
		default:  			DIGITAL_LED6_ON; break;		  //ʮ��λ����ܵ�Դ
	}
}
//==============================================================================
// ��ʽ�������ʾ
	#elif( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
// �������ɨ��
void led_Scan(void)
{ return; }
// ������ܵ�Դ�ر�
void All_Digital_Led_Pwr_OFF(void)
{ return; }

/*******************************************************************************
	                    HT1632оƬ������ǰ
	//�Ϳ����� 100�� HT1632;->Ҫ�ı�HT1632��̬ǰҪ���� 100�o1632
    Input :	 None
    Output:  None	
*******************************************************************************/

void HT1632_PRECOMMAND(void) 																								  
{
	// 1
	HT1632_WR_L;
	HT1632_DATA_H;
	HT1632_WR_H;
	// 0
	HT1632_WR_L;
	HT1632_DATA_L;
	HT1632_WR_H;
	// 0
	HT1632_WR_L;
	HT1632_DATA_L;
	HT1632_WR_H;
}
/*******************************************************************************
	                    HT1632оƬ����RAMǰ
	//�Ϳ����� 101�� HT1632;->Ҫ����HT1632 RAMǰҪ���� 101�o1632
    Input :	 None
    Output:  None	
*******************************************************************************/

void HT1632_PREDATA(void) 
{
	// 1
	HT1632_WR_L;
	HT1632_DATA_H;
	HT1632_WR_H;
	// 0
	HT1632_WR_L;
	HT1632_DATA_L;
	HT1632_WR_H;
	// 1
	HT1632_WR_L;
	HT1632_DATA_H;
	HT1632_WR_H;
}
/*******************************************************************************
	                    ��������� HT1632ǰҪ��ָ��λַ
    Input :	 address  ��ַ
    Output:  None	
*******************************************************************************/

void HT1632_ADDRESS(INT8U address)
{	
	INT8U i;
	//��ַȡ��7λ //���͸�λ
	for(i = 0 ; i < 7 ; i++)
	{
		address <<= 1;							   // ��Ҫ��λ
		HT1632_WR_L;                               //clk = 0 for data ready
		if((address & 0x80) == 0)  HT1632_DATA_L;
		else                   	   HT1632_DATA_H;
		HT1632_WR_H;                              //clk = 1 for data write into 1632		
	}
}
/*******************************************************************************
	                    �����ݸ� HT1632
    Input :	 data  ����
    Output:  None	
*******************************************************************************/

void HT1632_DATA(INT8U dat)
{	
	INT8U i;
	//���͸�λ
	for(i = 0 ; i < 8 ; i++)
	{
		HT1632_WR_L;                               //clk = 0 for data ready
		if((dat & 0x80) == 0)  HT1632_DATA_L;
		else                   HT1632_DATA_H;
		HT1632_WR_H;                              //clk = 1 for data write into 1632
		dat <<= 1;
	}
}

/*******************************************************************************
	                    �趨HT1632������RAMΪͬһ����ֵ
    Input :	 addr ��ʼ��ַ��lenд�볤�ȣ�datд������ 
    Output:  None	
*******************************************************************************/

void HT1632_Write_AddrLenDat(INT8U cs,INT8U addr,INT8U len,INT8U dat)
{
	INT8U i;

	switch(cs)
	{
		case HT1632_CS1: HT1632_CS1_L; break;//1632 CS0 enable					
		case HT1632_CS2: HT1632_CS2_L; break;//1632 CS1 enable								
	}
//   HT1632_CS1_L;        //1632 CS0 enable
//   HT1632_CS2_L;        //1632 CS1 enable
   
	HT1632_PREDATA();           	// "101" , ready for write
	HT1632_ADDRESS(addr);         // address = 0
	for(i=0 ; i<len ; i++)
	{
	  HT1632_DATA(dat);     	//all RAM write 1
	}

	switch(cs)
	{
		case HT1632_CS1: HT1632_CS1_H; break;//1632 CS0 disable					
		case HT1632_CS2: HT1632_CS2_H; break;//1632 CS1 disable								
	}
//   HT1632_CS1_H;       //1632 CS disable
//   HT1632_CS2_H;       //1632 CS disable
}

/*******************************************************************************
	                    �Ѵ洢������д��HT1632RAM��
    Input :	 addr ��ʼ��ַ��lenд�볤�ȣ�ptr�洢���׵�ַ 
    Output:  None	
*******************************************************************************/

void HT1632_Write_AddrLenPtr(INT8U cs,INT8U addr,INT8U len,INT8U XDATA *ptr)
{
	INT8U i;

	switch(cs)
	{
		case HT1632_CS1: HT1632_CS1_L; break;//1632 CS0 enable					
		case HT1632_CS2: HT1632_CS2_L; break;//1632 CS1 enable								
	}
//   HT1632_CS1_L;        //1632 CS0 enable
//   HT1632_CS2_L;        //1632 CS1 enable
   
	HT1632_PREDATA();           	// "101" , ready for write
	HT1632_ADDRESS(addr);         // address = 0
	for(i=0 ; i<len ; i++)
	{
	 	HT1632_DATA(*(ptr+i));     	//all RAM write 1
	}

	switch(cs)
	{
		case HT1632_CS1: HT1632_CS1_H; break;//1632 CS0 disable					
		case HT1632_CS2: HT1632_CS2_H; break;//1632 CS1 disable								
	}
//   HT1632_CS1_H;       //1632 CS disable
//   HT1632_CS2_H;       //1632 CS disable
}
/*******************************************************************************
	                    �趨HT1632������RAMΪͬһ����ֵ
    Input :	 ToSetData  Ҫ���õ�����
    Output:  None	
*******************************************************************************/

void HT1632_ALLRAM_OFF(void)
{
	HT1632_Write_AddrLenDat(HT1632_CS1,0x00,32,0x00);		  // �ӵ�ַ00��ʼд��32��0
	HT1632_Write_AddrLenDat(HT1632_CS2,0x00,32,0x00);		  // �ӵ�ַ00��ʼд��32��0
}

/*******************************************************************************
	                    ��������� HT1632
    Input :	 command  ������
    Output:  None	
*******************************************************************************/
void HT1632_COMMAND(INT8U command)
{
	INT8U i;
	//���͸�λ
	for(i = 0 ; i < 8 ; i++)
	{
		HT1632_WR_L;                               //clk = 0 for data ready
		if((command & 0x80) == 0)  HT1632_DATA_L;
		else                       HT1632_DATA_H;
		HT1632_WR_H;                              //clk = 1 for data write into 1632
		command <<= 1;
	}
	HT1632_WR_L;
	// _nop();
	// _nop();
	HT1632_WR_H;    // X
}
/*******************************************************************************
	                    LED����HT1632оƬ��ʼ��
    Input :	 None
    Output:  None	
*******************************************************************************/
void HT1632_INIT_NO(INT8U HT1632No)
{
	switch(HT1632No)
	{
		case HT1632_CS1: HT1632_CS1_L; break;//1632 CS0 enable					
		case HT1632_CS2: HT1632_CS2_L; break;//1632 CS1 enable								
	}
	
    HT1632_PRECOMMAND();					 // ������ǰ��ǰ��100
    HT1632_COMMAND(C_COMMAND1632_SYSEN);     //SYSTEM ENABLE
    switch(HT1632No)
    {
		case HT1632_CS1:
			HT1632_COMMAND(C_COMMAND1632_N_8COM);    //P-MOS OUTPUT AND 16COMMON OPTION	
			HT1632_COMMAND(C_COMMAND1632_MST_M);  	  //MASTER MODE
			break;
		case HT1632_CS2:
			HT1632_COMMAND(C_COMMAND1632_N_8COM);  //P-MOS OUTPUT AND 16COMMON OPTION
			HT1632_COMMAND(C_COMMAND1632_SLV_M);  	//SLAVE MODE			
			break;
	}
    HT1632_COMMAND(C_COMMAND1632_LEDON);     //LED ON
    HT1632_COMMAND(C_COMMAND1632_BLINKOFF);  //BLINK OFF
    HT1632_COMMAND(C_COMMAND1632_PWMDUTY);   //PWM DUTY SET

	switch(HT1632No)
	{
		case HT1632_CS1: HT1632_CS1_H; break;//1632 CS0 disable					
		case HT1632_CS2: HT1632_CS2_H; break;//1632 CS1 disable								
	}	
}
/*******************************************************************************
	                    LED����HT1632оƬ��ʼ��
    Input :	 None
    Output:  None	
*******************************************************************************/
void digital_led_cntrl_Init(void)
{ 	
	// IO�ڳ�ʼ��
	HT1632_CS1_H;
	HT1632_CS2_H;
	HT1632_RD_H;
	HT1632_WR_H;
	HT1632_DATA_H;

	HT1632_INIT_NO(HT1632_CS1);		 // оƬ1��ʼ��
	HT1632_INIT_NO(HT1632_CS2);		 // оƬ2��ʼ��
}

/*******************************************************************************
	 					��ʾ�汾
    input:   none     
	output:  none
*******************************************************************************/
void	DispVersion(void)
{
//	TextOutCountArea("V3.0",4);
	INT8U	i;
	DispBuffrefresh_OFF();		// ��ȫ����ʾ
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = Version[i]; }
	Digi_DotLed_Disp();
	DelayMs(800);    //��ʱ800��ʾ
}

/*******************************************************************************
	 					��ʾ������
    input:   none     
	output:  none
*******************************************************************************/

void	DispLedBit(void)
{
	//����
	HT1632_ALLRAM_OFF();

//	//ȫ�� ����2�� ����4��
//	HT1632_Write_AddrLenDat(0x00,8,0xff);
//	DelayMs(360);	
//	HT1632_Write_AddrLenDat(0x10,8,0xff);
//	DelayMs(660);
//	//ȫ��
//	HT1632_ALLRAM_OFF();
//	DelayMs(100);
//	//ȫ�� ����2�� ����4��
//	HT1632_Write_AddrLenDat(0x20,8,0xff);
//	DelayMs(360);	
//	HT1632_Write_AddrLenDat(0x30,8,0xff);
// 	DelayMs(660);

	HT1632_Write_AddrLenDat(HT1632_CS1,0x00,16,0xff);
	DelayMs(525);	HT1632_ALLRAM_OFF();
//	HT1632_Write_AddrLenDat(HT1632_CS1,0x10,8,0xff);
//	DelayMs(300);	HT1632_ALLRAM_OFF();
	HT1632_Write_AddrLenDat(HT1632_CS1,0x20,16,0xff);
	DelayMs(525);	HT1632_ALLRAM_OFF();
//	HT1632_Write_AddrLenDat(HT1632_CS1,0x30,8,0xff);
//	DelayMs(300);	HT1632_ALLRAM_OFF();

	HT1632_Write_AddrLenDat(HT1632_CS2,0x00,16,0xff);
	DelayMs(525);	HT1632_ALLRAM_OFF();
//	HT1632_Write_AddrLenDat(HT1632_CS2,0x10,8,0xff);
//	DelayMs(300);	HT1632_ALLRAM_OFF();
	HT1632_Write_AddrLenDat(HT1632_CS2,0x20,16,0xff);
	DelayMs(525);	//HT1632_ALLRAM_OFF();
//	HT1632_Write_AddrLenDat(HT1632_CS2,0x30,8,0xff);
//	DelayMs(300);
	//ȫ��
	HT1632_ALLRAM_OFF();
//	DelayMs(100);
}


/*******************************************************************************
	 				����ָ���ַ�����ʾ���ݿ������鵽Ŀ��������
    input:   none     
	output:  none
*******************************************************************************/
	

void AssCopyDispDataArry(INT8U *DestData,INT8U DispContent)
{
	INT8U i;//addr LED ��ʾλ��
	

	if(DispContent >= LED_TBL_MAX)
	{ DispContent = ' '; } 

	for(i = 0;i < LED_RANKCOUNT;i++)	
	{ DestData[i] = LED_TBL[DispContent][i]; }
}

/*******************************************************************************
	 					��ʾ�������飬����buffer������
    input:   none     
	output:  none
*******************************************************************************/	
void	Disp_Data_Roll_Recombine_com(INT8U XDATA *ptr1)
{
	INT8U j, temp1,temp2;//,temp3;
	INT8U tmp1[8],tmp2[8];//,tmp3[8]

	//0
	AssCopyDispDataArry((tmp1),Buffer[0]);
	AssCopyDispDataArry((tmp2),Buffer[1]);
	for(j = 0; j < LED_RANKCOUNT; j++)
	{
	 	temp1 = (tmp1[j]<<0)&0XFC;	//1.��6λ
		temp2 = (tmp2[j]>>6)&0X03;	//2.��2λ
		*(ptr1+j)  = temp1 | temp2;
	}
	//1
	ptr1 += LED_RANKCOUNT;
//		AssCopyDispDataArry((tmp1),DigiLed.Srcode[1]);
	AssCopyDispDataArry((tmp1),Buffer[2]);
	for(j = 0; j < LED_RANKCOUNT; j++)
	{
		temp1 = (tmp2[j]<<2)&0XF0;   //2.��4λ
		temp2 = (tmp1[j]>>4)&0X0F;	//3.��4λ
		*(ptr1+j)  = temp1 | temp2;
	}
	//2
	ptr1 += LED_RANKCOUNT;
//	AssCopyDispDataArry((tmp1),DigiLed.Srcode[2]);
	AssCopyDispDataArry((tmp2),Buffer[3]);
	for(j = 0; j < LED_RANKCOUNT; j++)
	{
		temp1 = (tmp1[j]<<4)&0XC0;   //3.��2λ
		temp2 = (tmp2[j]>>2)&0X3F;	//4.��6λ
		*(ptr1+j)  = temp1 | temp2; 
	}
}

void	Disp_Data_Roll_Recombine(INT8U	area)
{
	INT8U i,j,dat;


	if(area == SRCODE_AREA_ALL)
	{
		//0
		for(i = 0,j = 0; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&SRCodeData[0][0]);

		//3
		for(i = 0,j = 4; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}		
		Disp_Data_Roll_Recombine_com(&SRCodeData[3][0]);

		//6
 		for(i = 0,j = 8; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&SRCodeData[6][0]);	

		//9
 		for(i = 0,j = 12; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&SRCodeData[9][0]);
		
		//12
 		for(i = 0,j = 16; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&SRCodeData[12][0]);

		//15
 		for(i = 0,j = 20; i < 4; i++,j++)
		{ 
		   if (j <  MAX_SRCODE_NUM )
		   		{ dat = DigiLed.Srcode[j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&SRCodeData[15][0]);
		
		//18
// 		for(i = 0,j = 24; i < 4; i++,j++)
//		{ 
//		   if (j <  MAX_SRCODE_NUM )
//		   		{ dat = DigiLed.Srcode[j]; }
//		   else	{ dat = DIGI_LED_OFF; }
//		   Buffer[i] = dat; 
//		}
//		Disp_Data_Roll_Recombine_com(&SRCodeData[18][0]);
									
	}
	else  if(area == COUNT_AREA)
	{
		//0
		if( (DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF) )
			{ j = 0; }
		else{ j = 1; }

 		for(i = 0; i < 4; i++,j++)
		{ 
		   if (j <  DIGILED_COUNT )
		   		{ dat = DigiLed.num[2+j]; }
		   else	{ dat = DIGI_LED_OFF; }
		   Buffer[i] = dat; 
		}
		Disp_Data_Roll_Recombine_com(&CountCodeData[0][0]);	

		//1
		if( (DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF) )
		{
	 		for(i = 0,j = 4; i < 4; i++,j++)
			{ 
			   if (j <  DIGILED_COUNT )
			   		{ dat = DigiLed.num[2+j]; }
			   else	{ dat = DIGI_LED_OFF; }
			   Buffer[i] = dat; 
			}
			Disp_Data_Roll_Recombine_com(&CountCodeData[3][0]);	
		}		
	}
	else 
	{
		AssCopyDispDataArry(&Buffer[0],DigiLed.num[0]);
		for(j = 0; j < LED_RANKCOUNT; j++)
		{
			SKUCodeData[0][j] = (Buffer[j]>>2) & 0X3F;;		//
		}

		AssCopyDispDataArry(&Buffer[0],DigiLed.num[1]);
		for(j = 0; j < LED_RANKCOUNT; j++)
		{
			SKUCodeData[1][j] = (Buffer[j]>>1) & 0X7F;;
		}			
	}

}


/*******************************************************************************
	 				������ʾ	ֻ����������ģʽ�ͻ���ģʽ�Ž���
    input:   none     
	output:  none
*******************************************************************************/
//���к�֧�ֹ���
void ROLLING_SRDATA(UINT8 length)
{
	INT8U i,j,temp,startdata[LED_RANKCOUNT];
	
	if(length > MAX_SRCODE_NUM) length = MAX_SRCODE_NUM;

	//�ȼ��µ�һ��
	for(i = 0; i < LED_RANKCOUNT; i++)
		startdata[i] = (SRCodeData[0][i] >>7)&0x01;
	//������ǰŲ
	for(i = 0; i< length-1; i++)
	{
		for(j = 0; j<LED_RANKCOUNT; j++)
		{	
			//�������ݵ����λ
			temp = (SRCodeData[i+1][j] >>7)&0x01;
			//ǰ��һλ + �������ݵ����λ
			SRCodeData[i][j] = ((SRCodeData[i][j]<<1)&0xFE) + temp;			
		}
	}
	//���һ������ 
 	for(j = 0; j<LED_RANKCOUNT; j++)
	{	
		SRCodeData[i][j] = ((SRCodeData[i][j]<<1)&0xFE) + startdata[j];			
	}
}

//���к�֧�ֹ���
void ROLLING_COUNTDATA(UINT8 length)
{
	INT8U i,j,temp,startdata[LED_RANKCOUNT];
	
	if(length > DIGILED_COUNT) length = DIGILED_COUNT;

	//�ȼ��µ�һ��
	for(i = 0; i < LED_RANKCOUNT; i++)
		startdata[i] = (CountCodeData[0][i] >>7)&0x01;
	//������ǰŲ
	for(i = 0; i< length-1; i++)
	{
		for(j = 0; j<LED_RANKCOUNT; j++)
		{	
			//�������ݵ����λ
			temp = (CountCodeData[i+1][j] >>7)&0x01;
			//ǰ��һλ + �������ݵ����λ
			CountCodeData[i][j] = ((CountCodeData[i][j]<<1)&0xFE) + temp;			
		}
	}
	//���һ������ 
 	for(j = 0; j<LED_RANKCOUNT; j++)
	{	
		CountCodeData[i][j] = ((CountCodeData[i][j]<<1)&0xFE) + startdata[j];			
	}
}
/*******************************************************************************
	 				������ʾ	ֻ����������ģʽ�ͻ���ģʽ�Ž���
    input:   none     
	output:  none
*******************************************************************************/
void Disp_Roll_Process(void)
{
	INT8U nLength ;

	// ������ʱʱ��δ���򷵻�
	if(Disp.RollDelayCnt != 0) return;
	Disp.RollDelayCnt 	= DISP_ROLL_SPEED;				//������ʱֵ��ֵ

	if(para.str.DispMode == 0X01)
	{	// ������Ϣ�������
		if(DigiLed.SrLen > 10)
		{
			//������ʾ�����ַ�������ʵ����Ҫ��LEDģ������
			// 8,9,10,11��Ӧ����8��10��12��14���
			nLength = DigiLed.SrLen - DigiLed.SrLen/4;

			ROLLING_SRDATA(nLength+1);			
			HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,32,&SRCodeData[0][0]);		  // ������д��HTC1632
			HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,32,&SRCodeData[4][0]);	  // ������д��HTC1632
		} 
	}
	else
	{
		// ������Ϣ�������
		if(DigiLed.SrLen > 4) 
		{	
			//������ʾ�����ַ�������ʵ����Ҫ��LEDģ������
			// 8,9,10,11��Ӧ����8��10��12��14���
			nLength = DigiLed.SrLen - DigiLed.SrLen/4;

			ROLLING_SRDATA(nLength+1);			
			HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,24,&SRCodeData[0][0]);		  // ������д��HTC1632
		}
		// �����������
		if( (DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF) )
		{
			//������ʾ�����ַ�������ʵ����Ҫ��LEDģ������
			ROLLING_COUNTDATA(6);			  // 5*8�У�ʵ����Ч5*6�У�����10����Ϊ���			
			HT1632_Write_AddrLenPtr(HT1632_CS2,0x10,24,&CountCodeData[0][0]);		  // ������д��HTC1632
		}
		// ��λ��������
		Disp_Data_Roll_Recombine(SKU_AREA);
		HT1632_Write_AddrLenPtr(HT1632_CS1,0x30,8,&SKUCodeData[0][0]);		  // ������д��HTC1632
		HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,8,&SKUCodeData[1][0]);		  // ������д��HTC1632
	}
}

/*******************************************************************************
	                    HT1632 ������ˢ����ʾ            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
void	Digi_DotLed_Disp(void)
{	

	// ������Ϣ����
	if( ((Disp.f_Rolling & ROLL_MODE_SRCINFO) == 0x00)||((Disp.f_Rolling & ROLL_MODE_SRCINFO_ED) != 0x00)  )
	{	// �ǹ���ģʽ �� ����ģʽ����������������
		// ���κ���������,
		Disp_Data_Roll_Recombine(SRCODE_AREA_ALL);
		Disp.f_Rolling &= ~ROLL_MODE_SRCINFO_ED;	
	}

	// ���Ϊ�ı���ʾģʽ������ʾ��λ��������

	if( (para.str.DispMode == 0X01)&&((Disp.Page == PAGE_RUN)||(Disp.Page == PAGE_BACKDISPLAY)) )
	{ 
		HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,32,&SRCodeData[0][0]);		  // ������д��HTC1632
		HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,32,&SRCodeData[4][0]);		  // ������д��HTC1632
		return;
	}
	// д��������Ϣ����
	HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,24,&SRCodeData[0][0]);		  // ������д��HTC1632

	// ������Ϣ����
	if( ((Disp.f_Rolling & ROLL_MODE_COUNT) == 0x00)||((Disp.f_Rolling & ROLL_MODE_COUNT_ED) != 0x00)  )
	{
		Disp_Data_Roll_Recombine(COUNT_AREA);
		Disp.f_Rolling &= ~ROLL_MODE_COUNT_ED;
	}
	// д��������Ϣ����
	HT1632_Write_AddrLenPtr(HT1632_CS2,0x10,24,&CountCodeData[0][0]);		  // ������д��HTC1632

	// ��λ��Ϣ����
	Disp_Data_Roll_Recombine(SKU_AREA);
	HT1632_Write_AddrLenPtr(HT1632_CS1,0x30,8,&SKUCodeData[0][0]);		  // ������д��HTC1632
	HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,8,&SKUCodeData[1][0]);		  // ������д��HTC1632

}
	#endif
#endif



///===================================================================================
// ��������


/************************************************************************
** ���������� ��ʾ000��999(�����ֽڳ��ȣ���������ٶ�)
** ��������:  Order   �� ��������ƫ�Ƶ�ַ��
              ASICREL ���������Ƿ�Ҫ��ASCII��
              Dat     : ����
************************************************************************/
void Calc_Value3(INT8U Order,INT8U ASICREL,INT16U Dat)
{		
	INT8U  residue08;
	Dat = Dat%1000;	
	Buffer[Order++] = ASICREL + (Dat / 100);
	residue08 = (Dat % 100);
	Buffer[Order++] = ASICREL + (residue08 / 10);
	Buffer[Order]   = ASICREL + (residue08 % 10);	
}

/************************************************************************
** ���������� ��ʾ0000��9999(�����ֽڳ��ȣ���������ٶ�)
** ��������:  Order   �� ��������ƫ�Ƶ�ַ��
              ASICREL ���������Ƿ�Ҫ��ASCII��
              Dat     : ���� 0-9999
************************************************************************/

void Calc_Value4(INT8U Order,INT8U ASICREL,INT16U Dat)
{			
	Dat = Dat%10000;
	Buffer[Order++] = (Dat / 1000) + ASICREL;
	Calc_Value3(Order,ASICREL,Dat);
}

/************************************************************************
** ���������� ��ʾ0000��9999(�����ֽڳ��ȣ���������ٶ�)
** ��������:  Order   �� ��������ƫ�Ƶ�ַ��
              ASICREL ���������Ƿ�Ҫ��ASCII��
              Dat     : ���� 0-9999
************************************************************************/

void Calc_Value5(INT8U Order,INT8U ASICREL,INT16U Dat)
{			
	Buffer[Order++] = (Dat / 10000) + ASICREL;
	Calc_Value4(Order,ASICREL,Dat);
}

/************************************************************************
** ���������� ǰ��0����ʾ
** ��������:  Order   �� ��������ƫ�Ƶ�ַ��
************************************************************************/
void Disp0ChgOff(INT8U Order,INT8U len)
{			
	INT8U	i;

	for(i = 0; i < len;i++)
	{
		if( (Buffer[Order+i] != '0')&&(Buffer[Order+i] != 0) )
		{ break; }
		Buffer[Order+i] = DIGI_LED_OFF;//����ʾ
	}
}

/*******************************************************************************
	 					����������ʾҳ��
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_CFGPWD(void)
{
	UINT8 i;	        

	// ����
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i] = '-'; }

	if(Key.CfgPwdCnt != 0)
	{ DigiLed.num[LED_NUMBER-1] = Key.CfgPwdCnt; }
}
/*******************************************************************************
	 					������ʾ������_ȫΪ��
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_OFF(void)
{
	INT8U	i;
	DigiLed.SrLen = 4; 	
	for (i = 0; i < MAX_SRCODE_NUM; i++)
	{ DigiLed.Srcode[i]  = DIGI_LED_OFF; }

	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = DIGI_LED_OFF;	}
}

/*******************************************************************************
	 					������ʾ������F1
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_AD(void)
{

	DispBuffrefresh_OFF();		// ��ȫ����ʾ

//#if	 ( (DIGILED_SKU >= 2)&&(DIGILED_COUNT >= 3) )
if( (os_disp_set.DispLed_count >= 5)||((os_disp_set.DispLed_sku >= 2)&&(os_disp_set.DispLed_count >= 3)) )
{
	DigiLed.num[DIGILED_FIRST]   = 'A';		       
    DigiLed.num[DIGILED_FIRST+1] = 'd';	
}
//#endif	            
	
	DigiLed.CntLen = 3;
	Calc_Value3(0,0,para_bk.str.address);		

	if( KeyMode.f_Config == OFF )
	{ Disp0ChgOff(0,2); }
	else if( Disp.f_OffDisp == ON )
	{ Buffer[Disp.Focus] = DIGI_LED_OFF; } 

	DigiLed.num[LED_NUMBER-3] = Buffer[0];		       
	DigiLed.num[LED_NUMBER-2] = Buffer[1];
	DigiLed.num[LED_NUMBER-1] = Buffer[2];
}
/*******************************************************************************
	 					������ʾ������Fn����
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_FnCom(INT8U fn)
{
	DispBuffrefresh_OFF();				 // ��ȫ����ʾ
	
//#if	 (DIGILED_SKU >= 2)
if	( (os_disp_set.DispLed_sku >= 2)||(os_disp_set.DispLed_count >= 5) )
{
   	DigiLed.num[DIGILED_FIRST]   = 'F';
	DigiLed.num[DIGILED_FIRST+1] = '0'+fn;
}
else
{
//#else
   	DigiLed.num[LED_NUMBER-3] = 'F';
	DigiLed.num[LED_NUMBER-2] = '0'+fn;
}
//#endif

	DigiLed.CntLen = 1;
	if( (KeyMode.f_Config == OFF)||(Disp.f_OffDisp == OFF) )
	{
	   switch(fn)
	   {
			case 1:	   DigiLed.num[LED_NUMBER-1] = para_bk.str.FinishedLED; break;
			case 2:	   DigiLed.num[LED_NUMBER-1] = para_bk.str.WtTime;      break;
			case 3:	   DigiLed.num[LED_NUMBER-1] = para_bk.str.FnDefine;    break;
			case 4:	   DigiLed.num[LED_NUMBER-1] = para_bk.str.DispMode;    break;
			case 5:	   DigiLed.num[LED_NUMBER-1] = para_bk.str.KeyProssLED; break;
	   }
	}
}

/*******************************************************************************
	 					������ʾ������F1
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F1(void)
{		       

	DispBuffrefresh_FnCom(1);
}

/*******************************************************************************
	 					������ʾ������F2
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F2(void)
{		        
	DispBuffrefresh_FnCom(2);
}

/*******************************************************************************
	 					������ʾ������F3
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F3(void)
{		       
	DispBuffrefresh_FnCom(3);
}

/*******************************************************************************
	 					������ʾ������F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F4(void)
{		        
	DispBuffrefresh_FnCom(4);
}

/*******************************************************************************
	 					������ʾ������F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F5(void)
{		        
	DispBuffrefresh_FnCom(5);
}
/*******************************************************************************
	 					������ʾ������F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_Wait(void)
{
	UINT8 i;	        
	if(Disp.Shift > LED_NUMBER)
	{ Disp.Shift = DIGILED_FIRST; }
	// ����
	for (i = DIGILED_FIRST; i < Disp.Shift; i++)
	{ DigiLed.num[i] = '-'; }
	// ���
	for (i = Disp.Shift; i < LED_NUMBER; i++)
	{ DigiLed.num[i] = DIGI_LED_OFF; }

	DigiLed.CntLen = 4;
	// ������Ϣ�����ݸ���
	DigiLed.SrLen  = 4;

	DigiLed.Srcode[0] = 'W'; 
	DigiLed.Srcode[1] = 'a';
	DigiLed.Srcode[2] = 'i';
	DigiLed.Srcode[3] = 't';
	for(i = 4;i < MAX_SRCODE_NUM;i++)
	{ DigiLed.Srcode[i] = DIGI_LED_OFF; }
}

/*******************************************************************************
	 					��ʾ��ͬ�ַ�
    input:   none     
	output:  none
*******************************************************************************/

void	DispSameChar(INT8U	DispChar)
{
 	INT8U	i;
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = DispChar;	}
}

/*******************************************************************************
	 					������ʾ������_������Թ���
    input:   none     
	output:  none
*******************************************************************************/
void	DispBuffrefresh_Task_Back_com(TASK_INFO XDATA *ptr)
{
	INT8U	i,len,dat;
	INT16U   usValue;
	
	DispBuffrefresh_OFF();		// ��ȫ����ʾ
	dat 	= (*ptr).sku;
	usValue = (*ptr).count;
	len 	= (*ptr).srcode_len;



	// ��λ�����ݸ���
//#if	  (DIGILED_SKU == 3)  
if(os_disp_set.DispLed_sku == 3)
{
	DigiLed.num[DIGILED_FIRST+0] = (*ptr).srcode[len-3];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-3];    	//��λ�Ÿ�λ
	DigiLed.num[DIGILED_FIRST+1] = (*ptr).srcode[len-2];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-2];    	//��λ�ŵ�λ
	DigiLed.num[DIGILED_FIRST+2] = (*ptr).srcode[len-1];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-1]; 
}
else if(os_disp_set.DispLed_sku == 2)
{
//#elif (DIGILED_SKU == 2)     
	// ��λ�Ų�����FF����ʾ
	if(dat != 0xff)
	{                        
		DigiLed.num[DIGILED_FIRST+0] = (dat>>4) & 0X0F;//(UINT8)((DispContentStack_NO[CurBackDispIndex]>>4) &0X0F);		//��λ�Ÿ�λ
		DigiLed.num[DIGILED_FIRST+1] =      dat & 0X0F;//(UINT8)(DispContentStack_NO[CurBackDispIndex] &0X0F);    		//��λ�ŵ�λ
	}	
}
//#endif 

	if(usValue > 9999)
	{ DigiLed.CntLen = 5; }
	else if(usValue > 999)
	{ DigiLed.CntLen = 4; }
	else if(usValue > 99)
	{ DigiLed.CntLen = 3; }
	else if(usValue > 9)
	{ DigiLed.CntLen = 2; }
	else 
	{ DigiLed.CntLen = 1; }

//	if( DigiLed.CntLen > DIGILED_COUNT)
//	{ DigiLed.CntLen = DIGILED_COUNT; }
	if( DigiLed.CntLen > os_disp_set.DispLed_count)
	{ DigiLed.CntLen = os_disp_set.DispLed_count; }

	// ������Ϣ�����ݸ���
	DigiLed.SrLen  = len;
	for(i = 0;i < len;i++)
	{ DigiLed.Srcode[i] = (*ptr).srcode[i]; }

   // ���������ݸ���
	if(Key.f_IsFnKeyPressed == ON)
	{	//��ʾ"----"   �������
		DigiLed.CntLen = 0;
      
//		for(i = 1; i <= DIGILED_COUNT; i++)
		for(i = 1; i <= os_disp_set.DispLed_count; i++)
		{ DigiLed.num[LED_NUMBER-i] = '-'; }
	}
	else if(para.str.DispMode == 0X01)	//�ַ���ʾ��
	{	// �ַ������ݺ�3λ��λ����Ϣһ����
		DigiLed.CntLen = 0;

		if(len > LED_NUMBER)
			{ len -= LED_NUMBER; }
		else{ len = 0; }

		for(i = 0; i < LED_NUMBER; i++,len++)
		{	// ֻ��ʾ0~9��A~Z��a~z��-����������

			dat = DigiLed.Srcode[len];

			if( (dat > 'z')||((dat < '0')&&(dat != '-'))
			  ||((dat > '9')&&(dat  < 'A'))||((dat > 'Z')&&(dat  < 'a'))  )
			{ dat = DIGI_LED_OFF; }	  //����ʾ

			DigiLed.num[DIGILED_FIRST + i] =   dat;		   //���ƫ���� DIGILED_FIRST
		}				
	}
//	else  if(usValue < DISPLAY_MAX_NUM)	//������ʾ��
	else  if(usValue < os_disp_set.DispMax_count)	//������ʾ��
	{			
		Calc_Value5(0,0,usValue);
		if( KeyMode.f_ChangeValue == OFF )
		{ Disp0ChgOff(0,COUNT_MAX-1); }			  // ������ǰ��0
		else if( Disp.f_OffDisp == ON )			  // ��������ʱ����Ӧ����λ��˸
//		{ Buffer[Disp.Focus+(COUNT_MAX-DIGILED_COUNT)] = DIGI_LED_OFF; }  
		{ Buffer[Disp.Focus+(COUNT_MAX-os_disp_set.DispLed_count)] = DIGI_LED_OFF; }  
		// ����ʾ�����ݷ����Ӧλ��ʾ�洢��
//		for(i = 1; i <= DIGILED_COUNT; i++)
		for(i = 1; i <= os_disp_set.DispLed_count; i++)
		{ DigiLed.num[LED_NUMBER-i] = Buffer[COUNT_MAX-i]; }			
	}
	else 
	{   //��ʾ"FULL"
//	#if  (DIGILED_COUNT == 2)
	if  (os_disp_set.DispLed_count == 2)
	{
		DigiLed.num[LED_NUMBER-2] = 'o';//-'A'+10;
		DigiLed.num[LED_NUMBER-1] = 'v';//-'A'+10; 
	}
	else if(os_disp_set.DispLed_count == 3)
	{
//	#elif(DIGILED_COUNT == 3)
		DigiLed.num[LED_NUMBER-3] = 'F';//-'A'+10; 
		DigiLed.num[LED_NUMBER-2] = 'U';//-'A'+10;
		DigiLed.num[LED_NUMBER-1] = 'L';//-'A'+10; 
	}
	else if(os_disp_set.DispLed_count >= 4)
	{
//	#elif (DIGILED_COUNT >= 4) //(DIGILED_COUNT == 5)
		DigiLed.num[LED_NUMBER-4] = 'F';//-'A'+10; 
		DigiLed.num[LED_NUMBER-3] = 'U';//-'A'+10;
		DigiLed.num[LED_NUMBER-2] = 'L';//-'A'+10;
		DigiLed.num[LED_NUMBER-1] = 'L';//-'A'+10; 
//	#endif
	}					      
	}

		
	// ����������Ϣ������־��ˢ�¹�����������־
	if( ((para.str.DispMode == 0X01)&&(DigiLed.SrLen > 10))
     ||((para.str.DispMode == 0X00)&&(DigiLed.SrLen > 4)) )
	{
		if((Disp.f_Rolling & ROLL_MODE_SRCINFO) == 0x00)
		{ Disp.f_Rolling |= (ROLL_MODE_SRCINFO|ROLL_MODE_SRCINFO_ED); }
	}
	else
	{ Disp.f_Rolling &= ~(ROLL_MODE_SRCINFO|ROLL_MODE_SRCINFO_ED); }

	// ����������Ϣ������־��ˢ�¹�����������־ 		  
	if((para.str.DispMode == 0X00)&&(DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF)) 
	{
		if((Disp.f_Rolling & ROLL_MODE_COUNT) == 0x00)
		{ Disp.f_Rolling |= (ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); }	
	}
	else{ Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); }
}

/*******************************************************************************
	 					������ʾ������_������
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_BackDisp(void)
{
	DispBuffrefresh_Task_Back_com(&BackTask.info[BackTask.index]);
}

/*******************************************************************************
	 					������ʾ������_��ҵ��ʾ
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_TaskProcess(void)
{
	DispBuffrefresh_Task_Back_com(&UnfinishTask.info[0]);
}

/*******************************************************************************
	 					������ʾ������_���ݳ�����ʾ
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_NULL(void)
{
	INT8U	tmp = 0;
	DispBuffrefresh_OFF();		// ��ȫ����ʾ

//	DigiLed.CntLen = 4;
//#if	 (DIGILED_SKU >= 2)
//	
//	DigiLed.num[0] = 'N';//-'A'+10;		    //N
//    DigiLed.num[1] = 'U';//-'A'+10;		    //u
//
//	tmp = Disp.Value/256;
//	DigiLed.num[LED_NUMBER-4] = ((tmp>>4)&0x0f);
//	DigiLed.num[LED_NUMBER-3] = (tmp&0x0f);
//
//	tmp = (INT8U)Disp.Value;
//	DigiLed.num[LED_NUMBER-2] = ((tmp>>4)&0x0f);
//	DigiLed.num[LED_NUMBER-1] = (tmp&0x0f);
//#else
//	DigiLed.num[3] = 'N';//-'A'+10;		    //N
//    DigiLed.num[4] = 'U';//-'A'+10;		    //u
//	DigiLed.num[5] = 'L';//-'A'+10;		    //u
//#endif

}




/*******************************************************************************
	                    ����ǰ��ʾled����            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
void led_light_ctrl(UINT8 ucValue)
{
	switch (ucValue)
	{
		case LED_RED:  		LED_G_OFF; LED_B_OFF;  LED_R_ON;  break;		
		case LED_GREEN:		LED_R_OFF; LED_B_OFF;  LED_G_ON;  break;
		case LED_BLUE:		LED_R_OFF; LED_G_OFF;  LED_B_ON;  break;
		
		case LED_RG:  		LED_B_OFF; LED_R_ON;   LED_G_ON;  break;		
		case LED_RB:		LED_G_OFF; LED_R_ON;   LED_B_ON;  break;
		case LED_GB:		LED_R_OFF; LED_G_ON;   LED_B_ON;  break;
		case LED_RGB:		LED_R_ON;  LED_G_ON;   LED_B_ON;  break;			
		default:			  LED_R_OFF; LED_G_OFF; LED_B_OFF;break;
	}
}

/*******************************************************************************
				��ȡ����ʱ�䣬��λms
	Input :		none
	Output :	16λ�ĵ���ʱ�䣬��ϵͳʱ�����Լ�����ֵ
*******************************************************************************/

void GetLedOnTime(void)
{
	UINT16 unRet ;//= led_flash_period;
	switch(led.flash_mode)
	{
		case LED_FLASH_MODE_2V1:		unRet = (INT16U)(led.flash_period*2) ;break;
		case LED_FLASH_MODE_5V1:		unRet = (INT16U)(led.flash_period*5) ;break;
		case LED_FLASH_MODE_10V1:		unRet = (INT16U)(led.flash_period*10);break;
		default:						unRet = led.flash_period;	break;
	}

	led.time_refresh = unRet;		// ������˸ʱ��
}
/*******************************************************************************
				��ȡ����ʱ�䣬��λms
	Input :		none
	Output :	16λ�ĵ���ʱ�䣬��ϵͳʱ�����Լ�����ֵ
*******************************************************************************/

void GetLedOffTime(void)
{
	UINT16 unRet ;//= led_flash_period;
	switch(led.flash_mode)
	{
		case LED_FLASH_MODE_1V2:		unRet = (INT16U)(led.flash_period*2) ;break;
		case LED_FLASH_MODE_1V5:		unRet = (INT16U)(led.flash_period*5) ;break;
		case LED_FLASH_MODE_1V10:		unRet = (INT16U)(led.flash_period*10);break;
		default:						unRet = led.flash_period;	break;		
	}

	led.time_refresh = unRet;		// ������˸ʱ��
}
/*******************************************************************************
	 					ָʾ�ƿ���ģʽ
input:   
	    led_flash_period:  ��˸ʱ����
	    led_flash_NO:      ��˸�����
	    led_flash_mode:    ��˸ģʽ
output:  none					
*******************************************************************************/
void CMD_led_flash(void)
{
	if(led.light_type == LED_NONE)
	{ 	// ȫ��
		led_light_ctrl(LED_NONE);		// ȫ��
		return;
	}
	// ��ʾ��Ӧ��ɫ
	if(led.flash_mode == LED_FLASH_MODE_ALWAYS)	//������ʾ
	{   // ����˸ģʽ
		led_light_ctrl(led.light_type);			// ��������Ӧ��ɫ
		return;
	}
	//��˸��ʾ	
	if(led.time_refresh == 0)
	{ // ʱ�䵽״̬�ı�
	  //��ǰ��Ϊ��״̬
		if(led.flash_status == 0)
		{
			//�л�������״̬���������øı�������ڲ���
			led.flash_status = 1;			// �л�������״̬
			led_light_ctrl(led.light_type);	// ��������Ӧ��ɫ			
			GetLedOnTime();					// ȡ����ʱ�䣬ϵͳʱ�����Լ���0
		}
		//��ǰΪ����
		else
		{
			//�л�������״̬���������øı�������ڲ���
			led.flash_status = 0;			//�л�������״̬
			led_light_ctrl(LED_NONE);		// ȫ��	
			GetLedOffTime();	// ȡ���ʱ�䣬ϵͳʱ�����Լ���0
		}
	}
}

/*******************************************************************************
	 					���������ʾ
 // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Clear(void)
{
	Key.f_LongFunc        = FALSE; 		// �˳���������״̬
	KeyMode.f_Config      = OFF;		// �˳�����״̬
	KeyMode.f_BackDisp 	  = OFF;		// �˳�����״̬					   
	KeyMode.f_ChangeValue = OFF; 		// �˳��޸�״̬								   	
// 	BackTask.index = 0;		// ���Ա�������������0
	
	//��ʾ��Ϣ���	
	SetDispPage(PAGE_OFF);
	Disp.TatolTimeRefresh16 = DISP_IC_INI_PER; // �ر�ҳ���¶�ʱ��ʼ��IC
//	DispSameChar(DIGI_LED_OFF);
	//ָʾ�����
    led.light_type = LED_NONE;
//	led_light_ctrl(LED_NONE);
}


/*****************************************************************
	 					������ʾ���汾�ţ������룬
    input:   none     
	output:  none
*******************************************************************************/

void Start_Display(void)
{
	//��ʾ�汾 ������ʱʱ��
	DispVersion();

    //��ʾ������   // ��������
	DispLedBit();

	//��ʾȫ8
	DispSameChar('8');

	//LED��
	led_light_ctrl(LED_RED);			// ���������ʾ
 	DelayMs(500);

	led_light_ctrl(LED_GREEN);			// �����̵���ʾ
 	DelayMs(500);

	led_light_ctrl(LED_BLUE);			// ����������ʾ
 	DelayMs(500);

	// �����ȫ��
//	DispSameChar(' ');

//	led_light_ctrl(LED_RG);			// �������̵���ʾ
// 	DelayMs(500);
//	led_light_ctrl(LED_RB);			// ������������ʾ
// 	DelayMs(500);
//	led_light_ctrl(LED_GB);			// ������������ʾ
// 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// ��������ʾ

	Display_Para_AD_Fn();			// ��ʾAD ��ַ,F1~F4����
	Display_Clear();				// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
}

/************************************************************************
** ����������������ʾҳ�����ʾ��
** ���� :   LcdPage ��ʾ��ҳ�� , ��7λ����ҳ�棬��λ�����ҳ��ɷ񸲸�
************************************************************************/
void	SetDispPage(INT8U Page)
{	
//	 digital_led_refresh = ON;						 // ʹ�����ݸ��±�־
	 Disp.f_Refresh	= ON;						 // ʹ�����ݸ��±�־
	 Disp.Page = Page;
	 if( (Page != PAGE_RUN)&&(Page != PAGE_BACKDISPLAY) )
	 { Disp.f_Rolling = ROLL_MODE_OFF; }
}

/*******************************************************************************
	                    ��ʾ���������ݶ�ʱ���¼��            
    Input :	 None 
    Output:  None	
*******************************************************************************/
void	Time_LedDispBuffRefresh(void)
{
	if(Disp.f_Refresh == OFF) 
	{ 
		if(Key.f_LongFunc == TRUE)
		{  // ���ò�������״̬����ʱ�˳�
			if( Disp.TatolTimeRefresh8 == 0 )
			{ 
				Display_Clear(); 					 		// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
			}
		}		
		else if( (KeyMode.f_Config == ON)||(KeyMode.f_ChangeValue == ON)	)
		{
			if( Disp.TatolTimeRefresh8 == 0 )
			{  // �ܳ�ʱʱ�䵽
				if(KeyMode.f_Config == ON)
				{ Exit_Cfg_Mode();  }	   	// �˳�����ҳ��
				else
				{					
					Disp.Focus        		= 0;
					KeyMode.f_ChangeValue   = OFF;
					Disp.f_Refresh 			= ON;
				}           
			}
			else if(Disp.TimeRefresh == 0)
			{ 	// ��ʱ��˸
				Disp.f_Refresh = ON;		  //����ܸ���
				
				if( Disp.f_OffDisp == OFF )
				 	{ Disp.f_OffDisp = ON;  Disp.TimeRefresh = COUNT_FLICKER_OFF;  }
				else{ Disp.f_OffDisp = OFF; Disp.TimeRefresh = COUNT_FLICKER_ON; }				 
			}  		
		}		
		//��ʾLOADING ���� '-' '--' '---' '----' '-----' '------'
		else if((Disp.f_Loading != LOAD_MODE_OFF))
		{			
//			if(sys.digital_led_load_cnt == 0)
			if(Disp.TatolTimeRefresh16 == 0)
			{	// �ܳ�ʱʱ�䵽

				Disp.f_Loading    = LOAD_MODE_OFF;			// �����ȴ�������
				led.light_type    = led.light_type_BK;		// �ָ�ԭ��ʹ�õĵ��� 				 
				SetDispPage( PAGE_RUN );					// �ȴ���ɽ��빤��ҳ��              
			}
			else if(Disp.TimeRefresh == 0)
		   	{ 	// ��ʱ����
		   		Disp.f_Refresh   = ON; 				  			//����ܸ���
				Disp.TimeRefresh = DIGITAL_LED_LOAD_CNT_PER;	//����ֵ��ֵ
				Disp.Shift++;
			}  		
		}
   #if( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
		// HT1632C ��λ
		else  if(Disp.Page == PAGE_OFF)
		{
			if(Disp.TatolTimeRefresh16 == 0)	
			{ 
				Disp.TatolTimeRefresh16 = DISP_IC_INI_PER;
				digital_led_cntrl_Init(); 	 // �����оƬ��ʼ��
			}
		}

		// ������ʾ
		if(Disp.f_Rolling != ROLL_MODE_OFF) 
		{
			Disp_Roll_Process();								//����
		}
	#endif					
	}
}

/*******************************************************************************
	                    ��ʾ���������ݸ���            
    Input :	 None 
    Output:  None	
*******************************************************************************/
void	LedDispBuffRefresh(void)
{
	
	Time_LedDispBuffRefresh();			// ��ʱ���±�־
	// �޸������˳�
	if(Disp.f_Refresh == OFF) return; 
	// �и���
	Disp.f_Refresh = OFF;			// ����±�־

	switch(Disp.Page)
	{		
		case	PAGE_RUN:			DispBuffrefresh_TaskProcess();	break;			  //����ҳ��
		case	PAGE_WAIT:			DispBuffrefresh_Wait();			break;			  //�ȴ�����������ҳ��
		case	PAGE_BACKDISPLAY:	DispBuffrefresh_BackDisp(); 	break;			  //����ҳ��
		case	PAGE_CFGADDR:		DispBuffrefresh_AD();			break;			  //���õ�ַҳ��
		case	PAGE_CFGFUNC1:		DispBuffrefresh_F1();			break;			  //���ù���1ҳ��
		case	PAGE_CFGFUNC2:		DispBuffrefresh_F2();			break;			  //���ù���2ҳ��
		case	PAGE_CFGFUNC3:		DispBuffrefresh_F3();			break;			  //���ù���3ҳ��
		case	PAGE_CFGFUNC4:		DispBuffrefresh_F4();			break;			  //���ù���4ҳ��
		case	PAGE_CFGFUNC5:		DispBuffrefresh_F5();			break;			  //���ù���4ҳ��
		case	PAGE_NULL:			DispBuffrefresh_NULL();			break;			  //NULL
//		case	PAGE_START	 :		Page_Start();					break;			  //����ҳ��
		case	PAGE_CFGPWD:		DispBuffrefresh_CFGPWD();			break;			  //��������ҳ��
		default	:					DispBuffrefresh_OFF();		    break;			  //����ʾ														
	}

	// 
	Digi_DotLed_Disp();				// ��оƬˢ����ʾ
}

/*******************************************************************************
								END					
*******************************************************************************/

