// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChatServerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UMMOTEMPLATE_API AChatServerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void	BeginPlay();


	UFUNCTION(client, reliable)
	virtual void SendChat_CS(const FString & chat);

	UFUNCTION(server, reliable, WithValidation)
	virtual void SendChat_SC(const FString & chat);

	void Tick(float delta);

};
