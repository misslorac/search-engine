#ifndef ANET_IOCOMPONENT_H_
#define ANET_IOCOMPONENT_H_

#include <anet/atomic.h>
#include <anet/threadmutex.h>
#include <anet/socket.h>

namespace anet {
const int MAX_RECONNECTING_TIMES = 30;
const int64_t MAX_IDLE_TIME = 900000000;//15 minutes
const int64_t CONNECTING_TIMEOUT = 2000000;//2 seconds
const int64_t RECONNECTING_INTERVAL = 1000000;//1 seconds

class Connection;
class SocketEvent;
class IPacketStreamer;
class IServerAdapter;
class Socket;
class Transport;
class IOComponent {
    friend class Transport;
    friend class TCPCOMPONENTTF;
    friend class ConnecionTF;
    friend class TCPCONNECTIONTF;
public:
    enum IOCState {
        ANET_TO_BE_CONNECTING = 1,
        ANET_CONNECTING,
        ANET_CONNECTED,
        ANET_CLOSING,
        ANET_CLOSED,
    };

public:
    /**
     * @param owner: pointer of a transport object
     * @param socket pointer of a socket object
     */
    IOComponent(Transport *owner, Socket *socket);

    virtual ~IOComponent();

    /**
     * @param isServer indicate if this component is used as server
     * @return 
     */
    virtual bool init(bool isServer = false) = 0;

    /**
     * create a connection based on this iocomponent;
     * @param streamer:   streamer used to decode/encode data from/to
     * input/output buffer
     * @param serverAdapter:  user defined adapter to serve requests
     */ 
    virtual Connection *createConnection(IPacketStreamer *streamer,
            IServerAdapter *adapter = NULL) {
        return NULL;
    }
    /**
     * �ر�
     */
    virtual void close() {}

    virtual void closeSocket();
    virtual void closeSocketNoLock();

    virtual void closeConnection(Connection *);

    /**
     * �������ݿ�д��ʱ��Transport����
     * 
     * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
     */
    virtual bool handleWriteEvent() = 0;

    /**
     * �������ݿɶ�ʱ��Transport����
     * 
     * @return �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
     */
    virtual bool handleReadEvent() = 0;

    /***
     * called when error occured
     */
    virtual bool handleErrorEvent() = 0;

    /**
     * ��ʱ���
     * 
     * @param    now ��ǰʱ��(��λus)
     * @return when return false, we should remove this iocomponent
     * from timeout thread's checking list
     */
    virtual bool checkTimeout(int64_t now) = 0;

    /**
     * �õ�socket���
     * 
     * @return Socket
     */
    inline Socket *getSocket() {
        return _socket;
    }

    /**
     * ����SocketEvent
     */
    void setSocketEvent(SocketEvent *socketEvent) ;

    /**
     * �����ܶ�
     *
     * @param on ���Ƿ��
     */
    void enableRead(bool on) ;

    /**
     * ������д
     *
     * @param on д�Ƿ��
     */
    void enableWrite(bool on) ;

    inline void updateUseTime(int64_t now) {
        _lastUseTime = now;
    }

    /**
     * �������ü���
     */
    void addRef() ;

    /**
     * �������ü��� 
     */
    void subRef();

    /**
     * ȡ�����ü��� 
     */
    int getRef();
    /**
     * �Ƿ�����״̬, ������������״̬
     */
    inline bool isConnectState() {
        return getState() < ANET_CLOSING;
    }

    inline bool isClosed() {
        return getState() >= ANET_CLOSING;
    }

    /**
     * �����Ƿ�����
     */
    inline void setAutoReconn(bool on) {
        /***only attempts to reconnect 30 times at most*/
        _autoReconn = (_isServer || !on) ? 0 : MAX_RECONNECTING_TIMES;
    }
 
    /**
     * �õ�������־ 
     */
    inline bool isAutoReconn() {
        return (_autoReconn && !_isServer);
    }

    /***
     * Is this IOComponent referenced by read/write thread of transport
     */
    bool isReferencedByReadWriteThread() {
        return _refByRreadWriteThread;
    }

    /***
     * Set _refByRreadWriteThread flag
     */
    inline void referencedByReadWriteThread(bool flag) {
        _refByRreadWriteThread = flag;
    }

    void lock() ;
    void unlock();
    
    inline IOCState getState() {
        return _state;
    }

    virtual bool setState(IOCState state) ;
protected:
    Transport *_owner;
    Socket *_socket;    // һ��Socket���ļ����
    SocketEvent *_socketEvent;
    IOCState _state;         // ����״̬
    atomic_t _refcount; // ���ü���
    int _autoReconn;   // �Ƿ�����
    bool _isServer;     // �Ƿ�Ϊ��������
    int64_t _lastUseTime;   // ���ʹ�õ�ϵͳʱ��    
    bool _enableRead;
    bool _enableWrite;
    bool _refByRreadWriteThread;
    ThreadMutex _mutex;
private:
    IOComponent *_prev; // ��������
    IOComponent *_next; // ��������
};
}

#endif /*IOCOMPONENT_H_*/
