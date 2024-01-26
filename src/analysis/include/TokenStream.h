/******************************************************************************
*
* File     :TokenStream.h
* Version  :$Id: TokenStream.h,v 1.8 2005/03/29 02:54:32 jamesyu Exp $
* Desc     :token���Ľӿ���
*			
* Log      :Created by yusiheng, 2004-1-11
*
******************************************************************************/

#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

namespace analysis 
{
class CToken;

class CTokenStream
{
protected:
	CTokenStream *pNext;		//ָ����һ��tokenStream

public:
	/* ���캯�� */
	CTokenStream(void)
	{
		pNext = NULL;
	}
	/*�յ���������*/
	virtual ~CTokenStream(void)
	{
		if (pNext != NULL) {
			delete pNext;
		}
	}
	/* ���麯��,ȡ����һ��token�����ָ�� */
	virtual CToken* next() = 0;
};

} //end namespace

#endif
