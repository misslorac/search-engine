#ifndef ANET_HTTP_RESPONSE_PACKET_H
#define ANET_HTTP_RESPONSE_PACKET_H
#include <anet/packet.h>
#include <string>
#include <ext/hash_map>
namespace anet {

struct str_hash {
    size_t operator()(const std::string& str) const {
        return __gnu_cxx::__stl_hash_string(str.c_str());
    }
};
typedef __gnu_cxx::hash_map<std::string, std::string, str_hash> STRING_MAP;
typedef STRING_MAP::iterator STRING_MAP_ITER;

#define ANET_HTTP_STATUS_OK "HTTP/1.1 200 OK\r\n"
#define ANET_HTTP_STATUS_NOTFOUND "HTTP/1.1 404 Not Found\r\n"
#define ANET_HTTP_KEEP_ALIVE "Connection: Keep-Alive\r\nKeep-Alive: timeout=10, max=10\r\n"
#define ANET_HTTP_CONN_CLOSE "Connection: close\r\n"
#define ANET_HTTP_CONTENT_TYPE "Content-Type: text/html\r\n"
#define ANET_HTTP_CONTENT_LENGTH "Content-Length: %d\r\n"

class HttpResponsePacket : public Packet {
public:
    /*
     * ���캯��
     */
    HttpResponsePacket();

    /*
     * ��������
     */
    ~HttpResponsePacket();

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
     * ����header
     */
    void setHeader(const char *name, const char *value);

    /*
     * ����״̬
     */
    void setStatus(bool status);

    /*
     * ��������
     */
    void setBody(const char *body, int len);

    /*
     * �Ƿ�keepalive
     */
    void setKeepAlive(bool keepAlive);

private:
    bool _status;                   // ���ص�״̬, true => 200, false => 404
    char *_body;                    // ���ص�����
    int _bodyLen;                   // ���������ҳ���
    STRING_MAP _headerMap;          // ��������ͷ��Ϣ
    bool _isKeepAlive;              // �Ƿ�keepalive
};

}

#endif

