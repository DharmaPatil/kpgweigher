// ModifySerial.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CModifySerialApp:
// �йش����ʵ�֣������ ModifySerial.cpp
//

class CModifySerialApp : public CWinApp
{
public:
	CModifySerialApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CModifySerialApp theApp;
