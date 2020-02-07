// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "ChatOnlineBeaconManager.h"

#include "ChatServerGameManagerMode.generated.h"

//#define __CPP_REDIS_USE_CUSTOM_TCP_CLIENT

/**
 * 
 */
UCLASS()
class UMMOTEMPLATE_API AChatServerGameManagerMode : public AGameMode
{
	GENERATED_BODY()


		void BeginPlay();

		void Tick(float delta);
		void EndPlay(const EEndPlayReason::Type EndPlayReason);

		FChatOnlineBeaconManager BeaconManager;

};
