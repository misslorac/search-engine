#include <anet/packetqueue.h>
#include <anet/packet.h>
namespace anet {


/*
 * ���캯��
 */
PacketQueue::PacketQueue() {
    _head = NULL;
    _tail = NULL;
    _size = 0;
}
/*
 * ��������
 */
PacketQueue::~PacketQueue() {
}
    
/*
 * ������
 */
Packet *PacketQueue::pop() {
    if (_head == NULL) {
        return NULL;
    }
    Packet *packet = _head;
    _head = _head->_next;
    if (_head == NULL) {
        _tail = NULL;
    }
    _size --;
    return packet;
}
/*
 * ������
 */
void PacketQueue::push(Packet *packet) {
    packet->_next = NULL;
    if (_tail == NULL) {
        _head = packet;
    } else {
        _tail->_next = packet;
    }
    _tail = packet;
    _size++;
}
  
/*
 * ����
 */ 
size_t PacketQueue::size() {
    return _size;
}

/*
 * �Ƿ�Ϊ��
 */
bool PacketQueue::empty() {
    return (_size == 0);
}


void PacketQueue::moveTo(PacketQueue *destQueue) {
    if (_head == NULL) { // �ǿ���
        return;
    }
    if (destQueue->_tail == NULL) {
        destQueue->_head = _head;
    } else {
        destQueue->_tail->_next = _head;
    }
    destQueue->_tail = _tail;
    destQueue->_size += _size;
    _head = _tail = NULL;
    _size = 0;
}

void PacketQueue::moveBack(PacketQueue *srcQueue) {
    if (NULL == srcQueue->_head) {
        return;
    }
    if (NULL == _tail) {
        _head = srcQueue->_head;
    } else {
        _tail->_next = srcQueue->_head;
    }
    _tail = srcQueue->_tail;
    _size += srcQueue->_size;
    srcQueue->_head = srcQueue->_tail = NULL;
    srcQueue->_size = 0;
}

/*
 * �õ���ʱ��packet
 */
Packet *PacketQueue::getTimeoutList(int64_t now)
{
    Packet *list, *tail;
    list = tail = NULL;
    while (_head != NULL) {
        int64_t t = _head->getExpireTime();
        if (t >= now) break;
        if (tail == NULL) {
            list = _head;
        } else {
            tail->_next = _head;
        }
        tail = _head;
        _size --;
        _head = _head->_next;
    }
    if (tail) {
        tail->_next = NULL;
    }
    
    if (!_head) {
        _tail = _head;
    }

    return list;
}
}

