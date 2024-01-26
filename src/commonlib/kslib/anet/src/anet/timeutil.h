#ifndef ANET_TIMEUTIL_H_
#define ANET_TIMEUTIL_H_
#include <stdint.h>
namespace anet {

class TimeUtil {
public:
    /*
     * �õ���ǰʱ��
     */
    static int64_t getTime();

    /*
     * ���õ�ǰʱ�䵽now��
     */
    static void setNow();

public:
    static int64_t _now;
    static const int64_t MIN;
    static const int64_t MAX;
    static const int64_t PRE_MAX;
};

}

#endif
