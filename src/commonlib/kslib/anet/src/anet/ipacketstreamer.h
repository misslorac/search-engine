#ifndef ANET_IPACKETSTREAMER_H_
#define ANET_IPACKETSTREAMER_H_
#include <anet/controlpacket.h>
#include <anet/ipacketfactory.h>
namespace anet {
class DataBuffer;
class StreamingContext;
class IPacketFactory;

class IPacketStreamer {
public:

    IPacketStreamer(IPacketFactory *factory) {
        _factory = factory;
        _existPacketHeader = true;
    }

    virtual ~IPacketStreamer() {}

    virtual StreamingContext* createContext() = 0;

    /*
     * Determine whether data in DataBuffer is enough.
     * If return true,  ANET will process DataBuffer and construct 
     * a Packet. If return false, ANET will read more data from 
     * socket.
     *
     * @param input  data source
     * @param header some necessary information will be recorded in
     * header.
     * @return Return true if there's enough data, else return false.
     */
    virtual bool getPacketInfo(DataBuffer *input, PacketHeader *header, bool *broken) = 0;

    /*
     * �԰��Ľ���
     *
     * @param input
     * @param header
     * @return ���������ݰ�
     */
    virtual Packet *decode(DataBuffer *input, PacketHeader *header) = 0;

    /*
     * ��Packet����װ
     * 
     * @param packet ���ݰ�
     * @param output ��װ��������� 
     * @return �Ƿ�ɹ�
     */
    virtual bool encode(Packet *packet, DataBuffer *output) = 0;

    /*
     * �Ƿ������ݰ�ͷ
     */
    virtual bool existPacketHeader() {
        return _existPacketHeader;
    }
    
    /**
     * Read Data from DataBuffer and construct Packet. The Packet 
     * is recorded in StreamingContext. This function is added 
     * because muliti steps is needed when constructing a packet 
     * from DataBuffer.
     * This virtual function is implemented in DefaultPacketStreamer.
     
     * @param dataBuffer source data
     * @param context The context when you processData.
     * @return true return true when processData successfully and a
     * packet will be generated. Return false when processData 
     * failed.
     */
    virtual bool processData(DataBuffer *dataBuffer, 
			     StreamingContext *context) = 0;



protected:
    IPacketFactory *_factory;   // ����packet
    bool _existPacketHeader;    // �Ƿ���packet header, ��http���Լ�Э��Ͳ���Ҫ���ͷ��Ϣ
};
}

#endif /*RUNNABLE_H_*/
