    #include <stc51.h>
	#include <stdio.h>
	#include <intrins.h>
	#include<MATH.H>
 	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long
 ulong code Sector[10][4]={
	                       {0x8000,0x8001,0x8002,0x8003},
			               {0x8200,0x8201,0x8202,0x8203},
	           	           {0x8400,0x8401,0x8402,0x8403},
			               {0x8600,0x8601,0x8602,0x8603},
			               {0x8800,0x8801,0x8802,0x8803},
			               {0x8a00,0x8a01,0x8a02,0x8a03},
	        	           {0x8c00,0x8c01,0x8c02,0x8c03},
			               {0x8e00,0x8e01,0x8e02,0x8e03},
			               {0x9000,0x9001,0x9002,0x9003},
			               {0x9200,0x9201,0x9202,0x9203},						 
			                  };



extern void SectorErase(uint sector_addr);	// ��������
extern uchar byte_read(uint byte_addr);		// byte��
extern void byte_write(uint byte_addr, uchar original_data);	// byteд
extern uchar byte_write_verify(uint byte_addr, uchar original_data);// byteд��У��
extern uchar ArrayWrite(uint begin_addr, uint len, uchar code *array);	// byte����д��У��
extern void ArrayRead(uint begin_addr, uchar len);		// ����, ������Ttotal[]��

/*********************************************************************
                           EEPROMʹ��
*********************************************************************/
                /* - ����� - */
#define RdCommand		0x01	/* �ֽڶ� 	*/
#define PrgCommand		0x02	/* �ֽ�д 	*/
#define EraseCommand	0x03	/* �������� */
                /* ���峣�� */
#define Error   1
#define Ok      0
/* ����Flash��Ӧ��20MHz����ϵͳ�Ĳ����ȴ�ʱ�� */
/* ʱ�ӱ�ƵʱWaitTime�� 0x00*/

#define WaitTime	0x01
               /* - ����������С - */
#define PerSector		512
extern uchar Ttotal[];
               /* = �� ISP,IAP ���� = */
void ISP_IAP_enable(void)
{
	EA	=	0;			/* ���ж�       */
	ISP_CONTR =	ISP_CONTR & 0x18;       /* 0001,1000	*/
	ISP_CONTR =	ISP_CONTR | WaitTime;	/* д��Ӳ����ʱ	*/
	ISP_CONTR =	ISP_CONTR | 0x80;       /* ISPEN=1	*/
}
                 /* = �ر� ISP,IAP ���� = */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	/* ISPEN = 0 */
	ISP_TRIG	=	0x00;
	EA			=   1;			/* ���ж�    */
}
                /* = ���õĴ������� = */
void ISPgoon(void)
{
	ISP_IAP_enable();			/* �� ISP,IAP ����	*/
	ISP_TRIG	=	0x46;		/* ����ISP_IAP�����ֽ�1	*/
	ISP_TRIG	=	0xb9;		/* ����ISP_IAP�����ֽ�2	*/
	_nop_();
}
                   /* = �ֽڶ� = */
uchar byte_read(uint byte_addr)
{   
	ISP_ADDRH = (uchar)(byte_addr >> 8);	        /* ��ַ��ֵ     */
	ISP_ADDRL = (uchar)(byte_addr & 0x00ff);
	ISP_CMD   = ISP_CMD	& 0xf8;			/* �����3λ 	*/
	ISP_CMD   = ISP_CMD	| RdCommand;	       /* д�������	*/
	ISPgoon();					/* ����ִ��	*/
	ISP_IAP_disable();				/* �ر�ISP,IAP����*/
	return (ISP_DATA);				/* ���ض���������*/
}
                  /* = �������� = */
void SectorErase(uint sector_addr)
{
	uint iSectorAddr;
	iSectorAddr = (sector_addr & 0xfe00);         /* ȡ������ַ */
	ISP_ADDRH = (uchar)(iSectorAddr >> 8);
	ISP_ADDRL = 0x00;
	ISP_CMD	= ISP_CMD & 0xf8;		      /* ��յ�3λ 	*/
	ISP_CMD	= ISP_CMD | EraseCommand;	      /* ��������3 	*/
	ISPgoon();					/* ����ִ��	*/
	ISP_IAP_disable();				/* �ر�ISP,IAP����*/

}
               /* = �ֽ�д = */
void byte_write(uint byte_addr, uchar original_data)
{
  	ISP_ADDRH =	(uchar)(byte_addr >> 8); 	/* ȡ��ַ 	*/
	ISP_ADDRL =	(uchar)(byte_addr & 0x00ff);
	ISP_CMD	 = ISP_CMD & 0xf8;			/* ���3λ	*/
	ISP_CMD  = ISP_CMD | PrgCommand;		/* д����2	*/
	ISP_DATA = original_data;			/* д������׼��	*/
	ISPgoon();				        /* ����ִ��	*/
	ISP_IAP_disable();				/* �ر�IAP����	*/
}


/********����˵��********************************************
  byte_read(Sector[1][3])        ��һ���ֽ�
  byte_write(Sector[1][3],char)  ��charд������ֽ���
   SectorErase(Sector[1][2])     ������� 
**/
