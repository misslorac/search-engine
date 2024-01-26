/******************************************************************************
*
* File     :MetaCharAnalyzer.h
* Version  :$Id: MetaCharAnalyzer.h
* Desc     :Ĭ�ϵķִʷ�ʽ,��һ����һ���ַֽ�,���Ǹ����ķִ��õ�,�����Ӣ�ĵķִ�
*			��Ҫ����дһ��Analyzer

* Log      :Created by jianhao, 2009-12-15
*
******************************************************************************/
#ifndef _META_CHAR_ANALYZER_H_
#define _META_CHAR_ANALYZER_H_

#include "Analyzer.h"

namespace analysis {

class CAnalyzer;
class CTokenStream;


class CMetaCharAnalyzer : public CAnalyzer
{
public:
	/* ���캯��,����������ָ��stop words���б�,�б����ַ�������ʽ��ʾ */
	CMetaCharAnalyzer();
	/* �������� */
	virtual ~CMetaCharAnalyzer(void);
	/* ���ṩ���ı�,����һ��token����,��������ȡ�÷ֽⲢ�й��˺��token(term) */
	/* ��������ʹ���귵�ص�CTokenStreamָ��󣬱����ͷ�CTokenStream����ָ�� */
	CTokenStream *tokenStream(const char* szSentence,basis::n32_t nSLen = 0);
	/* ��̬���ɸ���һ������ĺ��� */
	static CAnalyzer* create();
};


}// end analysis namespace

#endif // #ifndef _META_CHAR_ANALYZER_H_
