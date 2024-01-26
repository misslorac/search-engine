#ifndef ANET_EPOLLSOCKETEVENT_H_
#define ANET_EPOLLSOCKETEVENT_H_
#include <anet/socketevent.h>
namespace anet {

class EPollSocketEvent : public SocketEvent {

public:
    /*
     * ���캯��
     */
    EPollSocketEvent();

    /*
     * ���캯��
     */
    ~EPollSocketEvent();

    /*
     * ����Socket���¼���
     *
     * @param socket ���ӵ�socket
     * @param enableRead: �����Ƿ�ɶ�
     * @param enableWrite: �����Ƿ��д
     * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
     */
    bool addEvent(Socket *socket, bool enableRead, bool enableWrite);

    /*
     * ����ɾ��Socket���¼���
     *
     * @param socket ���ӵ�socket
     * @param enableRead: �����Ƿ�ɶ�
     * @param enableWrite: �����Ƿ��д
     * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
     */
    bool setEvent(Socket *socket, bool enableRead, bool enableWrite);

    /*
     * ɾ��Socket���¼���
     *
     * @param socket ��ɾ��socket
     * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
     */
    bool removeEvent(Socket *socket);

    /*
     * �õ���д�¼���
     *
     * @param timeout  ��ʱʱ��(��λ:ms)
     * @param events  �¼�����
     * @param cnt   events�������С
     * @return �¼���, 0Ϊ��ʱ
     */
    int getEvents(int timeout, IOEvent *events, int cnt);

    void wakeUp();

private:
    int _iepfd;    // epoll��fd
    int _pipes[2];  // pipe for wakeup
//    ThreadMutex _mutex;  // ��fd��������
};
}

#endif /*EPOLLSOCKETEVENT_H_*/
