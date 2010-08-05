// MatchingString.h
//
// �ַ���ƥ��

#ifndef _MatchingString_h_
#define _MatchingString_h_

inline bool IsLetter(int n)
	{	return (_T('a') <= n && n <= _T('z') || _T('A') <= n && n <= _T('Z'));	}
inline bool IsNum(int n)
	{	return (_T('0') <= n && n <= _T('9'));	}
inline bool IsLetterNum(int n)
	{	return IsLetter(n) || IsNum(n);	}
inline bool IsEqualChar(TCHAR ch1, TCHAR ch2, bool bMatchCase = true)
	{	return bMatchCase
				? ( ch1 == ch2 )
				: ( ((_T('a')<=ch1&&ch1<=_T('z')) ? (ch1-_T('a')+_T('A')) : ch1)
					== ((_T('a')<=ch2&&ch2<=_T('z')) ? (ch2-_T('a')+_T('A')) : ch2)
				  );
	}

//��  �ܣ�ȥ���ַ������˵Ŀո�Tab��\r��\n
//��  ����...��start������trim�����ʼλ��
//����ֵ��trim��ĳ���
int StrTrim0(CONST TCHAR* src, int len, int* start = NULL);

//��  �ܣ�ȥ���ַ������˵Ŀո�Tab��\r��\n
//��  ����dst��Ŀ���ַ�buffer����֤dst�㹻��dst��src������ͬһ��ָ�롻
//����ֵ��dst
TCHAR* StrTrim(TCHAR* dst, CONST TCHAR* src, int len);

//��  �ܣ��ַ����滻
//��  ����tszTarget��Ŀ���ַ�����
//		  iBufSize��Ŀ���ַ����������ߴ磻
//		  tszRep��tszNew����tszNew�滻tszRep��tszNew����ΪNULL
//����ֵ���ɹ���tszTarget��ʧ�ܷ���NULL
TCHAR* StrReplace(TCHAR* tszTarget, int iBufSize, CONST TCHAR* tszRep, CONST TCHAR* tszNew);

//��  �ܣ���������Ϊ��Ӧ���ַ���
//��  ����iNumeralFormat��������ֵĽ��ƣ�
//˵  ���������ƺ�ʮ�����ư����޷���������
LPTSTR sprintfn(LPTSTR buf, __int64 num, int iNumeralFormat = 10);

//��  �ܣ�sprintfn()����ǿ��
//��  ����iDigit�����λ��������ǰ����0���㣬��������ʵ��λ�����(Ĭ�ϰ���ʵ��λ�����)��
//		  iSeparateSize�������ָ��ַ�֮������ָ���(Ĭ��û�зָ��ַ�)��
//		  Separater���ָ��ַ���
//����ֵ������buf
LPTSTR sprintfnEn(LPTSTR buf, __int64 num, int iNumeralFormat = 10, int iDigit = -1, int iSeparateSize = -1, TCHAR Separater = _T(','));

//��  �ܣ���lpszSour�в����ַ���lpszFind��
//		  lpszFind�п��԰���ͨ���ַ���?������ʾ�����ַ�������#������ʾ�ַ�'0'--'9'��
//��  ����...
//		  nSourLen��nFindLen��Դ���Ͳ��Ҵ��ĳ��ȣ�Ĭ��Ϊ0�������ַ���
//����ֵ���ɹ�����ƥ��λ�ã����򷵻�-1
int  FindingString(LPCTSTR lpszSour, LPCTSTR lpszFind, bool bMatchCase = true, int nSourLen = -1, int nFindLen = -1);

//��	  �ܣ���ͨ������ַ���ƥ��
//��	  ����lpszSour��һ����ͨ�ַ�����
//			  lpszMatch��һ���԰���ͨ������ַ�����
//			  bMatchCase��true�����ִ�Сд[��ȫƥ��]��false�������ִ�Сд��
//��  ��  ֵ��ƥ�䣬����true�����򷵻�false��
//ͨ������壺
//		��*��	���������ַ������������ַ�����
//		��?��	��������һ���ַ�������Ϊ�գ���2001.11.02��
//		��#��	�����ַ���0������9��		��2003.06.26��
bool MatchingString(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bMatchCase = true);

//��  �ܣ�����ƥ�䣬��ͬƥ���ַ���֮���á�,����;������
//			�磺��*.h,*.cpp��������ƥ�䡰*.h���͡�*.cpp��
//��  ����bAndLogic = false, ��ͬƥ�����true���룻bMatchCase, �Ƿ��С����
//����ֵ��
bool MultiMatching(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic = false, bool bMatchCase = true);
//ͬ�ϣ�ֻ�ǰ�ƥ�����е� "*." ����ƥ��û����չ�����ļ���
bool MultiMatching2(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic = false, bool bMatchCase = true);

#endif	//#ifndef _MatchingString_h_
