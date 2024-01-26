/******************************************************************************
*
* File     :UpperCaseFilter.h
* Version  :$Id: UpperCaseFilter.h v 0.1  2009/09/07  09:10  
* Desc     :��Tokenת���ɴ�д
*			
* Log      :
*
******************************************************************************/

#ifndef UPPER_CASE_FILTER_H
#define UPPER_CASE_FILTER_H

#include "TokenFilter.h"

namespace analysis {

class CToken;
class CTokenStream;

class CUpperCaseFilter : public CTokenFilter
{
public:
	/* ���캯�� */
	CUpperCaseFilter(CTokenStream *pInput) : CTokenFilter(pInput){}
	/* ϡ������ */
	~CUpperCaseFilter(void){}
	/* ������һ��token */
	CToken *next();
};

}

#endif
