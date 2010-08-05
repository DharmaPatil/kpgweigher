// Smart.h
//
// ���ļ�����������ģ���࣬��Щ���Ч�Ľ����һЩ�ձ��C++�������
// ע�⣺��Щ�಻��֤���̰߳�ȫ

// ��ͼ
//
//                    |===> CAutoPtr
//            inherit |
// CSmartBase =======>|                                            |===> CFreshTag
//                    |               inherit              combin  |
//                    |===> CSmartBuf =======> CSmartArray =======>|===> CRingQueue
//                                                                 |               combin
//                                                                 |===> CQuickMap ======> CQuickID
//

// guojun, 2004.01 beijing
// modified, 2004.03

#ifndef _Smart_h_
#define _Smart_h_

#include "AutoPtr.h"
#include "SmartBuf.h"
#include "SmartArray.h"
#include "FreshTag.h"
#include "RingQueue.h"
#include "QuickMap.h"
#include "QuickID.h"

#endif	// #ifdef _Smart_h_
