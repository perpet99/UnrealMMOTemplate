// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHost.h"


#include "ChatOnlineBeaconManager.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct  FChatOnlineBeaconManager
{
	GENERATED_BODY()


	class AChatOnlineBeaconHost* BeaconHost;
	class AChatOnlineBeaconHostObject * BeaconHostObject;


	static class  AChatOnlineBeaconClient* BeaconClient;

	void StartHost(UWorld* world);
	void StartClient(UWorld* world);
};
