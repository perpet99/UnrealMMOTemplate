// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatOnlineBeaconManager.h"

#include "ChatOnlineBeaconHost.h"
#include "ChatOnlineBeaconClient.h"
#include "ChatOnlineBeaconHostObject.h"

AChatOnlineBeaconClient* FChatOnlineBeaconManager::BeaconClient = nullptr;

void FChatOnlineBeaconManager::StartHost(UWorld* world)
{

	BeaconHost = world->SpawnActor<AChatOnlineBeaconHost>(AChatOnlineBeaconHost::StaticClass());
	if (nullptr == BeaconHost)
	{
		UE_LOG(LogTemp, Log, TEXT("nullptr == HostObject."));
		return;
	}

	if (false == BeaconHost->InitHost())
	{
		UE_LOG(LogTemp, Log, TEXT("false == HostObject->InitBeaconHost."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("BeaconHost Initialize Success."));
	BeaconHost->PauseBeaconRequests(false); // 조정 필요

	BeaconHostObject = world->SpawnActor<AChatOnlineBeaconHostObject>(AChatOnlineBeaconHostObject::StaticClass());

	if (nullptr != BeaconHostObject)
	{
		BeaconHost->RegisterHost(BeaconHostObject);
	}
}

void FChatOnlineBeaconManager::StartClient(UWorld* world)
{

	BeaconClient = world->SpawnActor<AChatOnlineBeaconClient>(AChatOnlineBeaconClient::StaticClass());

	if (nullptr == BeaconClient)
	{
		UE_LOG(LogTemp, Log, TEXT("nullptr == BeaconClient."));
		return;
	}

	FURL url(nullptr, TEXT("127.0.0.1"), TRAVEL_Absolute);

	url.Port = 7787;

	if (false == BeaconClient->InitClient(url))
	{
		UE_LOG(LogTemp, Log, TEXT("false == BeaconClient->InitClient."));
		return;
	}
}
