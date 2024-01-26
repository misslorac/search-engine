#include <anet/httprequestpacket.h>
#include <anet/databuffer.h>
#include <anet/log.h>
#include <ext/hash_map>
namespace anet {

/*
 * ���캯��
 */
HttpRequestPacket::HttpRequestPacket() {
    _strHeader = NULL;
    _strQuery = NULL;
    _isKeepAlive = true;
    _method = 0;
    _version = 0;
}

/*
 * ��������
 */
HttpRequestPacket::~HttpRequestPacket() {
    if (_strHeader) {
        ::free(_strHeader);
    }
}

/*
 * ��װ
 */
bool HttpRequestPacket::encode(DataBuffer *output) {
    return true;
}

/*
 * �⿪
 */
bool HttpRequestPacket::decode(DataBuffer *input, PacketHeader *header) {
    int len = header->_dataLen;
    _strHeader = (char*) malloc(len+1);
    assert(_strHeader);
    input->readBytes(_strHeader, len);
    _strHeader[len] = '\0';
    int line = 0;
    int first = 1;
    bool notSeeConnection = true;
    ANET_LOG(SPAM,"Http Header:\n%sHttp Header End.",_strHeader); 
    char *p, *name = NULL, *value;
    p = value = _strHeader;
    while (*p) {
        // ��ÿһ��
        if (*p == '\r' && *(p+1) == '\n') {
            if (value == p && line > 0) { // header ������
                break;
            }
            *p = '\0';
            // ȥǰ�ո�
            while (*value == ' ') value ++;
            if (line > 0) {
                if (strcmp(name, "Connection") == 0) {
                    if ( strcasecmp(value, CONNECTION_CLOSE) == 0) {
                        _isKeepAlive = false;
                    } else if (strcasecmp(value, CONNECTION_KEEP_ALIVE) != 0) {
                        _isKeepAlive = _version > 1;
                    } else {
                        _isKeepAlive = true;
                    }
                    notSeeConnection = false;
                } else {
                    _headerMap[name] = value;
                }
            } else {
                if (first != 3) {
                    return false;
                }
                if (strncmp(value, "HTTP/1.0", 8) == 0) {
                    _version = 1;
                } else {
                    _version = 2;
                }
            }
            value = p + 2;
            line ++;
            first = 1;
        } else if (line == 0 && *p == ' ') { // ����
            if (value < p) {
                switch (first) {
                case 1:
                    if (strncmp(value, "GET ", 4) == 0) {    // ��GET ����
                        _method = 1;
                    } else {
                        ANET_LOG(WARN, "Only support \"GET \" method now!");
                        return  false;
                    }
                    break;
                case 2:
                    _strQuery = value;
                    break;
                }
                first ++;
            }
            *p = '\0';
            value = p+1;
        } else if (*p == ':' && first == 1) {
            *p = '\0';
            name = value;
            value = p + 1;
            first = 0;
        }
        p ++;
    }

    if (notSeeConnection) {
        _isKeepAlive = _version > 1;
    }
    ANET_LOG(SPAM,"Method: %d, Version: %d. Keep-Alive: %s", _method,
             _version, _isKeepAlive ? "yes" : "no");
    return true;
}

/*
 * ��ѯ��
 */
char *HttpRequestPacket::getQuery() {
    return _strQuery;
}

/*
 * �Ƿ�keepalive
 */
bool HttpRequestPacket::isKeepAlive() {
    return _isKeepAlive;
}

/*
 * Ѱ������ͷ��Ϣ
 */
const char *HttpRequestPacket::findHeader(const char *name) {
    PSTR_MAP_ITER it = _headerMap.find(name);
    if (it != _headerMap.end()) {
        return it->second;
    }
    return NULL;
}


}


