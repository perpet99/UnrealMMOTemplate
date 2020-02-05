// Fill out your copyright notice in the Description page of Project Settings.


#include "RedisTestGameMode.h"


void ARedisTestGameMode::BeginPlay()
{
//	cpp_redis::client client;
	
	
	
	TcpCon = std::shared_ptr<cpp_redis::network::ue_tcp_client>(new cpp_redis::network::ue_tcp_client());
	RedisClient = TSharedPtr<cpp_redis::client>(new cpp_redis::client(TcpCon));

	

	RedisClient->connect();
}


void ARedisTestGameMode::Tick(float delta)
{
	Super::Tick(delta); 

	static float time = 0;
	time += delta;

	TcpCon->UpdateRedisConnection();
	 
	if (5 < time)
	{
		time = 0;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("redis set = "));

		RedisClient->set("hello", "42");

		RedisClient->get("hello", [](cpp_redis::reply& reply) {
			
			FString str = UTF8_TO_TCHAR(reply.as_string().c_str());
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("redis get %s"), *str));
//			reply.
			//UE_LOG(LogTemp, Log, TEXT("redis get = %s"),reply.as_string().c_str());
			std::cout << reply << std::endl;
		});
		RedisClient->commit();
	}
}

void ARedisTestGameMode::EndPlay(EEndPlayReason::Type reason)
{
	RedisClient->disconnect();
}

