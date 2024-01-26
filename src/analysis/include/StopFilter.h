#ifndef STOP_FILTER_H
#define STOP_FILTER_H

#include "TokenFilter.h"
#include "HashMap.h"
//#include "Svector.h"
#include <vector>

using namespace utility;

namespace analysis {

class CTokenStream;

class CStopFilter : public CTokenFilter
{
public:
	/* ���캯��,szStopWords�ַ���,��stop word֮���ö��Ÿ� */
	CStopFilter(CTokenStream *pInput, const char* szStopWords);
	/* ����stop words */
	void setStopWordList(const char *szStopWords);
	/* ϡ������ */
	~CStopFilter(void);
	/* ������һ��token */
	CToken *next();
	/* һ�����Ƿ���һ��stopword */
	bool IsStopWord(const char* szWord);

protected:
    /* ���stopwords list */
    void clearStopWordList();
        
private:
	basis::n32_t m_reserve;									//Ԥ��ֵȡ����С
    std::vector<CToken *> m_reserveTokens;		//���жϹ�����,�����µ�token
	CHashMap<char, char*> *m_pStopWordsTable;	//��stop word�ĵ�һ���ֽڽ���hashmap��Ӧ��

};

}	//end namespace analysis

#endif
