/**
 * @file xdict (dictionary)
 * @author Hightman Mar
 * @editor set number ; syntax on ; set autoindent ; set tabstop=4 (vim)
 * $Id: xdict.h,v 1.9 2011/10/26 08:41:44 hightman Exp $
 */

#ifndef	_SCWS_XDICT_20070528_H_
#define	_SCWS_XDICT_20070528_H_

#ifdef __cplusplus
extern "C" {
#endif

/* constant var define */
#define	SCWS_WORD_FULL		0x01	// ����: ����
#define	SCWS_WORD_PART		0x02	// ����: ǰ�ʶ�
#define	SCWS_WORD_USED		0x04	// ����: ��ʹ��
#define	SCWS_WORD_RULE		0x08	// ����: �Զ�ʶ���
#define	SCWS_WORD_LONG		0x10	// ����: �̴���ɵĳ���

#define	SCWS_WORD_MALLOCED	0x80	// xdict_query ���������� free

#define	SCWS_ZFLAG_PUT		0x02	// ����: ��ʹ��
#define	SCWS_ZFLAG_N2		0x04	// ����: ˫������ͷ
#define	SCWS_ZFLAG_NR2		0x08	// ����: ��ͷ��Ϊ˫������
#define	SCWS_ZFLAG_WHEAD	0x10	// ����: ��ͷ
#define	SCWS_ZFLAG_WPART	0x20	// ����: ��β�����
#define	SCWS_ZFLAG_ENGLISH	0x40	// ����: �����м��Ӣ��
#define SCWS_ZFLAG_SYMBOL   0x80    // ����: ����ϵ��
#define	SCWS_XDICT_PRIME	0x3ffd	// �ʵ�ṹ������16381

/* xdict open mode */
#define	SCWS_XDICT_XDB		1
#define	SCWS_XDICT_MEM		2
#define	SCWS_XDICT_TXT		4		// ...
#define	SCWS_XDICT_SET		4096	// set flag.

/* data structure for word(12bytes) */
typedef struct scws_word
{
	float tf;
	float idf;
	unsigned char flag;
	char attr[3];
}	word_st, *word_t;

typedef struct scws_xdict
{
	void *xdict;
	int xmode;
	struct scws_xdict *next;
}	xdict_st, *xdict_t;

/* pub function (api) */
xdict_t xdict_open(const char *fpath, int mode);
void xdict_close(xdict_t xd);

/* add a new dict file into xd, succ: 0, error: -1, Mblen only used for XDICT_TXT */
xdict_t xdict_add(xdict_t xd, const char *fpath, int mode, unsigned char *ml);

/* NOW this is ThreadSafe function */
word_t xdict_query(xdict_t xd, const char *key, int len);

#ifdef __cplusplus
}
#endif

#endif
