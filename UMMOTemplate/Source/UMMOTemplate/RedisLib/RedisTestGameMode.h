// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "cpp_redis/core/client.hpp"
#include "ue_tcp_client.hpp"
#include "RedisTestGameMode.generated.h"

#define __CPP_REDIS_USE_CUSTOM_TCP_CLIENT

/**
 * 
 */
UCLASS()
class UMMOTEMPLATE_API ARedisTestGameMode : public AGameMode
{
	GENERATED_BODY()

		std::shared_ptr < cpp_redis::network::ue_tcp_client> TcpCon;

		TSharedPtr<cpp_redis::client> RedisClient;

		void BeginPlay();

		void Tick(float delta);
		void EndPlay(EEndPlayReason::Type reason);
};
