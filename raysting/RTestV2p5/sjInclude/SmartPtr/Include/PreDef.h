// PreDef.h
//
// ����ȫ�ֱ��뿪�أ�����������ĺ꣬������ģ����
// Ӧ�ð�PreDef.h������Ԥ����ͷ�ļ��л������з�ϵͳͷ�ļ���֮ǰ
//
// ���ļ�����꣬����һ����
//
// #pragma compiling_remind("text")
// int offsetof(s, m);
// c* container_pointer(c, m);
// DWORD MAKEFOURCC(ch0, ch1, ch2, ch3);
// void ZeroStru(stru);
// void ZeroArray(arr);
// T1 min(T1 a, T2 b);
// T1 max(T1 a, T2 b);
// T abs(T a);
// T SignSwitch(T n);
// bool IsGrt(T t, T t0, T interval);
// bool IsGrtE(T t, T t0, T interval);
// DWORD DWORD_COMB(int low_word, int high_word);
// DWORD DWORD_COMB(POINT pt);
// int LOW_INT(DWORD dw);
// int HIGH_INT(DWORD dw);
// int Width(const RECT& rc);
// int Height(const RECT& rc);
// void ConstructObjs(T* objs, int count);
// void DestructObjs(T* objs, int count);
// void MoveConstruct(TYPE* dst, TYPE* src, int count);
// CONST CHAR* tctoc(CONST TCHAR* scr, CHAR* dst);

// template<typename Interface> class CInterfacePack;

// guojun, 2004.06 beijing

#ifndef _PreDef_h_
#define _PreDef_h_

#ifdef _MSC_VER
	#define inline __forceinline	// force inline
	#pragma warning(disable: 4710)	// function not inlined
	#pragma warning(disable: 4714)	// function not inlined

	#pragma warning(disable: 4127)	// conditional expression is constant

	#pragma warning(disable: 4239)	// CAutoPtr<T> ptr;
									// ptr = CAutoPtr<T>(1);
	#pragma warning(disable: 4514)	// unreferenced inline function has been removed
	#pragma warning(disable: 4201)	// nonstandard extension used : nameless struct/union

	#pragma warning(1: 4100)		// unreferenced formal parameter
	#pragma warning(1: 4101)		// unreferenced local parameter
	#pragma warning(1: 4700)		// local variable used without having been initialized

	#pragma warning(disable: 4200)	// zero-sized array in struct/union
	#pragma warning(error: 4005)	// macro redefinition
	#pragma warning(error: 4715)	// 'function' : not all control paths return a value
	#pragma warning(error: 4018)	// 'expression' : signed/unsigned mismatch
	#pragma warning(error: 4002)	// too many actual parameters for macro 'identifier'
	#pragma warning(error: 4390)	// ';' : empty controlled statement found; is this the intent?
	#pragma warning(error: 4307)	// integral constant overflow


	// ��Ǵ���
	#define remind_str0(x)			#x
	#define remind_str(x)			remind_str0(x)
	#define compiling_remind(desc)	message(__FILE__ "(" remind_str(__LINE__) "): " #desc)
//	����
//	#pragma compiling_remind("�Ժ��޸Ĵ˴�!")

#else
//	#define typename class
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "DbgHelp.h"

#ifndef CONST
	#define CONST const
#endif

typedef char			CHAR;
typedef unsigned short	WCHAR;
typedef unsigned long	DWORD;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	UINT;
typedef BYTE			UINT8;
typedef int				BOOL;

#ifndef NULL
	#define NULL 0
#endif

// s�ĳ�Աm�����ƫ�Ƶ�ַ
#ifdef offsetof
	#undef offsetof
#endif
#define offsetof(s, m)   (int)&(((s*)0)->m)

// �õ�������Ķ���ָ�롺�ڶ���m�ĳ�Ա������ʹ�á�
#define container_pointer(c, m)		(c*) ( (int)this - offsetof(c, m) )

#ifndef MAKEFOURCC
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)	\
			((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |	\
				((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#define ZeroStru(stru)		memset(&(stru), 0, sizeof(stru))
#define ZeroArray(arr)		ZeroStru(arr)

#ifdef min
	#undef min
	#undef max
#endif
template<typename T1, typename T2> inline T1 min(T1 a, T2 b)
	{	return a<T1(b) ? a : T1(b);	}
template<typename T1, typename T2> inline T1 max(T1 a, T2 b)
	{	return a>T1(b) ? a : T1(b);	}

#ifdef abs
	#undef abs
#endif
template<typename T> inline T abs(T a)
	{	return a<0 ? -a : a;	}

//�����л����������λȡ��
template<typename T> inline T SignSwitch(T n)
	{	return (T)(n ^ ((T)1 << (sizeof(T)*8-1)));	}

//�������Ƚϡ���֤t��������ת��ȽϽ����ȷ��
//����if(IsGrt(GetTickCount(), last_tick, 100))	....
template<typename T> inline bool IsGrt(T t, T t0, T interval)
{
#ifdef _DEBUG
	T upper_interval = (T)1 << (sizeof(T)*8-2);	// upper_interval = ����Tֵ/4
	ASSERT(interval <= upper_interval);	// interval̫��û������
#endif
	return T(t-t0) > interval;
}
template<typename T> inline bool IsGrtE(T t, T t0, T interval)
{
#ifdef _DEBUG
	T upper_interval = (T)1 << (sizeof(T)*8-2);
	ASSERT(interval <= upper_interval);
#endif
	return T(t-t0) >= interval;
}

inline DWORD DWORD_COMB(int low_word, int high_word)
	{	return (DWORD(high_word)<<16) | (DWORD(low_word)&0xffff);	}
#ifdef _WINDEF_
	inline DWORD DWORD_COMB(POINT pt)	// pt.y --> high word, pt.x --> low word
		{	return DWORD_COMB(pt.x, pt.y);	}
#endif

inline int LOW_INT(DWORD dw)
	{	return short(dw&0xffff);	}
inline int HIGH_INT(DWORD dw)
	{	return short(dw>>16);	}

#ifdef _WINDEF_
	inline int Width(const RECT& rc)
		{	return rc.right - rc.left;	}
	inline int Height(const RECT& rc)
		{	return rc.bottom - rc.top;	}
#endif

// Define ConstructObjs(...) and DestructObjs(...)
#if !defined(__AFXTEMPL_H__) && !defined(_NEW_) && !defined(XP_UNIX)
	inline void* operator new(unsigned int, void* p)
		{	return p;	}
	#if defined(_MSC_VER) && _MSC_VER>=1200
		inline void operator delete(void*, void*)
			{	return;	}
	#endif
#endif

#if !defined(__AFXTEMPL_H__) || !defined(_DEBUG)
	template<typename T> inline void ConstructObjs(T* objs, int count)
		{
			for(; count--; objs++)
				::new((void*)objs) T;
		}
	template<typename T> inline void DestructObjs(T* objs, int count)
		{
			objs; // Avoid compile warning while T::~T() do not need.
			for(int i=0; i<count; i++)
				objs[i].~T();
			DEBUG_EXP(memset(objs, 0xcd, count*sizeof(T)));
		}
#else
	#define ConstructObjs ConstructElements
	template<typename T> inline void DestructObjs(T* objs, int count)
		{
			DestructElements(objs, count);
			DEBUG_EXP(memset(objs, 0xcd, count*sizeof(T)));
		}
#endif

//��  �ܣ��ƶ�����
//��  ����dst��Ŀ�����buffer����buffer�еĶ���û�о�������
//		  src��Դ����buffer��Ϊ�Ϸ��Ķ���buffer
//ע  �⣺�������Զ�����dst��������TYPE�Ŀ���������������src�е����ж���
template<typename TYPE>
void MoveConstruct(TYPE* dst, TYPE* src, int count)
{
	ASSERT(count >= 0);

	ConstructObjs(dst, count);
	for(int i=0; i<count; i++) {
		dst[i] = src[i];
	}
	DestructObjs(src, count);
}

// Unicode ֧��
#ifndef XP_UNIX
	#include <tchar.h>
	
	#ifdef _UNICODE
		typedef WCHAR TCHAR;
	#else
		typedef CHAR TCHAR;
	#endif

	typedef TCHAR* LPTSTR;
	typedef CONST TCHAR* LPCTSTR;

	// TCHAR --> CHAR����Ҫ��֤dst�㹻��
	inline CONST CHAR* tctoc(CONST TCHAR* scr, CHAR* dst)
	{
	#ifdef _UNICODE
		#ifdef _WINNLS_
			::WideCharToMultiByte(CP_ACP, 0, scr, -1, dst, wcslen(scr)+1, NULL, NULL);
			return dst;
		#else
			#error "Need prototype of WideCharToMultiByte()."
		#endif
	#else
		dst;
		return scr;
	#endif
	}
#else
	#define _tfopen fopen
#endif

// ��MFC����֧��
#if !defined(__AFX_H__)
	#if defined(_WINBASE_)
		inline bool AfxIsValidString(LPCTSTR lpsz, int nLength = -1)
			{	return lpsz && FALSE==IsBadStringPtr(lpsz, nLength);	}
		inline bool AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = true)
			{	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
					(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));	}
	#else
		inline bool AfxIsValidString(LPCTSTR lpsz, int nLength = -1)
			{	nLength; return NULL != lpsz;	}
		inline bool AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = true)
			{	nBytes, bReadWrite; return NULL != lp;	}
	#endif

	#include "SmartStr.h"
	typedef CSmartStr CString;

#endif

//�ӿڰ�װģ����
template<typename Interface>
class CInterfacePack
{
	typedef Interface* FNCreateInterface();
public:
	CInterfacePack(FNCreateInterface* fnCreate)
		{	_interface = fnCreate();	}
	~CInterfacePack()
		{	_interface->Release();	}

	Interface* operator -> ()
		{	return _interface;	}
	const Interface* operator -> () const
		{	return _interface;	}

private:
	CInterfacePack(const CInterfacePack&);	//��ֹ��������
	CInterfacePack& operator = (const CInterfacePack&);	//��ֹ����

	Interface* _interface;
};
/* CInterfacePackʵ������

// �ӿڶ���
class IYouInterface
{
public:
	virtual void Release() = 0;
	virtual void f1() = 0;
	virtual ....
};

// �ӿڴ�������
extern "C" {
	IYouInterface* CreateIYouInterface();
}

// ��CInterfacePack����һ���ӿ�ʹ����
struct CYouInterface : public CInterfacePack<IYouInterface>
{
	CYouInterface()
		: CInterfacePack<IYouInterface>(CreateIYouInterface) {}
};

// ʹ����Ľӿ�
  ...
CYouInterface interface;
interface->f1();
  ...

*/

#endif	//#ifndef _PreDef_h_
