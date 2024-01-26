#ifndef ANET_TCPCONNECTION_H_
#define ANET_TCPCONNECTION_H_
#include <anet/databuffer.h>
#include <anet/connection.h>
namespace anet {

  class DataBuffer;
  class Socket;
  class IPacketStreamer;
  class IServerAdapter;

  class TCPConnection : public Connection {
    friend class TCPCONNECTIONTF;

  public:

    TCPConnection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    ~TCPConnection();

    /*
     * д������
     *
     * @return �Ƿ�ɹ�
     */
    bool writeData();

    /*
     * ��������
     *
     * @return ��������
     */
    bool readData();

    /*
     * ����д���Ƿ������ر�
     */
    void setWriteFinishClose(bool v) {
      _writeFinishClose = v;
    }

    /*
     * ���output��buffer
     */
    void clearOutputBuffer() {
      _output.clear();
    }

    /*
     * ���output��buffer
     */
    void clearInputBuffer() {
      _input.clear();
    }

  private:
    DataBuffer _output;     	// �����buffer
    DataBuffer _input;      	// �����buffer
    PacketHeader _packetHeader; // �����packet header
    bool _gotHeader;            // packet header�Ѿ�ȡ��
    bool _writeFinishClose;     // д��Ͽ�
};

}

#endif /*TCPCONNECTION_H_*/
