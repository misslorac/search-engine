/*
 *	GBK���������ַ����嵽����ת��������
 */
#ifndef GBK_T2GBK_S_FILTER_H
#define GBK_T2GBK_S_FILTER_H

#include "TokenFilter.h"

namespace analysis {

class CGBK_T2GBK_SFilter : public CTokenFilter
{
public:
	/* ���캯�� */
	CGBK_T2GBK_SFilter(CTokenStream *pInput) : CTokenFilter(pInput) {}
	/* ϡ������ */
	~CGBK_T2GBK_SFilter(){}
	/* ������һ��token */
	CToken *next();
	/* GBK���ַ�תΪ���ֽڽڷ� */
	static long ConvertTo(const char *pchFrom, char *pchTo);

protected:
	/* ����ƫ���� */
	static inline int GBKTableOffset(unsigned char c1, unsigned char c2);
	/* GBK�ַ������ֽ� */
	static inline bool isGBK1(unsigned char c);
	/* GBK�ַ������ֽ� */
	static inline bool isGBK2(unsigned char c);
	/* �����ַ����ж� */
	static inline bool isChineseCharacter(unsigned char c1, unsigned char c2);
};

}
#endif
