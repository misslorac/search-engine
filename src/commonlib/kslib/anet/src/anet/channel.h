#ifndef ANET_CHANNEL_H_
#define ANET_CHANNEL_H_
#include <stdint.h>
namespace anet {
  class IPacketHandler;

  class Channel {
    friend class ChannelPool;
    friend class CHANNELTF; //add by hua.huangrh
    friend class CHANNELPOOLTF; //add by hua.huangrh

  public:
    /*
     * ���캯��
     */
    Channel();

    /*
     * ����
     *
     * @param   chid    ChannelID
     */
    void setId(uint32_t id);

    /*
     * �õ�Channel ID
     */
    uint32_t getId();

    /*
     * ���ûش�����
     */
    void setArgs(void *args);

    /*
     * ȡ���ش�����
     */
    void *getArgs();

    /*
     * ����packethandler�ľ��
     */
    void setHandler(IPacketHandler *handler);

    /*
     * �õ����
     */
    IPacketHandler *getHandler();

    /*
     * ���ù���ʱ��, ����ʱ��
     *
     * @param   expireTime
     */
    void setExpireTime(int64_t expireTime);

    /*
     * ��һ��
     */
    Channel *getNext() {
      return _next;
    }

  private:
    uint32_t _id;      // channel id
    void *_args;    // �ش�����
    IPacketHandler *_handler;
    int64_t _expireTime; // ����ʱ��

  private:
    Channel *_prev;     // ��������
    Channel *_next;
  };
}

#endif /*CONNECTION_H_*/
