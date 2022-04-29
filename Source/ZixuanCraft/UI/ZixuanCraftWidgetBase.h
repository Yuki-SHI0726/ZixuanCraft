// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftWidgetBase.h
// Author: Zixuan Shi
// Base widget interface for input

#pragma once
#include "GameplayComponents/LootSlot.h"
#include "Utils/TypeDefs.h"

#include "Blueprint/UserWidget.h"
#include "ZixuanCraftWidgetBase.generated.h"

class UZixuanCraftButton;

/**
 * Base class for UI widgets in game
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Interface
	virtual void IToggleInventoryCrafting() {}
	virtual void IOnJumpButtonPressed() {}
	virtual void IOnJumpButtonReleased() {}
	virtual void IOnDestoryAttackButtonPressed() {}
	virtual void IOnPlaceUseItemButtonPressed() {}
	virtual int32 IGetSelectIndex() const { return InvalidIndex; }
	virtual void UpdateHealthBarPercent(float Percent) {}
	virtual void ScrollInventory(bool bIsScrollingDown) {};
	virtual int32 GetGameplayInventoryNum() const { return InvalidIndex; }
	virtual int32 GetTotalInventoryNum() const { return InvalidIndex; }
	virtual void ResetItemAt(int32 Index) {};
	virtual void SetSelectIndex(int32 NeWIndex) {};
	virtual bool IsDisplayingInventoryPanel() const { return false; }
	virtual void SetSelectedItemPanel(const FLootSlot& InSlot) {};
	virtual void SetButtonDataAt(const FLootSlot& Data, int32 Index) {};
	virtual UZixuanCraftButton* GetButtonAt(int32 WidgetIndex) const { return nullptr; }
	virtual int32 ToBackpackIndex(int32 WidgetIndex) const { return InvalidIndex; }
	virtual int32 ToCraftingIndex(int32 WidgetIndex) const { return InvalidIndex; }
	virtual bool RightMouseButtonDown() const { return false; }
};
