#ifndef ANET_HTTPPACKET_H_
#define ANET_HTTPPACKET_H_

#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#include <anet/packet.h>
#include <anet/databuffer.h>
#include <anet/defaultpacket.h>

namespace anet {
struct keyCompareCaseInsensive {
    bool operator()(const std::string &s1, 
                    const std::string &s2) 
    {
        const char* data1 = s1.c_str();
        const char* data2 = s2.c_str();
        return strcasecmp(data1, data2) < 0;
    }
    bool operator()(const char *s1, const char *s2) {
        return strcasecmp(s1, s2) < 0;
    }
};

class HTTPPacket : public DefaultPacket
{
public:
    HTTPPacket();
    enum HTTPPacketType 
    {
        PT_REQUEST = 0,
        PT_RESPONSE,
        PT_INVALID
    };
    enum HTTPVersion {
        HTTP_1_0  =  0,
        HTTP_1_1,
        HTTP_UNSUPPORTED
    };
    enum HTTPMethod{
        HM_OPTIONS = 0,
        HM_HEAD,
        HM_GET,
        HM_POST,
        HM_PUT,
        HM_DELETE,
        HM_TRACE,
        HM_CONNECT,
        HM_UNSUPPORTED
    };

    void setPacketType(HTTPPacketType type);
    HTTPPacketType getPacketType();
    
    void setVersion(HTTPVersion version);
    HTTPVersion getVersion();

    void setMethod(HTTPMethod method);
    HTTPMethod getMethod();
    void setMethod(const char* method);
    const char* getMethodString();
    
    void setURI(const char *);
    const char* getURI();

    void setReasonPhrase(const char *);
    const char* getReasonPhrase();
    

    
    bool addHeader(const char *key, const char *value);
    bool removeHeader(const char *key);
    const char* getHeader(const char *key);

    void setStatusCode(int);
    int getStatusCode();
    
    void setKeepAlive(bool keepalive);
    bool isKeepAlive();

    virtual ~HTTPPacket();

    bool encode(DataBuffer *output);
    bool encodeStartLine(DataBuffer *output);
    bool encodeHeaders(DataBuffer *output);
    bool encodeBody(DataBuffer *output);

    bool decode(DataBuffer *input, PacketHeader *header) {
        return false;
    }
protected:
    typedef std::map<char*, char*, keyCompareCaseInsensive> HeadersType;
public:
    typedef HeadersType::const_iterator ConstHeaderIterator;
    ConstHeaderIterator headerBegin();
    ConstHeaderIterator headerEnd();

protected:
    char* strdupTrimed(const char* str, bool front = true, bool end = true);
    bool toLowerCase(char *str, int length);
protected:
    HTTPPacketType _type;
    HTTPVersion _version;
    HTTPMethod _method;
    char *_URI;
    int _statusCode;
    char *_reasonPhrase;
    char *_extendMethod;
    HeadersType _headers;

    static const char *empty;
};

}/*end namespace anet*/
#endif /*ANET_HTTPPACKET_H_*/
