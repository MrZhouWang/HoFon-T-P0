
#define AP_KEY_GLOBALS	
#include "extern_symbol.h" 

/*
// ��������˵�� 
*/
								 
/*******************************************************************************
	 			  ���񱣴湲�ú���
	input:   ptr1Ϊ�������ĵ�ַ��ptr2 Ϊ�������ĵ�ַ     
	output:  none					
*******************************************************************************/
void	PutFinishedToStack_Com(TASK_INFO XDATA *ptr1,TASK_INFO XDATA *ptr2)
{
	UINT8 i ,len;
		  
	len = (*ptr1).srcode_len = (*ptr2).srcode_len;  

	if(len > MAX_SRCODE_NUM) { len = MAX_SRCODE_NUM; }
	// ����������Ϣ
	for(i = 0; i < len; i++)
	{  (*ptr1).srcode[i] = (*ptr2).srcode[i]; }

	(*ptr1).sku   = (*ptr2).sku;					// ��λ��
	(*ptr1).count = (*ptr2).count;					// ����
//	(*ptr1).full  = (*ptr2).full;					// ���������־
}	

/*******************************************************************************
	 			һ�����������ɺ󣬴���������ʷ��ջ���Թ�����ʹ��
	input:   none     
	output:  none					
*******************************************************************************/
void PutFinishedToStack(void)
{
	UINT8 i ;

	//��ջδ����ֱ��ѹ��
	if (BackTask.num < MAX_BACK_NUM)
	{
		// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
		PutFinishedToStack_Com(&BackTask.info[BackTask.num],&UnfinishTask.info[0]);
		BackTask.num++;
	}
	//��ջ���������ƶ���ѹ��
	else
	{
		for (i = 0; i < MAX_BACK_NUM-1; i++)
		{
			// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
			PutFinishedToStack_Com(&BackTask.info[i],&BackTask.info[i+1]);
		}
		// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
		PutFinishedToStack_Com(&BackTask.info[MAX_BACK_NUM-1],&UnfinishTask.info[0]);
		BackTask.num = MAX_BACK_NUM;
	}
}

/*******************************************************************************
 ����ģʽ�£��ı��Ӧ��ֵ������֤�ڷ�Χ��            
*******************************************************************************/
void NormolChgVal(INT8U sts)
{
	INT8U	rev,num;//,val[4];
	INT16U	usValue;
	
	if( KeyMode.f_ChangeValue == OFF ) {return;}

	//����ԭ��û�г�����Fn��������£����ҵ�ǰ��ʾ��ֵ<DISPLAY_MAX_NUM������������ʾʱ
//	usValue = ((UINT16)(task_un_goods_numH[0])) *256 + (UINT16)(task_un_goods_numL[0]);	
	num     = UnfinishTask.num;
	usValue = UnfinishTask.info[0].count;
//	if( (DispMode == 0X00) && (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) && (task_unfinish_num <= MAX_TASK_NUM) )
//	if( (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) && (task_unfinish_num <= MAX_TASK_NUM) )
//	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < DISPLAY_MAX_NUM)&&(DIGILED_COUNT != 0) )
	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < os_disp_set.DispMax_count)&&(os_disp_set.DispLed_count != 0) )
	{

	
//#if	(DIGILED_COUNT != 0)
//		Disp.Focus = (Disp.Focus % DIGILED_COUNT);
		Disp.Focus = (Disp.Focus % os_disp_set.DispLed_count);
//#endif
//		num = Disp.Focus+(COUNT_MAX-DIGILED_COUNT);
		num = Disp.Focus+(COUNT_MAX-os_disp_set.DispLed_count);
		Calc_Value5(0,0,usValue);

	   	rev = Buffer[num];

//		rev = Buffer[Disp.Focus];//val[ucFocus];
		if(sts != 0){ rev++; }
		else		{ rev--; }
		
		if(rev > 9)
		{
			if(sts != 0){ rev = 0; }
			else		{ rev = 9; }
		}

		Buffer[num] = rev;


		usValue = (Buffer[0]*10000 + Buffer[1]*1000 + Buffer[2]*100 + Buffer[3]*10 + Buffer[4]);
		UnfinishTask.info[0].count =  usValue;
		

		Disp.f_OffDisp 		    = OFF;				// ����ʾ��ֵ
		Disp.TatolTimeRefresh8	= TIMER_MODIFY;		// ��ʱ�˳�
		SetDispPage(PAGE_RUN);
	}
}

/*******************************************************************************
 			����ģʽ�£��������������ø���
			�ı��Ӧ�Ľ���λ������֤�ڷ�Χ��            
*******************************************************************************/
void NormolChgFocus(INT8U sts)
{
	INT8U	num;
	INT16U	usValue;	
				
//	usValue = ((UINT16)(task_un_goods_numH[0])) *256 + (UINT16)(task_un_goods_numL[0]);

	//����ԭ��û�г�����Fn��������£����ҵ�ǰ��ʾ��ֵ<DISPLAY_MAX_NUM������������ʾʱ
//	if (DispMode == 0X00 && task_unfinish_num > 0 && usValue <=DISPLAY_MAX_NUM)
//	if( (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) )
	num     = UnfinishTask.num;
	usValue = UnfinishTask.info[0].count;
//	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < DISPLAY_MAX_NUM)&&(DIGILED_COUNT != 0) )
	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < os_disp_set.DispMax_count)&&(os_disp_set.DispLed_count != 0) )
	{
		if(KeyMode.f_ChangeValue == OFF)
//		{ Disp.Focus = DIGILED_COUNT-1; }					 // ������ֵ�������λ		4λ��Ӧ3
		{ Disp.Focus = os_disp_set.DispLed_count-1; }					 // ������ֵ�������λ		4λ��Ӧ3
		else if(sts != 0)
		{ 
//			if( ++Disp.Focus >= DIGILED_COUNT)			 // ����λǰ��
			if( ++Disp.Focus >= os_disp_set.DispLed_count)			 // ����λǰ��
			{ Disp.Focus = CHG_TASK_COUNT_FIRST; }
		}
		else
		{
			if( Disp.Focus-- == CHG_TASK_COUNT_FIRST )
//			{ Disp.Focus = DIGILED_COUNT-1; }		   			 // ����λ����	
			{ Disp.Focus = os_disp_set.DispLed_count-1; }		   			 // ����λ����	
		}	
		
		KeyMode.f_ChangeValue  = ON; 				// ����ֵ�޸ı�־
		Disp.f_OffDisp 		   = ON;				// ����ʾ��
		Disp.TatolTimeRefresh8 = TIMER_MODIFY;		// ��ʱ�˳�
		SetDispPage(PAGE_RUN);
	}
}
/************************************************************************
* ������������,������״̬����������
* ���밴��ֵ
************************************************************************/
void	KeyNormolFun(INT8U keyvalue)
{
	 INT8U	i;
//	 UINT16 usValue = 0;

	// ��������ò�������״̬����"����"��������������ü���ʾ���˳�
//	// �����������������״̬
	if((Key.f_LongFunc == TRUE)&&( keyvalue != KEYFUN_BACK ))
	{  Display_Clear(); return; }

	// ��ȡ�����õĲ���
	// ԭ�Ȱ���Fn����������������,���OK��������ñ�־��ˢ����ʾ
	if( (Key.f_IsFnKeyPressed == ON)&&(keyvalue != KEYFUN_OK) )
	{
		Key.f_IsFnKeyPressed = OFF;  
		SetDispPage(Disp.Page);
		return;
	}

	//==================================================================
	//������״̬ģʽ�£�����������			
	switch( keyvalue )
	{	//--------------------------------------------------------------
		// ���ܼ�  ��������˳�����,�̰���ɲ���ҳ���л�����
		case KEYFUN_FUNC:
		{ 			
			//�����������������������ý��棬�����������F3����ѡ�����������������־
			if(Key.f_LongOnce == TRUE)
			{  
				Key.f_LongOnce = FALSE;		// ִֻ��һ��
				
				//����ҵ��������ɻ������գ�������������״̬.
//				if ((task_finished_num ==0)&&(task_unfinish_num ==0))//����ҵ��������ɻ������գ��ɽ�������״̬.
//				if ( (FinishTask.num == 0)&&(UnfinishTask.num == 0) )//����ҵ��������ɻ������գ��ɽ�������״̬.  	//��FinishTask���������������F1���ܣ���Finish���ᱻ��0������������ģʽ
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		// δ���Ϊ0��ʣ��Ϊ0������Ϊ����Ϊ��
				{
					Key.f_LongFunc = TRUE;						// ��������״̬��������ʾ״̬
					Key.CfgPwdCnt  = 0;							// ����˶Դ���
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/3;		// ��ʱ�˳�
					SetDispPage( PAGE_CFGPWD );				   	// �����ַ����������ʾҳ��
//					// ��������״̬
//					KeyMode.f_Config = ON;						   // ��������״̬
//					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // ��ʱ�˳�					
//					Disp.Focus		 = 2;						   // ����λ����,��ַ�����λ
//					Disp.MenuPage	 = 0;						   // �˵�ҳ������ �������ַ���ý���
// 
//					for (i = 0; i < MAX_CFGPARA; i++)
//					{ 			
//						para_bk.Byte[i] = para.Byte[i]; 			   // �����뱸����
//					}                  
//					SetDispPage( PAGE_CFGADDR );				   // �����ַ������ʾҳ��
				}
//				else if(task_unfinish_num > 0) //����ʾ������Ϣ��ʱ�򣬳���Fn�������㣨��֧����𣩻������������־
				else if(UnfinishTask.num > 0) //����ʾ������Ϣ��ʱ�򣬳���Fn�������㣨��֧����𣩻������������־
				{	// ���ݲ���F3 ѡ�����������������־�������ó���"-���������
					switch(para.str.FnDefine)
					{
//						case 0:
//						if(DispMode != 0X01)
//						{
//							task_un_goods_numH[0] =  0;
//							task_un_goods_numL[0] =  0;
////							digital_led_refresh = ON;    //ʹ��ˢ������
////							SetLcdLay( LCD.Type );				 // �ص�����ҳ��
//							LCD.f_LcdEnable  = WAIT_REFRESH;			// �ȴ�ˢ���ܱ�־
//						}
//						break;
						case 1:
						Key.f_IsFnKeyPressed = ON;					// ��Fn��������־
						SetDispPage(PAGE_RUN);
						break;
					}
				}                    				
			}
			//�̰�������������������������洢�������Խ��棬��������ָ���ǰ��ʾ����
			//������״̬��,�̰�Fn,��ʼ�ط����ݣ�ÿ��һ��FN����ǰ�ط�һ������ʱSYS����������ָʾ����ɫ���䣬������ʾ����OK������ȫ���ط���Ϻ�����ָ���ԭ��״̬
			else
			{					
				//��������У��̰�Fn�����ָ��·�ʱ�ĳ�ʼ����
//				if(DispMode != 0X01 && task_unfinish_num > 0)
//				if(task_unfinish_num > 0)
				if(UnfinishTask.num > 0)
				{	// �ɿ��ٻָ�����ֵ
//					task_un_goods_numH[0] =  task_curOri_goods_numH;
//					task_un_goods_numL[0] =  task_curOri_goods_numL;

					UnfinishTask.info[0].count = OsCmdSet.TaskCurCount;	   // �ָ���ǰ��ʾ����
					Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); //������������־	
					SetDispPage(PAGE_RUN);
				}
				//ֻ��ȫ�����������ɺ���ܽ������ģʽ   // δ��������ʣ�������Ϊ0
//				else if(task_unfinish_num == 0 && task_unupdate_num == 0) 
				else if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )
				{						
//					if(DispContentStackCount > 0)
					if(BackTask.num > 0)
					{
						//�������ģʽ
						if(KeyMode.f_BackDisp == OFF)
						{	// �ǻ���ģʽ���û���ģʽ��־�����������ҳ��
							KeyMode.f_BackDisp = ON;
							BackTask.index = BackTask.num - 1;		// �������һ��
//							CurBackDispIndex = DispContentStackCount -1;
							Disp.f_Rolling = ROLL_MODE_OFF; 		// �������־
							SetDispPage( PAGE_BACKDISPLAY );		// �������ҳ��
						}
						else
						{
//							if(CurBackDispIndex == 0)
							if(BackTask.index == 0)
							{	// ����ģʽ�һ��Ե���һ�����˳�����ҳ��
								KeyMode.f_BackDisp = OFF;
//								CurBackDispIndex = 0;
								Display_Clear();		   // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
							}
							else
							{
//								CurBackDispIndex--;
								BackTask.index--;
//								digital_led_refresh = ON;	  //ʹ��ˢ�������
								Disp.f_Rolling = ROLL_MODE_OFF; 				// �������־
								SetDispPage( PAGE_BACKDISPLAY );				// �������ҳ��
							}					
						}
					}
				}									 
			}
		}break;					
		//--------------------------------------------------------------
		// ���¼�		
		case KEYFUN_SUB :
		{						
			if(Key.f_LongOnce == TRUE)
			{  //������-����������ֵ��0���ܽ�������˵��������ִ����
				Key.f_LongOnce = FALSE;		// ִֻ��һ��

//				if( (task_unfinish_num > 0)&&(DispMode != 0X01) ) //����ʾ������Ϣ��ʱ�򣬳���-1�������㣨��֧�����
//				if( (task_unfinish_num > 0) ) //����ʾ������Ϣ��ʱ�򣬳���-1�������㣨��֧�����
				if( (UnfinishTask.num > 0) ) //����ʾ������Ϣ��ʱ�򣬳���-1�������㣨��֧�����
				{
					UnfinishTask.info[0].count = 0;		 // ��0
					Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); //������������־
					SetDispPage(PAGE_RUN);				 // ���������������ʾҳ��
				}
			}
			else	if(KeyMode.f_BackDisp == ON)
			{// ���Խ���
			}
			else
			{
				NormolChgVal(0);		// �ж�������������������������Ϊ����ִ���У������������������ʾֵ
			}			 	
		}break;		
		//--------------------------------------------------------------
		// ���ϼ�
//		case KEYFUN_ADD   :
//		{								
//			{
//				NormolChgVal(1);		// �ж�������������������
//			}
//		}break;							
		//--------------------------------------------------------------
		// ��ǰ�� 
//		case KEYFUN_FORWARD :
//		{								
//			if(ucBackDispMode == ON)
//			{  // ���Խ���
//				if((CurBackDispIndex >= DispContentStackCount)||(CurBackDispIndex >= MAX_BACK_NUM))
//				{	// ����ģʽ�һ��Ե���һ�����˳�����ҳ��
////					ucBackDispMode = OFF;
////					CurBackDispIndex = 0;
//					Display_Clear();		  		// ����������Ա�־������
////					LED_ALL_OFF();			  //�ص�
////					SetLcdLay( PAGE_RUN );				 // �������ҳ��
//				}
//				else
//				{
//					CurBackDispIndex++;
////					CurBackDispPage = CurBackDispIndex/5 + 1;
////					digital_led_refresh = ON;	  //ʹ��ˢ�������
////					SetLcdLay( PAGE_BACKDISPLAY );				 // �������ҳ��
////					LCD.f_LcdEnable  = WAIT_REFRESH;			// �ȴ�ˢ���ܱ�־
//					SetDispPage(Disp.Page);
//				}
//			}
//			else
//			{	// �ǻ��Խ��棬�����������ü����������
//				NormolChgFoc(1);						// ����λǰ��
//			}	
//		}break;
		
		//--------------------------------------------------------------
		// ���� 	
	    case KEYFUN_BACK:
	    {			
			if(KeyMode.f_BackDisp == ON)
			{// ���Խ���
//				if(CurBackDispIndex == 1)
//				{	// ����ģʽ�һ��Ե���һ�����˳�����ҳ��
////					ucBackDispMode = OFF;
////					CurBackDispIndex = 0;
//					Display_Clear();		  // ����������Ա�־������
////					LED_ALL_OFF();			  //�ص�
////					SetLcdLay( PAGE_RUN );				 // �������ҳ��
//				}
//				else
//				{
//					CurBackDispIndex--;
////					CurBackDispPage = CurBackDispIndex/5 + 1;
////					digital_led_refresh = ON;	  //ʹ��ˢ�������
////					SetLcdLay( PAGE_BACKDISPLAY );				 // �������ҳ��
////					LCD.f_LcdEnable  = WAIT_REFRESH;			// �ȴ�ˢ���ܱ�־
//					SetDispPage(Disp.Page);
//				}
			}
			else if(Key.f_LongFunc == TRUE)
			{
 				if( Key.CfgPwdCnt++ >= 3 )
				{
					Key.f_LongFunc =  FALSE;
					// ��������״̬
					KeyMode.f_Config = ON;						   // ��������״̬
					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // ��ʱ�˳�					
					Disp.Focus		 = 2;						   // ����λ����,��ַ�����λ
					Disp.MenuPage	 = 0;						   // �˵�ҳ������ �������ַ���ý���
 
					for (i = 0; i < MAX_CFGPARA; i++)
					{ 			
						para_bk.Byte[i] = para.Byte[i]; 			   // �����뱸����
					}                  
					SetDispPage( PAGE_CFGADDR );				   // �����ַ������ʾҳ��
				}
				else 
				{ 
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/6;		// ��ʱ�˳�
					SetDispPage( PAGE_CFGPWD ); 
				}				   // �����ַ����������ʾҳ��
			}
			else
			{	// �ǻ��Խ��棬��������δ����������0�������������������ֵ
				NormolChgFocus(0);						// ����λ���ƣ�����Ϊ����ִ���У������������������ʾֵ
			}	
		}break;	
		
		//--------------------------------------------------------------
		// ȷ�ϼ� 		
	    case KEYFUN_OK:
	    {		    			
			//�޸Ľ���λ��ֵ״̬�رգ�������˸
//			usCountModify = 0;
			KeyMode.f_ChangeValue = OFF;		// ��������޸ı�־

			//ֻ����δ������񲻿գ����ҷ���ʾ������ʱ������ӦOK����
//			if((task_unfinish_num > 0)&&(digital_led_loading == OFF))
			if( (UnfinishTask.num > 0)&&(Disp.f_Loading == LOAD_MODE_OFF) )
			{   //ˢ������
				//���������
				if(FinishTask.num < MAX_TASK_NUM)	//���������2
				{
					//�����ݴ�������ɵ�������
					// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
					PutFinishedToStack_Com(&FinishTask.info[FinishTask.num],&UnfinishTask.info[0]);					
					// �ж��Ƿ��������־
					if(Key.f_IsFnKeyPressed == ON)	
						{ FinishTask.info[FinishTask.num].full = 1; }//{ taks_finished_Full[0] = 1; }						
					else{ FinishTask.info[FinishTask.num].full = 0; }//{ taks_finished_Full[0] = 0; }
					FinishTask.num++;					// ���������+1

					//��ɵ���������ʷ�洢�Թ�����ʹ��
					PutFinishedToStack();
					//ԭ����2��������У���δ��ɿ���������
//					if(task_unfinish_num == 2)
					if(UnfinishTask.num == 2)
					{
						//��δ��ɿ������=2ʱ������ǰ�� ������ǰŲ                                           					
						// ��һ��ָ����Ҫ�������ĵ�ַ���ڶ���ָ�����������ĵ�ַ
						PutFinishedToStack_Com(&UnfinishTask.info[0],&UnfinishTask.info[1]);
						OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;		// ���浱ǰ����
					}
//					task_unfinish_num--;
					UnfinishTask.num--;
				}

				//�ñ�־�£�����OK���������ȫ�����
				if(Key.f_IsFnKeyPressed == ON)
				{
//					task_unupdate_num = 0;			   // ʣ��������0
//					task_unfinish_num = 0;
					UnfinishTask.num     = 0;		   // δ���������0
					UnfinishTask.remain  = 0;		   // ʣ��������0
					Key.f_IsFnKeyPressed = OFF; 	   // ���Fn������־
				}
				//������ҵһȫ��������񣬵����������� �����ݲ���F1��������ɵ���ɫ
//				if(task_unupdate_num == 0 && task_unfinish_num ==0)	 
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		// δ���Ϊ0��ʣ��Ϊ0������Ϊ����Ϊ��	 	
				{					
					Display_Clear();							  // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
					led.light_type = para.str.FinishedLED;		  // ������ɸ��ݲ���F1����Ӧ�ĵ�ɫ
					led.flash_mode = LED_FLASH_MODE_ALWAYS;		  // ����ģʽ
				}
				//������ҵ���д�ʰȡ���񣬽���������ʾ
				else											
				{	//���񻺳����ջ�����������ʣ������,��ʾ-------					
//					if(task_unfinish_num > 0)
					if(UnfinishTask.num > 0)
					{	
						Disp.f_Loading    	    = LOAD_MODE_SHORT;	  // �ö�ʱ�������־
//						Disp.TatolTimeRefresh32 = (para.str.WtTime+1)*100;	 // LOADING��ʱ��
						Disp.TatolTimeRefresh16 = (para.str.WtTime+1); // ��ʱ��
						
					}
					else //if(task_unupdate_num > 0)				//������ҵ�������񣬵�������û�·������豸�У���ʾ------
					{
						Disp.f_Loading    	    = LOAD_MODE_LONG;			 // �ó�ʱ�������־
//						Disp.TatolTimeRefresh32 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING��ʱ��
						Disp.TatolTimeRefresh16 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING��ʱ��						
					}

					os_time.Base_100ms= TIMER_BASE_100MS;			// ��100ms��׼ֵ���趨
					Disp.TimeRefresh  = DIGITAL_LED_LOAD_CNT_PER;	// ������ʱ�丳ֵ;
					Disp.Shift        = DIGILED_FIRST+1;			// ����"-"
					//�ȱ��浱ǰ�������������������ʱû�е���
					led.light_type_BK = led.light_type;				// ���ݵ���ɫ
					led.light_type    = LED_NONE;					// �ص�
					SetDispPage( PAGE_WAIT );						// ���ý�������ʾҳ��
				}
			}
			else if(KeyMode.f_BackDisp == ON) //����״̬�£���OK��ȡ������
			{
				Display_Clear(); 					 // �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־				
			}       	
		}break;			 							
		default:   	break;		 							
	}					
}

/************************************************************************
* �˳�����ģʽ
************************************************************************/
void	Exit_Cfg_Mode(void)	
{
	INT8U i;

	Display_Clear(); 					 		// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
	if (Disp.TatolTimeRefresh8 == 0) return;    // 3s��ʱ�˳�������

	// ������������д��������
	for (i = 0; i < MAX_CFGPARA; i++)
	{ para.Byte[i] = para_bk.Byte[i]; }			// ������������д��������

	SaveParaToBlock();					 		// �жϲ����Ƿ��ڷ�Χ��(�粻����ָ�)���������	
}	


/*******************************************************************************
	            ����ҳ�棬�ı��Ӧ����λ����ֵ ������֤�ڷ�Χ��
input:  sts = 0��ֵ��1����0���1
Disp.MenuPage ��Ӧ�ĸ�������Disp.Focus��Ӧ����ʾ����λ
�� ��Ӧ��������ʾҳ��				            
*******************************************************************************/
INT8U CODE	CfgPageDB[MAX_CFGPARA]={ PAGE_CFGADDR,PAGE_CFGFUNC1,PAGE_CFGFUNC2,PAGE_CFGFUNC3,PAGE_CFGFUNC4,PAGE_CFGFUNC5 };

void CfgChgParaVal(INT8U sts)
{

	INT8U	rev,max;
	INT8VU   *ptr;
	INT16U	 addr;
	switch(Disp.MenuPage)
	{
		 case 0:   max = 9;	   Calc_Value3(0,0,para_bk.str.address);
		 		   if(Disp.Focus > 2){ Disp.Focus = 2; }
				   else if(Disp.Focus == 0){ max = 2; }	
				   ptr = &Buffer[Disp.Focus]; 		   
		 		   break;
		 case 1:   ptr = &para_bk.str.FinishedLED; 	max = MAX_FINISHEDLED;	break;
		 case 2:   ptr = &para_bk.str.WtTime; 		max = MAX_WTTIME;		break;
		 case 3:   ptr = &para_bk.str.FnDefine; 	max = MAX_FNDEFINE;		break;
		 case 4:   ptr = &para_bk.str.DispMode;  	max = MAX_DISPMODE;		break;
		 default:  ptr = &para_bk.str.KeyProssLED;  max = MAX_KEYPROCLED;	break;
	}
	
	rev =   *ptr;
	if(sts != 0){ rev++; }
	else		{ rev--; }
	
	if(rev > max)
	{
		 if(sts != 0) rev = 0;
		 else		  rev = max;
	}
	*ptr = rev;
	// ��ַ��Ӧд��洢��
	if(Disp.MenuPage == 0)
	{ 
		addr  = Buffer[0]*100 + Buffer[1]*10 + Buffer[2]; 
		if(addr > MAX_ADDR)
		{ addr = MAX_ADDR;}
		para_bk.str.address = addr;
	}
		
	Disp.f_OffDisp 		    = OFF;				   // ����ʾ��ֵ
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // ���ó�ʱʱ��	
	SetDispPage( CfgPageDB[Disp.MenuPage]  );	   // ����ҳ�棬�ø��±�־
}
/*******************************************************************************
	                    ����ҳ�棬ҳ����      
input:  sts = 0��ֵ��1����0���1
Disp.MenuPage ��Ӧ�ĸ�������Disp.Focus��Ӧ����ʾ����λ	
���Ӧ�����Ľ���λ�����ֵ						   
*******************************************************************************/
INT8U CODE		  CfgParaFocusDb[MAX_CFGPARA] = {3,1,1,1,1,1} ;		 // ��Ӧ����ܵ�λ��

void CfgChgPage(INT8U sts)
{
	if(sts != 0)
	{ 
		if( ++Disp.MenuPage >= MAX_CFGPARA )
		{ Disp.MenuPage = 0; }			 	// ���ܲ���ҳ���л�   
	}
	else
	{
		if( Disp.MenuPage-- == 0 )
		{ Disp.MenuPage = MAX_CFGPARA-1; }	// ���ܲ���ҳ���л�      
	}
			   	
	Disp.Focus = CfgParaFocusDb[Disp.MenuPage]-1;	// ����λ��ʼ��
	
	Disp.f_OffDisp 		    = OFF;				    // ����ʾ��ֵ
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		 	// ��ʱ��������ʼ��
	SetDispPage( CfgPageDB[Disp.MenuPage]  );		// ����ҳ�棬�ø��±�־
}
/*******************************************************************************
	                    ����ҳ�棬��ֵ�Ľ���λ���      
input:  sts = 0��ֵ��1����0���1
Disp.MenuPage ��Ӧ�ĸ�������Disp.Focus��Ӧ����ʾ����λ	
���Ӧ�����Ľ���λ�����ֵ							      
*******************************************************************************/

void CfgChgFocus(INT8U sts)
{
	if(sts != 0)
	{ 
		if( ++Disp.Focus >= CfgParaFocusDb[Disp.MenuPage] )
		{ Disp.Focus = 0; }  
	}
	else
	{
	#if(MACHINETYPE == A2_820)
		if( Disp.Focus-- == 1 )
	#else
		if( Disp.Focus-- == 0 )
	#endif
		{ Disp.Focus = CfgParaFocusDb[Disp.MenuPage]-1; }     
	}
	if(Disp.MenuPage == 0)			   	
	{ Disp.f_OffDisp = ON; }				   	   // ����ʾ��
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // ��ʱ��������ʼ��
	SetDispPage( CfgPageDB[Disp.MenuPage] );	   // ����ҳ�棬�ø��±�־	
}

/************************************************************************
* ���ù���ģʽ�£�����������
* ���밴��ֵ
************************************************************************/
void	KeyConfigFun(INT8U keyvalue)
{
	//==================================================================			
	switch( keyvalue )
	{	
		//--------------------------------------------------------------
		// ���ܼ�  ��������˳�����,�̰���ɲ���ҳ���л�����
		case KEYFUN_FUNC:
		{ 			
			if(Key.f_LongOnce == TRUE)
			{
				Key.f_LongOnce = FALSE;		// ִֻ��һ��
				Exit_Cfg_Mode();			// �˳�����ģʽ���������ֵ���ָ���ʾ
			}
			else
			{ CfgChgPage(1); }			 	// ���ܲ���ҳ���л�								
		}break;						
		//--------------------------------------------------------------
		// -1		
		case KEYFUN_SUB :
		{						
			CfgChgParaVal(0);			  //�ı��Ӧ��ֵ������֤�ڷ�Χ��		 	
		}break;		
		//--------------------------------------------------------------
//		// +1
//		case KEYFUN_ADD   :
//		{								
//			CfgParaValChg(1);			 //�ı��Ӧ��ֵ������֤�ڷ�Χ��
//		}break;							
//		//--------------------------------------------------------------
		// ��ǰ�� 
//		case KEYFUN_FORWARD :
//		{								
//			CfgFocusChg(1);				//�ı��Ӧ�Ľ���λ������֤�ڷ�Χ��
//		}break;		
		//--------------------------------------------------------------
		// ���� 	
	    case KEYFUN_BACK:
	    {			
			CfgChgFocus(0);				//�ı��Ӧ�Ľ���λ������֤�ڷ�Χ��	
		}break;			
		//--------------------------------------------------------------
		// ȷ�ϼ� 		
	    case KEYFUN_OK:
	    {		    			
			 Exit_Cfg_Mode();		// �˳�����ģʽ
		}break;	
		//--------------------------------------------------------------
		// ������ 		 							
		default:	break;					 							
	}					
}
/************************************************************************
* ��������ִ���ӳ���
* ���밴��ֵ��ʹ��LCDˢ�±�־
************************************************************************/
void Key_Fun(INT8U keyvalue)
{		
//	if((FALSE == Key.f_Long)&&(KEYFUN_OK != keyvalue))
//	{ AlarmSet(ALMKEY); } 		//����������
		
	// �ж��Ƿ���������վַ
	if(OsCmdSet.beSettingAddr == 1)
	{ 
		//������Ŷ����µ�ַ
//		address = toSettedAddr;	
		para.str.address = para_bk.str.address;	// �����趨�ĵ�ַ
		SaveParaToBlock();						// �жϲ����Ƿ��ڷ�Χ��(�粻����ָ�)���������

		led.light_type = LED_BLUE;				// ����ǰ����һ��ƣ����պ����ڶ����
		OsCmdSet.beSettingAddr  = 2;			// ˵��վַ�趨�Ѿ����°���ȷ��
	} 

	//����״̬
	if ((KeyMode.f_Config == ON))
	{ KeyConfigFun(keyvalue); return; }

	//������״̬
	KeyNormolFun(keyvalue);
}

/************************************************************************
* ��ȡ����ֵ
* ���ؼ�ֵ		 // ��ȡһ�νӽ�3ms
************************************************************************/


INT8U	Scan_Val(void)
{

	INT8U	keyval = KEYFUN_NOKEY;
	//-------------------------------------------	
	 // ��IO��Ϊ1
 #if	( (MACHINETYPE == P0_910U)||(MACHINETYPE == A0_820) ) 
	 KEY_OK = 1;
	 NOP;  NOP;	 NOP; NOP; NOP;

	if (KEY_OK == 0)						//ȷ��
	{ keyval = KEYFUN_OK; }
 #else
	 KEY_OK = KEY_SUB = KEY_SHT = KEY_CFG = 1;
	 NOP;  NOP;	 NOP; NOP; NOP;

	if (KEY_OK == 0)						//ȷ��
	{ keyval = KEYFUN_OK; }
	else if (KEY_SUB == 0)					//��1
	{ keyval = KEYFUN_SUB; }
	else if (KEY_SHT == 0)				    //��λ
	{ keyval = KEYFUN_BACK; }
	else if (KEY_CFG == 0)					//����
	{ keyval = KEYFUN_FUNC; }
 #endif
	//-------------------------------------------		
	return( keyval );
}

/************************************************************************
** ���������� Key scan and Key Function module
************************************************************************/
void Key_Scan(void)
{		
	INT8U	keyval;
	/*------------------------------------------------------------------*/
	// ϵͳ��������յ����ݣ�������Ч
	if( (OsCmdSet.SystemLocked == ON) )//||(uart.rIndex != 0) ){return;}
	{return;}
	//ɨ���־ʹ��ȷ��,��ʱ��32msʹ��һ��
	if(Key.f_Enable == FALSE){return;}
	Key.f_Enable = FALSE;		//ɨ������ʹ�ܱ�־
//	Key.ScanCnt++;				//����ɨ�����		
	/*------------------------------------------------------------------*/
	//��ֵɨ�� ���õ���Ӧ����ֵ
	keyval = Scan_Val();					
	/*------------------------------------------------------------------*/
	//������μ�ֵ��һ�£������ֵ��������
	if( keyval != Key.Backup ) { Key.Backup = keyval; return; }		
	/*------------------------------------------------------------------*/
	//������μ�ֵһ�£�����
	if( KEYFUN_NOKEY == keyval )
	{ 	//�ް�������
		if( (TRUE == Key.f_Short)&&(FALSE == Key.f_Long) )
		{
		   if(Key.BackProssVal == KEYFUN_FUNC)
		   	{ Key_Fun(KEYFUN_FUNC); }	   		//ִ�ж̰����ܼ�
		   else if((Key.BackProssVal == KEYFUN_SUB)&&(UnfinishTask.num > 0))
		   { Key_Fun(KEYFUN_SUB); }	   			//ִ�ж̰�-1��
		}
		
		Key.f_Short    = FALSE ;			//�̰��ͷ�
		Key.f_LongOnce = FALSE ;			//����һ���ͷ�	
		Key.f_Long 	   = FALSE;				//�����ͷ�		
		Key.BackProssVal = keyval;			//���ݼ�ֵ
		return;								// ����
	}
	// δ�ͷţ�����̰�
	if( FALSE == Key.f_Short )
	{	//��һ�ΰ���	
		Key.f_Short = TRUE;					//�̰���Ч
		Key.OnCnt	= 0x00;					//����ʱ���ʼ��
//		if( KEYFUN_FUNC != keyval )			// ���ܼ����ȳ���
		if( ((KEYFUN_FUNC != keyval)&&(KEYFUN_SUB != keyval))
		  ||((KEYFUN_SUB == keyval)&&(UnfinishTask.num == 0)) )			// ���ܼ���-1�����ȳ���
		{ Key_Fun(keyval); }				//ִ�а�������
			
		Key.BackProssVal = keyval;			//���ݼ�ֵ
		return;								// ����
	}
	//����ס���ͷţ�ĳЩ����Ч			
	if( TRUE == Key.f_Long )
	{	//����ס���ͷţ�ĳЩ����Ч		
//		if( Key.OnCnt++ >= KEYLONGPROTIME )
//		{
//			Key.OnCnt	= 0x00;				//���ʱ���ʼ��
//			if( (KEYFUN_ADD == keyval)||(KEYFUN_SUB == keyval) )			
//			{ Key_Fun(keyval); }			//���һ��ʱ��ִ��һ�ΰ�������	
//		}
		return;								// ����
	}
	//������Ч��ִֻ��һ��
	if( Key.OnCnt++ >= KEYLONGTIME )
	{				
		Key.f_LongOnce  = TRUE;				//����һ����Ч
		Key.OnCnt	= 0x00;					//���ʱ���ʼ��

		// Fn�������͡�-����������δ��������0	���Ŵ���
		if( (KEYFUN_FUNC == keyval)||((KEYFUN_SUB == keyval)&&(UnfinishTask.num > 0)) )
		{ Key_Fun(keyval); }				//���������ܼ�

		Key.f_Long  	= TRUE;				//������Ч
	}
}
/*******************************************************************************
								END					
*******************************************************************************/

