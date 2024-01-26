/******************************************************************************
 *
 * File     :Token.h
 * Version  :$Id: Token.h,v 1.32 2007/01/22 07:01:22 regerett Exp $
 * Desc     :��һ���ַ����зֽ��������С��λ,Ҳ��֮Ϊterm,ͬʱҲ�������ڷֽ⴮��
 *			��λ����Ϣ�����term���͵ı�ʶ
 *
 * Log      :Created by yusiheng, 2004-1-11
 *
 ******************************************************************************/
#ifndef TOKEN_H
#define TOKEN_H

#include <string.h>
#include <stdio.h>
#include "BasisDef.h"
#include "charset_func.h"

namespace analysis 
{

class CToken
{
public:
	static const basis::u_n16_t IS_FINISHWORD_MASK = 0x1;		//��1λ�Ƿ�����Field�����һ��Loc����
	static const basis::u_n16_t IS_FIRSTWORD_MASK = 0x2;		//��2λ�Ƿ�����Field�ĵ�һ��Loc����

public:
	/* ���캯�� */
	CToken(void)
	{
		m_nStartOffset = 0;
		m_nEndOffset = 0;
		m_nSerial = 0;
		m_pText = NULL;
		m_category = -1;
	}
	/* ϡ������ */
	virtual ~CToken(void)
	{}

/* Ϊ�������ϵĿ���,��m_szText��Ϊpublic,����ֱ�ӷ��� */
public:
	basis::u_n32_t m_nStartOffset;					//��ʼλ��,��ԭʼ�ִ���ʼ��ƫ��λ��
	basis::u_n32_t m_nEndOffset;						//����λ��,��ԭʼ�ִ���ʼ��ƫ��λ��
	basis::u_n32_t m_nSerial;							//Token���,Ҳ��Ϊtoken��λ��,����ȷ������token�Ƿ��ڽ�
	basis::u_n16_t m_nFeatureMask;					//������,����ĳ�ԱֵΪ1��ʾ�и����Ա����
	size_t m_nTextLen;							//�ֽ����token����
	char m_szText[MAX_TOKEN_LENGTH + 1];		//����ֽ����token,����Ҫ�Թؼ�����ת��,��Ҫ����Դ�ַ���
	const char *m_pText;						//ָ��Դ�����ַ�����ָ��,����stopFilter��������Ҫ�����ĵĹ��˹���
	basis::n32_t m_category;      //��Ŀ

/* TokenStream��Token�ķ��ʽӿ� */
public:
	CToken& operator =(const CToken& token)
	{
    	if (this == &token) 
        	return *this;
		m_nStartOffset = token.m_nStartOffset;
		m_nEndOffset = token.m_nEndOffset;
		m_nSerial = token.m_nSerial;
		m_nFeatureMask = token.m_nFeatureMask;
		m_nTextLen = token.m_nTextLen;
		strncpy(m_szText, token.m_szText, token.m_nTextLen);
		m_szText[m_nTextLen] = 0;
		m_pText = token.m_pText;
		m_category = token.m_category;
		return *this;
	}
	void print()
	{
		printf("m_nStartOffset = %d\n", m_nStartOffset);
		printf("m_nEndOffset = %d\n", m_nEndOffset);
		printf("m_nSerial = %d\n", m_nSerial);
		printf("m_nFeatureMask = %d\n", m_nFeatureMask);
		printf("m_nTextLen = %d\n", m_nTextLen);
		printf("m_szText = %s\n", m_szText);
		printf("m_pText = %s\n", m_pText);
		printf("m_category = %d\n", m_category);
	}
	/* ����token��textֵ,��token�������������ʻ����,����֮�������ո����ֻ����Ϊһ�� */
	void setText(const char* szText, size_t nTextLen)
	{
		m_pText = szText;
		if (nTextLen > MAX_TOKEN_LENGTH) {	//�����token��������
			nTextLen = MAX_TOKEN_LENGTH;
		}
        int k = 0;
        bool bSpace = false;
        for (basis::u_n32_t i = 0; i < nTextLen; i++) {
        	if( k==0&&szText[i]==' ' )
        		continue;
            if (szText[i]==' '){
                if (bSpace){   //ȥ�������Ŀո�
                	continue;
                }
                else {
                    m_szText[k++] = ' ';
                    bSpace = true;
                }
           }
           else {
           	m_szText[k++] = szText[i];
           	bSpace = false;
          }
     }
     if(bSpace)
     	k--;
     m_szText[k] = 0;
	 m_nTextLen = k;
    }
	/*�������*/
	void incrementPos()
	{
		m_nSerial++;
	}
	/* ÿ��Ҫ��Token����set���ݵ�ʱ��,����Ҫ�ȵ����������,��ԭ����feature��ֵ��� */
	void clearFeature()
	{
		m_nFeatureMask = 0;
	}
	/* ��ĳ��feature��ֵ,��feature��mask���� or ����*/
	void setFeature(basis::u_n16_t mask)
	{
		m_nFeatureMask |= mask;
	}
    void toUTF8()
    {
        char szTmp[MAX_TOKEN_LENGTH+1];
        code_gbk_to_utf8(szTmp, MAX_TOKEN_LENGTH+1, m_szText, CC_FLAG_IGNORE);
        strncpy(m_szText, szTmp, strlen(szTmp));
        m_szText[strlen(szTmp)] = 0;
    }
};

} //end namespace analysis

#endif
