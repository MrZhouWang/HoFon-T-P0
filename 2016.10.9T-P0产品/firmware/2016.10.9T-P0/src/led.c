
#define AP_LED_GLOBALS	
#include "extern_symbol.h" 
#include "ASCII_Led.h"						   // ASCII 码表对应数码管

// 函数声明
void	Digi_DotLed_Disp(void);
void  	DispBuffrefresh_AD(void);
void 	DispBuffrefresh_F1(void);
void 	DispBuffrefresh_F2(void);
void 	DispBuffrefresh_F3(void);
void 	DispBuffrefresh_F4(void);
void 	DispBuffrefresh_F5(void);
void 	DispBuffrefresh_FnCom(INT8U fn);
void 	DispBuffrefresh_OFF(void);

// 各机型对应不同的地方
//==============================================================================
// 无数码管显示
#if	( (MACHINETYPE == P0_910U)||(MACHINETYPE == A0_820) )
// 无数码管扫描
void led_Scan(void)
{ return; }
// 无数码管初始化
void digital_led_cntrl_Init(void)
{ return; }
// 无版本显示
void	DispVersion(void)
{ return; }
// 无数码管段位显示
void	DispLedBit(void)
{ return; }
// 无开机参数显示
void Display_Para_AD_Fn(void)
{ return; }
// 无数码管电源关闭
void All_Digital_Led_Pwr_OFF(void)
{ return; }
// 无显示芯片刷新
void	Digi_DotLed_Disp(void)
{ return; }

//void	Disp_Roll_Process(void)
//{ return; }
//==============================================================================
// 有数码管显示
#else
/*******************************************************************************
	 					显示address和F1~F4参数
    input:   none     
	output:  none
*******************************************************************************/
void Display_Para_AD_Fn(void)
{
//	Disp.DatSel = 0;
	INT8U	i ;

	DispBuffrefresh_AD();
	Digi_DotLed_Disp();
	DelayMs(1000);						// 延时1000ms
	
	for (i = 1; i < MAX_CFGPARA; i++)
	{
		DispBuffrefresh_FnCom(i);
		Digi_DotLed_Disp();
		DelayMs(1000);						// 延时1000ms
	}
//	DispBuffrefresh_F1();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// 延时1000ms
//	
//	DispBuffrefresh_F2();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// 延时1000ms
//					
// 	DispBuffrefresh_F3();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// 延时1000ms
//	
//	DispBuffrefresh_F4();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// 延时1000ms
//
// 	DispBuffrefresh_F5();
//	Digi_DotLed_Disp();
//	DelayMs(1000);						// 延时1000ms
}	
	
// 段式数码管显示	
	#if( (MACHINETYPE == P3_910U)||(MACHINETYPE == P6_910U)||(MACHINETYPE == P6_910U_G3R3)||(MACHINETYPE == A2_820)||(MACHINETYPE == A4_820)||(MACHINETYPE == P3_920U)||(MACHINETYPE == P6_920U)||(MACHINETYPE == P6_920U_G3R3)||(MACHINETYPE == P6_910U_R6) )

// 无需刷新,在定时里用  led_Scan()；刷新显示芯片
void	Digi_DotLed_Disp(void)
{ return; }

//void	Disp_Roll_Process(void)
//{ return; }
/*******************************************************************************
	 					显示版本
    input:   none     
	output:  none
*******************************************************************************/
void	DispVersion(void)
{
 	INT8U	i;
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = Version[i]; }
	DelayMs(800);					  // 延时800ms
}

/*******************************************************************************
	 					显示各段码
    input:   none     
	output:  none
*******************************************************************************/
INT8U  CODE   LEDSecBit[7] = {37,38,39,40,41,42,36};		//段码对应的LED库表
void	DispLedBit(void)
{
 	INT8U	i,j;
	for(i = 0; i < 7; i++)
	{
		for(j = DIGILED_FIRST; j < LED_NUMBER; j++)
		{ DigiLed.num[j]   = LEDSecBit[i]; }		 //段码亮
		 		    
		DelayMs(300);    //延时150显示
	}
}


/*******************************************************************************
	                    LED数码管SPI控制IO初始化
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
	                    LED数码管 关闭所有电源
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
	                    显示当前扫描的led内容          
    Input :	 UINT8 ucValue   //当前要显示的数值
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
//	led_SupplyPower();						      //供电
	ST_CLK_SET;
}

/*******************************************************************************
	                    显示一个digital led         
    Input :	          None
    Output:         None
    Description:  显示一个数码管数字，扫描时间: 72us         
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

	// 全关闭所有显示电源
	All_Digital_Led_Pwr_OFF();
	// 对应显示数据送入595
    led_Show(LED_TBL[ucValue]);						//取表的值 送入595
	//对应数码管供电
//	led_SupplyPower();						      	//对应数码管供电
	switch(DigiLed.Order)
	{
		case DIGITAL_LED1:  DIGITAL_LED1_ON; break;		  //个位数码管电源
		case DIGITAL_LED2:  DIGITAL_LED2_ON; break;		  //十位数码管电源
		case DIGITAL_LED3:  DIGITAL_LED3_ON; break;		  //百位数码管电源
		case DIGITAL_LED4:  DIGITAL_LED4_ON; break;		  //千位数码管电源
		case DIGITAL_LED5:  DIGITAL_LED5_ON; break;		  //万位数码管电源
		default:  			DIGITAL_LED6_ON; break;		  //十万位数码管电源
	}
}
//==============================================================================
// 段式数码管显示
	#elif( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
// 无数码管扫描
void led_Scan(void)
{ return; }
// 无数码管电源关闭
void All_Digital_Led_Pwr_OFF(void)
{ return; }

/*******************************************************************************
	                    HT1632芯片发命令前
	//送控制码 100给 HT1632;->要改变HT1632狀态前要先送 100給1632
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
	                    HT1632芯片更新RAM前
	//送控制码 101给 HT1632;->要更新HT1632 RAM前要先送 101給1632
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
	                    送资料码给 HT1632前要先指定位址
    Input :	 address  地址
    Output:  None	
*******************************************************************************/

void HT1632_ADDRESS(INT8U address)
{	
	INT8U i;
	//地址取低7位 //先送高位
	for(i = 0 ; i < 7 ; i++)
	{
		address <<= 1;							   // 不要高位
		HT1632_WR_L;                               //clk = 0 for data ready
		if((address & 0x80) == 0)  HT1632_DATA_L;
		else                   	   HT1632_DATA_H;
		HT1632_WR_H;                              //clk = 1 for data write into 1632		
	}
}
/*******************************************************************************
	                    送数据给 HT1632
    Input :	 data  数据
    Output:  None	
*******************************************************************************/

void HT1632_DATA(INT8U dat)
{	
	INT8U i;
	//先送高位
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
	                    设定HT1632的所有RAM为同一个数值
    Input :	 addr 起始地址，len写入长度，dat写入数据 
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
	                    把存储区数据写入HT1632RAM区
    Input :	 addr 起始地址，len写入长度，ptr存储区首地址 
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
	                    设定HT1632的所有RAM为同一个数值
    Input :	 ToSetData  要设置的数据
    Output:  None	
*******************************************************************************/

void HT1632_ALLRAM_OFF(void)
{
	HT1632_Write_AddrLenDat(HT1632_CS1,0x00,32,0x00);		  // 从地址00开始写入32个0
	HT1632_Write_AddrLenDat(HT1632_CS2,0x00,32,0x00);		  // 从地址00开始写入32个0
}

/*******************************************************************************
	                    送命令码给 HT1632
    Input :	 command  命令码
    Output:  None	
*******************************************************************************/
void HT1632_COMMAND(INT8U command)
{
	INT8U i;
	//先送高位
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
	                    LED阵列HT1632芯片初始化
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
	
    HT1632_PRECOMMAND();					 // 发命令前提前发100
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
	                    LED阵列HT1632芯片初始化
    Input :	 None
    Output:  None	
*******************************************************************************/
void digital_led_cntrl_Init(void)
{ 	
	// IO口初始化
	HT1632_CS1_H;
	HT1632_CS2_H;
	HT1632_RD_H;
	HT1632_WR_H;
	HT1632_DATA_H;

	HT1632_INIT_NO(HT1632_CS1);		 // 芯片1初始化
	HT1632_INIT_NO(HT1632_CS2);		 // 芯片2初始化
}

/*******************************************************************************
	 					显示版本
    input:   none     
	output:  none
*******************************************************************************/
void	DispVersion(void)
{
//	TextOutCountArea("V3.0",4);
	INT8U	i;
	DispBuffrefresh_OFF();		// 灭全部显示
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i]   = Version[i]; }
	Digi_DotLed_Disp();
	DelayMs(800);    //延时800显示
}

/*******************************************************************************
	 					显示各段码
    input:   none     
	output:  none
*******************************************************************************/

void	DispLedBit(void)
{
	//清屏
	HT1632_ALLRAM_OFF();

//	//全亮 先亮2个 再亮4个
//	HT1632_Write_AddrLenDat(0x00,8,0xff);
//	DelayMs(360);	
//	HT1632_Write_AddrLenDat(0x10,8,0xff);
//	DelayMs(660);
//	//全灭
//	HT1632_ALLRAM_OFF();
//	DelayMs(100);
//	//全亮 先亮2个 再亮4个
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
	//全灭
	HT1632_ALLRAM_OFF();
//	DelayMs(100);
}


/*******************************************************************************
	 				拷贝指定字符的显示内容控制数组到目标数组中
    input:   none     
	output:  none
*******************************************************************************/
	

void AssCopyDispDataArry(INT8U *DestData,INT8U DispContent)
{
	INT8U i;//addr LED 显示位置
	

	if(DispContent >= LED_TBL_MAX)
	{ DispContent = ' '; } 

	for(i = 0;i < LED_RANKCOUNT;i++)	
	{ DestData[i] = LED_TBL[DispContent][i]; }
}

/*******************************************************************************
	 					显示数据重组，存入buffer数组中
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
	 	temp1 = (tmp1[j]<<0)&0XFC;	//1.低6位
		temp2 = (tmp2[j]>>6)&0X03;	//2.高2位
		*(ptr1+j)  = temp1 | temp2;
	}
	//1
	ptr1 += LED_RANKCOUNT;
//		AssCopyDispDataArry((tmp1),DigiLed.Srcode[1]);
	AssCopyDispDataArry((tmp1),Buffer[2]);
	for(j = 0; j < LED_RANKCOUNT; j++)
	{
		temp1 = (tmp2[j]<<2)&0XF0;   //2.低4位
		temp2 = (tmp1[j]>>4)&0X0F;	//3.高4位
		*(ptr1+j)  = temp1 | temp2;
	}
	//2
	ptr1 += LED_RANKCOUNT;
//	AssCopyDispDataArry((tmp1),DigiLed.Srcode[2]);
	AssCopyDispDataArry((tmp2),Buffer[3]);
	for(j = 0; j < LED_RANKCOUNT; j++)
	{
		temp1 = (tmp1[j]<<4)&0XC0;   //3.低2位
		temp2 = (tmp2[j]>>2)&0X3F;	//4.高6位
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
	 				滚屏显示	只有任务运行模式和回显模式才进入
    input:   none     
	output:  none
*******************************************************************************/
//序列号支持滚动
void ROLLING_SRDATA(UINT8 length)
{
	INT8U i,j,temp,startdata[LED_RANKCOUNT];
	
	if(length > MAX_SRCODE_NUM) length = MAX_SRCODE_NUM;

	//先记下第一列
	for(i = 0; i < LED_RANKCOUNT; i++)
		startdata[i] = (SRCodeData[0][i] >>7)&0x01;
	//后续的前挪
	for(i = 0; i< length-1; i++)
	{
		for(j = 0; j<LED_RANKCOUNT; j++)
		{	
			//后面内容的最高位
			temp = (SRCodeData[i+1][j] >>7)&0x01;
			//前移一位 + 后面内容的最高位
			SRCodeData[i][j] = ((SRCodeData[i][j]<<1)&0xFE) + temp;			
		}
	}
	//最后一个内容 
 	for(j = 0; j<LED_RANKCOUNT; j++)
	{	
		SRCodeData[i][j] = ((SRCodeData[i][j]<<1)&0xFE) + startdata[j];			
	}
}

//序列号支持滚动
void ROLLING_COUNTDATA(UINT8 length)
{
	INT8U i,j,temp,startdata[LED_RANKCOUNT];
	
	if(length > DIGILED_COUNT) length = DIGILED_COUNT;

	//先记下第一列
	for(i = 0; i < LED_RANKCOUNT; i++)
		startdata[i] = (CountCodeData[0][i] >>7)&0x01;
	//后续的前挪
	for(i = 0; i< length-1; i++)
	{
		for(j = 0; j<LED_RANKCOUNT; j++)
		{	
			//后面内容的最高位
			temp = (CountCodeData[i+1][j] >>7)&0x01;
			//前移一位 + 后面内容的最高位
			CountCodeData[i][j] = ((CountCodeData[i][j]<<1)&0xFE) + temp;			
		}
	}
	//最后一个内容 
 	for(j = 0; j<LED_RANKCOUNT; j++)
	{	
		CountCodeData[i][j] = ((CountCodeData[i][j]<<1)&0xFE) + startdata[j];			
	}
}
/*******************************************************************************
	 				滚屏显示	只有任务运行模式和回显模式才进入
    input:   none     
	output:  none
*******************************************************************************/
void Disp_Roll_Process(void)
{
	INT8U nLength ;

	// 滚屏定时时间未到则返回
	if(Disp.RollDelayCnt != 0) return;
	Disp.RollDelayCnt 	= DISP_ROLL_SPEED;				//滚屏定时值赋值

	if(para.str.DispMode == 0X01)
	{	// 批次信息区域滚屏
		if(DigiLed.SrLen > 10)
		{
			//根据显示内容字符数计算实际需要的LED模组数量
			// 8,9,10,11对应空余8，10，12，14间隔
			nLength = DigiLed.SrLen - DigiLed.SrLen/4;

			ROLLING_SRDATA(nLength+1);			
			HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,32,&SRCodeData[0][0]);		  // 把数据写入HTC1632
			HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,32,&SRCodeData[4][0]);	  // 把数据写入HTC1632
		} 
	}
	else
	{
		// 批次信息区域滚屏
		if(DigiLed.SrLen > 4) 
		{	
			//根据显示内容字符数计算实际需要的LED模组数量
			// 8,9,10,11对应空余8，10，12，14间隔
			nLength = DigiLed.SrLen - DigiLed.SrLen/4;

			ROLLING_SRDATA(nLength+1);			
			HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,24,&SRCodeData[0][0]);		  // 把数据写入HTC1632
		}
		// 数据区域滚屏
		if( (DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF) )
		{
			//根据显示内容字符数计算实际需要的LED模组数量
			ROLLING_COUNTDATA(6);			  // 5*8列，实际有效5*6列，空余10列作为间隔			
			HT1632_Write_AddrLenPtr(HT1632_CS2,0x10,24,&CountCodeData[0][0]);		  // 把数据写入HTC1632
		}
		// 库位区域数据
		Disp_Data_Roll_Recombine(SKU_AREA);
		HT1632_Write_AddrLenPtr(HT1632_CS1,0x30,8,&SKUCodeData[0][0]);		  // 把数据写入HTC1632
		HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,8,&SKUCodeData[1][0]);		  // 把数据写入HTC1632
	}
}

/*******************************************************************************
	                    HT1632 各区域刷新显示            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
void	Digi_DotLed_Disp(void)
{	

	// 批次信息区域
	if( ((Disp.f_Rolling & ROLL_MODE_SRCINFO) == 0x00)||((Disp.f_Rolling & ROLL_MODE_SRCINFO_ED) != 0x00)  )
	{	// 非滚屏模式 或 滚屏模式但允许数据区更新
		// 批次号区域数据,
		Disp_Data_Roll_Recombine(SRCODE_AREA_ALL);
		Disp.f_Rolling &= ~ROLL_MODE_SRCINFO_ED;	
	}

	// 如果为文本显示模式，则不显示库位和数量区

	if( (para.str.DispMode == 0X01)&&((Disp.Page == PAGE_RUN)||(Disp.Page == PAGE_BACKDISPLAY)) )
	{ 
		HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,32,&SRCodeData[0][0]);		  // 把数据写入HTC1632
		HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,32,&SRCodeData[4][0]);		  // 把数据写入HTC1632
		return;
	}
	// 写入批次信息区域
	HT1632_Write_AddrLenPtr(HT1632_CS1,0x00,24,&SRCodeData[0][0]);		  // 把数据写入HTC1632

	// 数量信息区域
	if( ((Disp.f_Rolling & ROLL_MODE_COUNT) == 0x00)||((Disp.f_Rolling & ROLL_MODE_COUNT_ED) != 0x00)  )
	{
		Disp_Data_Roll_Recombine(COUNT_AREA);
		Disp.f_Rolling &= ~ROLL_MODE_COUNT_ED;
	}
	// 写入数量信息区域
	HT1632_Write_AddrLenPtr(HT1632_CS2,0x10,24,&CountCodeData[0][0]);		  // 把数据写入HTC1632

	// 库位信息区域
	Disp_Data_Roll_Recombine(SKU_AREA);
	HT1632_Write_AddrLenPtr(HT1632_CS1,0x30,8,&SKUCodeData[0][0]);		  // 把数据写入HTC1632
	HT1632_Write_AddrLenPtr(HT1632_CS2,0x00,8,&SKUCodeData[1][0]);		  // 把数据写入HTC1632

}
	#endif
#endif



///===================================================================================
// 公共部分


/************************************************************************
** 功能描述： 显示000～999(考虑字节长度，提高运算速度)
** 输入声明:  Order   ： 数据区的偏移地址，
              ASICREL ：数字码是否要变ASCII码
              Dat     : 数据
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
** 功能描述： 显示0000～9999(考虑字节长度，提高运算速度)
** 输入声明:  Order   ： 数据区的偏移地址，
              ASICREL ：数字码是否要变ASCII码
              Dat     : 数据 0-9999
************************************************************************/

void Calc_Value4(INT8U Order,INT8U ASICREL,INT16U Dat)
{			
	Dat = Dat%10000;
	Buffer[Order++] = (Dat / 1000) + ASICREL;
	Calc_Value3(Order,ASICREL,Dat);
}

/************************************************************************
** 功能描述： 显示0000～9999(考虑字节长度，提高运算速度)
** 输入声明:  Order   ： 数据区的偏移地址，
              ASICREL ：数字码是否要变ASCII码
              Dat     : 数据 0-9999
************************************************************************/

void Calc_Value5(INT8U Order,INT8U ASICREL,INT16U Dat)
{			
	Buffer[Order++] = (Dat / 10000) + ASICREL;
	Calc_Value4(Order,ASICREL,Dat);
}

/************************************************************************
** 功能描述： 前导0灭显示
** 输入声明:  Order   ： 数据区的偏移地址，
************************************************************************/
void Disp0ChgOff(INT8U Order,INT8U len)
{			
	INT8U	i;

	for(i = 0; i < len;i++)
	{
		if( (Buffer[Order+i] != '0')&&(Buffer[Order+i] != 0) )
		{ break; }
		Buffer[Order+i] = DIGI_LED_OFF;//灭显示
	}
}

/*******************************************************************************
	 					配置密码显示页面
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_CFGPWD(void)
{
	UINT8 i;	        

	// 亮的
	for (i = DIGILED_FIRST; i < LED_NUMBER; i++)
	{ DigiLed.num[i] = '-'; }

	if(Key.CfgPwdCnt != 0)
	{ DigiLed.num[LED_NUMBER-1] = Key.CfgPwdCnt; }
}
/*******************************************************************************
	 					更新显示数据区_全为灭
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
	 					更新显示数据区F1
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_AD(void)
{

	DispBuffrefresh_OFF();		// 灭全部显示

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
	 					更新显示数据区Fn共用
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_FnCom(INT8U fn)
{
	DispBuffrefresh_OFF();				 // 灭全部显示
	
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
	 					更新显示数据区F1
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F1(void)
{		       

	DispBuffrefresh_FnCom(1);
}

/*******************************************************************************
	 					更新显示数据区F2
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F2(void)
{		        
	DispBuffrefresh_FnCom(2);
}

/*******************************************************************************
	 					更新显示数据区F3
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F3(void)
{		       
	DispBuffrefresh_FnCom(3);
}

/*******************************************************************************
	 					更新显示数据区F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F4(void)
{		        
	DispBuffrefresh_FnCom(4);
}

/*******************************************************************************
	 					更新显示数据区F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_F5(void)
{		        
	DispBuffrefresh_FnCom(5);
}
/*******************************************************************************
	 					更新显示数据区F4
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_Wait(void)
{
	UINT8 i;	        
	if(Disp.Shift > LED_NUMBER)
	{ Disp.Shift = DIGILED_FIRST; }
	// 亮的
	for (i = DIGILED_FIRST; i < Disp.Shift; i++)
	{ DigiLed.num[i] = '-'; }
	// 灭的
	for (i = Disp.Shift; i < LED_NUMBER; i++)
	{ DigiLed.num[i] = DIGI_LED_OFF; }

	DigiLed.CntLen = 4;
	// 批次信息区数据更新
	DigiLed.SrLen  = 4;

	DigiLed.Srcode[0] = 'W'; 
	DigiLed.Srcode[1] = 'a';
	DigiLed.Srcode[2] = 'i';
	DigiLed.Srcode[3] = 't';
	for(i = 4;i < MAX_SRCODE_NUM;i++)
	{ DigiLed.Srcode[i] = DIGI_LED_OFF; }
}

/*******************************************************************************
	 					显示相同字符
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
	 					更新显示数据区_任务回显共用
    input:   none     
	output:  none
*******************************************************************************/
void	DispBuffrefresh_Task_Back_com(TASK_INFO XDATA *ptr)
{
	INT8U	i,len,dat;
	INT16U   usValue;
	
	DispBuffrefresh_OFF();		// 灭全部显示
	dat 	= (*ptr).sku;
	usValue = (*ptr).count;
	len 	= (*ptr).srcode_len;



	// 库位区数据更新
//#if	  (DIGILED_SKU == 3)  
if(os_disp_set.DispLed_sku == 3)
{
	DigiLed.num[DIGILED_FIRST+0] = (*ptr).srcode[len-3];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-3];    	//货位号高位
	DigiLed.num[DIGILED_FIRST+1] = (*ptr).srcode[len-2];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-2];    	//货位号低位
	DigiLed.num[DIGILED_FIRST+2] = (*ptr).srcode[len-1];//DispContentStack_SrCode[CurBackDispIndex][DispContentStack_SrCodeLen[CurBackDispIndex]-1]; 
}
else if(os_disp_set.DispLed_sku == 2)
{
//#elif (DIGILED_SKU == 2)     
	// 库位号不等于FF则显示
	if(dat != 0xff)
	{                        
		DigiLed.num[DIGILED_FIRST+0] = (dat>>4) & 0X0F;//(UINT8)((DispContentStack_NO[CurBackDispIndex]>>4) &0X0F);		//货位号高位
		DigiLed.num[DIGILED_FIRST+1] =      dat & 0X0F;//(UINT8)(DispContentStack_NO[CurBackDispIndex] &0X0F);    		//货位号低位
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

	// 批次信息区数据更新
	DigiLed.SrLen  = len;
	for(i = 0;i < len;i++)
	{ DigiLed.Srcode[i] = (*ptr).srcode[i]; }

   // 数量区数据更新
	if(Key.f_IsFnKeyPressed == ON)
	{	//显示"----"   满箱操作
		DigiLed.CntLen = 0;
      
//		for(i = 1; i <= DIGILED_COUNT; i++)
		for(i = 1; i <= os_disp_set.DispLed_count; i++)
		{ DigiLed.num[LED_NUMBER-i] = '-'; }
	}
	else if(para.str.DispMode == 0X01)	//字符显示型
	{	// 字符型数据和3位库位的信息一致了
		DigiLed.CntLen = 0;

		if(len > LED_NUMBER)
			{ len -= LED_NUMBER; }
		else{ len = 0; }

		for(i = 0; i < LED_NUMBER; i++,len++)
		{	// 只显示0~9，A~Z，a~z，-，其他都灭

			dat = DigiLed.Srcode[len];

			if( (dat > 'z')||((dat < '0')&&(dat != '-'))
			  ||((dat > '9')&&(dat  < 'A'))||((dat > 'Z')&&(dat  < 'a'))  )
			{ dat = DIGI_LED_OFF; }	  //灭显示

			DigiLed.num[DIGILED_FIRST + i] =   dat;		   //添加偏移量 DIGILED_FIRST
		}				
	}
//	else  if(usValue < DISPLAY_MAX_NUM)	//任务显示型
	else  if(usValue < os_disp_set.DispMax_count)	//任务显示型
	{			
		Calc_Value5(0,0,usValue);
		if( KeyMode.f_ChangeValue == OFF )
		{ Disp0ChgOff(0,COUNT_MAX-1); }			  // 灭数量前导0
		else if( Disp.f_OffDisp == ON )			  // 设置数量时，对应焦点位闪烁
//		{ Buffer[Disp.Focus+(COUNT_MAX-DIGILED_COUNT)] = DIGI_LED_OFF; }  
		{ Buffer[Disp.Focus+(COUNT_MAX-os_disp_set.DispLed_count)] = DIGI_LED_OFF; }  
		// 把显示的数据放入对应位显示存储区
//		for(i = 1; i <= DIGILED_COUNT; i++)
		for(i = 1; i <= os_disp_set.DispLed_count; i++)
		{ DigiLed.num[LED_NUMBER-i] = Buffer[COUNT_MAX-i]; }			
	}
	else 
	{   //显示"FULL"
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

		
	// 设置批次信息滚屏标志和刷新滚屏数据区标志
	if( ((para.str.DispMode == 0X01)&&(DigiLed.SrLen > 10))
     ||((para.str.DispMode == 0X00)&&(DigiLed.SrLen > 4)) )
	{
		if((Disp.f_Rolling & ROLL_MODE_SRCINFO) == 0x00)
		{ Disp.f_Rolling |= (ROLL_MODE_SRCINFO|ROLL_MODE_SRCINFO_ED); }
	}
	else
	{ Disp.f_Rolling &= ~(ROLL_MODE_SRCINFO|ROLL_MODE_SRCINFO_ED); }

	// 设置数量信息滚屏标志和刷新滚屏数据区标志 		  
	if((para.str.DispMode == 0X00)&&(DigiLed.CntLen > 4)&&(KeyMode.f_ChangeValue == OFF)) 
	{
		if((Disp.f_Rolling & ROLL_MODE_COUNT) == 0x00)
		{ Disp.f_Rolling |= (ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); }	
	}
	else{ Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); }
}

/*******************************************************************************
	 					更新显示数据区_回显用
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_BackDisp(void)
{
	DispBuffrefresh_Task_Back_com(&BackTask.info[BackTask.index]);
}

/*******************************************************************************
	 					更新显示数据区_作业显示
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_TaskProcess(void)
{
	DispBuffrefresh_Task_Back_com(&UnfinishTask.info[0]);
}

/*******************************************************************************
	 					更新显示数据区_数据出错显示
    input:   none     
	output:  none
*******************************************************************************/
void DispBuffrefresh_NULL(void)
{
	INT8U	tmp = 0;
	DispBuffrefresh_OFF();		// 灭全部显示

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
	                    给当前显示led供电            
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
				获取灯亮时间，单位ms
	Input :		none
	Output :	16位的灯亮时间，在系统时钟里自减该数值
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

	led.time_refresh = unRet;		// 开灯闪烁时间
}
/*******************************************************************************
				获取灯灭时间，单位ms
	Input :		none
	Output :	16位的灯灭时间，在系统时钟里自减该数值
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

	led.time_refresh = unRet;		// 开灯闪烁时间
}
/*******************************************************************************
	 					指示灯控制模式
input:   
	    led_flash_period:  闪烁时间间隔
	    led_flash_NO:      闪烁灯序号
	    led_flash_mode:    闪烁模式
output:  none					
*******************************************************************************/
void CMD_led_flash(void)
{
	if(led.light_type == LED_NONE)
	{ 	// 全灭
		led_light_ctrl(LED_NONE);		// 全灭
		return;
	}
	// 显示对应灯色
	if(led.flash_mode == LED_FLASH_MODE_ALWAYS)	//常亮显示
	{   // 非闪烁模式
		led_light_ctrl(led.light_type);			// 仅点亮对应颜色
		return;
	}
	//闪烁显示	
	if(led.time_refresh == 0)
	{ // 时间到状态改变
	  //当前灯为灭状态
		if(led.flash_status == 0)
		{
			//切换到灯亮状态，并且设置改变灯亮周期参数
			led.flash_status = 1;			// 切换到灯亮状态
			led_light_ctrl(led.light_type);	// 仅点亮对应颜色			
			GetLedOnTime();					// 取亮灯时间，系统时钟里自减到0
		}
		//当前为亮灯
		else
		{
			//切换到灯灭状态，并且设置改变灯灭周期参数
			led.flash_status = 0;			//切换到灯亮状态
			led_light_ctrl(LED_NONE);		// 全灭	
			GetLedOffTime();	// 取灭灯时间，系统时钟里自减到0
		}
	}
}

/*******************************************************************************
	 					清除所有显示
 // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Clear(void)
{
	Key.f_LongFunc        = FALSE; 		// 退出配置密码状态
	KeyMode.f_Config      = OFF;		// 退出配置状态
	KeyMode.f_BackDisp 	  = OFF;		// 退出回显状态					   
	KeyMode.f_ChangeValue = OFF; 		// 退出修改状态								   	
// 	BackTask.index = 0;		// 回显备份区的索引清0
	
	//显示信息清除	
	SetDispPage(PAGE_OFF);
	Disp.TatolTimeRefresh16 = DISP_IC_INI_PER; // 关闭页面下定时初始化IC
//	DispSameChar(DIGI_LED_OFF);
	//指示灯清除
    led.light_type = LED_NONE;
//	led_light_ctrl(LED_NONE);
}


/*****************************************************************
	 					开机显示，版本号，各段码，
    input:   none     
	output:  none
*******************************************************************************/

void Start_Display(void)
{
	//显示版本 ，含延时时间
	DispVersion();

    //显示各段码   // 检测数码管
	DispLedBit();

	//显示全8
	DispSameChar('8');

	//LED亮
	led_light_ctrl(LED_RED);			// 仅亮红灯显示
 	DelayMs(500);

	led_light_ctrl(LED_GREEN);			// 仅亮绿灯显示
 	DelayMs(500);

	led_light_ctrl(LED_BLUE);			// 仅亮蓝灯显示
 	DelayMs(500);

	// 数码管全灭
//	DispSameChar(' ');

//	led_light_ctrl(LED_RG);			// 仅亮红绿灯显示
// 	DelayMs(500);
//	led_light_ctrl(LED_RB);			// 仅亮红蓝灯显示
// 	DelayMs(500);
//	led_light_ctrl(LED_GB);			// 仅亮绿蓝灯显示
// 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// 灭所有显示

	Display_Para_AD_Fn();			// 显示AD 地址,F1~F4参数
	Display_Clear();				// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
}

/************************************************************************
** 功能描述：设置显示页面和显示层
** 输入 :   LcdPage 显示的页面 , 低7位代表页面，高位代表该页面可否覆盖
************************************************************************/
void	SetDispPage(INT8U Page)
{	
//	 digital_led_refresh = ON;						 // 使能数据更新标志
	 Disp.f_Refresh	= ON;						 // 使能数据更新标志
	 Disp.Page = Page;
	 if( (Page != PAGE_RUN)&&(Page != PAGE_BACKDISPLAY) )
	 { Disp.f_Rolling = ROLL_MODE_OFF; }
}

/*******************************************************************************
	                    显示缓存区数据定时更新检测            
    Input :	 None 
    Output:  None	
*******************************************************************************/
void	Time_LedDispBuffRefresh(void)
{
	if(Disp.f_Refresh == OFF) 
	{ 
		if(Key.f_LongFunc == TRUE)
		{  // 配置参数密码状态，超时退出
			if( Disp.TatolTimeRefresh8 == 0 )
			{ 
				Display_Clear(); 					 		// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
			}
		}		
		else if( (KeyMode.f_Config == ON)||(KeyMode.f_ChangeValue == ON)	)
		{
			if( Disp.TatolTimeRefresh8 == 0 )
			{  // 总超时时间到
				if(KeyMode.f_Config == ON)
				{ Exit_Cfg_Mode();  }	   	// 退出配置页面
				else
				{					
					Disp.Focus        		= 0;
					KeyMode.f_ChangeValue   = OFF;
					Disp.f_Refresh 			= ON;
				}           
			}
			else if(Disp.TimeRefresh == 0)
			{ 	// 定时闪烁
				Disp.f_Refresh = ON;		  //数码管更新
				
				if( Disp.f_OffDisp == OFF )
				 	{ Disp.f_OffDisp = ON;  Disp.TimeRefresh = COUNT_FLICKER_OFF;  }
				else{ Disp.f_OffDisp = OFF; Disp.TimeRefresh = COUNT_FLICKER_ON; }				 
			}  		
		}		
		//显示LOADING 过程 '-' '--' '---' '----' '-----' '------'
		else if((Disp.f_Loading != LOAD_MODE_OFF))
		{			
//			if(sys.digital_led_load_cnt == 0)
			if(Disp.TatolTimeRefresh16 == 0)
			{	// 总超时时间到

				Disp.f_Loading    = LOAD_MODE_OFF;			// 结束等待进度条
				led.light_type    = led.light_type_BK;		// 恢复原先使用的灯组 				 
				SetDispPage( PAGE_RUN );					// 等待完成进入工作页面              
			}
			else if(Disp.TimeRefresh == 0)
		   	{ 	// 定时滚动
		   		Disp.f_Refresh   = ON; 				  			//数码管更新
				Disp.TimeRefresh = DIGITAL_LED_LOAD_CNT_PER;	//计数值赋值
				Disp.Shift++;
			}  		
		}
   #if( (MACHINETYPE == P10_910U)||(MACHINETYPE == P10_910U_R10)||(MACHINETYPE == A10S_820) )
		// HT1632C 复位
		else  if(Disp.Page == PAGE_OFF)
		{
			if(Disp.TatolTimeRefresh16 == 0)	
			{ 
				Disp.TatolTimeRefresh16 = DISP_IC_INI_PER;
				digital_led_cntrl_Init(); 	 // 数码管芯片初始化
			}
		}

		// 滚屏显示
		if(Disp.f_Rolling != ROLL_MODE_OFF) 
		{
			Disp_Roll_Process();								//滚屏
		}
	#endif					
	}
}

/*******************************************************************************
	                    显示缓存区数据更新            
    Input :	 None 
    Output:  None	
*******************************************************************************/
void	LedDispBuffRefresh(void)
{
	
	Time_LedDispBuffRefresh();			// 定时更新标志
	// 无更新则退出
	if(Disp.f_Refresh == OFF) return; 
	// 有更新
	Disp.f_Refresh = OFF;			// 清更新标志

	switch(Disp.Page)
	{		
		case	PAGE_RUN:			DispBuffrefresh_TaskProcess();	break;			  //运行页面
		case	PAGE_WAIT:			DispBuffrefresh_Wait();			break;			  //等待（进度条）页面
		case	PAGE_BACKDISPLAY:	DispBuffrefresh_BackDisp(); 	break;			  //回显页面
		case	PAGE_CFGADDR:		DispBuffrefresh_AD();			break;			  //配置地址页面
		case	PAGE_CFGFUNC1:		DispBuffrefresh_F1();			break;			  //配置功能1页面
		case	PAGE_CFGFUNC2:		DispBuffrefresh_F2();			break;			  //配置功能2页面
		case	PAGE_CFGFUNC3:		DispBuffrefresh_F3();			break;			  //配置功能3页面
		case	PAGE_CFGFUNC4:		DispBuffrefresh_F4();			break;			  //配置功能4页面
		case	PAGE_CFGFUNC5:		DispBuffrefresh_F5();			break;			  //配置功能4页面
		case	PAGE_NULL:			DispBuffrefresh_NULL();			break;			  //NULL
//		case	PAGE_START	 :		Page_Start();					break;			  //开机页面
		case	PAGE_CFGPWD:		DispBuffrefresh_CFGPWD();			break;			  //配置密码页面
		default	:					DispBuffrefresh_OFF();		    break;			  //灭显示														
	}

	// 
	Digi_DotLed_Disp();				// 送芯片刷新显示
}

/*******************************************************************************
								END					
*******************************************************************************/

