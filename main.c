/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.6 Professional
Automatic Program Generator
� Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
e-mail:office@hpinfotech.com

Project : M64_Bootloader
Version : 0.0
Date    : 2008-11-6
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega64
Program type        : Boot Loader - Size:4096words
Clock frequency     : 14.745600 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/

#include <mega64.h>
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
/******************************************************************************/
//���²����ɸ���ϵͳ���ÿ����Լ��Ķ�������UART_FORMAT�������£�
//0xE: ��У�� 2λֹͣλ         0x6: ��У�� 1λֹͣλ
//0x2E:żУ�� 2λֹͣλ         0x26:żУ�� 1λֹͣλ
//0x3E:��У�� 2λֹͣλ         0x36:��У�� 1λֹͣλ
/******************************************************************************/
#define SPM_PAGESIZE 256                     //M64��һ��FlashҳΪ256�ֽ�(128��) 
#define BAUD 9600                            //�����ʲ���9600bps 
#define UART_FORMAT 0xE                      //UART��ʽ����
#define CRYSTAL 14745600                     //ϵͳʱ��14.745600 MHz�����ڼ���
#define DATA_BUFFER_SIZE SPM_PAGESIZE        //������ջ��������� 

/******************************************************************************/
//���¶����������׸Ķ�
/******************************************************************************/
//����Ͷ���M64�Ĳ��������ò��� 
#define BAUD_SETTING (u8)((u32)CRYSTAL/(16*(u32)BAUD)-1) 
#define BAUD_H (u8)(BAUD_SETTING>>8) 
#define BAUD_L (u8)(BAUD_SETTING)   

//����Xmoden�����ַ� 
#define XMODEM_NUL 0x00 
#define XMODEM_SOH 0x01 
#define XMODEM_STX 0x02 
#define XMODEM_EOT 0x04 
#define XMODEM_ACK 0x06 
#define XMODEM_NAK 0x15 
#define XMODEM_CAN 0x18 
#define XMODEM_EOF 0x1A 
#define XMODEM_RECIEVING_WAIT_CHAR 'C' 

//����Flash�Ա��������
#define FILL_PAGE_CMD 0x01
#define ERASE_CMD 0x03
#define PGM_CMD 0x05 

u8 data[DATA_BUFFER_SIZE]; 
u16 address = 0;           //Ӧ�ó���������ڴӵ�ַ0x0��ʼ������
/******************************************************************************/
//��ʾ��Ϣ
/******************************************************************************/
//����ȫ�ֱ��� 
const char startupString[]="Bootloader now. Type 'd' within 3 seconds to upgrade.\n\r\0"; 
const char AppString[]="Run application now.\n\r\0";

/******************************************************************************/
//����(code=0x03)��д��(code=0x05)һ��Flashҳ 
//Zָ�루ZH�� R31�� ZL��R30������Flash SPM��Ѱַ�Ĵ�����
/******************************************************************************/ 
void boot_page_ew(u16 p_address,u8 code) 
{ 
#asm 
    ldd r30, y+1;
    ldd r31, y+2;
#endasm                    
    SPMCSR = code;          //�Ĵ���SPMCSR��Ϊ������ 
    #asm("spm")
}         
/******************************************************************************/
//���Flash����ҳ�е�һ���֣�code=0x01): R1R0Ϊ��д����֣�R31R30Ϊҳ�ڵ�ַ                                                 
/******************************************************************************/
void boot_page_fill(u16 ui_address,u16 uc_data) 
{ 
#asm 
    LDD R30,Y+2  ; R30 LSB 
    LDD R31,Y+3  ; R31 MSB 
    LD  R26,Y 
    LDD R27,Y+1 
    MOV R0,R26 
    MOV R1,R27 
    MOV R26,R30 
    MOV R27,R31 
#endasm 
    SPMCSR =FILL_PAGE_CMD;      //�Ĵ���SPMCSR��Ϊ������  
#asm    
    mov r30,r26 
    mov r31,r27 
#endasm 
    #asm("spm");                //��ָ��Flashҳ���в�����   
}
         
/******************************************************************************/
//�ȴ�һ��Flashҳ��д���
//SRMCSR[6]: RWW��æ��־������ҳ������ҳд�����ʱ�� RWWSB ��Ӳ����1��
//SRMCSR[0]: ��ҳ������ҳд������SPMEN ����Ϊ1ֱ��������ɡ�
//���SRMCSR[0]��SRMCSR[4]ͬʱ��д��1�����ڽ����ŵ��ĸ�ʱ�������ڵ�SPMָ���
//��ʹ��RWW����
/******************************************************************************/
void wait_page_rw_ok(void) 
{ 
     while(SPMCSR & 0x40) 
     { 
         while(SPMCSR & 0x01); 
         SPMCSR = 0x11;       //�ٴ�ʹ��RWW����
         #asm("spm") 
     } 
} 
/******************************************************************************/
//����һ��Flashҳ����������                                                     
/******************************************************************************/
void write_one_page(void) 
{ 
    u16 i,temp; 
    boot_page_ew(address,ERASE_CMD);            //����һ��Flashҳ 
    wait_page_rw_ok();                          //�ȴ�������� 
    for(i=0;i<SPM_PAGESIZE;i+=2)                //����������Flash����ҳ�� 
    { 
        temp = data[i+1]; 
        boot_page_fill(i,data[i]+(temp<<8));         
    } 
    boot_page_ew(address,PGM_CMD);              //������ҳ����д��һ��Flashҳ 
    wait_page_rw_ok();                          //�ȴ�д����� 
}         
/******************************************************************************/
//��RS232����һ���ֽ�                                                           
/******************************************************************************/
void uart_putchar(u8 c) 
{ 
    while(!(UCSR0A & 0x20)); 
    UDR0 = c; 
} 
/******************************************************************************/
//��RS232����һ���ֽ�                                                           
/******************************************************************************/
int uart_getchar(void) 
{ 
    u8 status,res; 
    if(!(UCSR0A & 0x80)) return -1;        //no data to be received 
    status = UCSR0A; 
    res = UDR0; 
    if (status & 0x1c) return -1;        // If error, return -1 
    return res; 
} 
/******************************************************************************/
//�ȴ���RS232����һ����Ч���ֽ�                                                 
/******************************************************************************/
char uart_waitchar(void) 
{ 
    int c; 
    while((c=uart_getchar())==-1); 
    return (char)c; 
} 
/******************************************************************************/
//����CRC                                                                       
/******************************************************************************/
int calcrc(char *ptr, int count) 
{ 
    int crc = 0; 
    char i; 
     
    while (--count >= 0) 
    { 
        crc = crc ^ (int) *ptr++ << 8; 
        i = 8; 
        do 
        { 
        if (crc & 0x8000) 
            crc = crc << 1 ^ 0x1021; 
        else 
            crc = crc << 1; 
        } while(--i); 
    } 
    return (crc); 
} 
/******************************************************************************/
//�˳�Bootloader���򣬴�0x0000��ִ��Ӧ�ó��� 
/******************************************************************************/
void quit(void)      
{ 
    u8 i;
    while(AppString[i]!='\0')       //�����˳�bootloader����ʾ��Ϣ
    { 
        uart_putchar(startupString[i]); 
        i++; 
    } 
    while(!(UCSR0A & 0x20));        //�ȴ�������ʾ��Ϣ������� 
    MCUCR = 0x01; 
    MCUCR = 0x00;                    //���ж�������Ǩ�Ƶ�Ӧ�ó�����ͷ�� 
    #asm("jmp 0x0000")               //��ת��Flash��0x0000����ִ���û���Ӧ�ó��� 
} 
/******************************************************************************/
//������                                                                        
/******************************************************************************/
void main(void) 
{ 
    int i = 0; 
    u8 timercount = 0; 
    u8 packNO = 1; 
    int bufferPoint = 0; 
    u16 crc; 
    //��ʼ��M64��USART0 
    UBRR0H = BAUD_H;     
    UBRR0L = BAUD_L;          //Set baud rate 
    UCSR0B = 0x18;            //Enable Receiver and Transmitter 
    UCSR0C = UART_FORMAT;            //Set frame format: 8data, 2stop bit 
    //��ʼ��M64��T/C0��15ms�Զ����� 
    OCR0 = 0xEA; 
    TCCR0 = 0x0F;     
    
    //��PC�����Ϳ�ʼ��ʾ��Ϣ 
    while(startupString[i]!='\0') 
    { 
        uart_putchar(startupString[i]); 
        i++; 
    } 
        
   //3���ֵȴ�PC�·�"d"�������˳�Bootloader���򣬴�0x0000��ִ��Ӧ�ó��� 
    while(1) 
    { 
        if(uart_getchar()== 'd') break; 
        if (TIFR & 0x02)                        //timer0 over flow 
        { 
               if (++timercount > 200) quit();  //200*15ms = 3s 
            TIFR = TIFR|0x02; 
        } 
    }             
    
    //ÿ����PC������һ�������ַ�"C"���ȴ������֡�soh�� 
    while(uart_getchar()!=XMODEM_SOH)           //receive the start of Xmodem 
    { 
         if(TIFR & 0x02)                        //timer0 over flow 
        { 
            if(++timercount > 67)               //wait about 1 second 
            { 
                uart_putchar(XMODEM_RECIEVING_WAIT_CHAR);    //send a "C" 
                timercount=0; 
            } 
            TIFR=TIFR | 0x02; 
        } 
    } 
 
/*************************************************************************************/
// XmodemЭ����һ��ʹ�ò��ŵ��ƽ�����ĸ��˼����ͨ���й㷺ʹ�õ��첽�ļ�����Э�顣
// ����Э����128�ֽڿ����ʽ�������ݣ�����ÿ���鶼ʹ��һ��У��͹��������д����⡣
// ������շ�����һ�����У��������ڷ��ͷ���У�����ͬʱ�����շ������ͷ�����һ��
// �Ͽ��ֽڡ�
// Xmodem�Ŀ����ַ���<soh> 01H��<eot> 04H��<ack> 06H��<nak> 15H��<can> 18H��<eof> 1AH��
// Xmodem�������ݿ��ʽ��"<soh> <packNO> <255-packNO> <128���ֽڵ����ݿ�> <cksum>"��
// ����<soh>Ϊ��ʼ�ֽڣ�<packNO>Ϊ���ݿ����ֽڣ�ÿ�μ�һ��<255-packNO>��ǰһ�ֽڵ�
// ���룻�������ǳ���Ϊ128�ֽڵ����ݿ飻����<cksum>��128�ֽ����ݵ�CRCУ���룬����
// Ϊ2���ֽڡ����ն��յ�һ�����ݿ鲢У����ȷʱ������<ack>�����մ������<nak>������
// ��<can>��ʾҪ���Ͷ�ֹͣ���͡� ���Ͷ��յ�<ack>�󣬿ɼ���������һ�����ݿ飨packNO+1����
// ���յ�<nak>����ٴ��ط���һ�����ݿ顣 ���Ͷ˷���<eot>��ʾȫ�����ݷ�����ɡ������
// ����Ҫ���͵����ݲ���128���ֽڣ���<eof>����һ�����ݿ顣 �����ַ�"C"����������ã�
// �����Ͷ��յ�"C"�����ַ�ʱ���������¿�ʼ��CRCУ�鷽ʽ�������ݿ飨packNO = 1����ÿ
// ����һ���µ����ݿ�<packNO>��1���ӵ�OxFF����һ�����ݿ��<packNO>Ϊ�㡣У�鷽ʽ��
// ��16λCRCУ��(X^16 + X^12 + X^5 + 1)�� 
/************************************************************************************/  
    //��ʼ�������ݿ� 
    do{ 
        if ((packNO == uart_waitchar()) && (packNO ==(~uart_waitchar()))) {    //�˶����ݿ�����ȷ 
            for(i=0;i<128;i++) {                         //����128���ֽ�����              
                data[bufferPoint]= uart_waitchar(); 
                bufferPoint++;     
            } 
            crc = uart_waitchar();
            crc = crc << 8;
            crc += uart_waitchar();                     //����2���ֽڵ�CRCЧ���� 
            if(calcrc(&data[bufferPoint-128],128)==crc){//CRCУ����֤             
                while(bufferPoint >= SPM_PAGESIZE)  {   //��ȷ����256���ֽڵ����� 
                    write_one_page();                   //�յ�256�ֽ�д��һҳFlash�� 
                    address += SPM_PAGESIZE;            //Flashҳ��1 
                    bufferPoint = 0; 
                }     
                uart_putchar(XMODEM_ACK);               //��ȷ�յ�һ�����ݿ� 
                packNO++;                               //���ݿ��ż�1 
            } 
            else { 
                uart_putchar(XMODEM_NAK);               //Ҫ���ط����ݿ� 
            } 
        } 
        else { 
            uart_putchar(XMODEM_NAK);                   //Ҫ���ط����ݿ� 
        } 
    }while(uart_waitchar()!=XMODEM_EOT);               //ѭ�����գ�ֱ��ȫ������ 
    
    uart_putchar(XMODEM_ACK);                           //֪ͨPC��ȫ���յ�      
    if(bufferPoint) write_one_page();                   //��ʣ�������д��Flash�� 
    quit();                                             //�˳�Bootloader���򣬴�0x0000��ִ��Ӧ�ó��� 
} 
