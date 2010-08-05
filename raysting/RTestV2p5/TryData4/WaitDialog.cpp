// WaitDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TryData4.h"
#include "WaitDialog.h"
#include ".\waitdialog.h"


// CWaitDialog �Ի���

IMPLEMENT_DYNAMIC(CWaitDialog, CDialog)
CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
	, m_sWaiting(_T(""))
{
	bQuit = false;
}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WAITTEXT, m_sWaiting);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWaitDialog ��Ϣ�������
void CWaitDialog::OnClose()
{
	if(bQuit)
		CDialog::OnClose();
	else
		this->ShowWindow(SW_HIDE);
}
void CWaitDialog::OnCancel()
{
	if(bQuit)
		CDialog::OnCancel();
	else
		this->ShowWindow(SW_HIDE);
}

void CWaitDialog::OnDestroy()
{
	if(bQuit) 
		CDialog::OnDestroy();
	else
		this->ShowWindow(SW_HIDE);
}
void CWaitDialog::OnBnClickedOk()
{
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	if(bQuit) 
		CDialog::OnOK();
	else
		this->ShowWindow(SW_HIDE);
}