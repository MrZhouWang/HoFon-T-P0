//#include "main.h"
//#include "eeprom.h"
//#include "uart.h"
//#include "led.h"

#define AP_EPROM_GLOBALS	
#include "extern_symbol.h" 

typedef union union_temp16
{
    UINT16 un_temp16;
    UINT8  un_temp8[2];
}UNION_16;


UINT8 Byte_Read(UINT8 * add);              //��һ�ֽڣ�����ǰ���IAP ����
void Byte_Program(UINT8 * add, UINT8 ch);  //�ֽڱ�̣�����ǰ���IAP ����
void Sector_Erase(UINT16 add);            //��������
void IAP_Disable();                       //�ر�IAP ����


//��һ�ֽڣ�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ������:A = �����ֽ�
UINT8 Byte_Read(UINT8 * add)
{
    UNION_16 my_unTemp16;
    
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM �ֽڶ�����

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    return (IAP_DATA);
}

//�ֽڱ�̣�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ, A= �����ֽڵ�����
void Byte_Program(UINT8 * add, UINT8 ch)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM �ֽڱ������

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    IAP_DATA = ch;                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

//��������, ���:DPTR = ������ַ
void Sector_Erase(UINT16 add)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM ������������

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

void IAP_Disable()
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}
 

/*************************************************************************
	                    Read len bytes from eeprom
**************************************************************************/

void eeprom_read_block(UINT8 * Buff, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Buff[i] = Byte_Read(beginAddr);
        beginAddr++;
    }
    
    return;
}

/*************************************************************************
	                    write len bytes to eeprom
**************************************************************************/
void eeprom_write_block(UINT8 * Buff, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Byte_Program(beginAddr, Buff[i]);
        beginAddr++;
    }

    return;
}

/*************************************************************************
	                    erase a sector
**************************************************************************/

//data flash: all 4K 
//512Byte/sector, 8 sectors
//Writing data flash must erase the sector, and then write every byte;
//Each sector address as follows:
UINT16 CODE SECTOR_PAGE[] = {
    0x0000, 0x0200, 0x0400, 0x0600, 
    0x0800, 0x0A00, 0x0C00, 0x0E00
};

void eeprom_erase_sector(UINT16 pageAddr)
{
    Sector_Erase(pageAddr);
    return;
}

/*************************************
	Read Parameter from inn EEPROM
	with verify
*************************************/
//#define PARA_LEN    5
#define PARA_LEN MAX_CFGPARA
UINT8 ReadParaInEeprom(UINT8 offset)
{
	UINT8 i = offset;
	UINT8 para_page_0[PARA_LEN];
	UINT8 para_page_1[PARA_LEN];
	UINT8 para_page_2[PARA_LEN];

	if (offset >= PARA_LEN) return 0xff;
	
    eeprom_read_block(para_page_0, (UINT8 *)SECTOR_PAGE[0], PARA_LEN);
    eeprom_read_block(para_page_1, (UINT8 *)SECTOR_PAGE[1], PARA_LEN);
    eeprom_read_block(para_page_2, (UINT8 *)SECTOR_PAGE[2], PARA_LEN);
   
	if (para_page_0[i] == para_page_1[i])
	{
		if (para_page_1[i] != para_page_2[i])           //sector 2 error
		{
		    para_page_2[i] = para_page_0[i];            //Modify error byte
            eeprom_erase_sector(SECTOR_PAGE[2]);        //Erase sector 2
                                                        //Write sector 2
	    	eeprom_write_block(para_page_2, (UINT8 *)SECTOR_PAGE[2] , PARA_LEN);
		}
		return para_page_0[i];	
	}
	else if (para_page_0[i] == para_page_2[i])          //sector 1 error
	{
        para_page_1[i] = para_page_0[i];                //Modify error byte
    	eeprom_erase_sector(SECTOR_PAGE[1]);            //Erase sector 1
    	                                                //Write sector 1
	    eeprom_write_block(para_page_1, (UINT8 *)SECTOR_PAGE[1] , PARA_LEN);
    	return para_page_0[i];	
	}
	else                                                //Sector 0 error
	{
	    para_page_0[i] = para_page_1[i];                //Modify error byte
	    eeprom_erase_sector(SECTOR_PAGE[0]);            //Erase sector 0
	    eeprom_write_block(para_page_0, (UINT8 *)SECTOR_PAGE[0] , PARA_LEN);
    	return para_page_1[i];				
	}
}

/*******************************************************************************
	 		   �������Ƿ񳬳���Χ��������ָ�Ĭ��ֵ,�����뱸����
	input:   none
	output:  none					
*******************************************************************************/
//												1            2            3            4			 5			   6
INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR,MAX_FINISHEDLED,MAX_WTTIME,MAX_FNDEFINE,MAX_DISPMODE,MAX_KEYPROCLED };
INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR,DEF_FINISHEDLED,DEF_WTTIME,DEF_FNDEFINE,DEF_DISPMODE,DEF_KEYPROCLED };

void	ChkParaOverToDefVal(void)
{
	UINT8 i;

	// �жϲ��������������Χ��ظ�Ĭ��ֵ

	for (i = 0; i < MAX_CFGPARA; i++)
	{ 
	  	// �ж��Ƿ񳬳���Χ
		if(para.Byte[i] > Para_MaxTB[i])
	  	{ para.Byte[i] = Para_DefTB[i]; }

		para_bk.Byte[i] = para.Byte[i]; 			   // �����뱸����
	}

}
/*******************************************************************************
	 		    		save Parameter to EEPROM
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock(void)
{
	INT8U i;
	// �������Ƿ����������ָ����������뱸������
	ChkParaOverToDefVal();
	
	// д��	eeprom	��д3������
	for (i = 0; i < 3; i++)
	{
	    eeprom_erase_sector(SECTOR_PAGE[i]);   
		eeprom_write_block (&para.Byte[0], (UINT8 *)SECTOR_PAGE[i], MAX_CFGPARA);
	}

	sys_disp_set();		// �����趨ֵ�ı��λλ��������λ��
}
/*******************************************************************************
	 		    		Load Parameter from EEPROM
	input:   none		   
	output:  none					
*******************************************************************************/
void LoadParaInEeprom(void)
{
	// ��������
	INT8U	i;
	for(i = 0;i < MAX_CFGPARA;i++)
	{ para.Byte[i] = ReadParaInEeprom(EE_ADDRESS+i); }


	// �������Ƿ����������ָ����������뱸������
	if( (para.str.address == 0) )//||(addr == 0) )  
    { para.str.address = DEF_ADDR;  }     //Ĭ�ϼ��ز���

	ChkParaOverToDefVal();
}

/*******************************************************************************
								END					
*******************************************************************************/


