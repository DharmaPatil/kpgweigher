// SmartArray.h

#ifndef _SmartArray_h_
#define _SmartArray_h_

#include "PreDef.h"
#include "SmartBuf.h"

#define _CArrayBase CSmartBuf<TYPE, stack_len>

// ��	�ܣ���������
// ��	����is_simple������TYPE�Ƿ�Ϊ�����ͣ�
//				�����Ͷ��壺����ĳ�Ա���������ö����Լ����ڴ档
//				���磬�������Ի������͡�MFC��CString��CArray�ȶ��Ǽ����ͣ�CSmartBuf���Ǽ����͡�
//			����������CSmartBuf��
// �� ע�⣺MFC������ģ����CArray��ֻ�ܴ�ż����͵Ķ���
template<typename TYPE, int stack_len, bool is_simple /* = true */>
class CSmartArray : public _CArrayBase
{
protected:
	int			_items;	// ����Ԫ�ظ��� <= _len

public:
	explicit CSmartArray(int max_len = stack_len);
	~CSmartArray();

	CSmartArray(const CSmartArray& r);
	CSmartArray& operator = (const CSmartArray& r);

public:
	int Items() const;
	int GetUpperBound() const;

	// ����һ���µ���󳤶ȣ���� max_len < _items����ú���������Ч
	// �ú�������Ӱ��ı� _items
	bool SetMaxLength(int max_len);	

	// ����Ԫ�ظ��������Զ�����SetMaxLength��
	void SetItems(int count);

	// bAutoGrow���Ƿ��ڶ�����ʱ�Զ����Ӷ��л���
	// nGrowCount��һ�����ӵ�����
	bool InsertAt(int insert, const TYPE& obj, bool bAutoGrow = true, int nGrowCount = 4);
	bool Add(const TYPE& obj, bool bAutoGrow = true, int nGrowCount = 4);

	void RemoveAt(int index);
	void RemoveAll();
};

#define _CSmartArray CSmartArray<TYPE, stack_len, is_simple>

//=============================================================================//

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::CSmartArray(int max_len /* = stack_len */)
	: _CArrayBase(max_len)
{
	_items = 0;
	DEBUG_EXP(_valid_cnt = &_items);

	ConstructObjs(_p, _len);
}

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::~CSmartArray()
{
	DestructObjs(_p, _len);
}

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::CSmartArray(const _CSmartArray& r)
	: _CArrayBase(0)
{
	_items = 0;
	DEBUG_EXP(_valid_cnt = &_items);

	*this = r;
}

template<typename TYPE, int stack_len, bool is_simple>
_CSmartArray& _CSmartArray::operator = (const _CSmartArray& r)
{
	if(this == &r)
		return *this;

	_items = 0;	// RemoveAll();

	if(_len < r._len)
		SetMaxLength(r._len);

	_items = r._items;// Set new item count.
	for(int i=0; i<_items; i++)	// Copy all right items.
		_p[i] = r._p[i];	// Must call copy function of TYPE.

	return *this;
}

template<typename TYPE, int stack_len, bool is_simple>
inline int _CSmartArray::Items() const
	{	return _items;	}

template<typename TYPE, int stack_len, bool is_simple>
inline int _CSmartArray::GetUpperBound() const
	{	return _items - 1;	}

template<typename TYPE, int stack_len, bool is_simple>
bool _CSmartArray::SetMaxLength(int max_len)
{
	ASSERT(_items <= _len);

	if(max_len < _items) {
		// ����
		return false;
	}
	else if(_items<=max_len && max_len<=_len) {
		bool bStatck = IsStack();

		DestructObjs(_p+max_len, _len-max_len);	// ��������Ķ���
		_len = max_len;

		if(!bStatck && _len<=StackLen()) {
			// ���ԭ����ȫ�ֶ��У������³��� <= ��ջ���ȣ���Ѷ����Ƶ���ջbuffer��

			if(is_simple)
				memcpy(_stack, _p, _len * sizeof(TYPE));
			else
				MoveConstruct((TYPE*)_stack, _p, _len);

			_p = (TYPE*)_stack;

			delete[] (int*)_heap;
			_heap = NULL;
		}
	}
	else {	// max_len > _len
		if(max_len <= StackLen()) {
			ASSERT(NULL == _heap);
			ASSERT(IsStack());

			ConstructObjs(_p+_len, max_len-_len);
			_len = max_len;
		}
		else {
			bool bStatck = IsStack();

			_heap = (TYPE*)new int[_CALC_INT_LEN(TYPE, max_len)];

			if(is_simple)
				memcpy(_heap, _p, _len * sizeof(TYPE));
			else
				MoveConstruct(_heap, _p, _len);

			ConstructObjs(_heap+_len, max_len-_len);

			if(!bStatck)
				delete[] (int*)_p;

			_p = _heap;
			_len = max_len;
		}
	}

	return true;
}

template<typename TYPE, int stack_len, bool is_simple>
inline void _CSmartArray::SetItems(int count)
{
	ASSERT(count >= 0);
	SetMaxLength(count);
	_items = count;
}

template<typename TYPE, int stack_len, bool is_simple>
bool _CSmartArray::InsertAt(int insert, const TYPE& obj, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */)
{
	ASSERT(_items <= _len);
	ASSERT(0<=insert && insert<=_items);

	// ����
	if(_items == _len) {
		if(!bAutoGrow) {
			return false;
		}
		else {	// ���Ӷ��л���
			ASSERT(nGrowCount > 0);
			if(!SetMaxLength(_len + nGrowCount))
				return false;
		}
	}

	if(insert < _items) {
		if(is_simple) {
			//�����ͣ�ֱ�Ӳ��������ڴ�
			char buf[sizeof(TYPE)];
			memcpy(buf, _p+_items, sizeof(TYPE));
			memmove(_p+insert+1, _p+insert, (_items-insert)*sizeof(TYPE));
			memcpy(_p+insert, buf, sizeof(TYPE));
		}
		else {
			//�������ͣ�������ö���Ŀ�������
			for(int i=_items-1; i>=insert; i--)
				_p[i+1] = _p[i];
		}
	}
	else
		ASSERT(insert == _items);

	_p[insert] = obj;
	_items++;

	return true;
}

template<typename TYPE, int stack_len, bool is_simple>
inline bool _CSmartArray::Add(const TYPE& obj, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */)
{
	return InsertAt(_items, obj, bAutoGrow, nGrowCount);
}

template<typename TYPE, int stack_len, bool is_simple>
void _CSmartArray::RemoveAt(int index)
{
	ASSERT(0<=index && index<_items);

	if(index < _items-1) {
		if(is_simple) {
			char buf[sizeof(TYPE)];
			memcpy(buf, _p+index, sizeof(TYPE));
			memmove(_p+index, _p+index+1, (_items-1-index)*sizeof(TYPE));
			memcpy(_p+_items-1, buf, sizeof(TYPE));
		}
		else {
			for(int i=index+1; i<_items; i++)
				_p[i-1] = _p[i];
		}
	}

	_items--;
}

template<typename TYPE, int stack_len, bool is_simple>
inline void _CSmartArray::RemoveAll()
	{	_items = 0;	}

#endif //_SmartArray_h_
