// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatOnlineBeaconHostObject.h"
#include "ChatOnlineBeaconClient.h"



AChatOnlineBeaconHostObject::AChatOnlineBeaconHostObject(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	/** Set our actual actor client class this host object will handle */
	ClientBeaconActorClass = AChatOnlineBeaconClient::StaticClass();
	/** Set the beacon type name **/
	BeaconTypeName = ClientBeaconActorClass->GetName();

	/** Make sure we can tick **/
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

bool AChatOnlineBeaconHostObject::Init()
{
	//just returning true for now
	return true;
}

void AChatOnlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	//Call super
	Super::OnClientConnected(NewClientActor, ClientConnection);

	//Cast to our actual APingBeacon
	AChatOnlineBeaconClient* BeaconClient = Cast<AChatOnlineBeaconClient>(NewClientActor);
	if (BeaconClient != NULL)
	{
		//It's good, so lets rpc back to the client and tell it we are ready
		BeaconClient->Ready();
	}
}

AOnlineBeaconClient* AChatOnlineBeaconHostObject::SpawnBeaconActor(UNetConnection* ClientConnection)
{
	//Just super for now, technically you can return NULL here as well to prevent spawning
	return Super::SpawnBeaconActor(ClientConnection);
}