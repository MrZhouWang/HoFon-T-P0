

#define AP_PROCESS_GLOBALS
#include "extern_symbol.h"

/*******************************************************************************
	 					��ʾ����
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Info(INT8U XDATA * buf)
{
	UINT8  usReTaskNum,len0;			//�����·���"���ӱ�ǩ��ʾ����"������

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	
	// ������״̬��ʱ�䡢ģʽ 	  uart.rBuffer[8] 
	i = * (buf);
	//1����˸�����
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (i >> 5)&0X07;	  // �����ڵ��鱸��������
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type  = led.light_type_BK; }			  // �ǽ�����ģʽ�Ÿ��µ�����ʾֵ
    
	//2����˸ʱ����
	switch((i >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3����˸ģʽ
    led.flash_mode   = (i)&0X07;
	//4����˸��������λ��ֹͣ��˸
	led.time_refresh = 0;			// ����˸ʱ�䵽
	led.flash_status = 0;			// ����

    // �ϴβɼ�����ť��������(��4λ)+������������(��4λ) uart.rBuffer[9] 
	usReTaskNum = (* (buf+1))&0X0F;		//�����·���"���ӱ�ǩ��ʾ����"������ 
	// ʣ��δ�·��������� uart.rBuffer[10]			
	UnfinishTask.remain = * (buf+2);		 //������ҵ�л�δ�·��ĸ���

	UnfinishTask.num = 0;		   // δ������������0
	FinishTask.num   = 0;		   // ��������������0
	UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      //�����һ������������

	BackTask.num   = 0;			   // ���Ա������ı����ܸ���
	KeyMode.f_BackDisp = OFF;	   // ����ģʽ��־���

	//���ݱ����·���ʾ�������������Э�����

	//��1�������ݣ�����ǰʣ�����������			

	curIndex = 4;					// ����λ��
	len0     = * (buf+3); //uart.rBuffer[11];	// ��һ�������кų���                 

	// ��Ӧ������
	pt = &UnfinishTask.info[UnfinishTask.num];		
	// ��������
	(*pt).srcode_len  = len0;
	for(i = 0; i < len0; i++)
	{
		if(i >= MAX_SRCODE_NUM) break;
		(*pt).srcode[i] = * (buf + curIndex + i);//uart.rBuffer[curIndex+i];
	}
	//��λ��+����
	curIndex += len0;			//uart.rBuffer[11]Ϊ��һ�����кų��� 

	(*pt).sku	 = * (buf + curIndex);//uart.rBuffer[curIndex];	  //��λ��
	(*pt).count  = (INT16U)((*(buf+curIndex+1))*256) + (INT16U)(*(buf+curIndex+2));//(INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //����

	if(UnfinishTask.num == 0)
	{ OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ; } 	  // ���浱ǰ����

	UnfinishTask.num++;

	{ SetDispPage(PAGE_RUN); }
}

/*******************************************************************************
	 					��ҵ����
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Task_Process(void)
{
	UINT8  usReTaskNum,len0,len1;			//�����·���"���ӱ�ǩ��ʾ����"������

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	//��ʾ���ݻ�ȡ����λ�ţ�������
	//�����·���"���ӱ�ǩ��ʾ����"������     
	usReTaskNum = (uart.rBuffer[9])&0X0F;		//�����·���"���ӱ�ǩ��ʾ����"������ 			
 	//������ҵ�л�δ�·��ĸ���
//	task_unupdate_num = uart.rBuffer[10];
	UnfinishTask.remain = uart.rBuffer[10];		 //������ҵ�л�δ�·��ĸ���
	//�ȳ�ʼ������
	//��2�����ʾ������������ԭ�������ݶ����������UN����ED��

	// ��λ���ϴβɼ���OK��ť����������
	switch(OsCmdSet.LastSoftGetOkCount)
	{
		case 0:     //��ҵ������ʱ���һ�η�
		{
			//�������ݴ�����������һ����ҵ���������,��ǰ�������������(�豸2���ظ����մ�����ʱ),������������
			UnfinishTask.num = 0;		   // δ������������0
			FinishTask.num   = 0;		   // ��������������0
			UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      //�����һ������������

			BackTask.num   = 0;			   // ���Ա������ı����ܸ���
			KeyMode.f_BackDisp = OFF;	   // ����ģʽ��־���
			break;
		}
		case 1:    //��ҵ��������λ������������֮ǰ�ɼ���1����ɵ���ʾ
		{
			FinishTask.num--;
			if(FinishTask.num == 1)
			{
				//����ǰŲ
				// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
				PutFinishedToStack_Com(&FinishTask.info[0],&FinishTask.info[1]);
				FinishTask.info[0].full = FinishTask.info[1].full;				
			}
			else
			{
				FinishTask.num = 0;
			}
			break;
		}
//		case 2:    //��ҵ��������λ������������֮ǰ�ɼ���2����ɵ���ʾ
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

	//���ݱ����·���ʾ�������������Э�����
	switch(usReTaskNum)
	{
		case 1:		//��1�������ݣ�����ǰʣ�����������
		{			
			// ʣ��0����2����������0�飬ʣ��1���򱣴���1��
			if(UnfinishTask.num >= MAX_TASK_NUM)
			{ UnfinishTask.num = 0; }

			pt = &UnfinishTask.info[UnfinishTask.num];
			if(uart.rBuffer[5] == 0X00)			//û�����кŵ�Э��
			{
				curIndex = 11;					// ����λ��
				len0     = 0;                   // ���γ���Ϊ0
			}
			else if(uart.rBuffer[5] == 0X01)	//�����кŵ�Э��
			{	
				curIndex = 12;					// ����λ��
		 		len0     = uart.rBuffer[11];	// ��һ�������кų���                 
			}
			
			// ��������
			(*pt).srcode_len  = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				(*pt).srcode[i] = uart.rBuffer[curIndex+i];
			}
			//��λ��+����
			curIndex += len0;			//uart.rBuffer[11]Ϊ��һ�����кų��� 

			(*pt).sku	 = uart.rBuffer[curIndex];	  //��λ��
			(*pt).count  = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //����

			if(UnfinishTask.num == 0)
			{ OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ; } 	  // ���浱ǰ����

			UnfinishTask.num++;
			break;
		}
		case 2:		//��2�������ݣ�ԭ�ȵĶ������ȫ������
		{
			if(uart.rBuffer[5] == 0X00)			//û�����кŵ�Э��
			{
				curIndex = 11;					// ����λ��
				len0      = 0;                   // ��1�����γ���Ϊ0
				len1      = 0;                   // ��2�����γ���Ϊ0
			}
			else if(uart.rBuffer[5] == 0X01)	//�����кŵ�Э��
			{	
				curIndex = 12;					// ����λ��
		 		len0  	 = uart.rBuffer[11];	// ��1�������кų���  
				len1     = uart.rBuffer[6] - len0 - 11; // ��2�������кų��� 				              
			}			
			// �����1��������Ϣ
			UnfinishTask.info[0].srcode_len = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[0].srcode[i] = uart.rBuffer[curIndex+i];
			}
			//��λ��1+����1
			curIndex += len0;			//uart.rBuffer[11]Ϊ��һ�����кų���
			UnfinishTask.info[0].sku   = uart.rBuffer[curIndex];													 //��λ��
			UnfinishTask.info[0].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //����
                    
			//���к�2
			curIndex += 3;
			UnfinishTask.info[1].srcode_len = len1;
			for(i = 0; i < len1; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[1].srcode[i] = uart.rBuffer[curIndex+i];
			}
			//��λ��2+����2
			curIndex += len1;			//uart.rBuffer[11]Ϊ��һ�����кų���
			UnfinishTask.info[1].sku   = uart.rBuffer[curIndex];													 //��λ��
			UnfinishTask.info[1].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //����
			OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;			   // ���浱ǰ����
			UnfinishTask.num = 2;
			break;
		}
		default: 		break;
	}

	//1����˸�����
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (uart.rBuffer[8] >> 5)&0X07;	  // �����ڵ��鱸��������
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type    = led.light_type_BK; }			  // �ǽ�����ģʽ�Ÿ��µ�����ʾֵ
    
	//2����˸ʱ����
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3����˸ģʽ
    led.flash_mode   = (uart.rBuffer[8])&0X07;
	//4����˸��������λ��ֹͣ��˸
	led.time_refresh = 0;			// ����˸ʱ�䵽
	led.flash_status = 0;			// ����

	//5��������ģʽ�´���
	// ���ȴ�����
	if(Disp.f_Loading == LOAD_MODE_LONG)
	{	
		INT16U	 LoadCount,MinWaitTime;

		//�ȴ�ʱ�� > ���µȴ�ʱ��,ֱ��������ʾ����
		LoadCount   = DIGITAL_LED_LOAD_TIME_LONG - Disp.TatolTimeRefresh16;
//		MinWaitTime = (para.str.WtTime+1)*100;
		MinWaitTime = (para.str.WtTime+1);
		//�ȴ�ʱ�� > ���µȴ�ʱ��,ֱ��������ʾ����
		if(LoadCount >= MinWaitTime)
		{
			Disp.f_Loading    = LOAD_MODE_OFF;			// �����ȴ�������
			led.light_type    = led.light_type_BK;		// �ָ�ԭ��ʹ�õĵ���
		}	
		//�ȴ�ʱ�仹û����С�ȴ�ʱ�䣬������ȴ�����Сʱ����������ʾ
		else
		{
			Disp.TatolTimeRefresh16  = MinWaitTime - LoadCount;
		}					
	}
	//�ǽ���ģʽ��������
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ SetDispPage(PAGE_RUN); }
}

/*******************************************************************************
	 					�Զ��ֽڶ�������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
//Ӧ��Modbus 0x03�Ųɼ���ť����
void ACK_Multi_Read(void)
{
	UINT8   addr;
	UINT16  usCRC;
    
	UnfinishTask.repeat = FALSE;					// ���ظ����յ������־    
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
    uart.wLen       = 2;    
	if(OsCmdSet.CurDispMode == DISPMODE_NONE)		//??0ģʽ�£�Ӧ���޲ɼ�����
	{  // ��ǰ����״̬Ϊ������״̬
		uart.wBuffer[2] = 0x00;                     //n byte	
	        
		usCRC = CRC16(uart.wBuffer, 3);
		uart.wBuffer[3] = (UINT8)usCRC;
		uart.wBuffer[4] = (UINT8)(usCRC>>8);
		uart.wLen       = 5; 	        
	}
	else if(OsCmdSet.CurDispMode == DISPMODE_TASK)
	{	// ��ǰ����״̬��Ϊ����״̬
		//������ҵ�����һ�����ݣ�����Ҫ���ж�ʰȡ��Ϻ����ϴ�����,ԭ�����£�
		//���һ�����ݣ������2������ʾ�������һ����ʾ��ϲ����Ѿ�����OK������ʱ��ǩ��ʾ�ڶ�������ʱ��λ��ѯ�ʱ�ǩ�Ƿ��Ѿ�����OK��
		//��ʱʵ���Ѿ�����һ��OK�����豸�ش���һ������λ�������ѯ���Ƿ���OK������Ϊ�豸������һ�����񣩣���ʱ��ǩ�ûش𣿣�
		//������������У���λ��ѯ�ʵ���ǩ�Ѿ�����OK���󣬻Ჹ����һ���������һ��������ǩ֮ǰ���ǲ����вɼ�OK�������ģ�
		//��˿��Կ��ƣ����ǣ����2���޷���˿��ƣ���˱���һ���ϴ�
		if(FinishTask.num == 0)				 
		{  // û�а���
			uart.wBuffer[2] = 0x02;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = 0X00;                    
	
			usCRC = CRC16(uart.wBuffer, 5);
			uart.wBuffer[5] = (UINT8)usCRC;
			uart.wBuffer[6] = (UINT8)(usCRC>>8);
			uart.wLen       = 7;                            
		}
		else  if(FinishTask.num == 1)
		{	// 1������
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
		{	// 2������
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
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	SendSerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
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
	 					�Զ��ֽ�д����������Ӧ
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
	//��ʾ���,���ݸ�λ
	Display_Clear();			   // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
	sys_data_reset();
	//��ַ����
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

	if (retCRC == usCRC) 						//crcУ��ɹ� 
	{
//	   	p_led_rxd = 0;
		LED_RXT_ON;							   // ��������LED��
		//0�Ź㲥�����ַ������ظ�
		if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
		{
			//00 06 ?? 00 ?? ?? CRCH CRCL
			if(uart.rBuffer[3] == 0X00)						//��ַ����
			{ ResetAddr(); }
			//00 06 ?? 01 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X01)				//���յ�ַ������ʾ������ȴ�����ȷ��
			{
				if(OsCmdSet.beSettingAddr != 0X02)			// δ�ظ���ַ�趨
				{
					OsCmdSet.beSettingAddr = 1;				// �����ַ�趨
//					toSettedAddr = uart.rBuffer[5];;
					para_bk.str.address = uart.rBuffer[5];	// ���趨��ַ���뱸����
					//���ø�����Ϣ����ʾ��ַ�趨��ʾ
					led.light_type = LED_GREEN;				// ����ǰ����һ��ƣ����պ����ڶ����
					led.flash_mode = LED_FLASH_MODE_ALWAYS;	// ����ģʽ

					SetDispPage(PAGE_CFGADDR);				// ������ʾҳ��Ϊ��ַ��ʾ
				}
			}
			//0X10��	�㲥��ʾ
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//�㲥��ʾ����,���## ## == 0X00 0X00������ʾ��ַ
			{
				Disp.Value  = ((UINT16)(uart.rBuffer[4])) *256 + (UINT16)(uart.rBuffer[5]);

				for (i = 0; i < MAX_CFGPARA; i++)
				{ 			
					para_bk.Byte[i] = para.Byte[i]; 			   // �����뱸����
				}
			    //���ݲ�����ʾ����
				if(Disp.Value == 0X00)
				{ SetDispPage(PAGE_CFGADDR);  }			   //��ʾ��ַ
				else if(Disp.Value == 10001)
				{ SetDispPage(PAGE_CFGFUNC1); }			   //��ʾF1 ����
				else if(Disp.Value == 10002)
				{ SetDispPage(PAGE_CFGFUNC2); }			   //��ʾF2 ����
				else if(Disp.Value == 10003)
				{ SetDispPage(PAGE_CFGFUNC3); }			   //��ʾF3 ����
				else if(Disp.Value == 10004)
				{ SetDispPage(PAGE_CFGFUNC4); }			   //��ʾF4 ����
				else if(Disp.Value == 10005)
				{ SetDispPage(PAGE_CFGFUNC5); }			   //��ʾF5 ����
				else
				{ SetDispPage(PAGE_NULL); }				   // ������ʾ

				led.light_type = LED_GREEN;					// ָʾ��
				led.flash_mode = LED_FLASH_MODE_ALWAYS;		// ����ģʽ
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//�㲥���
			{
				Display_Clear();						   // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
				sys_data_reset();						   // ���������Ϣ��
			}
			//0X20��	�㲥�洢��
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//�㲥���ò���1(��ǩ��Ӧ���������ɺ�����ģʽ)
			{
				para.str.FinishedLED  = uart.rBuffer[5];   // д�����
				SetDispPage(PAGE_CFGFUNC1);				   // ������ʾҳ��ΪF1��ʾ

				led.light_type = para.str.FinishedLED;		// ��ָʾ�Ƹĳ�F1��������ģʽ
				led.flash_mode = LED_FLASH_MODE_ALWAYS;	   // ����ģʽ				
				SaveParaToBlock();						   // �Բ������ж�				
			}
			//00 06 ?? 21 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X21)				//�㲥���ò���2(һ�Զ�����ʱ���������ȴ�ʱ��)
			{
				para.str.WtTime = uart.rBuffer[5];			// д�����
				SetDispPage(PAGE_CFGFUNC2);					// ������ʾҳ��ΪF2��ʾ
				
 				led.light_type = LED_GREEN ;	   			   // ����ģʽ
				led.flash_mode = LED_FLASH_MODE_1V1;	   // ����ģʽ
				led.time_refresh = 0;						// ����˸ʱ�䵽
				led.flash_status = 0;						// ����
				led.flash_period = 100;						// ��˸ʱ���׼
				SaveParaToBlock();							// �Բ������ж�				
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				//�㲥���ò���3(���ܼ��趨)
			{				
				para.str.FnDefine = uart.rBuffer[5];	   // д�����
				SetDispPage(PAGE_CFGFUNC3);				   // ������ʾҳ��ΪF3��ʾ

 				led.light_type = LED_GREEN ;	   			   // ����ģʽ
				led.flash_mode = LED_FLASH_MODE_1V1;	   // ����ģʽ
				led.time_refresh = 0;						// ����˸ʱ�䵽
				led.flash_status = 0;						// ����
				led.flash_period = 100;						// ��˸ʱ���׼
				SaveParaToBlock();							// �Բ������ж�				
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				//�㲥���ò���4(��ʾģʽ�趨)
			{
				para.str.DispMode = uart.rBuffer[5];	   // д�����
				SetDispPage(PAGE_CFGFUNC4);				   // ������ʾҳ��ΪF4��ʾ

 				led.light_type = LED_GREEN ;	   			   // ����ģʽ
				led.flash_mode = LED_FLASH_MODE_1V1;	   // ����ģʽ
				led.time_refresh = 0;						// ����˸ʱ�䵽
				led.flash_status = 0;						// ����	 
				led.flash_period = 100;						// ��˸ʱ���׼
				SaveParaToBlock();							// �Բ������ж�	
				
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X24)				//�㲥���ò���4(��ʾģʽ�趨)
			{
				para.str.KeyProssLED = uart.rBuffer[5];	   // д�����
				SetDispPage(PAGE_CFGFUNC5);				   // ������ʾҳ��ΪF5��ʾ

				led.light_type = para.str.KeyProssLED ;	   // ����ģʽ
				led.flash_mode = LED_FLASH_MODE_ALWAYS;	   // ����ģʽ
				SaveParaToBlock();						   // �Բ������ж�				
			}
		}

		//�������ǹ㲥���������Ϣ����Ҫ�ظ�
		else if( (uart.rBuffer[0] == addr)&&(addr != 0)	)			//��ַУ��
		{

			switch (uart.rBuffer[1])
			{
				case READ_MULTI_BYTE://�ն��ϴ�����,0X03��modbus���ͨ��ʱѯ���Ƿ��Ѿ�����ɼ������õ�ַʱѯ���Ƿ��Ѿ�����ɼ����յ�ַ
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//�Ĵ�������01��ʾ���Ե�ַ�Ƿ��Ѿ��趨������
					{
						ACK_Multi_ReadTestAddr();										//��ַ�趨�Ƿ�ɹ�����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//�Ĵ�������03��ʾ��ȡ�Ƿ�OK��������
					{        				        
						ACK_Multi_Read();												//Ӧ��Modbus 0x03�Ųɼ���ť����						
					}
					break;
				}
				case WRITE_SINGLE_BYTE:	//�������´�����,0X06��modbus�������������ǩ
				{
					if(uart.rIndex == 0X08)						//����06�������8�ֽڳ���
					{
						ACK_Single_Write();
						//���ȫ��(�����ʾ��Ϣ���ơ�������)�����OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						if(uart.rBuffer[3] == 0X00)
						{
							Display_Clear();						  // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
							sys_data_reset();						  // ���������Ϣ��
							OsCmdSet.CurDispMode = DISPMODE_NONE;	  // ��ǰ����״̬��Ϊ������״̬
						}
						//�����ʾ����(�����ʾ��Ϣ���ơ�������)�������������OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();						  // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
							sys_data_reset();						  // ���������Ϣ��
							OsCmdSet.CurDispMode = DISPMODE_NONE;	  // ��ǰ����״̬��Ϊ������״̬
						}
						else if(uart.rBuffer[3] == 0x20)		//��������
						{
							if(uart.rBuffer[5] == 0x00)			//����
							{
								OsCmdSet.SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//����
							{
								OsCmdSet.SystemLocked = ON;
							}
						}
						else if(uart.rBuffer[3] == 0x30)		//��λ���յ��ɼ���ť�����������n��OK����󣬻ش����豸����֪�豸�Ѿ����յ���n��OK��Ϣ(ֻ������λ���յ�1��OK��Ϣ�������豸�л���һ��������Ϣδ�ش�ʱ���Żᷢ�ʹ�����)
						{
							//2014.3.31 ����bug����λ�������޸�Ϊ��ͣ���Է������2����ɵ�ȷ�����ȷ��
							//���������ʱ�ſ������������ȷ���ź�ʧ������µ�ϵͳ����
							if(uart.rBuffer[5] == 0x01 && ((FinishTask.num == 1 && UnfinishTask.num == 1) || (FinishTask.num == 2 && UnfinishTask.num == 0)))			//1��OK��Ϣ
							{
								FinishTask.num--;
								if(FinishTask.num == 1)		//ԭ����2���Ѿ���ɵ����������ǰŲ����Ϊ�Ѿ���ȷ�յ�ǰһ������λ�����յ���Ϣ
								{
									// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
									PutFinishedToStack_Com(&FinishTask.info[0],&FinishTask.info[1]);
									FinishTask.info[0].full = FinishTask.info[1].full;	
								}
							}
						}
					}
					break;
				}
				case WRITE_MULTI_BYTE:														//�ն��´�����
				{
					//����״̬�յ�0X10������,���Զ��˳������Ҳ�����
//					if(KeyMode.f_Config == ON) 
//					{
//						usCountCfg = 0;
//					}
					//��ʼ����������������ݴ���ա�������Ӧ��
					if(uart.rBuffer[7]==DISPMODE_NONE && uart.rBuffer[6] == 0X0A)
					{
						ACK_Multi_Write();  
						Display_Clear();				// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
						sys_data_reset();				// ���������Ϣ��
					}
					else if( (uart.rBuffer[5] == 0X00 && uart.rBuffer[6] == 0X0A && uart.rBuffer[7] == DISPMODE_TASK)		   // �����κ�����
					       ||(uart.rBuffer[5] == 0X01 && uart.rBuffer[6] == uart.rIndex-9 && uart.rBuffer[7] ==DISPMODE_TASK && (uart.rBuffer[6]-uart.rBuffer[11]>=8)) )   // �����κ�����
					{
						//�·�������ʾ����ǰ����λ�����յ��ı���ǩ�ɼ���������������ͨ����������ȷ����������֮ǰ�Ѿ�����λ���ɼ����OK���Ĵ������ɴ˿���task_finished_num
						OsCmdSet.LastSoftGetOkCount = (uart.rBuffer[9]>>4)&0X0F; 
						//�˴������������Ѿ��������´������豸�����ұ��豸�Ѿ������ݻش������ǻش���������λ��������֮���ͨ�ų�����ʱ
						//�����½��������ݣ�ͨ������ǩ������ҵʣ��δ���������뵱ǰ����ģʽ�������ж�,�������LastSoftGetOkCount==0���������������գ�
						//2014.3.31����bug�������0��LastSoftGetOkCount�����ظ�����
						//1�Զ�����ʱ���������ʾ�����𡢽���������ʾ��ͬ�ĵ�һ�������𡢽�����......���豸ͨ�Ź���ʱ��
						//���;������ʱ��ͨ����λ���������ֱ���ǩ����������ɺ󣬷���clear������������clear״̬����ʾ״̬��
						//���clear״̬ʱ��LastSoftGetOkCount==0ʱ���ж���û�������������������������������������ACK_Multi_Write_RepeatError
						//���򣬴���
						//������������Ϣ
            if( (OsCmdSet.CurDispMode == DISPMODE_TASK)&&(UnfinishTask.repeat == TRUE) )
						{
							ACK_Multi_Write_RepeatError();		//���ظ��յ���ͬ�������Ӧ
							break;                                                                
						}
						ACK_Multi_Write();
						//�Ѿ��޸�Ϊ������ǰ����δ����������������������������ʾ�µģ�ԭ�Ȳ����գ�
						Commu_Task_Process(); 
						UnfinishTask.repeat	 = TRUE;		   // �����յ������־����ֹ�ظ�����
					}
					OsCmdSet.CurDispMode = uart.rBuffer[7];	  // ���浱ǰ����״̬
					break;
				}
				default:
				{
					break;
				}
			}
		}

				//�������鲥���������Ϣ�����ظ�
		else if( (uart.rBuffer[0] == MULCAST_ADDR)	)			//�鲥��ַУ��
		{
		   
		   if( (uart.rBuffer[1] == WRITE_MULTI_BYTE)&&(uart.rBuffer[5] == 0X02)&& (uart.rBuffer[6] == uart.rIndex-9) )   // �鲥����
		   {
		      if(uart.rBuffer[7] == 0X00)
			  {	// �鲥�������
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
						 Display_Clear();				// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
						 sys_data_reset();				// ���������Ϣ��
						 OsCmdSet.CurDispMode = DISPMODE_NONE;	  // ��ǰ����״̬��Ϊ������״̬					     
						 break;
					  }
				   }
			  }
			  else if(uart.rBuffer[7] == 0X03)
			  {	// �鲥��ʾ����
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
			     		 Display_Info( &uart.rBuffer[10 + uart.rBuffer[9]]);			// ��ʾ��Ϣ
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
	 					����MODBUS��վ�·�������
ÿ�����14���ֽڣ�
BYTE_ADD,BYTE_HEAD,BYTE1,---> BYTE10 , CRC_H , CRC_L

BYTE_ADD 	: �ն˵�ַ
BYTE_HEAD	:  �´�������
BYTE1		:  �´�ָ���
BYTE2		:  LEDָʾ����ʾ��ʽ
BYTE3		:  �����·���"���ӱ�ǩ"����
BYTE4		:  �����еĴ˵�ַʣ��"���ӱ�ǩ"����
BYTE5-BYTE7	:   ��һ��"���ӱ�ǩ"����
BYTE8-BYTE10:   �ڶ���"���ӱ�ǩ"����	
BYTE11		: CRC_H
BYTE12		: CRC_L

�´������:
00-	  ����豸��������ʾ��Ϣ����ǩ��������;
01-	  ���ӱ�ǩ����Ʒ������а�ģʽ����ʱ3~10λ������Ч�������쳤��2�ֽ�
02-    ָʾ����ʾ��ʽ�������ƣ���ʱ3~10λ������Ч�������쳤��2�ֽ�
03-	  �·���ʾ�����Լ�ָʾ�Ƶ���Ϣ����ʰȡ���ϻ����̵���ҵ��
         �����쳤��7�ֽڻ���10�ֽ�
*******************************************************************************/
void Cmd_Process(void)
{
	// ����δ��ɻ�����״̬���˳�
//	if( (uart.rFlag == OFF)||(KeyMode.f_Config == ON) )			// ԭ���ڽ�����������״̬�����������ǩ��ͨѶ��һֱ�������ã����Ľ�����������״̬��������ͨѶ����
	if( (uart.rFlag == OFF)||(KeyMode.f_Config == ON)||(Key.f_LongFunc == TRUE) )
	{ return; }

	//���ݰ����Ȳ���,����ModbusӦ��		 
	if( (uart.rIndex >= ONE_CMD_MIN_LENGTH) )
    //�ȿ���ַ��CRCУ�顢���ݳ��ȵ��ж�
	//�ٸ��ݵڶ����ֽڵķ�Χ�ж��ǲ�����Э��������Ǿ�Э�����
	//��ַ��CRC����У��,�´�ָ��洢,MODBUSӦ��,CRC_Check�����а�����ַ�趨����
	{ 
//		if( Key.f_LongFunc == TRUE )
//		{ Display_Clear(); }			 // ԭ���ڽ�����������״̬�����������ǩ��ͨѶ��һֱ��������
		DiscodeProtocalV1(); 
	}  		
	
	uart.rFlag   = OFF;				 // �������ɱ�־
	uart.rIndex  = 0;				 // ����ո���
}
/*******************************************************************************
								END					
*******************************************************************************/

