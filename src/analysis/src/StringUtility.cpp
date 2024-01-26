#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <BasisDef.h>
#include "StringUtility.h"
//#include "HtmlCodeReplacer.h"

using namespace basis;

namespace utility {

  namespace string_utility {
	/* �����ַ���,�����ڲ������ڴ�ռ�,�����߱��븺���ͷŷ��ص��ַ��� */
	char* replicate(const char* szStr) {
		if (szStr == NULL)
			return NULL;
		size_t len = strlen(szStr);
		char *szRet = new char[len+1];
        assert(len+1 > 0);
		strcpy(szRet, szStr);

		return szRet;
	}
	/* �����ַ���,�����ڴ���������, �����߲��踺�𷵻��ַ������ڴ��ͷ� */
/*	char* replicate(const char* szStr, SHeap *pHeap, basis::n32_t nLength) {
		if (szStr == NULL)
			return NULL;
		size_t len = 0;
		if (nLength == 0) {
		     len = strlen(szStr);
		} else {
		     len = nLength;
		}
		char *szRet = (char*)suHeapAlloc(pHeap, sizeof(char)*(len+1), sizeof(char*));
		memcpy(szRet, szStr, len);
		szRet[len] = '\0';
		return szRet;
	}
	char* replicate(const char* szStr, MemPool *pHeap, basis::n32_t nLength) {
		if (szStr == NULL)
			return NULL;
		size_t len = 0;
		if (nLength == 0) {
		     len = strlen(szStr);
		} else {
		     len = nLength;
		}
		char *szRet = (char*)NEW_VEC(pHeap, char, len+1);
		memcpy(szRet, szStr, len);
		szRet[len] = '\0';
		return szRet;
	}*/
	/* ��һ��long�͵�ֵת��Ϊ�ַ����������ڴ����������������߲��踺�𷵻��ַ������ڴ��ͷ� */
/*	char* longtostr(long n, SHeap *pHeap)
	{
		char *szBuf = (char*)suHeapAlloc(pHeap, sizeof(long)*8+1, sizeof(char*));
		sprintf(szBuf, "%ld", n);
		return szBuf;
	}
	char* longtostr(long n, MemPool *pHeap)
	{
		char *szBuf = (char*)NEW_VEC(pHeap, char, sizeof(long)*8+1);
		sprintf(szBuf, "%ld", n);
		return szBuf;
	}

	char* inttostr(basis::n32_t n, MemPool *pHeap)
	{
		char *szBuf = (char*)NEW_VEC(pHeap, char, sizeof(int)*8+1);
		sprintf(szBuf, "%d", n);
		return szBuf;
	}

	char* doubletostr(double d, MemPool *pHeap)
	{
		char *szBuf = (char*)NEW_VEC(pHeap, char, sizeof(double)*8+1);
		sprintf(szBuf, "%f", d);
		return szBuf;
	}*/
	/*
 	 * ��װ�ַ���ת�����ͺ���
 	 */
	basis::n64_t strtolong(const char* szText)
	{
#ifdef _WIN32
		return _atoi64(szText);
#else
		return atoll(szText);
#endif
	}
	/* �����ַ���,ȥ���ַ���ǰ��Ŀո�,������Ҫ�Լ��ͷŷ����ַ���delete[] result; */
	char* trim(const char* szStr) {
		const char *szTmp;
		char *szRet, *szTrim;

		szTmp = szStr;
		while (isSpace(*szTmp))
			szTmp++;
		size_t len = strlen(szTmp);
		szTrim = replicate(szTmp);		//����пո�
		if (len > 0 && isSpace(szTrim[len-1])) {
			szRet = &szTrim[len-1];
			while (isSpace(*szRet))
				szRet--;
			*(++szRet) = '\0';
		}

		return szTrim;
	}
	/* strtok, �зֺ������res��������Ҫ�Լ��ͷŷ��ص������е�ÿһ���ַ��� */
/*	void strtok(CSvector<char *>& res, char *szInput, int len, char *szChars) {
		res.clear();
		char *p1 = szInput;
		while (*p1 != '\0') {
			char *p2 = szChars;
			//������Ԥ��������зֵ��ַ����Ϊ\0
			while(*p2 != '\0') {
				if (*p1 == *p2) {
					*p1 = '\0';
					break;
				}
				p2++;
			}
			p1++;
		}

		p1 = szInput;
		while(p1 < szInput + len) {
			//�������Ϊ\0���ַ�
			if(*p1 == '\0') { p1++; continue; }
			char *e = replicate(p1);
			res.push_back(e);
			//�����Ѿ�������ַ���
			while (*p1!='\0' && p1 < szInput + len) { p1++; }
		}
	}
*/
	char * strtok_r(char *s1, const char *s2, char **lasts)
	{
		char *ret;
		if (s1 == NULL)
			s1 = *lasts;
		while(*s1 && strchr(s2, *s1))
			++s1;
		if(*s1 == '\0')
			return NULL;
		ret = s1;
		while(*s1 && !strchr(s2, *s1))
			++s1;
		if(*s1)
			*s1++ = '\0';
		*lasts = s1;
        return ret;
    }

/*    void split(char* str, char* delim, CSvector<char*> &list)
    {
        if (str == NULL)
        {
            return;
        }
        if (delim == NULL)
        {
            list.push_back(str);
            return;
        }

        char *s, *spanp;

        s = str;
        while(*s)
        {
            spanp = delim;
            while(*spanp)
            {
                if (*s == *spanp)
                {
                    list.push_back(str);
                    *s = '\0';
                    str = s+1;
                    break;
                }
                spanp ++;
            }
            s ++;
        }
        if (*str)
        {
            list.push_back(str);
        }
    }*/

   /*  
    * ���ַ���str�в���substr�ִ�
    */ 
    char* _strstr(char* str, char* substr, size_t len)
    {   
        size_t pos = 0;
        char *s1, *s2;
        if ( !*substr )
            return(str);
        while (pos < len)
        { 
            s1 = &str[pos];
            s2 = substr;
            size_t pos2 = 0;
            while ((pos + pos2) < len && s1[pos2] && *s2 && !(s1[pos2]-*s2))
                pos2++, s2++;
            if (!*s2)
                return(s1);
            pos++;
        }
        return(NULL); 
        
    }

	/* �ַ�����ϣ����,����Ϊ�ַ��� */
	unsigned int hash(const char *szStr) {
		unsigned int res = 0;
		const unsigned char *p;

		p = reinterpret_cast<const unsigned char *>(szStr);
		while (*p) {
			res = (res << 7) + (res >> 25) + *p++;
		}

		return res;
	}
	/* �̶������ֽ�����ϣ����,����Ϊ�ֽ�buffer���ֽڳ��� */
	unsigned int hash(const char* szBuf, int nLen) {
		unsigned int res = 0;
		const unsigned char *p;

		p = reinterpret_cast<const unsigned char *>(szBuf);
		while ((nLen--) > 0) {
			res = (res << 7) + (res >> 25) + *p++;
		}

		return res;
	}
	unsigned int hashString(const char *szStr, unsigned int dwHashType)
	{
		if (szStr == NULL) 
			return 0;
		unsigned int cKey;
		unsigned char *key = (unsigned char *)szStr;
		unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;

		while (*key != 0) {
			cKey = *key++;
			seed1 = CRYPT_TABLE[(dwHashType << 8) + cKey] ^ (seed1 + seed2);
			seed2 = cKey + seed1 + seed2 + (seed2 << 5) + 3;
		}

		return seed1;
	}
	unsigned int hashString(const char *szStr, unsigned int nLen, unsigned int dwHashType)
	{
		if (szStr == NULL || nLen == 0) 
			return 0;
		unsigned int cKey;
		unsigned char *key = (unsigned char *)szStr;
		unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
		unsigned int i = 0;
		while (i < nLen) {
			cKey = *key++;
			seed1 = CRYPT_TABLE[(dwHashType << 8) + cKey] ^ (seed1 + seed2);
			seed2 = cKey + seed1 + seed2 + (seed2 << 5) + 3;
			i++;
		}
		return seed1;
	}
	basis::u_n64_t hashString64(const char *szStr)
	{
		basis::u_n64_t h = hashString(szStr, 0);
		h <<= 32;
		h |= hashString(szStr, 1);
		return h;
	}
	basis::u_n64_t hashString64(const char *szStr, unsigned int nLen)
	{
		basis::u_n64_t h = hashString(szStr, nLen, 0);
		h <<= 32;
		h |= hashString(szStr, nLen, 1);
		return h;
	}
	/* �ж������ַ����Ƿ�������ͬ,���ִ�Сд */
	bool equal(const char* szStr1, const char* szStr2) {
		assert(szStr1&&szStr2);
		return strcmp(szStr1, szStr2) == 0;
	}
	/* ������ķ�����ͬ������Ҫ�Ƚ�str1��str2�Ƿ�Ϊnull */
	bool safe_equal(const char* szStr1, const char* szStr2) {
	  	if(szStr1 == NULL || szStr2 == NULL)
		  	return false;
	  	return strcmp(szStr1, szStr2) == 0;
	}
	/* �ж������ַ����Ƿ�������ͬ,�����ִ�Сд */
	bool equalNoCase(const char* szStr1, const char* szStr2) {
		assert(szStr1&&szStr2);
		/*char *s1 = replicate(szStr1);
		char *s2 = replicate(szStr2);
		toLower(s1);
		toLower(s2);
		bool bRet = equal(s1, s2);
		delete[] s1;
		delete[] s2;
		return bRet;
		*/
#ifndef _WIN32
		return strcasecmp(szStr1, szStr2) == 0;
#else
        return stricmp(szStr1, szStr2) == 0;
#endif
	}
	/* �ж������ַ����Ƿ�Ϊ���� */
	bool isInteger(const char* szStr) {
		while(*szStr) {
			if (!isNum(*szStr++))
				return false;
		}
		return true;
	}
	/* �������ַ������������,���Ƶ��µ��ַ���,�����߱��븺���ͷźϲ�����ַ��� */
	char* mergeString(const char* szStr1, const char* szStr2) {
		assert(szStr1&&szStr2);
		size_t len1 = strlen(szStr1);
		size_t len2 = strlen(szStr2);
		char* szRet = new char[len1 + len2 + 1];
		strcpy(szRet, szStr1);
		strcpy(szRet+len1, szStr2);
		return szRet;
	}
/*	char* mergeString(const char* szStr1, const char* szStr2, MemPool *mp) {
		assert(szStr1&&szStr2);
		size_t len1 = strlen(szStr1);
		size_t len2 = strlen(szStr2);
		char* szRet = NEW_VEC(mp, char, len1 + len2 + 1);
		strcpy(szRet, szStr1);
		strcpy(szRet+len1, szStr2);
		return szRet;
	}*/
	/* �ж��ַ����Ƿ�ȫΪASCII�ַ� */
	bool isAllASCII(const char* szStr) {
		if (szStr == NULL)
			return false;
		/*
		size_t nLen = strlen(szStr);
		for (size_t i = 0; i < nLen; i++) {
			if (szStr[i] <= 0x00 || szStr[i] >= 0x7F) {
				return false;
			}
		}
		*/
		while (*szStr)
		{
			if (*szStr <= 0x00 || *szStr >= 0x7F) {
				return false;
			}
			szStr++;
		}
		return true;
	}
	/* �ж��ַ����Ƿ�ȫΪGBK�����ַ� */
	bool isAllGBK(const char* szStr) {
		if (szStr == NULL)
			return false;
		size_t nLen = strlen(szStr);
		if (nLen % 2 != 0)
			return false;
		for (size_t i = 0; i < nLen; i+=2) {
			if ((unsigned char)szStr[i]<=0xAF || (unsigned char)szStr[i]>=0xF8) {
				return false;
			}
		}
		return true;
	}
	/* ���ַ����е�ÿ���ַ���תΪ��д�ַ�,ͨ������ָ��ı�ԭֵ */
	void toUpper(char* szStr) {
		if (szStr == NULL)
			return;
		/*
		size_t nLen = strlen(szStr);
		for (size_t i = 0; i < nLen; i++) {
			if ((szStr[i] & 0x80))
				i ++;
			else
				szStr[i] = toupper(szStr[i]);
		}
		*/
		size_t i = 0;
		while(szStr[i]) {
			if ((szStr[i] & 0x80))
				i += (szStr[i+1] ? 2 : 1);
			else{
				szStr[i] = toupper(szStr[i]);
				i++;
			}
		}
	}
	/* ���ַ����е�ÿ���ַ���תΪСд�ַ� */
	void toLower(char* szStr) {
		if (szStr == NULL)
			return;
		/*
		size_t nLen = strlen(szStr);
		for (size_t i = 0; i < nLen; i++) {
			if ((szStr[i] & 0x80))//˫�ֽ�
				i ++;
			else
				szStr[i] = tolower(szStr[i]);
		}
		*/
		size_t i = 0;
		while(szStr[i]) {
			if ((szStr[i] & 0x80))
				i += (szStr[i+1] ? 2 : 1);
			else{
				szStr[i] = tolower(szStr[i]);
				i++;
			}
		}
	}
	/* ���ַ�����ĳ���ַ�(cRes)�滻Ϊָ�����ַ�(cDest), �������滻����ַ���, �����߸����ͷ��滻����ַ��� */
	char* replace(const char* szStr, char cRes, char cDest) {
		char *szPtr, *szRep;

		szRep = replicate(szStr);
		while ((szPtr = strchr(szRep, cRes)) != NULL) {
			*szPtr = cDest;
		}

		return szRep;
	}
	/* ����һ��html�ĵ��е�html��ǩ�������ع��˺���ַ������ȣ������߸����ͷ��滻����ַ��� add by chucai 2007.10.15 */
/*	basis::n32_t filterHtml(char * szHtml,basis::n32_t nLen,bool bTrans, bool bTaobao) {
		//������Ϳ��ַ���������NULL
		if (!szHtml || nLen == 0) return 0;
		basis::n32_t j = 0;
		CHtmlCodeReplacer * hcr = CHtmlCodeReplacerCreator::makeHtmlCodeReplacer(bTaobao?"taobao":"dfl");
		for (basis::n32_t i = 0; i < nLen; ) {
			//�������<���ţ�ֱ���ҵ������ַ��˳�ѭ��
			if (szHtml[i] == '<') {
				while (i < nLen) {
					if ((szHtml[i++] == '>') && (szHtml[i] != '<')) break;
				}
			}
			else if ((szHtml[i] == '&') && bTrans)
			{
				int replacelen = hcr->CodeReplace((unsigned char *)(szHtml + i),(unsigned char *)&szHtml[j++]);
				if (((unsigned char)szHtml[j - 1] & 0x80) != 0) {
					szHtml[j++] = szHtml[++i];
				}
				i += replacelen;
			}
			else 
			//���ַ������ַ�����ǰ��λ��
			szHtml[j++] = szHtml[i++];
		}
		//�趨�ַ���������־
		szHtml[j] = '\0';
		return j;
	}//chucai add end */
/*
	basis::n32_t filterQuery(char * szHtml,basis::n32_t nLen,bool bTrans, bool bTaobao) {
		//������Ϳ��ַ���������NULL
		if (!szHtml || nLen == 0) return 0;
		basis::n32_t j = 0;
		CHtmlCodeReplacer * hcr = CHtmlCodeReplacerCreator::makeHtmlCodeReplacer(bTaobao?"taobao":"dfl");
		for (basis::n32_t i = 0; i < nLen; ) {
			if(bTrans) {
				int replacelen = hcr->CodeReplace((unsigned char *)(szHtml + i),(unsigned char *)&szHtml[j]);
				if(szHtml[j] != ' ' || j != 0) { //���˵��ַ���ǰ��Ŀո�
					j++;
					if (((unsigned char)szHtml[j - 1] & 0x80) != 0) {
						szHtml[j++] = szHtml[++i];
					}
				}
				i += replacelen;
			}
			else { //���ַ������ַ�����ǰ��λ��
				szHtml[j++] = szHtml[i++];
			}
		}
		//���˵��ַ�����β���Ŀո�
		int x=j-1;
		while (' ' == szHtml[x]) x--;
		szHtml[x+1] = '\0';//�趨�ַ���������־
		return x+1;
	}//chucai add end
	*/
	/* url encode */
	void urlEncode(char* buf, const char* str)
	{
		const char special[] = "!\"#$%&'()*+,/:;<=>?@[\\]^`{|}~%"; /* special characters */ 
		char tmp[10]; /* temp buffer */ 
		buf[0] = '\0';
		size_t nLen = strlen(str);
		for(size_t i = 0; i < nLen; i++) {
			char c = str[i];
			if(strchr(special, c)) {	/* special character */ 
				sprintf(tmp, "%%%02X", (unsigned char)c); 
				strcat(buf, tmp); 
			}
			else if(c == ' ') /* blank */ 
				strcat(buf, "+");
			else {
				if(c < 0) {	/* none ASCII character */
					sprintf(tmp, "%%%02X%%%02X", (unsigned char)str[i], (unsigned char)str[i+1]); 
					strcat(buf, tmp); 
					++i; 
				}
				else {	/* ASCII character */
					sprintf(tmp, "%c", c); 
					strcat(buf, tmp); 
				}
			} 
		}
	}
	
    void urlEncode2(char *buf, const char* url)
    {
        #define URL_HEXCHARS "0123456789ABCDEF"
        size_t len = strlen(url);
        char *str = buf;
        size_t x, y;    
        for (x = 0, y = 0; len--; x++, y++)
        {
            str[y] = (unsigned char) url[x];
            if (str[y] == ' ')
            {
                str[y] = '+';
            }
            else if ((str[y] < '0' && str[y] != '-' && str[y] != '.') ||
                       (str[y] < 'A' && str[y] > '9') ||
                       (str[y] > 'Z' && str[y] < 'a' && str[y] != '_') ||
                       (str[y] > 'z'))
            {
                str[y++] = '%';
                str[y++] = URL_HEXCHARS[(unsigned char) url[x] >> 4];
                str[y]   = URL_HEXCHARS[(unsigned char) url[x] & 15];
            }
        }
        str[y] = '\0';    
    }	

   void urlDecode(char *word) 
   { 
		int i,j; 
		for(i = 0, j = 0; i < strlen(word); i++) 
		{ 
				if(word[i] == '%') 
				{ 
						if(word[i+1] >= 'a' && word[i+1] <= 'f')     
								word[j] = word[i+1] - 'a' + 10; 
						else if(word[i+1] >= 'A' && word[i+1] <= 'F') 
								word[j] = word[i+1] - 'A' + 10; 
						else if(word[i+1] >= '0' && word[i+1] <= '9') 
								word[j] = word[i+1] - '0'; 
						else 
						{ 
								word[j] = '%'; 
								word[j+1] = word[i+1]; 
								j += 2; 
								i++; 
								continue; 
						} 
						word[j] *= 16; 
						if(word[i+2] >= 'a' && word[i+2] <= 'f')
								word[j] += word[i+2] - 'a' + 10; 
						else if(word[i+2] >= 'A' && word[i+2] <= 'F')
								word[j] += word[i+2] - 'A' + 10; 
						else if(word[i+2] >= '0' && word[i+2] <= '9')
								word[j] += word[i+2] - '0'; 
						else 
						{ 
								word[j] = '%'; 
								word[j+1] = word[i+1]; 
								word[j+2] = word[i+2]; 
								j += 3; 
								i += 2; 
								continue; 
						} 
						j++; 
						i += 2; 
				} 
				else 
				{ 
						word[j++] = word[i]; 
				} 
		} 
		word[j] = 0; 
   } 
	
    /* �����ֶ����ͽ�tokenֵת��Ϊ64λ�Ĺ�ϣֵ */
    basis::u_n64_t hashForNumberIndex(const char* szText, basis::EFieldType eType) {
        basis::u_n64_t nKeyHash;
        const char *szValue = szText;
        bool bNegative = false;

        if (szText[0] == '-') {	//�����Ǹ���,���Ǳ��뱣֤����ת����u_n64_t���͵�ֵ���ڸ���ת�����ֵ
            szValue = &szText[1];
            bNegative = true;
        }
        if (eType == ft_float || eType == ft_property 
            || ft_float4 == eType ) {	//�����ȣ���˫���ȸ���, ft_property���͵��ֶ�
            bool bPoint = false;
            const char *ptr = szValue;
            if (!string_utility::isNum(*ptr++))		//��һλ����������
                return 0;
            while (*ptr) {
                if (!string_utility::isNum(*ptr) &&  *ptr != '-') {
                    if (!bPoint&&((*ptr)=='.'||(*ptr)==':')&&(string_utility::isNum(*(ptr+1)) || *(ptr+1)=='-' )) {
                        bPoint = true;
                        ptr++;
                        continue;
                    }
                    return 0xFFFFFFFFFFFFFFFFLL;
                }
                ptr ++;
            }
            const char *szLeft = szValue;
            const char *szRight = strchr(szValue, '.');	//float���͵��ֶ�
            if (szRight == NULL)
                szRight = strchr(szValue, ':');		//�����ֶ�
            nKeyHash = atoi(szLeft);
            nKeyHash <<= 32;
            if (szRight != NULL && *szRight != '\0') {
                szRight++;
                if (*szRight != '\0') {
                	nKeyHash |= (basis::u_n32_t)atoi(szRight);
                }
            }
        }
        else if (eType == ft_integer) {
            basis::u_n32_t nKeyHash_u32 = atoi(szValue);
            if (bNegative)
                nKeyHash_u32 = ~nKeyHash_u32 & 0x7FFFFFFFLL;
            else
                nKeyHash_u32 |= 0x80000000LL;
            return (basis::u_n64_t)nKeyHash_u32;
        }
        else {    //integer, long��time���͵��ֶ�
        	nKeyHash = strtolong(szValue) << 1;     // ��� szValue Ϊ 0111...110, �� 0011...110 ��ͻ
        }
        if (bNegative)
            nKeyHash = ~nKeyHash & 0x7FFFFFFFFFFFFFFFLL;
        else
            nKeyHash |= 0x8000000000000000LL;

        return nKeyHash;
    } // end of hashForNumberIndex

    /* �����ֶ����ͽ�tokenֵת��Ϊ64λ�Ĺ�ϣֵ */
    basis::u_n64_t hashForStringIndex(const char* szText, basis::n32_t extendMap, basis::u_n64_t *pMd5)
    {
        basis::u_n64_t nKeyHash = 0, nKeyHashMd5 = 0;
        //�ؼ���ʵ������һ������,��������Ϊ��Ϊ��,������ֱ��ת��Ϊ64λ��hashֵ
        if (extendMap & basis::ie_int || extendMap & basis::ie_double) {		
			nKeyHash = strtolong(szText);
        }
        else if (pMd5 != NULL&&extendMap & basis::ie_md5) {
			if (strlen(szText) == 128 / 4) {	//�ؼ���ʵ������һ��md5(128λ)������,��32λ���ַ�������ʾ
				for (size_t i = 0; i < 128 / 4; i+=2) {
					if (szText[i] >= '0' && szText[i] <= '9')
						nKeyHash += (szText[i] - '0');
					else
						nKeyHash += (szText[i] - 'A') + 10;
					nKeyHash <<= 4;
				}
				for (size_t i = 1; i < 128 / 4; i+=2) {	//��64λ��ֵ�ȱ�������,����
					if (szText[i] >= '0' && szText[i] <= '9')
						nKeyHashMd5 += (szText[i] - '0');
					else
						nKeyHashMd5 += (szText[i] - 'A') + 10;
					nKeyHashMd5 <<= 4;
				}
			}
			*pMd5 = nKeyHashMd5;
        }
        else {		//��ͨ�ַ���
            nKeyHash = string_utility::hashString64(szText);
        }

        return nKeyHash;
    }

  }
}

