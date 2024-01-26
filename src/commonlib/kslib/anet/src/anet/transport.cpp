#include <list>
#include <anet/timeutil.h>
#include <anet/transport.h>
#include <anet/socketevent.h>
#include <anet/serversocket.h>
#include <anet/socket.h>
#include <anet/tcpacceptor.h>
#include <anet/tcpcomponent.h>
#include <anet/log.h>
#include <signal.h>
using namespace std;
namespace anet {

/*
 * ���캯��
 */
Transport::Transport() {
    _stop = false;
    _started = false;
    _iocListHead = _iocListTail = NULL;
    _promotePriority = false;
    _nextCheckTime = 0;
}

/*
 * ���캯��
 */
Transport::~Transport() {
    closeComponents();
}

/*
 * start two threads, one for read/write, one for timeout checking
 *
 * @return true - �ɹ�, false - ʧ�ܡ�
 */
bool Transport::start(bool promotePriority) {
    MutexGuard guard(&_stopMutex);
    if (_started) {
        return false;
    }
    _started = true;
    _promotePriority = promotePriority;
    signal(SIGPIPE, SIG_IGN);
    _readWriteThread.start(this, &_socketEvent);
    _timeoutThread.start(this, NULL);
    return true;
}

/*
 * ֹͣ��ͣ����д�̣߳������١�
 *
 * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
 */
bool Transport::stop() {
    MutexGuard guard(&_stopMutex);
    _stop = true;
    return true;
}

/*
 * �ȴ��߳���ȫ�˳���
 *
 * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
 */
bool Transport::wait() {
    _readWriteThread.join();
    _timeoutThread.join();
    closeComponents();
    return true;
}

/*
 * socket event �ļ��, ��run��������
 */
void Transport::eventLoop(SocketEvent *socketEvent) {
    while (!_stop) {
        int64_t now;// = TimeUtil::getTime();
        eventIteration(now);
    }
}

void Transport::eventIteration( int64_t &now) {
    IOEvent events[MAX_SOCKET_EVENTS];
    int cnt = _socketEvent.getEvents(100, events, MAX_SOCKET_EVENTS);
    now = TimeUtil::getTime();
    for (int i = 0; i < cnt; i++) {
        IOComponent *ioc = events[i]._ioc;
        assert(ioc);
        bool rc = true;
        if (events[i]._errorOccurred) { // ��������
            rc = ioc->handleErrorEvent();
        }

        // ��д
        if (rc && events[i]._readOccurred) {
            rc = ioc->handleReadEvent();
        }
        if (rc && events[i]._writeOccurred) {
            rc = ioc->handleWriteEvent();
        }
        ioc->updateUseTime(now);
    }
    processCommands();
}

/**
 * calling timeoutIteration() every 100ms
 */
void Transport::timeoutLoop() {
    while (!_stop) {
        usleep(100000);
        int64_t now = TimeUtil::getTime();
        timeoutIteration(now);
    }
}

void Transport::closeComponents() {
    collectNewComponets();
    list<IOComponent*>::iterator iter = _checkingList.begin();
    while ( iter != _checkingList.end() ) {
        IOComponent *ioc = *iter;
        ANET_LOG(DEBUG,"IOC(%p)->subRef(), [%d]", ioc, ioc->getRef());
        ioc->close();
        ioc->subRef();
        iter = _checkingList.erase(iter);
    }

    IOComponent *list, *ioc;
    list = _iocListHead;
    while (list) {
        ioc = list; 
        ANET_LOG(DEBUG,"IOC(%p)->subRef(), [%d]", ioc, ioc->getRef());
        list = list->_next;
        ioc->close();
        ioc->subRef();
    }
    _iocListHead = _iocListTail = NULL;

    //SHOULD clear commands at last!
    vector<TransportCommand>::iterator i = _commands.begin();
    while (i != _commands.end()) {
        ANET_LOG(DEBUG,"IOC(%p)->subRef(), [%d]", i->ioc, i->ioc->getRef());
        i->ioc->subRef();
        i++;
    }
    _commands.clear();
}

void Transport::collectNewComponets() {
    lock();
    if (_newCheckingList.size()) {
        for (vector<IOComponent*>::iterator i = _newCheckingList.begin();
             i != _newCheckingList.end(); i++) {
            _checkingList.push_back(*i);
        }
        _newCheckingList.clear();
    }
    unlock();
}

void Transport::timeoutIteration(int64_t now) {
    collectNewComponets();
    list<IOComponent*>::iterator iter = _checkingList.begin();
    while ( iter != _checkingList.end() ) {
        IOComponent *ioc = *iter;
        if (ioc->checkTimeout(now)) {
            iter ++;
        } else {
            ioc->subRef();
            iter = _checkingList.erase(iter);
        }
    }
    _nextCheckTime = now + 100000;
}

/*
 * �̵߳����к�����ʵ��Runnable�ӿ��еĺ���
 *
 * @param arg: ����ʱ�������
 */
void Transport::run(Thread *thread, void *arg) {
    if (thread == &_timeoutThread) {
        timeoutLoop();
    } else {
        if (_promotePriority) {
            thread->setPriority(1, SCHED_RR);
        }
        eventLoop((SocketEvent*)arg);
    }
}

void Transport::runIteration(int64_t &now) {
    eventIteration(now);
    if (now  >= _nextCheckTime) {
        timeoutIteration(now);
    }
}

void Transport::run() {
    int64_t now;
    while (!_stop) {
        runIteration(now);
    }
}

void Transport::stopRun() {
    _stop = true;
}

/**
 * ��[upd|tcp]:ip:port�ֿ�����args��
 *
 * @param src: Դ��ʽ
 * @param args: Ŀ������
 * @param   cnt: ������������
 * @return  ���ص������и���
 */
int Transport::parseAddr(char *src, char **args, int cnt) {
    if (src == NULL) return 0; //add by hua.huangrh

    int index = 0;
    char *prev = src;

    while (*src) {
        if (*src == ':') {
            *src = '\0';
            args[index++] = prev;

            if (index >= cnt) { // ��������,����
                return index;
            }

            prev = src + 1;
        }

        src ++;
    }

    args[index++] = prev;

    return index;
}

/*
 * ��һ�������˿ڡ�
 *
 * @param spec: ��ʽ [upd|tcp]:ip:port
 * @param streamer: ���ݰ���˫��������packet����������������
 * @param serverAdapter: ���ڷ������ˣ���Connection��ʼ����Channel����ʱ�ص�ʱ��
 * @return IO���һ�������ָ��
 */
IOComponent *Transport::listen (const char *spec, IPacketStreamer *streamer, 
                                IServerAdapter *serverAdapter, int timeout) {
    MutexGuard guard(&_stopMutex);
    if (_stop) { 
        ANET_LOG(SPAM, "Transport(%p) Stoped!", this);
        return NULL; 
    }
    if (NULL == spec || NULL == streamer || NULL == serverAdapter) {
        ANET_LOG(WARN, "Invalid parameters for listen(%p,%p,%p)",
                 spec, streamer, serverAdapter);
        return NULL;
    }

    char tmp[512];
    char *args[32];
    strncpy(tmp, spec, 512);
    tmp[511] = '\0';

    if (parseAddr(tmp, args, 32) != 3) {
        return NULL;
    }

    if (strcasecmp(args[0], "tcp") == 0) {
        char *host = args[1];
        int port = atoi(args[2]);

        // Server Socket
        ServerSocket *socket = new ServerSocket();
        assert(socket);
        if (!socket->setAddress(host, port)) {
            delete socket;
            return NULL;
        }

        // TCPAcceptor
        TCPAcceptor *acceptor 
            = new TCPAcceptor(this, socket, streamer, serverAdapter, timeout);
        assert(acceptor);
        if (!acceptor->init()) {
            delete acceptor;
            return NULL;
        }
        // ����
        return acceptor;
    } else if (strcasecmp(args[0], "udp") == 0) {}

    return NULL;
}

/*
 * ����һ��Connection�����ӵ�ָ���ĵ�ַ�������뵽Socket�ļ����¼��С�
 *
 * @param spec: ��ʽ [upd|tcp]:ip:port
 * @param streamer: ���ݰ���˫��������packet����������������
 * @return  ����һ��Connectoion����ָ��
 */
Connection *Transport::connect(const char *spec, 
                               IPacketStreamer *streamer, 
                               bool autoReconn) {
    MutexGuard guard(&_stopMutex);
    if (_stop) { return NULL; }

    if (NULL == spec || NULL == streamer) {
        ANET_LOG(WARN, "Invalid parameters for connect(%,%p,%d)",
                 spec, streamer, autoReconn);
        return NULL;
    }
    char tmp[512];
    char *args[32];
    strncpy(tmp, spec, 512);
    tmp[511] = '\0';

    if (parseAddr(tmp, args, 32) != 3) {
        return NULL;
    }

    if (strcasecmp(args[0], "tcp") == 0) {
        char *host = args[1];
        int port = atoi(args[2]);

        // Socket
        Socket *socket = new Socket();
        assert(socket);
        if (!socket->setAddress(host, port)) {
            delete socket;
            ANET_LOG(ERROR, "setAddress error: %s:%d, %s", host, port, spec);
            return NULL;
        }

        // TCPComponent
        TCPComponent *component = new TCPComponent(this, socket);
        assert(component);
        if (!component->init()) {
            delete component;
            ANET_LOG(ERROR, "Failed to init TCPComponent(%s:%d).", host, port);
            return NULL;
        }
        component->createConnection(streamer, NULL);
        component->setAutoReconn(autoReconn);
        ANET_LOG(INFO, "socket(fd:%d) connected.", component->getSocket()->getSocketHandle());
        addToCheckingList(component);
        return component->getConnection();
    } else if (strcasecmp(args[0], "udp") == 0) {}

    return NULL;
}

/*
 * ���뵽iocomponents��
 *
 * @param  ioc: IO���
 */
void Transport::addComponent(IOComponent *ioc) {
    assert(ioc);
    if (ioc->isReferencedByReadWriteThread()) {
        ANET_LOG(DEBUG, "Already referenced: %p", ioc);
        return;
    }
    // ����iocList��
    ioc->_prev = _iocListTail;
    ioc->_next = NULL;
    if (_iocListTail == NULL) {
        _iocListHead = ioc;
    } else {
        _iocListTail->_next = ioc;
    }
    _iocListTail = ioc;
    ioc->addRef();
    ioc->referencedByReadWriteThread(true);
}

/*
 * ɾ��iocomponet
 *
 * @param ioc: IO���
 */
void Transport::removeComponent(IOComponent *ioc) {
    assert(ioc);
    if (!ioc->isReferencedByReadWriteThread()) { 
        ANET_LOG(DEBUG, "Not referenced: %p", ioc);
        return;
    }
    // remove form iocList
    if (ioc == _iocListHead) { // head
        _iocListHead = ioc->_next;
    }
    if (ioc == _iocListTail) { // tail
        _iocListTail = ioc->_prev;
    }
    if (ioc->_prev != NULL)
        ioc->_prev->_next = ioc->_next;
    if (ioc->_next != NULL)
        ioc->_next->_prev = ioc->_prev;
    ioc->referencedByReadWriteThread(false);
    ioc->subRef();
}

void Transport::postCommand(const CommandType type, IOComponent *ioc) {
    assert(ioc);
    TransportCommand tc;
    tc.type = type;
    tc.ioc = ioc;
    ioc->addRef();
    lock();
    _commands.push_back(tc);
    unlock();
    _socketEvent.wakeUp();
}

void Transport::processCommands() {
    lock();
    for (vector<TransportCommand>::iterator i = _commands.begin();
         i != _commands.end(); i++) {
        switch (i->type) {
        case TC_ADD_IOC:
            addComponent(i->ioc);
            break;
        case TC_REMOVE_IOC:
            removeComponent(i->ioc);
            break;
        default:
            break;
        }
        i->ioc->subRef();
    }
    _commands.clear();
    unlock();
}

SocketEvent* Transport::getSocketEvent() {
    return &_socketEvent;
}

void Transport::addToCheckingList(IOComponent *ioc) {
    ioc->addRef();
    lock();
    _newCheckingList.push_back(ioc);
    unlock();
}

void Transport::lock() {
    _mutex.lock();
}

void Transport::unlock() {
    _mutex.unlock();
}

}
