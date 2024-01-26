#ifndef ANET_HTTP_PACKET_STREAMER_H
#define ANET_HTTP_PACKET_STREAMER_H
#include <anet/ipacketfactory.h>
#include <anet/defaultpacketstreamer.h>
#include <anet/httprequestpacket.h>
#include <anet/httpresponsepacket.h>

namespace anet {

  class DataBuffer;
  class PacketHeader;
class HttpPacketStreamer : public DefaultPacketStreamer {
public:
    /*
     * ���캯��
     */
    HttpPacketStreamer(IPacketFactory *factory);
    /*
     * ���ݰ���Ϣ������
     */
    bool getPacketInfo(DataBuffer *input, PacketHeader *header, bool *broken);
};

/**
 * packet��factory, ȱʡ��httpd packet factory
 *
 * pcode = 1 �������
 * pcode = 0 ����Ӧ��
 */
class DefaultHttpPacketFactory : public IPacketFactory {
public:
    Packet *createPacket(int pcode) {
        if (pcode == 1) {
            return new HttpRequestPacket();
        } else {
            return new HttpResponsePacket();
        }
    }
};

}

#endif

