#include <anet/log.h>
#include <anet/stats.h>

namespace anet {
    
StatCounter StatCounter::_gStatCounter;

/*
 * ���캯��
 */
StatCounter::StatCounter()
{
    clear();
}

/*
 * ��������
 */
StatCounter::~StatCounter()
{
}

/*
 * ��statд��log��
 */
void StatCounter::log()
{
    ANET_LOG(INFO, "_packetReadCnt: %u, _packetWriteCnt: %u, _dataReadCnt: %u, _dataWriteCnt: %u",
        _packetReadCnt, _packetWriteCnt, _dataReadCnt, _dataWriteCnt);
}

/*
 * ���
 */
void StatCounter::clear()
{
    _packetReadCnt = 0;
    _packetWriteCnt = 0;
    _dataReadCnt = 0;
    _dataWriteCnt = 0;
}

}
