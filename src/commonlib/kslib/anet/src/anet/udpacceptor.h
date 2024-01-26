#ifndef ANET_TCPACCEPTOR_H_
#define ANET_TCPACCEPTOR_H_

namespace anet {

class UDPAcceptor : public UDPComponent {

public:
    /**
    * ���캯������Transport���á�
    * ����:
    *  transport:  ��������:::spec: ��ʽ [upd|tcp]:ip:port
    *  streamer:  ���ݰ���˫��������packet����������������
    * serverAdapter: ���ڷ������ˣ���Connection��ʼ����Channel����ʱ�ص�ʱ��
    */
    UDPAcceptor(Transport *owner, char *spec, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    /**
    * �������ݿɶ�ʱ��Transport����
    * ����
    * �Ƿ�ɹ�, true - �ɹ�, false - ʧ�ܡ�
    */
    bool handleReadEvent();

    /**
    * ����
    */
    bool handleWriteEvent() {
        return false;
    }
};
}

#endif /*TCPACCEPTOR_H_*/
