// FreshTag.h

#ifndef _FreshTag_h_
#define _FreshTag_h_

#include "SmartArray.h"

// ��	�ܣ����¶��������
// ע	�⣺����ߴ粻���Զ�̬�ı�
template<typename TYPE, int stack_len>
class CFreshTag
{
protected:
	CSmartArray<TYPE, stack_len, false> _ar;

protected:
	int Find(const TYPE& obj) const;

public:
	explicit CFreshTag(int max_tags = stack_len);

	int Tags() const;

	// �ڶ���ĩ���һ����Ԫ�أ����ԭ���Ѿ����ڣ����obj�Ƶ����
	// ���ԭ����������������false��CFreshTag�ĳߴ粻���Զ�̬�ı䡻
	// ���򷵻�true
	bool Add(const TYPE& obj);

	// ������û��obj����false
	bool Del(const TYPE& obj);

	// ȡ���һ��Ԫ�������ţ�û�з���-1
	int Last() const;

	// Ԫ�ط���
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;
};

#define _CFreshTag CFreshTag<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline int _CFreshTag::Find(const TYPE& obj) const
{
	for(int i=0; i<_ar.Items(); i++) {
		if(obj == _ar[i])
			return i;
	}
	return -1;
}

template<typename TYPE, int stack_len>
inline _CFreshTag::CFreshTag(int max_tags /* = stack_len */)
	: _ar(max_tags) {}

template<typename TYPE, int stack_len>
inline int _CFreshTag::Tags() const
	{	return _ar.Items();	}

template<typename TYPE, int stack_len>
bool _CFreshTag::Add(const TYPE& obj)
{
	int i = Find(obj);

	if(-1 == i) {	// ԭ���Ķ�������û��obj
		if(_ar.Items() == _ar.MaxLength()) {	// ����
			ASSERT(false);
			return false;
		}
		else {
			return _ar.Add(obj, false);
		}
	}
	else {			// ��obj�ƶ������
		for(int j=i; j<_ar.Items()-1; j++) {
			_ar[j] = _ar[j+1];
		}

		_ar[_ar.Items()-1] = obj;
		return true;
	}
}

template<typename TYPE, int stack_len>
bool _CFreshTag::Del(const TYPE& obj)
{
	int i = Find(obj);
	if(-1 == i)
		return false;

	_ar.RemoveAt(i);
	return true;
}

template<typename TYPE, int stack_len>
inline int _CFreshTag::Last() const
	{	return Tags()>0 ? Tags()-1 : -1;	}

template<typename TYPE, int stack_len>
inline TYPE& _CFreshTag::operator [] (int index)
	{	return _ar[index];	}

template<typename TYPE, int stack_len>
inline const TYPE& _CFreshTag::operator [] (int index) const
	{	return _ar[index];	}

#endif //_FreshTag_h_
