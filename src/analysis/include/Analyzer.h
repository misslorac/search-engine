/******************************************************************************
 *
 * File     :CAnalyzer.h
 * Version  :$Id: Analyzer.h,v 1.9 2005/03/29 09:19:24 jamesyu Exp $
 * Desc     :������,������ı����зִʴ���,�ṩѡ���ķִʷ�ʽ�͹��˷�ʽ,�Բ�ͬ��
			 �ı�,�û���Ҫ�����Լ��ķ�����,�ڷ�������ָ���ִʷ�ʽ(ͨ��tokenizer
			 ������)�͹�����(filter������)�����

 * Log      :Create by yusiheng, 2004-12-24
 *
 ******************************************************************************/
#ifndef ANALYZER_H
#define ANALYZER_H

//#include "ObjectFactory.h"
//#include "Singleton.h"
#include "StringUtility.h"
#include "StopFilter.h"
#include "UnityFilter.h"
//#include "DictionaryHelper.h"

namespace analysis 
{

class CTokenStream;
class CTokenizer;
class CStopFilter;

class CAnalyzer
{
public:
	/* ���캯�� */
	CAnalyzer(void)
	{
		m_bIncreasePos = true;
		m_szStopWords  = NULL;
		m_pTokenizer   = NULL;
		m_pTokenStream = NULL;
		m_pStopFilter  = NULL;
//        m_pUnityFilter = NULL;
	}
	/* �ڴ�������������,��Ϊ���ɶ��󹤳�������analyzer֬���ܱ��ͷ� */
	virtual ~CAnalyzer(void)
	{
		if (m_szStopWords != NULL)
			delete[] m_szStopWords;
	    if (m_pTokenStream != NULL)
        delete m_pTokenStream;
	}
	/* ���ṩ���ı�,����һ��token����,��������ȡ�÷ֽ�,���˺��token(term) */
	virtual CTokenStream *tokenStream(const char* szSentence,n32_t nSLen = 0) = 0;
	/* Ϊ�ִ����趨stop word�б� */
	virtual void setStopWordList(const char *szStopWordList)
	{
		if (m_szStopWords == NULL&&szStopWordList != NULL)
			m_szStopWords = utility::string_utility::replicate(szStopWordList);
	    if (m_pStopFilter != NULL)
	        m_pStopFilter->setStopWordList(szStopWordList);
	}
	/*���� unityFilter��dictHelper*/
/*	virtual void setDictHelper(dictionary::CDictionaryHelper * pDictHelper)
	{
	    if (m_pUnityFilter != NULL)
	        m_pUnityFilter->setDictHelper(pDictHelper);
	}*/
	/* ���̳д˻�������඼���뽫�����ص�����ע�� */
//	static void registerAnalyzer(const char* szDictionaryPath, const char *szBwsConf = NULL);
    /* ���ݷִ�������ָ��ע���µķִ���,���̳����ฺ����Ӹ��ִ��� */
//    static void registerAnalyzer(utility::CObjectFactory<CAnalyzer> *pAnalyzerFactory, 
//								const char* szDictionaryPath,
//								const char *szBwsConf = NULL);
    /* �ж��Ƿ�Ϊstopword */
	bool IsStopWord(const char* szWord) {
	    if (m_pStopFilter != NULL) {
		    return m_pStopFilter->IsStopWord(szWord);
	    } else {
		    return false;
	    }
	}
	/* �ж��Ƿ���Ҫ����λ�� */
	bool isIncreasePos() {
		return m_bIncreasePos;
	}

protected:
	bool m_bIncreasePos;			//�ж��Ƿ�������ʱ�ۼ�ÿ��field�ִʳ�����tokenλ��
	char* m_szStopWords;			//���ִ���Ҫ�󱻹��˵ĺ���
	CTokenStream* m_pTokenStream;   //��¼���һ��tokenStream�������ͷ�����tokenStream��
	CTokenizer*   m_pTokenizer;     //��¼�ʼ��tokenizer,����������Ҫ�ִʵ��ַ���
    CStopFilter*  m_pStopFilter;    //��¼stopFilter����������stopWords
//	CUnityFilter* m_pUnityFilter;   //��¼UnityFilter����������dictHelper
};

//typedef utility::CObjectFactory<CAnalyzer> CAnalyzerFactory;
//typedef utility::Singleton<CAnalyzerFactory> AnalyzerFactory;		//�����ִ�������

}	//namespace analysis

#endif // ANALYZER_H
