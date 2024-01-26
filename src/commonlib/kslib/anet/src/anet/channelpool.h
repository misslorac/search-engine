#ifndef ANET_CHANNEL_POOL_H_
#define ANET_CHANNEL_POOL_H_
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <anet/atomic.h>
#include <anet/threadmutex.h>
#include <ext/hash_map>
#include <list>
namespace anet {
const size_t CHANNEL_CLUSTER_SIZE = 64lu;
 class Channel;

class ChannelPool {
public:	
    ChannelPool();
    ~ChannelPool();

    static const uint32_t AUTO_CHANNEL_ID_MAX;
    static const uint32_t HTTP_CHANNLE_ID;
    static const uint32_t ADMIN_CHANNLE_ID;
    /**
     * Allocate a new channel.
     * 
     * @param chid The desired channel id for the channel just allocated. If
     * chid is less than or equal to AUTO_CHANNEL_ID_MAX, the channel id of
     * the new channel will be generated automatically, which is larger than
     * 0 and less than AUTO_CHANNEL_ID_MAX. if chid is greater than
     * AUTO_CHANNEL_ID_MAX,  the channel id of the new channel will be chid.
     */
    Channel *allocChannel(uint32_t chid = 0);


    bool freeChannel(Channel *channel);

    /*
     * ����һ��channel
     *
     * @param id: channel id
     * @return Channel
     */
    Channel* findChannel(uint32_t id);

    /*
     * ��useList���ҳ���ʱ��channel��list,����hashmap�ж�Ӧ��ɾ��
     *
     * @param now: ��ǰʱ��
     */
    Channel* getTimeoutList(int64_t now);

    /*_mutex
     * ��addList��������뵽freeList��
     *
     * @param addList���ӵ�list
     */
    bool appendFreeList(Channel *addList);
    
    /*
     * ��������ĳ���
     */
    size_t getUseListCount() {
        return _useListCount;
    }

private:
    __gnu_cxx::hash_map<uint32_t, Channel*> _useMap; // ʹ�õ�map
    std::list<Channel*> _clusterList;                // cluster list
    ThreadMutex _mutex;

    Channel *_freeListHead;             // �յ�����
    Channel *_freeListTail;
    Channel *_useListHead;              // ��ʹ�õ�����
    Channel *_useListTail;
    size_t _useListCount;                  // ��������ĳ���

    static atomic_t _globalChannelId;   // ����ͳһ��id

    friend class CHANNELPOOLTF;    //add by hua.huangrh
};

}

#endif /*CHANNEL_POOL_H_*/
