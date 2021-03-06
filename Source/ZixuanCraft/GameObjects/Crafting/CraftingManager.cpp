// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Crafting/CraftingManager.h"
#include "UI/Gameplay/Widgets/ZixuanCraftWidgetBase.h"

#include "Kismet/GameplayStatics.h"

void ACraftingManager::BeginPlay()
{
	Super::BeginPlay();

	// Set up crafting data
	check(CraftingDataTable);
	for (const FName& Name : CraftingDataTable->GetRowNames())
	{
		FCraftingRules* DataRow = CraftingDataTable->FindRow<FCraftingRules>(Name, "");
		DataRow->Result.LootData.StaticMesh = NewObject<UStaticMesh>();
		GetStaticMeshFromType(DataRow->Result.LootData.Type);
		DataRow->Result.LootData.StaticMesh = OutStaticMeshToSet;
		CraftingMap.Emplace(DataRow->Input, DataRow->Result);
	}
}

void ACraftingManager::UpdateCraftingInput(int32 Index, EObjectType ObjectType)
{
	CurrentInput.Update(Index, ObjectType);
	
	if (FLootSlot* Result = CraftingMap.Find(CurrentInput))
	{
		Widget->SetButtonDataAt(*Result, CraftingResultButtonIndex);
	}
	else
	{
		Widget->ResetItemAt(CraftingResultButtonIndex);
	}
}

void ACraftingManager::Init(UZixuanCraftWidgetBase* InCraftingWidget, int32 InCraftingResultButtonIndex)
{
	Widget = InCraftingWidget;
	CraftingResultButtonIndex = InCraftingResultButtonIndex;
}