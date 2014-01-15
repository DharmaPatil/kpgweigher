#ifndef __FONT_H
#define __FONT_H

#include "typedef.h"

typedef flash struct typFNT_GB24		// ������ģ���ݽṹ
{
	uchar Index[4];
	uchar Msk[72];
}FNT_GB24;


typedef flash struct typFNT_GB16		// ������ģ���ݽṹ
{
	uchar Index[4];
	uchar Msk[32];
}FNT_GB16;

typedef flash struct typFNT_GB12		// ������ģ���ݽṹ
{
	uchar Index[4];
	uchar Msk[24];
}FNT_GB12;

//�ֿ�
extern FNT_GB12 flash HZK12[];
extern FNT_GB16 flash HZK16[];
extern uchar flash ASC8x16[256][16];
extern uchar flash Font24X32[13][96];
extern uchar flash font6x8[256][8];

#endif