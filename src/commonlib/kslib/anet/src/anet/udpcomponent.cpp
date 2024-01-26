#include <anet/socket.h>
#include <anet/udpcomponent.h>

namespace anet {
/**
  * ���캯������Transport���á�
  *
  * @param owner:       Transport
  * @param socket:      Socket
  * @param streamer:    ���ݰ���˫��������packet����������������
  * @param serverAdapter:  ���ڷ������ˣ���Connection��ʼ����Channel����ʱ�ص�ʱ��
  */
UDPComponent::UDPComponent(Transport *owner, Socket *socket, IPacketStreamer *streamer,
                           IServerAdapter *serverAdapter) : IOComponent(owner, socket) {
    _streamer = streamer;
    _serverAdapter = serverAdapter;
}

/*
 * ��������
 */
UDPComponent::~UDPComponent() {}

/*
 * ���ӵ�ָ���Ļ���
 *
 * @param  isServer: �Ƿ��ʼ��һ����������Connection
 * @return �Ƿ�ɹ�
 */
bool UDPComponent::init(bool isServer) {
    if (!isServer) { // ��Ҫconnect, ��accept������

        if (!_socket->connect()) {
            return false;
        }
    }
    _isServer = isServer;
    return true;
}

/*
 * �ر�
 */
void UDPComponent::close() {}

/**
   * �������ݿ�д��ʱ��Transport����
   *
   * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
   */
bool UDPComponent::handleWriteEvent() {
    return true;
}

/**
 * �������ݿɶ�ʱ��Transport����
 *
 * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
 */
bool UDPComponent::handleReadEvent() {
    return true;
}


}
