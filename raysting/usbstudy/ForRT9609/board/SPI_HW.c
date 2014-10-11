




/* CH374оƬ ���ģ��SPI�������ӵ�Ӳ������� V1.0 */
/* �ṩI/O�ӿ��ӳ���,�ṩ�Ĵ�������д�ӳ��� */

#include	"HAL.H"

#define SPI_S0          0x04
#define SPI_S1          0x08

#define SPIF            0x80                    //SPSTAT.7
#define WCOL            0x40                    //SPSTAT.6

#define SSIG            0x80                    //SPCTL.7
#define SPEN            0x40                    //SPCTL.6
#define DORD            0x20                    //SPCTL.5
#define MSTR            0x10                    //SPCTL.4
#define CPOL            0x08                    //SPCTL.3
#define CPHA            0x04                    //SPCTL.2
#define SPDHH           0x00                    //CPU_CLK/4
#define SPDH            0x01                    //CPU_CLK/16
#define SPDL            0x02                    //CPU_CLK/64
#define SPDLL           0x03                    //CPU_CLK/128

sbit SS             =   P5^5;                   //chip selection



//#define	CH374_SPI_SCS			P55		/* �ٶ�CH374��SCS���� */
//#define	CH374_SPI_SDI			P41		/* �ٶ�CH374��SDI���� */
//#define	CH374_SPI_SDO			P40		/* �ٶ�CH374��SDO���� */
//#define	CH374_SPI_SCK			P43		/* �ٶ�CH374��SCK���� */

void CH374_PORT_INIT( )  /* ����ʹ��ͨ��I/Oģ�Ⲣ�ڶ�дʱ��,���Խ��г�ʼ�� */
{
  	SPSTAT = SPIF | WCOL;                       //??SPI??
    SS = 1;
    SPCTL = SSIG | SPEN | MSTR;                 //??SPI????

}

/************************************************
SPI data exchange

    dat : byte to send
return :
    data from unit
************************************************/
BYTE SpiShift(BYTE dat)
{
    SPDAT = dat;                                //??SPI??
    while (!(SPSTAT & SPIF));                   //??SPI??????
    SPSTAT = SPIF | WCOL;                       //??SPI??
    
    return SPDAT;
}


void	Spi374Start( UINT8 addr, UINT8 cmd )  /* SPI��ʼ */
{
//	CH374_SPI_SCS = 1;
/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ�������,SPI_SDOΪ���뷽�� */
	SS = 0;  /* SPIƬѡ��Ч */
	Spi374OutByte( addr );
	Spi374OutByte( cmd );
}
void  MySpi374Stop()	/* SPI���� */
{ 
	SS = 1; 
}  
#define	Spi374Stop( )	{ SS = 1; }  /* SPI���� */
//void	Spi374Stop( void )  /* SPI���� */
//{
//	CH374_SPI_SCS = 1;	/* SPIƬѡ��Ч */
//}

UINT8	Read374Byte( UINT8 mAddr )  /* ��ָ���Ĵ�����ȡ���� */
{
	UINT8	d;
	Spi374Start( mAddr, CMD_SPI_374READ );
	d = Spi374InByte( );
	Spi374Stop( );
	return( d );
}

void	Write374Byte( UINT8 mAddr, UINT8 mData )  /* ��ָ���Ĵ���д������ */
{
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	Spi374OutByte( mData );
	Spi374Stop( );
}

void	Read374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* ��ָ����ʼ��ַ�������ݿ� */
{
	Spi374Start( mAddr, CMD_SPI_374READ );
	while ( mLen -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
}

void	Write374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* ��ָ����ʼ��ַд�����ݿ� */
{
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	while ( mLen -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
}


