// SmartBase.h

#ifndef _SmartBase_h_
#define _SmartBase_h_

#include "DbgHelp.h"

// ģ�����
template<typename TYPE>
class CSmartBase
{
protected:
	int		_len;	// Ԫ�ظ���
	TYPE*	_p;		// Ԫ��Buffer

#ifdef _DEBUG
	int*	_valid_cnt;	// ָ���¼����Ԫ�ظ����ı���
#endif

protected:
	CSmartBase();

public:
	bool IsValid() const;

	int MaxLength() const;	// Ԫ�ظ���
	int MaxSize() const;	// �ֽڳߴ�

	// ���� _p
	TYPE* Get();
	const TYPE* Get() const;

	// ���� _p[index]
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;

	// ���� _p+index
	TYPE* operator + (int index);
	const TYPE* operator + (int index) const;

	// ���ṩ operator TYPE* ()������������ң���Ҫ��ȷ���ú��� Get()��
};

#define _CSmartBase CSmartBase<TYPE>

template<typename TYPE>
inline _CSmartBase::CSmartBase()
	{	DEBUG_EXP(_valid_cnt = &_len);	}

template<typename TYPE>
inline bool _CSmartBase::IsValid() const
	{	return NULL != _p;	}

template<typename TYPE>
inline int _CSmartBase::MaxLength() const
	{	return _len;	}

template<typename TYPE>
inline int _CSmartBase::MaxSize() const
	{	return _len * sizeof(TYPE);	}

template<typename TYPE>
inline TYPE* _CSmartBase::Get()
	{	return _p;	}

template<typename TYPE>
inline const TYPE* _CSmartBase::Get() const
	{	return _p;	}

template<typename TYPE>
inline TYPE& _CSmartBase::operator [] (int index)
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p[index];	}

template<typename TYPE>
inline const TYPE& _CSmartBase::operator [] (int index) const
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p[index];	}

template<typename TYPE>
inline TYPE* _CSmartBase::operator + (int index)
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p+index;		}

template<typename TYPE>
inline const TYPE* _CSmartBase::operator + (int index) const
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p+index;		}

#endif //_SmartBase_h_
