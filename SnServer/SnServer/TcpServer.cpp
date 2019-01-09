#include "stdafx.h"
#include "AsyncLog.h"
#include "SnBuffer.h"
#include "TcpServer.h"

TcpServer::TcpServer() :spQueue_(std::make_shared<MessageQueue>()), pollNum_(3), workerNum_(1)
{
}
TcpServer::~TcpServer()
{
}
void TcpServer::run()
{
	listenAddress_.strIp_ = "0.0.0.0";
	listenAddress_.port = 4321;
	listtenSocket_.BindAddress(listenAddress_);
	if (!listtenSocket_.Listen(65535))
	{
		LOG_ERROR() << " listen failed";
		return;
	}
	socketutil::setNonblocking(listtenSocket_.GetSockFd());
	LOG_INFO() << "listen OK";
	std::vector<MessageQueuePtr> vecQueue;
	for (int i = 0; i < pollNum_; ++i)
	{
		auto spReadThread = std::make_shared<ReadThread>(listtenSocket_.GetSockFd());
		//pollThreads_.insert(std::make_pair(ThreadRAIIPtr(std::thread(std::bind(&ReadThread::run, spReadThread))), spReadThread));
		vecThreads_.emplace_back(ThreadRAII(std::thread(std::bind(&ReadThread::run, spReadThread)), ThreadRAII::DtorAction::join));
		vecQueue.push_back(spReadThread->getQueue());
	}
	for (int i = 0; i < workerNum_; ++i)
	{
		auto spWorker = std::make_shared<MessageProcessThread>(vecQueue[i%vecQueue.size()]);
		//workerThreads_.insert(std::make_pair(ThreadPtr(new std::thread(std::bind(&MessageProcessThread::run, spWorker))), spWorker));
		vecThreads_.emplace_back(ThreadRAII(std::thread(std::bind(&MessageProcessThread::run, spWorker)), ThreadRAII::DtorAction::join));
	}
}