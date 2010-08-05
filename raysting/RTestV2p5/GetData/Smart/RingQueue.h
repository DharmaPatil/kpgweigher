// RingQueue.h

#ifndef _RingQueue_h_
#define _RingQueue_h_

#include "SmartArray.h"

// ��	�ܣ����ζ���
// ע	�⣺����ߴ粻���Զ�̬�ı�
template<typename TYPE, int stack_len>
class CRingQueue
{
protected:
	CSmartArray<TYPE, stack_len, false> _ar;

	int	_head;	// ָ����ɵ�Ԫ��
	int	_tail;	// ָ����һ�����Ԫ��λ��

public:
	explicit CRingQueue(int max_len = stack_len);

	bool IsValid() const;

	// ���п��Դ�ŵ����Ԫ��
	int ItemRooms() const;

	// �õ����е�Ԫ�ظ���
	int ItemCount() const;

	bool IsNull() const;
	bool IsFull() const;


	// Push()��PopOld()��PopNew() ����˵��
	// �ɹ�����Ԫ�������ţ�ʧ�ܷ���-1
	// ���ص�������Ӧ���������κζ��в���ǰʹ��
	// �� operator []

	// �ڶ���β������Ԫ�أ�����-1��ʾ����������CRingQueue����ĳߴ粻���Զ�̬�ı䡻
	int Push();

	// Pop����ɵ�Ԫ��
	int PopOld();

	// Pop����ɵ�Ԫ��
	int PopNew();

	// �������Ԫ��
	void PopAll();

	// �õ�Ԫ�����á����Է�������Ԫ�أ��������������ЧԪ�ء�
	// ��ʹ�� Push()��PopOld()��PopNew()���ص�������
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;

	// index == 0��ȡ���Ԫ�أ�index == ItemCount()-1��ȡ���µ�Ԫ��
	// �������ڱ�������
	TYPE& GetAtBottom(int index);
	const TYPE& GetAtBottom(int index) const;
};

#define _CRingQueue CRingQueue<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline _CRingQueue::CRingQueue(int max_len /* = stack_len */)
	: _ar(max_len), _head(0), _tail(0) {}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsValid() const
	{	return _ar.IsValid();	};

template<typename TYPE, int stack_len>
inline int _CRingQueue::ItemRooms() const
	{	return _ar.MaxLength();	}

template<typename TYPE, int stack_len>
inline int _CRingQueue::ItemCount() const
	{	return _ar.Items();	}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsNull() const
	{	return 0 == ItemCount();	}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsFull() const
	{	return ItemRooms() == ItemCount();	}

template<typename TYPE, int stack_len>
int _CRingQueue::Push()
{
	if(IsFull())
		return -1;

	int index = _tail;

	_ar.SetItems(_ar.Items()+1);
	_tail++;
	if(_tail == _ar.MaxLength())
		_tail = 0;

	return index;
}

template<typename TYPE, int stack_len>
int _CRingQueue::PopOld()
{
	if(IsNull())
		return -1;

	int index = _head;

	_ar.SetItems(_ar.Items()-1);
	_head++;
	if(_head == _ar.MaxLength())
		_head = 0;

	return index;
}

template<typename TYPE, int stack_len>
int _CRingQueue::PopNew()
{
	if(IsNull())
		return -1;

	_ar.SetItems(_ar.Items()-1);
	_tail--;
	if(-1 == _tail)
		_tail = _ar.MaxLength()-1;

	return _tail;
}

template<typename TYPE, int stack_len>
inline void _CRingQueue::PopAll()
	{	_head = _tail = 0;
		_ar.SetItems(0);	}

template<typename TYPE, int stack_len>
inline TYPE& _CRingQueue::operator [] (int index)
	{	ASSERT(0<=index && index<_ar.MaxLength());
		return _ar.Get()[index];	}

template<typename TYPE, int stack_len>
inline const TYPE& _CRingQueue::operator [] (int index) const
	{	return (*(_CRingQueue*)this)[index];	}

template<typename TYPE, int stack_len>
inline TYPE& _CRingQueue::GetAtBottom(int index)
{
	ASSERT(0<=index && index<ItemCount());

	index += _head;
	if(index >= _ar.MaxLength())
		index -= _ar.MaxLength();

	return (*this)[index];
}

template<typename TYPE, int stack_len>
inline const TYPE& _CRingQueue::GetAtBottom(int index) const
	{	return ((_CRingQueue*)this)->GetAtBottom(index);	}

#endif //_RingQueue_h_
