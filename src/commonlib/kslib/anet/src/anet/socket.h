#ifndef ANET_SOCKET_H_
#define ANET_SOCKET_H_
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

namespace anet {
  class IOComponent;
  class ThreadMutex;
class Socket {

public:
    /*
     * ���캯��
     */
    Socket();

    /*
     * ��������
     */
    ~Socket();

    /*
     * ���õ�ַ
     *
     * @param address  host��ip��ַ
     * @param port  �˿ں�
     * @return �Ƿ�ɹ�
     */

    bool setAddress (const char *address, const int port);

    /*
     * ���ӵ�_address��
     * 
     * @return �Ƿ�ɹ�
     */
    bool connect();

    /*
     * �������ӵ�_address��
     *
     * @return �Ƿ�ɹ�
     */
    bool reconnect();    

    /**
     * �ر�����
     */
    void close();

    /*
     * �رն�д
     */
    void shutdown();

    /**
     * ʹ��UDP��socket
     * 
     * @return �Ƿ�ɹ�
     */
    bool createUDP();

    /*
     * ��socketHandle,��ipaddress���õ���socket��
     * 
     * @param  socketHandle: socket���ļ����
     * @param hostAddress: ��������ַ
     */

    void setUp(int socketHandle, struct sockaddr *hostAddress);

    /*
     * �����ļ����
     * 
     * @return �ļ����
     */
    int getSocketHandle();

    /*
     * ����IOComponent
     * 
     * @return  IOComponent
     */
    IOComponent *getIOComponent();

    /*
     * ����IOComponent
     * 
     * @param IOComponent
     */
    void setIOComponent(IOComponent *ioc);

    /*
     * д����
     */
    int write(const void *data, int len);

    /*
     * ������
     */
    int read(void *data, int len);

    /*
     * SetSoKeepAlive
     */
    bool setKeepAlive(bool on) {
        return setIntOption(SO_KEEPALIVE, on ? 1 : 0);
    }

    /*
     * setReuseAddress
     */
    bool setReuseAddress(bool on) {
        return setIntOption(SO_REUSEADDR, on ? 1 : 0);
    }

    /*
     * setSoLinger
     */
    bool setSoLinger (bool doLinger, int seconds);

    /*
     * setTcpNoDelay
     */
    bool setTcpNoDelay(bool noDelay);

    /*
     * setIntOption
     */
    bool setIntOption(int option, int value);

    /*
     * �Ƿ�����
     */
    bool setSoBlocking(bool on);

    /*
     * ���Socket����Ƿ񴴽�
     */
    bool checkSocketHandle();

    /*
     * �õ�Socket����
     */
    int getSoError();

    int getPort(bool active = true);
    uint32_t getIpAddress(bool active = true);
    /*
     * �õ�ip��ַ, д��tmp��
     */
    char *getAddr(char *dest, int len, bool active = false);

    /*
     * �õ����Ĵ���
     */
    static int getLastError() {
        return errno;
    }

    inline bool isAddressValid() {
      return _addressValid;
    }

    bool getSockAddr(sockaddr_in &addr, bool active = true);
protected:
    struct sockaddr_in  _address; // ��ַ
    int _socketHandle;    // socket�ļ����
    IOComponent *_iocomponent;
    static ThreadMutex _dnsMutex; //����ʵ����һ��dnsMutex
    bool _addressValid;
    
};
}

#endif /*SOCKET_H_*/
