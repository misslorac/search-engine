/*********************************************************************
 * 
 * File:	hashmap����,ͨ�����ݽṹ��
 * Version: $Id: HashMap.h,v 1.0 2007/05/19 09:19:24 victor Exp $
 * Desc:	bucket��С�Զ�������hashmap,�����е�KEY, VALUE��hash�㷨
 *			����ͨ��ģ���Զ�����������,��hash�㷨��ָ��ʱ,���Ը���������
 *			��Ĭ��hash�㷨
 * Log :
 * 			Create by victor, 2007/4/23
 *
 *********************************************************************/
#ifndef HASHMAP_H
#define HASHMAP_H

#include <assert.h>
#include <stdio.h>
#include "MemHeap.h"
#include "CommonAlgorithm.h"	//ͨ���㷨,KeyHash, KeyEqual���͵ķº���

/* Ĭ��ʹ���ڴ�ռ��С */
#define DEF_HASHMAP_MEM_SIZE 256

namespace utility {

#define MAX_SIZESTEP 25
extern unsigned int szHashMap_StepTable[];

/* hashmap�ڵ�ṹ */
template <typename Key, typename Value>
struct SHashEntry {
	SHashEntry* _next;			//��һ���ڵ�(ͬһ��entry�Ľڵ���γɵ�������)
	SHashEntry* _seq_next;		//������hashmap�еĽڵ���˫��������������,ָ��������нڵ����һ���ڵ�
	SHashEntry* _seq_prev;		//ָ��������нڵ����һ���ڵ�
	Key _key;					//hashmap�����е�key
	Value _value;				//hashmap�����е�value
};

/*
 *	ģ��hashmap��, Key����hash�Ķ���, Value��ϣ���е�Ԫ�ض���, HashFunc��������, Ĭ��Ϊ������hash�㷨����
 */
template< typename Key, typename Value, typename HashFunc = KeyHash<Key>, typename HashEqual = KeyEqual<Key> >
class CHashMap
{
public:
	//�ڵ���������
	typedef SHashEntry<Key,Value> HashEntry;
	//�������ȶ��������С�ڱȽϺ���
	struct SLessThan {
		inline bool operator()(HashEntry *pEntry1, HashEntry *pEntry2) {
			return pEntry1->_key < pEntry2->_key;
		}
	};

private:
	//����hash step��,����hash sizeӦ���Ƕ��ȽϺ���
	inline unsigned int getHashSizeStep(unsigned int nHashSize)
	{
		if (nHashSize < szHashMap_StepTable[0])
			return 0;
		BinarySearch<unsigned int> binarySearch;
		return binarySearch(szHashMap_StepTable,0,MAX_SIZESTEP,nHashSize);
	}

private:
	bool m_bMultiKey;				//�Ƿ�֧����ͬkey,true��ʾ�����key
	unsigned int m_nHashSize;		//Bucket�����С
	unsigned int m_nHashUsed;		//�Ѿ�ʹ�õ�Ԫ�ظ���
	unsigned int m_nRehashLimit;	//��Ҫ����rehash����ֵ
	unsigned int m_nHashSizeStep;	//ȡ��hash size���±�
	HashEntry **m_ppEntry;			//Bucket����,���һ������entryָ��

	HashFunc m_hasher;				//��key����hashֵ����ĺ�������
	HashEqual m_comparator;			//��key���бȽϵĺ�������		
	SHeap *m_pHeap;					//�ڵ��ڴ������

public:
	HashEntry *m_pFirst;			//����Ԫ�������ͷָ��
	HashEntry *m_pLast;				//����Ԫ�������βָ��

public:
	/* ���캯��,hash size�Ĵ�СĬ��ΪszHashMap_StepTable[2]��ֵ */
	CHashMap(unsigned char cHashSizeStep = 3);
	/* ���캯��, ֱ��ָ����Ҫ���Ŀռ������Ԫ�� */
	CHashMap(unsigned int nHashSize, bool bMaxSize = false);
	/* �������� */
	virtual ~CHashMap(void);

public:
	/* ����������Ϊ֧���ظ��Ķ�key */
	void setMultiKey(bool bMultiKey);
	/* �������в���key��value */
	inline void insert(const Key& _key, const Value& _value);
	/* ����key���ĳһ��Ԫ�� */
	inline void remove(const Key& _key);
	/* ��������е�����Ԫ�� */
	void clear(void);
	/* ����key����valueֵ,��key�������в�����ʱ,����noneĬ��ֵ */
	inline Value& find(const Key& _key, Value& _none) const;
	inline Value& find(const Key& _key, Value& _none, bool &bExist) const;
	/* ����key����valueֵ,��key�������в�����ʱ,��key��value��������,ͬʱ����value��ֵ */
	inline Value& find_or_insert(const Key& _key, Value& _value);
	/* ����key���ض�Ӧ��Entry,���������֧��multikey,��÷������Բ�����ͬkey��Ԫ�� */
	inline HashEntry* lookupEntry(const Key& _key) const;
	/* ����������Ԫ�صĸ��� */
	unsigned int size() const;
	/* ������Ԫ�صĴ�С������������3/5,��Ҫ���¹����hashmap */
	void rehash(unsigned int nHashSizeStep);
	/* �������ȶ���,��hashmap�е�Ԫ�ؽ��������� */
//	void sort(void);
	/* printf hash size */
	void print(void);
};

/* ���캯��,hash size�Ĵ�СĬ��ΪszHashMap_StepTable[2]��ֵ */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
CHashMap<Key,Value,HashFunc,HashEqual>::CHashMap(unsigned char cHashSizeStep)
{
	m_nHashSizeStep = cHashSizeStep > MAX_SIZESTEP ? MAX_SIZESTEP : cHashSizeStep;
	m_nHashSize = szHashMap_StepTable[m_nHashSizeStep];
	m_nRehashLimit = (m_nHashSize * 3) / 5;
	m_nHashUsed = 0;
	m_bMultiKey = false;
	m_ppEntry = NULL;
	m_pFirst = m_pLast = NULL;
	typedef HashEntry *HashEntryPtr;
	m_ppEntry = new HashEntryPtr[m_nHashSize];
	assert(m_ppEntry != NULL);
	memset(m_ppEntry, 0x0, m_nHashSize*sizeof(HashEntryPtr));
	m_pHeap = suNewHeap(DEF_HASHMAP_MEM_SIZE);
}
/* ���캯��, ֱ��ָ����Ҫ���Ŀռ������Ԫ�� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
CHashMap<Key,Value,HashFunc,HashEqual>::CHashMap(unsigned int nHashSize, bool bMaxSize)
{
	m_nHashSizeStep = getHashSizeStep(nHashSize);
	if(!bMaxSize) {
	m_nHashSize = szHashMap_StepTable[m_nHashSizeStep];
	m_nRehashLimit = (m_nHashSize * 3) / 5;
	} else {
	  	m_nHashSize = nHashSize + 1;
	  	m_nRehashLimit = m_nHashSize;
	}	
	m_nHashUsed = 0;
	m_bMultiKey = false;
	m_ppEntry = NULL;
	m_pFirst = m_pLast = NULL;

	typedef HashEntry *HashEntryPtr;
	m_ppEntry = new HashEntryPtr[m_nHashSize];
	assert(m_ppEntry != NULL);
	memset(m_ppEntry, 0x0, m_nHashSize*sizeof(HashEntry *));
	m_pHeap = suNewHeap(DEF_HASHMAP_MEM_SIZE);
}
/* �������� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
CHashMap<Key,Value,HashFunc,HashEqual>::~CHashMap(void)
{
	clear();				//������нڵ���ڴ�
	delete[] m_ppEntry;
	m_ppEntry = NULL;
	suFreeHeap(m_pHeap);
}
/* ����������Ϊ֧���ظ��Ķ�key */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::setMultiKey(bool bMultiKey)
{
	m_bMultiKey = bMultiKey;
}

/* print */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::print()
{
	for (n32_t i=0; i<m_nHashSize; i++) {
		n32_t nSize = 0;
		HashEntry *pEntry = m_ppEntry[i];
		while (pEntry != NULL) {
			nSize++;
			pEntry = pEntry->_next;
		}
		printf("hash i=%d, nSize=%d.\n", i, nSize);
	}
	printf("Hash map size=%d.\n", this->size());
}

/* �������в���key��value */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::insert(const Key& _key, const Value& _value)
{
	HashEntry *pMultiEntry = NULL;

	unsigned int nPos = m_hasher(_key) % m_nHashSize;			//����key��hashֵ
	HashEntry *pEntry = m_ppEntry[nPos];
	while (pEntry != NULL && !m_comparator(_key,pEntry->_key))	//��ͬhash��ÿһ��Ԫ����Ƚ�
		pEntry = pEntry->_next;
	if (pEntry != NULL) {		//��key�Ѿ�����
		if (m_bMultiKey) {		//���������key,��Ҫ������ָ����Ϊ��һ��Ԫ�ز����λ��
			pMultiEntry = pEntry;
		}
		else {					//�������κβ���
			return;
		}
	}

	pEntry = (HashEntry*)suAllocNode(m_pHeap, sizeof(HashEntry));	//���ڴ������з����ڴ�ռ�
	pEntry->_key = _key;
	pEntry->_value = _value;
	pEntry->_next = NULL;
	/* ����Ԫ�ز��뵽����λ�� */
	if (pMultiEntry != NULL) {
		pEntry->_next = pMultiEntry->_next;
//		while (pMultiEntry->_next != NULL) {
//			pMultiEntry = pMultiEntry->_next;
//		}
		pMultiEntry->_next = pEntry;
	}
	else {
		pEntry->_next = m_ppEntry[nPos];
		m_ppEntry[nPos] = pEntry;
	}
	m_nHashUsed ++;

	/* ���µ�Ԫ�ز���Ԫ�������� */
	pEntry->_seq_next = NULL;
	if (m_pLast != NULL) {
		m_pLast->_seq_next = pEntry;
		pEntry->_seq_prev = m_pLast;
	}
	else {
		assert(m_pFirst == NULL);
		m_pFirst = pEntry;
		pEntry->_seq_prev = NULL;
	}
	m_pLast = pEntry;
	/* �ж������Ƿ񳬹�����������rehash */
	if (m_nHashUsed >= m_nRehashLimit && m_nHashSizeStep < MAX_SIZESTEP) {
		rehash(++m_nHashSizeStep);
	}
}
/* ������Ԫ�صĴ�С������������3/5,��Ҫ���¹����hashmap */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::rehash(unsigned int nHashSizeStep)
{
	typedef HashEntry *HashEntryPtr;
	m_nHashSize = szHashMap_StepTable[nHashSizeStep];
	m_nRehashLimit = (m_nHashSize * 3) / 5;
	HashEntry **ppNewEntries = new HashEntryPtr[m_nHashSize];
	memset(ppNewEntries, 0x0, m_nHashSize*sizeof(HashEntryPtr));

	unsigned int nHashKey;
	//����ÿһ��bucket����,����ЩԪ������hash
	for (HashEntry *pEntry = m_pFirst; pEntry != NULL; pEntry = pEntry->_seq_next) {
		nHashKey = m_hasher(pEntry->_key) % m_nHashSize;
		pEntry->_next = ppNewEntries[nHashKey];
		ppNewEntries[nHashKey] = pEntry;
	}
	delete[] m_ppEntry;
	m_ppEntry = ppNewEntries;
}
/* ����key���ض�Ӧ��Entry,���������֧��multikey,��÷������Բ�����ͬkey������Ԫ�� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
SHashEntry<Key,Value>* CHashMap<Key,Value,HashFunc,HashEqual>::lookupEntry(const Key& _key) const
{
	unsigned int nPos = m_hasher(_key) % m_nHashSize;			//����key��hashֵ
	HashEntry *pEntry = m_ppEntry[nPos];
	while (pEntry != NULL && !m_comparator(_key,pEntry->_key))	//��ͬhash��ÿһ��Ԫ����Ƚ�
		pEntry = pEntry->_next;

	return pEntry;
}
/* ����key���ĳһ��Ԫ�� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::remove(const Key& _key)
{
	unsigned int nPos = m_hasher(_key) % m_nHashSize;
	HashEntry *pPrevEntry = NULL, *pCurEntry = m_ppEntry[nPos];

	while (pCurEntry != NULL && !m_comparator(_key,pCurEntry->_key)) {
		pPrevEntry = pCurEntry;
		pCurEntry = pCurEntry->_next;
	}

	if (pCurEntry != NULL) {	//���ǵ������Ƕ�keyֵ�����
		do {
			if (pPrevEntry == NULL) {					// ��entry�ĵ�һ��Ԫ��
				assert(pCurEntry == m_ppEntry[nPos]);
				m_ppEntry[nPos] = pCurEntry->_next;
			} else {
				pPrevEntry->_next = pCurEntry->_next;
			}

			/* ������Ԫ��������,����Ԫ���Ƴ� */
			if (pCurEntry == m_pFirst) {
				m_pFirst = pCurEntry->_seq_next;
			}
			else if(pCurEntry->_seq_prev) {
			  
				pCurEntry->_seq_prev->_seq_next = pCurEntry->_seq_next;
			}

			if (pCurEntry == m_pLast) {
				m_pLast = pCurEntry->_seq_prev;
			}
			else if(pCurEntry->_seq_next) {
				pCurEntry->_seq_next->_seq_prev = pCurEntry->_seq_prev;
			}
			m_nHashUsed --;
			pCurEntry = pCurEntry->_next;
		} while(m_bMultiKey&&( NULL != pCurEntry) && m_comparator(_key,pCurEntry->_key));
	}
}
/* ��������е�����Ԫ��,��������С�����ı� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::clear(void)
{
	suResetHeap(m_pHeap);
	if (m_nHashUsed > 0) {
		memset(m_ppEntry, 0x0, m_nHashSize*sizeof(HashEntry *));
	}
	m_nHashUsed = 0;
	m_pFirst = m_pLast = NULL;
}
/* ����key����valueֵ,��key�������в�����ʱ,����noneĬ��ֵ */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
Value& CHashMap<Key,Value,HashFunc,HashEqual>::find(const Key& _key, Value& _none) const
{
	HashEntry *pEntry = lookupEntry(_key);
	if (pEntry != NULL)
		return pEntry->_value;
	else
		return _none;
}
/* ����key����valueֵ,��key�������в�����ʱ,����noneĬ��ֵ */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
Value& CHashMap<Key,Value,HashFunc,HashEqual>::find(const Key& _key, Value& _none, bool &bExist) const
{
  	bExist = false;
	HashEntry *pEntry = lookupEntry(_key);
	if (pEntry != NULL) {
	  	bExist = true;
		return pEntry->_value;
	}
	else
		return _none;
}
/* ����key����valueֵ,��key�������в�����ʱ,��key��value��������,ͬʱ����value��ֵ */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
Value& CHashMap<Key,Value,HashFunc,HashEqual>::find_or_insert(const Key& _key, Value& _value)
{
	unsigned int nPos = m_hasher(_key) % m_nHashSize;			//����key��hashֵ
	HashEntry *pEntry = m_ppEntry[nPos];
	while (pEntry != NULL && !m_comparator(_key,pEntry->_key))	//��ͬhash��ÿһ��Ԫ����Ƚ�
		pEntry = pEntry->_next;
	if (pEntry != NULL) {				//��key�Ѿ�����
		return pEntry->_value;
	}

	pEntry = (HashEntry*)suAllocNode(m_pHeap, sizeof(HashEntry));	//���ڴ������з����ڴ�ռ�
	pEntry->_key = _key;
	pEntry->_value = _value;
	/* ����Ԫ�ز��뵽���ʵ�λ�� */
	pEntry->_next = m_ppEntry[nPos];
	m_ppEntry[nPos] = pEntry;
	m_nHashUsed ++;

	/* ���µ�Ԫ�ز���Ԫ�������� */
	pEntry->_seq_next = NULL;
	if (m_pLast != NULL) {
		m_pLast->_seq_next = pEntry;
		pEntry->_seq_prev = m_pLast;
	}
	else {
		assert(m_pFirst == NULL);
		m_pFirst = pEntry;
		pEntry->_seq_prev = NULL;
	}
	m_pLast = pEntry;
	/* �ж������Ƿ񳬹�����������rehash */
	if (m_nHashUsed >= m_nRehashLimit && m_nHashSizeStep < MAX_SIZESTEP) {
		rehash(++m_nHashSizeStep);
	}

	return pEntry->_value;
}
/* ����������Ԫ�صĸ��� */
template <typename Key, typename Value, typename HashFunc, typename HashEqual>
unsigned int CHashMap<Key,Value,HashFunc,HashEqual>::size() const
{
	return m_nHashUsed;
}
/* �������ȶ���,��hash���е�Ԫ�ؽ��������� */
/*template <typename Key, typename Value, typename HashFunc, typename HashEqual>
void CHashMap<Key,Value,HashFunc,HashEqual>::sort()
{
	if (m_nHashUsed < 2)
		return;
	CPriorityQueue<HashEntry*, SLessThan> queue;
	queue.initialize(m_nHashUsed);
	for (HashEntry *pEntry = m_pFirst; pEntry != NULL; pEntry = pEntry->_seq_next) {	//����ÿһ��bucket����
		queue.push(pEntry);
	}
	m_pFirst = m_pLast = queue.pop();
	m_pLast->_seq_next = NULL;
	//�����ȶ��е�����ǰ�ò��뷽ʽ��������
	while (queue.size() > 0) {
		HashEntry *pEntry = queue.pop();
		m_pFirst->_seq_prev = pEntry;
		pEntry->_seq_next = m_pFirst;
		m_pFirst = pEntry;
	}
	m_pFirst->_seq_prev = NULL;
} */

}	//namesapce utiltiy

#endif

