#pragma once
#ifndef CONNECTSESSION_H_
#define CONNECTSESSION_H_
#include"SnBuffer.h"
#include"Reactor.h"

class ConnectSession: public std::enable_shared_from_this<ConnectSession>
{
public:
	ConnectSession(int sock, ReactorPtr spReactor);

	void setIndex(int index);

	int& getRefIndex();

	int getFd();

	ReactorPtr getReactor();

	void close();

	SnBuffer readbuffer_;
	SnBuffer writebuffer_;

	std::mutex mutex_;
protected:
	int sock_;
	int index_;
	ReactorPtr spReactor_;
	
};
typedef std::shared_ptr<ConnectSession>  ConnectSessionPtr;

#endif // !CONNECTSESSION_H_