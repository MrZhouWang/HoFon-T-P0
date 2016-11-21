

#define AP_PROCESS_GLOBALS
#include "extern_symbol.h"

/*******************************************************************************
	 					显示命令
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Info(INT8U XDATA * buf)
{
	UINT8  usReTaskNum,len0;			//本次下发的"电子标签显示内容"个数，

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	
	// 灯亮的状态、时间、模式 	  uart.rBuffer[8] 
	i = * (buf);
	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (i >> 5)&0X07;	  // 保存在灯组备份区域中
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type  = led.light_type_BK; }			  // 非进度条模式才更新灯组显示值
    
	//2、闪烁时间间隔
	switch((i >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3、闪烁模式
    led.flash_mode   = (i)&0X07;
	//4、闪烁用数据置位，停止闪烁
	led.time_refresh = 0;			// 让闪烁时间到
	led.flash_status = 0;			// 先亮

    // 上次采集到按钮任务数量(高4位)+本次任务数量(低4位) uart.rBuffer[9] 
	usReTaskNum = (* (buf+1))&0X0F;		//本次下发的"电子标签显示内容"个数， 
	// 剩余未下发任务数量 uart.rBuffer[10]			
	UnfinishTask.remain = * (buf+2);		 //本次作业中还未下发的个数

	UnfinishTask.num = 0;		   // 未完成任务个数清0
	FinishTask.num   = 0;		   // 已完成任务个数清0
	UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      //计算出一次任务总数量

	BackTask.num   = 0;			   // 回显备份区的保存总个数
	KeyMode.f_BackDisp = OFF;	   // 回显模式标志清除

	//根据本次下发显示的任务个数进行协议解析

	//来1组新数据，看当前剩余个数做处理			

	curIndex = 4;					// 数据位置
	len0     = * (buf+3); //uart.rBuffer[11];	// 第一个的序列号长度                 

	// 对应的数组
	pt = &UnfinishTask.info[UnfinishTask.num];		
	// 批次数据
	(*pt).srcode_len  = len0;
	for(i = 0; i < len0; i++)
	{
		if(i >= MAX_SRCODE_NUM) break;
		(*pt).srcode[i] = * (buf + curIndex + i);//uart.rBuffer[curIndex+i];
	}
	//库位号+数量
	curIndex += len0;			//uart.rBuffer[11]为第一个序列号长度 

	(*pt).sku	 = * (buf + curIndex);//uart.rBuffer[curIndex];	  //库位号
	(*pt).count  = (INT16U)((*(buf+curIndex+1))*256) + (INT16U)(*(buf+curIndex+2));//(INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量

	if(UnfinishTask.num == 0)
	{ OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ; } 	  // 保存当前数量

	UnfinishTask.num++;

	{ SetDispPage(PAGE_RUN); }
}

/*******************************************************************************
	 					作业命令
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Task_Process(void)
{
	UINT8  usReTaskNum,len0,len1;			//本次下发的"电子标签显示内容"个数，

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	//显示数据获取（货位号，数量）
	//本次下发的"电子标签显示内容"个数，     
	usReTaskNum = (uart.rBuffer[9])&0X0F;		//本次下发的"电子标签显示内容"个数， 			
 	//本次作业中还未下发的个数
//	task_unupdate_num = uart.rBuffer[10];
	UnfinishTask.remain = uart.rBuffer[10];		 //本次作业中还未下发的个数
	//先初始化数据
	//有2组待显示数据下来，则将原来的数据都冲掉，包括UN区与ED区

	// 上位机上次采集到OK按钮的任务数量
	switch(OsCmdSet.LastSoftGetOkCount)
	{
		case 0:     //作业启动的时候第一次发
		{
			//回显数据处理区域，新来一次作业任务的数据,当前所有任务先清除(设备2此重复接收此命令时),回显数据清零
			UnfinishTask.num = 0;		   // 未完成任务个数清0
			FinishTask.num   = 0;		   // 已完成任务个数清0
			UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      //计算出一次任务总数量

			BackTask.num   = 0;			   // 回显备份区的保存总个数
			KeyMode.f_BackDisp = OFF;	   // 回显模式标志清除
			break;
		}
		case 1:    //作业启动后，上位机发本次命令之前采集到1组完成的显示
		{
			FinishTask.num--;
			if(FinishTask.num == 1)
			{
				//后往前挪
				// 第一个指针是要保存区的地址，第二个指针是数据区的地址
				PutFinishedToStack_Com(&FinishTask.info[0],&FinishTask.info[1]);
				FinishTask.info[0].full = FinishTask.info[1].full;				
			}
			else
			{
				FinishTask.num = 0;
			}
			break;
		}
//		case 2:    //作业启动后，上位机发本次命令之前采集到2组完成的显示
//		{
//			task_finished_num = 0;
//			break;
//		}
		default:
		{
			FinishTask.num = 0;
			break;
		}
	}

	//根据本次下发显示的任务个数进行协议解析
	switch(usReTaskNum)
	{
		case 1:		//来1组新数据，看当前剩余个数做处理
		{			
			// 剩余0个或2个都保存在0组，剩余1个则保存在1组
			if(UnfinishTask.num >= MAX_TASK_NUM)
			{ UnfinishTask.num = 0; }

			pt = &UnfinishTask.info[UnfinishTask.num];
			if(uart.rBuffer[5] == 0X00)			//没有序列号的协议
			{
				curIndex = 11;					// 数据位置
				len0     = 0;                   // 批次长度为0
			}
			else if(uart.rBuffer[5] == 0X01)	//带序列号的协议
			{	
				curIndex = 12;					// 数据位置
		 		len0     = uart.rBuffer[11];	// 第一个的序列号长度                 
			}
			
			// 批次数据
			(*pt).srcode_len  = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				(*pt).srcode[i] = uart.rBuffer[curIndex+i];
			}
			//库位号+数量
			curIndex += len0;			//uart.rBuffer[11]为第一个序列号长度 

			(*pt).sku	 = uart.rBuffer[curIndex];	  //库位号
			(*pt).count  = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量

			if(UnfinishTask.num == 0)
			{ OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ; } 	  // 保存当前数量

			UnfinishTask.num++;
			break;
		}
		case 2:		//来2组新数据，原先的都冲掉，全部接收
		{
			if(uart.rBuffer[5] == 0X00)			//没有序列号的协议
			{
				curIndex = 11;					// 数据位置
				len0      = 0;                   // 第1个批次长度为0
				len1      = 0;                   // 第2个批次长度为0
			}
			else if(uart.rBuffer[5] == 0X01)	//带序列号的协议
			{	
				curIndex = 12;					// 数据位置
		 		len0  	 = uart.rBuffer[11];	// 第1个的序列号长度  
				len1     = uart.rBuffer[6] - len0 - 11; // 第2个的序列号长度 				              
			}			
			// 保存第1个批次信息
			UnfinishTask.info[0].srcode_len = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[0].srcode[i] = uart.rBuffer[curIndex+i];
			}
			//库位号1+数量1
			curIndex += len0;			//uart.rBuffer[11]为第一个序列号长度
			UnfinishTask.info[0].sku   = uart.rBuffer[curIndex];													 //库位号
			UnfinishTask.info[0].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量
                    
			//序列号2
			curIndex += 3;
			UnfinishTask.info[1].srcode_len = len1;
			for(i = 0; i < len1; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[1].srcode[i] = uart.rBuffer[curIndex+i];
			}
			//库位号2+数量2
			curIndex += len1;			//uart.rBuffer[11]为第一个序列号长度
			UnfinishTask.info[1].sku   = uart.rBuffer[curIndex];													 //库位号
			UnfinishTask.info[1].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量
			OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;			   // 保存当前数量
			UnfinishTask.num = 2;
			break;
		}
		default: 		break;
	}

	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (uart.rBuffer[8] >> 5)&0X07;	  // 保存在灯组备份区域中
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type    = led.light_type_BK; }			  // 非进度条模式才更新灯组显示值
    
	//2、闪烁时间间隔
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3、闪烁模式
    led.flash_mode   = (uart.rBuffer[8])&0X07;
	//4、闪烁用数据置位，停止闪烁
	led.time_refresh = 0;			// 让闪烁时间到
	led.flash_status = 0;			// 先亮

	//5、进度条模式下处理
	// 长等待则处理
	if(Disp.f_Loading == LOAD_MODE_LONG)
	{	
		INT16U	 LoadCount,MinWaitTime;

		//等待时间 > 最新等待时间,直接亮灯显示数据
		LoadCount   = DIGITAL_LED_LOAD_TIME_LONG - Disp.TatolTimeRefresh16;
//		MinWaitTime = (para.str.WtTime+1)*100;
		MinWaitTime = (para.str.WtTime+1);
		//等待时间 > 最新等待时间,直接亮灯显示数据
		if(LoadCount >= MinWaitTime)
		{
			Disp.f_Loading    = LOAD_MODE_OFF;			// 结束等待进度条
			led.light_type    = led.light_type_BK;		// 恢复原先使用的灯组
		}	
		//等待时间还没到最小等待时间，则继续等待到最小时间后才亮灯显示
		else
		{
			Disp.TatolTimeRefresh16  = MinWaitTime - LoadCount;
		}					
	}
	//非进度模式下条控制
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ SetDispPage(PAGE_RUN); }
}

/*******************************************************************************
	 					对多字节读命令响应
	input:   none     
	output:  none					
*******************************************************************************/
//应答Modbus 0x03号采集按钮命令
void ACK_Multi_Read(void)
{
	UINT8   addr;
	UINT16  usCRC;
    
	UnfinishTask.repeat = FALSE;					// 清重复接收到任务标志    
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
    uart.wLen       = 2;    
	if(OsCmdSet.CurDispMode == DISPMODE_NONE)		//??0模式下，应该无采集命令
	{  // 当前任务状态为非运行状态
		uart.wBuffer[2] = 0x00;                     //n byte	
	        
		usCRC = CRC16(uart.wBuffer, 3);
		uart.wBuffer[3] = (UINT8)usCRC;
		uart.wBuffer[4] = (UINT8)(usCRC>>8);
		uart.wLen       = 5; 	        
	}
	else if(OsCmdSet.CurDispMode == DISPMODE_TASK)
	{	// 当前任务状态设为运行状态
		//本次作业的最后一组数据，则需要所有都拾取完毕后再上传数据,原因如下：
		//最后一组数据，如果有2个待显示，如果第一个显示完毕并且已经按下OK键，此时标签显示第二个，此时上位机询问标签是否已经按下OK键
		//此时实际已经按下一个OK键，设备回答按下一个，上位机会继续询问是否按下OK键（因为设备还有下一个任务），此时标签该回答？？
		//但是任务过程中，上位机询问到标签已经按下OK键后，会补发下一道命令，在下一道命令到达标签之前，是不会有采集OK键命令到达的，
		//因此可以控制，但是，最后2组无法如此控制，因此必须一起上传
		if(FinishTask.num == 0)				 
		{  // 没有按下
			uart.wBuffer[2] = 0x02;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = 0X00;                    
	
			usCRC = CRC16(uart.wBuffer, 5);
			uart.wBuffer[5] = (UINT8)usCRC;
			uart.wBuffer[6] = (UINT8)(usCRC>>8);
			uart.wLen       = 7;                            
		}
		else  if(FinishTask.num == 1)
		{	// 1个按下
			uart.wBuffer[2] = 0x04;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = ((FinishTask.info[0].full<<4)|FinishTask.num); //((taks_finished_Full[0]<<4)|FinishTask.num);    
			uart.wBuffer[5] = FinishTask.info[0].count/256; //task_ed_goods_numH[0];                        
			uart.wBuffer[6] = (INT8U)FinishTask.info[0].count;//task_ed_goods_numL[0];    
	                
			usCRC = CRC16(uart.wBuffer, 7);
			uart.wBuffer[7] = (UINT8)usCRC;
			uart.wBuffer[8] = (UINT8)(usCRC>>8);
			uart.wLen       = 9;                            
		}
		else  if(FinishTask.num == 2)
		{	// 2个按下
			uart.wBuffer[2] = 0x06;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = ((FinishTask.info[1].full<<4)|FinishTask.num); //((taks_finished_Full[1]<<4)|FinishTask.num);    
			uart.wBuffer[5] = FinishTask.info[0].count/256; //task_ed_goods_numH[0];                        
			uart.wBuffer[6] = (INT8U)FinishTask.info[0].count;//task_ed_goods_numL[0];    
			uart.wBuffer[7] = FinishTask.info[1].count/256; //task_ed_goods_numH[1];                        
			uart.wBuffer[8] = (INT8U)FinishTask.info[1].count;//task_ed_goods_numL[1];    
	                
			usCRC = CRC16(uart.wBuffer, 9);
			uart.wBuffer[9] = (UINT8)usCRC;
			uart.wBuffer[10] = (UINT8)(usCRC>>8);
			uart.wLen       = 11;                            
		}
	}       
	SendSerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
}

void ACK_Multi_ReadTestAddr(void)
{
	UINT8   addr;
	UINT16  usCRC;
        
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
	uart.wBuffer[2] = 0x00;                     	//Len
	usCRC = CRC16(uart.wBuffer, 3);
	uart.wBuffer[3] = (UINT8)usCRC;
	uart.wBuffer[4] = (UINT8)(usCRC>>8);
	uart.wLen   = 5;                            
	SendSerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	SendSerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					对多字节写命令错误的响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write_RepeatError(void)
{
	UINT8   addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16  usCRC;

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x90;                         //ERROR code
	uart.wBuffer[2] = OsCmdSet.LastSoftGetOkCount;                       

	usCRC = CRC16(uart.wBuffer, 3);

	uart.wBuffer[3] = (UINT8)usCRC;
	uart.wBuffer[4] = (UINT8)(usCRC>>8);

	uart.wLen        = 5; 
	SendSerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write(void)
{
	UINT8   addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16  usCRC;

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x10;                         //function code
	uart.wBuffer[2] = 0x00;                         //start addr
	uart.wBuffer[3] = 0x00;
	uart.wBuffer[4] = 0x00;
	uart.wBuffer[5] = 0x05;

	usCRC = CRC16(uart.wBuffer, 6);

	uart.wBuffer[6] = (UINT8)usCRC;
	uart.wBuffer[7] = (UINT8)(usCRC>>8);

	uart.wLen        = 8; 
	SendSerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void ResetAddr(void)
{
	//显示清除,数据复位
	Display_Clear();			   // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	sys_data_reset();
	//地址清零
//	address[0] = 0;		address[1] = 0;		address[2] = 0;
	para.str.address = 0;
	SaveParaToBlock();
}
/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void DiscodeProtocalV1(void)
{
	UINT8   addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16  usCRC, retCRC;
//	UINT16 usValue = 0;
	UINT8 i = 0;

	retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
	usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
	usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H

	if (retCRC == usCRC) 						//crc校验成功 
	{
//	   	p_led_rxd = 0;
		LED_RXT_ON;							   // 点亮接收LED灯
		//0号广播命令地址，不需回复
		if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
		{
			//00 06 ?? 00 ?? ?? CRCH CRCL
			if(uart.rBuffer[3] == 0X00)						//地址清零
			{ ResetAddr(); }
			//00 06 ?? 01 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X01)				//接收地址，并显示，进入等待按键确认
			{
				if(OsCmdSet.beSettingAddr != 0X02)			// 未回复地址设定
				{
					OsCmdSet.beSettingAddr = 1;				// 进入地址设定
//					toSettedAddr = uart.rBuffer[5];;
					para_bk.str.address = uart.rBuffer[5];	// 把设定地址存入备份区
					//设置更新信息，显示地址设定提示
					led.light_type = LED_GREEN;				// 接收前亮第一组灯，接收后亮第二组灯
					led.flash_mode = LED_FLASH_MODE_ALWAYS;	// 常亮模式

					SetDispPage(PAGE_CFGADDR);				// 设置显示页面为地址显示
				}
			}
			//0X10段	广播显示
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//广播显示命令,如果## ## == 0X00 0X00，则显示地址
			{
				Disp.Value  = ((UINT16)(uart.rBuffer[4])) *256 + (UINT16)(uart.rBuffer[5]);

				for (i = 0; i < MAX_CFGPARA; i++)
				{ 			
					para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
				}
			    //根据参数显示参数
				if(Disp.Value == 0X00)
				{ SetDispPage(PAGE_CFGADDR);  }			   //显示地址
				else if(Disp.Value == 10001)
				{ SetDispPage(PAGE_CFGFUNC1); }			   //显示F1 参数
				else if(Disp.Value == 10002)
				{ SetDispPage(PAGE_CFGFUNC2); }			   //显示F2 参数
				else if(Disp.Value == 10003)
				{ SetDispPage(PAGE_CFGFUNC3); }			   //显示F3 参数
				else if(Disp.Value == 10004)
				{ SetDispPage(PAGE_CFGFUNC4); }			   //显示F4 参数
				else if(Disp.Value == 10005)
				{ SetDispPage(PAGE_CFGFUNC5); }			   //显示F5 参数
				else
				{ SetDispPage(PAGE_NULL); }				   // 出错显示

				led.light_type = LED_GREEN;					// 指示灯
				led.flash_mode = LED_FLASH_MODE_ALWAYS;		// 常亮模式
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//广播清除
			{
				Display_Clear();						   // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
				sys_data_reset();						   // 清除任务信息区
			}
			//0X20段	广播存储段
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//广播设置参数1(标签对应拣货任务完成后亮灯模式)
			{
				para.str.FinishedLED  = uart.rBuffer[5];   // 写入参数
				SetDispPage(PAGE_CFGFUNC1);				   // 设置显示页面为F1显示

				led.light_type = para.str.FinishedLED;		// 把指示灯改成F1参数亮的模式
				led.flash_mode = LED_FLASH_MODE_ALWAYS;	   // 常亮模式				
				SaveParaToBlock();						   // 对参数做判断				
			}
			//00 06 ?? 21 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X21)				//广播设置参数2(一对多任务时，进度条等待时间)
			{
				para.str.WtTime = uart.rBuffer[5];			// 写入参数
				SetDispPage(PAGE_CFGFUNC2);					// 设置显示页面为F2显示
				
 				led.light_type = LED_GREEN ;	   			   // 亮的模式
				led.flash_mode = LED_FLASH_MODE_1V1;	   // 常亮模式
				led.time_refresh = 0;						// 让闪烁时间到
				led.flash_status = 0;						// 先亮
				led.flash_period = 100;						// 闪烁时间基准
				SaveParaToBlock();							// 对参数做判断				
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				//广播设置参数3(功能键设定)
			{				
				para.str.FnDefine = uart.rBuffer[5];	   // 写入参数
				SetDispPage(PAGE_CFGFUNC3);				   // 设置显示页面为F3显示

 				led.light_type = LED_GREEN ;	   			   // 亮的模式
				led.flash_mode = LED_FLASH_MODE_1V1;	   // 常亮模式
				led.time_refresh = 0;						// 让闪烁时间到
				led.flash_status = 0;						// 先亮
				led.flash_period = 100;						// 闪烁时间基准
				SaveParaToBlock();							// 对参数做判断				
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				//广播设置参数4(显示模式设定)
			{
				para.str.DispMode = uart.rBuffer[5];	   // 写入参数
				SetDispPage(PAGE_CFGFUNC4);				   // 设置显示页面为F4显示

 				led.light_type = LED_GREEN ;	   			   // 亮的模式
				led.flash_mode = LED_FLASH_MODE_1V1;	   // 常亮模式
				led.time_refresh = 0;						// 让闪烁时间到
				led.flash_status = 0;						// 先亮	 
				led.flash_period = 100;						// 闪烁时间基准
				SaveParaToBlock();							// 对参数做判断	
				
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X24)				//广播设置参数4(显示模式设定)
			{
				para.str.KeyProssLED = uart.rBuffer[5];	   // 写入参数
				SetDispPage(PAGE_CFGFUNC5);				   // 设置显示页面为F5显示

				led.light_type = para.str.KeyProssLED ;	   // 亮的模式
				led.flash_mode = LED_FLASH_MODE_ALWAYS;	   // 常亮模式
				SaveParaToBlock();						   // 对参数做判断				
			}
		}

		//其它，非广播命令，接收信息，需要回复
		else if( (uart.rBuffer[0] == addr)&&(addr != 0)	)			//地址校验
		{

			switch (uart.rBuffer[1])
			{
				case READ_MULTI_BYTE://终端上传数据,0X03号modbus命令：通信时询问是否已经按完成键，设置地址时询问是否已经按完成键接收地址
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//寄存器数量01表示尝试地址是否已经设定的命令
					{
						ACK_Multi_ReadTestAddr();										//地址设定是否成功测试
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//寄存器数量03表示读取是否按OK键的命令
					{        				        
						ACK_Multi_Read();												//应答Modbus 0x03号采集按钮命令						
					}
					break;
				}
				case WRITE_SINGLE_BYTE:	//控制器下传数据,0X06号modbus命令：锁定解锁标签
				{
					if(uart.rIndex == 0X08)						//所有06号命令都是8字节长度
					{
						ACK_Single_Write();
						//清除全部(清除显示信息、灯、蜂鸣器)，清除OK信息+扫描枪信息+灯塔信息
						if(uart.rBuffer[3] == 0X00)
						{
							Display_Clear();						  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
							sys_data_reset();						  // 清除任务信息区
							OsCmdSet.CurDispMode = DISPMODE_NONE;	  // 当前任务状态设为非运行状态
						}
						//清除显示部分(清除显示信息、灯、蜂鸣器)，不包括，清除OK信息+扫描枪信息+灯塔信息
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();						  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
							sys_data_reset();						  // 清除任务信息区
							OsCmdSet.CurDispMode = DISPMODE_NONE;	  // 当前任务状态设为非运行状态
						}
						else if(uart.rBuffer[3] == 0x20)		//锁定解锁
						{
							if(uart.rBuffer[5] == 0x00)			//解锁
							{
								OsCmdSet.SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//锁定
							{
								OsCmdSet.SystemLocked = ON;
							}
						}
						else if(uart.rBuffer[3] == 0x30)		//上位机收到采集按钮命令带回来的n个OK命令后，回传给设备，告知设备已经接收到了n个OK信息(只有在上位机收到1个OK信息，但是设备中还有一个任务信息未回传时，才会发送此命令)
						{
							//2014.3.31 发现bug，上位机驱动修改为不停尝试发送最后2个完成的确认命令，确保
							//下述情况下时才可以清除，避免确认信号失败情况下的系统错误
							if(uart.rBuffer[5] == 0x01 && ((FinishTask.num == 1 && UnfinishTask.num == 1) || (FinishTask.num == 2 && UnfinishTask.num == 0)))			//1个OK信息
							{
								FinishTask.num--;
								if(FinishTask.num == 1)		//原先有2个已经完成的任务，则后往前挪，因为已经明确收到前一个被上位机接收的信息
								{
									// 第一个指针是要保存区的地址，第二个指针是数据区的地址
									PutFinishedToStack_Com(&FinishTask.info[0],&FinishTask.info[1]);
									FinishTask.info[0].full = FinishTask.info[1].full;	
								}
							}
						}
					}
					break;
				}
				case WRITE_MULTI_BYTE:														//终端下传数据
				{
					//配置状态收到0X10号命令,则自动退出配置且不保存
//					if(KeyMode.f_Config == ON) 
//					{
//						usCountCfg = 0;
//					}
					//开始处理命令，包括数据暂存接收、各种响应等
					if(uart.rBuffer[7]==DISPMODE_NONE && uart.rBuffer[6] == 0X0A)
					{
						ACK_Multi_Write();  
						Display_Clear();				// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
						sys_data_reset();				// 清除任务信息区
					}
					else if( (uart.rBuffer[5] == 0X00 && uart.rBuffer[6] == 0X0A && uart.rBuffer[7] == DISPMODE_TASK)		   // 无批次号命令
					       ||(uart.rBuffer[5] == 0X01 && uart.rBuffer[6] == uart.rIndex-9 && uart.rBuffer[7] ==DISPMODE_TASK && (uart.rBuffer[6]-uart.rBuffer[11]>=8)) )   // 有批次号命令
					{
						//下发本次显示数据前，上位机接收到的本标签采集到的数据组数，通过此数据来确定发此数据之前已经被上位机采集点的OK键的次数，由此控制task_finished_num
						OsCmdSet.LastSoftGetOkCount = (uart.rBuffer[9]>>4)&0X0F; 
						//此处纠正当网关已经将命令下传到本设备，并且本设备已经将数据回传，但是回传过程中上位机与网关之间的通信出问题时
						//不重新接收新数据（通过本标签本次作业剩余未操作个数与当前命令模式的与来判定,另外如果LastSoftGetOkCount==0的情况，则允许接收）
						//2014.3.31发现bug，如果是0个LastSoftGetOkCount，会重复接收
						//1对多任务时，会出现显示、拍灭、进度条、显示相同的第一个、拍灭、进度条......（设备通信故障时）
						//解决途径，暂时可通过上位驱动程序发现本标签本次任务完成后，发送clear命令，本程序记下clear状态与显示状态，
						//如果clear状态时，LastSoftGetOkCount==0时，判断有没有任务接收下来，如果有任务接收下来，则发送ACK_Multi_Write_RepeatError
						//否则，处理
						//任务驱动型信息
            if( (OsCmdSet.CurDispMode == DISPMODE_TASK)&&(UnfinishTask.repeat == TRUE) )
						{
							ACK_Multi_Write_RepeatError();		//回重复收到相同命令的响应
							break;                                                                
						}
						ACK_Multi_Write();
						//已经修改为（即当前若有未处理完的任务，来了新任务，则接收显示新的，原先不接收）
						Commu_Task_Process(); 
						UnfinishTask.repeat	 = TRUE;		   // 置已收到任务标志，防止重复发送
					}
					OsCmdSet.CurDispMode = uart.rBuffer[7];	  // 保存当前任务状态
					break;
				}
				default:
				{
					break;
				}
			}
		}

				//其它，组播命令，接收信息，不回复
		else if( (uart.rBuffer[0] == MULCAST_ADDR)	)			//组播地址校验
		{
		   
		   if( (uart.rBuffer[1] == WRITE_MULTI_BYTE)&&(uart.rBuffer[5] == 0X02)&& (uart.rBuffer[6] == uart.rIndex-9) )   // 组播命令
		   {
		      if(uart.rBuffer[7] == 0X00)
			  {	// 组播清除命令
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
						 Display_Clear();				// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
						 sys_data_reset();				// 清除任务信息区
						 OsCmdSet.CurDispMode = DISPMODE_NONE;	  // 当前任务状态设为非运行状态					     
						 break;
					  }
				   }
			  }
			  else if(uart.rBuffer[7] == 0X03)
			  {	// 组播显示命令
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
			     		 Display_Info( &uart.rBuffer[10 + uart.rBuffer[9]]);			// 显示信息
						 OsCmdSet.CurDispMode = DISPMODE_TASK;
						 break;
					  }
				   }
			  }
		   }
		}

//		p_led_rxd = 1;
		LED_RXT_OFF;
	}
//	uart.rIndex = 0;
//	uart.rFlag  = OFF;
}

/*******************************************************************************
	 					处理MODBUS总站下发的命令
每组命令共14个字节；
BYTE_ADD,BYTE_HEAD,BYTE1,---> BYTE10 , CRC_H , CRC_L

BYTE_ADD 	: 终端地址
BYTE_HEAD	:  下传命令码
BYTE1		:  下传指令号
BYTE2		:  LED指示灯显示方式
BYTE3		:  本次下发的"电子标签"个数
BYTE4		:  主机中的此地址剩余"电子标签"个数
BYTE5-BYTE7	:   第一个"电子标签"内容
BYTE8-BYTE10:   第二个"电子标签"内容	
BYTE11		: CRC_H
BYTE12		: CRC_L

下传命令号:
00-	  清除设备的所有显示信息即标签将不工作;
01-	  电子标签与商品条码进行绑定模式；此时3~10位数据无效，数据侦长度2字节
02-    指示灯显示方式单独控制，此时3~10位数据无效，数据侦长度2字节
03-	  下发显示数据以及指示灯的信息，即拾取、上货、盘点作业；
         数据侦长度7字节或者10字节
*******************************************************************************/
void Cmd_Process(void)
{
	// 接收未完成或配置状态则退出
//	if( (uart.rFlag == OFF)||(KeyMode.f_Config == ON) )			// 原先在进入设置密码状态，如果其他标签在通讯则一直不能设置，更改进入密码设置状态，不接收通讯数据
	if( (uart.rFlag == OFF)||(KeyMode.f_Config == ON)||(Key.f_LongFunc == TRUE) )
	{ return; }

	//数据包长度不够,不做Modbus应答		 
	if( (uart.rIndex >= ONE_CMD_MIN_LENGTH) )
    //先看地址、CRC校验、数据长度等判断
	//再根据第二个字节的范围判断是采用新协议解析还是旧协议解析
	//地址与CRC数据校验,下传指令存储,MODBUS应答,CRC_Check函数中包含地址设定命令
	{ 
//		if( Key.f_LongFunc == TRUE )
//		{ Display_Clear(); }			 // 原先在进入设置密码状态，如果其他标签在通讯则一直不能设置
		DiscodeProtocalV1(); 
	}  		
	
	uart.rFlag   = OFF;				 // 清接收完成标志
	uart.rIndex  = 0;				 // 清接收个数
}
/*******************************************************************************
								END					
*******************************************************************************/

