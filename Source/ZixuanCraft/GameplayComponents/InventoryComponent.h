// Fill out your copyright notice in the Description page of Project Settings.
// InventoryComponent.h
// Author: Zixuan Shi
// Component for inventory system

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootSlot.h"
#include "InventoryComponent.generated.h"

class ALoot;

/** 
 * Actor Component for Inventory
 */
UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZIXUANCRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Total maximum slots for an inventory */
	static constexpr int32 MaxSize = 36;

private:
	/** Buffer holding all the loots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FLootSlot> Inventory;

public:	
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);

	/**
	 * Try to add a new loot into inventory
	 * @param Loot		The loot trying to add
	 * @return			Index of the added loot in Inventory, if couldn't add, return TNumericLimits<int32>::Max()
	 */
	int32 TryAdd(ALoot* Loot);

	/**
	 * Subtract count in the inventory, remove the loot if count <= 0
	 * @param Index		The index in the inventory to remove item
	 * @return			true If the item is removed because the count is <= 0, false if it's still here
	 */
	bool SubtractItem(int32 Index);

	/**
	 * Swap to loots' indices in the array
	 * @param LeftIndex		First loot's index
	 * @param RightIndex	Second loot's index
	 */
	void SwapLoot(int32 LeftIndex, int32 RightIndex);

	/**
	 * Force adding a loot slot at index. Note this will overwrite what's in the inventory at Index already
	 * @param LootSlot		New loot slot we are trying to add
	 * @param Index			Index at Inventory to add
	 */
	void SetLootAt(const FLootSlot& LootSlot, int32 Index);

	/**
	 * Reset the loot at index position. This won't shrink the array, just remove the element at that place
	 * @param Index		The index to remove loot
	 */
	void ResetLootAt(int32 Index);

	const TArray<FLootSlot>& GetInventory() const { return Inventory; }
	const FLootSlot& GetData(int32 Index) const { return Inventory[Index]; }
};
