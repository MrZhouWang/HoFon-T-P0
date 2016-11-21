#ifndef _dnet_h_ 
#define _dnet_h_

/************************************************************************************/
#ifdef  AP_PROCESS_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_PROCESS_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_PROCESS_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

//BYTE0   命令号
//BYTE1   货位号 
//BYTE2   数量高位
//BYTE3   数量低位
//BYTE4   保留
//BYTE5   保留

//协议如下
//  0      1    2   3   4   5   6   7   8     
//命令字  长度 【2 - 7字节协议如上述】  和校验 
//        6+1     
//长度：从第一个数据字节起到最后一个字节，包括校验码。
//校验：校验码的产生方式—累加和。从命令号开始计算，到数据结束。

#define ONE_CMD_MIN_LENGTH      6	// 一次有效命令的最小长度

//下传的命令码
#define DISPMODE_NONE	0X00		//清空所有数据，无提示,led无显示
#define DISPMODE_ASII	0X02		//字符串显示
#define DISPMODE_TASK	0X03		//作业命令

#define READ_MULTI_BYTE			0x03		   // 03号命令（读取信息）
#define WRITE_SINGLE_BYTE		0x06		   // 06号命令（下发控制）
#define WRITE_MULTI_BYTE     	0x10		   // 10号命令（多数据下发控制）

#define  MULCAST_ADDR		 248
//================================================================================
// 函数声明
AP_PROCESS_EXT  void Cmd_Process(void);		 // 通讯命令接收解析处理及回复

#endif













