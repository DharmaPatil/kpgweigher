#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////////////////
/////     ˵ ��
////
// 1. ������к���ִ��ʱ���ش�����GetLastError()�õ�������Ϣ
// 2. ����д������׼��Ini�ļ���ʽ
// 3. ��CATCH{}������Եõ��쳣��Ϣ

#define  MAX_LEN   1000      
///����CString��ʹ��CString��ǰ��ù����������������п��ܳ��ֵ����                             
///�ַ����ȣ���GetBuffer(MAX_LEN)���������������⽫����Ƶ��������ɾ��
///�������������Ч�ʣ�Ҳ�������ڴ���Ƭ��
CString  strIniPath = _T("\\�ڲ��洢\\profile.ini");  ///ȫ�ֱ�����Ԥ������һ���ļ�·����
                                                 ///�ļ�·��û���̷�����C:\ ��D:\ ��
                                                 ///����Ϊ����·����CE��û���·�����

BOOL  MyWriteProfileString(const CString strSection, const CString strEntry, const CString strValue)
{
	////���ж����в����Ƿ�Ϊ��
	if(strSection == L"" || strEntry == L"" || strValue == L"")
	{
		return FALSE;
	}
	CFile    IniFile;
	CString  strCombine;

	TRY
	{
		////���ļ�
		if(! IniFile.Open(strIniPath, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return FALSE;
		}
		///�ж��ļ��Ƿ�Ϊ�գ�Ϊ����ֱ��д���ݣ��Ͳ����Ȱ��ļ����ݶ�������
		if(IniFile.GetLength() == 0)
		{
			strCombine = L"[" + strSection + L"]" + L"\r\n" 
				         + strEntry + L"=" + strValue + L"\r\n";
			///�õ�strCombine�����Ļ������׵�ַ�����������֣�						
			LPTSTR  lpCombine = strCombine.GetBuffer(0);  
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		///�����ļ��������ݵ�������
		WCHAR  *pBuf;
		pBuf = new WCHAR[IniFile.GetLength() / 2 + 1];  ///�ļ�����ΪUNICODE�������ļ�����һ����2�ı���
		if(pBuf == NULL)
		{
			IniFile.Close();
			return FALSE;
		}
		if(IniFile.Read(pBuf, IniFile.GetLength()) != IniFile.GetLength())
		{
			delete[]  pBuf;
			IniFile.Close();
			return FALSE;
		}
		///�ѻ�������ַ����CString����Ϊ��ʹ��CString�����ĺ�����
		///һ��Ini�ļ��Ƚ�С�����Դӻ�������CString�ĸ��ƹ�����ʱ�̡ܶ��ķ���ԴҲ����
		pBuf[IniFile.GetLength() / 2] = NULL;
		strCombine.GetBuffer(MAX_LEN);   ///�ȴ�����Ļ�����
		strCombine = pBuf;
		delete[]   pBuf;

		//////��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = strCombine.Find(L"[" + strSection + L"]\r\n");
		if(iIndex1 == -1)  ///û�ҵ�
		{
			strCombine += L"[" + strSection + L"]" + L"\r\n" 
				          + strEntry + L"=" + strValue + L"\r\n";
			///�õ�strCombine�����Ļ������׵�ַ
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);   ///ɾ��ԭ������
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = strCombine.Find(strEntry + L"=", iIndexT);
		if(iIndex2 == -1)  ///û�ҵ�
		{
			strCombine.Insert(iIndexT, strEntry + L"=" + strValue + L"\r\n");
			///�õ�strCombine�����Ļ������׵�ַ
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		else
		{
			iIndex3 = strCombine.Find(L"\r\n", iIndex2 + 1);
			if(iIndex3 == -1)
			{
				IniFile.Close();
				return FALSE;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			strCombine.Delete(iIndexT, iIndex3 - iIndexT);
			strCombine.Insert(iIndexT, strValue);
			///�õ�strCombine�����Ļ������׵�ַ
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);  
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		
	} ///end TRY
	CATCH(CFileException, e)
	{
/*		////���ڵ��ԣ��õ�������Ϣ
		CString strT;
		switch(e->m_cause)
		{
		case CFileException::generic:
		case CFileException::badPath:
		case CFileException::accessDenied:
		case CFileException::hardIO:
		case CFileException::diskFull:
		...............
		default:
		}
*/	}
	END_CATCH     ///�������Ժ�
	
	IniFile.Close();  
	return FALSE;
}


CString GetProfileString(const CString strSection, const CString strEntry, const CString strDefault)
{
	////���ж�ǰ���������Ƿ�Ϊ��
	if(strSection == L"" || strEntry == L"")
	{
		return strDefault;   ///���ɹ��򷵻�Ĭ��ֵ
	}
	CFile    IniFile;
	CString  strCombine;

	TRY
	{
		////���ļ�
		if(! IniFile.Open(strIniPath, CFile::modeRead))
		{
			return strDefault;
		}
		///�ж��ļ��Ƿ�Ϊ��
		if(IniFile.GetLength() == 0)
		{
			IniFile.Close();
			return strDefault;
		}
		///�����ļ��������ݵ�������
		WCHAR  *pBuf;
		pBuf = new WCHAR[IniFile.GetLength() / 2 + 1];  
		if(pBuf == NULL)
		{
			IniFile.Close();
			return strDefault;
		}
		if(IniFile.Read(pBuf, IniFile.GetLength()) != IniFile.GetLength())
		{
			delete[]  pBuf;
			IniFile.Close();
			return strDefault;
		}
		pBuf[IniFile.GetLength() / 2] = NULL;
		strCombine.GetBuffer(MAX_LEN);   ///�ȴ�����Ļ�����
		strCombine = pBuf;
		delete[]   pBuf;

		//////��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = strCombine.Find(L"[" + strSection + L"]\r\n");
		if(iIndex1 == -1)  ///û�ҵ�
		{
			IniFile.Close();
			return strDefault;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = strCombine.Find(strEntry + L"=", iIndexT);
		if(iIndex2 == -1)  ///û�ҵ�
		{
			IniFile.Close();
			return strDefault;
		}
		else
		{
			iIndex3 = strCombine.Find(L"\r\n", iIndex2 + 1);
			if(iIndex3 == -1)
			{
				IniFile.Close();
				return strDefault;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			IniFile.Close();
			return  strCombine.Mid(iIndexT, iIndex3 - iIndexT);
		}
	}
	CATCH(CFileException, e)
	{		
	}
	END_CATCH      ///�������Ժ�
	
	IniFile.Close();  
	return strDefault;
}


BOOL  WriteProfileInt(const CString strSection, const CString strEntry, const int iValue)
{
	////����ͬWriteProfileString���ơ�
	////ֻ�轫int iValueת��CString strValue
	////ʹ�ú�����wchar_t * _itow( int value, wchar_t *string, int radix );
	return TRUE;
} 


int  GetProfileInt(const CString strSection, const CString strEntry, const int iDefault)
{
	////����ͬGetProfileString���ơ�
	////ֻ�轫�õ���CString����ת����int��
	////ʹ�ú�����int _wtoi( const wchar_t *string );
	return iDefault;
}  

