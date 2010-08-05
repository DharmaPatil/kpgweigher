/*
 *	File Name : SJDouble2String.h
 *	Author	  : sojo at sunext
 *	Details	  : omnifarious double-to-string conversions
 *	Notes	  : ���ֵ�λ�±�Ϊ ...3210.-1-2-3..
 *			Date		Description		
 *	History   : 
 * 			2004.11.4	created ,used in trydata3(RtestV1.4)
 */
 
#ifndef _SJ_DOUBLE2STRING_H_
#define _SJ_DOUBLE2STRING_H_
#include "stdafx.h"
#include "math.h"

void SJTryRound(double &value)
{
	if(value < 0) 
		value -= 1e-14;
	else
		value += 1e-14;
}
/*
 *	�õ�posnλ�õ��ַ�
 */
char SJD2SGetchar(double value,int posn)
{
	SJTryRound(value);
	return (char)('0'+(int(floor(value*pow(10.0,-posn)))-10*int(floor(floor(value*pow(10.0,-posn))/10.0))));
}

/*
 * ��ӡ���֣�ֱ��lastposλ��
 *
 * ����һλ��Ч������lastpos����ȫΪ0
 * lastpos����С��0,�����ӡ������������
 * ע���ɿ�����printf("% .*f",-lastpos,value)ʵ��
*/
CString SJD2SByPosition(double value,int lastpos)
{
	SJTryRound(value);
	bool vsign = (value >= 0);
	value = vsign?value: -value;
	CString sv;
	
	sv.Format("%d",int(floor(value)));
	
	if (lastpos < 0)
		sv += '.';
			
	while(lastpos++ < 0)
	{
		value = (value-int(value))*10.0;
		sv +=  char(int(value)+'0');
	}
	return vsign?sv:("-"+sv);
}
/*
 *	��ӡ���֣�ֱ����nλ��ʾ����
 *
 *  	����������λ������n���ӡ���е���������
 */
CString SJD2SByLength(double value,int n)
{
	SJTryRound(value);
	CString sv;
	sv.Format("% d",int(value));
	n -= (sv.GetLength()-1);
	
	if (n > 0)
		sv = SJD2SByPosition(value,-n);
	return sv;
}
/*
 *	�õ���1����0���ֺ��nλ�����֣��ǵ�n����Ч����)
 */
char SJD2SGetcharV(double value,int n)
{
	SJTryRound(value);
	if (n <= 0) return '0';
	if (value < 0) value = -value;
	
	if(value < pow(10,-n))
		return '0';

	while(value >= 1.0)
		value /= 10.0;
	while(value < 0.1)
		value *= 10.0;

	return SJD2SGetchar(value,-n);
}


/*
 *	�ɲο��ַ�ochar(omittable char)��ֵ��
 *	���һ������λ���ַ�vchar(valid char)��ֵ�õ���λֵ��0��1��
 *
 *	selΪѡ��ĩβֵ�����������1��2��5��,
 * 	����ʹ����4��5��+ż������
 */

int SJD2SGetcarryI(int vchar,int ochar,int sel)
{
	int tmp = ((ochar > 5) || ((ochar == 5)&&((vchar%2) == 1)))?1:0; //4��5��+ż������
	
	if (sel == 5)
	{	//5 case
		return (vchar+tmp) - ((vchar+tmp)%5) - vchar;
	}
	if (sel == 1)
	{	//1 case
		return (ochar >= 5)? 1:0;//4��5��
	}
	if (sel == 2)
	{	//2 case
		tmp = vchar+tmp;
		if (tmp%2 == 0)	return tmp-vchar;
		
		switch(tmp) {
		case 1:
			return 2-vchar;
		case 3:
		case 5:
			return 4-vchar;
		case 7:
		case 9:
			return 8-vchar;
		default:
			return tmp-vchar;
		}
	}
	return 0;	
}
int SJD2SGetcarryC(char vchar,char ochar,int sel)
{
	return SJD2SGetcarryI(vchar-'0',ochar-'0',sel);
}
#endif
