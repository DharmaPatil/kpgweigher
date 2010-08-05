// SmartBuf.h

#ifndef _SmartBuf_h_
#define _SmartBuf_h_

#include "SmartBase.h"

// �������Ϊsizeof(int)������TYPE�ĳߴ�
#define _CALC_INT_LEN(TYPE, len)	\
	( (sizeof(TYPE) * (len) + sizeof(int)-1) / sizeof(int) )

// ��	�ܣ�����Buffer����������ֲ�����ʱ��СBufferʹ�ö�ջ����Buffer��ȫ�ֶ�������
// ��	����TYPE���������ͣ�stack_len��������>=0�����ڶ�ջ�ﶨ�������TYPE������ߴ�
// ע	�⣺CSmartBuf�����������TYPE�Ĺ��졺TYPE::TYPE()����������TYPE::~TYPE()��������
//			�������������ConstructObjs(...) �� DestructObjs(...)���������������
template<typename TYPE, int stack_len>
class CSmartBuf : public _CSmartBase
{
	// ��ֹ�����Ϳ�������
	CSmartBuf(const CSmartBuf&);
	CSmartBuf& operator = (const CSmartBuf&);

protected:
	TYPE*	_heap;	// ȫ�ֶ�buffer
	int		_stack[_CALC_INT_LEN(TYPE, stack_len)];	// ��ջbuffer

public:
	// �õ���ջbuffer�ֽڳ���
	const int StackSize() const;

	// �õ���ջbuffer����
	const int StackLen() const;

	// �Ƿ���ʹ�õĶ�ջbuffer
	bool IsStack() const;

	explicit CSmartBuf(int len = stack_len);
	~CSmartBuf();
};

#define _CSmartBuf CSmartBuf<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline const int _CSmartBuf::StackSize() const
{
#ifdef _DEBUG
	int size = sizeof(int) * _CALC_INT_LEN(TYPE, stack_len); size;
#endif
	return sizeof(int) * _CALC_INT_LEN(TYPE, stack_len);
}

template<typename TYPE, int stack_len>
inline const int _CSmartBuf::StackLen() const
{
#ifdef _DEBUG
	int len = StackSize() / sizeof(TYPE); len;
#endif
	return StackSize() / sizeof(TYPE);
}

template<typename TYPE, int stack_len>
inline bool _CSmartBuf::IsStack() const
{
#ifdef _DEBUG
	ASSERT(NULL != _p);
	if(_p == (TYPE*)_stack) {
		ASSERT(_len <= StackLen());
		ASSERT(NULL == _heap);
	}
	else {
		ASSERT(_p == _heap);
		ASSERT(_len > StackLen());
	}
#endif
	return _p == (TYPE*)_stack;
}

template<typename TYPE, int stack_len>
inline _CSmartBuf::CSmartBuf(int len /* = stack_len */)
{
#ifdef _DEBUG
	// Ԥ�����ջ�ߴ粻��Ҫ̫�󣬷���û��ʲôʵ������
	if(StackSize() > 512*1024)
		TRACE("stack_size too large: %.3fkb", StackSize()/1024.0);
#endif	
	
	ASSERT(len >= 0);

	if(len <= StackLen()) {
		_heap = NULL;
		_p = (TYPE*)_stack;
	}
	else {
		_p = _heap = (TYPE*)new int[_CALC_INT_LEN(TYPE, len)];
	}

	_len = NULL!=_p ? len : 0;
}

template<typename TYPE, int stack_len>
inline _CSmartBuf::~CSmartBuf()
{
	if(_heap)
		delete[] (int*)_heap;
}

#endif //_SmartBuf_h_
