/*********************************************************************
 * 
 * File:	InnerWordItem.h 
 * Desc:	�����������ʾдmmap�ļ���wordItem����
 * Log :
 * 	Create by regerett, 2007/5/21
 *
 *********************************************************************/

#ifndef INNER_WORD_ITEM_H
#define INNER_WORD_ITEM_H

#include <string>
#include "BasisDef.h"
#include "StringUtility.h"

namespace analysis {
	/* ��شʵļ��� */
	enum ERelationLevel {
		RL_Upper = 'u',					//��λ��
		RL_Lower = 'l',					//��λ��
		RL_Contain = 'c',				//�̺���ϵ��
		RL_Relation = 'r',				//ͬ������ش�
		RL_Accessory = 'a',				//���������ܹ�ϵ����ش�
		RL_Other = 'o'					//������ش�
	};

	struct SRelationWord {
		char* m_pKey;					//��شʹؼ���
		basis::n32_t m_nCategory;		//��ش����ڵ���Ŀ
		basis::n32_t m_nRank;			//��ض�
		ERelationLevel m_level;			//�������
		/* Ĭ�Ϲ��� */
		SRelationWord()
		{
			m_pKey = NULL;
		}
		SRelationWord(const SRelationWord &rRelWord) {
			m_pKey = utility::string_utility::replicate(rRelWord.m_pKey);
			m_nCategory = rRelWord.m_nCategory;
			m_nRank = rRelWord.m_nRank;
			m_level = rRelWord.m_level;
		}
		
		~SRelationWord()
		{
			if(m_pKey) delete []m_pKey;
		}
		
		static size_t getSize() 
		{
			//ʵ�ʴ�ſռ�Ϊ����ضȣ�������� �� ��شʵ�����ƫ��λ��
			size_t size = sizeof(basis::n32_t)  + sizeof(ERelationLevel) + sizeof(basis::n32_t);
			return size;
		}
	};
	
	namespace word_kind
	{
		typedef basis::u_n16_t TYPE;             //����wordkind������
		const TYPE NLP_MARKER	= 0x1; //֧�ִַ�
		const TYPE SPE_MARKER	= 0x2; //רҵ����ͨ�ô�
		const TYPE CAT_MARKER	= 0x4; //��Ŀ�������Ŀ��
		
		void clean(TYPE &marker);
		/* ����Ϊ��֧�ִַ� */
		void enableNLP(TYPE &marker);
		/* ����Ϊ����֧�ִַ� */
		void unenableNLP(TYPE &marker);
		/* �ж��Ƿ�֧�ִַ� */
		bool beNLP(TYPE marker);
		/* ��Ϊרҵ�� */
		void setSpeWord(TYPE &marker);
		/* ��Ϊͨ�ô� */
		void setGenWord(TYPE &marker);
		/* �Ƿ���ͨ�ô� */
		bool beGenWord(TYPE marker);
		/* �Ƿ���רҵ�� */
		bool beSpeWord(TYPE marker);
		/* ����Ϊ��Ŀ�� */
		void setCategoryWord(TYPE &marker);
		/* ����Ϊ����Ŀ�� */
		void unsetCategoryWord(TYPE &marker);
		/* �ж��Ƿ�����Ŀ�� */
		bool beCategoryWord(TYPE marker);
	}

	class CInnerWordItem 
	{
	public:
		static const basis::n32_t MAX_WORD_LENGTH = 32;	//���ؼ��ֳ���
	public:
		char *m_pKey; //�ؼ���
		basis::n32_t m_nCategory; //��Ŀid
		
		CInnerWordItem() {
			m_pKey = NULL;
		}
		
		CInnerWordItem(const char *pKey, basis::n32_t nCategory) {
			m_pKey = new char[strlen(pKey)+1];
			strcpy(m_pKey, pKey);
			m_nCategory = nCategory;
		}
		
		virtual ~CInnerWordItem() {
			delete []m_pKey;
		}
		
		virtual size_t getSize() 
		{
			return 0;
		}
		
	};


}


#endif
