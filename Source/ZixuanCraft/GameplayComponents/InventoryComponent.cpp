// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "GameObjects/Loot/Loot.h"
#include "Utils/TypeDefs.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super{ ObjectInitializer }
{
	Inventory.SetNum(MaxSize);
}

int32 UInventoryComponent::TryAdd(ALoot* Loot)
{
	// Don't add empty loot. This is preventing the bug that 
	// if the player digs straight down, the loot will overlap with the player first, then initialize the data, which will cause an UI widget bug that display empty loot
	if (Loot->GetLootData().Type == EObjectType::Empty)
	{
		return InvalidIndex;
	}

	// Find if there is a valid loot slot for this loot. Try to add this loot to that slot
	int32 Index = 0;
	for (FLootSlot& LootSlot : Inventory)
	{
		// If this slot is empty, it is available for a new loot, add the first loot to this slot
		if (LootSlot.Count == 0)
		{
			LootSlot.AddFirstLoot(Loot);
			return Index;
		}
		// If this slot is not empty, but we can append the new loot to this slot, do so
		if (LootSlot.TryAppend(Loot))
		{			
			return Index;
		}		
		++Index;
	}

	// At this point, we didn't find a existing valid loot slot to append the new loot
	// and ran out of space in this inventory, we can't add this loot
	return InvalidIndex;
}

bool UInventoryComponent::SubtractItem(int32 Index)
{
	if (Index < MaxSize)
	{
		--Inventory[Index].Count;
		if (Inventory[Index].Count <= 0)
		{
			Inventory[Index].Reset();
			return true;
		}
	}
	return false;
}

void UInventoryComponent::SwapLoot(int32 LeftIndex, int32 RightIndex)
{
	Inventory.Swap(LeftIndex, RightIndex);
}

void UInventoryComponent::SetLootAt(const FLootSlot& LootSlot, int32 Index)
{
	check(Index < MaxSize);
	Inventory[Index] = LootSlot;
}

void UInventoryComponent::ResetLootAt(int32 Index)
{
	check(Index < MaxSize);
	Inventory[Index].Reset();
}
