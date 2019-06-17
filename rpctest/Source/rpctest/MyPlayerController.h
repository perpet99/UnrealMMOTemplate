// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataUpdater/DataUpdaterManager.h"
#include "MyPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class RPCTEST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:





	void missionClear_CS1()
	{
		// 보상정보 계산

		FItemInfo item;

		InventoryItemList.Add(item);
		Gold -= 100;
		//updateDB(InventoryItemList);
		//updateDB(Gold);

		missionClear_SC1(item,100);
	}

	void missionClear_SC1(FItemInfo item,int gold)
	{
		//연출

		Gold -= gold;

		InventoryItemList.Add(item);
	}

	void buyshop_CS1()
	{
		// 
		FItemInfo item;


		InventoryItemList.Add(item);
		Gold -= 100;
		MissionList.Remove(1);

		//updateDB(InventoryItemList);
		//updateDB(Gold);


		buyshop_SC1(item, 100, 1);

	}

	void buyshop_SC1(FItemInfo item, int gold, int missionid)
	{
		//연출

		Gold -= gold;

		InventoryItemList.Add(item);

		MissionList.Remove(missionid);
	}








	TArray<int> MissionList;
	TArray<FItemInfo> InventoryItemList;

	int Gold;



	void cheat_cs(FName method = "AddItem_Inventory_S",Far param = "11,1")
	{
		auto um = GameUpdateManager::NewUpdateManager(this);
	
		/*auto func = um->FindFunction(method);
		if (func)*/
		{
			//func->
			//func->Invoke(um,)
		}
		//auto func = um->findMethod(method);

		//func->setarg();


		auto updateResult = um->ApplyServer();

		cheat_sc(*updateResult);

	}


	void cheat_sc(const FUpdateManagerDataInfo& result)
	{
		
		GameUpdateManager::ApplyClient(this, result);
	}


	void missionClear_CS2()
	{
		//미션 클리어 했냐?
		// 보상 정보는?

		
		auto um = GameUpdateManager::NewUpdateManager(this);
		um->AddItem_Inventory_S(11,1);
		um->UpdateGold(100);









		auto updateResult = um->ApplyServer();

		missionClear_SC(*updateResult);
	}

	void buyshop_CS2()
	{
		auto um = GameUpdateManager::NewUpdateManager(this);

		um->AddItem_Inventory_S(11, 1);
		um->UpdateGold(-100);

		auto updateResult = um->ApplyServer();
		buyshop_SC(*updateResult);
	}

	void buyshop_SC(const FUpdateManagerDataInfo & result)
	{
		// 보상 연출
		//Show(result)

		GameUpdateManager::ApplyClient(this, result);

	}


	void missionClear_SC(const FUpdateManagerDataInfo & result)
	{

		// 보상 연출
		//Show(result)

		GameUpdateManager::ApplyClient(this, result);
	}


};
