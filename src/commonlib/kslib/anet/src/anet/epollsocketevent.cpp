#include <anet/epollsocketevent.h>
#include <anet/log.h>
#include <anet/socket.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <assert.h>
#include <anet/filecontrol.h>
namespace anet {

/*
 * ���캯��
 */
EPollSocketEvent::EPollSocketEvent() {
    _iepfd = epoll_create(MAX_SOCKET_EVENTS);
    assert(_iepfd >= 0);
    int rc = pipe(_pipes);
    assert(rc == 0);
    FileControl::setCloseOnExec(_iepfd);
    FileControl::setCloseOnExec(_pipes[0]);
    FileControl::setCloseOnExec(_pipes[1]);
    struct epoll_event ev;
    ev.events = EPOLLET;
    ev.data.ptr=this;
    rc =  epoll_ctl(_iepfd, EPOLL_CTL_ADD, _pipes[1], &ev);
    assert(rc == 0);
}

/*
 * ���캯��
 */
EPollSocketEvent::~EPollSocketEvent() {
    close(_pipes[0]);
    close(_pipes[1]);
    close(_iepfd);
}

/*
 * ����Socket���¼���
 *
 * @param socket ���ӵ�socket
 * @param enableRead: �����Ƿ�ɶ�
 * @param enableWrite: �����Ƿ��д
 * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
 */
bool EPollSocketEvent::addEvent(Socket *socket, bool enableRead, bool enableWrite) {

    struct epoll_event ev;
    ev.data.ptr = socket->getIOComponent();
    // ����Ҫ������¼�����
//    ev.events = EPOLLET;
    ev.events = 0; //use level triggered to reduce call to epoll_ctl()
    if (enableRead) {
        ev.events |= EPOLLIN;
    }
    if (enableWrite) {
        ev.events |= EPOLLOUT;
    }

    //_mutex.lock();
    bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_ADD, socket->getSocketHandle(), &ev) == 0);
    //_mutex.unlock();
    ANET_LOG(DEBUG, "read: %d, write: %d. (IOC:%p)", enableRead, 
             enableWrite, ev.data.ptr);
    return rc;
}

/*
 * ����ɾ��Socket���¼���
 *
 * @param socket ���ӵ�socket
 * @param enableRead: �����Ƿ�ɶ�
 * @param enableWrite: �����Ƿ��д
 * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
 */
bool EPollSocketEvent::setEvent(Socket *socket, bool enableRead, bool enableWrite) {

    struct epoll_event ev;
    ev.data.ptr = socket->getIOComponent();
    // ����Ҫ������¼�����
//    ev.events = EPOLLET;
    ev.events = 0; //use level triggered to reduce call to epoll_ctl()
    if (enableRead) {
        ev.events |= EPOLLIN;
    }
    if (enableWrite) {
        ev.events |= EPOLLOUT;
    }

    //_mutex.lock();
    bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_MOD, socket->getSocketHandle(), &ev) == 0);
    //_mutex.unlock();
    //ANET_LOG(ERROR, "EPOLL_CTL_MOD: %d => %d,%d, %d", socket->getSocketHandle(), enableRead, enableWrite, pthread_self());
    return rc;
}

/*
 * ɾ��Socket���¼���
 *
 * @param socket ��ɾ��socket
 * @return  �����Ƿ�ɹ�, true  �ɹ�, false  ʧ��
 */
bool EPollSocketEvent::removeEvent(Socket *socket) {

    struct epoll_event ev;
    ev.data.ptr = socket->getIOComponent();
    // ����Ҫ������¼�����
    ev.events = 0;
    //_mutex.lock();
    bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_DEL, socket->getSocketHandle(), &ev) == 0);
    //_mutex.unlock();
    //ANET_LOG(ERROR, "EPOLL_CTL_DEL: %d", socket->getSocketHandle());
    return rc;
}

/*
 * �õ���д�¼���
 *
 * @param timeout  ��ʱʱ��(��λ:ms)
 * @param events  �¼�����
 * @param cnt   events�������С
 * @return �¼���, 0Ϊ��ʱ, -1Ϊ������
 */
int EPollSocketEvent::getEvents(int timeout, IOEvent *ioevents, int cnt) {

    struct epoll_event events[MAX_SOCKET_EVENTS];

    if (cnt > MAX_SOCKET_EVENTS) {
        cnt = MAX_SOCKET_EVENTS;
    }

    int res = epoll_wait(_iepfd, events, cnt , timeout);

    // ��events���¼�ת����IOEvent���¼�
    int j = 0;
    for (int i = 0; i < res; i++) {
        if (events[i].data.ptr == this) {
            continue;
        }

        ioevents[j]._ioc = (IOComponent*)events[i].data.ptr;
        ioevents[j]._errorOccurred = events[i].events & (EPOLLERR | EPOLLHUP)
                                     ? true
                                     : false; 
        ioevents[j]._readOccurred = events[i].events & EPOLLIN
                                    ? true
                                    : false;
        ioevents[j]._writeOccurred = events[i].events & EPOLLOUT
                                    ? true
                                    : false;
        j++;
    }

    return j;
}

void EPollSocketEvent::wakeUp() {
    struct epoll_event ev;
    ev.events = EPOLLET | EPOLLOUT;
    ev.data.ptr=this;
    int rc =  epoll_ctl(_iepfd, EPOLL_CTL_MOD, _pipes[1], &ev);
    assert(rc == 0);
}
}
