#ifndef ANET_HTTP_REQUEST_PACKET_H
#define ANET_HTTP_REQUEST_PACKET_H
#include <anet/packet.h>
#include <ext/hash_map>
namespace anet {
class DataBuffer;
class PacketHeader;
struct eqstr {
    bool operator()(const char* s1, const char* s2) const {
        return strcmp(s1, s2) == 0;
    }
};
typedef __gnu_cxx::hash_map<const char*, const char*, __gnu_cxx::hash<const char*>, eqstr> PSTR_MAP;
typedef PSTR_MAP::iterator PSTR_MAP_ITER;

class HttpRequestPacket : public Packet {
  friend class HttpRequestAndResponsePacketTF;
public:
    /*
     * ���캯��
     */
    HttpRequestPacket();

    /*
     * ��������
     */
    ~HttpRequestPacket();


    /*
     * ��������ݰ��ĳ���
     */
    void countDataLen();
    /*
     * ��װ
     */
    bool encode(DataBuffer *output);

    /*
     * �⿪
     */
    bool decode(DataBuffer *input, PacketHeader *header);

    /*
     * ��ѯ��
     */
    char *getQuery();

    /*
     * �Ƿ�keepalive
     */
    bool isKeepAlive();

    /*
     * Ѱ������ͷ��Ϣ
     */
    const char *findHeader(const char *name);

private:
    char *_strHeader;       // ����ͷ���ݵ�buffer
    char *_strQuery;        // ��ѯ��
    bool _isKeepAlive;      // �Ƿ�֧��keepalive
    int _method;            // get - 1
    int _version;           // http version 1-"HTTP/1.0"; 2="HTTP/1.1+"
    PSTR_MAP _headerMap;    // ����ͷ��Ϣ��map
};

}

#endif

