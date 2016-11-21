#ifndef _MYTYPE_H_
#define _MYTYPE_H_


typedef unsigned char  		    BOOLEAN;
typedef unsigned char  		    INT8U; 
typedef signed   char  		    INT8S;
typedef unsigned short int	    INT16U;
typedef signed   short int	    INT16S;
typedef unsigned long  int   	INT32U;  
typedef signed   long  int   	INT32S;
//typedef unsigned long long   INT64U; 
//typedef signed   long long   INT64S;
	
typedef volatile unsigned char  INT8VU;
typedef volatile unsigned short int	INT16VU;
typedef volatile unsigned long  int INT32VU;

typedef unsigned char			UINT8;	
typedef unsigned short int	    UINT16;	
typedef unsigned long  int   	UINT32;

typedef signed   char  		    SINT8;	
typedef signed   short int	    SINT16;	
typedef signed   long  int   	SINT32;	

#define DATA	data
#define IDATA	idata
#define XDATA	xdata		  	
	
#define CODE	const code

#define FAIL 		0
#define OK   		1

#define FALSE       0
#define TRUE        1


#define NO          0
#define YES         1

#define ON			1
#define OFF			0

#define HIGH(x) ((x &0xff00)>>8)
#define LOW(x)  (x &0xff)
#define _MIN(a,b)  (a)<(b) ? a :b
#define _MAX(a,b)  (a)>(b) ? a :b

#define NOP	    (_nop_())



#endif
