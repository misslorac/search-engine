/**
 * File name: defaultpacket.cpp
 * Author: zhangli
 * Create time: 2008-12-25 11:10:50
 * $Id$
 * 
 * Description: ***add description here***
 * 
 */

#include <anet/defaultpacket.h>
#include <anet/log.h>
#include <anet/databuffer.h>

namespace anet {
DefaultPacket::DefaultPacket() {
    _body = NULL;
    _bodyLength = 0;
    _capacity = 0;
}

DefaultPacket::~DefaultPacket() {
    if (_body) ::free(_body);
}

bool DefaultPacket::setBody(const char *str, size_t length) {
    if (0 == length) {
        if (_body) ::free(_body);
        _body = NULL;
        _bodyLength = 0;
        _capacity = 0;
        return true;
    }
    int oriLen = _bodyLength;
    _bodyLength = 0;
    if (appendBody(str, length)) {
        return true;
    } else {
        _bodyLength = oriLen;
        return false;
    }
}

bool DefaultPacket::appendBody(const char *str, size_t length) {
    if (0 == length || NULL == str) {
        return false;
    }
    
    if (_capacity - _bodyLength < length) {
        int tmpSize = (_capacity*2 - _bodyLength) >= length ?
                      _capacity*2 : length*2 + _bodyLength;
        if (!setCapacity(tmpSize)) {
            return false;
        }
    }
    memcpy(_body + _bodyLength, str, length);
    _bodyLength += length;
     return true;
}

const char* DefaultPacket::getBody(size_t &length) const {
    length = _bodyLength;
    return _body;
}

char* DefaultPacket::getBody(size_t &length) {
    length = _bodyLength;
    return _body;
}

const char* DefaultPacket::getBody() const {
    return _body;
}

char* DefaultPacket::getBody() {
    return _body;
}

size_t DefaultPacket::getBodyLen() {
    return _bodyLength;
}

bool DefaultPacket::setCapacity(size_t capacity) {
    if (capacity < _bodyLength) {
        return false;
    }
    if (capacity == _capacity) {
        return true;
    }
    char *tmp = (char *)realloc(_body, capacity);
    if (!tmp) {
        return false;
    }
    _body = tmp;
    _capacity  = capacity;
    return true;
}

size_t DefaultPacket::getCapacity() {
    return _capacity;
}

bool DefaultPacket::encode(DataBuffer *output) {
    if (_bodyLength > 0) {
        output->writeBytes(_body, _bodyLength);
    }
    return true;
}

bool DefaultPacket::decode(DataBuffer *input, PacketHeader *header) {
    assert(input->getDataLen() >= header->_dataLen);
    bool rc = setBody(input->getData(), header->_dataLen);
    input->drainData(header->_dataLen);
    return rc;
} 

}/*end namespace anet*/
