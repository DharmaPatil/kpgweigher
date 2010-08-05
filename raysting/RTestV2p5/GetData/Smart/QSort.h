// QSort.h
//
// ���������㷨�ȣ�QSortPtr()��QSortObj()��BSearchObj()��BSearchPtr()��SpecialSort()

#ifndef _QSort_h_
#define _QSort_h_

#ifndef NULL
	#define NULL 0
#endif

//��  �ܣ�ʹ�ñ�׼C����qsort()���С�����ָ�����顿����
//��  ����dwCmpPar���Ƚϲ��������ݸ��ȽϺ����������Զ���
//ע  �⣺TYPE�����й�����Ա����
//			int TYPE::Compare(const TYPE& cmper) const;
//		  �ȽϺ�������ֵ����
//		  >0��*this >  cmper
//		  =0��*this == cmper
//		  <0��*this <  cmper
template<typename TYPE> struct tagCmpFunc {
	static int qcmp(const void* p1, const void* p2)
	{	return (*(TYPE**)p1)->Compare( (const TYPE&) (**(TYPE**)p2) ); }
};
template<typename TYPE>
inline void QSortPtr(TYPE* const* ppObj, int nTail, int nHead = 0)
	{	if(nHead < nTail)
			qsort((void*)(ppObj+nHead), nTail-nHead+1, sizeof(TYPE*), tagCmpFunc<TYPE>::qcmp);
	}

//��  �ܣ�ʹ�ñ�׼C����qsort()���С��������顿����
//		  ������ͬ�ϡ�
template<typename TYPE> struct tagCmpFunc2 {
	static int qcmp(const void* p1, const void* p2)
	{	return ((TYPE*)p1)->Compare( (const TYPE&) (*(TYPE*)p2) ); }
};
template<typename TYPE>
inline void QSortObj(TYPE* pObj, int nTail, int nHead = 0)
	{	if(nHead < nTail)
			qsort(pObj+nHead, nTail-nHead+1, sizeof(TYPE), tagCmpFunc2<TYPE>::qcmp);
	}


template<typename TYPE>
inline TYPE const* GetPtrType(TYPE const* v)
	{	return v;	}

template<typename TYPE>
inline TYPE const* GetPtrType(TYPE const* const* v)
	{	return *v;	}

//��  �ܣ��۰���һ���ģ�塺ͬʱ���� BSearchObj() �� BSearchPtr()��
//��  ����insert����Ӧ�ò����λ�á�������ҳɹ���insert==����ֵ��
//����ֵ���ɹ���Χ�ҵ���Ԫ�ص��±�ţ�ʧ�ܷ���-1��
//ע  �⣺ typename TYPE�ıȽϺ���ԭ��Ϊ int TYPE::Compare(const KEY_TYPE& cmper) const;
//		  ģ����������� BSearchObj() �� BSearchPtr() ����
template<typename SEARCH_TYPE, typename OBJ_TYPE, typename KEY_TYPE>
int BSearchTempl(SEARCH_TYPE base, const KEY_TYPE& key, int tail, int head /* = 0 */, int* insert /* = NULL*/)
{
	int nMid = 0;
	int iComp = 0;

	while(head <= tail) {
		nMid  = (head+tail) / 2;

		// ���� GetPtrType<OBJ_TYPE>(base+nMid);
		// ��BSearchObj(...) �л��Զ����� TYPE const* GetPtrType(TYPE const* v)
		// ��BSearchPtr(...) �л��Զ����� TYPE const* GetPtrType(TYPE const* const* v)
		iComp = GetPtrType<OBJ_TYPE>(base+nMid)->Compare(key);

		if(iComp == 0) {
			if(insert) *insert = nMid;
			return nMid;
		}
		else {
			if(iComp > 0) {
				tail = nMid-1;
				iComp = -1;
			}
			else {
				head = nMid+1;
				iComp = 1;
			}
		}
	}

	if(insert) {
		if(iComp > 0)
			*insert = nMid+1;
		else
			*insert = nMid;
	}

	return -1;
}

//��    �ܣ����������۰����
//ʹ��ע�⣺��baseʵ��Ϊ���������飬����ת��Ϊ��������ʹ��ʱ��Ҫ��֤��������û�����ݳ�Ա
template<typename TYPE, typename KEY_TYPE>
inline int BSearchObj(TYPE const* base, const KEY_TYPE& key, int tail, int head = 0, int* insert = NULL)
	{	return BSearchTempl<TYPE const*, TYPE, KEY_TYPE>(base, key, tail, head, insert);	}

//��    �ܣ�����ָ�������۰���ҡ���������ͬQSortPtr��
template<typename TYPE, typename KEY_TYPE>
inline int BSearchPtr(TYPE const* const* base, const KEY_TYPE& key, int tail, int head = 0, int* insert = NULL)
	{	return BSearchTempl<TYPE const* const*, TYPE, KEY_TYPE>(base, key, tail, head, insert);	}

//��  �ܣ�ָ��˳������
//��  ����base����Ҫ��������飻count��������
//		  order��ָ����˳������Ϊ 0 �� count-1 �����֣�ÿ����������ֻ��һ��
//����ֵ��
template<typename TYPE>
void SpecialSort(TYPE* base, int count, const int* order)
{
	ASSERT(count>=0);

	BYTE* p = NULL;
	TYPE* _base;

	DWORD tmpbuf[1024];
	int total_size = sizeof(TYPE)*count;
	if(total_size > sizeof(tmpbuf)) {
		p = new BYTE[sizeof(TYPE)*count];
		_base = (TYPE*)p;
	}
	else
		_base = (TYPE*)tmpbuf;

	for(int i=0; i<count; i++) {
#ifdef _DEBUG
		ASSERT(0<=order[i] && order[i]<count);
		int j;
		for(j=0; j<count; j++) {
			if(i == order[j])
				break;
		}
		ASSERT(j<count);
#endif
		memcpy(_base+order[i], base+i, sizeof(TYPE));
	}

	memcpy(base, _base, sizeof(TYPE)*count);

	if(NULL != p)
		delete[] p;
}

#endif	//#ifndef _QSort_h_
