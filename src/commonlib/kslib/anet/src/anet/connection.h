#ifndef ANET_CONNECTION_H_
#define ANET_CONNECTION_H_

#define READ_WRITE_SIZE 8192
#include <anet/packetqueue.h>
#include <anet/threadcond.h>
#include <anet/channelpool.h>
#include <anet/ipacketstreamer.h>
namespace anet {
class IPacketHandler;
class Packet;
class IOComponent;
class IServerStreamer;
class IServerAdapter;
class Socket;
class Connection {
    friend class ConnectionTF;
    friend class TCPCONNECTIONTF;
public:

    Connection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    virtual ~Connection();

    /**
     * �����Ƿ�Ϊ��������
     */
    void setServer(bool isServer) {
        _isServer = isServer;
    }

    bool isServer() {
        return _isServer;
    }

    void setIOComponent(IOComponent *ioc) {
        _iocomponent = ioc;
    }

    IOComponent *getIOComponent() {
        return _iocomponent;
    }

    /**
     * ����Ĭ�ϵ�packetHandler
     */
    void setDefaultPacketHandler(IPacketHandler *ph) {
        _defaultPacketHandler = ph;
    }

    /**
     * ����packet�����Ͷ���
     *
     * @param packet ���ݰ�
     * @param packetHandler 
     * @param args �Զ������
     * @param blocking whether blocking if output queue is full.
     */
    bool postPacket(Packet *packet, IPacketHandler *packetHandler = NULL,
		    void *args = NULL, bool blocking = false);

    /**
     * Send a request packet to server, then waiting for the replay packet.
     * If the return value is NULL, caller is responsible for freeing the
     * request packet; If the return value is NOT NULL, ANet will free the
     * request packet, and caller is responsible for freeing the reply packet.
     *
     * @param packet the request to be sent to server
     * @return the value returned is a pointer to a reply packet received
     * from server, or NULL if fail to send the reply.
     */
    Packet *sendPacket(Packet *packet);

    /**
     * �������յ�ʱ�Ĵ�����
     */
    bool handlePacket(Packet *header);

    uint32_t getPacketPostedCount() {return _packetPostedCount;}
    uint32_t getPacketHandledCount() {return _packetHandledCount;}
    /**
     * ��鳬ʱ
     */
    bool checkTimeout(int64_t now);

    void closeHook();

    virtual bool writeData() = 0;

    virtual bool readData() = 0;

    /**
     * ����д���Ƿ�ر�, ֻTCPҪ��
     */
    virtual void setWriteFinishClose(bool v) {
        ;
    }

    /**
     * ���ö��еĳ�ʱʱ��
     */
    void setQueueTimeout(int queueTimeout) {
        _queueTimeout = queueTimeout;
    }

    /**
     * ���output��buffer
     */
    virtual void clearOutputBuffer() {
        ;
    }

    /**
     * ���input��buffer
     */
    virtual void clearInputBuffer() {
        ;
    }
    
    
    /**
     * ����queue��󳤶�, 0 - ������
     */
    bool setQueueLimit(size_t limit); 
    size_t getQueueLimit();

    virtual void close();
    virtual void addRef();
    virtual void subRef();
    virtual int getRef();

    virtual bool isClosed();

    void wakeUpSender();
    void beforeCallback();
    void afterCallback();

    void resetContext(); 
    StreamingContext* getContext() {
        return _context;
    }
protected:
    IPacketHandler *_defaultPacketHandler;  // connection��Ĭ�ϵ�packet handler
    bool _isServer;                         
    IOComponent *_iocomponent;
    Socket *_socket;                        
    IPacketStreamer *_streamer;             // Packet Streamer
    IServerAdapter *_serverAdapter;         // Server Adapter

    PacketQueue  _outputQueue;              // Output Queue
    PacketQueue _myQueue;	            // temporary queue used in write data
    ThreadCond _outputCond;                 // output condition
    ChannelPool _channelPool;
    int _queueTimeout;                      // tiemout spesification (millseconds)
    size_t _queueLimit;                     // max pending set packet allowed
    int _waitingThreads;
    bool _closed;
    StreamingContext *_context;
    uint32_t _packetPostedCount;
    uint32_t _packetHandledCount;
};
}

#endif /*CONNECTION_H_*/

