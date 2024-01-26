#ifndef ANET_PACKET_H_
#define ANET_PACKET_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace anet {

#define ANET_PACKET_FLAG 0x416e4574  // AnEt
#define CONNECTION_CLOSE "close"
#define CONNECTION_KEEP_ALIVE "Keep-Alive"
class DataBuffer;
class Channel;
class PacketHeader {
public:
    uint32_t _chid;         // ͨ��ID
    int32_t _pcode;             // ���ݰ�����
    int32_t _dataLen;           // ���ݰ�body����(��ͷ��Ϣ��)
};

class Packet {
    friend class PacketQueue;

public:
    /*
     * ���캯��, ��������
     */
    Packet();

    /*
     * ��������
     */
    virtual ~Packet();

    /*
     * ����ChannelID
     */
    void setChannelId(uint32_t chid) {
        _packetHeader._chid = chid;
    }

    /*
     * �õ�Channel ID
     */
    uint32_t getChannelId() {
        return _packetHeader._chid;
    }

    /*
     * �õ����ݰ�header info
     */
    PacketHeader *getPacketHeader() {
        return &_packetHeader;
    }

    /*
     * �������ݰ�header info
     */
    void setPacketHeader(PacketHeader *header) {
        if (header) {
            memcpy(&_packetHeader, header, sizeof(PacketHeader));
        }
    }

  /**
   * The Packet is freed through this->free(). In most situation,
   * packet are freed in TCPConnection::writeData() atomatically
   * through this function.
   */
  virtual void free() {
    delete this;
  }

    /*
     * �Ƿ����ݰ�
     */
    virtual bool isRegularPacket() {
        return true;
    }

    /** 
     * Write data into DataBuffer. This function is called by 
     * Streamer. Streamer uses this function write data into 
     * DataBuffer. The packet will be deleted through packet->free()
     * after this function return.
     *
     * @param output target DataBuffer
     * @return Return true when decode success! Else return false.
     */
    virtual bool encode(DataBuffer *output) = 0;

    /**
     * Read data form DataBuffer according to the information in
     * PacketHeader and construct packet. The DataBuffer contains
     * all the data you need. PacketHeader records dataLength and
     * some other information.
     *
     * @param input source data
     * @param header packet information
     * @return Return true when decode success! Else return false.
     */
    virtual bool decode(DataBuffer *input, PacketHeader *header) = 0;

    /*
     * ��ʱʱ��
     */
    int64_t getExpireTime() {
        return _expireTime;   
    }
    
    /*
     * ���ù���ʱ��
     * 
     * @param milliseconds ������, 0Ϊ��������
     */
    void setExpireTime(int milliseconds);
    
    /*
     * ����Channel
     */
    void setChannel(Channel *channel);

    /*
     * �õ�Channel
     */
    Channel *getChannel()
    {
        return _channel;
    }    
    
    /*
     * �õ�next
     */
    Packet *getNext()
    {
        return _next;
    }

protected:
    PacketHeader _packetHeader; // ���ݰ���ͷ��Ϣ
    int64_t _expireTime;        // ����ʱ��
    Channel *_channel;
    
    Packet *_next;              // ����packetqueue����
};

}

#endif /*PACKET_H_*/
