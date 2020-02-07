// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "ChatServerGameMode.generated.h"

#define __CPP_REDIS_USE_CUSTOM_TCP_CLIENT

/**
 * https://forums.unrealengine.com/community/community-content-tools-and-tutorials/1355434-onlinebeacons-tutorial-with-blueprint-access
 * https://www.slideshare.net/flashgamm/online-beacons-tips-and-tricks
 */
UCLASS()
class UMMOTEMPLATE_API AChatServerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
		void BeginPlay();

		void Tick(float delta);
		void EndPlay(const EEndPlayReason::Type EndPlayReason);
		static class  AChatOnlineBeaconClient* BeaconClient;

};
