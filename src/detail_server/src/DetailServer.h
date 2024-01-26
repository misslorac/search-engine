#ifndef _DI_CLASS_DETAILSERVER_H_
#define _DI_CLASS_DETAILSERVER_H_


#include "mxml.h"
#include "util/common.h"
#include "framework/Worker.h"
#include "framework/Server.h"
#include "framework/Session.h"
#include "framework/Command.h"
#include "framework/Context.h"
#include "framework/MemPoolFactory.h"
#include "framework/namespace.h"
#include "update/DetailUpdater.h"


#define DETAIL detail


using namespace UPDATE;

namespace detail{

class DetailWorker : public FRAMEWORK::Worker
{
	public:
		
		DetailWorker(FRAMEWORK::Session &session, 
		             uint64_t timeout = 0);
		
		~DetailWorker();
		
		/**
		 *@brief DetailWorker�Ĵ�����
		 *@return 0,�ɹ� ��0,ʧ��
		 */
		virtual int run();
		
		/**
		 *@brief ���շ�����Ϣ
		 *@param req request����
		 *@return 0,�ɹ� ��0,ʧ��
		 */
		void handleTimeout();
		
	private:
		
		uint64_t _timeout;
};

class DetailWorkerFactory : public FRAMEWORK::WorkerFactory
{
	public:
		
		DetailWorkerFactory();
		
		~DetailWorkerFactory();
		
	public:
		
		/**
		 *@brief ����������Ϣ�����г�ʼ��
		 *@param path �����ļ�·��
		 *@return 0,�ɹ� ��0,ʧ��
		 */
		virtual int initilize(const char *path);
		
		/**
		 *@brief ����Worker����
		 *@param session Ŀ��session����
		 *@return Worker����
		 */
		virtual FRAMEWORK::Worker * makeWorker(FRAMEWORK::Session &session);
		
	private:
		
		bool _ready;
		mxml_node_t *_pXMLTree;
		
		Updater *_pDetailUpdater;
		pthread_t _updateTid;
};

class DetailServer : public FRAMEWORK::Server
{
	public:
	
		DetailServer() { }
		
		~DetailServer() { }
		
	protected:
		
		virtual FRAMEWORK::WorkerFactory* makeWorkerFactory();
		
};

class DetailCommand : public FRAMEWORK::CommandLine
{
	public:
		
		DetailCommand() : CommandLine("ks_search_server") { }
		
		~DetailCommand() { }
		
	protected:
		
		virtual FRAMEWORK::Server * makeServer();
};


};


#endif //_SEARCHERSERVER_H_
