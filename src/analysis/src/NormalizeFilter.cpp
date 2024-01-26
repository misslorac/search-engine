#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "NormalizeFilter.h"
#include "StringUtility.h"
#include "BasisDef.h"

using namespace utility;

namespace analysis {

/* ������һ��token */
CToken *CNormalizeFilter::next()
{	
	if(!m_pInput)
		return NULL;

	CToken *pToken = m_pInput->next();

	if (NULL == pToken)
		return NULL;
	//�Ƿ���������Ŀո�,��˫�ֽڵĿո���
	char *ptr, *szToken;
	if ((ptr=strstr(pToken->m_szText,"  ")) != NULL || (ptr=strstr(pToken->m_szText, "����")) != NULL) {
		szToken = string_utility::replicate(pToken->m_szText);
		/*size_t nPos = (ptr - pToken->m_szText);	
        //������һ���ո��λ��
        if (szToken[nPos] == ' ')
            nPos++;
        else 
            nPos += 2;*/
        size_t nPos = 0;

        //Ϊ�˷�ֹ����һ�����ֵĵڶ��ֽںͺ�һ�����ֵ�ͷ�ֽڶ�ΪA1������������ͷ��ʼɨ��
		bool bSpace = false;
		for (size_t k = nPos; k < pToken->m_nTextLen; k++) {
			if (szToken[k] == ' ' || ((unsigned char)szToken[k]==0xA1 && (unsigned char)szToken[k+1]==0xA1)) {
				if ((unsigned char)szToken[k] == 0xA1)	//˫�ֽ�
					k ++;
				if (!bSpace) {
					pToken->m_szText[nPos++] = ' ';	//���ܴ��������Ŀո�
					bSpace = true;
				}
				continue;
			}
			pToken->m_szText[nPos++] = szToken[k];
			if (string_utility::isChinese(szToken[k]))
				pToken->m_szText[nPos++] = szToken[++k];
			bSpace = false;
		}
		delete[] szToken;
		pToken->m_szText[nPos] = '\0';
		pToken->m_nTextLen = nPos;
	}

	return pToken;
}

}
