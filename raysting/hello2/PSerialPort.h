// PSerialPort.h: interface for the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void  (*LPDataArriveProc)(char *data,int length,DWORD userdata);

class CPSerialPort  
{
public:
	CPSerialPort();
	virtual ~CPSerialPort();

	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity,LPDataArriveProc proc=NULL,DWORD userdata=0); //�򿪴���
	BOOL ClosePort(); //�رմ���
	inline BOOL Close() {return ClosePort();}
	//������رճ�������
	BOOL Activate();
	BOOL Deactivate();
	BOOL IsActive();
	
	//���������̺߳���
	static DWORD WINAPI ReadPortThread(LPVOID lpParameter);
	

	DWORD WritePort(unsigned char *data,int length); //��������
	inline DWORD SendData(unsigned char *data,int length){return WritePort(data,length);}
	inline DWORD SendData(char *data,int length){return WritePort((unsigned char*)data,length);}
private:
	DWORD ReadPort(char *data,int length); //��ȡһ�����ȵ�����
	HANDLE m_hComm; //�����豸handle
	HANDLE m_hReadThread; //�������߳�handle
	BOOL m_bReceiving; //�Ƿ��������
	int m_nBufferSize; //�����С

	char *Buffer; //������
	
	LPDataArriveProc m_lpDataArriveProc;
	DWORD m_dwUserData;

	//���������Լ���ʱ����
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;	
	
};

