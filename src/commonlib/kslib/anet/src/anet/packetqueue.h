#ifndef ANET_PACKET_QUEUE_H_
#define ANET_PACKET_QUEUE_H_
#include <sys/types.h>
namespace anet {
  class Packet;
  class PacketQueue {
public:
    /*
     * ���캯��
     */
    PacketQueue();
    
    /*
     * ��������
     */
    ~PacketQueue();
    
    /*
     * ����
     */
    Packet *pop();
    
    /*
     * ����
     */
    void push(Packet *packet);
  
    /*
     * ����
     */ 
    size_t size();

    /*
     * �Ƿ�Ϊ��
     */
    bool empty();

    /**
     * move all entries of current queue to the tail of destQueue 
     */
    void moveTo(PacketQueue *destQueue);

    /**
     * move all entries of  srcQueue back to the head of current queue
     */
    void moveBack(PacketQueue *srcQueue);

    /*
     * �õ���ʱ��packet
     */
    Packet *getTimeoutList(int64_t now);

protected:
    Packet *_head;  // ��ͷ
    Packet *_tail;  // ��β
    size_t _size;      // Ԫ������
};

}

#endif

