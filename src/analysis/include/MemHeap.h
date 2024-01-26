/****************************************************************************
 *
 *      File:   �ڴ�ѷ�����
 *      Desc:   һ��ȫ�ֵ��ڴ�ѷ�����,�����ڴ�ʹ����ֻ���䲻�ͷ�,ֻ�����̽���
 *				ʱ��ȫ���ͷŶ��ڴ�
 *      Log:
 *              created by victor, 2007/4/23
 *
 ****************************************************************************/ 
#ifndef MEM_HEAP_H
#define MEM_HEAP_H

#include <stddef.h>
/////////////////////////////////////////////////////////////////////////////
//
// Data types
//
/////////////////////////////////////////////////////////////////////////////       
typedef struct _SMemPage {
	size_t iBytesUsed;			// number of bytes already used from this page
	size_t iPageSize;			// default value is specified in heap. can be changed, if necessary
	void *pMem;					// pointer to the memory block that to allocated
	struct _SMemPage *pNext;
	struct _SMemPage *pPrev;
} SMemPage;

typedef struct _SHeap {
  SMemPage *pHead;
  SMemPage *pCur;
  size_t iDefPageSize;
  size_t iNumOfPages;
} SHeap;

/////////////////////////////////////////////////////////////////////////////
//
// Prototypes
//
/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

/* �����ڴ�� */
SHeap *suNewHeap(size_t nSize);
/* ���������С���ڴ�ռ�,iAlign�ֽڶ����С,ͨ�����Ϊsizeof(char*) */
void *suHeapAlloc(SHeap *pHeap, size_t iSize, size_t iAlign);
/* Ϊ�̶���С���ڴ���������ڴ�ռ�,�˺���������suHeapAlloc�������,�˺���ֻ������list,hashtable,hashmap������ */
void *suAllocNode(SHeap *pHeap, size_t iSize);
/* ���ڴ�ռ�������λ����,�������нϴ����ڴ�ҳ�ͷ� */
void suResetHeap( SHeap *pHeap);
/* �ͷ������ڴ�� */
void suFreeHeap( SHeap *pHeap);
size_t suGetHeapSize( SHeap *pHeap );
size_t suGetHeapUsedSize( SHeap *pHeap );
//char *suGetHeapDetail( char *szBuf, size_t iLen, SHeap *pHeap );

#ifdef __cplusplus
};
#endif


#endif  //_SE_HEAP_H
