/******************************************************************************
*
* File     :Tokenizer.h
* Version  :$Id: Tokenizer.h,v 1.5 2005/03/09 02:41:22 kwq Exp $
* Desc     :�ֽ����Ļ���,�ֽ����������ǽ��������ַ���,����ĳ�ֲ��Խ��зֽ�
*			Tokenizer�Ƿִ�����������,�����������ڴ�֮��
*			
* Log      :Created by yusiheng, 2004-1-11
*
******************************************************************************/
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Token.h"
#include "TokenStream.h"
#include "charset_func.h"

namespace analysis {

class CTokenizer : public CTokenStream
{
public:
	/* ���캯��,Ϊ��ûһ��tokenizer�ִʳ�����token�����ظ�����,���ڻ������洴��һ��ȫ�ֵ�token���� */
	CTokenizer(const char* szInput)
	{
		m_nSLen = 0;
        strncpy(m_szInput, szInput, strlen(szInput));
        m_szInput[strlen(szInput)] = 0;
	//	m_szInput = szInput;
		m_nStart = 0;				//Ĭ���Ǵӵ�һ���ַ���ʼ
		m_pToken = new CToken();
	}
	CTokenizer()
	{
	   // m_szInput = NULL;
	    m_nStart = 0;
	    m_pToken = new CToken();
	}
	virtual void setText(const char* szInput,basis::n32_t nSLen)
    {
        code_utf8_to_gbk(m_szInput, DEFAULT_BUFFER_LENGTH, szInput, CC_FLAG_IGNORE);
	   // m_szInput = szInput;
		m_nSLen = strlen(m_szInput);
	    m_nStart = 0;
	    if (m_pToken != NULL) {
	        m_pToken->m_nFeatureMask = 0;
	        m_pToken->m_nStartOffset = 0;
		    m_pToken->m_nEndOffset = 0;
		    m_pToken->m_nSerial = 0;
		    m_pToken->m_pText = NULL;
		    m_pToken->m_category = -1;
	    }
	}
	/* ϡ������ */
	virtual ~CTokenizer(void)
	{
		delete m_pToken;
		m_pToken = NULL;
	}

protected:
	/* ���ڷִʵ�ԭ�ַ��� */
	char m_szInput[DEFAULT_BUFFER_LENGTH];
	/* ����ֽ������token��Ϣ,�ɹ��캯������ */
	CToken *m_pToken;
	/* ��ʼ�ֽ�token��Դ�ַ�����λ�� */
	basis::n32_t m_nStart;
	/* ����ִ�Դ�ַ������� */
	basis::n32_t m_nSLen;
};

}
#endif

