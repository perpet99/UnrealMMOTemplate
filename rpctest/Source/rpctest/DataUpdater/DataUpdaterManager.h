// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "InventoryDataUpdater.h"

#include "DataUpdaterManager.generated.h"





class DBmanager
{
public:
	template <typename T>
	TArray<T> Select(FString query)
	{
		TArray<T> result;

		return result;
	}

	template <typename T>
	TArray<T> Load(FString query)
	{
		TArray<T> result;

		return result;
	}

	//template <typename T>
	//TArray<T> query(FString query)
	//{
	//	TArray<T> result;

	//	return result;
	//}


	void Update(FString query) {
		{

		}

	};


	void Delete(FString query) {
		{

		}
	};

};


USTRUCT(Blueprintable)
struct RPCTEST_API FItemInfo
{
	GENERATED_BODY()

		UPROPERTY() int UserID;
	UPROPERTY() int ItemID;
	UPROPERTY() int itemkey = 0;
	UPROPERTY() int ItemCOunt;
	UPROPERTY() int command; // {add,remove,update}
};

USTRUCT(Blueprintable)
struct RPCTEST_API FUpdateManagerDataInfo
{
	GENERATED_BODY()
public:
	TArray< FItemInfo> ItemList;

	TArray<int> UpdateGold;

	TArray<int> missionlist;
};

class GameUpdateManager
{
public:


	//class InventoryDataUpdater *idu;








	//

	class AMyPlayerController * memoryUser;

	FUpdateManagerDataInfo  umi;

	DBmanager db;

	static GameUpdateManager* I;

	static  GameUpdateManager* NewUpdateManager(class AMyPlayerController * user)
	{
		if (I == nullptr)
			I = new GameUpdateManager();

		I->memoryUser = user;
		return I;

	}
	
	void Init()
	{
		
	}


	void LoadDB(AMyPlayerController * user);


	FUpdateManagerDataInfo * ApplyServer();
	void ApplyMemory();
	void ApplyDB();


	static void ApplyClient(AMyPlayerController * user, const FUpdateManagerDataInfo & umi);

	///////////////////////////////////////////////////////////////////////////////


	void UpdateGold(int gold)
	{
		umi.UpdateGold.Add(gold);
	}


	void AddItem_Inventory_S(int itemid, int itemcount);
};

