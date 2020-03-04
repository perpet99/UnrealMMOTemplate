// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatServerGameMode.h"
#include "ChatOnlineBeaconClient.h"




void AChatServerGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	const TCHAR* cmd = FCommandLine::Get();

	bool RunManagerServer = false;

	if (FParse::Bool(cmd, TEXT("RunManagerServer="), RunManagerServer))
	{
		UE_LOG(LogTemp, Log, TEXT("read managerServer %d"), (int)RunManagerServer);
	}

//#if WITH_EDITOR
//	BeaconManager.StartHost(GetWorld());
//#endif
	if (RunManagerServer)
	{
		UE_LOG(LogTemp, Log, TEXT("run managerServer"));

		BeaconManager.StartHost(GetWorld());
	}
	else
	{
		BeaconManager.StartHost(GetWorld());
		BeaconManager.StartClient(GetWorld());
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

