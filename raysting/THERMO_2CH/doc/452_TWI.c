//�����ӳ����ṩ���������������ο�
// *******************************************************************
// Website:  http://wch.cn
// Email:    tech@wch.cn
// Author:   TECH  2008.11.25
// *******************************************************************

//�˳����������202714085���ŵ�CH452����ʹ��
//ATMEGA16 PC0   -------    CH452 SCL
//ATMEGA16 PC2   -------    CH452 INT#
//ATMEGA16 PC1   -------    CH452 SDA
//����CH452�ӵ�ַ1

//ICCAVR�±���

// **************************************
// ����ͷ�ļ�
// **************************************
#include <iom16v.h>


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

#define	CH452_INT_IN		{ PORTC |= (1<<PC2); DDRC &= ~(1<<PC2); }
#define mINT				( PINC&(1<<PC2) )		//�жϽŽ�INT#

#define	CH452_I2C_ADDR0		0x40			// CH452��ʱ���ַλ����I2C��ַ�޹�
#define	CH452_I2C_ADDR1		0x60			// CH452��ʱ���ַλ����I2C��ַ�޹�
#define	CH452_I2C_MASK		0x3E			// CH452��2�߽ӿڸ��ֽ���������



// **************************************
// ���Ͷ���
// **************************************
typedef unsigned char UINT8;
typedef unsigned short UINT16;

// **************************************
// CH452��ȫ�ֱ���
// **************************************
UINT8 mADDR;//I2C��ַ



// **************************************
// ��ʱ����,��׼
// **************************************
void mDelaymS( unsigned char ms )   
{
    unsigned short i;
    while ( ms -- )
    {
        for( i = 0; i != 1000; i++ );
    }
}


// **************************************
// CH452��ʼ��
// **************************************
void CH452_I2C_Init( void )
{
 	 PORTC |= (1<<PC1)|(1<<PC0);
	 DDRC |= (1<<PC1)|(1<<PC0);
   	 TWBR = 0x64; 
	 TWSR = (1<<TWPS1)|(1<<TWPS0);
   	 TWCR = ( 1<<TWEN ); //ʹ��TWI
}


// **************************************
// I2C START
// **************************************
void CH452_START( void )
{
    TWCR = ( 1<<TWEN )|(1<<TWSTA )|(1<<TWINT);//�� START
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2C STOP
// **************************************
void CH452_STOP( void ) 
{ 
    TWCR = ( 1<<TWEN )|(1<<TWSTO)|(1<<TWINT); //STOP 
}

// **************************************
// I2C �������ַ
// **************************************
void CH452_Wr_Cmd( UINT8 cmd )//��CH452����
{
 	TWDR = cmd;

	TWCR = ( 1<<TWEN )|(1<<TWINT);
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2C ������
// **************************************
void CH452_Wr_Data( UINT8 bData )//��CH452����
{
	TWDR = bData;
	
	TWCR = ( 1<<TWEN )|(1<<TWINT);
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2CдCH452����
// **************************************
void CH452_Write( UINT16 cmd )
{
     UINT8 try;//���Դ���
	 
	 try = 50;
	 do
	 {
 	   	 CH452_START();
	 
	 	 if( mADDR == 1 )
		 {
   	         CH452_Wr_Cmd((UINT8)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1);    
		 }
		 else
		 {
	         CH452_Wr_Cmd((UINT8)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0);
		 }
		
	 	 if( ( TWSR & 0xF8 ) == 0x18 )//�յ�ACK
	 	 {
	  	  	 CH452_Wr_Data( (UINT8)cmd );
			 CH452_STOP();
			 break;
	 	 }
	 	 
		 CH452_STOP();
		 
		 if( try ) try--;
	 }
	 while( try );
	 
}

// **************************************
// I2C��CH452����
// **************************************
UINT8 CH452_Read( UINT16 read_cmd )		
{
    UINT8 try;//���Դ���
	UINT8 val;
	
	try = 50;
	do
	{
	  	val = 0xFF;
		
   	  	CH452_START();

		if( mADDR == 1 )
		{
   	         CH452_Wr_Cmd((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1|0x01);    
		}
		else
		{
	         CH452_Wr_Cmd((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0|0x01);
		}
		
		if( ( TWSR & 0xF8 ) == 0x40 )//�յ�ACK
   		{
	 	 	TWCR = ( 1<<TWEN )|(1<<TWINT);
			while( !(TWCR & (1<<TWINT)) );
	    	val = TWDR;
			CH452_STOP();
			break;
    	}
		
		CH452_STOP();
		
		if( try ) try--;
 	}
	while( try );
	
	return val;
}

// **************************************
// ������
// **************************************
void main( void )
{
     UINT8 CH452_Key;
	 
 	 mDelaymS(20);
	 CH452_I2C_Init();
	 
	 //ѡ�񱻲�����CH452��ַ  0��1��ѡ
   	 mADDR = 1;//����1��ַCH452
   	 
	 CH452_Write( 0x0700 );//��ACK����
   	 CH452_Write( 0x403 );//����ʾ����
   	 CH452_Write( 0x500 );//ֱ������    
   	 CH452_Write( 0x580 );//BCD����
  
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

        	 CH452_Key = CH452_Read(0x0700);
			 if( CH452_Key != 0xFF )//����ȷ
			 {
			  	 CH452_Key &= 0x3F;

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
}







