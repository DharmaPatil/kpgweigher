// ModifySerialDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ModifySerial.h"
#include "ModifySerialDlg.h"
#include ".\modifyserialdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModifySerialDlg �Ի���



CModifySerialDlg::CModifySerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifySerialDlg::IDD, pParent)
	, m_strKey(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModifySerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strKey);
}

BEGIN_MESSAGE_MAP(CModifySerialDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifySerialDlg ��Ϣ�������

BOOL CModifySerialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CModifySerialDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CModifySerialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CModifySerialDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CModifySerialDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString sAppFolder;
	char szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	sAppFolder=szPath;
	int id=sAppFolder.ReverseFind('\\');
	CString oldpath = sAppFolder.Left(id);

	::WinExec(oldpath+"\\postrun.bat",SW_NORMAL);	
	::Sleep(3000);
	UpdateData();
	sAppFolder = sAppFolder.Left(id) + CString("\\Default\\License.txt");
	
	CStdioFile csf;
	if(csf.Open(sAppFolder,CFile::modeCreate|CFile::modeWrite))
	{
		csf.WriteString(m_strKey);
		csf.Close();
		OnOK();
	}else{
		::AfxMessageBox("��д��Ʒ���к�ʧ��");
		OnBnClickedCancel();
	}

}

void CModifySerialDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
