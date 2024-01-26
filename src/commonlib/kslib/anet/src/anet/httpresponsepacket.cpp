#include <stdio.h>
#include <anet/httpresponsepacket.h>
#include <anet/databuffer.h>
#include <anet/packet.h>
namespace anet {

/*
 * ���캯��
 */
HttpResponsePacket::HttpResponsePacket() {
    _status = true;
    _body = NULL;
    _bodyLen = 0;
    _isKeepAlive = false;
}

/*
 * ��������
 */
HttpResponsePacket::~HttpResponsePacket() {
    if (_body) {
        ::free(_body);
    }
}

/*
 * ��װ
 */
bool HttpResponsePacket::encode(DataBuffer *output) {
    if (_status) { //HTTP/1.1 200 OK
        output->writeBytes(ANET_HTTP_STATUS_OK, strlen(ANET_HTTP_STATUS_OK));
    } else { // HTTP/1.1 404 Not Found
        output->writeBytes(ANET_HTTP_STATUS_NOTFOUND, strlen(ANET_HTTP_STATUS_NOTFOUND));
    }
    //�̶��ֶ�
    if (_isKeepAlive) {
        output->writeBytes(ANET_HTTP_KEEP_ALIVE, strlen(ANET_HTTP_KEEP_ALIVE));
    } else {
        output->writeBytes(ANET_HTTP_CONN_CLOSE, strlen(ANET_HTTP_CONN_CLOSE));
    }
    if (_headerMap.find("Content-Type") == _headerMap.end()) {
        output->writeBytes(ANET_HTTP_CONTENT_TYPE, strlen(ANET_HTTP_CONTENT_TYPE));
    }
    char tmp[64];
    int len = sprintf(tmp, ANET_HTTP_CONTENT_LENGTH, _bodyLen);
    output->writeBytes(tmp, len);

    // �û��Զ��峤��
    for (STRING_MAP_ITER it=_headerMap.begin(); it!=_headerMap.end(); it++) {
        output->writeBytes(it->first.c_str(), strlen(it->first.c_str()));
        output->writeBytes(": ", 2);
        output->writeBytes(it->second.c_str(), strlen(it->second.c_str()));
        output->writeBytes("\r\n", 2);
    }

    // ����
    output->writeBytes("\r\n", 2);
    // bodyLen
    output->writeBytes(_body, _bodyLen);
    //assert(_packetHeader._dataLen == output->getDataLen());

    return true;
}

/*
 * �⿪
 */
bool HttpResponsePacket::decode(DataBuffer *input, PacketHeader *header) {
    return true;
}

/*
 * ����header
 */
void HttpResponsePacket::setHeader(const char *name, const char *value) {
    if (name[0] == 'C') {
        if (strcmp(name, "Connection") == 0 || strcmp(name, "Content-Length") == 0) {
            return;
        }
    }
    _headerMap[name] = value;
}

/*
 * ����״̬
 */
void HttpResponsePacket::setStatus(bool status) {
    _status = status;
}

/*
 * ��������
 */
void HttpResponsePacket::setBody(const char *body, int len) {
    if (body && (len > 0)) {
        _body = (char *) malloc(len);
        assert(_body);
        memcpy(_body, body, len);
        _bodyLen = len;
    }
}

/*
 * �Ƿ�keepalive
 */
void HttpResponsePacket::setKeepAlive(bool keepAlive) {
    _isKeepAlive = keepAlive;
}


}

