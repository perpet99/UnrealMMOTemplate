// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatServerPlayerController.h"
#include <cpp_redis/core/client.hpp>
#include "ChatServerGameMode.h"
#include "ChatOnlineBeaconClient.h"

void AChatServerPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AChatServerPlayerController::Tick(float delta)
{
	static float time = 0.0f;
	static int count = 0;
	time += delta;
	if (5 < time)
	{
		time = 0;
		SendChat_CS(FString::FormatAsNumber( count) + "message ");
	}
}

void AChatServerPlayerController::SendChat_CS_Implementation(const FString & chat)
{
	if (AChatServerGameMode::BeaconClient->GetConnectionState() == EBeaconConnectionState::Open)
	{
		AChatServerGameMode::BeaconClient->Chat_CS(chat);
	}
}


bool AChatServerPlayerController::SendChat_SC_Validate(const FString & chat)
{
	return true;
}


void AChatServerPlayerController::SendChat_SC_Implementation(const FString & chat)
{

}