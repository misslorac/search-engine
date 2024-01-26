/******************************************************************************
 *
 * File     :MMapFile.h
 * Version  :$Id: MMapFile.h,v 1.9 2005/09/19 09:23:39 kwq Exp $
 * Desc     :Ӳ���ļ�Ӱ���ļ��༰һЩ��Ӳ���ļ�Ӱ���һЩ����
 * Log		:Created by victor, 2004-12-24
 *
 ******************************************************************************/
#ifndef MMAPFILE_H
#define MMAPFILE_H

#include <iostream>
#include <assert.h>
#	include <sys/mman.h>
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#include "util/Log.h"
#include "BaseFile.h"

namespace store
{

class CMMapFile : public CBaseFile
{
public:
	//���캯��
	CMMapFile(bool unload=false);
	//��������
	virtual ~CMMapFile();
	/*
	 * ���ļ�
	 *
	 */
	bool open(const char *szFileName, int flag = FILE_OPEN_READ, size_t length = 0);
	bool openEx(const char *szFileName, int flag = FILE_OPEN_READ, size_t length = 0);
	/*
	 * ����ӳ���ļ�
	 */
	bool reopen(off_t nOffset, size_t nLength);
	/*
	 * �ر��ڴ�ӳ���ļ�
	 * ˵����
	 *		���ڴ�ӳ���ļ����浽Ӳ���ļ���
	 *		�ͷź͸ô˲����йص�һ�����ݺ���Դ��
	 */
	bool close();

  /*
   * Ԥ��δ���ڴ������еĲ���
   */
  void readahead(int32_t off, int32_t len) {
    ::readahead(m_pHander.m_fd, off, len);
  }
  void preLoad(const char* filename) {
      setPosition(0);

      char buf[4096];
      size_t len = 0;
      time_t begin = time(NULL);
      while(len < m_nLength) {
          size_t ret = read(buf, 4096);
          len += ret;
      }
      TLOG("read %s, len=%lu, use time=%ld", filename, m_nLength, time(NULL) - begin);
  }

	/* ��ƫ����ת��Ϊ��ַ */
	inline char *offset2Addr(size_t nOffset) const
	{
		assert(m_pBase!=NULL);
		return (m_pBase + nOffset);
	}
	/* �����ļ���С */
	size_t getSize()
	{
		return m_nLength;
	}
	/* �ڴ�ӳ���ļ����ݱ��ı��,���ô˺�������ͬ������ */
	bool flush();
	// �����ļ�λ��
	bool setPosition(size_t pos, int whence = SEEK_SET);
	// �õ��ļ���ǰλ��
	size_t getPosition();
	// ���ļ�, �����Ƕ����ֽ���
	size_t read(void *src, size_t len);
	// д�ļ�, ������д���ֽ���
	size_t write(void *src, size_t len);
    	int getFileType() { return FILE_TYPE_MMAP; }
        bool release()
        {
                if (m_unload)
                {
                        int ret = madvise(m_pBase, m_nLength, MADV_DONTNEED);
                        if (ret != 0)
			{
				TERR("release page cache error, ret = %d", ret);
				return false;
			}
                }
		return true;
        }
   	bool m_unload;
private:
	/*
	* ��ĳһ���ļ�ӳ�䵽�ڴ�,bReadOnlyָ�����򿪵��ļ��Ƿ�ֻ���ڶ�,������Խ��ж�д
	* ���ļ��ɹ�����ӳ����ڴ����ַ��ʧ�ܷ���NULL��
	*/
	bool openInternal(const char* pszName, bool bReadOnly = false);
	/*
	* ����һ��ָ����С���ڴ�ӳ���ļ�,���е����ݶ��������ڴ���,����Ӳ���ϵ�������ӳ��
	* ���أ�
	*		ӳ����ڴ����ַ��ʧ�ܷ���NULL��
	*/
	bool create(const char* pszName, size_t nLength);

private:
	size_t m_nLength;				//�ļ���С
	size_t m_nPos;					//λ��
	char* m_pBase;					//���ݻ�ָ��

	struct SFileHandler				//ʵ��������
	{
#ifdef _WIN32
		HANDLE m_hFile;
		HANDLE m_hMapFile;
#else
		int m_fd;		//�ļ�������
#endif
	};
	struct SFileHandler m_pHander;		    //ʵ�ֶ���ָ��
};

}

#endif //MMAPFILE_H
