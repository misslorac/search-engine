#include "InnerWordItem.h"

namespace analysis {
	namespace word_kind {
		void clean(TYPE &marker)
		{
			marker = 0x0;
		}
		
		/* ����Ϊ��֧�ִַ� */
		void enableNLP(TYPE &marker) 
		{
			marker |= NLP_MARKER;
		}
		
		/* ����Ϊ����֧�ִַ� */
		void unenableNLP(TYPE &marker) 
		{
			marker &= ~NLP_MARKER;
		}
		
		/* �ж��Ƿ�֧�ִַ� */
		bool beNLP(TYPE marker)
		{
			return ((marker & NLP_MARKER) != 0);
		}
		
		/* ��Ϊרҵ�� */
		void setSpeWord(TYPE &marker) 
		{
			marker |= SPE_MARKER;
		}
		
		/* ��Ϊͨ�ô� */
		void setGenWord(TYPE &marker) 
		{
			marker &= ~SPE_MARKER;
		}
		
		/* �Ƿ���ͨ�ô� */
		bool beGenWord(TYPE marker)
		{
			return ((marker & SPE_MARKER) == 0);
		}
		
		/* �Ƿ���רҵ�� */
		bool beSpeWord(TYPE marker)
		{
			return ((marker & SPE_MARKER) != 0);
		}
		
		/* ����Ϊ��Ŀ�� */
		void setCategoryWord(TYPE &marker)
		{
			marker |= CAT_MARKER;
		}
		
		/* ����Ϊ����Ŀ�� */
		void unsetCategoryWord(TYPE &marker)
		{
			marker &= ~CAT_MARKER;
		}
		
		/* �ж��Ƿ�����Ŀ�� */
		bool beCategoryWord(TYPE marker)
		{
			return ((marker & CAT_MARKER) != 0);
		}
	}
}
