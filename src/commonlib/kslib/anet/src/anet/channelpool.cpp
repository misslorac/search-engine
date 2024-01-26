#include <ext/hash_map>
#include <list>
#include <anet/threadmutex.h>
#include <anet/channelpool.h>
#include <anet/channel.h>
namespace anet {
const uint32_t ChannelPool::AUTO_CHANNEL_ID_MAX = 0x0FFFFFFFu;
const uint32_t ChannelPool::HTTP_CHANNLE_ID     = 0x10000000u;
const uint32_t ChannelPool::ADMIN_CHANNLE_ID    = 0x10000001u;

atomic_t ChannelPool::_globalChannelId = {0};

/*
 * ���캯��
 */
ChannelPool::ChannelPool() {
    _freeListHead = NULL;
    _freeListTail = NULL;
    _useListHead = NULL;
    _useListTail = NULL;
    _useListCount = 0;
}

/*
 * ��������
 */
ChannelPool::~ChannelPool() {
     std::list<Channel*>::iterator it = _clusterList.begin();

     for (;it != _clusterList.end(); it++) {
         delete[] *it;
     }
}

/*
 * �õ�һ���µ�channel
 *
 * @return һ��Channel
 */
Channel *ChannelPool::allocChannel(uint32_t chid) {
    Channel *channel = NULL;
    _mutex.lock();
    if (_freeListHead == NULL) { // ����ǿգ��·���һЩ�ŵ�freeList��
        assert(CHANNEL_CLUSTER_SIZE>2);
        Channel *channelCluster = new Channel[CHANNEL_CLUSTER_SIZE];
		
		//add by hua.huangrh 08.7.21
		assert(channelCluster != NULL);
		_clusterList.push_back(channelCluster);
		//end add by hua.huangrh
		
        _freeListHead = _freeListTail = &channelCluster[1];
        for (size_t i = 2; i < CHANNEL_CLUSTER_SIZE; i++) {
            _freeListTail->_next = &channelCluster[i];
            channelCluster[i]._prev = _freeListTail;
            _freeListTail = _freeListTail->_next;
        }
        _freeListHead->_prev = NULL;
        _freeListTail->_next = NULL;
        channel = &channelCluster[0];   // �ѵ�һ��Ԫ���ù���ֱ����, ���ŵ�freelist��
    } else {
        // ����ͷȡ��
        channel = _freeListHead;
        _freeListHead = _freeListHead->_next;
        if (_freeListHead != NULL) {
            _freeListHead->_prev = NULL;
        } else {//add by hua.huangrh 08.7.22 11:09
            _freeListTail = NULL;
        }
    }

    // ��channel�ŵ�_useList��
    channel->_prev = _useListTail;
    channel->_next = NULL;
    if (_useListTail == NULL) {
        _useListHead = channel;
    } else {
        _useListTail->_next = channel;
    }
    _useListTail = channel;
    _useListCount ++;

    // ����id
    uint32_t id = chid;
    if (id <= AUTO_CHANNEL_ID_MAX) {
        do {
        id = atomic_add_return(1, &_globalChannelId);
        id &= AUTO_CHANNEL_ID_MAX;
        } while (0 == id);
    }

    channel->_id = id;
    channel->_expireTime = 0;
    channel->_handler = NULL;
    channel->_args = NULL;

    // ��channel�ŵ�hashmap��
    _useMap[id] = channel;
    _mutex.unlock();

    return channel;
}

/*
 * �ͷ�һ��channel
 *
 * @param channel: Ҫ�ͷŵ�channel
 * @return
 */
bool ChannelPool::freeChannel(Channel *channel) {
	if (channel == NULL) return false;
    _mutex.lock();
    _useMap.erase(channel->_id);

    // ��_userListɾ��
    if (channel == _useListHead) { // head
        _useListHead = channel->_next;
    }
    if (channel == _useListTail) { // tail
        _useListTail = channel->_prev;
    }
    if (channel->_prev != NULL)
        channel->_prev->_next = channel->_next;
    if (channel->_next != NULL)
        channel->_next->_prev = channel->_prev;
    _useListCount --;

    // ���뵽_freeList
    channel->_prev = _freeListTail;
    channel->_next = NULL;
    if (_freeListTail == NULL) {
        _freeListHead = channel;
    } else {
        _freeListTail->_next = channel;
    }
    _freeListTail = channel;
    _mutex.unlock();

    return true;
}

/*
 * ����ID���ҳ�һ��Channel
 *
 * @param  id: ͨ��ID
 * @reutrn Channel
 */
Channel *ChannelPool::findChannel(uint32_t id) {
    Channel *channel = NULL;

    __gnu_cxx::hash_map<uint32_t, Channel*>::iterator it;
    _mutex.lock();
    it = _useMap.find(id);
    if (it != _useMap.end()) {
        channel = it->second;
    }
    if (channel) {
        /**@todo why to use such strange logic?*/
        channel->_expireTime += 200000;
    }
    _mutex.unlock();

    return channel;
}

/*
 * ��useList���ҳ���ʱ��channel��list,����hashmap�ж�Ӧ��ɾ��
 *
 * @param now: ��ǰʱ��
 */
Channel* ChannelPool::getTimeoutList(int64_t now) {
    Channel *list = NULL;

    _mutex.lock();
    if (_useListHead == NULL) { //�ǿ�
        _mutex.unlock();
        return list;
    }

    Channel *channel = _useListHead;
    while (channel != NULL) {
        if (channel->_expireTime == 0 || channel->_expireTime >= now) break;
        _useMap.erase(channel->_id);
        channel = channel->_next;
        _useListCount --;
    }

    // �г�ʱ��list
    if (channel != _useListHead) {
        list = _useListHead;
        if (channel == NULL) {  // ȫ����ʱ
            _useListHead = _useListTail = NULL;
        } else {
            channel->_prev->_next = NULL;
            channel->_prev = NULL;
            _useListHead = channel;
        }
    }
    _mutex.unlock();

    return list;
}

/*
 * ��addList��������뵽freeList��
 *
 * @param addList���ӵ�list
 */
bool ChannelPool::appendFreeList(Channel *addList) {
    // �ǿ�
    if (addList == NULL) {
        return true;
    }
    
    _mutex.lock();
    
    // ��tail
    Channel *tail = addList;
    while (tail->_next != NULL) {
        tail = tail->_next;
    }

    // ���뵽_freeList
    addList->_prev = _freeListTail;
    if (_freeListTail == NULL) {
        _freeListHead = addList;
    } else {
        _freeListTail->_next = addList;
    }
    _freeListTail = tail;

    _mutex.unlock();
    return true;
}

}


