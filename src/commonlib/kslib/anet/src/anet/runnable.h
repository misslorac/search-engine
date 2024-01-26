#ifndef ANET_RUNNABLE_H_
#define ANET_RUNNABLE_H_

namespace anet {
  class Thread;
  class Runnable {

  public:
    /*
     * ����
     */
    virtual ~Runnable() {}
    /*
     * ������ں���
     */
    virtual void run(Thread *thread, void *arg) = 0;
  };
}

#endif /*RUNNABLE_H_*/
