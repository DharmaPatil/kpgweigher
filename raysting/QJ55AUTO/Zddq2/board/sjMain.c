#include "sjDefine.h"
#include "sjSerial.h"
#include <stdio.h>

//the MARK of header ender and error and quest and reset
#define  HMARK 0x55	//U
#define  EMARK 0x56 //V
#define  QMARK 0x57 //W
#define  RMARK 0x58 //X
  
//the length of the switch table ,
//which must lower than 3fh
#define  LINIT 32 

//for P1 output
//for P0 output
const unsigned char code tbl[8]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};




//for Init use(regaddr of pc)
const unsigned char code inittbl[LINIT]={
    0x26, 0x27, 0x0F, 0X3E, 0x3F, 0x3C, 0x34, 0x0D, 
	0x0C, 0x0E, 0x2D, 0x14, 0x0B, 0x33, 0x21, 0x32,//for QJ55 ONLY
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
/*
	0x2B, 0x39, 0x38, 0x18, 0x17, 0x1A, 0x43, 0x41, 
	0x08, 0x1B, 0x09, 0x19, 0x2C, 0x26, 0x27, 0x0F, 
	0x3E, 0x3F, 0x3C, 0x3D, 0x0C, 0x23, 0x20, 0x14, 
	0x1F, 0x24, 0x02, 0x33, 0x21, 0x32, 0xff, 0xff
*/

/*********** 05-1-25  8:44 ****************/

//  FKI1,FKI2,FKI3,OKI4,PKTT,FHP9,FHP7,FST2,
//    0x37,0x36,0x35,0x3D,0x33,0x32,0x31,0x30,

//  FHP3,FHN3,FHP4,FHN4,FST1,FHX1,FHX2,FFX1,  
//	0x26,0x27,0x24,0x25,0x22,0x23,0x20,0x21,

//  FHP10,FHP1,FHN1,FHP2,FHN2,FHN5,FHP8,FHN6,
//	0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,

//  FKN1,FKN2,FKN3,FKN4,FKN5,OKN6,FHP6,FHP5
//	0x06,0x07,0x04,0x05,0x02,0x0A,0x00,0x01
/*******************************************
//  FKI1,FKI2,FKI3,OKI4,PKTT,FHP9,FHP7,FST2,
    0x37,0x36,0x35,0x3D,0x33,0x17,0x11,0x30,

//  FHP3,FHN3,FHP4,FHN4,FST1,FHX1,FHX2,FFX1,  
	0x24,0x27,0x01,0x25,0x22,0x23,0x20,0x21,

//  FHP10,FHP1,FHN1,FHP2,FHN2,FHN5,FHP8,FHN6,
	0x32,0x14,0x15,0x26,0x13,0x12,0x32,0x10,

//  FKN1,FKN2,FKN3,FKN4,FKN5,OKN6,FHP6,FHP5
	0x06,0x07,0x04,0x05,0x02,0x0A,0x31,0x00
********************************************/
};

//when modification is applied,
//no address should be modified ,just set the mcu addr= 256,
//and add new item in mcuaddr
//C? IT IS SO EASY ,
//BUT ,A NEW REGISTER MODEL WILL BE APPLIED IN THE FUTURE.
/*	    board allocation (F,O)=P0*8+P1
		48,49	50,51	52,53	54,55	57,56	59,58	61,60	63,62	//HPo,HNo,??,HP0,H8,H7,STo,STi
		33,32	35,34	37,36	39,38	41,40	43,42	45,44	47,46   //HN4,HP4,HN3,HP3,HN6,HP6,HN5,HP5
		17,16	19,18	21,20	23,22	25,24	27,26	29,28	31,30	//KN4,KN3,KN2,KN1,H9,FX1,KTT,KN5
		0,1		2,3		4,5		6,7		9,8		11,10	13,12	15,14 	//H2,H1,HX2,HX1,KI4,KI3,KI2,KI1
const unsigned char pcaddr[] ={
		0x16,0x14,0x26,0x24,0x01,0x00,0x31,0x11,0x32,0x17,//FHP 0-9 (pc addr)
		0x1f,0x1d,0x2f,0x2d,0x08,0x09,0x38,0x18,0x3b,0x1e,//OHP 10-19 (pc addr)
		0x15,0x13,0x27,0x25,0x12,0x10,					  //FHN 20-25  (pc addr)
		0x1c,0x1a,0x2e,0x2c,0x1b,0x19,					  //OHN 26-31  (pc addr)
		0x06,0x07,0x04,0x05,0x02,0x03,					  //FKN 32-37  (pc addr)
		0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,					  //OKN 38-42  (pc addr)
		0x37,0x36,0x35,0x34,							  //FKI 44-47	   (pc addr)
		0x3e,0x3f,0x3c,0x3d,							  //OKI 48-51	   (pc addr)
		0x23,0x20,0x21,0x33,0x22,0x30,					  //FHX1,FHX2,FFX1,PKTT,FSTI,FSTO 52-57  (pc addr)
		0x2a,0x29,0x28,0x3a,0x2b,0x39,					  //OHX1,OHX2,OFX1,NKTT,OSTI,OSTO, 58-63 (pc addr)
		0x41,0x40,0x43,0x42};							  //FHPO,OHP0,FHNO,OHNO  (pc addr)
const unsigned char mcuaddr[] ={
	     54,  2	,  0,	39,	 35,  47,  43,	59,  57, 25,   //FHP 0-9 (pc addr)
		 55,  3 ,  1,	38,	 34,  46,  42,	58,	 56, 24,   //OHP 10-19 (pc addr)
  		 255, 255,  37,  33,  45,   41,					   //FHN 20-25  (pc addr)
		 255, 255,  36,  32,  44,   40,                    //OHN 26-31  (pc addr)
		 23,   21,  19,  17,  31,  255, 		   		   //FKN 32-37  (pc addr) ?(54)
		 22,   20,  18,  16,  30,  255,	                   //OKN 38-42  (pc addr) ?(55)
		 15,   13,  11,  9,								   //FKI 44-47	   (pc addr)
		 14,   12,  10,  8,								   //OKI 48-51	   (pc addr)
		 6,   4,   27,  29,  63,  61, 					   //FHX1,FHX2,FFX1,PKTT,FSTI,FSTO 52-57  (pc addr)
		 7,   5,   26,  28,  62,  60,					   //OHX1,OHX2,OFX1,NKTT,OSTI,OSTO, 58-63 (pc addr)
		 48,  49,  50,  51};							   //FHPO,OHP0,FHNO,OHNO  (pc addr)

*/
const unsigned char code lut[]={
/*****05-1-25 8:44****
47,35,31,255,19,17,23,21,  //0-7
34,46,255,30,16,18,20,22,  //8-f
41,59,45,255,2,255,54,25,  //10-17
58,40,255,44,255,3,24,55,  //18-1f
4,27,63,6,39,33,0,37,      //20-27
26,5,7,62,32,38,36,1,      //28-2f
61,43,57,29,9,11,13,15,    //30-37
42,60,28,56,10,8,14,12,    //38-3f 
49,48,51,50				   //40-43
**********************/
/*****05-1-25 8:44****/
43,47,31,255,19,17,23,21,  //0-7  
46,42,255,30,16,18,20,22,  //8-f
41,57,45,53,0,255,2,54,  //10-17
56,40,52,44,255,1,55,3,  //18-1f
4,27,63,6,35,33,39,37,      //20-27
26,5,7,62,32,34,36,38,      //28-2f
61,59,25,29,9,11,13,15,    //30-37
58,60,28,24,10,8,14,12,    //38-3f 
49,48,51,50				   //40-43
/**********************/
};

extern void turn_rx_on( void );
extern void turn_rx_off( void );
extern void flush_input_buffer( void );
extern void ioputchar(char ch);
extern char iogetchar();
extern char kbhit( void );
extern void init_uart( void );
unsigned char delayc;

//delay loop
void swiDelay(unsigned char dl1,unsigned char dl2)
{
	unsigned char i,j;
	for(i=0;i<dl1;i++)
		for(j=0;j<dl2;j++)
			;
}


void swiAction(unsigned char SwiID)
{
	unsigned char exID;

	if(SwiID >= sizeof(lut))
		return;

	exID = lut[SwiID];
	if(exID == 255)
		return;

	P1 = tbl[(exID&7)];
	P0 = tbl[(exID>>3)];
	swiDelay(0x1f,0xff);

	P1 = 0xff;
	P0 = 0xff;
	swiDelay(0x03,0xff);
}
void swiReset()
{
	unsigned char i;
	for(i=0;i<LINIT;i++)
	{
		swiAction(inittbl[i]);
	}
}

/*****************************************************
Interface Routine for I/O analog serial port (UART.C)
(added by songj)
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.

******************************************************/
unsigned char get_rx_pin_status()
{
	return asp_rx;
}
void set_tx_pin_high()
{
	asp_tx = 1;
}
void set_tx_pin_low()
{
	asp_tx = 0;
}
void idle()
{

}

//end interface routine
void main()
{
	unsigned char temp;
	unsigned char sid;//id of serial to send 
	long i=1000;
	IE = 0;//close int

	sjSerialInit();
	

	//set serial port parameter (clock 11.0592M)
	//9600 baut rate 8 data non parity and 1 stop.
	SCON = 0x70;
	PCON = 0x00;
	//timer count
	TH1 = 0xfd;
	//use timer 1 to be serial
	//use timer 0 to be analog I/O
	TMOD = 0x22;
	TR1 = 1;

	init_uart();
	turn_rx_on();//enable I/O serial
	flush_input_buffer();
	IE = 0x92;//enable serial int and timer0 interrupt//IE=90
	sid = 0;
	
	asp_tx = 1;//set the asp_tx to be output pin
	if(asp_rx);//set the asp_rx to be input pin

//	sjSerialSendByte('?');
	swiReset();
	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
	swiDelay(0x0f,0xff);
	
	while(1)
	{

		if(sjSerialIsDataWaiting())
		{
			temp = sjSerialWaitForOneByte();
			if(temp == HMARK)
			{
				temp = sjSerialWaitForOneByte();		
				if(temp != HMARK)
				{
					sid = temp;
					continue;
				}
			}
			if(sid == 's')//send to switch
			{
				if(temp == HMARK)
				{
					while(1)
					{
						temp = sjSerialWaitForOneByte();
						if(temp == EMARK)
							break;
						if(temp == RMARK)
						{
							swiReset();
							continue;
						}
						if(temp == QMARK)
							continue;
						swiAction(temp);
					}
					sjSerialSendByte(HMARK);
					temp = P2;
					sjSerialSendByte(temp);
					temp = P3;
/*
					temp = 0;
					if(P3^4)
						temp |= (0x10);
					if(P3^5)
						temp |= (0x20);
*/
					sjSerialSendByte(temp);
					sjSerialSendByte(EMARK);
				}
			}
			if(sid == 'n')//send to navmeter
			{
				ioputchar(temp);
			}
			if(sid == 't')
			{
				P0=sjSerialWaitForOneByte();
				P1=sjSerialWaitForOneByte();
				sjSerialSendByte(EMARK);
			}
		}
		if(kbhit())
		{
			temp = iogetchar();
			sjSerialSendByte(temp);
		}
	}
}



