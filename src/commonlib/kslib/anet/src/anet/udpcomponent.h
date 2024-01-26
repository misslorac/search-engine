#ifndef ANET_UDPCOMPONENT_H_
#define ANET_UDPCOMPONENT_H_
#include <anet/iocomponent.h>
#include <ext/hash_map>
namespace anet {
  class UDPConnection;
class UDPComponent : public IOComponent {

public:
    /**
     * ���캯������Transport���á�
     *
     * @param owner:      Transport
     * @param socket:     Socket
     * @param streamer:   ���ݰ���˫��������packet����������������
     * @param serverAdapter:  ���ڷ������ˣ���Connection��ʼ����Channel����ʱ�ص�ʱ��
     */
    UDPComponent(Transport *owner, Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    /*
     * ��������
     */
    ~UDPComponent();

    /*
        * ��ʼ��
        * 
        * @return �Ƿ�ɹ�
        */
    bool init(bool isServer = false);

    /*
     * �ر�
     */
    void close();

    /*
        * �������ݿ�д��ʱ��Transport����
        * 
        * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
        */
    bool handleWriteEvent();

    /*
     * �������ݿɶ�ʱ��Transport����
     *
     * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
     */
    bool handleReadEvent();

private:
    __gnu_cxx::hash_map<int, UDPConnection*> _connections;  // UDP���Ӽ���
    IPacketStreamer *_streamer;                             // streamer
    IServerAdapter *_serverAdapter;                         // serveradapter
};
}

#endif /*UDPCOMPONENT_H_*/
