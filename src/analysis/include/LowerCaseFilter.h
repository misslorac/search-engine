#ifndef LOWER_CASE_FILTER_H
#define LOWER_CASE_FILTER_H

#include "TokenFilter.h"

namespace analysis {

class CToken;
class CTokenStream;

class CLowerCaseFilter : public CTokenFilter
{
public:
	/* ���캯�� */
	CLowerCaseFilter(CTokenStream *pInput) : CTokenFilter(pInput){}
	/* ��ϡ�� */
	~CLowerCaseFilter(void){}
	/* ������һ��token */
	CToken *next();
};

}

#endif
