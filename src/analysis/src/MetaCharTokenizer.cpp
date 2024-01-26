#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Token.h"
#include "StringUtility.h"
#include "MetaCharTokenizer.h"
#include "charset_func.h"

using namespace basis;
using namespace utility;
using namespace string_utility;

namespace analysis { 
	// ����ȫ���ַ��ӵ�IGNORE_CHAR�У����ַ������Ӣ���ַ������ֵַ�һ��
	static const char *IGNORE_CHAR="&+'��������";

	/* ��ʼ���캯�� */
	CMetaCharTokenizer::CMetaCharTokenizer(const char* szSentence) : CTokenizer(szSentence)
	{
		m_pToken->m_nSerial = 0;	//��Ŵ��㿪ʼ
	}

	CMetaCharTokenizer::CMetaCharTokenizer()
	{
		m_pToken->m_nSerial = 0;   //��Ŵ��㿪ʼ
	}

	/* ����ʵ�ֻ���Ĵ��麯��
	 * ��������ASCII�ǿո��ַ���˫�ֽ��ַ���˫�ֽ�������Ϊһ��term���� */
	CToken *CMetaCharTokenizer::next(void)
	{
		if (m_szInput == NULL)
			return NULL;

		n32_t nLen = m_nSLen ? m_nSLen : static_cast<n32_t>(strlen(m_szInput));
		if (nLen == 0 || m_nStart >= nLen || m_nStart < 0)
			return NULL;

		n32_t nTokenLen = 0;
		int iEndFlag = -1;
		m_pToken->clearFeature();

		while (m_nStart < nLen) {
			if (m_szInput[m_nStart] == 0) {
				m_nStart++;
				continue;
			}

			iEndFlag = -1;

			if (isWholeAlpha(m_szInput + m_nStart) || isIgnore(m_szInput + m_nStart)) {
				m_pToken->m_nStartOffset = m_nStart;
				bool bIncomplete = true;;
				do {
					if ('&' == m_szInput[m_nStart]) {
						size_t quoted = isQuoted(m_szInput + m_nStart);
						if (quoted) {
							bIncomplete = false;
							m_nStart += quoted;
							iEndFlag = -1;
							continue;
						}
					}
					if (isWholeAlpha(m_szInput + m_nStart)) {
						bIncomplete = false;
					}
					else if (isIgnore(m_szInput + m_nStart)) {
					}
					else {
						break;
					}

					m_nStart += Utf8Width(m_szInput + m_nStart, basis::sc_GBK);
				} while(m_nStart < nLen);

				// ���ȫΪ���ţ������
				if (bIncomplete) {
					m_pToken->m_nStartOffset = m_nStart;
					continue;
				}
			}
			else if (isWholeNum(m_szInput + m_nStart)) {
				m_pToken->m_nStartOffset = m_nStart;
				bool bDecimal = false;
				do {
					if (isWholeNum(m_szInput + m_nStart)) {
						m_nStart += Utf8Width(m_szInput + m_nStart, basis::sc_GBK);
						continue;
					}
					// �ж��Ƿ�ΪС����.��/��ǰ�����������һ��������ǰ��û�г��ֹ�.��/������С��
					else if ((m_szInput[m_nStart] == '.' || m_szInput[m_nStart] == '/')
							&& !bDecimal && m_nStart<(nLen-1) && isWholeNum(m_szInput+m_nStart+1)) {
						m_nStart++;
						bDecimal = true;
						continue;
					}
					else {
						break;
					}
				} while(m_nStart < nLen);
			}
			else if (isChineseWord(m_szInput + m_nStart)) {
				m_pToken->m_nStartOffset = m_nStart;

				m_nStart += Utf8Width(m_szInput + m_nStart, basis::sc_GBK);

				if (isChineseInterpunction((unsigned char *)(char *)(m_szInput + m_pToken->m_nStartOffset))) {
					// ���ı��term id���ܼ�1��������sb-600��ʱ�򣬲����sb��6000
					//m_pToken->incrementPos();
					continue;
				}
			}
			else {
				m_pToken->m_nStartOffset = m_nStart;
				m_nStart += Utf8Width(m_szInput + m_nStart, basis::sc_GBK);
				continue;
			}

			m_pToken->incrementPos();
			m_pToken->m_nEndOffset = iEndFlag >= 0 ? iEndFlag : m_nStart;
			m_pToken->setText(m_szInput + m_pToken->m_nStartOffset, m_pToken->m_nEndOffset - m_pToken->m_nStartOffset);
			if (m_pToken->m_nEndOffset - m_pToken->m_nStartOffset <= 0)
				continue;
/*            char szTmp[MAX_TOKEN_LENGTH+1];
            code_gbk_to_utf8(szTmp, MAX_TOKEN_LENGTH+1, m_pToken->m_szText, CC_FLAG_IGNORE);
            strncpy(m_pToken->m_szText, szTmp, strlen(szTmp));*/
            
			return m_pToken;
		}
		return NULL;
	}

	/*
	 *	������Щ����endword,���Լ������
	 *  Ӣ��: "," �ո� ":" "-"
	 *  ����: ���ı����� ȫ�Ƿֺ� ȫ�ǿո� "��" "��" "��" "��"  "����" "����" "��ͼ" "(ͼ" "����"
	 */
	bool CMetaCharTokenizer::hasEndWord(n32_t nStartOfferset) const
	{
		n32_t nInputLen = static_cast<n32_t>(strlen(m_szInput));
		if (nStartOfferset >= nInputLen) 
		{
			return true;
		}
		unsigned char c1,c2;
		c1 = static_cast<unsigned char>(m_szInput[nStartOfferset]);
		c2 = static_cast<unsigned char>(m_szInput[nStartOfferset+1]);
		if (isChinese(c1)) {
			if (c1==0xA1 || (c1==0xA3&&!((c2>=0xB0&&c2<=0xB9)||(c2>=0xC1&&c2<=0xDA)||(c2>=0xE1&&c2<=0xFA))) || 
					c1==0xBA&&c2==0xCD || c1==0xA9&&c2==0x72 ||						//�� ��
					c1==0xBC&&c2==0xB0 || c1==0xBB&&c2==0xF2 ||						//�� ��
					c1==0xD7&&c2==0xD3 || c1==0xA9&&c2==0x6F || c1==0xB5&&c2==0xC8)			//�� �� ��
				return true;
			else if((nStartOfferset+2)<nInputLen && isChinese(m_szInput[nStartOfferset+2]) && 
					(c1==0xB4&&c2==0xFA || c1==0xBA&&c2==0xCF || c1==0xA3&&c2==0xA8 || c1==0xB3&&c2==0xCF)) {//�� �� �� ��
				c1 = (unsigned char)m_szInput[nStartOfferset+2];
				c2 = (unsigned char)m_szInput[nStartOfferset+3];
				if (c1==0xC0&&c2==0xED || c1==0xD7&&c2==0xF7 || c1==0xCD&&c2==0xBC || c1==0xD5&&c2==0xD0)//�� �� ͼ ��
					return true;
			}
		} else if (c1==',' || c1==' ' || c1==';' || c1 =='-' || c1=='.') {
			return true;
		} else if (c1=='(' && (nStartOfferset+2)<nInputLen && isChinese(c2)) {
			if (c1==0xCD&&c2==0xBC)		// (ͼ
				return true;
		}
		return false;
	}

	bool CMetaCharTokenizer::isIgnore(const char *pchChar) 
	{
		if (!pchChar || *pchChar == '\0')
			return false;

		if (isChinese(*pchChar))
			return wStrWChr(IGNORE_CHAR, pchChar);
		else 
			return wStrChr(IGNORE_CHAR, *pchChar);
	}

	bool CMetaCharTokenizer::isChineseWord(const char *pchChar) 
	{
		if (!pchChar || *pchChar == '\0')
			return false;

		if (!isChinese(*pchChar))
			return false;
		if (isChineseAlphaNum(pchChar[0], pchChar[1]))
			return false;
		return true;
	}

	const char* CMetaCharTokenizer::wStrWChr(const char *pWstr, const char *pchChar) {
		if (NULL == pWstr || NULL == pchChar)
			return NULL;

		const char *pPtr = pWstr;
		while (*pPtr) {
			if (*pPtr & 0x80) {
				if (strncmp(pPtr, pchChar, 2) == 0) {
					return pPtr;
				}
				else {
					pPtr += 2;
				}
			}
			else {
				pPtr += 1;
			}
		}
		return NULL;
	}

	const char* CMetaCharTokenizer::wStrChr(const char *pWstr, const char cChar) {
		if (NULL == pWstr || cChar < 0)
			return NULL;

		const char *pPtr = pWstr;
		while (*pPtr) {
			if (*pPtr & 0x80) {
				pPtr += 2;
			}
			else {
				if (*pPtr == cChar) {
					return pPtr;
				}
				else {
					pPtr += 1;
				}
			}
		}
		return NULL;
	}

	// ֻҪ����&��ͷ��;��β���м�ΪӢ���ַ����ҳ�����4-6֮��ģ���������htmlת���ַ�
	size_t CMetaCharTokenizer::isQuoted(const char *szStr) {
		if (NULL == szStr || '&' != szStr[0]) 
			return 0;

		char ch[7];
		strncpy(ch, szStr, 6);
		ch[6] = '\0';

		size_t i = 1;
		while (ch[i]) {
			if (';' == ch[i] ) {
				break;
			}
			if (!isAlpha(ch[i])) {
				return 0;
			}
			i++;
		}
		if (i >=3 && i <= 5) {
			return i+1;
		}
		else {
			return 0;
		}
	}

}

