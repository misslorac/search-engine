/******************************************************************************
 *
 * File     :SpecialWordItem.h
 * Version  :$Id: SpecialWordItem.h,
 * Desc     :nCategory������רҵ�ʵ䣬���ֶα�ʾרҵ�����ڵ���Ŀ������ͨ�ôʵ���ֶα�ʾ����
 * �����ʣ����ʣ�����һЩ���������������Ŀ�����߶���һ��ID����ʾ������רҵ�ʵ���һ��category��
 * ������ÿһ��ID����˼����Ŀ�Ĳ�ι�ϵ������ͨ�ôʵ䣬��һ��wordKind��������ÿһ��id����˼��
 * ��������չ�Ľӿ� ע�ͣ�����רҵ�ʵ䣬���ֶα�ʾרҵ�����ڵ���Ŀ������ͨ�ôʵ���ֶα�ʾ
 * ���ԣ����ʣ����ʣ���һЩ���������������Ŀ�����߶���һ��int����ʾ��
 * Log		:Created by victor, 2004-12-29  modified by xionglin 
 *
 ******************************************************************************/

#ifndef SPECIAL_WORD_ITEM_H
#define SPECIAL_WORD_ITEM_H

#include <stdio.h>
#include <vector>
#include <string>
#include "BasisDef.h"
#include "StringUtility.h"
#include "InnerWordItem.h"

using namespace utility;

namespace analysis {

/* gbk�����0xA3A1-0xA3FD��ӦASCII��0x21-0x7D */
#define CC_ASCII(c1,c2) ((unsigned char)(c2)-0x80)
/* ��������ת��Ϊ��0��ʼ�������� */
#define CC_ID(c1,c2) ((unsigned char)(c1)-0xB0)*94+((unsigned char)(c2)-0xA1)

/* �ؼ������ͱ� */
enum EWordType {
	WT_DELIMITER = 0,		//��Ӣ�ı�����(,/����)
	WT_CHINESE = 1,			//���ĺ���(��)
	WT_ALPHA = 2,			//���ֽڷ���(a-z,A-Z)
	WT_NUM = 3,				//���ֽ�����(0-9)
	WT_INDEX = 4,			//������	(I-IV)
	WT_LETTER = 5,			//˫�ֽ���ĸ
	WT_SPACE = 6,			//�ո񣨰�����Ӣ�ĵĿո�
	WT_OTHER = 7			//����
};

/* ��Ʒ�ؼ��ֵ���� */
enum EWordKind {
	WK_Independence = 'i',   		//������(�Ӵ��������������)
	WK_Complex = 'c',				//���ϴ�
	WK_Category = 'k',				//������Ŀ(����Ŀ��һ�������Ĳ�Ʒ��)
	WK_Concept = 'p',				//������Ŀ������ĳϵ�в�Ʒ����Ŀ��
	WK_Auto = 'a',
	WK_Other = 'o'					//Ʒ�ƣ����ϣ�Ʒ�֣����ص��������͵Ĵ�
};

/* �������� */
enum EFeatureType {
	FT_Text = 't',
	FT_String = 's',
	FT_Enum = 'e',
	FT_Integer = 'i',
	FT_Float = 'f',
	FT_Range = 'r'
};

/* ���Լ��� */
enum EFeatureLevel {
	FL_Essential = 'e',				//������
	FL_Optional = 'o',				//��ѡ��
	FL_Triggered = 't',				//�����������������Դ����õ��ˣ�����A����B
	FL_Reserved = 'r'				//������,�������Կ��ܻ���Ӧ��
};

/* ��Ʒ�ؼ������Խṹ */
struct SWordFeature {
	basis::n32_t nId;						//����ID
	char *pszFeature;				//����
	EFeatureType type;				//�ֶ�����
	EFeatureLevel level;			//���Լ���,������,��ѡ��,������,������
	basis::n32_t nParentId;				//ǰ������ID
	char* pszForwardValue;			//ǰ�����Ե�ĳ��ֵ,��Щ���ԵĴ�����ͨ����Aֵ����B���ԵĹ�ϵ
	std::vector<char*> values;			//����ֵ, ÿ�����Կ����кܶ�����ֵ
	/* Ĭ�Ϲ��� */
	SWordFeature()
	{
		pszFeature = NULL;
		pszForwardValue = NULL;
	}
	/* �������캯�� */
	SWordFeature( const SWordFeature& wFeature )
	{
		nId = wFeature.nId;
		pszFeature = string_utility::replicate( wFeature.pszFeature );
		type = wFeature.type;
		level = wFeature.level;
		nParentId = wFeature.nParentId;
		pszForwardValue = string_utility::replicate( wFeature.pszForwardValue );
		for( size_t i = 0; i < wFeature.values.size(); i++ )
		{
			values.push_back( string_utility::replicate(wFeature.values[i]) );
		}
	}
	/* ���ظ�ֵ������ */
	SWordFeature& operator=( const SWordFeature& wFeature )
	{
		if(this!=&wFeature)
		{
			nId = wFeature.nId;
			if(pszFeature) delete[] pszFeature;
			pszFeature = string_utility::replicate( wFeature.pszFeature );
			type = wFeature.type;
			level = wFeature.level;
			nParentId = wFeature.nParentId;
			if(pszForwardValue) delete[] pszForwardValue;
			pszForwardValue = string_utility::replicate( wFeature.pszForwardValue );
			for( size_t i = 0; i < values.size(); i++ )
			{
				if(values[i]) {
					delete[] values[i];
					values[i] = NULL;
				}
			}
			for( size_t i = 0; i < wFeature.values.size(); i++ )
			{
				values.push_back( string_utility::replicate(wFeature.values[i]) );
			}
		}

		return *this;
	}
	/* �������� */
	~SWordFeature()
	{
		if(pszFeature) delete[] pszFeature;
		if(pszForwardValue) delete[] pszForwardValue;
		for( size_t i = 0; i < values.size(); i++ )
		{
			if(values[i]) delete[] values[i];
		}
	}
};

class CSpecialWordItem {
public:
	char* pszWord;		 	   //��(�ؼ���)
	basis::n32_t nCategory;            //��Ŀ
	basis::n32_t nFrequency;	   //�ʵ�Ƶ��

public:
	std::vector<char*> synonyms;		//ͬ���
 	EWordKind kind;				//�ʵ�����,������,���ϴ�
	std::vector<SRelationWord> rWords;	//��ش�����
	std::vector<SWordFeature> wFeatures;	//��Ʒ�ؼ�������
	std::vector<char*> alias;               //����

public:
	/* �չ��캯�� */	
	CSpecialWordItem();	
	/* ���캯�� */	
	CSpecialWordItem( const char* pszWord, basis::n32_t nCategory = 0, EWordKind kind = WK_Other);	
	/* �������캯�� */	
	CSpecialWordItem( const CSpecialWordItem& wordItem );
	/* �������� */	
	virtual ~CSpecialWordItem();
	/*��ȡ�ؼ�������*/
	EWordType getWordType();
	/*��ȡ�ؼ��ֵĹ�ϣֵ*/
	basis::n32_t hashCode();
	/* ȡ�����ݳ�Ա���������ֽ��� */
	virtual size_t getSize();
	/*�����ݳ�Ա��������ָ��ָ�뿪ʼ���ڴ���*/
	void outputData(char*& pPtr);
	/*�����ݳ�Ա��ָ��ָ�봦�����ڴ�*/
	void inputData( char*& pPtr );
	/*��ӡ��SpecialWordItem����������*/
	void print();
};

}

#endif
