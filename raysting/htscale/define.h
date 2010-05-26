#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "stc_new_8051.h"

#include <absacc.h>
#include <intrins.h>

#define	MAX(a,b)	(a>b?a:b)
#define MIN(a,b)	(a<b?a:b)

typedef unsigned char	u8;
typedef unsigned int	u16;
typedef int		i16;
typedef char		i8;
typedef unsigned char	bool;
typedef unsigned long	u32;
typedef	long		i32;
typedef	char		BOOL;
typedef unsigned char  INT8U;
typedef unsigned int   INT16U;
typedef unsigned char BYTE;



#define	false		0
#define	true		1

#ifndef NULL
#define	NULL		0
#endif

/*
	PT related function
*/
// ����PT6312�ĳ���
// ��ʼ��PT6312
void PT6312_Init();

// ��ջ�����
void PT6312_Clear();

// д��LED����
void PT6312_WriteLED( u8 dat );

// ��ȡ����
u32 PT6312_ReadKey();

void PT6312_Test();

/*
	ds1302 related constant
*/
#define DS1302_RSTART	0x80
#define DS1302_WSTART	0x81
#define DS1302_DATALEN	7

sbit DS1302_RST = P1^3;//mcu pin 43
sbit DS1302_IO = P1^2;//mcu pin 42
sbit DS1302_SCLK = P1^1;//mcu pin 41



void ds1302_get(u8 addr,u8 *p, u8 n);


/*
	eeprom constants
*/
//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx ϵ�� EEPROM ������ʼ��ַ

INT8U Byte_Read(INT16U add);              //��һ�ֽڣ�����ǰ���IAP ����
void Byte_Program(INT16U add, INT8U ch);  //�ֽڱ�̣�����ǰ���IAP ����
void Sector_Erase(INT16U add);            //��������
void IAP_Disable();                       //�ر�IAP ����
void Delay();

/* 
	void int0_hander(void) interrupt 0
	void timer0_hander(void) interrupt 1
	void int1_hander(void) interrupt 2
	void timer1_hander(void) interrupt 3
	void uart_hander(void) interrupt 4
	void lvd_hander(void) interrupt 6
*/
#endif
