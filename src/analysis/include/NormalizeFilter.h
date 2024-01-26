/******************************************************************************
 *
 * File     :NormalizeFilter.h
 * Version  :$Id: NormalizeFilter.h,v 1.9 2005/03/29 09:19:24 victor Exp $
 * Desc     :һ����token���б�׼����token,��׼���Ĺ�������ȥ�������Ŀո����д
			 ����һЩ�仯,����·�ƺŶ�ͳһΪ���������ֵ�
 * Log      :Create by victor, 2004-12-24
 *
 ******************************************************************************/
#ifndef NORMALIZE_FILTER_H
#define NORMALIZE_FILTER_H

#include "TokenFilter.h"

namespace analysis {

class CToken;
class CTokenStream;

class CNormalizeFilter : public CTokenFilter
{
public:
	/* ���캯��,Ŀǰ��׼��ֻ�ǽ������Ŀո�ȥ�� */
	CNormalizeFilter(CTokenStream *pInput) : CTokenFilter(pInput) {}
	/* ϡ������ */
	~CNormalizeFilter(void) {}
	/* ������һ��token */
	CToken *next();

};

}

#endif
