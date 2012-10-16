//�����ӳ����ṩһ�ֲ��������������ο�
// *******************************************************************
// Website:  http://wch.cn
// Email:    tech@wch.cn
// Author:   TECH  2008.10.14
// *******************************************************************

//ATMEGA16 PB7   -------    CH452 SCL
//ATMEGA16 PB6   -------    CH452 INT#
//ATMEGA16 PB5   -------    CH452 SDA
//CH452����ӵ�ַ1

//ICCAVR�±���

// *******************
// ����ͷ�ļ�
// *******************
#include <iom16v.h>
#include <macros.h>


// *******************
// CH452�Ķ���
// *******************
#define CH452_DIG0      0x0800                  //�����λ0��ʾ
#define CH452_DIG1      0x0900		            //�����λ1��ʾ 
#define CH452_DIG2      0x0a00		            //�����λ2��ʾ
#define CH452_DIG3      0x0b00		            //�����λ3��ʾ
#define CH452_DIG4      0x0c00		            //�����λ4��ʾ
#define CH452_DIG5      0x0d00	                //�����λ5��ʾ	
#define CH452_DIG6      0x0e00	                //�����λ6��ʾ	
#define CH452_DIG7      0x0f00		            //�����λ7��ʾ

#define	CH452_SDA_OUT   	{ DDRB |= (1<<PB5); }
#define	CH452_SDA_IN		{ PORTB |= (1<<PB5); DDRB &= ~(1<<PB5); }
#define	CH452_SDA_SET		PORTB |= (1<<PB5)
#define	CH452_SDA_CLR		PORTB &= ~(1<<PB5)
#define	CH452_SDA			( PINB & (1<<PB5) )
#define	CH452_SCL_OUT		{ PORTB |= (1<<PB7); DDRB |= (1<<PB7); }
#define	CH452_SCL_SET		PORTB |= (1<<PB7)
#define	CH452_SCL_CLR		PORTB &= ~(1<<PB7)
#define	CH452_INT_IN		{ PORTB |= (1<<PB6); DDRB &= ~(1<<PB6); }
#define mINT				( PINB&(1<<PB6) )		//�жϽŽ�DOUT
#define DELAY				mDelayuS(1)

#define	CH452_I2C_ADDR0		0x40			// CH452��ADDR=0ʱ�ĵ�ַ
#define	CH452_I2C_ADDR1		0x60			// CH452��ADDR=1ʱ�ĵ�ַ,Ĭ��ֵ
#define	CH452_I2C_MASK		0x3E			// CH452��2�߽ӿڸ��ֽ���������

// *******************
// ���Ͷ���
// *******************
typedef unsigned char UINT8;
typedef unsigned short UINT16;

// **************************************
// CH452��ȫ�ֱ���
// **************************************
UINT8 mADDR;//I2C��ַ


// *******************
// ��ʱuS
// *******************
void mDelayuS( UINT8 n ) 
{	
   while( --n ) 
   {
       NOP();
	   NOP();
   }
}

// *******************
// ��ʱmS
// *******************
void mDelaymS( UINT8 delay )
{ 	
   UINT8 ms;
   UINT8 sm;
   UINT8 c;
	
   for( ms = delay; ms != 0; ms-- )
   { 	
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 240; sm != 0; sm-- ) c += 3;  							 
   }	
}

// **************************************
// I2C START
// **************************************									   
void CH452_I2c_Start( void )
{
	CH452_SDA_SET; /*������ʼ�����������ź�*/
	CH452_SDA_OUT;  /* ����SDAΪ������� */
 	DELAY;
	
	CH452_SCL_SET;/* ����SCLΪ������� */
	DELAY;
	
	CH452_SDA_CLR;   /*������ʼ�ź�*/
	DELAY;    
	  
	CH452_SCL_CLR;  /*ǯסI2C���ߣ�׼�����ͻ�������� */
	DELAY;
}

// **************************************
// I2C STOP
// **************************************	
void CH452_I2c_Stop( void )
{
	CH452_SDA_CLR; 
	CH452_SDA_OUT; /* ����SDAΪ������� */
	DELAY;
	
	CH452_SCL_SET; 
	DELAY;
	
	CH452_SDA_SET; /*����I2C���߽����ź�*/
	DELAY;   
	
	CH452_SDA_IN; /* ����SDAΪ���뷽�� */
	DELAY; 
}

// **************************************
// I2C дһ���ֽ�
// **************************************
void CH452_I2c_WrByte( UINT8 c )	//дһ���ֽ�����
{
	UINT8 i;
	
	CH452_SDA_SET;
	CH452_SDA_OUT; /* ����SDAΪ������� */
	DELAY;
	
	for( i = 0; i != 8; i++ ) 
	{
		if( c & 0x80 ) { CH452_SDA_SET; }
		else { CH452_SDA_CLR; }
		DELAY;
		
		CH452_SCL_SET;
		
		c <<= 1;
		DELAY;
		
		CH452_SCL_CLR;
	}
	
	CH452_SDA_IN;   /* ����SDAΪ���뷽�� */
	DELAY;
	
	CH452_SCL_SET;  
	DELAY;
	
	CH452_SCL_CLR;
	DELAY;
}

// **************************************
// I2C ��һ���ֽ�
// **************************************
UINT8  CH452_I2c_RdByte( void )		//��һ���ֽ�����
{
	UINT8 c, i;
	
	CH452_SDA_IN;   /* ����SDAΪ���뷽�� */
	
	c = 0;
	for( i = 0; i != 8; i++ )  // ����8λ����
	{
		CH452_SCL_SET;
		DELAY;
		
		c <<= 1;
		if( CH452_SDA ) c++;  // ����1λ
		
		CH452_SCL_CLR;
		DELAY;
	}
	
	CH452_SDA_SET;
	CH452_SDA_OUT;
	
	CH452_SCL_SET;  // ������ЧӦ��
	DELAY;
	
	CH452_SCL_CLR;
	DELAY;
	
	return(c);
}

// **************************************
// CH452д����
// **************************************
void CH452_Write( UINT16 command )	//д����
{	
     CH452_I2c_Start();               /*��������*/
	 
	 if( mADDR == 0 )
	 {
   	     CH452_I2c_WrByte((UINT8)(command>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0);  // CH452��ADDR=0ʱ
	 }
     else
     {
   	     CH452_I2c_WrByte((UINT8)(command>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1);  // CH452��ADDR=1ʱ(Ĭ��)
	 }
	 
   	 CH452_I2c_WrByte( (UINT8)command );               /*��������*/
	 
  	 CH452_I2c_Stop();                 /*��������*/
}

// **************************************
// CH452������
// **************************************
UINT8 CH452_Read( UINT16 read_cmd )		
{
	UINT8 val;

   	CH452_I2c_Start();/*��������*/

	if( mADDR == 1 )
	{
   	     CH452_I2c_WrByte((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1|0x01);    // ��������CH452����,��ADDR=0ʱ,���޸�ΪCH452_I2C_ADDR0
	}
	else
	{
	     CH452_I2c_WrByte((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0|0x01);
	}

   	val = CH452_I2c_RdByte();      /*��ȡ����*/

	CH452_I2c_Stop();                /*��������*/
 
	return val;
}

// *******************
// CH452�ĳ�ʼ������
// *******************
void CH452_Init( void )
{
	 CH452_SCL_OUT;
	 CH452_SDA_SET;
	 CH452_SDA_OUT;
	 CH452_INT_IN;
}



// *******************
// ������
// *******************
void main( void )
{
 	 UINT8 CH452_Key;
	 
 	 mDelaymS(20);
	 CH452_Init();
	 
	 //ѡ�񱻲�����CH452��ַ  0��1��ѡ
   	 mADDR = 1;//����1��ַCH452
   	 
   	 CH452_Write(0x403);//����ʾ����
   	 CH452_Write(0x500);//ֱ������    
   	 CH452_Write(0x580);//BCD����
  
   	 //��ʾ 1,2,3,4,5,6,7,8
   	 CH452_Write( CH452_DIG0 | 1 ); 
   	 CH452_Write( CH452_DIG1 | 2 ); 
   	 CH452_Write( CH452_DIG2 | 3 ); 
   	 CH452_Write( CH452_DIG3 | 4 );
   	 CH452_Write( CH452_DIG4 | 5 );
   	 CH452_Write( CH452_DIG5 | 6 );
   	 CH452_Write( CH452_DIG6 | 7 ); 
   	 CH452_Write( CH452_DIG7 | 8 );

   	 //������
   	 while(1)
   	 {
        	 while( mINT );//�ȴ��ж�

        	 CH452_Key = CH452_Read(0x0700)&0x3F;

        	 CH452_Write(0x300);//������λ
        	 CH452_Write(0x300);
   
        	 CH452_Write(CH452_DIG1|(CH452_Key/10));
        	 CH452_Write(CH452_DIG0|(CH452_Key%10));
			 
			 while(1)//�жϰ����Ƿ��ͷ�
			 {
			  	  CH452_Key = CH452_Read(0x0700);
				  if( CH452_Key & 0x40 )
				  {
				   	  CH452_Write( 0x0603 );
				  }
				  else
				  {
				   	  CH452_Write( 0x0600 );
					  break;
				  }
			 }
   	 }
}

