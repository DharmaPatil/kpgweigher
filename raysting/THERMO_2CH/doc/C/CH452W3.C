/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2005.4
 CH452 ��2�߽ӿ��ӳ���
****************************
 CH452��2�߽ӿڣ����������ж�Ϊ2��I/O���ţ��������ж�Ϊ3��I/O������I2C/IICʱ��
 ����Ƶ�ʵ���24MHz��MCS51��Ϊ�˽�Լ����ʱ�䣬�����ʵ�����SCL/SDA֮�����ʱ
*************************************************************************** */
#include	"PIN.H"			// �޸ĸ��ļ�����ӦӲ������/��Ƭ���ͺŵ�
#include	"CH452CMD.H"	// CH452��������

void CH452_I2c_Start(void)  // ������ʼ
{
	DISABLE_KEY_INTERRUPT;  //��ֹ�����ж�,��ֹ��ʼʱ��CH452�ж϶������жϷ�������е�START
	CH452_SDA_SET;   /*������ʼ�����������ź�*/
	CH452_SDA_D_OUT;   /* ����SDAΪ������� */
	CH452_SCL_SET;
	CH452_SCL_D_OUT;   /* ����SCLΪ������� */
	DELAY_1US;
	CH452_SDA_CLR;   /*������ʼ�ź�*/
	DELAY_1US;      
	CH452_SCL_CLR;   /*ǯסI2C���ߣ�׼�����ͻ�������� */
	DELAY_1US;
}

void CH452_I2c_Stop(void)  // ��������
{
	CH452_SDA_CLR;
	CH452_SDA_D_OUT;   /* ����SDAΪ������� */
	DELAY_1US;
	CH452_SCL_SET;
	DELAY_1US;
	CH452_SDA_SET;  /*����I2C���߽����ź�*/
	DELAY_1US;
	CH452_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	DELAY_1US;
  	ENABLE_KEY_INTERRUPT;
}

void CH452_I2c_WrByte(unsigned char dat)	//дһ���ֽ�����
{
	unsigned char i;
	CH452_SDA_D_OUT;   /* ����SDAΪ������� */
	for(i=0;i!=8;i++)  // ���8λ����
	{
		if(dat&0x80) {CH452_SDA_SET;}
		else {CH452_SDA_CLR;}
		DELAY_1US;
		CH452_SCL_SET;
		dat<<=1;
		DELAY_1US;
		DELAY_1US;
		CH452_SCL_CLR;
		DELAY_1US;
	}
	CH452_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	CH452_SDA_SET;
	DELAY_1US;
	CH452_SCL_SET;  // ����Ӧ��
	DELAY_1US;
	DELAY_1US;
	CH452_SCL_CLR;
	DELAY_1US;
}

unsigned char  CH452_I2c_RdByte(void)		//��һ���ֽ�����
{
	unsigned char dat,i;
	CH452_SDA_SET;
	CH452_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	dat=0;
	for(i=0;i!=8;i++)  // ����8λ����
	{
		CH452_SCL_SET;
		DELAY_1US;
		DELAY_1US;
		dat<<=1;
		if(CH452_SDA_IN) dat++;  // ����1λ
		CH452_SCL_CLR;
		DELAY_1US;
//		DELAY_1US;
	}
	CH452_SDA_SET;
	DELAY_1US;
	CH452_SCL_SET;  // ������ЧӦ��
	DELAY_1US;
	DELAY_1US;
	CH452_SCL_CLR;
	DELAY_1US;
	return(dat);
}

void CH452_Write(unsigned short cmd)	//д����
{
	CH452_I2c_Start();               /*��������*/
#ifdef	ENABLE_2_CH452				// ��������CH452����
   	CH452_I2c_WrByte((unsigned char)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0);  // CH452��ADDR=0ʱ
#else 
   	CH452_I2c_WrByte((unsigned char)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1);  // CH452��ADDR=1ʱ(Ĭ��)
#endif
   	CH452_I2c_WrByte((unsigned char)cmd);     /*��������*/
  	CH452_I2c_Stop();                 /*��������*/ 
}

unsigned char CH452_Read(void)		//��ȡ����
{
	unsigned char keycode;
   	CH452_I2c_Start();                /*��������*/
   	CH452_I2c_WrByte((unsigned char)(CH452_GET_KEY>>7)&CH452_I2C_MASK|0x01|CH452_I2C_ADDR1);    // ��������CH452����,��ADDR=0ʱ,���޸�ΪCH452_I2C_ADDR0
   	keycode=CH452_I2c_RdByte();      /*��ȡ����*/
	CH452_I2c_Stop();                /*��������*/ 
	return(keycode);
}