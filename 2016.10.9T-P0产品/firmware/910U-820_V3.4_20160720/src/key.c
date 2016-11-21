
#define AP_KEY_GLOBALS	
#include "extern_symbol.h" 

/*
// 按键功能说明 
*/
								 
/*******************************************************************************
	 			  任务保存共用函数
	input:   ptr1为保存区的地址，ptr2 为数据区的地址     
	output:  none					
*******************************************************************************/
void	PutFinishedToStack_Com(TASK_INFO XDATA *ptr1,TASK_INFO XDATA *ptr2)
{
	UINT8 i ,len;
		  
	len = (*ptr1).srcode_len = (*ptr2).srcode_len;  

	if(len > MAX_SRCODE_NUM) { len = MAX_SRCODE_NUM; }
	// 保存批次信息
	for(i = 0; i < len; i++)
	{  (*ptr1).srcode[i] = (*ptr2).srcode[i]; }

	(*ptr1).sku   = (*ptr2).sku;					// 库位号
	(*ptr1).count = (*ptr2).count;					// 数量
//	(*ptr1).full  = (*ptr2).full;					// 满箱操作标志
}	

/*******************************************************************************
	 			一个拣货任务完成后，存入任务历史堆栈，以供回显使用
	input:   none     
	output:  none					
*******************************************************************************/
void PutFinishedToStack(void)
{
	UINT8 i ;

	//堆栈未满，直接压入
	if (BackTask.num < MAX_BACK_NUM)
	{
		// 第一个指针是要保存区的地址，第二个指针是数据区的地址
		PutFinishedToStack_Com(&BackTask.info[BackTask.num],&UnfinishTask.info[0]);
		BackTask.num++;
	}
	//堆栈已满，先移动再压入
	else
	{
		for (i = 0; i < MAX_BACK_NUM-1; i++)
		{
			// 第一个指针是要保存区的地址，第二个指针是数据区的地址
			PutFinishedToStack_Com(&BackTask.info[i],&BackTask.info[i+1]);
		}
		// 第一个指针是要保存区的地址，第二个指针是数据区的地址
		PutFinishedToStack_Com(&BackTask.info[MAX_BACK_NUM-1],&UnfinishTask.info[0]);
		BackTask.num = MAX_BACK_NUM;
	}
}

/*******************************************************************************
 正常模式下，改变对应的值，并保证在范围内            
*******************************************************************************/
void NormolChgVal(INT8U sts)
{
	INT8U	rev,num;//,val[4];
	INT16U	usValue;
	
	if( KeyMode.f_ChangeValue == OFF ) {return;}

	//其它原先没有长按下Fn键的情况下，并且当前显示数值<DISPLAY_MAX_NUM，并且任务显示时
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
		

		Disp.f_OffDisp 		    = OFF;				// 先显示数值
		Disp.TatolTimeRefresh8	= TIMER_MODIFY;		// 超时退出
		SetDispPage(PAGE_RUN);
	}
}

/*******************************************************************************
 			正常模式下，符合条件则设置更改
			改变对应的焦点位，并保证在范围内            
*******************************************************************************/
void NormolChgFocus(INT8U sts)
{
	INT8U	num;
	INT16U	usValue;	
				
//	usValue = ((UINT16)(task_un_goods_numH[0])) *256 + (UINT16)(task_un_goods_numL[0]);

	//其它原先没有长按下Fn键的情况下，并且当前显示数值<DISPLAY_MAX_NUM，并且任务显示时
//	if (DispMode == 0X00 && task_unfinish_num > 0 && usValue <=DISPLAY_MAX_NUM)
//	if( (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) )
	num     = UnfinishTask.num;
	usValue = UnfinishTask.info[0].count;
//	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < DISPLAY_MAX_NUM)&&(DIGILED_COUNT != 0) )
	if( (num > 0) && (num <= MAX_TASK_NUM) && (usValue < os_disp_set.DispMax_count)&&(os_disp_set.DispLed_count != 0) )
	{
		if(KeyMode.f_ChangeValue == OFF)
//		{ Disp.Focus = DIGILED_COUNT-1; }					 // 把设置值设在最低位		4位对应3
		{ Disp.Focus = os_disp_set.DispLed_count-1; }					 // 把设置值设在最低位		4位对应3
		else if(sts != 0)
		{ 
//			if( ++Disp.Focus >= DIGILED_COUNT)			 // 焦点位前移
			if( ++Disp.Focus >= os_disp_set.DispLed_count)			 // 焦点位前移
			{ Disp.Focus = CHG_TASK_COUNT_FIRST; }
		}
		else
		{
			if( Disp.Focus-- == CHG_TASK_COUNT_FIRST )
//			{ Disp.Focus = DIGILED_COUNT-1; }		   			 // 焦点位后移	
			{ Disp.Focus = os_disp_set.DispLed_count-1; }		   			 // 焦点位后移	
		}	
		
		KeyMode.f_ChangeValue  = ON; 				// 设置值修改标志
		Disp.f_OffDisp 		   = ON;				// 先显示灭
		Disp.TatolTimeRefresh8 = TIMER_MODIFY;		// 超时退出
		SetDispPage(PAGE_RUN);
	}
}
/************************************************************************
* 按键正常功能,非配置状态按键处理函数
* 输入按键值
************************************************************************/
void	KeyNormolFun(INT8U keyvalue)
{
	 INT8U	i;
//	 UINT16 usValue = 0;

	// 如进入配置参数密码状态，非"后退"按键，则清除配置及显示，退出
//	// 其他按键清除该配置状态
	if((Key.f_LongFunc == TRUE)&&( keyvalue != KEYFUN_BACK ))
	{  Display_Clear(); return; }

	// 提取出共用的部分
	// 原先按下Fn做满箱操作的情况下,则非OK键，清除该标志并刷新显示
	if( (Key.f_IsFnKeyPressed == ON)&&(keyvalue != KEYFUN_OK) )
	{
		Key.f_IsFnKeyPressed = OFF;  
		SetDispPage(Disp.Page);
		return;
	}

	//==================================================================
	//非配置状态模式下，按键处理函数			
	switch( keyvalue )
	{	//--------------------------------------------------------------
		// 功能键  长按完成退出功能,短按完成参数页面切换功能
		case KEYFUN_FUNC:
		{ 			
			//长按，如果无任务则进入配置界面，有任务则根据F3参数选择零拣或置满箱操作标志
			if(Key.f_LongOnce == TRUE)
			{  
				Key.f_LongOnce = FALSE;		// 只执行一次
				
				//无作业任务且完成缓冲区空，长按进入配置状态.
//				if ((task_finished_num ==0)&&(task_unfinish_num ==0))//无作业任务且完成缓冲区空，可进入配置状态.
//				if ( (FinishTask.num == 0)&&(UnfinishTask.num == 0) )//无作业任务且完成缓冲区空，可进入配置状态.  	//用FinishTask做条件如果是启用F1功能，则Finish不会被清0，进不了配置模式
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		// 未完成为0，剩余为0，则认为任务为空
				{
					Key.f_LongFunc = TRUE;						// 进入配置状态的密码显示状态
					Key.CfgPwdCnt  = 0;							// 密码核对次数
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/3;		// 超时退出
					SetDispPage( PAGE_CFGPWD );				   	// 进入地址配置密码显示页面
//					// 进入配置状态
//					KeyMode.f_Config = ON;						   // 进入配置状态
//					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 超时退出					
//					Disp.Focus		 = 2;						   // 焦点位设置,地址的最低位
//					Disp.MenuPage	 = 0;						   // 菜单页面设置 ，进入地址设置界面
// 
//					for (i = 0; i < MAX_CFGPARA; i++)
//					{ 			
//						para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
//					}                  
//					SetDispPage( PAGE_CFGADDR );				   // 进入地址配置显示页面
				}
//				else if(task_unfinish_num > 0) //有显示数字信息的时候，长按Fn键，清零（以支持零拣）或置满箱操作标志
				else if(UnfinishTask.num > 0) //有显示数字信息的时候，长按Fn键，清零（以支持零拣）或置满箱操作标志
				{	// 根据参数F3 选择零拣或置满箱操作标志，零拣改用长按"-“键来完成
					switch(para.str.FnDefine)
					{
//						case 0:
//						if(DispMode != 0X01)
//						{
//							task_un_goods_numH[0] =  0;
//							task_un_goods_numL[0] =  0;
////							digital_led_refresh = ON;    //使能刷新数据
////							SetLcdLay( LCD.Type );				 // 回到运行页面
//							LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标志
//						}
//						break;
						case 1:
						Key.f_IsFnKeyPressed = ON;					// 置Fn键长按标志
						SetDispPage(PAGE_RUN);
						break;
					}
				}                    				
			}
			//短按，如果无任务且已有完成任务存储则进入回显界面，有任务则恢复当前显示任务
			//非配置状态下,短按Fn,开始回翻数据，每按一次FN键往前回翻一个，此时SYS灯亮，并且指示灯颜色不变，快闪显示，按OK键或者全部回翻完毕后结束恢复至原来状态
			else
			{					
				//拣货过程中，短按Fn键，恢复下发时的初始数据
//				if(DispMode != 0X01 && task_unfinish_num > 0)
//				if(task_unfinish_num > 0)
				if(UnfinishTask.num > 0)
				{	// 可快速恢复到数值
//					task_un_goods_numH[0] =  task_curOri_goods_numH;
//					task_un_goods_numL[0] =  task_curOri_goods_numL;

					UnfinishTask.info[0].count = OsCmdSet.TaskCurCount;	   // 恢复当前显示任务
					Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); //清数量滚屏标志	
					SetDispPage(PAGE_RUN);
				}
				//只有全部拣货任务完成后才能进入回显模式   // 未完成任务和剩余任务均为0
//				else if(task_unfinish_num == 0 && task_unupdate_num == 0) 
				else if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )
				{						
//					if(DispContentStackCount > 0)
					if(BackTask.num > 0)
					{
						//进入回显模式
						if(KeyMode.f_BackDisp == OFF)
						{	// 非回显模式则置回显模式标志，并进入回显页面
							KeyMode.f_BackDisp = ON;
							BackTask.index = BackTask.num - 1;		// 回显最后一个
//							CurBackDispIndex = DispContentStackCount -1;
							Disp.f_Rolling = ROLL_MODE_OFF; 		// 清滚屏标志
							SetDispPage( PAGE_BACKDISPLAY );		// 进入回显页面
						}
						else
						{
//							if(CurBackDispIndex == 0)
							if(BackTask.index == 0)
							{	// 回显模式且回显到第一个则退出回显页面
								KeyMode.f_BackDisp = OFF;
//								CurBackDispIndex = 0;
								Display_Clear();		   // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
							}
							else
							{
//								CurBackDispIndex--;
								BackTask.index--;
//								digital_led_refresh = ON;	  //使能刷新数码管
								Disp.f_Rolling = ROLL_MODE_OFF; 				// 清滚屏标志
								SetDispPage( PAGE_BACKDISPLAY );				// 进入回显页面
							}					
						}
					}
				}									 
			}
		}break;					
		//--------------------------------------------------------------
		// 向下键		
		case KEYFUN_SUB :
		{						
			if(Key.f_LongOnce == TRUE)
			{  //长按‘-’键，数量值清0，能进入这里说明在任务执行中
				Key.f_LongOnce = FALSE;		// 只执行一次

//				if( (task_unfinish_num > 0)&&(DispMode != 0X01) ) //有显示数字信息的时候，长按-1键，清零（以支持零拣）
//				if( (task_unfinish_num > 0) ) //有显示数字信息的时候，长按-1键，清零（以支持零拣）
				if( (UnfinishTask.num > 0) ) //有显示数字信息的时候，长按-1键，清零（以支持零拣）
				{
					UnfinishTask.info[0].count = 0;		 // 清0
					Disp.f_Rolling &= ~(ROLL_MODE_COUNT | ROLL_MODE_COUNT_ED); //清数量滚屏标志
					SetDispPage(PAGE_RUN);				 // 设置任务进行中显示页面
				}
			}
			else	if(KeyMode.f_BackDisp == ON)
			{// 回显界面
			}
			else
			{
				NormolChgVal(0);		// 判断条件符合则设置数量，条件为任务执行中，且数量不超过最大显示值
			}			 	
		}break;		
		//--------------------------------------------------------------
		// 向上键
//		case KEYFUN_ADD   :
//		{								
//			{
//				NormolChgVal(1);		// 判断条件符合则设置数量
//			}
//		}break;							
		//--------------------------------------------------------------
		// 向前键 
//		case KEYFUN_FORWARD :
//		{								
//			if(ucBackDispMode == ON)
//			{  // 回显界面
//				if((CurBackDispIndex >= DispContentStackCount)||(CurBackDispIndex >= MAX_BACK_NUM))
//				{	// 回显模式且回显到第一个则退出回显页面
////					ucBackDispMode = OFF;
////					CurBackDispIndex = 0;
//					Display_Clear();		  		// 包含清除回显标志和索引
////					LED_ALL_OFF();			  //关灯
////					SetLcdLay( PAGE_RUN );				 // 进入回显页面
//				}
//				else
//				{
//					CurBackDispIndex++;
////					CurBackDispPage = CurBackDispIndex/5 + 1;
////					digital_led_refresh = ON;	  //使能刷新数码管
////					SetLcdLay( PAGE_BACKDISPLAY );				 // 进入回显页面
////					LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标志
//					SetDispPage(Disp.Page);
//				}
//			}
//			else
//			{	// 非回显界面，有任务则进入该拣货数量界面
//				NormolChgFoc(1);						// 焦点位前移
//			}	
//		}break;
		
		//--------------------------------------------------------------
		// 向后键 	
	    case KEYFUN_BACK:
	    {			
			if(KeyMode.f_BackDisp == ON)
			{// 回显界面
//				if(CurBackDispIndex == 1)
//				{	// 回显模式且回显到第一个则退出回显页面
////					ucBackDispMode = OFF;
////					CurBackDispIndex = 0;
//					Display_Clear();		  // 包含清除回显标志和索引
////					LED_ALL_OFF();			  //关灯
////					SetLcdLay( PAGE_RUN );				 // 进入回显页面
//				}
//				else
//				{
//					CurBackDispIndex--;
////					CurBackDispPage = CurBackDispIndex/5 + 1;
////					digital_led_refresh = ON;	  //使能刷新数码管
////					SetLcdLay( PAGE_BACKDISPLAY );				 // 进入回显页面
////					LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标志
//					SetDispPage(Disp.Page);
//				}
			}
			else if(Key.f_LongFunc == TRUE)
			{
 				if( Key.CfgPwdCnt++ >= 3 )
				{
					Key.f_LongFunc =  FALSE;
					// 进入配置状态
					KeyMode.f_Config = ON;						   // 进入配置状态
					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 超时退出					
					Disp.Focus		 = 2;						   // 焦点位设置,地址的最低位
					Disp.MenuPage	 = 0;						   // 菜单页面设置 ，进入地址设置界面
 
					for (i = 0; i < MAX_CFGPARA; i++)
					{ 			
						para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
					}                  
					SetDispPage( PAGE_CFGADDR );				   // 进入地址配置显示页面
				}
				else 
				{ 
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/6;		// 超时退出
					SetDispPage( PAGE_CFGPWD ); 
				}				   // 进入地址配置密码显示页面
			}
			else
			{	// 非回显界面，满足条件未完成任务大于0，且数量不超过最大数值
				NormolChgFocus(0);						// 焦点位后移，条件为任务执行中，且数量不超过最大显示值
			}	
		}break;	
		
		//--------------------------------------------------------------
		// 确认键 		
	    case KEYFUN_OK:
	    {		    			
			//修改焦点位数值状态关闭，不再闪烁
//			usCountModify = 0;
			KeyMode.f_ChangeValue = OFF;		// 清除数量修改标志

			//只有在未完成任务不空，并且非显示进度条时，才响应OK键。
//			if((task_unfinish_num > 0)&&(digital_led_loading == OFF))
			if( (UnfinishTask.num > 0)&&(Disp.f_Loading == LOAD_MODE_OFF) )
			{   //刷新数据
				//检查完成情况
				if(FinishTask.num < MAX_TASK_NUM)	//最大任务数2
				{
					//把数据存入已完成的数据区
					// 第一个指针是要保存区的地址，第二个指针是数据区的地址
					PutFinishedToStack_Com(&FinishTask.info[FinishTask.num],&UnfinishTask.info[0]);					
					// 判断是否有满箱标志
					if(Key.f_IsFnKeyPressed == ON)	
						{ FinishTask.info[FinishTask.num].full = 1; }//{ taks_finished_Full[0] = 1; }						
					else{ FinishTask.info[FinishTask.num].full = 0; }//{ taks_finished_Full[0] = 0; }
					FinishTask.num++;					// 完成任务数+1

					//完成的数据做历史存储以供回显使用
					PutFinishedToStack();
					//原先有2个任务队列，则未完成块中作处理
//					if(task_unfinish_num == 2)
					if(UnfinishTask.num == 2)
					{
						//当未完成块的数据=2时，数据前移 ，后往前挪                                           					
						// 第一个指针是要保存区的地址，第二个指针是数据区的地址
						PutFinishedToStack_Com(&UnfinishTask.info[0],&UnfinishTask.info[1]);
						OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;		// 保存当前数量
					}
//					task_unfinish_num--;
					UnfinishTask.num--;
				}

				//该标志下，按下OK则后面任务全部清除
				if(Key.f_IsFnKeyPressed == ON)
				{
//					task_unupdate_num = 0;			   // 剩余任务清0
//					task_unfinish_num = 0;
					UnfinishTask.num     = 0;		   // 未完成任务清0
					UnfinishTask.remain  = 0;		   // 剩余任务清0
					Key.f_IsFnKeyPressed = OFF; 	   // 清除Fn长按标志
				}
				//本次作业一全部完成任务，灯灭，数据清零 ，根据参数F1亮任务完成的颜色
//				if(task_unupdate_num == 0 && task_unfinish_num ==0)	 
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		// 未完成为0，剩余为0，则认为任务为空	 	
				{					
					Display_Clear();							  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
					led.light_type = para.str.FinishedLED;		  // 任务完成根据参数F1亮对应的灯色
					led.flash_mode = LED_FLASH_MODE_ALWAYS;		  // 常亮模式
				}
				//本次作业还有待拾取任务，进行拉条显示
				else											
				{	//任务缓冲区空或者主机还有剩余任务,显示-------					
//					if(task_unfinish_num > 0)
					if(UnfinishTask.num > 0)
					{	
						Disp.f_Loading    	    = LOAD_MODE_SHORT;	  // 置短时间载入标志
//						Disp.TatolTimeRefresh32 = (para.str.WtTime+1)*100;	 // LOADING短时间
						Disp.TatolTimeRefresh16 = (para.str.WtTime+1); // 短时间
						
					}
					else //if(task_unupdate_num > 0)				//本次作业还有任务，但是任务还没下发到本设备中，显示------
					{
						Disp.f_Loading    	    = LOAD_MODE_LONG;			 // 置长时间载入标志
//						Disp.TatolTimeRefresh32 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING长时间
						Disp.TatolTimeRefresh16 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING长时间						
					}

					os_time.Base_100ms= TIMER_BASE_100MS;			// 对100ms基准值重设定
					Disp.TimeRefresh  = DIGITAL_LED_LOAD_CNT_PER;	// 进度条时间赋值;
					Disp.Shift        = DIGILED_FIRST+1;			// 先亮"-"
					//先保存当前哪组灯亮，在设置拉条时没有灯亮
					led.light_type_BK = led.light_type;				// 备份灯颜色
					led.light_type    = LED_NONE;					// 关灯
					SetDispPage( PAGE_WAIT );						// 设置进度条显示页面
				}
			}
			else if(KeyMode.f_BackDisp == ON) //回显状态下，按OK键取消回显
			{
				Display_Clear(); 					 // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志				
			}       	
		}break;			 							
		default:   	break;		 							
	}					
}

/************************************************************************
* 退出配置模式
************************************************************************/
void	Exit_Cfg_Mode(void)	
{
	INT8U i;

	Display_Clear(); 					 		// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	if (Disp.TatolTimeRefresh8 == 0) return;    // 3s超时退出不保存

	// 备份区的数据写入主存区
	for (i = 0; i < MAX_CFGPARA; i++)
	{ para.Byte[i] = para_bk.Byte[i]; }			// 备份区的数据写入主存区

	SaveParaToBlock();					 		// 判断参数是否在范围内(如不再则恢复)，保存参数	
}	


/*******************************************************************************
	            配置页面，改变对应焦点位的数值 ，并保证在范围内
input:  sts = 0则值键1，非0则加1
Disp.MenuPage 对应哪个参数，Disp.Focus对应的显示焦点位
表 对应的设置显示页面				            
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
	// 地址对应写入存储器
	if(Disp.MenuPage == 0)
	{ 
		addr  = Buffer[0]*100 + Buffer[1]*10 + Buffer[2]; 
		if(addr > MAX_ADDR)
		{ addr = MAX_ADDR;}
		para_bk.str.address = addr;
	}
		
	Disp.f_OffDisp 		    = OFF;				   // 先显示数值
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 设置超时时间	
	SetDispPage( CfgPageDB[Disp.MenuPage]  );	   // 设置页面，置更新标志
}
/*******************************************************************************
	                    配置页面，页面变更      
input:  sts = 0则值键1，非0则加1
Disp.MenuPage 对应哪个参数，Disp.Focus对应的显示焦点位	
表对应参数的焦点位最大数值						   
*******************************************************************************/
INT8U CODE		  CfgParaFocusDb[MAX_CFGPARA] = {3,1,1,1,1,1} ;		 // 对应数码管的位置

void CfgChgPage(INT8U sts)
{
	if(sts != 0)
	{ 
		if( ++Disp.MenuPage >= MAX_CFGPARA )
		{ Disp.MenuPage = 0; }			 	// 功能参数页面切换   
	}
	else
	{
		if( Disp.MenuPage-- == 0 )
		{ Disp.MenuPage = MAX_CFGPARA-1; }	// 功能参数页面切换      
	}
			   	
	Disp.Focus = CfgParaFocusDb[Disp.MenuPage]-1;	// 焦点位初始化
	
	Disp.f_OffDisp 		    = OFF;				    // 先显示数值
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		 	// 超时计数器初始化
	SetDispPage( CfgPageDB[Disp.MenuPage]  );		// 设置页面，置更新标志
}
/*******************************************************************************
	                    配置页面，数值的焦点位变更      
input:  sts = 0则值键1，非0则加1
Disp.MenuPage 对应哪个参数，Disp.Focus对应的显示焦点位	
表对应参数的焦点位最大数值							      
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
	{ Disp.f_OffDisp = ON; }				   	   // 先显示灭
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 超时计数器初始化
	SetDispPage( CfgPageDB[Disp.MenuPage] );	   // 设置页面，置更新标志	
}

/************************************************************************
* 配置功能模式下，按键处理函数
* 输入按键值
************************************************************************/
void	KeyConfigFun(INT8U keyvalue)
{
	//==================================================================			
	switch( keyvalue )
	{	
		//--------------------------------------------------------------
		// 功能键  长按完成退出功能,短按完成参数页面切换功能
		case KEYFUN_FUNC:
		{ 			
			if(Key.f_LongOnce == TRUE)
			{
				Key.f_LongOnce = FALSE;		// 只执行一次
				Exit_Cfg_Mode();			// 退出配置模式，保存参数值，恢复显示
			}
			else
			{ CfgChgPage(1); }			 	// 功能参数页面切换								
		}break;						
		//--------------------------------------------------------------
		// -1		
		case KEYFUN_SUB :
		{						
			CfgChgParaVal(0);			  //改变对应的值，并保证在范围内		 	
		}break;		
		//--------------------------------------------------------------
//		// +1
//		case KEYFUN_ADD   :
//		{								
//			CfgParaValChg(1);			 //改变对应的值，并保证在范围内
//		}break;							
//		//--------------------------------------------------------------
		// 向前键 
//		case KEYFUN_FORWARD :
//		{								
//			CfgFocusChg(1);				//改变对应的焦点位，并保证在范围内
//		}break;		
		//--------------------------------------------------------------
		// 向后键 	
	    case KEYFUN_BACK:
	    {			
			CfgChgFocus(0);				//改变对应的焦点位，并保证在范围内	
		}break;			
		//--------------------------------------------------------------
		// 确认键 		
	    case KEYFUN_OK:
	    {		    			
			 Exit_Cfg_Mode();		// 退出配置模式
		}break;	
		//--------------------------------------------------------------
		// 其他键 		 							
		default:	break;					 							
	}					
}
/************************************************************************
* 按键功能执行子程序
* 输入按键值，使能LCD刷新标志
************************************************************************/
void Key_Fun(INT8U keyvalue)
{		
//	if((FALSE == Key.f_Long)&&(KEYFUN_OK != keyvalue))
//	{ AlarmSet(ALMKEY); } 		//蜂鸣器叫声
		
	// 判断是否主机设置站址
	if(OsCmdSet.beSettingAddr == 1)
	{ 
		//任意键排都记下地址
//		address = toSettedAddr;	
		para.str.address = para_bk.str.address;	// 保存设定的地址
		SaveParaToBlock();						// 判断参数是否在范围内(如不再则恢复)，保存参数

		led.light_type = LED_BLUE;				// 接收前亮第一组灯，接收后亮第二组灯
		OsCmdSet.beSettingAddr  = 2;			// 说明站址设定已经按下按键确认
	} 

	//配置状态
	if ((KeyMode.f_Config == ON))
	{ KeyConfigFun(keyvalue); return; }

	//非配置状态
	KeyNormolFun(keyvalue);
}

/************************************************************************
* 获取按键值
* 返回键值		 // 读取一次接近3ms
************************************************************************/


INT8U	Scan_Val(void)
{

	INT8U	keyval = KEYFUN_NOKEY;
	//-------------------------------------------	
	 // 置IO口为1
 #if	( (MACHINETYPE == P0_910U)||(MACHINETYPE == A0_820) ) 
	 KEY_OK = 1;
	 NOP;  NOP;	 NOP; NOP; NOP;

	if (KEY_OK == 0)						//确认
	{ keyval = KEYFUN_OK; }
 #else
	 KEY_OK = KEY_SUB = KEY_SHT = KEY_CFG = 1;
	 NOP;  NOP;	 NOP; NOP; NOP;

	if (KEY_OK == 0)						//确认
	{ keyval = KEYFUN_OK; }
	else if (KEY_SUB == 0)					//减1
	{ keyval = KEYFUN_SUB; }
	else if (KEY_SHT == 0)				    //移位
	{ keyval = KEYFUN_BACK; }
	else if (KEY_CFG == 0)					//功能
	{ keyval = KEYFUN_FUNC; }
 #endif
	//-------------------------------------------		
	return( keyval );
}

/************************************************************************
** 功能描述： Key scan and Key Function module
************************************************************************/
void Key_Scan(void)
{		
	INT8U	keyval;
	/*------------------------------------------------------------------*/
	// 系统锁定或接收到数据，按键无效
	if( (OsCmdSet.SystemLocked == ON) )//||(uart.rIndex != 0) ){return;}
	{return;}
	//扫描标志使能确认,定时器32ms使能一次
	if(Key.f_Enable == FALSE){return;}
	Key.f_Enable = FALSE;		//扫描则清使能标志
//	Key.ScanCnt++;				//按键扫描计数		
	/*------------------------------------------------------------------*/
	//键值扫描 ，得到对应按键值
	keyval = Scan_Val();					
	/*------------------------------------------------------------------*/
	//如果两次键值不一致，保存键值，并返回
	if( keyval != Key.Backup ) { Key.Backup = keyval; return; }		
	/*------------------------------------------------------------------*/
	//如果两次键值一致，则处理
	if( KEYFUN_NOKEY == keyval )
	{ 	//无按键按下
		if( (TRUE == Key.f_Short)&&(FALSE == Key.f_Long) )
		{
		   if(Key.BackProssVal == KEYFUN_FUNC)
		   	{ Key_Fun(KEYFUN_FUNC); }	   		//执行短按功能键
		   else if((Key.BackProssVal == KEYFUN_SUB)&&(UnfinishTask.num > 0))
		   { Key_Fun(KEYFUN_SUB); }	   			//执行短按-1键
		}
		
		Key.f_Short    = FALSE ;			//短按释放
		Key.f_LongOnce = FALSE ;			//长按一次释放	
		Key.f_Long 	   = FALSE;				//长按释放		
		Key.BackProssVal = keyval;			//备份键值
		return;								// 返回
	}
	// 未释放，处理短按
	if( FALSE == Key.f_Short )
	{	//第一次按下	
		Key.f_Short = TRUE;					//短按有效
		Key.OnCnt	= 0x00;					//按下时间初始化
//		if( KEYFUN_FUNC != keyval )			// 功能键优先长按
		if( ((KEYFUN_FUNC != keyval)&&(KEYFUN_SUB != keyval))
		  ||((KEYFUN_SUB == keyval)&&(UnfinishTask.num == 0)) )			// 功能键和-1键优先长按
		{ Key_Fun(keyval); }				//执行按键功能
			
		Key.BackProssVal = keyval;			//备份键值
		return;								// 返回
	}
	//长按住不释放，某些键有效			
	if( TRUE == Key.f_Long )
	{	//长按住不释放，某些键有效		
//		if( Key.OnCnt++ >= KEYLONGPROTIME )
//		{
//			Key.OnCnt	= 0x00;				//间隔时间初始化
//			if( (KEYFUN_ADD == keyval)||(KEYFUN_SUB == keyval) )			
//			{ Key_Fun(keyval); }			//间隔一定时间执行一次按键功能	
//		}
		return;								// 返回
	}
	//长按有效，只执行一次
	if( Key.OnCnt++ >= KEYLONGTIME )
	{				
		Key.f_LongOnce  = TRUE;				//长按一次有效
		Key.OnCnt	= 0x00;					//间隔时间初始化

		// Fn键长按和”-“键长按且未完成任务非0	，才处理
		if( (KEYFUN_FUNC == keyval)||((KEYFUN_SUB == keyval)&&(UnfinishTask.num > 0)) )
		{ Key_Fun(keyval); }				//处理长按功能键

		Key.f_Long  	= TRUE;				//长按有效
	}
}
/*******************************************************************************
								END					
*******************************************************************************/

