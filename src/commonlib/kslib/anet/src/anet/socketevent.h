#ifndef ANET_SOCKETEVENT_H_
#define ANET_SOCKETEVENT_H_


#include <stdint.h>

namespace anet {

#define MAX_SOCKET_EVENTS 256

  class IOComponent;
  class Socket;
class IOEvent {

public:
    bool _readOccurred;  // ������
    bool _writeOccurred;  // д����
    bool _errorOccurred;  // ������
    IOComponent *_ioc;  // �ش�����
};

class SocketEvent {

public:
    /*
     * ���캯��
     */
    SocketEvent();
    /*
     * ��������
     */
    virtual ~SocketEvent();
    /*
     * ����Socket���¼���
     * 
     * @param socket ���ӵ�socket
     * @param enableRead: �����Ƿ�ɶ�
     * @param enableWrite: �����Ƿ��д
     * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
     */
    virtual bool addEvent(Socket *socket, bool enableRead, bool enableWrite) = 0;

    /*
     * ����ɾ��Socket���¼���
     * 
     * @param socket ���ӵ�socket
     * @param enableRead: �����Ƿ�ɶ�
     * @param enableWrite: �����Ƿ��д
     * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
     */
    virtual bool setEvent(Socket *socket, bool enableRead, bool enableWrite) = 0;

    /*
        * ɾ��Socket���¼���
        * 
        * @param socket ��ɾ��socket
        * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
        */
    virtual bool removeEvent(Socket *socket) = 0;

    /*
     * �õ���д�¼���
     * 
    * @param timeout  ��ʱʱ��(��λ:ms)
    * @param events  �¼�����
    * @param cnt   events�������С
    * @return �¼���, 0Ϊ��ʱ
     */
    virtual int getEvents(int timeout, IOEvent *events, int cnt) = 0;

    /**
     * Wake up thread which is blocking in getEvents()
     * This interface may changed in future.
     */
    virtual void wakeUp() = 0;
};
}

#endif /*SOCKETEVENT_H_*/
