#ifndef ANET_SERVERSOCKET_H_
#define ANET_SERVERSOCKET_H_
#include <anet/socket.h>
namespace anet {

class ServerSocket : public Socket {

public:
    /*
     * ���캯��
     */
    ServerSocket();
    /*
     * acceptһ���µ�����
     * 
     * @return һ��Socket
     */
    Socket *accept();

    /*
     * �򿪼���
     * 
     * @return �Ƿ�ɹ�
     */
    bool listen();

private:
    int _backLog; // backlog
};

}

#endif /*SERVERSOCKET_H_*/
