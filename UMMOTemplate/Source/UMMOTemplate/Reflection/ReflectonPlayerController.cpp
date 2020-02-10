// Fill out your copyright notice in the Description page of Project Settings.


#include "ReflectionPlayerController.h"

void AReflectionPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void AReflectionPlayerController::Tick(float delta)
{
	Super::Tick(delta);
	
}

void AReflectionPlayerController::SendChat_CS_Implementation(const FString & chat)
{
	
}


bool AReflectionPlayerController::SendChat_CS_Validate(const FString & chat)
{
	return true;
}


void AReflectionPlayerController::SendChat_SC_Implementation(const FString & chat)
{

	
}