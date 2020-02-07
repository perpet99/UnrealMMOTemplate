// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include <../Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Public/OnlineBeaconHostObject.h>
#include "OnlineBeaconHostObject.h"
#include "ChatOnlineBeaconHostObject.generated.h"

/**
 * 
 */
UCLASS()
class UMMOTEMPLATE_API AChatOnlineBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
public:
		AChatOnlineBeaconHostObject(const FObjectInitializer& ObjectInitializer);
		//~ Begin AOnlineBeaconHost Interface
   /** You can do stuff in this one if you want, but we just use the super for this example */
		virtual AOnlineBeaconClient* SpawnBeaconActor(class UNetConnection* ClientConnection) override;
	virtual void OnClientConnected(class AOnlineBeaconClient* NewClientActor, class UNetConnection* ClientConnection) override;
	//~ End AOnlineBeaconHost Interface

	/** In case you ever want to do other things */
	virtual bool Init();

	class AChatOnlineBeaconClient * GetClient(int num);
	//	Other override functions of the AOnlineBeaconHostObject:

		/**
		 * Disconnect a given client from the host
		 *
		 * param ClientActor the beacon client to disconnect
		 */

	//virtual void DisconnectClient(AOnlineBeaconClient* ClientActor);

	/**
	 * Notification that a client has been disconnected from the host in some way (timeout, client initiated, etc)
	 *
	 * param LeavingClientActor actor that has disconnected
	 */
	//virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor);

	/**
	 * Called when this class is unregistered by the beacon host
	 * Do any necessary cleanup.
	 */
	
	//virtual void Unregister();
	

};
