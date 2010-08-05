// AutoPtr.h

#ifndef _AutoPtr_h_
#define _AutoPtr_h_

#include "SmartBase.h"

#ifdef _MSC_VER
	#pragma warning(disable: 4284)	// CAutoPtr::operator -> not a UDT or reference to a UDT.
#endif

// ��	�ܣ��Զ�ָ��
// ˵	����
//		���ⲿ�ͷ�ָ��ķ���
//		if(ptr.MaxLength() > 1)
//			delete[] ptr.Drop();
//		else if(ptr.MaxLength() == 1)
//			delete ptr.Drop();
template<typename TYPE>
class CAutoPtr : public _CSmartBase
{
public:
	explicit CAutoPtr(int len = 0);
	CAutoPtr(TYPE* p, int len);

	void Release();

	// ������ָ�뽫�ᱻ�ͷţ�r��ָ���ת�Ƹ������
	CAutoPtr& operator = (CAutoPtr& r);

	// ��¡��simple_obj���Ƿ�򵥶���trueʹ��memcpy��falseʹ��TYPE�ĸ�ֵ�����
	CAutoPtr& Clone(const CAutoPtr& r, bool simple_obj);

	// ͬ operator = ()
	CAutoPtr(CAutoPtr& r);

	~CAutoPtr();

	// ���� _p
	TYPE* operator -> () const;

	// ���� *_p
	TYPE& operator * ();

	// ����ָ�����Ȩ������_p��len����Ԫ�ظ���
	TYPE* Drop(int* len = NULL);
};

#define _CAutoPtr CAutoPtr<TYPE>

//=============================================================================//

template<typename TYPE>
_CAutoPtr::CAutoPtr(int len /* = 0 */)
{
	ASSERT(len >= 0);

	if(0 == len)
		_p = NULL;
	else if(1 == len)
		_p = new TYPE();
	else
		_p = new TYPE[len];

	_len = NULL!=_p ? len : 0;
}

template<typename TYPE>
inline _CAutoPtr::CAutoPtr(TYPE* p, int len)
{
	ASSERT(len > 0);
	_p = p;
	_len = NULL!=_p ? len : 0;
}

template<typename TYPE>
void _CAutoPtr::Release()
{
	if(IsValid()) {
		ASSERT(_len > 0);

		if(1 == _len)
			delete _p;
		else
			delete[] _p;

		_len = 0;
		_p = NULL;
	}
}

template<typename TYPE>
_CAutoPtr& _CAutoPtr::operator = (_CAutoPtr& r)
{
	if(this == &r)
		return *this;

	Release();

	_len = r._len;
	_p = r._p;

	r._len = 0;
	r._p = NULL;

	return *this;
}

template<typename TYPE>
_CAutoPtr& _CAutoPtr::Clone(const CAutoPtr& r, bool simple_obj)
{
	if(this == &r)
		return *this;

	*this = CAutoPtr(r._len);

	if(simple_obj) {
		memcpy(_p, r._p, r.MaxSize());
	}
	else {
		for(int i=0; i<r._len; i++)
			_p[i] = r._p[i];
	}

	return *this;
}

template<typename TYPE>
inline _CAutoPtr::CAutoPtr(_CAutoPtr& r)
{
	_len = 0;
	_p = NULL;
	*this = r;
}

template<typename TYPE>
inline _CAutoPtr::~CAutoPtr()
	{	Release();	}

template<typename TYPE>
inline TYPE* _CAutoPtr::operator -> () const
	{	ASSERT(IsValid()); return _p;	}

template<typename TYPE>
inline TYPE& _CAutoPtr::operator * ()
	{	ASSERT(IsValid()); return *_p;	}

template<typename TYPE>
inline TYPE* _CAutoPtr::Drop(int* len /* = NULL */)
{
	if(len)
		*len = _len;

	TYPE* p = _p;
	_len = 0;
	_p = NULL;

	return p;
}

#endif //_AutoPtr_h_
