#ifndef ANET_TCPACCEPTOR_H_
#define ANET_TCPACCEPTOR_H_
#include <anet/iocomponent.h>

namespace anet {
class IOComponent;
class TCPAcceptor : public IOComponent {

public:
    /**
     * @param owner: pointer of a transport object
     * @param socket pointer of a socket object
     * @param timeout: set timeout in millisecond for server sending replies
     */
    TCPAcceptor(Transport *owner, Socket *socket, IPacketStreamer *streamer,
		IServerAdapter *serverAdapter, int timeout = 5000 );

    bool init(bool isServer = false);

    void close();

    /***
    * �������ݿɶ�ʱ��Transport����
    * 
    * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
    */
    bool handleReadEvent();

    /***
     * ��accept��û��д�¼�
     */
    bool handleWriteEvent() {
        return true;
    }

    bool handleErrorEvent(); 

    /**
     * ��ʱ���
     * 
     * @param    now ��ǰʱ��(��λus)
     */
    bool checkTimeout(int64_t now);

private:
    IPacketStreamer *_streamer;      // ���ݰ�������
    IServerAdapter  *_serverAdapter; // ������������
    int _timeout;
};
}

#endif /*TCPACCEPTOR_H_*/
