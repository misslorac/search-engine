#ifndef _DELAYFREEQUEUE_H
#define _DELAYFREEQUEUE_H

#include <stdio.h>
#include <stdint.h>

typedef uint32_t u_int; 

namespace index_mem {

struct delay_t {
    u_int reltime;
    u_int addres;
};

struct queue_t {
    int _capacity;
    int _size;
    int _front;
    int _rear;
    struct delay_t _array[0];
};

class DelayFreeQueue
{
private:
	struct queue_t *_data;		//���д洢���ݿռ�
    size_t          _dSize;     //���д洢���ݿռ��С

public:
	DelayFreeQueue() {
		_data  = NULL;
        _dSize = 0;
	}

	~DelayFreeQueue() {
	}

	/**
	 * @brief ���ض���֧�ֵ��������
	 *
	 * @return  int ����֧�ֵ��������
	**/
	int capacity();

	/**
	 * @brief �ж϶����Ƿ�Ϊ��
	 *
	 * @return  bool ����true���ǿգ�false�ǿ�
	**/
	bool empty();

	/**
	 * @brief ���ض����Ƿ�����
	 *
	 * @return  bool ����true��������false����
	**/
	bool full();

	/**
	 * @brief ��ն��е����ݣ��������տռ�
	 *
	 * @return  void 
	**/
	void clear();

	/**
	 * @brief ��������
	 *
	 * @param [in qcap   : int ����֧�ֵ���󳤶�
	 * @return  int �ɹ�����0������ʧ��
	**/
	int create(struct queue_t * addr, int qcap);

    int load(struct queue_t * addr);

	int destroy();
	
	int push_back(const struct delay_t & val);

	int push_front(const struct delay_t &val);

	int top_back(struct delay_t &val);
	
	int pop_back(struct delay_t &val);

	int top_front(struct delay_t &val);

	int pop_front(struct delay_t &val);

	int pop_backs(struct delay_t *val, int nums); 	
};
}
#endif

