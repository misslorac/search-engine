#ifndef ANET_TCPCOMPONENT_H_
#define ANET_TCPCOMPONENT_H_
#include <anet/iocomponent.h>

namespace anet {
  class Socket;
  class TCPComponent : public IOComponent {
    friend class TCPCOMPONENTTF;
    friend class TCPCONNECTIONTF;
  public:
    TCPComponent(Transport *owner, Socket *socket);
    ~TCPComponent();

    bool init(bool isServer = false);

    Connection *createConnection(IPacketStreamer *streamer,
                                 IServerAdapter *adapter = NULL);

    /*
     * �ر�
     */
    void close();
    void closeConnection(Connection *conn);
//     void closeSocket();
//     void closeSocketNoLock();

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

    bool handleErrorEvent();
    /*
     * �õ�connection
     * 
     * @return TCPConnection
     */
    Connection *getConnection() {
      return _connection;
    }

    /*
     * ��ʱ���
     * 
     * @param    now ��ǰʱ��(��λus)
     */
    bool checkTimeout(int64_t now);
    bool setState(IOCState state);
  protected:
    /*
     * ���ӵ�socket
     */
    bool socketConnect();

    Connection *_connection;
    int64_t _startConnectTime;
  };
}

#endif /*TCPCOMPONENT_H_*/
