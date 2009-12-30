    #include <stc51.h>
	#include <stdio.h>
	#include <intrins.h>


#define F_TM F0

#define TIMER0_ENABLE  TL0=TH0; TR0=1;
#define TIMER0_DISABLE TR0=0;

sbit ACC0=   ACC^0;
sbit ACC1=   ACC^1;
sbit ACC2=   ACC^2;
sbit ACC3=   ACC^3;
sbit ACC4=   ACC^4;
sbit ACC5=   ACC^5;
sbit ACC6=   ACC^6;
sbit ACC7=   ACC^7;



	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long

    sbit dat= P0^1;   //����������
    sbit clk= P0^0;   //����ʱ�Ӷ�
//	sbit clr= P3^5;   //�����
    sbit sclk= P1^0;  //����ʱ�Ӷ�
    sbit sdata= P1^1;   //����������
    sbit drdy= P1^2;    //�߼������
	sbit a0= P1^3;      //��ַ�����
    sbit rfs= P1^4;    //ͬ�����ܶ�
    sbit tfs= P1^5;    //ͬ�������

    sbit jdq1= P1^7;   //�̵���XI
	sbit jdq2= P1^6;   //�̵���X2

//	sbit CH0= P2^6;	   //
	sbit CH1= P2^7;    //
    sbit CH2 =P2^6;
   // sbit BT_REC =P2^5;

	sbit key_tl=  P0^2; //����
    sbit key_xz=  P0^3; //У׼

   sbit  LED=P0^0;
 
    #define N 47 //Nֵ�ɸ���ʵ���������


    ulong dd;  
   	ulong de;           //��������ȫ�ֱ�������
	ulong xz;           //У׼���ݼĴ�
    uchar quantifier;   //����ѡ�� 0��2V ;1��200mV;2��20mv;3������2V;4������200mv;5������20mv
    uchar CH;           //ͨ��ѡ��
    uchar code rrc[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      
                      0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,}; //LED��ʾ
    uint code tune[3]={0,0,980};     //��������
   // float code diagraph[4]={1,1,1};
    ulong code control[3]={0x09C51c,0x09C534,0x09Cb04};//{0xBE51c,0xBE534,0xBE504}; 



    ulong code Sector[2][4]={
	                       {0x8000,0x8001,0x8002,0x8003},
			               {0x8200,0x8201,0x8202,0x8203},
						   }   ;




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

 /*********************��ʱ***************/
void delay(unsigned int n) 
{
  unsigned int i;
  for(i=0; i<n; i++) {;}
}
     void d164(uchar n)   //74164������λһ����
        {
         uchar i=0;
         uchar c=rrc[n];
         do{  
             if(c&0x01)     
                   	dat=1;
                    else
					dat=0;
		     c>>=1;        //����tabel�еĵ�N���������ƶ�һλ
			 _nop_(); _nop_();
             clk=0;            //�½��ؽ�����д��164������ʱ�Ӷ�ֹͣ�������͵ȴ���һ��д��
             _nop_(); _nop_();
			 clk=1;
			  _nop_(); _nop_();
             clk=0;
			  _nop_(); _nop_();
             i++;
            }
        while(i<8);
        }
void send_char_com(uchar ch) 
{  
SBUF=ch; 
 
while (TI== 0 );  
TI= 0 ;  
}  
     display()
        {
		  uchar d1,d2,d3,d4,d5,d6,d7;
	      ulong de1;
		  de1=de;
          d1=de1/0x989680;
  		  d2=(de1%0x989680)/0xf4240;
		  d3=((de1%0xf989680)%0xf4240)/0x186a0;
		  d4=(((de1%0xf989680)%0xf4240)%0x186a0)/0x2710;
		  d5=((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)/0x3e8;
		  d6=(((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)/0x64;
          d7=((((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)%0x64)/10;
      
		   d164(d1+20);
   
        d164(d2);
        d164(d3);
        d164(d4);
        d164(d5);
        d164(d6);
        d164(d7);
		  d164(0);
     
}
//********************************
 read7710()
	   {
	    uint j;
        sclk=0;
		a0=1;
		tfs=1;
		rfs=1;
		sdata=1;
       
		 
	RD:	
		    de=0;
 		    if(drdy==0)
		       { rfs=0 ;
                 for(j=0;j<0x18;j++)
			     {sclk=0;
                 _nop_();
				  _nop_();
                 _nop_();
                 _nop_();
				  sclk=1;
				  _nop_();
                  _nop_();
                 _nop_();
                 _nop_();
			      de=de|sdata;
				  sclk=0;
			      de<<=1;
				  
				    if (drdy==1)
					 j=0x18;
			     }
			 
               }
			else 
              goto RD;
    
 
	 }
//************************
   void yanshi(void)
	   {
	    ulong i;
		for(i=0;i<1;i++)
		  
		  ;
        }
	void write(ulong j)
	   {  
        uint i;
        sclk=0;
     	tfs=0;
		rfs=1;
	    a0=0;
		for(i=0;i<25;i++)
		 {
		  sclk=0;
		  _nop_();
	     _nop_();
   	      sclk=1;
		    sdata=j&1;
		  _nop_();
		  _nop_();
		  j=(j>>=1);
		  }
         tfs=1;       
	  }

/*****************���ݴ���***********************/
 with()

{

   char count,i,j;
   ulong value_buf[N];
   ulong  sum=0,temp;
   for  (count=0;count<N;count++)
   {
	   read7710();
      value_buf[count] = de;
      delay(2);
   }
   for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
      }
   }
   for(count=1;count<N-1;count++)
     {  sum += value_buf[count];	 }
  de=sum/(N-2);
}


/************************************/	   
     quantifier_with()//,uint dd )     
      {
	    
	    float de2;
		 if(de>16777216)
	    { de=de-16777216;
        }
	   	else
        { de=16777216-de;//+down;
        }
          de2=xz*0.0000001;
		  de2=de/de2;

          de=de2;
	   }



/***************��·��λ*********************/
WLCL()
{
jdq2=0;
jdq1=1;
delay(60000); delay(60000);
write(0x09C501);
delay(60000);delay(60000);
jdq1=0;
jdq2=1;
delay(60000);delay(60000);
//write(control[quantifier]);
jdq2=0;
}
/******************��·У׼*******************/
DLXZ()
{
 with(); //���ݲɼ�����
 if(de>16777216)
	    { de=de-16777216;
        }
	   	else
        { de=16777216-de;
        }
     SectorErase(Sector[0][0]); 
     byte_write(Sector[0][0],de&0xff); 
     byte_write(Sector[0][1],de>>8&0xff);
     byte_write(Sector[0][2],de>>16&0xff);
     byte_write(Sector[0][3],de>>24&0xff);
	  display();
 xz=de;

}

/******************�������********************/
key()
{
 if(key_tl==0)
 {
  WLCL() ;
 

 }
   if(key_xz==0)
 {
  DLXZ() ;
 
 }

}


/***********����һ���ַ�***********************
void PSendChar(unsigned char inch)
{

unsigned char ii;

ii=0;

F_TM=0;
BT_SND=0; //start bit
TIMER0_ENABLE; //����
while(!F_TM);

while(ii<8)
{
if(inch&1)
{
BT_SND=1;
}
else
{
BT_SND=0;
}
F_TM=0;
while(!F_TM);
ii++;
inch>>=1;
}
BT_SND=1;
F_TM=0;
while(!F_TM);

TIMER0_DISABLE; //ֹͣtimer
}

/************�ô������ⷢ4���ֽ�**************/
void ComOutStr()
{
uchar  i,xyw  ;
ulong dT;
uchar  xdata da[4];

  dT=dd;
  da[3] =  dT&0xff; 
  da[2] =  dT>>8&0xff;
  da[1] =  dT>>16&0xff;
  da[0] =  dT>>24&0xff; 

  xyw=da[0]+da[1]+da[2]+da[3];	
  

  	
 SBUF =0xaa; //����ַ�
while(!TI); //������ж��ַ��Ƿ���
TI = 0;	
	 
  for(i=0;i<4;i++) //�ж��Ƿ����ַ�����β��
{
SBUF =da[i]; //����ַ�
while(!TI); //������ж��ַ��Ƿ���
TI = 0; //��TI

delay(100);
 

}
 SBUF =xyw; //����ַ�
while(!TI); //������ж��ַ��Ƿ���
TI = 0;


}

/********************************************
void INT_TO()
{
TMOD=0x22; /*��ʱ��1Ϊ����ģʽ2(8λ�Զ���װ)��0Ϊģʽ2(8λ�Զ���װ) 
PCON=0x80;

TR0=0; //�ڷ��ͻ���ղſ�ʼʹ��
TF0=0;
TH0=(256-96); //9600bps ���� 1000000/9600=104.167΢�� ִ�е�timer��
              //104.167*11.0592/12= 96
TL0=TH0;
ET0=1;
EA=1;
 }
/********************************
void IntTimer0() interrupt 1	  
{
F_TM=1;
}
/***************************************************************/
#define BUS_IDLE	0	//searching for sync
#define BUS_SYNCED	1	//waiting for data filling
#define BUS_SENDING	2	//is sending out data
#define HEAD_MARK	0xcc00	//header mark
#define HEAD_MASK	0xff00	//header mark
#define CMD_READ	0x01	//read data
uint busbuf = 0;
uchar busstate = BUS_IDLE;
uchar obit_cnt; //���bit ����
ulong dT;
sbit busIbit = P2^7;	//INPUT
sbit busObit = P2^4;	//OUTPUT
sbit busCbit = P2^5;	//CLOCK

uchar  da[5];
void int_0() interrupt 0 using 3
{   
	if(busstate == BUS_IDLE)
	{
		busbuf = (busbuf<<1);
		if(busIbit)
		   busbuf |= 0x0001;
		else
		   busbuf &= 0xfffe;
		if((busbuf & HEAD_MASK) == HEAD_MARK) //valid frame is got
		{
			if( (uchar)busbuf == CMD_READ)
			{
			    da[3] =  dd&0xff; 
				da[2] =  dd>>8&0xff;
				da[1] =  dd>>16&0xff;
				da[0] =  dd>>24&0xff; 
				da[4]=da[0]+da[1]+da[2]+da[3];
				obit_cnt = 0;
				busstate = BUS_SENDING;
			}
		}
		return;
	}
	if(busstate == BUS_SENDING)
	{
		if(obit_cnt < 40)
		{
			if(da[(obit_cnt >> 3)] & 0x80)
				busObit = 1;
			else
				busObit = 0;
			da[(obit_cnt >> 3)] = da[(obit_cnt >> 3)] << 1;
			obit_cnt = obit_cnt+1;
		}else{
			obit_cnt = busObit; //set Obit to input
			obit_cnt = 0;
			busstate = BUS_IDLE;
		}
	}
	
}
 
/***********************�ж����ʼ��****************************/
 void INIT_W1(void)
{
    EX0=1;              // INT0�жϿ�
	IT0=1;              // INT0�͵�ƽ����(Ϊ1��Ϊ�½��ش���)
	EA=1; 
}
//*******************************************/
main( )
     {
	 
   		uchar sj1,sj2;
		busstate = BUS_IDLE;
        jdq2=1;jdq1=0;
		delay(20000);
		xz=byte_read(Sector[0][3])*16777216+byte_read(Sector[0][2])*65536+byte_read(Sector[0][1])*256+byte_read(Sector[0][0]);
        jdq1=0;jdq2=0;
		P0=0XFF;
       	de=0; 
        quantifier=2;
        display();    
    	write(control[quantifier]);
		WLCL();	//��·��λ
    SCON = 0x50; //���ڷ�ʽ1,�������
    TMOD = 0x20; //��ʱ��1��ʱ��ʽ2
    TH1 = 0xe8;
    TL1 = 0xe8;
    PCON = 0x80; //�����ʼӱ�����,SMODλ
    TI = 1;
    TR1 = 1; //������ʱ��
	INIT_W1();
 //    INT_TO(); 
 
   do{
      	
        with(); //���ݲɼ����� 
        quantifier_with(); //���ݵ���
        display(); //��ʾ	   

	  	 dd=de;

		//ComOutStr();
		key();

        }
 	  while(1);
      }
     
