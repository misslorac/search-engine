�������ļ�˵����
Ŀ¼�� src/online

Command�ࣺ��װ�˷����������ֹͣ�Ȳ���
Command.cpp��Command.h

Server�ࣺ���������������ܡ����������Դ
Server.cpp��Server.h

Service�ࣺ����anet��ͨ�ŷ���ˣ��������ķ��񣨷���˿ڡ�anet�Ļص������������
Service.cpp��Service.h

Session�ࣺ���ڱ���Ự��Ϣ
Session.cpp��Session.h

TaskQueue�ࣺ������У����ڴ洢�ȴ����ȵ�session
TaskQueue.cpp��TaskQueue.h

Dispatcher�ࣺ�����ࡣ��һ���������̣߳������TaskQueue�еõ�session������session������Ӧ��worker�����̳߳�ָ���߳������и�worker��
Dispatcher.cpp��Dispatcher.h

WorkerFactory��Worker�ࣺWorker���������߳�ִ�в����ķ�װ�ࡢ���ղ�ͬ�Ľ�ɫ��ΪSearcherWork,MergerWorker,BlenderWorker
Worker.h��Worker.cpp

ParallelCounter�ࣺ���ڷ�����������Ƽ��쳣�ָ�
ParallelCounter.cpp��ParallelCounter.h

CacheHandle�ࣺ��memcache client�ķ�װ
Cache.cpp��Cache.h

ConnectionPool�ࣺ���ڹ���anet connection
ConnectionPool.h, ConnectionPool.cpp

Request�ࣺ����anet��ͨ�ſͻ��ˣ�����blender��merger��merger��searcher��������
Request.cpp��Request.h

WebProbe�ࣺ�����ṩ�򵥵��ļ����ʷ���
WebProbe.cpp��WebProbe.h

Query�ࣺ�������query��
Query.h��Query.cpp

CMClient�ࣺclustermap�ͻ���
