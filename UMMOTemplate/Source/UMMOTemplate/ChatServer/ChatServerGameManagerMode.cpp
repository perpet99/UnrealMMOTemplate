// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatServerGameManagerMode.h"
#include "ChatOnlineBeaconHost.h"
#include "ChatOnlineBeaconHostObject.h"


void AChatServerGameManagerMode::BeginPlay()
{
	Super::BeginPlay();
	


	HostObject = GetWorld()->SpawnActor<AChatOnlineBeaconHost>(AChatOnlineBeaconHost::StaticClass());
	if (nullptr == HostObject)
	{
		UE_LOG(LogTemp, Log, TEXT("nullptr == HostObject."));
		return;
	}

	if (false == HostObject->InitHost())
	{
		UE_LOG(LogTemp, Log, TEXT("false == HostObject->InitBeaconHost."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("BeaconHost Initialize Success."));
	HostObject->PauseBeaconRequests(false); // 조정 필요

	SessionObject = GetWorld()->SpawnActor<AChatOnlineBeaconHostObject>(AChatOnlineBeaconHostObject::StaticClass());

	if (nullptr != SessionObject)
	{
		HostObject->RegisterHost(SessionObject);
	}
}


void AChatServerGameManagerMode::Tick(float delta)
{
	Super::Tick(delta); 


}

void AChatServerGameManagerMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

