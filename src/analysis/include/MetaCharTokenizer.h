/******************************************************************************
 *
 * File     :MetaCharTokenizer.h
 * Version  :$Id: MetaCharTokenizer.h,v 1.8 2006/12/19 02:41:39 kwq Exp $
 * Desc     :���ַ������зֽ����,�ֽ��һ��һ������,��Ӣ�ĵ��ʶ���,�Ƿֽ�ɵ���
 *			 �ĵ���,�Ժ��ֶ���,�Ƿֽ����,�Էֽ��������,��Ҫ�ж���token��type
 * Log      :Created by yusiheng, 2005-1-11
 *
 ******************************************************************************/

#ifndef _META_CHAR_TOKENIZER_H_
#define _META_CHAR_TOKENIZER_H_

#include "Tokenizer.h"

namespace analysis {

class CToken;

class CMetaCharTokenizer : public CTokenizer
{
public:
	/* ��ʼ�����캯�� */
	CMetaCharTokenizer(const char* szSentence);
	CMetaCharTokenizer();
	/* �������� */
	~CMetaCharTokenizer(void){};
	/* ����ʵ�ֻ���Ĵ��麯�� */
	CToken* next(void);

private:
	/*
	 * ���ַ���nStartOffersetλ��֮����ַ�,�Ƿ�ΪEndWord
	 * ����EndWord�ĸ���Ϊ,�����д�֮ǰΪEndWord
	 *  Ӣ��: "," �ո� ":" "-"
	 *  ����: ���ı����� ȫ�Ƿֺ� ȫ�ǿո� "��" "��" "��" "��"  "����" "����" "��ͼ" "(ͼ" "����"
	 */
	bool hasEndWord(basis::n32_t nStartOfferset) const;
	bool isTrim(const char *pchChar);
	bool isSplit(const char *pchChar);
	bool isIgnore(const char *pchChar);
	bool isChineseWord(const char *pchChar);
	const char* wStrWChr(const char *pWstr, const char *pchChar);
	const char* wStrChr(const char *pWstr, const char cChar);
    size_t isQuoted(const char *szStr);
};

}

#endif

