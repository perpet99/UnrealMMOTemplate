// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Runtime/Core/Public/HAL/ThreadSafeBool.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include <cpp_redis/network/tcp_client_iface.hpp>
//#include "../../../../work/trunk5/Plugins/RedisClientPlugin/Source/RedisClientPlugin/cpp_redis/network/tcp_client_iface.hpp"

//#include "Task.h"

class FSocket;
class TCPRedisConnection;
class Task;
class TaskThreadPool;
class FRunnableThread;
class IRedisReply;

//struct RecvProcResult;
//struct SendProcResult;



struct RecvProcResult
{
	cpp_redis::network::tcp_client_iface::async_read_callback_t recv_callback;
	cpp_redis::network::tcp_client_iface::read_result recv_result;
};

struct SendProcResult
{
	cpp_redis::network::tcp_client_iface::async_write_callback_t send_callback;
	cpp_redis::network::tcp_client_iface::write_result send_result;
};

/**
 * 
 */
class TaskThread : public FRunnable
{
public:	
	explicit TaskThread(FSocket* sock);
	virtual ~TaskThread();

	virtual bool Init() override;

	//This is called if a thread is requested to terminate early.
	virtual void Stop() override;

	// Upon completion, Exit() is called to allow correct clean up.
	virtual void Exit() override;
	
	virtual void Close();
	
	void WaitForCompletion();

protected:
	bool CheckSocketState_Unsafe();

protected:
	FSocket* RedisSock;
	FThreadSafeBool bRunning;
	TSharedPtr<FRunnableThread> RunnableThread;
};

class RecvTaskThread : public TaskThread
{
public:
	RecvTaskThread(FSocket* Sock, TQueue<RecvProcResult, EQueueMode::Mpsc>& queue);		
	virtual ~RecvTaskThread() {}
	
	virtual uint32 Run() override;

	void EnqueueRecvRequest(cpp_redis::network::tcp_client_iface::read_request& req);

private:
	bool ProcRecvRequest_Unsafe(cpp_redis::network::tcp_client_iface::read_request& req);

private:
	TQueue<RecvProcResult, EQueueMode::Mpsc>& RecvedQueue;
	TQueue<cpp_redis::network::tcp_client_iface::read_request> RequestQueue;
};

class SendTaskThread : public TaskThread
{
public:
	SendTaskThread(FSocket* Sock, TQueue<SendProcResult, EQueueMode::Mpsc>& queue);
	virtual ~SendTaskThread() {}

	virtual uint32 Run() override;

	void EnqueueSendRequest(cpp_redis::network::tcp_client_iface::write_request& req);

private:
	bool ProcSendRequest_Unsafe(cpp_redis::network::tcp_client_iface::write_request& req);

private:
	TQueue<SendProcResult, EQueueMode::Mpsc>& SendedQueue;
	TQueue<cpp_redis::network::tcp_client_iface::write_request> RequestQueue;
};
