// ModifySerialDlg.h : ͷ�ļ�
//

#pragma once


// CModifySerialDlg �Ի���
class CModifySerialDlg : public CDialog
{
// ����
public:
	CModifySerialDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MODIFYSERIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	CString m_strKey;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
