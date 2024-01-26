#include "MetaCharAnalyzer.h"

#include "Analyzer.h"
#include "MetaCharTokenizer.h"
#include "StopFilter.h"
#include "LowerCaseFilter.h"
#include "UpperCaseFilter.h"
#include "GBK_T2GBK_SFilter.h"
#include "NormalizeFilter.h"

namespace analysis { 

/* ���캯��,���ⲿָ��stop words���б� */
CMetaCharAnalyzer::CMetaCharAnalyzer()
{
	CTokenStream* pTokenStream = new CMetaCharTokenizer();
	m_pTokenizer   = static_cast<CTokenizer *>(pTokenStream);
	pTokenStream = new CGBK_T2GBK_SFilter(pTokenStream);
	pTokenStream = new CLowerCaseFilter(pTokenStream);
	pTokenStream = new CNormalizeFilter(pTokenStream);
	pTokenStream = new CStopFilter(pTokenStream, m_szStopWords);
	m_pStopFilter = static_cast<CStopFilter *>(pTokenStream);
	m_pTokenStream = pTokenStream;   
}
/* �������� */
CMetaCharAnalyzer::~CMetaCharAnalyzer()
{
}
/* �����߸����Ƿ��ͷŷ���ֵ */
CTokenStream* CMetaCharAnalyzer::tokenStream(const char *szSentence,basis::n32_t nSLen)
{
	if (NULL == szSentence)
		return NULL;
	if (nSLen <= 0)
		nSLen = strlen(szSentence);

	m_pTokenizer->setText(szSentence,nSLen);
	return m_pTokenStream;
}
/* ��̬���������� */
CAnalyzer* CMetaCharAnalyzer::create()
{
	return new CMetaCharAnalyzer();
}

}
