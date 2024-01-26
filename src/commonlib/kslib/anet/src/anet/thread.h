#ifndef ANET_THREAD_H_
#define ANET_THREAD_H_
#include <pthread.h>
#include <anet/runnable.h>
//#include <anet/log.h>
namespace anet {
  class Runnable;
  class Thread {

  public:
    /**
     * ���캯��
     */
    Thread() {
      tid = 0;
    }

    /**
     * ��һ���̣߳���ʼ����
     */
    void start(Runnable *r, void *a) {
      runnable = r;
      args = a;
      pthread_create(&tid, NULL, Thread::hook, this);
    }

    /**
     * �ȴ��߳��˳�
     */
    void join() {
      if (tid) {
	pthread_join(tid, NULL);
      }
      tid = 0;
    }

    /**
     * �õ�Runnable����
     * 
     * @return Runnable
     */
    Runnable *getRunnable() {
      return runnable;
    }
    
    /**
     * set the priority of this thread
     * @param priority Note only process with root privilege can set
     * priority to value greater than 0.
     * @param policy scheduling policy. Valid values are SCHED_OTHER,
     * SCHED_FIFO, SCHED_RR. SCHED_OTHER only has one priority 0.
     * SCHED_FIFO and SCHED_RR have priorities ranges from 1 to 99.
     * @return return true if success. 
     **/
    bool setPriority(int priority, int policy = SCHED_OTHER) {
      struct sched_param schedparam;
      schedparam.sched_priority = priority;
      int ret = pthread_setschedparam(tid, policy, &schedparam);
      if(ret != 0) {
//	ANET_LOG(WARN,"setPriority failed! Thread (%d), Error No(%d)", tid,ret);
	return false;
      }
      return true;
    }

    /**
     * �õ��ص�����
     * 
     * @return args
     */
    void *getArgs() {
      return args;
    }

    /**
     * �̵߳Ļص�����
     * 
     */

    static void *hook(void *arg) {
      Thread *thread = (Thread*) arg;

      if (thread->getRunnable()) {
	thread->getRunnable()->run(thread, thread->getArgs());
      }

      return (void*) NULL;
    }

  private:

    pthread_t tid;
    Runnable *runnable;
    void *args;
  };

}

#endif /*THREAD_H_*/
