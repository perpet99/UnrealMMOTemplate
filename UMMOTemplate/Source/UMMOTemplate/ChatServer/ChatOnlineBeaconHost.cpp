// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatOnlineBeaconHost.h"

//AChatOnlineBeaconHost::AChatOnlineBeaconHost(const FObjectInitializer& ObjectInitializer) :
//	Super(ObjectInitializer)
//{
//	//Set the beacon host state to allow requests
//	BeaconState = EBeaconState::AllowRequests;
//}

bool AChatOnlineBeaconHost::Start()
{
	BeaconState = EBeaconState::AllowRequests;

	//Call our init to start up the network interface
	IsReady = InitHost();
	return IsReady;
}

void AChatOnlineBeaconHost::AddHost(AOnlineBeaconHostObject* HostObject)
{
	/** Make sure we inited properly */
	if (IsReady)
	{
		RegisterHost(HostObject);
	}
}