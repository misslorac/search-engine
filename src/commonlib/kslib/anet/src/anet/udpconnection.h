#ifndef ANET_UDPCONNECTION_H_
#define ANET_UDPCONNECTION_H_
#include <anet/connection.h>
namespace anet {

  class UDPConnection : public Connection {
    /*
     * ���캯��
     */
    UDPConnection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    /*
     * ���캯��
     */
    ~UDPConnection();

    /*
     * д������
     *
     * @return �Ƿ�ɹ�
     */
    bool writeData();

    /*
     * ��������
     *
     * @return ��������
     */
    bool readData();

};

}

#endif /*UDPCONNECTION_H_*/
