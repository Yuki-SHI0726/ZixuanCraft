// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameObjects/Loot/LootData.h"
#include "GameplayComponents/LootSlot.h"
#include "Utils/TypeDefs.h"

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Misc/Crc.h"
#include "CraftingManager.generated.h"

class UZixuanCraftWidgetBase;
class UDataTable;

/** Input source for crafting */
USTRUCT(BlueprintType)
struct FCraftingInput
{
	GENERATED_BODY()

	/** 
	 * Array contains input data. Always 9 elements in 3x3 matrix format 
	 * 0 1 2
	 * 3 4 5
	 * 6 7 8
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EObjectType> Input;

	FCraftingInput()
	{
		Input.SetNum(CraftingPanelSize);
	}

	void Update(int32 Index, EObjectType ObjectType)
	{
		Input[Index] = ObjectType;
	}

	bool operator==(const FCraftingInput& Other) const
	{
		return GetTypeHash(*this) == GetTypeHash(Other);
	}

	/** Hash function for CraftingInput */
	friend FORCEINLINE uint32 GetTypeHash(const FCraftingInput& Key)
	{
		uint32 Hash = 0;
		for (EObjectType ObjectType : Key.Input)
		{
			Hash = HashCombine(Hash, GetTypeHash(ObjectType));
		}
		return Hash;
	}
};

/**
* This table shows all the crafting rules. Will be used in data table
*/
USTRUCT(BlueprintType)
struct FCraftingRules : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCraftingInput Input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootSlot Result;

	FCraftingRules() {};
};

/**
 * Responsible for the crafting logic in gameplay
 */
UCLASS()
class ZIXUANCRAFT_API ACraftingManager : public AActor
{
	GENERATED_BODY()
	
private:
	/** Hash Map used for looking up crafting data. Key uses a matrix, Value as the result */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FCraftingInput, FLootSlot> CraftingMap;

	/** Hash Map used for looking up crafting data. Key uses a matrix, Value as the result */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* CraftingDataTable = nullptr;

	/** Pointer to the widget with crafting panel */
	UZixuanCraftWidgetBase* Widget = nullptr;

	int32 CraftingResultButtonIndex = InvalidIndex;

	/** The current crafting source input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FCraftingInput CurrentInput;

public:
	virtual void BeginPlay() override final;

	void UpdateCraftingInput(int32 Index, EObjectType ObjectType);
	void Init(UZixuanCraftWidgetBase* InCraftingWidget, int32 InCraftingResultButtonIndex);

protected:
	/**
	 * Blueprint event to set NewLoot's static mesh
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetStaticMeshFromType(EObjectType ObjectType, UStaticMeshComponent* StaticMeshComponent);
};
