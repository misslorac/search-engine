#ifndef BASE_FILE_H
#define BASE_FILE_H

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace store {

#define FILE_OPEN_READ		1
#define FILE_OPEN_WRITE		2
#define FILE_OPEN_APPEND	4
#define FILE_OPEN_UPDATE    5

//��ǰ֧�ֵ��ļ�����
#define FILE_TYPE_MEMREAD   1
#define FILE_TYPE_SHM       2
#define FILE_TYPE_MMAP      3
#define FILE_TYPE_FS        4
#define FILE_TYPE_BFS		5

class CBaseFile
{
public:
	CBaseFile(void);
	virtual ~CBaseFile(void) = 0;
	// ���ļ�
	virtual bool open(const char *szFileName, int flag, size_t length = 0) = 0;
	// �����ļ�λ��
	virtual bool setPosition(size_t pos, int whence = SEEK_SET) = 0;
	// �õ��ļ���ǰλ��
	virtual size_t getPosition() = 0;
	// ���ļ�, �����Ƕ����ֽ���
	virtual size_t read(void *src, size_t len) = 0;
	// д�ļ�, ������д���ֽ���
	virtual size_t write(void *src, size_t len) = 0;
	// �õ��ļ���С
	virtual size_t getSize() = 0;
	// flush
	virtual bool flush() = 0;
	// �ر��ļ�
	virtual bool close() = 0;
	/* ��ƫ����ת��Ϊ��ַ */
	virtual inline char *offset2Addr(size_t nOffset=0) const {return NULL;}
	/* �л��ڴ棬����shmfileʹ�� */
	virtual bool refresh() {return true;}
	/* �õ��ļ��� */
	char *getFileName() const {
		return m_szFileName;
	}
	/* ���ļ���󿪱�һ���µĿռ䣬 �������¿��ٿռ�ƫ���� */
	int32_t makeNewPage(int32_t nPageSize);
    /* �ļ�TYPE */
    virtual int getFileType() = 0;
	/* �ͷ��ļ���ռ�õ��ڴ���Դ page cache  */
	virtual inline bool release() {return true;}
    
protected:
	char *m_szFileName;
};

}

#endif
