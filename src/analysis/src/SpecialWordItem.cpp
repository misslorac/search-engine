#include <iostream>
#include "StringUtility.h"
#include "SpecialWordItem.h"

using namespace std;
using namespace basis;
using namespace utility;

namespace analysis {

/*
 * Desc:	Ĭ�Ϲ��캯��
 * Params:
 *			None
 * Return:
 *			None
 */
CSpecialWordItem::CSpecialWordItem()
{
	pszWord = NULL;
	nCategory = 0;
	nFrequency = 0;
	kind = WK_Other;
}
/*
 * Desc:		���캯��,����ؼ��ֺ����ڵ���Ŀ
 * Params:
 *			const char* p_szWord:	�ؼ���
 *			int nCategory:			������Ŀ��Ĭ��ֵ��0��
 * Return:
 *			None
 */
CSpecialWordItem::CSpecialWordItem( const char* p_szWord, n32_t nCategory, EWordKind kind)
{
	this->pszWord = string_utility::replicate( p_szWord );
	this->nCategory = nCategory;
	this->kind = kind;
	this->nFrequency = 1;
}
/*
 * Desc:	�������캯��
 * Params:
 *			const CSpecialWordItem& wordItem:	����һ����������
 * Return:
 *			None
 */
CSpecialWordItem::CSpecialWordItem( const CSpecialWordItem& wordItem )
{
	pszWord = string_utility::replicate(wordItem.pszWord);
	nCategory = wordItem.nCategory;
	nFrequency = wordItem.nFrequency;
	kind = wordItem.kind;
	for( size_t i = 0; i < wordItem.synonyms.size(); i++ )
	{
		synonyms.push_back( string_utility::replicate(wordItem.synonyms[i]) );
	}
	for( size_t i = 0; i < wordItem.rWords.size(); i++ )
	{
		rWords.push_back( wordItem.rWords[i] );
	}
	for( size_t i = 0; i < wordItem.wFeatures.size(); i++ )
	{
		wFeatures.push_back( wordItem.wFeatures[i] );
	}
	for( size_t i = 0; i < wordItem.alias.size(); i++ )
	{
		alias.push_back( string_utility::replicate(wordItem.alias[i]) );
	}
}
/*
 * Desc:	��������
 * Params:
 *			None
 * Return:
 *			None
 */
CSpecialWordItem::~CSpecialWordItem()
{
	if(pszWord) {
		delete[] pszWord;
		pszWord = NULL;
	}
	for( size_t i = 0; i < synonyms.size(); i++ )
	{
		if(synonyms[i]) {
			delete[] synonyms[i];
			synonyms[i] = NULL;
		}
	}
	for( size_t i = 0; i < alias.size(); i++ )
	{
		if(alias[i]) {
			delete[] alias[i];
			alias[i] = NULL;
		}
	}
}
/*
 * Desc:	ȡ�ùؼ�������
 * Params:
 *			None
 * Return:
 *			���ض���õĹؼ�������
 */
EWordType CSpecialWordItem::getWordType()
{
	unsigned char c1 = pszWord[0];
	unsigned char c2 = pszWord[1];

	if( c1 > 0x20 && c1 < 0x7F )
	{
		if( strchr( "\"!:;,.?()[]{}", c1 ) )
			return WT_DELIMITER;
		else if( c1 >= 0x30 && c1 <= 0x39 )
			return WT_NUM;
		else
			return WT_ALPHA;
	}
	else if( c1==0xA2 || (c1==0xA3&&c2>=0xB0&&c2<=0xB9) )//������������
	{
		return WT_INDEX;
	}
	else if( c1==0xA3 && (c2>=0xC1&&c2<=0xDA||c2>=0xE1&&c2<=0xFA) )//˫�ֽڵ���ĸ��a-z,A-Z��
	{
		return WT_LETTER;
	}
	else if( c1==0xA1 )
	{
		return WT_DELIMITER;
	}
	else if( c1>=0xB0&&c1<=0xF7&&c2>=0xA1 )
	{
		return WT_CHINESE;
	}
	else if( c1 == ' ' || (c1 == 0xA1&&c2== 0xA1) )
	{
		return WT_SPACE;
	}
	return WT_OTHER;
}
/*
 * Desc:	�����ַ�������,����ʵ��е�hashֵ
 * Params:
 *			None
 * Return:
 *			���ض�Ӧ��hashֵ
 */
n32_t CSpecialWordItem::hashCode()
{
	n32_t nHash;
	EWordType type = getWordType();

	if( type == WT_CHINESE )		//������
	{
		nHash = CC_ID( pszWord[0],pszWord[1] );
	}
	else if( type == WT_NUM )		//���ֽ�����
	{
		nHash = 3755;
	}
	else if( type == WT_LETTER )	//˫�ֽ��ַ�
	{
		nHash = 3756;
	}
	else if( type == WT_ALPHA )		//���ֽ��ַ�
	{
		nHash = 3757;
	}
	else if( type == WT_INDEX || type == WT_LETTER ) //�������ֺ��ַ�
	{
		nHash = 3758;
	}
	else
	{
		nHash = 3759;
	}

	return nHash;
}
/*
 * Desc:	ȡ�����ݳ�Ա���������ֽ���
 * Params:
 *			None
 * Return:
 *			���ֽڴ�С
 */
size_t CSpecialWordItem::getSize()
{
	size_t nSize = 0;
	//�ؼ��ִ�С
	nSize += ( strlen(pszWord)+1 )*sizeof(char);
	//��Ŀ + Ƶ��
	nSize += sizeof(n32_t) + sizeof(n32_t);
	//����
	nSize += sizeof(char);
	//ͬ���
	nSize += sizeof(n32_t);
	for( size_t i = 0; i < synonyms.size(); i++ )
	{
		nSize += (strlen( synonyms[i] ) + 1);
	}
	//��ش�
	nSize += sizeof(n32_t);//��С
	for( size_t i = 0; i < rWords.size(); i++ )
	{
		nSize += (strlen( rWords[i].m_pKey) + 1);
		nSize += sizeof(n32_t) + sizeof(n32_t);	//��Ŀ + ��ض�
		nSize += sizeof(char);				//����
	}
	//����
	nSize += sizeof(n32_t);	//��С
	for( size_t i = 0; i < wFeatures.size(); i++ )
	{
		nSize += sizeof(n32_t);								//����id
		nSize += (strlen( wFeatures[i].pszFeature ) + 1);	//��������
		nSize += (sizeof(char) + sizeof(char));				//�ֶ����͡�+ ���Լ���
		nSize += sizeof(n32_t);								//ǰ������id
		nSize += (strlen( wFeatures[i].pszForwardValue ) + 1);
		nSize += sizeof(n32_t);								//����ֵ����
		for( size_t j = 0; j < wFeatures[i].values.size(); j ++ )
		{
			nSize += (strlen( wFeatures[i].values[i] ) + 1);
		}
	}
    //����
	nSize += sizeof(n32_t);
	for( size_t i = 0; i < alias.size(); i++ )
	{
		nSize += (strlen( alias[i] ) + 1);
	}
	return nSize;
}
/*
 * Desc:	�����ݳ�Ա��������ָ��ָ�뿪ʼ���ڴ���
 * Params:
 *			char*& pPtr:	�ڴ��д�����ݵ���ʼ��ַ,�õ�ַ�ı仯���ظ��û�
 * Return:
 *			None
 */
void CSpecialWordItem::outputData( char*& pPtr )
{
	//�ؼ���
	strcpy( pPtr, pszWord );
	pPtr += ( strlen(pszWord) + 1 );
	//��Ŀ
	*(n32_t*)pPtr = nCategory;
	pPtr += sizeof(n32_t);
	//��Ƶ
	*(n32_t*)pPtr = nFrequency;
	pPtr += sizeof(n32_t);
	//����
	*(char*)pPtr = kind;
	pPtr += sizeof(char);
	//ͬ���
	*(n32_t*)pPtr = static_cast<n32_t>( synonyms.size() );
	pPtr += sizeof(n32_t);
	for( size_t i = 0; i < synonyms.size(); i++ )
	{
		strcpy( pPtr, synonyms[i] );
		pPtr += (strlen( synonyms[i] ) + 1);
	}
	//��ش�
	*(n32_t*)pPtr = static_cast<n32_t>( rWords.size() );
	pPtr += sizeof(n32_t);	//��С
	for( size_t i = 0; i < rWords.size(); i++ )
	{
		strcpy( pPtr, rWords[i].m_pKey);
		pPtr += (strlen( rWords[i].m_pKey) + 1);
		*(n32_t*)pPtr = rWords[i].m_nCategory;	//��Ŀ
		pPtr += sizeof(n32_t);
		*(n32_t*)pPtr = rWords[i].m_nRank;
		pPtr += sizeof(n32_t);				//��ض�
		*(char*)pPtr = rWords[i].m_level;
		pPtr += sizeof(char);				//����
	}
	//����
	*(n32_t*)pPtr = static_cast<n32_t>( wFeatures.size() );
	pPtr += sizeof(n32_t);	//��С
	for( size_t i = 0; i < wFeatures.size(); i++ )
	{
		*(n32_t*)pPtr = wFeatures[i].nId;
		pPtr += sizeof(n32_t);
		strcpy( pPtr, wFeatures[i].pszFeature );
		pPtr += (strlen( wFeatures[i].pszFeature ) + 1);	//��������
		*(char*)pPtr = wFeatures[i].type;					//�ֶ�����
		pPtr += sizeof(char);
		*(char*)pPtr = wFeatures[i].level;					//���Լ���
		pPtr += sizeof(char);
		*(n32_t*)pPtr = wFeatures[i].nParentId;				//ǰ������id
		pPtr += sizeof(n32_t);
		strcpy( pPtr, wFeatures[i].pszForwardValue );		//ǰ�����Ե�ĳ��ֵ
		pPtr += (strlen( wFeatures[i].pszForwardValue ) + 1);
		*(n32_t*)pPtr = static_cast<n32_t>( wFeatures[i].values.size() ); //����ֵ����
		pPtr += sizeof(n32_t);
		for( size_t j = 0; j < wFeatures[i].values.size(); j ++ )
		{
			strcpy( pPtr, wFeatures[i].values[i] );
			pPtr += (strlen( wFeatures[i].values[i] ) + 1);
		}
	}
	//����
	*(n32_t*)pPtr = static_cast<n32_t>( alias.size() );
	pPtr += sizeof(n32_t);
	for( size_t i = 0; i < alias.size(); i++ )
	{
		strcpy( pPtr, alias[i] );
		pPtr += (strlen( alias[i] ) + 1);
	}
}
/*
 * Desc:	��ĳ����ַ��ʼ���������ݳ�Ա��ֵ,�ö���ʹ��һ����New�����Ŀն������pszWord���վ����ڴ�й©  
 * Params:
 *			char*& pPtr:	�ڴ��д�����ݵ���ʼ��ַ,�õ�ַ�ı仯���ظ��û�
 * Return:
 *			None
 */
void CSpecialWordItem::inputData( char*& pPtr )
{
	//�ؼ���
	size_t nLen = strlen(pPtr);
	if(pszWord != NULL)
	{
		delete[] pszWord;
		pszWord = NULL;
	}
	pszWord = new char[nLen+1];
	strcpy(pszWord, pPtr);
	pPtr += (nLen+1);
	//��Ŀ
	nCategory = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);
	//��Ƶ
	nFrequency = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);
	//��Ʒ����
	char ch = *(char*)pPtr;
	pPtr += sizeof(char);
	switch( ch )
	{
		case 'i':
			kind = WK_Independence;
			break;
		case 'c':
			kind = WK_Complex;
			break;
		case 'k':
			kind = WK_Category;
			break;
		case 'p':
			kind = WK_Concept;
			break;
		case 'a':
			kind = WK_Auto;
			break;
		default:
			kind = WK_Other;
			break;
	};
	//ͬ���
	n32_t nSize = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);
	for( n32_t i = 0; i < nSize; i++ )
	{
		synonyms.push_back( string_utility::replicate( pPtr ) );
		pPtr += (strlen( pPtr ) + 1);
	}
	//��ش�
	nSize = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);	//��С
	for( n32_t i = 0; i < nSize; i++ )
	{
		SRelationWord rWord;
		rWord.m_pKey= string_utility::replicate( pPtr );
		pPtr += (strlen( pPtr ) + 1);
		rWord.m_nCategory = *(n32_t*)pPtr;	//��Ŀ
		pPtr += sizeof(n32_t);
		rWord.m_nRank = *(n32_t*)pPtr;
		pPtr += sizeof(n32_t);				//��ض�
		ch = *(char*)pPtr;
		pPtr += sizeof(char);				//����
		switch( ch )
		{
			case 'u':
				rWord.m_level = RL_Upper;
				break;
			case 'l':
				rWord.m_level = RL_Lower;
				break;
			case 'c':
				rWord.m_level = RL_Contain;
				break;
			case 'r':
				rWord.m_level = RL_Relation;
				break;
			case 'a':
				rWord.m_level = RL_Accessory;
				break;
		};
		rWords.push_back( rWord );
	}
	//����
	nSize = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);	//��С
	for( n32_t i = 0; i < nSize; i++ )
	{
		SWordFeature wFeature;
		wFeature.nId = *(n32_t*)pPtr;
		pPtr += sizeof(n32_t);
		wFeature.pszFeature = string_utility::replicate( pPtr );
		pPtr += (strlen( pPtr ) + 1);	//��������
		ch = *(char*)pPtr;					//�ֶ�����
		pPtr += sizeof(char);
		switch( ch )
		{
			case 's':
				wFeature.type = FT_String;
				break;
			case 'e':
				wFeature.type = FT_Enum;
				break;
			case 'i':
				wFeature.type = FT_Integer;
				break;
			case 'f':
				wFeature.type = FT_Float;
				break;
			case 'r':
				wFeature.type = FT_Range;
				break;
		};
		ch = *(char*)pPtr;					//���Լ���
		pPtr += sizeof(char);
		switch( ch )
		{
			case 'e':
				wFeature.level = FL_Essential;
				break;
			case 'o':
				wFeature.level = FL_Optional;
				break;
			case 't':
				wFeature.level = FL_Triggered;
				break;
			case 'r':
				wFeature.level = FL_Reserved;
				break;
		};
		wFeature.nParentId = *(n32_t*)pPtr;				//ǰ������id
		pPtr += sizeof(n32_t);
		wFeature.pszForwardValue = string_utility::replicate(pPtr);		//ǰ�����Ե�ĳ��ֵ
		pPtr += (strlen( pPtr ) + 1);
		n32_t nValueSize = *(n32_t*)pPtr;			//����ֵ����
		pPtr += sizeof(n32_t);
		for( n32_t j = 0; j < nValueSize; j ++ )
		{
			wFeature.values.push_back( string_utility::replicate( pPtr ) );
			pPtr += (strlen( pPtr ) + 1);
		}
		wFeatures.push_back( wFeature );
	}
	//����
	nSize = *(n32_t*)pPtr;
	pPtr += sizeof(n32_t);
	for( n32_t i = 0; i < nSize; i++ )
	{
		alias.push_back( string_utility::replicate( pPtr ) );
		pPtr += (strlen( pPtr ) + 1);
	}
}

/*��ӡ��SpecialWordItem����������*/
void CSpecialWordItem::print()
{
	cout << "keyword = " << pszWord << endl;
	cout << "Category= " << nCategory << endl;
	cout << "Frequence=" << nFrequency << endl;
	cout << "��Ʒ���� =" << kind << endl;
	cout << "ͬ��� =";
	for(n32_t i = 0; i < synonyms.size(); i++)
	{
		cout << " " << synonyms[i];
	}
	cout << endl; 
	cout << "��ش� = ";
	for(n32_t i = 0; i < rWords.size(); i++)
	{
		cout << "keyword=" << rWords[i].m_pKey
		     << "Category=" << rWords[i].m_nCategory
		     << "Rank=" << rWords[i].m_nRank
		     << "Level=" << rWords[i].m_level;
	}
	cout << endl;
	cout << "���� = ";
	for(n32_t i = 0; i < wFeatures.size(); i++)
	{
		cout << "Feature ID = " << wFeatures[i].nId 
		     << " Name= " << wFeatures[i].pszFeature 
		     << " Type= " << wFeatures[i].type
		     << " Level= " << wFeatures[i].level
		     << " ǰ������=" << wFeatures[i].nParentId
	 	     << "ǰ������ֵ=" << wFeatures[i].pszForwardValue
		     << "����ֵ=";
		for(n32_t j = 0; j < (wFeatures[i].values).size(); j++)
		{
			cout << (wFeatures[i].values)[j];
		}
	}
	cout << endl;
	cout << "���� =";
	for(n32_t i = 0; i < alias.size(); i++)
	{
		cout << alias[i];
	}
	cout << endl;
}

}
