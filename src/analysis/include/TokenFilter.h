#ifndef TOKENFILTER_H
#define TOKENFILTER_H

#include "TokenStream.h"

namespace analysis {

class CToken;

class CTokenFilter : public CTokenStream
{
public:
	/* ���캯�� */
	CTokenFilter(CTokenStream *pInput)
	{
		pNext = pInput;
		m_pInput = pInput;
	}
	/* ϡ������ */
	virtual ~CTokenFilter(void) {}

protected:
	CTokenStream *m_pInput;	//����

};

}  //end namespace

#endif
