#include <anet/udpconnection.h>
namespace anet {

/*
 * ���캯��
 */
UDPConnection::UDPConnection(Socket *socket, IPacketStreamer *streamer,
                             IServerAdapter *serverAdapter) : Connection(socket, streamer, serverAdapter) {}

/*
 * ���캯��
 */
UDPConnection::~UDPConnection() {}

/*
 * д������
 *
 * @return �Ƿ�ɹ�
 */
bool UDPConnection::writeData() {
    return true;
}

/*
 * ��������
 *
 * @return ��������
 */
bool UDPConnection::readData() {
    return true;
}

}
