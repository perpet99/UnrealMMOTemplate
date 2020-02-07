// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatServerPlayerController.h"
#include <cpp_redis/core/client.hpp>
#include "ChatServerGameMode.h"
#include "ChatOnlineBeaconClient.h"

void AChatServerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const TCHAR* cmd = FCommandLine::Get();

	if (FParse::Value(cmd, TEXT("UserName="), UserName))
	{
		
	}

}

void AChatServerPlayerController::Tick(float delta)
{

	// 클라만 콜
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		static float time = 0.0f;
		static int count = 0;
		time += delta;
		if (5 < time)
		{
			time = 0;
			// send test chat 

			FString message = UserName + " : Client " + FString::FormatAsNumber(count++);

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, FString::Printf(TEXT("send message : %s"), *message));

			SendChat_CS(message);
		}
	}
}

void AChatServerPlayerController::SendChat_CS_Implementation(const FString & chat)
{
	if (FChatOnlineBeaconManager::BeaconClient->GetConnectionState() == EBeaconConnectionState::Open)
	{
		// send to manager server
		UE_LOG(LogTemp, Log, TEXT("C=>S : %s"),*chat);
		FChatOnlineBeaconManager::BeaconClient->Chat_SM(chat+"=>game server");
	}
}


bool AChatServerPlayerController::SendChat_CS_Validate(const FString & chat)
{
	return true;
}


void AChatServerPlayerController::SendChat_SC_Implementation(const FString & chat)
{

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, FString::Printf(TEXT("recv message : %s =>Client"), *chat));
}