// Fill out your copyright notice in the Description page of Project Settings.

#include "TaskThread.h"
//#include "LogDefines.h"
//#include "Task.h"

#include "Runtime/Core/Public/HAL/Event.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Core/Public/HAL/PlatformMisc.h"

#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
//#include "../cpp_redis/network/tcp_client_iface.hpp"

using namespace cpp_redis::network;

TaskThread::TaskThread(FSocket* sock)
	: RedisSock(sock)
	, bRunning(false)
{
	//FRunnableThread* Thread = FRunnableThread::Create(this, TEXT("TaskThread"), 0, TPri_Normal);
	//RunnableThread = TSharedPtr<FRunnableThread>(Thread);
}


TaskThread::~TaskThread()
{

}

bool TaskThread::Init()
{
	if (nullptr == RedisSock)
		return false;

	bRunning.AtomicSet(true);
	return true;
}

void TaskThread::Stop()
{
	bRunning.AtomicSet(false);
}

void TaskThread::Exit()
{
	UE_LOG(LogTemp, Log, TEXT("Task Thread terminated."));
}

void TaskThread::Close()
{
	if (RunnableThread.IsValid())
	{
		RunnableThread->Kill(true);
		RunnableThread->WaitForCompletion();
	}
}

bool TaskThread::CheckSocketState_Unsafe()
{
	if (nullptr == RedisSock)
		return false;

	ESocketConnectionState state = RedisSock->GetConnectionState();
	switch (state)
	{
	case SCS_NotConnected:
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::CheckSocketState_Unsafe - SCS_NotConnected"));
		return false;
	case SCS_Connected:
		return true;
	case SCS_ConnectionError:
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::CheckSocketState_Unsafe - SCS_ConnectionError"));
		return false;
	default:
		break;
	}

	return false;
}
 
void TaskThread::WaitForCompletion()
{
	RunnableThread->WaitForCompletion();
}

RecvTaskThread::RecvTaskThread(FSocket* Sock, TQueue<RecvProcResult, EQueueMode::Mpsc>& queue)
	: TaskThread(Sock)
	, RecvedQueue(queue)
{
	FRunnableThread* Thread = FRunnableThread::Create(this, TEXT("RecvTaskThread"), 0, TPri_Normal);
	RunnableThread = TSharedPtr<FRunnableThread>(Thread);
}

uint32 RecvTaskThread::Run()
{
	UE_LOG(LogTemp, Log, TEXT("RecvTaskThread running"));
	while (bRunning)
	{
		if (RequestQueue.IsEmpty())
		{
			//이 sleep은 내부적으로 busy-waiting을 하므로 cpu점유율이 높아짐.
			//FGenericPlatformProcess::ConditionalSleep([this]()->bool { return true; }, 0.5f);

			FPlatformProcess::Sleep(0.01f);
			continue;
		}

		tcp_client_iface::read_request read_req;
		while (RequestQueue.Dequeue(read_req))
		{
			if (!ProcRecvRequest_Unsafe(read_req))
			{
				UE_LOG(LogTemp, Log, TEXT("RecvTaskThread request dequeue failed"));
				return 1;
			}
		}
	}

	return 0;
}

void RecvTaskThread::EnqueueRecvRequest(cpp_redis::network::tcp_client_iface::read_request& req)
{
	RequestQueue.Enqueue(req);
}

bool RecvTaskThread::ProcRecvRequest_Unsafe(cpp_redis::network::tcp_client_iface::read_request& req)
{
	RecvProcResult ProcRes;
	ProcRes.recv_callback = req.async_read_callback;

	tcp_client_iface::read_result& rres = ProcRes.recv_result;
	rres.success = false;

	do
	{
		if (!CheckSocketState_Unsafe())
			return false;

		if (0 == req.size)
		{
			UE_LOG(LogTemp, Log, TEXT("RecvTaskThread::ProcRecvRequest_NotSafe - invalid read request"));
			break;
		}

		int32 RecvByteLen = 0;
		rres.buffer.resize(req.size, 0);

		// 여기서 Block됨.
		if (false == RedisSock->Recv((uint8*)&rres.buffer[0], req.size, RecvByteLen, ESocketReceiveFlags::None))
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_read - recv failed"));
			ISocketSubsystem* SubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
			if (nullptr != SubSystem)
			{
				ESocketErrors errCode = SubSystem->GetLastErrorCode();
				UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_read - error code : %d"), errCode);
			}
			break;
		}

		rres.success = true;
		// packet parsing 처리는 cpp_redis 내부에서 처리함.
		rres.buffer.resize(RecvByteLen);

	} while (false);

	RecvedQueue.Enqueue(ProcRes);	
	return true;
}

SendTaskThread::SendTaskThread(FSocket* Sock, TQueue<SendProcResult, EQueueMode::Mpsc>& queue)
	: TaskThread(Sock)
	, SendedQueue(queue)
{
	FRunnableThread* Thread = FRunnableThread::Create(this, TEXT("SendTaskThread"), 0, TPri_Normal);
	RunnableThread = TSharedPtr<FRunnableThread>(Thread);
}

uint32 SendTaskThread::Run()
{
	UE_LOG(LogTemp, Log, TEXT("SendTaskThread running"));
	while (bRunning)
	{
		if (RequestQueue.IsEmpty())
		{
			FPlatformProcess::Sleep(0.01f);
			continue;
		}

		tcp_client_iface::write_request write_req;
		while (RequestQueue.Dequeue(write_req))
		{
			if (!ProcSendRequest_Unsafe(write_req))
			{
				UE_LOG(LogTemp, Log, TEXT("SendTaskThread request dequeue failed"));
				return 1;
			}
		}
	}

	return 0;
}

void SendTaskThread::EnqueueSendRequest(cpp_redis::network::tcp_client_iface::write_request & req)
{
	RequestQueue.Enqueue(req);
}

bool SendTaskThread::ProcSendRequest_Unsafe(cpp_redis::network::tcp_client_iface::write_request & req)
{
	SendProcResult ProcRes;
	ProcRes.send_callback = req.async_write_callback;

	tcp_client_iface::write_result& wres = ProcRes.send_result;
	wres.success = false;

	do
	{
		if (!CheckSocketState_Unsafe())
			return false;

		if (0 == req.buffer.size())
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_write - invalid write request"));
			break;
		}

		int32 SentByteLen = 0;
		// 여기서 Block됨.
		if (false == RedisSock->Send((uint8*)&req.buffer[0], req.buffer.size(), SentByteLen))
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_write - recv failed"));
			ISocketSubsystem* SubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
			if (nullptr != SubSystem)
			{
				ESocketErrors errCode = SubSystem->GetLastErrorCode();
				UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_write - error code : %d"), errCode);
			}
			break;
		}

		if (SentByteLen != req.buffer.size())
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::async_write - send failed. size invalid"));
			break;
		}

		wres.success = true;
		wres.size = SentByteLen;
	} while (false);

	SendedQueue.Enqueue(ProcRes);
	return true;
}
