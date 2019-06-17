// Fill out your copyright notice in the Description page of Project Settings.

#include "DataUpdaterManager.h"
#include "../MyPlayerController.h"

GameUpdateManager* GameUpdateManager::I = nullptr;



void GameUpdateManager::AddItem_Inventory_S(int itemid, int itemcount)
{
	FItemInfo fi;
	fi.ItemID = itemid;
	fi.ItemCOunt = itemcount;
	//fi.itemkey = InventoryItemList.select(itemid == itemid).count;
	// 여기서 add 할지 update 할지 판단

	// 아이템 스택어블 하냐 안하냐 판단
	//if (memoryUser->InventoryItemList.Find(fi))
	if( 0)
	{
		fi.command = 0;
	}
	else
	{
		fi.command = 1;
	}


	umi.ItemList.Add(fi);

	//umi.AddItemList.Add(fi);



}

void GameUpdateManager::LoadDB(AMyPlayerController * user)
{
	//auto result = db.Select< FItemInfo>("select * from inventory where id = 1");
	auto result = db.Load< FItemInfo>("1");

		

	for ( auto elem : result)
	{
		user->InventoryItemList.Add(elem);
	}
}

FUpdateManagerDataInfo * GameUpdateManager::ApplyServer()
{
	

	ApplyMemory();

	ApplyDB();

	//ApplyLog();
	return &umi;


}

void GameUpdateManager::ApplyMemory()
{
	for (auto elem : umi.ItemList)
	{
		switch (elem.command)
		{
		case 0:
			// 기존아이템 찾고
			// 기획상에 스택어블 하면 

			memoryUser->InventoryItemList.Add(elem);

			break;
		case 1:
			break;


		}
		
	}

}

void GameUpdateManager::ApplyDB()
{

	for (auto elem : umi.ItemList)
	{
		switch (elem.command)
		{
			case 0:
				break;
		default:
			db.Update("update item");
			break;
		}
		
	}

}


void GameUpdateManager::ApplyClient(AMyPlayerController * user, const FUpdateManagerDataInfo & umi)
{

	for (auto elem : umi.ItemList)
	{
		switch (elem.command)
		{
		case 0:
			user->InventoryItemList.Add(elem);
			break;

			
		}
		
	}



}