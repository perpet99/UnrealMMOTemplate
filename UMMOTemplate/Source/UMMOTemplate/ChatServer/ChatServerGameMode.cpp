// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatServerGameMode.h"
#include "ChatOnlineBeaconClient.h"

AChatOnlineBeaconClient* AChatServerGameMode::BeaconClient = nullptr;


void AChatServerGameMode::BeginPlay()
{
	Super::BeginPlay();
	

	BeaconClient = GetWorld()->SpawnActor<AChatOnlineBeaconClient>(AChatOnlineBeaconClient::StaticClass());

	if (nullptr == BeaconClient)
	{
		UE_LOG(LogTemp, Log, TEXT("nullptr == BeaconClient."));
		return ;
	}

	FURL url(nullptr, TEXT("127.0.0.1"), TRAVEL_Absolute);

	url.Port = 7787;

	if (false == BeaconClient->InitClient(url))
	{
		UE_LOG(LogTemp, Log, TEXT("false == BeaconClient->InitClient."));
		return ;
	}

}


void AChatServerGameMode::Tick(float delta)
{
	Super::Tick(delta); 

	//static float time = 0;
	//time += delta;
	//if (5 < time)
	//{
	//	time = 0;

	//	if (BeaconClient->ConnectionState == EBeaconConnectionState::Open)
	//	{
	//		BeaconClient->
	//	}


	//}
}

void AChatServerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

