// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHost.h"
#include "ChatOnlineBeaconHost.generated.h"

/**
 * 
 */
UCLASS()
class UMMOTEMPLATE_API AChatOnlineBeaconHost : public AOnlineBeaconHost
{
	GENERATED_BODY()
public:
	/** Blueprint accessor to init the beacon host */
	UFUNCTION(BlueprintCallable, Category = "PingBeaconHost")
		bool Start();

	/** A blueprint helper to add our PingBeaconHostObject */
	UFUNCTION(BlueprintCallable, Category = "PingBeaconHost")
		void AddHost(AOnlineBeaconHostObject* HostObject);

		/** You can also remove a host if you so wish to as well */
		/** You can remove it with: UnregisterHost(const FString& BeaconType) */

protected:
	/** If we successfully started are not */
	bool IsReady;
};
