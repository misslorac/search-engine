#include "BaseFile.h"

namespace store {

CBaseFile::CBaseFile() {}
CBaseFile::~CBaseFile() {}

/*
 *	���ļ���󿪱�һ���µĿռ䣬 �������¿���ͷλ��
 */
int32_t CBaseFile::makeNewPage(int32_t nPageSize)
{
	int32_t nDocPos = static_cast<int32_t>(getSize());	// �õ��ļ����λ��
	//���ļ���󿪱�һ���µĿռ䣬 ���ļ��Ƶ��ض���λ�ý���д��дʲô����û��ϵ��Ŀ��ֻ�ǰ��ļ��Ŵ�
	setPosition(nPageSize-1, SEEK_END); 
	write(&nDocPos, 1);
	return (nDocPos&0x7FFFFFFF);
}

}
