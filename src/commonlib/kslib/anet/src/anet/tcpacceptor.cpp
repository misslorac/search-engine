#include <anet/transport.h>
#include <anet/serversocket.h>
#include <anet/socket.h>
#include <anet/tcpacceptor.h>
#include <anet/tcpcomponent.h>
#include <anet/log.h>

namespace anet {
/**
 * ���캯������Transport���á�
 *
 * @param  owner:    ��������
 * @param  host:   ����ip��ַ��hostname
 * @param port:   �����˿�
 * @param streamer:   ���ݰ���˫��������packet����������������
 * @param serverAdapter:  ���ڷ������ˣ���Connection��ʼ����Channel����ʱ�ص�ʱ��
 */
TCPAcceptor::TCPAcceptor(Transport *owner, Socket *socket,
                         IPacketStreamer *streamer, 
                         IServerAdapter *serverAdapter,
                         int timeout) : IOComponent(owner, socket) {
    _streamer = streamer;
    _serverAdapter = serverAdapter;
    _timeout = timeout;
}

/*
 * ��ʼ��, ��ʼ����
 */
bool TCPAcceptor::init(bool isServer) {
    setSocketEvent(_owner->getSocketEvent());
    assert(_socketEvent);
    _socket->setSoBlocking(false);
    bool rc = ((ServerSocket*)_socket)->listen();
    if (rc) {
        setState(ANET_CONNECTED);//may be need new state
        _socketEvent->addEvent(_socket, true,false);
        _owner->postCommand(Transport::TC_ADD_IOC, this);
    }
    return rc;
}

/**
* �������ݿɶ�ʱ��Transport����
*
* @return �Ƿ�ɹ�
*/
bool TCPAcceptor::handleReadEvent() {
    lock();
    Socket *socket;
    while ((socket = ((ServerSocket*)_socket)->accept()) != NULL) {
        ANET_LOG(DEBUG, "New connection coming. fd=%d", socket->getSocketHandle());
        TCPComponent *component = new TCPComponent(_owner, socket);
        assert(component);

         if (!component->init(true)) {
             delete component;/**@TODO: may coredump?*/
             return true;
        }
        Connection *conn = component->createConnection(_streamer, _serverAdapter);
        conn->setQueueTimeout(_timeout);
         _owner->addToCheckingList(component);

        //transport's Read Write Thread and Timeout thread will have their
        //own reference of component, so we need to subRef() the initial one
        component->subRef();
    }
    unlock();
    return true;
}

bool TCPAcceptor::handleErrorEvent() {
    close();
    return false;
}

void TCPAcceptor::close() {
    lock();
    if (getState() != ANET_CLOSED) {
        _socket->close();
        setState(ANET_CLOSED);
    }
    _owner->postCommand(Transport::TC_REMOVE_IOC, this);
    unlock();
}

/*
 * ��ʱ���
 *
 * @param    now ��ǰʱ��(��λus)
 */
bool TCPAcceptor::checkTimeout(int64_t now) {return true;}
}
