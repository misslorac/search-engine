#ifndef __UNITYFILTER_H__
#define __UNITYFILTER_H__

#include "TokenFilter.h"
//#include "dictionary/DictionaryHelper.h"

namespace analysis 
{
	class CTokenStream;
	
	class CUnityFilter :public CTokenFilter 
        {
	public:
		/*���캯��*/
		CUnityFilter(CTokenStream *pInput);
		/*��������*/
		~CUnityFilter();
		/* ������һ��token */
		CToken *next();
		/*�����ֵ���*/
//		void setDictHelper(dictionary::CDictionaryHelper * pDictHelper);

//	private:
//		dictionary::CDictionaryHelper *m_DictHelper;
	};
}

#endif

