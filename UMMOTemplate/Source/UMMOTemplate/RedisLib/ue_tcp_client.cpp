// MIT License
//
// Copyright (c) 2016-2017 Simon Ninon <simon.ninon@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cpp_redis/misc/macro.hpp>
#include <cpp_redis/network/tcp_client.hpp>

#include "Runtime/Sockets/Public/SocketTypes.h"
#include "Runtime/Sockets/Public/IPAddress.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Core/Public/Misc/ScopeLock.h"
#include "ue_tcp_client.hpp"

namespace cpp_redis {

namespace network {

void
ue_tcp_client::connect(const std::string& addr, std::uint32_t port, std::uint32_t timeout_ms) {

	UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - Connect Start"));
	ISocketSubsystem* SubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (nullptr == SubSystem)
	{
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - SubSystem is nullptr"));
		return;
	}

	RedisSock = SubSystem->CreateSocket(NAME_Stream, TEXT("redis_socket"));
	if (nullptr == RedisSock)
	{
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - socket is nullptr"));
		return;
	}

	// ´Ù¸¥ threadÀÇ ½ºÄÉÁì¸µÀÌ ±ÍÂúÀ¸¹Ç·Î blocking ÇÑ´Ù.
	RedisSock->SetNonBlocking(true);
	//RedisSock->SetNonBlocking(false);

	TSharedRef<FInternetAddr> NetAddr = SubSystem->CreateInternetAddr(0, port);
	//TSharedPtr<FInternetAddr> NetAddr = SubSystem->GetAddressFromString(ANSI_TO_TCHAR(addr.c_str()));
	//NetAddr->SetPort(port);

	ESocketErrors SockError = SubSystem->GetHostByName(addr.c_str(), *NetAddr);
	//auto result = SubSystem->GetHostByName(addr.c_str());

	if (SE_NO_ERROR != SockError)
	{
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - Socket error is something wrong."));
		return;
	}

	//const auto &resolvedAddr = result->GetResolvedAddress();

	UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - create socket success. request connect."));
	bool bSuccess = RedisSock->Connect(*NetAddr); // block

	UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - connect finish result is - %d"),
		bSuccess == true ? 1 : 0);

	if (!bSuccess)
	{
		ESocketErrors errCode = SubSystem->GetLastErrorCode();
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::connect - connect failed - error code : %d"), (int)errCode);

		RedisSock->Close();
		SubSystem->DestroySocket(RedisSock);
		RedisSock = nullptr;
	}
	else
	{
		if (!SocketReceiver.IsValid())
		{
			SocketReceiver = TSharedPtr<RecvTaskThread>(
				new RecvTaskThread(RedisSock, RecvResultQueue));
		}
		if (!SocketSender.IsValid())
		{
			SocketSender = TSharedPtr<SendTaskThread>(
				new SendTaskThread(RedisSock, SendResultQueue));
		}
	}

	bIsConnected.AtomicSet(bSuccess);
}


void
ue_tcp_client::disconnect(bool wait_for_removal) {
	if (nullptr != RedisSock && true == bIsConnected)
	{
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - Disconnect process start"));
		if (RedisSock->Shutdown(ESocketShutdownMode::ReadWrite))
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - socket shutdown successful"));
		}
		else
		{
			uint32 LastErr = FPlatformMisc::GetLastError();
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - shutdown failed [error code %d]"), LastErr);
		}

		if (RedisSock->Close())
		{
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - socket close successful"));
		}
		else
		{
			uint32 LastErr = FPlatformMisc::GetLastError();
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - socket close failed [error code %d]"), LastErr);
		}

		if (SocketReceiver.IsValid())
		{
			SocketReceiver->Close();
			SocketReceiver.Reset();
		}
		if (SocketSender.IsValid())
		{
			SocketSender->Close();
			SocketSender.Reset();
		}

		ISocketSubsystem* SubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (SubSystem)
		{
			SubSystem->DestroySocket(RedisSock);
			UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - Socket destoryed."));
		}

		RedisSock = nullptr;
		UE_LOG(LogTemp, Log, TEXT("TCPRedisConnection::disconnect - Disconnect process finish"));
	}
}

bool
ue_tcp_client::is_connected(void) const {
	if (nullptr == RedisSock)
		return false;

	ESocketConnectionState state = RedisSock->GetConnectionState();
	switch (state)
	{
	case SCS_NotConnected:
		return false;
	case SCS_Connected:
		return true;
	case SCS_ConnectionError:
		return false;
	default:
		break;
	}

	return false;
}

void
ue_tcp_client::set_nb_workers(std::size_t nb_threads) {
//  m_client.get_io_service()->set_nb_workers(nb_threads);
}

void
ue_tcp_client::async_read(read_request& request) {
  
	if (nullptr == RedisSock || false == bIsConnected)
		return;

	if (!SocketReceiver.IsValid())
		return;

	SocketReceiver->EnqueueRecvRequest(request);
}

void
ue_tcp_client::async_write(write_request& request) {
  
	if (nullptr == RedisSock || false == bIsConnected)
		return;

	if (!SocketSender.IsValid())
		return;

	SocketSender->EnqueueSendRequest(request);
}

void
ue_tcp_client::set_on_disconnection_handler(const disconnection_handler_t& disconnection_handler) {
  //m_client.set_on_disconnection_handler(disconnection_handler);
}


void ue_tcp_client::UpdateRedisConnection()
{
	if (!RecvResultQueue.IsEmpty())
	{
		RecvProcResult result;
		while (RecvResultQueue.Dequeue(result))
		{
			if (!result.recv_callback)
				continue;

			result.recv_callback(result.recv_result);
		}
	}

	if (!SendResultQueue.IsEmpty())
	{
		SendProcResult result;
		while (SendResultQueue.Dequeue(result))
		{
			if (!result.send_callback)
				continue;

			result.send_callback(result.send_result);
		}
	}
}


//
//void
//set_default_nb_workers(std::size_t nb_threads) {
//  //tacopie::get_default_io_service()->set_nb_workers(__CPP_REDIS_LENGTH(nb_threads));
//}

} // namespace network

} // namespace cpp_redis
