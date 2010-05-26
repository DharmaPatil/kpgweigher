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
#define KEY_A	0x100000
#define KEY_B	0x200000
#define KEY_C	0x400000
#define KEY_D	0x800000

#define KEY_E	0x10000
#define KEY_F	0x20000
#define KEY_G	0x40000
#define KEY_H	0x80000

#define KEY_I	0x1000
#define KEY_J	0x2000
#define KEY_K	0x4000
#define KEY_L	0x8000

#define KEY_M	0x100
#define KEY_N	0x200
#define KEY_O	0x400
#define KEY_P	0x800
// ����PT6312�ĳ���
// ��ʼ��PT6312
void PT6312_Init();

// ��ջ�����
void PT6312_Refresh();

void PT6312_Print(u8 *buf, u8 size);

// д��LED����
void PT6312_WriteLED( u8 dat );

// ��ȡ����
u32 PT6312_ReadKey();

void PT6312_Test();

char highc(u8 x);
char lowc(u8 x);
/*
	ds1302 related constant
*/

sbit T_CLK = P1^1; /*ʵʱʱ��ʱ�������� */
sbit T_IO = P1^2; /*ʵʱʱ������������ */
sbit T_RST = P1^3; /*ʵʱʱ�Ӹ�λ������ */



void ds1302_init();

void ds1302_get(u8 ucCurtime[]);
void ds1302_set(u8 ucCurtime[]);
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
