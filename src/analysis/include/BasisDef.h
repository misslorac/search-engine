#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H
#include <assert.h>
#include <cstring>

namespace basis {

/* ϵ�����ݳ������� */
#define MAX_TOKEN_LENGTH 128		//�������token����
#define MAX_FIELD_NAME_LENGTH 128	//��������ֶ����ĳ���
#define MAX_PARSE_QUERY_FIELD_LENGTH 1023 // define max field length while parse query field
#define MAX_FILE_PATH 512			//��������ļ�·������
#define MAX_FILE_NAME_LEN 256		//����ļ�������
#define DEFAULT_BUFFER_LENGTH 1024	//Ĭ��buffer��С,��Ĭ��ֵӦ���ڸ���ģ����
#define STOP_WORD_SPLIT_CHAR ','	//����stop word֮��ķָ��ַ� ','

#define TEXT_WORD_POS_BIT_COUNT 23
#define TEXT_FIELDBIT_COUNT 27

#define MIN_TIME_LENGTH 10			//��С��ʱ���ʽ
#define MAX_TIME_LENGTH 22			//���ʱ���ʽ

#define RESERVER_DOC_SIZE 128		//��ʱ����ģʽ��,Ԥ���ĵ�(doc)�ռ��С
#define RESERVER_OCC_SIZE 128		//��ʱ����ģʽ��,Ԥ��λ��(occ)�ռ��С
#define OCC_CHUNK_SIZE 260   //RESERVER_OCC_SIZE*sizeof( SOccInfo )+sizeof( SDocInfo )
#define EXTOCC_CHUNK_SIZE 388 //RESERVER_OCC_SIZE*sizeof( SExtOccInfo )+sizeof( SDocInfo )
#define OCC_READ_BUFFER_SIZE 16384	//λ����Ϣ��ȡʱ����ռ��С,�ô�С�������IOʵ�ʵĻ���ֵ(һ��Ϊ4096byte),����Ϊ�û���ֵ��������
#define MAX_OCCCOUNT_INONEDOC 4096  //һƪdoc��Occ���������ֵ

#define DO_ERASE_DOC_COUNT    100   //����ɾ�����ĵ�������100�ſ�ʼִ�����
#define DO_ERASE_DOC_PERCENT  5     //����ɾ�����ĵ����ٷֱȳ���5%��Ҳִ�����

#define SEGMENT_BUILD_MODE 'S'		//segment����ģʽ
#define INSTANT_BUILD_MODE 'I'		//��ʱ����(instant)����ģʽ
#define PROGRESSIVE_INSTANT 'P'		//�����еļ�ʱ����ģʽ

#define SEGMENT_ISUPDATE_MODE 'S'              //isupdate segmentģʽ
#define INSTANT_ISUPDATE_MODE 'I'              //isupdate instantģʽ

#define DEFAULT_TEXT_KEY_TIMES   131072  //����text���͵�����,�����ؼ��ֵĸ���Ĭ��Ϊ131072��
#define DEFAULT_STRING_KEY_TIMES 3		//����string���͵�����,�����ؼ��ֵĸ���Ĭ��3�������ĵ���

#define SEGMENT_COUNT_LIMIT 128		//����һ��segmentInfos�ļ�������������ݶ�(segment)����

#define INDEX_INVALIDINDEX -1 	//��Ч������ֵ
#define INDEX_INVALIDSERIAL -2	//Ϊ��������������������������һ������ֵ,�����Ϊ������;
#define INDEX_INVALID_XADRANK (INDEX_INVALIDINDEX-4)	//Ϊ��һ����ָ��Ϊ XADRANK ����
#define INDEX_INVALID_P4PPRICE (INDEX_INVALIDINDEX-5)	//Ϊ��һ����ָ��Ϊ P4PPrice ����

#define TIME_OF_DAY  86400   //һ������� 24*60*60 
#define FLOAT_TO_NUMBER 100 //2λС������,��������ֻ��ȷ2λ

#define MAX_QUERY_KEYWORD_LENGTH 1024
#define MAX_QUERY_LENGTH 2048		//��ѯ��䱻�������󳤶�
#define MAX_AREA_NAME 1280            //area_name����󳤶�


#define MIN_RESULT_NUM_TO_CHECK 10		//���йؼ��ּ�����С�����
#define MIN_TOKENIZER_WORD_LENGTH 6		//���зִʵ���С����
#define MAX_SPELL_CHECK_WORD_LENGTH 64	//����ƴд�������ؼ��ֳ���
#define DEFAULT_MAX_MEMPOOL_SIZE 2048 //Ĭ�ϲ�ѯʹ�õ�mempool�����size, ��λMB

#define RANK_COUNT 3	////����������������
#define FIRST_RANK  0      //��һ����,��Ӧ�����ļ��е�һ�����ֶ�
#define SECOND_RANK  1 //�ڶ�����,��Ӧ�����ļ��еڶ������ֶ�
#define THIRD_RANK  2      //��ʱ�������ֶ�,����distinct����

  //  #define ACC_VALUE_COUNT(a) (a&0x7fffffff)     //attribute ֵ������ʵ��ֵ(number ����attributeʹ�����λ���ڱ�ʶֵ�Ƿ���Ч,'1'
  //#define IS_ACCVALUE_INVALID(a) (a&0x80000000) //���attribute ֵ������ֵ�Ƿ���Ч
  //#define ACC_VALUE_INVALID(a) (a|0x80000000)   //��attributeֵ������ֵ������Ч

#define MAJOR_VERSION 4  //��ǰ�汾�����汾��
#define MINOR_VERSION 12  //��ǰ�汾�Ĵΰ汾��

#define DELETE_DELAY_TIME 200

//#define MAX_UNIQINFO_NUM 8   //����Uniq��Uniq Info������������С��5��
//#define MAX_UNIQINFO_CONTENT_NUM 4   //����uniq, ָ��content���ͷ���ʱ����󷵻صĸ���

#define MAX_CUSVALUE_LENTH 512  //����Custom��ʽͳ�Ƶ����ָ��ֵ�ַ�������

#define UNION_SEARCH_DELIM "\01"
#define UNION_SEARCH_MAX_RESULT 2500000
#define UNION_SEARCH_MAX_QUERY 5

#define MAX_MULVALUE_LENTH 512  //�����ά��ʽͳ�Ƶ����ָ��ֵ�ַ�������
#define MAX_MULCAT_DIM 3  //�����ά��ʽͳ�Ƶ����ά��

#define APP_KIND_P4P 1  // application for p4p 
#define APP_KIND_OTHER 0  // application for others

#define DICT_BLOCK_SIZE 64 // idx�ֵ�skiplistƽ������

#define ABNORMAL_MAX_QUERY_NUM 1000 // �ߺĴʱ����query��
#define ABNORMAL_MAXN 1000 // ��ϵͳ���ȶ������С����������MAXNֵ
#define ABNORMAL_MAX_FILTER_NUM 3 // ��ϵͳ���ȶ�������£���������FILTER����
#define ABNORMAL_MAX_POLAND_NUM 5 // ��ϵͳ���ȶ�������£���������query��Ĳ�ѯ��������
#define ABNORMAL_MAX_KEYWORD_NUM 4 // ��ϵͳ���ȶ�������£�����ѯ�������Ĺؼ��ʸ�����������TextQuery��UniqueQuery��
#define ABNORMAL_MAX_Q_LENGTH 30

#define INVALIDSLICEID 0
#define OPAND 1
#define OPOR 0
typedef char n8_t;
typedef short n16_t;
typedef int n32_t;
typedef long long n64_t;

typedef unsigned char u_n8_t;
typedef unsigned short u_n16_t;
typedef unsigned int u_n32_t;
typedef unsigned long long u_n64_t;

/* 
 * �����ֵ�õ���ͳһ����
 * �����Դ�������(32bit��64bit),Ҳ���Դ����ַ���
 * �������ַ���������ͨ�����2λ���ж�
 * �����λ: 10Ϊ�ַ���,11Ϊ����,����Ϊ����
 */
typedef n64_t ScoreType;

#define CHAR_FLAG 0x8000000000000000LL
#define HIGH_FLAG 0xC000000000000000LL
#define STR_TO_NUM(p) (((n64_t)(p)) | CHAR_FLAG)
#define NUM_TO_STR(n) (reinterpret_cast<char*>((n) ^ CHAR_FLAG))
#define IS_STRING(n) (((n) & HIGH_FLAG) != 0 && (((n) & HIGH_FLAG)^CHAR_FLAG) == 0)

/* �����ֶ����Ͷ���,���Ͷ����Ƕ��ֶζ��Ե�,����ע�ֶ�ֵ�ľ������� */
enum EFieldType
{
	ft_text,			//�ı�����,һ��Ϊ�ɱ䳤����Ҫ�ִʵ��ַ���
	ft_string,			//�ַ�������,һ��Ϊ�ı�������ּ�������
	ft_enum,			//ö������
	ft_integer,			//��������,�����͵��ֶο���NUMBER���͵�����
	ft_float,			//����������
	ft_long,			//����������,64bit
	ft_time,			//ʱ������,��ʽ�����Զ���
	ft_location,		//��ά��������
	ft_char,			//���ֽ�����(char)
    ft_online,			//online ����
	ft_property,		//property����
	ft_unknown,			//δ֪����
	ft_2tuple,			//��Ԫ��
	ft_triple,			//��Ԫ��
	ft_join,					//�����������
    ft_short,            //16bitԪ��,add by wanmc
    ft_float4,            //4�ֽڵ����ȸ���
	   ft_custom     //�û��Զ������ͣ���Ҫͨ��֪����loader���� 
};
//add by jinghua for Cache Type
enum ECacheType
{
	ct_default,
	ct_staticslice,
	ct_dynamicslice
};

// Uniq II --> unqi Info����
enum EFieldInfoType
{
	fi_unknown,
	fi_sum,				//��Uniq Info����sum
	fi_average,			//��Uniq Info����average
	fi_min,				//��Uniq Infoȡmin
	fi_max,				//��Uniq Infoȡmax
	fi_content			//����Uniq Info
};

/* �������Ͷ��� */
enum EIndexType
{
	it_text,			//�ı�����������ԴΪ��Ҫ�ִʵĿɱ䳤���ַ���������ģ������
	it_string,			//�ַ�����������ԴΪ����ִʵĿɱ䳤���ַ�����������ȷ����
	it_enum,			//ö������������ԴΪ�ַ����͵�ö�٣���"HangZhou"���������پ�ȷ����
	it_property,		//��������������ԴΪXML��ʽ���ַ���������רҵ������������
	it_number,			//��������������ԴΪ������ֵ����string������ʽ��ͬ���������Զ����ֵķ�Χ����
	it_range,			//��Χ����������ԴΪ������ֵ��������Χ���ң������û������Զ��巶Χ��
	it_scope,			//��������������ԴΪ��ά����,�����������
	it_unique,          //��������,Ϊ�˿���delete
	it_unknown			//δ֪����������������֮����������ͣ�������
};

/* ������Ϣ�������Ͷ��� */
enum EIndexExtend
{
	ie_occ_none = 1,			//�����ļ��в���Ҫ����λ�ù�ϵ
	ie_occ_array = 2,			//�����ļ���Ҫ�������е�λ�ù�ϵ
	ie_occ_first = 4,			//�����ļ���ֻҪ�����һ��λ��
	ie_int = 8,					//����String���͵�����,�ؼ���ʵ������һ������
	ie_pk = 16,					//����String���͵�����,�ؼ�����Ψһ��,��һ��primary key
	ie_md5 = 32,				//����String���͵�����,�ؼ���ʵ������һ��md5(128λ)������
	ie_double = 64,				//����String���͵�����,�ؼ���ʵ������һ��64λ������
	ie_range = 128,				//����Integer���͵�����,��Ҫ���������κ���з�Χ���ҹ���
	ie_occ_preload = 512,			//occlist�����ļ��Ƿ�Ԥ����
	ie_wildcard = 1024,			//����֧��wildcard��ѯ����
	ie_cache = 2048,			//�Բ�ѯ�������cache, Ĭ��TEXT���͵�PACKAGE����cache
	ie_stopword_ignore = 4096,	//��stopword�Ĵ������Ϊtrue��ʾ���ԣ����򷵻�null���
	ie_bitmap_preload = 8192,      //bitmap�ļ��Ƿ�Ԥ����
	ie_bits = 16384,			// ��Integer���͵�����,ȡInteger��bitλ��������
	ie_read_position = 32768,             // ��չ��λ�ù���
        ie_unload = 65536,                      //�����ص��ڴ���
	ie_index_freq = 131072,             //�Ƿ�������Ƶ��
    ie_unknown = 262143
};

enum ELoadType
{
        lt_preload,
        lt_load,
        lt_unload,   //��ȡ�����page cache
        lt_directio, //ʵ�����Ƕ�ȡ��
        lt_unknown
};


//֧�����Ե����Ͷ���
enum ESupportLanguage
{
	sl_English,
	sl_Chinese,
	sl_Traditional,
	sl_Japanese,
	sl_Korean,
	sl_unknown
};

//�ֶ�ֵ������ı������Ͷ���
enum ESupportCoding
{
	sc_UTF8,
	sc_Unicode,
	sc_Big5,
	sc_GBK,
	sc_Ansi
};

//���ֶ����ݴ�������Ͷ���
enum EFieldFixed
{
	ff_Fixed,			//�̶�����
	ff_Dynamic,			//��̬����������ժҪ
	ff_Highlight,		//��������ժҪ
	ff_Text				//ȫ���ı�
};

//�����������������ֱ仯��״̬
enum EBuildState
{
	bs_new = 'N',
	bs_merge = 'M',
	bs_dispose = 'D',
	bs_erase = 'E',
	bs_incre = 'A',
	bs_change = 'C',
	bs_unknown = 'U'
};

// ��ѯ�Ӿ�֮������ӷ���
enum EQuerySign
{
	qs_and,			//�߼�������
	qs_or,			//�߼�������
	qs_not,			//�߼�������
	qs_left,		//������
	qs_right,		//������
	qs_prohibite,	//��ֹ���㣬�����ķ��ؼ����а�������ϵ����ж��������˵�
	qs_require,		//�ڷ��صĽ���б���������ؼ���
	qs_unknown,		//δ֪����(Query)
	qs_otherwise,   // Otherwise ������
	qs_bothand		// bothand ������ (A BOTHAND B) <=> (A AND B) OR A OR B
};

//��ѯ�Ӿ������

//���ؽ�����ֶ�����,��Щ����ָʾ�����ݵ���Դ������
enum EResultType
{
	rt_notNull,		//���ؽ������Ϊ��
	rt_attribute,	//���ؽ��������������attribute��������
	rt_union,		//��������union table������,����������Ϣ��
	rt_primary,		//���ؽ��Ϊ��һ�����ֵ
	rt_secondary,	//���ؽ��Ϊ�ڶ������ֵ
	rt_unknown,		//δָ������, Ĭ�Ͻ������detail������
	rt_joinarea,	//���ؽ��Ϊ����Area������
	rt_joinarea_attribute, // ���ؽ��Ϊ����Area��������������
	rt_custom  //�û��Զ������ͣ���Ҫͨ��loadr����
};
/* �Բ�ѯ����ؼ��ֵĽ���˵�� */
enum EKeyExplain {
	ke_unknown = 0,					//�޽���˵��
	ke_synonmys = 1,				//ͬ�������
	ke_category = 2,				//��Ŀ����
	ke_spellcheck_less = 3,			//�����������ƴд���
	ke_wordtokenizer_less = 4,		//����������ķִʴ���
	ke_spellCheck = 5,				//�������ƴд���
	ke_wordtokenizer = 6,			//������ķִʴ���
	ke_dodelete	 = 7				//ɾ����¼
};
/* �Էֲ�ʽʱ��ѯ����Ķ��� */
enum ESearchStep {
	ss_ignore,		//��ͨ������ģʽ,�Ƿֲ�ʽ
	ss_first,		//�ֲ�ʽ�ĵ�һ������
	ss_merge,		//�ֲ�ʽ��merge״̬
	ss_second		//�ֲ�ʽ�ĵڶ�������
};

#if 0
enum EnumWorkMode {
    enum_SearchAndDocServer = 0,	//search ��DocData����	
    enum_SearchServer,		//ֻ��Search����
    enum_DocDataServer		//ֻ��DocData����
};
#endif

/**The role in Document Separation mode. jinhui.li added in 2009.1.19*/
const int ROLE_SEARCH 	= 1 << 0; 
const int ROLE_DOC 		= 1 << 1;
const int ROLE_MIX 		= ROLE_SEARCH | ROLE_DOC; //the role of searcher and doc

//������Χ�ṹ
struct SRange
{
	n32_t nMax;		//���ֵ
	n32_t nMin;		//��Сֵ
	bool operator<(const SRange& rhs) const
	{
		return nMin < rhs.nMin;
	}
	bool operator!=(const SRange& rhs) const
	{
		return !(operator==(rhs));
	}
	bool operator==(const SRange& rhs) const
	{
		return nMin==rhs.nMin && nMax==rhs.nMax;
	}
	static const n32_t MAXVALUE = 0x7FFFFFFF;
	static const n32_t MINVALUE = 0x0;
};
//��ѯ������Ϣ
struct QueryConfInfo {
	QueryConfInfo()
	{
        mPoolExtendSize = 1;
		loadAreaName[0] = '\0';
        iMaxDocCacheNum = 1000000;
		iAverageDocSize = 1024;
		role = ROLE_MIX;
		m_nMaxMemPoolSize = DEFAULT_MAX_MEMPOOL_SIZE;
		m_nMaxQueryLength = MAX_QUERY_LENGTH;

		m_nDocsFoundThreshold = 100000;
		m_nLatencyThreshold = 20;
		m_szAbnormalQueryFile[0] = '\0';
		m_nMaxSetOpComplexity = 10000000;
	}
	QueryConfInfo(const QueryConfInfo &other)
	{
		strncpy(loadAreaName, other.loadAreaName, 256);
		mPoolExtendSize = other.mPoolExtendSize;
		iMaxDocCacheNum = other.iMaxDocCacheNum;
		iAverageDocSize = other.iAverageDocSize;
		role = other.role;
		m_nMaxMemPoolSize = other.m_nMaxMemPoolSize;
		m_nMaxQueryLength = other.m_nMaxQueryLength;
		m_nDocsFoundThreshold = other.m_nDocsFoundThreshold;
		m_nLatencyThreshold = other.m_nLatencyThreshold;
		strncpy(m_szAbnormalQueryFile, other.m_szAbnormalQueryFile, 1024);
		m_nMaxSetOpComplexity = other.m_nMaxSetOpComplexity;
	}
	~QueryConfInfo() {}
	char loadAreaName[256];			//Ԥ�ȼ��ص�area
	basis::n32_t mPoolExtendSize;	//mempool��size����
	basis::n32_t iMaxDocCacheNum;   //���cache��doc��
	basis::n32_t iAverageDocSize;   //ƽ���ĵ���С
	int role; //�����Ľ�ɫ
	int    m_nMaxMemPoolSize;  //�����߳�����mempool�ɷ����size
	int    m_nMaxQueryLength; //����query����

    // added by zhanghp for stablizing the system when it is attacked by abnormal queries
    u_n32_t m_nDocsFoundThreshold;	
    u_n32_t m_nLatencyThreshold;	
	char m_szAbnormalQueryFile[1024];
	u_n32_t m_nMaxSetOpComplexity;
	// ended by zhanghp
};
union RangeType
{
  n8_t cvalue;
  n16_t svalue;
  float fvalue4;
    n32_t nvalue;
    n64_t lvalue;
    double fvalue;
    u_n64_t keyHash;
    char *szValue;
};

typedef RangeType ValueType;

struct NumRange
{
    RangeType min;
    RangeType max;
};

typedef NumRange SNumRange;

enum ENextPeriod {
	np_1st_less_now = 1,
	np_2nd_less_now
};

}

#endif
